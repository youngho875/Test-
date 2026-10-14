// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "pch.h"
#include "C4IMap.h"
#include "Mainfrm.h"
#include "MapObjectManager.h"

#include "ComDefStringValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEF_MAPOBJMGR_VIEW		0
#define DEF_MAPOBJMGR_ORDER		1

/////////////////////////////////////////////////////////////////////////////
// COutputBar

CMapObjectManager::CMapObjectManager()
{
	
}

CMapObjectManager::~CMapObjectManager()
{

}

BEGIN_MESSAGE_MAP(CMapObjectManager, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

int CMapObjectManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 탭 창을 만듭니다.
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_3D_ONENOTE, rectDummy, this, 1, CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("출력 탭 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 순서조절 및 도시설정 창을 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
									TVS_CHECKBOXES | TVS_DISABLEDRAGDROP;

	DWORD dwListStyle = LVS_SHOWSELALWAYS | LVS_REPORT | LVS_NOCOLUMNHEADER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndMapObjectViewTree.Create(dwViewStyle, rectDummy, &m_wndTabs, 2) || 
		!m_wndMapObjectOrderList.Create(dwListStyle, rectDummy, &m_wndTabs, 3))
	{
		TRACE0("순서조절 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_MapObjectViewImages.Create(IDB_MAPVIEW_TREE, 16, 0, RGB(255, 0, 255));
	m_wndMapObjectViewTree.SetImageList(&m_MapObjectViewImages, TVSIL_NORMAL);

	m_MapObjectOrderImages.Create(IDB_MAPORDER_LIST, 16, 6, ILC_MASK); 
	m_wndMapObjectOrderList.SetImageList(&m_MapObjectOrderImages, LVSIL_SMALL);
	m_wndMapObjectOrderList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndMapObjectOrderList.InsertColumn(0, "NAME",	LVCFMT_LEFT, 180);

	OnChangeVisualStyleTree();
	OnChangeVisualStyleList();

	m_wndMapObjectViewTree.SetFont(&m_Font);
	m_wndMapObjectOrderList.SetFont(&m_Font);

	m_wndTabs.AddTab(&m_wndMapObjectViewTree, DEF_DOCKPANE_TITLE_MAPMANAGER_SUB1, (UINT)DEF_MAPOBJMGR_VIEW);
	m_wndTabs.AddTab(&m_wndMapObjectOrderList, DEF_DOCKPANE_TITLE_MAPMANAGER_SUB2, (UINT)DEF_MAPOBJMGR_ORDER);

	//AdjustLayout();

	return 0;
}

void CMapObjectManager::OnDestroy()
{
	if(m_MapObjectViewImages)
		m_MapObjectViewImages.DeleteImageList();

	if(m_TreeImagesCheckBox)
		m_TreeImagesCheckBox.DeleteImageList();

	if(m_MapObjectOrderImages)
		m_MapObjectOrderImages.DeleteImageList();

	CDockablePane::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMapObjectManager::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab 컨트롤은 전체 클라이언트 영역을 처리해야 합니다.
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CMapObjectManager::OnChangeVisualStyleTree()
{
	m_MapObjectViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_MAPVIEW_TREE_24 : IDB_MAPVIEW_TREE;

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

	m_MapObjectViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_MapObjectViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndMapObjectViewTree.SetImageList(&m_MapObjectViewImages, TVSIL_NORMAL);

	m_TreeImagesCheckBox.Create(IDB_TREE_CHECKBOX, 13, 4, ILC_MASK);
	m_wndMapObjectViewTree.SetImageList(&m_TreeImagesCheckBox, TVSIL_STATE);
}

void CMapObjectManager::OnChangeVisualStyleList()
{
	m_MapObjectOrderImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_MAPORDER_LIST_24 : IDB_MAPORDER_LIST;

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

	m_MapObjectOrderImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_MapObjectOrderImages.Add(&bmp, RGB(255, 0, 255));

	m_wndMapObjectOrderList.SetImageList(&m_MapObjectOrderImages, LVSIL_SMALL);
}

// 순서가 변경된 레이어 목록을 전달한다.
BOOL CMapObjectManager::GetMapObjectLayerChangeInfo(std::vector<MAP_OBJECT_ORDER_LAYERCHANGE> &vMapObjectLayerChange)
{
	return m_wndMapObjectOrderList.GetMapObjectLayerChangeInfo(vMapObjectLayerChange);
}

// 지도 도시 요소 모두 삭제
void CMapObjectManager::ClearMapObjectAll(BOOL bOnlyOrderList)
{
	if(bOnlyOrderList == FALSE)
		m_wndMapObjectViewTree.RemoveAllMapObjectViewItems();

	m_wndMapObjectOrderList.ClearMapObjectAll();
}

// 지도 도시 요소 레이어 데이터 추가
void CMapObjectManager::AddMapObjectLayer(CString strCategory, CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex, BOOL bOnlyOrderList)
{
	// 위성 영상은 조절이 불가하도록 한다.
	//if(strCategory != MAP_OBJECT_RASTER_CATEGORY)
	
	if(bOnlyOrderList == FALSE)
		m_wndMapObjectViewTree.InsertMapObjectLayer(strCategory, strLayerName, nStypeType, nLayerIndex);
	//m_wndMapObjectViewTree.SetMapAutoScale(TRUE);
	m_wndMapObjectOrderList.AddMapObjectLayer(strLayerName, nStypeType, nLayerIndex);
}

// 지도 도시 요소 레이어 별 피쳐 데이터 추가
void CMapObjectManager::AddMapObjectFeature(CString strLayerName, CString strElementName, int nFeatureIndex)
{
	m_wndMapObjectViewTree.InsertMapObjectFeature(strLayerName, strElementName, nFeatureIndex);
	//m_wndMapObjectViewTree.SetMapAutoScale(TRUE);
}

// 축척별 자동 설정
void CMapObjectManager::SetMapAutoScale(BOOL bAutoSacle)
{
	m_wndMapObjectViewTree.SetMapAutoScale(bAutoSacle);
}

// 레이어 체크 설정
void CMapObjectManager::SetVisibleMapObjectLayer(CString strLayerName, BOOL bVisible)
{
	m_wndMapObjectViewTree.SetCheckMapObjectLayer(strLayerName, bVisible);
}

// 피쳐 체크 설정
void CMapObjectManager::SetVisibleMapObjectFeature(CString strLayerName, CString strElementName, BOOL bVisible)
{
	m_wndMapObjectViewTree.SetCheckMapObjectFeature(strLayerName, strElementName, bVisible);
}

// 위성지도 체크 설정
void CMapObjectManager::SetVisibleMapObjectCategoryRaster(BOOL bVisible)
{
	m_wndMapObjectViewTree.SetCheckMapObjectCategoryRaster(bVisible);
}

// 음영도 체크 설정
void CMapObjectManager::SetVisibleMapObjectCategoryDTED(BOOL bVisible)
{
	m_wndMapObjectViewTree.SetCheckMapObjectCategoryDTED(bVisible);
}

// 레이어 종류에 해당하는 레이어 인덱스 목록 획득
BOOL CMapObjectManager::GetLayerIndexByCategory(CString strCategoryName, vector<int> &vLayerIndex)
{
	return m_wndMapObjectViewTree.GetLayerIndexByCategory(strCategoryName, vLayerIndex);
}

//void CMapObjectManager::OnSetFocus(CWnd* pOldWnd)
//{
//	CDockablePane::OnSetFocus(pOldWnd);
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int nActiveTab = m_wndTabs.GetActiveTab();
//
//	if(nActiveTab == DEF_MAPOBJMGR_ORDER)
//	{
//		m_wndMapObjectOrderList.SetFocus();
//	}
//	else
//	{
//		m_wndMapObjectViewTree.SetFocus();
//	}
//}

void CMapObjectManager::OnSetFocus(CWnd* pOldWnd)
{
	//CDockablePane::OnSetFocus(pOldWnd);

	int nActiveTab = m_wndTabs.GetActiveTab();

	if(nActiveTab == DEF_MAPOBJMGR_ORDER)
		m_wndMapObjectOrderList.SetFocus();
	else
		m_wndMapObjectViewTree.SetFocus();
}
