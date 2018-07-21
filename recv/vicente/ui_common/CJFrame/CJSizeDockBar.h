// CJSizeDockBar.h : header file
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
//			1.00	12 Jan 1999	- Initial release to add side-by-side 
//								  docking support for CCJControlBar class.
//			1.01	29 Jan 1999 - Made some cosmetic enhancements to more
//								  closely match DevStudio docking windows.
// ==========================================================================
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJSIZEDOCKBAR_H__
#define __CJSIZEDOCKBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCJSizeDockBar class
class CCJControlBar;
//##ModelId=49B87BAA0148
class CCJSizeDockBar : public CDockBar
{
	DECLARE_DYNAMIC(CCJSizeDockBar)

// Construction
public:
	//##ModelId=49B87BAA014A
	CCJSizeDockBar();

// Attributes
public:
protected:
	//##ModelId=49B87BAA0157
	int			m_iSafeSpace;
	//##ModelId=49B87BAA0158
	CPoint		m_ptCurDrag;
	//##ModelId=49B87BAA0159
	CRect		m_rcTrack;
	//##ModelId=49B87BAA015A
	CPoint		m_ptStartDrag;
	//##ModelId=49B87BAA0167
	BOOL		m_bDragging;
	//##ModelId=49B87BAA0168
	BOOL		m_bOkToDrag;
	//##ModelId=49B87BAA0169
	HCURSOR		m_curHSplitBar;
	//##ModelId=49B87BAA016A
	HCURSOR		m_curVSplitBar;
	//##ModelId=49B87BAA0177
	int			m_iTrackBorderSize;
	//##ModelId=49B87BAA0178
	int			m_iActualSize;
	//##ModelId=49B87BAA0179
	COLORREF	m_clrBtnHilite;
	//##ModelId=49B87BAA017A
	COLORREF	m_clrBtnShadow;
	//##ModelId=49B87BAA017B
	COLORREF	m_clrBtnFace;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCJSizeDockBar)
	public:
	//##ModelId=49B87BAA0186
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	//##ModelId=49B87BAA018A
	virtual void DoPaint(CDC *pDC);
	//##ModelId=49B87BAA0196
	virtual BOOL IsDockBar() const;
	//##ModelId=49B87BAA0198
	virtual BOOL IsDockSizeBar() const;
	//##ModelId=49B87BAA019A
	virtual void OnInvertTracker(const CRect &rect);
	//##ModelId=49B87BAA01A7
	virtual void HitTest(const CPoint& point);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87BAA01AA
	virtual ~CCJSizeDockBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCJSizeDockBar)
	//##ModelId=49B87BAA01B5
	afx_msg void OnNcPaint();
	//##ModelId=49B87BAA01B7
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//##ModelId=49B87BAA01C5
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=49B87BAA01CA
	afx_msg UINT OnNcHitTest(CPoint point);
	//##ModelId=49B87BAA01D4
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//##ModelId=49B87BAA01D8
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=49B87BAA01E4
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=49B87BAA01E8
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//##ModelId=49B87BAA01EA
	void Normalize(CCJControlBar*);
	//##ModelId=49B87BAA01F4
	void Maximize(CCJControlBar *pBar);
	//##ModelId=49B87BAA01F6
	void CalcSizeBarLayout();
	//##ModelId=49B87BAA01F7
	void SetActualSize(int iSize);
	//##ModelId=49B87BAA01F9
	void EraseNonClient();
	//##ModelId=49B87BAA01FA
	void DrawBorders(CDC *pDC,CRect& rect);
	//##ModelId=49B87BAA01FD
	void RecalcAllExcept(CCJSizeDockBar *pBar);
	//##ModelId=49B87BAA0203
	CCJControlBar* GetDockedSizeBar(int nPos);
protected:
	//##ModelId=49B87BAA0205
	BOOL IsLastControlBar(int index);
	//##ModelId=49B87BAA0207
	void GetAvailableRect(CRect &rect);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __CJSIZEDOCKBAR_H__

