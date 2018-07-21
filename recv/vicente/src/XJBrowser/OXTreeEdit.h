// ==========================================================================
// 					Class Specification : 
//						COXTreeEdit
// ==========================================================================

// Header file : COXTreeEdit.h

// Product Version: 7.52

// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                         
#ifndef _OXTREE_EDIT_H
#define _OXTREE_EDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CEdit

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

//Description:
// This is a small helper class designed for
//in-place label edit in COXTreeCtrl

class OX_CLASS_DECL COXTreeEdit : public CEdit
{
// Construction
public:
	int m_nDeltaY;
	int m_nDeltaX;
	void KeepBounds(CSize sz);
	//In: sz: specified bounded edit control
	void Init();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : performs initialization
	void KeepPos(CPoint pt, DWORD dwAlign);
	//In: pt: point to be aligned
	//		dwAlign: one of following ES_LEFT,ES_RIGHT,ES_CENTER
	//		specified edit control alignment

	COXTreeEdit();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : constructor

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTreeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXTreeEdit();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destructor

	// Generated message map functions
protected:
	CSize m_szBounds;
	BOOL m_bKeepBounds;
	DWORD m_dwAlign;
	CPoint m_pos;
	BOOL m_bKeepPos;
	//{{AFX_MSG(COXTreeEdit)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
