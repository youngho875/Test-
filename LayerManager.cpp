#include "pch.h"
#include "LayerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CLayerManager::CLayerManager(UINT nMaxLayer, UINT nObjectPerLayer)
{
	UINT i = 0;
	for (i = 1; i <= nMaxLayer; i++)
	{
		m_LayerIDPOOL[i] = i;
	}

	m_nMaxLayer = nMaxLayer;
	m_nMaxObjectPerLayer = nObjectPerLayer;
}

CLayerManager::~CLayerManager()
{
	ClearLayer();

	m_LayerIDPOOL.clear();
	m_AllocLayerID.clear();;
}

INT CLayerManager::AllocLayerID()
{
	std::map<UINT, UINT>::iterator it = m_LayerIDPOOL.begin();
	if (m_LayerIDPOOL.size() < 1)
		return -1;

	UINT nLayerID = 0;
	if (it != m_LayerIDPOOL.end())
	{
		nLayerID = it->second;
		m_LayerIDPOOL.erase(nLayerID);
		m_AllocLayerID[nLayerID] = nLayerID;
		return nLayerID;
	}

	return -1;
}

void CLayerManager::ReturnLayerID(INT nID)
{
	if (m_AllocLayerID[nID] != 0)
	{
		m_LayerIDPOOL[nID] = nID;
		m_AllocLayerID.erase(nID);
	}
}

BOOL CLayerManager::AddLayer(CLayerStructure* pLayer)
{
	if (pLayer == NULL)
		return FALSE;

	if (m_LayerMap.size() >= m_nMaxLayer)
		return FALSE;

	INT nLayerID = AllocLayerID();

	if (nLayerID <= 0)
		return FALSE;

	m_LayerMap[nLayerID] = pLayer;

	m_LayerIndexList.push_front(nLayerID);

	return TRUE;
};

BOOL CLayerManager::AddLayer(const char* strLayerName, int nStrLen)
{
	if (m_LayerMap.size() >= m_nMaxLayer)
		return FALSE;

	INT nLayerID = AllocLayerID();

	if (nLayerID <= 0)
		return FALSE;

	CLayerStructure* pAddLayer = new CLayerStructure(m_nMaxObjectPerLayer);
	pAddLayer->SetLayerName(strLayerName, nStrLen);
	m_LayerMap[nLayerID] = pAddLayer;

	m_LayerIndexList.push_front(nLayerID);
	return TRUE;
};

BOOL CLayerManager::DeleteLayer(INT nIndex)
{
	UINT nLayerID = m_LayerIndexList.get_item(nIndex);
	if (nLayerID <= 0)
		return FALSE;

	CLayerStructure* pAddLayer = m_LayerMap[nLayerID];

	if (pAddLayer != NULL)
	{
		m_LayerIndexList.erase(nIndex);
		m_LayerMap.erase(nLayerID);
		delete pAddLayer;
	}
	else
		return FALSE;

	ReturnLayerID(nLayerID);
	return TRUE;
}

void CLayerManager::ClearLayer()
{
	std::map<UINT, CLayerStructure*>::iterator it = m_LayerMap.begin();

	CLayerStructure* pLayer = NULL;
	for (; it != m_LayerMap.end(); it++)
	{
		pLayer = it->second;
		if (pLayer != NULL)
			delete pLayer;
	}

	m_LayerMap.clear();
	m_LayerIndexList.clear();

	std::map<UINT, UINT>::iterator it_pool = m_AllocLayerID.begin();
	for (; it_pool != m_AllocLayerID.end(); it_pool++)
	{
		m_LayerIDPOOL[it_pool->second] = it_pool->second;
	}
	m_AllocLayerID.clear();
}

CLayerStructure* CLayerManager::GetLayer(INT nIndex)
{
	UINT nLayerID = m_LayerIndexList.get_item(nIndex);

	CLayerStructure* pLayer = m_LayerMap[nLayerID];

	if (pLayer == NULL)
		m_LayerMap.erase(nLayerID);
	return pLayer;
}

BOOL CLayerManager::AddObject(OVERLAY::OBJECT* pObject)
{
	if (m_LayerMap.size() < 1 || pObject == NULL)
		return FALSE;

	// 첫번째(최상위) 레이어만 도형을 추가할수 있다.
	CLayerStructure* pLayer = GetLayer(0);

	if (pLayer == NULL)
		return FALSE;

	if (pLayer->AddObject(pObject) == FALSE)
	{
		AfxMessageBox(_T("레이어에 도형을 추가할 수 없습니다."));
		//	DeleteObject(pObject->nID);
		return FALSE;
	}

	return TRUE;
}

BOOL CLayerManager::DeleteObject(INT nID)
{
	if (m_LayerMap.size() < 1)
		return FALSE;

	CLayerStructure* pLayer = GetLayer(0);

	if (pLayer == NULL)
		return FALSE;

	return pLayer->DeleteObject(nID);
}

OVERLAY::OBJECT* CLayerManager::GetOverlayObject(UINT nID)
{
	OVERLAY::OBJECT* pObject = NULL;

	if (m_LayerMap.size() < 1)
		return NULL;

	CLayerStructure* pLayer = GetLayer(0);

	if (pLayer == NULL)
		return NULL;

	pObject = pLayer->get_object(nID);

	return pObject;
}

// 레이어 위치변경
BOOL CLayerManager::MoveLayerTop(INT nIndex)
{
	if (m_LayerIndexList.size() > 1 && nIndex > 0)
	{
		m_LayerIndexList.move(nIndex, 0);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerManager::MoveLayerBottom(INT nIndex)
{
	if (m_LayerIndexList.size() > 1)
	{
		m_LayerIndexList.move(nIndex, m_LayerIndexList.size() - 1);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerManager::MoveLayerUp(INT nIndex)
{
	if (m_LayerIndexList.size() > 1 && nIndex > 0)
	{
		m_LayerIndexList.move(nIndex, nIndex - 1);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLayerManager::MoveLayerDown(INT nIndex)
{
	if (m_LayerIndexList.size() > 1 && nIndex < ((INT)m_LayerIndexList.size() - 1))
	{
		m_LayerIndexList.move(nIndex, nIndex + 1);
		return TRUE;
	}
	else
		return FALSE;
}

// 도형 위치변경
BOOL CLayerManager::MoveObjectTop(INT nIndex)
{
	if (m_LayerIndexList.size() < 1)
		return FALSE;

	CLayerStructure* pLayer = NULL;
	pLayer = GetLayer(0);

	if (pLayer != NULL)
		pLayer->MoveToTop(nIndex);


	return TRUE;
}

BOOL CLayerManager::MoveObjectBottom(INT nIndex)
{
	if (m_LayerIndexList.size() < 1)
		return FALSE;

	CLayerStructure* pLayer = NULL;
	pLayer = GetLayer(0);

	if (pLayer != NULL)
		return pLayer->MoveToBottom(nIndex);

	return FALSE;
}

BOOL CLayerManager::MoveObjectUp(INT nIndex)
{
	if (m_LayerIndexList.size() < 1)
		return FALSE;

	CLayerStructure* pLayer = NULL;
	pLayer = GetLayer(0);

	if (pLayer != NULL)
		return pLayer->MoveToUp(nIndex);;

	return FALSE;
}

BOOL CLayerManager::MoveObjectDown(INT nIndex)
{
	if (m_LayerIndexList.size() < 1)
		return FALSE;

	CLayerStructure* pLayer = NULL;
	pLayer = GetLayer(0);

	if (pLayer != NULL)
		return pLayer->MoveToDown(nIndex);

	return FALSE;
}

INT CLayerManager::NumberOfLayer()
{
	return m_LayerMap.size();
}

INT CLayerManager::NumberOfObject(UINT nLyaerIndex)
{
	if (nLyaerIndex >= m_LayerIndexList.size())
		return 0;
	CLayerStructure* pLayer = GetLayer((INT)nLyaerIndex);
	return pLayer ? pLayer->NumberOfObjectInLayer() : 0;
}
