#pragma once
#include "MapBuildOptions.h"
#pragma pack(push,8)
#include <string>
#include <vector>
class CCoordMgr;
struct MapSourceLayer {
    std::string id,name,type,family,path,extensions,endpoint,layer,matrix,style,format="image/png",version="1.1.1",srs="EPSG:4326";
    double scale=0,minScale=0,maxScale=0;
    bool visual=false,recursive=true;
};
class CMapSources {
public:
    CMapSources();~CMapSources();
    bool Load(const CString& xml,CString& error);
    bool Save(const CString& xml,const std::vector<MapSourceLayer>& layers,CString& error);
    std::vector<MapSourceLayer> Layers()const;
    void SetVisible(size_t index,bool visible);
    CString Path()const; CString Status()const;
    void Configure(CWnd* parent);
    void Draw(CDC* dc,CCoordMgr* coord);
    bool Poll();
    bool WaitForIdle(unsigned milliseconds);
    void ResetCache();
private:
    struct Impl;Impl* p;
    CMapSources(const CMapSources&)=delete;
    CMapSources& operator=(const CMapSources&)=delete;
};

CMapSources& MapSources();
#pragma pack(pop)
