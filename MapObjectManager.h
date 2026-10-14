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

#include "MapObjectViewTree.h"
#include "MapObjectOrderList.h"

class CMapObjectManager : public CDockablePane
{
// 생성입니다.
public:
    void RefreshLocalShapeTree(){m_wndMapObjectViewTree.RefreshLocalShapeTree();}
	CMapObjectManager();
	virtual ~CMapObjectManager();
	
	// 구현입니다.
public:
	void SetMapAutoScale(BOOL bAutoSacle);	// 축척별 자동 설정
	void ClearMapObjectAll(BOOL bOnlyOrderList = FALSE);				// 정보 초기화
	void AddMapObjectLayer(CString strCategory, CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex, BOOL bOnlyOrderList = FALSE);	// 지도 레이어 데이터 추가
	void AddMapObjectFeature(CString strLayerName, CString strElementName, int nFeatureIndex);	// 지도 레이어 별 피쳐 데이터 추가

	BOOL GetMapObjectLayerChangeInfo(std::vector<MAP_OBJECT_ORDER_LAYERCHANGE> &vMapObjectLayerChange);	// 순서가 변경된 레이어 목록을 전달한다.

	// 레이어 도시 표기 설정
	void SetVisibleMapObjectLayer(CString strLayerName, BOOL bVisible);
	// 피쳐 도시 표기 설정
	void SetVisibleMapObjectFeature(CString strLayerName, CString strElementName, BOOL bVisible);

	// 그룹에 해당하는 레이어 인덱스 목록 획득
	BOOL GetLayerIndexByCategory(CString strCategoryName, std::vector<int> &vLayerIndex);

	// 위성영상 도시 표기 설정
	void SetVisibleMapObjectCategoryRaster(BOOL bVisible);
	// 음영기복도 도시 표기 설정
	void SetVisibleMapObjectCategoryDTED(BOOL bVisible);


// 특성입니다.
protected:
	CFont m_Font;
	CMFCTabCtrl	m_wndTabs;

	CMapObjectViewTree m_wndMapObjectViewTree;
	CImageList m_MapObjectViewImages;				// 지도 레이어 트리 아이템 이미지
	CImageList m_TreeImagesCheckBox;				// 지도 레이어 트리 체크 이미지

	CMapObjectOrderList m_wndMapObjectOrderList;
	CImageList m_MapObjectOrderImages;					// 레이어 목록 아이템 이미지

protected:
	void OnChangeVisualStyleTree();		// 도시 스타일 변경
	void OnChangeVisualStyleList();		// 도시 스타일 변경	

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

