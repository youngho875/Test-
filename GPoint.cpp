// GPoint.cpp: implementation of the GPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "GPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GPoint::~GPoint()
{

}

GPoint& GPoint::operator=(const GPoint& _pt)
{	
	x = _pt.x;
	y = _pt.y;

	return *this;
}

GPoint& GPoint::operator-=(const GPoint& _pt)
{
	x = x - _pt.x;
	y = y - _pt.y;
	
	return *this;
}

GPoint& GPoint::operator+=(const GPoint& _pt)
{
	x = x + _pt.x;
	y = y + _pt.y;
	
	return *this;
}
GPoint GPoint::operator+(const GPoint & _pt) const
{
	GPoint sum;
	sum.x = x + _pt.x;
	sum.y = y + _pt.y;
	return sum;
}

GPoint GPoint::operator-(const GPoint & _pt) const
{
	GPoint diff;
	diff.x = x - _pt.x;
	diff.y = y - _pt.y;
	return diff;
}


bool GPoint::operator==(const GPoint& _pt)
{
	return ((x == _pt.x) && (y == _pt.y));
}

bool GPoint::operator!=(const GPoint& _pt)
{
	return !(*this == _pt);
}

double 	GPoint::dist2Point(const GPoint& _pt)	
{
	return sqrt(pow(x - _pt.x, 2) + pow(y - _pt.y, 2));
}

GPoint GPoint::operator *(float _ff) const
{
	GPoint mult;
	mult.x = _ff * x;
	mult.y = _ff * y;
	return mult;
}

GPoint operator*(float _ff, const GPoint & _pt)
{
	return _pt * _ff;
}