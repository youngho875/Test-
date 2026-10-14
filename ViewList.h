#pragma once


class CViewList : public CListCtrl
{
public:
	CViewList();

protected:
	BOOL CopyToClipboard(LPCTSTR lpszSeparator = _T("\t"), BOOL bCopyHeaderText = FALSE);

	// 재정의입니다.
protected:

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 구현입니다.
public:
	virtual ~CViewList();

protected:
	DECLARE_MESSAGE_MAP()

};

