#pragma once

#include "ObjectProperty.h"
#include "Object.h"


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif


class CObjectPieDraw;
namespace  OVERLAY
{
	class AFX_DATA ObjectPie : public OBJECT
	{
	public:
		friend class CObjectPieDraw;
		ObjectPie();
		virtual ~ObjectPie();

		virtual void SetOutLine();
		virtual void Move(CPoint ptMove, OUT_LINE nKindOfSelectedPoint, UINT nIndex = 0);

		void SetSweepAngle(double Angle_1, double Angle_2);
		void GetSweepAngle(double* Angle_1, double* Angle_2);
		void GetSweepAngle(UINT nIndex, double* Angle);
		double GetSweepAngle(UINT nIndex);

	public:
		double		m_dSweepAngle[2];
	};
}

#undef	AFX_DATA
#define	AFX_DATA