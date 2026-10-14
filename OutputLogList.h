#pragma once


// COutputLogList

class COutputLogList : public CListBox
{
	DECLARE_DYNAMIC(COutputLogList)

public:
	COutputLogList();
	virtual ~COutputLogList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnLogCopy();
	afx_msg void OnUpdateLogCopy(CCmdUI* pCmdUI);
	afx_msg void OnLogClear();
	afx_msg void OnUpdateLogClear(CCmdUI* pCmdUI);
	afx_msg void OnLogAllClear();
	afx_msg void OnUpdateLogAllClear(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
