#include "pch.h"

#include "MainFrm.h"
#include "ItemView.h"
#include "Resource.h"
#include "C4IMap.h"
#include "C4IMapDoc.h"
#include "C4IMapView.h"



class CItemViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CItemView;

	DECLARE_SERIAL(CItemViewMenuButton)

public:
	CItemViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
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

IMPLEMENT_SERIAL(CItemViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 생성/소멸
//////////////////////////////////////////////////////////////////////

CItemView::CItemView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE_COP;
	m_nDrawItemListClickNum = -1;
	m_nlistCtrl_DrawItem_Rs_cursor = -1;
	//m_pMsgBaseList = NULL;
}

CItemView::~CItemView()
{
}

BEGIN_MESSAGE_MAP(CItemView, CDockablePane)
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

	ON_NOTIFY(NM_CLICK, IDC_COP_LISTCTRL_ITEM, &CItemView::OnNMClickListDrawitem)
	ON_NOTIFY(NM_DBLCLK, IDC_COP_LISTCTRL_ITEM, &CItemView::OnNMDblclkListDrawitem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_COP_LISTCTRL_ITEM, &CItemView::OnLvnItemchangedListDrawitem)
	ON_NOTIFY(NM_RCLICK, IDC_COP_LISTCTRL_ITEM, &CItemView::OnNMRClickListDrawitem)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COP_LISTCTRL_ITEM, &CItemView::OnNMCustomdrawListDrawitem)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_COP_LISTCTRL_ITEM, &CItemView::OnLvnGetdispinfoOwnerDrawitem)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 메시지 처리기

int CItemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_CHECKBOXES | TVS_EX_MULTISELECT;

	if (!m_wndView.Create(dwViewStyle, rectDummy, this, IDC_COP_LISTCTRL_ITEM))
	{
		TRACE0("항목 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	DWORD dwExStyle = m_wndView.GetExtendedStyle();
	m_wndView.SetExtendedStyle(dwExStyle | LVS_EX_CHECKBOXES | LVS_EX_BORDERSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | TVS_EX_MULTISELECT | LVS_EX_AUTOSIZECOLUMNS | LVS_OWNERDATA);
	//m_wndView.InsertColumn(IER_DRAWITEM_CHECK_LISTCOL, IER_DRAWITEM_CHECKALL, LVCFMT_LEFT, 25);
	//m_wndView.InsertColumn(IER_DRAWITEM_KEY_LISTCOL, IER_DRAWITEM_KEY, LVCFMT_LEFT, 162);
	//m_wndView.InsertColumn(IER_DRAWITEM_COORD_LISTCOL, IER_DRAWITEM_COORD, LVCFMT_CENTER, 48);
	//m_wndView.InsertColumn(IER_DRAWITEM_IMPORTENT_LISTCOL, _T("중요"), LVCFMT_CENTER, 36);
	//m_wndView.InsertColumn(IER_DRAWITEM_ALWAYS_LISTCOL, _T("상시"), LVCFMT_CENTER, 36);
	//m_wndView.EnableGroupView(TRUE);
	m_wndView.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	// Owner 
	ListView_SetItemCount(m_wndView, 100000);

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

	m_wndCOPToolBar.ReplaceButton(ID_SORT_MENU_COP, CItemViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CItemViewMenuButton* pButton = DYNAMIC_DOWNCAST(CItemViewMenuButton, m_wndCOPToolBar.GetButton(0));

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
	TRACE("[CREATE VIEW] CItemView OnCreate\n");
#endif // _DEBUG
	return 0;
}

void CItemView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CItemView::FillClassView()
{

}

void CItemView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CListCtrl* pWndList = (CListCtrl*)&m_wndView;
	ASSERT_VALID(pWndList);

	if (pWnd != pWndList)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptList = point;
		pWndList->ScreenToClient(&ptList);

		UINT flags = 0;
		int nHit = pWndList->HitTest(ptList, &flags);
		if (nHit != -1)
		{
			pWndList->SetHotItem(nHit);
		}
	}

	pWndList->SetFocus();
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

void CItemView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndCOPToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndCOPToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);

}

BOOL CItemView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CItemView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CItemViewMenuButton* pButton = DYNAMIC_DOWNCAST(CItemViewMenuButton, m_wndCOPToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndCOPToolBar.Invalidate();
		m_wndCOPToolBar.UpdateWindow();
	}
}

void CItemView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CItemView::OnCOPAddMemberFunction()
{
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void CItemView::OnCOPAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CItemView::OnCOPDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CItemView::OnCOPProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CItemView::OnNewFolder_COP()
{
	AfxMessageBox(_T("새 폴더..."));
}

void CItemView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectList;
	m_wndView.GetWindowRect(rectList);
	ScreenToClient(rectList);

	rectList.InflateRect(1, 1);
	dc.Draw3dRect(rectList, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CItemView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

void CItemView::OnChangeVisualStyle()
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

	m_wndView.SetImageList(&m_COPViewImages, TVSIL_NORMAL);

	m_wndCOPToolBar.CleanUpLockedImages();
	m_wndCOPToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_COP_24 : IDR_SORT_COP, 0, 0, TRUE /* 잠금 */);
}

CListCtrl* CItemView::GetView()
{
	return &m_wndView;
}

//void CItemView::SetMsgMgr(CMapXMsgProcBase_List* pMsgBaseList)
//{
//	m_pMsgBaseList = pMsgBaseList;
//}


void CItemView::OnNMClickListDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (m_pMsgBaseList)
	//{
	//	m_pMsgBaseList->windowProc(IER_LIST_LCLICK, (LPARAM)pNMHDR, (WPARAM)pResult);
	//	m_pMsgBaseList->copDrawItemFocusContinue(-1, FALSE);
	//	m_pMsgBaseList->copDrawItemKeySearchContinueInListCtrl();
	//}
	m_nDrawItemListClickNum = pNMItemActivate->iItem;
	m_wndView.SetItemState(pNMItemActivate->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}

void CItemView::OnNMDblclkListDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (m_pMsgBaseList)
	//{
	//	m_pMsgBaseList->windowProc(IER_LIST_LDCLICK, (LPARAM)pNMHDR, (WPARAM)pResult);

	//	//도시요소선택시 화면이동에 따른 인덱스맵갱신(14-04-03) 
	//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//	if (pFrame)
	//	{
	//		CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
	//		if (pView)
	//		{
	//			pView->SendMessage(WM_USERLAYERLIST_UPDATEINDEX);
	//		}
	//	}
	//}
	msgBaseListPatrol(FALSE);
	ResetDrawItemFind();
	m_nlistCtrl_DrawItem_Rs_cursor = -1;
	m_nDrawItemListClickNum = -1;

	*pResult = 0;
}

// 리스트 컨트롤 Insert 속도 빠르게.. 리스트컨트롤 속성 : owner True  로 변경
void CItemView::OnLvnGetdispinfoOwnerDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.   

	// 현재는 그냥 리턴
	*pResult = 0;
	return;

/*
	if (pDispInfo->item.mask & LVIF_TEXT)
	{

		if (m_pMsgBaseList)
			m_pMsgBaseList->CopDrawItemOwnerSync(pDispInfo);

#ifdef _DEBUG
		TRACE("Owner insert [%d][%d][%d][%s]\n", pDispInfo->item.iItem, pDispInfo->item.iSubItem, (int)pDispInfo->item.lParam, pDispInfo->item.pszText);
#endif // _DEBUG
	}
	else if (pDispInfo->item.mask & LVIF_IMAGE)
	{

		if (m_pMsgBaseList)
			m_pMsgBaseList->CopDrawItemOwnerSync(pDispInfo);
	}
*/
	/*else if(pDispInfo->item.mask & LVIF_GROUPID)
	{
		pDispInfo->item.mask |= LVIF_GROUPID;
		pDispInfo->item.stateMask = LVIF_GROUPID;
		if(m_pMsgBaseList)
			m_pMsgBaseList->CopDrawItemOwnerSync(pDispInfo);
	}*/


	*pResult = 0;
}

void CItemView::OnLvnItemchangedListDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
/*
	if (m_pMsgBaseList)
	{
		m_pMsgBaseList->windowProc(IER_LIST_SELCHANDED, (LPARAM)pNMHDR, (WPARAM)pResult);


		UINT nOldCheck = pNMLV->uOldState & LVIS_STATEIMAGEMASK;
		UINT nNewCheck = pNMLV->uNewState & LVIS_STATEIMAGEMASK;

		// check 버튼 해제상태 확인
		if (nOldCheck && nNewCheck && nOldCheck != nNewCheck)
		{
			//m_pParent->SendMessage(WM_3DMAP_UPDATE, 0, 0);
		}
	}
*/
	*pResult = 0;
}


void CItemView::OnNMRClickListDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다. 
	//if (m_pMsgBaseList)
	//	m_pMsgBaseList->windowProc(IER_LIST_RCLICK, (LPARAM)pNMHDR, (WPARAM)pResult);

	//m_nDrawItemListClickNum = -1;

	*pResult = 0;
}
void CItemView::OnNMCustomdrawListDrawitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;

	//if (m_pMsgBaseList)
	//	m_pMsgBaseList->windowProc(IER_LIST_CUSTOMDRAW, (LPARAM)(NMHDR*)pNMHDR, (WPARAM)(LRESULT*)pResult);
}

void CItemView::msgBaseListPatrol(BOOL bPatrol)
{
/*
	if (m_pMsgBaseList)
	{
		CString strButtonNm;
		//GetDlgItemText(IDC_BTN_DRAWITEM_IMPO_PATRAL, strButtonNm); 
		if (bPatrol)
		{
			if (strButtonNm.CompareNoCase(_T("중요보기")) == 0)
				if (m_pMsgBaseList->copDrawItemImportentPatrol(TRUE))
				{
					//SetDlgItemText(IDC_BTN_DRAWITEM_IMPO_PATRAL, _T("순회중"));
				}
		}
		else
		{
			if (strButtonNm.CompareNoCase(_T("순회중")) == 0)
				if (!m_pMsgBaseList->copDrawItemImportentPatrol(FALSE))
				{
					//SetDlgItemText(IDC_BTN_DRAWITEM_IMPO_PATRAL, _T("중요보기"));
				}
		}
	}
*/
}

void CItemView::ResetDrawItemFind()
{
	//SetDlgItemText(IDC_EDIT_DRAWITEM, _T(""));
	//RefreshSearchCount(0);
	if (m_nlistCtrl_DrawItem_Rs_cursor != -1)
	{
		m_wndView.SetItemState(-1, 0, LVIS_FOCUSED | LVIS_SELECTED);
		m_nlistCtrl_DrawItem_Rs_cursor = -1;
	}
}