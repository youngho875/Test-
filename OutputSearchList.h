#pragma once

#include "../inc/define_struct.h"

typedef enum SEARCHLIST_KIND
{
	SEARCHLIST_NONE = 0, // 결과 없음
	SEARCHLIST_SCREEN,	 // 공간 검색 결과
	SEARCHLIST_NAME,	 // 검색어 검색 결과
	SEARCHLIST_COP_IMPORTANT,	// 상황도 중요도시요소
	SEARCHLIST_COP_ALWAYS,	// 상황도 상시시현요소
	SEARCHLIST_COP,			// 상황도 검색 결과
};

// COutputSearchList

class COutputSearchList : public CMFCListCtrl
{
	DECLARE_DYNAMIC(COutputSearchList)

public:
	COutputSearchList();
	virtual ~COutputSearchList();

public:
	SEARCHLIST_KIND m_emListType;

	void ClearSearch();
	void SetSearchScreen(CString strCategory, CString strLayer, std::vector<MAPSEARCH_LAYER_RESULT>& vResult);
	void SetSearchName(CString strLayer,std::vector<MAPSEARCH_LAYER_RESULT>& vResult);
	void SetSearchCOP(CString strLayer, std::vector<MAPSEARCH_LAYER_RESULT>& vResult, SEARCHLIST_KIND emListType);

private:
	int m_nColumnCount;
	CMapStringToString m_mapListColumn;
	std::vector<MAPSEARCH_LAYER_RESULT> m_vResult;
	CString m_strTitle;

	void SetSearchResult(std::vector<MAPSEARCH_LAYER_RESULT>& vResult);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnResultCopy();
	afx_msg void OnUpdateResultCopy(CCmdUI* pCmdUI);
	afx_msg void OnResultMove();
	afx_msg void OnUpdateResultMove(CCmdUI* pCmdUI);
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
