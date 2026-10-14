#pragma once


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: CMathematics
// 기능				: 
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

#ifdef M_PI_ 
#undef M_PI_ 
#endif

#define M_PI_ 3.1415926535897932384626433832795

//extern const double MATH_PI;
//extern const double MATH_DEGREE_TO_RADIAN;
//extern const double MATH_RADIAN_TO_DEGREE;

namespace OVERLAY
{
	AFX_DATA extern const double MATH_PI;
	AFX_DATA extern const double MATH_DEGREE_TO_RADIAN;
	AFX_DATA extern const double MATH_RADIAN_TO_DEGREE;

	class AFX_DATA CMathematics
	{
	public:
		CMathematics();
		~CMathematics();

	private:
		double m_dRadianToDegree;
		double m_dDegreeToRadian;

	public:
		double Radian2Degree(double dRadian);
		double Degree2Radian(double Degree);
		double Speed(double xVel, double yVel);
		double CourseSpeedToX(double dCourse, double dSpeed);
		double CourseSpeedToY(double dCourse, double dSpeed);
		void   CourseSpeedToXY(double dCourse, double dSpeed, double* dVelx, double* dVely);
		double Course(double xVel, double yVel);
		double CourseRadian(double xVel, double yVel);
		double Distance(double x1, double y1, double x2, double y2);
		void   ReferencePositon_Distance_Angle2TargetPosition(double dDistance, double dAngle, double dReferencePositionX, double dReferencePositionY, double* dTargetX, double* dTargetY);
		void   ReferencePositon_Radius_Angle2TargetPosition(double dRadius, double dAngle, double dReferencePositionX, double dReferencePositionY, double* dTargetX, double* dTargetY);
	};

	//extern const double MATH_PI;
	//extern const double MATH_DEGREE_TO_RADIAN;
	//extern const double MATH_RADIAN_TO_DEGREE;
}

#undef	AFX_DATA
#define	AFX_DATA

