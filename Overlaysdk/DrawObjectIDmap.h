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
// 기능				: 도형을 선택하기위해 레이어에 해당 도형의 ID로 선택영역을 그린다.
// 비고				: 최상위 레이어의 도형들의 ID값을 지정해준다.
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------
namespace OVERLAY
{
	class AFX_DATA DrawObjectIdMap : public DrawField
	{	
	public:
		DrawObjectIdMap(Gdiplus::Graphics& g, long lw, long lh);
		~DrawObjectIdMap(void);

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
	};
}
#undef	AFX_DATA
#define	AFX_DATA