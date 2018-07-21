// COXHookWnd is a generic class for hooking another window's messages.
//
// Product Version: 7.52

#include "StdAfx.h"
#include "OXHookWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This trick is used so the hook map isn't
// instantiated until someone actually requests it.
//
#define	theHookMap (COXHookWndMap::GetHookMap())

////////////////////////////////////////////////////////////////
// COXHookWndMap implementation

COXHookWndMap::~COXHookWndMap()
{
	// all hooks should be removed!	
	ASSERT(IsEmpty());	
}

//////////////////
// Get the one and only global hook map
// 
COXHookWndMap& COXHookWndMap::GetHookMap()
{
	// By creating theMap here, C++ doesn't instantiate it until/unless
	// it's ever used! This is a good trick to use in C++, to
	// instantiate/initialize a static object the first time it's used.
	//
	static COXHookWndMap theMap;
	return theMap;
}

/////////////////
// Add hook to map; i.e., associate hook with window
//
void COXHookWndMap::Add(HWND hWnd, COXHookWnd* pHook)
{
	ASSERT(hWnd && ::IsWindow(hWnd));

	// Add to front of list
	pHook->m_pNext=Lookup(hWnd);
	SetAt(hWnd,pHook);
	
	if (pHook->m_pNext==NULL) 
	{
		// If this is the first hook added, subclass the window
		pHook->m_pOldWndProc=
			(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)HookWndProc);
	} 
	else 
	{
		// just copy wndproc from next hook
		pHook->m_pOldWndProc=pHook->m_pNext->m_pOldWndProc;
	}

	ASSERT(pHook->m_pOldWndProc);
}

//////////////////
// Remove hook from map
//
void COXHookWndMap::Remove(COXHookWnd* pUnHook)
{
	HWND hWnd=pUnHook->m_hWndHooked;
	ASSERT(hWnd && ::IsWindow(hWnd));

	COXHookWnd* pHook=Lookup(hWnd);
	ASSERT(pHook);
	if(pHook==pUnHook) 
	{
		// hook to remove is the one in the hash table: replace with next
		if(pHook->m_pNext)
		{
			SetAt(hWnd,pHook->m_pNext);
		}
		else 
		{
			// This is the last hook for this window: restore wnd proc
			RemoveKey(hWnd);
			SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)pHook->m_pOldWndProc);
		}
	} 
	else 
	{
		// Hook to remove is in the middle: just remove from linked list
		while(pHook->m_pNext!=pUnHook)
		{
			pHook=pHook->m_pNext;
		}
		ASSERT(pHook && pHook->m_pNext==pUnHook);
		pHook->m_pNext=pUnHook->m_pNext;
	}

}

//////////////////
// Remove all the hooks for a window
//
void COXHookWndMap::RemoveAll(HWND hWnd)
{
	COXHookWnd* pHook=Lookup(hWnd);
	ASSERT(pHook);

	while(pHook)
	{
		// unhook window
		// in COXHookWnd::UnhookWnd we call Remove function
		pHook->UnhookWindow();	
		// if the window was hooked more than once, get next COXHookWnd object
		pHook=Lookup(hWnd);
	}
}

/////////////////
// Find first hook associate with window
//
COXHookWnd* COXHookWndMap::Lookup(HWND hWnd) const
{
	COXHookWnd* pFound=NULL;
	if(hWnd == NULL)
	{
		return NULL;
	}
	if (CMapPtrToPtr::Lookup(hWnd,(void*&)pFound))
	{
		ASSERT_KINDOF(COXHookWnd,pFound);
	}
	return pFound;
}

///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//	COXHookWnd implementation

IMPLEMENT_DYNAMIC(COXHookWnd, CWnd);

COXHookWnd::COXHookWnd()
{
	m_pNext=NULL;
	m_pOldWndProc=NULL;	
	m_hWndHooked=NULL;

	m_nCheckMouseTimerID=0;
	m_bMouseIsOver=FALSE;
}

COXHookWnd::~COXHookWnd()
{
	if(m_hWndHooked!=NULL)
	{
		TRACE(_T("Unhooking window in destructor: WM_DESTROY and WM_NCDESTROY messages won't be handled\n"));
		UnhookWindow();
	}
	// can't destroy while still hooked!
	ASSERT(m_hWndHooked==NULL);		
	ASSERT(m_pOldWndProc==NULL);
}

//////////////////
// Hook a window.
// This installs a new window proc that directs messages to the COXHookWnd.
//
void COXHookWnd::HookWindow(CWnd* pWnd)
{
	ASSERT_VALID(pWnd);

	RemoveSpy();

	// Hook the window
	ASSERT(m_hWndHooked==NULL);
	HWND hWnd=pWnd->m_hWnd;
	ASSERT(hWnd && ::IsWindow(hWnd));
	// Add to map of hooks
	theHookMap.Add(hWnd, this);			
	m_hWndHooked=hWnd;
}

//////////////////
// Unhook a window.
//
void COXHookWnd::UnhookWindow()
{
	ASSERT(m_hWndHooked!=NULL);

	RemoveSpy();

	// Remove from map
	theHookMap.Remove(this);				

	m_pOldWndProc=NULL;
	m_hWndHooked=NULL;
}

BOOL COXHookWnd::InstallSpy()
{
	if(!IsHooked())
	{
		TRACE(_T("COXHookWnd::InstallSpy: there's no hooked window"));
		return FALSE;
	}
	else
	{
		m_nCheckMouseTimerID=GetHookedWnd()->SetTimer(
			ID_CHECKMOUSETIMER,DFLT_CHECKMOUSEDELAY,NULL);
		if(m_nCheckMouseTimerID==0)
		{
			TRACE(_T("COXHookWnd::InstallSpy: failed to set timer"));
			return FALSE;
		}
	}

	return TRUE;
}

void COXHookWnd::RemoveSpy()
{
	if(m_nCheckMouseTimerID!=0)
	{
		ASSERT(m_hWndHooked!=NULL);
		ASSERT(::IsWindow(m_hWndHooked));

		GetHookedWnd()->KillTimer(m_nCheckMouseTimerID);
	}

	m_nCheckMouseTimerID=0;
	m_bMouseIsOver=FALSE;
}

void COXHookWnd::CheckMousePos()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	// Check whether the mouse is over the hooked window
	CPoint point;
	::GetCursorPos(&point);
	CWnd* pWnd=CWnd::WindowFromPoint(point);
	BOOL bIsMouseOver=(pWnd->GetSafeHwnd()==m_hWndHooked);

	if(bIsMouseOver!=m_bMouseIsOver)
	{
		m_bMouseIsOver=bIsMouseOver;
		GetHookedWnd()->PostMessage(
			(m_bMouseIsOver ? HWM_MOUSEENTER : HWM_MOUSELEAVE));
	}
}	

//////////////////
// Window proc-like virtual function which specific COXHookWnd-derived class
// will override to do stuff. Default passes the message to the next hook; 
// the last hook passes the message to the original window.
// You MUST call this at the end of your WindowProc if you want the real
// window to get the message. This is just like CWnd::WindowProc, except that
// a COXHookWnd is not a window.
//
LRESULT COXHookWnd::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(m_pOldWndProc);

	if(msg==WM_TIMER)
	{
		if(m_nCheckMouseTimerID==(UINT)wp)
			CheckMousePos();
	}
	else if(!m_bMouseIsOver && msg==WM_MOUSEMOVE)
	{
		CheckMousePos();
	}

	LRESULT lResult=(m_pNext ? m_pNext->WindowProc(msg, wp, lp) :	
		::CallWindowProc(m_pOldWndProc,m_hWndHooked,msg,wp,lp));

	return lResult;
}

//////////////////
// Like calling base class WindowProc, but with no args, so individual
// message handlers can do the default thing. Like CWnd::Default
//
LRESULT COXHookWnd::Default()
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// MFC stores current MSG in thread state
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	// Note: must explicitly call COXHookWnd::WindowProc to avoid infinte
	// recursion on virtual function
	return COXHookWnd::WindowProc(curMsg.message, curMsg.wParam, curMsg.lParam);
}

//////////////////
// Subclassed window proc for message hooks. Replaces AfxWndProc (or whatever
// else was there before.)
//
LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// Set up MFC message state just in case anyone wants it
	// This is just like AfxCallWindowProc, but we can't use that because
	// a COXHookWnd is not a CWnd.
	//
	MSG& curMsg=AfxGetThreadState()->m_lastSentMsg;
	// save for nesting
	MSG oldMsg=curMsg;   
	curMsg.hwnd=hWnd;
	curMsg.message=msg;
	curMsg.wParam=wp;
	curMsg.lParam=lp;

	// Get hook object for this window. Get from hook map
	COXHookWnd* pHook=theHookMap.Lookup(hWnd);
	ASSERT(pHook);

	LRESULT lr;
	if (msg==WM_NCDESTROY) 
	{
		// Window is being destroyed: unhook all hooks (for this window)
		// and pass msg to orginal window proc
		//
		WNDPROC wndproc=pHook->m_pOldWndProc;
		theHookMap.RemoveAll(hWnd);
		lr=::CallWindowProc(wndproc, hWnd, msg, wp, lp);

	} 
	else 
	{
		// pass to msg hook
		lr=pHook->WindowProc(msg,wp,lp);
	}

	// pop state
	curMsg=oldMsg;			

	return lr;
}

