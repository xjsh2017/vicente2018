////////////////////////////////////////////////////////////////
// Copyright 1998 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef _SUBCLASSW_H
#define _SUBCLASSW_H

//////////////////
// Generic class to hook messages on behalf of a CWnd.
// Once hooked, all messages go to CSubclassWnd::WindowProc before going
// to the window. Specific subclasses can trap messages and do something.
//
// To use:
//
// * Derive a class from CSubclassWnd.
//
// * Override CSubclassWnd::WindowProc to handle messages. Make sure you call
//   CSubclassWnd::WindowProc if you don't handle the message, or your
//   window will never get messages. If you write seperate message handlers,
//   you can call Default() to pass the message to the window.
//
// * Instantiate your derived class somewhere and call HookWindow(pWnd)
//   to hook your window, AFTER it has been created.
//	  To unhook, call HookWindow(NULL).
//
// This is a very important class, crucial to many of the widgets Window
// widgets implemented in PixieLib. To see how it works, look at the HOOK
// sample program.
//
//##ModelId=49B87B890053
class CSubclassWnd : public CObject {
public:
	DECLARE_DYNAMIC(CSubclassWnd);
	//##ModelId=49B87B89005E
	CSubclassWnd();
	//##ModelId=49B87B89005F
	~CSubclassWnd();

	// Subclass a window. Hook(NULL) to unhook (automatic on WM_NCDESTROY)
	//##ModelId=49B87B890060
	BOOL	HookWindow(HWND  hwnd);
	//##ModelId=49B87B890062
	BOOL	HookWindow(CWnd* pWnd)	{ return HookWindow(pWnd->GetSafeHwnd()); }
	//##ModelId=49B87B890064
	BOOL	IsHooked()					{ return m_hWnd!=NULL; }

	//##ModelId=49B87B89008C
	friend LRESULT CALLBACK HookWndProc(HWND, UINT, WPARAM, LPARAM);
	friend class CSubclassWndMap;

#ifdef _DEBUG
	//##ModelId=49B87B890065
	virtual void AssertValid() const;
	//##ModelId=49B87B89006E
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//##ModelId=49B87B890071
	HWND				m_hWnd;				// the window hooked
	//##ModelId=49B87B890072
	WNDPROC			m_pOldWndProc;		// ..and original window proc
	//##ModelId=49B87B890076
	CSubclassWnd*	m_pNext;				// next in chain of hooks for this window

	// Override this to handle messages in specific handlers
	//##ModelId=49B87B89007D
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
	//##ModelId=49B87B890082
	LRESULT Default();				// call this at the end of handler fns
};

#endif // _SUBCLASSW_H

