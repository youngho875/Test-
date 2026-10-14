// 작성 : 외부, unknown
// 수정 : subifox,06.06.14 - 정리 및 수정

#ifndef __HSS_CONFIGRATIONS_OPTIONS_H__
#define __HSS_CONFIGRATIONS_OPTIONS_H__

#include <afxtempl.h>

struct TOPTION_ITEM
{
	enum EOPTION_FLAGS 
	{
		OF_TRUE		= -1,
		OF_FALSE,
		OF_PART,	
	};

	enum EENABLE_FLAGS
	{
		EF_STATIC_DISABLE,
		EF_STATIC_ENABLE, 
		EF_DISABLE, 
		EF_ENABLE, 		
	};

	char	szText[100];		//Text shown in TreeCtrl, '\t' means one Indent, or child item.
	char	szName[50];			//ID of a Option
	int		nTrueValue;			//True value (>=0)

	union
	{
		int nTImgIndex;		//True Image in the TreeCtrl
		int nImgIndex;			//Image
	};
	union
	{
		int nFImgIndex;		//False Image in the TreeCtrl
		int nSelImgIndex;	//Selected Image
	};

	int nPImgIndex;			// if node has children, some children is true and some children is false

	int		nCritical;				//Critical Level
	EENABLE_FLAGS	eEnabled;	//Enable or not
};

class COptions
{
private:
	CWnd* m_pOwnerWnd; 
	
public:
	COptions(CWnd* pOwnerWnd = NULL);
	virtual ~COptions();
	
	void SetOwnerWnd(CWnd *p) { m_pOwnerWnd=p; }
public:	
	CMap<CString, LPCSTR, int, int&>			m_mapOpts;			// subifox - 실제 정보 저장	
	CList<TOPTION_ITEM*, TOPTION_ITEM*&>		m_lstItems;			// subifox - COptionTreeCtrl에서 사용할 Item Map	
	CMap<CString, LPCSTR, int, int&>			m_mapBackups;		// subifox - backup용
	
	CImageList m_Img;												// subifox - Image(BMP Res) 관리
	CMap<UINT, UINT, int, int&>					m_mapImgs;			// subifox - Image(BMP Res) ResID와 ImageIndex의 상관관계를 저장

public:
	int operator[](CString);
	int operator[](char*);

	void* SetOptionItem(CString strText, UINT nImgOnResID, UINT nImgOffResID, UINT nImgPartResID=0);
	void* SetOptionItem(CString strName, CString strText, int nTrueVal, UINT nImgOnResID, UINT nImgOffResID, UINT nImgPartResID=0, UINT nCri = 0, TOPTION_ITEM::EENABLE_FLAGS e = TOPTION_ITEM::EF_ENABLE);
	
	int  AddImage(UINT nResID);

	BOOL GetOption(CString strKey, int val);
	BOOL SetOption(CString key, int val);
	

	int GetImageIndex(UINT img);
	int GetOption(CString key);

	void RegSave(CString section);
	void RegLoad(CString section);

	void BeginTrans(void);
	void CommitTrans(void);
	void RollbackTrans(void);

	void SetStaticEnable(void* p);
	void SetStaticDisable(void * p);
	void ClearStatic(void *p);

	void InformToChangeOpt(char *szKey, TOPTION_ITEM::EOPTION_FLAGS e); 


	void SetOptions(CString strOptions)
	{
		CString strKey, strTemp;

		int			nVal;
		int			pos = 0;
		while(1) {
			strKey = strOptions.Tokenize(_T(";"),pos);
			if(pos == -1) break;
			nVal = atoi(strOptions.Tokenize(_T(";"),pos));
			if(pos == -1) break;
			m_mapOpts.SetAt(strKey, nVal);
		}
	}

	CString GetOptions(void)
	{
		CString strKey, strTemp, strOptions=_T("");
		int			nVal;
		POSITION pos = m_mapOpts.GetStartPosition();
		while(pos) {
			m_mapOpts.GetNextAssoc(pos, strKey, nVal);
			strTemp.Format(_T(";%d;"),nVal);
			strOptions += strKey + strTemp;
		}
		return strOptions;
	}
};

#endif