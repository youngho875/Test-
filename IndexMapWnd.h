#pragma once

#include "GPoint.h"
#include "GeoDC/Coord.h"
#include "GeoDC/CoordMgr.h"
#include "ComDefStringValue.h"

// MiniMap


class CIndexMapWnd : public CWnd
{
	DECLARE_DYNAMIC(CIndexMapWnd)

public:
	CIndexMapWnd();
	virtual ~CIndexMapWnd();

public:
	CCoordMgr* m_pCoordMgr;
	CCoord	m_Coord; 

	CPoint	m_ptMapMoveFirstPoint;
	CPoint  m_ptPrevPoint;

protected:
	MOUSE_STATE m_emMouseState;

	//CC4IMapView* m_pParentWnd;
	
	double m_fPrevScale;
	CPoint m_ptOld;
	int m_nMoveCount;
	CPoint m_ptZoomEnd;
	CPoint m_ptZoomStart;
	CRgn m_rgnUdoMemo;
	int m_nLockTrkno;
	BOOL m_bOnZoom;
	BOOL m_bAltAlways;
	CImageList m_imgDrag;
	CString m_strDE;
	CString m_strTitle;
	BOOL m_bMapRedraw;
	BOOL m_bMapErase;
	//CDisplayPlane m_DP;

	GPoint m_fHookPoint;
	CPoint m_ptBearingStart, m_ptStart, m_ptEnd;

	int m_nHookType;
	CBitmap m_bmpMap;	

	//BOOL m_bCardIndicator;
	//BOOL m_bLButtonDownIndicator;
	//BOOL m_bZoomIndicator;
	//BOOL m_bMovingIndicator;
	//BOOL m_bWheelRotationIndicator;

public:
	void DrawZoomRect(CDC* pDC, const CRect& rect);
	void DrawMap(CDC* pDC);
	void DrawMapBitmap(CDC* pDC);

protected:
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL CreateEx(CWnd* pParentWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


