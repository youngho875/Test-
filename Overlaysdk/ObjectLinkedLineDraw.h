#pragma once

#include "ObjectDraw.h"
#include "ObjectLinkedLine.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CObjectDraw
// 목적				: 도형 그리기 
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{
	class AFX_DATA CObjectLinkedLineDraw : public CObjectDraw
	{
	public:
		CObjectLinkedLineDraw();
		virtual ~CObjectLinkedLineDraw();
		virtual void DrawOutLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);
		virtual void DrawSelectPoint(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);
		virtual void Draw(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		virtual void DrawLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);


	protected:
	};
}

#undef	AFX_DATA
#define	AFX_DATA