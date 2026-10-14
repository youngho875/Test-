#include "pch.h"
#include "C4IMap.h"
#include "MainFrm.h"
#include "C4IMapView.h"
#include "OutputSearchList.h"

// COutputSearchList

IMPLEMENT_DYNAMIC(COutputSearchList, CMFCListCtrl)

COutputSearchList::COutputSearchList()
{
	m_emListType = SEARCHLIST_NONE;
	m_nColumnCount = 0;
}

COutputSearchList::~COutputSearchList()
{
	//ClearSearch();
}


BEGIN_MESSAGE_MAP(COutputSearchList, CMFCListCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_OUTPUT_COPY, OnResultCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OUTPUT_COPY, OnUpdateResultCopy)
	ON_COMMAND(ID_POPUP_OUTPUT_MOVE, OnResultMove)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OUTPUT_MOVE, OnUpdateResultMove)
	ON_NOTIFY_REFLECT(NM_CLICK, &COutputSearchList::OnNMClick)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// COutputSearchList 메시지 처리기입니다.



void COutputSearchList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	HINSTANCE hOrgInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(AfxGetInstanceHandle());

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SEARCH);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
	AfxSetResourceHandle(hOrgInstance);
}


void COutputSearchList::OnResultCopy()
{
	POSITION pos = GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		CString sResult;

		CWaitCursor wait;
		int nItem, nCount = 0;
		CString strColumn;

		MAPSEARCH_LAYER_RESULT stResult = m_vResult.at(0);
		for (int i = 0; i < (int)stResult.vInfo.size(); i++)
		{
			strColumn += stResult.vInfo.at(i).strTitle;
			strColumn += _T("\t");
		}

		if (strColumn.IsEmpty() == FALSE)
			strColumn += _T("\r\n");

		while (pos)
		{
			nItem = GetNextSelectedItem(pos);
			if (0 != nCount)
				sResult += _T("\r\n");

			for (int i = 0; i < m_nColumnCount; ++i)
			{
				sResult += GetItemText(nItem, i);
				if (i != m_nColumnCount - 1)
					sResult += _T("\t");
			}

			++nCount;
		}

		sResult = strColumn + sResult;

		if (OpenClipboard())
		{
			EmptyClipboard();

			int nLen = sResult.GetLength() * sizeof(WCHAR) + 1;
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
			LPBYTE pGlobalData = (LPBYTE)GlobalLock(hGlobal);

			USES_CONVERSION_EX;
			CopyMemory(pGlobalData, T2CW_EX(sResult, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
			SetClipboardData(CF_UNICODETEXT, hGlobal);

			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
			CloseClipboard();
		}
	}
}

void COutputSearchList::OnUpdateResultCopy(CCmdUI* pCmdUI)
{
	if (0 < GetItemCount())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void COutputSearchList::OnResultMove()
{
	int nSelectIndex = GetSelectionMark();

	if (nSelectIndex < (int)m_vResult.size())
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		if (pFrame)
		{
			CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
			if (pView)
			{
				switch (m_emListType)
				{
				case SEARCHLIST_NONE:// = 0, // 결과 없음
				case SEARCHLIST_SCREEN://,	 // 공간 검색 결과
				{
					//pView->SendMessage(OUTPUT_SEARCH_AREA_MOVE, (WPARAM)nSelectIndex);
					break;
				}
				case SEARCHLIST_NAME://,	 // 검색어 검색 결과
				{
					//pView->SendMessage(OUTPUT_SEARCH_NAME_MOVE, (WPARAM)nSelectIndex);
					break;
				}
				case SEARCHLIST_COP_IMPORTANT://,	// 상황도 중요도시요소
				case SEARCHLIST_COP_ALWAYS://,	// 상황도 상시시현요소
				case SEARCHLIST_COP:
				{
					if (nSelectIndex < (int)m_vResult.size())
					{
						MAPSEARCH_LAYER_RESULT stResult = m_vResult.at(nSelectIndex);

						MAPSEARCH_LAYER_SELECT stSelectInfo;
						stSelectInfo.nIndex = nSelectIndex;
						stSelectInfo.strTitle = m_strTitle;
						//pView->SendMessage(OUTPUT_SEARCH_COP_MOVECENTER, (WPARAM)&stResult, (LPARAM)&stSelectInfo);
					}
					break;
				}
				}
			}
		}
	}
}

void COutputSearchList::OnUpdateResultMove(CCmdUI* pCmdUI)
{
	if (GetSelectedCount() == 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void COutputSearchList::ClearSearch()
{
	m_emListType = SEARCHLIST_NONE;	// 결과 없음

	if (GetSafeHwnd())
		DeleteAllItems();

	for (int i = 0; i < m_nColumnCount; i++)
		DeleteColumn(0);

	for (int j = 0; j < (int)m_vResult.size(); j++)
	{
		MAPSEARCH_LAYER_RESULT resultInfo = m_vResult.at(j);
		resultInfo.vInfo.clear();
	}

	m_nColumnCount = 0;
	m_vResult.clear();
	m_mapListColumn.RemoveAll();
	m_strTitle = "";
}

void COutputSearchList::SetSearchResult(vector<MAPSEARCH_LAYER_RESULT>& vResult)
{
	if (0 < (int)vResult.size())
	{
		MAPSEARCH_LAYER_RESULT stResult = vResult.at(0);

		m_nColumnCount = 1;
		InsertColumn(0, "", LVCFMT_CENTER, 0);

		for (int i = 0; i < (int)stResult.vInfo.size(); i++)
		{
			MAPSEARCH_LAYER_INFO stInfo = stResult.vInfo.at(i);
			InsertColumn(m_nColumnCount, stInfo.strTitle, LVCFMT_CENTER, 100);

			CString strColumnIndex;
			strColumnIndex.Format("%d", m_nColumnCount++);
			m_mapListColumn.SetAt(stInfo.strTitle, strColumnIndex);
		}
	}

	for (int i = 0; i < (int)vResult.size(); i++)
	{
		MAPSEARCH_LAYER_RESULT stResult = vResult.at(i);
		m_vResult.push_back(stResult);

		CString strIndex;
		strIndex.Format("%d", stResult.nIndex);

		InsertItem(i, strIndex);

		for (int j = 0; j < (int)stResult.vInfo.size(); j++)
		{
			MAPSEARCH_LAYER_INFO stInfo = stResult.vInfo.at(j);

			CString strColumnIndex;

			if (m_mapListColumn.Lookup(stInfo.strTitle, strColumnIndex))
			{
				SetItemText(i, atoi(strColumnIndex), stInfo.strContent);
			}
			else
			{
				InsertColumn(m_nColumnCount, stInfo.strTitle, LVCFMT_CENTER, 100);

				CString strColumnIndex;
				strColumnIndex.Format("%d", m_nColumnCount++);
				m_mapListColumn.SetAt(stInfo.strTitle, strColumnIndex);
				SetItemText(i, atoi(strColumnIndex), stInfo.strContent);
			}
		}
	}

}

void COutputSearchList::SetSearchScreen(CString strCategory, CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult)
{
	ClearSearch();
	m_emListType = SEARCHLIST_SCREEN;	// 공간 검색 결과
	m_strTitle = strCategory + "-" + strLayer;
	SetSearchResult(vResult);

}

void COutputSearchList::SetSearchName(CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult)
{
	ClearSearch();
	m_emListType = SEARCHLIST_NAME;	// 검색어 검색 결과
	m_strTitle = strLayer;
	SetSearchResult(vResult);
}

void COutputSearchList::SetSearchCOP(CString strLayer, vector<MAPSEARCH_LAYER_RESULT>& vResult, SEARCHLIST_KIND emListType)
{
	ClearSearch();
	m_emListType = emListType;	// 검색어 검색 결과
	m_strTitle = strLayer;
	SetSearchResult(vResult);
}

void COutputSearchList::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// 결과 단일 선택 시 속성 정보에 출력
	if (GetSelectedCount() == 1)
	{
		int nSelectIndex = GetSelectionMark();

		if (nSelectIndex < (int)m_vResult.size())
		{
			MAPSEARCH_LAYER_RESULT stResult = m_vResult.at(nSelectIndex);

			MAPSEARCH_LAYER_SELECT stSelectInfo;
			stSelectInfo.nIndex = nSelectIndex;
			stSelectInfo.strTitle = m_strTitle;

			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

			if (pFrame)
			{
				CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
				if (pView)
				{
					switch (m_emListType)
					{
					case SEARCHLIST_NONE:// = 0, // 결과 없음
					case SEARCHLIST_SCREEN://,	 // 공간 검색 결과
					{
						//pView->SendMessage(OUTPUT_SEARCH_AREA_SELECT, (WPARAM)&stResult, (LPARAM)&stSelectInfo);
						//pView->SendMessage(OUTPUT_SEARCH_AREA_MOVE, (WPARAM)nSelectIndex);
						break;
					}
					case SEARCHLIST_NAME://,	 // 검색어 검색 결과
					{
						//pView->SendMessage(OUTPUT_SEARCH_NAME_SELECT, (WPARAM)&stResult, (LPARAM)&stSelectInfo);
						//pView->SendMessage(OUTPUT_SEARCH_NAME_MOVE, (WPARAM)nSelectIndex);
						break;
					}
					case SEARCHLIST_COP_IMPORTANT://,	// 상황도 중요도시요소
					case SEARCHLIST_COP_ALWAYS://,	// 상황도 상시시현요소
					case SEARCHLIST_COP:
					{
						//pView->SendMessage(OUTPUT_SEARCH_NAME_SELECT, (WPARAM)&stResult, (LPARAM)&stSelectInfo);
						//pView->SendMessage(OUTPUT_SEARCH_COP_MOVE, (WPARAM)&stResult, (LPARAM)&stSelectInfo);
						break;
					}
					}
				}
			}
		}
	}

	*pResult = 0;
}



BOOL COutputSearchList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect rt;
	GetClientRect(rt);
	ClientToScreen(rt);

	// 마우스 휠 이동 시 영역 외 일 경우 메인으로 포커스 이동
	if (!rt.PtInRect(pt))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame != NULL)
			pFrame->SetFocus();
		return FALSE;
	}

	return CMFCListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}