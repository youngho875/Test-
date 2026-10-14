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
	class AFX_DATA ObjectImage : public OBJECT
	{
	public:
		ObjectImage(char* strImageFileName, int nStrImageFileNameLen);
		virtual ~ObjectImage();

		char m_strImageFileName[512];
	};
}

#undef	AFX_DATA
#define	AFX_DATA