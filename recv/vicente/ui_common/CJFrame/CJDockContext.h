// CJDockContext.h : header file
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
//			1.00	12 Jan 1999 - Initial creation to add side-by-side 
//								  docking support for CCJControlBar class.
//			1.01	03 Mar 1999 - Scott Killen [s_killen@yahoo.com] - If your 
//								  frame is not your main window in your app, 
//								  you cannot dock a floating CCJControlBar. 
//								  Changed (CFrameWnd*)AfxGetApp()->m_pMainWnd
//								  to (CFrameWnd*)m_pDockSite in CanDockDockBar()
//								  and DockSizeBar()
//			1.02	10 Mar 1999 - Un-commented toggle docking code.
// ==========================================================================
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CJDOCKCONTEXT_H__
#define __CJDOCKCONTEXT_H__

/////////////////////////////////////////////////////////////////////////////
// CCJDockContext class

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "StdAfx.h"

class CCJSizeDockBar;
class CCJControlBar;
//##ModelId=49B87BAA02E1
class CCJDockContext : public CDockContext
{
// Construction
public:
	//##ModelId=49B87BAA02EF
	CCJDockContext(CControlBar* pBar);

// Attributes
public:
protected:

// Operations
public:
	//##ModelId=49B87BAA02F1
	void EndDragDockBar();
	//##ModelId=49B87BAA02F2
	void MoveDockBar(CPoint pt);
	//##ModelId=49B87BAA02F4
	void StartDragDockBar(CPoint pt);
	//##ModelId=49B87BAA02FD
	DWORD CanDockDockBar();
	//##ModelId=49B87BAA02FE
	DWORD CanDockDockBar(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar = NULL);
	//##ModelId=49B87BAA0302
	BOOL TrackDockBar();
	//##ModelId=49B87BAA0303
	CCJSizeDockBar* GetDockBar(DWORD dwOverDockStyle);
	//##ModelId=49B87BAA0305
	void DockSizeBar(CControlBar *,CCJSizeDockBar* =NULL,LPRECT =NULL);
	//##ModelId=49B87BAA030F
	virtual void ToggleDocking();

// Implementation
public:
	//##ModelId=49B87BAA0311
	virtual ~CCJDockContext();
};

/////////////////////////////////////////////////////////////////////////////

#endif // __CJDOCKCONTEXT_H__
