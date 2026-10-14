#include "pch.h"
#include "C4IMap.h"
#include "MainFrm.h"
#include "IndexMapView.h"



CIndexMapView::CIndexMapView()
{
}

CIndexMapView::~CIndexMapView()
{
}

BEGIN_MESSAGE_MAP(CIndexMapView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

int CIndexMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	if (!m_wndIndexMapWnd.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	m_wndIndexMapViewToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_INDEXMAP_TOOLBAR);
	m_wndIndexMapViewToolBar.LoadToolBar(IDR_INDEXMAP_TOOLBAR, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndIndexMapViewToolBar.SetPaneStyle(m_wndIndexMapViewToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndIndexMapViewToolBar.SetPaneStyle(m_wndIndexMapViewToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndIndexMapViewToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndIndexMapViewToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}

void CIndexMapView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CIndexMapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	return;
}

void CIndexMapView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndIndexMapViewToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndIndexMapViewToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndIndexMapWnd.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CIndexMapView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndIndexMapWnd.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));

	//m_wndIndexMapWnd.DrawMapBitmap(&dc);
}

void CIndexMapView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndIndexMapWnd.SetFocus();
}

void CIndexMapView::OnChangeVisualStyle()
{
	m_wndIndexMapViewToolBar.CleanUpLockedImages();
	m_wndIndexMapViewToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_INDEXMAP_TOOLBAR_24 : IDR_INDEXMAP_TOOLBAR, 0, 0, TRUE /* 잠금 */);
}
