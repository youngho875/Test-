#pragma once

#include <afxdockablepane.h>
#include "IndexMapWnd.h"


class CIndexMapViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CIndexMapView : public CDockablePane
{
	// 생성입니다.
public:
	CIndexMapView();
	virtual ~CIndexMapView();

	void AdjustLayout();
	void OnChangeVisualStyle();


	// 특성입니다.
protected:
	CIndexMapWnd m_wndIndexMapWnd;					// 인덱스 지도 뷰
	CIndexMapViewToolBar m_wndIndexMapViewToolBar;	// 인덱스 지도 툴바

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

