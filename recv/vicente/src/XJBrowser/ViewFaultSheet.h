// ***************************************************************
//  ViewFaultSheet   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ÊÂ¹Ê±¨¸æTABÒ³
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWFAULTSHEET_H__F92871ED_B7C2_474D_82B2_4DE2497D00AB__INCLUDED_)
#define AFX_VIEWFAULTSHEET_H__F92871ED_B7C2_474D_82B2_4DE2497D00AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFaultSheet.h : header file
//
#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFaultSheet view

//##ModelId=49B87B8301CA
class CViewFaultSheet : public CTabView
{
public:
	//##ModelId=49B87B8301D5
	CViewFaultSheet();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFaultSheet)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFaultSheet)
	protected:
	//##ModelId=49B87B8301D6
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);      // overridden to draw this view
	//##ModelId=49B87B8301D9
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//##ModelId=49B87B8301DC
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B8301E6
	virtual ~CViewFaultSheet();
#ifdef _DEBUG
	//##ModelId=49B87B8301E8
	virtual void AssertValid() const;
	//##ModelId=49B87B8301EA
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewFaultSheet)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULTSHEET_H__F92871ED_B7C2_474D_82B2_4DE2497D00AB__INCLUDED_)
