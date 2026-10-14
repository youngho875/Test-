#pragma once

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: --
// 목적				: 투명도 관련 설정값 정의
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

#define		MOUSE_MOVE						0								// 마우스 이벤트(마우스 이동)
#define		LEFT_BUTTON_DOWN				1								// 마우스 이벤트(마우스 Left button down)
#define		LEFT_BUTTON_UP					2								// 마우스 이벤트(마우스 Left button Up)
#define		RIGHT_BUTTON_DOWN				3								// 마우스 이벤트(마우스 Right button down)
#define		RIGHT_BUTTON_UP					4								// 마우스 이벤트(마우스 Right button Up)
#define		LEFT_BUTTON_DCLICK				5								// 마우스 이벤트(마우스 Right button double click)


#define		MAX_SCREEN_X					2048							// 전시화면 크기(가로)
#define		MAX_SCREEN_Y					2048							// 전시화면 크기(세로)
#define		MAX_ALLOC_OBJECT_ID				OBJECT_PER_LAYER*MAX_LAYER		// 투명도 도형의 최대개수.


#define		LEFT_UP							0								// 도형 드래그 시작지점.
#define		RIGHT_DOWN						1								// 도형 드래그 끝지점.
#define		TRIANGLE_VERTEX					2								// 삼각형의 꼭지점.
#define		ROUND_RECT_VERTEX				2								// 모서리가 둥근 사각형의 곡선 설정 점.
#define		SWEEP_1_VERTEX					2								// 부채꼴을 이루는 선분 1의 화면상의 각.
#define		SWEEP_2_VERTEX					3								// 부채꼴을 이루는 선분 2의 화면상의 각
#define		ARROW_1							2								// 시작점의 화살표
#define		ARROW_2							3								// 끝점의 화살표

#define		OBJECT_PER_LAYER				50								// 레이어당 도형의 개수
#define		MAX_LAYER						10								// 최대 레이어 개수
#define		MAX_POINTS						50								// 도형을 이루는 최대 점의 개수
#define		MAX_OBJECT						OBJECT_PER_LAYER*MAX_LAYER		// 전시할수 있는 최대 도형의 개수

#define		SIZE_OF_OUT_LINE_POINT			10								// 도형 편집영역 점의 굵기
#define		EDIT_LINE_WIDTH					1								// 도형 편집영역 굵기
#define		EDIT_SELECT_LINE_WIDTH			10								// 도형 편집선택 영역 굵기/눈에 보이지 않음

#define		OUT_LINE_COLOR_A				(BYTE)127						// 도형 편집영역 색상
#define		OUT_LINE_COLOR_R				(BYTE)255						// 도형 편집영역 색상
#define		OUT_LINE_COLOR_B				(BYTE)127						// 도형 편집영역 색상
#define		OUT_LINE_COLOR_G				(BYTE)127						// 도형 편집영역 색상
#define		OUT_LINE_POINT_COLOR_A			(BYTE)255						// 도형 편집영역 색상
#define		OUT_LINE_POINT_COLOR_R			(BYTE)0							// 도형 편집영역 색상
#define		OUT_LINE_POINT_COLOR_G			(BYTE)255						// 도형 편집영역 색상
#define		OUT_LINE_POINT_COLOR_B			(BYTE)0							// 도형 편집영역 색상
#define		OUT_LINE_POINT_OPTION_COLOR_A	(BYTE)127						// 도형 모양 변경점 편집영역 색상(삼각형, 모서리가 둥근 사각형, 부채꼴..) 
#define		OUT_LINE_POINT_OPTION_COLOR_R	(BYTE)255						// 도형 모양 변경점 편집영역 색상
#define		OUT_LINE_POINT_OPTION_COLOR_G	(BYTE)0							// 도형 모양 변경점 편집영역 색상
#define		OUT_LINE_POINT_OPTION_COLOR_B	(BYTE)277						// 도형 모양 변경점 편집영역 색상

#define		OFF_SET_FROM_TOP				30								// 도형 회전편집 지점
#define		ID_BASE_R						123								// 도형을 선택할때 선택된 도형의 구분하기위해 설정된 기준 Red값
#define		SELECT_LINE_WIDTH				10								// 도형의 선을 선택시 

#define		MAX_LINKED_OBJECT				OBJECT_PER_LAYER				// 연결선의 점의 최대수
#define		DEFAULT_LINKED_OBJECT			2								// 연결선 생성시 도형과 연결되는 연결선의 점의 수

#define		 MAX_TEXT_STRING				512								// Text Object의 문자열

#define		FONT_RESOLUTION					0.7f							// Font크기와 사각영역의 도시 비율

#define		DRAW_RESOLUTION					10								// 값이 작을수록 도형이 정교해 진다.

