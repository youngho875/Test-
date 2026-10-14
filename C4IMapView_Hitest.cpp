#include "pch.h"
#include "C4IMapViewHeader.h"



BOOL CC4IMapView::HitestDisplayPoint(double fXCoord, double fYCoord, double fCenterX, double fCenterY, double fScale)
{
	/*
	if (m_PointDisplay.ResearchHookPoint(fXCoord, fYCoord, fCenterX, fCenterY, fScale) != -1)
	{
		m_nHookType = HOOKED_DISPLAY_POINT;
		DisplayPoint point = m_PointDisplay.GetPointByID(m_PointDisplay.m_nHookPoint);
		m_fHookPoint.x = point.fX;
		m_fHookPoint.y = point.fY;
		if (m_nHookSDLSymbol != -1 && m_nHookSDL != -1) {
			//m_SDLPointObjects[m_nHookSDL].m_nHookSymbol = -1;
			m_nHookSDL = -1;
		}

		//m_TrackMgr.SetProjectedVector(FALSE);
		//m_TrackMgr.SetHookTrackAsNumber(0);
		//m_SpecialPointMgr.m_nHookSpecialPoint = -1;
		//		DrawSituation(FALSE, TRUE, TRUE, TRUE);
		return TRUE;
	}
	else
	{
		m_PointDisplay.m_nHookPoint = -1;
	}
	*/
	return FALSE;
}