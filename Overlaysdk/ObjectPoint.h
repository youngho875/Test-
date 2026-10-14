#ifndef OBJECT_POINT_H
#define OBJECT_POINT_H

#pragma once

#include "OverlayDefine.h"


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

class CCoordMgr; 

namespace OVERLAY
{

	class AFX_DATA CPointInt
	{
	public:
		int		Data;
		double* pData;
		CPointInt(double* p);
		~CPointInt();
	};

	class AFX_DATA CPointDouble
	{
	public:
		int* pData;
		double	Data;
		CPointDouble(int* p);
		~CPointDouble();
	};

	//static CCoordMgr* g_pCoord;
	class AFX_DATA Point
	{
	private:
		double lat;
		double lng;
		double	x;
		double	y;
	public:

		static CCoordMgr* g_pCoord;
		Point();
		~Point();

		double X();
		double Y();
		double Lat();
		double Lng();
		void SetLatLong(double dlat, double dlng);
		void SetXY(double dx, double dy);

		void operator=(Point& point);
		void ReSetPoint();
		void ReSetLatLong(double _lng, double _lat);
	};

}

#undef	AFX_DATA
#define	AFX_DATA

#endif