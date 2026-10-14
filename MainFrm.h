// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include "LayerView.h"
#include "ItemView.h"
#include "TypeView.h"
#include "IndexMapView.h"
//#include "OutputWnd.h"
#include "MapObjectManager.h"
#include "MapPropertiesWnd.h"
#include "ShapeT.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	BOOL CreateCaptionBar();

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	//CFileView         m_wndFileView;
	//CClassView        m_wndClassView;
	
	//CPropertiesWnd    m_wndProperties;
	CMFCCaptionBar	  m_wndCaptionBar;
	//CMFCStatusBar     m_wndStatusBar;

	//CComboBox		m_ctrlCombo;
	//CComboBox		m_ctrlCombo1;

	//COutputWnd			 m_wndOutput;
	CTypeView			m_wndCOPTypeView;	// 상황도 유형 창
	CItemView 			m_wndCOPItemView;	// 상황도 도시요소 창
	CLayerView			m_wndCOPLayerView;	// 상황도 레이어 창
	CIndexMapView		m_wndIndexMapView;	// 인덱스 레이어 창
	CMapObjectManager	m_wndMapObjectManager;
	CMapPropertiesWnd	m_wndMapProperties;
	CShapeT				m_indexMap;

	//COverlayItemView 			m_wndOverlayItemView;
	//COverlayLayerView			m_wndOverlayLayerView;

	CMFCRibbonEdit* m_pRibbonMapCurrentScale;		// 도시 축척
	CMFCRibbonEdit* m_pRibbonMapMinScale;			// 도시 최소 축척
	CMFCRibbonEdit* m_pRibbonMapMaxScale;			// 도시 최대 축척
	CMFCRibbonEdit* m_pRibbonMapZoomScale;			// 확대/축소 간격 축척

	CMFCRibbonComboBox* m_pRibbonMapBookMarkCombo;	// 즐겨찾기 목록

	CMFCRibbonSlider* m_pRibbonMapBrightSlider;	// 밝기 슬라이드
	CMFCRibbonEdit* m_pRibbonMapBrightEdit;		// 밝기 에디트	
	CMFCRibbonSlider* m_pRibbonMapContrastSlider;	// 명암 슬라이드
	CMFCRibbonEdit* m_pRibbonMapContrastEdit;	// 명암 에디트

	CMFCRibbonButton* m_pRibbonMapAutoChange;		// 축척 변경 시 지도 자동 조절 On/Off

	CMFCRibbonComboBox* m_pRibbonSearchAreaTarget;		// 공간검색 레이어 명칭
	CMFCRibbonComboBox* m_pRibbonSearchAreaType;		// 공간검색 화면 선택 방식
	CMFCRibbonColorButton* m_pRibbonSearchAreaColor;	// 공간검색 결과 색상

	//CMFCRibbonComboBox* m_pRibbonSearchMapLayer;		// 검색어 검색 레이어 명칭
	CMFCRibbonComboBox* m_pRibbonSearchMapField;		// 검색어 검색 필드 명칭
	CMFCRibbonEdit* m_pRibbonSearchMapInput;			// 검색어

	CMFCRibbonEdit* m_pRibbonAnalysisVisibleRange;			// 가시권 분석 범위
	CMFCRibbonEdit* m_pRibbonAnalysisVisibleHeight;			// 가시권 분석 높이
	CMFCRibbonColorButton* m_pRibbonAnalysisVisibleColor;	// 가시권 분석 색상

	CMFCRibbonEdit* m_pRibbonRadiusInterval;			// 반경 측정 간격
	CMFCRibbonEdit* m_pRibbonRadiusRange;				// 반경 측정 범위
	CMFCRibbonComboBox* m_pRibbonAnalysisRadiusWidth;	// 반경 측정 선 굵기
	CMFCRibbonColorButton* m_pRibbonAnalysisRadiusColor;	// 반경 측정 선 색상

	// 상황도
	CMFCRibbonSlider* m_pRibbonCOPItemSizeSlider;
	CMFCRibbonSlider* m_pRibbonCOPItemBrightSlider;
	CMFCRibbonEdit* m_pRibbonCOPItemSizeSliderEdit;
	CMFCRibbonEdit* m_pRibbonCOPItemBrightSliderEdit;
	CMFCRibbonColorButton* m_pRibbonCOPItemColor;
	CMFCRibbonCheckBox* m_pRibbonCOPItemColorOnOff;
	CMFCRibbonButton* m_pwndCOPLayerViewOnOff;	// 상황도 레이어 On/Off
	CMFCRibbonButton* m_pwndCOPItemViewOnOff;	// 상황도 도시요소 창 On/Off
	CMFCRibbonButton* m_pwndCOPTypeViewOnOff;	// 상황도 유형 창 On/Off
	CMFCRibbonButton* m_pwndCOPImportantPatrol;	// 중요도시요소 패트롤 창 
	CMFCRibbonComboBox* m_pRibbonCOPSearchLayer;		// 상황도 공간검색 레이어 명칭
	CMFCRibbonComboBox* m_pRibbonCOPSearchColumn;		// 상황도 공간검색 컬럼 명칭
	CMFCRibbonColorButton* m_pRibbonCOPSearchColor;	// 상황도 공간검색 결과 색상
	CMFCRibbonComboBox* m_pRibbonCOPSearchType;		// 상황도 공간검색 화면 선택 방식
	CMFCRibbonEdit* m_pRibbonCOPSearchWord;			// 검색어

	CMFCRibbonButton* m_pwndOverlayLayerViewOnOff;	// 투명도 레이어 On/Off
	CMFCRibbonButton* m_pwndOverlayItemViewOnOff;	// 투명도 도시요소 창 On/Off

	CMFCRibbonColorButton* m_pRibbonDrawShapeLineColor;		// 투명도 속성 선 색상
	CMFCRibbonGallery* m_pRibbonDrawShapeLineWidth;
	CMFCRibbonGallery* m_pRibbonDrawShapeLineDesh;
	CMFCRibbonGallery* m_pRibbonDrawShapeLineBow;
	CMFCRibbonColorButton* m_pRibbonDrawShapeFillColor;		// 투명도 속성 내부 색상
	CMFCRibbonColorButton* m_pRibbonDrawShapePatternColor;	// 투명도 속성 패턴 색상
	CMFCRibbonColorButton* m_pRibbonDrawShapeGradientColor;	// 투명도 속성 그라데이션 색상

	CMFCRibbonEdit* m_pRibbonDrawShapeTextString;			// 투명도 속성 폰트 내용(도형 삽입 텍스트)
	CMFCRibbonFontComboBox* m_pRibbonDrawShapeTextKind;		// 투명도 속성 폰트 종류
	CMFCRibbonComboBox* m_pRibbonDrawShapeTextSize;			// 투명도 속성 폰트 크기
	CMFCRibbonColorButton* m_pRibbonDrawShapeTextColor;		// 투명도 속성 폰트 색상
	CMFCRibbonEdit* m_pRibbonDrawShapeTextLineInterval;		// 투명도 속성 폰트 줄간격
	CMFCRibbonGallery* m_pRibbonDrawGallery;				// 투명도 도형 그리기

	// 추후에 다른곳으로 이동 구현
	typedef struct IMX_REP_Gallery
	{
		int nGalleryID;
		int nGalleryGrpImageID;
		int nGalleryImageID;
		CString strGalleryImageStr;
	} IMX_REP_Gallery, * LPIMX_REP_Gallery;

	int m_nRibbonDrawGalleryRecently;
	std::map<int, IMX_REP_Gallery> m_mRibbonDrawGallery;	// 투명도 도형 그리기 툴팁
	std::vector<int> m_vRibbonDrawGalleryRecently;

	CMFCToolBarImages m_mtbi_Line;							// 투명도 선 이미지
	CMFCToolBarImages m_mtbi_Rect;							// 투명도 사각형 이미지
	CMFCToolBarImages m_mtbi_default;						// 투명도 기본도형 이미지
	CMFCRibbonGallery* m_pRibbonSortGallery;				// 투명도 도형 정렬
	CMFCRibbonGallery* m_pRibbonStyleGallery;				// 투명도 도형 스타일

	CMFCRibbonGallery* m_pRibbonGradationGallery;				// 투명도 도형 그라데이션
	CMFCRibbonGallery* m_pRibbonTextureGallery;				// 투명도 도형 질감
	CMFCRibbonGallery* m_pRibbonPatternGallery;				// 투명도 도형 패턴


	std::vector<CString> m_vSystemFontList;	// 시스템 폰트 종류 백업

	std::map<CString, CMFCRibbonCategory*> m_mRibbonCategory;


	// 3D
	int							m_nCopSymDraw;
	BOOL						m_bSunSystemTime;
	BOOL						m_bDtedAutoRate;
	BOOL						m_bCopSymDraw;
	BOOL						m_bCopSymDrawModify;
	double						m_dScale;
	double						m_dResolution;
	CMFCRibbonButton* m_p3D1Mesh;
	CMFCRibbonButton* m_p3D2Mesh;
	CMFCRibbonButton* m_p3DPolygon;
	CMFCRibbonButton* m_p3DShadow;
	CMFCRibbonButton* m_p3DSpectrum;
	CMFCRibbonButton* m_p3DTexture;
	CMFCRibbonButton* m_pGrid;
	CMFCRibbonButton* m_pRibbonSky;
	CMFCRibbonButton* m_pRibbonRain;
	CMFCRibbonButton* m_pRibbonSnow;
	CMFCRibbonButton* m_pRibbonFog;
	CMFCRibbonSlider* m_pRenderFogDepthSlider;
	CMFCRibbonEdit* m_pRenderFogDepthEdit;
	CMFCRibbonSlider* m_pRenderSpeedSlider;
	CMFCRibbonEdit* m_pRenderSpeedEdit;
	CMFCRibbonButton* m_pRibbonDtedAuto;
	CMFCRibbonSlider* m_pRenderDtedSlider;
	CMFCRibbonEdit* m_pRenderDtedEdit;
	CMFCRibbonButton* m_pSunSystemTime;
	CMFCRibbonSlider* m_pSunUserTimeSlider;
	CMFCRibbonEdit* m_pSunUserTimeEdit;
	CMFCRibbonButton* m_pCopSymDrawOnOff;
	CMFCRibbonButton* m_pCopSymDraw;
	CMFCRibbonButton* m_pCopSymDrawModify;
	CMFCRibbonButton* m_pClipboard;

	// 인쇄
	CMFCRibbonBar		m_wndPrintRibbonBar;	/**< 리본바 */
	CMFCRibbonButton* m_pPrintRun;
	CMFCRibbonButton* m_pPrintSet;
	CMFCRibbonButton* m_pPageH;
	CMFCRibbonButton* m_pPageW;
	CMFCRibbonButton* m_pPageReal;
	CMFCRibbonComboBox* m_pPrintPageKindCombo;
	CMFCRibbonComboBox* m_pPrintPageCountCombo;

	// pane
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane* m_pCurrOutlookPage;

	CList<COLORREF, COLORREF> m_lstMainColors;
	CList<COLORREF, COLORREF> m_lstAdditionalColors;
	CList<COLORREF, COLORREF> m_lstStandardColors;

	CMFCRibbonButton* m_pRibbonMapMgrOnOff;			// 지도 레이어 관리 창 On/Off
	CMFCRibbonButton* m_pRibbonOverlayOnOff;		// 투명도 레이어 관리 창 On/Off
	CMFCRibbonButton* m_pRibbonMapPropertyOnOff;	// 지도 속성 창 On/Off
	CMFCRibbonButton* m_pRibbonOverPropertyOnOff;	// 투명도 속성 창 On/Off
	CMFCRibbonButton* m_pRibbonIndexMapOnOff;		// 인덱스 지도 창 On/Off
	CMFCRibbonButton* m_pRibbonOutputOnOff;			// 출력 창 On/Off

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);

	//afx_msg void OnMenuOutputOnOff();
	//afx_msg void OnUpdateMenuOutputOnOff(CCmdUI* pCmdUI);
	afx_msg void OnLayerOnOff();
	afx_msg void OnUpdateLayerOnOff(CCmdUI* pCmdUI);
	afx_msg void OnItemOnOff();
	afx_msg void OnUpdateItemOnOff(CCmdUI* pCmdUI);
	afx_msg void OnTypeOnOff();
	afx_msg void OnUpdateTypeOnOff(CCmdUI* pCmdUI);
	afx_msg void OnMenuIndexMapOnOff();
	afx_msg void OnUpdateMenuIndexMapOnOff(CCmdUI* pCmdUI);
	afx_msg void OnMenuMapManagerOnOff();
	afx_msg void OnUpdateMenuMapManagerOnOff(CCmdUI* pCmdUI);
	afx_msg void OnMenuMapPropOnOff();
	afx_msg void OnUpdateMenuMapPropOnOff(CCmdUI* pCmdUI);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

public:
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	void CreateDocumentColors();
	void InitializeRibbon();
	void AddRibbonCategory(CString strCategoryName, CMFCRibbonCategory* pCategory);
	CMFCRibbonCategory* GetRibbonCategory(CString strCategoryName);
	void SetRibbonColorButton(CMFCRibbonColorButton* pRibbonColorButton, COLORREF clrInit);

	void SetOverlayRecentlyDefDrawItem(CMFCToolBarImages& mtbi_Src, UINT nSize);
	void AddOverlayRentlyDrawItem(int nIdx);
	CString GetOverlayDrawItem(int nIdx);
	size_t SetOverlayRecentlyDrawItemImg(CMFCToolBarImages& mtbi_Src);
	void SetOverlayRecentlyRankDrawItem(CMFCToolBarImages& mtbi_Src, UINT nSize);
	int SetOverlayRecentlyDrawItem(CMFCToolBarImages& mtbi_Src, CMFCToolBarImages& mtbi_Target, int nIdx);

};


