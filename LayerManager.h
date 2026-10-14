#pragma once

#include <map>
#include "../LIBSrc/Overlay/Object.h"
#include "ItemList.h"
#include "LayerStructure.h"


//#undef	AFX_DATA
//#ifdef	OVERLAY_IMPL
//#define	AFX_DATA	_declspec(dllexport)
//#else
//#define	AFX_DATA	_declspec(dllimport)
//#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CLayerManager
// 기능				: 투명도 레이어 관리
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------


struct OVERLAY_FILE_HEADER
{
	INT nNumberOfObject;
};


class  CLayerManager
{
public:
	CLayerManager(UINT nMaxLayer, UINT nObjectPerLayer);
	virtual ~CLayerManager();

private:
	UINT		m_nMaxLayer;
	UINT		m_nMaxObjectPerLayer;

	typedef std::map<UINT, CLayerStructure*>			LAYER_MAP;
	typedef std::map<UINT, UINT>						LAYER_ID_POOL;
	typedef std::map<UINT, UINT>						ALLOC_LAYER_ID;

	LAYER_ID_POOL			m_LayerIDPOOL;			// 각각의 레이어에 할당될 ID					
	ALLOC_LAYER_ID			m_AllocLayerID;			// 할당된 레이어 ID

	LAYER_MAP				m_LayerMap;				// 모든 레이어를 저장 관리 
	CItemList<UINT>			m_LayerIndexList;		// 각각 할당된 레이어 ID의 순서 설정

	// 레이어 ID 할당
	INT AllocLayerID();								// 새로 생성된 레이어의 ID 할당
	void ReturnLayerID(INT nID);					// 할당된 레이어 ID 반납.

public:
	// 레이어 추가/삭제
	virtual BOOL AddLayer(CLayerStructure* pLayer);
	virtual BOOL AddLayer(const char* strLayerName, int nStrLen);

	BOOL DeleteLayer(INT nIndex);
	virtual void ClearLayer();

	// 레이어 불러오기
	CLayerStructure* GetLayer(INT nIndex);

	// 도형 추가/삭제
	virtual BOOL AddObject(OVERLAY::OBJECT* pObject);
	virtual BOOL DeleteObject(INT nID);

	// 도형 선택
	OVERLAY::OBJECT* GetOverlayObject(UINT nID);

	// 레이어 위치변경
	virtual BOOL MoveLayerTop(INT nIndex);
	virtual BOOL MoveLayerBottom(INT nIndex);
	virtual BOOL MoveLayerUp(INT nIndex);
	virtual BOOL MoveLayerDown(INT nIndex);

	// 레이어에 포함된 도형의 위치변경
	virtual BOOL MoveObjectTop(INT nIndex);
	virtual BOOL MoveObjectBottom(INT nIndex);
	virtual BOOL MoveObjectUp(INT nIndex);
	virtual BOOL MoveObjectDown(INT nIndex);

	// 레이어 개수
	INT NumberOfLayer();

	// 레이어의 도형개수
	INT NumberOfObject(UINT nLyaerIndex);
};
