#pragma once

#undef	AFX_DATA
#ifdef	OVERLAY_IMPL
#define	AFX_DATA	_declspec(dllexport)
#else
#define	AFX_DATA	_declspec(dllimport)
#endif

namespace OVERLAY
{
	class AFX_DATA CObjectGroupInfo
	{
	public:
		CObjectGroupInfo();
		CObjectGroupInfo(UINT nMaxSize);
		~CObjectGroupInfo();

		UINT Size();
		BOOL AddObjectInGroup(UINT nID);
		UINT GetObjectID(UINT nIndex);

	private:
		UINT* m_ObjectArray;
		UINT m_MaxObjectInGroup;
		UINT m_nIndex;


	};
}

#undef	AFX_DATA
#define	AFX_DATA