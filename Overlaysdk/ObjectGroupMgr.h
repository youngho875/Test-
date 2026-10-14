#include "OverlayDefine.h"
#include <vector>
#include <map>

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

//-----------------------------------------------------------------------------------------------------------
// 클래스명 : CObjectGroupMgr
// 작성자	: 유영호
// 작성일	: 20080831
// 목적		: 도형그룹 관리
// 비고		: 도형간에 그룹관리를 위해 별도의 그룹 관리자가 필요하며 그룹 설정 및 해제 시 
//			  그룹관리자는 각각의 도형이 속해있는 그룹을 기록 관리한다.
//-----------------------------------------------------------------------------------------------------------
namespace OVERLAY
{
	class AFX_DATA CObjectGroupMgr
	{
	public:
		CObjectGroupMgr();
		~CObjectGroupMgr();

		typedef std::map<UINT, UINT>	SELECTED_OBJECT_FOR_GROUP;
		typedef std::vector<UINT>	OBJECT_GROUP_VECTOR;
		//---------------------------------------
		// 함수명	: BOOL AddObjectInGroup(그룹 ID, 도형 ID);
		// 목적		: 그룹에 도형 추가.
		// 반환		: 도형의 그룹 추가 성공 여부(TRUE: 성공,FALSE:실패)
		//--------------------------------------
		BOOL AddObjectInGroup(UINT nGroupID, UINT nObjectID);

	private:
		typedef std::vector<UINT>	OBJECT_GROUP_VECTOR;
		OBJECT_GROUP_VECTOR			m_ObjectGroupVector[OBJECT_PER_LAYER];
	};
}

#undef	AFX_DATA
#define	AFX_DATA