//---------------------------------------
// 작성자			: 유영호
// 클래스명			: COverlayEventMgr
// 목적				: 투명도의 마우스 이벤트 관리
// 생성일			: 2009-05-24
// 최종수정자		: 유영호
// 최종수정일		: 2009-05-24
//---------------------------------------

#pragma once

#include "Object.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif


#define MAX_OVERLAY_EVENT		100

namespace OVERLAY
{
	enum AFX_DATA E_OVERLAY_EVENT
	{
		OVERLAY_MOUSE_LBUTTON_DOWN,
		OVERLAY_MOUSE_LBUTTON_UP,
		OVERLAY_MOUSE_RBUTTON_DOWN,
		OVERLAY_MOUSE_RBUTTON_UP,
		OVERLAY_MOUSE_LBUTTON_DCLICK,
		OVERLAY_MOUSE_RBUTTON_DCLICK,
		OVERLAY_MOUSE_MBUTTON_DOWN,
		OVERLAY_MOUSE_MBUTTON_UP,
		OVERLAY_MOUSE_MOVE,
		OVERLAY_MOUSE_WHEEL,
		OVERLAY_MULTI_SELECT,
		OVERLAY_RELEASE_SELECT,
		OVERLAY_CREATE_NEW,
		OVERLAY_DELETE_OBJECT,
		OVERLAY_CANCEL,
		OVERLAY_ZOOM_IN_OUT,
		OVERLAY_REDRAW,
		OVERLAY_MOUSE_INVALID,
		OVERLYA_REDRAW,
		OVERLAY_REPOSITION,
		OVERLYA_DRAW_GRID,
		OVERLYA_SET_GRID,
	};

	enum AFX_DATA EVENTS_FOR_OBJECT_DRAW
	{
	};

	class AFX_DATA COverlayEvent
	{
	public:
		COverlayEvent();
		COverlayEvent(E_OVERLAY_EVENT event_in, CPoint pt);
		COverlayEvent(UINT nFlags_, OVERLAY::OBJECT_TYPE eType, UINT nRepCnt_);
		COverlayEvent(E_OVERLAY_EVENT event_in, CPoint pt, UINT nFlags_);

		~COverlayEvent();

	public:
		E_OVERLAY_EVENT					e_evnet;
		UINT							nFlags;
		OVERLAY::OBJECT_TYPE			eObjectType;
		UINT							nRepCnt;
		CPoint							point;
		HDC								hDC;
		INT								nX;
		INT								nY;
		// Grid
		BOOL							bGrid;
		HWND							hWnd;
		UINT							nDetail;

		char							lpszFileName[512];
	};

	class AFX_DATA COverlayEventMgr
	{
	public:
		COverlayEventMgr();
		~COverlayEventMgr();

	private:
		COverlayEvent	m_nMouseEventQueue[MAX_OVERLAY_EVENT];
		INT		m_nPopPosition;
		INT		m_nPushPosition;
		INT		m_nSize;

		void	clear();
	public:

		BOOL push(COverlayEvent OverlayEvent);
		COverlayEvent* pop();
		COverlayEvent* get_next();
		INT	size();

	};
}


#undef	AFX_DATA
#define	AFX_DATA
