#include "pch.h"
#pragma pack(push, 8)
#include "LocalOverlay.h"
#include <vector>
#include <memory>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <locale>
#include <gdiplus.h>
#include "GeoDC/Coord.h"
#include "GeoDC/CoordMgr.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectLine.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectRect.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectEllipse.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectLineDraw.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectRectDraw.h"
#include "OverlaySDK/LibSrc/Overlay/ObjectEllipseDraw.h"
#include "cpl_minixml.h"
#pragma comment(lib,"gdiplus.lib")

struct CLocalOverlay::Impl 
{
    struct Item 
    { 
        int type=Line; 
        double x1=0,y1=0,x2=0,y2=0; 
        COLORREF color=RGB(255,60,40);
    };

    std::vector<Item> items;
    Item pending;
    int tool=Pan;
    bool drawing=false;
    bool modified=false;
    CPoint start;
    COLORREF color=RGB(255,60,40);
    ULONG_PTR token=0;
    Impl(){ Gdiplus::GdiplusStartupInput in; Gdiplus::GdiplusStartup(&token,&in,NULL); }
    ~Impl(){ if(token) Gdiplus::GdiplusShutdown(token); }
};
CLocalOverlay::CLocalOverlay():p(new Impl){}
CLocalOverlay::~CLocalOverlay(){delete p;}
void CLocalOverlay::SetTool(int tool){Cancel();p->tool=tool>=Pan&&tool<=Ellipse?tool:Pan;}
int CLocalOverlay::GetTool()const{return p->tool;}
bool CLocalOverlay::IsDrawing()const{return p->drawing;}
void CLocalOverlay::Cancel(){p->drawing=false;}
size_t CLocalOverlay::Count()const{return p->items.size();}
COLORREF CLocalOverlay::Color()const{return p->color;}
void CLocalOverlay::SetColor(COLORREF c){p->color=c;}
void CLocalOverlay::Undo(){Cancel();if(!p->items.empty()){p->items.pop_back();p->modified=true;}}
bool CLocalOverlay::IsModified()const{return p->modified;}
void CLocalOverlay::Clear(){Cancel();p->items.clear();p->modified=false;p->tool=Pan;}
bool CLocalOverlay::Begin(CPoint pt,CCoordMgr* c){
    if(!c||p->tool==Pan)return false;
    p->pending=Impl::Item(); p->pending.type=p->tool;p->pending.color=p->color;
    c->ScreenXY2WorldXY(pt.x,pt.y,&p->pending.x1,&p->pending.y1);
    p->pending.x2=p->pending.x1;p->pending.y2=p->pending.y1;
    p->start=pt;p->drawing=true;return true;
}
bool CLocalOverlay::Move(CPoint pt,CCoordMgr* c){
    if(!c||!p->drawing)return false;
    c->ScreenXY2WorldXY(pt.x,pt.y,&p->pending.x2,&p->pending.y2);return true;
}
bool CLocalOverlay::End(CPoint pt,CCoordMgr* c){
    if(!Move(pt,c))return false;
    if(abs(pt.x-p->start.x)+abs(pt.y-p->start.y)>=4){p->items.push_back(p->pending);p->modified=true;}
    p->drawing=false;return true;
}
void CLocalOverlay::Draw(CDC* dc,CCoordMgr* c){
    if(!dc||!c||!p->token)return;
    Gdiplus::Graphics g(dc->GetSafeHdc());g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    // C4IMap owns projection. Supply screen coordinates to SDK objects without
    // its global Point coordinate manager (which uses a different projection).
    auto previous=OVERLAY::Point::g_pCoord;OVERLAY::Point::g_pCoord=NULL;
    struct Restore { CCoordMgr* value; ~Restore(){OVERLAY::Point::g_pCoord=value;} } restore{previous};
    auto draw=[&](const Impl::Item& item){
        double x1,y1,x2,y2;
        c->WorldXY2ScreenXY(item.x1,item.y1,&x1,&y1);
        c->WorldXY2ScreenXY(item.x2,item.y2,&x2,&y2);
        if(!std::isfinite(x1)||!std::isfinite(y1)||!std::isfinite(x2)||!std::isfinite(y2))return;
        std::unique_ptr<OVERLAY::OBJECT> object;
        std::unique_ptr<OVERLAY::CObjectDraw> renderer;
        if(item.type==Rectangle){object.reset(new OVERLAY::ObjectRect);renderer.reset(new OVERLAY::CObjectRectDraw);}
        else if(item.type==Ellipse){object.reset(new OVERLAY::ObjectEllipse);renderer.reset(new OVERLAY::CObjectEllipseDraw);}
        else {object.reset(new OVERLAY::ObjectLine);renderer.reset(new OVERLAY::CObjectLineDraw);}
        if(item.type==Rectangle||item.type==Ellipse){if(x1>x2)std::swap(x1,x2);if(y1>y2)std::swap(y1,y2);}
        object->pt[0].SetXY(x1,y1);object->pt[1].SetXY(x2,y2);
        object->Property.LineColor=Gdiplus::Color(255,GetRValue(item.color),GetGValue(item.color),GetBValue(item.color)).GetValue();
        object->Property.FillColor=Gdiplus::Color(40,GetRValue(item.color),GetGValue(item.color),GetBValue(item.color)).GetValue();
        object->Property.dLineWidth=3;object->Property.bIsLine=TRUE;object->Property.bIsFill=FALSE;
        if(item.type==Arrow)object->Property.nEndLineArrowStyle=4;
        renderer->Draw(object.get(),&g);
    };
    for(const auto& item:p->items)draw(item);
    if(p->drawing)draw(p->pending);
}

bool CLocalOverlay::Save(LPCTSTR path,CString& error)const{
    std::ostringstream out;out.imbue(std::locale::classic());out<<std::setprecision(17);
    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<C4IMapOverlay version=\"1\" coordinates=\"GeoDC-world\">\n";
    for(const auto& i:p->items)out<<"  <Object type=\""<<i.type<<"\" x1=\""<<i.x1<<"\" y1=\""<<i.y1<<"\" x2=\""<<i.x2<<"\" y2=\""<<i.y2<<"\" color=\""<<i.color<<"\"/>\n";
    out<<"</C4IMapOverlay>\n";
    CString temp(path);temp+=_T(".tmp");
    try{CFile file(temp,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);auto s=out.str();file.Write(s.data(),(UINT)s.size());file.Flush();file.Close();}
    catch(CFileException* e){TCHAR msg[512];e->GetErrorMessage(msg,512);error=msg;e->Delete();DeleteFile(temp);return false;}
    if(!MoveFileEx(temp,path,MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)){error=_T("Overlay XML replacement failed.");DeleteFile(temp);return false;}
    p->modified=false;return true;
}
bool CLocalOverlay::Load(LPCTSTR path,CString& error){
    std::string bytes;
    try{CFile file(path,CFile::modeRead|CFile::shareDenyWrite);auto len=file.GetLength();if(len==0||len>8*1024*1024){error=_T("Overlay XML is too large.");return false;}bytes.resize((size_t)len);if(file.Read(&bytes[0],(UINT)len)!=len){error=_T("Overlay XML read failed.");return false;}}
    catch(CFileException* e){TCHAR msg[512];e->GetErrorMessage(msg,512);error=msg;e->Delete();return false;}
    std::unique_ptr<CPLXMLNode,decltype(&CPLDestroyXMLNode)> tree(CPLParseXMLString(bytes.c_str()),CPLDestroyXMLNode);
    CPLXMLNode* root=nullptr;for(auto n=tree.get();n;n=n->psNext)if(n->eType==CXT_Element&&strcmp(n->pszValue,"C4IMapOverlay")==0)root=n;
    auto fail=[&](){error=_T("Unsupported or invalid C4IMap Overlay XML.");return false;};
    if(!root||strcmp(CPLGetXMLValue(root,"version",""),"1")||strcmp(CPLGetXMLValue(root,"coordinates",""),"GeoDC-world"))return fail();
    std::vector<Impl::Item> items;
    auto number=[](CPLXMLNode* n,const char* key,double& v){const char* s=CPLGetXMLValue(n,key,"");std::istringstream in(s);in.imbue(std::locale::classic());return bool(in>>v)&&in.peek()==EOF&&std::isfinite(v);};
    for(auto n=root->psChild;n;n=n->psNext){
        if(n->eType!=CXT_Element)continue;
        if(strcmp(n->pszValue,"Object"))return fail();
        Impl::Item i;double type,color;
        if(!number(n,"type",type)||type<Line||type>Ellipse||type!=floor(type)||!number(n,"color",color)||color<0||color>0xffffff||color!=floor(color)||!number(n,"x1",i.x1)||!number(n,"y1",i.y1)||!number(n,"x2",i.x2)||!number(n,"y2",i.y2))return fail();
        i.type=(int)type;i.color=(COLORREF)color;items.push_back(i);if(items.size()>10000)return fail();
    }
    Cancel();p->items.swap(items);p->modified=false;return true;
}
#pragma pack(pop)
