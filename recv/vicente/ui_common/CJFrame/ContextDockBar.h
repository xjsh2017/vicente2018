#if !defined(AFX_CONTEXTDOCKBAR_H__082C63A1_1889_11D4_AF31_0080C8ECC8F7__INCLUDED_)
#define AFX_CONTEXTDOCKBAR_H__082C63A1_1889_11D4_AF31_0080C8ECC8F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CJ60Lib.h"
// ContextDockBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContextDockBar view

//##ModelId=49B87BA9039A
class CContextDockBar : public CSubclassWnd
{
public:
	//##ModelId=49B87BA9039C
	CContextDockBar();           // protected constructor used by dynamic creation
	//##ModelId=49B87BA903A9
	~CContextDockBar();
	//##ModelId=49B87BA903AA
	void SetContextMenu(UINT nMenuID);
	DECLARE_DYNCREATE(CContextDockBar)

protected:
	//##ModelId=49B87BA903AD
	CMenu	m_menu;
	//##ModelId=49B87BA903B1
	HWND	m_hWndSubClassed;

protected:
	//##ModelId=49B87BA903B2
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTEXTDOCKBAR_H__082C63A1_1889_11D4_AF31_0080C8ECC8F7__INCLUDED_)
