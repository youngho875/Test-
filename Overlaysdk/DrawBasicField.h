#pragma once


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: DrawBasicField
// 기능				: 도형을 화면에 도시하기 위한 레이어를 정의 하며 도형을 그리다.
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------
namespace OVERLAY
{
	class AFX_DATA DrawBasicField
	{
	public:
		Gdiplus::Bitmap		BMP;

		DrawBasicField(Gdiplus::Graphics& g, long lw, long lh);
		virtual ~DrawBasicField(void);

		//---------------------------------------
		// 함수명	: void Clear();
		// 기능		: 레이어 클리어
		//--------------------------------------
		virtual void Clear();

		//---------------------------------------
		// 함수명	: void ClearColor();
		// 기능		: 선택된 색상으로 레이어 클리어
		//--------------------------------------
		virtual void ClearColor(Gdiplus::Color color);

		//---------------------------------------
		// 함수명	: void DrawImages(비트멥 이미지);
		// 기능		: 비트멥 이미지 그리기
		//--------------------------------------
		void DrawImages(Gdiplus::Bitmap& bitmap);

		//---------------------------------------
		// 함수명	: Gdiplus::Bitmap* GetImage();
		// 기능		: 비트멥 이미지 가져오기
		//--------------------------------------
		Gdiplus::Bitmap* GetImage();

		//---------------------------------------
		// 함수명	: HDC GetHDC();
		// 기능		: HDC 가져오기.
		//--------------------------------------
		HDC GetHDC();

	protected:
		Gdiplus::Graphics	GRP;

	};
}

#undef	AFX_DATA
#define	AFX_DATA				   
