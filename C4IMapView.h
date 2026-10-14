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

// C4IMapView.h: CC4IMapView 클래스의 인터페이스
//

#pragma once
#include "MapSources.h"
#include "LocalOverlay.h"

#include "C4IMapDoc.h"
#include "MainFrm.h"

#include "GeoDC/Coord.h"
#include "GeoDC/CoordMgr.h"

//#include "PointDisplay.h"
//#include "FilePath.h"
#include "TmsMgr.h"
//#include "MapGeneratorDlg.h"
//#include "CaptureWindow.h"
#include "ComDefStringValue.h"
#include "GPoint.h"
#include "../inc/define_struct.h"

//#include "ShapeT.h"
#include "AltitudeMgr/Altitude.h"
//#include "ObjectProp.h"
//#include "PPTooltip.h"


//#import "Altitude.tlb" no_namespace named_guids



#define	 HOOKED_TRACK				0
#define	 HOOKED_GROUND				1
#define  HOOKED_SYMBOL				2
#define  HOOKED_SPECIAL_POINT		3
#define  HOOKED_DISPLAY_POINT		4
#define  HOOKED_AMDCELL				5
#define	 MAX_BEARING_RANGE_COUNT	20
#define	 NEW_SYMBOL					21
#define	 NEW_SYMBOL_MOVABLE			22
#define	 DETOXI_CALC				23
#define	 SYMBOL_PROP				24

// ToolTip 종류
//#define IDB_MAP                   3333

#define WM_OPTION_CHANGED			WM_USER+2	
#define WM_MOVE_WITH_COORD			WM_USER+3
#define WM_ON_MINIMAP_CLOSE			WM_USER+6


class COverlayThread;
class CDlgOverlayControler;
class CDlgObjectList;
class CDlgOverlayProperty;
class CDlgOverlayTextEdit;
class CDlgOverlayEditObject;
class CDlgOverlayEditObjectPoint;

class CGDoublePoint;
class CTmsMgr;


class CC4IMapView : public CView
{
protected: // serialization에서만 만들어집니다.
	CC4IMapView();
	DECLARE_DYNCREATE(CC4IMapView)

// 특성입니다.
public:
	CC4IMapDoc* GetDocument() const;

// 작업입니다.
public:
	//CMapConfigMgr* m_pMapConfigMgr;
	//CMapDataMgr* m_pMapDataMgr;
	//CGMapDisplayMgr* m_pMapDisplayMgr;

	//CPPToolTip	m_ToolTip;

	//int			m_nToolTipTrackNum;
	//char			m_szToolTipArmyCode[16];
	//CRect			m_ToolTipRect[5];


	// 작업입니다.
public:

	int OriginX;	// mbc geotiff
	int OriginY;	// mbc geotiff

	BOOL m_bDCInitialized;

	CDC* m_dcMain;
	CGDCGeo* m_dcMainTemp;
	CDC* m_dcMapLayer;
	CDC* m_dcShpLayer;
	//CDC* m_dcSymLayer;		// 심볼 데이터를 그리기 위한 DC (Layer)
	CDC* m_dcDTEDLayer;
	CDC* m_dcRasterLayer;

	CCoordMgr* m_pCoordMgr;
	CCoord m_Coord;

	CBitmap	m_pBitmap4MainTemp;
	CBitmap	m_pBitmap4Map;
	CBitmap	m_pBitmap4Shape;
	CBitmap m_pBitmap4DTED;
	CBitmap m_pBitmap4Raster;
	CBitmap	m_pBitmap4Sym;

	CBitmap* m_pOldBitmap4MainTemp;
	CBitmap* m_pOldBitmap4Map;
	CBitmap* m_pOldBitmap4Shape;
	CBitmap* m_pOldBitmap4DTED;
	CBitmap* m_pOldBitmap4Raster;
	CBitmap* m_pOldBitmap4Sym;

	CBrush m_brush_10_10_10;
	CRect rect;  

	//CWinThread* m_Thread;

	CDC BufferDC;
	CBitmap bmpBuffer;
	BOOL mb_moving;

	int m_nHookType;

	char m_lat_cur[8], m_long_cur[9];

	CRect m_recSave;
	double 	m_fPrevScale;
	BOOL m_bRaster;
	BOOL m_bsatellite;
	BOOL m_bShape;
	BOOL m_bDted;
	BOOL m_bMapMove;
	BOOL m_bPanning;
	BOOL m_bOnZoom;
	BOOL m_bIsDrawSituationInProcess;	// 현재 그리기 작업중.

	BOOL m_bOverlayAdd;					// HJYOO : 현재 로드된 투명도전시여부
	BOOL m_bOverlayLoad;

	CPoint m_ptMapMoveFirstPoint;
	CPoint m_ptPrevPoint;
	CPoint m_ptZoomWindowLT, m_ptZoomWindowBR;
	CPoint m_ptZoomStart, m_ptZoomEnd;

	//CCaptureWindow scrCap;
	//CPointDisplay  m_PointDisplay;

	//CObjectProp* m_adjObj;
	CTmsMgr m_TmsMgr;

public:
	COverlayThread* m_pOverlayThread;

	CDlgOverlayControler* m_pDlgOverlayControler;
	CDlgObjectList* m_pDlgObjectList;
	CDlgOverlayProperty* m_pDlgOverlayProperty;
	CDlgOverlayTextEdit* m_pDlgOverlayTextEdit;
	CDlgOverlayEditObject* m_pDlgOverlayEditObject;
	CDlgOverlayEditObjectPoint* m_pDlgOverlayEditObjectPoint;

	int m_nOverlyaEditObjectPointX;
	int m_nOverlyaEditObjectPointY;

	Gdiplus::Bitmap* m_pDoubleBuffer;	// 더블버퍼링용 이미지

	// 도시 형태
	//ICOPS::SWarSymDrawMode		m_sDrawMode;
	//BOOL	m_bSpeedTest;


/*
	//CMilitarySymbol		m_MilitarySymbol;

	// 바탕 이미지
	Gdiplus::Bitmap		*m_pBgImg;

	// 프로젝션
	ICOPS::IMapProjection	*m_piMapProj;
	double	m_rMPW;

	// 아이콘 편집
	ICOPS::IWarSymEditTool	*m_piEditTool;
	//CC4IRTDSViewEditOwner	*m_pEditOwner;
	CString		m_strLastSymCode;	// 최근에 선택된 부호 코드

	// 화면 조절
	Gdiplus::PointF		m_ptCenter;		// 화면 중심 이동
	Gdiplus::REAL		m_fZoom;		// 화면 배율
	Gdiplus::Bitmap* m_pDoubleBuffer;	// 더블버퍼링용 이미지

	// 도시 형태
	ICOPS::SWarSymDrawMode		m_sDrawMode;
	BOOL	m_bCacheSymImg;

	// 속도 시험용 부호
	//void	DrawSpeedTest(Gdiplus::Graphics *pGrpx);
	ICOPS::CTypedBuffer<ICOPS::IWarSymbol *>	m_piSyms;
	ICOPS::CTypedBuffer<Gdiplus::PointF>	m_testPts;
	BOOL	m_bSpeedTest;
	BOOL	m_bSymbolDraw;

	void OnSetSymbol();
	void OnPrepareGraphics(Gdiplus::Graphics* pGrpx, const CRect& rcBounds);
	void DrawOverItems(Gdiplus::Graphics *pGrpx);
	void ApplyDrawMode(CTest_OverItem *pItem);
	void ApplyDrawModeToAll();
	Gdiplus::Color GetBgColor();

	void DrawMilitarySymbol(//CDC *pDC);
	void SymZoomIn();
	void SymZoomOut();
	void MoveToSelectedCenter();
	void Pan(int dirX, int dirY);

	// IWarSymEditOwner implementation

	virtual void	GetEditables(ICOPS::CPtrArrayIWarSymEditable &ret);
	virtual void	AddEditable(ICOPS::IWarSymEditable *piEditable, BOOL bCompleted);
	virtual void	DeleteEditable(ICOPS::IWarSymEditable *piEditable);
	virtual void	DoContextMenu(Gdiplus::Graphics *pGrpx, UINT nFlags, const CPoint &point, CPtrArrayIWarSymEditable *pSelected);
	virtual void	Refresh();
	virtual CWnd	*GetWnd();

	void	SelectSymbol();
	void	LoadBackgroundImage();
	void	InitProjection();
	void	InitSymbolEditTool();
*/

private:
	// 후크된 SDL 파일 인덱스
	int m_nHookSDL;
	// 후크된 심볼 인덱스 
	int m_nHookSDLSymbol;
	// 후크된 오브젝트 타입(0:항적,1:지형,2:심볼,3:Special Point) 
	//int m_nHookType;

	// 후크된 지점 좌표
	GPoint m_fHookPoint;

public:
	//CFilePath	m_pathConfig;
	//CFilePath	m_pathDTED;
	//CFilePath	m_pathScrCap;

	MAP_STATUS_INFO	m_pStatusInfo; /**< 상태바 정보 */

	MOUSE_STATE m_pMouseState; /**< 마우스 상태 */
	MOUSE_STATE m_pMouseStateOld; /**< 마우스 기존 상태 */
	MOUSE_STATE m_pMouseStateCoordOld;	/**< 마우스 Ctrl key 변경 상태 */

	SEARCH_OPTION m_pSearchOption; /**< 검색 모양 */
	STATE_COORD m_pStateBarCoord; /**< 상태바 표시 정보 */

	MOUSE_STATE m_pMouseOldStatus;	/**< 마우스 변경 이전 상태 */

	//CMapGeneratorDlg* m_generateDlg;

public:
	CMainFrame* get_mainfrm();

	void draw_maindc_only(CPoint ptNewPoint);
	void DrawSituation(BOOL bMapRedraw, BOOL bSymRedraw, BOOL bMCRCRedraw, CPoint pStartPoint);
	void DrawMap();

	void InitializeDisplayParameter(CRect pClientRect);

	void ZoomIn();
	void ZoomOut();

	void DrawZoomRect(CDC* pDC, CRect& pRect);

	void ReInitializeDCs();

	void DrawShape(void);
	//void worldmapShpDraw(CGDCGeo* pDC, double dZoomFactor, int iViewType);

	void ScreenCaptureRouting();
	bool smallworldOpen(CString _filename);

	void SetStatusBar1Txt(CString strMsg);
	void SetStatusBar2Txt(CString strMsg);
	void UpdateStatusBar(CPoint point);

	void ChangeScale(float fScale);
	BOOL HitestDisplayPoint(double fXCoord, double fYCoord, double fCenterX, double fCenterY, double fScale);
	BOOL HitTest(double fXCoord, double fYCoord, double fCenterX, double fCenterY, double fScale);

	MOUSE_STATE GetMouseState();
	void SetMouseState(MOUSE_STATE pState);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CC4IMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//임시로 만듬
	//std::vector<GPoint> mvecPts;
	//void gdalshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);
	//void shapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);
	//void LoadShp();
	//void OpenShapeFile(/*string filename*/);

	//CGDtedMgr* m_dted;
	//IAltitude* sp_Height;

	CAltitude* mHeights;

	BOOL BeginView();

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    CPoint m_localClick;
    void ShowLocalFeature(CPoint point);
    afx_msg void OnLocalSettings();
    afx_msg void OnLocalIdentify();
    afx_msg void OnLocalToggle();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	CLocalOverlay m_localOverlay;
    afx_msg void OnLocalOverlayCommand(UINT id);
    afx_msg void OnUpdateLocalOverlay(CCmdUI* ui);
    afx_msg void OnCaptureChanged(CWnd* wnd);
    virtual BOOL PreTranslateMessage(MSG* msg);
    afx_msg void OnMapSourcesXml();
    afx_msg void OnTimer(UINT_PTR id);
    DECLARE_MESSAGE_MAP()

public:
    BOOL SaveLocalOverlayIfModified();
    void ResetLocalOverlay(){m_localOverlay.Clear();Invalidate(FALSE);}
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnOptionChange(WPARAM pText, LPARAM nFlag);
	afx_msg LRESULT move_with_coord(WPARAM w, LPARAM l);

	//afx_msg void OnComboChangeScale();
	afx_msg void OnComboCenterChange();
	afx_msg void OnCheckSatellite();
	afx_msg void OnUpdateCheckSatellite(CCmdUI* pCmdUI);
	afx_msg void OnBtnMapgenerater();
	afx_msg void OnCheckRaster();
	afx_msg void OnCheckShape();
	afx_msg void OnUpdateCheckRaster(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheckShape(CCmdUI* pCmdUI);
	afx_msg void OnCheckDted();
	afx_msg void OnUpdateCheckDted(CCmdUI* pCmdUI);

	// 지도 핸들링
	afx_msg void OnMenuMouseBase();
	afx_msg void OnUpdateMenuMouseBase(CCmdUI* pCmdUI);
	afx_msg void OnMenuMouseZoomIn();
	afx_msg void OnUpdateMenuMouseZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnMenuMouseZoomOut();
	afx_msg void OnUpdateMenuMouseZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnMenuMousePanning();
	afx_msg void OnUpdateMenuMousePanning(CCmdUI* pCmdUI);

public:
	// 투명도 
	//afx_msg void OnOverlayDraw();
	//afx_msg void OnUpdateOverlayDraw(CCmdUI* pCmdUI);
	//afx_msg void OnOverlayNew();
	//afx_msg void OnUpdateOverlayNew(CCmdUI* pCmdUI);
	//afx_msg void OnOverlayClear();
	//afx_msg void OnOverlayProperty();			
	//afx_msg void OnOverlayMoveBottom();			
	//afx_msg void OnOverlayMoveTop();			
	//afx_msg void OnOverlayEditObject();			
	//afx_msg void OnOverlayGroup();				
	//afx_msg void OnOverlayGroupRelease();		
	//afx_msg BOOL OnOverlayPopUpMenu(CPoint point);
};

#ifndef _DEBUG  // C4IMapView.cpp의 디버그 버전
inline CC4IMapDoc* CC4IMapView::GetDocument() const
   { return reinterpret_cast<CC4IMapDoc*>(m_pDocument); }
#endif

