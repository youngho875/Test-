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
// 클래스명			: DrawEditField
// 기능				: 편집하고자 하는 도형의 외곽선과 도형을 함께 그린다.
// 비고				: 편집중인 도형이 도시되는 버퍼
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------
namespace OVERLAY
{
	class AFX_DATA DrawEditField : public DrawField
	{
	public:
		DrawEditField(Gdiplus::Graphics& g, long lw, long lh);
		virtual ~DrawEditField(void);

		//---------------------------------------
		// 함수명	: BOOL Draw(도형);
		// 기능		: 도형 그리기
		//--------------------------------------
		virtual BOOL Draw(OVERLAY::OBJECT* pObject);

	protected:
	};
}
#undef	AFX_DATA
#define	AFX_DATA