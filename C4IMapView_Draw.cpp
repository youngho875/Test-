#include "pch.h"
#include "C4IMapViewHeader.h"
#include <iostream>
#include "WorldMapView.h"
#include "BaseMapOptions.h"
#include "MapDecoration.h"



void CC4IMapView::draw_maindc_only(CPoint ptNewPoint)
{
	float fWidth = m_pCoordMgr->m_nDisplayViewWidth;
	float fHeight = m_pCoordMgr->m_nDisplayViewHeight;

	GetClientRect(&rect);
	BufferDC.FillSolidRect(rect, RGB(15, 15, 15));
	BufferDC.BitBlt(ptNewPoint.x, ptNewPoint.y, fWidth, fHeight, m_dcMainTemp, 0, 0, SRCCOPY);

	DrawMapDecoration(&BufferDC,m_pCoordMgr,get_mainfrm(),rect);
	m_dcMain->BitBlt(0, 0, fWidth, fHeight, &BufferDC, 0, 0, SRCCOPY);
}

void CC4IMapView::DrawSituation(BOOL bMapRedraw, BOOL bSymRedraw, BOOL bMCRCRedraw, CPoint pStartPoint)
{
	if (mb_moving == TRUE)
		return;

	if (!m_bDCInitialized || m_dcMainTemp == NULL || m_bIsDrawSituationInProcess)
		return;
	m_bIsDrawSituationInProcess = TRUE;

	float fWidth = (float)m_pCoordMgr->m_nDisplayViewWidth;
	float fHeight = (float)m_pCoordMgr->m_nDisplayViewHeight;
	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

    // TMS and all overlays share GeoDC's geographic display projection.
    PrepareWorldMapView(*m_pCoordMgr);

	//if (bMapRedraw)
	//{
	//	DrawMap();    //DrawOverlay(); 
	//}

	//if (m_bShape)
	//{
	//	DrawShape();
	//}

	

/*
	if (m_bDTED)		DrawDTED();
	if (bSymRedraw)	DrawSym();
	if (bMCRCRedraw)	DrawMCRC();
*/
	//if (m_pDlgOverlayControler->IsWindowVisible() == TRUE)  DrawOverlay();

	((CDC*)m_dcMainTemp)->FillSolidRect(0, 0, fWidth, fHeight, RGB(10, 10, 10));
	//COLORREF colorOldBack4Sym;
	COLORREF colorOldBack4DTED;
	COLORREF colorOldBack4Raster;
	COLORREF colorOldBack4Shp;

	// TRANSPARENT Color Setting
	//colorOldBack4Sym = m_dcSymLayer->SetBkColor(RGB(0, 0, 0));
	colorOldBack4DTED = m_dcDTEDLayer->SetBkColor(RGB(0, 0, 0));
	colorOldBack4Raster = m_dcRasterLayer->SetBkColor(RGB(0, 0, 0));
	colorOldBack4Shp = m_dcShpLayer->SetBkColor(RGB(0, 0, 0));

	CRect ClientRect;
	GetClientRect(&ClientRect);
	if (m_bRaster)
	{
		//컬러 위성영상
		//mRasterMgr.DrawTiff(m_dcMainTemp, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale /*, m_DlgBrightness.m_iBrightness, m_DlgBrightness.m_bBlur*/);////밝기

		//if (m_pCoordMgr->m_fScale >= 1 && m_bRaster)
		//	mRasterMgr.DrawCadrg(m_dcMainTemp, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale,
		//		ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);

	}


	//if (m_bsatellite)
	{

#if C4MAP_ENABLE_LOCAL
		if(BaseMapVisible(2)){m_TmsMgr.Load(ClientRect);
		m_TmsMgr.Draw(m_dcMainTemp);}
#endif
        MapSources().Draw(m_dcMainTemp,m_pCoordMgr);
	}

	//if (m_bShape)
	{
		DrawShape();
	}


	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcMapLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));

	//m_pOverlayThread->Redraw(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y);
	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcShpLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcDTEDLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcRasterLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	//::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcSymLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));


	//투명도보다 위에 전시하려 했으나 투명도에서 알파값을 조정 후 복구를 안시켜버림.
	//추후 해결 후 투명도보다 위에 전시하도록 순서 변경 필요.	
	//m_pOverlayThread->Redraw(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y);
	//m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX - pStartPoint.x;
	//m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY - pStartPoint.y;
	//m_dcMainTemp->SetCoordMgr(m_pCoordMgr);
	//DrawArmyTroop_Etc(m_dcMainTemp);


	// HJYOO : 투명도를 그린후 좌표값을 원상태로 복구한다.
	m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX + pStartPoint.x;
	m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY + pStartPoint.y;
	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);




/*
	DrawArmyTroop();
*/
//	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcMapLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));

	/*
		DrawOverlay();
	*/


	//m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX + pStartPoint.x;
	//m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY + pStartPoint.y;
	//m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	m_localOverlay.Draw(m_dcMainTemp,m_pCoordMgr);
	m_dcMain->BitBlt(0, 0, fWidth, fHeight, m_dcMainTemp, 0, 0, SRCCOPY);
    DrawMapDecoration(m_dcMain,m_pCoordMgr,get_mainfrm(),ClientRect);
	m_dcDTEDLayer->SetBkColor(colorOldBack4DTED);
	m_dcRasterLayer->SetBkColor(colorOldBack4Raster);
	m_dcShpLayer->SetBkColor(colorOldBack4Shp);
	//m_dcSymLayer->SelectObject(&colorOldBack4Sym);
	m_bIsDrawSituationInProcess = FALSE;

	/*
		m_dcShpLayer->SelectObject(&colorOldBack4Sym);
		m_dcMCRCLayer->SelectObject(&colorOldBack4MCRC);
		//m_bIsDrawSituationInProcess = FALSE;
	*/
	CPen pen, * p_pen;
	pen.CreateStockObject(WHITE_PEN);
	p_pen = m_dcMain->SelectObject(&pen);
	CRect client_rect;
	GetClientRect(&client_rect);
	int center_x = client_rect.right / 2;
	int center_y = client_rect.bottom / 2;
	m_dcMain->MoveTo(center_x - 10, center_y);
	m_dcMain->LineTo(center_x + 10, center_y);
	m_dcMain->MoveTo(center_x, center_y - 10);
	m_dcMain->LineTo(center_x, center_y + 10);
	m_dcMain->SelectObject(p_pen);
}

void CC4IMapView::DrawMap()
{
	//	Bitmap?? ???? ?????. 
	m_dcMapLayer->SetBkMode(TRANSPARENT);
	CBrush pBrush, * pOldBrush;


	pBrush.CreateSolidBrush(RGB(10, 10, 10));


	pOldBrush = m_dcMapLayer->SelectObject(&pBrush);
	m_dcMapLayer->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight, RGB(10, 10, 10));

	//	m_dcMapLayer->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, 
	//						m_pCoordMgr->m_nDisplayViewHeight, RGB(51,55,70)); 

	//m_dcMapLayer->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, 
	//					m_pCoordMgr->m_nDisplayViewHeight, RGB(0,30,125)); 



	m_dcMapLayer->Rectangle(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight);
	m_dcMapLayer->SelectObject(pOldBrush);

	/*
		if (m_bRaster)
			DrawRaster();
		//	LineObject???? ?????.

		int i = 0;
		for (i = 0; i < m_nSDLLineObjectsIndex; i++)

		{
			if (m_SDLLineObjects[i].GetMapLoaded() && m_SDLLineObjects[i].GetMapDisplay())
				m_SDLLineObjects[i].Draw(m_dcMapLayer, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY,
					m_pCoordMgr->m_fScale);
		}


		if (m_bRawsGrid)
		{
			m_RawsMgrGrid.RAWSAreaDraw(m_dcMapLayer, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY,
				m_pCoordMgr->m_fScale);
		}
		if (m_bRawsSelect) {
			m_RawsMgrGrid.RAWSStateDraw(m_dcMapLayer, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY,
				m_pCoordMgr->m_fScale);
		}
		m_dlgCaDisplay.m_dlgCaSet.Draw(m_dcMapLayer, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY,
			m_pCoordMgr->m_fScale);
	*/

}

void CC4IMapView::DrawShape()
{
    if(!C4MAP_ENABLE_LOCAL)return;
	//ReInitializeDCs();
	//m_dcShpLayer->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight, RGB(10, 0, 0));
	//m_pMapDisplayMgr->ShapeMap(m_dcShpLayer);
	//shapeDraw(m_dcMainTemp, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);
	//mSh.shapeDraw(m_dcMainTemp, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);

    get_mainfrm()->m_indexMap.BeginLocalFrame();
    DrawVisibleWorldCopies(*m_pCoordMgr, [&](double centerX) {
        get_mainfrm()->m_indexMap.shapeDraw(m_dcMainTemp, static_cast<float>(centerX),
            m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);
    });
}



/*
void CC4IMapView::DrawShape(void)
{
	ReInitializeDCs();
	m_dcShpLayer->FillSolidRect(0, 0, m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight, RGB(10,0,0)); 
	//m_pMapDisplayMgr->ShapeMap(m_dcShpLayer);
	
	if (m_bIsSmallWorldMapOpen != TRUE)
	{
		m_pMapDataMgr->Open(SM_SMALLMAP, "D:\\mapdata\\smallmap\\small_map.shp");
		//m_pMapDataMgr->smallworldOpen("D:\\mapdata\\smallmap\\small_map.shp");
		m_bIsSmallWorldMapOpen = TRUE;
		m_pMapDataMgr->worldmapShpDraw(m_dcShpLayer, m_pCoordMgr->m_fScale, SHPT_POLYGONZ);
	}

	if (CMapDataMgr::IsWorldMapOpen())	//세계 지도 도시 
	{
		m_pMapDataMgr->worldmapShpDraw(m_dcShpLayer, m_pCoordMgr->m_fScale, SHPT_POLYGONZ);
		//worldmapShpDraw(m_dcShpLayer, dZoomFactor, SHPT_ARC);
		//worldmapShpDraw(m_dcShpLayer, dZoomFactor, SHPT_POINT);
	}
}
*/

/*
#include "gdal_priv.h"

int main() {
	GDALAllRegister();  // Register all available file formats

	GDALDataset *dataset = (GDALDataset*) GDALOpen("path_to_your_dted_file.dt0", GA_ReadOnly);
	if(dataset == nullptr) {
		// Handle error
		return 1;
	}
	
	// Get the elevation band (usually the first band in a DTED file)
	GDALRasterBand *band = dataset->GetRasterBand(1);

	int width = band->GetXSize();
	int height = band->GetYSize();

	float *elevations = new float[width * height];
	band->RasterIO(GF_Read, 0, 0, width, height, elevations, width, height, GDT_Float32, 0, 0);

	// At this point, 'elevations' contains the elevation data from the DTED file.
	// Do something with the data...

	delete[] elevations;
	GDALClose(dataset);

	return 0;
}
*/

/*
bool CC4IMapView::smallworldOpen(CString _filename)
{
	// 전체경로에서 파일명 추출
	CString name = _filename.Right(_filename.GetLength() - _filename.ReverseFind('\\') - 1);

	stSmallMap* pSmallMap = NULL;
	int nShapeType;
	CWnd* pWnd = AfxGetMainWnd();
	FILE* fpSHP = fopen(name, "rb");

	if (fpSHP == NULL)
		return FALSE;

	// get shp type
	fseek(fpSHP, 32L, SEEK_SET);
	fread(&nShapeType, 4, 1, fpSHP);

	// data opening..
	switch (nShapeType)
	{
	case SHPT_POINT:
		pSmallMap = new stSmallMap;
		pSmallMap->m_pObject = new shpPoint;
		if (!((shpPoint*)pSmallMap->m_pObject)->Open(name))
		{
			delete pSmallMap->m_pObject;
			delete pSmallMap;
			fclose(fpSHP);
			return false;
		}
		break;
	case SHPT_ARC:
		pSmallMap = new stSmallMap;
		pSmallMap->m_pObject = new shpPolyLine;
		if (!((shpPolyLine*)pSmallMap->m_pObject)->Open(name))
		{
			delete pSmallMap->m_pObject;
			delete pSmallMap;
			fclose(fpSHP);
			return false;
		}
		break;
	case SHPT_POLYGON:
		pSmallMap = new stSmallMap;
		pSmallMap->m_pObject = new shpPolygon(pWnd);
		if (!((shpPolygon*)pSmallMap->m_pObject)->Open(name))
		{
			delete pSmallMap->m_pObject;
			delete pSmallMap;
			fclose(fpSHP);
			return false;
		}
		break;
	default:
	{
		fclose(fpSHP);
		return false;
	}
	}
	fclose(fpSHP);

	pSmallMap->m_enType = SM_WORLDMAP;
	pSmallMap->m_strFileName = name;

	m_smallworldmaps.AddTail(pSmallMap);

	return true;
}
*/


//#include "gdal_priv.h"
//#include "ogr_api.h"
//#include "ogrsf_frmts.h"
////#include "ogr_geometry.h"


/*
void CC4IMapView::LoadShp()
{
	OGRRegisterAll();
	GDALDataset* poDS;

	string path = "D:/mapdata/smallmap/name_map.shp";
	poDS = (GDALDataset*)GDALOpenEx(path.c_str(), GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}

	OGRFeature* poFeature;

	int cnt_layer = poDS->GetLayerCount();
	OGRLayer* poLayer = poDS->GetLayer(cnt_layer - 1);

	poLayer->ResetReading();
	while ((poFeature = poLayer->GetNextFeature()) != NULL)
	{
		OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
		int iField;

		OGRGeometry* poGeometry;

		poGeometry = poFeature->GetGeometryRef();
		if (poGeometry != NULL
			&& wkbFlatten(poGeometry->getGeometryType()) == wkbPoint)
		{
			OGRPoint* poPoint = (OGRPoint*)poGeometry;

			double geo_x = poPoint->getX();
			double geo_y = poPoint->getY();

			//std::cout << geo_x << ":" << geo_y << std::endl;

			mvecPts.push_back(GPoint(geo_x, geo_y));

			//mvecPts.push_back(GPoint(poPoint->getX(), poPoint->getY()));
		}
		else if (poGeometry != NULL
			&& wkbFlatten(poGeometry->getGeometryType()) == wkbLineString)
		{
			OGRLineString* ls = (OGRLineString*)poGeometry;

			//points.push_back(vector<glm::vec2>());
			for (int i = 0; i < ls->getNumPoints(); i++)
			{
				OGRPoint p;
				ls->getPoint(i, &p);

				CPoint	ScreenXY;
				// This function can transform a larget set of points.....
				double geo_x = p.getX();
				double geo_y = p.getY();

				mvecPts.push_back(GPoint(geo_x, geo_y));
			}
		}
		else if (poGeometry != NULL
			&& wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
		{

			OGRLineString* ls = (OGRLineString*)poGeometry->getBoundary();

			for (int i = 0; i < ls->getNumPoints(); i++)
			{
				OGRPoint p;
				ls->getPoint(i, &p);

				// This function can transform a larget set of points.....
				double geo_x = p.getX();
				double geo_y = p.getY();

				mvecPts.push_back(GPoint(geo_x, geo_y));
			}
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDS);
}


void CC4IMapView::OpenShapeFile()
{
	
	//OGRDataSource* poDataSource;
	OGRRegisterAll();

	GDALDataset* poDS;
	OGRErr error;

	string filename = "D:/mapdata/smallmap/small_map.shp";
	//poDataSource = OGRSFDriverRegistrar::Open(filename.c_str(), false);
	poDS = (GDALDataset*)GDALOpenEx(filename.c_str(), GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL);

	OGRLayer* poLayer;
	poLayer = poDS->GetLayer(0);
	OGREnvelope Envelope;
	error = poLayer->GetExtent(&Envelope, true);
	sBoundingBox.fMaxX = Envelope.MaxX;
	sBoundingBox.fMaxY = Envelope.MaxY;
	sBoundingBox.fMinX = Envelope.MinX;
	sBoundingBox.fMinY = Envelope.MinY;

	OGRwkbGeometryType LayerGeometryType = poLayer->GetGeomType();
	int NumberOfFeatures = poLayer->GetFeatureCount(true);
	poLayer->ResetReading();

	//Point Shapefile
	if (wkbFlatten(LayerGeometryType) == wkbPoint)
	{
		OGRFeature* poFeature;
		for (int i = 0; i < NumberOfFeatures; i++)
		{
			poFeature = poLayer->GetNextFeature();
			OGRGeometry* poGeometry;
			poGeometry = poFeature->GetGeometryRef();
			if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint)
			{
				OGRPoint* poPoint = (OGRPoint*)poGeometry;
				MyPoint2D pt;
				pt.dX = poPoint->getX();
				pt.dY = poPoint->getY();
				PointLayer.push_back(pt);
			}
			OGRFeature::DestroyFeature(poFeature);
		}
	}

	//Multipoint Shapefile
	if (wkbFlatten(LayerGeometryType) == wkbMultiPoint)
	{
		OGRFeature* poFeature;
		MultipointFeature MultiPoint;
		for (int i = 0; i < NumberOfFeatures; i++)
		{
			poFeature = poLayer->GetNextFeature();
			OGRGeometry* poGeometry;
			poGeometry = poFeature->GetGeometryRef();
			if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPoint)
			{
				OGRMultiPoint* poMultipoint = (OGRMultiPoint*)poGeometry;
				int NumberOfGeometries = poMultipoint->getNumGeometries();
				MultiPoint.PointsOfFeature.resize(NumberOfGeometries);
				for (int j = 0; j < NumberOfGeometries; j++)
				{
					OGRGeometry* poPointGeometry = poMultipoint->getGeometryRef(j);
					OGRPoint* poPoint = (OGRPoint*)poPointGeometry;
					MyPoint2D pt;
					pt.dX = poPoint->getX();
					pt.dY = poPoint->getY();
					MultiPoint.PointsOfFeature.at(j) = pt;
				}
				MultipointLayer.push_back(MultiPoint);
			}
			OGRFeature::DestroyFeature(poFeature);
		}
	}

	//Polyline Shapefile
	if (wkbFlatten(LayerGeometryType) == wkbLineString)
	{
		OGRFeature* poFeature;
		LineFeature Polyline;
		OGRPoint ptTemp;
		for (int i = 0; i < NumberOfFeatures; i++)
		{
			poFeature = poLayer->GetNextFeature();
			OGRGeometry* poGeometry;
			poGeometry = poFeature->GetGeometryRef();
			if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbLineString)
			{
				OGRLineString* poLineString = (OGRLineString*)poGeometry;
				Polyline.LinesOfFeature.resize(1);
				int NumberOfVertices = poLineString->getNumPoints();
				Polyline.LinesOfFeature.at(0).LineString.resize(NumberOfVertices);
				for (int k = 0; k < NumberOfVertices; k++)
				{
					poLineString->getPoint(k, &ptTemp);
					MyPoint2D pt;
					pt.dX = ptTemp.getX();
					pt.dY = ptTemp.getY();
					Polyline.LinesOfFeature.at(0).LineString.at(k) = pt;
				}
				LineLayer.push_back(Polyline);
			}
			else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiLineString)
			{
				OGRMultiLineString* poMultiLineString = (OGRMultiLineString*)poGeometry;
				int NumberOfGeometries = poMultiLineString->getNumGeometries();
				Polyline.LinesOfFeature.resize(NumberOfGeometries);
				for (int j = 0; j < NumberOfGeometries; j++)
				{
					OGRGeometry* poLineGeometry = poMultiLineString->getGeometryRef(j);
					OGRLineString* poLineString = (OGRLineString*)poLineGeometry;
					int NumberOfVertices = poLineString->getNumPoints();
					Polyline.LinesOfFeature.at(j).LineString.resize(NumberOfVertices);
					for (int k = 0; k < NumberOfVertices; k++)
					{
						poLineString->getPoint(k, &ptTemp);
						MyPoint2D pt;
						pt.dX = ptTemp.getX();
						pt.dY = ptTemp.getY();
						Polyline.LinesOfFeature.at(j).LineString.at(k) = pt;
					}
				}
				LineLayer.push_back(Polyline);
			}
			OGRFeature::DestroyFeature(poFeature);
		}
		
	}

	//Polygon Shapefile
	if (wkbFlatten(LayerGeometryType) == wkbPolygon)
	{
		OGRFeature* poFeature = nullptr;
		PolygonFeature Polygon;
		OGRPoint ptTemp;
		for (int i = 0; i < NumberOfFeatures; i++)
		{
			poFeature = poLayer->GetNextFeature();
			OGRGeometry* poGeometry;
			poGeometry = poFeature->GetGeometryRef();
			if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
			{
				OGRPolygon* poPolygon = (OGRPolygon*)poGeometry;
				Polygon.PolygonsOfFeature.resize(1);
				int NumberOfInnerRings = poPolygon->getNumInteriorRings();
				OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
				Polygon.PolygonsOfFeature.at(0).Polygon.resize(NumberOfInnerRings + 1);
				Polygon.PolygonsOfFeature.at(0).Polygon.at(0).IsClockwised = poExteriorRing->isClockwise();
				int NumberOfExteriorRingVertices = poExteriorRing->getNumPoints();
				Polygon.PolygonsOfFeature.at(0).Polygon.at(0).RingString.resize(NumberOfExteriorRingVertices);
				for (int k = 0; k < NumberOfExteriorRingVertices; k++)
				{
					poExteriorRing->getPoint(k, &ptTemp);
					MyPoint2D pt;
					pt.dX = ptTemp.getX();
					pt.dY = ptTemp.getY();
					Polygon.PolygonsOfFeature.at(0).Polygon.at(0).RingString.at(k) = pt;
				}
				for (int h = 1; h <= NumberOfInnerRings; h++)
				{
					OGRLinearRing* poInteriorRing = poPolygon->getInteriorRing(h - 1);
					Polygon.PolygonsOfFeature.at(0).Polygon.at(h).IsClockwised = poInteriorRing->isClockwise();
					int NumberOfInteriorRingVertices = poInteriorRing->getNumPoints();
					Polygon.PolygonsOfFeature.at(0).Polygon.at(h).RingString.resize(NumberOfInteriorRingVertices);
					for (int k = 0; k < NumberOfInteriorRingVertices; k++)
					{
						poInteriorRing->getPoint(k, &ptTemp);
						MyPoint2D pt;
						pt.dX = ptTemp.getX();
						pt.dY = ptTemp.getY();
						Polygon.PolygonsOfFeature.at(0).Polygon.at(h).RingString.at(k) = pt;
					}
				}
				PolygonLayer.push_back(Polygon);
			}
			else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPolygon)
			{
				OGRMultiPolygon* poMultiPolygon = (OGRMultiPolygon*)poGeometry;
				int NumberOfGeometries = poMultiPolygon->getNumGeometries();
				Polygon.PolygonsOfFeature.resize(NumberOfGeometries);
				for (int j = 0; j < NumberOfGeometries; j++)
				{
					OGRGeometry* poPolygonGeometry = poMultiPolygon->getGeometryRef(j);
					OGRPolygon* poPolygon = (OGRPolygon*)poPolygonGeometry;
					int NumberOfInnerRings = poPolygon->getNumInteriorRings();
					OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
					Polygon.PolygonsOfFeature.at(j).Polygon.resize(NumberOfInnerRings + 1);
					Polygon.PolygonsOfFeature.at(j).Polygon.at(0).IsClockwised = poExteriorRing->isClockwise();
					int NumberOfExteriorRingVertices = poExteriorRing->getNumPoints();
					Polygon.PolygonsOfFeature.at(j).Polygon.at(0).RingString.resize(NumberOfExteriorRingVertices);
					for (int k = 0; k < NumberOfExteriorRingVertices; k++)
					{
						poExteriorRing->getPoint(k, &ptTemp);
						MyPoint2D pt;
						pt.dX = ptTemp.getX();
						pt.dY = ptTemp.getY();
						Polygon.PolygonsOfFeature.at(j).Polygon.at(0).RingString.at(k) = pt;
					}
					for (int h = 1; h <= NumberOfInnerRings; h++)
					{
						OGRLinearRing* poInteriorRing = poPolygon->getInteriorRing(h - 1);
						Polygon.PolygonsOfFeature.at(j).Polygon.at(h).IsClockwised = poInteriorRing->isClockwise();
						int NumberOfInteriorRingVertices = poInteriorRing->getNumPoints();
						Polygon.PolygonsOfFeature.at(j).Polygon.at(h).RingString.resize(NumberOfInteriorRingVertices);
						for (int k = 0; k < NumberOfInteriorRingVertices; k++)
						{
							poInteriorRing->getPoint(k, &ptTemp);
							MyPoint2D pt;
							pt.dX = ptTemp.getX();
							pt.dY = ptTemp.getY();
							Polygon.PolygonsOfFeature.at(j).Polygon.at(h).RingString.at(k) = pt;
						}
					}
				}
				PolygonLayer.push_back(Polygon);
			}
		}
		OGRFeature::DestroyFeature(poFeature);
	}

	GDALClose(poDS);
}


void CC4IMapView::gdalshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale)
{
	CCoordMgr* coord_mgr = GP_COORDMGR;
	CCoord coord;
	coord.initialize_adaptation_data();

	OGRRegisterAll();
	GDALDataset* poDS;

	string path = "D:/mapdata/smallmap/small_map.shp";
	poDS = (GDALDataset*)GDALOpenEx(path.c_str(), GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL);
	if (poDS == NULL)
	{
		printf("Open failed.\n");
		exit(1);
	}



	CPen pen, * pPen;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);


	//OGRLayer* poLayer = NULL;
	int cnt_layer = poDS->GetLayerCount();
	OGRLayer* poLayer = poDS->GetLayer(cnt_layer-1);
	if (poLayer != NULL)
	{
		OGREnvelope extent;
		poLayer->GetExtent(&extent);
		OGRFeatureDefn* pFeatureDefn = poLayer->GetLayerDefn();
		OGRwkbGeometryType geometryType = wkbFlatten(pFeatureDefn->GetGeomType());


		if (wkbPoint == geometryType)
		{
			poLayer->ResetReading();
			OGRFeature* pFeature = NULL;
			while ((pFeature = poLayer->GetNextFeature()) != NULL)
			{
				OGRGeometry* pGeometry = pFeature->GetGeometryRef();
				OGRPoint* pPoint = (OGRPoint*)pGeometry;

				//OGRPoint* pPoint = pGeometry->toPoint();

				//char* pszwkt = NULL;
				//pGeometry->exportToWkt(&pszwkt);

				//std::cout << pszwkt << std::endl;

				//CPLFree(pszwkt);


				CPoint	ScreenXY;
				double  geo_x = pPoint->getX();
				double  geo_y = pPoint->getY();

				coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
				coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

				pDC->Ellipse( ScreenXY.x - 5,  ScreenXY.y - 5, ScreenXY.x + 5,  ScreenXY.y + 5 );

				CString value = pFeature->GetFieldAsString("NON");
				if (value == "") 
					value = pFeature->GetFieldAsString("TXTDESC");

				CRect DrawRect(ScreenXY.x - 13, ScreenXY.y - 13, ScreenXY.x + 60, ScreenXY.y + 10);

				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(255, 255, 255));
				if (fScale > 2000)
					pDC->DrawText(value, DrawRect, DT_CENTER | DT_SINGLELINE | DT_LEFT);
				OGRFeature::DestroyFeature(pFeature);
			}
		}
		else if (wkbLineString == geometryType)
		{
			poLayer->ResetReading();
			OGRFeature* pFeature = NULL;
			while ((pFeature = poLayer->GetNextFeature()) != NULL)
			{
				OGRGeometry* pGeometry = pFeature->GetGeometryRef();
				OGRLineString* pLine = (OGRLineString*)pGeometry;
				int cnt_pt = pLine->getNumPoints();
				for (int cnt = 0; cnt < cnt_pt - 1; ++cnt)
				{

					CPoint	ScreenXY;
					double  geo_x = pLine->getX(cnt);
					double  geo_y = pLine->getY(cnt);
					coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
					coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

					CPoint	ScreenXY2;
					double  geo_x2 = pLine->getX(cnt + 1);
					double  geo_y2 = pLine->getY(cnt + 1);
					coord.degrees_lat_long_to_x_y(geo_y2, geo_x2, &geo_x2, &geo_y2);
					coord_mgr->WorldXY2ScreenXY(geo_x2, geo_y2, &ScreenXY2.x, &ScreenXY2.y, fCenterX, fCenterY, fScale);

					pDC->MoveTo(ScreenXY.x, ScreenXY.y);
					pDC->LineTo(ScreenXY2.x, ScreenXY2.y);
				}
				OGRFeature::DestroyFeature(pFeature);
			}
		}
		else if (wkbPolygon == geometryType)
		{
			poLayer->ResetReading();
			OGRFeature* pFeature = NULL;

			while ((pFeature = poLayer->GetNextFeature()) != NULL)
			{
				OGRGeometry* pGeometry = pFeature->GetGeometryRef();
				//OGRPolygon* pPolygon = (OGRPolygon*)pGeometry;
				//OGRPoint* points = pGeometry->toPoint();

				OGRLineString* pLine = (OGRLineString*)pGeometry->getBoundary();
				
				//if (pPolygon == NULL)
				//	continue;

				//pPolygon->PointOnSurface(points);

				//OGRBoolean is_ring = pPolygon->IsRing();
				//OGRLinearRing* pRing = pPolygon->getExteriorRing();
				int cnt_pt = pLine->getNumPoints();

				POINT* pPoints = new POINT[cnt_pt];
				for (int cnt_x = 0; cnt_x < cnt_pt; ++cnt_x)
				{
					OGRPoint Point;
					pLine->getPoint(cnt_x, &Point);
					//pRing->getPoint(cnt_x, &Point);

					CPoint	ScreenXY;
					double geo_x = Point.getX();
					double geo_y = Point.getY();

					coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
					coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

					

					pPoints[cnt_x].x = ScreenXY.x;
					pPoints[cnt_x].y = ScreenXY.y;
				}
				pDC->Polygon(pPoints, cnt_pt);

				if(pPoints != NULL)
					delete[] pPoints;
				OGRFeature::DestroyFeature(pFeature);
			}
		}
		pDC->SelectObject(pPen);
	}
	//OGRDataSource::DestroyDataSource(poDS);
	GDALClose(poDS);
	//pDC->SelectObject(pPen);
}
*/