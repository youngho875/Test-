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

// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "MapSources.h"
#include "framework.h"
#include "C4IMap.h"

#include "MainFrm.h"
#include "C4IMapView.h"
#include "ComDefStringValue.h"
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)

	// docking bar 처리
	ON_COMMAND(ID_RIBBON_COP_LAYERLIST, &CMainFrame::OnLayerOnOff)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_COP_LAYERLIST, &CMainFrame::OnUpdateLayerOnOff)
	ON_COMMAND(ID_RIBBON_COP_ITEMLIST, &CMainFrame::OnItemOnOff)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_COP_ITEMLIST, &CMainFrame::OnUpdateItemOnOff)
	ON_COMMAND(ID_RIBBON_COP_TYPELIST, &CMainFrame::OnTypeOnOff)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_COP_TYPELIST, &CMainFrame::OnUpdateTypeOnOff)
	ON_COMMAND(ID_MENU_INDEXMAP_ONOFF, &CMainFrame::OnMenuIndexMapOnOff)
	ON_UPDATE_COMMAND_UI(ID_MENU_INDEXMAP_ONOFF, &CMainFrame::OnUpdateMenuIndexMapOnOff)
	ON_COMMAND(ID_MENU_MAPMANAGER_ONOFF, &CMainFrame::OnMenuMapManagerOnOff)
	ON_UPDATE_COMMAND_UI(ID_MENU_MAPMANAGER_ONOFF, &CMainFrame::OnUpdateMenuMapManagerOnOff)
	ON_COMMAND(ID_MENU_MAPPROP_ONOFF, &CMainFrame::OnMenuMapPropOnOff)
	ON_UPDATE_COMMAND_UI(ID_MENU_MAPPROP_ONOFF, &CMainFrame::OnUpdateMenuMapPropOnOff)
	//ON_COMMAND(ID_MENU_OUTPUT_ONOFF, &CMainFrame::OnMenuOutputOnOff)
	//ON_UPDATE_COMMAND_UI(ID_MENU_OUTPUT_ONOFF, &CMainFrame::OnUpdateMenuOutputOnOff)

	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	//theApp.m_nAppLook = 212; // 
	//theApp.m_nAppLook = 198; //= theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);

	m_nRibbonDrawGalleryRecently = 1;

	// 지도
	m_pRibbonMapCurrentScale = NULL;				// 도시 축척
	m_pRibbonMapMinScale = NULL;					// 도시 최소 축척
	m_pRibbonMapMaxScale = NULL;					// 도시 최대 축척
	m_pRibbonMapZoomScale = NULL;					// 확대/축소 간격 축척
	m_pRibbonMapBookMarkCombo = NULL;				// 즐겨찾기 목록
	m_pRibbonMapBrightSlider = NULL;				// 밝기 슬라이드
	m_pRibbonMapBrightEdit = NULL;					// 밝기 에디트	
	m_pRibbonMapContrastSlider = NULL;				// 명암 슬라이드
	m_pRibbonMapContrastEdit = NULL;				// 명암 에디트
	m_pRibbonMapAutoChange = NULL;					// 축척 변경 시 지도 자동 조절 On/Off
	m_pRibbonSearchAreaTarget = NULL;				// 공간검색 레이어 명칭
	m_pRibbonSearchAreaType = NULL;					// 공간검색 화면 선택 방식
	m_pRibbonSearchAreaColor = NULL;				// 공간검색 결과 색상
	m_pRibbonSearchMapField = NULL;					// 검색어 검색 필드 명칭
	m_pRibbonSearchMapInput = NULL;					// 검색어
	m_pRibbonAnalysisVisibleRange = NULL;			// 가시권 분석 범위
	m_pRibbonAnalysisVisibleHeight = NULL;			// 가시권 분석 높이
	m_pRibbonAnalysisVisibleColor = NULL;			// 가시권 분석 색상
	m_pRibbonRadiusInterval = NULL;					// 반경 측정 간격
	m_pRibbonRadiusRange = NULL;					// 반경 측정 범위
	m_pRibbonAnalysisRadiusWidth = NULL;			// 반경 측정 선 굵기
	m_pRibbonAnalysisRadiusColor = NULL;			// 반경 측정 선 색상

	// 상황도
	m_pRibbonCOPItemSizeSlider = NULL;
	m_pRibbonCOPItemBrightSlider = NULL;
	m_pRibbonCOPItemSizeSliderEdit = NULL;
	m_pRibbonCOPItemBrightSliderEdit = NULL;
	m_pRibbonCOPItemColor = NULL;
	m_pRibbonCOPItemColorOnOff = NULL;
	m_pwndCOPLayerViewOnOff = NULL;					// 상황도 레이어 On/Off
	m_pwndCOPItemViewOnOff = NULL;					// 상황도 도시요소 창 On/Off
	m_pwndCOPTypeViewOnOff = NULL;					// 상황도 유형 창 On/Off
	m_pwndCOPImportantPatrol = NULL;				// 중요도시요소 패트롤 창 
	m_pRibbonCOPSearchLayer = NULL;					// 상황도 공간검색 레이어 명칭
	m_pRibbonCOPSearchColumn = NULL;				// 상황도 공간검색 컬럼 명칭
	m_pRibbonCOPSearchColor = NULL;					// 상황도 공간검색 결과 색상
	m_pRibbonCOPSearchType = NULL;					// 상황도 공간검색 화면 선택 방식
	m_pRibbonCOPSearchWord = NULL;					// 검색어

	// 투명도
	m_pwndOverlayLayerViewOnOff = NULL;				// 투명도 레이어 On/Off
	m_pwndOverlayItemViewOnOff = NULL;				// 투명도 도시요소 창 On/Off
	m_pRibbonDrawShapeLineColor = NULL;				// 투명도 속성 선 색상
	m_pRibbonDrawShapeLineWidth = NULL;
	m_pRibbonDrawShapeLineDesh = NULL;
	m_pRibbonDrawShapeLineBow = NULL;
	m_pRibbonDrawShapeFillColor = NULL;				// 투명도 속성 내부 색상
	m_pRibbonDrawShapePatternColor = NULL;			// 투명도 속성 패턴 색상
	m_pRibbonDrawShapeGradientColor = NULL;			// 투명도 속성 그라데이션 색상
	m_pRibbonDrawShapeTextKind = NULL;				// 투명도 속성 폰트 종류
	m_pRibbonDrawShapeTextSize = NULL;				// 투명도 속성 폰트 크기
	m_pRibbonDrawShapeTextColor = NULL;				// 투명도 속성 폰트 색상
	m_pRibbonDrawGallery = NULL;					// 투명도 도형 그리기
	m_pRibbonDrawShapeTextString = NULL;			// 텍스트 내용
	m_pRibbonDrawShapeTextString = NULL;			// 텍스트 줄간격

	//m_indexMap = NULL;
}

CMainFrame::~CMainFrame()
{
	//if (m_indexMap)
	//{
	//	delete m_indexMap;
	//	m_indexMap = NULL;
	//}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);

	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, _T(" Lon:000° 00' 00.000, Lat: 00° 00' 00.000 (X:000.000000, Y:00.000000) \
							| MGRS : 00ABC 00000 00000 | UTM : 00A 00000 00000 | GEOREF : ABCD0000000000 | GARS : 000AB00 | Alt: 000m"), TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(59220, _T("Scale 1:000,000,000"), TRUE), _T("Scale"));
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);
	
	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// 캡션 표시줄을 만듭니다.
	if (!CreateCaptionBar())
	{
		TRACE0("캡션 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// Outlook 표시줄이 만들어지고 왼쪽에 도킹할 수 있도록 허용해야 합니다.
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
		return -1;
	}

	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCOPTypeView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCOPItemView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCOPLayerView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCOPTypeView);

	CDockablePane* pTabbedBar = nullptr; 
	m_wndCOPItemView.AttachToTabWnd(&m_wndCOPTypeView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndCOPLayerView.AttachToTabWnd(&m_wndCOPTypeView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndCOPLayerView.AttachToTabWnd(&m_wndCOPTypeView, DM_SHOW, TRUE, &pTabbedBar);
	//m_wndClassView.AttachToTabWnd(&m_wndCOPLayerView, DM_SHOW, TRUE, &pTabbedBar);
	
	//m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndOutput);


	m_wndMapObjectManager.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndOverlayObjectView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMapObjectManager);
	m_wndIndexMapView.EnableDocking(CBRS_ALIGN_ANY);	// 인덱스 지도 뷰
	m_wndIndexMapView.DockToWindow(&m_wndMapObjectManager, CBRS_BOTTOM);
	//m_wndOverlayObjectView.AttachToTabWnd(&m_wndMapObjectManager, DM_SHOW, FALSE, &pTabbedBar);

	m_wndMapProperties.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndOverlayProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMapProperties);
	//m_wndOverlayProperties.AttachToTabWnd(&m_wndMapProperties, DM_SHOW, TRUE, &pTabbedBar);

	if (m_wndIndexMapView.GetSafeHwnd() != NULL)
	{
		//m_indexMap = new CShapeT();

#if C4MAP_ENABLE_LOCAL
        m_indexMap.OpenShapeFile();
        CString localMessage;
        CString style=_T("D:\\map\\styles\\cop_land_100_unix.xml");
        if(GetFileAttributes(style)==INVALID_FILE_ATTRIBUTES)style.Empty();
        m_indexMap.LoadLocalCatalog(_T("D:/map/shp"),localMessage);
        OutputDebugString(localMessage+_T("\n"));
#endif
        TCHAR path[MAX_PATH]={0};
		GetModuleFileName(nullptr,path,MAX_PATH);
		CString catalog(path);
		catalog=catalog.Left(catalog.ReverseFind('\\')+1)+_T("map-sources.xml");
		CString sourceError;MapSources().Load(catalog,sourceError);
        m_wndMapObjectManager.RefreshLocalShapeTree();
	}

	// 보관된 값에 따라 비주얼 관리자 및 스타일을 설정합니다.
	OnApplicationLook(theApp.m_nAppLook);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서-
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;
	cs.style &= ~FWS_PREFIXTITLE;

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// 색상정보 생성
	CreateDocumentColors();

	// Tmp Object
	CString strCategoryName;
	CMFCRibbonCategory* pCategory = NULL;
	CMFCRibbonPanel* pPanel = NULL;
	CMFCRibbonButtonsGroup* pButtonsGroup = NULL;
	CMFCRibbonButton* pButton = NULL;
	int nTmp = 0;

	// 창 이미지를 로드합니다.
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// 초기화 주 단추입니다.
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize(45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
	pBtnPrint->SetKeys(_T("p"), _T("w"));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_LABEL);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_QUICK);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_PREVIEW);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_SETUP);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
	pMainPanel->Add(pBtnPrint);
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
	ASSERT(bNameValid);
	pMainPanel->AddRecentFilesList(strTemp);

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

	//////////////////////////////////////////////////////////////////////////
	// 지도 범주
	strCategoryName = _T("지도");
	pCategory = m_wndRibbonBar.AddCategory(strCategoryName, IDB_PNG_RBAR_SCREEN_S, IDB_PNG_RBAR_SCREEN_L);
	AddRibbonCategory(strCategoryName, pCategory);

	pPanel = pCategory->AddPanel(_T("컨트롤"));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MOUSE_BASE, DEF_RBAR_SCREEN_PANEL1_CMD1, 0, 0));		// "기본"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MOUSE_ZOOMIN, DEF_RBAR_SCREEN_PANEL1_CMD2, 1, 1));		// "확대"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MOUSE_ZOOMOUT, DEF_RBAR_SCREEN_PANEL1_CMD3, 2, 2));	// "축소"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MOUSE_MOVE, DEF_RBAR_SCREEN_PANEL1_CMD4, 3, 3));		// "이동"
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_MENU_COORDINATE_MOVE, DEF_RBAR_SCREEN_PANEL1_CMD5, 4, 4));	// "좌표 이동"
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MAP_UNDO, DEF_RBAR_SCREEN_PANEL1_CMD6, 5));			// "이전 화면"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MAP_REDO, DEF_RBAR_SCREEN_PANEL1_CMD7, 6));			// "다음 화면"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MAP_INIT, DEF_RBAR_SCREEN_PANEL1_CMD8, 7));			// "초기 화면"

	pPanel = pCategory->AddPanel(_T("설정"));	// "지도 설정"
	// "축척" (단일 창을 쓸 경우 사용)
	//CMFCRibbonButton* pRibbonMapScale = new CMFCRibbonButton(ID_MENU_MAP_SCALE, DEF_RBAR_SCREEN_PANEL2_CMD2, 9, 9);
	//pPanelMapSetting->Add(pRibbonMapScale);
	std::auto_ptr<CMFCRibbonButtonsGroup> apScaleGroup(new CMFCRibbonButtonsGroup);
	CMFCRibbonLabel* pBtnMapScaleInfoLabel = new CMFCRibbonLabel(_T("축 적  범 위"));
	apScaleGroup->AddButton(pBtnMapScaleInfoLabel);
	m_pRibbonMapMinScale = new CMFCRibbonEdit(ID_MENU_MAP_SCALE_MIN, 40);

	apScaleGroup->AddButton(m_pRibbonMapMinScale);
	CMFCRibbonLabel* pBtnMapScaleInfoLabel2 = new CMFCRibbonLabel("~");
	apScaleGroup->AddButton(pBtnMapScaleInfoLabel2);
	m_pRibbonMapMaxScale = new CMFCRibbonEdit(ID_MENU_MAP_SCALE_MAX, 60);

	apScaleGroup->AddButton(m_pRibbonMapMaxScale);
	pPanel->Add(apScaleGroup.release());
	pPanel->Add(m_pRibbonMapZoomScale = new CMFCRibbonEdit(ID_MENU_MAP_SCALE_ZOOM, 112, _T("스크롤축척")));
	pPanel->Add(m_pRibbonMapCurrentScale = new CMFCRibbonEdit(ID_MENU_MAP_SCALE_CURRENT, 112, _T("현 재  축 적")));
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MAP_ROTATE, DEF_RBAR_SCREEN_PANEL2_CMD3, 10, 10));		// "회전"
	pButton = new CMFCRibbonButton(ID_MENU_PROJECTION, DEF_RBAR_SCREEN_PANEL2_CMD4, 11, 11);		// "투영"
	pButton->SetDefaultCommand(FALSE);
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_GEO, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB1));	// "Geodetic"
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_UTM, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB2));	// "UTM"
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_MER, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB3));	// "Mercator"
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_LCC, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB4));	// "Lambert Conformal Conic"
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_STE, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB5));	// "StereoGraphic"
	pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_EQU, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB6));	// "EQUIDISTANTCYLINDRICAL"
	//pButton->AddSubItem(new CMFCRibbonButton(ID_MENU_PROJECTION_TM, DEF_RBAR_SCREEN_PANEL2_CMD4_SUB7));	// "TM"
	pPanel->Add(pButton);
	pPanel->Add(new CMFCRibbonButton(ID_MENU_COORDGRID_SET, DEF_RBAR_VIEW_PANEL1_CMD1, 0, 0));	// 그리드
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_MENU_MAP_SELECT, DEF_RBAR_SCREEN_PANEL2_CMD1, 8, 8));	// "선택"

	// "밝기 조절"
	pPanel = pCategory->AddPanel(DEF_RBAR_VIEW_PANEL3);
	pPanel->Add(new CMFCRibbonButton(ID_MENU_BRICON_INIT, _T("초기화"), 2, 2));		// "기본값"
	pPanel->AddSeparator();
	pPanel->SetCenterColumnVert();

	pButtonsGroup = new CMFCRibbonButtonsGroup;
	pButtonsGroup->AddButton(new CMFCRibbonLabel(DEF_RBAR_VIEW_PANEL3_CMD1));		// "밝기:"
	m_pRibbonMapBrightSlider = new CMFCRibbonSlider(ID_MENU_BRIGHT_SLIDER, 70);
	m_pRibbonMapBrightSlider->SetZoomButtons();
	m_pRibbonMapBrightSlider->SetRange(-100, 100);
	m_pRibbonMapBrightSlider->SetPos(0);
	pButtonsGroup->AddButton(m_pRibbonMapBrightSlider);
	m_pRibbonMapBrightEdit = new CMFCRibbonEdit(ID_MENU_BRIGHT_EDIT, 30);
	CString strBrightness;
	strBrightness.Format("%d%%", m_pRibbonMapBrightSlider->GetPos());
	m_pRibbonMapBrightEdit->SetEditText(strBrightness);
	pButtonsGroup->AddButton(m_pRibbonMapBrightEdit);
	pPanel->Add(pButtonsGroup);

	pButtonsGroup = new CMFCRibbonButtonsGroup;
	pButtonsGroup->AddButton(new CMFCRibbonLabel(DEF_RBAR_VIEW_PANEL3_CMD2));		// "명암:"
	m_pRibbonMapContrastSlider = new CMFCRibbonSlider(ID_MENU_CONTRAST_SLIDER, 70);
	m_pRibbonMapContrastSlider->SetZoomButtons();
	m_pRibbonMapContrastSlider->SetRange(-100, 100);
	m_pRibbonMapContrastSlider->SetPos(0);
	pButtonsGroup->AddButton(m_pRibbonMapContrastSlider);
	m_pRibbonMapContrastEdit = new CMFCRibbonEdit(ID_MENU_CONTRAST_EDIT, 30);
	CString strContrast;
	strContrast.Format("%d%%", m_pRibbonMapContrastSlider->GetPos());
	m_pRibbonMapContrastEdit->SetEditText(strContrast);
	pButtonsGroup->AddButton(m_pRibbonMapContrastEdit);
	pPanel->Add(pButtonsGroup);

	// 검색
	pPanel = pCategory->AddPanel(_T("검색"));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SEARCH_CLEAR, DEF_RBAR_SEARCH_PANEL1_CMD1, 0, 0));	// "초기화"
	pPanel->AddSeparator();
	pPanel->Add(m_pRibbonSearchAreaTarget = new CMFCRibbonComboBox(ID_MENU_SEARCH_AREA_TARGET, FALSE, 80, DEF_RBAR_SEARCH_PANEL3_CMD1));	// "검색 레이어"
	pPanel->Add(m_pRibbonSearchMapField = new CMFCRibbonComboBox(ID_MENU_SEARCH_MAP_FIELD, FALSE, 92, DEF_RBAR_SEARCH_PANEL4_CMD2));		// "검색 필드"
	pPanel->Add(m_pRibbonSearchAreaColor = new CMFCRibbonColorButton(ID_MENU_SEARCH_AREA_COLOR, DEF_RBAR_SEARCH_PANEL3_CMD3, 2));			// "결과 색상"
	SetRibbonColorButton(m_pRibbonSearchAreaColor, DEF_COLOR_RED);
	pPanel->AddSeparator();
	pPanel->Add(m_pRibbonSearchAreaType = new CMFCRibbonComboBox(ID_MENU_SEARCH_AREA_SHAPE, FALSE, 92, _T("")));	// "검색 형태"
	m_pRibbonSearchAreaType->AddItem(DEF_RBAR_SEARCH_PANEL3_CMD2_SUB1);		// "사각형"
	m_pRibbonSearchAreaType->AddItem(DEF_RBAR_SEARCH_PANEL3_CMD2_SUB2);		// "원"
	m_pRibbonSearchAreaType->SelectItem(0);
	pPanel->Add(m_pRibbonSearchMapInput = new CMFCRibbonEdit(ID_MENU_SEARCH_MAP_INPUT, 108, _T("")));	// "검색 어"
	m_pRibbonSearchMapInput->SetEditText(_T("검색어 입력"));
	/*m_pRibbonSearchMapInput->EnableSpinButtons(0,99);
	m_pRibbonSearchMapInput->SetEditText("검색어 입력");*/
	pPanel->Add(new CMFCRibbonLabel(_T("")));
	pPanel->Add(new CMFCRibbonLabel(_T("")));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SEARCH_AREA, _T("영역"), 2));	// "검색영역 선택"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SEARCH_MAP, _T("속성"), 3));	// "검색"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SEARCH_POINT, _T("클릭"), 1));	// "속성 조회"

	// "즐겨찾기"
	pPanel = pCategory->AddPanel(DEF_RBAR_SCREEN_PANEL3);
	pPanel->Add((m_pRibbonMapBookMarkCombo = new CMFCRibbonComboBox(ID_MENU_BOOKMARK, FALSE, 50))); // 1~9 < 9이하만 입력 가능 >
	pButtonsGroup = new CMFCRibbonButtonsGroup;
	pButtonsGroup->AddButton(new CMFCRibbonButton(ID_MENU_ADD_BOOKMARK, DEF_RBAR_SCREEN_PANEL3_CMD2, 1));
	pButtonsGroup->AddButton(new CMFCRibbonButton(ID_MENU_MOVE_BOOKMARK, DEF_RBAR_SCREEN_PANEL3_CMD2, 2));
	pButtonsGroup->AddButton(new CMFCRibbonButton(ID_MENU_DELETE_BOOKMARK, DEF_RBAR_SCREEN_PANEL3_CMD3, 3));
	pPanel->Add(pButtonsGroup);

	// 저장
	pPanel = pCategory->AddPanel(_T("이미지"));	// "저장"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SAVE_CLIPBOARD, _T("클립 보드"), 3, 3));	// "클립 보드"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_SAVE_IMAGEFILE, _T("파일 저장"), 4, 4));	// "이미지"

	// 보기
	pPanel = pCategory->AddPanel(_T("보기"));
	pPanel->Add(m_pRibbonMapMgrOnOff = new CMFCRibbonCheckBox(ID_MENU_MAPMANAGER_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD1));		// 지도레이어 관리
	pPanel->Add(m_pRibbonMapPropertyOnOff = new CMFCRibbonCheckBox(ID_MENU_MAPPROP_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD3));		// "지도레이어 속성"
	pPanel->Add(m_pRibbonIndexMapOnOff = new CMFCRibbonCheckBox(ID_MENU_INDEXMAP_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD5));		// "인덱스 지도"


	//////////////////////////////////////////////////////////////////////////
	// 상황도 범주
	strCategoryName = _T("상황도");
	pCategory = m_wndRibbonBar.AddCategory(strCategoryName, IDB_FILESMALL, IDB_PNG_3DRIBBON);
	AddRibbonCategory(strCategoryName, pCategory);
	// 컨트롤
	pPanel = pCategory->AddPanel(_T("컨트롤"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SELFREE, _T("선택해제"), DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SHOW, _T("감추기 항목 보기"), DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(m_pwndCOPImportantPatrol = new CMFCRibbonButton(ID_RIBBON_COP_IMPO_PATROL, _T("중요요소 순회"), DEF_OF_3DRIBBON_IMG_1MASH));

	// 부호
	pPanel = pCategory->AddPanel(_T("부호"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_DEFAULT, _T("초기화"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->SetCenterColumnVert();
	pButtonsGroup = new CMFCRibbonButtonsGroup;
	pButtonsGroup->AddButton(new CMFCRibbonLabel(_T("크기")));	// "크기:"
	m_pRibbonCOPItemSizeSlider = new CMFCRibbonSlider(ID_RIBBON_COP_SYM_ZOOM_SLIDER);
	//m_pRibbonCOPItemSizeSlider->SetZoomButtons();
	m_pRibbonCOPItemSizeSlider->SetRange(0, 50);
	m_pRibbonCOPItemSizeSlider->SetPos(7);
	m_pRibbonCOPItemSizeSlider->SetText(_T("크기"));
	pButtonsGroup->AddButton(m_pRibbonCOPItemSizeSlider);
	m_pRibbonCOPItemSizeSliderEdit = new CMFCRibbonEdit(ID_RIBBON_COP_SYM_ZOOM_SLIDER_EDIT, 30);
	strBrightness.Format("%d px", m_pRibbonCOPItemSizeSlider->GetPos());
	m_pRibbonCOPItemSizeSliderEdit->SetEditText(strBrightness);
	pButtonsGroup->AddButton(m_pRibbonCOPItemSizeSliderEdit);
	pPanel->Add(pButtonsGroup);

	pButtonsGroup = new CMFCRibbonButtonsGroup;
	pButtonsGroup->AddButton(new CMFCRibbonLabel(_T("밝기")));	// "밝기"	
	m_pRibbonCOPItemBrightSlider = new CMFCRibbonSlider(ID_RIBBON_COP_SYM_BRIGHT_SLIDER);
	//m_pRibbonCOPItemBrightSlider->SetZoomButtons();
	m_pRibbonCOPItemBrightSlider->SetRange(-64, +64);
	m_pRibbonCOPItemBrightSlider->SetPos(0);
	pButtonsGroup->AddButton(m_pRibbonCOPItemBrightSlider);
	m_pRibbonCOPItemBrightSliderEdit = new CMFCRibbonEdit(ID_RIBBON_COP_SYM_BRIGHT_EDIT, 30);
	strContrast.Format("%d%%", m_pRibbonCOPItemBrightSlider->GetPos());
	m_pRibbonCOPItemBrightSliderEdit->SetEditText(strContrast);
	pButtonsGroup->AddButton(m_pRibbonCOPItemBrightSliderEdit);
	pPanel->Add(pButtonsGroup);

	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_SIMPLE, _T("간략화"), 0));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_AUTOSIZE, _T("자동크기"), 1));
	pPanel->Add(m_pRibbonCOPItemColor = new CMFCRibbonColorButton(ID_RIBBON_COP_SYM_COLOR, _T("색상"), 2));
	SetRibbonColorButton(m_pRibbonCOPItemColor, DEF_COLOR_RED);
	pPanel->AddSeparator();
	pButton = new CMFCRibbonButton(ID_RIBBON_COP_SYM_FILLWATER_PWD, _T("물채움"), 3);				// "도시"
	pButton->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_FILLWATER_PWD, _T("전투력"), 3));	// 음영도
	pButton->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_FILLWATER_IFF, _T("피아색상"), 4));	// 색상도
	pPanel->Add(pButton);

	// 정보
	pPanel = pCategory->AddPanel(_T("정보"));
	CMFCRibbonButton* pBtn3 = new CMFCRibbonButton(ID_RIBBON_COP_SYM_INFO_HIDE, _T("보기 감추기"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH);
	//pBtn3->SetMenu(IDR_RIBBON_MENU_1, TRUE);
	pBtn3->SetAlwaysLargeImage();
	pBtn3->RemoveSubItem(0);
	pBtn3->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_INFO_HIDE, _T("감추기"), 0), 0);
	pBtn3->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_INFO_ADD, _T("추가정보"), 1), 1);
	pBtn3->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_INFO_HISTORY, _T("이력정보"), 2), 2);
	pBtn3->AddSubItem(new CMFCRibbonButton(ID_RIBBON_COP_SYM_INFO_ABILITY, _T("능력도"), 3), 3);
	pPanel->Add(pBtn3);

	// 검색
	pPanel = pCategory->AddPanel(_T("검색"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SEARCH_CLEAR, DEF_RBAR_SEARCH_PANEL1_CMD1, 0, 0));	// "초기화"
	pPanel->AddSeparator();
	pPanel->Add(m_pRibbonCOPSearchLayer = new CMFCRibbonComboBox(ID_RIBBON_COP_SEARCH_LAYER, FALSE, 80, DEF_RBAR_SEARCH_PANEL3_CMD1));	// "검색 레이어"
	pPanel->Add(m_pRibbonCOPSearchColumn = new CMFCRibbonComboBox(ID_RIBBON_COP_SEARCH_FIELD, FALSE, 92, DEF_RBAR_SEARCH_PANEL4_CMD2));	// "검색 필드"
	pPanel->Add(m_pRibbonCOPSearchColor = new CMFCRibbonColorButton(ID_RIBBON_COP_SEARCH_COLOR, DEF_RBAR_SEARCH_PANEL3_CMD3, 2));		// "결과 색상"
	SetRibbonColorButton(m_pRibbonCOPSearchColor, DEF_COLOR_RED);
	pPanel->AddSeparator();
	pPanel->Add(m_pRibbonCOPSearchType = new CMFCRibbonComboBox(ID_RIBBON_COP_SEARCH_TYPE, FALSE, 92, _T("")));	// "검색 형태"	
	m_pRibbonCOPSearchType->AddItem(DEF_RBAR_SEARCH_PANEL3_CMD2_SUB1); // "사각형"
	m_pRibbonCOPSearchType->AddItem(DEF_RBAR_SEARCH_PANEL3_CMD2_SUB2); // "원"
	m_pRibbonCOPSearchType->SelectItem(0);
	pPanel->Add(m_pRibbonCOPSearchWord = new CMFCRibbonEdit(ID_RIBBON_COP_SEARCH_WORD, 108, _T("")));	// "검색 어"
	m_pRibbonCOPSearchWord->SetEditText(_T("검색어 입력"));

	pPanel->Add(new CMFCRibbonLabel(_T("")));
	pPanel->Add(new CMFCRibbonLabel(_T("")));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SEARCH_AREA, _T("영역"), 2));	// "검색영역 선택"
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SEARCH_ATTR, _T("속성"), 3));	// "검색"

	// 갱신
	pPanel = pCategory->AddPanel(_T("갱신"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_REFASH, _T("갱신"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));

	// 변환
	pPanel = pCategory->AddPanel(_T("변환"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_OVERLAY_NEW, _T("신규 투명도"), DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_SYM_OVERLAY_INSERT, _T("투명도 추가"), DEF_OF_3DRIBBON_IMG_1MASH));

	// 보기
	pPanel = pCategory->AddPanel(_T("보기"));
	pPanel->Add(m_pwndCOPLayerViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_LAYERLIST, _T("레이어")));
	pPanel->Add(m_pwndCOPItemViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_ITEMLIST, _T("도시요소")));
	pPanel->Add(m_pwndCOPTypeViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_TYPELIST, _T("유형정보")));
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_IMPO_LIST, _T("중요요소"), 4));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_ALWS_LIST, _T("상시시현"), 5));


	//////////////////////////////////////////////////////////////////////////
	// 투명도 범주
	strCategoryName = _T("투명도");
	pCategory = m_wndRibbonBar.AddCategory(strCategoryName, IDB_PNG_RBAR_DRAW_S, IDB_PNG_RBAR_DRAW_L);
	AddRibbonCategory(strCategoryName, pCategory);

	// "투명도 레이어"
	pPanel = pCategory->AddPanel(_T("레이어"));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_OVERLAY_NEW, DEF_RBAR_DRAW_PANEL1_CMD1, 0, 0));	// "생성"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_OVERLAY_OPEN, DEF_RBAR_DRAW_PANEL1_CMD2, 1, 1));	// "열기"
	//pPanelShapeLayer->Add(new CMFCRibbonButton(ID_MENU_OVERLAY_SAVE, DEF_RBAR_DRAW_PANEL1_CMD3, 2, 2));	// "저장"	// View에서 선택 저장
	//pPanelShapeLayer->Add(new CMFCRibbonButton(ID_MENU_OVERLAY_DELETE, DEF_RBAR_DRAW_PANEL1_CMD4, 3, 3));	// "삭제"	// View에서 선택 삭제

	// 편집
	pPanel = pCategory->AddPanel(_T("편집"));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_OVERLAY_GRADATION, DEF_RBAR_DRAW_PANEL1_CMD5, 4, 4));	// "눈금 설정"
/*	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(OV_EDIT_UNDO, DEF_RBAR_DRAW_PANEL1_CMD6, 5, 5));		// Undo
	pPanel->Add(new CMFCRibbonButton(OV_EDIT_REDO, DEF_RBAR_DRAW_PANEL1_CMD7, 6, 6));		// Redo
*/
	// "그리기"
	pPanel = pCategory->AddPanel(_T("그리기"));

	m_pRibbonDrawGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_DRAW_GALLERY, _T("도형"), -1, 0);

	CMFCToolBarImages mtbi_Recently;
	CBitmap bmRecently;

	m_mtbi_Line.Load(IDB_OVERLAY_DRAW_LINE);
	m_mtbi_Line.SetImageSize(CSize(24, 24), FALSE);
	m_mtbi_Line.SetTransparentColor(RGB(220, 233, 250));
	m_mtbi_Rect.Load(IDB_OVERLAY_DRAW_RECT);
	m_mtbi_Rect.SetImageSize(CSize(24, 24), FALSE);
	m_mtbi_Rect.SetTransparentColor(RGB(220, 233, 250));
	m_mtbi_default.Load(IDB_OVERLAY_DRAW_DEFAULT);
	m_mtbi_default.SetImageSize(CSize(24, 24), FALSE);
	m_mtbi_default.SetTransparentColor(RGB(220, 233, 250));

	int nIconRow = 3;
	int nIcon = m_nRibbonDrawGalleryRecently;
	int width = 24 * nIcon;
	int height = 24;
	int plane = 1;
	int bitpercolor = 24;

	int bitmapSize = width * height * bitpercolor;
	BYTE* lpBitdata = new BYTE[bitmapSize];

	memset(lpBitdata, 1, bitmapSize);
	bmRecently.CreateBitmap(width, height, plane, bitpercolor, lpBitdata);
	mtbi_Recently.SetImageSize(CSize(height, height), FALSE);
	mtbi_Recently.AddImage(bmRecently, FALSE);

	SetOverlayRecentlyDefDrawItem(mtbi_Recently, m_nRibbonDrawGalleryRecently);

	m_pRibbonDrawGallery->AddGroup(_T("최근에 사용한 도형"), mtbi_Recently);
	m_pRibbonDrawGallery->AddGroup(_T("선"), m_mtbi_Line);
	m_pRibbonDrawGallery->AddGroup(_T("사각형"), m_mtbi_Rect);
	m_pRibbonDrawGallery->AddGroup(_T("기본도형"), m_mtbi_default);

	m_pRibbonDrawGallery->SetIconsInRow(12);
	m_pRibbonDrawGallery->EnableMenuResize(TRUE, TRUE);
	m_pRibbonDrawGallery->SetButtonMode(TRUE);

	delete[] lpBitdata;

	pPanel->Add(m_pRibbonDrawGallery);

	//m_pRibbonSortGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_SORT_GALLERY, _T("정렬"), 3, 3, IDB_OVERLAY_DRAW_QUICKSTYLE, 64);
	//m_pRibbonSortGallery->AddGroup(_T("개체순서"), -1);
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(1, _T("맨 앞으로 가져오기")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(2, _T("맨 뒤로 보내기")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(3, _T("앞으로 가져오기")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(4, _T("뒤로 보내기")));
	//m_pRibbonSortGallery->AddGroup(_T("개체그룹"), -1);
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(5, _T("그룹")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(6, _T("그룹 해제")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(7, _T("재그룹")));
	//m_pRibbonSortGallery->AddGroup(_T("개체위치"), -1);
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(8, _T("맟춤")));
	//m_pRibbonSortGallery->AddSubItem(new CMFCRibbonButton(9, _T("회전")));
	//m_pRibbonSortGallery->SetButtonMode();
	//m_pRibbonSortGallery->EnableMenuResize(FALSE);
	//pPanel->Add(m_pRibbonSortGallery);

	m_pRibbonStyleGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_QUICKSTYLE_GALLERY, _T("빠른 스타일"), 3, 3, IDB_OVERLAY_DRAW_QUICKSTYLE, 64);
	m_pRibbonStyleGallery->SetButtonMode();
	m_pRibbonStyleGallery->SetIconsInRow(4);
	m_pRibbonStyleGallery->EnableMenuResize(TRUE);
	pPanel->Add(m_pRibbonStyleGallery);

	pPanel->Add(m_pRibbonDrawShapeFillColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_FILL_COLOR, _T("도형 채우기"), 30));
	m_pRibbonDrawShapeFillColor->EnableOtherButton(_T("다른 채우기 색(&M)"), _T("다른 채우기 색"));
	m_pRibbonDrawShapeFillColor->SetColor(DEF_COLOR_RED);
	m_pRibbonDrawShapeFillColor->SetColumns(10);
	m_pRibbonDrawShapeFillColor->SetColorBoxSize(CSize(17, 17));
	m_pRibbonDrawShapeFillColor->AddColorsGroup(_T("테마 색"), m_lstMainColors);
	m_pRibbonDrawShapeFillColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	m_pRibbonDrawShapeFillColor->AddColorsGroup(_T("표준 색"), m_lstStandardColors);
	m_pRibbonDrawShapeFillColor->EnableAutomaticButton(_T("채우기 없음(&N)"), (COLORREF)-1, TRUE, NULL, FALSE);
	m_pRibbonDrawShapeFillColor->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_IMAGE, _T("그림(&P)")));
	m_pRibbonGradationGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_GRADATION_GALLERY, _T("그라데이션(&G)"), 0, -1);
	m_pRibbonGradationGallery->SetButtonMode();
	m_pRibbonGradationGallery->SetIconsInRow(5);
	m_pRibbonGradationGallery->EnableMenuResize(TRUE);
	m_pRibbonGradationGallery->AddGroup(_T("그라데이션 없음"), mtbi_Recently);
	m_pRibbonGradationGallery->AddGroup(_T("밝은 그라데이션"), m_mtbi_Line);
	m_pRibbonGradationGallery->AddGroup(_T("어두운 그라데이션"), m_mtbi_Rect);
	m_pRibbonGradationGallery->AddSubItem(new CMFCRibbonButton(ID_RIBBON_OVERLAY_GRADATION_ETC_GALLERY, _T("기타 그라데이션(&M)")));
	m_pRibbonTextureGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_TEXTURE_GALLERY, _T("질감(&T)"), 3, 3, IDB_OVERLAY_DRAW_QUICKSTYLE, 64);
	m_pRibbonTextureGallery->SetButtonMode();
	m_pRibbonTextureGallery->SetIconsInRow(4);
	m_pRibbonTextureGallery->EnableMenuResize(TRUE);
	m_pRibbonTextureGallery->AddSubItem(new CMFCRibbonButton(ID_RIBBON_OVERLAY_TEXTURE_ETC_GALLERY, _T("기타 질감(&M)")));
	m_pRibbonPatternGallery = new CMFCRibbonGallery(ID_RIBBON_OVERLAY_PATTERN_GALLERY, _T("패턴"), 3, 3, IDB_OVERLAY_DRAW_QUICKSTYLE, 64);
	m_pRibbonPatternGallery->SetButtonMode();
	m_pRibbonPatternGallery->SetIconsInRow(4);
	m_pRibbonPatternGallery->EnableMenuResize(TRUE);
	m_pRibbonDrawShapeFillColor->AddSubItem(m_pRibbonGradationGallery);
	m_pRibbonDrawShapeFillColor->AddSubItem(m_pRibbonTextureGallery);
	m_pRibbonDrawShapeFillColor->AddSubItem(m_pRibbonPatternGallery);

	pPanel->Add(m_pRibbonDrawShapeLineColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_LINE_COLOR, _T("도형 윤곽선"), 27));
	m_pRibbonDrawShapeLineColor->EnableOtherButton(_T("다른 윤곽선 색(&M)"), _T("다른 윤곽선 색"));
	m_pRibbonDrawShapeLineColor->SetColor(DEF_COLOR_BLUE2);
	m_pRibbonDrawShapeLineColor->SetColumns(10);
	m_pRibbonDrawShapeLineColor->SetColorBoxSize(CSize(17, 17));
	m_pRibbonDrawShapeLineColor->AddColorsGroup(_T("테마 색"), m_lstMainColors);
	m_pRibbonDrawShapeLineColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	m_pRibbonDrawShapeLineColor->AddColorsGroup(_T("표준 색"), m_lstStandardColors);
	m_pRibbonDrawShapeLineColor->EnableAutomaticButton(_T("윤곽선 없음(&N)"), (COLORREF)-1, TRUE, NULL, FALSE);

	m_pRibbonDrawShapeLineWidth = new CMFCRibbonGallery(ID_MENU_OVERLAY_LINE_WIDTH_COLOR, _T("두께(&W)"), 0, -1);
	m_pRibbonDrawShapeLineWidth->SetButtonMode();
	m_pRibbonDrawShapeLineWidth->SetIconsInRow(1);
	m_pRibbonDrawShapeLineWidth->EnableMenuResize(TRUE);
	m_pRibbonDrawShapeLineWidth->SetAlwaysLargeImage(FALSE);
	mtbi_Recently.Clear();
	mtbi_Recently.Load(IDB_OVERLAY_DRAW_LINEWIDTH);
	mtbi_Recently.SetImageSize(CSize(155, 24), FALSE);
	m_pRibbonDrawShapeLineWidth->SetPalette(mtbi_Recently);
	m_pRibbonDrawShapeLineWidth->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_ETC_COLOR, _T("다른 선(&L)")));

	m_pRibbonDrawShapeLineDesh = new CMFCRibbonGallery(ID_MENU_OVERLAY_LINE_DESH_COLOR, _T("대시(&S)"), 0, -1);
	m_pRibbonDrawShapeLineDesh->SetButtonMode();
	m_pRibbonDrawShapeLineDesh->SetIconsInRow(1);
	m_pRibbonDrawShapeLineDesh->EnableMenuResize(TRUE);
	m_pRibbonDrawShapeLineDesh->SetAlwaysLargeImage(FALSE);
	mtbi_Recently.Clear();
	mtbi_Recently.Load(IDB_OVERLAY_DRAW_LINEDESH);
	mtbi_Recently.SetImageSize(CSize(155, 24), FALSE);
	m_pRibbonDrawShapeLineDesh->SetPalette(mtbi_Recently);
	m_pRibbonDrawShapeLineDesh->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_DESH_ETC_COLOR, _T("다른 선(&L)")));

	m_pRibbonDrawShapeLineBow = new CMFCRibbonGallery(ID_MENU_OVERLAY_LINE_BOW_COLOR, _T("화살표(&R)"), 0, -1);
	m_pRibbonDrawShapeLineBow->SetButtonMode();
	m_pRibbonDrawShapeLineBow->SetIconsInRow(1);
	m_pRibbonDrawShapeLineBow->EnableMenuResize(TRUE);
	m_pRibbonDrawShapeLineBow->SetAlwaysLargeImage(FALSE);
	mtbi_Recently.Clear();
	mtbi_Recently.Load(IDB_OVERLAY_DRAW_LINEBOW);
	mtbi_Recently.SetImageSize(CSize(155, 24), FALSE);
	m_pRibbonDrawShapeLineBow->SetPalette(mtbi_Recently);
	m_pRibbonDrawShapeLineBow->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_BOW_ETC_COLOR, _T("다른 화살표(&M)")));

	m_pRibbonDrawShapeLineColor->AddSubItem(m_pRibbonDrawShapeLineWidth);
	m_pRibbonDrawShapeLineColor->AddSubItem(m_pRibbonDrawShapeLineDesh);
	m_pRibbonDrawShapeLineColor->AddSubItem(m_pRibbonDrawShapeLineBow);

	// "속성"
	CMFCRibbonPanel* pPanelShapeProperty = pCategory->AddPanel(DEF_RBAR_DRAW_PANEL4);

	// "선색상"
	m_pRibbonDrawShapeLineColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_LINE_COLOR, DEF_RBAR_DRAW_PANEL4_CMD1, 27);
	SetRibbonColorButton(m_pRibbonDrawShapeLineColor, DEF_COLOR_BLUE);
	pPanelShapeProperty->Add(m_pRibbonDrawShapeLineColor);

	// "선굵기"
	CMFCRibbonButton* pBtnEditLineBorder = new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH, DEF_RBAR_DRAW_PANEL4_CMD4, 28);
	pBtnEditLineBorder->SetDefaultCommand(FALSE);
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_0, _T("0")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_1, _T("1")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_2, _T("2")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_3, _T("3")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_4, _T("4")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_5, _T("5")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_6, _T("6")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_7, _T("7")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_8, _T("8")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_9, _T("9")));
	pBtnEditLineBorder->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_WIDTH_10, _T("10")));
	pPanelShapeProperty->Add(pBtnEditLineBorder);
	
	// "점선모양"
	CMFCRibbonButton* pBtnEditLineType = new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE, DEF_RBAR_DRAW_PANEL4_CMD7, 29);
	pBtnEditLineType->SetDefaultCommand(FALSE);
	pBtnEditLineType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE_0, DEF_PROPERTY_LINESTYLE_SOLID));
	pBtnEditLineType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE_1, DEF_PROPERTY_LINESTYLE_DASH));
	pBtnEditLineType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE_2, DEF_PROPERTY_LINESTYLE_DOT));
	pBtnEditLineType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE_3, DEF_PROPERTY_LINESTYLE_DASHDOT));
	pBtnEditLineType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINE_STYLE_4, DEF_PROPERTY_LINESTYLE_DASHDOTDOT));
	pPanelShapeProperty->Add(pBtnEditLineType);

	//lwh2015/05/11 Start -> CMainFrame::InitializeRibbon => 선 추가 속성
	// 선치우침상태 => *안쪽으로 치우침 선택 시 겹선은 무시됩니다.
	CMFCRibbonButton* pBtnEditLineAlignment = new CMFCRibbonButton(ID_MENU_OVERLAY_LINEALIGNMENT_STYLE, _T("선치우침"), 29);
	pBtnEditLineAlignment->SetDefaultCommand(FALSE);
	pBtnEditLineAlignment->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEALIGNMENT_STYLE_0, DEF_PROPERTY_LINESTYLE_ALIGNMENT1));
	pBtnEditLineAlignment->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEALIGNMENT_STYLE_1, DEF_PROPERTY_LINESTYLE_ALIGNMENT2));
	pPanelShapeProperty->Add(pBtnEditLineAlignment);
	
	// 겹선종류
	CMFCRibbonButton* pBtnEditLineStripeType = new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE, _T("겹선종류"), 29);
	pBtnEditLineStripeType->SetDefaultCommand(FALSE);
	pBtnEditLineStripeType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE_0, DEF_PROPERTY_LINESTYLE_STRIPE1));
	pBtnEditLineStripeType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE_1, DEF_PROPERTY_LINESTYLE_STRIPE2));
	pBtnEditLineStripeType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE_2, DEF_PROPERTY_LINESTYLE_STRIPE3));
	pBtnEditLineStripeType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE_3, DEF_PROPERTY_LINESTYLE_STRIPE4));
	pBtnEditLineStripeType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINESTRIPE_STYLE_4, DEF_PROPERTY_LINESTYLE_STRIPE5));
	pPanelShapeProperty->Add(pBtnEditLineStripeType);

	// 화살표모양(시작점)
	CMFCRibbonButton* pBtnEditStartLineArrowType = new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE, _T("화살표(시작)"), 33);
	pBtnEditStartLineArrowType->SetDefaultCommand(FALSE);
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_0, DEF_PROPERTY_LINECAP_NONE));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_1, DEF_PROPERTY_LINECAP_ARROW));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_2, DEF_PROPERTY_LINECAP_ARROW_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_3, DEF_PROPERTY_LINECAP_ARROW_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_4, DEF_PROPERTY_LINECAP_TAIL));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_5, DEF_PROPERTY_LINECAP_TAIL_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_6, DEF_PROPERTY_LINECAP_TAIL_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_7, DEF_PROPERTY_LINECAP_TAIL_F));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_8, DEF_PROPERTY_LINECAP_TAIL_LF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_9, DEF_PROPERTY_LINECAP_TAIL_RF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_10, DEF_PROPERTY_LINECAP_TENT_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_11, DEF_PROPERTY_LINECAP_TENT_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_12, DEF_PROPERTY_LINECAP_TENT_LF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_13, DEF_PROPERTY_LINECAP_TENT_RF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_14, DEF_PROPERTY_LINECAP_SLASH_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_15, DEF_PROPERTY_LINECAP_SLASH_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_16, DEF_PROPERTY_LINECAP_CROSS));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_17, DEF_PROPERTY_LINECAP_TRI));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_18, DEF_PROPERTY_LINECAP_TRI_F));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_19, DEF_PROPERTY_LINECAP_TRI_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_20, DEF_PROPERTY_LINECAP_TRI_LF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_21, DEF_PROPERTY_LINECAP_TRI_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_22, DEF_PROPERTY_LINECAP_TRI_RF));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_23, DEF_PROPERTY_LINECAP_RECT));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_24, DEF_PROPERTY_LINECAP_RECT_F));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_25, DEF_PROPERTY_LINECAP_CIRCLE));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_26, DEF_PROPERTY_LINECAP_CIRCLE_F));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_27, DEF_PROPERTY_LINECAP_DIA));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_28, DEF_PROPERTY_LINECAP_DIA_F));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_29, DEF_PROPERTY_LINECAP_THICK));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_30, DEF_PROPERTY_LINECAP_DBLARROW));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_31, DEF_PROPERTY_LINECAP_DBLARROW_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_32, DEF_PROPERTY_LINECAP_DBLARROW_R));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_33, DEF_PROPERTY_LINECAP_DBLTAIL));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_34, DEF_PROPERTY_LINECAP_DBLTAIL_L));
	pBtnEditStartLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ARROW_STYLE_35, DEF_PROPERTY_LINECAP_DBLTAIL_R));
	pPanelShapeProperty->Add(pBtnEditStartLineArrowType);

	// 선끝모양
	CMFCRibbonButton* pBtnEditLineCapType = new CMFCRibbonButton(ID_MENU_OVERLAY_LINECAP_STYLE, _T("선끝모양"), 29);
	pBtnEditLineCapType->SetDefaultCommand(FALSE);
	pBtnEditLineCapType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINECAP_STYLE_0, DEF_PROPERTY_LINESTYLE_DASHCAP1));
	pBtnEditLineCapType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINECAP_STYLE_1, DEF_PROPERTY_LINESTYLE_DASHCAP2));
	pBtnEditLineCapType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINECAP_STYLE_2, DEF_PROPERTY_LINESTYLE_DASHCAP3));
	pPanelShapeProperty->Add(pBtnEditLineCapType);

	// 선연결모양(조인)
	CMFCRibbonButton* pBtnEditLineJoinType = new CMFCRibbonButton(ID_MENU_OVERLAY_LINEJOIN_STYLE, _T("선연결모양"), 29);
	pBtnEditLineJoinType->SetDefaultCommand(FALSE);
	pBtnEditLineJoinType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEJOIN_STYLE_0, DEF_PROPERTY_LINESTYLE_JOIN1));
	pBtnEditLineJoinType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEJOIN_STYLE_1, DEF_PROPERTY_LINESTYLE_JOIN2));
	pBtnEditLineJoinType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEJOIN_STYLE_2, DEF_PROPERTY_LINESTYLE_JOIN3));
	pBtnEditLineJoinType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_LINEJOIN_STYLE_3, DEF_PROPERTY_LINESTYLE_JOIN4));
	pPanelShapeProperty->Add(pBtnEditLineJoinType);

	// 화살표모양(끝점)
	CMFCRibbonButton* pBtnEditEndLineArrowType = new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE, _T("화살표(끝)"), 33);
	pBtnEditEndLineArrowType->SetDefaultCommand(FALSE);
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_0, DEF_PROPERTY_LINECAP_NONE));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_1, DEF_PROPERTY_LINECAP_ARROW));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_2, DEF_PROPERTY_LINECAP_ARROW_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_3, DEF_PROPERTY_LINECAP_ARROW_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_4, DEF_PROPERTY_LINECAP_TAIL));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_5, DEF_PROPERTY_LINECAP_TAIL_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_6, DEF_PROPERTY_LINECAP_TAIL_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_7, DEF_PROPERTY_LINECAP_TAIL_F));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_8, DEF_PROPERTY_LINECAP_TAIL_LF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_9, DEF_PROPERTY_LINECAP_TAIL_RF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_10, DEF_PROPERTY_LINECAP_TENT_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_11, DEF_PROPERTY_LINECAP_TENT_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_12, DEF_PROPERTY_LINECAP_TENT_LF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_13, DEF_PROPERTY_LINECAP_TENT_RF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_14, DEF_PROPERTY_LINECAP_SLASH_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_15, DEF_PROPERTY_LINECAP_SLASH_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_16, DEF_PROPERTY_LINECAP_CROSS));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_17, DEF_PROPERTY_LINECAP_TRI));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_18, DEF_PROPERTY_LINECAP_TRI_F));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_19, DEF_PROPERTY_LINECAP_TRI_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_20, DEF_PROPERTY_LINECAP_TRI_LF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_21, DEF_PROPERTY_LINECAP_TRI_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_22, DEF_PROPERTY_LINECAP_TRI_RF));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_23, DEF_PROPERTY_LINECAP_RECT));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_24, DEF_PROPERTY_LINECAP_RECT_F));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_25, DEF_PROPERTY_LINECAP_CIRCLE));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_26, DEF_PROPERTY_LINECAP_CIRCLE_F));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_27, DEF_PROPERTY_LINECAP_DIA));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_28, DEF_PROPERTY_LINECAP_DIA_F));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_29, DEF_PROPERTY_LINECAP_THICK));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_30, DEF_PROPERTY_LINECAP_DBLARROW));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_31, DEF_PROPERTY_LINECAP_DBLARROW_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_32, DEF_PROPERTY_LINECAP_DBLARROW_R));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_33, DEF_PROPERTY_LINECAP_DBLTAIL));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_34, DEF_PROPERTY_LINECAP_DBLTAIL_L));
	pBtnEditEndLineArrowType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_ENDARROW_STYLE_35, DEF_PROPERTY_LINECAP_DBLTAIL_R));
	pPanelShapeProperty->Add(pBtnEditEndLineArrowType);

	pPanelShapeProperty->AddSeparator();

	// "채움사용" => 채움방식
	CMFCRibbonButton* pBtnEditFillType = new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE, DEF_RBAR_DRAW_PANEL4_CMD5, 30);
	pBtnEditFillType->SetDefaultCommand(FALSE);
	pBtnEditFillType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE_0, DEF_PROPERTY_FILLTYLE_NONE));
	pBtnEditFillType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE_1, DEF_PROPERTY_FILLTYLE_SOLID));
	pBtnEditFillType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE_2, DEF_PROPERTY_FILLTYLE_HATCH));
	pBtnEditFillType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE_3, DEF_PROPERTY_FILLTYLE_PATHGRADIENT));
	pBtnEditFillType->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_FILL_TYPE_4, DEF_PROPERTY_FILLTYLE_LINEARGRADATION));
	pPanelShapeProperty->Add(pBtnEditFillType);
	
	// "채움색상"
	m_pRibbonDrawShapeFillColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_FILL_COLOR, DEF_RBAR_DRAW_PANEL4_CMD2, 30);
	SetRibbonColorButton(m_pRibbonDrawShapeFillColor, DEF_COLOR_BLUE2);
	pPanelShapeProperty->Add(m_pRibbonDrawShapeFillColor);
	
	// "반투명"
	CMFCRibbonButton* pRibbonGraphicFillTrance = new CMFCRibbonButton(ID_MENU_OVERLAY_TRANS_USE, DEF_RBAR_DRAW_PANEL4_CMD8, 32);
	pPanelShapeProperty->Add(pRibbonGraphicFillTrance);

	//Empty
	CMFCRibbonLabel* pEmptyLabel = new CMFCRibbonLabel(_T(" "));
	pPanelShapeProperty->Add(pEmptyLabel);
	
	// "패턴색상"
	m_pRibbonDrawShapePatternColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_PATTERN_COLOR, DEF_RBAR_DRAW_PANEL4_CMD3, 31);
	SetRibbonColorButton(m_pRibbonDrawShapePatternColor, DEF_COLOR_YELLOW);
	pPanelShapeProperty->Add(m_pRibbonDrawShapePatternColor);

	// "패턴모양" => 음영모양
	CMFCRibbonButton* pBtnEditPatternType = new CMFCRibbonButton(ID_MENU_OVERLAY_PATTERN_STYLE, DEF_RBAR_DRAW_PANEL4_CMD9, 33);
	pBtnEditPatternType->SetDefaultCommand(FALSE);
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_0, DEF_PROPERTY_HATCHSTYLE_0));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_1, DEF_PROPERTY_HATCHSTYLE_1));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_2, DEF_PROPERTY_HATCHSTYLE_2));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_3, DEF_PROPERTY_HATCHSTYLE_3));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_4, DEF_PROPERTY_HATCHSTYLE_4));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_5, DEF_PROPERTY_HATCHSTYLE_5));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_6, DEF_PROPERTY_HATCHSTYLE_6));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_7, DEF_PROPERTY_HATCHSTYLE_7));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_8, DEF_PROPERTY_HATCHSTYLE_8));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_9, DEF_PROPERTY_HATCHSTYLE_9));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_10, DEF_PROPERTY_HATCHSTYLE_10));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_11, DEF_PROPERTY_HATCHSTYLE_11));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_12, DEF_PROPERTY_HATCHSTYLE_12));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_13, DEF_PROPERTY_HATCHSTYLE_13));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_14, DEF_PROPERTY_HATCHSTYLE_14));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_15, DEF_PROPERTY_HATCHSTYLE_15));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_16, DEF_PROPERTY_HATCHSTYLE_16));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_17, DEF_PROPERTY_HATCHSTYLE_17));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_18, DEF_PROPERTY_HATCHSTYLE_18));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_19, DEF_PROPERTY_HATCHSTYLE_19));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_20, DEF_PROPERTY_HATCHSTYLE_20));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_21, DEF_PROPERTY_HATCHSTYLE_21));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_22, DEF_PROPERTY_HATCHSTYLE_22));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_23, DEF_PROPERTY_HATCHSTYLE_23));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_24, DEF_PROPERTY_HATCHSTYLE_24));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_25, DEF_PROPERTY_HATCHSTYLE_25));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_26, DEF_PROPERTY_HATCHSTYLE_26));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_27, DEF_PROPERTY_HATCHSTYLE_27));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_28, DEF_PROPERTY_HATCHSTYLE_28));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_29, DEF_PROPERTY_HATCHSTYLE_29));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_30, DEF_PROPERTY_HATCHSTYLE_30));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_31, DEF_PROPERTY_HATCHSTYLE_31));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_32, DEF_PROPERTY_HATCHSTYLE_32));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_33, DEF_PROPERTY_HATCHSTYLE_33));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_34, DEF_PROPERTY_HATCHSTYLE_34));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_35, DEF_PROPERTY_HATCHSTYLE_35));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_36, DEF_PROPERTY_HATCHSTYLE_36));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_37, DEF_PROPERTY_HATCHSTYLE_37));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_38, DEF_PROPERTY_HATCHSTYLE_38));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_39, DEF_PROPERTY_HATCHSTYLE_39));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_40, DEF_PROPERTY_HATCHSTYLE_40));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_41, DEF_PROPERTY_HATCHSTYLE_41));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_42, DEF_PROPERTY_HATCHSTYLE_42));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_43, DEF_PROPERTY_HATCHSTYLE_43));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_44, DEF_PROPERTY_HATCHSTYLE_44));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_45, DEF_PROPERTY_HATCHSTYLE_45));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_46, DEF_PROPERTY_HATCHSTYLE_46));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_47, DEF_PROPERTY_HATCHSTYLE_47));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_48, DEF_PROPERTY_HATCHSTYLE_48));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_49, DEF_PROPERTY_HATCHSTYLE_49));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_50, DEF_PROPERTY_HATCHSTYLE_50));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_51, DEF_PROPERTY_HATCHSTYLE_51));
	pBtnEditPatternType->AddSubItem(new	CMFCRibbonButton(ID_MENU_OVERLAY_HATCH_STYLE_52, DEF_PROPERTY_HATCHSTYLE_52));
	pPanelShapeProperty->Add(pBtnEditPatternType);

	//그라데이션 색상
	m_pRibbonDrawShapeGradientColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_GARDIENT_COLOR, "그라데이션색상", 31);
	SetRibbonColorButton(m_pRibbonDrawShapeGradientColor, DEF_COLOR_RED);
	pPanelShapeProperty->Add(m_pRibbonDrawShapeGradientColor);

	//그라데이션 위치
	CMFCRibbonButton* pRibbonGraphicFillGradientPos = new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_BLENDPOS, "그라데이션위치", 31);
	pPanelShapeProperty->Add(pRibbonGraphicFillGradientPos);

	//그라데이션 방향
	CMFCRibbonButton* pRibbonGraphicFillGradientMode = new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_LINERMODE, "그라데이션방향", 31);
	pRibbonGraphicFillGradientMode->SetDefaultCommand(FALSE);
	pRibbonGraphicFillGradientMode->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_LINERMODE_0, DEF_PROPERTY_LINEARGRD_HOLIZONTAL));
	pRibbonGraphicFillGradientMode->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_LINERMODE_1, DEF_PROPERTY_LINEARGRD_VERTICAL));
	pRibbonGraphicFillGradientMode->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_LINERMODE_2, DEF_PROPERTY_LINEARGRD_FORWARDDIAGONAL));
	pRibbonGraphicFillGradientMode->AddSubItem(new CMFCRibbonButton(ID_MENU_OVERLAY_GARDIENT_LINERMODE_3, DEF_PROPERTY_LINEARGRD_BACKWARDDIAGONAL));
	pPanelShapeProperty->Add(pRibbonGraphicFillGradientMode);

	// "글꼴"
	CMFCRibbonPanel* pPanelTextProperty = pCategory->AddPanel(DEF_RBAR_DRAW_PANEL5);

	// 내용
	pPanelTextProperty->Add(m_pRibbonDrawShapeTextString = new CMFCRibbonEdit(ID_MENU_OVERLAY_TEXT_STRING, 100, _T("내용")));

	//#define DEF_RBAR_DRAW_PANEL5_CMD1				"글씨체"
	CMFCRibbonFontComboBox::m_bDrawUsingFont = TRUE;
	m_pRibbonDrawShapeTextKind = new CMFCRibbonFontComboBox(ID_MENU_OVERLAY_TEXT_KIND);
	m_pRibbonDrawShapeTextKind->SetWidth(50, FALSE); // Width in "floaty" mode
	m_pRibbonDrawShapeTextKind->SelectItem(_T("굴림"));
	m_pRibbonDrawShapeTextKind->SetDropDownHeight(500);
	pPanelTextProperty->Add(m_pRibbonDrawShapeTextKind);

	// 도형 속성 창을 위한 폰트 종류 백업
	for (int i = 0; i < (int)m_pRibbonDrawShapeTextKind->GetCount(); i++)
		m_vSystemFontList.push_back(m_pRibbonDrawShapeTextKind->GetItem(i));

	//#define DEF_RBAR_DRAW_PANEL5_CMD2				"글크기"
	m_pRibbonDrawShapeTextSize = new CMFCRibbonComboBox(ID_MENU_OVERLAY_TEXT_SIZE, FALSE, 30);
	m_pRibbonDrawShapeTextSize->AddItem(_T("8"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("9"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("10"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("11"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("12"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("14"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("16"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("18"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("20"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("22"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("24"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("26"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("28"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("36"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("48"));
	m_pRibbonDrawShapeTextSize->AddItem(_T("72"));
	m_pRibbonDrawShapeTextSize->SetWidth(20, TRUE); // Width in "floaty" mode
	m_pRibbonDrawShapeTextSize->SelectItem(2);
	pPanelTextProperty->Add(m_pRibbonDrawShapeTextSize); //

	// 줄간격
	pPanelTextProperty->Add(m_pRibbonDrawShapeTextLineInterval = new CMFCRibbonEdit(ID_MENU_OVERLAY_TEXT_LINEINTERVAL, 50, _T("줄간격")));
	m_pRibbonDrawShapeTextLineInterval->EnableSpinButtons(-1000, 1000);
	m_pRibbonDrawShapeTextLineInterval->SetEditText(_T("100"));

	//#define DEF_RBAR_DRAW_PANEL5_CMD3				"글색상"
	m_pRibbonDrawShapeTextColor = new CMFCRibbonColorButton(ID_MENU_OVERLAY_TEXT_COLOR, "", 20);
	SetRibbonColorButton(m_pRibbonDrawShapeTextColor, DEF_COLOR_BLACK);
	pPanelTextProperty->Add(m_pRibbonDrawShapeTextColor);

	pPanelTextProperty->AddToolBar(IDR_FONT);
	pPanelTextProperty->AddToolBar(IDR_FONT_ALIGN);
	pPanelTextProperty->AddToolBar(IDR_FONT_ALIGNY);

	// "클립보드"
	CMFCRibbonPanel* pPanelShapeClipboard = pCategory->AddPanel(DEF_RBAR_DRAW_PANEL6);

	// "붙여넣기"
	CMFCRibbonButton* pRibbonPaste = new CMFCRibbonButton(ID_MENU_OVERLAY_PASTE, DEF_RBAR_DRAW_PANEL6_CMD1, 34, 34);
	pPanelShapeClipboard->Add(pRibbonPaste);

	// "자르기"
	CMFCRibbonButton* pRibbonCut = new CMFCRibbonButton(ID_MENU_OVERLAY_CUT, DEF_RBAR_DRAW_PANEL6_CMD2, 35);
	pPanelShapeClipboard->Add(pRibbonCut);

	// "복사"
	CMFCRibbonButton* pRibbonCopy = new CMFCRibbonButton(ID_MENU_OVERLAY_COPY, DEF_RBAR_DRAW_PANEL6_CMD3, 36);
	pPanelShapeClipboard->Add(pRibbonCopy);

	// 보기
	pPanel = pCategory->AddPanel(_T("보기"));
	pPanel->Add(m_pwndOverlayLayerViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_OVERLAY_LAYERLIST, _T("레이어")));
	pPanel->Add(m_pwndOverlayItemViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_OVERLAY_ITEMLIST, _T("도시요소")));


	//////////////////////////////////////////////////////////////////////////
	// 분석 범주
	strCategoryName = _T("분석");
	pCategory = m_wndRibbonBar.AddCategory(strCategoryName, IDB_PNG_RBAR_ANALYSIS_S, IDB_PNG_RBAR_ANALYSIS_L);
	AddRibbonCategory(strCategoryName, pCategory);

	// 초기화
	pPanel = pCategory->AddPanel(DEF_RBAR_ANALYSIS_PANEL1);
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_CLEAR, DEF_RBAR_ANALYSIS_PANEL1_CMD1, 0, 0));	// "초기화"

	// "기본"
	pPanel = pCategory->AddPanel(DEF_RBAR_ANALYSIS_PANEL2);
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_DISTANCE, DEF_RBAR_ANALYSIS_PANEL2_CMD1, 1, 1));// "거리 측정"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_AREA, DEF_RBAR_ANALYSIS_PANEL2_CMD2, 2, 2));// "면적 측정"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_SECTION, DEF_RBAR_ANALYSIS_PANEL2_CMD3, 3, 3));//"단면도 측정"

	// "가시권"
	pPanel = pCategory->AddPanel(DEF_RBAR_ANALYSIS_PANEL3);
	pPanel->Add(m_pRibbonAnalysisVisibleRange = new CMFCRibbonEdit(ID_MENU_ANALYSIS_VISIBLE_RANGE, 50, DEF_RBAR_ANALYSIS_PANEL3_CMD1));	// "관측 범위(m)"
	pPanel->Add(m_pRibbonAnalysisVisibleHeight = new CMFCRibbonEdit(ID_MENU_ANALYSIS_VISIBLE_HEIGHT, 50, DEF_RBAR_ANALYSIS_PANEL3_CMD2));	// "관측 높이(m)"
	pPanel->Add(m_pRibbonAnalysisVisibleColor = new CMFCRibbonColorButton(ID_MENU_ANALYSIS_VISIBLE_COLOR, DEF_RBAR_ANALYSIS_PANEL3_CMD3, 6));	// "결과 색상"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_VISIBLE_SELECT, DEF_RBAR_ANALYSIS_PANEL3_CMD4, 6, 6));	// "관측지점 선택"

	// "반경 표시"
	pPanel = pCategory->AddPanel(DEF_RBAR_ANALYSIS_PANEL4);
	pPanel->Add(m_pRibbonRadiusInterval = new CMFCRibbonEdit(ID_MENU_ANALYSIS_RADIUS_INTERVAL, 50, DEF_RBAR_ANALYSIS_PANEL4_CMD1));// "반경 간격(m)"
	pPanel->Add(m_pRibbonRadiusRange = new CMFCRibbonEdit(ID_MENU_ANALYSIS_RADIUS_RANGE, 50, DEF_RBAR_ANALYSIS_PANEL4_CMD2));	// "반경 범위(m)"
	pPanel->Add(new CMFCRibbonLabel(_T(" ")));
	pPanel->Add(m_pRibbonAnalysisRadiusWidth = new CMFCRibbonComboBox(ID_MENU_ANALYSIS_RADIUS_WIDTH, FALSE, 22, DEF_RBAR_ANALYSIS_PANEL4_CMD3));	// "선 두께"
	pPanel->Add(m_pRibbonAnalysisRadiusColor = new CMFCRibbonColorButton(ID_MENU_ANALYSIS_RADIUS_COLOR, DEF_RBAR_ANALYSIS_PANEL4_CMD4, 5));	// "선 색상"
	pPanel->Add(new CMFCRibbonLabel(_T(" ")));
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_RADIUS_SELECT, DEF_RBAR_ANALYSIS_PANEL4_CMD5, 4, 4));	// "반경지점 선택"
	pPanel->Add(new CMFCRibbonButton(ID_MENU_ANALYSIS_RADIUS_INPUT, DEF_RBAR_ANALYSIS_PANEL4_CMD6, 5, 5));// "반경지점 입력"

	SetRibbonColorButton(m_pRibbonAnalysisVisibleColor, RGB(255, 0, 0)); // DEF_COLOR_RED);
	m_pRibbonAnalysisVisibleHeight->SetEditText("20");	// 2m
	m_pRibbonAnalysisVisibleRange->SetEditText("1000");

	m_pRibbonRadiusInterval->SetEditText("200");	// 0.2km
	m_pRibbonRadiusRange->SetEditText("1000");	// 1km
	m_pRibbonAnalysisRadiusWidth->AddItem("1");
	m_pRibbonAnalysisRadiusWidth->AddItem("2");
	m_pRibbonAnalysisRadiusWidth->AddItem("3");
	m_pRibbonAnalysisRadiusWidth->AddItem("4");
	m_pRibbonAnalysisRadiusWidth->AddItem("5");
	m_pRibbonAnalysisRadiusWidth->AddItem("6");
	m_pRibbonAnalysisRadiusWidth->AddItem("7");
	m_pRibbonAnalysisRadiusWidth->AddItem("8");
	m_pRibbonAnalysisRadiusWidth->AddItem("9");
	m_pRibbonAnalysisRadiusWidth->AddItem("10");
	m_pRibbonAnalysisRadiusWidth->SelectItem("1");
	SetRibbonColorButton(m_pRibbonAnalysisRadiusColor, RGB(255, 0, 0)); // DEF_COLOR_RED);

	//////////////////////////////////////////////////////////////////////////
	// 보기 범주
	pCategory = m_wndRibbonBar.AddCategory(_T("보기"), IDB_WRITESMALL, IDB_WRITELARGE);
	// "지도"
	pPanel = pCategory->AddPanel(_T("지도"), m_PanelImages.ExtractIcon(7));
	pPanel->Add(m_pRibbonMapMgrOnOff = new CMFCRibbonCheckBox(ID_MENU_MAPMANAGER_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD1));// 지도레이어 관리
	pPanel->Add(m_pRibbonMapPropertyOnOff = new CMFCRibbonCheckBox(ID_MENU_MAPPROP_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD3));// "지도레이어 속성"
	pPanel->Add(m_pRibbonIndexMapOnOff = new CMFCRibbonCheckBox(ID_MENU_INDEXMAP_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD5));// "인덱스 지도"
	// 상황도
	pPanel = pCategory->AddPanel(_T("상황도"), m_PanelImages.ExtractIcon(7));
	pPanel->Add(m_pwndCOPLayerViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_LAYERLIST, _T("레이어")));
	pPanel->Add(m_pwndCOPItemViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_ITEMLIST, _T("도시요소")));
	pPanel->Add(m_pwndCOPTypeViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_COP_TYPELIST, _T("유형정보")));
	pPanel->AddSeparator();
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_IMPO_LIST, _T("중요요소"), 4));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_COP_ALWS_LIST, _T("상시시현"), 5));
	// 투명도
	pPanel = pCategory->AddPanel(_T("투명도"), m_PanelImages.ExtractIcon(7));
	pPanel->Add(m_pRibbonOverPropertyOnOff = new CMFCRibbonCheckBox(ID_MENU_OVERPROP_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD4));// "도형 속성"
	pPanel->Add(m_pRibbonOverlayOnOff = new CMFCRibbonCheckBox(ID_MENU_OVERVIEW_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD2));// "투명도 관리"
	pPanel->AddSeparator();
	pPanel->Add(m_pwndOverlayLayerViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_OVERLAY_LAYERLIST, _T("레이어")));
	pPanel->Add(m_pwndOverlayItemViewOnOff = new CMFCRibbonCheckBox(ID_RIBBON_OVERLAY_ITEMLIST, _T("도시요소")));


	// 프로그램
	pPanel = pCategory->AddPanel(_T("프로그램"), m_PanelImages.ExtractIcon(7));
	//pPanel->Add(m_pRibbonOutputOnOff = new CMFCRibbonCheckBox(ID_MENU_OUTPUT_ONOFF, DEF_RBAR_VIEW_PANEL6_CMD6));// "출력"
	pPanel->Add(new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, _T("상태바")));
	pPanel->Add(new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, _T("알림바")));

	//////////////////////////////////////////////////////////////////////////
	// Test 범주
	//////////////////////////////////////////////////////////////////////////
/*
	strCategoryName = _T("시험");
	pCategory = m_wndRibbonBar.AddCategory(strCategoryName, IDB_PNG_RBAR_ANALYSIS_S, IDB_PNG_RBAR_ANALYSIS_L);
	AddRibbonCategory(strCategoryName, pCategory);

	pPanel = pCategory->AddPanel(_T("입력"));
	//CMFCRibbonGallery* pBtn5 = new CMFCRibbonGallery(50000, _T("Groups && Subitems"), 4, 4);
	//pBtn5->AddGroup(_T("Group 1"), 50001, 64);
	//pBtn5->AddGroup(_T("Group 2"), 50002, 64);
	//pBtn5->SetButtonMode();
	//pBtn5->SetIconsInRow(4);
	//pBtn5->EnableMenuResize(TRUE);
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_AIR, _T("공중"), 1));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_SEA, _T("해상"), 2));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_MIL, _T("미사일"), 3));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_TBM, _T("탄도탄"), 4));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_NBC, _T("화생방"), 5));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_TIS, _T("차량위치"), 6));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PT, _T("점형"), 7));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PL, _T("선형"), 8));
	//pBtn5->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PG, _T("면형"), 9));
	//pPannel->Add(pBtn5);

	CMFCRibbonButton* pRibbonDataIn = new CMFCRibbonButton(ID_MENU_PROJECTION, _T("데이터 입력"), 11, 11);
	pRibbonDataIn->SetDefaultCommand(FALSE);
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_AIR, _T("공중"), 1));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_SEA, _T("해상"), 2));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_MIL, _T("미사일"), 3));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_TBM, _T("탄도탄"), 4));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_NBC, _T("화생방"), 5));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_TIS, _T("차량위치"), 6));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PT, _T("점형"), 7));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PL, _T("선형"), 8));
	pRibbonDataIn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_TEST_INDATA_PG, _T("면형"), 9));
	pPanel->Add(pRibbonDataIn);
	pPanel = pCategory->AddPanel(_T("삭제"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_DELDATA_ALL, _T("전체"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_DELDATA_EACH, _T("선택"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));


	pPanel = pCategory->AddPanel(_T("검색"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_AERA, _T("영역"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_HISTORY, _T("이력"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_AEARTRACE, _T("영역 추적"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_AEARHISTORY, _T("영역 이력"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_HISTORYTRACE, _T("이력 추적"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_SEARCHDATA_CLEAR, _T("검색 초기화"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));

	pPanel = pCategory->AddPanel(_T("파일"));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_FILEDATA_SAVE, _T("저장"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	pPanel->Add(new CMFCRibbonButton(ID_RIBBON_TEST_FILEDATA_OPEN, _T("열기"), DEF_OF_3DRIBBON_IMG_1MASH, DEF_OF_3DRIBBON_IMG_1MASH));
	//
*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  test end
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 탭의 오른쪽에 요소를 추가합니다.
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, strTemp, -1, -1);

	//pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE  기본값 없음 명령입니다. , TRUE  오른쪽 맞춤입니다.);

	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_TIP);
	ASSERT(bNameValid);
	pVisualStyleButton->SetToolTipText(strTemp);
	bNameValid = strTemp.LoadString(IDS_RIBBON_STYLE_DESC);
	ASSERT(bNameValid);
	pVisualStyleButton->SetDescription(strTemp);
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// 빠른 실행 도구 모음 명령을 추가합니다.
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon(0)));

}

void CMainFrame::AddRibbonCategory(CString strCategoryName, CMFCRibbonCategory* pCategory)
{
	m_mRibbonCategory.insert(std::pair<CString, CMFCRibbonCategory*>(strCategoryName, pCategory));
}

CMFCRibbonCategory* CMainFrame::GetRibbonCategory(CString strCategoryName)
{
	std::map<CString, CMFCRibbonCategory*>::iterator iter = m_mRibbonCategory.find(strCategoryName);
	if (iter != m_mRibbonCategory.end())
		return (*iter).second;
	return NULL;
}

void CMainFrame::SetRibbonColorButton(CMFCRibbonColorButton* pRibbonColorButton, COLORREF clrInit)
{
	//pRibbonColorButton->SetDefaultCommand(FALSE);
	//pRibbonColorButton->EnableAutomaticButton(_T("자동(&A)"), RGB(0, 0, 0));
	//pRibbonColorButton->EnableOtherButton(_T("다른 색(&M)..."), _T("More Shading Colors"));
	//pRibbonColorButton->SetColor(clrInit);
	//pRibbonColorButton->SetColumns(10);
	//pRibbonColorButton->AddColorsGroup(_T("테마 색"), m_lstMainColors);
	//pRibbonColorButton->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	//pRibbonColorButton->AddColorsGroup(_T("표준 색"), m_lstStandardColors);
	//pRibbonColorButton->SetColorBoxSize(CSize(17, 17));
	//pRibbonColorButton->SetButtonMode(FALSE);
	//pRibbonColorButton->EnableAutomaticButton(_T("빈색상(&N)"), -1, TRUE, NULL, FALSE /*Bottom*/);

	pRibbonColorButton->EnableAutomaticButton(_T("자동(&A)"), RGB(0, 0, 0));
	pRibbonColorButton->EnableOtherButton(_T("다른 색(&M)..."), _T("다른 색"));
	pRibbonColorButton->SetColor(clrInit);
	pRibbonColorButton->SetColumns(10);
	pRibbonColorButton->SetColorBoxSize(CSize(17, 17));
	pRibbonColorButton->AddColorsGroup(_T("테마 색"), m_lstMainColors);
	pRibbonColorButton->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	pRibbonColorButton->AddColorsGroup(_T("표준 색"), m_lstStandardColors);
}


BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 상황도 유형뷰를 만듭니다.
	CString strCOPTypeView;
	bNameValid = strCOPTypeView.LoadString(IDS_COPTYPE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndCOPTypeView.Create(strCOPTypeView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_COPTYPEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("상황도 유형뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 상황도 목록뷰를 만듭니다.
	CString strCOPListView;
	bNameValid = strCOPListView.LoadString(IDS_COPITEM_VIEW);
	ASSERT(bNameValid);
	if (!m_wndCOPItemView.Create(strCOPListView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_COPITEMVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("상황도 목록뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 상황도 레이어뷰를 만듭니다.
	CString strCOPLayerView;
	bNameValid = strCOPLayerView.LoadString(IDS_COPLAYER_VIEW);
	ASSERT(bNameValid);
	if (!m_wndCOPLayerView.Create(strCOPLayerView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_COPLAYERVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("상황도 레이어뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("출력 창을 만들지 못했습니다.\n");
	//	return FALSE; // 만들지 못했습니다.
	//}

	// 속성 창을 만듭니다.
	if (!m_wndMapProperties.Create(DEF_DOCKPANE_TITLE_MAPPROP, this, CRect(0, 0, 200, 200), TRUE, ID_MAPROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("지도레이어 속성 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	if (!m_wndIndexMapView.Create(DEF_DOCKPANE_TITLE_INDEXMAP, this, CRect(0, 0, 250, 200), TRUE, ID_INDEXMAP_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("인덱스 지도 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	if (!m_wndMapObjectManager.Create(DEF_DOCKPANE_TITLE_MAPMANAGER, this, CRect(0, 0, 200, 200), TRUE, ID_MAP_MANAGERWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("지도레이어 관리 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 클래스 뷰를 만듭니다.
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("클래스 뷰 창을 만들지 못했습니다.\n");
	//	return FALSE; // 만들지 못했습니다.
	//}

	// 파일 뷰를 만듭니다.
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("파일 뷰 창을 만들지 못했습니다.\n");
	//	return FALSE; // 만들지 못했습니다.
	//}

	// 출력 창을 만듭니다.
	//CString strOutputWnd;
	//bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	//ASSERT(bNameValid);
	//if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("출력 창을 만들지 못했습니다.\n");
	//	return FALSE; // 만들지 못했습니다.
	//}

	// 속성 창을 만듭니다.
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("속성 창을 만들지 못했습니다.\n");
	//	return FALSE; // 만들지 못했습니다.
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hCOPTypeViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_COPTYPE_VIEW_HC : IDI_COPTYPE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCOPTypeView.SetIcon(hCOPTypeViewIcon, FALSE);

	HICON hCOPItemViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_COPITEM_VIEW_HC : IDI_COPITEM_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCOPItemView.SetIcon(hCOPItemViewIcon, FALSE);

	HICON hCOPLayerViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_COPLAYER_VIEW_HC : IDI_COPLAYER_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCOPLayerView.SetIcon(hCOPLayerViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hMapObjectManageIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMapObjectManager.SetIcon(hMapObjectManageIcon, FALSE);

	HICON hMapPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMapProperties.SetIcon(hMapPropertiesBarIcon, FALSE);


	//HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	//HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("캡션 표시줄을 만들지 못했습니다.\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);

	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);

	//CC4IMapView* pView = DYNAMIC_DOWNCAST(CC4IMapView, GetActiveView());
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

// CAPTION BAR 사용 유무
void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}


void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}


void CMainFrame::CreateDocumentColors()
{
	typedef struct
	{
		COLORREF	color;
		TCHAR* szName;
	}
	ColorTableEntry;

	int i = 0;
	int nNumColours = 0;

	static ColorTableEntry colorsMain[] =
	{
		{	RGB(255, 255, 255),	_T("White, Background 1")		},
		{	RGB(0, 0, 0),			_T("Black, Text 1")			},
		{	RGB(238, 236, 225),	_T("Tan, Background 2")			},
		{	RGB(31, 73, 125),		_T("Dark Blue, Text 2")		},
		{	RGB(79, 129, 189),		_T("Blue, Accent 1")		},
		{	RGB(192, 80, 77),		_T("Red, Accent 2")			},
		{	RGB(155, 187, 89),		_T("Olive Green, Accent 3")	},
		{	RGB(128, 100, 162),	_T("Purple, Accent 4")			},
		{	RGB(75, 172, 198),		_T("Aqua, Accent 5")		},
		{	RGB(245, 150, 70),		_T("Orange, Accent 6")		}
	};

	static ColorTableEntry colorsAdditional[] =
	{
		{	RGB(242, 242, 242),	_T("White, Background 1, Darker 5%")	},
		{	RGB(127, 127, 127),	_T("Black, Text 1, Lighter 50%")		},
		{	RGB(214, 212, 202),	_T("Tan, Background 2, Darker 10%")		},
		{	RGB(210, 218, 229),	_T("Dark Blue, Text 2, Lighter 80%")	},
		{	RGB(217, 228, 240),	_T("Blue, Accent 1, Lighter 80%")		},
		{	RGB(244, 219, 218),	_T("Red, Accent 2, Lighter 80%")		},
		{	RGB(234, 241, 221),	_T("Olive Green, Accent 3, Lighter 80%")},
		{	RGB(229, 223, 235),	_T("Purple, Accent 4, Lighter 80%")		},
		{	RGB(216, 237, 242),	_T("Aqua, Accent 5, Lighter 80%")		},
		{	RGB(255, 234, 218),	_T("Orange, Accent 6, Lighter 80%")		},

		{	RGB(215, 215, 215),	_T("White, Background 1, Darker 15%")	},
		{	RGB(89, 89, 89),	_T("Black, Text 1, Lighter 35%")		},
		{	RGB(177, 176, 167),	_T("Tan, Background 2, Darker 25%")		},
		{	RGB(161, 180, 201),	_T("Dark Blue, Text 2, Lighter 60%")	},
		{	RGB(179, 202, 226),	_T("Blue, Accent 1, Lighter 60%")		},
		{	RGB(233, 184, 182),	_T("Red, Accent 2, Lighter 60%")		},
		{	RGB(213, 226, 188),	_T("Olive Green, Accent 3, Lighter 60%")},
		{	RGB(203, 191, 215),	_T("Purple, Accent 4, Lighter 60%")		},
		{	RGB(176, 220, 231),	_T("Aqua, Accent 5, Lighter 60%")		},
		{	RGB(255, 212, 181),	_T("Orange, Accent 6, Lighter 60%")		},

		{	RGB(190, 190, 190),	_T("White, Background 1, Darker 25%")	},
		{	RGB(65, 65, 65),		_T("Black, Text 1, Lighter 25%")	},
		{	RGB(118, 117, 112),	_T("Tan, Background 2, Darker 35%")		},
		{	RGB(115, 143, 175),	_T("Dark Blue, Text 2, Lighter 40%")	},
		{	RGB(143, 177, 213),	_T("Blue, Accent 1, Lighter 40%")		},
		{	RGB(222, 149, 147),	_T("Red, Accent 2, Lighter 40%")		},
		{	RGB(192, 213, 155),	_T("Olive Green, Accent 3, Lighter 40%")},
		{	RGB(177, 160, 197),	_T("Purple, Accent 4, Lighter 40%")		},
		{	RGB(137, 203, 218),	_T("Aqua, Accent 5, Lighter 40%")		},
		{	RGB(255, 191, 145),	_T("Orange, Accent 6, Lighter 40%")		},

		{	RGB(163, 163, 163),	_T("White, Background 1, Darker 35%")	},
		{	RGB(42, 42, 42),	_T("Black, Text 1, Lighter 15%")		},
		{	RGB(61, 61, 59),	_T("Tan, Background 2, Darker 50%")		},
		{	RGB(20, 57, 92),	_T("Dark Blue, Text 2, Darker 25%")		},
		{	RGB(54, 96, 139),	_T("Blue, Accent 1, Darker 25%")		},
		{	RGB(149, 63, 60),	_T("Red, Accent 2, Darker 25%")			},
		{	RGB(114, 139, 71),	_T("Olive Green, Accent 3, Darker 25%")	},
		{	RGB(97, 76, 119),	_T("Purple, Accent 4, Darker 25%")		},
		{	RGB(41, 128, 146),	_T("Aqua, Accent 5, Darker 25%")		},
		{	RGB(190, 112, 59),	_T("Orange, Accent 6, Darker 25%")		},

		{	RGB(126, 126, 126),	_T("White, Background 1, Darker 50%")	},
		{	RGB(20, 20, 20),	_T("Black, Text 1, Lighter 5%")			},
		{	RGB(29, 29, 28),	_T("Tan, Background 2, Darker 90%")		},
		{	RGB(17, 40, 64),	_T("Dark Blue, Text 2, Darker 50%")		},
		{	RGB(38, 66, 94),	_T("Blue, Accent 1, Darker 50%")		},
		{	RGB(100, 44, 43),	_T("Red, Accent 2, Darker 50%")			},
		{	RGB(77, 93, 49),	_T("Olive Green, Accent 3, Darker 50%")	},
		{	RGB(67, 53, 81),	_T("Purple, Accent 4, Darker 50%")		},
		{	RGB(31, 86, 99),	_T("Aqua, Accent 5, Darker 50%")		},
		{	RGB(126, 77, 42),	_T("Orange, Accent 6, Darker 50%")		},
	};

	static ColorTableEntry colorsStandard[] =
	{
		{	RGB(200, 15, 18),		_T("Dark Red")		},
		{	RGB(255, 20, 24),		_T("Red")			},
		{	RGB(255, 191, 40),		_T("Orange")		},
		{	RGB(255, 255, 49),		_T("Yellow")		},
		{	RGB(138, 207, 87),		_T("Light Green")	},
		{	RGB(0, 175, 84),		_T("Green")			},
		{	RGB(0, 174, 238),		_T("Light Blue")	},
		{	RGB(0, 111, 189),		_T("Blue")			},
		{	RGB(0, 36, 95),			_T("Black")			},
		{	RGB(114, 50, 157),		_T("Purple")		},
	};

	m_lstMainColors.RemoveAll();
	nNumColours = sizeof(colorsMain) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstMainColors.AddTail(colorsMain[i].color);
		CMFCRibbonColorButton::SetColorName(colorsMain[i].color, colorsMain[i].szName);
	};

	m_lstAdditionalColors.RemoveAll();
	nNumColours = sizeof(colorsAdditional) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstAdditionalColors.AddTail(colorsAdditional[i].color);
		CMFCRibbonColorButton::SetColorName(colorsAdditional[i].color, colorsAdditional[i].szName);
	};

	m_lstStandardColors.RemoveAll();
	nNumColours = sizeof(colorsStandard) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstStandardColors.AddTail(colorsStandard[i].color);
		CMFCRibbonColorButton::SetColorName(colorsStandard[i].color, colorsStandard[i].szName);
	};
}

void CMainFrame::SetOverlayRecentlyDefDrawItem(CMFCToolBarImages& mtbi_Src, UINT nSize)
{
	int nGalleryImageID = m_nRibbonDrawGalleryRecently;
	int nGalleryGrpImageID = 0;
	IMX_REP_Gallery galleryInfo;

	// 선	: 1
	nGalleryGrpImageID = 0;

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("직선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("화살표 직선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("양쪽 화살표 직선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("꺾인 연결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("꺾인 화살표 연결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("꺾인 양쪽 화살표 연결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("구부러진 연결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("구부러진 화살표 결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("구부러진 양쪽 화살표 연결선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("폴리라인");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("폐곡선스플라인/닫힌곡선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 1;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("스플라인/곡선");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);


	// 사각형	: 2
	nGalleryGrpImageID = 0;

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("사각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("둥근사각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("이등변 삼각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("직각 삼각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("평행 사변형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("사다리꼴");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("마름모");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("오각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 2;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("육각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);


	// 기본	: 3	
	nGalleryGrpImageID = 0;

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("텍스트");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("세로텍스트");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("타원");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);
	/*
	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("이등변 삼각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("직각 삼각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("평행 사변형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("사다리꼴");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("마름모");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("오각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("육각형");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);
	*/

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("OLE");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("이미지");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("그림파일");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	//galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID ++; 	galleryInfo.nGalleryImageID = nGalleryImageID ++;	galleryInfo.strGalleryImageStr = _T("좌표열폴리라인");
	//m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID  , galleryInfo));
	//AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("폴리곤");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("호");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("부채꼴");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("현");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("표준군대부호(군대부호->부호선택");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	galleryInfo.nGalleryID = 3;	galleryInfo.nGalleryGrpImageID = nGalleryGrpImageID++; 	galleryInfo.nGalleryImageID = nGalleryImageID++;	galleryInfo.strGalleryImageStr = _T("표준군대부호코드로 작성");
	m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
	AddOverlayRentlyDrawItem(nGalleryImageID - 1);

	std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.begin();
	for (; iter != m_mRibbonDrawGallery.end(); iter++)
		m_pRibbonDrawGallery->SetItemToolTip((*iter).first, (*iter).second.strGalleryImageStr);

	SetOverlayRecentlyDrawItemImg(mtbi_Src);
}

void CMainFrame::SetOverlayRecentlyRankDrawItem(CMFCToolBarImages& mtbi_Src, UINT nSize)
{
	if (SetOverlayRecentlyDrawItemImg(mtbi_Src))
		for (std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.begin(); iter != m_mRibbonDrawGallery.end(); iter++)
			m_pRibbonDrawGallery->SetItemToolTip((*iter).first, (*iter).second.strGalleryImageStr);
}

int CMainFrame::SetOverlayRecentlyDrawItem(CMFCToolBarImages& mtbi_Src, CMFCToolBarImages& mtbi_Target, int nIdx)
{
	return mtbi_Src.AddImage(mtbi_Target, nIdx);
}

size_t CMainFrame::SetOverlayRecentlyDrawItemImg(CMFCToolBarImages& mtbi_Src)
{
	size_t nSize = 0;
	for (std::vector<int>::reverse_iterator riter = m_vRibbonDrawGalleryRecently.rbegin(); riter != m_vRibbonDrawGalleryRecently.rend(); riter++)
	{
		IMX_REP_Gallery galleryInfo;

		std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.find((*riter));
		if (iter != m_mRibbonDrawGallery.end())
		{
			int nGalleryID = (*iter).second.nGalleryID;
			int nGalleryGrpImageID = (*iter).second.nGalleryGrpImageID;

			switch (nGalleryID)
			{
			case 1:				// 선 도형
			{
				if (SetOverlayRecentlyDrawItem(mtbi_Src, m_mtbi_Line, nGalleryGrpImageID) != -1)
					nSize++;
				break;
			}
			case 2:				// 사각형 도형
			{
				if (SetOverlayRecentlyDrawItem(mtbi_Src, m_mtbi_Rect, nGalleryGrpImageID) != -1)
					nSize++;
				break;
			}
			case 3:				// 기본 도형
			{
				if (SetOverlayRecentlyDrawItem(mtbi_Src, m_mtbi_default, nGalleryGrpImageID) != -1)
					nSize++;
				break;
			}
			}
		}
	}
	return nSize;
}

CString CMainFrame::GetOverlayDrawItem(int nIdx)
{
	AddOverlayRentlyDrawItem(nIdx);

	std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.find(nIdx);
	if (iter != m_mRibbonDrawGallery.end())
	{
		return (*iter).second.strGalleryImageStr;
	}
	return _T("");
}

void CMainFrame::AddOverlayRentlyDrawItem(int nIdx)
{

	if (m_nRibbonDrawGalleryRecently > nIdx)
		return;

	m_vRibbonDrawGalleryRecently.push_back(nIdx);

	if (m_nRibbonDrawGalleryRecently < (int)m_vRibbonDrawGalleryRecently.size())
	{
		std::vector<int>::iterator iter = m_vRibbonDrawGalleryRecently.begin();
		if (iter != m_vRibbonDrawGalleryRecently.end())
			m_vRibbonDrawGalleryRecently.erase(iter);
	}

	int nGalleryImageID = 0;
	for (std::vector<int>::reverse_iterator riter = m_vRibbonDrawGalleryRecently.rbegin(); riter != m_vRibbonDrawGalleryRecently.rend(); riter++)
	{
		IMX_REP_Gallery galleryInfo;

		std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.find((*riter));
		if (iter != m_mRibbonDrawGallery.end())
		{
			galleryInfo.nGalleryID = (*iter).second.nGalleryID;
			galleryInfo.nGalleryGrpImageID = (*iter).second.nGalleryGrpImageID;
			galleryInfo.nGalleryImageID = nGalleryImageID++;
			galleryInfo.strGalleryImageStr = (*iter).second.strGalleryImageStr;

			std::map<int, IMX_REP_Gallery>::iterator iter = m_mRibbonDrawGallery.find(galleryInfo.nGalleryImageID);
			if (iter != m_mRibbonDrawGallery.end())
				(*iter).second = galleryInfo;
			else
				m_mRibbonDrawGallery.insert(std::pair<int, IMX_REP_Gallery>(galleryInfo.nGalleryImageID, galleryInfo));
		}
	}
}

// 보기 범주 - 출력 창 On/Off
//void CMainFrame::OnMenuOutputOnOff()
//{
//	if (m_wndOutput.IsVisible() == TRUE)
//	{
//		ShowPane(&m_wndOutput, FALSE, FALSE, FALSE);
//
//		if (m_pRibbonOutputOnOff != NULL)
//		{
//			m_pRibbonOutputOnOff->SetImageIndex(20, FALSE);
//			m_pRibbonOutputOnOff->SetImageIndex(20, TRUE);
//		}
//	}
//	else
//	{
//		ShowPane(&m_wndOutput, TRUE, FALSE, TRUE);
//
//		if (m_pRibbonOutputOnOff != NULL)
//		{
//			m_pRibbonOutputOnOff->SetImageIndex(19, FALSE);
//			m_pRibbonOutputOnOff->SetImageIndex(19, TRUE);
//		}
//	}
//}
//
//void CMainFrame::OnUpdateMenuOutputOnOff(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(m_wndOutput.IsVisible());
//}

// 상황도 - 레이어 창 On/Off
void CMainFrame::OnLayerOnOff()
{
	if (m_wndCOPLayerView.IsVisible() == TRUE)
	{
		ShowPane(&m_wndCOPLayerView, FALSE, FALSE, FALSE);

		if (m_pwndCOPLayerViewOnOff != NULL)
		{
			m_pwndCOPLayerViewOnOff->SetImageIndex(20, FALSE);
			m_pwndCOPLayerViewOnOff->SetImageIndex(20, TRUE);
		}
	}
	else
	{
		ShowPane(&m_wndCOPLayerView, TRUE, FALSE, TRUE);

		if (m_pwndCOPLayerViewOnOff != NULL)
		{
			m_pwndCOPLayerViewOnOff->SetImageIndex(19, FALSE);
			m_pwndCOPLayerViewOnOff->SetImageIndex(19, TRUE);
		}
	}
}

void CMainFrame::OnUpdateLayerOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCOPLayerView.IsVisible());
}

// 상황도 - 아이템 창 On/Off
void CMainFrame::OnItemOnOff()
{
	if (m_wndCOPItemView.IsVisible() == TRUE)
	{
		ShowPane(&m_wndCOPItemView, FALSE, FALSE, FALSE);

		if (m_pwndCOPItemViewOnOff != NULL)
		{
			m_pwndCOPItemViewOnOff->SetImageIndex(20, FALSE);
			m_pwndCOPItemViewOnOff->SetImageIndex(20, TRUE);
		}
	}
	else
	{
		ShowPane(&m_wndCOPItemView, TRUE, FALSE, TRUE);

		if (m_pwndCOPItemViewOnOff != NULL)
		{
			m_pwndCOPItemViewOnOff->SetImageIndex(19, FALSE);
			m_pwndCOPItemViewOnOff->SetImageIndex(19, TRUE);
		}
	}
}

void CMainFrame::OnUpdateItemOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCOPItemView.IsVisible());
}

// 상황도 - 유형 창 On/Off
void CMainFrame::OnTypeOnOff()
{
	if (m_wndCOPTypeView.IsVisible() == TRUE)
	{
		ShowPane(&m_wndCOPTypeView, FALSE, FALSE, FALSE);

		if (m_pwndCOPTypeViewOnOff != NULL)
		{
			m_pwndCOPTypeViewOnOff->SetImageIndex(20, FALSE);
			m_pwndCOPTypeViewOnOff->SetImageIndex(20, TRUE);
		}
	}
	else
	{
		ShowPane(&m_wndCOPTypeView, TRUE, FALSE, TRUE);

		if (m_pwndCOPTypeViewOnOff != NULL)
		{
			m_pwndCOPTypeViewOnOff->SetImageIndex(19, FALSE);
			m_pwndCOPTypeViewOnOff->SetImageIndex(19, TRUE);
		}
	}
}

void CMainFrame::OnUpdateTypeOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCOPTypeView.IsVisible());
}

// 보기 범주 - 지도레이어 속성 창 On/Off
void CMainFrame::OnMenuMapPropOnOff()
{
	if (m_wndMapProperties.IsVisible() == TRUE)
	{
		ShowPane(&m_wndMapProperties, FALSE, FALSE, FALSE);

		if (m_pRibbonMapPropertyOnOff != NULL)
		{
			m_pRibbonMapPropertyOnOff->SetImageIndex(14, FALSE);
			m_pRibbonMapPropertyOnOff->SetImageIndex(14, TRUE);
			m_wndRibbonBar.Invalidate(FALSE);
		}
	}
	else
	{
		ShowPane(&m_wndMapProperties, TRUE, FALSE, TRUE);

		if (m_pRibbonMapPropertyOnOff != NULL)
		{
			m_pRibbonMapPropertyOnOff->SetImageIndex(13, FALSE);
			m_pRibbonMapPropertyOnOff->SetImageIndex(13, TRUE);
			m_wndRibbonBar.Invalidate(FALSE);
		}
	}
}

void CMainFrame::OnUpdateMenuMapPropOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndMapProperties.IsVisible());
}


// 보기 범주 - 지도레이어 관리 창 On/Off
void CMainFrame::OnMenuMapManagerOnOff()
{
	if (m_wndMapObjectManager.IsVisible() == TRUE)
	{
		ShowPane(&m_wndMapObjectManager, FALSE, FALSE, FALSE);

		if (m_pRibbonMapMgrOnOff != NULL)
		{
			m_pRibbonMapMgrOnOff->SetImageIndex(24, FALSE);
			m_pRibbonMapMgrOnOff->SetImageIndex(24, TRUE);
			m_wndRibbonBar.Invalidate(FALSE);
		}
	}
	else
	{
		ShowPane(&m_wndMapObjectManager, TRUE, FALSE, TRUE);

		if (m_pRibbonMapMgrOnOff != NULL)
		{
			m_pRibbonMapMgrOnOff->SetImageIndex(23, FALSE);
			m_pRibbonMapMgrOnOff->SetImageIndex(23, TRUE);
			m_wndRibbonBar.Invalidate(FALSE);
		}
	}
}

void CMainFrame::OnUpdateMenuMapManagerOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndMapObjectManager.IsVisible());
}



// 보기 범주 - 인덱스 지도 창 On/Off
void CMainFrame::OnMenuIndexMapOnOff()
{
	if (m_wndIndexMapView.IsVisible() == TRUE)
	{
		ShowPane(&m_wndIndexMapView, FALSE, FALSE, FALSE);

		if (m_pRibbonIndexMapOnOff != NULL)
		{
			m_pRibbonIndexMapOnOff->SetImageIndex(18, FALSE);
			m_pRibbonIndexMapOnOff->SetImageIndex(18, TRUE);
		}
	}
	else
	{
		ShowPane(&m_wndIndexMapView, TRUE, FALSE, TRUE);

		if (m_pRibbonIndexMapOnOff != NULL)
		{
			m_pRibbonIndexMapOnOff->SetImageIndex(17, FALSE);
			m_pRibbonIndexMapOnOff->SetImageIndex(17, TRUE);
		}
	}
}

void CMainFrame::OnUpdateMenuIndexMapOnOff(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndIndexMapView.IsVisible());
}


void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWndEx::OnClose();
}
