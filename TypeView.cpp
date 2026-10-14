#include "pch.h"

#include "MainFrm.h"
#include "TypeView.h"
#include "Resource.h"
#include "C4IMap.h"
#include "C4IMapDoc.h"
#include "C4IMapView.h"


class CTypeViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CTypeView;

	DECLARE_SERIAL(CTypeViewMenuButton)

public:
	CTypeViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CTypeViewMenuButton, CMFCToolBarMenuButton, 1)


//////////////////////////////////////////////////////////////////////
// 생성/소멸
//////////////////////////////////////////////////////////////////////

CTypeView::CTypeView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE_COP;
	//m_pMsgBaseList = NULL;
	m_nGrpFstItemNum = 0;
}

CTypeView::~CTypeView()
{
}

BEGIN_MESSAGE_MAP(CTypeView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COP_ADD_MEMBER_FUNCTION, OnCOPAddMemberFunction)
	ON_COMMAND(ID_COP_ADD_MEMBER_VARIABLE, OnCOPAddMemberVariable)
	ON_COMMAND(ID_COP_DEFINITION, OnCOPDefinition)
	ON_COMMAND(ID_COP_PROPERTIES, OnCOPProperties)
	ON_COMMAND(ID_NEW_FOLDER_COP, OnNewFolder_COP)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE_COP, ID_SORTING_SORTBYACCESS_COP, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE_COP, ID_SORTING_SORTBYACCESS_COP, OnUpdateSort)

	ON_NOTIFY(NM_CLICK, IDC_COP_TREECTRL_TYPE, &CTypeView::OnNMClickTreeDrawType)
	ON_NOTIFY(NM_DBLCLK, IDC_COP_TREECTRL_TYPE, &CTypeView::OnNMDblclkTreeDrawType)
	ON_NOTIFY(TVN_SELCHANGED, IDC_COP_TREECTRL_TYPE, &CTypeView::OnTvnSelchangedTreeDrawType)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_COP_TREECTRL_TYPE, &CTypeView::OnTvnItemexpandedTreeDrawType)
	ON_NOTIFY(TVN_ITEMCHANGED, IDC_COP_TREECTRL_TYPE, &CTypeView::OnTvnItemChangedTreeDrawType)
	ON_NOTIFY(NM_RCLICK, IDC_COP_TREECTRL_TYPE, &CTypeView::OnNMRClickTreeDrawType)
	ON_NOTIFY(NM_TVSTATEIMAGECHANGING, IDC_COP_TREECTRL_TYPE, &CTypeView::OnNMTVStateImageChangingTreeDrawType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 메시지 처리기

int CTypeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_CHECKBOXES;
	//const DWORD dwViewStyle = TVS_HASBUTTONS | TVS_HASLINES | TVS_CHECKBOXES | TVS_INFOTIP | WS_BORDER | WS_HSCROLL | WS_TABSTOP;

	if (!m_wndCOPView.Create(dwViewStyle, rectDummy, this, IDC_COP_TREECTRL_TYPE))
	{
		TRACE0("유형 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 이미지를 로드합니다.
	m_wndCOPToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT_COP);
	m_wndCOPToolBar.LoadToolBar(IDR_SORT_COP, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndCOPToolBar.SetPaneStyle(m_wndCOPToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndCOPToolBar.SetPaneStyle(m_wndCOPToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndCOPToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndCOPToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT_COP);

	m_wndCOPToolBar.ReplaceButton(ID_SORT_MENU_COP, CTypeViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CTypeViewMenuButton* pButton = DYNAMIC_DOWNCAST(CTypeViewMenuButton, m_wndCOPToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
	FillClassView();

#ifdef _DEBUG
	TRACE("[CREATE VIEW] CTypeView OnCreate\n");
#endif // _DEBUG

	return 0;
}

void CTypeView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CTypeView::FillClassView()
{
	HTREEITEM hRoot = m_wndCOPView.InsertItem(_T("도시요소 유형"), 0, 0);
	m_wndCOPView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);


}

void CTypeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndCOPView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_SORT_COP))
	{
		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	}
}

void CTypeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndCOPToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndCOPToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndCOPView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

BOOL CTypeView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CTypeView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CTypeViewMenuButton* pButton = DYNAMIC_DOWNCAST(CTypeViewMenuButton, m_wndCOPToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndCOPToolBar.Invalidate();
		m_wndCOPToolBar.UpdateWindow();
	}
}

void CTypeView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CTypeView::OnCOPAddMemberFunction()
{
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void CTypeView::OnCOPAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CTypeView::OnCOPDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CTypeView::OnCOPProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CTypeView::OnNewFolder_COP()
{
	AfxMessageBox(_T("새 폴더..."));
}

void CTypeView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndCOPView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CTypeView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndCOPView.SetFocus();
}

void CTypeView::OnChangeVisualStyle()
{
	m_COPViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_COP_VIEW_24 : IDB_COP_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_COPViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_COPViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndCOPView.SetImageList(&m_COPViewImages, TVSIL_NORMAL);

	m_wndCOPToolBar.CleanUpLockedImages();
	m_wndCOPToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_COP_24 : IDR_SORT_COP, 0, 0, TRUE /* 잠금 */);
}

CTreeCtrl* CTypeView::GetView()
{
	return &m_wndCOPView;
}

//void CTypeView::SetMsgMgr(CMapXMsgProcBase_List* pMsgBaseList)
//{
//	m_pMsgBaseList = pMsgBaseList;
//}

void CTypeView::OnNMClickTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

#ifdef _DEBUG
	TRACE("OnNMClickTreeCopdb \n");
#endif // _DEBUG

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CPoint pt, screenPt;
	::GetCursorPos(&pt);
	screenPt = pt;
	CTreeCtrl* pctrl = &m_wndCOPView;
	pctrl->ScreenToClient(&pt);
	UINT nFlags = 0;

	//현재 커서가 위치한 곳의 Tree
	HTREEITEM hSelectItem = pctrl->HitTest(CPoint(pt), &nFlags);

	if (hSelectItem == NULL)
	{
#ifdef _DEBUG
		TRACE("OnNMClickTreeCopdb hit Test error [%d,%d][%d]\n", pt.x, pt.y, nFlags);
#endif // _DEBUG
		* pResult = 0;
		return;
	}
	CString str = pctrl->GetItemText(hSelectItem);
	CString* pNodeKey = (CString*)pctrl->GetItemData(hSelectItem);

	if (nFlags == 8 || nFlags == 16 || str.IsEmpty())
	{
#ifdef _DEBUG
		TRACE("OnNMClickTreeCopdb Flag [%d]\n", nFlags);
#endif // _DEBUG
		* pResult = 0;
		return;
	}
#ifdef _DEBUG
	TRACE("OnNMClickTreeCopdb hit Test Ok [%d,%d][%d]\n", pt.x, pt.y, nFlags);
#endif // _DEBUG

	if (hSelectItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		if (pctrl->GetCheck(hSelectItem))
			UnCheckChildItem(hSelectItem);
		else
			CheckChildItem(hSelectItem);


	}

	pctrl->SetItemState(hSelectItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);



	if (!pctrl->GetCheck(hSelectItem)) {    // 체크박스에 체크전이라 반대로 체크	  
		// 상위 체크 업데이트
		HTREEITEM hParentItem = pctrl->GetParentItem(hSelectItem);
		while (hParentItem != NULL)
		{
			//m_treeCtrl_CopDB.SetCheck(hParentItem, TRUE); 
			hParentItem = m_wndCOPView.GetParentItem(hParentItem);
		}
	}
	else
	{
		//uncheck 상태일때 자식트리에 체크 없을때 부모트리두 uncheck  
	}

	HCURSOR oldCursor = ::GetCursor();
	CString strPath;
	//strPath.Format("%s\\..\\%s", IMX_ENGIN_PATH, IMX3D_ANI_CUR_WATING);
	HCURSOR hcurAni = LoadCursorFromFile(strPath);
	if (hcurAni)
		::SetCursor(hcurAni);
	else
		BeginWaitCursor();

#ifdef _DEBUG
	CTime t = CTime::GetCurrentTime();
	TRACE("OnNMClickTreeCopdb start [%s]\n", t.Format(_T("%Y%m%d%H%M%S")));
	CString strTime;
	DWORD startTime, endTime;
	startTime = ::GetTickCount();
#endif // _DEBUG

	//if (m_pMsgBaseList)
	//{
	//	m_pMsgBaseList->windowProc(IER_TREE_LCLICK, (LPARAM)pNMHDR, (WPARAM)pResult);
	//	MoveListgrp(pNodeKey);
	//}

#ifdef _DEBUG         
	endTime = ::GetTickCount();
	t = CTime::GetCurrentTime();
	TRACE("====> OnNMClickTreeCopdb processing Time [%s][%d ms]\n", t.Format(_T("%Y%m%d%H%M%S")), endTime - startTime);
	startTime = ::GetTickCount();
#endif // _DEBUG

	::SetCursor(oldCursor);
	if (hcurAni)
		DestroyCursor(hcurAni);
	else
		EndWaitCursor();

	*pResult = 0;
}
void CTypeView::OnTvnItemexpandedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
#ifdef _DEBUG
	TRACE("OnTvnItemexpandedTreeCopdb \n");
#endif // _DEBUG

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
#ifdef _DEBUG
	TRACE("expand Action %d\n", pNMTreeView->action);
#endif // _DEBUG
	* pResult = 0;
}
void CTypeView::OnNMDblclkTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _DEBUG
	TRACE("OnNMDblclkTreeCopdb \n");
#endif // _DEBUG

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
}

void CTypeView::OnTvnSelchangedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	UINT unAction = pNMTreeView->action;

//#ifdef _DEBUG
//	TRACE("OnTvnSelchangedTreeCopdb %d\n", unAction);
//#endif // _DEBUG

	if (unAction == 0)
		return;

	//if (m_pMsgBaseList)
	//{
	//	m_pMsgBaseList->windowProc(IER_TREE_SELCHANDED, (LPARAM)pNMHDR, (WPARAM)pResult);
	//}
	*pResult = 0;
}



void CTypeView::OnTvnItemChangedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVITEMCHANGE* pNMTVItemChange = reinterpret_cast<NMTVITEMCHANGE*>(pNMHDR);


#ifdef _DEBUG
	TRACE("OnTvnItemChangedTreeCopdb \n");
#endif // _DEBUG

	* pResult = 0;
}

void CTypeView::OnNMRClickTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _DEBUG
	TRACE("OnNMRClickTreeCopdb \n");
#endif // _DEBUG

	//if (m_pMsgBaseList)
	//	m_pMsgBaseList->windowProc(IER_TREE_RCLICK, (LPARAM)pNMHDR, (WPARAM)pResult);
	*pResult = 0;
}

void CTypeView::OnNMTVStateImageChangingTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult)
{
#ifdef _DEBUG
	TRACE("OnNMTVStateImageChangingTreeCopdb \n");
#endif // _DEBUG

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	LPNMTREEVIEW pLPNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	UINT nOldState = pNMTreeView->itemOld.state;
	UINT nNewState = pNMTreeView->itemNew.state;
	UINT unAction = pNMTreeView->action;

	CTreeCtrl* pctrl = &m_wndCOPView;
	CPoint pt, screenPt;
	::GetCursorPos(&pt);
	screenPt = pt;
	pctrl->ScreenToClient(&pt);
	UINT nFlags = 0;
	HTREEITEM hItem = pctrl->HitTest(CPoint(pt), &nFlags);

	TRACE("TVStateImageChanging old [%d] new [%d] Action[%d] [%d]\n", nOldState, nNewState, unAction, nFlags & TVHT_ONITEMSTATEICON);

	*pResult = 0;
}

void CTypeView::MoveListgrp(CString* pStrGrpKey)
{
/*
	if (m_pMsgBaseList && pStrGrpKey)
	{
		LVFINDINFO info;
		info.flags = LVFI_PARTIAL | LVFI_STRING;
		info.psz = (LPSTR)(LPCTSTR)m_pMsgBaseList->getListGrpFstItemStr(*pStrGrpKey);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame)
		{
			CListCtrl* pListCtrl = (CListCtrl*)pFrame->m_wndCOPItemView.GetView();
			int nGrpFstItemNum = pListCtrl->FindItem(&info);

			if (nGrpFstItemNum == 0)
			{
				if (m_pMsgBaseList->getListGrpFstItemID(*pStrGrpKey) == -1)
					return;
			}

			if (nGrpFstItemNum != -1)
			{
				int nlistCount = pListCtrl->GetItemCount();
				int nloop = 0;
				BOOL bret = pListCtrl->EnsureVisible(nGrpFstItemNum, TRUE);
				while (!bret)
				{
					bret = pListCtrl->EnsureVisible(nGrpFstItemNum + (++nloop), TRUE);

					if (nlistCount < nGrpFstItemNum + nloop)
						break;
					nloop++;
				}
				m_nGrpFstItemNum = nGrpFstItemNum + nloop;
			}
		}
	}
*/
}

//하위 아이템 언체크
void CTypeView::UnCheckChildItem(HTREEITEM hItem)
{
	CTreeCtrl* pctrl = &m_wndCOPView;
	HTREEITEM hChildItem = pctrl->GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		pctrl->SetCheck(hChildItem, FALSE);

		if (pctrl->ItemHasChildren(hChildItem))
			UnCheckChildItem(hChildItem);

		hChildItem = pctrl->GetNextItem(hChildItem, TVGN_NEXT);
	}
}


//하위 아이템 체크
void CTypeView::CheckChildItem(HTREEITEM hItem)
{
	CTreeCtrl* pctrl = &m_wndCOPView;
	HTREEITEM hChildItem = pctrl->GetChildItem(hItem);
	while (hChildItem != NULL)
	{
		pctrl->SetCheck(hChildItem, TRUE);

		if (pctrl->ItemHasChildren(hChildItem))
			CheckChildItem(hChildItem);

		hChildItem = pctrl->GetNextSiblingItem(hChildItem);
	}


}
