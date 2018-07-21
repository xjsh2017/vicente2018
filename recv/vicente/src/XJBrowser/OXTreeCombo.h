
// Product Version: 7.52

#ifndef __OXTREE_COMBO_H
#define __OXTREE_COMBO_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXTreeItem.h"


// ==========================================================================
// 					Class Specification : 
//						COXTreeCombo
// ==========================================================================

// Header file : COXTreeCal.h

// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CComboBox

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

//Description:
// This is a small helper class designed for
//in-place combobox in COXTreeCtrl

class OX_CLASS_DECL COXTreeCombo : public CComboBox
{
// Construction
public:
	COXTreeCombo();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : constructor

// Attributes
public:

// Operations
public:
	void Init(HTREEITEM hItem,int iItem,int iSubItem);
	// --- In  :	hItem - Specifies handle to the item
	//				iItem - Specifies the zero-based index of the item 
	//				iSubItem - Specifies the index of column
	// --- Out : 
	// --- Returns :
	//--- Effect : performs initialization

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTreeCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXTreeCombo();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destructor

	// Generated message map functions
protected:
	BOOL m_bNotifySent;
	void FinishEdit(BOOL bOK);
	int m_nSubItem;
	int m_nItem;
	//{{AFX_MSG(COXTreeCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
