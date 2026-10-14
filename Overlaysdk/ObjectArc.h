#pragma once

#include "ObjectProperty.h"
#include "Object.h"


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

class CObjectArcDraw;

namespace OVERLAY
{
	class AFX_DATA ObjectArc : public OBJECT
	{
	public:
		friend class CObjectArcDraw;
		ObjectArc();
		virtual ~ObjectArc();
		//---------------------------------------
		// 함수명	: void SetOutLine();
		// 기능		: 도형 외곽선 계산
		//--------------------------------------
		virtual void SetOutLine();

		//---------------------------------------
		// 함수명	: void SetSweepAngle();
		// 기능		: 호를 형성하는 점(도형 변경점)의 각도를 설정한다.
		//--------------------------------------
		void SetSweepAngle(double Angle_1, double Angle_2);
		//---------------------------------------
		// 함수명	: void GetSweepAngle();
		// 기능		: 호를 형성하는 점(도형 변경점)의 각도를 얻어온다.
		//--------------------------------------점
		void GetSweepAngle(double* Angle_1, double* Angle_2);
		//---------------------------------------
		// 함수명	: void GetSweepAngle();
		// 기능		: 호를 형성하는 점(도형 변경점)을 선택하여 그 지점의 각도를 얻어온다.
		//--------------------------------------
		void GetSweepAngle(UINT nIndex, double* Angle);
		//---------------------------------------
		// 함수명	: void GetSweepAngle(도형Index);
		// 기능		: 호를 형성하는 점(도형 변경점)을 선택하여 그 지점의 각도를 얻어온다.
		// 반환		: 각도
		//--------------------------------------
		double GetSweepAngle(UINT nIndex);
		//---------------------------------------
		// 함수명	: void Move(이동 지점, 선택된 점의 종류, 지정된 점);
		// 기능		: 도형 편집점(도형 외관선) 이동 편집
		//--------------------------------------
		virtual void Move(CPoint ptMove, OUT_LINE nKindOfSelectedPoint, UINT nIndex = 0);

	public:
		double		m_dSweepAngle[2];
	};
}

#undef	AFX_DATA
#define	AFX_DATA