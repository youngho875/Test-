// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://msdn.microsoft.com/officeui를 참조하십시오.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "pch.h"
#include "MainFrm.h"
#include "C4IMap.h"
#include "C4IMapView.h"
#include "MapPropertiesWnd.h"

#include "ComDefStringValue.h"
#include "../inc/define_struct.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CMapPropertiesWnd::CMapPropertiesWnd()
{
	m_emPropKind = MAPPROPERTY_NONE;
	m_nLayerIndex = -1;
	m_nFeatureIndex = -1;
}

CMapPropertiesWnd::~CMapPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CMapPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

void CMapPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CMapPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CMapPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CMapPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CMapPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CMapPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CMapPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CMapPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CMapPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CMapPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CMapPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
}

void CMapPropertiesWnd::ClearPropList()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.Invalidate(FALSE);

	m_emPropKind = MAPPROPERTY_NONE;
	m_nLayerIndex = -1;
	m_nFeatureIndex = -1;
}

// 지형 검색 속성 정보 설정
void CMapPropertiesWnd::SetSearchFeatureInfo(CString strTitle, vector<CString> vInfoName, vector<CString> vInfoContent)
{
	m_emPropKind = MAPPROPERTY_SEARCH;
	m_nLayerIndex = -1;
	m_nFeatureIndex = -1;

	if(strTitle.IsEmpty())
		strTitle = DEF_PROPERTY_SEARCH_FEATURE_TITLE;

	CMFCPropertyGridProperty* pGroupTitle = new CMFCPropertyGridProperty(strTitle);
	pGroupTitle->SetData(MAPPROPERTY_TITLE);
	
	for(int i=0; i<(int)vInfoName.size(); i++)
	{
		CMFCPropertyGridProperty* pGroupItem = new CMFCPropertyGridProperty(vInfoName.at(i), (_variant_t)vInfoContent.at(i), _T(""));
		pGroupItem->AllowEdit(FALSE);
		pGroupItem->SetData(MAPPROPERTY_CONTENT);
		pGroupTitle->AddSubItem(pGroupItem);
	}

	m_wndPropList.AddProperty(pGroupTitle);
}

void CMapPropertiesWnd::AddSelectLayerInfo(LAYER_ATTRIBUTE* pLayerInfo, COLORREF clrLayer, BOOL bOnlyLayer)
{
	if(pLayerInfo == NULL)
		return;

	m_emPropKind = MAPPROPERTY_SELECT_LAYER;
	m_nLayerIndex = pLayerInfo->nIndex;
	m_nFeatureIndex = -1;

	// 레이어 정보
	CMFCPropertyGridProperty* pGroupLayer = new CMFCPropertyGridProperty(DEF_PROPERTY_SELECT_LAYERE_TITLE);
	pGroupLayer->SetData(MAPPROPERTY_TITLE);

	// 레이어 인덱스
	CMFCPropertyGridProperty* pGroupItem = new CMFCPropertyGridProperty("인덱스", (_variant_t)pLayerInfo->nIndex, "레이어 인덱스");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupLayer->AddSubItem(pGroupItem);

	CString strContent;
	switch(pLayerInfo->type)
	{
	case MAP_PRIMITIVE_POINT:
		strContent = "점";
		break;
	case MAP_PRIMITIVE_LINE:
		strContent = "선";
		break;
	case MAP_PRIMITIVE_POLYGON:
		strContent = "다각형";
		break;
	case MAP_PRIMITIVE_RASTER:
		strContent = "Raster";
		break;
	case MAP_PRIMITIVE_DEM:
		strContent = "Dem";
		break;
	default:
		strContent = "기타";
		break;
	}

	pGroupItem = new CMFCPropertyGridProperty("형식", (_variant_t)strContent, "레이어 타입");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupLayer->AddSubItem(pGroupItem);

	pGroupItem = new CMFCPropertyGridProperty("명칭", (_variant_t)pLayerInfo->layerName.c_str(), "레이어 명칭");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupLayer->AddSubItem(pGroupItem);

	pGroupItem = new CMFCPropertyGridProperty("분류", (_variant_t)pLayerInfo->layerCategory.c_str(), "레이어 분류 명칭");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupLayer->AddSubItem(pGroupItem);

	if(bOnlyLayer == TRUE && (pLayerInfo->type == MAP_PRIMITIVE_LINE || pLayerInfo->type == MAP_PRIMITIVE_POLYGON))
	{
		CMFCPropertyGridColorProperty* pPropertyColor = new CMFCPropertyGridColorProperty("색상", clrLayer, NULL, "레이어 색상 설정", 0);
		pPropertyColor->SetData(MAPPROPERTY_COLOR);
		pPropertyColor->EnableOtherButton(DEF_PROPERTY_USER_VALUE);
		pPropertyColor->AllowEdit(FALSE);
		pGroupLayer->AddSubItem(pPropertyColor);
	}


	m_wndPropList.AddProperty(pGroupLayer);
}

void CMapPropertiesWnd::AddSelectFeatureInfo(LAYER_ATTRIBUTE* pLayerInfo, FEATUREGROUP_ATTRIBUTE * pFeatureInfo, COLORREF clrFeature)
{
	AddSelectLayerInfo(pLayerInfo, clrFeature, FALSE);

	if(pFeatureInfo == NULL)
		return;

	m_emPropKind = MAPPROPERTY_SELECT_FEATURE;
	m_nFeatureIndex = pFeatureInfo->nIndex;

	// 레이어 정보
	CMFCPropertyGridProperty* pGroupFeature = new CMFCPropertyGridProperty(DEF_PROPERTY_SELECT_FEATURE_TITLE);
	pGroupFeature->SetData(MAPPROPERTY_TITLE);

	// 피쳐 인덱스
	CMFCPropertyGridProperty* pGroupItem = new CMFCPropertyGridProperty("인덱스", (_variant_t)pFeatureInfo->nIndex, "피쳐 인덱스");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupFeature->AddSubItem(pGroupItem);

	// 피쳐 인덱스
	pGroupItem = new CMFCPropertyGridProperty("명칭", (_variant_t)pFeatureInfo->featureName.c_str(), "피쳐 명칭");
	pGroupItem->AllowEdit(FALSE);
	pGroupItem->SetData(MAPPROPERTY_CONTENT);
	pGroupFeature->AddSubItem(pGroupItem);

	if(pLayerInfo != NULL && (pLayerInfo->type == MAP_PRIMITIVE_LINE || pLayerInfo->type == MAP_PRIMITIVE_POLYGON))
	{
		CMFCPropertyGridColorProperty* pPropertyColor = new CMFCPropertyGridColorProperty("색상", clrFeature, NULL, "레이어 색상 설정", 0);
		pPropertyColor->SetData(MAPPROPERTY_COLOR);
		pPropertyColor->EnableOtherButton(DEF_PROPERTY_USER_VALUE);
		pPropertyColor->AllowEdit(FALSE);
		pGroupFeature->AddSubItem(pPropertyColor);
	}

	m_wndPropList.AddProperty(pGroupFeature);
}

// wparam  - The control ID of the property list.
// lparam   - A pointer to the property (CMFCPropertyGridProperty) that changed.
LRESULT CMapPropertiesWnd::OnPropertyChanged( WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCPropertyGridProperty *pChangedProp = (CMFCPropertyGridProperty*)lparam;

	if(pChangedProp != NULL && pFrame != NULL)
	{
		if(pChangedProp->GetData() == MAPPROPERTY_COLOR)
		{
			if(m_emPropKind == MAPPROPERTY_SELECT_LAYER)
			{
				MAPVIEW_PROP_COLOR stPropColor;
				stPropColor.nLayerIndex = m_nLayerIndex;
				stPropColor.clr = pChangedProp->GetValue().lVal;

				CC4IMapView * pView = (CC4IMapView*)pFrame->GetActiveView();
				if(pView)
					pView->SendMessage(MAPVIEW_PROP_EDIT_LAYERCOLOR, (WPARAM)&stPropColor);
			
			}
			else if(m_emPropKind == MAPPROPERTY_SELECT_FEATURE)
			{
				MAPVIEW_PROP_COLOR stPropColor;
				stPropColor.nLayerIndex = m_nLayerIndex;
				stPropColor.nFeatureIndex = m_nFeatureIndex;
				stPropColor.clr = pChangedProp->GetValue().lVal;

				COLORREF clrChangedValue = pChangedProp->GetValue().lVal;

				CC4IMapView* pView = (CC4IMapView*)pFrame->GetActiveView();
				if(pView)
					pView->SendMessage(MAPVIEW_PROP_EDIT_FEATURECOLOR, (WPARAM)&stPropColor);
			}
		}
	}

	return TRUE;
}
