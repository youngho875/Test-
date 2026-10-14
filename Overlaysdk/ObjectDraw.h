#pragma once

#include "Object.h"
#include "Mathematics.h"


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
	class AFX_DATA CObjectDraw : public CMathematics
	{
	public:
		CObjectDraw();
		virtual ~CObjectDraw();
		virtual void DrawOutLine(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		virtual void DrawSelectPoint(OVERLAY::OBJECT* object, Gdiplus::Graphics* g);
		virtual void DrawMap(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION);
		virtual void Draw(OVERLAY::OBJECT* object, Gdiplus::Graphics* g, int nResolution = DRAW_RESOLUTION) = 0;

	protected:
		// 0. 회전
		void Rotate(double cx, double cy, double dAngle, int nAngleNum, Gdiplus::PointF* pt, Gdiplus::Graphics* g);
		void GetEllipsePositon(double a, double b, double cx, double cy, double theta, double* x, double* y);

	};
}

#undef	AFX_DATA
#define	AFX_DATA