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

// C4IMapView.cpp: CC4IMapView 클래스의 구현
//

#include "pch.h"
//#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
//#ifndef SHARED_HANDLERS
//#include "C4IMap.h"
//#endif

#include "C4IMapViewHeader.h"
#include "C4IMapView.h"

//#include "GlobalInfo.h"
#include "../inc/Useful.h"
#include "GPoint.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CC4IMapView

IMPLEMENT_DYNCREATE(CC4IMapView, CView)

BEGIN_MESSAGE_MAP(CC4IMapView, CView)
    ON_COMMAND(ID_MAP_SOURCES_XML, &CC4IMapView::OnMapSourcesXml)
    ON_WM_TIMER()
    ON_COMMAND_RANGE(ID_C4OVER_PAN, ID_C4OVER_LOAD, &CC4IMapView::OnLocalOverlayCommand)
    ON_UPDATE_COMMAND_UI_RANGE(ID_C4OVER_PAN, ID_C4OVER_LOAD, &CC4IMapView::OnUpdateLocalOverlay)
    ON_WM_CAPTURECHANGED()
    ON_COMMAND(ID_LOCAL_SHP_SETTINGS,&CC4IMapView::OnLocalSettings)
    ON_COMMAND(ID_LOCAL_SHP_IDENTIFY,&CC4IMapView::OnLocalIdentify)
    ON_COMMAND(ID_LOCAL_SHP_TOGGLE,&CC4IMapView::OnLocalToggle)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CC4IMapView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_OPTION_CHANGED, &CC4IMapView::OnOptionChange)
	ON_MESSAGE(WM_MOVE_WITH_COORD, &CC4IMapView::move_with_coord)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	//ON_COMMAND(IDC_COMBO, &CC4IMapView::OnComboChangeScale)
	//ON_COMMAND(IDC_COMBO1, &CC4IMapView::OnComboCenterChange)
	ON_COMMAND(IDC_CHECK_SATELLITE, &CC4IMapView::OnCheckSatellite)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SATELLITE, &CC4IMapView::OnUpdateCheckSatellite)
	ON_COMMAND(ID_BTN_MAPGENERATER, &CC4IMapView::OnBtnMapgenerater)
	ON_COMMAND(IDC_CHECK_RASTER, &CC4IMapView::OnCheckRaster)
	ON_COMMAND(IDC_CHECK_SHAPE, &CC4IMapView::OnCheckShape)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_RASTER, &CC4IMapView::OnUpdateCheckRaster)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_SHAPE, &CC4IMapView::OnUpdateCheckShape)
	ON_COMMAND(IDC_CHECK_DTED, &CC4IMapView::OnCheckDted)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_DTED, &CC4IMapView::OnUpdateCheckDted)

	// 지도 핸들링
	ON_COMMAND(ID_MENU_MOUSE_BASE, &CC4IMapView::OnMenuMouseBase)
	ON_UPDATE_COMMAND_UI(ID_MENU_MOUSE_BASE, &CC4IMapView::OnUpdateMenuMouseBase)
	ON_COMMAND(ID_MENU_MOUSE_ZOOMIN, &CC4IMapView::OnMenuMouseZoomIn)
	ON_UPDATE_COMMAND_UI(ID_MENU_MOUSE_ZOOMIN, &CC4IMapView::OnUpdateMenuMouseZoomIn)
	ON_COMMAND(ID_MENU_MOUSE_ZOOMOUT, &CC4IMapView::OnMenuMouseZoomOut)
	ON_UPDATE_COMMAND_UI(ID_MENU_MOUSE_ZOOMOUT, &CC4IMapView::OnUpdateMenuMouseZoomOut)
	ON_COMMAND(ID_MENU_MOUSE_MOVE, &CC4IMapView::OnMenuMousePanning)
	ON_UPDATE_COMMAND_UI(ID_MENU_MOUSE_MOVE, &CC4IMapView::OnUpdateMenuMousePanning)

	//ON_COMMAND(ID_MENU_OVERLAY_NEW, OnOverlayNew)
	//ON_UPDATE_COMMAND_UI(ID_MENU_OVERLAY_NEW, OnUpdateOverlayNew)
	//ON_COMMAND(ID_RIBBON_OVERLAY_DRAW_GALLERY, OnOverlayDraw)
	//ON_UPDATE_COMMAND_UI(ID_RIBBON_OVERLAY_DRAW_GALLERY, OnUpdateOverlayDraw)

END_MESSAGE_MAP()

// CC4IMapView 생성/소멸

CC4IMapView::CC4IMapView()
//	: m_Thread(NULL)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	OriginX = 0;
	OriginY = 0;
	m_recSave.SetRectEmpty();
	m_pMouseState = MOUSE_DEFAULT;
	m_pSearchOption = SEARCH_NONE;
	m_pMouseStateCoordOld = MOUSE_DEFAULT;
	//m_generateDlg = NULL;

	m_pCoordMgr = GP_COORDMGR;
	m_Coord.initialize_adaptation_data();

	m_dcMain = NULL;
	m_dcMainTemp = NULL;
	m_dcMapLayer = NULL;
	m_dcShpLayer = NULL;
	m_dcDTEDLayer = NULL;
	m_dcRasterLayer = NULL;
	//m_dcSymLayer = NULL;

	m_pOldBitmap4MainTemp = NULL;
	m_pOldBitmap4Map = NULL;
	m_pOldBitmap4Shape = NULL;
	m_pOldBitmap4DTED = NULL;
	m_pOldBitmap4Raster = NULL;
	m_pOldBitmap4Sym = NULL;

	m_brush_10_10_10.CreateSolidBrush(RGB(10, 10, 10));

	m_fPrevScale = 0.0;

	m_bRaster = FALSE;
	m_bsatellite = FALSE;
	m_bShape = FALSE;
	m_bDted = FALSE;
	m_bDCInitialized = FALSE;
	m_bMapMove = FALSE;
	m_bPanning = FALSE;
	mb_moving = FALSE;
	m_bOnZoom = FALSE;
	m_bIsDrawSituationInProcess = FALSE;

	//m_pMapConfigMgr = NULL;
	//m_pMapDataMgr = NULL;
	//m_pMapDisplayMgr = NULL;

	// 투명도
	m_pOverlayThread = NULL;
	m_pDlgOverlayControler = NULL;
	m_pDlgObjectList = NULL;
	m_pDlgOverlayProperty = NULL;
	m_pDlgOverlayTextEdit = NULL;
	m_pDlgOverlayEditObject = NULL;
	m_pDlgOverlayEditObjectPoint = NULL;

	m_nOverlyaEditObjectPointX = 0;
	m_nOverlyaEditObjectPointY = 0;

	//m_TmsMgr = NULL;
	mHeights = NULL;

}

CC4IMapView::~CC4IMapView()
{
	if (m_dcMainTemp && m_pOldBitmap4MainTemp)
		m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	if (m_dcMapLayer && m_pOldBitmap4Map)
		m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	if (m_dcShpLayer && m_pOldBitmap4Shape)
		m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	if (m_dcDTEDLayer && m_pOldBitmap4DTED)
		m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	if (m_dcRasterLayer && m_pOldBitmap4Raster)
		m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);
	//m_dcSymLayer->SelectObject(m_pOldBitmap4Sym);

	m_pBitmap4MainTemp.DeleteObject();
	m_pBitmap4Map.DeleteObject();
	m_pBitmap4Shape.DeleteObject();
	m_pBitmap4DTED.DeleteObject();
	m_pBitmap4Raster.DeleteObject();
	//m_pBitmap4Sym.DeleteObject();

	//m_dcSymLayer->DeleteDC();
	if (m_dcRasterLayer) m_dcRasterLayer->DeleteDC();
	if (m_dcDTEDLayer) m_dcDTEDLayer->DeleteDC();
	if (m_dcShpLayer) m_dcShpLayer->DeleteDC();
	if (m_dcMapLayer) m_dcMapLayer->DeleteDC();
	if (m_dcMainTemp) m_dcMainTemp->DeleteDC();

	if (m_dcMain && m_dcMain->GetSafeHdc())
		::ReleaseDC(this->GetSafeHwnd(), m_dcMain->Detach());
	//m_pOverlayThread->End();

	//SAFE_DELETE(m_dcSymLayer);
	SAFE_DELETE(m_dcRasterLayer);
	SAFE_DELETE(m_dcDTEDLayer);
	SAFE_DELETE(m_dcShpLayer);
	SAFE_DELETE(m_dcMapLayer);
	SAFE_DELETE(m_dcMainTemp);
	SAFE_DELETE(m_dcMain);

	CCoordMgr::ReleaseInst();

	//if (m_pMapDataMgr)
	//{
	//	delete m_pMapDataMgr;
	//	m_pMapDataMgr = NULL;
	//}

	//if (m_pMapDisplayMgr)
	//{
	//	delete m_pMapDisplayMgr;
	//	m_pMapDisplayMgr = NULL;
	//}

	//if (m_TmsMgr)
	//{
	//	delete m_TmsMgr;
	//	m_TmsMgr = NULL;

	//}
	//if (m_pMapConfigMgr)
	//{
	//	delete m_pMapConfigMgr;
	//	m_pMapConfigMgr = NULL;
	//}

	//SAFE_DELETE(m_pOverlayThread);
	//SAFE_DELETE(m_pDlgOverlayControler);
	//SAFE_DELETE(m_pDlgObjectList);
	//SAFE_DELETE(m_pDlgOverlayProperty);
	//SAFE_DELETE(m_pDlgOverlayTextEdit);
	//SAFE_DELETE(m_pDlgOverlayEditObject);
	//SAFE_DELETE(m_pDlgOverlayEditObjectPoint);


}

// viewer 시작
BOOL CC4IMapView::BeginView()
{
	//m_pathConfig.SetWork(FILEPATH_CONFIG);

	// 투명도 관련 대화상자 생성
	//m_pDlgOverlayControler = new CDlgOverlayControler(this);
	//if (m_pDlgOverlayControler != NULL)
	//{
	//	// 투명도
	//	m_pDlgOverlayControler->Create(CDlgOverlayControler::IDD, this);
	//}
	 
	//m_pDlgObjectList = new CDlgObjectList(this);
	//if (m_pDlgObjectList != NULL)
	{
		//m_pDlgObjectList->Create(CDlgObjectList::IDD, this);
 		//m_pOverlayThread = new COverlayThread(this);
		//m_pDlgOverlayControler->BindOverlay(m_pOverlayThread, m_pDlgObjectList);
		//m_pOverlayThread->SetViewPointer(this);
	}
/*
	m_pDlgOverlayProperty = new CDlgOverlayProperty(this, m_pOverlayThread->GetOverlayView());
	if (m_pDlgOverlayProperty != NULL)
		m_pDlgOverlayProperty->Create(CDlgOverlayProperty::IDD, this);

	m_pDlgOverlayTextEdit = new CDlgOverlayTextEdit(this);
	if (m_pDlgOverlayTextEdit != NULL)
	{
		m_pDlgOverlayTextEdit->Create(CDlgOverlayTextEdit::IDD, this);
		m_pDlgOverlayTextEdit->BindOverlayView(m_pOverlayThread->GetOverlayView());
	}

	m_pDlgOverlayEditObject = new CDlgOverlayEditObject(this);
	if (m_pDlgOverlayEditObject != NULL)
		m_pDlgOverlayEditObject->Create(CDlgOverlayEditObject::IDD, this);

	m_pDlgOverlayEditObjectPoint = new CDlgOverlayEditObjectPoint(this);
	if (m_pDlgOverlayEditObjectPoint != NULL)
		m_pDlgOverlayEditObjectPoint->Create(CDlgOverlayEditObjectPoint::IDD, this);
*/
	//m_adjObj = CObjectProp::GetInst();
	//m_adjObj->SetWnd(this);

	//m_TmsMgr = new CTmsMgr();

#if C4MAP_ENABLE_LOCAL
	m_TmsMgr.Initalize();
#endif
    SetTimer(59200,150,nullptr);

	return TRUE;
}

BOOL CC4IMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CC4IMapView 그리기

void CC4IMapView::OnDraw(CDC* pDC)
{
	CC4IMapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CC4IMapView 인쇄


void CC4IMapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CC4IMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CC4IMapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CC4IMapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CC4IMapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    if(m_localOverlay.IsDrawing()){m_localOverlay.Cancel();ReleaseCapture();Invalidate(FALSE);return;}
    m_localClick=point;
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CC4IMapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CC4IMapView 진단

#ifdef _DEBUG
void CC4IMapView::AssertValid() const
{
	CView::AssertValid();
}

void CC4IMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CC4IMapDoc* CC4IMapView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CC4IMapDoc)));
	return (CC4IMapDoc*)m_pDocument;
}
#endif //_DEBUG


// CC4IMapView 메시지 처리기

void CC4IMapView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.

	DrawSituation(TRUE, TRUE, TRUE, TRUE);

	//if (!m_bDCInitialized)
	//{
	//	DrawSituation(TRUE, TRUE, TRUE, TRUE);
	//	m_bDCInitialized = TRUE;
	//}
	//DrawSituation(FALSE, FALSE, FALSE, FALSE);
}

void CC4IMapView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pDC->SetMapMode(MM_ANISOTROPIC);

	CView::OnPrepareDC(pDC, pInfo);
}

BOOL CC4IMapView::HitTest(double fXCoord, double fYCoord, double fCenterX, double fCenterY, double fScale)
{
	if (HitestDisplayPoint(fXCoord, fYCoord, fCenterX, fCenterY, fScale)) return TRUE;
	//if (HitestSymbol(fXCoord, fYCoord, fCenterX, fCenterY, fScale)) return TRUE;
	//if (HitestSpecialPoint(fXCoord, fYCoord, fCenterX, fCenterY, fScale)) return TRUE;
	//if (HitestTrack(fXCoord, fYCoord, fCenterX, fCenterY, fScale)) return TRUE;


	m_nHookType = HOOKED_GROUND;

	return FALSE;
}

CMainFrame* CC4IMapView::get_mainfrm()
{
	return (CMainFrame*)AfxGetMainWnd();
}


void CC4IMapView::ZoomIn()
{
	FLOAT	fZoom, fScale;

	//fScale = m_pDlgOverlayControler->m_piMapProj->GetMapScale();
	//fZoom = m_pDlgOverlayControler->m_piMapProj->GetZoomFactor();
	//fZoom = m_pCoordMgr->m_fScale;

	////fZoom *= 1.2f;	//2.0;
	//m_pDlgOverlayControler->m_piMapProj->SetZoomFactor(fZoom);
	//m_pDlgOverlayControler->m_rMPW = fScale / fZoom;
	//m_pDlgOverlayControler->m_piEditTool->SetMeterPerPixel(m_pDlgOverlayControler->m_rMPW);
	Invalidate();
}


void CC4IMapView::ZoomOut()
{
	FLOAT	fZoom, fScale;

	//fScale = m_pDlgOverlayControler->m_piMapProj->GetMapScale();
	//fZoom = m_pDlgOverlayControler->m_piMapProj->GetZoomFactor();
	//fZoom = m_pCoordMgr->m_fScale;
	////fZoom /= 1.2f;	//2.0;

	//m_pDlgOverlayControler->m_piMapProj->SetZoomFactor(fZoom);
	//m_pDlgOverlayControler->m_rMPW = fScale / fZoom;
	//m_pDlgOverlayControler->m_piEditTool->SetMeterPerPixel(m_pDlgOverlayControler->m_rMPW);
	Invalidate();
}

void CC4IMapView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (cx <= 0 || cy <= 0 || m_pCoordMgr == NULL)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rec = m_recSave;
	GetWindowRect(&m_recSave);

	CRect pRectView;
	pRectView.top = 0;
	pRectView.left = 0;
	pRectView.right = cx;
	pRectView.bottom = cy;

	double x, y;
	m_pCoordMgr->GetDisplayCenter(&x, &y);
	if (m_bDCInitialized)
	{
		m_pCoordMgr->m_fCenterX = x + rec.left - m_recSave.left;
		m_pCoordMgr->m_fCenterY = y + rec.top - m_recSave.top;
	}



	m_pCoordMgr->m_nDisplayViewWidth = pRectView.Width();
	m_pCoordMgr->m_nDisplayViewHeight = pRectView.Height();
	m_pCoordMgr->m_fHorVerRatio = (double)(pRectView.Width()) / (double)(pRectView.Height());

	m_pCoordMgr->InitDisplayParameter(pRectView);


	GetWindowRect(pRectView);
	//	m_wndToolBar.GetWindowRect(pRectDlg); //0310����

	//m_DlgMenuBar.SetViewPosSize(pRectView); //0310����

	if (m_bDCInitialized) {
		//		m_DlgMenuBar.SetViewPosSize(pRectView); //0310���� 
		//		m_wndToolBar.SetViewPosSize(pRectView); //0310���� 

		ReInitializeDCs();
		//m_pOverlayThread->OnSize(cx,cy);
		SetFocus();
		//DrawSituation(TRUE, TRUE, TRUE, TRUE);
	}
}




void CC4IMapView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CC4IMapView::DrawZoomRect(CDC* pDC, CRect& pRect)
{
	CPen pen, * pOldPen;
	pen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));

	pOldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(&pRect);

	if (abs(pRect.Width()) > 15 && abs(pRect.Height()) > 15) 
	{
		pDC->MoveTo(pRect.CenterPoint().x - 15, pRect.CenterPoint().y);
		pDC->LineTo(pRect.CenterPoint().x + 15, pRect.CenterPoint().y);
		pDC->MoveTo(pRect.CenterPoint().x, pRect.CenterPoint().y - 15);
		pDC->LineTo(pRect.CenterPoint().x, pRect.CenterPoint().y + 15);
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
}

LRESULT CC4IMapView::OnOptionChange(WPARAM pText, LPARAM nFlag)
{
	return 0;
}

void CC4IMapView::OnBtnMapgenerater()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	/*
	m_generateDlg = new CMapGeneratorDlg(this);
	if (m_generateDlg != NULL)
	{
		m_generateDlg->Create(IDD_MAP_GENERATOR, this);
		m_generateDlg->ShowWindow(true);
	}
	*/
}


void CC4IMapView::OnLocalSettings(){if(!C4MAP_ENABLE_LOCAL)return;get_mainfrm()->m_indexMap.ConfigureLocal(this);get_mainfrm()->m_wndMapObjectManager.RefreshLocalShapeTree();DrawSituation(TRUE,TRUE,TRUE,TRUE);}
void CC4IMapView::OnLocalToggle(){get_mainfrm()->m_indexMap.ToggleLocal();DrawSituation(TRUE,TRUE,TRUE,TRUE);}
void CC4IMapView::OnLocalIdentify(){ShowLocalFeature(m_localClick);}
void CC4IMapView::ShowLocalFeature(CPoint point){
    CString title;std::vector<CString> fields,values;
    auto frame=get_mainfrm();
    frame->m_wndMapProperties.ClearPropList();
    if(frame->m_indexMap.IdentifyLocal(point,title,fields,values)){
        frame->m_wndMapProperties.SetSearchFeatureInfo(title,fields,values);
        frame->m_wndMapProperties.ShowPane(TRUE,FALSE,TRUE);
    }else {frame->m_wndMapProperties.SetSearchFeatureInfo(_T("SHP: 선택된 객체 없음"),fields,values);}
    DrawSituation(TRUE,TRUE,TRUE,TRUE);
}

void CC4IMapView::OnLocalOverlayCommand(UINT id)
{
    m_localOverlay.Cancel();
    if(GetCapture()==this)ReleaseCapture();
    m_bPanning=FALSE;m_bMapMove=FALSE;m_bOnZoom=FALSE;mb_moving=FALSE;
    if(id>=ID_C4OVER_PAN && id<=ID_C4OVER_ELLIPSE)m_localOverlay.SetTool(id-ID_C4OVER_PAN);
    else if(id==ID_C4OVER_COLOR){CColorDialog dlg(m_localOverlay.Color(),CC_FULLOPEN,this);if(dlg.DoModal()==IDOK)m_localOverlay.SetColor(dlg.GetColor());}
    else if(id==ID_C4OVER_UNDO)m_localOverlay.Undo();
    else if(id==ID_C4OVER_SAVE || id==ID_C4OVER_LOAD){
        BOOL load=id==ID_C4OVER_LOAD;
        CFileDialog dlg(load,_T("xml"),_T("overlay.xml"),OFN_HIDEREADONLY|(load?OFN_FILEMUSTEXIST:OFN_OVERWRITEPROMPT),_T("C4IMap Overlay XML (*.xml)|*.xml||"),this);
        if(dlg.DoModal()==IDOK){
            if(load && m_localOverlay.Count() && AfxMessageBox(_T("현재 Overlay 도형을 선택한 파일로 교체하시겠습니까? 저장하지 않은 도형은 사라집니다."),MB_YESNO|MB_ICONQUESTION)!=IDYES)return;
            CString error;bool ok=load?m_localOverlay.Load(dlg.GetPathName(),error):m_localOverlay.Save(dlg.GetPathName(),error);
            if(!ok)AfxMessageBox(error,MB_ICONERROR);
        }
    }
    Invalidate(FALSE);
}
void CC4IMapView::OnUpdateLocalOverlay(CCmdUI* ui){
    ui->Enable(ui->m_nID!=ID_C4OVER_UNDO || m_localOverlay.Count()>0);
    if(ui->m_nID<=ID_C4OVER_ELLIPSE)ui->SetRadio(m_localOverlay.GetTool()==int(ui->m_nID-ID_C4OVER_PAN));
}
BOOL CC4IMapView::PreTranslateMessage(MSG* msg){
    if(msg->message==WM_KEYDOWN && msg->wParam==VK_ESCAPE && (m_localOverlay.IsDrawing()||m_localOverlay.GetTool()!=CLocalOverlay::Pan)){
        m_localOverlay.SetTool(CLocalOverlay::Pan);if(GetCapture()==this)ReleaseCapture();Invalidate(FALSE);return TRUE;
    }
    return CView::PreTranslateMessage(msg);
}
void CC4IMapView::OnCaptureChanged(CWnd* wnd){
    if(m_localOverlay.IsDrawing()){m_localOverlay.Cancel();Invalidate(FALSE);}
    CView::OnCaptureChanged(wnd);
}

BOOL CC4IMapView::SaveLocalOverlayIfModified(){
    if(!m_localOverlay.IsModified())return TRUE;
    int answer=AfxMessageBox(_T("Overlay 도형 변경 내용을 XML로 저장하시겠습니까?"),MB_YESNOCANCEL|MB_ICONQUESTION);
    if(answer==IDCANCEL)return FALSE;
    if(answer==IDNO)return TRUE;
    OnLocalOverlayCommand(ID_C4OVER_SAVE);
    return !m_localOverlay.IsModified();
}

void CC4IMapView::OnMapSourcesXml(){MapSources().Configure(this);get_mainfrm()->m_wndMapObjectManager.RefreshLocalShapeTree();Invalidate(FALSE);}
void CC4IMapView::OnTimer(UINT_PTR id){if(id==59200&&MapSources().Poll()){get_mainfrm()->SetMessageText(MapSources().Status());Invalidate(FALSE);}CView::OnTimer(id);}
