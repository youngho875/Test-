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

#pragma once

//#include <vector>
//#include <map>
#include "OutputLogList.h"
#include "OutputSearchList.h"


/////////////////////////////////////////////////////////////////////////////
// COutputList 창

class COutputList : public CListBox
{
	// 생성입니다.
public:
	COutputList();

	// 구현입니다.
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};


class COutputWnd : public CDockablePane
{
	// 생성입니다.
public:
	COutputWnd(); 

	// 특성입니다.
protected:
	CFont m_Font;

	CMFCTabCtrl	m_wndTabs;
	COutputList m_wndOutputDebug;

	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputFind;
	COutputLogList m_wndOutputLog;

	UINT m_nExtTabID;
	std::map<CString, COutputSearchList*> m_mpwndOutputSearchResultEx;
	std::map<CString, UINT> m_mpwndOutputSearchResultExID;

public:
	void AddBuildWindow(CString& strMsg);
	void AddDebugWindow(CString& strMsg);
	void AddFindWindow(CString& strMsg);
	void ClearBuildWindow();
	void ClearDebugWindow();
	void ClearFindWindow();

	void SetActiveTab(CString strLabel = _T(""));
	void ClearSearch();
	void SetSearchScreen(CString strCategory, CString strLayer, std::vector<MAPSEARCH_LAYER_RESULT>& vResult);
	void SetSearchName(CString strLayer, std::vector<MAPSEARCH_LAYER_RESULT>& vResult);
	void SetSearchCOPBegin();
	void SetSearchCOP(CString strLayer, std::vector<MAPSEARCH_LAYER_RESULT>& vResult, SEARCHLIST_KIND emListType);


protected:
	COutputSearchList* NewSearchList(CString strLabel);
	COutputSearchList* GetSearchList(CString strLabel);
	UINT GetSearchListID(CString strLabel);
	void ClearSearchList();
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

	// 구현입니다.
public:
	virtual ~COutputWnd();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

