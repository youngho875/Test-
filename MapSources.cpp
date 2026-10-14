#include "pch.h"
#pragma pack(push,8)
#include "MapSources.h"
#include "GeoDC/Coord.h"
#include "GeoDC/CoordMgr.h"
#include <gdal_priv.h>
#include <gdal_utils.h>
#include <gdal_alg.h>
#include <ogrsf_frmts.h>
#include <cpl_minixml.h>
#include <cpl_string.h>
#include <atlimage.h>
#include <shlobj.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <set>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <functional>
#pragma comment(lib,"msimg32.lib")

namespace 
{
std::string SU(const CString& s)
{
    CStringW w(s);
    int n=WideCharToMultiByte(CP_UTF8,0,w,-1,0,0,0,0);
    std::string r(n,0);
    WideCharToMultiByte(CP_UTF8,0,w,-1,&r[0],n,0,0);
    r.pop_back();
    return r;
}

CString ST(const std::string& s)
{
    int n=MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,0,0);
    CStringW w;
    MultiByteToWideChar(CP_UTF8,0,s.c_str(),-1,w.GetBuffer(n),n);
    w.ReleaseBuffer();
    return CString(w);
}

std::string SV(CPLXMLNode* n,const char* key,const char* def="")
{
    return CPLGetXMLValue(n,key,def);
}

std::string SX(const std::string& s)
{
    char* p=CPLEscapeString(s.c_str(),-1,CPLES_XML);
    std::string r=p?p:"";
    CPLFree(p);
    return r;
}

std::string Lower(std::string s)
{
    for(auto& c:s)
        c=(char)tolower((unsigned char)c);
    return s;
}

std::string ReadUtf8(const CString& path)
{
    CFile f(path,CFile::modeRead|CFile::shareDenyWrite);
    auto n=f.GetLength();
    if(n>8*1024*1024)
        throw std::runtime_error("Catalog too large");
    std::string s((size_t)n,0);
    if(n)
        f.Read(&s[0],(UINT)n);
    return s;
}

void AtomicWrite(const CString& path,const std::string& bytes)
{
    CString tmp=path+_T(".tmp");
    CFile f(tmp,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
    f.Write(bytes.data(),(UINT)bytes.size());
    f.Flush();
    f.Close();
    if(!MoveFileEx(tmp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))
    {
        DeleteFile(tmp);
        throw std::runtime_error("Cannot replace XML file");
    }
}

using Dataset=std::unique_ptr<GDALDataset,void(*)(GDALDataset*)>;
Dataset DS(GDALDataset* p)
{
    return Dataset(p,[](GDALDataset* d){if(d)GDALClose(d);});
}

struct Tile 
{
    double west,north,east,south;
    std::vector<unsigned char> bgra;
};

struct TileKey 
{
    int z,x,y;
    bool operator<(const TileKey& b)const
    {
        return std::tie(z,x,y)<std::tie(b.z,b.x,b.y);
    }
};

struct Request 
{
    int z=0;
    std::vector<TileKey> tiles;std::vector<size_t> sources;
    bool operator==(const Request& b)const
    {
        return z==b.z&&sources==b.sources&&tiles.size()==b.tiles.size()&& std::equal(tiles.begin(),tiles.end(),b.tiles.begin(),[](const TileKey&a,const TileKey&b){return a.z==b.z&&a.x==b.x&&a.y==b.y;});
    }
};

struct RasterFile 
{
    std::string path;
    double west=-180,south=-90,east=180,north=90;
};

bool Allowed(const MapSourceLayer& l)
{
    return (C4MAP_ENABLE_LOCAL&&(l.type=="Raster"||l.type=="CIB"))||(C4MAP_ENABLE_OGC&&(l.type=="WMS"||l.type=="WFS"||l.type=="WMTS"));
}

std::string Number(double v)
{
    std::ostringstream s;
    s.imbue(std::locale::classic());
    s<<std::setprecision(17)<<v;
    return s.str();
}

std::string Connection(const MapSourceLayer& l)
{
#if C4MAP_ENABLE_OGC
    if(l.type=="WMS")return "<GDAL_WMS><Service name=\"WMS\"><Version>"+SX(l.version)+"</Version><ServerUrl>"+SX(l.endpoint)+"</ServerUrl><SRS>EPSG:4326</SRS><CRS>EPSG:4326</CRS><BBoxOrder>"+(l.version=="1.3.0"?std::string("yxYX"):std::string("xyXY"))+"</BBoxOrder><Layers>"+SX(l.layer)+"</Layers><Styles>"+SX(l.style)+"</Styles><ImageFormat>"+SX(l.format)+"</ImageFormat><Transparent>TRUE</Transparent></Service><DataWindow><UpperLeftX>-180</UpperLeftX><UpperLeftY>90</UpperLeftY><LowerRightX>180</LowerRightX><LowerRightY>-90</LowerRightY><SizeX>268435456</SizeX><SizeY>134217728</SizeY></DataWindow><Projection>EPSG:4326</Projection><BandsCount>4</BandsCount><BlockSizeX>256</BlockSizeX><BlockSizeY>256</BlockSizeY><OverviewCount>20</OverviewCount><Timeout>8</Timeout><MaxConnections>2</MaxConnections></GDAL_WMS>";
    if(l.type=="WMTS")return "<GDAL_WMTS><GetCapabilitiesUrl>"+SX(l.endpoint)+"</GetCapabilitiesUrl><Layer>"+SX(l.layer)+"</Layer><Style>"+SX(l.style)+"</Style><TileMatrixSet>"+SX(l.matrix)+"</TileMatrixSet></GDAL_WMTS>";
#endif
    return l.path;
}
}
struct CMapSources::Impl 
{
    mutable std::mutex mutex;
    std::condition_variable cv;
    std::thread worker;
    bool stop=false,pending=false,busy=false,changed=false;
    std::atomic<unsigned> revision{0},ticket{0};
    unsigned dataRevision=0;
    unsigned activeTicket=0;std::vector<MapSourceLayer> layers;
    Request request;
    CString xml,status;
    std::vector<std::shared_ptr<Tile>> ready;
    std::map<std::tuple<size_t,int,int,int>,std::shared_ptr<Tile>> cache;
    std::map<size_t,std::vector<RasterFile>> indexes;
    std::map<size_t,Dataset> remote;
    void Note(const CString& s)
    {
        std::lock_guard<std::mutex> lock(mutex);
        status=s;
        changed=true;
    }

    Impl()
    {
        worker=std::thread([this]{Run();});
    }
    ~Impl()
    {
        {std::lock_guard<std::mutex> l(mutex);stop=true;++revision;cv.notify_all();}
        worker.join();
    }
    bool Cancel(unsigned rev)
    {
        return revision.load()!=rev||ticket.load()!=activeTicket;
    }
    void Scan(const MapSourceLayer& source,const CString& folder,std::vector<RasterFile>& result,unsigned rev,std::set<std::string>& visited)
    {
        if(Cancel(rev))
            return;
        DWORD attr=GetFileAttributes(folder);
        if(attr==INVALID_FILE_ATTRIBUTES)
            return;
        if(attr&FILE_ATTRIBUTE_DIRECTORY)
        {
            if(attr&FILE_ATTRIBUTE_REPARSE_POINT)
                return;
            CFileFind find;
            BOOL more=find.FindFile(folder+_T("/*"));
            while(more&&!Cancel(rev))
            {
                more=find.FindNextFile();
                if(find.IsDots())
                    continue;
                if(find.IsDirectory())
                {
                    if(source.recursive)
                        Scan(source,find.GetFilePath(),result,rev,visited);
                }
                else 
                    Scan(source,find.GetFilePath(),result,rev,visited);
            }
            return;
        }

        std::string path=SU(folder),ext=Lower(path.substr(path.find_last_of('.')+1));
        if(!source.extensions.empty()&&(";"+Lower(source.extensions)+";").find(";"+ext+";")==std::string::npos)
            return;
        if(!visited.insert(path).second)
            return;
        auto ds=DS((GDALDataset*)GDALOpenEx(path.c_str(),GDAL_OF_RASTER|GDAL_OF_READONLY,nullptr,nullptr,nullptr));
        if(!ds||!ds->GetRasterCount())
            return;
        double gt[6];
        if(ds->GetGeoTransform(gt)!=CE_None)
            return;
        OGRSpatialReference input,target;
        target.SetWellKnownGeogCS("WGS84");
        target.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        if(input.SetFromUserInput(ds->GetProjectionRef())!=OGRERR_NONE)
        {
            if(source.srs.empty()||input.SetFromUserInput(source.srs.c_str())!=OGRERR_NONE)
                return;
        }
        input.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        auto tr=OGRCreateCoordinateTransformation(&input,&target);
        if(!tr)
            return;
        RasterFile f;
        f.path=path;
        f.west=f.south=1e30;
        f.east=f.north=-1e30;
        for(int iy=0;iy<3;++iy)
            for(int ix=0;ix<3;++ix)
            {
                double px=ds->GetRasterXSize()*ix/2.0,py=ds->GetRasterYSize()*iy/2.0,x=gt[0]+px*gt[1]+py*gt[2],y=gt[3]+px*gt[4]+py*gt[5];
                if(tr->Transform(1,&x,&y))
                {
                    f.west=(std::min)(f.west,x);
                    f.east=(std::max)(f.east,x);
                    f.south=(std::min)(f.south,y);
                    f.north=(std::max)(f.north,y);
                }
            }

        OCTDestroyCoordinateTransformation(tr);
        if(f.west<=f.east&&f.south<=f.north)
            result.push_back(f);
    }
    static int Progress(double,const char*,void* ptr)
    {
        auto pair=(std::pair<Impl*,unsigned>*)ptr;
        return !pair->first->Cancel(pair->second);
    }

    void Blend(Tile& tile,GDALDataset* image)
    {
        int n=image->GetRasterCount();
        if(n<1)
            return;
        std::vector<unsigned char> rgba(256*256*4,255);
        auto band=image->GetRasterBand(1);
        auto ct=band->GetColorTable();
        if(n==1||n==2)
        {
            std::vector<unsigned char> grey(256*256);
            if(band->RasterIO(GF_Read,0,0,256,256,grey.data(),256,256,GDT_Byte,0,0)!=CE_None)
                return;
            for(size_t i=0;i<grey.size();++i)
            {
                auto color=ct?ct->GetColorEntry(grey[i]):nullptr;
                rgba[i*4]=color?color->c1:grey[i];
                rgba[i*4+1]=color?color->c2:grey[i];
                rgba[i*4+2]=color?color->c3:grey[i];
                rgba[i*4+3]=color?color->c4:255;
            }
            if(n==2&&image->GetRasterBand(2)->RasterIO(GF_Read,0,0,256,256,rgba.data()+3,256,256,GDT_Byte,4,256*4)!=CE_None)
                return;
        }
        else
        {
            int bands[4]={1,2,3,4};
            if(image->RasterIO(GF_Read,0,0,256,256,rgba.data(),256,256,GDT_Byte,(std::min)(4,n),bands,4,256*4,1)!=CE_None)
                return;
        }
        for(size_t i=0;i<256*256;++i)
        {
            unsigned a=rgba[i*4+3],inv=255-a;
            for(int j=0;j<3;++j)
                tile.bgra[i*4+j]=(unsigned char)((rgba[i*4+2-j]*a+tile.bgra[i*4+j]*inv)/255);
            tile.bgra[i*4+3]=(unsigned char)(a+tile.bgra[i*4+3]*inv/255);
        }
    }
    void Warp(Tile& tile,GDALDataset* input,const MapSourceLayer& source,unsigned rev)
    {
        std::vector<std::string> args={"-of","MEM","-t_srs","EPSG:4326","-te",Number(tile.west),Number(tile.south),Number(tile.east),Number(tile.north),"-ts","256","256","-r","near","-dstalpha","-wo","INIT_DEST=NO_DATA","-wm","32"};
        if(!strlen(input->GetProjectionRef())&&!source.srs.empty())
        {
            args.push_back("-s_srs");
            args.push_back(source.srs);
        }
        char** options=nullptr;
        for(const auto& a:args)
            options=CSLAddString(options,a.c_str());
        auto opts=GDALWarpAppOptionsNew(options,nullptr);
        CSLDestroy(options);
        std::pair<Impl*,unsigned> callback{this,rev};
        GDALWarpAppOptionsSetProgress(opts,Progress,&callback);
        GDALDatasetH handle=input;
        int usage=0;auto warped=DS((GDALDataset*)GDALWarp("",nullptr,1,&handle,opts,&usage));
        GDALWarpAppOptionsFree(opts);
        if(warped&&!Cancel(rev))Blend(tile,warped.get());
        else if(!warped)
            Note(_T("타일 좌표 변환 실패: ")+ST(CPLGetLastErrorMsg()));
    }
    void Wfs(Tile& tile,const MapSourceLayer& source,unsigned rev)
    {
#if C4MAP_ENABLE_OGC
        auto ds=DS((GDALDataset*)GDALOpenEx(("WFS:"+source.endpoint).c_str(),GDAL_OF_VECTOR|GDAL_OF_READONLY,nullptr,nullptr,nullptr));
        if(!ds)
        {
            Note(_T("WFS 연결 실패: 서버/레이어 설정을 확인하세요."));
            return;
        }
        auto layer=ds->GetLayerByName(source.layer.c_str());
        if(!layer)
        {
            Note(_T("WFS 레이어를 찾을 수 없습니다."));
            return;
        }
        OGRSpatialReference wgs;
        wgs.SetWellKnownGeogCS("WGS84");
        wgs.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        auto sr=layer->GetSpatialRef();
        if(!sr)
        {
            Note(_T("WFS 좌표계가 없습니다."));
            return;
        }
        auto native=std::unique_ptr<OGRSpatialReference>(sr->Clone());
        native->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        auto forward=OGRCreateCoordinateTransformation(native.get(),&wgs);
        auto inverse=OGRCreateCoordinateTransformation(&wgs,native.get());
        if(!forward||!inverse)
        {
            if(forward)
                OCTDestroyCoordinateTransformation(forward);
            if(inverse)
                OCTDestroyCoordinateTransformation(inverse);
            return;
        }
        double xs[4]={tile.west,tile.east,tile.east,tile.west},ys[4]={tile.south,tile.south,tile.north,tile.north};
        if(!inverse->Transform(4,xs,ys))
        {
            OCTDestroyCoordinateTransformation(forward);
            OCTDestroyCoordinateTransformation(inverse);
            return;
        }
        layer->SetSpatialFilterRect(*std::min_element(xs,xs+4),*std::min_element(ys,ys+4),*std::max_element(xs,xs+4),*std::max_element(ys,ys+4));
        auto image=DS(GetGDALDriverManager()->GetDriverByName("MEM")->Create("",256,256,4,GDT_Byte,nullptr));
        double gt[6]={tile.west,(tile.east-tile.west)/256,0,tile.north,0,(tile.south-tile.north)/256};
        image->SetGeoTransform(gt);
        image->SetProjection(SRS_WKT_WGS84_LAT_LONG);
        for(int i=1;i<=4;++i)
            image->GetRasterBand(i)->Fill(0);
        int bands[4]={1,2,3,4};
        double burn[4]={40,190,255,255};
        char* opts[]={(char*)"ALL_TOUCHED=TRUE",nullptr};
        int count=0;
        layer->ResetReading();
        while(!Cancel(rev))
        {
            auto feature=std::unique_ptr<OGRFeature,decltype(&OGRFeature::DestroyFeature)>(layer->GetNextFeature(),OGRFeature::DestroyFeature);
            if(!feature)
                break;
            if(++count>10000)
            {
                Note(_T("WFS 타일 객체 수 10,000개 제한: 확대해서 확인하세요."));
                break;
            }
            auto g=feature->GetGeometryRef();
            if(!g)
                continue;
            auto copy=std::unique_ptr<OGRGeometry>(g->clone());
            if(copy->transform(forward)!=OGRERR_NONE)
                continue;
            OGRGeometryH h=copy.get();
            GDALRasterizeGeometries(image.get(),4,bands,1,&h,nullptr,nullptr,burn,opts,nullptr,nullptr);
        }
        OCTDestroyCoordinateTransformation(forward);
        OCTDestroyCoordinateTransformation(inverse);
        if(!Cancel(rev))
            Blend(tile,image.get());
#endif
    }
    std::shared_ptr<Tile> Render(size_t id,const MapSourceLayer& source,TileKey key,unsigned rev)
    {
        auto tile=std::make_shared<Tile>();
        double step=180.0/(1<<key.z);
        tile->west=-180+key.x*step;
        tile->east=tile->west+step;
        tile->north=90-key.y*step;
        tile->south=tile->north-step;
        tile->bgra.assign(256*256*4,0);
        if(source.type=="WFS")
        {
            Wfs(*tile,source,rev);
            return tile;
        }
        if(source.type=="WMS"||source.type=="WMTS")
        {
#if C4MAP_ENABLE_OGC
            if(!remote.count(id))
            {
                auto ds=DS((GDALDataset*)GDALOpenEx(Connection(source).c_str(),GDAL_OF_RASTER|GDAL_OF_READONLY,nullptr,nullptr,nullptr));
                if(!ds)
                {
                    Note(_T("지도 서비스 연결 실패: 주소/레이어/행렬 설정을 확인하세요."));
                    return tile;
                }
                remote.emplace(id,std::move(ds));
            }
            Warp(*tile,remote.at(id).get(),source,rev);
#endif
            return tile;
        }
#if C4MAP_ENABLE_LOCAL
        if(!indexes.count(id))
        {
            Note(ST(source.name)+_T(": 하위 폴더 지도 인덱스 작성 중"));
            std::vector<RasterFile> files;
            std::set<std::string> visited;
            Scan(source,ST(source.path),files,rev,visited);
            if(Cancel(rev))
                return tile;
            indexes[id]=std::move(files);
            if(indexes[id].empty())
                Note(ST(source.name)+_T(": 읽을 수 있는 지리참조 영상이 없습니다."));
        }
        for(const auto& file:indexes[id])
        {
            if(Cancel(rev))
                break;
            if(file.east<tile->west||file.west>tile->east||file.north<tile->south||file.south>tile->north)
                continue;
            auto ds=DS((GDALDataset*)GDALOpenEx(file.path.c_str(),GDAL_OF_RASTER|GDAL_OF_READONLY,nullptr,nullptr,nullptr));
            if(ds)
                Warp(*tile,ds.get(),source,rev);
        }
#endif
        return tile;
    }
    void Run()
    {
        CPLSetThreadLocalConfigOption("GDAL_HTTP_TIMEOUT","8");
        CPLSetThreadLocalConfigOption("GDAL_HTTP_CONNECTTIMEOUT","5");
        CPLSetThreadLocalConfigOption("GDAL_HTTP_MAX_RETRY","0");
        CPLSetThreadLocalConfigOption("OGR_WFS_PAGING_ALLOWED","ON");
        CPLSetThreadLocalConfigOption("OGR_WFS_PAGE_SIZE","1000");
        CPLPushErrorHandler(CPLQuietErrorHandler);
        unsigned generation=~0u;
        for(;;)
        {
            Request r;std::vector<MapSourceLayer> sources;
            unsigned rev,dataRev;
            {
                std::unique_lock<std::mutex> l(mutex);
                cv.wait(l,[&]{return stop||pending;});
                if(stop)
                    break;
                pending=false;busy=true;
                r=request;
                sources=layers;
                rev=revision;
                dataRev=dataRevision;
                activeTicket=ticket;
            }
            if(dataRev!=generation)
            {
                cache.clear();
                indexes.clear();
                remote.clear();
                generation=dataRev;
            }
            std::vector<std::shared_ptr<Tile>> result;
            for(auto id:r.sources)
            {
                if(id>=sources.size())
                    continue;
                for(auto key:r.tiles)
                {
                    if(Cancel(rev)) break;
                    auto k=std::make_tuple(id,key.z,key.x,key.y);
                    auto found=cache.find(k);
                    std::shared_ptr<Tile> tile=found==cache.end()?Render(id,sources[id],key,rev):found->second;
                    if(Cancel(rev)) break;
                    if(cache.size()>=256)
                        cache.erase(cache.begin());
                    cache[k]=tile;
                    result.push_back(tile);
                }
                if(Cancel(rev)) break;
            }
            {
                std::lock_guard<std::mutex> l(mutex);
                if(!Cancel(rev)&&request==r)
                {
                    ready.swap(result);
                    changed=true;
                }
                busy=false;
                cv.notify_all();
            }
        }
        remote.clear();CPLPopErrorHandler();
    }
};

CMapSources& MapSources(){static CMapSources source;return source;}
CMapSources::CMapSources():p(new Impl){GDALAllRegister();}
CMapSources::~CMapSources(){delete p;}

std::vector<MapSourceLayer> CMapSources::Layers()const
{
    std::lock_guard<std::mutex> l(p->mutex);
    return p->layers;
}

CString CMapSources::Path()const
{
    std::lock_guard<std::mutex> l(p->mutex);
    return p->xml;
}

CString CMapSources::Status()const
{
    std::lock_guard<std::mutex> l(p->mutex);
    return p->status;
}

void CMapSources::SetVisible(size_t i,bool visible)
{
    std::lock_guard<std::mutex> l(p->mutex);
    if(i<p->layers.size()&&p->layers[i].visual!=visible)
    {
        p->layers[i].visual=visible;
        ++p->revision;
        p->ready.clear();
        p->request=Request();
        p->changed=true;
    }
}

void CMapSources::ResetCache()
{
    std::lock_guard<std::mutex> l(p->mutex);
    ++p->revision;
    ++p->dataRevision;
    p->request=Request();
    p->ready.clear();
    p->changed=true;
}

bool CMapSources::Poll()
{
    std::lock_guard<std::mutex> l(p->mutex);
    bool v=p->changed;p->changed=false;
    return v;
}

bool CMapSources::WaitForIdle(unsigned ms)
{
    std::unique_lock<std::mutex> l(p->mutex);
    return p->cv.wait_for(l,std::chrono::milliseconds(ms),[&]{return !p->busy&&!p->pending;});
}

bool CMapSources::Load(const CString& xml,CString& error)
{
    try{
        auto bytes=ReadUtf8(xml);
        std::unique_ptr<CPLXMLNode,decltype(&CPLDestroyXMLNode)> doc(CPLParseXMLString(bytes.c_str()),CPLDestroyXMLNode);
        CPLXMLNode* root=nullptr;
        for(auto n=doc.get();n;n=n->psNext)
            if(n->eType==CXT_Element&&!strcmp(n->pszValue,"MapSources"))
                root=n;
        if(!root||SV(root,"version")!="1")
            throw std::runtime_error("MapSources version=1 XML required");
        std::set<std::string> ids;std::vector<MapSourceLayer> layers;
        for(auto n=root->psChild;n;n=n->psNext)
        {
            if(n->eType!=CXT_Element)
                continue;
            if(strcmp(n->pszValue,"Layer"))
                throw std::runtime_error("Unknown catalog element");
            MapSourceLayer s;
            s.id=SV(n,"id");
            s.name=SV(n,"name");
            s.type=SV(n,"type");
            s.family=SV(n,"family");
            s.path=SV(n,"Path");
            s.extensions=Lower(SV(n,"Extensions","tif;tiff;img;vrt;tl2;tc2;i42;i21;i22;i41;ntf;nitf"));
            s.endpoint=SV(n,"Endpoint");
            s.layer=SV(n,"LayerName");
            s.matrix=SV(n,"TileMatrixSet");
            s.style=SV(n,"Style");
            s.format=SV(n,"Format","image/png");
            s.version=SV(n,"Version","1.1.1");
            s.srs=SV(n,"SRS","");
            auto number=[&](const char* key)
            {
                std::string v=SV(n,key,"0");
                std::istringstream in(v);
                in.imbue(std::locale::classic());
                double d;
                if(!(in>>d)||in.peek()!=EOF||!std::isfinite(d)||d<0)
                    throw std::runtime_error("Invalid scale");
                return d;
            };
            s.scale=number("Scale");
            s.minScale=number("MinScale");
            s.maxScale=number("MaxScale");
            if(s.maxScale&&s.maxScale<s.minScale)
                throw std::runtime_error("Scale range reversed");
            auto boolean=[&](const char* key,const char* def)
            {
                auto v=Lower(SV(n,key,def));
                if(v!="true"&&v!="false")
                    throw std::runtime_error("Expected true or false");
                return v=="true";
            };
            s.visual=boolean("visual","false");
            s.recursive=boolean("Recursive","true");
            if(s.id.empty()||s.name.empty()||!ids.insert(s.id).second)
                throw std::runtime_error("Layer IDs and names must be nonempty; IDs must be unique");
            if(s.type!="Raster"&&s.type!="CIB"&&s.type!="WMS"&&s.type!="WFS"&&s.type!="WMTS")
                throw std::runtime_error("Unknown layer type");
            if((s.type=="Raster"||s.type=="CIB")&&s.path.empty())
                throw std::runtime_error("Local layer Path required");
            if(s.type=="Raster"||s.type=="CIB")
            {
                if(s.path.find(':')==std::string::npos&&s.path.substr(0,2)!="\\\\")
                {
                    auto base=xml.Left(xml.ReverseFind('\\')+1);
                    if(base.IsEmpty())
                        base=xml.Left(xml.ReverseFind('/')+1);
                    s.path=SU(base)+s.path;
                }
            }
            else if(s.visual&&(s.endpoint.empty()||s.layer.empty()))
                throw std::runtime_error("Enabled service needs Endpoint and LayerName");
            if(!s.endpoint.empty()&&s.endpoint.rfind("http://",0)!=0&&s.endpoint.rfind("https://",0)!=0)
                throw std::runtime_error("Service endpoint must be HTTP(S)");
            layers.push_back(s);
            if(layers.size()>512)
                throw std::runtime_error("Too many layers");
        }
        std::lock_guard<std::mutex> l(p->mutex);
        ++p->revision;
        ++p->dataRevision;
        p->layers.swap(layers);
        p->xml=xml;
        p->request=Request();
        p->ready.clear();
        p->status=_T("XML 지도 정의를 불러왔습니다.");
        p->changed=true;
        return true;
    }catch(CFileException* e)
    {
        TCHAR text[512];
        e->GetErrorMessage(text,512);
        error=text;
        e->Delete();
        return false;
    }
    catch(const std::exception& e)
    {
        error=ST(e.what());
        return false;
    }
}

bool CMapSources::Save(const CString& xml,const std::vector<MapSourceLayer>& layers,CString& error)
{
    try
    {
        std::string s="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<MapSources version=\"1\">\n";
        for(const auto& l:layers)
        {
            s+="  <Layer id=\""+SX(l.id)+"\" name=\""+SX(l.name)+"\" type=\""+SX(l.type)+"\" family=\""+SX(l.family)+"\">\n";
            auto add=[&](const char* n,const std::string& v)
            {
                s+="    <"+std::string(n)+">"+SX(v)+"</"+n+">\n";
            };
            add("visual",l.visual?"true":"false");
            add("Path",l.path);
            add("Extensions",l.extensions);
            add("Recursive",l.recursive?"true":"false");
            add("Scale",Number(l.scale));
            add("MinScale",Number(l.minScale));
            add("MaxScale",Number(l.maxScale));
            add("SRS",l.srs);
            add("Endpoint",l.endpoint);
            add("LayerName",l.layer);
            add("Version",l.version);
            add("Style",l.style);
            add("Format",l.format);
            add("TileMatrixSet",l.matrix);s+="  </Layer>\n";}s+="</MapSources>\n";
            AtomicWrite(xml,s);
            return true;
    }
    catch(CFileException* e)
    {
        TCHAR text[512];
        e->GetErrorMessage(text,512);
        error=text;
        e->Delete();
        return false;
    }
    catch(const std::exception& e)
    {
        error=ST(e.what());
        return false;
    }
}

void CMapSources::Draw(CDC* dc,CCoordMgr* c)
{
    if(!dc||!c||!c->m_bFlat||c->m_fScale<=0)
        return;
    double lon0=127.031-c->m_fCenterX/(47.5*c->m_fScale),lat0=37.0861+c->m_fCenterY/(60*c->m_fScale),lon1=lon0+c->m_nDisplayViewWidth/(47.5*c->m_fScale),lat1=lat0-c->m_nDisplayViewHeight/(60*c->m_fScale);
    Request r;
    r.z=(std::max)(0,(std::min)(20,(int)std::floor(log2(180*47.5*c->m_fScale/256))));
    double step=180.0/(1<<r.z);
    int nx=2<<r.z,ny=1<<r.z;
    int a=(int)floor((lon0+180)/step),b=(int)floor((lon1+180)/step),top=(std::max)(0,(int)floor((90-lat0)/step)),bottom=(std::min)(ny-1,(int)floor((90-lat1)/step));
    for(int y=top;y<=bottom;++y)
        for(int x=a;x<=b;++x)
        {
            int xx=(x%nx+nx)%nx;
            r.tiles.push_back({r.z,xx,y});
        }
    if(r.tiles.size()>256)
        return;
    double latitude=(std::max)(-85.0,(std::min)(85.0,(lat0+lat1)/2)),denominator=111320*cos(latitude*3.141592653589793/180)/(47.5*c->m_fScale)*dc->GetDeviceCaps(LOGPIXELSX)/0.0254;
    std::vector<std::shared_ptr<Tile>> ready;
    {
        std::lock_guard<std::mutex> l(p->mutex);
        std::map<std::string,std::pair<double,size_t>> best;
        for(size_t i=0;i<p->layers.size();++i)
        {
            const auto& source=p->layers[i];
            if(!Allowed(source)||!source.visual||(source.minScale&&denominator<source.minScale)||(source.maxScale&&denominator>source.maxScale))
                continue;
            if(source.type=="Raster"&&source.scale>0)
            {
                auto key=source.type+source.family;
                double score=fabs(log(denominator/source.scale));
                if(!best.count(key)||score<best[key].first)
                    best[key]={score,i};
            }
            else 
                r.sources.push_back(i);
        }
        for(const auto& v:best)
            r.sources.push_back(v.second.second);
        std::sort(r.sources.begin(),r.sources.end());
        if(r.tiles.size()*r.sources.size()>256)
        {
            p->status=_T("동시 타일 수 제한: 표시 레이어를 줄여 주세요.");
            return;
        }
        if(!(p->request==r))
        {
            p->request=r;
            ++p->ticket;
            p->pending=true;
            p->ready.clear();
            p->cv.notify_one();
        }
        ready=p->ready;
    }
    for(const auto& tile:ready)
    {
        CImage img;
        if(!img.Create(256,256,32,CImage::createAlphaChannel))
            continue;
        for(int y=0;y<256;++y)
            memcpy(img.GetPixelAddress(0,y),tile->bgra.data()+y*256*4,256*4);
        int first=(int)floor((lon0-tile->east)/360),last=(int)ceil((lon1-tile->west)/360);
        for(int copy=first;copy<=last;++copy)
        {
            double x0,y0,x1,y1;
            c->WorldLonLat2ScreenXY(tile->west+360*copy,tile->north,&x0,&y0,c->m_fCenterX,c->m_fCenterY,c->m_fScale);
            c->WorldLonLat2ScreenXY(tile->east+360*copy,tile->south,&x1,&y1,c->m_fCenterX,c->m_fCenterY,c->m_fScale);
            if(x1<0||x0>c->m_nDisplayViewWidth)
                continue;
            img.AlphaBlend(dc->GetSafeHdc(),(int)floor(x0),(int)floor(y0),(int)ceil(x1)-(int)floor(x0),(int)ceil(y1)-(int)floor(y0),0,0,256,256);
        }
    }
}
#pragma pack(pop)

#pragma pack(push,8)
namespace 
{
class SourceDialog:public CDialog 
{
    CMapSources& owner;std::vector<MapSourceLayer> data;
    std::vector<std::unique_ptr<CWnd>> controls;
    CListBox list;
    CFont font;
    std::vector<size_t> rows;
    int selected=-1;
    enum{LIST=7101,OPEN=7102,SAVE=7103,APPLY=7104,ADD=7105,RELOAD=7106,REFRESH=7107};

    CString Value(int id)
    {
        CString s;
        GetDlgItemText(id,s);
        return s;
    }
    void Make(const char* type,const CString& text,int id,int x,int y,int w,int h,DWORD extra=0)
    {
        auto c=std::make_unique<CWnd>();
        c->CreateEx(0,CString(type),text,WS_CHILD|WS_VISIBLE|extra,CRect(x,y,x+w,y+h),this,id);
        c->SetFont(&font);
        controls.push_back(std::move(c));
    }
    void Fill()
    {
        list.ResetContent();rows.clear();
        for(size_t i=0;i<data.size();++i)
            if(Allowed(data[i]))
            {
                rows.push_back(i);
                list.AddString(ST(data[i].name+" ["+data[i].type+"]"));
            }
        if(!rows.empty())
        {
            list.SetCurSel(0);
            Select();
        }
    }

    void Select()
    {
        int row=list.GetCurSel();
        if(row<0||row>=(int)rows.size())
            return;
        selected=(int)rows[row];const auto& s=data[selected];
        std::vector<std::string> values={s.name,s.path,s.extensions,Number(s.scale),Number(s.minScale),Number(s.maxScale),s.endpoint,s.layer,s.matrix,s.style,s.version,s.srs};
        for(int i=0;i<(int)values.size();++i)
            SetDlgItemText(7200+i,ST(values[i]));
        CheckDlgButton(7250,s.visual?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(7251,s.recursive?BST_CHECKED:BST_UNCHECKED);
    }
    bool Apply()
    {
        if(selected<0)
            return true;
        auto s=data[selected];
        s.name=SU(Value(7200));
        s.path=SU(Value(7201));
        s.extensions=SU(Value(7202));
        s.endpoint=SU(Value(7206));
        s.layer=SU(Value(7207));
        s.matrix=SU(Value(7208));
        s.style=SU(Value(7209));
        s.version=SU(Value(7210));
        s.srs=SU(Value(7211));
        auto number=[&](int id,double& d)
        {
            std::istringstream in(SU(Value(id)));
            in.imbue(std::locale::classic());
            return bool(in>>d)&&in.peek()==EOF&&std::isfinite(d)&&d>=0;
        };
        if(s.name.empty()||!number(7203,s.scale)||!number(7204,s.minScale)||!number(7205,s.maxScale)||(s.maxScale&&s.maxScale<s.minScale))
        {
            AfxMessageBox(_T("이름과 축척 값을 확인하세요. 제한 없음은 0입니다."));
            return false;
        }
        s.visual=IsDlgButtonChecked(7250)==BST_CHECKED;
        s.recursive=IsDlgButtonChecked(7251)==BST_CHECKED;
        if((s.type=="WMS"||s.type=="WFS"||s.type=="WMTS")&&s.visual&&(s.endpoint.empty()||s.layer.empty()))
        {
            AfxMessageBox(_T("표시할 서비스의 주소와 레이어를 입력하세요."));
            return false;
        }
        if((s.type=="Raster"||s.type=="CIB")&&s.path.empty())
        {
            AfxMessageBox(_T("데이터 폴더 또는 파일 경로를 입력하세요."));
            return false;
        }
        if(!s.endpoint.empty()&&s.endpoint.rfind("http://",0)!=0&&s.endpoint.rfind("https://",0)!=0)
        {
            AfxMessageBox(_T("서버 주소는 http:// 또는 https:// 로 시작해야 합니다."));
            return false;
        }
        data[selected]=s;
        int row=list.GetCurSel();
        list.DeleteString(row);
        list.InsertString(row,ST(s.name+" ["+s.type+"]"));
        list.SetCurSel(row);
        return true;
    }
    BOOL OnInitDialog()override
    {
        CDialog::OnInitDialog();
        SetWindowText(_T("XML 지도 소스 설정"));
        font.CreatePointFont(90,_T("맑은 고딕"));
        list.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|LBS_NOTIFY,CRect(10,10,240,485),this,LIST);
        list.SetFont(&font);
        const char* labels[]={"이름","데이터 경로","확장자 (; 구분)","대표 축척 분모","최소 축척 분모","최대 축척 분모","서버 / Capabilities URL","서비스 레이어 이름","WMTS TileMatrixSet","스타일","WMS 버전","원본 좌표계 (선택)"};
        for(int i=0;i<12;++i)
        {
            int y=10+i*34;
            Make("STATIC",CString(labels[i]),-1,252,y,190,20);
            Make("EDIT",_T(""),7200+i,442,y,355,24,WS_BORDER|ES_AUTOHSCROLL|WS_TABSTOP);
        }
        Make("BUTTON",_T("기본 표시 (visual)"),7250,260,423,230,25,BS_AUTOCHECKBOX|WS_TABSTOP);
        Make("BUTTON",_T("하위 폴더 전체 탐색"),7251,510,423,245,25,BS_AUTOCHECKBOX|WS_TABSTOP);
        Make("BUTTON",_T("선택 항목 반영"),APPLY,255,460,155,28,WS_TABSTOP);
        Make("BUTTON",_T("XML 저장 및 적용"),SAVE,420,460,170,28,WS_TABSTOP);
        Make("BUTTON",_T("XML 불러오기"),OPEN,600,460,160,28,WS_TABSTOP);
        Make("BUTTON",_T("XML 다시 읽기"),RELOAD,10,502,155,28,WS_TABSTOP);
        Make("BUTTON",_T("타일 새로 고침"),REFRESH,175,502,155,28,WS_TABSTOP);
        Make("BUTTON",_T("닫기"),IDCANCEL,675,502,120,28,WS_TABSTOP);
        Make("STATIC",owner.Status(),7300,10,540,785,40);Fill();return TRUE;
    }

    BOOL OnCommand(WPARAM wp,LPARAM lp)override
    {
        int id=LOWORD(wp);
        if(id==LIST&&HIWORD(wp)==LBN_SELCHANGE)
        {
            Select();
            return TRUE;
        }
        if(id==APPLY)
        {
            Apply();
            return TRUE;
        }
        if(id==REFRESH)
        {
            owner.ResetCache();
            SetDlgItemText(7300,_T("타일 캐시를 초기화했습니다."));
            return TRUE;
        }
        if(id==SAVE)
        {
            if(!Apply())
                return TRUE;
            CString path=owner.Path();
            CFileDialog dlg(FALSE,_T("xml"),path,OFN_OVERWRITEPROMPT,_T("지도 정의 XML (*.xml)|*.xml||"),this);
            if(dlg.DoModal()==IDOK)
            {
                CString err;
                if(owner.Save(dlg.GetPathName(),data,err)&&owner.Load(dlg.GetPathName(),err))
                {
                    SetDlgItemText(7300,_T("XML 저장 및 적용 완료"));
                }
                else 
                    AfxMessageBox(err,MB_ICONERROR);
            }
            return TRUE;
        }
        if(id==OPEN||id==RELOAD)
        {
            CString path=owner.Path();
            if(id==OPEN)
            {
                CFileDialog dlg(TRUE,_T("xml"),path,OFN_FILEMUSTEXIST,_T("지도 정의 XML (*.xml)|*.xml||"),this);
                if(dlg.DoModal()!=IDOK)
                    return TRUE;
                path=dlg.GetPathName();
            }
            CString err;
            if(owner.Load(path,err))
            {
                data=owner.Layers();
                Fill();
                SetDlgItemText(7300,owner.Status());
            }
            else 
                AfxMessageBox(err,MB_ICONERROR);
            return TRUE;
        }
        return CDialog::OnCommand(wp,lp);
    }
public:SourceDialog(CMapSources& o):owner(o),data(o.Layers())
{
}
    INT_PTR Show(CWnd* parent)
    {
        struct Template
        {
            DLGTEMPLATE dlg;
            WORD menu,klass,title;
        } 
        t={};
        t.dlg.style=WS_POPUP|WS_CAPTION|WS_SYSMENU|DS_MODALFRAME;
        t.dlg.cx=540;
        t.dlg.cy=390;InitModalIndirect(&t.dlg,parent);
        return DoModal();
    }
};
}
void CMapSources::Configure(CWnd* parent)
{
    SourceDialog dlg(*this);
    dlg.Show(parent);
}
#pragma pack(pop)
