// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해
// 추가적으로 제공되는 내용입니다.
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// C4IMap.h: C4IMap 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "MainFrm.h"




//#include "../inc/ICOPS2010V11520/ICOPS_Framework/CAppContext.h"
//#include "../inc/ICOPS2010V11520/ICOPS_WarSym/IWarSymFactory.h"
//#include "../inc/ICOPS2010V11520/ICOPS_LogManager/CLog.h"

// CC4IMapApp:
// 이 클래스의 구현에 대해서는 C4IMap.cpp을(를) 참조하세요.
//

class CC4IMapApp : public CWinAppEx
{
public:
	CC4IMapApp() noexcept;


// 재정의입니다.
public:

	BOOL	InitICOPS(void);
	void	ExitICOPS(void);


	CMainFrame* m_pMainFrame;

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 군대부호
	//ICOPS::CAppContext* m_pAppCxt;
	//ICOPS::IWarSymFactoryPtr	m_piSymFac;

private:
	//GDI+를 위한 변수 선언 
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

	//HANDLE m_hMutex;

	 
	void Begin(void);
	//void End(void);


public:
// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	//afx_msg void OnFrame1Focus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAppAbout();
	afx_msg void OnViewFrame();
	afx_msg void OnUpdateViewFrame(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

extern CC4IMapApp theApp;
