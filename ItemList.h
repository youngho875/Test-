#pragma once

#include "memory.h"
#include <map>

//#undef	AFX_DATA
//#ifdef	OVERLAY_IMPL
//#define	AFX_DATA	_declspec(dllexport)
//#else
//#define	AFX_DATA	_declspec(dllimport)
//#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CItemList
// 목적				: List 자료구조
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------


template <typename LIST_TYPE>
struct LIST_Node
{
	LIST_Node()
	{
		front = NULL;
		next = NULL;
	}
	LIST_Node* front;
	LIST_Node* next;
	LIST_TYPE		data;
};

template <typename LIST_TYPE>

class CItemList
{
public:
	CItemList()
	{
		InitList();
	}
	~CItemList()
	{
		clear();
	}

private:
	INT						m_nSize;
	LIST_Node<LIST_TYPE>	m_Header;
	LIST_Node<LIST_TYPE>	m_Tail;
	LIST_Node<LIST_TYPE>* m_pCurrent;

public:
	// 0.
	void InitList()
	{
		m_Header.front = &m_Header;
		m_Header.next = &m_Tail;
		m_Tail.next = &m_Tail;
		m_Tail.front = &m_Header;
		m_pCurrent = &m_Header;
		m_nSize = 0;
	}
	// 0. 
	LIST_Node<LIST_TYPE>* get_list()
	{
		m_pCurrent = m_Header.next;
		if (m_Header.next == &m_Tail)
			return NULL;
		return m_Header.next;
	}


	//LIST_TYPE* get_object(UINT nID)
	//{
	//	return m_ObjectMap[nID];
	//}

	// 0. 
	LIST_Node<LIST_TYPE>* get_next()
	{
		m_pCurrent = m_pCurrent->next;
		if (m_pCurrent == &m_Tail)
			return NULL;
		return m_pCurrent;
	}
	// 1. List 추가
	void push_front(LIST_TYPE data)
	{
		LIST_Node<LIST_TYPE>* pNode = new LIST_Node<LIST_TYPE>;
		pNode->data = data;

		pNode->next = m_Header.next;
		pNode->front = &m_Header;

		m_Header.next->front = pNode;
		m_Header.next = pNode;

		m_nSize++;
	}

	void push_back(LIST_TYPE data)
	{
		LIST_Node<LIST_TYPE>* pNode = new LIST_Node<LIST_TYPE>;
		pNode->data = data;

		pNode->next = &m_Tail;
		pNode->front = m_Tail.front;

		m_Tail.front->next = pNode;
		m_Tail.front = pNode;

		m_nSize++;

	}

	BOOL insert(INT nIndex, LIST_TYPE data)
	{
		INT nCount = 0;
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;

		LIST_Node<LIST_TYPE>* pNode = m_Header.next;
		LIST_Node<LIST_TYPE>* pAddNode = new LIST_Node<LIST_TYPE>;
		pAddNode->data = data;

		while (nCount <= nIndex)
		{
			pNode = pNode->next;
			nCount++;
		}

		pNode->front->next = pAddNode;
		pAddNode->front = pNode->front;
		pAddNode->next = pNode;
		pNode->front = pAddNode;

		m_nSize++;
		return TRUE;
	}

	// 2. List 제거
	void clear()
	{
		if (m_nSize <= 0)
			return;
		while (m_Header.next != &m_Tail)
		{
			LIST_Node<LIST_TYPE>* pNode = m_Header.next;
			m_Header.next = pNode->next;
			m_nSize--;
			if (m_nSize < 0)
			{
				m_nSize = 0;
				InitList();
				return;
			}
			delete pNode;
		}
		InitList();
		if (m_nSize != 0)
		{
			//TRACE("Object가 정상적으로 제거되지 않았습니다. 확이해 주세요 (%d 만큼 제거되지 않았네요.)\n",m_nSize);
		}
	}

	BOOL pop_front(LIST_TYPE* Item)
	{
		if (m_nSize <= 0)
			return FALSE;

		LIST_Node<LIST_TYPE>* pNode;
		*Item = m_Header.next->data;

		pNode = m_Header.next;
		m_Header.next = m_Header.next->next;
		m_Header.next->front = &m_Header;

		if (m_pCurrent == pNode) m_pCurrent = pNode->front;
		delete pNode;
		m_nSize--;
		if (m_nSize < 0)
			m_nSize = 0;

		return TRUE;
	}

	BOOL pop_back(LIST_TYPE* Item)
	{
		if (m_nSize <= 0)
			return FALSE;

		LIST_Node<LIST_TYPE>* pNode;
		*Item = m_Tail.front->data;

		pNode = m_Tail.front;
		m_Tail.front->front->next = &m_Tail;
		m_Tail.front = m_Tail.front->front;

		if (m_pCurrent == pNode) m_pCurrent = pNode->front;
		delete pNode;
		m_nSize--;
		if (m_nSize < 0)
			m_nSize = 0;

		return TRUE;
	}

	BOOL delete_object(INT nID)
	{
		INT nCount = 0;
		LIST_Node<LIST_TYPE>* pNode = m_Header.next;
		while (pNode != &m_Tail)
		{
			if (pNode->data == nID)
			{
				erase(nCount);
				return TRUE;
			}
			pNode = pNode->next;
			nCount++;
		}
		return FALSE;
	}

	BOOL erase(INT nIndex)
	{
		INT nCount = 0;
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;

		LIST_Node<LIST_TYPE>* pNode = &m_Header;

		while (nCount <= nIndex)
		{
			pNode = pNode->next;
			nCount++;
		}

		pNode->front->next = pNode->next;
		pNode->next->front = pNode->front;

		if (m_pCurrent == pNode) m_pCurrent = pNode->front;
		delete pNode;
		m_nSize--;
		if (m_nSize < 0)
			m_nSize = 0;
		return TRUE;
	}

	// 3. 항목 가져오기
	LIST_TYPE get_front()
	{
		if (m_nSize <= 0)
			return NULL;

		return m_Header.next->data;
	}

	LIST_TYPE get_back()
	{
		if (m_nSize <= 0)
			return NULL;

		return m_Tail.front->data;
	}

	// 4. List 교환
	BOOL swap_head_tail()
	{
		if (m_nSize < 2)
			return FALSE;

		LIST_TYPE item_front;
		LIST_TYPE item_back;

		if (pop_front(&item_front) == FALSE)
			return FALSE;
		if (pop_back(&item_back) == FALSE)
			return FALSE;

		push_back(item_front);
		push_front(item_back);

		return TRUE;
	}

	LIST_TYPE get_item(INT nIndex)
	{
		INT nCount = 0;
		if (nIndex < 0 || nIndex >= m_nSize)
			return NULL;

		LIST_Node<LIST_TYPE>* pNode = &m_Header;

		while (nCount <= nIndex)
		{
			pNode = pNode->next;
			nCount++;
		}
		return pNode->data;
	}

	BOOL swap(INT nIndex1, INT nIndex2)
	{
		INT nCount1 = 0;
		INT nCount2 = 0;
		if (nIndex1 < 0 || nIndex2 < 0 || nIndex1 >= m_nSize || nIndex2 >= m_nSize || nIndex1 == nIndex2)
			return FALSE;

		LIST_Node<LIST_TYPE>* pNode1 = &m_Header;
		LIST_Node<LIST_TYPE>* pNode2 = &m_Header;

		while (nCount1 <= nIndex1)
		{
			pNode1 = pNode1->next;
			nCount1++;
		}

		while (nCount2 <= nIndex2)
		{
			pNode2 = pNode2->next;
			nCount2++;
		}

		LIST_TYPE tempData = pNode2->data;
		pNode2->data = pNode1->data;
		pNode1->data = tempData;

		return TRUE;
	}

	BOOL move(INT nFrom, INT nTo)
	{
		INT nCount1 = 0;
		INT nCount2 = 0;
		if (nFrom < 0 || nTo < 0 || nFrom >= m_nSize || nTo >= m_nSize || nFrom == nTo)
			return FALSE;

		LIST_Node<LIST_TYPE>* pFrom = &m_Header;
		LIST_Node<LIST_TYPE>* pTo = &m_Header;

		while (nCount1 <= nFrom)
		{
			pFrom = pFrom->next;
			nCount1++;
		}

		while (nCount2 <= nTo)
		{
			pTo = pTo->next;
			nCount2++;
		}

		int i = 0;
		int nCount = 0;

		LIST_TYPE tempData;
		if (nFrom > nTo)
		{//Move up
			nCount = nFrom - nTo;

			tempData = pFrom->data;
			for (i = 0; i < nCount; i++)
			{
				pFrom->data = pFrom->front->data;
				pFrom = pFrom->front;
			}
			pTo->data = tempData;
		}
		else
		{//Move down		
			nCount = nTo - nFrom;

			tempData = pFrom->data;
			for (i = 0; i < nCount; i++)
			{
				pFrom->data = pFrom->next->data;
				pFrom = pFrom->next;
			}
			pTo->data = tempData;
		}
		return TRUE;
	}

	// 5. Util
	UINT size()
	{
		return m_nSize;
	}
};
