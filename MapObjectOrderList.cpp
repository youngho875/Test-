// MapObjectOrderList.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "C4IMap.h"
#include "Mainfrm.h"
#include "C4IMapView.h"
#include "MapObjectOrderList.h"

// CMapObjectOrderList

IMPLEMENT_DYNAMIC(CMapObjectOrderList, CMFCListCtrl)

CMapObjectOrderList::CMapObjectOrderList()
{

}

CMapObjectOrderList::~CMapObjectOrderList()
{
	m_listMapObjectLayerMoved.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMapObjectOrderList, CMFCListCtrl)
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MAPORDER_TOP, OnMapOrderTop)
	ON_UPDATE_COMMAND_UI(ID_MAPORDER_TOP, OnUpdateMapOrderTop)
	ON_COMMAND(ID_MAPORDER_UP, OnMapOrderUp)
	ON_UPDATE_COMMAND_UI(ID_MAPORDER_UP, OnUpdateMapOrderUp)
	ON_COMMAND(ID_MAPORDER_DOWN, OnMapOrderDown)
	ON_UPDATE_COMMAND_UI(ID_MAPORDER_DOWN, OnUpdateMapOrderDown)
	ON_COMMAND(ID_MAPORDER_BOTTOM, OnMapOrderBottom)
	ON_UPDATE_COMMAND_UI(ID_MAPORDER_BOTTOM, OnUpdateMapOrderBottom)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMapObjectOrderList::OnNMClick)
END_MESSAGE_MAP()



// CMapObjectOrderList 메시지 처리기입니다.


// 지도 위에서 마우스 휠 이동시 확대축소를 위하여 메인으로 포커스 전달
BOOL CMapObjectOrderList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 마우스 휠 이동 시 영역 외 일 경우 메인으로 포커스 이동
	CRect rt;
	GetClientRect(rt);
	ClientToScreen(rt);

	if(!rt.PtInRect(pt))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if(pFrame != NULL)
			pFrame->SetFocus();
		return FALSE;
	}

	return CMFCListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CMapObjectOrderList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	HINSTANCE hOrgInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetInstanceHandle());

	CMenu menu;
	if(menu.LoadMenu(IDR_POPUP_MAPORDER))
	{
		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	}
	SetFocus();
	AfxSetResourceHandle(hOrgInstance);
}

void CMapObjectOrderList::ClearMapObjectAll()
{
	DeleteAllItems();
	m_MapObjectLayer.RemoveAll();
	m_vMapObjectLayerChange.clear();
}

// 지도 레이어 데이터 추가
void CMapObjectOrderList::AddMapObjectLayer(CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex)
{
	int nImageIndex = 0;

	switch(nStypeType)
	{
	case MAP_PRIMITIVE_POINT:	// 점
		nImageIndex = 0;
		break;

	case MAP_PRIMITIVE_LINE:	// 선
		nImageIndex = 1;
		break;

	case MAP_PRIMITIVE_POLYGON: // 면
		nImageIndex = 2;
		break;

	case MAP_PRIMITIVE_DEM:	// 고도
		nImageIndex = 5;
		break;

	case MAP_PRIMITIVE_RASTER:	// 영상
		nImageIndex = 4;
		break;

	case MAP_PRIMITIVE_UNKNOWN:
	default:
		nImageIndex = 5;
		break;
	}

	int nListCount = GetItemCount();
	InsertItem(nListCount, strLayerName, nImageIndex);

	CString strValue = "";
	strValue.Format("%d", nLayerIndex);
	m_MapObjectLayer.SetAt(strLayerName, strValue);

	// Select 처리
	int nSelectedPos = 0;
	POSITION pos = m_listMapObjectLayerMoved.GetHeadPosition();

	while(pos)
	{
		COMMON_INDEX_STRING stSelectedInfo = m_listMapObjectLayerMoved.GetNext(pos);

		if(stSelectedInfo.strData == strLayerName)
		{
			SetItemState(nListCount, LVIS_SELECTED, LVIS_SELECTED);
			EnsureVisible(nListCount, FALSE);
		}
	}
}

// 순서가 변경된 레이어 목록을 전달한다.
BOOL CMapObjectOrderList::GetMapObjectLayerChangeInfo(vector<MAP_OBJECT_ORDER_LAYERCHANGE> &vMapObjectLayerChange)
{
	int nCount = (int) m_vMapObjectLayerChange.size();

	for(int i=0; i<nCount; i++)
		vMapObjectLayerChange.push_back(m_vMapObjectLayerChange.at(i));

	m_vMapObjectLayerChange.clear();
	return (BOOL)nCount;
}

int CMapObjectOrderList::FindMapObjectLayerIndex(CString strLayerName)
{
	CString strValue = "";
	m_MapObjectLayer.Lookup(strLayerName, strValue);

	if(strValue == "")
		return -1;

	return atoi(strValue);
}

// 레이어 순서변경 요청
void CMapObjectOrderList::SendMsgMapObjectLayerMove()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame != NULL)
	{
		CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
			pView->SendMessage(MAP_OBJECT_ORDER_LAYER_SET);
	}
}

void CMapObjectOrderList::OnMapOrderTop()
{
	m_vMapObjectLayerChange.clear();
	m_listMapObjectLayerMoved.RemoveAll();

	POSITION pos = GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		// 선택한 항목을 저장
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);

			// save move item
			COMMON_INDEX_STRING stLayerMoveInfo;
			stLayerMoveInfo.nIndex = nItem;
			stLayerMoveInfo.strData = GetItemText(nItem, 0);
			m_listMapObjectLayerMoved.AddHead(stLayerMoveInfo);
		}

		if(m_listMapObjectLayerMoved.GetCount())
		{
			int nMoveCnt = 0;
			POSITION pos = m_listMapObjectLayerMoved.GetHeadPosition();

			while(pos)
			{
				COMMON_INDEX_STRING stLayerMoveInfo = m_listMapObjectLayerMoved.GetNext(pos);

				int nLayerIndex = FindMapObjectLayerIndex(stLayerMoveInfo.strData) + nMoveCnt;
				int nTargetIndex = FindMapObjectLayerIndex(GetItemText(0, 0));

				if(-1 < nLayerIndex && -1 < nTargetIndex && nLayerIndex != nTargetIndex)
				{
					// save move order change item
					MAP_OBJECT_ORDER_LAYERCHANGE stLayerChangeInfo;
					stLayerChangeInfo.nLayerIndex = FindMapObjectLayerIndex(stLayerMoveInfo.strData) + nMoveCnt;
					stLayerChangeInfo.nTargetIndex = FindMapObjectLayerIndex(GetItemText(0, 0));
					m_vMapObjectLayerChange.push_back(stLayerChangeInfo);
				}

				nMoveCnt++;
			}

			if(0<(int)m_vMapObjectLayerChange.size())
				SendMsgMapObjectLayerMove();
		}
	}
}

void CMapObjectOrderList::OnUpdateMapOrderTop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((BOOL)GetItemCount());
}

// 레이어 순서 위로
void CMapObjectOrderList::OnMapOrderUp()
{
	m_vMapObjectLayerChange.clear();
	m_listMapObjectLayerMoved.RemoveAll();

	POSITION pos = GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		// 선택한 항목을 저장
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);

			// save move item
			COMMON_INDEX_STRING stLayerMoveInfo;
			stLayerMoveInfo.nIndex = nItem;
			stLayerMoveInfo.strData = GetItemText(nItem, 0);
			m_listMapObjectLayerMoved.AddTail(stLayerMoveInfo);
		}

		if(m_listMapObjectLayerMoved.GetCount())
		{
			int nMoveCnt = 0;
			POSITION pos = m_listMapObjectLayerMoved.GetHeadPosition();

			while(pos)
			{
				COMMON_INDEX_STRING stLayerMoveInfo = m_listMapObjectLayerMoved.GetNext(pos);

				int nLayerIndex = FindMapObjectLayerIndex(stLayerMoveInfo.strData);
				int nTargetIndex = FindMapObjectLayerIndex(GetItemText(stLayerMoveInfo.nIndex-1, 0));

				if(-1 < nLayerIndex && -1 < nTargetIndex && nLayerIndex != nTargetIndex)
				{
					// save move order change item
					MAP_OBJECT_ORDER_LAYERCHANGE stLayerChangeInfo;
					stLayerChangeInfo.nLayerIndex = nLayerIndex; 
					stLayerChangeInfo.nTargetIndex = nTargetIndex;
					m_vMapObjectLayerChange.push_back(stLayerChangeInfo);
				}
				else
					break;
			}

			if(0<(int)m_vMapObjectLayerChange.size())
				SendMsgMapObjectLayerMove();
		}
	}
}

void CMapObjectOrderList::OnUpdateMapOrderUp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((BOOL)GetItemCount());
}

// 레이어 순서 아래로
void CMapObjectOrderList::OnMapOrderDown()
{
	m_vMapObjectLayerChange.clear();
	m_listMapObjectLayerMoved.RemoveAll();

	POSITION pos = GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		// 선택한 항목을 저장
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);

			// save move item
			COMMON_INDEX_STRING stLayerMoveInfo;
			stLayerMoveInfo.nIndex = nItem;
			stLayerMoveInfo.strData = GetItemText(nItem, 0);
			m_listMapObjectLayerMoved.AddHead(stLayerMoveInfo);
		}

		if(m_listMapObjectLayerMoved.GetCount())
		{
			POSITION pos = m_listMapObjectLayerMoved.GetHeadPosition();

			while(pos)
			{
				COMMON_INDEX_STRING stLayerMoveInfo = m_listMapObjectLayerMoved.GetNext(pos);

				int nLayerIndex = FindMapObjectLayerIndex(stLayerMoveInfo.strData);
				int nTargetIndex = FindMapObjectLayerIndex(GetItemText(stLayerMoveInfo.nIndex+1, 0));

				if(-1 < nTargetIndex && -1 < nLayerIndex && nLayerIndex != nTargetIndex)
				{
					// save move order change item
					MAP_OBJECT_ORDER_LAYERCHANGE stLayerChangeInfo;
					stLayerChangeInfo.nLayerIndex = nLayerIndex; 
					stLayerChangeInfo.nTargetIndex = nTargetIndex;
					m_vMapObjectLayerChange.push_back(stLayerChangeInfo);
				}
				else
					break;
			}

			if(0<(int)m_vMapObjectLayerChange.size())
				SendMsgMapObjectLayerMove();
		}
	}
}

void CMapObjectOrderList::OnUpdateMapOrderDown(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((BOOL)GetItemCount());
}

// 레이어 순서 맨 아래로
void CMapObjectOrderList::OnMapOrderBottom()
{
	m_vMapObjectLayerChange.clear();
	m_listMapObjectLayerMoved.RemoveAll();

	POSITION pos = GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		// 선택한 항목을 저장
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);

			// save move item
			COMMON_INDEX_STRING stLayerMoveInfo;
			stLayerMoveInfo.nIndex = nItem;
			stLayerMoveInfo.strData = GetItemText(nItem, 0);
			m_listMapObjectLayerMoved.AddTail(stLayerMoveInfo);
		}

		if(m_listMapObjectLayerMoved.GetCount())
		{
			int nMoveCnt = 0;
			POSITION pos = m_listMapObjectLayerMoved.GetHeadPosition();

			while(pos)
			{
				COMMON_INDEX_STRING stLayerMoveInfo = m_listMapObjectLayerMoved.GetNext(pos);

				int nLayerIndex = FindMapObjectLayerIndex(stLayerMoveInfo.strData) - nMoveCnt;
				int nTargetIndex = FindMapObjectLayerIndex(GetItemText(GetItemCount()-1, 0));

				if(-1 < nLayerIndex && -1 < nTargetIndex && nLayerIndex != nTargetIndex)
				{
					// save move order change item
					MAP_OBJECT_ORDER_LAYERCHANGE stLayerChangeInfo;
					stLayerChangeInfo.nLayerIndex = nLayerIndex; 
					stLayerChangeInfo.nTargetIndex = nTargetIndex;
					m_vMapObjectLayerChange.push_back(stLayerChangeInfo);
				}

				nMoveCnt++;
			}

			if(0<(int)m_vMapObjectLayerChange.size())
				SendMsgMapObjectLayerMove();
		}
	}
}

void CMapObjectOrderList::OnUpdateMapOrderBottom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((BOOL)GetItemCount());
}

void CMapObjectOrderList::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if(GetSelectedCount() == 1 && pFrame != NULL)
	{
		int nSelectIndex = GetSelectionMark();
		CC4IMapView * pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
			pView->SendMessage(MAP_OBJECT_VIEW_LAYER_SELECT, (WPARAM)nSelectIndex);
	}
}
