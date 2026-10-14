#include "pch.h"
#include "TmsMgr.h"
#include "BaseMapOptions.h"
#include "GeoDC/Coord.h"
#include "GeoDC/CoordMgr.h"
#include <algorithm>
#include <cmath>

CTmsMgr::CTmsMgr(void)
    : mCoordMgr(GP_COORDMGR), mDirectoryPath(_T("D:/webcop/NaturalEarthII"))
{
    mCoord.initialize_adaptation_data();
}

CTmsMgr::~CTmsMgr(void)
{
    UnLoad();
}

long CTmsMgr::CalcLevel(double left, double top, double right, double bottom, CRect rect)
{
    const double width = std::fabs(right - left);
    if (rect.Width() <= 0 || !std::isfinite(width) || width <= 0)
        return 0;
    const double level = std::floor(std::log2(180.0 * rect.Width() / (256.0 * width)));
    return static_cast<long>(std::max(0.0, std::min(20.0, level)));
}

long CTmsMgr::CalcIndexX(long level, double longitude) const
{
    return static_cast<long>(std::floor(std::pow(2.0, level) * ((longitude + 180.0) / 180.0)));
}

long CTmsMgr::CalcIndexY(long level, double latitude) const
{
    return static_cast<long>(std::floor(std::pow(2.0, level) * ((latitude + 90.0) / 180.0)));
}

double CTmsMgr::CalcMapX(long level, long index) const
{
    return CalcResolution(level) * 256 * index - 180.0;
}

double CTmsMgr::CalcMapY(long level, long index) const
{
    return CalcResolution(level) * 256 * index - 90.0;
}

double CTmsMgr::CalcResolution(long level) const
{
    return (180.0 / 256) / std::pow(2.0, level);
}

void CTmsMgr::Initalize()
{
    UnLoad();
    mCoordMgr = GP_COORDMGR;
    mCoord.initialize_adaptation_data();
    mAvailableLevels.clear();
    CString configured=BaseMapPath("NaturalEarth.Path","D:/webcop/NaturalEarthII");
    const LPCTSTR directories[] = { configured.GetString() };
    for (LPCTSTR directory : directories)
    {
        CFileFind finder;
        BOOL more = finder.FindFile(CString(directory) + _T("/*"));
        while (more)
        {
            more = finder.FindNextFile();
            if (!finder.IsDirectory() || finder.IsDots()) continue;
            CString name = finder.GetFileName();
            TCHAR* end = NULL;
            const long level = _tcstol(name, &end, 10);
            if (end != name.GetString() && *end == 0 && level >= 0 && level <= 20)
                mAvailableLevels.insert(level);
        }
        finder.Close();
        if (!mAvailableLevels.empty())
        {
            mDirectoryPath = directory;
            return;
        }
    }
    OutputDebugString(_T("C4IMap: no geographic TMS tile directory was found.\n"));
}

bool CTmsMgr::ScreenToGeo(int x, int y, double& longitude, double& latitude)
{
    if (mCoordMgr->m_bFlat)
    {
        longitude = 127.031 + (x - mCoordMgr->m_fCenterX) / (47.5 * mCoordMgr->m_fScale);
        latitude = 37.0861 - (y - mCoordMgr->m_fCenterY) / (60.0 * mCoordMgr->m_fScale);
    }
    else
    {
        double worldX, worldY;
        mCoordMgr->ScreenXY2WorldXY(x, y, &worldX, &worldY);
        mCoord.x_y_to_degrees_lat_long(worldX, worldY, &latitude, &longitude);
    }
    if (!std::isfinite(longitude) || !std::isfinite(latitude) || latitude < -90.0 || latitude > 90.0)
        return false;
    // Geographic TMS repeats at the antimeridian.
    longitude = std::fmod(longitude + 180.0, 360.0);
    if (longitude < 0) longitude += 360.0;
    longitude -= 180.0;
    return true;
}

CImage* CTmsMgr::UseTile(long level, long x, long y, MapImagesType& used, std::set<CString>& missing)
{
    CString key;
    key.Format(_T("%ld_%ld_%ld"), level, x, y);
    auto current = used.find(key);
    if (current != used.end()) return current->second;
    if (missing.find(key) != missing.end()) return NULL;
    auto cached = mImages.find(key);
    if (cached != mImages.end())
    {
        CImage* tile = cached->second;
        used.insert(*cached);
        mImages.erase(cached);
        return tile;
    }
    CImage* tile = new CImage;
    const LPCTSTR extensions[] = { _T("png"), _T("jpg"), _T("jpeg") };
    for (LPCTSTR extension : extensions)
    {
        CString path;
        path.Format(_T("%s/%ld/%ld/%ld.%s"), mDirectoryPath.GetString(), level, x, y, extension);
        if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES) continue;
        if (SUCCEEDED(tile->Load(path)) && !tile->IsNull()) break;
        tile->Destroy();
    }
    // Normalize indexed/monochrome tiles once, so the pixel loop never uses GetPixel/GDI.
    if (!tile->IsNull() && tile->GetBPP() != 24 && tile->GetBPP() != 32)
    {
        CImage* converted = new CImage;
        if (converted->Create(tile->GetWidth(), tile->GetHeight(), 32))
        {
            HDC target = converted->GetDC();
            PatBlt(target, 0, 0, tile->GetWidth(), tile->GetHeight(), BLACKNESS);
            tile->Draw(target, 0, 0);
            converted->ReleaseDC();
        }
        delete tile;
        tile = converted;
    }
    if (tile->IsNull())
    {
        delete tile;
        missing.insert(key);
        return NULL;
    }
    used.insert(std::make_pair(key, tile));
    return tile;
}

void CTmsMgr::Load(CRect rect)
{
    if (!mCoordMgr || rect.Width() <= 0 || rect.Height() <= 0 ||
        !std::isfinite(mCoordMgr->m_fScale) || mCoordMgr->m_fScale <= 0 || mAvailableLevels.empty())
    {
        UnLoad();
        return;
    }
    // Choose resolution from local pixel spacing, not projected viewport corners.
    // The inverse projection can bend or wrap those corners at overview scales.
    double lon0, lat0, lonX, latX, lonY, latY;
    long desiredLevel = *mAvailableLevels.begin();
    const CPoint center = rect.CenterPoint();
    if (ScreenToGeo(center.x, center.y, lon0, lat0) &&
        ScreenToGeo(center.x + 1, center.y, lonX, latX) &&
        ScreenToGeo(center.x, center.y + 1, lonY, latY))
    {
        double dx = std::fabs(lonX - lon0);
        if (dx > 180.0) dx = 360.0 - dx;
        const double degreesPerPixel = std::max(dx, std::fabs(latY - lat0));
        desiredLevel = CalcLevel(0, 0, degreesPerPixel, 0, CRect(0, 0, 1, 1));
    }
    auto selected = mAvailableLevels.upper_bound(desiredLevel);
    if (selected != mAvailableLevels.begin()) --selected;
    const long level = *selected;
    const long rows = 1L << level;
    const long columns = rows * 2;
    const double tilesPerDegree = rows / 180.0;

    if (mRenderedImage.IsNull() || mRenderedImage.GetWidth() != rect.Width() ||
        mRenderedImage.GetHeight() != rect.Height())
    {
        mRenderedImage.Destroy();
        if (!mRenderedImage.Create(rect.Width(), rect.Height(), 32))
        {
            UnLoad();
            return;
        }
    }
    mRenderedRect = rect;
    MapImagesType used;
    std::set<CString> missing;
    long previousX = -1, previousY = -1;
    CImage* tile = NULL;
    // Inverse mapping writes every destination pixel once. Warped tile edges cannot
    // overlap, leave gaps, or use the wrong rectangular bounding box.
    for (int y = 0; y < rect.Height(); ++y)
    {
        BYTE* destination = static_cast<BYTE*>(mRenderedImage.GetPixelAddress(0, y));
        for (int x = 0; x < rect.Width(); ++x, destination += 4)
        {
            destination[0] = destination[1] = destination[2] = 10;
            destination[3] = 255;
            double longitude, latitude;
            if (!ScreenToGeo(rect.left + x, rect.top + y, longitude, latitude)) continue;
            const double tileX = (longitude + 180.0) * tilesPerDegree;
            const double tileY = (latitude + 90.0) * tilesPerDegree;
            const long indexX = std::min(columns - 1, static_cast<long>(std::floor(tileX)));
            const long indexY = std::min(rows - 1, static_cast<long>(std::floor(tileY)));
            if (indexX != previousX || indexY != previousY)
            {
                tile = UseTile(level, indexX, indexY, used, missing);
                previousX = indexX;
                previousY = indexY;
            }
            if (!tile) continue;
            const int sourceX = std::max(0, std::min(tile->GetWidth() - 1,
                static_cast<int>((tileX - indexX) * tile->GetWidth())));
            // TMS rows grow northwards; decoded image rows grow southwards.
            const int sourceY = std::max(0, std::min(tile->GetHeight() - 1,
                static_cast<int>((1.0 - (tileY - indexY)) * tile->GetHeight())));
            const BYTE* source = static_cast<const BYTE*>(tile->GetPixelAddress(sourceX, sourceY));
            destination[0] = source[0];
            destination[1] = source[1];
            destination[2] = source[2];
        }
    }
    for (auto& image : mImages) delete image.second;
    mImages.clear();
    mImages.swap(used);
}

void CTmsMgr::UnLoad()
{
    for (auto& image : mImages) delete image.second;
    mImages.clear();
    mRenderedImage.Destroy();
    mRenderedRect.SetRectEmpty();
}

void CTmsMgr::NeedLoad()
{
}

void CTmsMgr::Draw(CDC* dc)
{
    if (!dc || !dc->GetSafeHdc() || mRenderedImage.IsNull()) return;
    mRenderedImage.BitBlt(dc->GetSafeHdc(), mRenderedRect.left, mRenderedRect.top, SRCCOPY);
}
