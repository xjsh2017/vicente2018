// CJControlBar.h : header file
//  
// DevStudio Style Resizable Docking Control Bar.  
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
//			1.01	20 Oct 1998	- Fixed problem with gripper and buttons  
//								  disappearing when docking toggled. Overloaded  
//								  IsFloating() method from base class.  
//			1.02    22 Nov 1998 - Modified set cursor to display normal size
//								  cursor when static linked.
//			2.00    12 Jan 1999 - Total class re-write, added multiple/side-by-side
//								  controlbar docking. No longer uses CSizingControlBar
//								  base class.
//			2.01    31 Jan 1999 - Removed extra line (typo) from OnLButtonUp().
//								  Thanks to Ioannis Stamatopoulos (ystamat@mail.datamedia.gr)
//							      for pointing this out.
//			2.02	28 Feb 1999 - Calls default wnd proc if no context menu is defined
//								  Gonzalo Pereyra [persys@adinet.com.uy]
//			2.03	10 Mar 1999 - Added AfxRegisterWndClass() to create method to
//								  handle double clicks. Thanks to Takehiko Mizoguti [mizoguti@m2.sys.to.casio.co.jp]
//								  for some thoughts on this.
//								- Fixed memory leak with button tooltips.
//			2.04	13 Mar 1999 - Patrick Bergeron [patb@softimage.com] fixed the
//								  following bugs:
//
//								- I had a dialog nested in a CJControlBar. Unfortunately 
//								  when I tried to dock it to the left or right, the 
//								  bottom part of my dialog would be clipped off. 
//
//								  Fix: When in docked mode, ::CalcDynamicLayout needed 
//								  to return the desired size, while taking into	account 
//								  the size of the gripper bar (if any). Simply added a 
//								  simple "+GetMinExt()" Now my dialog's bottom doesn't 
//								  get clipped. Woo-ha.
//									 
//								- When nesting a CDialog based window inside a 
//								  CJControlBar, the 3D rect drawn by ::OnEraseBkgnd would
//								  be overwritten by the top and left dialog borders. The 
//							      problem was caused by the fact that in the 
//								  ::OnWindowPosChanged() method, the rectangle which is 
//								  used to move the window contains the border. 
//									 
//								  A simple call to rc.DeflateRect(1,1) solved this problem.
//
//								- Added a call to UpdateWindow() in ::OnEraseBkgnd(). 
//								  This helps with the flickering a bit, but I Am not sure
//								  this is the correct way to go. Incorporate at your own risks.
//
//								- Added 2 ASSERT()s in ::OnMouseMove(). This was not to 
//								  fix a bug, but at one point I was seeing strange things
//								  that lead me to add these asserts. They don't do 
//								  anything bad, so why remove them?
//
// ==========================================================================  
//  
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJCONTROLBAR_H__
#define __CJCONTROLBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CJFlatButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCJControlBar class

//##ModelId=49B87BAA035B
class CCJControlBar : public CControlBar
{
	DECLARE_DYNAMIC(CCJControlBar)

// Construction
public:
	//##ModelId=49B87BAA035D
	CCJControlBar();

// Attributes
public:
	//##ModelId=49B87BAA036B
	BOOL			m_bUnique;
	//##ModelId=49B87BAA036C
	BOOL			m_bToFit;
protected:			
	//##ModelId=49B87BAA036D
	int				m_iTrackBorderSize;
	//##ModelId=49B87BAA036E
	int				m_cxOffset;
	//##ModelId=49B87BAA036F
	int				m_cyOffset;
	//##ModelId=49B87BAA037A
	UINT			m_iAuxImage;
	//##ModelId=49B87BAA037B
	UINT			m_menuID;
	//##ModelId=49B87BAA037C
	BOOL			m_bOkToDrag;
	//##ModelId=49B87BAA037D
	BOOL			m_bMaximized;
	//##ModelId=49B87BAA037E
	BOOL			m_bDragging;
	//##ModelId=49B87BAA038A
	BOOL			m_bGripper;
	//##ModelId=49B87BAA038B
	BOOL			m_bButtons;
	//##ModelId=49B87BAA038C
	CRect			m_rcTrack;
	//##ModelId=49B87BAA038D
	CSize			m_sizeNormal;
	//##ModelId=49B87BAA0399
	CSize			m_sizeDesired;
	//##ModelId=49B87BAA039A
	CPoint			m_ptCurDrag;
	//##ModelId=49B87BAA039B
	CPoint			m_ptStartDrag;
	//##ModelId=49B87BAA039C
	HCURSOR			m_curHSplitBar;
	//##ModelId=49B87BAA039D
	HCURSOR			m_curVSplitBar;
	//##ModelId=49B87BAA03AA
	CWnd*			m_pChildWnd;
	//##ModelId=49B87BAA03AF
	CImageList		m_ImageList;
	//##ModelId=49B87BAA03BC
	CCJFlatButton	m_btnClose;
	//##ModelId=49B87BAA03C9
	CCJFlatButton	m_btnMinim;
	//##ModelId=49B87BAA03CE
	CToolTipCtrl	m_ToolTip;
	//##ModelId=49B87BAA03D8
	COLORREF		m_clrBtnHilite;
	//##ModelId=49B87BAA03D9
	COLORREF		m_clrBtnShadow;
	//##ModelId=49B87BAB0000
	COLORREF		m_clrBtnFace;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJControlBar)
	public:
	//##ModelId=49B87BAB0001
	virtual BOOL Create(CWnd* pParentWnd, UINT nID, LPCTSTR lpszWindowName = NULL, CSize sizeDefault = CSize(200,100), DWORD dwStyle = CBRS_LEFT);
	//##ModelId=49B87BAB0013
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//##ModelId=49B87BAB001F
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	//##ModelId=49B87BAB002E
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
 	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87BAB003E
	virtual ~CCJControlBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCJControlBar)
	//##ModelId=49B87BAB0040
	afx_msg void OnNcPaint();
	//##ModelId=49B87BAB004E
	afx_msg void OnPaint();
	//##ModelId=49B87BAB0050
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=49B87BAB005D
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=49B87BAB006D
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=49B87BAB0071
	afx_msg UINT OnNcHitTest(CPoint point);
	//##ModelId=49B87BAB007F
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//##ModelId=49B87BAB008C
	afx_msg void OnSysColorChange();
	//##ModelId=49B87BAB008E
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//##ModelId=49B87BAB009C
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=49B87BAB009F
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//##ModelId=49B87BAB00AB
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=49B87BAB00AF
	afx_msg void OnButtonClose();
	//##ModelId=49B87BAB00BB
	afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
	//##ModelId=49B87BAB00BE
	afx_msg void OnButtonMinimize();
	//##ModelId=49B87BAB00C0
	afx_msg void OnUpdateButtonMinimize(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//##ModelId=49B87BAB00CB
	virtual void GetChildRect(CRect &rect);
	//##ModelId=49B87BAB00CE
	int GetMinExt();
	//##ModelId=49B87BAB00CF
	BOOL IsLeftDocked();
	//##ModelId=49B87BAB00D0
	BOOL IsRightDocked();
	//##ModelId=49B87BAB00D1
	BOOL IsTopDocked();
	//##ModelId=49B87BAB00DA
	BOOL IsBottomDocked();
	//##ModelId=49B87BAB00DB
	BOOL IsHorzDocked();
	//##ModelId=49B87BAB00DC
	BOOL IsVertDocked();
	//##ModelId=49B87BAB00DD
	void SetNormalSize(const CSize &cs);
	//##ModelId=49B87BAB00DF
	void Normalize();
	//##ModelId=49B87BAB00E0
	void Minimize();
	//##ModelId=49B87BAB00EA
	void Maximize(int size);
	//##ModelId=49B87BAB00EC
	void EnableDockingOnSizeBar(DWORD dwDockStyle);
	//##ModelId=49B87BAB00EE
	void ShowFrameControls(BOOL bGripper = TRUE, BOOL bButtons = TRUE);
	//##ModelId=49B87BAB00FB
	void SetMenuID(UINT nID);
	//##ModelId=49B87BAB00FD
	UINT GetMenuID();
protected:
	//##ModelId=49B87BAB00FE
	CRect GetGripperRect();
	//##ModelId=49B87BAB00FF
	CRect GetButtonRect();
	//##ModelId=49B87BAB0109
	void DrawGripper(CDC* pDC);
	//##ModelId=49B87BAB010B
	void SetChild(CWnd* pWnd);
	//##ModelId=49B87BAB010D
	void OnInvertTracker(const CRect& rect);
	//##ModelId=49B87BAB0119
	void HitTest(CPoint &point);
	//##ModelId=49B87BAB011B
	virtual void DrawBorders(CDC *pDC, CRect &rect);
	//##ModelId=49B87BAB011F
	void EraseNonClient();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CJCONTROLBAR_H__

