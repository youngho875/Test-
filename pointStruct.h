#pragma once

#include <vector>

using namespace std;

//data structure for points
typedef struct MyPoint2D
{
	double dX;
	double dY;
}MyPoint2D;

//Holds Coordinates of Point Shapefile
//vector<MyPoint2D>PointLayer;

//data structure for a multipoint feature
typedef struct MultipointFeature
{
	vector<MyPoint2D>PointsOfFeature;
}MultipointFeature;

//Holds Coordinates of multiPoint Shapefile
//vector<MultipointFeature> MultipointLayer;

//data structure for lines
typedef struct MyLine2D
{
	vector<MyPoint2D> LineString;
}MyLine2D;

//data structure for a line feature
typedef struct LineFeature
{
	vector<MyLine2D> LinesOfFeature;
}LineFeature;

//Holds Coordinates of Line Shapefile
//vector<LineFeature> LineLayer;

//data structure for rings
typedef struct MyRing2D
{
	vector<MyPoint2D> RingString;
	bool IsClockwised;
}MyRing2D;

//data structure for polygons
typedef struct MyPolygon2D
{
	vector<MyRing2D> Polygon;
}MyPolygon2D;

//data structure for a polygon feature
typedef struct PolygonFeature
{
	vector<MyPolygon2D> PolygonsOfFeature;
}PolygonFeature;

//Holds Coordinates of Polygon Shapefile
//vector<PolygonFeature> PolygonLayer;

//data structure to hold bounding box
typedef struct SBoundingBox
{
	float fMaxX;
	float fMaxY;
	float fMinX;
	float fMinY;
}SBoundingBox;

//Bounding Box of Shapefile 
///SBoundingBox sBoundingBox;