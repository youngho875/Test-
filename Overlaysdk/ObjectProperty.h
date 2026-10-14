#pragma once

#include "OverlayDefine.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

namespace OVERLAY
{

	enum LINE_STYLE
	{
		Solid,          // 0
		Dash,           // 1
		Dot,            // 2
		DashDot,        // 3
		DashDotDot,     // 4
		Custom          // 5
	};

	enum DashCap
	{
		DashCapFlat = 0,
		DashCapRound = 2,
		DashCapTriangle = 3
	};

	enum LineCap
	{
		LineCapFlat = 0,
		LineCapSquare = 1,
		LineCapRound = 2,
		LineCapTriangle = 3,

		LineCapNoAnchor = 0x10, // corresponds to flat cap
		LineCapSquareAnchor = 0x11, // corresponds to square cap
		LineCapRoundAnchor = 0x12, // corresponds to round cap
		LineCapDiamondAnchor = 0x13, // corresponds to triangle cap
		LineCapArrowAnchor = 0x14, // no correspondence

		LineCapCustom = 0xff, // custom cap

		LineCapAnchorMask = 0xf0  // mask to check for anchor or not.
	};

	class AFX_DATA ObjectProperty
	{
	public:
		ObjectProperty();
		~ObjectProperty();
		void InitProperty();
		void SetStartLineStyle(LineCap eLineStyle);
		void SetEndLineStyle(LineCap eLineStyle);
		void SetTension(double dTension_);

		COLORREF		LineColor;
		COLORREF		FillColor;

		LINE_STYLE		eLineStyle;
		UINT			FillStyle;

		double			dLineWidth;
		double			dAngle;

		UINT			nStartLineArrowStyle;
		UINT			nEndLineArrowStyle;

		LineCap			eStartLineStyle;
		LineCap			eEndLineStyle;
		DashCap			eDashStyle;
		BOOL			bIsFill;
		BOOL			bIsLine;

		double			dTension;
	};
}

#undef	AFX_DATA
#define	AFX_DATA