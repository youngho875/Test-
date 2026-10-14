// GPoint.h: interface for the GPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPOINT_H__008FAFCF_8248_4112_BDD2_49078743B52B__INCLUDED_)
#define AFX_GPOINT_H__008FAFCF_8248_4112_BDD2_49078743B52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/* Container class for Geo Point
 * Coded by Scene, Sanghee(shshin@gaia3d.com)
 * 2001-10-10
*/
class GPoint  
{
	//attibute
public:
	double x;
	double y;

public:
	GPoint(const CPoint& _pt):x(_pt.x), y(_pt.y){};
	GPoint(double _x, double _y):x(_x), y(_y){};
	GPoint(const GPoint& _pt):x(_pt.x), y(_pt.y){};
	GPoint():x(0.),y(0.){};

	virtual ~GPoint();

	// simple operators
	GPoint& operator= (const GPoint& _pt);
	GPoint& operator-=(const GPoint& _pt);
	GPoint& operator+=(const GPoint& _pt);
	GPoint operator-(const GPoint& _pt) const; 
	GPoint operator+(const GPoint& _pt) const; 
	GPoint operator*(float _ff) const;
	friend GPoint operator*(float _ff, const GPoint & _pt);

	
	bool operator==(const GPoint& _pt);
	bool operator!=(const GPoint& _pt);
	
	// distance from a point to another
	double dist2Point(const GPoint& _pt);	
};

#endif // !defined(AFX_GPOINT_H__008FAFCF_8248_4112_BDD2_49078743B52B__INCLUDED_)
