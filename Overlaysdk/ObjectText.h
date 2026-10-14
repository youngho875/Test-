#pragma once

#include "ObjectProperty.h"
#include "Object.h"
#include "ObjectTextDraw.h"

#define MAX_FONT_FAMILY_NAME		5


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif


namespace OVERLAY
{
	AFX_DATA extern const char* strFontStyle[];
	AFX_DATA extern const WCHAR* strFamilyName[];

	enum E_FAMILY_NAME_
	{
		FAMILY_NAME_GOOLIM,
		FAMILY_NAME_GODICK,
		FAMILY_NAME_DODWOOM,
		FAMILY_NAME_BATANG,
	};

	class AFX_DATA ObjectText : public OBJECT
	{
	public:
		friend class CObjectTextDraw;
		ObjectText();
		virtual ~ObjectText();

	private:
		double				m_dFontSize;
		COLORREF			m_FontColor;
		E_FAMILY_NAME_		m_eFamilyName;
		Gdiplus::FontStyle	m_eFontStyle;
		Gdiplus::Unit		m_eUnit;
		WCHAR				m_Text[MAX_TEXT_STRING];

	public:
		void SetText(WCHAR* pText, UINT nLength);
		void SetColor(COLORREF color);
		void SetSize(double dSize);
		void SetFontFamilyName(E_FAMILY_NAME_ eFontType);
		void SetFontStyle(Gdiplus::FontStyle eFontStyle);
		void SetFontUnit(Gdiplus::Unit eUnit);

		double				GetFontSize();
		COLORREF			GetFontColor();
		E_FAMILY_NAME_		GetFamilyName();
		Gdiplus::FontStyle	GetFontStyle();
		Gdiplus::Unit		GetUnit();
		WCHAR* GetText();
		int					GetSize();
	};
}

#undef	AFX_DATA
#define	AFX_DATA