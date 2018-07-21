// ***************************************************************
//  PTSheet   version:  1.0    date: 10:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 保护属性sheet页, 继承于CTabView, 处理视图分页和管理
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_PTSHEET_H__3883B8F1_0024_4186_8C20_16B269BCE9D9__INCLUDED_)
#define AFX_PTSHEET_H__3883B8F1_0024_4186_8C20_16B269BCE9D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTSheet.h : header file
//
#include "TabView.h"

/////////////////////////////////////////////////////////////////////////////
// CPTSheet view

//##ModelId=49B87B8B008D
class CPTSheet : public CTabView
{
protected:
	//##ModelId=49B87B8B009D
	CPTSheet();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTSheet)

// Attributes
public:
	//##ModelId=49B87B8B009E
	BOOL	m_bPoor; //分辨率是否<1000
	//##ModelId=49B87B8B009F
	int		m_nType; //0表示是保护的属性页, 1表示是录波器的属性页

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTSheet)
	protected:
	//##ModelId=49B87B8B00A0
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//##ModelId=49B87B8B00A3
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//##ModelId=49B87B8B00AC
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8B00B1
	virtual ~CPTSheet();
#ifdef _DEBUG
	//##ModelId=49B87B8B00B3
	virtual void AssertValid() const;
	//##ModelId=49B87B8B00B5
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPTSheet)
	//##ModelId=49B87B8B00B8
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//##ModelId=49B87B8B00BD
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87B8B00C2
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSHEET_H__3883B8F1_0024_4186_8C20_16B269BCE9D9__INCLUDED_)
