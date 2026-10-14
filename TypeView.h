#pragma once

#include "ViewTree.h"

class CTypeToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CTypeView : public CDockablePane
{
public:
	CTypeView();
	virtual ~CTypeView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	CTreeCtrl* GetView();
	//void SetMsgMgr(CMapXMsgProcBase_List* pMsgBaseList);

protected:
	void MoveListgrp(CString* pStrGrpKey);
	void UnCheckChildItem(HTREEITEM hItem);
	void CheckChildItem(HTREEITEM hItem);

protected:
	CTypeToolBar m_wndCOPToolBar;
	CViewTree m_wndCOPView;
	CImageList m_COPViewImages;
	UINT m_nCurrSort;

private:
	//CMapXMsgProcBase_List* m_pMsgBaseList;
	int			m_nGrpFstItemNum;

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

	afx_msg void OnNMClickTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnItemexpandedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnItemChangedTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMTVStateImageChangingTreeDrawType(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

