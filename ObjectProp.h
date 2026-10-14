#pragma once

#include <map>
#include <vector>


class CObjectProp
{
private:
	static CObjectProp* _init;

public:
	CObjectProp(void);
	~CObjectProp(void);
	static CObjectProp* GetInst(void);
	static void				ReleaseInst(void);

	typedef struct _Object_Uint
	{
		CRect m_rect;
		CFont* m_font;
		int m_iZoom;
		int m_iChro;
		int m_iType;
		bool m_bSel;
		CString strName;
	}S_OBJECT_UNIT;
	
	std::map<CString,S_OBJECT_UNIT> m_mapObj;
	std::map<CString,S_OBJECT_UNIT> tempV;
	CWnd* view;
	CMFCColorPickerCtrl picker;


	void Halfreset(void);
	void SelReset(int type);
	void AllReset(void);
	bool DrawAlt(CDC* pdc, int x, int y, CRect* rec, CDC* srcDC, CString Key,DWORD flag,CString name =_T(""), int type = 0);
	void SetZoom(int InOut);
	void SetChro(int Value);
	void SetFont(LOGFONT nFont);
	bool CheckHit(CPoint point);
	void CheckPoint(CPoint point);
	void SetInst(CObjectProp* init);
	bool FontAlt(CString Key, CFont* font, int type = 0);
	bool LogFontE(LOGFONT LogF1, LOGFONT LogF2);
	void SetWnd(CWnd* viewWnd);
	void SetMenuSel(UINT ID);
	void ZoomCurRect(int zoom, CRect* rec);
	void ChroCurDC(int chro, CDC* tempDC,CRect rec);
	COLORREF ChangeChro(COLORREF rgb, int Chro);
	double HuetoRGB(double m1, double m2, double h);
};

