#pragma once


#include "ViewList.h"


class CItemToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CItemView : public CDockablePane
{
public:
	CItemView();
	virtual ~CItemView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	CListCtrl* GetView();
	//void SetMsgMgr(CMapXMsgProcBase_List* pMsgBaseList);

protected:
	void msgBaseListPatrol(BOOL bPatrol);
	void ResetDrawItemFind();

protected:
	CItemToolBar m_wndCOPToolBar;
	CViewList m_wndView;
	CImageList m_COPViewImages;
	UINT m_nCurrSort;

private:
	int								m_nDrawItemListClickNum;
	int								m_nlistCtrl_DrawItem_Rs_cursor;
	/*
	@brief 도시요소 레이어 관리자
	*/
	//CMapXMsgProcBase_List* m_pMsgBaseList;

	void FillClassView();

	// 재정의입니다.
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnCOPAddMemberFunction();
	afx_msg void OnCOPAddMemberVariable();
	afx_msg void OnCOPDefinition();
	afx_msg void OnCOPProperties();
	afx_msg void OnNewFolder_COP();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	afx_msg void OnNMClickListDrawitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListDrawitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListDrawitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickListDrawitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawListDrawitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnGetdispinfoOwnerDrawitem(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};