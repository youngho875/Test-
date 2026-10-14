// MiniMap.cpp: 구현 파일
//

#include "pch.h"
#include "C4IMap.h"
#include "MainFrm.h"
#include "C4IMapView.h"
#include "IndexMapWnd.h"


// MiniMap

IMPLEMENT_DYNAMIC(CIndexMapWnd, CWnd)

CIndexMapWnd::CIndexMapWnd()
{
	m_pCoordMgr = NULL;
	m_emMouseState = MOUSE_DEFAULT;
	m_nLockTrkno = 0;
	m_bOnZoom = FALSE;
	m_bAltAlways = FALSE;
	m_bMapRedraw = TRUE;
	m_bMapErase = FALSE;

	//m_bCardIndicator = FALSE;
	//m_bLButtonDownIndicator = FALSE;
	//m_bZoomIndicator = FALSE;
	//m_bMovingIndicator = FALSE;
	//m_bWheelRotationIndicator = FALSE;
}

CIndexMapWnd::~CIndexMapWnd()
{
	delete m_pCoordMgr;
}


BEGIN_MESSAGE_MAP(CIndexMapWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// MiniMap 메시지 처리기
/*
BOOL CIndexMapWnd::CreateEx(CWnd* pParentWnd)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pParentWnd = (CC4IMapView*)pParentWnd;
	ASSERT(m_pParentWnd != NULL);

	DWORD dwExStyle = WS_EX_CLIENTEDGE;
	CString szClassName = ::AfxRegisterWndClass(CS_HREDRAW, 0, (HBRUSH)::GetStockObject(BLACK_BRUSH), ::AfxGetApp()->LoadIcon(111));
	DWORD dwStyle = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_THICKFRAME
		| WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_MAXIMIZEBOX;

	static int nOrder = 0;
	const int k_nWidth = 284;	
	const int k_nHeight = 319;

	CRect rectParent;
	m_pParentWnd->GetClientRect(&rectParent);
	m_pParentWnd->ClientToScreen(&rectParent);

	CSize size(k_nWidth, k_nHeight);
	CPoint ptPos(rectParent.left + int((1 + 0.1 * nOrder) * k_nWidth - k_nWidth), (rectParent.bottom - k_nHeight));

	const CRect rect(ptPos, size);

	nOrder++;
	nOrder %= 4;

	m_pCoordMgr = CreateIndependentCoordMgr();
	m_pCoordMgr->InitDisplayParameter(rect);
	m_pCoordMgr->m_fScale = 0.5;
	m_pCoordMgr->SetDisplayCenter(170, 170);
	//m_pAirspaceInfoMgr = new CAirspaceInfoMgr(m_pParentWnd);
	//m_pWarTimeAirspaceInfoMgr = new CWarTimeAirspaceInfoMgr(m_pParentWnd);
	//m_WarTimeAirInfoMgr.SetOwnerWnd(m_pParentWnd);
	//m_AirInfoMgr.SetOwnerWnd(m_pParentWnd);

	return CWnd::CreateEx(dwExStyle, szClassName, "미니맵", dwStyle, rect, pParentWnd, NULL);
}
*/
int CIndexMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);

	m_pCoordMgr = CreateIndependentCoordMgr();
	m_pCoordMgr->InitDisplayParameter(rect);
	m_pCoordMgr->m_fScale = 0.5;
	m_pCoordMgr->SetDisplayCenter(rect.Width()/2, rect.Height()/2);


	SetTimer(0, 500, NULL);

	return 0;
}

void CIndexMapWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rect;
	GetClientRect(&rect);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetBkMode(1);

	CBitmap bitmap, * pOldBitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);

	DrawMapBitmap(&MemDC);

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
}

void CIndexMapWnd::DrawMapBitmap(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SetBkMode(TRANSPARENT);

	CBitmap* pOldBitmap;

	if (!m_bMapRedraw)
	{
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_bmpMap);
	}
	else
	{
		m_bmpMap.DeleteObject();
		m_bmpMap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_bmpMap);

		DrawMap(&MemDC);

		//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		//pFrame->m_indexMap->IndexMapDraw(&MemDC, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);

		m_bMapRedraw = FALSE;
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

}

void CIndexMapWnd::DrawMap(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	CBrush pBrush, * pOldBrush;


	pBrush.CreateSolidBrush(RGB(51, 55, 70));
	pOldBrush = pDC->SelectObject(&pBrush);

	pDC->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight, RGB(51, 55, 70));

	pDC->Rectangle(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight);
	pDC->SelectObject(pOldBrush);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_indexMap.IndexMapDraw(pDC, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);


	//minmap.shapeDraw(pDC, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);

	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();

	//if (pView->m_PointDisplay.GetCount() > 0)
	//	pView->m_PointDisplay.Draw(pDC);

}

BOOL CIndexMapWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if ((zDelta > 0 && m_pCoordMgr->m_fScale < 0.3) ||
		(zDelta < 0 && m_pCoordMgr->m_fScale > 100.0))
		return FALSE;

	if (zDelta == 0)
		return FALSE;

	m_fPrevScale = m_pCoordMgr->m_fScale;

	double	fXCoord, fYCoord;
	m_pCoordMgr->ScreenXY2WorldXY(m_pCoordMgr->m_nDisplayViewWidth / 2, m_pCoordMgr->m_nDisplayViewHeight / 2, &fXCoord, &fYCoord);

	float	fZoomRatio = (zDelta / abs(zDelta) * (-1)) * m_pCoordMgr->m_fScale / 10.0;
	m_pCoordMgr->m_fScale = m_pCoordMgr->m_fScale + fZoomRatio;

	long nCenterX, nCenterY;
	m_pCoordMgr->WorldXY2ScreenXY(fXCoord, fYCoord, &nCenterX, &nCenterY);
	m_pCoordMgr->SetNewCenter(nCenterX, nCenterY);

	m_bMapRedraw = TRUE;

	Invalidate();

	return TRUE;
}

void CIndexMapWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_pCoordMgr == NULL || cx <= 0 || cy <= 0)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect pRectView;
	pRectView.top = 0;
	pRectView.left = 0;
	pRectView.right = cx;
	pRectView.bottom = cy;

	m_pCoordMgr->InitDisplayParameter(pRectView);
	GetWindowRect(pRectView);

	m_bMapRedraw = TRUE;
}

BOOL CIndexMapWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;

	//return CWnd::OnEraseBkgnd(pDC);
}

BOOL CIndexMapWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	switch (m_emMouseState)
	{
	case MOUSE_ZOOMIN:// 확대
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN));
		break;

	case MOUSE_ZOOMOUT:// 축소
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT));
		break;

	case MOUSE_PANNING:// 이동
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PANNING));
		break;

	case MOUSE_DEFAULT:// 기본
	default:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DEFAULT));
		break;
	}

	return TRUE;
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CIndexMapWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0) {
		m_bMapRedraw = TRUE;
		Invalidate(FALSE);
	}

	CWnd::OnTimer(nIDEvent);
}

void CIndexMapWnd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//m_pParentWnd->SendMessage(WM_ON_MINIMAP_CLOSE, (UINT)this);

	CWnd::PostNcDestroy();
}

void CIndexMapWnd::DrawZoomRect(CDC* pDC, const CRect& rect)
{
	CPen pen, * pOldPen;
	pen.CreatePen(PS_DOT, 0, RGB(0, 0, 0));

	pOldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(&rect);

	if (abs(rect.Width()) > 15 && abs(rect.Height()) > 15) {
		pDC->MoveTo(rect.CenterPoint().x - 15, rect.CenterPoint().y);
		pDC->LineTo(rect.CenterPoint().x + 15, rect.CenterPoint().y);
		pDC->MoveTo(rect.CenterPoint().x, rect.CenterPoint().y - 15);
		pDC->LineTo(rect.CenterPoint().x, rect.CenterPoint().y + 15);
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);

}

void CIndexMapWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bOnZoom == TRUE && nFlags == MK_LBUTTON)
	{
		CRect rectClient;
		GetClientRect(&rectClient);

		CClientDC dc(this);
		DrawZoomRect(&dc, CRect(m_ptZoomStart, m_ptZoomEnd));
		m_ptZoomEnd = point;

		if (!rectClient.PtInRect(m_ptZoomEnd))
		{
			if (m_ptZoomEnd.x < rectClient.left)
				m_ptZoomEnd.x = rectClient.left;
			else if (m_ptZoomEnd.x > rectClient.right)
				m_ptZoomEnd.x = rectClient.right - 1;

			if (m_ptZoomEnd.y < rectClient.top)
				m_ptZoomEnd.y = rectClient.top;
			else if (m_ptZoomEnd.y > rectClient.bottom)
				m_ptZoomEnd.y = rectClient.bottom - 1;
		}

		DrawZoomRect(&dc, CRect(m_ptZoomStart, m_ptZoomEnd));
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CIndexMapWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_ptPrevPoint = point;
	m_ptMapMoveFirstPoint = point;

	//	if(m_bRaster != 0)
	//		return;

	//if(m_bPanning) 
	//{
	//m_bMapMove = TRUE;
	//SetCapture();
	//}

	CWnd::OnMButtonDown(nFlags, point);
}

void CIndexMapWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptNewPoint;
	ptNewPoint.x = -point.x + m_ptMapMoveFirstPoint.x;
	ptNewPoint.y = -point.y + m_ptMapMoveFirstPoint.y;


	m_pCoordMgr->m_fCenterX -= ptNewPoint.x;
	m_pCoordMgr->m_fCenterY -= ptNewPoint.y;

	m_bMapRedraw = TRUE;
	Invalidate();

	CWnd::OnMButtonUp(nFlags, point);
}

void CIndexMapWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	double fWorldX, fWorldY;

	CRect rectClient;
	GetClientRect(&rectClient);

	CClientDC dc(this);

	//char globalLat[10] = "370000N";
	//char globalLong[10] = "1270000E";

	double geo_x, geo_y;

	m_pCoordMgr->ScreenXY2WorldXY(point.x, point.y, &fWorldX, &fWorldY);
	m_Coord.x_y_to_degrees_lat_long(fWorldX, fWorldY, &geo_y, &geo_x);

	CBrush pBrush, * pOldBrush;
	pBrush.CreateSolidBrush(RGB(255, 0, 0));
	pOldBrush = dc.SelectObject(&pBrush);

	dc.Ellipse(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
	dc.SelectObject(pOldBrush);

	CPoint ptTempo;

	if (m_bOnZoom)
	{
		if (m_ptZoomEnd.x < m_ptZoomStart.x)
		{
			ptTempo.x = m_ptZoomEnd.x;
			m_ptZoomEnd.x = m_ptZoomStart.x;
			m_ptZoomStart.x = ptTempo.x;
		}
		if (m_ptZoomEnd.y < m_ptZoomStart.y)
		{
			ptTempo.y = m_ptZoomEnd.y;
			m_ptZoomEnd.y = m_ptZoomStart.y;
			m_ptZoomStart.y = ptTempo.y;
		}

		CRect rectZoom = CRect(m_ptZoomStart, m_ptZoomEnd);

		DrawZoomRect(&dc, rectZoom);

		ReleaseCapture();
		m_bOnZoom = FALSE;


		m_fPrevScale = m_pCoordMgr->m_fScale;


		m_bOnZoom = FALSE;
		m_pCoordMgr->CalcNewCenterAndScale(rectZoom);
	}


	CWnd::OnLButtonUp(nFlags, point);
}

void CIndexMapWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_ptBearingStart = point;

	double fXCoord, fYCoord;
	double geo_x, geo_y;

	m_pCoordMgr->ScreenXY2WorldXY(point.x, point.y, &fXCoord, &fYCoord);
	m_Coord.x_y_to_degrees_lat_long(fXCoord, fYCoord, &geo_y , &geo_x);


	m_fHookPoint.x = fXCoord;
	m_fHookPoint.y = fYCoord;

	//BOOL isHit = m_pParentWnd->HitTest(fXCoord, fYCoord, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);

	CStringArray* parray_value = new CStringArray();
	parray_value->Add(std::to_string(geo_y).c_str());
	parray_value->Add(std::to_string(geo_x).c_str());
	//m_pParentWnd->SendMessage(WM_MOVE_WITH_COORD, (WPARAM)parray_value, NULL);

	//m_pParentWnd->OpenDetailFromTrack();

	m_ptPrevPoint = point;
	m_ptZoomStart = m_ptZoomEnd = point;
	
	m_bOnZoom = TRUE;

	CWnd::OnLButtonDown(nFlags, point);
}

void CIndexMapWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_ptBearingStart = point;

	//m_nHookType = HOOKED_GROUND;

	double			fXCoord, fYCoord;

	m_pCoordMgr->ScreenXY2WorldXY(point.x, point.y, &fXCoord, &fYCoord);

	m_fHookPoint.x = fXCoord;
	m_fHookPoint.y = fYCoord;

	//BOOL isHit = m_pParentWnd->HitTest(fXCoord, fYCoord, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);

	CWnd::OnRButtonDown(nFlags, point);
}

void CIndexMapWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnRButtonUp(nFlags, point);
}


void CIndexMapWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	BOOL	bPtInRect = FALSE;

	/*
	bPtInRect = m_pParentWnd->m_TrackMgr.MousePtInLabelRect(point, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY,
		m_pCoordMgr->m_fScale, &m_pParentWnd->m_nLabelIndex);

	if (bPtInRect)
	{
		m_pParentWnd->m_TrackMgr.ResetLabelPosInfo(m_pParentWnd->m_nLabelIndex);
		m_bMapRedraw = TRUE;
		Invalidate();
	}

	BOOL ret = m_pParentWnd->OpenDetailFromTrack();

	if (ret == FALSE && m_pParentWnd->m_BearingRangeMgr.HitTest(point.x, point.y))
	{
		m_bMapRedraw = TRUE;
		Invalidate();
	}
	*/

	CWnd::OnLButtonDblClk(nFlags, point);
}



BOOL CIndexMapWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style ^= FWS_ADDTOTITLE;

	return CWnd::PreCreateWindow(cs);
}
