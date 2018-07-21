
// Product Version: 7.52

#if !defined(_OXITEMTIPWND_H_)
#define _OXITEMTIPWND_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


// generic constant used to define that none of custom color was set
#define ID_OX_COLOR_NONE				0xffffffff
	
#define ID_OXITEMTIP_TIMER				221
#define ID_OXITEMTIP_TIMER_DELAY		100

// COXItemTipWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COXItemTipWnd window. Class to display item tips. Window functionality is 
// similar to tool tip window. It is used by COXItemWnd class to display item
// tips. But it can be used directly by calling it's public functions. 
//
// To use this class you have to:
//
//	1)	call Create(CWnd* pParentWnd, HBRUSH hbrBackground=NULL) function. 
//		You can use hbrBackground argument to provide the background color you 
//		preffer
//
//	2)	call Display(CRect& rect, CString sText, int nOffset, 
//					int nAlignment=DT_LEFT, CFont* m_pFont=NULL, 
//					COLORREF clrText=ID_OX_COLOR_NONE, 
//					COLORREF clrBackground=ID_OX_COLOR_NONE)
//		function to display an item tip. You have to set the coordinates of 
//		item tip window using rect argument; text to be drawn is in sText;
//		offset from left and right sides of item tip window to draw text;
//		alignment of text: DT_LEFT, DT_CENTER or DT_RIGHT; font to draw text;
//		color to draw text and color of background.
//
//	3)	call Hide() to hide item tip window
//
//

class OX_CLASS_DECL COXItemTipWnd : public CWnd
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXItemTipWnd();

// Data
protected:
	// parent window
	CWnd* m_pParentWnd;

	// timer ID
	UINT m_nTimerID;

//////////////////////////////////////////////
// data for inner use


// Attributes
public:

// Operations
public:
	// --- In  :	pParentWnd		-	pointer to parent window
	//				hbrBackground	-	brush to be used to fill background
	// --- Out : 
	// --- Returns:	TRUE if item tip window was successfully created, or FALSE 
	//				otherwise
	// --- Effect : creates item tip window
	BOOL Create(CWnd* pParentWnd, HBRUSH hbrBackground=NULL);


	// --- In  :	rect			-	the size of item tip window
	//				sText			-	text to be drawn in item tip window
	//				nAlignment		-	allignment of text to draw, can be:
	//									DT_LEFT
	//									DT_CENTER
	//									DT_RIGHT
	//				pFont			-	font to draw text
	//				clrText			-	color to draw text
	//				clrBackground	-	color to fill item tip window background
	// --- Out : 
	// --- Returns:	
	// --- Effect : displays item tip 
	virtual void Display(CRect& rect, CString sText, int nOffset, 
		int nAlignment=DT_LEFT, CFont* pFont=NULL, 
		COLORREF clrText=ID_OX_COLOR_NONE, 
		COLORREF clrBackground=ID_OX_COLOR_NONE);

	
	// --- In  :	pMsg	-	pointer to MSG structure that can be sent to pWnd 
	//							on hiding item tip window
	//				pWnd	-	pointer to window to recieve pMsg
	// --- Out : 
	// --- Returns:	
	// --- Effect : hides item tip. Hiding can be caused by different reasons, 
	//				e.g by user clicking on item tip window. In that case you'd 
	//				be probably interested in sending WM_LBUTTONDOWN to parent 
	//				window, that is accomplished by using pMSG and pWnd arguments.
	virtual void Hide(MSG* pMsg=NULL, CWnd* pWnd=NULL);
	

	// retrieves coordinates of the monitor to which the specified point
	// (in screen coordinates) belongs
	static CRect GetMonitorRectFromPoint(const CPoint& ptHitTest, BOOL bOnlyWorkArea);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXItemTipWnd)
	public:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXItemTipWnd();

// implementation
protected:
	// helper function, determines if pWndChild is descendant of pWndParent
	BOOL IsDescendant(CWnd* pWndParent, CWnd* pWndChild);

// Generated message map functions
protected:
	//{{AFX_MSG(COXItemTipWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXITEMTIPWND_H_)
