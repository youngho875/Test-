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

// C4IMapDoc.cpp: CC4IMapDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "C4IMap.h"
#endif

#include "C4IMapDoc.h"
#include "C4IMapView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CC4IMapDoc

IMPLEMENT_DYNCREATE(CC4IMapDoc, CDocument)

BEGIN_MESSAGE_MAP(CC4IMapDoc, CDocument)
END_MESSAGE_MAP()


// CC4IMapDoc 생성/소멸

CC4IMapDoc::CC4IMapDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CC4IMapDoc::~CC4IMapDoc()
{
	DeleteContents();
}

BOOL CC4IMapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CC4IMapDoc serialization

void CC4IMapDoc::Serialize(CArchive& ar)
{
/*
	bool		bRes;
	CString		strXml;
	CFile* pFile;
	CHAR* pszMB;
	int			nLen;

	if (ar.IsStoring())
	{
		bRes = CTest_OverItem::GetOverlayXmlString(m_parOverItems, strXml);
		if (!bRes) return;

		_bstr_t	bstrXml(strXml);

		nLen = ::WideCharToMultiByte(CP_UTF8, 0, bstrXml, -1, NULL, 0, NULL, NULL);
		if (nLen > 0)
		{
			pszMB = new CHAR[nLen + 2];

#if _MFC_VER < 0x0800
			::ZeroMemory(pszMB, sizeof(CHAR) * (nLen + 2));
#else
			::SecureZeroMemory(pszMB, sizeof(CHAR) * (nLen + 2));
#endif

			nLen = ::WideCharToMultiByte(CP_UTF8, 0, bstrXml, -1, pszMB, nLen, NULL, NULL);
			ar.Write(pszMB, nLen - 1);
			delete[] pszMB;
		}
	}
	else
	{
		WCHAR* pszWC;
		int		nLenWC;

		pFile = ar.GetFile();
		nLen = (int)pFile->GetLength();
		pszMB = new CHAR[nLen + 2];

#if _MFC_VER < 0x0800
		::ZeroMemory(pszMB, sizeof(CHAR) * (nLen + 2));
#else
		::SecureZeroMemory(pszMB, sizeof(CHAR) * (nLen + 2));
#endif

		ar.Read(pszMB, nLen);
		pszMB[nLen] = '\0';

		strXml.Empty();
		nLenWC = ::MultiByteToWideChar(CP_UTF8, 0, pszMB, -1, NULL, 0);
		if (nLenWC > 0)
		{
			pszWC = new WCHAR[nLenWC + 1];
			nLenWC = ::MultiByteToWideChar(CP_UTF8, 0, pszMB, -1, pszWC, nLenWC);
			strXml = (LPCTSTR)_bstr_t(pszWC);
			delete[] pszWC;
		}
		delete[] pszMB;

		CTest_OverItem::SetOverlayXmlString(m_parOverItems, strXml);

	}
*/
}

void CC4IMapDoc::DeleteContents()
{
    POSITION pos=GetFirstViewPosition();
    while(pos){auto view=DYNAMIC_DOWNCAST(CC4IMapView,GetNextView(pos));if(view)view->ResetLocalOverlay();}
/*
	INT_PTR		inx, count;
	ULONG		nRefCount;
	CTest_OverItem* pOverItem;

	count = m_parOverItems.GetSize();
	if (count > 0)
	{
		for (inx = 0; inx < count; inx++)
		{
			pOverItem = m_parOverItems.GetAt(inx);
			nRefCount = pOverItem->Release();
			ASSERT(nRefCount == 0);
			while (nRefCount > 0) nRefCount = pOverItem->Release();
		}
		m_parOverItems.RemoveAll();
	}
*/
	CDocument::DeleteContents();
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CC4IMapDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CC4IMapDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CC4IMapDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CC4IMapDoc 진단

#ifdef _DEBUG
void CC4IMapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CC4IMapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CC4IMapDoc 명령

BOOL CC4IMapDoc::SaveModified(){
    POSITION pos=GetFirstViewPosition();
    while(pos){auto view=DYNAMIC_DOWNCAST(CC4IMapView,GetNextView(pos));if(view && !view->SaveLocalOverlayIfModified())return FALSE;}
    return CDocument::SaveModified();
}
