#include "pch.h"
#include "../LIBSrc/Overlay/object.h"
#include "../LIBSrc/Overlay/ObjectArc.h"
#include "../LIBSrc/Overlay/ObjectPie.h"
#include "../LIBSrc/Overlay/ObjectLinkedLine.h"
#include "../LIBSrc/Overlay/ObjectLinkedMultiLine.h"
#include "../LIBSrc/Overlay/ObjectLinkedCurve.h"
#include "../LIBSrc/Overlay/ObjectLinkedMultiCurve.h"
#include "LayerStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CLayerStructure::CLayerStructure(UINT nMaxObject)
	//: m_strLayerName(_T("LAYER_NAME"))
	: m_bIsDisplay(TRUE)
{
	m_nMaxObject = nMaxObject;

	UINT i = 0;
	for (i = 1; i <= m_nMaxObject; i++)
	{
		m_ObjectIDPOOL[i] = i;
		m_GroupIDPOOL[i] = i;
	}

	memset(m_strLayerName, 0, sizeof(m_strLayerName));
}

CLayerStructure::~CLayerStructure()
{
	ClearObject();
}


INT CLayerStructure::AllocObjectID()
{
	std::map<UINT, UINT>::iterator it = m_ObjectIDPOOL.begin();

	UINT nObjectID = 0;
	if (it != m_ObjectIDPOOL.end())
	{
		nObjectID = it->second;
		m_ObjectIDPOOL.erase(nObjectID);
		m_AllocObjectID[nObjectID] = nObjectID;
		return nObjectID;
	}

	return -1;
}

INT CLayerStructure::AllocObjectID(UINT nID)
{
	std::map<UINT, UINT>::iterator it = m_ObjectIDPOOL.find(nID);

	if (it != m_ObjectIDPOOL.end())
	{
		m_ObjectIDPOOL.erase(nID);
		m_AllocObjectID[nID] = nID;
		return nID;
	}

	return -1;
}

void CLayerStructure::ReturnObjectID(INT nID)
{
	if (m_AllocObjectID[nID] != 0)
	{
		m_ObjectIDPOOL[nID] = nID;
		m_AllocObjectID.erase(nID);
	}
}

INT CLayerStructure::AllocGroupID()
{
	std::map<UINT, UINT>::iterator it = m_GroupIDPOOL.begin();

	UINT nGroupID = 0;
	if (it != m_GroupIDPOOL.end())
	{
		nGroupID = it->second;
		m_GroupIDPOOL.erase(nGroupID);
		m_AllocGroupID[nGroupID] = nGroupID;
		return nGroupID;
	}

	return -1;
}

void CLayerStructure::ReturnGroupID(INT nID)
{
	if (m_AllocGroupID[nID] != 0)
	{
		m_GroupIDPOOL[nID] = nID;
		m_AllocGroupID.erase(nID);
	}
}

BOOL CLayerStructure::AddObject(OVERLAY::OBJECT* pObject)
{
	if (m_ObjectMap.size() >= m_nMaxObject)
		return FALSE;

	if (pObject->nID <= 0)
		pObject->nID = AllocObjectID();
	else
	{
		if (AllocObjectID(pObject->nID) <= 0)
			return FALSE;
	}

	if (pObject->nID <= 0)
		return FALSE;

	if (pObject->GetType() == OVERLAY::ROUND_RECT && (pObject->pt[0].X() > pObject->pt[1].X())) {

		return FALSE;
	}

	m_ObjectMap[pObject->nID] = pObject;
	m_ObjectList.push_back(pObject->nID);

	return TRUE;
}

BOOL CLayerStructure::DeleteObject(UINT nID, BOOL bEraseMap/*=TRUE*/)
{
	OVERLAY::OBJECT* pObject = NULL;
	if (nID <= 0)
		return FALSE;
	pObject = m_ObjectMap[nID];
	if (pObject != NULL)
	{
		// 도형 삭제시 도형간 연결선이 존재할 경우 도형간 연결선을 지운다.
		if (pObject->ObjectLinkedLineProperty.ExistLinkedLine() == TRUE)
		{
			UINT nTempIndex = 0;
			UINT nLinkedLine_ID = 0;
			while (pObject->ObjectLinkedLineProperty.CurrentLinkedLineCount() > 0)
			{
				if (nTempIndex > OBJECT_PER_LAYER)
					break;
				nTempIndex++;

				nLinkedLine_ID = pObject->ObjectLinkedLineProperty.GetBack();
				DeleteLinkedLine(nLinkedLine_ID);
			}
			if (bEraseMap == TRUE)
			{
				m_ObjectMap.erase(nID);
				m_ObjectList.delete_object(nID);
				ReturnObjectID(nID);
			}
			SAFE_DELETE(pObject);

			return TRUE;
		}
		else
		{
			if (bEraseMap == TRUE)
			{
				m_ObjectList.delete_object(nID);
				ReturnObjectID(nID);
				m_ObjectMap.erase(nID);
			}
			SAFE_DELETE(pObject);
		}

		return TRUE;
	}
	else
		m_ObjectMap.erase(nID);

	return FALSE;
}

BOOL CLayerStructure::ClearObject()
{
	std::map<UINT, OVERLAY::OBJECT*>::iterator it = m_ObjectMap.begin();
	OVERLAY::OBJECT* pObject = NULL;

	for (; it != m_ObjectMap.end(); it++)
	{
		pObject = it->second;
		if (pObject != NULL)
		{
			DeleteObject(pObject->nID, FALSE);
		}
	}

	m_ObjectList.clear();
	m_ObjectMap.clear();

	m_ObjectIDPOOL.clear();
	m_AllocObjectID.clear();

	// Clear Group Info.
	std::map<UINT, OVERLAY::CObjectGroupInfo*>::iterator it_ = m_GroupMap.begin();
	OVERLAY::CObjectGroupInfo* pObjectGroupInfo = NULL;

	for (; it_ != m_GroupMap.end(); it_++)
	{
		pObjectGroupInfo = it_->second;
		if (pObjectGroupInfo != NULL)
		{
			delete pObjectGroupInfo;
			pObjectGroupInfo = NULL;
		}
	}

	m_GroupMap.clear();

	return TRUE;
}

void CLayerStructure::SetGroup(UINT nGroupID, OVERLAY::CObjectGroupInfo* pObjectGroupInfo)
{
	if (pObjectGroupInfo == NULL)
		return;

	m_GroupMap[nGroupID] = pObjectGroupInfo;
}

void CLayerStructure::AddGroup(UINT& nGroupID, OVERLAY::CObjectGroupInfo* pObjectGroupInfo)
{
	if (pObjectGroupInfo == NULL)
		return;

	nGroupID = AllocGroupID();
	m_GroupMap[nGroupID] = pObjectGroupInfo;
}

void CLayerStructure::ReleaseGroup(UINT nGroupID)
{
	std::map<UINT, OVERLAY::CObjectGroupInfo*>::iterator it = m_GroupMap.find(nGroupID);

	if (it == m_GroupMap.end())
		return;

	OVERLAY::CObjectGroupInfo* pObjectGroupInfo = it->second;

	if (pObjectGroupInfo == NULL)
		return;

	ReturnGroupID(nGroupID);

	delete pObjectGroupInfo;
	pObjectGroupInfo = NULL;

	m_GroupMap.erase(it);
}

OVERLAY::CObjectGroupInfo* CLayerStructure::GetGroupInfo(UINT nGroupID)
{
	std::map<UINT, OVERLAY::CObjectGroupInfo*>::iterator it = m_GroupMap.find(nGroupID);

	if (it == m_GroupMap.end())
		return NULL;

	return it->second;
}

BOOL CLayerStructure::MoveToUp(UINT nIndex)
{
	if (m_ObjectList.size() > 1 && nIndex > 0)
	{
		m_ObjectList.move(nIndex, nIndex - 1);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerStructure::MoveToDown(UINT nIndex)
{
	if (m_ObjectList.size() > 1 && nIndex < (m_ObjectList.size() - 1))
	{
		m_ObjectList.move(nIndex, nIndex + 1);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerStructure::MoveToTop(UINT nIndex)
{
	if (m_ObjectList.size() > 1 && nIndex > 0)
	{
		m_ObjectList.move(nIndex, 0);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerStructure::MoveToBottom(UINT nIndex)
{
	if (m_ObjectList.size() > 1 && nIndex < (m_ObjectList.size() - 1))
	{
		m_ObjectList.move(nIndex, (m_ObjectList.size() - 1));
		return TRUE;
	}
	else
		return FALSE;
}

OVERLAY::OBJECT* CLayerStructure::get_object(UINT nID)
{
	if (nID <= 0)
		return NULL;

	OVERLAY::OBJECT* pObject = m_ObjectMap[nID];

	if (pObject != NULL)
		return pObject;
	else
		m_ObjectMap.erase(nID);

	return NULL;
}

OVERLAY::OBJECT* CLayerStructure::get_object_with_index(UINT nIndex)
{
	UINT nID = 0;
	nID = m_ObjectList.get_item(nIndex);

	if (nID <= 0)
		return NULL;

	OVERLAY::OBJECT* pObject = m_ObjectMap[nID];
	if (pObject != NULL)
		return pObject;
	else
		m_ObjectMap.erase(nID);

	return NULL;
}

OVERLAY::OBJECT* CLayerStructure::get_first_object()
{
	LIST_Node<UINT>* pListOfObject = m_ObjectList.get_list();

	if (pListOfObject == NULL)
		return NULL;

	if (pListOfObject->data <= 0)
		return NULL;

	OVERLAY::OBJECT* pObject = m_ObjectMap[pListOfObject->data];
	if (pObject != NULL)
		return pObject;
	else
		m_ObjectMap.erase(pListOfObject->data);

	return NULL;
}

OVERLAY::OBJECT* CLayerStructure::get_next_object()
{
	LIST_Node<UINT>* pListOfObject = m_ObjectList.get_next();

	if (pListOfObject == NULL)
		return NULL;

	if (pListOfObject->data <= 0)
		return NULL;

	OVERLAY::OBJECT* pObject = m_ObjectMap[pListOfObject->data];
	if (pObject != NULL)
		return pObject;
	else
		m_ObjectMap.erase(pListOfObject->data);

	return NULL;
}

INT CLayerStructure::NumberOfObjectInLayer()
{
	return (INT)m_ObjectMap.size();
}
/*
void CLayerStructure::AddLinkedLine(OVERLAY::OBJECT* pObject)
{
	OVERLAY::OBJECT* pTempObject = NULL;
	//UINT nID = 0;
	pObject->ObjectLinkedLineProperty.AddObjectID(pObject->nID);
/*
	if(pObject->ObjectLinkedLineProperty.ExistLinkedLine()==TRUE)
	{
		for(UINT i=0; i<pObject->ObjectLinkedLineProperty.CurrentLinkedLineCount(); i++)
		{
			nID = pObject->ObjectLinkedLineProperty.GetID(i);
			pTempObject = m_ObjectMap[nID];
			if(pTempObject==NULL)
				m_ObjectMap.erase(nID);
			else
				pObject->ObjectLinkedLineProperty.AddObjectID(pObject->nID);
		}
	}*//*
}
*/
void CLayerStructure::DeleteLinkedLine(UINT nID)
{
	OVERLAY::OBJECT* pObject = NULL;
	if (nID <= 0)
		return;
	pObject = m_ObjectMap[nID];

	if (pObject != NULL)
	{
		if (pObject->GetType() == OVERLAY::LINKED_LINE ||
			pObject->GetType() == OVERLAY::LINKED_MULTI_LINE ||
			pObject->GetType() == OVERLAY::LINKED_CURVE ||
			pObject->GetType() == OVERLAY::LINKED_MULTI_CURVE)
		{
			OVERLAY::ObjectLinkedLine* temp_linked_line = (OVERLAY::ObjectLinkedLine*)pObject;
			UINT nLinkedLine_ID = 0;

			OVERLAY::OBJECT* pLinkedLine_Start_Object = get_object(temp_linked_line->nLinkedLine_Start_ID);
			OVERLAY::OBJECT* pLinkedLine_End_Object = get_object(temp_linked_line->nLinkedLine_End_ID);

			while (temp_linked_line->ObjectLinkedLineProperty.CurrentLinkedLineCount() > 0)
			{
				nLinkedLine_ID = pObject->ObjectLinkedLineProperty.GetBack();
				DeleteLinkedLine(nLinkedLine_ID);
			}

			if (temp_linked_line != NULL)
			{
				if (pLinkedLine_Start_Object != NULL)
				{
					pLinkedLine_Start_Object->ObjectLinkedLineProperty.RemoveObjectID(nID);
				}

				if (pLinkedLine_End_Object != NULL)
				{
					pLinkedLine_End_Object->ObjectLinkedLineProperty.RemoveObjectID(nID);
				}

				SAFE_DELETE(temp_linked_line);
			}

			m_ObjectMap.erase(nID);
			m_ObjectList.delete_object(nID);
			ReturnObjectID(nID);
		}
	}
	else
		m_ObjectMap.erase(nID);
}

char* CLayerStructure::GetLayerName()
{
	//*nStrLen = strlen(m_strLayerName);
	return m_strLayerName;
}

void CLayerStructure::SetLayerName(const char* strLayerName, int nStrLen)
{
	memset(m_strLayerName, 0, sizeof(m_strLayerName));
	memcpy(m_strLayerName, strLayerName, nStrLen);
	//m_strLayerName = strLayerName;
}

BOOL CLayerStructure::IsDisplay()
{
	return m_bIsDisplay;
}

void CLayerStructure::SetDisplay(BOOL bIsDisplay)
{
	m_bIsDisplay = bIsDisplay;
}
