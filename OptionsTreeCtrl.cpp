// COptionsTreeCtrl.cpp : implementation file
//

#include "pch.h"
#include "OptionsTreeCtrl.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsTreeCtrl

/****************************************************************************************
* Function	:	COptionsTreeCtrl::COptionsTreeCtrl
* Description		:	
* Return Value	:	
****************************************************************************************/
COptionsTreeCtrl::COptionsTreeCtrl()
{
}

/****************************************************************************************
* Function	:	COptionsTreeCtrl::~COptionsTreeCtrl
* Description		:	
* Return Value	:	
****************************************************************************************/
COptionsTreeCtrl::~COptionsTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(COptionsTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(COptionsTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsTreeCtrl message handlers
/****************************************************************************************
* Function	:	COptionsTreeCtrl::Initialize
* Description		:	
* Return Value	:	void
* Parameter		:	char* str	-	
****************************************************************************************/
void COptionsTreeCtrl::Initialize(char* str)
{
	int nLevel = 0;
	int nIndent;

	HTREEITEM hItem, hExpandItem = NULL;
	TVINSERTSTRUCT tvi;
	TOPTION_ITEM* p;

	hItem = GetRootItem();
	SetImageList(&(m_pOptions->m_Img), TVSIL_NORMAL);

	POSITION pos = m_pOptions->m_lstItems.GetHeadPosition();
	while(pos) {
		p = m_pOptions->m_lstItems.GetNext(pos);
		
		for (nIndent = 0; p->szText[nIndent] == '\t'; nIndent ++);

		tvi.itemex.mask						= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.itemex.pszText				= LPSTR_TEXTCALLBACK;	//&(p->szText[nIndent]);
		tvi.itemex.cchTextMax			= 50;
		tvi.itemex.iImage					= I_IMAGECALLBACK;
		tvi.itemex.iSelectedImage = I_IMAGECALLBACK;
		tvi.itemex.lParam					= (LPARAM)p;
		

		// 2. 부모 노드 결정
		if (nIndent == nLevel) {
			tvi.hParent = GetParentItem(hItem);
		}
		else if (nIndent > nLevel) {
			tvi.hParent = hItem;
			nLevel ++;
		}
		else if (nIndent < nLevel) {
			tvi.hParent = GetParentItem(hItem);
			while(nLevel != nIndent)	{
				tvi.hParent = GetParentItem(tvi.hParent);
				nLevel --;
			}
		}
		
		tvi.hInsertAfter = TVI_LAST;
		hItem = InsertItem(&tvi);

		if (str != NULL && _stricmp(p->szText, str) == 0)
			hExpandItem = hItem;		
	}

	if (hExpandItem)
	{
		ExpandItem(hExpandItem);
		SelectSetFirstVisible(hExpandItem);
	}
	else		
	{
		hItem = GetRootItem();
		while(hItem) {
				if (ItemHasChildren(hItem))
				ExpandItem(hItem);
			hItem = GetNextSiblingItem(hItem);
		}
		SelectSetFirstVisible(GetRootItem());
	}

	hItem = GetRootItem();
	while(hItem) {
		if (ItemHasChildren(hItem))
			EnableItem(hItem, TOPTION_ITEM::EF_ENABLE, TRUE);
		hItem = GetNextSiblingItem(hItem);
	}
}

/****************************************************************************************
* Function			:	COptionsTreeCtrl::OnGetdispinfo
* Description		:	
* Return Value	:	void
* Parameter			:	NMHDR* pNMHDR   	-	
* Parameter			:	LRESULT* pResult	-	
****************************************************************************************/
void COptionsTreeCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TOPTION_ITEM* p = (TOPTION_ITEM*)GetItemData(pTVDispInfo->item.hItem);
	pTVDispInfo->item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	if (strlen(p->szName) > 0)
	{
		pTVDispInfo->item.iImage = (m_pOptions->GetOption((CString)(p->szName)) == p->nTrueValue? p->nTImgIndex : p->nFImgIndex);
		pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage;
	}
	else
	{
		pTVDispInfo->item.iImage = p->nImgIndex;
		pTVDispInfo->item.iSelectedImage = p->nSelImgIndex;
	}

	if (p->eEnabled == TOPTION_ITEM::EF_DISABLE || p->eEnabled == TOPTION_ITEM::EF_STATIC_DISABLE)
	{
		pTVDispInfo->item.mask			|= TVIF_STATE;
		pTVDispInfo->item.stateMask	= TVIS_CUT;
		pTVDispInfo->item.state			= TVIS_CUT;
	}

	int indent;
	for (indent = 0 ; p->szText[indent] == '\t' ; indent ++);
	pTVDispInfo->item.pszText = &(p->szText[indent]);
	*pResult = 0;
}


/****************************************************************************************
* Function	:	COptionsTreeCtrl::OnClick
* Description		:	
* Return Value	:	void
* Parameter		:	NMHDR* pNMHDR   	-	
* Parameter		:	LRESULT* pResult	-	
****************************************************************************************/
void COptionsTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UINT		nFlag;
	CPoint	pt;
	CRect		rt;

	GetCursorPos(&pt);
	ScreenToClient(&pt);

	HTREEITEM hItem = HitTest(pt, &nFlag);


	if ((nFlag & TVHT_ONITEMICON) != TVHT_ONITEMICON)
		return;
	TOPTION_ITEM* p = (TOPTION_ITEM*)GetItemData(/*pNMTreeView->itemNew.*/hItem);

	if (strlen(p->szName) > 0)	{
		if (p->eEnabled == TOPTION_ITEM::EF_ENABLE || p->eEnabled == TOPTION_ITEM::EF_STATIC_ENABLE)
		{
			switch(p->nTrueValue) {
			case TOPTION_ITEM::OF_TRUE:
				{
					BOOL nFlag = (m_pOptions->GetOption((CString)p->szName) == p->nTrueValue);
					m_pOptions->SetOption((CString)p->szName, nFlag ? TOPTION_ITEM::OF_FALSE : p->nTrueValue);
					EnableItem(hItem, (!nFlag) ? TOPTION_ITEM::EF_ENABLE : TOPTION_ITEM::EF_DISABLE, FALSE);
					InvalidateItem(hItem, TRUE);

					m_pOptions->InformToChangeOpt(p->szName, (TOPTION_ITEM::EOPTION_FLAGS)!nFlag);
				} break;
			case TOPTION_ITEM::OF_FALSE:
				{
					m_pOptions->SetOption((CString)p->szName, p->nTrueValue);
					hItem = GetParentItem(hItem);
					m_pOptions->InformToChangeOpt(p->szName, (TOPTION_ITEM::EOPTION_FLAGS)p->nTrueValue); 
					
					hItem = GetChildItem(hItem);
					while(hItem) {
						p = (TOPTION_ITEM*)GetItemData(hItem);
						EnableItem(hItem, m_pOptions->GetOption((CString)p->szName) == p->nTrueValue ? TOPTION_ITEM::EF_ENABLE : TOPTION_ITEM::EF_DISABLE, false);
						InvalidateItem(hItem, true);
						hItem = GetNextSiblingItem(hItem);
					}
				}
			}
		}
	}
	*pResult = 0;
}

/****************************************************************************************
* Function	:	COptionsTreeCtrl::ExpandItem
* Description		:	
* Return Value	:	void
* Parameter		:	HTREEITEM hItem	-	
****************************************************************************************/
void COptionsTreeCtrl::ExpandItem(HTREEITEM hItem)
{
//	CTreeCtrl::Expand(hItem, TVE_EXPAND);
	hItem = GetChildItem(hItem);
	while(hItem) {
		if (ItemHasChildren(hItem))
			ExpandItem(hItem);
		hItem = GetNextSiblingItem(hItem);
	}
}

/****************************************************************************************
* Function	:	COptionsTreeCtrl::EnableItem
* Description		:	
* Return Value	:	void
* Parameter		:	HTREEITEM hItem	-	
* Parameter		:	BOOL enable    	-	
* Parameter		:	BOOL self      	-	
****************************************************************************************/
void COptionsTreeCtrl::EnableItem(HTREEITEM hItem, TOPTION_ITEM::EENABLE_FLAGS eEnabled, BOOL b)
{
	TOPTION_ITEM* p = (TOPTION_ITEM*)GetItemData(hItem);
	TOPTION_ITEM::EENABLE_FLAGS flag;

	ASSERT(eEnabled != TOPTION_ITEM::EF_STATIC_ENABLE);
	ASSERT(eEnabled != TOPTION_ITEM::EF_STATIC_DISABLE);

	if (b && p->eEnabled != TOPTION_ITEM::EF_STATIC_ENABLE && p->eEnabled != TOPTION_ITEM::EF_STATIC_DISABLE)
	{
		p->eEnabled = eEnabled;
	}

	if (strlen(p->szName) != 0)
	{
		if (p->eEnabled == TOPTION_ITEM::EF_ENABLE || p->eEnabled == TOPTION_ITEM::EF_STATIC_ENABLE)
		{
			flag = (m_pOptions->GetOption((CString)p->szName) == p->nTrueValue) ? TOPTION_ITEM::EF_ENABLE : TOPTION_ITEM::EF_DISABLE;
		}
		else
		{
			ASSERT(p->eEnabled == TOPTION_ITEM::EF_DISABLE || p->eEnabled == TOPTION_ITEM::EF_STATIC_DISABLE);
			flag = TOPTION_ITEM::EF_DISABLE;
		}
	}
	else
	{
		if (p->eEnabled == TOPTION_ITEM::EF_STATIC_ENABLE || p->eEnabled == TOPTION_ITEM::EF_ENABLE)
		{
			flag = TOPTION_ITEM::EF_ENABLE;
		}
		else
		{
			ASSERT(p->eEnabled == TOPTION_ITEM::EF_DISABLE || p->eEnabled == TOPTION_ITEM::EF_STATIC_DISABLE);
			flag = TOPTION_ITEM::EF_DISABLE;
		}
	}
	
	for (hItem = GetChildItem(hItem) ; hItem != NULL ; )
	{

		if (ItemHasChildren(hItem))
		{
			EnableItem(hItem, flag, TRUE);
		}
		else
		{
			p = (TOPTION_ITEM*)GetItemData(hItem);
			if (p->eEnabled != TOPTION_ITEM::EF_STATIC_ENABLE && p->eEnabled != TOPTION_ITEM::EF_STATIC_DISABLE)
				p->eEnabled = flag;
		}
		hItem = GetNextSiblingItem(hItem);
	}

}

/****************************************************************************************
* Function	:	COptionsTreeCtrl::InvalidateItem
* Description		:	
* Return Value	:	void
* Parameter		:	HTREEITEM hItem	-	
* Parameter		:	BOOL self      	-	
****************************************************************************************/
void COptionsTreeCtrl::InvalidateItem(HTREEITEM hItem, BOOL b)
{
	CRect rt;
	if (b)
	{
		GetItemRect(hItem, &rt, FALSE);
		InvalidateRect(&rt, TRUE);
		
	}

	hItem = GetChildItem(hItem);

	while(hItem) {
		if (ItemHasChildren(hItem) && GetItemState(hItem, TVIS_EXPANDED)) {
			InvalidateItem(hItem, TRUE);
		}
		else {
			GetItemRect(hItem, &rt, FALSE);
			InvalidateRect(&rt, TRUE);
		}
		hItem = GetNextSiblingItem(hItem);
	}
}


void COptionsTreeCtrl::ApplyLoad(void)
{
	CString strKey, strTemp, strOption;
	strOption = m_pOptions->GetOptions();


	int			nVal;
	int			pos = 0;

	while(1){
		strKey = strOption.Tokenize(_T(";"),pos);
		if(pos == -1) break;
		nVal = atoi(strOption.Tokenize(_T(";"),pos));
		if(pos == -1) break;
		//BOOL nFlag = (m_pOptions->GetOption(strKey ) == nVal );
		BOOL nFlag = nVal +1;
		m_pOptions->InformToChangeOpt(const_cast<char*>(strKey.GetString()),(TOPTION_ITEM::EOPTION_FLAGS)!nFlag);
	}
	
}
