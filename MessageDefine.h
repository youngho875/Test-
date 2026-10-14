#pragma once

// Web 과 동기화 해야되는 정의들

//**Maxp_Web_CommonMessage 5000~5999**//

#define MAPX_PROC_MSG				5000
#define MAPX_INIT_MAIN				5001	// 메인 지도용 초기화
#define MAPX_INIT_INDEX				5002	// 인덱스 지도용 초기화
#define MAPX_EXIST_FILE				5003	// 파일 존재여부
#define MAPX_INDEX_POINT			5004	// 인덱스 화면 클릭
#define MAPX_INDEX_RECT				5005	// 인덱스 화면 영역
#define MAPX_VIEW_UPDATE			5010	// 지도 업데이트
#define MAPX_LOAD_XMLFILE			5011	// XML 파일 로드
#define MAPX_SET_CENTERGEO			5012	// 경위도 중심좌표
#define MAPX_GET_CENTERGEO			5013	// 경위도 중심좌표
#define MAPX_SET_CURRENTSCALE		5014	// 도시 축척
#define MAPX_GET_CURRENTSCALE		5015	// 도시 축척
#define MAPX_SET_DEFAULTSCALE		5016	// 기본 축척    
#define MAPX_GET_DEFAULTSCALE		5017	// 기본 축척
#define MAPX_SET_MINMAXSCALE		5018	// 최소 최대 축척
#define MAPX_GET_MINMAXSCALE		5019	// 최소 최대 축척
#define MAPX_SET_ZOOMSCALE			5020	// 확대 축소 단위 축척
#define MAPX_GET_ZOOMSCALE			5021	// 확대 축소 단위 축척 
#define MAPX_SET_PROJECTION			5022	// 투영
#define MAPX_GET_PROJECTION			5023	// 투영
#define MAPX_SET_GEOBOUND			5024	// 경위도 영역값 설정   
#define MAPX_GET_GEOBOUND			5025	// 경위도 영역값 획득
#define MAPX_GET_BRIGHT				5026	// 밝기 값 획득
#define MAPX_GET_CONTRAST			5027	// 명암 값 획득
#define MAPX_SET_BRIGHTCONTRAST		5028	// 밝기 명암 설정
#define MAPX_GET_ROTATION			5029	// 회전값 획득
#define MAPX_SET_ROTATION			5030	// 회전값 설정
#define MAPX_MAP_DEFAULT			5031	// 기본 
#define MAPX_MAP_ZOOMIN				5032	// 확대
#define MAPX_MAP_ZOOMOUT			5033	// 축소
#define MAPX_MAP_PANNING			5034	// 이동
#define MAPX_MAP_UNDO				5035	// 이전
#define MAPX_MAP_UNDOENABLE			5036	// 이전 사용 가능
#define MAPX_MAP_REDO				5037	// 다음
#define MAPX_MAP_REDOENABLE			5038	// 다음 사용 가능
#define MAPX_MAP_SELECTINFO			5039	// 속성검색
#define MAPX_MAP_DISTANCE			5040	// 거리측정
#define MAPX_MAP_ZONE				5041	// 면적측정
#define MAPX_MAP_SECTION			5042	// 단면도측정
#define MAPX_MAP_CLIPBOARD			5043	// 클립보드 저장
#define MAPX_MAP_IMAGEFILE			5044	// 그림파일 저장F
#define MAPX_MAP_PRINTVIEW			5045	// 인쇄 
#define MAPX_MAP_STATUSBAR			5046	// 상태바 업데이트
#define MAPX_MAP_SCALEEDIT			5047	// 상태바 & 축척 업데이트
#define MAPX_MAP_COMPASS			5048	// 나침반 도시여부
#define MAPX_MAP_SCALEBAR			5049	// 축척바 도시여부
#define MAPX_MAP_AUTOVIEW			5050	// 도시자동조절여부
#define MAPX_SEARCH_POINT_RESULT	5051	// 속성 선택 결과
#define MAPX_SEARCH_AREA_CIRCLE		5053	// 원 형태의 영역 속성 검색
#define MAPX_SEARCH_AREA_RECTANGLE	5054	// 사각형 형태의 영역 속성 검색
#define MAPX_SEARCH_AREA_RESULT		5055	// 영역 속성 검색 결과
#define MAPX_SEARCH_AREA_SELECT		5056	// 검색 결과 중 선택 마크
#define MAPX_SEARCH_LAYER_INIT		5057	// 레이어 필드 내용 검색을 위한 레이어 및 필드 설정
#define MAPX_SEARCH_LAYER_CONTENT	5058	// 설정한 레이어의 필드의 내용 검색 요청
#define MAPX_SEARCH_LAYER_SELECT	5059	// 설정한 레이어의 필드의 내용 검색 결과 선택 인덱스
#define MAPX_SECTION_DIALOG			5060	// 단면도 측정 화면 열기
#define MAPX_SECTION_RESULT			5061	// 단면도 측정 결과
#define MAPX_SECTION_RESULTEX		5062	// 단면도 측정 결과
#define MAPX_LAYER_INFO_NAME		5063	// 지도 레이어 명칭 목록 요청
#define MAPX_LAYER_INFO_SOURCE		5064	// 지도 레이어 DataSource 목록 요청
#define MAPX_LAYER_INFO_ALL			5065	// 지도 레이어 정보
#define MAPX_LAYER_FEATURE_INFO		5066	// 지도 레이어 피쳐 정보
#define MAPX_LAYER_VIEW_ALL			5067	// 지도 레이어 전체 On/Off
#define MAPX_LAYER_VIEW_VECTOR		5068	// 지도 레이어 중 벡터 On/Off
#define MAPX_LAYER_VIEW_SELECT		5069	// 지도 레이어 중 선택 레이어 On/Off
#define MAPX_LAYER_FEATURE_VIEW_OFF	5070	// 지도 레이어 중 선택 레이어의 선택 피쳐 On
#define MAPX_LAYER_FEATURE_VIEW_ON	5071	// 지도 레이어 중 선택 레이어의 선택 피쳐 Off
#define MAPX_LAYER_FIELD_LIST		5072	// 지도 레이어 중 선택 레이어의 필드명 목록 요청
#define MAPX_LAYER_ORDER			5073	// 지도 레이어 도시 순서 설정
#define MAPX_ANALYSIS_VISIBLE		5074	// 가시원 분석 요청
#define MAPX_GET_VISIBLE_RADIUSE	5075	// 반경 도시 설정 값 획득
#define MAPX_SET_VISIBLE_RADIUSE	5076	// 반경 도시 설정
#define MAPX_GET_VISIBLE_GRID		5077	// 그리드 도시 설정 값 획득
#define MAPX_SET_VISIBLE_GRID		5078	// 그리드 도시 설정
#define MAPX_GEODETIC_TO_MGRS		5079	// 경위도를 MGRS 좌표로 변환
#define MAPX_GEODETIC_TO_UTM		5080	// 경위도를 UTM 좌표로 변환
#define MAPX_GEODETIC_TO_GEOREF		5081	// 경위도를 GEOREF 좌표로 변환
#define MAPX_GEODETIC_TO_GARS		5082	// 경위도를 GARS로 변환
#define MAPX_MGRS_TO_GEODETIC		5083	// MGRS를 경위도로 변환
#define MAPX_MGRS_TO_UTM			5084	// MGRS를 UTM으로 변환
#define MAPX_MGRS_TO_GEOREF			5085	// MGRS를 GEOREF로 변환
#define MAPX_MGRS_TO_GARS			5086	// MGRS를 GARS로 변환
#define MAPX_UTM_TO_GEODETIC		5087	// UTM을 경위도로 변환
#define MAPX_UTM_TO_MGRS			5088	// UTM을 MGRS로 변환
#define MAPX_UTM_TO_GEOREF			5089	// UTM을 GEOREF로 변환
#define MAPX_UTM_TO_GARS			5090	// UTM을 GARS로 변환
#define MAPX_GEOREF_TO_GEODETIC		5091	// GEOREF를 경위도로 변환
#define MAPX_GEOREF_TO_MGRS			5092	// GEOREF를 MGRS로 변환
#define MAPX_GEOREF_TO_UTM			5093	// GEOREF를 UTM으로 변환
#define MAPX_GEOREF_TO_GARS			5094	// GEOREF를 GARS로 변환
#define MAPX_GARS_TO_GEODETIC		5095	// GARS를 경위도로 변환
#define MAPX_GARS_TO_MGRS			5096	// GARS를 MGRS로 변환
#define MAPX_GARS_TO_UTM			5097	// GARS를 UTM으로 변환
#define MAPX_GARS_TO_GEOREF			5098	// GARS를 GEPREF로 변환
#define MAPX_SET_CENTERMGRS			5099	// MGRS 중심 좌표 설정
#define MAPX_GET_CENTERMGRS			5100	// MGRS 중심 좌표 획득
#define MAPX_SET_CENTERUTM			5101	// UTM 중심 좌표 설정
#define MAPX_GET_CENTERUTM			5102	// UTM 중심 좌표 획득
#define MAPX_SET_CENTERGEOREF		5103	// GEOREF 중심 좌표 설정
#define MAPX_GET_CENTERGEOREF		5104	// GEOREF 중심 좌표 획득
#define MAPX_GET_CENTERGARS			5105	// GARS 중심좌표 획득
#define MAPX_SET_CENTERGARS			5106	// GARS 중심좌표 설정F
#define MAPX_GET_STATEBAR_COORD		5107	// 좌표계 정보 획득
#define MAPX_SET_STATEBAR_COORD		5108	// 좌표계 정보 설정
#define MAPX_3D_DISPLEY				5109	// 3차원 지도 도시 요청
#define MAPX_INDEX_POSITION_SET		5110    // 지명검색후 인덱스맵에 위치표기
#define MAPX_CLOSEING				5111	// ActiveX Main close
#define MAPX_NOW_MOUSE_STATE		5112	//	마우스 상태 ocx->web으로 알려줌
#define MAPX_MAP_MOVE				5113	// 중심좌표에서 거리방향 만큼 이동
#define MAPX_MAP_MOUSE_STATE_DEFAULT 5114	//마우스 상태만 변경(WEB->OCX)



//**COP_Web_CommonMessage 6000~6999**//
/** Debug Event ******************************************************************/
#define MAPX_COP_OBJECT					6001		// 도시요소 목록
#define MAPX_COP_ADDINFO_SET			6002		// 추가정보 설정(WEB에서 선택된 추가정보를 받음)
#define MAPX_COP_ADDINFO_GET			6014		// 선택된 추가정보를 WEB으로 알려줌
#define MAPX_COP_OBJECT_CNT				6003		// 전체 도시요소목록-선택된갯수
#define MAPX_COP_ID_SET					6015		// Web->ocx로 상황도ID를 Send
#define MAPX_COP_REALITEM				6016		// 실시간 도시요소
#define MAPX_COP_CENTER_MOVE			6017		//부대이동 위치표기

  /**상황도 녹화기능************************************************************/
#define MAPX_COP_RECORD_START			6004		//상황도 녹화시작 (ocx<->web)
#define MAPX_COP_RECORD_CANCLE			6005		//상황도 녹화중지 (ocx<->web)
#define MAPX_COP_RECORD_INDEX_ADD		6006 		//상황도 녹화인덱스 추가
#define MAPX_COP_RECORD_SAVE			6007		//상황도 녹화파일저장 
#define MAPX_COP_RECORD_END				6008		//상황도 녹화종료(ocx->web)
#define MAPX_COP_RECORD_SET_CHANGE      6009		//상황도 녹화설정변경(web->ocx)
#define MAPX_COP_RECORD_SAVE_CANCLE     6010		//상황도 녹화된 파일을 저장하지 않고 취소 (web-ocx)	
#define MAPX_COP_RECORD_VIEWER_TOOL		6020		//녹화된 상황도 볼수 있는 TOOL  호출(RecordViewer.exe)

/**웹캅 기능**************************************************************************/
#define MAPX_COP_WEB_COP_AUTO	        6011		//웹캅자동공유  (ocx<->web)
#define MAPX_COP_WEB_COP_MANUAL	        6012		//웹캅수동공유  (ocx<->web)
#define MAPX_COP_WEB_COP_FILE_DEL	    6013		//웹캅파일삭제  (ocx<->web)


/**도시요소기능***********************************************************************/
#define  MAPX_COP_IER_LATEST			6501	// 도시요소 최신화
#define  MAPX_COP_IER_OVER_INSERT		6502	// 도시요소 투명도 삽입
#define  MAPX_COP_IER_OVER_NEW			6503	// 도시요소 투명도 생성
#define  MAPX_COP_IER_HISTORY_SHOW		6504	// 도시요소 이동경로 도시
#define  MAPX_COP_IER_MANUAL_RENEWAL	6505	// 도시요소 수동갱신
#define  MAPX_COP_IER_MANUAL_RENEWAL_LIST	6514	// 도시요소 수동갱신 List(OCX->WEB으로 도시요소키를 날림)
#define  MAPX_COP_IER_SYMBOL_SIMPLE		6506    // 부호간략화 (WEB->OCX)
#define  MAPX_COP_IER_DATA_FILE_SAVE	6507    // 도시요소 데이터 파일로 저장
#define  MAPX_COP_IER_DRAW_TYPE 		6508    // 도시요소유형정보
#define  MAPX_COP_IER_DATA_FILE_CHECK	6509    // 도시요소데이터파일 & 도시유형정보파일이 있는지 검사
#define  MAPX_COP_IER_SYMBOL_AUTO		6511    // 부호자동축척
#define  MAPX_COP_IER_LATEST_CYCLE_SET		6513	// 도시요소 최신화주기설정 Set (web->ocx로 주기값 set)
#define  MAPX_COP_IER_LATEST_CYCLE_GET		6515	// 도시요소 최신화주기설정 Get (ocx->web로 현재 주기값 보냄)
#define  MAPX_COP_IER_LATEST_CYCLE			6516	//ocx->web으로 주기 시간이 되었으니 최신화 데이터를 보내달라 요청
#define  MAPX_COP_IER_DATA_CHECK			6517	//도시요소Data가 있는지 없는지에 대한 체크(OCX<->WEB)
#define  MAPX_COP_IER_DATA_DRAW				6518    // 도시요소 데이터를 화면 도시
#define  MAPX_COP_IER_DATA_FILLTER			6519	// 도시요소 필터링 정보를 받음	(WEB->OCX)
#define  MAPX_COP_IER_DATA_FILLTER_CLEAR	6522	// 도시요소 필터링 정보를 해제	(WEB->OCX)
#define  MAPX_COP_IER_DATA_DETAIL_VIEW		6526    // 도시요소 상세보기 기능 (OCX->WEB)
#define  MAPX_COP_IER_DATA_SEARCH			6527    // 도시요소 검색기능(선택된 도시요소) (OCX->WEB)
#define  MAPX_COP_IER_DATA_REQUEST			6528    // 도시요소Data 요청(선택된 도시요소) (OCX->WEB)
#define  MAPX_COP_IER_ALARAM_KEY			6529    //경보상황 키(OCX->WEB) 형식은 (키/키/, 복귀(좌표))



/** 상황도Load *************************************************************************/
#define  MAPX_COP_IER_DATE_SET_BG		6520   //OCX에서 idle time에 다른종류의 상황도를 저장하기 위해  다른 상황도를 내려달라고 OCX->WEB으로 알림
#define  MAPX_COP_IER_ALL_CLEAR			6521    // 상황도/투명 화면 clear
#define  MAPX_COP_IER_DATA_ALLCOUNT		6523	// 내려줄 도시요소 총개수 (WEB->OCX)
#define  MAPX_COP_IER_DRAWIMFOSET		6524	// 상황도 설정정보 세팅 명령 (WEB->OCX)
#define  MAPX_COP_FIRE_INITCOMPLETE		6525	// 도시요소 초기 삽입완료 이벤트  (OCX<->WEB)

#define  MAPX_COP_SETUPXML_LOAD			6550	//설정파일 로드
#define  MAPX_COP_SETUPXML_SAVE			6551	//설정파일 저장
#define  MAPX_COP_SETUPFILE_REQUEST		6552	//설정파일 요청


//wparam - 도시요소키,도시요소유형코드,도시요소명,임무키,군대부호추가첩보,전투력,유효시작일시,유효종료일시,좌표,기본정보,메타정보,도시여부
//도시요소 다 보낸 후 wParam-1|도시요소개수
#define  MAPX_COP_IER_TO_DUTYAPPLY_DATA      6600			//상황도 도시요소를 임무응용에서 데이터를 받을때
#define  MAPX_COP_IER_TO_DUTYAPPLY_CLEAR	 6605			//임무응용에서 날리는 상황도 초기화

//ocx->web 도시요소ID
//web->ocx  이력총갯수/좌표정보/시간정보
//DITEM_ID(도시요소ID)§DITEM_TYPE_CD(유형정보)§이력개수§YYYY-MM-DD HH24:M#COORD¶YYYY-MM-DD HH24:M#COORD¶
#define MAPX_COP_IER_TO_DUTYAPPLY_HISTORY_SHOW		6601	// 상황도 도시요소를 임무응용에서 이력도시 할때

// 중요경보상황설정
#define MapX_COP_IER_EW_SET							6602	// 중요경보상황 설정
#define MapX_COP_IER_EW_GET							6603	// 중요경보상황 설정정보

// 3D On/Off 상태
#define MAPX_3D_EXIT								6651	// 3D 종료상태 메세지 OCX->Web
#define MAPX_3D_FORCED_EXIT							6652	// 3D 종료 메세지 Web->OCX






//** 상황도 도시요소유형별 Data Down ******************************************************/
#define MAPX_COP_DRAWTYPE_ID_SET					6801	// 상황도 ID SET (WEB->OCX)
#define MAPX_COP_DRAWTYPE_XML_DOWN					6802	// 도시요소유형파일(.xml)다운
#define MAPX_COP_DRAWTYPE_DATA_FILE_CHECK           6803	// 도시요소유형타입에 따른 DataFile Local에 있는지 체크 필요한 도시요소유형Data파일 WEB으로 전송(OCX->WEB)
#define MAPX_COP_DRAWTYPE_DATA_DRAW					6804	// 도시해야될 Data (WEB->OCX) /ocx는 도시하고 파일로저장(파일 저장은 Backup)
#define MAPX_COP_DRAWTYPE_DATA_SAVE					6805	// 저장해야될 Data(WEB<->OCX)/ocx는 리턴으로 저장완료(backup으로 작업)
#define MAPX_COP_DRAWTYPE_DATA_FILE					6806	// OCX가 가지고 있는 도시요소유형Data 파일의 종류를 web으로 알려줌 (Backup로 작업)]
#define MAPX_COP_DRAWTYPE_FILE_CHECK				6807	// 도시요소유형에 따른 data파일 체크()
#define MAPX_COP_DRAWTYPE_UPDATE_DATA				6808	// 최신화Data
#define MAPX_COP_DRAWTYPE_DATA_ENCODING_DOWN		6809	// 도시요소Data 인코딩 값으로 Down(web->ocx)
#define MAPX_COP_DRAWTYPE_ENCODING_LATEST			6810	// 도시요소 최신화데이터 압축파일로 받음(WEB->OCX)
#define MAPX_COP_DRAWTYPE_LATEST_TIME_TO_WEB		6811	// 도시요소최신화최종시간 웹으로 전송
#define MAPX_COP_DRAWTYPE_START_TIME_TO_OCX			6812	// 상황도 시작 Time OCX으로 전송
#define MAPX_COP_ITEM_DETAIL_SEARCH					6813	// 도시요소 "적기지 등" 기본상세조회 OCX->WEB
#define MAPX_COP_DRAWTYPE_FULLDATA_ENCODING_DOWN	6814	// 도시요소Data 인코딩 값으로 Down(web->ocx)





//**MOD 확인****************************************************************************/
#define MAPX_COP_MOD                    6901   //진행모드가 연습모드인지 실제모드인지 (WEB->OCX)
#define MAPX_COP_CONNECT_STATE_MOD		6902   //단독모드인지 부대모드인지 (WEB->OCX)
#define MAPX_COP_DIALOG_EXIT			6903   //도시요소목록 다이얼로그창 종료(OCX->WEB)



//**OVL_Web_CommonMessage 7000~7999**//  

//!!!!!! IER_Mgs_def.h 와 일치화 !!!!!!

#define MAPX_OVER_SELECT			7000	//투명도 선택  (web->ocx)
#define MAPX_OVER_NEWCRT			7001	//신규생성
#define MAPX_OVER_LOADXML_FILEPATH	7002	//파일경로로 로딩
#define MAPX_OVER_LOADXML_FILEDLG	7003	//파일열기다이얼로그
#define MAPX_OVER_LOADXML_STRING	7004	//스트링형태로 로딩
#define MAPX_OVER_SAVEXML_FILEPATH	7005	//파일경로에 저장
#define MAPX_OVER_SAVEXML_FILEDLG	7006	//파일저장다이얼로그 
#define MAPX_OVER_SAVEXML_STRING	7007	//스트링형태로 저장
#define MAPX_OVER_MILSYMDLGOPEN		7008	//군대부호도형선택기 열기
#define MAPX_OVER_CONVERT_STRINGTOSTDXML 7009	//웹에서 보내준 스트링을 저장할 다이얼로그 열어준다
#define MAPX_OVER_DRAWITEMDLG		7010	//도시요소다이얼로그
#define MAPX_OVER_MAKEGUID			7011	//아이디생성 (ocx<->web)
#define MAPX_OVER_CONVERT_GUIDNNAME	7012	//투명도의 guid와 명칭을 변경해준다
#define MAPX_OVER_CVTSTR_KJTOSTDXML 7013	//구KJCCS xml을 표준xml로 변환하여 문자열 리턴
#define MAPX_OVER_CVTPATH_KJTOSTDXML 7014	//구KJCCS xml파일을 표준xml파일로 변환하여 저장
#define MAPX_OVER_LAYERSHOW			7015	//레이어 onoff

#define MAPX_OVER_SET_XMLINFO_NAME		7020	//문서이름
#define MAPX_OVER_SET_XMLINFO_SOURCE	7021	//문서출처 (부대+기능실)
#define MAPX_OVER_SET_XMLINFO_ID		7022	//문서ID
#define MAPX_OVER_SET_XMLINFO_AUTHOR	7023	//작성자
#define MAPX_OVER_SET_XMLINFO_SECLEV	7024	//비밀등급
#define MAPX_OVER_SET_XMLINFO_KIND		7025	//문서종류
#define MAPX_OVER_SET_XMLINFO_CRTDATE	7026	//문서작성일자
#define MAPX_OVER_SET_XMLINFO_MODDATE	7027	//문서수정일자

#define MAPX_OVER_GET_XMLINFO_NAME		7028	//문서이름
#define MAPX_OVER_GET_XMLINFO_SOURCE	7029	//문서출처 (부대+기능실)
#define MAPX_OVER_GET_XMLINFO_ID		7030	//문서ID
#define MAPX_OVER_GET_XMLINFO_AUTHOR	7031	//작성자
#define MAPX_OVER_GET_XMLINFO_SECLEV	7032	//비밀등급
#define MAPX_OVER_GET_XMLINFO_KIND		7033	//문서종류
#define MAPX_OVER_GET_XMLINFO_CRTDATE	7034	//문서작성일자
#define MAPX_OVER_GET_XMLINFO_MODDATE	7035	//문서수정일자

#define MAPX_OVER_SET_XMLINFO_GUID		7036	//문서 GUID
#define MAPX_OVER_GET_XMLINFO_GUID		7037	//문서 GUID


#define MAPX_OVER_SET_ODERING		7040	//투명도 순서변경 적용
#define MAPX_OVER_SET_COMMANDID		7041	//투명도 명령
#define MAPX_OVER_SET_GRIDPROPERTY	7042	//그리드속성설정
#define MAPX_OVER_GET_GRIDPROPERTY	7043	//그리드속성리턴
#define MAPX_OVER_SET_COORDMAPMODE	7044	//좌표모드설정
#define MAPX_OVER_GET_COORDMAPMODE	7045	//좌표모드리턴
#define MAPX_OVER_SET_SHAPEPROPERTY	7046	//도형속성설정
#define MAPX_OVER_GET_SHAPEPROPERTY	7047	//도형속성리턴
#define MAPX_OVER_SET_SECRETVIEW	7048	//비밀등급표시설정
#define MAPX_OVER_GET_SECRETVIEW	7049	//비밀등급표시값리턴
#define MAPX_OVER_GET_ALLLAYERINFO	7050	//전체레이어 정보 리턴


#define MAPX_OVER_SET_XMLINFO_SHOW		7100	//문서 SHOW
#define MAPX_OVER_SET_XMLINFO_SHAREKIND	7101	//문서 SHAREKIND
#define MAPX_OVER_SET_XMLINFO_BASIC		7102	//문서 BASIC
#define MAPX_OVER_SET_XMLINFO_ORD		7103	//문서 ORD
#define MAPX_OVER_SET_XMLINFO_NEW		7104	//문서 NEW
#define MAPX_OVER_SET_XMLINFO_SRCCODE	7105	//문서 SRCCODE
#define MAPX_OVER_SET_XMLINFO_DPTCODE	7106	//문서 DPTCODE
#define MAPX_OVER_SET_XMLINFO_DPTNAME	7107	//문서 DPTNAME
#define MAPX_OVER_SET_XMLINFO_AUTHORID	7108	//문서 AUTHORID
#define MAPX_OVER_SET_XMLINFO_LIVE		7109	//문서 LIVE

#define MAPX_OVER_GET_XMLINFO_SHOW		7150	//문서 SHOW
#define MAPX_OVER_GET_XMLINFO_SHAREKIND	7151	//문서 SHAREKIND
#define MAPX_OVER_GET_XMLINFO_BASIC		7152	//문서 BASIC
#define MAPX_OVER_GET_XMLINFO_ORD		7153	//문서 ORD
#define MAPX_OVER_GET_XMLINFO_NEW		7154	//문서 NEW
#define MAPX_OVER_GET_XMLINFO_SRCCODE	7155	//문서 SRCCODE
#define MAPX_OVER_GET_XMLINFO_DPTCODE	7156	//문서 DPTCODE
#define MAPX_OVER_GET_XMLINFO_DPTNAME	7157	//문서 DPTNAME
#define MAPX_OVER_GET_XMLINFO_AUTHORID	7158	//문서 AUTHORID
#define MAPX_OVER_GET_XMLINFO_LIVE		7159	//문서 LIVE


//이벤트
#define MAPX_OVER_FIRE_SHAPEPROPERTY 7500	//도형속성웹창 오픈 이벤트
#define MAPX_OVER_FIRE_SINGLESELECT  7501	//단일도형 선택 이벤트
#define MAPX_OVER_FIRE_WEBWINDOWSAVE 7502	//레이어저장화면 호출




//**ANS_Web_CommonMessage 8000~8999**//
#define MAPX_ANALYSIS_CLEARALL			8001    
#define MAPX_ANALYSIS_WEAPON			8002		// 무기사격
#define MAPX_ANALYSIS_ARTILLERY			8003		// 포병무기사격능력	
#define MAPX_ANALYSIS_SEA				8004		// 해상장비능력
#define MAPX_ANALYSIS_ARTRADAR			8005		// 포병레이다탐지능력
#define MAPX_ANALYSIS_SEARADAR			8006		// 해안레이다탐지능력
#define MAPX_ANALYSIS_AIRPLANE			8007		// 항공기 행동반경
#define MAPX_ANALYSIS_NUKE				8008		// 핵 오염예측
#define MAPX_ANALYSIS_CHEMAA			8009		// 화생방 오염예측(화학탄 A-a형)
#define MAPX_ANALYSIS_CHEMAB			8010		// 화생방 오염예측 (화학탄 A-b형)
#define MAPX_ANALYSIS_CHEMBA			8011		// 화생방 오염예측 (화학탄 B-a형)
#define MAPX_ANALYSIS_CHEMBB			8012		// 화생방 오염예측 (화학탄 B-b형)
#define MAPX_ANALYSIS_CHEMBC			8013		// 화생방 오염예측 (화학탄 B-c형)
#define MAPX_ANALYSIS_CHEMBD			8014		// 화생방 오염예측 (화학탄 B-d형)
#define MAPX_ANALYSIS_BIOAA				8015		// 화생방 오염예측 (생물학 A-a형
#define MAPX_ANALYSIS_BIOAB				8016		// 화생방 오염예측 (생물학 A-b형)
#define MAPX_ANALYSIS_BIOB				8017		// 화생방 오염예측 (생물학 B형)
#define MAPX_ANALYSIS_ENABLE			8018		// 장비 타격 가능여부
#define MAPX_ANALYSIS_PREDICT			8019		// 관측분석
#define MAPX_ANALYSIS_DEAD_SPACE		8020		// 사계분석(사격할 수 없는 지역)
#define MAPX_ANALYSIS_RECEIVE_SPACE		8021		// 수신가능지역
#define MAPX_ANALYSIS_COMM_ABLE			8022		// 통신가능성 분석
#define MAPX_ANALYSIS_RADAR_VISIBLERAY	8023		// 레이다가시선 분석
#define MAPX_ANALYSIS_INSPECT_EQUIP		8024		// 감청장비분석
#define MAPX_ANALYSIS_ABILITY			8025		// 도시요소능력도 도시(OCX->WEB로 키를 날려줌)
#define MAPX_ANALYSIS_ABILITY_POINT		8026		// 타격가능한장비의 목표점을 WEB->OCX로 날려줌
#define MAPX_ANALYSIS_ABILITY_KEY		8027        //타격가능한 장비 키를 OCX<->WEB으로 키를 날려줌

// 08.22
#define MAPX_MEASURE_CLEARALL				8101	// 측정 초기화
#define MAPX_MEASURE_DISTANCE_INPUT			8102	// 거리계산 - 좌표 입력
#define MAPX_MEASURE_AREA_INPUT				8103	// 면적계산 - 좌표 입력
#define MAPX_MEASURE_LEADTIME_MOUSE			8104	// 소요시간계산 - 마우스 지정
#define MAPX_MEASURE_LEADTIME_INPUT			8105	// 소요시간계산 - 좌표 입력
#define MAPX_SPACEFILTER_CIRCLE_MOUSE		8106	// 공간필터설정 원 - 마우스 지정
#define MAPX_SPACEFILTER_CIRCLE_INPUT		8107	// 공간필터설정 원 - 좌표 입력
#define MAPX_SPACEFILTER_POLYGON_MOUSE		8108	// 공간필터설정 다각형 - 마우스 지정
#define MAPX_SPACEFILTER_POLYGON_INPUT		8109	// 공간필터설정 다각형 - 좌표 입력
#define MAPX_MEASURE_COORD_SEND				8110	// 웹으로 마우스 좌표를 보냄

// CDlgRotation 로부터의 메시지
#define DLG_ROTATION_SET					(WM_USER + 1241)
#define DLG_ROTATION_CLOSE					(WM_USER + 1242)

// CCoordinatePropertySheet 로부터의 메시지
#define SHEET_COORD_MOVE_CLOSE				(WM_USER + 1231)
#define SHEET_COORD_RADIUS_CLOSE			(WM_USER + 1232)
#define SHEET_COORD_MOVE					(WM_USER + 1233)
#define SHEET_COORD_RADIUS					(WM_USER + 1234)

// CGridSettingPropertySheet 로부터의 메시지
#define SHEET_GRIDSET_SET					(WM_USER + 1236)
#define SHEET_GRIDSET_CLOSE					(WM_USER + 1237)

// CDlgBookMarkAdd 로부터의 메시지
#define DLG_BOOKMARK_ADD					(WM_USER + 1245)
#define DLG_BOOKMARK_ADD_CLOSE				(WM_USER + 1247)

// CDlgMapSelectSet 로부터의 메시지
#define DLG_MAPSELECT_CHANGE				(WM_USER + 1251)

// DlgCoordinate 로부터의 메시지
#define DLG_COORDINATE_SET					(WM_USER + 1261)
#define DLG_COORDINATE_CLOSE				(WM_USER + 1262)

// CDlgScale 로부터의 메시지
#define DLG_SCALE_SET						(WM_USER + 1243)
#define DLG_SCALE_CLOSE						(WM_USER + 1244)

// COutputWnd 로부터의 메시지
#define OUTPUT_SEARCH_AREA_SELECT			(WM_USER + 1265)
#define OUTPUT_SEARCH_NAME_SELECT			(WM_USER + 1266)
#define OUTPUT_SEARCH_AREA_MOVE				(WM_USER + 1267)
#define OUTPUT_SEARCH_NAME_MOVE				(WM_USER + 1268)
#define OUTPUT_SEARCH_COP_MOVE				(WM_USER + 1269)
#define OUTPUT_SEARCH_COP_MOVECENTER		(WM_USER + 1270)

// CIndexMapWnd로부터의 메시지
#define INDEXMAP_MOVE_CENTER				(WM_USER + 1221)
#define INDEXMAP_MOVE_AREA					(WM_USER + 1222)
#define INDEXMAP_SETTING					(WM_USER + 1223)

// MapObjectView 로부터의 지도요소 도시 설정 메시지
#define MAP_OBJECT_VIEW_ALL_CHECK			(WM_USER + 1061)
#define MAP_OBJECT_VIEW_ALL_UNCHECK			(WM_USER + 1062)
#define MAP_OBJECT_VIEW_CATEGORY_CHECK		(WM_USER + 1063)
#define MAP_OBJECT_VIEW_CATEGORY_UNCHECK	(WM_USER + 1064)
#define MAP_OBJECT_VIEW_LAYER_CHECK			(WM_USER + 1065)
#define MAP_OBJECT_VIEW_LAYER_UNCHECK		(WM_USER + 1066)
#define MAP_OBJECT_VIEW_FEATURE_CHECK		(WM_USER + 1067)
#define MAP_OBJECT_VIEW_FEATURE_UNCHECK		(WM_USER + 1068)
#define MAP_OBJECT_VIEW_MAP_AUTO_ONOFF		(WM_USER + 1069)
#define MAP_OBJECT_VIEW_ALL_SELECT			(WM_USER + 1070)
#define MAP_OBJECT_VIEW_CATEGORY_SELECT		(WM_USER + 1071)
#define MAP_OBJECT_VIEW_LAYER_SELECT		(WM_USER + 1072)
#define MAP_OBJECT_VIEW_FEATURE_SELECT		(WM_USER + 1073)

// COverlayObjectViewTree 로부터의 메시지 
#define OVERLAY_VIEW_SELECT_TITLE			(WM_USER + 1081)
#define OVERLAY_VIEW_SELECT_LAYER			(WM_USER + 1082)
#define OVERLAY_VIEW_SELECT_SHAPE			(WM_USER + 1083)
#define OVERLAY_VIEW_CHECK_TITLE			(WM_USER + 1084)
#define OVERLAY_VIEW_CHECK_LAYER			(WM_USER + 1085)
#define OVERLAY_VIEW_CHECK_SHAPE			(WM_USER + 1086)
#define OVERLAY_VIEW_UNCHECK_TITLE			(WM_USER + 1087)
#define OVERLAY_VIEW_UNCHECK_LAYER			(WM_USER + 1088)
#define OVERLAY_VIEW_UNCHECK_SHAPE			(WM_USER + 1089)
#define OVERLAY_VIEW_MOVETOP_LAYER			(WM_USER + 1090)
#define OVERLAY_VIEW_MOVETOP_SHAPE			(WM_USER + 1091)
#define OVERLAY_VIEW_MOVEUP_LAYER			(WM_USER + 1092)
#define OVERLAY_VIEW_MOVEUP_SHAPE			(WM_USER + 1093)
#define OVERLAY_VIEW_MOVEDOWN_LAYER			(WM_USER + 1094)
#define OVERLAY_VIEW_MOVEDOWN_SHAPE			(WM_USER + 1095)
#define OVERLAY_VIEW_MOVEBOTTOM_LAYER		(WM_USER + 1096)
#define OVERLAY_VIEW_MOVEBOTTOM_SHAPE		(WM_USER + 1097)
#define OVERLAY_VIEW_DELETE_LAYER			(WM_USER + 1098)
#define OVERLAY_VIEW_DELETE_SHAPE			(WM_USER + 1099)
#define OVERLAY_VIEW_NEW_LAYER				(WM_USER + 1100)
#define OVERLAY_VIEW_OPEN_LAYER				(WM_USER + 1101)
#define OVERLAY_VIEW_SAVE_LAYER				(WM_USER + 1102)
#define OVERLAY_VIEW_COORD_LAYER			(WM_USER + 1103)
#define OVERLAY_VIEW_SHAPE_PROPERTY			(WM_USER + 1104)
#define OVERLAY_VIEW_SHAPE_MOVE				(WM_USER + 1105)

// CMapPropertiesWnd 로부터의 메시지
#define MAPVIEW_PROP_EDIT_LAYERCOLOR		(WM_USER + 1215)
#define MAPVIEW_PROP_EDIT_FEATURECOLOR		(WM_USER + 1216)

// CMapObjectOrder 로부터의 지도요소 도시 설정 메시지
#define MAP_OBJECT_ORDER_LAYER_SET			(WM_USER + 1051)

// COverPropertiesWnd 로부터의 메시지
#define OVERLAY_PROP_COMMON_NAME			(WM_USER + 1001)
#define OVERLAY_PROP_COMMON_DESCRIPT		(WM_USER + 1002)
#define OVERLAY_PROP_EDIT_LINEWIDTH			(WM_USER + 1003)
#define OVERLAY_PROP_EDIT_LINECOLOR			(WM_USER + 1004)
#define OVERLAY_PROP_EDIT_LINESTYLE			(WM_USER + 1005)
#define OVERLAY_PROP_EDIT_FILLUSE			(WM_USER + 1006)
#define OVERLAY_PROP_EDIT_FILLCOLOR			(WM_USER + 1007)
#define OVERLAY_PROP_EDIT_FILLTRANCE		(WM_USER + 1008)
#define OVERLAY_PROP_EDIT_PATTERNUSE		(WM_USER + 1009)
#define OVERLAY_PROP_EDIT_PATTERNCOLOR		(WM_USER + 1010)
#define OVERLAY_PROP_EDIT_PATTERNSTYLE		(WM_USER + 1011)
#define OVERLAY_PROP_EDIT_ROTATE			(WM_USER + 1012)
#define OVERLAY_PROP_LINE_STRAIGHT			(WM_USER + 1013)
#define OVERLAY_PROP_LINE_STARTARROW		(WM_USER + 1014)
#define OVERLAY_PROP_LINE_ENDARROW			(WM_USER + 1015)
#define OVERLAY_PROP_LINE_CLOSED			(WM_USER + 1016)
#define OVERLAY_PROP_NGON_COUNT				(WM_USER + 1017)
#define OVERLAY_PROP_RECT_ROUND				(WM_USER + 1018)
#define OVERLAY_PROP_ARROW_TAIL				(WM_USER + 1019)
#define OVERLAY_PROP_ARROW_WIDTH			(WM_USER + 1020)
#define OVERLAY_PROP_TEXT_SIZE				(WM_USER + 1021)
#define OVERLAY_PROP_TEXT_COLOR				(WM_USER + 1022)
#define OVERLAY_PROP_TEXT_KIND				(WM_USER + 1023)
#define OVERLAY_PROP_TEXT_ALIGN				(WM_USER + 1024)
#define OVERLAY_PROP_TEXT_STRING			(WM_USER + 1025)
#define OVERLAY_PROP_TEXT_WEIGHT			(WM_USER + 1026)
#define OVERLAY_PROP_TEXT_ITALIC			(WM_USER + 1027)
#define OVERLAY_PROP_TEXT_UNDERLINE			(WM_USER + 1028)
#define OVERLAY_PROP_TEXT_STRIKEOUT			(WM_USER + 1029)
#define OVERLAY_PROP_LAYER_NAME				(WM_USER + 1030)
#define OVERLAY_PROP_MILPT_SIZE				(WM_USER + 1031)
#define OVERLAY_PROP_MILPT_COLOR			(WM_USER + 1032)
#define OVERLAY_PROP_MILPT_ADDINFO_COLOR	(WM_USER + 1033)

#define MAINVIEW_INVALIDATE_EXCLUDE_MAP		(WM_USER + 500)
#define MAINVIEW_INVALIDATE_INCLUDE_MAP		(WM_USER + 501)

// CDlgMapSelectSet 로부터의 메시지
#define DLG_MAPSELECT_CHANGE				(WM_USER + 1251)

// CDlgGraphicSection 로부터의 메시지
#define DLG_GRAPHIC_DIALOG_EXIT				(WM_USER + 1253)

// CDlgMapAutoChangeSet 로부터의 메시지
#define DLG_MAPAUTOCHANGE_CLOSE				(WM_USER + 1255)