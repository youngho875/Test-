#pragma once

#include "ObjectDraw.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CObjectLineDraw
// 목적				: 도형 편집 연산기
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{
	class AFX_DATA CObjectLineDraw : public CObjectDraw
	{
	public:
		CObjectLineDraw();
		~CObjectLineDraw();
		void Draw(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		void DrawOutLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		void DrawSelectPoint(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);

	private:
		void DrawLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);
	};
}

#undef	AFX_DATA
#define	AFX_DATA