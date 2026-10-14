#pragma once

#include "ObjectProperty.h"
#include "Object.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

namespace OVERLAY
{
	class AFX_DATA ObjectRoundRect : public OBJECT
	{
	public:
		ObjectRoundRect();
		virtual ~ObjectRoundRect();
		virtual void Move(CPoint ptMove, OUT_LINE nKindOfSelectedPoint, UINT nIndex = 0);
	};
}

#undef	AFX_DATA
#define	AFX_DATA