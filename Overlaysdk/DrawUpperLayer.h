#pragma once

#include "DrawField.h"

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: DrawField
// 기능				: 최상위 레이어의 도형이 도시되는 버퍼
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------
namespace OVERLAY
{
	class AFX_DATA DrawUpperLayer : public DrawField
	{
	public:
		DrawUpperLayer(Gdiplus::Graphics& g, long lw, long lh);
		~DrawUpperLayer(void);
		//---------------------------------------
		// 함수명	: BOOL Draw(도형);
		// 기능		: 도형 그리기
		//--------------------------------------
		virtual BOOL Draw(OVERLAY::OBJECT* pObject);
	};
}

#undef	AFX_DATA
#define	AFX_DATA