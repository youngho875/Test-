#include "pch.h"
#include "options.h"
#include "MainFrm.h"
#include "WinMsgSpec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

COptions::COptions(CWnd* pOwnerWnd)
{
	m_Img.Create(16, 16, TRUE, 5, 1);
	m_pOwnerWnd = pOwnerWnd;
	m_mapOpts.RemoveAll();
}

COptions::~COptions()
{
	POSITION pos = m_lstItems.GetHeadPosition();

	while(pos) {		
		TOPTION_ITEM* p = m_lstItems.GetNext(pos);
		SAFE_DELETE(p);
	}
	m_lstItems.RemoveAll();
}

void* COptions::SetOptionItem(CString strText, UINT nImgOnResID, UINT nImgOffResID, UINT nImgPartResID)
{
	return SetOptionItem("", strText, TOPTION_ITEM::OF_TRUE, nImgOnResID, nImgOffResID, nImgPartResID);
}

void* COptions::SetOptionItem(CString strName, CString strText, int nTrueVal, UINT nImgOnResID, UINT nImgOffResID, UINT nImgPartResID, UINT nCri, TOPTION_ITEM::EENABLE_FLAGS e)
{
	TOPTION_ITEM * oi = new TOPTION_ITEM;
	ASSERT(strText.GetLength() < 99);
	ASSERT(strName.GetLength() < 49);

	strcpy(oi->szName, (LPSTR)(LPCSTR)strName);
	strcpy(oi->szText, (LPSTR)(LPCSTR)strText);
	oi->nTrueValue	= nTrueVal;
	oi->nTImgIndex	= AddImage(nImgOnResID);
	oi->nFImgIndex	= AddImage(nImgOffResID);
	if(nImgPartResID != 0) 
		oi->nPImgIndex	= AddImage(nImgPartResID);
	oi->nCritical		= nCri;
	oi->eEnabled		= e;

	m_lstItems.AddTail(oi);

	return oi; 
}

int COptions::AddImage(UINT nResID)
{
	if (!nResID) return -1;
	int nIndex;
	if (!m_mapImgs.Lookup(nResID, nIndex))
	{
		CBitmap bmp;
		if (!bmp.LoadBitmap(nResID))
			return -1;
		nIndex = m_Img.Add(&bmp, (COLORREF)0xFFFFFF);
		if (nIndex == -1) return -1;
		m_mapImgs.SetAt(nResID, nIndex);
	}
	return nIndex;
}

BOOL COptions::GetOption(CString key, int val)
{
	return (m_mapOpts.Lookup(key, val));
}

int COptions::GetOption(CString key)
{
	int i;
	//VERIFY(m_mapOpts.Lookup(key, i));
	m_mapOpts.Lookup(key, i);
	return i;
}

BOOL COptions::SetOption(CString key, int val)
{
	m_mapOpts.SetAt(key, val);
	return true;
}

int COptions::GetImageIndex(UINT img)
{
	int i;
	if (m_mapImgs.Lookup(img, i))
		return i;
	return -1;
}

void COptions::RegSave(CString section)
{
	CString strKey;
	int			nVal;
	POSITION pos = m_mapOpts.GetStartPosition();
	while(pos) {
		m_mapOpts.GetNextAssoc(pos, strKey, nVal);
		AfxGetApp()->WriteProfileInt(section, strKey, nVal);
	}
}

void COptions::RegLoad(CString section)
{
	CString strKey;
	int			nVal;
	POSITION pos = m_mapOpts.GetStartPosition();
	while(pos) {
		m_mapOpts.GetNextAssoc(pos, strKey, nVal);
		AfxGetApp()->GetProfileInt(section, strKey, nVal);
		m_mapOpts.SetAt(strKey, nVal);
	}
}

void COptions::BeginTrans()
{
	m_mapBackups.RemoveAll();

	CString strKey;
	int			nVal;
	POSITION pos = m_mapOpts.GetStartPosition();
	while(pos) {
		m_mapOpts.GetNextAssoc(pos, strKey, nVal);		
		m_mapBackups.SetAt(strKey, nVal);
	}
}

void COptions::CommitTrans()
{
}

void COptions::RollbackTrans()
{
	CString strKey;
	int			nVal;
	POSITION pos = m_mapOpts.GetStartPosition();
	while(pos) {
		m_mapOpts.GetNextAssoc(pos, strKey, nVal);	
		m_mapBackups.Lookup(strKey, nVal);
		m_mapOpts.SetAt(strKey, nVal);
	}
}

int COptions::operator[](CString str)
{
	return GetOption(str);
}

int COptions::operator[](char* str)
{
	return GetOption((CString)str);
}

void COptions::SetStaticEnable(void* p)
{
	((TOPTION_ITEM*)p)->eEnabled = TOPTION_ITEM::EF_STATIC_ENABLE;
}

void COptions::SetStaticDisable(void * p)
{
	((TOPTION_ITEM*)p)->eEnabled = TOPTION_ITEM::EF_STATIC_DISABLE;
}

void COptions::ClearStatic(void *p)
{
	((TOPTION_ITEM*)p)->eEnabled = TOPTION_ITEM::EF_ENABLE;
}

void COptions::InformToChangeOpt(char *sz, TOPTION_ITEM::EOPTION_FLAGS e)
{
	//	nFlag=3: Enable, nFlag=2: Disable
	if(m_pOwnerWnd)	
		m_pOwnerWnd->SendMessage(WM_OPTION_CHANGE, (WPARAM)sz, (LPARAM)e);
	else
		((CMainFrame*)AfxGetMainWnd())->GetActiveView()->SendMessage(WM_OPTION_CHANGE, (WPARAM)sz, (LPARAM)e); 
}