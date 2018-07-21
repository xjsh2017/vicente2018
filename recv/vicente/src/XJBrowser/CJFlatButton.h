// CJFlatButton.h : header file
//
// Copyright © 1998-99 Kirk Stowell   
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This source code may be used in compiled form in any way you desire. 
// Source file(s) may be redistributed unmodified by any means PROVIDING
// they are not sold for profit without the authors expressed written consent,
// and providing that this notice and the authors name and all copyright
// notices remain intact. If the source code is used in any commercial
// applications then a statement along the lines of:
//
// "Portions Copyright © 1998-99 Kirk Stowell" must be included in the
// startup banner, "About" box or printed documentation. An email letting
// me know that you are using it would be nice as well. That's not much to ask
// considering the amount of work that went into this.
//
// This software is provided "as is" without express or implied warranty. Use
// it at your own risk! The author accepts no liability for any damage/loss of
// business that this product may cause.
//
// ==========================================================================  
//
// Acknowledgements:
//	<>  Many thanks to all of you, who have encouraged me to update my articles
//		and code, and who sent in bug reports and fixes.
//  <>  Many thanks Zafir Anjum (zafir@codeguru.com) for the tremendous job that
//      he has done with codeguru, enough can not be said!
//	<>  Many thanks to Microsoft for making the source code availiable for MFC. 
//		Since most of this work is a modification from existing classes and 
//		methods, this library would not have been possible.
//
// ==========================================================================  
// HISTORY:	
// ==========================================================================
//			1.00	17 Oct 1998	- Initial re-write and release.
//			1.01	02 Jan 1999 - Code clean up and re-organization, renamed
//								  CCJButton to CCJFlatButton.
//			1.02	13 Mar 1999 - Patrick Bergeron [patb@softimage.com] fixed the
//								  following bugs:
//
//								- Moving the cursor over the flat button doesn't 
//								  raise the button right away. We have to wait for 
//								  the first timer message to arrive, which is a bit 
//								  annoying. It gives the impression that the system 
//								  isn't very responsive.
//
//								  So in the OnMouseMove method, we call OnTimer(1) 
//								  to force the initial update of the button.
//
//								- Sometimes, a CJFlatButton would stay in a raised 
//								  state when moving the mouse quickly over the client 
//								  area of a few CJFlatButtons. The reason turned out 
//								  to be the use of a static (ahem, global) variable 
//								  "bPainted". The simple fix for this was to put 
//								  bPainted as a member variable ("m_bPainted") and use
//								  this member variable instead.
//
//								  Flat buttons are now much more responsive and behave 
//								  correctly in all cases.
// ==========================================================================
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJFLATBUTTON_H__
#define __CJFLATBUTTON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCJFlatButton window

//##ModelId=49B87BAA0290
class CCJFlatButton : public CButton
{
// Construction
public:
	//##ModelId=49B87BAA0292
	CCJFlatButton();

// Attributes
public:
protected:
	//##ModelId=49B87BAA0293
	UINT		m_nState;
	//##ModelId=49B87BAA0294
	CRect		m_rcItem;
	//##ModelId=49B87BAA029F
	BOOL		m_bLBtnDown;
	//##ModelId=49B87BAA02A0
	BOOL		m_bFlatLook;
	//##ModelId=49B87BAA02A1
	CSize		m_sizeIcon;
	//##ModelId=49B87BAA02A2
	COLORREF	m_clrHilite;
	//##ModelId=49B87BAA02A3
	COLORREF	m_clrShadow;
	//##ModelId=49B87BAA02A4
	COLORREF	m_clrDkShad;
	//##ModelId=49B87BAA02AF
	COLORREF	m_clrNormal;
	//##ModelId=49B87BAA02B0
	COLORREF	m_clrTextGy;
	//##ModelId=49B87BAA02B1
	COLORREF	m_clrTextNm;
	//##ModelId=49B87BAA02B2
	BOOL		m_bPainted;
// Operations
public:
	//##ModelId=49B87BAA02BF
	void DisableFlatLook() { m_bFlatLook = FALSE; }
	//##ModelId=49B87BAA02C0
	void SetIcon(HICON hIcon, CSize size);
protected:
	//##ModelId=49B87BAA02C3
	void DrawIcon(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJFlatButton)
	public:
	//##ModelId=49B87BAA02C5
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87BAA02C8
	virtual ~CCJFlatButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCJFlatButton)
	//##ModelId=49B87BAA02CA
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=49B87BAA02D1
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=49B87BAA02D5
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=49B87BAA02D9
	afx_msg void OnTimer(UINT nIDEvent);
	//##ModelId=49B87BAA02DF
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CJFLATBUTTON_H__
