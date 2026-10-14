#pragma once
#include <cmath>
#include "GeoDC/Coord.h"

inline bool MapScreenLonLat(CCoordMgr* mgr,double x,double y,double& lon,double& lat)
{
    if(!mgr||mgr->m_fScale<=0)
        return false;
    if(mgr->m_bFlat)
    {
        lon=127.031+(x-mgr->m_fCenterX)/(47.5*mgr->m_fScale);
        lat=37.0861-(y-mgr->m_fCenterY)/(60.0*mgr->m_fScale);
    }
    else
    {
        double wx,wy;
        mgr->ScreenXY2WorldXY((int)x,(int)y,&wx,&wy);
        CCoord c;
        c.initialize_adaptation_data();
        c.x_y_to_degrees_lat_long(wx,wy,&lat,&lon);
    }
    return std::isfinite(lon)&&std::isfinite(lat)&&fabs(lat)<=90;
}
inline double MapDistanceMetres(double lon1,double lat1,double lon2,double lat2)
{
    const double rad=3.141592653589793/180;
    double dlat=(lat2-lat1)*rad,dlon=std::remainder(lon2-lon1,360.0)*rad;
    double a=sin(dlat/2)*sin(dlat/2)+cos(lat1*rad)*cos(lat2*rad)*sin(dlon/2)*sin(dlon/2);
    return 6371008.8*2*asin(sqrt((std::max)(0.0,(std::min)(1.0,a))));
}
inline double MapNiceDistance(double maximum)
{
    if(!std::isfinite(maximum)||maximum<=0)
        return 0;
    double power=pow(10.0,floor(log10(maximum))),fraction=maximum/power;
    return power*(fraction>=5?5:fraction>=2?2:1);
}
inline void DrawMapDecoration(CDC* dc,CCoordMgr* mgr,CMainFrame* frame,const CRect& viewport)
{
    if(!dc||!mgr||viewport.Width()<220||viewport.Height()<120)
        return;
    double lon,lat,lon2,lat2;
    double x=viewport.CenterPoint().x,y=viewport.CenterPoint().y;
    if(!MapScreenLonLat(mgr,x,y,lon,lat)||!MapScreenLonLat(mgr,x+1,y,lon2,lat2))
        return;
    const double metresPerPixel=MapDistanceMetres(lon,lat,lon2,lat2);
    if(metresPerPixel<=0)
        return;
    double denominator=metresPerPixel*dc->GetDeviceCaps(LOGPIXELSX)/0.0254;
    CString number;
    number.Format(_T("%.0f"),denominator);
    for(int pos=number.GetLength()-3;pos>0;pos-=3)
        number.Insert(pos,_T(','));
    CString status=BaseMapConfig().Flag("ScaleStatus")?_T("Scale 1:")+number:CString();
    if(frame)
    {
        auto pane=frame->m_wndStatusBar.FindElement(59220);
        if(pane&&CString(pane->GetText())!=status)
        {
            pane->SetText(status);
            frame->m_wndStatusBar.RecalcLayout();
            frame->m_wndStatusBar.RedrawWindow();
        }
    }
    int saved=dc->SaveDC();
    dc->SetBkMode(TRANSPARENT);
    dc->SetTextColor(RGB(20,30,40));
    CFont font;
    font.CreatePointFont(90,_T("Segoe UI"),dc);dc->SelectObject(&font);
    const int left=viewport.left+18,bottom=viewport.bottom-20;
    // Distance at the bar's latitude (projection scale varies with latitude).
    double blon,blat,elon,elat;
    if(BaseMapConfig().Flag("ScaleBar")&&MapScreenLonLat(mgr,left,bottom,blon,blat)&&MapScreenLonLat(mgr,left+1,bottom,elon,elat))
    {
        double unit=MapDistanceMetres(blon,blat,elon,elat);
        double distance=MapNiceDistance(unit*140);
        if(unit>0&&distance>0)
        {
            int width=(int)std::lround(distance/unit);
            CString label;
            if(distance>=1000)
                label.Format(_T("%g km"),distance/1000);
            else
                label.Format(_T("%g m"),distance);
            dc->FillSolidRect(left-8,bottom-28,180,45,RGB(255,255,255));
            dc->TextOut(left,bottom-25,_T("0"));
            dc->TextOut(left+width/2,bottom-25,label);
            dc->FillSolidRect(left,bottom-5,width,7,RGB(20,30,40));
            dc->FillSolidRect(left+width/2,bottom-4,width-width/2-1,5,RGB(255,255,255));
        }
    }
    if(!BaseMapConfig().Flag("Compass"))
    {
        dc->RestoreDC(saved);
        return;
    }
    const int cx=viewport.right-44,cy=viewport.top+54;
    dc->FillSolidRect(cx-30,cy-43,60,83,RGB(255,255,255));
    double nx=0,ny=-1;
    if(!mgr->m_bFlat)
    {
        double northX,northY;
        mgr->WorldLonLat2ScreenXY(lon,(std::min)(89.999,lat+0.1),&northX,&northY,mgr->m_fCenterX,mgr->m_fCenterY,mgr->m_fScale);
        nx=northX-x;
        ny=northY-y;
        double length=hypot(nx,ny);
        if(length<1e-8)
        {
            nx=0;
            ny=-1;
        }
        else
        {
            nx/=length;
            ny/=length;
        }
    }
    CPen pen(PS_SOLID,1,RGB(40,50,60));
    dc->SelectObject(&pen);
    dc->SelectStockObject(NULL_BRUSH);
    dc->Ellipse(cx-23,cy-23,cx+24,cy+24);
    CBrush red(RGB(200,45,45));
    dc->SelectObject(&red);CPoint arrow[]={{cx+(int)(nx*22),cy+(int)(ny*22)},{cx+(int)(-ny*7),cy+(int)(nx*7)},{cx+(int)(ny*7),cy-(int)(nx*7)}};
    dc->Polygon(arrow,3);
    dc->TextOut(cx-5,cy-41,_T("N"));
    dc->TextOut(cx-5,cy+24,_T("S"));
    dc->RestoreDC(saved);
}
