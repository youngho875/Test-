#include "pch.h"
#include "ViewList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CViewList::CViewList()
{
}

CViewList::~CViewList()
{
}

BEGIN_MESSAGE_MAP(CViewList, CListCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

BOOL CViewList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CListCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

BOOL CViewList::CopyToClipboard(LPCTSTR lpszSeparator, BOOL bCopyHeaderText)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	CString sResult;
	POSITION pos = GetFirstSelectedItemPosition();
	if (!pos)
		return TRUE;

	CWaitCursor wait;
	int nItem, nCount = 0;
	int nColumn = 1;

	if ((GetStyle() & LVS_TYPEMASK) == LVS_REPORT &&
		GetExtendedStyle() & LVS_EX_FULLROWSELECT)
	{
		CHeaderCtrl* pHeader = GetHeaderCtrl();
		nColumn = pHeader ? pHeader->GetItemCount() : 1;

		if (bCopyHeaderText && pHeader)
		{
			for (int i = 0; i < nColumn; ++i)
			{
				TCHAR szBuffer[256];
				HDITEM hdi;
				hdi.mask = HDI_TEXT;
				hdi.pszText = szBuffer;
				hdi.cchTextMax = 256;

				pHeader->GetItem(i, &hdi);
				sResult += szBuffer;
				if (i != nColumn - 1)
					sResult += lpszSeparator;
			}
			++nCount;
		}
	}

	while (pos)
	{
		nItem = GetNextSelectedItem(pos);
		if (0 != nCount)
			sResult += _T("\r\n");

		for (int i = 0; i < nColumn; ++i)
		{
			sResult += GetItemText(nItem, i);
			if (i != nColumn - 1)
				sResult += lpszSeparator;
		}
		++nCount;
	}

	if (OpenClipboard())
	{
		EmptyClipboard();

		int nLen = sResult.GetLength() * sizeof(WCHAR) + 1;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
		LPBYTE pGlobalData = (LPBYTE)GlobalLock(hGlobal);

		USES_CONVERSION_EX;
		CopyMemory(pGlobalData, T2CW_EX(sResult, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
		SetClipboardData(CF_UNICODETEXT, hGlobal);

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);

		CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

