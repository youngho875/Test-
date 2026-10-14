#pragma once


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: DrawGridField
// 기능				: 도형 격자를 그린다.
// 비고				: 도형 편집용 격자가 그려질 레이어
// 생성일			: 2009-6-2
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------
namespace OVERLAY
{
	class AFX_DATA DrawGridField
	{
	public:
		Gdiplus::Bitmap		BMP;
		DrawGridField(Gdiplus::Graphics& g, long lw, long lh);
		virtual ~DrawGridField(void);

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
		// 함수명	: Gdiplus::Bitmap* GetImage();
		// 기능		: 비트멥 이미지 가져오기
		//--------------------------------------
		Gdiplus::Bitmap* GetImage();
		//---------------------------------------
		// 함수명	: void DrawGrid();
		// 기능		: 격자 그리기.
		//--------------------------------------
		void DrawGrid(HWND hWnd, UINT nDetail);

	protected:
		Gdiplus::Graphics	GRP;
		int					m_nIsLatLong;
		BOOL				m_bIsFirst;
		double dCenterWorldX;
		double dCenterWorldY;
		double dLeftWorldX;
		double dLeftWorldY;
		double dRightWorldX;
		double dRightWorldY;
		double dTopWorldX;
		double dTopWorldY;
		double dBottomWorldX;
		double dBottomWorldY;
		double dScreenX;
		double dScreenY;
	};
}

#undef	AFX_DATA
#define	AFX_DATA
