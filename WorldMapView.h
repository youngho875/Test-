#pragma once
#include "GeoDC/CoordMgr.h"
#include <algorithm>
#include <cmath>

inline void PrepareWorldMapView(CCoordMgr& mgr)
{
    const double width = mgr.m_nDisplayViewWidth;
    const double height = mgr.m_nDisplayViewHeight;
    if (width <= 0 || height <= 0) return;
    if (!mgr.m_bFlat) 
    {
        double wx, wy, sx, sy;
        mgr.ScreenXY2WorldXY(static_cast<int>(width / 2), static_cast<int>(height / 2), &wx, &wy);
        mgr.m_bFlat = true;
        mgr.WorldXY2ScreenXY(wx, wy, &sx, &sy);
        mgr.SetNewCenter(sx, sy);
    }
    const double fit = (std::min)(width / (360.0 * 47.5), height / (180.0 * 60.0));
    if (!std::isfinite(mgr.m_fScale) || mgr.m_fScale < fit)
        mgr.m_fScale = fit;

    // Keep horizontal panning periodic and bounded numerically; clamp only latitude.
    const double worldWidth = 360.0 * 47.5 * mgr.m_fScale;
    const double worldHeight = 180.0 * 60.0 * mgr.m_fScale;
    double left = mgr.m_fCenterX + (-180.0 - 127.031) * 47.5 * mgr.m_fScale;
    double top = mgr.m_fCenterY - (90.0 - 37.0861) * 60.0 * mgr.m_fScale;
    left = width / 2 + std::fmod(left - width / 2, worldWidth);
    if (left > width / 2) 
        left -= worldWidth;

    top = worldHeight <= height ? (height - worldHeight) / 2 : (std::max)(height - worldHeight, (std::min)(0.0, top));
    mgr.m_fCenterX = left + (180.0 + 127.031) * 47.5 * mgr.m_fScale;
    mgr.m_fCenterY = top + (90.0 - 37.0861) * 60.0 * mgr.m_fScale;
}

// Render geographic features in each visible world copy. One extra copy on
// either side includes paths unwrapped across the antimeridian.
template<class Draw>
inline void DrawVisibleWorldCopies(const CCoordMgr& mgr, Draw draw)
{
    if (!mgr.m_bFlat) 
    {
        draw(mgr.m_fCenterX); 
        return;
    }
    const double period = 360.0 * 47.5 * mgr.m_fScale;
    if (!std::isfinite(period) || period <= 0) 
        return;

    const double left = mgr.m_fCenterX - (180.0 + 127.031) * 47.5 * mgr.m_fScale;
    const int first = static_cast<int>(std::floor(-left / period)) - 1;
    const int last = static_cast<int>(std::floor((mgr.m_nDisplayViewWidth - left) / period)) + 1;
    for (int copy = first; copy <= last; ++copy) 
        draw(mgr.m_fCenterX + copy * period);
}
