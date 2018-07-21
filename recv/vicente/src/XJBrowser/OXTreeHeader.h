// ==========================================================================
// 					Class Specification : 
//						COXTreeHeader
// ==========================================================================

// Header file : COXTreeHeader.h

// Product Version: 7.52

// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                         
#ifndef __OXTREE_HEADER_H
#define __OXTREE_HEADER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


/////////////////////////////////////////////////////////////////////////////
// COXTreeHeader window

class OX_CLASS_DECL COXTreeHeader : public CHeaderCtrl
{
// Construction
public:
	COXTreeHeader();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : constructor

// Attributes
public:

protected:
	int m_nSortCol;
	int m_nSortOrder;

	BOOL m_bLButtonDown;
// Operations
public:
	inline int GetSortCol() { return m_nSortCol; }
	// --- In  :
	// --- Out : 
	// --- Returns: number of column the tree control is sorted on
	// --- Effect : 
	inline int GetSortOrder() { return m_nSortOrder; }
	// --- In  :
	// --- Out : 
	// --- Returns: sorting order:	1	-	ascending
	//								0	-	none
	//								-1	-	descending
	// --- Effect : 

	BOOL SortColumn(int nCol, int nSortOrder);
	// --- In  :	nCol		-	number of column to sort on
	// --- Out :	nSortOrder	-	sorting order:	1	-	ascending
	//												0	-	none
	//												-1	-	descending
	// --- Returns: TRUE if success, or FALSE otherwise
	// --- Effect : 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTreeHeader)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXTreeHeader();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destructor

protected:
	virtual void  DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	// Generated message map functions
protected:
	//{{AFX_MSG(COXTreeHeader)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
