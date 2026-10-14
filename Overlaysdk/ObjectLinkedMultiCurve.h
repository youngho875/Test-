#pragma once

#include "ObjectLinkedLine.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: --
// 목적				: 도형 구조 정의
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{
	class AFX_DATA ObjectLinkedMultiCurve : public ObjectLinkedLine
	{
	public:
		ObjectLinkedMultiCurve(UINT nNumberOfPoint);
		virtual ~ObjectLinkedMultiCurve();

		void SetSecondLinePoint(Point* pPoint);
		Point* GetSecondPoint();
	};
}

#undef	AFX_DATA
#define	AFX_DATA