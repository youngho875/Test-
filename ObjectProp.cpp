#include "pch.h"
#include "ObjectProp.h"
#include "resource.h"


CObjectProp::CObjectProp(void)
{
}


CObjectProp::~CObjectProp(void)
{
}


CObjectProp* CObjectProp::_init = NULL;

CObjectProp* CObjectProp::GetInst(void)
{
	if (!_init)
		_init = new CObjectProp();
	return _init;
}

void CObjectProp::ReleaseInst(void)
{
		if(_init != NULL)
		{
			delete _init;
			_init = NULL;
		}
}

void CObjectProp::Halfreset(void)//type 0 reset
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end(); )
	{
		if(itr_begin->second.m_iType == 0)
		{
			//delete itr_begin->second.m_font;
			itr_begin  = m_mapObj.erase(itr_begin);
		}
		else itr_begin++;
	}

}


void CObjectProp::SelReset(int type)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end(); )
	{
		if(itr_begin->second.m_iType == type)
		{
			//delete itr_begin->second.m_font;
			itr_begin  = m_mapObj.erase(itr_begin);
		}
		else itr_begin++;
	}
}


void CObjectProp::AllReset(void)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end(); )
	{
		//delete itr_begin->second.m_font;
	}
	m_mapObj.clear();
}


bool CObjectProp::DrawAlt(CDC* pdc, int x, int y, CRect* rec, CDC* srcDC, CString Key,DWORD flag, CString name,int type)
{
	bool k=true;
	std::map<CString,S_OBJECT_UNIT>::iterator itr = m_mapObj.find(Key);
	if(itr == m_mapObj.end())
	{
		S_OBJECT_UNIT tempUnit;
		tempUnit.m_rect = *rec;
		tempUnit.strName = name;
		tempUnit.m_iType = type;
		tempUnit.m_bSel = false;
		tempUnit.m_font = NULL;
		tempUnit.m_iZoom = 0;
		tempUnit.m_iChro = 240;
		m_mapObj[Key] = tempUnit;
	}else{
		itr->second.m_rect = *rec;
		if(itr->second.m_iZoom != 0 || itr->second.m_iChro != 240 )
		{
			ZoomCurRect(itr->second.m_iZoom, &itr->second.m_rect); 
			ChroCurDC(itr->second.m_iChro, srcDC,CRect(0,0,rec->Width(),rec->Height()));
			if(!flag) 
			{
				CDC dcTemp;
				//CRect Vrec;
				//view->GetWindowRect(&Vrec);
				dcTemp.CreateCompatibleDC(pdc); 
				CBitmap m_pBitmapTemp;
				m_pBitmapTemp.CreateCompatibleBitmap(pdc,itr->second.m_rect.Width(),itr->second.m_rect.Height());
				dcTemp.SelectObject(&m_pBitmapTemp);

				dcTemp.StretchBlt(0,0,itr->second.m_rect.Width(),itr->second.m_rect.Height(),
					srcDC,0,0,rec->Width(),rec->Height(),SRCCOPY);
				pdc->TransparentBlt(itr->second.m_rect.left,itr->second.m_rect.top,itr->second.m_rect.Width(),itr->second.m_rect.Height(), &dcTemp,0,0,itr->second.m_rect.Width(),itr->second.m_rect.Height(),RGB(0,0,0) );

			}else{
				pdc->StretchBlt(itr->second.m_rect.left,itr->second.m_rect.top,itr->second.m_rect.Width(),itr->second.m_rect.Height(),
					srcDC,0,0,rec->Width(),rec->Height(),flag);
			}
			*rec = itr->second.m_rect;
			k =  false;

		}
		if(itr->second.m_bSel)
		{
			CGdiObject* brush =  pdc->SelectStockObject(NULL_BRUSH);
			CPen pen;
			pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
			CPen* oldpen = pdc->SelectObject(&pen);
			pdc->Rectangle(itr->second.m_rect);
			pdc->SelectObject(oldpen);
			pdc->SelectObject(brush);

		}
	}

	return k;
}


void CObjectProp::SetZoom(int InOut)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
	{
		if(itr_begin->second.m_bSel)
		{
			if((itr_begin->second.m_iZoom <= 4 && InOut >0)||(itr_begin->second.m_iZoom >= -4 && InOut <0))
				itr_begin->second.m_iZoom += InOut;
		}
	}
}


void CObjectProp::SetChro(int Value)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
	{
		if(itr_begin->second.m_bSel)
		{
			itr_begin->second.m_iChro = Value;
		}
	}

}


void CObjectProp::SetFont(LOGFONT nFont)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
	{
		if(itr_begin->second.m_bSel)
		{
			
			if(!itr_begin->second.m_font) delete itr_begin->second.m_font;//itr_begin->second.m_font->DeleteObject();
			itr_begin->second.m_font = new CFont();
			//itr_begin->second.m_font->CreatePointFontIndirect(&logFont);
			itr_begin->second.m_font->CreateFontA(nFont.lfHeight,nFont.lfWidth,nFont.lfEscapement,nFont.lfOrientation,
				nFont.lfWeight,nFont.lfItalic,nFont.lfUnderline,nFont.lfStrikeOut,nFont.lfCharSet,nFont.lfOutPrecision,
				nFont.lfClipPrecision,nFont.lfQuality,nFont.lfPitchAndFamily,nFont.lfFaceName);
		}
	}

}


bool CObjectProp::CheckHit(CPoint point)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
	{
		if(itr_begin->second.m_bSel)
		{
			if(itr_begin->second.m_rect.PtInRect(point)) 
				return true;
		}
	}

	return false;
}


void CObjectProp::CheckPoint(CPoint point)
{
	bool reset = true;;
	std::map<CString,S_OBJECT_UNIT>::iterator itr_begin;
	tempV.clear();

	for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
	{
		if(itr_begin->second.m_rect.PtInRect(point)) 
		{
			tempV[itr_begin->first] = itr_begin->second;
		}
	}
	if(tempV.size())
	{
		std::map<CString,S_OBJECT_UNIT>::iterator itr;
		if(tempV.size() > 1)
		{
			CMenu menu;
			menu.CreatePopupMenu();
			
			
			int i =0;
			for(itr = tempV.begin(); itr != tempV.end();itr++)
			{
				CString TempStrN;
				if(itr->second.strName == _T("")) TempStrN.Format(_T("%d"),i);
				else TempStrN = itr->second.strName ;

				// 메뉴정리 후 활성화 필요 - YOUNGHO
				menu.InsertMenu(i,MF_BYCOMMAND,ID_MENU_1_AO+i,TempStrN);
				menu.CheckMenuItem(ID_MENU_1_AO+i, itr->second.m_bSel?MF_CHECKED:MF_UNCHECKED);
				i++;
				//if(i > ID_MENU_20_AO)break;
			}
			CPoint scP = point;
			view->ClientToScreen(&scP);
			
			menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,scP.x,scP.y,view);

		}else{
			m_mapObj.find(tempV.begin()->first)->second.m_bSel = true;
		}

	}
	else
	{
		for(itr_begin = m_mapObj.begin(); itr_begin != m_mapObj.end();itr_begin++ )
			itr_begin->second.m_bSel = false;
	}

}


void CObjectProp::SetInst(CObjectProp* init)
{
	_init = init;
}


bool CObjectProp::FontAlt(CString Key, CFont* font,int type)
{
	std::map<CString,S_OBJECT_UNIT>::iterator itr = m_mapObj.find(Key);
	if(itr != m_mapObj.end())
	{
		LOGFONT logFont,logFontN;
		font->GetLogFont(&logFont);
		if(!itr->second.m_font)
		{
			itr->second.m_font = new CFont();
			//itr->second.m_font->CreatePointFontIndirect(&logFont);
			itr->second.m_font->CreateFontA(logFont.lfHeight,logFont.lfWidth,logFont.lfEscapement,logFont.lfOrientation,
				logFont.lfWeight,logFont.lfItalic,logFont.lfUnderline,logFont.lfStrikeOut,logFont.lfCharSet,logFont.lfOutPrecision,
				logFont.lfClipPrecision,logFont.lfQuality,logFont.lfPitchAndFamily,logFont.lfFaceName);
		}
		itr->second.m_font->GetLogFont(&logFontN);
		if(!LogFontE(logFontN,logFont))
		{
			font->DeleteObject();
			font->CreateFontA(logFontN.lfHeight,logFontN.lfWidth,logFontN.lfEscapement,logFontN.lfOrientation,
				logFontN.lfWeight,logFontN.lfItalic,logFontN.lfUnderline,logFontN.lfStrikeOut,logFontN.lfCharSet,logFontN.lfOutPrecision,
				logFontN.lfClipPrecision,logFontN.lfQuality,logFontN.lfPitchAndFamily,logFontN.lfFaceName);
			return false;
		}
	}
	return true;
}


bool CObjectProp::LogFontE(LOGFONT LogF1, LOGFONT LogF2)
{
	CString name1 = LogF1.lfFaceName,name2 = LogF2.lfFaceName;
	
	
	if(LogF1.lfHeight					==	LogF2.lfHeight					&&
		LogF1.lfWidth					==	LogF2.lfWidth					&&
		LogF1.lfEscapement				==	LogF2.lfEscapement				&&
		LogF1.lfOrientation				==	LogF2.lfOrientation				&&
		LogF1.lfWeight					==	LogF2.lfWeight					&&
		LogF1.lfItalic					==	LogF2.lfItalic					&&
		LogF1.lfUnderline				==	LogF2.lfUnderline				&&
		LogF1.lfStrikeOut				==	LogF2.lfStrikeOut				&&
		LogF1.lfCharSet					==	LogF2.lfCharSet					&&
		LogF1.lfOutPrecision			==	LogF2.lfOutPrecision			&&
		LogF1.lfClipPrecision			==	LogF2.lfClipPrecision			&&
		LogF1.lfQuality					==	LogF2.lfQuality					&&
		LogF1.lfPitchAndFamily			==	LogF2.lfPitchAndFamily			&&
		name1	==	name2)return true;


	return false;
}


void CObjectProp::SetWnd(CWnd* viewWnd)
{
	view = viewWnd;
}


void CObjectProp::SetMenuSel(UINT ID)
{
	// 메뉴 정리 후 활성화 -- YOUNGHO
	//UINT i = ID-ID_MENU_1_AO;
	//std::map<CString,S_OBJECT_UNIT>::iterator itr;
	//if(tempV.size() >i)
	//{
	//	itr = tempV.begin();
	//	while(i)
	//	{
	//		itr++;
	//		i--;
	//	}
	//	m_mapObj.find(itr->first)->second.m_bSel = true;
	//}
	
		
}


void CObjectProp::ZoomCurRect(int zoom, CRect* rec)
{
	rec->left  -= zoom*rec->Width()/6;
	rec->top   -= zoom*rec->Height()/6;
	rec->bottom+= zoom*rec->Height()/6;
	rec->right += zoom*rec->Width()/6;

}


void CObjectProp::ChroCurDC(int chro, CDC* tempDC,CRect rec)
{
	for(int i = 0;i<= rec.right;i++)
	{
		for(int j = 0;j <= rec.bottom;j++)
		{
			COLORREF color = tempDC->GetPixel(i,j);
			if(color==0) continue;
			color = ChangeChro(color,chro);
			tempDC->SetPixelV(i,j,color);
		}
	}
	
}


COLORREF CObjectProp::ChangeChro(COLORREF rgb, int Chro)
{
	double H, S, L;
	double m1, m2;
	double delta;
   double r = (double)GetRValue(rgb) / 255;
   double g = (double)GetGValue(rgb) / 255;
   double b = (double)GetBValue(rgb) / 255;
   double cmax = max(r, max(g, b));
   double cmin = min(r, min(g, b));
   L = (cmax + cmin) / 2.0;
   if (cmax == cmin) {
      S = 0;
      H = 0; // it's really undefined
   } else {
      if (L < 0.5) 
         S = (cmax - cmin) / (cmax + cmin);
      else
         S = (cmax - cmin) / (2.0 - cmax - cmin);
      delta = cmax - cmin;
      if (r == cmax)
          H = (g - b) / delta;
      else if (g == cmax)
          H = 2.0 + ( b - r) / delta;
      else
          H = 4.0 + (r - g) / delta;
      H /= 6.0;
      if(H < 0.0)
          H += 1;
   }

   S = S*Chro/240;
   if (S == 0) {
		r = g = b = L;
	}
	else {
		if (L <= 0.5) {
			m2 = L * (1.0 + S);
		}
		else {
			m2 = L + S - L * S;
		}

		m1 = 2.0 * L - m2;
		r = HuetoRGB(m1, m2, H + 1.0 / 3.0);
		g = HuetoRGB(m1, m2, H);
		b = HuetoRGB(m1, m2, H - 1.0 / 3.0);
	}
	return RGB((BYTE)(r * 255), (BYTE)(g * 255), (BYTE)(b * 255));
}


double CObjectProp::HuetoRGB(double m1, double m2, double h)
{
	if (h < 0) h += 1.0;
   if (h > 1) h -= 1.0;
   if (6.0 * h < 1 )
      return ( m1 + (m2 - m1) * h * 6.0);
   if (2.0 * h < 1)
      return m2;
   if (3.0 * h < 2.0)
      return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
   return m1;
}
