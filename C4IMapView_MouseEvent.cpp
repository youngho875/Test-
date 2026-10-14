#include "pch.h"
#include "C4IMapViewHeader.h"

#include <iostream>



char		globalLat[10] = CENTERLAT;
char		globalLong[10] = CENTERLONG;


void CC4IMapView::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_localOverlay.Move(point,m_pCoordMgr)){Invalidate(FALSE);return;}
	CView::OnMouseMove(nFlags, point);

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateStatusBar(point);

	if (m_bPanning && nFlags == MK_LBUTTON)
	{
		mb_moving = TRUE;

		if (m_ptPrevPoint == point)
			return;

		//CKB 메인 DC만 그리도록 해서 성능 확보  
		m_ptPrevPoint.x = point.x - m_ptZoomWindowLT.x;
		m_ptPrevPoint.y = point.y - m_ptZoomWindowLT.y;

		draw_maindc_only(m_ptPrevPoint);

		return;
	}

	if (m_bOnZoom == TRUE && nFlags == MK_LBUTTON + MK_SHIFT)
	{
		// Draging for Zooming
		CRect rectClient;
		GetClientRect(&rectClient);

		/////////////////////////////////////////////////////////
		// Zoom Rect를 그리고 계산한다. 
		/////////////////////////////////////////////////////////

		//	1. 이전의 Rect를 지운다. 
		DrawZoomRect(m_dcMain, CRect(m_ptZoomStart, m_ptZoomEnd));

		//	2. 새로운 Rect를 계산하고 예외처리를 한다.  
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
		//	3. 새로운 Rect를 그린다.  
		DrawZoomRect(m_dcMain, CRect(m_ptZoomStart, m_ptZoomEnd));
	}
	//CView::OnMouseMove(nFlags, point);
}


void CC4IMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
    if(m_localOverlay.Begin(point,m_pCoordMgr)){SetFocus();SetCapture();Invalidate(FALSE);return;}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	m_bPanning = (nFlags & (MK_SHIFT | MK_CONTROL)) == 0;
	
	m_ptZoomStart = m_ptZoomEnd = point;
	SetCapture();
	m_bOnZoom = (nFlags & MK_SHIFT) != 0;

	m_ptPrevPoint = point;
	m_ptMapMoveFirstPoint = point;
	if (nFlags == MK_LBUTTON)
	{
		m_ptZoomWindowLT = m_ptZoomWindowBR = point;
		m_bMapMove = TRUE;

		HCURSOR hCursor;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		SetCursor(hCursor);
		SetCapture();
	}



	CView::OnLButtonDown(nFlags, point);
}


void CC4IMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_localOverlay.End(point,m_pCoordMgr)){ReleaseCapture();Invalidate(FALSE);return;}
    const bool localClick=!m_bOnZoom&&!(nFlags&(MK_SHIFT|MK_CONTROL))&&abs(point.x-m_ptMapMoveFirstPoint.x)<=3&&abs(point.y-m_ptMapMoveFirstPoint.y)<=3;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);

	ReleaseCapture();
	CRect		rc;


	CClientDC	dc(this);
	CPoint ClinetPoint = point;

	m_bPanning = FALSE;
	mb_moving = FALSE;//

	if (m_bMapMove && point != m_ptMapMoveFirstPoint && nFlags != MK_CONTROL && nFlags != MK_SHIFT)
	{
		CPoint ptNewPoint;
		ptNewPoint.x = -point.x + m_ptMapMoveFirstPoint.x;
		ptNewPoint.y = -point.y + m_ptMapMoveFirstPoint.y;
		m_pCoordMgr->m_fCenterX -= ptNewPoint.x;
		m_pCoordMgr->m_fCenterY -= ptNewPoint.y;
		//m_pOverlayThread->RePosition();
		//if (m_pDlgOverlayControler->IsWindowVisible() == TRUE)
		//	m_pDlgOverlayControler->UpDateGridDraw();


		DrawSituation(TRUE, TRUE, TRUE, TRUE);
		m_bMapMove = FALSE;
		HCURSOR hCursor;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		SetCursor(hCursor);

	} 

	double fWorldX, fWorldY;
	m_pCoordMgr->ScreenXY2WorldXY(point.x, point.y, &fWorldX, &fWorldY);
	m_Coord.x_y_to_degrees_lat_long_str(fWorldX, fWorldY, globalLat, globalLong);


	// 점 그리기
	CBrush pBrush, * pOldBrush;
	pBrush.CreateSolidBrush(RGB(255, 0, 0));
	pOldBrush = m_dcMain->SelectObject(&pBrush);
	m_dcMain->Ellipse(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
	m_dcMain->SelectObject(pOldBrush);

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
		DrawZoomRect(m_dcMain, rectZoom);
		ReleaseCapture();
		m_bOnZoom = FALSE;
		m_fPrevScale = m_pCoordMgr->m_fScale;
		m_bOnZoom = FALSE;
		if (rectZoom.Width() > 1 && rectZoom.Height() > 1)
			m_pCoordMgr->CalcNewCenterAndScale(rectZoom);

		//CString sStatus;
		//sStatus.Format("배율:%f       ", m_pCoordMgr->m_fScale);
		//CMainFrame* pFrame;
		//pFrame = (CMainFrame*)AfxGetMainWnd();

		//m_pOverlayThread->RePosition();
		//if (m_pDlgOverlayControler->IsWindowVisible() == TRUE)
		//	m_pDlgOverlayControler->UpDateGridDraw();
	
		DrawSituation(TRUE, TRUE, TRUE, TRUE);
	}

    if(localClick){m_bMapMove=FALSE;ShowLocalFeature(point);}
}


BOOL CC4IMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((zDelta > 0 && m_pCoordMgr->m_fScale < -100.0) ||
		(zDelta < 0 && m_pCoordMgr->m_fScale > 2500.0))
		return FALSE;

	if (zDelta == 0)
		return FALSE;


	double geo_min_x;
	double geo_max_x;
	double geo_min_y;
	double geo_max_y;

	double scr_min_x;
	double scr_max_x;
	double scr_min_y;
	double scr_max_y;

	GetClientRect(&rect);
	m_pCoordMgr->ScreenXY2WorldXY(rect.left, rect.top, &geo_min_x, &geo_min_y);
	m_pCoordMgr->ScreenXY2WorldXY(rect.right, rect.bottom, &geo_max_x, &geo_max_y);

	m_fPrevScale = m_pCoordMgr->m_fScale;

	double	fXCoord, fYCoord;
	m_pCoordMgr->ScreenXY2WorldXY(m_pCoordMgr->m_nDisplayViewWidth / 2, m_pCoordMgr->m_nDisplayViewHeight / 2, &fXCoord, &fYCoord);

	float	fZoomRatio = (zDelta / abs(zDelta) * (-1)) * m_pCoordMgr->m_fScale / 3.0;
	m_pCoordMgr->m_fScale = m_pCoordMgr->m_fScale + fZoomRatio;
	if (m_pCoordMgr->m_fScale > 2500)
		m_pCoordMgr->m_fScale = 2500;

	long nCenterX, nCenterY;
	m_pCoordMgr->WorldXY2ScreenXY(fXCoord, fYCoord, &nCenterX, &nCenterY);
	m_pCoordMgr->SetNewCenter(nCenterX, nCenterY);

	m_pCoordMgr->WorldXY2ScreenXY(geo_min_x, geo_min_y, &scr_min_x, &scr_min_y, nCenterX, nCenterY, m_pCoordMgr->m_fScale);
	m_pCoordMgr->WorldXY2ScreenXY(geo_max_x, geo_max_y, &scr_max_x, &scr_max_y, nCenterX, nCenterY, m_pCoordMgr->m_fScale);

	/*
	if (m_bRaster != 0)
	{
		if (m_fPrevScale < 2.3 && m_pCoordMgr->m_fScale >= 2.3)
		{
			m_bRaster = FALSE;
			//OnShowRaster();
		}
		else if (m_fPrevScale >= 2.3 && m_pCoordMgr->m_fScale < 10.0)
		{
			m_bRaster = FALSE;
			//OnShowRaster();
		}
		else if (m_fPrevScale < 10.0 && m_pCoordMgr->m_fScale >= 10.0)
		{
			m_bRaster = FALSE;
			//OnShowRaster();
		}
	}
	*/

	CString strScale;
	strScale.Format("%f", m_pCoordMgr->m_fScale);
	//get_mainfrm()->m_wndStatusBar.SetPaneText(5, strScale);

	if (zDelta < 0)
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CC4IMapView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptPrevPoint = point;
	m_ptMapMoveFirstPoint = point;

	//	if(m_bRaster != 0)
	//		return;

	if (m_bPanning)
	{
		m_bMapMove = TRUE;
		SetCapture();
	}


	CView::OnMButtonDown(nFlags, point);
}


void CC4IMapView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bPanning)
	{
		ReleaseCapture();
	}

	// if(m_bMapMove) m_bMapMove = FALSE;

	if (m_bMapMove && point != m_ptMapMoveFirstPoint)
	{
		CPoint ptNewPoint;
		ptNewPoint.x = -point.x + m_ptMapMoveFirstPoint.x;
		ptNewPoint.y = -point.y + m_ptMapMoveFirstPoint.y;


		m_pCoordMgr->m_fCenterX -= ptNewPoint.x;
		m_pCoordMgr->m_fCenterY -= ptNewPoint.y;

		//m_pOverlayThread->RePosition();
		//if (m_pDlgOverlayControler->IsWindowVisible() == TRUE)
		//	m_pDlgOverlayControler->UpDateGridDraw();

		DrawSituation(TRUE, TRUE, TRUE, TRUE);
		m_bMapMove = FALSE;
	}

	CView::OnMButtonUp(nFlags, point);
}


MOUSE_STATE CC4IMapView::GetMouseState()
{
	return m_pMouseState;
}
void CC4IMapView::SetMouseState(MOUSE_STATE pState)
{

	m_pMouseState = pState;

	// 좌표추출 이벤트 처리 2014-05-09
	if (pState == MOUSE_COORD_INPUT && m_pMouseStateCoordOld != MOUSE_COORD_INPUT)
	{
		m_pMouseStateCoordOld = pState;
#ifdef _DEBUG
		TRACE("[좌표추출 시작]\n");
#endif // _DEBUG
	}
	else
	{
		//if (pState != MOUSE_COORD_INPUT && m_pMouseStateCoordOld == MOUSE_COORD_INPUT)
		//{
		//	if (m_pTracker)
		//	{
		//		m_pTracker->TrackerPointClip();
		//	}
		//	m_pMouseStateCoordOld = pState;

#ifdef _DEBUG
			TRACE("[좌표추출 종료변경]\n");
#endif // _DEBUG
		//}
	}

#ifdef _DEBUG
	TRACE("CURSOR %d\n", m_pMouseState);
#endif

	//if (m_pMeasure != NULL) {

	//	// Measure Draw
	//	ClearMeasureTemp();
	//	if (m_pMeasure->GetCurrentMode() != MEASURE_MODE_NONE)
	//		Invalidate(FALSE);
	//	m_pMeasure->SetCurrentMode(MEASURE_MODE_NONE);
	//	m_bNewCopPanning = FALSE;
	//}

	//m_pSearchOption = SEARCH_NONE;
}

BOOL CC4IMapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HINSTANCE hOrgInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(AfxGetInstanceHandle());
	switch (m_pMouseState)
	{
	case MOUSE_DEFAULT:// 기본
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DEFAULT));
		break;

	case MOUSE_ZOOMIN:// 확대
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN));
		break;

	case MOUSE_ZOOMOUT:// 축소
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT));
		break;
	case MOUSE_PANNING:// 이동
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PANNING));
		break;
	case MOUSE_ROTATE:// 회전
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ROTATE));
		break;
	case MOUSE_PRINT: // 인쇄 영역 선택
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_RECT));
		break;

	case MOUSE_SEARCH_POINT: // 속성 조회
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_INFO));
		break;

	case MOUSE_SEARCH_CIRCLE: //공간검색 원형
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CIRCLE));
		break;

	case MOUSE_SEARCH_RECTANGLE: //공간검색 사각형
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_RECT));
		break;

	case MOUSE_SEARCH:// 지형 검색 (점, 원, 사각형)
	{
		switch (m_pSearchOption)
		{
		case SEARCH_POINT:
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_INFO));
			break;
		case SEARCH_CIRCLE:
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_CIRCLE));
			break;
		case SEARCH_RECT:
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_RECT));
			break;
		case SEARCH_POLYGON:
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_POLYGON));
			break;
		default:
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DEFAULT));
			break;
		}
	}
	break;

	case MOUSE_DISTANCE:// 거리측정
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LENGTH));
		break;

	case MOUSE_AREA:// 면적측정
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_AREA));
		break;

	case MOUSE_SECTION:// 단면도측정
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_SECTION));
		break;

	case MOUSE_VISIBLE:// 가시권 분석
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VISIBLE));
		break;

	case MOUSE_RADIUS:// 반경 표시
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VISIBLE));
		break;
	case MOUSE_SPACEFILTER_CIRCLE:	// 08.22 공간필터 원 중심 설정
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_VISIBLE));
		break;

	case MOUSE_TERRAIN:// 기타 영역 선택 분석
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_TERRAIN));
		break;

	case MOUSE_OVERLAY:// 투명도
		//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_OVER));
		break;

	case MOUSE_COPLAY://상황도
		//	SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_COP));
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DEFAULT));
		break;
	case MOUSE_COORD_INPUT:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_COORD_INPUT));
		break;

	default:
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DEFAULT));
		break;
	}

	AfxSetResourceHandle(hOrgInstance);

#ifdef _DEBUG
	//TRACE("CURSOR %d\n", m_pMouseState);
#endif

	return TRUE;
}