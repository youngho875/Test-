#ifndef _OPTION_TREECTRL_H_
#define _OPTION_TREECTRL_H_

#pragma once

#include "Options.h"

class COptionsTreeCtrl : public CTreeCtrl
{
// Construction
public:
	COptionsTreeCtrl();

// Attributes
public:
	COptions* m_pOptions;
	
// Operations
public:
	void Initialize(char* strExpand);
	void ExpandItem(HTREEITEM hItem);
	void EnableItem(HTREEITEM hItem, TOPTION_ITEM::EENABLE_FLAGS eEnabled, BOOL b);
	void InvalidateItem(HTREEITEM hItem, BOOL self);
	void AttachOptions(COptions* p) { m_pOptions = p; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionsTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionsTreeCtrl)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void ApplyLoad(void);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONTREECTRL_H__C422DA81_B1D3_11D2_9CCB_000021001339__INCLUDED_)
