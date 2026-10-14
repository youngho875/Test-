#pragma once

#include "ObjectProperty.h"
#include "ObjectPoint.h"
#include "Mathematics.h"
#include "../../inc/Useful.h"

#include <stack>

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//---------------------------------------
// 작성자			: 유영호
// 클래스명			: --
// 기능				: 도형 구조 정의
// 생성일			: 2024-10-07
// 최종수정자		: 유영호
// 최종수정일		: 2009-10-15
//---------------------------------------


#define INVALID_OBJECT_ID	-1
#define INVALID_VALUE		-1

namespace OVERLAY
{
	extern AFX_DATA char* strObjectType[];


	enum AFX_DATA OUT_LINE
	{
		LEFT_TOP,
		TOP_CENTER,
		RIGHT_TOP,
		RIGHT_CENTER,
		RIGHT_BOTTOM,
		BOTTOM_CENTER,
		LEFT_BOTTOM,
		LEFT_CENTER,
		ROTATE_CENTER,
		SWEEP_POSITION,
		SWEEP_ANGLE_1,
		SWEEP_ANGLE_2,
		MAX_OUT_LINE_POINT,
		OUT_LINE_SELECT,
		INVALIDATE_DATA,
		GENERAL_POINTS = 20,
	};

	enum AFX_DATA LINKED_LINE_POINTS
	{
		START_POINT,
		MID_POINT,
		END_POINT,
	};

	enum AFX_DATA OBJECT_TYPE
	{
		NULL_TYPE,
		CURVE,
		POINT_TYPE,
		LINE,
		TRIANGLE,
		RECT_TYPE,
		DIAMOND,
		ELLIPSE,
		POLYGON,
		PIE,
		POLYLINE,
		ROUND_RECT,
		TEXT,
		PENTAGON,
		SIX_ANGLE,
		EIGHT_ANGLE,
		IMAGE,
		ARC,
		LINKED_LINE,
		LINKED_MULTI_LINE,
		LINKED_CURVE,
		LINKED_MULTI_CURVE,
		OUTLINE,
		SYMBOL
	};

	enum AFX_DATA NUMBER_OF_POINTS
	{
		POINTS_OF_OBJECT_CURVE = MAX_POINTS,
		POINTS_OF_OBJECT_POINT = MAX_POINTS,
		POINTS_OF_OBJECT_LINKED_LINE = MAX_POINTS,
		POINTS_OF_OBJECT_LINE = 2,
		POINTS_OF_OBJECT_TRIANGLE = 3,
		POINTS_OF_OBJECT_RECT = 2,
		POINTS_OF_OBJECT_DIAMOND = 2,
		POINTS_OF_OBJECT_ELLIPSE = 2,
		POINTS_OF_OBJECT_POLYGON = MAX_POINTS,
		POINTS_OF_OBJECT_PIE = 4,
		POINTS_OF_OBJECT_POLYLINE = MAX_POINTS,
		POINTS_OF_OBJECT_ROUNDRECT = 3,
		POINTS_OF_OBJECT_TEXT = 2,
		POINTS_OF_OBJECT_PENTAGON = 2,
		POINTS_OF_OBJECT_SIXANGLE = 2,
		POINTS_OF_OBJECT_EIGHTANGLE = 2,
		POINTS_OF_OBJECT_IMAGE = 2,
		POINTS_OF_OBJECT_ARC = 4,
		POINTS_OF_OBJECT_OUTLINE = MAX_OUT_LINE_POINT,
	};


	//---------------------------------------
	// 작성자			: 유영호
	// 클래스명			: OBJECT_LINKED_LINE
	// 기능				: 도형간 연결선 관리
	// 생성일			: 2009-6-2
	// 최종수정자		: 유영호
	// 최종수정일		: 2009-10-15
	//---------------------------------------
	class AFX_DATA OBJECT_LINKED_LINE
	{
	public:
		//---------------------------------------
		// 함수명	: void AddObjectID(도형ID);
		// 기능		: 도형간 연결선 추가.
		//--------------------------------------
		void AddObjectID(UINT nID);
		//---------------------------------------
		// 함수명	: BOOL RemoveObjectID(도형ID);
		// 기능		: 도형간 연결선 제거.
		//--------------------------------------
		BOOL RemoveObjectID(UINT nID);
		//---------------------------------------
		// 함수명	: BOOL ExistLinkedLine();
		// 기능		: 도형간 연결선 유무 확인.
		// 반환		: 연결선이 존재함(TRUE), 연결선이 존재하지 않음(FALSE )
		//--------------------------------------
		BOOL ExistLinkedLine();
		//---------------------------------------
		// 함수명	: UINT CurrentLinkedLineCount();
		// 기능		: 도형간 연결선 수.	
		// 반환		: 도형간 연결선 수.
		//--------------------------------------
		UINT CurrentLinkedLineCount();
		//---------------------------------------
		// 함수명	: UINT GetBack();
		// 기능		: 도형에 연결된 연결선정보 중 마지막으로 추가된 도형ID 가져오기.
		// 반환		: 도형ID
		//--------------------------------------
		UINT GetBack();
		//---------------------------------------
		// 함수명	: UINT GetID(저장된 연결선정보 중 해당 인텍스);
		// 기능		: 도형간 연결선정보를 선택적으로 가져오기.
		// 반환		: 도형ID
		//--------------------------------------
		UINT GetID(UINT nIndex);
		//---------------------------------------
		// 함수명	: void RemoveAll();
		// 기능		: 도형간 연결선정보 모두제거	
		//--------------------------------------
		void RemoveAll();

		OBJECT_LINKED_LINE();
		~OBJECT_LINKED_LINE();
	private:
		// 도형 연결선 관련
		BOOL			bIsLinkedLine;							// 도형간 연결선 유무
		UINT			nLinkedLineObjectID[OBJECT_PER_LAYER];	// 도형한개가 최대 갖을수 있는 연결선의 개수		
		UINT			nCurrentLinkedLineCount;				// 현재 도형의 연결선 수

	};

	//---------------------------------------
	// 작성자			: 유영호
	// 클래스명			: OBJECT
	// 기능				: 도형정의, 도형속성설정, 도형편집 연산
	// 생성일			: 2009-6-2
	// 최종수정자		: 유영호
	// 최종수정일		: 2009-10-15
	//---------------------------------------
	class AFX_DATA OBJECT : public CMathematics
	{
	public:
		OBJECT(OBJECT_TYPE eType, UINT nNumberOfPoints);
		virtual ~OBJECT();
	protected:
		//---------------------------------------
		// 함수명	: void InitPoints();
		// 기능		: 도형 초기화.	
		//--------------------------------------
		void InitPoints();
		//---------------------------------------
		// 함수명	: void RotateOutLine();
		// 기능		: 도형 외곽선을 도형회전각에 해당하는 위치로 수정.	
		//--------------------------------------
		void RotateOutLine();
		//---------------------------------------
		// 함수명	: void RepositonSweepPoint();
		// 기능		: 삼각형, 모서리가 둥근사각형의 모양변경점 재계산.	
		//--------------------------------------
		void RepositonSweepPoint(double dPosition_x);
		//---------------------------------------
		// 함수명	: void RotatePoint(도형의 X중점, 도형의 Y중점, 지정된 X점, 지정된 Y점, 최전 처리 지점);
		// 기능		: 도형 중심으로 지정된 점을 회전각만큼 회전.
		//--------------------------------------
		void RotatePoint(double cx, double cy, double dRotateAngle, double in_x, double in_y, OVERLAY::Point& out_pt);
		//---------------------------------------
		// 함수명	: double GetDividedFace(도형의 X중점, 도형의 Y중점, 지정된 X점, 지정된 Y점);
		// 기능		: 화면좌표계와 직교좌표간의 지정점과 중점간의 각도 차이보정.
		// 반환		: 보정각
		//--------------------------------------
		double GetDividedFace(double x1, double y1, int x2, int y2);
		//---------------------------------------
		// 함수명	: void MoveRotatePoint(이동 지점);
		// 기능		: 도형 편집(회전)
		//--------------------------------------
		void MoveRotatePoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveTopLeftPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:좌상단).	
		//--------------------------------------
		void MoveTopLeftPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveTopRightPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:우하단).	
		//--------------------------------------
		void MoveTopRightPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveTopCenterPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:상단 중심).	
		//--------------------------------------
		void MoveTopCenterPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveLeftCenterPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:좌측 중심).		
		//--------------------------------------
		void MoveLeftCenterPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveRightCenterPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:우측 중심).			
		//--------------------------------------
		void MoveRightCenterPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveBottomLeftPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:좌하단).	
		//--------------------------------------
		void MoveBottomLeftPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveBottomRightPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:우하단).	
		//--------------------------------------
		void MoveBottomRightPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveBottomCenterPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:하단 중심).	
		//--------------------------------------
		void MoveBottomCenterPoint(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void MoveSweepPoint(이동 지점);
		// 기능		: 도형 편집(도형외곽선 편집:삼각형 또는 모서리가 둥근사각형의 모양 변경점).	
		//--------------------------------------
		void MoveSweepPoint(CPoint ptMove);
		//---------------------------------------	
		// 함수명	: void MoveSweepAngle1(이동 지점, 처리결과: 회전 각);
		// 기능		: 도형 편집(도형외곽선 편집:부채꼴 또는 호의 모양 변경점1).	
		//--------------------------------------
		void MoveSweepAngle1(CPoint ptMove, double& dSweepAngle);
		//---------------------------------------	
		// 함수명	: void MoveSweepAngle2(이동 지점, 처리결과: 회전 각);
		// 기능		: 도형 편집(도형외곽선 편집:부채꼴 또는 호의 모양 변경점2).	
		void MoveSweepAngle2(CPoint ptMove, double& dSweepAngle);
		//---------------------------------------
		// 함수명	: void MoveGeneralPoint(이동 지점, 선택된 점);
		// 기능		: 도형 편집(도형외곽선 편집:다각형, 선, 연결선, 꺽임선 등 점편집이 가능한 도형 선택된 점 편집).	
		//--------------------------------------
		void MoveGeneralPoint(CPoint ptMove, UINT nIndex);

	protected:
		OBJECT_TYPE		Type;
		UINT			nPoints;

		// 도형의 중점.
		double			m_cx;
		double			m_cy;

		//std::stack<UINT>			GroupID_Stack;
		UINT			nGroupID;

	public:
		SHORT			nID;
		ObjectProperty	Property;

		UINT				nPointIndex;
		OBJECT_LINKED_LINE	ObjectLinkedLineProperty;

		Point			pt_out_line[POINTS_OF_OBJECT_OUTLINE];
		Point			pt_old_out_line[POINTS_OF_OBJECT_OUTLINE];
		Point* pt;
		//---------------------------------------
		// 함수명	: double Angle(지정된 점);
		// 기능		: 도형의 중점을 기준으로 특정 지점의 각을 얻어냄.
		// 반환		: 각도.
		//--------------------------------------
		double Angle(Point reference_point);
		//---------------------------------------
		// 함수명	: void SetOldOutLinePosition();
		// 기능		: 도형 회전 편집시 연산오차 보정을 위한 편집전 도형 외곽선 저장
		//--------------------------------------
		void SetOldOutLinePosition();
		//---------------------------------------
		// 함수명	: void RearrangePoints();
		// 기능		: 도형 재정렬
		//--------------------------------------
		void RearrangePoints();
		//---------------------------------------
		// 함수명	: void EditPoint(이동 지점, 지정된 점);
		// 기능		: 다각형, 선, 꺽임선 등 점편집이 가능한 도형 편집.
		//--------------------------------------
		virtual void EditPoint(CPoint ptMove, INT nIndex);
		//---------------------------------------
		// 함수명	: void Move(이동 지점);
		// 기능		: 도형 이동
		//--------------------------------------
		virtual void Move(CPoint ptMove);
		//---------------------------------------
		// 함수명	: void Move(이동 지점, 선택된 점의 종류, 지정된 점);
		// 기능		: 도형 편집점(도형 외관선) 이동 편집
		//--------------------------------------
		virtual void Move(CPoint ptMove, OUT_LINE eKindOfSelectedPoint, UINT nIndex = 0);
		//---------------------------------------
		// 함수명	: void SetOutLine();
		// 기능		: 도형 외곽선 계산
		//--------------------------------------
		virtual void SetOutLine();
		//---------------------------------------
		// 함수명	: int GetPoints();
		// 기능		: 도형을 형성하고있는 점의개수
		//--------------------------------------
		int	GetPoints();
		//---------------------------------------
		// 함수명	: int GetAddedPointsNum();
		// 기능		: 도형에 추가될 점의 배열 인덱스
		//--------------------------------------
		int GetAddedPointsNum();
		//---------------------------------------
		// 함수명	: void AddPoints();
		// 기능		: 도형에 점 추가
		//--------------------------------------
		void AddPoints();
		//---------------------------------------
		// 함수명	: OBJECT_TYPE GetType();
		// 기능		: 도형의 종류
		//--------------------------------------
		OBJECT_TYPE	GetType();
		//---------------------------------------
		// 함수명	: void SetGroup(그룹 ID);
		// 기능		: 도형 그룹설정
		//--------------------------------------
		void SetGroup(UINT nGroupID);
		//---------------------------------------
		// 함수명	: void ReleaseGroup();
		// 기능		: 도형 그룹 해제
		//--------------------------------------
		void ReleaseGroup();
		//---------------------------------------
		// 함수명	: UINT GetGroupID();
		// 기능		: 도형의 그룹 ID 가져오기.
		// 반환		: 그룹 ID
		//--------------------------------------
		UINT GetGroupID();
		void SetPoints(UINT  input_nPoints);
	};
}

#undef	AFX_DATA
#define	AFX_DATA