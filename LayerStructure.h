#pragma once

#include "../LIBSrc/Overlay/Object.h"
#include "../LIBSrc/Overlay/ObjectGroupInfo.h"
#include "memory.h"
#include "ItemList.h"
#include <list>


//#undef	AFX_DATA
//#ifdef	OVERLAY_IMPL
//#define	AFX_DATA	_declspec(dllexport)
//#else
//#define	AFX_DATA	_declspec(dllimport)
//#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CLayerStructure
// 기능				: Layer 자료구조
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------
class CLayerStructure
{
public:
	CLayerStructure(UINT nMaxObject);
	~CLayerStructure();

	BOOL AddObject(OVERLAY::OBJECT* object);
	BOOL DeleteObject(UINT nID, BOOL bEraseMap = TRUE);
	BOOL ClearObject();

	void SetGroup(UINT nGroupID, OVERLAY::CObjectGroupInfo* pObjectGroupInfo);
	void AddGroup(UINT& nGroupID, OVERLAY::CObjectGroupInfo* pObjectGroupInfo);
	void ReleaseGroup(UINT nGroupID);
	OVERLAY::CObjectGroupInfo* GetGroupInfo(UINT nGroupID);

private:
	UINT	m_nMaxObject;
	std::map<UINT, OVERLAY::OBJECT*>	m_ObjectMap;		// 생성된 모든 도형정보를 저장 관리
	CItemList<UINT> m_ObjectList;								// 도형을 그린 순서대로 도시


	typedef std::map<UINT, UINT>						OBJECT_ID_POOL;
	typedef std::map<UINT, UINT>						ALLOC_OBJECT_ID;

	typedef std::map<UINT, UINT>						GROUP_ID_POOL;
	typedef std::map<UINT, UINT>						ALLOC_GROUP_ID;

	OBJECT_ID_POOL			m_ObjectIDPOOL;				// 각각의 도형에 할당될 ID
	ALLOC_OBJECT_ID			m_AllocObjectID;			// 할당된 도형 ID

	GROUP_ID_POOL			m_GroupIDPOOL;				// 각각의 그룹에 할당될 ID
	ALLOC_GROUP_ID			m_AllocGroupID;				// 할당된 그룹 ID

	typedef std::map<UINT, OVERLAY::CObjectGroupInfo*> GROUP_MAP;
	GROUP_MAP	m_GroupMap;								// 그룹 목록.


public:

	// 도형의 위치 변경
	BOOL MoveToUp(UINT nIndex);
	BOOL MoveToDown(UINT nIndex);
	BOOL MoveToTop(UINT nIndex);
	BOOL MoveToBottom(UINT nIndex);

	// 도형불러오기
	OVERLAY::OBJECT* get_object(UINT nID);
	OVERLAY::OBJECT* get_object_with_index(UINT nIndex);
	OVERLAY::OBJECT* get_first_object();
	OVERLAY::OBJECT* get_next_object();

	// 레이어 명칭
	char* GetLayerName();
	void SetLayerName(const char* strLayerName, int nStrLen);

	// 레이어 전시 유무
	BOOL IsDisplay();
	void SetDisplay(BOOL bIsDisplay);

	// 레이어의 도형 개수
	INT NumberOfObjectInLayer();

private:
	// 도형 ID 할당
	INT AllocObjectID();							// 새로 생성된 도형의 ID 할당
	INT AllocObjectID(UINT nID);					// 새로 생성된 도형의 ID 할당
	void ReturnObjectID(INT nID);					// 할당된 도형 ID 반납.

	INT AllocGroupID();								// 새로 생성된 그룹의 ID 할당	
	void ReturnGroupID(INT nID);					// 할당된 그룹 ID 반납.

	// 연결선 삭제.
	void DeleteLinkedLine(UINT nID);

	char		m_strLayerName[512];		// 레이어 명칭
	BOOL		m_bIsDisplay;		// 레이어 전시 유무
};
