#pragma once

#include "./OverlayFileMgr/OverlayFileHeader.h"
#include "./OverlayFileMgr/OverlayFileRecordHeader.h"
#include "./OverlayFileMgr/OverlayFileRecordCommon.h"
#include "./OverlayFileMgr/OverlayFileRecordImage.h"
#include "./OverlayFileMgr/OverlayFileRecordText.h"
#include "LayerStructure.h"
#include "ObjectText.h"
#include "../GeoDC/Coord.h"



//#include "ICOPS_Framework/ICOPS_struct.h"
//#include "ICOPS_Framework/CTypedBuffer.h"
//#include "ICOPS_WarSym/IWarSymbol.h"
//#include "ICOPS_WarSym/IWarSymEditable.h"
//#include "CTest_OverItem.h"

using namespace OVERLAY;

class CXMLControl
{

public:
	CXMLControl();
	~CXMLControl();

public:
	CString SetOVD();
	CString SetOVI(OverlayFileRecordCommon *data, LayerStructure* pLayer);
	CString SetOVSD();
	CString SetOVG(OverlayFileRecordCommon *data);
	CString SetOVS(OverlayFileRecordCommon *data, LayerStructure *layer);
	CString SetGEO(OverlayFileRecordCommon *data);
	CString LineProperty(OverlayFileRecordCommon *data);
	CString StartLineCap(OverlayFileRecordCommon *data);
	CString EndLineCap(OverlayFileRecordCommon *data);
	CString LineDash(OverlayFileRecordCommon *data);
	CString FillProperty(OverlayFileRecordCommon *data);
	CString HatchBrush(OverlayFileRecordCommon *data);
	CString PathGradient(OverlayFileRecordCommon *data);
	CString LinerGradient(OverlayFileRecordCommon *data);
	CString TextProperty(OverlayFileRecordText *data);
	CString ExtraSymbol(OverlayFileRecordCommon *data);
	CString SetSPL(OverlayFileRecordCommon *data);
	CString SetSPT(OverlayFileRecordCommon *data);
	CString SetSRT(OverlayFileRecordCommon *data);
	CString SetSEL(OverlayFileRecordCommon *data);
	CString SetSAR(OverlayFileRecordCommon *data);
	CString SetSTS(OverlayFileRecordCommon *data);
	CString SetOVP();
	CString PropertyGroup();
	CString PropertyItem();
	CString PointType(OverlayFileRecordCommon *data);

private:
	CString OutLinePoints(OverlayFileRecordCommon *data);
	CString LinePoints(OverlayFileRecordCommon *data);
	CString MultiLinePoints(OverlayFileRecordCommon *data);
	CString CuverPoints(OverlayFileRecordCommon *data);

public:
	 //BOOL GetXMLLoad(char* filename, int nStrLen);


protected:
	//CMarkup			m_markup;

	CString strNewGuid;
	CString m_strLastSaveTM;

	CString GetGUID();
	CString GetSaveTime();

	void TransPointsData(OverlayFileRecordCommon *data);
};