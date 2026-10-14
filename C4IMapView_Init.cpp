#include "pch.h"
#include "C4IMapViewHeader.h"




void CC4IMapView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	BeginView();
	//m_pathConfig.SetWork(FILEPATH_CONFIG);

	CRect pClientRect;
	GetClientRect(pClientRect);

	//	Create DCs for Objects
	m_dcMain = new CDC;
	m_dcMainTemp = new CGDCGeo;
	m_dcMapLayer = new CDC;
	m_dcShpLayer = new CDC;
	m_dcDTEDLayer = new CDC;
	m_dcRasterLayer = new CDC;

	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	HDC dc = ::GetDC(this->GetSafeHwnd());
	m_dcMain->Attach(dc);

	m_dcMainTemp->CreateCompatibleDC((CGDCGeo*)m_dcMain);
	m_dcMapLayer->CreateCompatibleDC(m_dcMain);
	m_dcMapLayer->SetBkMode(TRANSPARENT);
	m_dcMapLayer->SelectObject(&m_brush_10_10_10);

	m_dcShpLayer->CreateCompatibleDC(m_dcMain);
	m_dcDTEDLayer->CreateCompatibleDC(m_dcMain);
	m_dcRasterLayer->CreateCompatibleDC(m_dcMain);

	InitializeDisplayParameter(pClientRect);
	int nWidth, nHeight;
	nWidth = m_pCoordMgr->m_nDisplayViewWidth;
	nHeight = m_pCoordMgr->m_nDisplayViewHeight;

	m_pBitmap4MainTemp.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Map.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Shape.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4DTED.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Raster.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);


	m_pOldBitmap4MainTemp = m_dcMainTemp->SelectObject(&m_pBitmap4MainTemp);
	m_pOldBitmap4Map = m_dcMapLayer->SelectObject(&m_pBitmap4Map);
	m_pOldBitmap4Shape = m_dcShpLayer->SelectObject(&m_pBitmap4Shape);
	m_pOldBitmap4DTED = m_dcDTEDLayer->SelectObject(&m_pBitmap4DTED);
	m_pOldBitmap4Raster = m_dcRasterLayer->SelectObject(&m_pBitmap4Raster);

	 
	// 임시로 막음 유영호
	//m_pDlgOverlayControler->InitProjection();
	//m_pDlgOverlayControler->InitSymbolEditTool();
	//m_pDlgOverlayControler->m_piEditTool->OnChangeEditableList();
	//////////////////////////

	// 투명도 초기화
	//CCoordMgr* pCoordMgr = GP_COORDMGR;
	//m_pOverlayThread->Initialize(pCoordMgr, LAT_DEGREE, LAT_MIN, LAT_SEC, LNG_DEGREE, LNG_MIN, LNG_SEC, LAT_CHAR, LNG_CHAR);


	m_pCoordMgr->m_fScale = 1.0;
	double x_coordinate, y_coordinate;
	CPoint pPoint;
	m_Coord.degrees_lat_long_to_x_y(38.0, 128.0, &x_coordinate, &y_coordinate);
	m_pCoordMgr->WorldXY2ScreenXY(x_coordinate, y_coordinate, &pPoint.x, &pPoint.y, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);
	m_pCoordMgr->SetNewCenter((double)pPoint.x, (double)pPoint.y);

	AfxGetMainWnd()->SetFocus();
	m_bDCInitialized = TRUE;

	BufferDC.CreateCompatibleDC(m_dcMain);
	//bmpBuffer.CreateCompatibleBitmap(m_dcMain, rect.Width(), rect.Height());
	bmpBuffer.CreateCompatibleBitmap(m_dcMain, 2000, 2000);
	(CBitmap*)BufferDC.SelectObject(&bmpBuffer);

	// ini 파일 구조 점검이 필요해서 잠시 막음
	//m_pMapConfigMgr = new CMapConfigMgr(this);
	//if(!m_pMapConfigMgr) 
	//	return;    
	//		
	//m_pMapConfigMgr->Init(INIT_FILE);

	//m_pMapConfigMgr->InitMapGroupItem();			//MapGroup, MapGroupList 파일 내용을 로드 . 주제별 분류 
	//m_pMapConfigMgr->InitDefaultTextPriority();		//DefaultTextPriority 파일 로드, 축척별 폰트 설정		
	//m_pMapConfigMgr->InitWorldMapPriority();		//WorldMapPriority 파일 로드, 세계지도 속성 정보r
	//m_pMapConfigMgr->InitVectorMapInfo();
	//m_pMapConfigMgr->InitVectorMapProperty();
	//m_pMapConfigMgr->InitMapPriority();
	//m_pMapConfigMgr->OpenRasterMapInfo();			//RasterMapInfo 파일 로드, CADRG이미지 폴더,파일, MBR ,속도 잡아먹는 주범 
	//m_pMapConfigMgr->OpenNameData();				//NameList 파일로드, 지명 로드속도 잡아먹는 주범 		
	//m_pMapConfigMgr->load_shp_attribute(); 	

	//if (m_pMapDataMgr == NULL)
	//	m_pMapDataMgr = new CMapDataMgr(this);
	//else
	//	return;
	
	//if(m_pMapDisplayMgr == NULL)
	//	m_pMapDisplayMgr = new CGMapDisplayMgr(this);
	//if (!m_pMapDisplayMgr) 
	//	return;

	

	
	//LoadShp();
	//mSh.OpenShapeFile();
	//mSh.OpenDTEDFile("d:/mapdata/GDEM-10km-BW.tif");



	CRect rc;
	CGRectGP rect;

	double left, top, right, bottom;

	//CMapViewView *pView=(CMapViewView *)m_pWnd; 
	GetClientRect(&rc);
	//pView->ScreenToClient(&rc);

	m_pCoordMgr->ScreenXY2WorldXY(rc.left, rc.top, &left, &top);
	m_pCoordMgr->ScreenXY2WorldXY(rc.right, rc.bottom, &right, &bottom);
	m_Coord.x_y_to_degrees_lat_long(left, top, &rect.m_R.top, &rect.m_R.left);
	m_Coord.x_y_to_degrees_lat_long(right, bottom, &rect.m_R.bottom, &rect.m_R.right);


	CString leftTop, rightBottom;
	leftTop.Format("%fE%fN", rect.m_R.left, rect.m_R.top);
	rightBottom.Format("%fE%fN", rect.m_R.right, rect.m_R.bottom);

	//m_dted= new CGDtedMgr();
	//m_dted->Create(leftTop, rightBottom, CGDoublePoint(33.0, 33.0), m_dted->m_meterPerSecond.x, m_dted->m_meterPerSecond.y);

	//mHeights = new CAltitude();

}

void CC4IMapView::InitializeDisplayParameter(CRect pClientRect)
{
	m_pCoordMgr->InitDisplayParameter(pClientRect);
}

void CC4IMapView::ReInitializeDCs()
{
	int nWidth, nHeight;
	nWidth = m_pCoordMgr->m_nDisplayViewWidth;
	nHeight = m_pCoordMgr->m_nDisplayViewHeight;

	// 투명도 디바이스 컨텍스트에게 현재 view정보를 전달한다.
	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);

	m_pBitmap4MainTemp.DeleteObject();
	m_pBitmap4Map.DeleteObject();
	m_pBitmap4Shape.DeleteObject();
	m_pBitmap4DTED.DeleteObject();
	m_pBitmap4Raster.DeleteObject();

	m_pBitmap4MainTemp.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Map.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Shape.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4DTED.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Raster.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);

	m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);

	m_pOldBitmap4MainTemp = m_dcMainTemp->SelectObject(&m_pBitmap4MainTemp);
	m_pOldBitmap4Map = m_dcMapLayer->SelectObject(&m_pBitmap4Map);
	m_pOldBitmap4Shape = m_dcShpLayer->SelectObject(&m_pBitmap4Shape);
	m_pOldBitmap4DTED = m_dcDTEDLayer->SelectObject(&m_pBitmap4DTED);
	m_pOldBitmap4Raster = m_dcRasterLayer->SelectObject(&m_pBitmap4Raster);
}
