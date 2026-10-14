
#include "pch.h"
#include "BaseMapOptions.h"
#include "MapSources.h"
#include "C4IMap.h"
#include "Mainfrm.h"
#include "C4IMapView.h"
#include "MapObjectViewTree.h"

#include "ComDefStringValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**
* @struct TREEITEM_INFO
* @brief  트리 아이템 정보
*         only use in CViewOpTree class
*/
struct MAPOBJVIEWTREE_INFO
{
	int nItemType;			
	CString strMyName;		
	CString strParentName;
	int nImageIndex;
	int nMyIndex;			
	int nParentIndex;

	int nLayerType;
};

/**
* @brief  트리 아이템 타입
*/
typedef enum
{
	MAPOBJVIEWTREE_TITLE		= 0, /**< 트리 루트 */
	MAPOBJVIEWTREE_CATEGORY		= 1, /**< 레이어 종류 */
	MAPOBJVIEWTREE_LAYER		= 2, /**< 레이어 */
	MAPOBJVIEWTREE_FEATURE		= 3  /**< 피쳐 */

} MAPOBJVIEWTREE_TYPE;

/////////////////////////////////////////////////////////////////////////////
// CMapObjectViewTree

CMapObjectViewTree::CMapObjectViewTree()
{
	m_bAutoScale = TRUE;
	m_pMapCategoryName.RemoveAll();
	m_pMapLayerName.RemoveAll();   
	m_pMapFeatureName.RemoveAll(); 

	m_hRootItem = NULL;
}

CMapObjectViewTree::~CMapObjectViewTree()
{
	m_pMapCategoryName.RemoveAll();
	m_pMapLayerName.RemoveAll();   
	m_pMapFeatureName.RemoveAll(); 
}

BEGIN_MESSAGE_MAP(CMapObjectViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMapObjectViewTree::OnNMCustomdraw)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MAPVIEW_AUTO_ON, OnMapViewAutoOn)
	ON_UPDATE_COMMAND_UI(ID_MAPVIEW_AUTO_ON, OnUpdateMapViewAutoOn)
	ON_COMMAND(ID_MAPVIEW_AUTO_OFF, OnMapViewAutoOff)
	ON_UPDATE_COMMAND_UI(ID_MAPVIEW_AUTO_OFF, OnUpdateMapViewAutoOff)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapObjectViewTree 메시지 처리기

void CMapObjectViewTree::OnDestroy()
{
	RemoveAllMapObjectViewItems(FALSE);
	CTreeCtrl::OnDestroy();
}

BOOL CMapObjectViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CMapObjectViewTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVCUSTOMDRAW * pCustomDraw = (NMTVCUSTOMDRAW*)pNMHDR;
	*pResult = 0;

	HTREEITEM hItem = NULL;

	switch(pCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:// 아이템 그리기 전에 알려주세요.
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		{
			hItem = (HTREEITEM)pCustomDraw->nmcd.dwItemSpec;

			HTREEITEM hSelectItem = GetSelectedItem();

			// 포커스 없을 때도 짙은 선택 색상 유지하려고 둠 
			if(hItem == hSelectItem)
			{
				pCustomDraw->clrText = RGB(255,255,255); // white text
				pCustomDraw->clrTextBk = RGB(51, 153, 255); // select Color
			}
			else
			{
				pCustomDraw->clrText = RGB(0,0,0); // Black text
				pCustomDraw->clrTextBk = RGB(255, 255, 255); // white Color
			}

			*pResult = CDRF_DODEFAULT; // do not set *pResult = CDRF_SKIPDEFAULT
		}
		break;
	default:
		break;
	}
}

// 모든 아이템 삭제
void CMapObjectViewTree::RemoveAllMapObjectViewItems(BOOL bInitTitle)
{
    m_baseRoot=nullptr;m_baseItems.clear();m_localShapeItems.clear();m_localShapeRoot=nullptr;m_sourceRoots.clear();m_sourceItems.clear();
	m_pMapCategoryName.RemoveAll();
	m_pMapLayerName.RemoveAll();   
	m_pMapFeatureName.RemoveAll(); 

	// Title 제외 삭제
	HTREEITEM hItem = GetRootItem();

	if(hItem)
	{
		MAPOBJVIEWTREE_INFO* pTreeTitleInfo = (MAPOBJVIEWTREE_INFO*)(GetItemData(hItem));

		if(pTreeTitleInfo)
			delete pTreeTitleInfo;
		pTreeTitleInfo = NULL;

		HTREEITEM hSubItem = GetChildItem(hItem);

		if(hSubItem)
			RemoveTreeItem(hSubItem, TRUE);

		DeleteItem(hItem);
	}

	// 소멸 시에는 타이틀을 추가하지 않도록 한다.
	if(bInitTitle == TRUE){InsertMapObjectTitle();RefreshLocalShapeTree();}
}

// 트리 아이템 삭제
void CMapObjectViewTree::RemoveTreeItem(HTREEITEM hItem, BOOL bRemoveNextSiblingItem)
{
	if(hItem)
	{
		MAPOBJVIEWTREE_INFO* pTreeItemInfo = (MAPOBJVIEWTREE_INFO*)(GetItemData(hItem));

		if(pTreeItemInfo)
		{
			RemoveMapKey(pTreeItemInfo->nItemType, pTreeItemInfo->strMyName, pTreeItemInfo->strParentName); 
			delete pTreeItemInfo;
		}
		pTreeItemInfo = NULL;

		HTREEITEM hSubItem = GetChildItem(hItem);

		if(hSubItem)
			RemoveTreeItem(hSubItem, TRUE);

		HTREEITEM hDelItem = hItem;
		hItem = GetNextSiblingItem(hItem);
		DeleteItem(hDelItem);

		if(bRemoveNextSiblingItem)
			RemoveTreeItem(hItem, bRemoveNextSiblingItem);
	}
}

// 트리 아이템 맵 삭제
void CMapObjectViewTree::RemoveMapKey(int nItemType, CString strMyName, CString strParentName)
{
	switch(nItemType)
	{
	case MAPOBJVIEWTREE_FEATURE:
		m_pMapFeatureName.RemoveKey(strParentName+strMyName);
		break;

	case MAPOBJVIEWTREE_CATEGORY:
		m_pMapCategoryName.RemoveKey(strMyName);
		break;

	case MAPOBJVIEWTREE_LAYER:
		m_pMapLayerName.RemoveKey(strMyName);
		break;
	}
}

void CMapObjectViewTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar==VK_SPACE){SetCheckEventItem(GetSelectedItem());return;}
    CTreeCtrl::OnKeyDown(nChar,nRepCnt,nFlags);
	// 체크 박스에 영향을 주어 주석처리함
	//CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapObjectViewTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 체크 박스에 영향을 주어 주석처리함
	//CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CMapObjectViewTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);

	// 아이템 선택 상태
	if(hItem != NULL)
	{
		SelectItem(hItem);
		SendMsgSelectItem(hItem);

		// 체크 클릭
		if(uFlags == TVHT_ONITEMSTATEICON)
			SetCheckEventItem(hItem);

		//else if(hItem != GetSelectedItem())
		//	SelectItem(hItem);
	}

	// 체크 박스에 영향을 주어 예외처리함
	if(uFlags != TVHT_ONITEMSTATEICON)
		CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMapObjectViewTree::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 체크 박스에 영향을 주어 주석 처리함
	//CTreeCtrl::OnRButtonDown(nFlags, point);
}

// 체크 박스 이벤트 처리
void CMapObjectViewTree::SetCheckEventItem(HTREEITEM hItem)
{
	BeginWaitCursor();

	if(hItem != NULL)
	{
		int nCheckOp = 0;	// 미사용

		// 클릭 이전 상태 
		UINT state  = GetItemState(hItem, TVIS_STATEIMAGEMASK);

		state = state >> 12;

		switch(state)
		{
		case TREE_MEMBER_HFCHECK:
		case TREE_MEMBER_UNCHECK:
			{
				// 반체크, 언체크 -> 체크로 변경
				SetItemState(hItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
				SendMsgSelectCheckItem(hItem, TRUE);
				nCheckOp = 1;	// 체크 사용
			}
			break;

		case TREE_MEMBER_ALLCHECK:
			{
				// 체크 -> 언체크로 변경
				SetItemState(hItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);
				SendMsgSelectCheckItem(hItem, FALSE);
				nCheckOp = 2; // 언체크 사용
			}
			break;

		case TREE_MEMBER_NOT:
		default:
			break;
		}

		if(nCheckOp)
		{
			// 하위 체크 반영
			if(ItemHasChildren(hItem))
			{
				HTREEITEM hSubItem = GetChildItem(hItem);

				if(nCheckOp == 1)
					UpdateCheckEventChildItem(hSubItem, TRUE);
				else
					UpdateCheckEventChildItem(hSubItem, FALSE);
			}

			// 상위 체크 업데이트
			HTREEITEM hParentItem = GetParentItem(hItem);

			if(hParentItem != NULL /*&& hParentItem != m_hRootItem*/)
			//if(hParentItem != NULL)
				UpdateCheckEventParentItem(hParentItem);
		}
	}

	ApplyLocalShapeChecks(hItem);
	EndWaitCursor();
}

// 체크 변경 시 하위 아이템 갱신
void CMapObjectViewTree::UpdateCheckEventChildItem(HTREEITEM hItem, BOOL bCheck)
{
	if(hItem)
	{
		while(hItem)
		{
			UINT state  = GetItemState(hItem, TVIS_STATEIMAGEMASK);
			state = state >> 12;

			if(state != TREE_MEMBER_NOT)
				SetCheck(hItem, bCheck);

			if(ItemHasChildren(hItem))
			{
				HTREEITEM hSubItem = GetChildItem(hItem);			
				UpdateCheckEventChildItem(hSubItem, bCheck);
			}

			hItem = GetNextSiblingItem(hItem);
		}
	}
}

// 체크 변경 시 상위 아이템 갱신
void CMapObjectViewTree::UpdateCheckEventParentItem(HTREEITEM hItem)
{
	// Only aggregate groups. Leaf state comes from the layer definition.
	if(!hItem || !ItemHasChildren(hItem)) return;
	int nCheckCount = 0;
	int nUnCheckCount = 0;
	int nHalfCount = 0;

	if(hItem)
	{
		if(ItemHasChildren(hItem))
		{
			HTREEITEM hSubItem = GetChildItem(hItem);

			while(hSubItem)
			{
				UINT state  = GetItemState(hSubItem, TVIS_STATEIMAGEMASK);

				state = state >> 12;

				switch(state)
				{
				case TREE_MEMBER_NONE:
				case TREE_MEMBER_NOT:
					break;

				case TREE_MEMBER_UNCHECK:
					nUnCheckCount++;
					break;

				case TREE_MEMBER_ALLCHECK:
					nCheckCount++;
					break;

				case TREE_MEMBER_HFCHECK:
					nHalfCount++;
					break;

				default:
					break;
				}

				hSubItem = GetNextSiblingItem(hSubItem);
			}
		}

		if(nCheckCount == 0 && nUnCheckCount == 0 && nHalfCount == 0)
		{
			SetItemState(hItem, (TREE_MEMBER_NOT << 12), TVIS_STATEIMAGEMASK);
			UpdateCheckEventParentItem(GetParentItem(hItem));
			return;
		}

		if(0 < nUnCheckCount)
		{
			if(0 < nCheckCount || 0 < nHalfCount)
				SetItemState(hItem, TREE_STATE_HFCHECK, TVIS_STATEIMAGEMASK);
			else
				SetItemState(hItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);
		}
		else
		{
			if(0 < nHalfCount)
				SetItemState(hItem, TREE_STATE_HFCHECK, TVIS_STATEIMAGEMASK);
			else
				SetItemState(hItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
		}

		if(0 < nCheckCount || 0 < nUnCheckCount || 0 < nHalfCount)
		{
			HTREEITEM hParentItem = GetParentItem(hItem);

			//if(hParentItem)
			if(hParentItem != NULL /*&& hParentItem != m_hRootItem*/)
				UpdateCheckEventParentItem(hParentItem);
		}
	}
}

// 축척별 자동 설정 off 시 체크 disable, on 시 체크 Enable
void CMapObjectViewTree::SetMapAutoScale(BOOL bAutoSacle)
{
	m_bAutoScale = bAutoSacle;

	HTREEITEM hItem = GetRootItem();

	if(hItem)
	{	
		HTREEITEM hSubItem = GetChildItem(hItem);

		if(hSubItem)
			SetDisableCheck(hSubItem, bAutoSacle);

		if(bAutoSacle /*|| hItem == m_hRootItem*/)
			SetItemState(hItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);
	}	
}

// 체크 사용 불가로 변경
void CMapObjectViewTree::SetDisableCheck(HTREEITEM hItem, BOOL bDisable)
{
	while(hItem)
	{
        if(hItem==m_baseRoot||hItem==m_localShapeRoot||std::find(m_sourceRoots.begin(),m_sourceRoots.end(),hItem)!=m_sourceRoots.end()){hItem=GetNextSiblingItem(hItem);continue;}
		HTREEITEM hSubItem = GetChildItem(hItem);

		if(hSubItem)
			SetDisableCheck(hSubItem, bDisable);

		if(bDisable)
			SetItemState(hItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);


		hItem = GetNextSiblingItem(hItem);
	}	
}

// 체크 변경 메시지 전달
void CMapObjectViewTree::SendMsgSelectCheckItem(HTREEITEM hItem, BOOL bChecked)
{
	MAPOBJVIEWTREE_INFO* pTreeSelectInfo = (MAPOBJVIEWTREE_INFO*)GetItemData(hItem);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(pTreeSelectInfo != NULL && pFrame != NULL)
	{
		CC4IMapView * pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
		{

			switch(pTreeSelectInfo->nItemType)
			{
			case MAPOBJVIEWTREE_TITLE: // 트리 루트
				{
					if(bChecked)
						pView->SendMessage(MAP_OBJECT_VIEW_ALL_CHECK);
					else
						pView->SendMessage(MAP_OBJECT_VIEW_ALL_UNCHECK);
				}
				break;

			case MAPOBJVIEWTREE_CATEGORY: // 레이어 종류
				{
					if(bChecked)
						pView->SendMessage(MAP_OBJECT_VIEW_CATEGORY_CHECK, (WPARAM)&pTreeSelectInfo->strMyName);
					else
						pView->SendMessage(MAP_OBJECT_VIEW_CATEGORY_UNCHECK, (WPARAM)&pTreeSelectInfo->strMyName);
				}
				break;

			case MAPOBJVIEWTREE_LAYER: // 레이어
				{
					if(bChecked)
						pView->SendMessage(MAP_OBJECT_VIEW_LAYER_CHECK, (WPARAM)pTreeSelectInfo->nMyIndex);
					else
						pView->SendMessage(MAP_OBJECT_VIEW_LAYER_UNCHECK, (WPARAM)pTreeSelectInfo->nMyIndex);
				}
				break;

			case MAPOBJVIEWTREE_FEATURE:// 피쳐
				{
					if(bChecked)
						pView->SendMessage(MAP_OBJECT_VIEW_FEATURE_CHECK, (WPARAM)pTreeSelectInfo->nParentIndex, (LPARAM)pTreeSelectInfo->nMyIndex);
					else
						pView->SendMessage(MAP_OBJECT_VIEW_FEATURE_UNCHECK, (WPARAM)pTreeSelectInfo->nParentIndex, (LPARAM)pTreeSelectInfo->nMyIndex);
				}
				break;
			}
		}
	}
}

// 선택 이벤트 발생 메지시 전달
void CMapObjectViewTree::SendMsgSelectItem(HTREEITEM hItem)
{
	HINSTANCE hOrgInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetInstanceHandle());

	MAPOBJVIEWTREE_INFO* pTreeSelectInfo = (MAPOBJVIEWTREE_INFO*)GetItemData(hItem);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(pTreeSelectInfo != NULL && pFrame != NULL)
	{
		CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
		{
			switch(pTreeSelectInfo->nItemType)
			{
			case MAPOBJVIEWTREE_TITLE: // 트리 루트
				{
					pView->SendMessage(MAP_OBJECT_VIEW_ALL_SELECT);
				}
				break;

			case MAPOBJVIEWTREE_CATEGORY: // 레이어 종류
				{
					pView->SendMessage(MAP_OBJECT_VIEW_CATEGORY_SELECT, (WPARAM)&pTreeSelectInfo->strMyName);
				}
				break;

			case MAPOBJVIEWTREE_LAYER: // 레이어
				{
					pView->SendMessage(MAP_OBJECT_VIEW_LAYER_SELECT, (WPARAM)pTreeSelectInfo->nMyIndex);
				}
				break;

			case MAPOBJVIEWTREE_FEATURE:// 피쳐
				{
					pView->SendMessage(MAP_OBJECT_VIEW_FEATURE_SELECT, (WPARAM)pTreeSelectInfo->nParentIndex, (LPARAM)pTreeSelectInfo->nMyIndex);
				}
				break;
			}
		}
	}

	AfxSetResourceHandle(hOrgInstance);
}

// 지도 루트명 입력
HTREEITEM CMapObjectViewTree::InsertMapObjectTitle()
{
	HTREEITEM hRootItem = GetRootItem();

	if(hRootItem == NULL)
	{
		MAPOBJVIEWTREE_INFO* pTreeTitleInfo = new MAPOBJVIEWTREE_INFO;

		pTreeTitleInfo->nItemType = MAPOBJVIEWTREE_TITLE;	// 타이틀
		pTreeTitleInfo->strMyName = DEF_DOCKPANE_TITLE_MAPVIEW_SUB1;
		pTreeTitleInfo->strParentName = "";
		pTreeTitleInfo->nImageIndex = 0;
		pTreeTitleInfo->nMyIndex = -1;
		pTreeTitleInfo->nParentIndex = -1;		
		pTreeTitleInfo->nLayerType = -1;

		// insert tree item
		m_hRootItem = InsertItem(pTreeTitleInfo->strMyName, pTreeTitleInfo->nImageIndex, pTreeTitleInfo->nImageIndex);
		//hRootItem = InsertItem(pTreeTitleInfo->strMyName, pTreeTitleInfo->nImageIndex, pTreeTitleInfo->nImageIndex);

		// set data to tree item
		SetItemData(m_hRootItem,  (DWORD_PTR)pTreeTitleInfo);
		SetItemState(m_hRootItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);
	}
	
	return m_hRootItem;
}

// 지도 레이어 데이터 추가
HTREEITEM CMapObjectViewTree::InsertMapObjectLayer(CString strCategory, CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex)
{
	HTREEITEM hCateItem = InsertMapObjectCategory(strCategory);
	HTREEITEM hLayerItem = FindMapObjectLayerItem(strLayerName);

	if(hCateItem != NULL && hLayerItem == NULL)
	{
		MAPOBJVIEWTREE_INFO* pTreeLayerInfo = new MAPOBJVIEWTREE_INFO;

		pTreeLayerInfo->nItemType = MAPOBJVIEWTREE_LAYER;	// 타이틀
		pTreeLayerInfo->strMyName = strLayerName;
		pTreeLayerInfo->strParentName = strCategory;
		pTreeLayerInfo->nImageIndex = 0;
		pTreeLayerInfo->nMyIndex = nLayerIndex;			
		pTreeLayerInfo->nParentIndex = -1;
		pTreeLayerInfo->nLayerType = (int)nStypeType;
		
		switch(nStypeType)
		{
		case MAP_PRIMITIVE_POINT://  점
			pTreeLayerInfo->nImageIndex = 1;
			break;

		case MAP_PRIMITIVE_LINE: // 선
			pTreeLayerInfo->nImageIndex = 2;
			break;

		case MAP_PRIMITIVE_POLYGON:// 다각형
			pTreeLayerInfo->nImageIndex = 3;
			break;

		case MAP_PRIMITIVE_RASTER: // Raster (CIB, JPG, CADRG 등 )
			pTreeLayerInfo->nImageIndex = 5;
			break;

		case MAP_PRIMITIVE_DEM: // DEM 고도 데이타 등
			pTreeLayerInfo->nImageIndex = 6;
			break;
		}

		hLayerItem = InsertItem(pTreeLayerInfo->strMyName, pTreeLayerInfo->nImageIndex, pTreeLayerInfo->nImageIndex, hCateItem);
		m_pMapLayerName.SetAt(pTreeLayerInfo->strMyName, (void*) hLayerItem);
		
		// set data to tree item
		SetItemData(hLayerItem,  (DWORD_PTR)pTreeLayerInfo);
		SetItemState(hLayerItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);
	}

	return hLayerItem;
}

// 지도 레이어 별 피쳐 데이터 추가
void CMapObjectViewTree::InsertMapObjectFeature(CString strLayerName, CString strElementName, int nFeatureIndex)
{
	HTREEITEM hLayerItem = FindMapObjectLayerItem(strLayerName);
	HTREEITEM hFeatureItem = FindMapObjectFeatureItem(strElementName, strElementName);

	if(hLayerItem != NULL && hFeatureItem == NULL)
	{
		MAPOBJVIEWTREE_INFO* pTreeLayerInfo = (MAPOBJVIEWTREE_INFO*)GetItemData(hLayerItem);
		
		if(pTreeLayerInfo != NULL)
		{
			MAPOBJVIEWTREE_INFO* pTreeFeatureInfo = new MAPOBJVIEWTREE_INFO;

			pTreeFeatureInfo->nItemType = MAPOBJVIEWTREE_FEATURE;	// 타이틀
			pTreeFeatureInfo->strMyName = strElementName;
			pTreeFeatureInfo->strParentName = pTreeLayerInfo->strMyName;
			pTreeFeatureInfo->nImageIndex = pTreeLayerInfo->nImageIndex;
			pTreeFeatureInfo->nMyIndex = nFeatureIndex;			
			pTreeFeatureInfo->nParentIndex = pTreeLayerInfo->nMyIndex;		
			pTreeFeatureInfo->nLayerType = pTreeLayerInfo->nLayerType;

			hFeatureItem = InsertItem(pTreeFeatureInfo->strMyName, pTreeFeatureInfo->nImageIndex, pTreeFeatureInfo->nImageIndex, hLayerItem);	// 그룹형
			m_pMapFeatureName.SetAt(pTreeFeatureInfo->strParentName + pTreeFeatureInfo->strMyName, (void*) hFeatureItem);

			// set data to tree item
			SetItemData(hFeatureItem,  (DWORD_PTR)pTreeFeatureInfo);
			SetItemState(hFeatureItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);

			// 하위를 가지는 레이어 아이템을 그룹형 이미지로 변경
			SetItemImage(hLayerItem, 0, 0);
		}		
	}
}

// Tree에 Category를 추가한다
HTREEITEM CMapObjectViewTree::InsertMapObjectCategory(CString strCategoryName)
{
	HTREEITEM hCateItem = FindMapObjectCategoryItem(strCategoryName);

	if(hCateItem == NULL)
	{
		HTREEITEM hRootItem = GetRootItem();

		if(hRootItem == NULL)
			hRootItem = InsertMapObjectTitle();

		MAPOBJVIEWTREE_INFO* pTreeCategoryInfo = new MAPOBJVIEWTREE_INFO;

		pTreeCategoryInfo->nItemType = MAPOBJVIEWTREE_CATEGORY;
		pTreeCategoryInfo->strMyName = strCategoryName;
		pTreeCategoryInfo->strParentName = DEF_DOCKPANE_TITLE_MAPVIEW_SUB1;
		pTreeCategoryInfo->nMyIndex = -1;			
		pTreeCategoryInfo->nParentIndex = -1;	
		pTreeCategoryInfo->nLayerType = -1;

		// insert tree item
		hCateItem = InsertItem(pTreeCategoryInfo->strMyName, 0, 0, hRootItem);
		// set map
		m_pMapCategoryName.SetAt(pTreeCategoryInfo->strMyName, (void*) hCateItem);
		// set data to tree item
		SetItemData(hCateItem,  (DWORD_PTR)pTreeCategoryInfo);
		SetItemState(hCateItem, TREE_STATE_NONE, TVIS_STATEIMAGEMASK);
		Expand(hRootItem, TVE_EXPAND);
	}

	return hCateItem;
}

// Category 아이템을 찾는다.
HTREEITEM CMapObjectViewTree::FindMapObjectCategoryItem(CString strCategoryName)
{
	void * ptr = NULL;
	HTREEITEM hCateItem = NULL;

	if(m_pMapCategoryName.Lookup(strCategoryName, (void*) ptr))
		hCateItem = (HTREEITEM) ptr;

	return hCateItem;
}

// Layer 아이템을 찾는다.
HTREEITEM CMapObjectViewTree::FindMapObjectLayerItem(CString strLayerName)
{
	void * ptr = NULL;
	HTREEITEM hLayerItem = NULL;

	if(m_pMapLayerName.Lookup(strLayerName, (void*) ptr))
		hLayerItem = (HTREEITEM) ptr;

	return hLayerItem;
}

// Feature 아이템을 찾는다.
HTREEITEM CMapObjectViewTree::FindMapObjectFeatureItem(CString strLayerName, CString strFeatureName)
{
	void * ptr = NULL;
	HTREEITEM hFeatureItem = NULL;

	if(m_pMapFeatureName.Lookup(strLayerName+strFeatureName, (void*) ptr))
		hFeatureItem = (HTREEITEM) ptr;

	return hFeatureItem;
}

// Layer 아이템 체크 상태 설정
void CMapObjectViewTree::SetCheckMapObjectLayer(CString strLayerName, BOOL bVisible)
{
	HTREEITEM hLayerItem = FindMapObjectLayerItem(strLayerName);

	if(hLayerItem != NULL) // && bVisible == TRUE)
	{
		if(bVisible == TRUE)
			SetItemState(hLayerItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hLayerItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);

		HTREEITEM hParentItem = GetParentItem(hLayerItem);

		if(hParentItem != NULL /*&& hParentItem != m_hRootItem*/)
			UpdateCheckEventParentItem(hParentItem);
	}
}

// Feature 체크 상태 설정 및 Layer 상태 업데이트
void CMapObjectViewTree::SetCheckMapObjectFeature(CString strLayerName, CString strElementName, BOOL bVisible)
{
	HTREEITEM hFeatureItem = FindMapObjectFeatureItem(strLayerName, strElementName);

	if(hFeatureItem != NULL) // && bVisible == TRUE)
	{
		if(bVisible == TRUE)
			SetItemState(hFeatureItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hFeatureItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);

		HTREEITEM hParentItem = GetParentItem(hFeatureItem);

		if(hParentItem != NULL /*&& hParentItem != m_hRootItem*/)
			UpdateCheckEventParentItem(hParentItem);
	}
}

// Catrgory 및 Root 상태 업데이트
void CMapObjectViewTree::UpdateCheckMapObject()
{
	HTREEITEM hRootItem = GetRootItem();

	if(hRootItem)
	{	
		HTREEITEM hCategoryItem = GetChildItem(hRootItem);

		while(hCategoryItem)
		{
			UpdateCheckEventParentItem(hCategoryItem);
			hCategoryItem = GetNextSiblingItem(hCategoryItem);
		}

		//UpdateCheckEventParentItem(hRootItem);
	}	
}

// 위성영상 체크 설정
void CMapObjectViewTree::SetCheckMapObjectCategoryRaster(BOOL bVisible)
{
	HTREEITEM hCategoryItem = FindMapObjectCategoryItem(MAP_OBJECT_RASTER_CATEGORY);

	if(hCategoryItem != NULL)
	{
		if(bVisible)
			SetItemState(hCategoryItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hCategoryItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);

		UpdateCheckEventChildItem(GetChildItem(hCategoryItem), bVisible);
	}
}

// 음영기복도 체크 설정
void CMapObjectViewTree::SetCheckMapObjectCategoryDTED(BOOL bVisible)
{
	HTREEITEM hCategoryItem = FindMapObjectCategoryItem(MAP_OBJECT_DTED_CATEGORY);

	if(hCategoryItem != NULL)
	{
		if(bVisible)
			SetItemState(hCategoryItem, TREE_STATE_ALLCHECK, TVIS_STATEIMAGEMASK);
		else
			SetItemState(hCategoryItem, TREE_STATE_UNCHECK, TVIS_STATEIMAGEMASK);

		UpdateCheckEventChildItem(GetChildItem(hCategoryItem), bVisible);
	}
}

// 종류에 해당하는 레이어 인덱스 목록 반환
BOOL CMapObjectViewTree::GetLayerIndexByCategory(CString strCategoryName, vector<int> &vLayerIndex)
{
	int nCount = 0;
	HTREEITEM hCategoryItem = FindMapObjectCategoryItem(strCategoryName);

	if(hCategoryItem)
	{
		if(ItemHasChildren(hCategoryItem))
		{
			HTREEITEM hLayerItem = GetChildItem(hCategoryItem);

			while(hLayerItem != NULL)
			{
				MAPOBJVIEWTREE_INFO* pLayerInfo = (MAPOBJVIEWTREE_INFO*)GetItemData(hLayerItem);

				if(pLayerInfo != NULL)
					vLayerIndex.push_back(pLayerInfo->nMyIndex);

				hLayerItem = GetNextSiblingItem(hLayerItem);
				nCount++;
			}
		}
	}

	return (BOOL)nCount;
}

// 지도 화면 위에서 휠 이동 시 포커스를 반환하기 위하여
BOOL CMapObjectViewTree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

	return CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CMapObjectViewTree::OnContextMenu(CWnd* pWnd, CPoint point)
{
	HINSTANCE hOrgInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( AfxGetInstanceHandle());
	CMenu menu;
	if(menu.LoadMenu(IDR_POPUP_MAPVIEW))
	{
		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			{
				AfxSetResourceHandle(hOrgInstance);
				return;

			}

			((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}
	}

	SetFocus();
	AfxSetResourceHandle(hOrgInstance);
}

void CMapObjectViewTree::OnMapViewAutoOn()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(pFrame != NULL)
	{
		CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
			pView->SendMessage(MAP_OBJECT_VIEW_MAP_AUTO_ONOFF);
	}
}

void CMapObjectViewTree::OnUpdateMapViewAutoOn(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bAutoScale);
}

// 자동 조절 Off
void CMapObjectViewTree::OnMapViewAutoOff()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(pFrame != NULL)
	{
		CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
		if(pView)
			pView->SendMessage(MAP_OBJECT_VIEW_MAP_AUTO_ONOFF);
	}
}

void CMapObjectViewTree::OnUpdateMapViewAutoOff(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bAutoScale);
}

void CMapObjectViewTree::RefreshLocalShapeTree(){
    if(!GetSafeHwnd())return;
    RefreshSourceTree();
    if(m_localShapeRoot)DeleteItem(m_localShapeRoot);
    m_localShapeRoot=nullptr;m_localShapeItems.clear();
    auto frame=DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());if(!frame)return;
    if(m_baseRoot)DeleteItem(m_baseRoot);m_baseRoot=nullptr;m_baseItems.clear();
#if C4MAP_ENABLE_LOCAL
    if(!m_hRootItem)InsertMapObjectTitle();
    m_baseRoot=InsertItem(_T("기본지도"),0,0,m_hRootItem);
    const LPCTSTR titles[]={_T("소축척 지도"),_T("세계지도"),_T("Natural Earth 배경영상"),_T("지명")};
    for(int i=0;i<4;++i){auto item=InsertItem(titles[i],0,0,m_baseRoot);m_baseItems[item]=i;SetCheck(item,BaseMapVisible(i));}
    UpdateCheckEventParentItem(m_baseRoot);Expand(m_baseRoot,TVE_EXPAND);
#endif
    auto layers=frame->m_indexMap.LocalDisplayLayers();if(layers.empty())return;
    if(!m_hRootItem)InsertMapObjectTitle();
    m_localShapeRoot=InsertItem(frame->m_indexMap.HasLocalCatalog()?_T("지도 (자동 축척)"):_T("로컬 SHP"),0,0,m_hRootItem);
    std::map<CString,HTREEITEM> groups;
    auto group=[&](const CString& key,const CString& label,HTREEITEM parent){auto it=groups.find(key);if(it!=groups.end())return it->second;auto item=InsertItem(label,0,0,parent);groups[key]=item;return item;};
    for(const auto& layer:layers){HTREEITEM parent=m_localShapeRoot;CString key;
        if(!layer.family.IsEmpty()){key=layer.family;parent=group(key,layer.family,parent);key+=_T("|")+layer.scale;parent=group(key,layer.scale,parent);}
        key+=_T("|")+layer.category;parent=group(key,layer.category,parent);
        auto item=InsertItem(layer.name,0,0,parent);m_localShapeItems[item]=layer.id;
        if(layer.available)SetCheck(item,layer.checked);else {SetItemState(item,(TREE_MEMBER_NOT << 12),TVIS_STATEIMAGEMASK);}
    }
    // Children before parents, preserving partial check state.
    for(auto it=groups.rbegin();it!=groups.rend();++it)UpdateCheckEventParentItem(it->second);
    for(const auto& entry:groups)if(GetParentItem(entry.second)==m_localShapeRoot)Expand(entry.second,TVE_EXPAND);
    Expand(m_localShapeRoot,TVE_EXPAND);Expand(m_hRootItem,TVE_EXPAND);

}
void CMapObjectViewTree::ApplyLocalShapeChecks(HTREEITEM changed){
    auto frame=DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());if(!frame||!changed)return;bool touched=false;
    for(const auto& entry:m_baseItems){auto parent=entry.first;while(parent&&parent!=changed)parent=GetParentItem(parent);if(parent){SetBaseMapVisible(entry.second,(GetItemState(entry.first,TVIS_STATEIMAGEMASK)>>12)==TREE_MEMBER_ALLCHECK);touched=true;}}
    for(const auto& entry:m_localShapeItems){HTREEITEM parent=entry.first;while(parent&&parent!=changed)parent=GetParentItem(parent);
        if(parent){const UINT state=GetItemState(entry.first,TVIS_STATEIMAGEMASK)>>12;
            if(state!=TREE_MEMBER_NOT){frame->m_indexMap.SetLocalDisplayVisible(entry.second,state==TREE_MEMBER_ALLCHECK);touched=true;}}}
    for(const auto& entry:m_sourceItems){auto parent=entry.first;while(parent&&parent!=changed)parent=GetParentItem(parent);if(parent){MapSources().SetVisible(entry.second,(GetItemState(entry.first,TVIS_STATEIMAGEMASK)>>12)==TREE_MEMBER_ALLCHECK);touched=true;}}
    if(touched){auto view=DYNAMIC_DOWNCAST(CC4IMapView,frame->GetActiveView());if(view)view->DrawSituation(TRUE,TRUE,TRUE,TRUE);}
}

void CMapObjectViewTree::RefreshSourceTree(){
    for(auto root:m_sourceRoots)DeleteItem(root);m_sourceRoots.clear();m_sourceItems.clear();
    if(!m_hRootItem)InsertMapObjectTitle();std::map<CString,HTREEITEM> groups;
    auto group=[&](const CString& key,const CString& title,HTREEITEM parent){auto it=groups.find(key);if(it!=groups.end())return it->second;auto item=InsertItem(title,0,0,parent);groups[key]=item;SetCheck(item,FALSE);if(parent==m_hRootItem)m_sourceRoots.push_back(item);return item;};
#if C4MAP_ENABLE_LOCAL
    auto raster=group(_T("Raster"),_T("래스터"),m_hRootItem);group(_T("Raster|육도"),_T("육도"),raster);group(_T("Raster|공도"),_T("공도"),raster);group(_T("CIB"),_T("CIB"),m_hRootItem);
#endif
    auto layers=MapSources().Layers();
    auto text=[](const std::string& s){int n=MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,nullptr,0);CStringW w;MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,w.GetBuffer(n),n);w.ReleaseBuffer();return CString(w);};
    for(size_t i=0;i<layers.size();++i){const auto& layer=layers[i];bool local=layer.type=="Raster"||layer.type=="CIB";if((local&&!C4MAP_ENABLE_LOCAL)||(!local&&!C4MAP_ENABLE_OGC))continue;
        CString type=text(layer.type);auto root=group(type,layer.type=="Raster"?_T("래스터"):type,m_hRootItem);auto parent=root;
        if(layer.type=="Raster")parent=group(type+_T("|")+text(layer.family),text(layer.family),root);
        auto item=InsertItem(text(layer.name),0,0,parent);m_sourceItems[item]=i;SetCheck(item,layer.visual);UpdateCheckEventParentItem(parent);
    }
    for(auto root:m_sourceRoots)Expand(root,TVE_EXPAND);Expand(m_hRootItem,TVE_EXPAND);
}
