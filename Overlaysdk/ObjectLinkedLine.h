#pragma once

#include "Object.h"

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
// 생성일			: 2009-06-24
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{

	class AFX_DATA ObjectLinkedLine : public OBJECT
	{
	public:
		ObjectLinkedLine(UINT nNumberOfPoint);
		ObjectLinkedLine(UINT nNumberOfPoint, OVERLAY::OBJECT_TYPE eType);
		virtual ~ObjectLinkedLine();

		void SetLineStyle(LINE_STYLE eLineStyle);
		void SetLineColor(COLORREF LineColor);
		void SetStartArrow(LineCap eLineStyle);
		void SetEndArrow(LineCap eLineStyle);

		Point* GetStartPoint();
		Point* GetEndPoint();

		void SetLinkPoint(UINT nIndex, OVERLAY::Point* pPoint);
		void SetPoint(UINT nIndex, int x, int y);

		OVERLAY::Point** GetLinkedLinePoint();
	protected:
		Point* pObjectPoint[MAX_LINKED_OBJECT];

	public:
		UINT	nLinkedLine_Start_ID;		// 연견될 도형의 ID
		UINT	nLinkedLine_End_ID;			// 연견될 도형의 ID

		UINT	nObject_start_point_id;	// 도형의 연결지점. pt[nObject_start_point_index]
		UINT	nObject_end_point_id;	// 도형의 연결지점. pt[nObject_end_point_index]
	};
}

#undef	AFX_DATA
#define	AFX_DATA