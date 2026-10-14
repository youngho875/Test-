#include "pch.h"
#include "MapBuildOptions.h"
#include "BaseMapOptions.h"
//#include "C4IMap.h"
#include "ShapeT.h"
#include "GPoint.h"
#include "GeoDC/CoordMgr.h"
#include "GeoDC/Coord.h"
#include "gdal_priv.h"
#include "ogr_feature.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include <string>
#include <iostream>

#include <afxdlgs.h>
#include <afxdialogex.h>
#include <afxcmn.h>
#include <atlimage.h>
#include <atlconv.h>
#include <memory>
#include <functional>
#include <algorithm>
#include <set>
#include <cmath>
#include <sstream>
#include "cpl_minixml.h"
#include "resource.h"
#include "MapMakeStandardRules.h"

namespace {
// CA2T/CT2A are identity conversions in an MBCS build. Convert via UTF-16
// explicitly so GDAL UTF-8 and the MFC UI code page are never confused.
CString LocalText(const char* utf8) {
    CA2W wide(utf8 ? utf8 : "", CP_UTF8);
    return CString(CW2T(wide, CP_ACP));
}
std::string LocalUtf8(const CString& text) {
    CT2W wide(text, CP_ACP);
    return std::string(CW2A(wide, CP_UTF8));
}
std::string LocalKey(std::string s) {
    std::replace(s.begin(), s.end(), '\\', '/');
    auto p=s.find_last_of('/');if(p!=s.npos)s=s.substr(p+1);
    p=s.find_last_of('.');if(p!=s.npos)s=s.substr(0,p);
    for(auto& c:s)if(c>='A'&&c<='Z')c+=32;
    // MapX Source names append the map series/scale; SHP basenames do not.
    // Strip only known trailing suffixes, preserving underscores in real names.
    for(const char* suffix:{"_tmk","_mok","_jop","_tsm","_tsp",
        "_air250k","_air500k","_air1m","_air2m",
        "_navykr1","_navykr2","_navykr3","_navykr4","_navykr5"}){
        const size_t length=strlen(suffix);
        if(s.size()>length&&s.compare(s.size()-length,length,suffix)==0){s.resize(s.size()-length);break;}
    }
    return s;
}
const CPLXMLNode* LocalChild(const CPLXMLNode* n,const char* key) {
    if(!n)return nullptr;for(auto c=n->psChild;c;c=c->psNext)if(c->eType==CXT_Element&&strcmp(c->pszValue,key)==0)return c;return nullptr;
}
std::string LocalValue(const CPLXMLNode* n,const char* key,const char* fallback="") {
    if(!n)return fallback;if(!key){for(auto c=n->psChild;c;c=c->psNext)if(c->eType==CXT_Text)return c->pszValue;return fallback;}const char* v=CPLGetXMLValue(n,key,nullptr);return v?v:fallback;
}
bool LocalYes(std::string v){
    v.erase(std::remove_if(v.begin(),v.end(),[](unsigned char c){return isspace(c)!=0;}),v.end());
    for(auto& c:v)c=(char)tolower((unsigned char)c);return v=="true"||v=="1";
}
bool LocalXmlVisible(const CPLXMLNode* layer){
    for(const char* tag:{"visual","visible"})for(auto c=layer->psChild;c;c=c->psNext)
        if(c->eType==CXT_Element&&_stricmp(c->pszValue,tag)==0)return LocalYes(LocalValue(c,nullptr));
    return true;
}
double LocalNumber(const std::string& v,double fallback=0){try{size_t end=0;double n=std::stod(v,&end);return end==v.size()&&std::isfinite(n)?n:fallback;}catch(...){return fallback;}}
COLORREF LocalColor(std::string s,COLORREF fallback){if(!s.empty()&&s[0]=='#')s.erase(0,1);if(s.size()!=6||s.find_first_not_of("0123456789abcdefABCDEF")!=s.npos)return fallback;unsigned long c=strtoul(s.c_str(),nullptr,16);return RGB((c>>16)&255,(c>>8)&255,c&255);}
struct LocalXmlDelete{void operator()(CPLXMLNode* n)const{if(n)CPLDestroyXMLNode(n);}};
using LocalXml=std::unique_ptr<CPLXMLNode,LocalXmlDelete>;
LocalXml LocalReadXml(const CString& path){
    CFile file;if(!file.Open(path,CFile::modeRead|CFile::shareDenyWrite))throw std::runtime_error("Cannot open style XML");
    if(file.GetLength()>32*1024*1024)throw std::runtime_error("Style XML exceeds 32 MB");std::string bytes((size_t)file.GetLength(),'\0');if(!bytes.empty())file.Read(&bytes[0],(UINT)bytes.size());
    std::string head=bytes.substr(0,200);for(auto& c:head)c=(char)tolower((unsigned char)c);
    if(head.find("euc-kr")!=head.npos||head.find("ks_c_5601")!=head.npos||head.find("cp949")!=head.npos){CStringW wide(CA2W(bytes.c_str(),949));bytes=std::string(CW2A(wide,CP_UTF8));}
    LocalXml xml(CPLParseXMLString(bytes.c_str()));if(!xml)throw std::runtime_error("Invalid style XML");return xml;
}
// Registered by mapMake; legacy styles remain the fallback for unregistered scales.
bool LocalRegisteredStyle(CString name,CString& xml,CString& gss,const CString& catalogPath=_T("D:/map/styles/mapmake-style-catalog.xml")){
    name.Replace(_T("_unix.xml"),_T(".xml"));
    try{auto doc=LocalReadXml(catalogPath);auto root=doc.get();while(root&&(root->eType!=CXT_Element||strcmp(root->pszValue,"MapMakeStyleCatalog")!=0))root=root->psNext;
        if(!root)return false;for(auto n=root->psChild;n;n=n->psNext)if(n->eType==CXT_Element&&strcmp(n->pszValue,"Entry")==0&&LocalText(LocalValue(n,"file").c_str()).CompareNoCase(name)==0){
            CString path=LocalText(LocalValue(n,"xml").c_str());DWORD a=GetFileAttributes(path);if(a==INVALID_FILE_ATTRIBUTES||(a&FILE_ATTRIBUTE_DIRECTORY))return false;
            xml=path;CString assets=LocalText(LocalValue(n,"gss").c_str());a=GetFileAttributes(assets);if(a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY))gss=assets;return true;}
    }catch(...){ }return false;
}
bool LocalEvaluateCondition(const CPLXMLNode* n,OGRFeature* feature,std::string& error){
    if(!n)return true;std::string tag=n->pszValue;
    if(tag=="ogc:Filter"||tag=="ogc:And"||tag=="ogc:Or"){
        bool found=false,all=true,any=false;for(auto c=n->psChild;c;c=c->psNext)if(c->eType==CXT_Element){found=true;bool value=LocalEvaluateCondition(c,feature,error);all=all&&value;any=any||value;}return found&&(tag=="ogc:Or"?any:all);
    }
    auto field=LocalValue(LocalChild(n,"ogc:PropertyName"),nullptr);auto literal=LocalValue(LocalChild(n,"ogc:Literal"),nullptr);int index=feature->GetFieldIndex(field.c_str());
    if(index<0){error="Missing standard condition DBF field: "+field;return false;}if(!feature->IsFieldSetAndNotNull(index))return false;
    int comparison=0;auto type=feature->GetFieldDefnRef(index)->GetType();
    if(type==OFTInteger||type==OFTInteger64||type==OFTReal){double expected=LocalNumber(literal,NAN),actual=feature->GetFieldAsDouble(index);if(!std::isfinite(expected)||!std::isfinite(actual))return false;comparison=actual<expected?-1:actual>expected?1:0;}
    else comparison=std::string(feature->GetFieldAsString(index)).compare(literal);
    if(tag=="ogc:PropertyIsEqualTo")return comparison==0;if(tag=="ogc:PropertyIsNotEqualTo")return comparison!=0;if(tag=="ogc:PropertyIsGreaterThan")return comparison>0;if(tag=="ogc:PropertyIsGreaterThanOrEqualTo")return comparison>=0;if(tag=="ogc:PropertyIsLessThan")return comparison<0;if(tag=="ogc:PropertyIsLessThanOrEqualTo")return comparison<=0;
    error="Unsupported standard predicate: "+tag;return false;
}
void LocalFiles(const CString& dir,const CString& suffix,std::vector<CString>& result){
    CFileFind find;BOOL more=find.FindFile(dir+_T("\\*"));while(more){more=find.FindNextFile();if(find.IsDots())continue;
        if(find.IsDirectory()){if(!(GetFileAttributes(find.GetFilePath())&FILE_ATTRIBUTE_REPARSE_POINT))LocalFiles(find.GetFilePath(),suffix,result);}
        else {auto name=find.GetFileName();if(name.GetLength()>=suffix.GetLength()&&name.Right(suffix.GetLength()).CompareNoCase(suffix)==0)result.push_back(find.GetFilePath());}
    }
}
struct LocalDataset {
    CString path;int kind=0;bool indexed=false;double west=-180,south=-90,east=180,north=90;std::string key;GDALDataset* dataset=nullptr;OGRLayer* layer=nullptr;OGRCoordinateTransformation* forward=nullptr;OGRCoordinateTransformation* inverse=nullptr;bool visible=true;
    void Close(){if(dataset)GDALClose(dataset);dataset=nullptr;layer=nullptr;}
    ~LocalDataset(){if(forward)OCTDestroyCoordinateTransformation(forward);if(inverse)OCTDestroyCoordinateTransformation(inverse);if(dataset)GDALClose(dataset);}
};
struct LocalRule {std::string source,alias,category,display,geometry,label,labelField,table,field;double minScale=0,maxScale=0;bool visible=true;};
struct LocalHit {size_t dataset;GIntBig fid;std::vector<POINT> points;std::vector<int> counts;int kind;CString layer;};
struct LocalPaint {COLORREF color=RGB(80,175,230),outline=RGB(70,90,110);int width=1,size=7;bool fill=true;std::string icon,font="Arial",text;int fontSize=10;std::vector<DWORD> dash;double dx=0,dy=0;};
}
struct LocalShapeState {
    CString folder,xmlPath,gssPath,diagnostics;int fallbackEpsg=4326;bool visible=true;size_t selectedDataset=(size_t)-1;GIntBig selectedFid=-1;
    std::vector<std::shared_ptr<LocalDataset>> datasets;std::vector<LocalHit> hits;LocalXml xml,gssXml;
    std::map<const CPLXMLNode*,LocalXml> conditions;std::set<const CPLXMLNode*> disabledConditions;
    std::map<std::string,const CPLXMLNode*> styles,tables,gssStyles;std::vector<LocalRule> rules;
    std::map<std::string,CString> images;std::map<std::string,std::shared_ptr<CImage>> bitmaps;std::set<std::string> warnings;
    std::map<std::pair<std::string,int>,LocalPaint> paintCache;
    double scale=0;bool standard=false;
    void Warn(const std::string& message){if(warnings.insert(message).second){diagnostics+=LocalText(message.c_str())+_T("\r\n");OutputDebugString(LocalText(message.c_str())+_T("\n"));}}
};


struct LocalScaleEntry {
    CString family,title,folder,xml,gss;double denominator=0,displayCeiling=0;bool available=false,attempted=false;
    std::vector<LocalDisplayLayer> layers;std::unique_ptr<CShapeT> renderer;
};
struct LocalScaleCatalog {
    CString root;bool visible=true;std::vector<LocalScaleEntry> entries;std::vector<size_t> active;
};
namespace {
using LocalTransform=std::shared_ptr<OGRCoordinateTransformation>;
bool LocalIndexDataset(LocalDataset& ds,int fallback,std::map<std::string,LocalTransform>& transforms){
    CFile file;if(!file.Open(ds.path,CFile::modeRead|CFile::shareDenyNone))return false;
    unsigned char header[100];if(file.Read(header,100)!=100)return false;
    int shapeType=0;memcpy(&shapeType,header+32,4);shapeType%=10;ds.kind=shapeType==5?2:shapeType==3?1:0;
    double box[4];memcpy(box,header+36,32);for(double v:box)if(!std::isfinite(v))return false;
    CString prj=ds.path.Left(ds.path.ReverseFind(_T('.')))+_T(".prj");CFile projection;std::string definition;
    if(projection.Open(prj,CFile::modeRead|CFile::shareDenyNone)&&projection.GetLength()<65536){definition.resize((size_t)projection.GetLength());if(!definition.empty())projection.Read(&definition[0],(UINT)definition.size());}
    if(definition.empty())definition="EPSG:"+std::to_string(fallback);
    auto found=transforms.find(definition);
    if(found==transforms.end()){
        OGRSpatialReference source,target;target.SetWellKnownGeogCS("WGS84");
        LocalTransform transform;
        if(source.SetFromUserInput(definition.c_str())==OGRERR_NONE){
#if GDAL_VERSION_NUM >= 3000000
            source.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);target.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif
            transform=LocalTransform(OGRCreateCoordinateTransformation(&source,&target),[](OGRCoordinateTransformation* p){if(p)OCTDestroyCoordinateTransformation(p);});
        }
        found=transforms.emplace(definition,transform).first;
    }
    if(!found->second)return false;
    double x[9],y[9];int n=0;for(int row=0;row<3;++row)for(int col=0;col<3;++col){x[n]=box[0]+(box[2]-box[0])*col/2;y[n]=box[1]+(box[3]-box[1])*row/2;++n;}
    if(!found->second->Transform(9,x,y))return false;
    ds.west=*std::min_element(x,x+9);ds.east=*std::max_element(x,x+9);ds.south=*std::min_element(y,y+9);ds.north=*std::max_element(y,y+9);
    // Expand for curved projected edges and numeric rounding. Unknown extents never get culled.
    double pad=std::max(0.01,(ds.east-ds.west+ds.north-ds.south)*0.02);
    ds.west-=pad;ds.east+=pad;ds.south-=pad;ds.north+=pad;return true;
}
bool LocalOpenDataset(LocalShapeState& state,LocalDataset& ds){
    if(ds.dataset)return true;
    ds.dataset=(GDALDataset*)GDALOpenEx(LocalUtf8(ds.path).c_str(),GDAL_OF_VECTOR|GDAL_OF_READONLY,nullptr,nullptr,nullptr);
    if(!ds.dataset||!(ds.layer=ds.dataset->GetLayer(0))){ds.Close();state.Warn("Cannot open SHP: "+LocalUtf8(ds.path));return false;}
    if(ds.forward&&ds.inverse)return true;
    OGRSpatialReference source,target;target.SetWellKnownGeogCS("WGS84");
    if(ds.layer->GetSpatialRef())source=*ds.layer->GetSpatialRef();
    else if(source.importFromEPSG(state.fallbackEpsg)!=OGRERR_NONE){ds.Close();return false;}
#if GDAL_VERSION_NUM >= 3000000
    source.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);target.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
#endif
    if(ds.forward)OCTDestroyCoordinateTransformation(ds.forward);if(ds.inverse)OCTDestroyCoordinateTransformation(ds.inverse);
    ds.forward=OGRCreateCoordinateTransformation(&source,&target);ds.inverse=OGRCreateCoordinateTransformation(&target,&source);
    if(!ds.forward||!ds.inverse){ds.Close();state.Warn("Cannot transform CRS: "+LocalUtf8(ds.path));return false;}return true;
}
size_t LocalChooseScale(const LocalScaleCatalog& catalog,const CString& family,double denominator){
    size_t chosen=(size_t)-1,fallback=(size_t)-1;double ceiling=1e100,largest=0;
    for(size_t i=0;i<catalog.entries.size();++i){const auto& e=catalog.entries[i];if(e.family!=family||!e.available||e.denominator<=0)continue;
        double upper=e.displayCeiling>0?e.displayCeiling:e.denominator;
        if(upper>largest){largest=upper;fallback=i;}
        if(denominator<=upper&&upper<ceiling){ceiling=upper;chosen=i;}
    }
    return chosen==(size_t)-1?fallback:chosen;
}
}

using namespace std;

#include "BaseMapDetails.inc"

CShapeT::CShapeT()
{
#if GDAL_VERSION_NUM >= 3000000
    // Use the SDK's matching PROJ database; do not inherit an unrelated PostGIS DB.
    TCHAR executable[MAX_PATH]={0};
	GetModuleFileName(nullptr,executable,MAX_PATH);
	CString base(executable);base=base.Left(base.ReverseFind('\\')+1);
    for(const auto& candidate:std::vector<CString>{_T("D:/map/styles/proj"),
		base+_T("../../gdal_sdk/bin/proj9/share"),base+_T("proj9/share"),_T("D:/work/상황도/gdal_sdk/bin/proj9/share")})
	{
        if(GetFileAttributes(candidate+_T("/proj.db"))!=INVALID_FILE_ATTRIBUTES)
		{
			auto utf=LocalUtf8(candidate);
			const char* paths[]={utf.c_str(),nullptr};
			OSRSetPROJSearchPaths(paths);
			break;
		}
    }
#endif
	OGRRegisterAll();
#if C4MAP_ENABLE_LOCAL
	SetBase();
#endif

}

CShapeT::~CShapeT()
{
	PointLayer.clear();
	MultipointLayer.clear();
	LineLayer.clear();
	PolygonLayer.clear();

	m_local.reset();
    for(auto& entry:mmap_ShpInfo)
		if(entry.second.IsData&&entry.second.poDS)
			GDALClose(entry.second.poDS);
}

static int id = 1;
void CShapeT::SetBase()
{
	CString filename = "";
	GDALDataset* poDS = NULL;
	S_SHP_INFO tmp;

	filename = BaseMapPath("SmallScale.Path","D:/map/small_scale_map/small_map.shp");  // D:/mapdata/worldmap/world.shp
	//filename = "D:/mapdata/worldmap/world.shp";
	poDS = static_cast<GDALDataset*>(GDALOpenEx(filename, GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL));
	if (poDS != NULL)
	{
		tmp.poDS = poDS;
		tmp.IsData = true;
		tmp.GeoType = "miniMap";
	}
	else tmp.IsData = false;
	mmap_ShpInfo[id] = tmp;  ++id;

	//filename = "D:/mapdata/worldmap/world.shp";
	//poDS = static_cast<GDALDataset*>(GDALOpenEx(filename, GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL));
	//if (poDS != NULL)
	//{
	//	tmp.poDS = poDS;
	//	tmp.IsData = true;
	//	tmp.GeoType = "MultiPolygon";
	//}
	//else tmp.IsData = false; 
	//mmap_ShpInfo[id] = tmp;  ++id;

	filename = BaseMapPath("SmallScale.LabelPath","D:/map/small_scale_map/name_map.shp");
	poDS = static_cast<GDALDataset*>(GDALOpenEx(filename, GDAL_OF_VECTOR | GDAL_OF_READONLY, NULL, NULL, NULL));
	if (poDS != NULL)
	{
		tmp.poDS = poDS;
		tmp.IsData = true;
		tmp.GeoType = "point";
	}
	else tmp.IsData = false;
	mmap_ShpInfo[id] = tmp;  ++id;

}

void CShapeT::OpenShapeFile()
{
	GDALDataset* poDS;
	OGRErr error;

	itr_ShpInfo itr = mmap_ShpInfo.begin();
	for (; itr != mmap_ShpInfo.end(); ++itr)
	{
		if (itr->second.IsData == false) continue;

		poDS = itr->second.poDS;
		int cnt_layer = poDS->GetLayerCount();

		OGRLayer* poLayer;
		poLayer = poDS->GetLayer(cnt_layer - 1);
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
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
					OGRPoint* poPoint = poGeometry->toPoint();
#else
					OGRPoint* poPoint = (OGRPoint*)poGeometry;
#endif
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
			OGRFeature* poFeature = nullptr;
			MultipointFeature MultiPoint;
			for (int i = 0; i < NumberOfFeatures; i++)
			{
				poFeature = poLayer->GetNextFeature();
				OGRGeometry* poGeometry;
				poGeometry = poFeature->GetGeometryRef();
				if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPoint)
				{
					OGRMultiPoint* poMultipoint = poGeometry->toMultiPoint();
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
			OGRFeature* poFeature = nullptr;
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
	}
}

void CShapeT::shapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale)
{
	CCoordMgr* coord_mgr = GP_COORDMGR;
	CCoord coord;
	coord.initialize_adaptation_data();

	//CPen pen, * pPen;
	//pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	//pPen = pDC->SelectObject(&pen);
	//pDC->SelectStockObject(NULL_BRUSH);

	itr_ShpInfo itr = mmap_ShpInfo.begin();
	for (; itr != mmap_ShpInfo.end(); ++itr)
	{
		if(itr->second.IsData == false) continue;

		if(false && itr->second.GeoType == "point")
		{
			CPen pen, * pPen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 128));
			pPen = pDC->SelectObject(&pen);
			pDC->SelectStockObject(NULL_BRUSH);

			for (int i = 0; i < PointLayer.size(); i++)
			{
				CPoint ScreenXY;

				double geo_x = PointLayer.at(i).dX;
				double geo_y = PointLayer.at(i).dY;
				coord_mgr->WorldLonLat2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

				pDC->Ellipse(ScreenXY.x - 2, ScreenXY.y - 2, ScreenXY.x + 2, ScreenXY.y + 2);
			}
			pDC->SelectObject(pPen);
		}
		
		if (BaseMapVisible(0) && itr->second.GeoType == "miniMap")
		//if(itr->second.GeoType == "MultiPolygon")
		{
			CPen pen, * pPen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			pPen = pDC->SelectObject(&pen);
			pDC->SelectStockObject(NULL_BRUSH);

			//POINT* pPoints = NULL;
			for (int i = 0; i < PolygonLayer.size(); i++)
			{
				int cnt = PolygonLayer.at(i).PolygonsOfFeature.size();

				for (int j = 0; j < cnt; j++)
				{
					//POINT* pPoints = NULL;
					int pt = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.size();
					//std::cout << pt << std::endl;

					//if (_heapchk() != _HEAPOK)
					//	DebugBreak();

					POINT* pPoints = new POINT[pt];
                    double previousLongitude = 0;
					//if (_heapchk() != _HEAPOK)
					//	DebugBreak();

					for (int k = 0; k < pt; k++)
					{
						CPoint ScreenXY;

						double geo_x = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.at(k).dX;
						double geo_y = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.at(k).dY;
						if (coord_mgr->m_bFlat && k > 0) {
                            // Follow the short edge across +/-180, not a line across the map.
                            while (geo_x - previousLongitude > 180.0) geo_x -= 360.0;
                            while (geo_x - previousLongitude < -180.0) geo_x += 360.0;
                        }
                        previousLongitude = geo_x;
                        coord_mgr->WorldLonLat2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

						pPoints[k].x = ScreenXY.x;
						pPoints[k].y = ScreenXY.y;
					}
					pDC->Polygon(pPoints, pt);

					if (pPoints != NULL)
						delete[] pPoints;

				}

				//if (pPoints != NULL)
				//	delete[] pPoints;

			}
			pDC->SelectObject(pPen);

		}
	}
	//pDC->SelectObject(pPen);

    DrawBaseDetails(pDC,fCenterX,fCenterY,fScale);
    DrawLocal(pDC,fCenterX,fCenterY,fScale);
}

void CShapeT::PolylineshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale)
{
	CCoordMgr* coord_mgr = GP_COORDMGR;
	CCoord coord;
	coord.initialize_adaptation_data();

	CPen pen, * pPen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

}

void CShapeT::PointshapeDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale)
{
	CCoordMgr* coord_mgr = GP_COORDMGR;
	CCoord coord;
	coord.initialize_adaptation_data();

	CPen pen, * pPen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	pPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

	itr_ShpInfo itr = mmap_ShpInfo.begin();
	//for( int cnt = 0;  cnt < cnt_path; ++cnt )
	for (; itr != mmap_ShpInfo.end(); ++itr)
	{
		if (itr->second.IsData == false) continue;

		for (int i = 0; i < PointLayer.size(); i++)
		{
			CPoint ScreenXY;

			double geo_x = PointLayer.at(i).dX;
			double geo_y = PointLayer.at(i).dY;
			coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
			coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

			pDC->Ellipse(ScreenXY.x - 1, ScreenXY.y - 1, ScreenXY.x + 1, ScreenXY.y + 1);
		}
	}
	pDC->SelectObject(pPen);
}

void CShapeT::IndexMapDraw(CDC* pDC, float fCenterX, float fCenterY, float fScale)
{
	CCoordMgr* coord_mgr = GP_COORDMGR;
	CCoord coord;
	coord.initialize_adaptation_data();

	CPen pen, * pPen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	pPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);

/*
	for (const auto& polygonLayer : PolygonLayer) {
		for (const auto& feature : polygonLayer.PolygonsOfFeature) {
			int ptCount = feature.Polygon.at(0).RingString.size();
			std::vector<POINT> pPoints(ptCount);  // using vector instead of raw pointer

			for (int k = 0; k < ptCount; k++) {
				CPoint ScreenXY;
				double geo_x = feature.Polygon.at(0).RingString.at(k).dX;
				double geo_y = feature.Polygon.at(0).RingString.at(k).dY;
				coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
				coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

				pPoints[k].x = ScreenXY.x;
				pPoints[k].y = ScreenXY.y;
			}
			pDC->Polygon(pPoints.data(), ptCount);  // use data() for vector
		}
	}
	pDC->SelectObject(pPen);
*/

	//int p = 0;
	//POINT* pPoints = NULL;
	for (int i = 0; i < PolygonLayer.size(); i++)
	{
		//int p = 0;
		//POINT* pPoints = NULL;
		int cnt = PolygonLayer.at(i).PolygonsOfFeature.size();
		
		for (int j = 0; j < cnt; j++)
		{
			//int pt = 0;
			//POINT* pPoints = NULL;
			int pt = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.size();
			POINT* pPoints = new POINT[pt];

			for (int k = 0; k < pt; k++)
			{
				CPoint ScreenXY;

				double geo_x = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.at(k).dX;
				double geo_y = PolygonLayer.at(i).PolygonsOfFeature.at(j).Polygon.at(0).RingString.at(k).dY;
				coord.degrees_lat_long_to_x_y(geo_y, geo_x, &geo_x, &geo_y);
				coord_mgr->WorldXY2ScreenXY(geo_x, geo_y, &ScreenXY.x, &ScreenXY.y, fCenterX, fCenterY, fScale);

				pPoints[k].x = ScreenXY.x;
				pPoints[k].y = ScreenXY.y;
			}
			pDC->Polygon(pPoints, pt);

			if (pPoints != NULL)
			{
				delete[] pPoints;
				pPoints = nullptr;
			}

		}
		//pDC->Polygon(pPoints, cnt);

		//if (pPoints != NULL)
		//	delete[] pPoints;

	}
	pDC->SelectObject(pPen);

}

void CShapeT::OpenDTEDFile(std::string filename) 
{
	//CCoordMgr* coord_mgr = GP_COORDMGR;
	//CCoord coord;
	//coord.initialize_adaptation_data();

	//CPoint point;
	//GPoint		ptWorldPoint;
	//double adfGeoTransform[6];
	//float buffEleveationValue[1];
	//int nSizeX, nSizeY;
	//double noData = -9999;

	//GDALAllRegister();

	// Open the file
	GDALDataset* dataset = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);
	if (dataset == NULL) 
	{
		std::cout << "Failed to open" << std::endl;
		exit(1);
	}

	// Get image metadata
	width = dataset->GetRasterXSize();
	height = dataset->GetRasterYSize();

	std::cout << "Image is " << width << " x " << height << " px" << std::endl;

	//double originX, originY, pixelSizeX, pixelSizeY;

	double geoTransform[6];
	if (dataset->GetGeoTransform(geoTransform) == CE_None) 
	{
		originX = geoTransform[0];
		originY = geoTransform[3];
		pixelSizeX = geoTransform[1];
		pixelSizeY = geoTransform[5];
	}
	else 
	{
		std::cout << "Failed read geotransform" << std::endl;
		exit(1);
	}

	std::cout << "Origin: " << originX << ", " << originY << " degrees" << std::endl;
	std::cout << "Pixel size: " << pixelSizeX << ", " << pixelSizeY << " degrees" << std::endl;

	// pixelSizeY is negative because the origin of the image is the north-east corner and positive
	// Y pixel coordinates go towards the south

	// Get the raster band (DEM has one raster band representing elevation)
	GDALRasterBand* elevationBand = dataset->GetRasterBand(1);

	// Create an array of width*height 32-bit floats (~400MB memory)
	//std::vector<float> data(width * height, 0.0f);
	
	data.assign(width * height, 0.0f);
	// Read the entire file into the array (you can change the options to read only a portion
	// of the file, or even scale it down if you want)

	std::cout << "Loading array..." << std::flush;
	elevationBand->RasterIO(GF_Read, 0, 0, width, height, &data[0], width, height, GDT_Float32, 0, 0);
	std::cout << "done" << std::endl;

	// Close the file
	GDALClose(dataset);

/*
	//GDALRasterBand* poBand;

	GDALDataset* poDataset;
	GDALAllRegister();
	poDataset = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);

	printf("Driver: %s/%s\n", poDataset->GetDriver()->GetDescription(), poDataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME));
	printf("Size is %dx%dx%d\n", poDataset->GetRasterXSize(), poDataset->GetRasterYSize(), poDataset->GetRasterCount());

	if (poDataset->GetProjectionRef() != NULL)
		printf("Projection is `%s'\n", poDataset->GetProjectionRef());
	if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
	{
		printf("Origin = (%.6f,%.6f)\n", adfGeoTransform[0], adfGeoTransform[3]);
		printf("Pixel Size = (%.6f,%.6f)\n", adfGeoTransform[1], adfGeoTransform[5]);
	}

	poDataset->GetGeoTransform(adfGeoTransform);
	band = poDataset->GetRasterBand(1);

	nSizeX = band->GetXSize();
	nSizeY = band->GetYSize();
	noData = band->GetNoDataValue();

	GDALInvGeoTransform(adfGeoTransform, inverseTransform);
*/


	//std::vector<float> buffElevation(nSizeY * nSizeX);
/*
	double dLong, dLat;
	//int col_x, col_y;
	//for (int y = 0; y < nSizeY; y++)
	//{
	//	for (int x = 0; x < nSizeX; x++)
	//	{
			coord_mgr->ScreenXY2WorldXY(point.x, point.y, &ptWorldPoint.x, &ptWorldPoint.y, coord_mgr->m_fCenterX, coord_mgr->m_fCenterY, coord_mgr->m_fScale);
			coord.x_y_to_degrees_lat_long(ptWorldPoint.x, ptWorldPoint.y, &dLat, &dLong);

			//col_x = (int)((dLong - adfGeoTransform[0]) / (adfGeoTransform[1] + 1));
			//col_y = (int)((adfGeoTransform[0] - dLat) / (adfGeoTransform[1] + 1));

			//poBand->RasterIO(GF_Read, 0, 0, columns, rows, buffElevation.data(), columns, rows, GDT_Int16, 0, 0); // read the cell value
	//	}
	//}

	


			int  height = valueAt(poBand, dLat, dLong);

			//std::cout <<  "고도 : %d"  << height << std::endl;
*/
	
/*

	int col_x, col_y;

	col_x = (int)((dLong - adfGeoTransform[0]) / (adfGeoTransform[1] + 1));
	col_y = (int)((adfGeoTransform[0] - dLat) / (adfGeoTransform[1] + 1));


	//col_x = (int)((geo_x - adfGeoTransform[0]) / adfGeoTransform[1]);
	//col_y = (int)((adfGeoTransform[3] - geo_y) / -adfGeoTransform[5]);



	poBand->RasterIO(GF_Read, 0, 0, columns, rows, buffElevation.data(), columns, rows, GDT_Int16, 0, 0); // read the cell value
*/

}

double CShapeT::valueAt(double pointLat, double pointLon)
{
/*
	int x = static_cast<int>(floor(inverseTransform[0] + inverseTransform[1] * lon + inverseTransform[2] * lat));
	int y = static_cast<int>(floor(inverseTransform[3] + inverseTransform[4] * lon + inverseTransform[5] * lat));

	int32_t pixelValue;
	band->RasterIO(GF_Read, x, y, 1, 1, &pixelValue, 1, 1, GDT_Int32, 0, 0);

	//band->GetColorTable()->GetColorEntry(pixelValue);
	//int bSuccess;
	//double dfOffset = GDALGetRasterOffset(band, &bSuccess);
	//double dfScale = GDALGetRasterScale(band, &bSuccess);

	//pixelValue = pixelValue * dfScale + dfOffset;
	//height( lat,  lon);
*/


	double pixelX = (pointLon - originX) / pixelSizeX;
	double pixelY = (pointLat - originY) / pixelSizeY;

	std::cout << pointLat << "," << pointLon << " maps to pixel " << pixelX << "," << pixelY << std::endl;

	if (pixelX < 0 || pixelX > width || pixelY < 0 || pixelY > height) {
		std::cout << "Point out of bounds!" << std::endl;
		exit(1);
	}

	double elev = data[int(pixelX) + int(pixelY) * width];

	//std::cout << "Elevation there is " << elev << "m" << std::endl;

	return elev;


	//return pixelValue;
}


/*
#include <../proj9/proj.h> 
#include <iostream>

void CShapeT::height(double lat, double lon)
{
	auto proj_context = proj_context_create();
	//double lat = 44;
	//double lon = -78;
	double alt = 200;

	auto lla_to_ecef_proj = proj_create(proj_context, "+proj=latlong +ellps=WGS84 +datum=WGS84 "
		"+to +proj=geocentric");

	auto coord_4 = proj_trans(lla_to_ecef_proj, PJ_DIRECTION::PJ_FWD, PJ_COORD{ lat,lon, alt, 0 });

	std::cout << coord_4.v[0] << std::endl;
	std::cout << coord_4.v[1] << std::endl;
	std::cout << coord_4.v[2] << std::endl;

	proj_destroy(lla_to_ecef_proj);
	proj_context_destroy(proj_context);

}
*/

void CShapeT::ResolveLocalStyles(const CString& folder,CString& xml,CString& gss)
{
    CString normalized=folder;normalized.Replace(_T('/'),_T('\\'));normalized.TrimRight(_T("\\"));
    CString name,walk=normalized;
    while(!walk.IsEmpty()){
        int slash=walk.ReverseFind(_T('\\'));CString part=walk.Mid(slash+1);part.MakeUpper();
        for(const auto& prefix:std::vector<std::pair<CString,CString>>{{_T("EAR_"),_T("land")},{_T("LAND_"),_T("land")},{_T("AIR_"),_T("air")},{_T("KR_"),_T("kr")}}){
            if(part.Left(prefix.first.GetLength())!=prefix.first)continue;
            CString scale=part.Mid(prefix.first.GetLength());scale.TrimRight(_T("M"));if(scale==_T("2.5")||scale==_T("2_5"))scale=_T("2_5");
            if(!scale.IsEmpty()&&scale.SpanIncluding(_T("0123456789_"))==scale)name=_T("cop_")+prefix.second+_T("_")+scale+_T("_unix.xml");
        }
        if(!name.IsEmpty()||slash<0)break;walk=walk.Left(slash);
    }
    std::vector<CString> dirs;
    if(!xml.IsEmpty()){CString parent=xml;parent.Replace(_T('/'),_T('\\'));int p=parent.ReverseFind(_T('\\'));if(p>=0)dirs.push_back(parent.Left(p));}
    dirs.push_back(_T("D:/map/styles"));dirs.push_back(normalized);
    walk=normalized;for(int i=0;i<4;++i){int p=walk.ReverseFind(_T('\\'));if(p<0)break;walk=walk.Left(p);dirs.push_back(walk+_T("\\styles"));dirs.push_back(walk);}
    if(!name.IsEmpty()&&LocalRegisteredStyle(name,xml,gss))return;
    xml.Empty();
    if(!name.IsEmpty())for(const auto& dir:dirs){CString candidate=dir+_T("\\")+name;if(GetFileAttributes(candidate)!=INVALID_FILE_ATTRIBUTES){xml=candidate;break;}}
    if(!gss.IsEmpty()&&(GetFileAttributes(gss)&FILE_ATTRIBUTE_DIRECTORY)&&GetFileAttributes(gss)!=INVALID_FILE_ATTRIBUTES)return;
    gss.Empty();for(const auto& dir:dirs){CString candidate=dir+_T("\\GSS");DWORD attr=GetFileAttributes(candidate);if(attr!=INVALID_FILE_ATTRIBUTES&&(attr&FILE_ATTRIBUTE_DIRECTORY)){gss=candidate;break;}}
}

bool CShapeT::LoadLocal(const CString& folder,const CString& requestedXml,const CString& requestedGss,int fallbackEpsg,CString& message)
{
    CString xmlPath=requestedXml,gssPath=requestedGss;
    if(xmlPath.IsEmpty()||gssPath.IsEmpty()){CString automaticXml=xmlPath,automaticGss=gssPath;ResolveLocalStyles(folder,automaticXml,automaticGss);if(xmlPath.IsEmpty())xmlPath=automaticXml;if(gssPath.IsEmpty())gssPath=automaticGss;}
    try 
	{
        auto next=std::make_unique<LocalShapeState>();
		next->folder=folder;
		next->xmlPath=xmlPath;
		next->gssPath=gssPath;
		next->fallbackEpsg=fallbackEpsg;
        if(!xmlPath.IsEmpty())
		{
            next->xml=LocalReadXml(xmlPath);
			const CPLXMLNode* root=next->xml.get();
			while(root&&(root->eType!=CXT_Element||strcmp(root->pszValue,"InSIDEMapX3")!=0))
				root=root->psNext;
            if(!root)throw std::runtime_error("Expected MapX InSIDEMapX3 XML (not SLD)");
            for(auto n=root->psChild;n;n=n->psNext)
				if(n->eType==CXT_Element)
				{
                if(strcmp(n->pszValue,"style")==0)
					next->styles[LocalValue(n,"name")]=n;
                if(strcmp(n->pszValue,"StyleTable")==0)
					next->tables[LocalValue(n,"name")]=n;
            }
            if(!LocalValue(root,"StandardMap").empty()){next->standard=true;
                CString standardGss=LocalText(LocalValue(root,"StandardGssRoot").c_str());DWORD a=GetFileAttributes(standardGss);if(requestedGss.IsEmpty()&&a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY)){gssPath=standardGss;next->gssPath=gssPath;}
                auto notes=LocalChild(root,"StandardWarnings");if(notes)for(auto n=notes->psChild;n;n=n->psNext)if(n->eType==CXT_Element)next->Warn("STANDARD: "+LocalValue(n,"Text"));
            }
            for(auto& table:next->tables)if(LocalValue(table.second,"ConditionMode")=="STANDARD")for(auto e=table.second->psChild;e;e=e->psNext)if(e->eType==CXT_Element&&strcmp(e->pszValue,"Element")==0){
                if(LocalYes(LocalValue(e,"disabled"))){next->disabledConditions.insert(e);continue;}
                try{auto filter=mm::StandardFilter(LocalValue(e,"Condition"));if(filter.empty())next->conditions[e]=LocalXml();else {LocalXml parsed(CPLParseXMLString(filter.c_str()));if(!parsed)throw std::runtime_error("Invalid predicate XML");next->conditions[e]=std::move(parsed);}}
                catch(const std::exception& ex){next->disabledConditions.insert(e);next->Warn(std::string("Disabled condition: ")+ex.what());}
            }
            auto layers=LocalChild(root,"Layers");
			if(!layers)throw std::runtime_error("XML has no Layers");
            for(auto n=layers->psChild;n;n=n->psNext)
				if(n->eType==CXT_Element&&strcmp(n->pszValue,"Layer")==0)
				{
					LocalRule r;r.source=LocalKey(LocalValue(n,"Source"));
					r.alias=LocalValue(n,"alias");r.display=LocalValue(n,"DisplayType","GEOMETRY");
					for(auto& c:r.display)
						c=(char)toupper((unsigned char)c);
                r.geometry=LocalValue(n,"GeometryStyleName");
				r.label=LocalValue(n,"LabelStyleName");
				r.labelField=LocalValue(n,"LabelColumnName");
				r.visible=LocalXmlVisible(n);r.category=LocalValue(n,"Category");
                if(LocalYes(LocalValue(n,"UseStyleTable")))
				{
					r.table=LocalValue(n,"StyleTableName");
					r.field=LocalValue(n,"StyleColumnName");
				}
                if(LocalYes(LocalValue(n,"UseScale")))
				{
					r.minScale=LocalNumber(LocalValue(n,"MinScale"));
					r.maxScale=LocalNumber(LocalValue(n,"MaxScale"));
					if(r.minScale>r.maxScale&&r.maxScale>0)
						std::swap(r.minScale,r.maxScale);
				}
                next->rules.push_back(r);
            }
        }
        if(!gssPath.IsEmpty())
		{
            std::vector<CString> images;
			LocalFiles(gssPath,_T(".bmp"),images);LocalFiles(gssPath,_T(".png"),images);
			for(auto& path:images)
			{
				auto k=LocalKey(LocalUtf8(path));
				if(!next->images.emplace(k,path).second)
					next->Warn("Duplicate GSS bitmap basename: "+k);
			}
            CString semantic=gssPath+_T("\\GSSXML\\GSS_STYLE_v1.1.xml");
			if(GetFileAttributes(semantic)==INVALID_FILE_ATTRIBUTES)semantic=gssPath+_T("\\GSS_STYLE.xml");
			if(GetFileAttributes(semantic)!=INVALID_FILE_ATTRIBUTES)
			{
				next->gssXml=LocalReadXml(semantic);
				auto root=next->gssXml.get();
				while(root&&(root->eType!=CXT_Element||root->pszValue[0]=='?'))
					root=root->psNext;
				if(root)
					for(auto n=root->psChild;n;n=n->psNext)
						if(n->eType==CXT_Element)next->gssStyles[LocalValue(n,"name")]=n;
			}
        }
        std::vector<CString> files;
		LocalFiles(folder,_T(".shp"),files);
		if(files.empty())	throw std::runtime_error("No SHP files in selected folder");
        // Dataset headers are indexed; GDAL handles are opened only for visible files.
        std::map<std::string,LocalTransform> transforms;
        for(const auto& path:files){
            auto ds=std::make_shared<LocalDataset>();ds->path=path;ds->key=LocalKey(LocalUtf8(path));
            ds->visible=ds->key!="clip"&&ds->key!="_clip";
            ds->indexed=LocalIndexDataset(*ds,fallbackEpsg,transforms);
            next->datasets.push_back(ds);
        }
        if(next->datasets.empty())throw std::runtime_error("No SHP files");
        if(next->rules.empty())std::stable_sort(next->datasets.begin(),next->datasets.end(),[](const std::shared_ptr<LocalDataset>& a,const std::shared_ptr<LocalDataset>& b){return a->kind>b->kind;});
        for(const auto& r:next->rules)
			if(r.visible&&!std::any_of(next->datasets.begin(),next->datasets.end(),[&](const std::shared_ptr<LocalDataset>& d){return d->key==r.source;}))
				next->Warn("XML source without SHP: "+r.source);
        message.Format(_T("로컬 SHP %u개 로드 완료. 도형 클릭: 속성 조회 / 우클릭: 폴더·스타일 설정"),(UINT)next->datasets.size());
        next->Warn("Local renderer: basic geometry, colors, BMP/PNG symbols/patterns, GSS dash and labels. Advanced MapX effects may differ.");
        if(xmlPath.IsEmpty())next->Warn("No matching scale XML; using basic styles. Select XML manually if needed.");
        next->Warn("XML: "+LocalUtf8(xmlPath)+" | GSS: "+LocalUtf8(gssPath));
        m_catalog.reset();m_local=std::move(next);
		return true;
    }catch(const std::exception& e)
	{
		message=LocalText(e.what());
		return false;
	}
}

void CShapeT::BeginLocalFrame(){if(m_local)m_local->hits.clear();if(m_catalog){m_catalog->active.clear();for(auto& e:m_catalog->entries)if(e.renderer)e.renderer->BeginLocalFrame();}}
bool CShapeT::LocalVisible()const{return m_catalog?m_catalog->visible:(m_local&&m_local->visible);}
void CShapeT::ToggleLocal(){if(m_catalog){m_catalog->visible=!m_catalog->visible;m_catalog->active.clear();return;}if(m_local){m_local->visible=!m_local->visible;m_local->hits.clear();}}

namespace 
{
void LocalLonLat(CCoordMgr* mgr,double cx,double cy,double scale,int x,int y,double& lon,double& lat)
{
    if(mgr->m_bFlat)
	{
		lon=127.031+(x-cx)/(47.5*scale);
		lat=37.0861+(cy-y)/(60*scale);
	}
    else 
	{
		CCoord c;
		c.x_y_to_degrees_lat_long((x-cx)/scale,(cy-y)/scale,&lat,&lon);
	}
}
std::shared_ptr<CImage> LocalImage(LocalShapeState& state,const std::string& name)
{
    if(name.empty())
		return {};
	auto found=state.bitmaps.find(name);
	if(found!=state.bitmaps.end())
		return found->second;
    CString path=LocalText(name.c_str());
	if(GetFileAttributes(path)==INVALID_FILE_ATTRIBUTES)
	{
		auto it=state.images.find(LocalKey(name));
		if(it==state.images.end())
		{
			state.Warn("Missing bitmap: "+name);state.bitmaps[name]={};
			return {};
		}
		path=it->second;
	}
    auto image=std::make_shared<CImage>();
	if(FAILED(image->Load(path)))
	{
		state.Warn("Bitmap load failed: "+name);
		state.bitmaps[name]={};
		return {};
	}
    state.bitmaps[name]=image;
	return image;
}
LocalPaint LocalStyleUncached(LocalShapeState& state,const std::string& name,int kind){
    LocalPaint p;p.fill=kind==2;
    if(name.empty())return p;auto it=state.styles.find(name);if(it==state.styles.end()){state.Warn("Missing style: "+name);return p;}auto n=it->second;auto type=LocalValue(n,"type");
    p.color=LocalColor(LocalValue(n,type=="POLYGON"?"FillColor":"Color"),p.color);p.outline=LocalColor(LocalValue(n,type=="POLYGON"?"LineColor":"OutColor"),p.color);
    p.width=(int)std::max(0.0,std::min(64.0,LocalNumber(LocalValue(n,type=="POLYGON"?"LineWidth":"Width","1"),1)));
    p.size=(int)std::max(1.0,std::min(128.0,LocalNumber(LocalValue(n,"Size","8"),8)));p.fontSize=p.size;p.font=LocalValue(n,"Font","Arial");
    p.icon=LocalValue(n,type=="POLYGON"?"PatternName":"IconName");p.fill=LocalValue(n,"FillType","0")!="1";p.dx=LocalNumber(LocalValue(n,"xmove"));p.dy=LocalNumber(LocalValue(n,"ymove"));
    auto dash=LocalValue(n,type=="POLYGON"?"LineDashArray":"DashArray");if(!dash.empty()){std::istringstream in(dash);double v;while(in>>v)if(v>0&&std::isfinite(v))p.dash.push_back((DWORD)std::min(1000.0,v));}
    if(p.dash.empty()&&type=="LINE"){
        auto code=name;auto start=code.find('['),end=code.find(']');if(start!=code.npos&&end>start)code=code.substr(start+1,end-start-1);
        auto gss=state.gssStyles.find(code);if(gss!=state.gssStyles.end()){auto dn=LocalChild(LocalChild(gss->second,"LineLayer"),"Dash");if(dn)for(auto e=dn->psChild;e;e=e->psNext)if(e->eType==CXT_Element){auto v=LocalNumber(LocalValue(e,nullptr));if(v>0)p.dash.push_back((DWORD)std::min(1000.0,v));}}
    }
    if(type=="LINE"&&LocalValue(n,"Type","0")!="0"&&p.dash.empty())state.Warn("Special line approximated as solid: "+name);
    if(type=="POLYGON"&&LocalValue(n,"FillType","0")!="0"&&LocalValue(n,"FillType")!="1"&&p.icon.empty())state.Warn("Special fill approximated as solid: "+name);
    if(type=="LABEL"&&LocalValue(n,"Type","0")!="0")state.Warn("Special label effect not applied: "+name);
    return p;
}
// Cache is owned by the loaded XML/GSS state and replaced on reload.
const LocalPaint& LocalStyle(LocalShapeState& state,const std::string& name,int kind){
    auto key=std::make_pair(name,kind);auto found=state.paintCache.find(key);
    if(found!=state.paintCache.end())return found->second;
    return state.paintCache.emplace(key,LocalStyleUncached(state,name,kind)).first->second;
}
// Screen-space LOD: endpoints stay exact, maximum error is below one pixel.
// Polygon rings and symbols intentionally do not use this simplification.
void LocalSimplifyLine(std::vector<POINT>& points,double tolerance=0.8){
    if(points.size()<4||tolerance<=0)return;
    const size_t n=points.size();std::vector<unsigned char> keep(n,0);keep[0]=keep[n-1]=1;
    std::vector<std::pair<size_t,size_t>> pending;pending.push_back({0,n-1});
    size_t work=0;const size_t budget=n*32;
    while(!pending.empty()){
        auto range=pending.back();pending.pop_back();size_t a=range.first,b=range.second,index=a;
        double ax=points[a].x,ay=points[a].y,dx=double(points[b].x)-ax,dy=double(points[b].y)-ay;
        double length=dx*dx+dy*dy,maxDistance=tolerance*tolerance;
        for(size_t i=a+1;i<b;++i){
            // Adversarial zig-zags keep their original points instead of stalling a frame.
            if(++work>budget)return;
            double px=double(points[i].x)-ax,py=double(points[i].y)-ay;
            double t=length>0?(std::max)(0.0,(std::min)(1.0,(px*dx+py*dy)/length)):0;
            double ex=px-t*dx,ey=py-t*dy,distance=ex*ex+ey*ey;
            if(distance>maxDistance){maxDistance=distance;index=i;}
        }
        if(index!=a){keep[index]=1;pending.push_back({a,index});pending.push_back({index,b});}
    }
    size_t count=0;for(size_t i=0;i<n;++i)if(keep[i])points[count++]=points[i];points.resize(count);
}
void LocalProjectLine(OGRLineString* line,std::vector<POINT>& points,CCoordMgr* mgr,double cx,double cy,double scale){
    double previous=0;for(int i=0;i<line->getNumPoints();++i){double lon=line->getX(i),lat=line->getY(i);if(!std::isfinite(lon)||!std::isfinite(lat))continue;
        if(mgr->m_bFlat&&i){while(lon-previous>180)lon-=360;while(lon-previous< -180)lon+=360;}previous=lon;
        double x,y;mgr->WorldLonLat2ScreenXY(lon,lat,&x,&y,cx,cy,scale);if(!std::isfinite(x)||!std::isfinite(y)||fabs(x)>1e8||fabs(y)>1e8)continue;
        POINT p={(LONG)x,(LONG)y};if(points.empty()||points.back().x!=p.x||points.back().y!=p.y)points.push_back(p);
    }
}
void LocalGeometry(LocalShapeState& state,CDC* dc,OGRGeometry* geometry,const LocalPaint& paint,size_t dataset,GIntBig fid,const CString& alias,CCoordMgr* mgr,double cx,double cy,double scale,const CRect& viewport){
    auto type=wkbFlatten(geometry->getGeometryType());
    if(type==wkbMultiPoint||type==wkbMultiLineString||type==wkbMultiPolygon||type==wkbGeometryCollection){auto collection=(OGRGeometryCollection*)geometry;for(int i=0;i<collection->getNumGeometries();++i)LocalGeometry(state,dc,collection->getGeometryRef(i),paint,dataset,fid,alias,mgr,cx,cy,scale,viewport);return;}
    LocalHit hit;hit.dataset=dataset;hit.fid=fid;hit.layer=alias;
    if(type==wkbPoint){auto pt=(OGRPoint*)geometry;double x,y;mgr->WorldLonLat2ScreenXY(pt->getX(),pt->getY(),&x,&y,cx,cy,scale);if(!std::isfinite(x)||!std::isfinite(y)||fabs(x)>1e8||fabs(y)>1e8)return;hit.kind=0;hit.points.push_back({(LONG)x,(LONG)y});}
    else if(type==wkbLineString){hit.kind=1;LocalProjectLine((OGRLineString*)geometry,hit.points,mgr,cx,cy,scale);LocalSimplifyLine(hit.points);if(hit.points.size()<2)return;hit.counts.push_back((int)hit.points.size());}
    else if(type==wkbPolygon){hit.kind=2;auto poly=(OGRPolygon*)geometry;for(int i=-1;i<poly->getNumInteriorRings();++i){auto ring=i<0?poly->getExteriorRing():poly->getInteriorRing(i);if(!ring)continue;std::vector<POINT> part;LocalProjectLine(ring,part,mgr,cx,cy,scale);if(part.size()<3)continue;hit.counts.push_back((int)part.size());hit.points.insert(hit.points.end(),part.begin(),part.end());}if(hit.counts.empty())return;}
    else return;
    LONG left=hit.points[0].x,right=left,top=hit.points[0].y,bottom=top;for(auto p:hit.points){left=std::min(left,p.x);right=std::max(right,p.x);top=std::min(top,p.y);bottom=std::max(bottom,p.y);}CRect bounds(left-128,top-128,right+129,bottom+129),intersect;if(!intersect.IntersectRect(bounds,viewport))return;
    int saved=dc->SaveDC();dc->SetPolyFillMode(ALTERNATE);dc->SetBkMode(TRANSPARENT);
    LOGBRUSH lb={BS_SOLID,paint.color,0};CPen pen;
    if(!paint.dash.empty()&&hit.kind==1)pen.Attach(ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_FLAT,std::max(1,paint.width),&lb,(DWORD)paint.dash.size(),paint.dash.data()));
    else pen.CreatePen(paint.width==0?PS_NULL:PS_SOLID,std::max(1,paint.width),hit.kind==2?paint.outline:paint.color);
    dc->SelectObject(&pen);CBrush brush;auto image=LocalImage(state,paint.icon);
    CString imagePath=LocalText(paint.icon.c_str());auto indexedImage=state.images.find(LocalKey(paint.icon));if(indexedImage!=state.images.end())imagePath=indexedImage->second;bool png=imagePath.Right(4).CompareNoCase(_T(".png"))==0;
    if(image&&hit.kind==2)brush.CreatePatternBrush(CBitmap::FromHandle((HBITMAP)*image));else brush.CreateSolidBrush(paint.color);
    if((paint.fill||hit.kind==0)&&!(image&&png&&hit.kind==2))dc->SelectObject(&brush);else dc->SelectStockObject(NULL_BRUSH);
    if(hit.kind==0){auto p=hit.points[0];int size=paint.size;if(image){
        // GSS bitmap symbols have their own pixel dimensions. XML Size is
        // also used by geometric marks; do not shrink a 20px icon to 3px.
        double factor=std::min(1.0,128.0/std::max(image->GetWidth(),image->GetHeight()));
        int width=std::max(1,(int)std::lround(image->GetWidth()*factor));
        int height=std::max(1,(int)std::lround(image->GetHeight()*factor));
        if(png)image->Draw(dc->GetSafeHdc(),p.x-width/2+(int)paint.dx,p.y-height/2-(int)paint.dy,width,height);
        else image->TransparentBlt(dc->GetSafeHdc(),p.x-width/2+(int)paint.dx,p.y-height/2-(int)paint.dy,width,height,RGB(255,0,255));
    }else dc->Ellipse(p.x-size/2,p.y-size/2,p.x+size/2+1,p.y+size/2+1);}
    if(hit.kind==1)dc->Polyline(hit.points.data(),(int)hit.points.size());
    if(hit.kind==2){
        if(image&&png&&paint.fill){int clip=dc->SaveDC();dc->BeginPath();dc->PolyPolygon(hit.points.data(),hit.counts.data(),(int)hit.counts.size());dc->EndPath();dc->SelectClipPath(RGN_AND);
            int w=std::max(1,image->GetWidth()),h=std::max(1,image->GetHeight());for(int y=std::max(0,(int)top);y<std::min((int)viewport.bottom,(int)bottom+1);y+=h)for(int x=std::max(0,(int)left);x<std::min((int)viewport.right,(int)right+1);x+=w)image->Draw(dc->GetSafeHdc(),x,y);dc->RestoreDC(clip);}
        dc->PolyPolygon(hit.points.data(),hit.counts.data(),(int)hit.counts.size());
    }
    if(state.selectedDataset==dataset&&state.selectedFid==fid){CPen selection(PS_SOLID,3,RGB(255,220,0));auto old=dc->SelectObject(&selection);dc->SelectStockObject(NULL_BRUSH);if(hit.kind==0)dc->Ellipse(left-8,top-8,left+9,top+9);if(hit.kind==1)dc->Polyline(hit.points.data(),(int)hit.points.size());if(hit.kind==2)dc->PolyPolygon(hit.points.data(),hit.counts.data(),(int)hit.counts.size());dc->SelectObject(old);}
    dc->RestoreDC(saved);state.hits.push_back(std::move(hit));
}
}
void CShapeT::DrawLocal(CDC* dc,float cx,float cy,float scale)
{
    if(m_catalog){
        if(!m_catalog->visible||!dc||scale<=0)return;auto mgr=GP_COORDMGR;
        double lon1,lat1,lon2,lat2;LocalLonLat(mgr,cx,cy,scale,mgr->m_nDisplayViewWidth/2,mgr->m_nDisplayViewHeight/2,lon1,lat1);LocalLonLat(mgr,cx,cy,scale,mgr->m_nDisplayViewWidth/2+100,mgr->m_nDisplayViewHeight/2,lon2,lat2);
        double dx=(lon2-lon1)*cos(lat1*3.141592653589793/180),dy=lat2-lat1;
        double denominator=sqrt(dx*dx+dy*dy)*111320/100*dc->GetDeviceCaps(LOGPIXELSX)/0.0254;
        for(const CString& family:std::vector<CString>{_T("육도"),_T("공도"),_T("해도")}){
            size_t index=LocalChooseScale(*m_catalog,family,denominator);if(index==(size_t)-1)continue;auto& entry=m_catalog->entries[index];
            if(!std::any_of(entry.layers.begin(),entry.layers.end(),[](const LocalDisplayLayer& l){return l.checked;}))continue;
            if(!entry.renderer&&!entry.attempted){entry.attempted=true;auto renderer=std::make_unique<CShapeT>();CString message;
                if(renderer->LoadLocal(entry.folder,entry.xml,entry.gss,4326,message)){
                    // Catalog visibility comes exclusively from its XML tree, including unmatched data.
                    for(const auto& layer:renderer->LocalDisplayLayers())renderer->SetLocalDisplayVisible(layer.id,false);
                    for(const auto& layer:entry.layers)renderer->SetLocalDisplayVisible(layer.id,layer.checked);
                    entry.renderer=std::move(renderer);
                }else OutputDebugString(message);
            }
            if(entry.renderer){if(std::find(m_catalog->active.begin(),m_catalog->active.end(),index)==m_catalog->active.end())m_catalog->active.push_back(index);entry.renderer->DrawLocal(dc,cx,cy,scale);}
        }
        // Evict inactive scale handles while retaining tree check state and file indexes.
        for(size_t i=0;i<m_catalog->entries.size();++i)if(std::find(m_catalog->active.begin(),m_catalog->active.end(),i)==m_catalog->active.end()){
            auto& renderer=m_catalog->entries[i].renderer;if(renderer&&renderer->m_local)for(auto& ds:renderer->m_local->datasets)ds->Close();
        }
        return;
    }

    if(!LocalVisible()||!dc||!std::isfinite(scale)||scale<=0)
		return;auto& state=*m_local;
	auto mgr=GP_COORDMGR;
    CRect viewport(0,0,mgr->m_nDisplayViewWidth,mgr->m_nDisplayViewHeight);
	if(viewport.IsRectEmpty())	return;
    double minLon=1e30,maxLon=-1e30,minLat=90,maxLat=-90;
    for(int y=0;y<=2;++y)
		for(int x=0;x<=2;++x)
		{
			double lon,lat;
			LocalLonLat(mgr,cx,cy,scale,viewport.Width()*x/2,viewport.Height()*y/2,lon,lat);
			if(!std::isfinite(lon)||!std::isfinite(lat))	
				continue;
			minLon=std::min(minLon,lon);
			maxLon=std::max(maxLon,lon);
			minLat=std::min(minLat,lat);
			maxLat=std::max(maxLat,lat);
		}

    if(maxLon< -180||minLon>180||maxLat< -90||minLat>90)
		return;
	minLon=std::max(-180.0,minLon);
	maxLon=std::min(180.0,maxLon);
	minLat=std::max(-89.9,minLat);
	maxLat=std::min(89.9,maxLat);
    double lon1,lat1,lon2,lat2;
	LocalLonLat(mgr,cx,cy,scale,viewport.Width()/2,viewport.Height()/2,lon1,lat1);
	LocalLonLat(mgr,cx,cy,scale,viewport.Width()/2+100,viewport.Height()/2,lon2,lat2);
    double dx=(lon2-lon1)*cos(lat1*3.141592653589793/180),dy=lat2-lat1;
	state.scale=sqrt(dx*dx+dy*dy)*111320/100*dc->GetDeviceCaps(LOGPIXELSX)/0.0254;
    auto drawDataset=[&](size_t index,const LocalRule* rule)
	{
        auto ds=state.datasets[index];
		if(!ds->visible)
			return;
        if(ds->indexed&&(ds->east<minLon||ds->west>maxLon||ds->north<minLat||ds->south>maxLat))
		{
			ds->Close();
			return;
		}
        if(rule&&(!rule->visible||(rule->minScale>0&&state.scale<rule->minScale)||(rule->maxScale>0&&state.scale>rule->maxScale)))
			return;
        if(!LocalOpenDataset(state,*ds))
			return;
        double xs[9],ys[9];
		int i=0;
		for(int y=0;y<3;++y)
			for(int x=0;x<3;++x)
			{
				xs[i]=minLon+(maxLon-minLon)*x/2;ys[i]=minLat+(maxLat-minLat)*y/2;
				++i;
			}
        if(ds->inverse->Transform(9,xs,ys))
		{
			double a=xs[0],b=ys[0],c=a,d=b;
			for(int j=1;j<9;++j)
			{
				a=std::min(a,xs[j]);
				b=std::min(b,ys[j]);
				c=std::max(c,xs[j]);
				d=std::max(d,ys[j]);
			}
			ds->layer->SetSpatialFilterRect(a,b,c,d);
		}
		else 
			ds->layer->SetSpatialFilter(nullptr);
        ds->layer->ResetReading();
		OGRFeature* raw;
        while((raw=ds->layer->GetNextFeature())!=nullptr)
		{
			std::unique_ptr<OGRFeature,decltype(&OGRFeature::DestroyFeature)> feature(raw,OGRFeature::DestroyFeature);
			auto original=feature->GetGeometryRef();
			if(!original||original->IsEmpty())
				continue;
            std::vector<std::string> names;
			std::string label=rule?rule->label:"",labelField=rule?rule->labelField:"";
			bool geometry=!rule||rule->display!="LABEL";
            std::vector<std::pair<std::string,std::string>> labels;
            if(rule&&!rule->table.empty()){
                auto table=state.tables.find(rule->table);if(table==state.tables.end()){state.Warn("Missing style table: "+rule->table);continue;}
                bool standard=LocalValue(table->second,"ConditionMode")=="STANDARD";int field=feature->GetFieldIndex(rule->field.c_str());
                if(!standard&&field<0){state.Warn("Missing style DBF field: "+rule->field);continue;}
                std::string value=standard?"":feature->GetFieldAsString(field);bool matched=false;
                for(auto n=table->second->psChild;n;n=n->psNext)if(n->eType==CXT_Element&&strcmp(n->pszValue,"Element")==0){
                    bool matches=false;
                    if(standard){if(state.disabledConditions.count(n))continue;auto filter=state.conditions.find(n);if(filter==state.conditions.end())continue;std::string error;matches=LocalEvaluateCondition(filter->second.get(),feature.get(),error);if(!error.empty())state.Warn(error);}
                    else {auto expected=LocalValue(n,"value");auto type=LocalValue(table->second,"type","CHARACTER");matches=value==expected;if(type!="CHARACTER"&&!value.empty()&&!expected.empty())matches=LocalNumber(value,NAN)==LocalNumber(expected,NAN);}
                    if(!matches)continue;matched=true;for(auto key:{"polygon","line","point"}){auto v=LocalValue(n,key);if(!v.empty())names.push_back(v);}
                    auto fieldOverride=LocalValue(n,"labelcolumn");labels.push_back({LocalValue(n,"label"),fieldOverride.empty()?labelField:fieldOverride});if(!standard)break;
                }
                if(!matched)continue;
            }else {names.push_back(rule?rule->geometry:"");labels.push_back({label,labelField});}
            // Only matched style rules need projected geometry.
            std::unique_ptr<OGRGeometry> geo(original->clone());
            if(geo->transform(ds->forward)!=OGRERR_NONE)continue;
            auto alias=rule?LocalText(rule->alias.c_str()):
				LocalText(ds->key.c_str());
            if(geometry)
                for(const auto& name:names){
                    if(state.standard&&!name.empty()&&!state.styles.count(name)){state.Warn("Omitted missing standard style: "+name);continue;}
                    LocalGeometry(state,dc,geo.get(),LocalStyle(state,name,geo->getDimension()),index,feature->GetFID(),alias,mgr,cx,cy,scale,viewport);
                }
            for(const auto& labelEntry:labels)if(rule&&rule->display!="GEOMETRY"&&!labelEntry.first.empty()&&!labelEntry.second.empty())
			{
                int field=feature->GetFieldIndex(labelEntry.second.c_str());
				if(field>=0&&feature->IsFieldSetAndNotNull(field))
				{
                    OGREnvelope e;geo->getEnvelope(&e);
					double x,y;mgr->WorldLonLat2ScreenXY((e.MinX+e.MaxX)/2,(e.MinY+e.MaxY)/2,&x,&y,cx,cy,scale);
                    if(x>=0&&x<viewport.right&&y>=0&&y<viewport.bottom)
					{
						auto style=LocalStyle(state,labelEntry.first,0);
						CFont font;font.CreatePointFont(std::max(10,style.fontSize*10),LocalText(style.font.c_str()));
						int saved=dc->SaveDC();dc->SelectObject(&font);dc->SetBkMode(TRANSPARENT);
						dc->SetTextColor(style.color);auto text=LocalText(feature->GetFieldAsString(field));
						dc->TextOut((int)(x+style.dx),(int)(y-style.dy),text);
						auto extent=dc->GetTextExtent(text);dc->RestoreDC(saved);
						if(!geometry)
						{
							LocalHit hit;hit.dataset=index;hit.fid=feature->GetFID();
							hit.layer=alias;hit.kind=2;LONG left=(LONG)(x+style.dx),top=(LONG)(y-style.dy);
							hit.points={{left,top},{left+extent.cx,top},{left+extent.cx,top+extent.cy},{left,top+extent.cy}};
							hit.counts={4};
							state.hits.push_back(std::move(hit));
						}
					}
                }
            }
        }
        ds->layer->SetSpatialFilter(nullptr);
    };
    std::set<size_t> covered;
    for(const auto& rule:state.rules)
		for(size_t i=0;i<state.datasets.size();++i)
			if(state.datasets[i]->key==rule.source)
				covered.insert(i);
    for(size_t i=0;i<state.datasets.size();++i)
		if(!covered.count(i))
			drawDataset(i,nullptr);
    for(const auto& rule:state.rules)
		for(size_t i=0;i<state.datasets.size();++i)
			if(state.datasets[i]->key==rule.source)
				drawDataset(i,&rule);
}
bool CShapeT::IdentifyLocal(CPoint point,CString& title,std::vector<CString>& fields,std::vector<CString>& values)
{
    if(m_catalog)
	{
		fields.clear();
		values.clear();
		if(!m_catalog->visible)
			return false;
		for(auto i=m_catalog->active.rbegin();i!=m_catalog->active.rend();++i)
		{
			auto& renderer=m_catalog->entries[*i].renderer;
			if(renderer&&renderer->IdentifyLocal(point,title,fields,values))
				return true;
		}
		return false;
	}

    fields.clear();values.clear();
	if(!LocalVisible())
		return false;auto& state=*m_local;
    for(auto it=state.hits.rbegin();it!=state.hits.rend();++it)
	{
		bool hit=false;
        if(it->kind==0)
		{
			double dx=point.x-it->points[0].x,dy=point.y-it->points[0].y;
			hit=dx*dx+dy*dy<=64;
		}
        else if(it->kind==2)
		{
			CRgn region;
			if(region.CreatePolyPolygonRgn(it->points.data(),it->counts.data(),(int)it->counts.size(),ALTERNATE))
				hit=region.PtInRegion(point);
		}
        if(!hit&&it->kind!=0)
		{
			size_t offset=0;
			for(auto count:it->counts)
			{
				for(int i=1;i<count;++i)
				{
					auto a=it->points[offset+i-1],b=it->points[offset+i];
					double dx=b.x-a.x,dy=b.y-a.y,len=dx*dx+dy*dy;double t=len?((point.x-a.x)*dx+(point.y-a.y)*dy)/len:0;
					t=std::max(0.0,std::min(1.0,t));
					double x=point.x-a.x-t*dx,y=point.y-a.y-t*dy;
					if(x*x+y*y<=36)
					{
						hit=true;
						break;
					}
				}
				offset+=count;
				if(hit)
					break;
			}
		}
        if(!hit)
			continue;
		auto ds=state.datasets[it->dataset];
		if(!LocalOpenDataset(state,*ds))
			continue;
		std::unique_ptr<OGRFeature,decltype(&OGRFeature::DestroyFeature)> feature(ds->layer->GetFeature(it->fid),OGRFeature::DestroyFeature);
		if(!feature)
			continue;
        title=_T("SHP 속성 - ")+it->layer;
		fields.push_back(_T("파일"));
		values.push_back(ds->path);
		fields.push_back(_T("FID"));
		CString fid;fid.Format(_T("%lld"),(long long)it->fid);
		values.push_back(fid);
        for(int i=0;i<feature->GetFieldCount();++i)
		{
			fields.push_back(LocalText(feature->GetFieldDefnRef(i)->GetNameRef()));
			values.push_back(feature->IsFieldSetAndNotNull(i)?LocalText(feature->GetFieldAsString(i)):_T("(NULL)"));
		}
        state.selectedDataset=it->dataset;
		state.selectedFid=it->fid;
		return true;
    }
    state.selectedDataset=(size_t)-1;
	state.selectedFid=-1;
	
	return false;
}

namespace 
{
class LocalShpDialog:public CDialogEx 
{
    CShapeT& owner_;CListCtrl list_;
    CString Value(int id){CString s;GetDlgItemText(id,s);s.Trim();return s;}
    void Refresh(){list_.DeleteAllItems();auto layers=owner_.LocalLayerList();for(size_t i=0;i<layers.size();++i){int row=list_.InsertItem((int)i,layers[i].first);list_.SetCheck(row,layers[i].second);}SetDlgItemText(IDC_LOCAL_STATUS,owner_.LocalReport());}
public:
    LocalShpDialog(CShapeT& owner,CWnd* parent):CDialogEx(IDD_LOCAL_SHP,parent),owner_(owner){}
    BOOL OnInitDialog()override{CDialogEx::OnInitDialog();list_.SubclassDlgItem(IDC_LOCAL_LAYERS,this);list_.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);list_.InsertColumn(0,_T("레이어 / 파일 (체크: 표시)"),LVCFMT_LEFT,780);CString folder,xml,gss;int epsg;owner_.LocalSettings(folder,xml,gss,epsg);SetDlgItemText(IDC_LOCAL_FOLDER,folder);SetDlgItemText(IDC_LOCAL_XML,xml);SetDlgItemText(IDC_LOCAL_GSS,gss);SetDlgItemInt(IDC_LOCAL_EPSG,epsg);Refresh();return TRUE;}
    afx_msg void Folder(){CFolderPickerDialog d(Value(IDC_LOCAL_FOLDER),0,this);if(d.DoModal()==IDOK){SetDlgItemText(IDC_LOCAL_FOLDER,d.GetPathName());CString xml=Value(IDC_LOCAL_XML),gss=Value(IDC_LOCAL_GSS);CShapeT::ResolveLocalStyles(d.GetPathName(),xml,gss);SetDlgItemText(IDC_LOCAL_XML,xml);SetDlgItemText(IDC_LOCAL_GSS,gss);}}
    afx_msg void Xml(){CFileDialog d(TRUE,_T("xml"),nullptr,OFN_FILEMUSTEXIST,_T("MapX XML (*.xml)|*.xml||"),this);if(d.DoModal()==IDOK)SetDlgItemText(IDC_LOCAL_XML,d.GetPathName());}
    afx_msg void Gss(){CFolderPickerDialog d(Value(IDC_LOCAL_GSS),0,this);if(d.DoModal()==IDOK)SetDlgItemText(IDC_LOCAL_GSS,d.GetPathName());}
    afx_msg void Load(){CWaitCursor wait;CString oldFolder,oldXml,oldGss;int oldEpsg;owner_.LocalSettings(oldFolder,oldXml,oldGss,oldEpsg);if(oldFolder!=Value(IDC_LOCAL_FOLDER)){CString xml=Value(IDC_LOCAL_XML),gss=Value(IDC_LOCAL_GSS);CShapeT::ResolveLocalStyles(Value(IDC_LOCAL_FOLDER),xml,gss);SetDlgItemText(IDC_LOCAL_XML,xml);SetDlgItemText(IDC_LOCAL_GSS,gss);}CString message;bool allScales=GetFileAttributes(Value(IDC_LOCAL_FOLDER)+_T("/EAR_100M"))!=INVALID_FILE_ATTRIBUTES;bool loaded=allScales?owner_.LoadLocalCatalog(Value(IDC_LOCAL_FOLDER),message):owner_.LoadLocal(Value(IDC_LOCAL_FOLDER),Value(IDC_LOCAL_XML),Value(IDC_LOCAL_GSS),GetDlgItemInt(IDC_LOCAL_EPSG),message);if(!loaded){AfxMessageBox(message,MB_ICONERROR);return;}CString loadedFolder,loadedXml,loadedGss;int loadedEpsg;owner_.LocalSettings(loadedFolder,loadedXml,loadedGss,loadedEpsg);SetDlgItemText(IDC_LOCAL_XML,loadedXml);SetDlgItemText(IDC_LOCAL_GSS,loadedGss);Refresh();SetDlgItemText(IDC_LOCAL_STATUS,message+_T("\r\n")+owner_.LocalReport());}
    void OnOK()override{CString folder,xml,gss;int epsg;owner_.LocalSettings(folder,xml,gss,epsg);if(folder!=Value(IDC_LOCAL_FOLDER)||xml!=Value(IDC_LOCAL_XML)||gss!=Value(IDC_LOCAL_GSS)||epsg!=(int)GetDlgItemInt(IDC_LOCAL_EPSG)){AfxMessageBox(_T("경로를 바꿨으면 먼저 불러오기를 누르세요."));return;}for(int i=0;i<list_.GetItemCount();++i)owner_.SetLocalLayerVisible(i,list_.GetCheck(i)!=FALSE);CDialogEx::OnOK();}
    DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(LocalShpDialog,CDialogEx)
 ON_BN_CLICKED(IDC_LOCAL_FOLDER_PICK,&LocalShpDialog::Folder)
 ON_BN_CLICKED(IDC_LOCAL_XML_PICK,&LocalShpDialog::Xml)
 ON_BN_CLICKED(IDC_LOCAL_GSS_PICK,&LocalShpDialog::Gss)
 ON_BN_CLICKED(IDC_LOCAL_LOAD,&LocalShpDialog::Load)
END_MESSAGE_MAP()
}
void CShapeT::LocalSettings(CString& folder,CString& xml,CString& gss,int& epsg)const
{
	if(m_catalog){folder=m_catalog->root;
	xml.Empty();
	gss=_T("D:/map/styles/GSS");
	epsg=4326;
	return;
	}
	folder=m_local?m_local->folder:_T("D:\\map\\shp\\EAR_100M");
	xml=m_local?m_local->xmlPath:_T("D:\\map\\styles\\cop_land_100_unix.xml");
	gss=m_local?m_local->gssPath:_T("D:\\map\\styles\\GSS");
	epsg=m_local?m_local->fallbackEpsg:4326;
}
std::vector<std::pair<CString,bool>> CShapeT::LocalLayerList()const
{
	std::vector<std::pair<CString,bool>> list;
	if(m_catalog)
	{
		for(const auto& layer:LocalDisplayLayers())
			list.push_back({layer.family+_T(" / ")+layer.scale+_T(" / ")+layer.name,layer.checked});
		return list;
	}
	if(m_local)
		for(auto& ds:m_local->datasets)list.push_back({ds->path,ds->visible});
	return list;
}
void CShapeT::SetLocalLayerVisible(size_t index,bool visible)
{
	if(m_catalog)
	{
		auto layers=LocalDisplayLayers();
		if(index<layers.size())
			SetLocalDisplayVisible(layers[index].id,visible);
		return;
	}
	if(m_local&&index<m_local->datasets.size())
	{
		m_local->datasets[index]->visible=visible;
		m_local->hits.clear();
	}
}
CString CShapeT::LocalReport()const
{
	if(m_catalog)
		return _T("전체 축척 모드: 육도/공도는 지도 축척에 따라 자동 전환합니다. 해도는 XML 트리만 표시합니다. 최초 상세 축척 표시에는 파일 범위 인덱싱 시간이 필요합니다.");
	return m_local?m_local->diagnostics:_T("아직 로드하지 않았습니다. 폴더와 XML/GSS를 선택하세요.");
}
void CShapeT::ConfigureLocal(CWnd* parent)
{
	LocalShpDialog d(*this,parent);
	d.DoModal();
}

std::vector<LocalDisplayLayer> CShapeT::LocalDisplayLayers()const
{
    if(m_catalog)
	{
		std::vector<LocalDisplayLayer> result;
		for(size_t i=0;i<m_catalog->entries.size();++i)
		{
			const auto& entry=m_catalog->entries[i];
			for(auto layer:entry.layers)
			{
				layer.id+=(i+1)*100000;
				layer.family=entry.family;
				layer.scale=entry.title;
				layer.available=entry.available;
				result.push_back(layer);
			}
		}
		return result;
	}
    std::vector<LocalDisplayLayer> result;
	if(!m_local)
		return result;std::set<size_t> covered;
    for(size_t r=0;r<m_local->rules.size();++r)
	{
		const auto& rule=m_local->rules[r];
		bool found=false,enabled=false;
        for(size_t d=0;d<m_local->datasets.size();++d)
			if(m_local->datasets[d]->key==rule.source)
			{
				found=true;
				enabled|=m_local->datasets[d]->visible;
				covered.insert(d);
			}
        if(found)
			result.push_back({r,rule.category.empty()?_T("미분류"):LocalText(rule.category.c_str()),LocalText((rule.alias.empty()?rule.source:rule.alias).c_str()),rule.visible&&enabled});
    }
    for(size_t d=0;d<m_local->datasets.size();++d)
		if(!covered.count(d))
			result.push_back({m_local->rules.size()+d,_T("XML 미연결"),LocalText(m_local->datasets[d]->key.c_str()),m_local->datasets[d]->visible});
    return result;
}

void CShapeT::SetLocalDisplayVisible(size_t id,bool visible)
{
    if(m_catalog)
	{
		size_t group=id/100000;
		if(!group||group>m_catalog->entries.size())
			return;
		auto& entry=m_catalog->entries[group-1];
		size_t local=id%100000;
		for(auto& layer:entry.layers)
			if(layer.id==local)
			{
				layer.checked=visible&&entry.available;
				if(entry.renderer)
					entry.renderer->SetLocalDisplayVisible(local,layer.checked);
			}
		return;
	}
    if(!m_local)	return;
    if(id<m_local->rules.size())
	{
		auto& rule=m_local->rules[id];
		rule.visible=visible;
		if(visible)
			for(auto& ds:m_local->datasets)
				if(ds->key==rule.source)
					ds->visible=true;
	}
    else 
	{
		size_t d=id-m_local->rules.size();
		if(d<m_local->datasets.size())
			m_local->datasets[d]->visible=visible;
	}
    m_local->hits.clear();
	m_local->selectedDataset=(size_t)-1;
	m_local->selectedFid=-1;
}

bool CShapeT::HasLocalCatalog()const{return m_catalog!=nullptr;}
bool CShapeT::LoadLocalCatalog(const CString& root,CString& message){
    try{
        auto catalog=std::make_unique<LocalScaleCatalog>();
		catalog->root=root;
        struct Definition 
		{
			const TCHAR* family;
			const TCHAR* folder;
			const TCHAR* title;
			const TCHAR* xml;
			double denominator;
		};

        const Definition definitions[]={
            {_T("육도"),_T("EAR_100M"),_T("1:1,000,000"),_T("cop_land_100_unix.xml"),1000000},
            {_T("육도"),_T("EAR_50M"),_T("1:500,000"),_T("cop_land_50_unix.xml"),500000},
            {_T("육도"),_T("EAR_25M"),_T("1:250,000"),_T("cop_land_25_unix.xml"),250000},
            {_T("육도"),_T("EAR_10M"),_T("1:100,000"),_T("cop_land_10_unix.xml"),100000},
            {_T("육도"),_T("EAR_5M"),_T("1:50,000"),_T("cop_land_5_unix.xml"),50000},
            {_T("육도"),_T("EAR_2_5M"),_T("1:25,000"),_T("cop_land_2_5_unix.xml"),25000},
            {_T("공도"),_T("AIR_200M"),_T("1:2,000,000"),_T("cop_air_200_unix.xml"),2000000},
            {_T("공도"),_T("AIR_100M"),_T("1:1,000,000"),_T("cop_air_100_unix.xml"),1000000},
            {_T("공도"),_T("AIR_50M"),_T("1:500,000"),_T("cop_air_50_unix.xml"),500000},
            {_T("공도"),_T("AIR_25M"),_T("1:250,000"),_T("cop_air_25_unix.xml"),250000},
            {_T("해도"),_T("KR_1"),_T("KR1 - 데이터 없음"),_T("cop_kr_1_unix.xml"),0},
            {_T("해도"),_T("KR_2"),_T("KR2 - 데이터 없음"),_T("cop_kr_2_unix.xml"),0},
            {_T("해도"),_T("KR_3"),_T("KR3 - 데이터 없음"),_T("cop_kr_3_unix.xml"),0},
            {_T("해도"),_T("KR_4"),_T("KR4 - 데이터 없음"),_T("cop_kr_4_unix.xml"),0},
            {_T("해도"),_T("KR_5"),_T("KR5 - 데이터 없음"),_T("cop_kr_5_unix.xml"),0}};

        for(const auto& def:definitions)
		{
            LocalScaleEntry entry;
			entry.family=def.family;
			entry.title=def.title;
			entry.folder=root+_T("/")+def.folder;
			entry.xml=_T("D:/map/styles/")+CString(def.xml);
			entry.gss=_T("D:/map/styles/GSS");
            LocalRegisteredStyle(CString(def.xml),entry.xml,entry.gss);
            if(CString(def.folder)==_T("EAR_2_5M")&&GetFileAttributes(entry.folder)==INVALID_FILE_ATTRIBUTES)entry.folder=root+_T("/EAR_2.5M");
			entry.denominator=def.denominator;
            DWORD attributes=GetFileAttributes(entry.folder);
			entry.available=def.denominator>0&&attributes!=INVALID_FILE_ATTRIBUTES&&(attributes&FILE_ATTRIBUTE_DIRECTORY);
            if(GetFileAttributes(entry.xml)==INVALID_FILE_ATTRIBUTES){LocalDisplayLayer missing;missing.id=0;missing.category=_T("XML 없음");missing.name=_T("mapMake에서 축척별 XML 생성 후 적용하세요");missing.checked=false;entry.layers.push_back(missing);catalog->entries.push_back(std::move(entry));continue;}
            auto document=LocalReadXml(entry.xml);
			auto xmlRoot=document.get();
			while(xmlRoot&&(xmlRoot->eType!=CXT_Element||strcmp(xmlRoot->pszValue,"InSIDEMapX3")!=0))
				xmlRoot=xmlRoot->psNext;
            auto layers=LocalChild(xmlRoot,"Layers");
			if(!layers)	throw std::runtime_error("Scale XML has no Layers");
            // Derive the source-selection ceiling from bitmap point layers.
            // Nominal SHP scale alone can switch away before its icons become visible.
            std::map<std::string,const CPLXMLNode*> definitions,tables;
            for(auto n=xmlRoot->psChild;n;n=n->psNext)
				if(n->eType==CXT_Element)
				{
					if(strcmp(n->pszValue,"style")==0)
						definitions[LocalValue(n,"name")]=n;
					if(strcmp(n->pszValue,"StyleTable")==0)
						tables[LocalValue(n,"name")]=n;}
            auto bitmapPoint=[&](const std::string& name)
			{
				auto it=definitions.find(name);
				return it!=definitions.end()&&LocalValue(it->second,"type")=="POINT"&&!LocalValue(it->second,"IconName").empty();
			};
            std::vector<double> ceilings;
            for(auto layer=layers->psChild;layer;layer=layer->psNext)
				if(layer->eType==CXT_Element&&strcmp(layer->pszValue,"Layer")==0&&LocalXmlVisible(layer)&&LocalYes(LocalValue(layer,"UseScale")))
				{
                bool symbol=bitmapPoint(LocalValue(layer,"GeometryStyleName"));
                if(LocalYes(LocalValue(layer,"UseStyleTable")))
				{
					auto table=tables.find(LocalValue(layer,"StyleTableName"));
					if(table!=tables.end())
						for(auto element=table->second->psChild;element;element=element->psNext)
							if(element->eType==CXT_Element)
								symbol|=bitmapPoint(LocalValue(element,"point"));
				}
                double upper=LocalNumber(LocalValue(layer,"MaxScale"));
				if(symbol&&upper>0)
					ceilings.push_back(upper);
            }
            if(!ceilings.empty())
			{
				std::sort(ceilings.begin(),ceilings.end());
				entry.displayCeiling=ceilings[(ceilings.size()-1)/2];
			}
            size_t id=0;
			for(auto n=layers->psChild;n;n=n->psNext)
				if(n->eType==CXT_Element&&strcmp(n->pszValue,"Layer")==0)
				{
                LocalDisplayLayer layer;
				layer.id=id++;
				layer.category=LocalText(LocalValue(n,"Category").c_str());
				layer.name=LocalText(LocalValue(n,"alias",LocalValue(n,"Source").c_str()).c_str());
				layer.checked=entry.available&&LocalXmlVisible(n);
				entry.layers.push_back(layer);
            }
            catalog->entries.push_back(std::move(entry));
        }
        m_local.reset();
		m_catalog=std::move(catalog);
		message=_T("육도/공도 자동 축척 전환, 해도 XML 트리 로드 완료");
		return true;
    }catch(const std::exception& error)
	{
		message=LocalText(error.what());
		return false;
	}
}
