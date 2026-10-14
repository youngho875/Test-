#pragma once

#include "ObjectLinkedLine.h"
#include "ObjectLinkedLineDraw.h"

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
	class AFX_DATA CObjectLinkedMultiCurveDraw : public CObjectLinkedLineDraw
	{
	public:
		CObjectLinkedMultiCurveDraw();
		virtual ~CObjectLinkedMultiCurveDraw();
	protected:
	};
}

#undef	AFX_DATA
#define	AFX_DATA