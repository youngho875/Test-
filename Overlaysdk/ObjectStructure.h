#pragma once

#include "OverlayDefine.h"
#include <map>


#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif
//---------------------------------------
// 작성자			: 유영호
// 클래스명			: ObjectStructure
// 목적				: Object 자료구조
// 생성일			: 2024-10-07
// 최종수정자		:
// 최종수정일		:
//---------------------------------------

namespace OVERLAY
{
	template <typename OBJECT>
	class AFX_DATA CObjectStructure
	{
	public:
		CObjectStructure()
		{
			m_nMaxObject = MAX_OBJECT;
		}
		~CObjectStructure()
		{
			m_ObjectMap.clear();
		}

		typedef std::map<UINT, OBJECT>	OBJECT_MAP;

		UINT		m_nMaxObject;

		OBJECT_MAP	m_ObjectMap;

		// 1. Object 추가
		BOOL AddObject(UINT nID, OBJECT Object)
		{
			BOOL ret = TRUE;
			m_ObjectMap[nID] = Object;

			return ret;
		};

		// 2. Object 제거
		BOOL DeleteObject(UINT nID)
		{
			BOOL ret = TRUE;
			m_ObjectMap.erase(nID);

			return TRUE;
		};

		// 3.
		OBJECT Get_Object(UINT nID)
		{
			if (nID <= 0 || nID >= MAX_OBJECT)
				return NULL;
			return m_ObjectMap[nID];
		}

		BOOL ClearObject()
		{// 배열로 생성된 데이터의 포인터만 참고한다.
			BOOL ret = TRUE;
			m_ObjectMap.clear();
			return ret;
		};
	};
}

#undef	AFX_DATA
#define	AFX_DATA