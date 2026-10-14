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

// C4IMap.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "C4IMap.h"
#include "MainFrm.h"
#include "C4IMapDoc.h"
#include "C4IMapView.h"
//#include "FilePath.h"

//#include "../inc/ICOPS2010V11520/ICOPS_Framework/CAppContext.h"
//#include "../inc/ICOPS2010V11520/ICOPS_WarSym/IWarSymFactory.h"
//#include "../inc/ICOPS2010V11520/ICOPS_LogManager/CLog.h"

#define _GLOBAL_DECLARATION_
//#include "GlobalVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CC4IMapApp

BEGIN_MESSAGE_MAP(CC4IMapApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CC4IMapApp::OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FRAME, OnUpdateViewFrame)
	ON_COMMAND(ID_VIEW_FRAME, OnViewFrame)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CC4IMapApp 생성

CC4IMapApp::CC4IMapApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;

	// 군대부호 막음 유영호
	//ICOPS::CLog::SetAppName(_T("C4IMap"));
	//ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Info, _T("Starting C4IMap Application."));

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("C4IMap.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CC4IMapApp 개체입니다.

CC4IMapApp theApp;


// CC4IMapApp 초기화

BOOL CC4IMapApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	if (!InitICOPS())
	{
		AfxMessageBox(_T("ICOPS 컴포넌트 초기화를 실패했습니다."));
		return FALSE;
	}

	//Begin();
		
	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CC4IMapDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CC4IMapView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Gdiplus start
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);  // SW_SHOWMAXIMIZED
	m_pMainWnd->UpdateWindow();

	//CString strTitle;
	//strTitle.LoadString(IDS_PIMTITLE);
	m_pMainWnd->SetWindowText(_T("상황도"));

	return TRUE;
}

int CC4IMapApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	//GDI+ 종료
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	//ReleaseMutex(m_hMutex);

/*
	ExitICOPS();
	ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Info, _T("Ending C4IMap Application."));
*/

	return 1;
}

// CC4IMapApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CC4IMapApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CC4IMapApp 사용자 지정 로드/저장 방법

void CC4IMapApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CC4IMapApp::LoadCustomState()
{
}

void CC4IMapApp::SaveCustomState()
{
}

void CC4IMapApp::OnViewFrame()
{
	if (m_pMainFrame->GetStyle() & WS_VISIBLE)
		m_pMainFrame->ShowWindow(SW_HIDE);
	else
		m_pMainFrame->ShowWindow(SW_SHOW);
}

void CC4IMapApp::OnUpdateViewFrame(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_pMainFrame->GetStyle() & WS_VISIBLE);
	if ((m_pMainFrame->GetStyle() & WS_VISIBLE))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable();
}


//void CC4IMapApp::OnFrame1Focus(WPARAM w/* wParam */, LPARAM l/* lParam */)
//{
//	m_pMainWnd = m_pMainFrame;
//	//return 0;
//}

void CC4IMapApp::Begin(void)
{

	// subifox,06.06.19
	// 1. 작업 폴더 구하기
	//CFilePath::ResetRoot();
}

//void CC4IMapApp::End(void)
//{
//	LOGGER_RELEASE();
//	PROFILER_RELEASE();
//}

// CC4IMapApp 메시지 처리기

// 군대부호처리
BOOL CC4IMapApp::InitICOPS(void)
{
/*
	BOOL		bRes;
	CString		strUrl;
	ICOPS::CAppContext* pAppCxt;
	ICOPS::IWarSymFactoryPtr	piFac;

	pAppCxt = new ICOPS::CAppContext;

	// Application context 정보를 담고 있는 파일 경로 얻기
	//strUrl = "C:/AFC4I/Bin/ICOPS_AppContext.xml";
	//#if defined(_DEBUG)
	//	strUrl = GetProfileString(_T("Settings"), _T("app_context_debug"));
	//#else
	//	strUrl = GetProfileString(_T("Settings"), _T("app_context"));
	//#endif
	// 
	strUrl = L"D:/work/상황도/ICOPS/config/ICOPS_AppContext_test.xml";
	ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Info, _T("실행환경 설정정보 파일 경로 : %s"), strUrl);
	if (strUrl.IsEmpty())
	{
		::AfxMessageBox(_T("실행환경 설정정보 파일이 지정되지 않았습니다."), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// Initialize application context
	bRes = pAppCxt->Initialize(_T("ICOPS_Framework"), _T(".dll"), _T("CAppContextDataAgentXml"), "D:/work/상황도/ICOPS/config/ICOPS_AppContext_test.xml");
	if (!bRes)
	{
		ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Error, _T("실행환경 설정정보 초기화 실패."));
		delete pAppCxt;
		return FALSE;
	}

	m_pAppCxt = pAppCxt;

	// 군대부호를 사용하려면 군대부호관리 객체를 초기화한다.
	bRes = m_pAppCxt->GetStockObject(_T("IWarSymFactory"), (void**)&piFac);
	if (!bRes)
	{
		ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Error, _T("군대부호관리 객체 생성 실패."));
		return FALSE;
	}
	bRes = piFac->Initialize(m_pAppCxt);
	if (!bRes)
	{
		ICOPS::CLog::AddLog(__FILE__, __LINE__, ICOPS::CLog::Log_Error, _T("군대부호관리 객체 초기화 실패."));
		return FALSE;
	}
	m_piSymFac = piFac;
*/

	return TRUE;
}

void CC4IMapApp::ExitICOPS(void)
{
	// CAppContext에서 gdiplus 해제하기 전에 객체 해제해야함
/*
 	m_piSymFac = NULL;							
	if (m_pAppCxt != NULL) delete m_pAppCxt;
*/
}

