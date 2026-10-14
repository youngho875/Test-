#pragma once
#include "MapBuildOptions.h"
#include "cpl_minixml.h"
#include <atlconv.h>
#include <string>
#pragma pack(push,8)
struct BaseMapConfiguration
{
    CPLXMLNode* xml=nullptr;
    std::string file;

    BaseMapConfiguration()
    {
        WCHAR exe[32768]={0};
        GetModuleFileNameW(nullptr,exe,32768);
        CStringW path(exe);
        path=path.Left(path.ReverseFind(L'\\')+1)+L"base-map.xml";
        file=CW2A(path,CP_UTF8);
        if(GetFileAttributesW(path)!=INVALID_FILE_ATTRIBUTES)xml=CPLParseXMLFile(file.c_str());
        if(xml && !CPLGetXMLNode(xml,"=BaseMap"))
        {
            CPLDestroyXMLNode(xml);
            xml=nullptr;
        }
        if(!xml)
        {
            xml=CPLParseXMLString("<BaseMap version=\"1\"><SmallScale><Path>D:/map/small_scale_map/small_map.shp</Path><LabelPath>D:/map/small_scale_map/name_map.shp</LabelPath><visual>true</visual></SmallScale><World><Path>D:/map/world_map/Continents.shp</Path><BoundaryPath>D:/map/world_map/national_boundary_polygon.shp</BoundaryPath><LabelPath>D:/map/world_map/placenam.shp</LabelPath><CityPath>D:/map/world_map/World_city_pt.shp</CityPath><visual>true</visual></World><NaturalEarth><Path>D:/webcop/NaturalEarthII</Path><visual>true</visual></NaturalEarth><Names><visual>true</visual></Names><ScaleStatus>true</ScaleStatus><ScaleBar>true</ScaleBar><Compass>true</Compass></BaseMap>");
            if(GetFileAttributesW(path)==INVALID_FILE_ATTRIBUTES)
                Save();
        }
    }
    ~BaseMapConfiguration(){CPLDestroyXMLNode(xml);}
    std::string Value(const char* key,const char* fallback="")const
    {
        return CPLGetXMLValue(CPLGetXMLNode(xml,"=BaseMap"),key,fallback);
    }
    bool Flag(const char* key)const
    {
        auto value=Value(key,"true");
        return value=="true"||value=="1"||value=="TRUE";
    }
    void Save()
    {
        if(!CPLSerializeXMLTreeToFile(xml,file.c_str()))
            OutputDebugString(_T("C4IMap: base-map.xml save failed\n"));
    }
};
#pragma pack(pop)
inline BaseMapConfiguration& BaseMapConfig()
{
    static BaseMapConfiguration config;
    return config;
}
inline bool BaseMapVisible(int index)
{
    static const char* keys[]={"SmallScale.visual","World.visual","NaturalEarth.visual","Names.visual"};
    return C4MAP_ENABLE_LOCAL&&index>=0&&index<4&&BaseMapConfig().Flag(keys[index]);
}
inline void SetBaseMapVisible(int index,bool visible)
{
    static const char* keys[]={"SmallScale.visual","World.visual","NaturalEarth.visual","Names.visual"};
    if(index>=0&&index<4)
    {
        CPLSetXMLValue(CPLGetXMLNode(BaseMapConfig().xml,"=BaseMap"),keys[index],visible?"true":"false");
        BaseMapConfig().Save();
    }
}
inline CString BaseMapPath(const char* key,const char* fallback)
{
    auto path=BaseMapConfig().Value(key,fallback);
    CA2W wide(path.c_str(),CP_UTF8);
    return CString(CW2T(wide,CP_ACP));
}
