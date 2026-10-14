#pragma once

#include "ObjectDraw.h"
#include "ObjectCurveDraw.h"
#include "ObjectArcDraw.h"
#include "ObjectPieDraw.h"
#include "ObjectEllipseDraw.h"
#include "ObjectLineDraw.h"
#include "ObjectRectDraw.h"
#include "ObjectRoundRectDraw.h"
#include "ObjectTriangleDraw.h"
#include "ObjectDiamondDraw.h"
#include "ObjectPentagonDraw.h"
#include "ObjectSixAngleDraw.h"
#include "ObjectEightAngleDraw.h"
#include "ObjectImageDraw.h"
#include "ObjectTextDraw.h"
#include "ObjectPolygonDraw.h"
#include "ObjectPolyLineDraw.h"
#include "ObjectLinkedLineDraw.h"
#include "ObjectLinkedMultiLineDraw.h"
#include "ObjectLinkedCurveDraw.h"
#include "ObjectLinkedMultiCurveDraw.h"
#include "DrawBasicField.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: DrawField
// 기능				: 도형을 그린다.
// 비고				: 최상위 레이어를 제외한 모든 레이어의 도형이 도시되는 버퍼
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------

namespace OVERLAY
{
	class AFX_DATA DrawField : public DrawBasicField
	{
	public:
		DrawField(Gdiplus::Graphics& g, long lw, long lh);
		virtual ~DrawField(void);

		//---------------------------------------
		// 함수명	: BOOL Draw(도형);
		// 기능		: 도형 그리기
		//--------------------------------------
		virtual BOOL Draw(OVERLAY::OBJECT* pObject);
		//---------------------------------------
		// 함수명	: SHORT GetObjectID(X좌표, Y좌표);
		// 기능		: X,Y좌표 위치하는 도형중 최상위에 위치해 있는 도형의 ID를 가져온다.
		// 반환		: 도형 ID
		//--------------------------------------
		virtual SHORT GetObjectID(int x, int y);

	protected:
		// 각 도형의 그리기 객체
		CObjectArcDraw				m_ObjectArcDraw;
		CObjectPieDraw				m_ObjectPieDraw;
		CObjectCurveDraw			m_ObjectCurveDraw;
		CObjectEllipseDraw			m_ObjectEllipseDraw;
		CObjectLineDraw				m_ObjectLineDraw;
		CObjectRectDraw				m_ObjectRectDraw;
		CObjectRoundRectDraw		m_ObjectRoundRectDraw;
		CObjectTriangleDraw			m_ObjectTriangleDraw;
		CObjectDiamondDraw			m_ObjectDiamondDraw;
		CObjectPentagonDraw			m_ObjectPentagonDraw;
		CObjectSixAngleDraw			m_ObjectSixAngleDraw;
		CObjectEightAngleDraw		m_ObjectEightAngleDraw;
		CObjectImageDraw			m_ObjectImageDraw;
		CObjectTextDraw				m_ObjectTextDraw;
		CObjectPolygonDraw			m_ObjectPolygonDraw;
		CObjectPolyLineDraw			m_ObjectPolyLineDraw;
		CObjectLinkedLineDraw		m_ObjectLinkedLineDraw;
		CObjectLinkedMultiLineDraw	m_ObjectLinkedMultiLineDraw;
		CObjectLinkedCurveDraw		m_ObjectLinkedCurveDraw;
		CObjectLinkedMultiCurveDraw	m_ObjectLinkedMultiCurveDraw;
	};
}

#undef	AFX_DATA
#define	AFX_DATA
