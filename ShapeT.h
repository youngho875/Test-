#pragma once

// Shared UI headers may leave pack(1) active. Keep this C++ ABI identical
// in ShapeT.cpp and all callers, then restore the caller's packing.
#pragma pack(push, 8)

#include <map>
#include <memory>
#include <vector>
struct BaseMapState;
struct LocalShapeState;
struct LocalScaleCatalog;
struct LocalDisplayLayer { size_t id; CString category,name; bool checked; CString family,scale; bool available=true; };
#include <string>
#include "pointStruct.h"


class GDALDataset;
class GDALRasterBand;

typedef struct
{
	CString FullPath;
	CString FileName;
	CString GeoType;			//점, 선, 면
	CString MApType;			//전술조치선 or 비행기지지도
	GDALDataset* poDS;
	BOOL IsData;
}S_SHP_INFO;

typedef struct
{
	CString NO;
	CString CI;
	CString LayerName;
	CString Directory;
	CString GeoType;

	CString PointColor;
	CString PointSize;
	CString PointSymbol;
	CString LineColor;
	CString LineType;

	CString LineWidth;
	CString PolygonInner;
	CString PolygonInnerColor;
	CString PolygonInnerType;
}S_SHP_CONFIG;



class CShapeT
{
public:
	CShapeT();
	~CShapeT();

private:
	std::map< int, S_SHP_INFO > mmap_ShpInfo;
	typedef std::map< int, S_SHP_INFO >::iterator itr_ShpInfo;

	vector<MyPoint2D> PointLayer;
	vector<MultipointFeature> MultipointLayer;
	vector<LineFeature> LineLayer;
	vector<PolygonFeature> PolygonLayer;
	SBoundingBox sBoundingBox;

public:
	void SetBase();
	void OpenShapeFile();
	void PointshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);
	void PolylineshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);
	//void PolygonshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);
	void IndexMapDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);

	void shapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale);

	void OpenDTEDFile(std::string filename);
	double valueAt(double lat, double lon);

	//void height(double lat, double lon);

public:
    bool LoadLocal(const CString& folder,const CString& xml,const CString& gss,int fallbackEpsg,CString& message);
    void ConfigureLocal(CWnd* parent);
    bool LoadLocalCatalog(const CString& root,CString& message);
    bool HasLocalCatalog() const;
    static void ResolveLocalStyles(const CString& folder,CString& xml,CString& gss);
    std::vector<LocalDisplayLayer> LocalDisplayLayers() const;
    void SetLocalDisplayVisible(size_t id,bool visible);
    void BeginLocalFrame();
    void DrawLocal(CDC* dc,float cx,float cy,float scale);
    bool IdentifyLocal(CPoint point,CString& title,std::vector<CString>& fields,std::vector<CString>& values);
    bool LocalVisible() const;
    void ToggleLocal();
    void LocalSettings(CString& folder,CString& xml,CString& gss,int& epsg) const;
    std::vector<std::pair<CString,bool>> LocalLayerList() const;
    void SetLocalLayerVisible(size_t index,bool visible);
    CString LocalReport() const;
private:
    std::unique_ptr<BaseMapState> m_baseMap;
    void DrawBaseDetails(CDC* dc,float cx,float cy,float scale);
    std::unique_ptr<LocalShapeState> m_local;
    std::unique_ptr<LocalScaleCatalog> m_catalog;
    unsigned width, height;
	double originX, originY, pixelSizeX, pixelSizeY;
	std::vector<float> data;

};


static_assert(sizeof(LocalDisplayLayer) == (sizeof(void*) == 8 ? 56 : 28), "LocalDisplayLayer ABI mismatch");
static_assert(alignof(CShapeT) >= alignof(void*), "CShapeT ABI mismatch");
#pragma pack(pop)
