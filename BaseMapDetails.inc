#pragma pack(push,8)
struct BaseMapState 
{
    struct Line 
    { 
        std::vector<OGRPoint> points; 
        OGREnvelope bounds; 
    };
    struct Label 
    { 
        double lon,lat; 
        CStringW text; 
        int source; 
    };
    std::vector<Line> lines;
    std::vector<Label> labels;
    bool loaded=false;
    void Geometry(OGRGeometry* g) 
    {
        if(!g)  return;
        const auto type=wkbFlatten(g->getGeometryType());
        if(type==wkbLineString||type==wkbLinearRing)
        {
            auto line=g->toLineString();
            if(line->getNumPoints()<2)  return;
            Line out;
            double previous=0;

            for(int i=0;i<line->getNumPoints();++i)
            {
                double lon=line->getX(i),lat=line->getY(i);
                if(i)
                {
                    while(lon-previous>180)
                        lon-=360;
                    while(lon-previous< -180)
                        lon+=360;
                }
                previous=lon;out.points.emplace_back(lon,lat);
            }
            // Cache a display-only simplified outline; no GEOS runtime required.
            if(out.points.size()>2)
            {
                std::vector<unsigned char> keep(out.points.size(),0);
                keep.front()=keep.back()=1;
                std::vector<std::pair<size_t,size_t>> pending{{0,out.points.size()-1}};
                size_t work=0,budget=out.points.size()*64;
                while(!pending.empty()&&work<budget)
                {
                    auto range=pending.back();pending.pop_back();
                    auto& a=out.points[range.first];
                    auto& b=out.points[range.second];
                    double dx=b.getX()-a.getX(),dy=b.getY()-a.getY(),len=dx*dx+dy*dy,best=0;
                    size_t split=range.first;

                    for(size_t i=range.first+1;i<range.second;++i)
                    {
                        ++work;auto& point=out.points[i];
                        double t=len?std::max(0.0,std::min(1.0,((point.getX()-a.getX())*dx+(point.getY()-a.getY())*dy)/len)):0;
                        double ex=point.getX()-a.getX()-t*dx,ey=point.getY()-a.getY()-t*dy,d=ex*ex+ey*ey;
                        if(d>best)
                        {
                            best=d;
                            split=i;
                        }
                    }
                    if(best>0.003*0.003&&split>range.first)
                    {
                        keep[split]=1;
                        pending.push_back({range.first,split});
                        pending.push_back({split,range.second});
                    }
                }
                if(pending.empty())
                {
                    std::vector<OGRPoint> reduced;
                    for(size_t i=0;i<keep.size();++i)
                        if(keep[i])
                            reduced.push_back(out.points[i]);
                            out.points.swap(reduced);
                }
            }
            out.bounds.MinX=out.bounds.MaxX=out.points[0].getX();
            out.bounds.MinY=out.bounds.MaxY=out.points[0].getY();
            for(auto& p:out.points)
            {
                out.bounds.MinX=std::min(out.bounds.MinX,p.getX());
                out.bounds.MaxX=std::max(out.bounds.MaxX,p.getX());
                out.bounds.MinY=std::min(out.bounds.MinY,p.getY());
                out.bounds.MaxY=std::max(out.bounds.MaxY,p.getY());
            }
            lines.push_back(std::move(out));
        }
        else if(type==wkbPolygon)
        {
            auto p=g->toPolygon();
            Geometry(p->getExteriorRing());
        }
        else if(OGR_GT_IsSubClassOf(type,wkbGeometryCollection))
        {
            auto c=g->toGeometryCollection();
            for(int i=0;i<c->getNumGeometries();++i)
            Geometry(c->getGeometryRef(i));
        }
    }

    void Load(const char* path,int source,bool names) 
    {
        // Legacy DBF files without a .cpg use Korean CP949; GDAL returns UTF-8.
        std::string cpg(path);
        cpg=cpg.substr(0,cpg.find_last_of('.'))+".cpg";
        const char* options[]={"ENCODING=CP949",nullptr};
        auto ds=static_cast<GDALDataset*>(GDALOpenEx(path,GDAL_OF_VECTOR|GDAL_OF_READONLY,nullptr,GetFileAttributesA(cpg.c_str())==INVALID_FILE_ATTRIBUTES?options:nullptr,nullptr));
        if(!ds) return;
        auto layer=ds->GetLayer(0);
        if(!layer)
        {
            GDALClose(ds);
            return;
        }
        OGRSpatialReference target;
        target.SetWellKnownGeogCS("WGS84");
        target.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        auto srs=layer->GetSpatialRef();
        OGRCoordinateTransformation* transform=nullptr;
        if(srs)
        {
            auto src=*srs;
            src.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
            if(!src.IsSame(&target))
                transform=OGRCreateCoordinateTransformation(&src,&target);
            if(!src.IsSame(&target)&&!transform)
            {
                GDALClose(ds);
                return;
            }
        }
        int field=-1;
        for(auto key:{"TXTDESC","PLACENAME","NAME_KO","NAME","name","NAME_EN"})
        {
            field=layer->GetLayerDefn()->GetFieldIndex(key);
            if(field>=0)break;
        }
        layer->ResetReading();
        while(auto feature=layer->GetNextFeature())
        {
            auto original=feature->GetGeometryRef();
            auto g=original?original->clone():nullptr;
            if(g && (!transform||g->transform(transform)==OGRERR_NONE))
            {
                if(names&&field>=0&&feature->IsFieldSetAndNotNull(field)&&wkbFlatten(g->getGeometryType())==wkbPoint)
                {
                    auto p=g->toPoint();
                    CA2W text(feature->GetFieldAsString(field),CP_UTF8);
                    labels.push_back({p->getX(),p->getY(),CStringW(text),source});
                }
                else if(!names)
                {
                    Geometry(g);
                }
            }
            OGRGeometryFactory::destroyGeometry(g);OGRFeature::DestroyFeature(feature);
        }
        if(transform)OCTDestroyCoordinateTransformation(transform);GDALClose(ds);
    }
};
#pragma pack(pop)

void CShapeT::DrawBaseDetails(CDC* dc,float cx,float cy,float scale){
#if C4MAP_ENABLE_LOCAL
    if(!dc||scale<=0||(!BaseMapVisible(0)&&!BaseMapVisible(1))) return;
    if(!m_baseMap)
    {
        m_baseMap=std::make_unique<BaseMapState>();
        m_baseMap->Load(BaseMapConfig().Value("World.Path","D:/map/world_map/Continents.shp").c_str(),1,false);
        m_baseMap->Load(BaseMapConfig().Value("World.BoundaryPath","D:/map/world_map/national_boundary_polygon.shp").c_str(),1,false);
        m_baseMap->Load(BaseMapConfig().Value("SmallScale.LabelPath","D:/map/small_scale_map/name_map.shp").c_str(),0,true);
        m_baseMap->Load(BaseMapConfig().Value("World.LabelPath","D:/map/world_map/placenam.shp").c_str(),1,true);
        m_baseMap->Load(BaseMapConfig().Value("World.CityPath","D:/map/world_map/World_city_pt.shp").c_str(),1,true);
    }
    auto mgr=GP_COORDMGR;
    CRect viewport(0,0,mgr->m_nDisplayViewWidth,mgr->m_nDisplayViewHeight);
    int saved=dc->SaveDC();
    dc->IntersectClipRect(viewport);
    CPen pen(PS_SOLID,1,RGB(110,120,130));
    dc->SelectObject(&pen);
    auto screen=[&](double lon,double lat)
    {
        CPoint p;mgr->WorldLonLat2ScreenXY(lon,lat,&p.x,&p.y,cx,cy,scale);
        return p;
    };
    if(BaseMapVisible(1))
    for(auto& line:m_baseMap->lines)
    {
        auto a=screen(line.bounds.MinX,line.bounds.MaxY),b=screen(line.bounds.MaxX,line.bounds.MinY);
        CRect bounds(a,b);
        bounds.NormalizeRect();
        CRect overlap;
        if(!overlap.IntersectRect(bounds,viewport)) continue;
        std::vector<CPoint> points;
        points.reserve(line.points.size());
        for(auto& p:line.points)
        {
            auto xy=screen(p.getX(),p.getY());
            if(points.empty()||abs(xy.x-points.back().x)+abs(xy.y-points.back().y)>1)
            points.push_back(xy);
        }
        if(points.size()>1)
        dc->Polyline(points.data(),(int)points.size());
    }
    if(BaseMapVisible(3))
    {
        CFont font;
        font.CreatePointFont(90,_T("맑은 고딕"),dc);
        auto old=dc->SelectObject(&font);
        dc->SetBkMode(TRANSPARENT);
        dc->SetTextAlign(TA_LEFT|TA_TOP);
        std::vector<CRect> occupied;
        for(auto& label:m_baseMap->labels)
        {
            if(!BaseMapVisible(label.source)||label.text.IsEmpty())
                continue;
            auto p=screen(label.lon,label.lat);
            if(!viewport.PtInRect(p))   
                continue;
            SIZE size;
            GetTextExtentPoint32W(dc->GetSafeHdc(),label.text,label.text.GetLength(),&size);
            CRect box(p.x+4,p.y-size.cy/2,p.x+size.cx+10,p.y+size.cy/2+4);
            bool hit=false;
            for(auto& previous:occupied)
            {
                CRect overlap;if(overlap.IntersectRect(previous,box))
                {
                    hit=true;   break;
                }
            }
            if(hit) 
                continue;
            occupied.push_back(box);
            dc->SetTextColor(RGB(255,255,255));
            for(auto offset:{CPoint(-1,0),CPoint(1,0),CPoint(0,-1),CPoint(0,1)})
                TextOutW(dc->GetSafeHdc(),box.left+offset.x,box.top+offset.y,label.text,label.text.GetLength());
            dc->SetTextColor(RGB(25,35,45));
            TextOutW(dc->GetSafeHdc(),box.left,box.top,label.text,label.text.GetLength());
            dc->Ellipse(p.x-2,p.y-2,p.x+2,p.y+2);
        }
        dc->SelectObject(old);
    }
    dc->RestoreDC(saved);
#endif
}
