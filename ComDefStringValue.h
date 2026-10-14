#pragma once

// 공통 사용 값 정의

#pragma once

// 프로그램 타이틀
#define DEF_MAPXVIEWER_TITLE	"MapX Viewer 3.5"

// 메뉴 스트링
#define DEF_RBAR_SCREEN_CATEGORY				"화면"
#define DEF_RBAR_SCREEN_PANEL1					"지도 탐색"
#define DEF_RBAR_SCREEN_PANEL1_CMD1				"기본"
#define DEF_RBAR_SCREEN_PANEL1_CMD2				"확대"
#define DEF_RBAR_SCREEN_PANEL1_CMD3				"축소"
#define DEF_RBAR_SCREEN_PANEL1_CMD4				"이동"
#define DEF_RBAR_SCREEN_PANEL1_CMD5				"좌표 이동"
#define DEF_RBAR_SCREEN_PANEL1_CMD6				"이전 화면"
#define DEF_RBAR_SCREEN_PANEL1_CMD7				"다음 화면"
#define DEF_RBAR_SCREEN_PANEL1_CMD8				"초기 화면"
#define DEF_RBAR_SCREEN_PANEL2					"지도 설정"
#define DEF_RBAR_SCREEN_PANEL2_CMD1				"선택"
#define DEF_RBAR_SCREEN_PANEL2_CMD2				"축척"
#define DEF_RBAR_SCREEN_PANEL2_CMD2_SUB1		"축척 범위 :"
#define DEF_RBAR_SCREEN_PANEL2_CMD2_SUB2		"축척 간격 :"
#define DEF_RBAR_SCREEN_PANEL2_CMD2_SUB3		"도시 축척 :"
#define DEF_RBAR_SCREEN_PANEL2_CMD3				"회전"
#define DEF_RBAR_SCREEN_PANEL2_CMD4				"투영"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB1		"Geodetic"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB2		"UTM"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB3		"Mercator"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB4		"Lambert Conformal Conic"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB5		"StereoGraphic"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB6		"EQUIDISTANTCYLINDRICAL"
#define DEF_RBAR_SCREEN_PANEL2_CMD4_SUB7		"TM"
#define DEF_RBAR_SCREEN_PANEL2_CMD5				"축척별 자동변경"
#define DEF_RBAR_SCREEN_PANEL2_CMD5_SUB1		"자동변경 지도설정"

#define DEF_RBAR_SCREEN_PANEL3					"즐겨찾기"
#define DEF_RBAR_SCREEN_PANEL3_CMD1				"즐겨찾기 목록"
#define DEF_RBAR_SCREEN_PANEL3_CMD2				"추가"
#define DEF_RBAR_SCREEN_PANEL3_CMD3				"삭제"

#define DEF_RBAR_3DMAP_PANEL					"3차원 지도"
#define DEF_RBAR_3DMAP_PANEL_CMD1				"3차원지도 실행"

#define DEF_RBAR_VIEW_CATEGORY					"보기"
#define DEF_RBAR_VIEW_PANEL1					"그리드"
#define DEF_RBAR_VIEW_PANEL1_CMD1				"그리드 설정"
#define DEF_RBAR_VIEW_PANEL2					"좌표계"
#define DEF_RBAR_VIEW_PANEL2_CMD1				"좌표계 설정"
#define DEF_RBAR_VIEW_PANEL3					"밝기/명암"
#define DEF_RBAR_VIEW_PANEL3_CMD1				"밝기:"
#define DEF_RBAR_VIEW_PANEL3_CMD2				"명암:"
#define DEF_RBAR_VIEW_PANEL3_CMD3				"기본값"
#define DEF_RBAR_VIEW_PANEL4					"저장"
#define DEF_RBAR_VIEW_PANEL4_CMD1				"클립 보드"
#define DEF_RBAR_VIEW_PANEL4_CMD2				"이미지"
#define DEF_RBAR_VIEW_PANEL5					"인쇄"
#define DEF_RBAR_VIEW_PANEL5_CMD1				"영역 선택"
#define DEF_RBAR_VIEW_PANEL5_CMD2				"미리 보기"
#define DEF_RBAR_VIEW_PANEL6					"창 관리"
#define DEF_RBAR_VIEW_PANEL6_CMD1				"지도레이어 관리"
#define DEF_RBAR_VIEW_PANEL6_CMD2				"투명도 관리"
#define DEF_RBAR_VIEW_PANEL6_CMD3				"지도레이어 속성"
#define DEF_RBAR_VIEW_PANEL6_CMD4				"도형 속성"
#define DEF_RBAR_VIEW_PANEL6_CMD5				"인덱스 지도"
#define DEF_RBAR_VIEW_PANEL6_CMD6				"출력"

#define DEF_RBAR_SEARCH_CATEGORY				"검색"
#define DEF_RBAR_SEARCH_PANEL1					"초기화"
#define DEF_RBAR_SEARCH_PANEL1_CMD1				"초기화"
#define DEF_RBAR_SEARCH_PANEL2					"속성조회"
#define DEF_RBAR_SEARCH_PANEL2_CMD1				"속성 조회"
#define DEF_RBAR_SEARCH_PANEL3					"공간검색"
#define DEF_RBAR_SEARCH_PANEL3_CMD1				"검색 레이어"
#define DEF_RBAR_SEARCH_PANEL3_CMD2				"검색 형태"
#define DEF_RBAR_SEARCH_PANEL3_CMD2_SUB1		"사각형"
#define DEF_RBAR_SEARCH_PANEL3_CMD2_SUB2		"원"
#define DEF_RBAR_SEARCH_PANEL3_CMD3				"결과 색상"
#define DEF_RBAR_SEARCH_PANEL3_CMD4				"검색영역 선택"
#define DEF_RBAR_SEARCH_PANEL4					"지도레이어 검색"
#define DEF_RBAR_SEARCH_PANEL4_CMD1				"검색 레이어"
#define DEF_RBAR_SEARCH_PANEL4_CMD2				"검색 필드"
#define DEF_RBAR_SEARCH_PANEL4_CMD3				"검색 내용"
#define DEF_RBAR_SEARCH_PANEL4_CMD4				"지도레이어 검색"

#define DEF_RBAR_ANALYSIS_CATEGORY				"분석"
#define DEF_RBAR_ANALYSIS_PANEL1				"초기화"
#define DEF_RBAR_ANALYSIS_PANEL1_CMD1			"초기화"
#define DEF_RBAR_ANALYSIS_PANEL2				"기본"
#define DEF_RBAR_ANALYSIS_PANEL2_CMD1			"거리 측정"
#define DEF_RBAR_ANALYSIS_PANEL2_CMD2			"면적 측정"
#define DEF_RBAR_ANALYSIS_PANEL2_CMD3			"단면도 측정"
#define DEF_RBAR_ANALYSIS_PANEL3				"가시권"
#define DEF_RBAR_ANALYSIS_PANEL3_CMD1			"관측 범위(m)"
#define DEF_RBAR_ANALYSIS_PANEL3_CMD2			"관측 높이(m)"
#define DEF_RBAR_ANALYSIS_PANEL3_CMD3			"결과 색상"
#define DEF_RBAR_ANALYSIS_PANEL3_CMD4			"관측지점 선택"
#define DEF_RBAR_ANALYSIS_PANEL4				"반경 표시"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD1			"반경 간격(m)"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD2			"반경 범위(m)"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD3			"선 두께"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD4			"선 색상"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD5			"반경지점 선택"
#define DEF_RBAR_ANALYSIS_PANEL4_CMD6			"반경지점 입력"

#define DEF_RBAR_DRAW_CATEGORY					"그리기"
#define DEF_RBAR_DRAW_PANEL1					"투명도 레이어"
#define DEF_RBAR_DRAW_PANEL1_CMD1				"생성"
#define DEF_RBAR_DRAW_PANEL1_CMD2				"열기"
#define DEF_RBAR_DRAW_PANEL1_CMD3				"저장"
#define DEF_RBAR_DRAW_PANEL1_CMD4				"삭제"
#define DEF_RBAR_DRAW_PANEL1_CMD5				"눈금 설정"
#define DEF_RBAR_DRAW_PANEL1_CMD6				"실행 취소"
#define DEF_RBAR_DRAW_PANEL1_CMD7				"다시 실행"
#define DEF_RBAR_DRAW_PANEL2					"기본 도형"
#define DEF_RBAR_DRAW_PANEL2_CMD1				"사각형"
#define DEF_RBAR_DRAW_PANEL2_CMD2				"둥근 사각형"
#define DEF_RBAR_DRAW_PANEL2_CMD3				"삼각형"
#define DEF_RBAR_DRAW_PANEL2_CMD4				"다각형"
#define DEF_RBAR_DRAW_PANEL2_CMD5				"원"
#define DEF_RBAR_DRAW_PANEL2_CMD6				"타원"
#define DEF_RBAR_DRAW_PANEL2_CMD7				"부채꼴"
#define DEF_RBAR_DRAW_PANEL2_CMD8				"호"
#define DEF_RBAR_DRAW_PANEL2_CMD9				"직선"
#define DEF_RBAR_DRAW_PANEL2_CMD10				"폴리라인"
#define DEF_RBAR_DRAW_PANEL2_CMD11				"스플라인"
#define DEF_RBAR_DRAW_PANEL2_CMD12				"폴리곤"
#define DEF_RBAR_DRAW_PANEL2_CMD13				"폐곡선"
#define DEF_RBAR_DRAW_PANEL2_CMD14				"텍스트"
#define DEF_RBAR_DRAW_PANEL3					"확장 도형"
#define DEF_RBAR_DRAW_PANEL3_CMD1				"별"
#define DEF_RBAR_DRAW_PANEL3_CMD2				"섹터"
#define DEF_RBAR_DRAW_PANEL3_CMD3				"화살표"
#define DEF_RBAR_DRAW_PANEL3_CMD4				"이미지"
#define DEF_RBAR_DRAW_PANEL3_CMD5				"OLE"
#define DEF_RBAR_DRAW_PANEL3_CMD6				"군대부호"
#define DEF_RBAR_DRAW_PANEL4					"속성"
#define DEF_RBAR_DRAW_PANEL4_CMD1				"선색상"
#define DEF_RBAR_DRAW_PANEL4_CMD2				"채움색상"
#define DEF_RBAR_DRAW_PANEL4_CMD3				"패턴색상"
#define DEF_RBAR_DRAW_PANEL4_CMD4				"선굵기"
#define DEF_RBAR_DRAW_PANEL4_CMD5				"채움방법"
#define DEF_RBAR_DRAW_PANEL4_CMD6				"패턴사용"
#define DEF_RBAR_DRAW_PANEL4_CMD7				"선모양"
#define DEF_RBAR_DRAW_PANEL4_CMD8				"반투명"
#define DEF_RBAR_DRAW_PANEL4_CMD9				"패턴모양"
#define DEF_RBAR_DRAW_PANEL5					"글꼴"
#define DEF_RBAR_DRAW_PANEL5_CMD1				"글씨체"
#define DEF_RBAR_DRAW_PANEL5_CMD2				"글크기"
#define DEF_RBAR_DRAW_PANEL5_CMD3				"글색상"
#define DEF_RBAR_DRAW_PANEL6					"클립보드"
#define DEF_RBAR_DRAW_PANEL6_CMD1				"붙여넣기"
#define DEF_RBAR_DRAW_PANEL6_CMD2				"자르기"
#define DEF_RBAR_DRAW_PANEL6_CMD3				"복사"

// 색상
#define DEF_COLOR_RED		RGB(255,0,0)
#define DEF_COLOR_BLUE		RGB(0, 0, 255)
#define DEF_COLOR_BLUE2		RGB(0, 125, 255)
#define DEF_COLOR_YELLOW	RGB(255, 255, 0)
#define DEF_COLOR_BLACK		RGB(0,0,0)

// 선 스타일 정보
#define DEF_PROPERTY_LINESTYLE_SOLID		"Solid"		// _T("━━━━━━━")
#define DEF_PROPERTY_LINESTYLE_DASH			"Dash"		// _T("-- -- ---- -- --")
#define DEF_PROPERTY_LINESTYLE_DOT			"Dot"		// _T("················")
#define DEF_PROPERTY_LINESTYLE_DASHDOT		"DashDot"	// _T("· - · - · - · - ")
#define DEF_PROPERTY_LINESTYLE_DASHDOTDOT	"DashDotDot"//	_T("·· - ·· - ·· - ··")

// 겹선종류
#define	DEF_PROPERTY_LINESTYLE_STRIPE1		"SIMPLE"	//단순
#define	DEF_PROPERTY_LINESTYLE_STRIPE2		"2STRIPE"	//이중
#define	DEF_PROPERTY_LINESTYLE_STRIPE3		"THICKTHIN"	//굵고얇고
#define	DEF_PROPERTY_LINESTYLE_STRIPE4		"THINTHICK"	//얇고굵고
#define	DEF_PROPERTY_LINESTYLE_STRIPE5		"3STRIPE"	//삼중

// 선치우침상태
#define	DEF_PROPERTY_LINESTYLE_ALIGNMENT1	"CENTER"	//치우치지않음
#define	DEF_PROPERTY_LINESTYLE_ALIGNMENT2	"INSET"		//안쪽으로치우침

// 점선사이선끝모양
#define	DEF_PROPERTY_LINESTYLE_DASHCAP1		"FLAT"	//직선
#define	DEF_PROPERTY_LINESTYLE_DASHCAP2		"ROUND"	//원형
#define	DEF_PROPERTY_LINESTYLE_DASHCAP3		"TRIANGLE"	//삼각형

// 선연결모양
#define	DEF_PROPERTY_LINESTYLE_JOIN1		"MITER"			//
#define	DEF_PROPERTY_LINESTYLE_JOIN2		"BEVEL"			//
#define	DEF_PROPERTY_LINESTYLE_JOIN3		"ROUND"			//
#define	DEF_PROPERTY_LINESTYLE_JOIN4		"METERCLIPPED"	//

// 채움방법 정보
#define DEF_PROPERTY_FILLTYLE_NONE				"채우지않음" //채우지않음
#define DEF_PROPERTY_FILLTYLE_SOLID				"단색"		 //단색
#define DEF_PROPERTY_FILLTYLE_HATCH				"음영"		 //음영
#define DEF_PROPERTY_FILLTYLE_EMPTY				"빈칸"		 //빈칸
#define DEF_PROPERTY_FILLTYLE_PATHGRADIENT		"경로 그라데이션"	// PATHGRADIENT
#define DEF_PROPERTY_FILLTYLE_LINEARGRADATION	"선 그라데이션"		// LINEARGRADATION

// 패턴 스타일 정보
#define DEF_PROPERTY_PATTERNSTYLE_0		"▤"
#define DEF_PROPERTY_PATTERNSTYLE_1		"▥"
#define DEF_PROPERTY_PATTERNSTYLE_2		"▧"
#define DEF_PROPERTY_PATTERNSTYLE_3		"▨"
#define DEF_PROPERTY_PATTERNSTYLE_4		"▦"
#define DEF_PROPERTY_PATTERNSTYLE_5		"▩"

// 음영 스타일 정보
#define DEF_PROPERTY_HATCHSTYLE_0		"▤"
#define DEF_PROPERTY_HATCHSTYLE_1		"▥"
#define DEF_PROPERTY_HATCHSTYLE_2		"▧"
#define DEF_PROPERTY_HATCHSTYLE_3		"▨"
#define DEF_PROPERTY_HATCHSTYLE_4		"▦"
#define DEF_PROPERTY_HATCHSTYLE_5		"▩"
#define	DEF_PROPERTY_HATCHSTYLE_6		"PERCENT05"		
#define	DEF_PROPERTY_HATCHSTYLE_7		"PERCENT10"		
#define	DEF_PROPERTY_HATCHSTYLE_8		"PERCENT20"		
#define	DEF_PROPERTY_HATCHSTYLE_9		"PERCENT25"		
#define	DEF_PROPERTY_HATCHSTYLE_10		"PERCENT30"		
#define	DEF_PROPERTY_HATCHSTYLE_11		"PERCENT40"		
#define	DEF_PROPERTY_HATCHSTYLE_12		"PERCENT50"		
#define	DEF_PROPERTY_HATCHSTYLE_13		"PERCENT60"		
#define	DEF_PROPERTY_HATCHSTYLE_14		"PERCENT70"		
#define	DEF_PROPERTY_HATCHSTYLE_15		"PERCENT75"		
#define	DEF_PROPERTY_HATCHSTYLE_16		"PERCENT80"		
#define	DEF_PROPERTY_HATCHSTYLE_17		"PERCENT90"		
#define	DEF_PROPERTY_HATCHSTYLE_18		"LIGHTDOWNWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_19		"LIGHTUPWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_20		"DARKDOWNWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_21		"DARKUPWARD "		
#define	DEF_PROPERTY_HATCHSTYLE_22		"WIDEDOWNWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_23		"WIDEUPWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_24		"LIGHTVERT"		
#define	DEF_PROPERTY_HATCHSTYLE_25		"LIGHTHORIZ"		
#define	DEF_PROPERTY_HATCHSTYLE_26		"NARROWVERT"		
#define	DEF_PROPERTY_HATCHSTYLE_27		"NARROWHORIZ"		
#define	DEF_PROPERTY_HATCHSTYLE_28		"DARKVERT"		
#define	DEF_PROPERTY_HATCHSTYLE_29		"DARKHORIZ"		
#define	DEF_PROPERTY_HATCHSTYLE_30		"DASHEDDOWNWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_31		"DASHEDUPWARD"		
#define	DEF_PROPERTY_HATCHSTYLE_32		"DASHEDHORIZ"		
#define	DEF_PROPERTY_HATCHSTYLE_33		"DASHEDVERT"		
#define	DEF_PROPERTY_HATCHSTYLE_34		"SMALLCONFET"		
#define	DEF_PROPERTY_HATCHSTYLE_35		"LARGECONFET"		
#define	DEF_PROPERTY_HATCHSTYLE_36		"ZIGZAG"		
#define	DEF_PROPERTY_HATCHSTYLE_37		"WAVE"		
#define	DEF_PROPERTY_HATCHSTYLE_38		"DIAGBRICK"		
#define	DEF_PROPERTY_HATCHSTYLE_39		"HORIZBRICK"		
#define	DEF_PROPERTY_HATCHSTYLE_40		"WEAVE"		
#define	DEF_PROPERTY_HATCHSTYLE_41		"PLAID"		
#define	DEF_PROPERTY_HATCHSTYLE_42		"DIVOT"		
#define	DEF_PROPERTY_HATCHSTYLE_43		"DOTTEDGRID"		
#define	DEF_PROPERTY_HATCHSTYLE_44		"DOTTEDDIAMOND"		
#define	DEF_PROPERTY_HATCHSTYLE_45		"SHINGLE"		
#define	DEF_PROPERTY_HATCHSTYLE_46		"TRELLIS"		
#define	DEF_PROPERTY_HATCHSTYLE_47		"SPHERE"		
#define	DEF_PROPERTY_HATCHSTYLE_48		"SMALLGRID"		
#define	DEF_PROPERTY_HATCHSTYLE_49		"SMALLCHKBOARD"		
#define	DEF_PROPERTY_HATCHSTYLE_50		"LARGECHKBOARD"		
#define	DEF_PROPERTY_HATCHSTYLE_51		"OUTLINEDIA"		
#define	DEF_PROPERTY_HATCHSTYLE_52		"SOLIDDIA"	

// 화살표 스타일 정보
#define DEF_PROPERTY_LINECAP_NONE		"없음"
#define DEF_PROPERTY_LINECAP_ARROW		"화살표"
#define DEF_PROPERTY_LINECAP_ARROW_L	"화살표-좌"
#define DEF_PROPERTY_LINECAP_ARROW_R    "화살표-우"
#define DEF_PROPERTY_LINECAP_TAIL       "화살꼬리"
#define DEF_PROPERTY_LINECAP_TAIL_L     "화살꼬리-좌"
#define	DEF_PROPERTY_LINECAP_TAIL_R     "화살꼬리-우"
#define	DEF_PROPERTY_LINECAP_TAIL_F     "화살꼬리채움"
#define	DEF_PROPERTY_LINECAP_TAIL_LF    "화살꼬리채움-좌"
#define	DEF_PROPERTY_LINECAP_TAIL_RF    "화살꼬리채움-우"
#define	DEF_PROPERTY_LINECAP_TENT_L     "텐트-좌"
#define	DEF_PROPERTY_LINECAP_TENT_R     "텐트-우"
#define	DEF_PROPERTY_LINECAP_TENT_LF    "텐트채움-좌"
#define	DEF_PROPERTY_LINECAP_TENT_RF    "텐트채움-우"
#define	DEF_PROPERTY_LINECAP_SLASH_L    "사선"
#define	DEF_PROPERTY_LINECAP_SLASH_R    "역사선"
#define	DEF_PROPERTY_LINECAP_CROSS      "가위표"
#define	DEF_PROPERTY_LINECAP_TRI        "삼각형"
#define	DEF_PROPERTY_LINECAP_TRI_F      "삼각형채움"
#define	DEF_PROPERTY_LINECAP_TRI_L      "삼각형-좌"
#define	DEF_PROPERTY_LINECAP_TRI_LF     "삼각형채움-좌"
#define	DEF_PROPERTY_LINECAP_TRI_R      "삼각형-우"
#define	DEF_PROPERTY_LINECAP_TRI_RF     "삼각형채움-우"
#define	DEF_PROPERTY_LINECAP_RECT       "사각형"
#define	DEF_PROPERTY_LINECAP_RECT_F     "사각형채움"
#define	DEF_PROPERTY_LINECAP_CIRCLE     "원"
#define	DEF_PROPERTY_LINECAP_CIRCLE_F   "원채움"
#define	DEF_PROPERTY_LINECAP_DIA        "마름모"
#define	DEF_PROPERTY_LINECAP_DIA_F      "마름모채움"
#define	DEF_PROPERTY_LINECAP_THICK      "겹화살표"
#define	DEF_PROPERTY_LINECAP_DBLARROW   "이중화살표"
#define	DEF_PROPERTY_LINECAP_DBLARROW_L	"이중화살표-좌"
#define	DEF_PROPERTY_LINECAP_DBLARROW_R	"이중화살표-우"
#define	DEF_PROPERTY_LINECAP_DBLTAIL    "이중화살꼬리"
#define	DEF_PROPERTY_LINECAP_DBLTAIL_L  "이중화살꼬리-좌"
#define	DEF_PROPERTY_LINECAP_DBLTAIL_R  "이중화살꼬리-우"

// 선형 그라데이션 방향
#define	DEF_PROPERTY_LINEARGRD_HOLIZONTAL		"HOLIZONTAL"
#define	DEF_PROPERTY_LINEARGRD_VERTICAL			"VERTICAL"
#define	DEF_PROPERTY_LINEARGRD_FORWARDDIAGONAL	"FORWARDDIAGONAL"
#define	DEF_PROPERTY_LINEARGRD_BACKWARDDIAGONAL	"BACKWARDDIAGONAL"

// 창 타이틀
#define DEF_DOCKPANE_TITLE_MAPMANAGER			"지도레이어 관리"
#define DEF_DOCKPANE_TITLE_MAPMANAGER_SUB1		"도시설정"
#define DEF_DOCKPANE_TITLE_MAPMANAGER_SUB2		"순서조절"
#define DEF_DOCKPANE_TITLE_MAPVIEW				"지도레이어 도시설정"
#define DEF_DOCKPANE_TITLE_MAPVIEW_SUB1			"지도"
#define DEF_DOCKPANE_TITLE_MAPORDER				"지도레이어 순서조절"
#define DEF_DOCKPANE_TITLE_OVERLAY				"투명도 관리"
#define DEF_DOCKPANE_TITLE_OVERLAY_SUB1			"목록"
#define DEF_DOCKPANE_TITLE_MAPPROP				"지도레이어 속성"
#define DEF_DOCKPANE_TITLE_OVERPROP				"투명도 속성"
#define DEF_DOCKPANE_TITLE_OUTPUT				"출력"
#define DEF_DOCKPANE_TITLE_OUTPUT_SUB1			"로그"
#define DEF_DOCKPANE_TITLE_OUTPUT_SUB2			"검색 결과"
#define DEF_DOCKPANE_TITLE_INDEXMAP				"인덱스 지도"

// 위성 및 음영도 On/Off 
#define MAP_OBJECT_RASTER_CATEGORY				"Raster"
#define MAP_OBJECT_RASTER_LAYER					"GeoTiff"
#define MAP_OBJECT_DTED_CATEGORY				"DTED2"
#define MAP_OBJECT_DTED_LAYER					"DTED2"

// 속성 창 출력 정보
#define DEF_PROPERTY_SEARCH_FEATURE_TITLE		"지형 속성 정보"
#define DEF_PROPERTY_SELECT_LAYERE_TITLE		"Layer"
#define DEF_PROPERTY_SELECT_FEATURE_TITLE		"Feature"

#define DEF_PROPERTY_OVERLAY_TITLE				"투명도"
#define DEF_PROPERTY_OVERLAY_NAME				"명칭"

#define DEF_PROPERTY_OVERLAY_DEFAULT_TITLE						"도형 기본"
#define DEF_PROPERTY_OVERLAY_DEFAULT_CAPTION_NAME				"명칭"
#define DEF_PROPERTY_OVERLAY_DEFAULT_CAPTION_NAME_REMARK		"명칭을 입력합니다."
#define DEF_PROPERTY_OVERLAY_DEFAULT_CAPTION_DES				"설명"
#define DEF_PROPERTY_OVERLAY_DEFAULT_CAPTION_DES_REMARK			"간략한 설명을 입력합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_TITLE							"도형 편집"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINEWIDTH				"선 굵기"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINEWIDTH_REMARK		"선 굵기를 0~20 사이의 값으로 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINESTYLE				"선 모양"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINESTYLE_REMARK		"선 모양을 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINECOLOR				"선 색상"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_LINECOLOR_REMARK		"선 색상을 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLUSE				"채움"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLUSE_REMARK		"도형의 내부 채움 사용 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLCOLOR				"채움 색상"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLCOLOR_REMARK		"도형의 내부 채움 색상을 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLTRANCE			"반투명"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_FILLTRANCE_REMARK		"도형의 내부 채움 반투명 사용 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNUSE			"채움문양"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNUSE_REMARK		"채움문양 사용 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNCOLOR			"채움문양 색상"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNCOLOR_REMARK	"채움문양 색상을 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNSTYLE			"채움문양 모양"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_PATTERNSTYLE_REMARK	"채움문양 모양을 설정합니다."
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_ROTATE				"회전"
#define DEF_PROPERTY_OVERLAY_EDIT_CAPTION_ROTATE_REMARK			"회전 값을 0~360 사이의 값으로 설정합니다."
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_SELECT				"직선/곡선"
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_SELECT_REMARK			"직선 또는 곡선을 선택합니다."
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_STARROW				"시작 화살표"
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_STARROW_REMARK		"시작 화살표 모양을 설정합니다."
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_EDARROW				"끝 화살표"
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_EDARROW_REMARK		"끝 화살표 모양을 설정합니다."
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_CLOSED				"시작/끝점 연결"
#define DEF_PROPERTY_OVERLAY_LINE_CAPTION_CLOSED_REMARK			"시작점과 끝점 연결 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_NGON_CAPTION_COUNT					"꼭지점 수"
#define DEF_PROPERTY_OVERLAY_NGON_CAPTION_COUNT_REMARK			"다각형의 꼭지점 수를 0~30 사이의 값으로 설정합니다."
#define DEF_PROPERTY_OVERLAY_RECT_CAPTION_ROUND					"사각형 곡률"
#define DEF_PROPERTY_OVERLAY_RECT_CAPTION_ROUND_REMARK			"사각형 곡률을 0~360 사이의 값으로 설정합니다."
#define DEF_PROPERTY_OVERLAY_ARROW_CAPTION_TAIL					"꼬리모양"
#define DEF_PROPERTY_OVERLAY_ARROW_CAPTION_TAIL_REMARK			"꼬리모양을 설정합니다."
#define DEF_PROPERTY_OVERLAY_ARROW_CAPTION_WIDTH				"너비값"
#define DEF_PROPERTY_OVERLAY_ARROW_CAPTION_WIDTH_REMARK			"너비값을 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_NAME					"글꼴"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_NAME_REMARK			"글꼴을 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_SIZE					"크기"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_SIZE_REMARK			"글 크기를 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_COLOR					"색상"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_COLOR_REMARK			"글 색상을 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_BOLD					"굵게"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_BOLD_REMARK			"글 굵기 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_ITALIC				"기울기"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_ITALIC_REMARK			"글 기울기 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_UNDERLINE				"밑줄"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_UNDERLINE_REMARK		"글 밑줄 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_STRIKEOUT				"취소선"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_STRIKEOUT_REMARK		"글 취소선 여부를 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_ALIGN					"정렬"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_ALIGN_REMARK			"글 정렬을 설정합니다."
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_STRING				"내용"
#define DEF_PROPERTY_OVERLAY_TEXT_CAPTION_STRING_REMARK			"글 내용을 입력합니다."

#define DEF_PROPERTY_OVERLAY_LINE_TITLE		"선/화살표"
#define DEF_PROPERTY_OVERLAY_RECT_TITLE		"사각형"
#define DEF_PROPERTY_OVERLAY_NGON_TITLE		"다각형"
#define DEF_PROPERTY_OVERLAY_ARROW_TITLE	"화살표"
#define DEF_PROPERTY_OVERLAY_TEXT_TITLE		"텍스트"

// 속성창 사용 문자열
#define DEF_PROPERTY_USER_VALUE				"색 편집"
#define DEF_PROPERTY_ENABLE					"사용"
#define DEF_PROPERTY_DISABLE				"사용안함"
#define DEF_PROPERTY_POLYLINE				"직선"
#define DEF_PROPERTY_SPLINE					"곡선"
#define DEF_PROPERTY_OPEN					"열림"	// "<"	">"
#define DEF_PROPERTY_CLOSE					"닫힘"	// "◀"	"▶"
#define DEF_PROPERTY_CONNECT				"연결"
#define DEF_PROPERTY_DISCONNECT				"해제"
#define DEF_PROPERTY_RIBBON					"리본"
#define DEF_PROPERTY_LEFT					"왼쪽"
#define DEF_PROPERTY_CENTER					"가운데"
#define DEF_PROPERTY_RIGHT					"오른쪽"

// 트리 컨트롤
// TREE MEMBER
#define TREE_MEMBER_NONE		0	// 
#define TREE_MEMBER_UNCHECK		1	// 체크 해제
#define TREE_MEMBER_ALLCHECK	2	// 체크 
#define TREE_MEMBER_HFCHECK		3	// 반체크
#define TREE_MEMBER_NOT			4	// 체크 불가

// TREE STATE
#define TREE_STATE_NONE			0x0000
#define TREE_STATE_UNCHECK		0x1000	// 체크 해제
#define TREE_STATE_ALLCHECK		0x2000	// 체크
#define TREE_STATE_HFCHECK		0x3000	// 반체크
#define TREE_STATE_NOT			0x4000	// 체크 불가

// Property Sheet Title 정보
#define DEF_PROSHEET_GRIDSET_CENTER		"중심선 도시 여부를 설정합니다."
#define DEF_PROSHEET_GRIDSET_GEO		"경위도(Geodetic Reference System) 그리드 도시 여부를 설정합니다."
#define DEF_PROSHEET_GRIDSET_MGRS		"MGRS(Military Grid Reference System) 그리드 도시 여부를 설정합니다."
#define DEF_PROSHEET_GRIDSET_UTM		"UTM(Universal Transverse Mercator) 그리드 도시 여부를 설정합니다."
#define DEF_PROSHEET_GRIDSET_GEOREF		"GEOREF(World Geographic Reference System) 그리드 도시 여부를 설정합니다."
#define DEF_PROSHEET_GRIDSET_GARS		"GARS(Global Area Reference System) 그리드 도시 여부를 설정합니다."

#define DEF_PROSHEET_RADIUS_GEODETIC	"입력한 경위도(Geodetic Reference System) 좌표에 반경을 표시합니다."
#define DEF_PROSHEET_RADIUS_MGRS		"입력한 MGRS(Military Grid Reference System) 좌표에 반경을 표시합니다."
#define DEF_PROSHEET_RADIUS_UTM			"입력한 UTM(Universal Transverse Mercator) 좌표에 반경을 표시합니다."
#define DEF_PROSHEET_RADIUS_GEOREF		"입력한 GeoRef(World Geographic Reference System) 좌표에 반경을 표시합니다."
#define DEF_PROSHEET_RADIUS_GARS		"입력한 GARS(Global Area Reference System) 좌표에 반경을 표시합니다."

#define DEF_PROSHEET_MOVEMAP_GEODETIC	"입력한 경위도(Geodetic Reference System) 좌표로 지도를 이동합니다."
#define DEF_PROSHEET_MOVEMAP_MGRS		"입력한 MGRS(Military Grid Reference System) 좌표로 지도를 이동합니다."
#define DEF_PROSHEET_MOVEMAP_UTM		"입력한 UTM(Universal Transverse Mercator) 좌표로 지도를 이동합니다."
#define DEF_PROSHEET_MOVEMAP_GEOREF		"입력한 GeoRef(World Geographic Reference System) 좌표로 지도를 이동합니다."
#define DEF_PROSHEET_MOVEMAP_GARS		"입력한 GARS(Global Area Reference System) 좌표로 지도를 이동합니다."

// 대화 상자 버튼 텍스트 정보
#define DEF_DLG_BTN_APPLY	"적용"
#define DEF_DLG_BTN_CLOSE	"닫기"

// 대화 상자 Title 정보
#define DEF_DLGTITLE_COORDINATE			"현재 마우스 포인터의 좌표를 표시합니다."
#define DEF_DLGTITLE_ROTATION			"지도 회전률을 설정합니다."
#define DEF_DLGTITLE_SCALE				"지도 축척 정보를 설정합니다."
#define DEF_DLGTITLE_DLGBOOKMARK_ADD	"추가 할 북마크 정보를 설정합니다."
#define DEF_DLGTITLE_PRINT				"사용자 정의 인쇄 항목을 설정합니다."
#define DEF_DLGTITLE_SECTION			"단면도 측정 결과입니다."
#define DEF_DLGTITLE_SEARCHAREA			"공간검색 결과입니다."
#define DEF_DLGTITLE_DLGOVERGRID		"투명도 눈금을 설정합니다."
#define DEF_DLGTITLE_MAPSELECT			"지도 정보를 설정합니다."
#define DEF_DLGTITLE_MAPAUTOSET			"축척별 지도 자동 변경에 사용 할 지도를 설정합니다."
#define DEF_DLGTITLE_INDEXMAPSET		"인덱스 지도를 설정합니다."

// 메시지 박스 출력 정보
#define DEF_COMMON_CANCEL				"취소되었습니다."
#define DEF_COMMON_DELETE				"삭제되었습니다."
#define DEF_COMMON_SAVE					"저장되었습니다."
#define DEF_COMMON_FAIL					"실패하였습니다."
#define DEF_COMMON_NOCHANGE				"변경된 사항이 존재하지 않습니다."
#define DEF_COMMON_CHANGE				"변경된 사항이 존재합니다. 종료하시겠습니까?"
#define DEF_COMMON_SEARCH_NONE			"검색 결과가 존재하지 않습니다."
#define DEF_COMMON_COORD_NONE			"좌표 정보가 존재하지 않습니다."
#define DEF_COMMON_CLIPBOARD			"클립보드에 저장 되었습니다."
#define DEF_BOOKMARK_SELECT				"선택한 북마크 위치로 이동하시겠습니까?"
#define DEF_BOOKMARK_ADD_ERROE			"현재 중심 위치 정보를 알 수 없습니다."
#define DEF_BOOKMARK_DELETE				"선택한 북마크 정보를 삭제하시겠습니까?"
#define DEF_BOOKMARK_DELETE_SELECT		"삭제 할 북마크 정보를 선택하여 주십시요."
#define DEF_BOOKMARK_INPUT_ERROR		"이미 존재하는 명칭 또는 잘못된 입력값 입니다."
#define DEF_BOOKMARK_INPUT_SUCCESS		"북마크 정보가 추가되었습니다."
#define DEF_BOOKMARK_INPUT_ALL			"모든 항목을 입력하여 주십시요."
#define DEF_OVERLAY_DELETE				"레이어를 삭제하시겠습니까?"
#define DEF_OVERLAY_SELECT				"투명도 관리 화면에서 대상을 선택하여 주십시요."
#define DEF_OVERLAY_SAVE				"레이어를 파일 저장하시겠습니까?"
#define DEF_OVERLAY_MOVETOP				"레이어를 맨 위로 이동하시겠습니까?"
#define DEF_OVERLAY_MOVEUP				"레이어를 위로 이동하시겠습니까?"
#define DEF_OVERLAY_MOVEDOWN			"레이어를 아래로 이동하시겠습니까?"
#define DEF_OVERLAY_MOVEBOTTOM			"레이어를 맨 아래로 이동하시겠습니까?"
#define DEF_CONNECT_SUCCESS				"접속을 성공하였습니다."
#define DEF_CONNECT_FAIL				"접속을 실패하였습니다."
#define DEF_COPCONFIG_SELECT			"적어도 한 항목 이상을 체크하여 주십시요."
#define DEF_DBUPDATE_TIME				"10초 이상 입력하여 주십시요."
#define DEF_ROTATE_VALUE				"0~360 사이의 값을 입력하여 주십시요."
#define DEF_SCALE_VALUE					"도시 축척은 도시 가능 범위 값으로 설정하여 주십시요."
#define DEF_SELECT_TARGET				"검색 대상을 선택하여 주십시요."
#define DEF_SEARCH_INPUT				"검색 할 명칭을 입력하여 주십시요."


#define DEF_OF_3DRIBBON_IMG_1MASH		0
#define DEF_OF_3DRIBBON_IMG_2MASH		2
#define DEF_OF_3DRIBBON_IMG_POLYGON		4
#define DEF_OF_3DRIBBON_IMG_SHADOW		6
#define DEF_OF_3DRIBBON_IMG_SPECTRUM	8
#define DEF_OF_3DRIBBON_IMG_TEXTURE		10
#define DEF_OF_3DRIBBON_IMG_GRID		12
#define DEF_OF_3DRIBBON_IMG_CLIPBOARD	14
#define DEF_OF_3DRIBBON_IMG_SKY			16
#define DEF_OF_3DRIBBON_IMG_FOG			18
#define DEF_OF_3DRIBBON_IMG_RAIN		20
#define DEF_OF_3DRIBBON_IMG_SNOW		22
#define DEF_OF_3DRIBBON_IMG_SUN			24
#define DEF_OF_3DRIBBON_IMG_COP_DRAW	24
#define DEF_OF_3DRIBBON_IMG_COP_DRAP	26
#define DEF_OF_3DRIBBON_IMG_COP_BILL	28


/**
* @brief  마우스 상태 모드
*/
typedef enum MOUSE_STATE
{
	MOUSE_DEFAULT = 0,  /**< 기본 */
	MOUSE_ZOOMIN = 1,  /**< 확대 */
	MOUSE_ZOOMOUT = 2,  /**< 축소 */
	MOUSE_PANNING = 3,  /**< 이동 */
	MOUSE_ROTATE = 4,  /**< 회전 */
	MOUSE_SEARCH_POINT = 20,		/**< 속성 조회 */
	MOUSE_SEARCH_CIRCLE = 21,	/**< 공간검색 원형 */
	MOUSE_SEARCH_RECTANGLE = 22,	/**< 공간검색 사각형 */
	MOUSE_SEARCH = 5,  /**< 지형 검색 (점, 원, 사각형, 폴리곤) */
	MOUSE_DISTANCE = 6,  /**< 거리측정 */
	MOUSE_AREA = 7,  /**< 면적측정 */
	MOUSE_SECTION = 8,  /**< 단면도측정 */
	MOUSE_VISIBLE = 9,  /**< 가시권 분석 */
	MOUSE_TERRAIN = 10, /**< 기타 영역 선택 분석 */
	MOUSE_PRINT = 11, /**< 인쇄 영역 선택 */
	MOUSE_RADIUS = 12, /**< 반경 도시 중심 선택 */
	MOUSE_OVERLAY = 13, /**< 투명도 */
	MOUSE_COPLAY = 14, /**< 상황도 */
	MOUSE_EDIT = 15, /**< 지도 편집 */
	MOUSE_NAME_SEARCH = 16, /**< 지명 검색 */
	MOUSE_SPACEFILTER_CIRCLE = 17,	// 08.22 공간필터 원 중심 설정
	MOUSE_SPACEFILTER_POLYGON = 18,	// 08.22 공간필터 폴리곤 설정
	MOUSE_COORD_INPUT = 19,			//마우스로 좌표를 입력시

} MOUSE_STATE;

/**
* @brief  검색 모드
*/
typedef enum SEARCH_OPTION
{
	SEARCH_NONE = 0, /**< 없음 */
	SEARCH_POINT = 1, /**< 2 point */
	SEARCH_POINTS = 2, /**< multi point */
	SEARCH_CIRCLE = 3, /**< circle */
	SEARCH_RECT = 4, /**< rectangle */
	SEARCH_POLYGON = 5, /**< polygon */
} SEARCH_OPTION;

/**
* @brief  Tracker 모드
*/
typedef enum TRACKER_MODE
{
	TRACKER_NONE = 0,  /**< 없음 */
	TRACKER_POINT = 1,  /**< 2 point */
	TRACKER_POINTS = 2,  /**< multi point */
	TRACKER_CIRCLE = 3,  /**< circle */
	TRACKER_RECT = 4,  /**< rectangle */
	TRACKER_POLYGON = 5,  /**< polygon */
	TRACKER_POINT_CLIP = 6, //클립보드저장용
} TRACKER_MODE;


/**
* @brief  측정 모드
*/
typedef enum MEASURE_MODE
{
	MEASURE_MODE_NONE, /**< 없음 */
	MEASURE_MODE_DISTANCE, /**< 거리 */
	MEASURE_MODE_LEADTIME, // 08.22 소요시간 계산
	MEASURE_MODE_AREA, /**< 면적 */
	MEASURE_MODE_SECTION, /**< 단면도 */
	MEASURE_MODE_VISIBILE, /**< 가시권 */
	MEASURE_MODE_RADIUS, /**< 반경 */
} MEASURE_MODE;

/**
* @struct STATE_COORD
* @brief  좌표계 표시 정보 구조
*/
struct STATE_COORD
{
	BOOL bGeodetic;  /**< 경위도 표시 여부 */
	BOOL bMGRS;      /**< MGRS 표시 여부 */
	int  nMGRSLevel; /**< MGRS 표시 레벨 */
	BOOL bUTM;       /**< UTM 표시 여부 */
	BOOL bGeoRef;    /**< GEOREF 표시 여부 */
	BOOL bGARS;		 /**< GARS 표시 여부 */

	STATE_COORD()
	{

#ifdef _DEBUG
		bGeodetic = TRUE;
		bMGRS = TRUE;
		nMGRSLevel = 5;
		bUTM = TRUE;
		bGeoRef = TRUE;
		bGARS = TRUE;
#else
		bGeodetic = TRUE;
		bMGRS = TRUE;
		nMGRSLevel = 5;
		bUTM = FALSE;
		bGeoRef = FALSE;
		bGARS = FALSE;

#endif // _DEBUG
	}
};
