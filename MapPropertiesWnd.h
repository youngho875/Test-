// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once
#include "PropertiesWnd.h"



// 속성 창
class CMapPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CMapPropertiesWnd();
	virtual ~CMapPropertiesWnd();

	// Property Kind 구별자
	enum emMapPropertyWndKind
	{
		MAPPROPERTY_NONE = 0,		// 없음 
		MAPPROPERTY_SEARCH,			// 검색 결과
		MAPPROPERTY_SELECT_LAYER,	// 레이어 선택 정보
		MAPPROPERTY_SELECT_FEATURE,	// 피쳐 선택 정보
	};

	enum emMapPropertyKind
	{
		MAPPROPERTY_TITLE = 0,		// 제목
		MAPPROPERTY_CONTENT,		// 일반 표기 항목
		MAPPROPERTY_COLOR,			// 색상
	};

	void AdjustLayout();

	// 구현입니다.
public:
	// 속성창 내용 초기화
	void ClearPropList();

	// 지형 요소 표기
	void SetSearchFeatureInfo(CString strTitle, vector<CString> vInfoName, vector<CString> vInfoContent);

	void AddSelectLayerInfo(LAYER_ATTRIBUTE* pLayerInfo, COLORREF clrLayer, BOOL bOnlyLayer);
	void AddSelectFeatureInfo(LAYER_ATTRIBUTE* pLayerInfo, FEATUREGROUP_ATTRIBUTE * pFeatureInfo, COLORREF clrFeature);

private:

	emMapPropertyWndKind m_emPropKind;

	int m_nLayerIndex;
	int m_nFeatureIndex;

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	LRESULT OnPropertyChanged( WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();
};

