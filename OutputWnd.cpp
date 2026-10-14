// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해
// 추가적으로 제공되는 내용입니다.
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "pch.h"
#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include <algorithm> 

#include "../inc/define_struct.h"



#define DEF_OUTPUTLIST_LOG			0
#define DEF_OUTPUTLIST_SEARCH		1
#define DEF_OUTPUTLIST_BUILD		2
#define DEF_OUTPUTLIST_DEBUG		3
#define DEF_OUTPUTLIST_FIND			4
#define DEF_OUTPUTTAB				5

#define DEF_OUTPUTLIST_EXT			100


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
	for (std::map<CString, COutputSearchList*>::iterator iter = m_mpwndOutputSearchResultEx.begin();
		iter != m_mpwndOutputSearchResultEx.end();
		iter++)
	{
		delete (*iter).second;
	}
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 탭 창을 만듭니다.
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, DEF_OUTPUTTAB))
	{
		TRACE0("출력 탭 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	const DWORD dwListBoxStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	const DWORD dwListCtrlStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputLog.Create(dwListBoxStyle, rectDummy, &m_wndTabs, DEF_OUTPUTLIST_LOG))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndOutputLog.SetFont(&m_Font);

	// 출력 창을 만듭니다.
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	if (!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, DEF_OUTPUTLIST_DEBUG))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndOutputDebug.SetFont(&m_Font);

	CString strTabName;
	BOOL bNameValid;

	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputDebug, strTabName, (UINT)DEF_OUTPUTLIST_DEBUG);

	m_nExtTabID = m_wndTabs.GetTabsNum() + 10;

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab 컨트롤은 전체 클라이언트 영역을 처리해야 합니다.
	m_wndTabs.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_Font);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = std::max<int>(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	/*m_wndOutputBuild.AddString(_T("여기에 빌드 출력이 표시됩니다."));
	m_wndOutputBuild.AddString(_T("출력이 목록 뷰 행에 표시되지만"));
	m_wndOutputBuild.AddString(_T("표시 방법을 원하는 대로 변경할 수 있습니다."));*/
}

void COutputWnd::FillDebugWindow()
{
#ifdef _DEBUG
	m_wndOutputDebug.AddString(_T("여기에 디버그 출력이 표시됩니다."));
	m_wndOutputDebug.AddString(_T("출력이 목록 뷰 행에 표시되지만"));
	m_wndOutputDebug.AddString(_T("표시 방법을 원하는 대로 변경할 수 있습니다."));
#endif // _DEBUG
}

void COutputWnd::FillFindWindow()
{
	/*m_wndOutputFind.AddString(_T("여기에 찾기 출력이 표시됩니다."));
	m_wndOutputFind.AddString(_T("출력이 목록 뷰 행에 표시되지만"));
	m_wndOutputFind.AddString(_T("표시 방법을 원하는 대로 변경할 수 있습니다."));*/
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 메시지 처리기

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	if (menu.LoadMenu(IDR_OUTPUT_POPUP))
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

		SetFocus();
	}
}

void COutputList::OnEditCopy()
{
	if (OpenClipboard())
	{
		EmptyClipboard();

		size_t ncount = GetCount();
		CString strMsg;
		for (size_t nidx = 0; nidx < ncount; nidx++)
		{
			if (0 != nidx)
				strMsg += _T("\r\n");

			CString strBuf;
			GetText(nidx, strBuf);
			strMsg.Append(strBuf);
		}
		int nLen = strMsg.GetLength() * sizeof(WCHAR) + 1;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
		LPBYTE pGlobalData = (LPBYTE)GlobalLock(hGlobal);

		USES_CONVERSION_EX;
		CopyMemory(pGlobalData, T2CW_EX(strMsg, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
		SetClipboardData(CF_UNICODETEXT, hGlobal);

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);

		CloseClipboard();
	}
}

void COutputList::OnEditClear()
{
	ResetContent();
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}


void COutputWnd::AddBuildWindow(CString& strMsg)
{
	/*int nret = m_wndOutputBuild.AddString(strMsg);
	m_wndOutputBuild.SetCurSel(nret);*/
}

void COutputWnd::AddDebugWindow(CString& strMsg)
{
	CString strBuf;

	CTime t = CTime::GetCurrentTime();
	strBuf.Format("[%s]%s", t.Format(_T("%Y%m%d%H%M%S")), strMsg);
	int nret = m_wndOutputDebug.AddString(strBuf);
	m_wndOutputDebug.SetCurSel(nret);
}

void COutputWnd::AddFindWindow(CString& strMsg)
{
	/*int nret = m_wndOutputFind.AddString(strMsg);
	m_wndOutputFind.SetCurSel(nret);*/
}

void COutputWnd::ClearBuildWindow()
{
	//m_wndOutputBuild.ResetContent();
}

void COutputWnd::ClearDebugWindow()
{
	//#ifdef _DEBUG
	m_wndOutputDebug.ResetContent();
	//#endif // _DEBUG
}

void COutputWnd::ClearFindWindow()
{
	//m_wndOutputFind.ResetContent();
}
void COutputWnd::SetActiveTab(CString strLabel)
{
	if (strLabel.IsEmpty())
		m_wndTabs.SetActiveTab(0);
	int nTab = m_wndTabs.GetTabsNum();
	CString strTabLable;
	for (int nIdx = 0; nIdx < nTab; nIdx++)
	{
		if (m_wndTabs.GetTabLabel(nIdx, strTabLable))
		{
			if (strTabLable.CompareNoCase(strLabel) == 0)
			{
				m_wndTabs.SetActiveTab(nIdx);
				break;
			}
		}
	}
}

void COutputWnd::ClearSearch()
{
	for (std::map<CString, COutputSearchList*>::iterator iter = m_mpwndOutputSearchResultEx.begin();
		iter != m_mpwndOutputSearchResultEx.end();
		iter++)
		(*iter).second->ClearSearch();
}

void COutputWnd::SetSearchScreen(CString strCategory, CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult)
{
	//if(vResult.size())
	{
		ClearSearchList();
		CString strTmp;
		strTmp.Format("%s-%s", strCategory, strLayer);
		COutputSearchList* pwndOutputSearchResultEx = GetSearchList(strTmp);

		if (pwndOutputSearchResultEx)
		{
			BOOL bExists = FALSE;
			int nTab = m_wndTabs.GetTabsNum();
			CString strTabLable;
			for (int nIdx = 0; nIdx < nTab; nIdx++)
			{
				if (m_wndTabs.GetTabLabel(nIdx, strTabLable))
				{
					if (strTabLable.CompareNoCase(strTmp) == 0)
						bExists = TRUE;
				}
			}
			if (!bExists)
				m_wndTabs.AddTab(pwndOutputSearchResultEx, strTmp, GetSearchListID(strTmp));

			pwndOutputSearchResultEx->SetSearchScreen(strCategory, strLayer, vResult);
		}
	}
}

void COutputWnd::SetSearchName(CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult)
{
	ClearSearchList();
	CString strTmp;
	strTmp.Format("%s", strLayer);
	COutputSearchList* pwndOutputSearchResultEx = GetSearchList(strTmp);

	if (pwndOutputSearchResultEx)
	{
		BOOL bExists = FALSE;
		int nTab = m_wndTabs.GetTabsNum();
		CString strTabLable;
		for (int nIdx = 0; nIdx < nTab; nIdx++)
		{
			if (m_wndTabs.GetTabLabel(nIdx, strTabLable))
			{
				if (strTabLable.CompareNoCase(strTmp) == 0)
					bExists = TRUE;
			}
		}
		if (!bExists)
			m_wndTabs.AddTab(pwndOutputSearchResultEx, strTmp, GetSearchListID(strTmp));

		pwndOutputSearchResultEx->SetSearchName(strLayer, vResult);
	}
}

COutputSearchList* COutputWnd::NewSearchList(CString strLabel)
{
	COutputSearchList* pwndOutputSearchResultEx = new COutputSearchList;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwListCtrlStyle = LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!pwndOutputSearchResultEx->Create(dwListCtrlStyle, rectDummy, &m_wndTabs, DEF_OUTPUTLIST_SEARCH))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		delete pwndOutputSearchResultEx;
		return NULL;
	}
	pwndOutputSearchResultEx->SetFont(&m_Font);
	DWORD dwExStyle = pwndOutputSearchResultEx->GetExtendedStyle();
	dwExStyle |= (LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	pwndOutputSearchResultEx->SetExtendedStyle(dwExStyle);

	m_mpwndOutputSearchResultEx.insert(std::pair<CString, COutputSearchList*>(strLabel, pwndOutputSearchResultEx));
	m_mpwndOutputSearchResultExID.insert(std::pair<CString, UINT>(strLabel, m_nExtTabID++));

	return pwndOutputSearchResultEx;
}

COutputSearchList* COutputWnd::GetSearchList(CString strLabel)
{
	std::map<CString, COutputSearchList*>::iterator iter = m_mpwndOutputSearchResultEx.find(strLabel);
	if (iter != m_mpwndOutputSearchResultEx.end())
	{
		return (*iter).second;
	}
	else
		return NewSearchList(strLabel);
}

UINT COutputWnd::GetSearchListID(CString strLabel)
{
	std::map<CString, UINT>::iterator iter = m_mpwndOutputSearchResultExID.find(strLabel);
	if (iter != m_mpwndOutputSearchResultExID.end())
		return (*iter).second;
	return 0;
}

void COutputWnd::ClearSearchList()
{
	int nTab = m_wndTabs.GetTabsNum();
	CString strTabLable;
	for (int nIdx = 0; nIdx < nTab; nIdx++)
	{
		if (m_wndTabs.GetTabLabel(nIdx, strTabLable))
		{
			std::map<CString, UINT>::iterator IDiter = m_mpwndOutputSearchResultExID.find(strTabLable);
			if (IDiter != m_mpwndOutputSearchResultExID.end())
				m_mpwndOutputSearchResultExID.erase(IDiter);

			std::map<CString, COutputSearchList*>::iterator iter = m_mpwndOutputSearchResultEx.find(strTabLable);
			if (iter != m_mpwndOutputSearchResultEx.end())
			{
				m_wndTabs.RemoveTab(nIdx--, FALSE);
				delete (*iter).second;
				m_mpwndOutputSearchResultEx.erase(iter);
			}
		}
	}
	m_wndTabs.Invalidate();
}

void COutputWnd::SetSearchCOPBegin()
{
	ClearSearchList();
}

void COutputWnd::SetSearchCOP(CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult, SEARCHLIST_KIND emListType)
{
	//if(vResult.size())
	{
		COutputSearchList* pwndOutputSearchResultEx = GetSearchList(strLayer);

		if (pwndOutputSearchResultEx)
		{
			BOOL bExists = FALSE;
			int nTab = m_wndTabs.GetTabsNum();
			CString strTabLable;
			for (int nIdx = 0; nIdx < nTab; nIdx++)
			{
				if (m_wndTabs.GetTabLabel(nIdx, strTabLable))
				{
					if (strTabLable.CompareNoCase(strLayer) == 0)
						bExists = TRUE;
				}
			}
			if (!bExists)
				m_wndTabs.AddTab(pwndOutputSearchResultEx, strLayer, GetSearchListID(strLayer));

			pwndOutputSearchResultEx->SetSearchCOP(strLayer, vResult, emListType);
		}
	}
}

LRESULT COutputWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (message == OUTPUT_SEARCH_AREA_SELECT || message == OUTPUT_SEARCH_NAME_SELECT ||
	//	message == OUTPUT_SEARCH_NAME_MOVE || message == OUTPUT_SEARCH_AREA_MOVE)
		GetParent()->SendMessage(message, wParam, lParam);

	return CDockablePane::WindowProc(message, wParam, lParam);
}

void COutputWnd::OnSetFocus(CWnd* pOldWnd)
{
	//CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int nActviveIndex = m_wndTabs.GetActiveTab();

	if (nActviveIndex == DEF_OUTPUTLIST_LOG)
		m_wndOutputLog.SetFocus();
	else
	{
		//m_wndOutputSearchResult.SetFocus();
		CString strLabel;
		if (m_wndTabs.GetTabLabel(nActviveIndex, strLabel))
		{
			COutputSearchList* pList = GetSearchList(strLabel);
			if (pList)
				pList->SetFocus();
		}
	}
}