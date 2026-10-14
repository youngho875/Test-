
#pragma once

#include "../inc/define.h"
#include "MessageDefine.h"
#include <vector>
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CMapObjectViewTree 창입니다.

class CMapObjectViewTree : public CTreeCtrl
{
// 생성입니다.
public:
	CMapObjectViewTree();

	void RefreshLocalShapeTree();
	BOOL m_bAutoScale;	// 축척별 자동 설정 여부
	
	HTREEITEM InsertMapObjectLayer(CString strCategory, CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex);	// 지도 레이어 데이터 추가
	void InsertMapObjectFeature(CString strLayerName, CString strElementName, int nFeatureIndex);	// 지도 레이어 별 피쳐 데이터 추가

	void SetMapAutoScale(BOOL bAutoSacle);	// 축척별 자동 설정

	// 레이어 도시 설정
	void SetCheckMapObjectLayer(CString strLayerName, BOOL bVisible);
	// 피쳐 도시 설정
	void SetCheckMapObjectFeature(CString strLayerName, CString strElementName, BOOL bVisible);
	// 도시 설정 갱신
	void UpdateCheckMapObject();

	// 위성영상 도시 설정
	void SetCheckMapObjectCategoryRaster(BOOL bVisible);
	// 음영기복도 도시 설정
	void SetCheckMapObjectCategoryDTED(BOOL bVisible);

	// 모든 아이템 삭제
	void RemoveAllMapObjectViewItems(BOOL bInitTitle = TRUE);
	// 그룹에 해당하는 레이어 인덱스 목록 획득
	BOOL GetLayerIndexByCategory(CString strCategoryName, std::vector<int> &vLayerIndex);

private:
    HTREEITEM m_baseRoot=nullptr;
    std::map<HTREEITEM,int> m_baseItems;
    HTREEITEM m_localShapeRoot=nullptr;
    std::vector<HTREEITEM> m_sourceRoots;
    std::map<HTREEITEM,size_t> m_sourceItems;
    void RefreshSourceTree();
    std::map<HTREEITEM,size_t> m_localShapeItems;
    void ApplyLocalShapeChecks(HTREEITEM item);
	// 지도 레이어 트리 아이템 맵
	CMapStringToPtr m_pMapCategoryName; /**< Map for Category name */
	CMapStringToPtr m_pMapLayerName;    /**< Map for Layer name */
	CMapStringToPtr m_pMapFeatureName;  /**< Map for Feature name */

	HTREEITEM m_hRootItem;

	// 트리아이템 삭제
	void RemoveTreeItem(HTREEITEM hItem, BOOL bRemoveNextSiblingItem);
	// 트리아이템 별 맵 키 삭제
	void RemoveMapKey(int nItemType, CString strMyName, CString strParentName);

	// 체크 불가 설정
	void SetDisableCheck(HTREEITEM hItem, BOOL bDisable);
	// 체크 이벤트 발생
	void SetCheckEventItem(HTREEITEM hItem);
	// 체크 이벤트에 따른 하위 업데이트
	void UpdateCheckEventChildItem(HTREEITEM hItem, BOOL bCheck);
	// 체크 이벤트에 따른 상위 업데이트
	void UpdateCheckEventParentItem(HTREEITEM hItem);
	// 체크 이벤트 발생 메시지 전달
	void SendMsgSelectCheckItem(HTREEITEM hItem, BOOL bChecked);
	// 선택 이벤트 발생 메지시 전달
	void SendMsgSelectItem(HTREEITEM hItem);

	HTREEITEM InsertMapObjectTitle();	// 지도 루트명 입력
	HTREEITEM InsertMapObjectCategory(CString strCategoryName);	// Tree에 Category를 추가한다

	HTREEITEM FindMapObjectCategoryItem(CString strCategoryName);					// Category 아이템을 찾는다.
	HTREEITEM FindMapObjectLayerItem(CString strLayerName);							// Layer 아이템을 찾는다.
	HTREEITEM FindMapObjectFeatureItem(CString strLayerName, CString strFeatureName);// Feature 아이템을 찾는다.


// 재정의입니다.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// 구현입니다.
public:
	virtual ~CMapObjectViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnMapViewAutoOn();
	afx_msg void OnUpdateMapViewAutoOn(CCmdUI *pCmdUI);
	afx_msg void OnMapViewAutoOff();
	afx_msg void OnUpdateMapViewAutoOff(CCmdUI *pCmdUI);
	//afx_msg void OnMapViewSetColor();
	//afx_msg void OnUpdateMapViewSetColor(CCmdUI *pCmdUI);
};
