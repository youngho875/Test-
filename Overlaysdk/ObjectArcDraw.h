#pragma once

#include "ObjectDraw.h"
#include "ObjectArc.h"


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CObjectArcDraw
// 기능				: 도형 그리기
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{
	class AFX_DATA CObjectArcDraw : public CObjectDraw
	{
	public:
		CObjectArcDraw();
		~CObjectArcDraw();
		void Draw(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		void DrawOutLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		void DrawSelectPoint(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);
		void DrawMap(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
	private:
		void DrawRotate(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution);
		void DrawObject(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution);
	};
}

#undef	AFX_DATA
#define	AFX_DATA