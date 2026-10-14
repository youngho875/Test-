// OutputLogList.cpp : 구현 파일입니다.
//

#include "pch.h"
#include "C4IMap.h"
#include "MainFrm.h"
#include "OutputLogList.h"


// COutputLogList

IMPLEMENT_DYNAMIC(COutputLogList, CListBox)

COutputLogList::COutputLogList()
{

}

COutputLogList::~COutputLogList()
{
}


BEGIN_MESSAGE_MAP(COutputLogList, CListBox)
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_POPUP_OUTPUT_COPY, OnLogCopy)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OUTPUT_COPY, OnUpdateLogCopy)
	ON_COMMAND(ID_POPUP_OUTPUT_CLEAR, OnLogClear)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OUTPUT_CLEAR, OnUpdateLogClear)
	ON_COMMAND(ID_POPUP_OUTPUT_ALLCLEAR, OnLogAllClear)
	ON_UPDATE_COMMAND_UI(ID_POPUP_OUTPUT_ALLCLEAR, OnUpdateLogAllClear)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// COutputLogList 메시지 처리기입니다.



void COutputLogList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_OUTPUT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputLogList::OnLogCopy()
{

}

void COutputLogList::OnUpdateLogCopy(CCmdUI* pCmdUI)
{

}

void COutputLogList::OnLogClear()
{

}

void COutputLogList::OnUpdateLogClear(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(FALSE);
}

void COutputLogList::OnLogAllClear()
{

}

void COutputLogList::OnUpdateLogAllClear(CCmdUI* pCmdUI)
{

}
BOOL COutputLogList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect rt;
	GetClientRect(rt);
	ClientToScreen(rt);

	// 마우스 휠 이동 시 영역 외 일 경우 메인으로 포커스 이동
	if (!rt.PtInRect(pt))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame != NULL)
			pFrame->SetFocus();
		return FALSE;
	}

	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}
