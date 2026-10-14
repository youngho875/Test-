#pragma once

#include <vector>
#include "../inc/define_struct.h"
#include "MessageDefine.h"

// CMapObjectOrderList

class CMapObjectOrderList : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMapObjectOrderList)

public:
	CMapObjectOrderList();
	virtual ~CMapObjectOrderList();

public:
	void ClearMapObjectAll();
	void AddMapObjectLayer(CString strLayerName, MAP_PRIMITIVETYPE nStypeType, int nLayerIndex);	// 지도 레이어 데이터 추가
	BOOL GetMapObjectLayerChangeInfo(std::vector<MAP_OBJECT_ORDER_LAYERCHANGE> &vMapObjectLayerChange);	// 순서가 변경된 레이어 목록을 전달한다.

	CMapStringToString m_MapObjectLayer;     /**< 레이어명으로 예전 순서, 스타일 타임 저장 */
	std::vector<MAP_OBJECT_ORDER_LAYERCHANGE> m_vMapObjectLayerChange; /**< 순서 변경 할 목록 */
	CList<COMMON_INDEX_STRING, COMMON_INDEX_STRING> m_listMapObjectLayerMoved;/**< 순서 변경 한 레이어 정보 목록 */

	int FindMapObjectLayerIndex(CString strLayerName);	// 레이어의 인덱스 획득
	void SendMsgMapObjectLayerMove();	// 레이어 순서변경 메시지 전달

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnMapOrderTop();						// 순서 맨 위로
	afx_msg void OnUpdateMapOrderTop(CCmdUI *pCmdUI);
	afx_msg void OnMapOrderUp();						// 순서 위로
	afx_msg void OnUpdateMapOrderUp(CCmdUI *pCmdUI);
	afx_msg void OnMapOrderDown();						// 순서 아래로
	afx_msg void OnUpdateMapOrderDown(CCmdUI *pCmdUI);		
	afx_msg void OnMapOrderBottom();					// 순서 맨 아래로
	afx_msg void OnUpdateMapOrderBottom(CCmdUI *pCmdUI);	
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


