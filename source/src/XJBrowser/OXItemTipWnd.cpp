// OXItemTipWnd.cpp : implementation file
//
// Product Version: 7.52


#include "stdafx.h"
#include <winuser.h>
#include <windowsx.h>
#include "OXItemTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COXItemTipWnd

COXItemTipWnd::COXItemTipWnd()
{
	m_nTimerID=0;
}


COXItemTipWnd::~COXItemTipWnd()
{
}


BEGIN_MESSAGE_MAP(COXItemTipWnd, CWnd)
	//{{AFX_MSG_MAP(COXItemTipWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void COXItemTipWnd::PostNcDestroy() 
{
	DestroyWindow();
	CWnd::PostNcDestroy();
}


BOOL COXItemTipWnd::Create(CWnd* pParentWnd, HBRUSH hbrBackground)
{
	ASSERT_VALID(pParentWnd);
    ASSERT(::IsWindow(pParentWnd->GetSafeHwnd()));

	m_pParentWnd=pParentWnd;

	// creation of window
    // 
	if(hbrBackground==NULL)
	{
		hbrBackground=(HBRUSH)(COLOR_INFOBK+1);
	}

	WNDCLASS wndClass;
	wndClass.style=CS_SAVEBITS|CS_DBLCLKS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hCursor=0; 
    wndClass.hbrBackground=hbrBackground; 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("ItemTipWnd");
	
	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	CRect rect(0,0,0,0);
    if(!CWnd::CreateEx(WS_EX_TOOLWINDOW,wndClass.lpszClassName,_T(""), 
		WS_BORDER|WS_POPUP,rect,NULL,0,NULL))
	{
        return FALSE;
	}

    return TRUE;
}


void COXItemTipWnd::Display(CRect& rect, CString sText, int nOffset, int nAlignment,
							CFont* pFont, COLORREF clrText, COLORREF clrBackground)
{
	ASSERT_VALID(m_pParentWnd);
    ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	ASSERT(::IsWindow(m_hWnd));

	CWnd* pWnd=GetActiveWindow();
	if(pWnd==NULL || (!IsDescendant(pWnd,m_pParentWnd) && 
		(m_pParentWnd->GetExStyle()&WS_EX_TOPMOST)!=WS_EX_TOPMOST))
	{
		return;
	}

	CClientDC dc(this); // device context for drawing
	
	CFont* pOldFont=NULL;
	if(pFont)
	{
		pOldFont=dc.SelectObject(pFont);
	}
	COLORREF clrOld=ID_OX_COLOR_NONE;
	if(clrText!=ID_OX_COLOR_NONE)
	{
		clrOld=dc.SetTextColor(clrText);
	}
	int nOldBkMode=dc.SetBkMode(TRANSPARENT);

	CRect rectDraw=rect;
	dc.DrawText(sText,&rectDraw,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_NOPREFIX);
	rectDraw.right+=2*nOffset;
	if(rectDraw.Width()>rect.Width())
	{
		rect.right=rect.left+rectDraw.Width();
	}
	if(rectDraw.Height()>rect.Height())
	{
		rect.bottom=rect.top+rectDraw.Height();
	}

	// it's show time!
	m_pParentWnd->ClientToScreen(&rect);

	// adjust rectangle to fit the screen
	//
	CWnd* pParentWnd=m_pParentWnd;
	BOOL bTopMostParent=FALSE;
	while(pParentWnd!=NULL)
	{
		if(pParentWnd->GetExStyle() & WS_EX_TOPMOST)
		{
			bTopMostParent=TRUE;
			break;
		}
		pParentWnd=pParentWnd->GetParent();
	}

	DWORD dwMessagePos=::GetMessagePos();
	CPoint point(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
	CRect rectDisplay=GetMonitorRectFromPoint(point,!bTopMostParent);

	if(rect.right>rectDisplay.right)
	{
		rect.OffsetRect(rectDisplay.right-rect.right,0);
	}
	if(rect.left<rectDisplay.left)
	{
		rect.OffsetRect(rectDisplay.left-rect.left,0);
	}
	//
	/////////////////////////////////////////////////////////////

	if(bTopMostParent)
	{
		SetWindowPos(&wndTopMost,rect.left,rect.top,rect.Width(),
			rect.Height(),SWP_NOACTIVATE);
	}
	else
	{
		SetWindowPos(NULL,rect.left,rect.top,rect.Width(),
			rect.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	ShowWindow(SW_SHOWNA);

	if(clrBackground!=ID_OX_COLOR_NONE)
	{
		CBrush brush;
		brush.CreateSolidBrush(clrBackground);
		CRect rectClient;
		GetClientRect(rectClient);
		dc.FillRect(&rectClient,&brush);
	}
	else
	{
		SendMessage(WM_ERASEBKGND,(WPARAM)(HDC)dc);
	}

	rectDraw=rect;
	rectDraw.DeflateRect(nOffset,0);
	ScreenToClient(&rectDraw);
	dc.DrawText(sText,&rectDraw,nAlignment|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);

	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}
	if(clrOld!=ID_OX_COLOR_NONE)
	{
		dc.SetTextColor(clrOld);
	}
	dc.SetBkMode(nOldBkMode);

	m_nTimerID=SetTimer(ID_OXITEMTIP_TIMER,ID_OXITEMTIP_TIMER_DELAY,NULL);
	if(m_nTimerID==0)
	{
		TRACE(_T("COXItemTipWnd::Display: failed to set timer to check item tip state\n"));
	}
}


void COXItemTipWnd::Hide(MSG* pMsg, CWnd* pWnd)
{
	ASSERT_VALID(m_pParentWnd);
    ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	ASSERT(::IsWindow(m_hWnd));

	if(m_nTimerID!=0)
	{
		KillTimer(m_nTimerID);
	}

	ShowWindow(SW_HIDE);
	if(pMsg!=NULL)
	{
		if(pWnd==NULL)
		{
			pWnd=m_pParentWnd;
		}

		if(::IsWindow(pWnd->GetSafeHwnd()))
		{
			pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
		}
	}
}


BOOL COXItemTipWnd::PreTranslateMessage(MSG* pMsg) 
{	
	ASSERT_VALID(m_pParentWnd);
    ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	ASSERT(::IsWindow(m_hWnd));

	CWnd *pWnd=NULL;	
	int nHitTest;

	pWnd=m_pParentWnd;
	while(pWnd!=NULL && (pWnd->GetStyle()&WS_CHILD)!=0)
	{
		pWnd=pWnd->GetParent();
	}
	ASSERT(pWnd!=NULL);

	if(pMsg->message==WM_MOUSEMOVE || 
		(pMsg->message==WM_TIMER && pMsg->wParam==(WPARAM)m_nTimerID))	
	{
		CPoint point;
		::GetCursorPos(&point);
		CRect rect;
		GetWindowRect(rect);
		pWnd=WindowFromPoint(point);
		ASSERT(pWnd!=NULL);
		if(pWnd->GetSafeHwnd()==GetSafeHwnd()) 
		{
			return CWnd::PreTranslateMessage(pMsg);
		}
		else
		{
			if(pMsg->message!=WM_MOUSEMOVE)
			{
				Hide();
				return TRUE;
			}
		}
	}

	switch(pMsg->message)	
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:	
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:	
	case WM_MOUSEMOVE:	
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		{
			POINTS points=MAKEPOINTS(pMsg->lParam);	
			CPoint point(points.x,points.y);
			ClientToScreen(&point);
			pWnd=WindowFromPoint(point);
			ASSERT(pWnd);
			if(pWnd==this) 
			{
				pWnd=m_pParentWnd;
			}

			nHitTest=(int)pWnd->SendMessage(WM_NCHITTEST,0,
				MAKELONG(point.x,point.y));	
			if(nHitTest==HTCLIENT) 
			{
				pWnd->ScreenToClient(&point);		
			} 
			else 
			{	
				switch(pMsg->message) 
				{
				case WM_LBUTTONDOWN: 
					{
						pMsg->message=WM_NCLBUTTONDOWN;		
						break;
					}
				case WM_RBUTTONDOWN: 
					{
						pMsg->message=WM_NCRBUTTONDOWN;		
						break;
					}
				case WM_MBUTTONDOWN: 
					{
						pMsg->message=WM_NCMBUTTONDOWN;		
						break;
					}
				case WM_LBUTTONUP: 
					{
						pMsg->message=WM_NCLBUTTONUP;		
						break;
					}
				case WM_RBUTTONUP: 
					{
						pMsg->message=WM_NCRBUTTONUP;		
						break;
					}
				case WM_MBUTTONUP: 
					{
						pMsg->message=WM_NCMBUTTONUP;		
						break;
					}
				case WM_LBUTTONDBLCLK: 
					{
						pMsg->message=WM_NCLBUTTONDBLCLK;		
						break;
					}
				case WM_RBUTTONDBLCLK: 
					{
						pMsg->message=WM_NCRBUTTONDBLCLK;		
						break;
					}
				case WM_MBUTTONDBLCLK: 
					{
						pMsg->message=WM_NCMBUTTONDBLCLK;		
						break;
					}
				case WM_MOUSEMOVE: 
					{
						pMsg->message=WM_NCMOUSEMOVE;		
						break;
					}
				}
				pMsg->wParam=nHitTest;
			}
			pMsg->lParam=MAKELONG(point.x,point.y);

			ASSERT(pWnd!=this);
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			ASSERT(pWnd!=NULL);

			Hide(pMsg,pWnd);
			return TRUE;	
		}
	}	

	BOOL bResult=CWnd::PreTranslateMessage(pMsg);
	return bResult;
}


void COXItemTipWnd::OnDestroy()
{
	if(m_nTimerID!=0)
	{
		KillTimer(m_nTimerID);
	}
}


BOOL COXItemTipWnd::IsDescendant(CWnd* pWndParent, CWnd* pWndChild)
	// helper for detecting whether child descendent of parent
	//  (works with owned popups as well)
{
	ASSERT(pWndParent!=NULL);
	ASSERT(::IsWindow(pWndParent->GetSafeHwnd()));
	ASSERT(pWndChild!=NULL);
	ASSERT(::IsWindow(pWndChild->GetSafeHwnd()));

	HWND hWndParent=pWndParent->GetSafeHwnd();
	HWND hWndChild=pWndChild->GetSafeHwnd();
	do
	{
		if (hWndParent == hWndChild)
			return TRUE;

		// check for permanent-owned window first
		CWnd* pWnd=CWnd::FromHandlePermanent(hWndChild);
		if(pWnd!=NULL)
			hWndChild=pWnd->GetOwner()->GetSafeHwnd();
		else
			hWndChild=(::GetWindowLong(hWndChild,GWL_STYLE)&WS_CHILD) ?
				::GetParent(hWndChild) : ::GetWindow(hWndChild,GW_OWNER);

	} while (hWndChild != NULL);

	return FALSE;
}


CRect COXItemTipWnd::GetMonitorRectFromPoint(const CPoint& ptHitTest, 
											 BOOL bOnlyWorkArea)
{
	CRect rectDisplay;

#if(defined(GetMonitorInfo) && defined(MonitorFromPoint))
	// if we use Win 98/NT5 we have to take into account multiple monitors
	OSVERSIONINFO osvi={ sizeof(OSVERSIONINFO) };
	VERIFY(::GetVersionEx(&osvi)!=0);
	if(osvi.dwMajorVersion>4 || 
		(osvi.dwMajorVersion==4 && osvi.dwMinorVersion>0))
	{
		HMONITOR hMonitor=::MonitorFromPoint(ptHitTest,MONITOR_DEFAULTTONEAREST);
		ASSERT(hMonitor!=NULL);
		MONITORINFO monitorInfo={ sizeof(MONITORINFO) };
		VERIFY(::GetMonitorInfo(hMonitor,&monitorInfo));
		if(!bOnlyWorkArea)
		{
			rectDisplay=monitorInfo.rcMonitor;
		}
		else
		{
			rectDisplay=monitorInfo.rcWork;
		}
	}
	else
	{
#endif
		if(!bOnlyWorkArea)
		{
			GetDesktopWindow()->GetWindowRect(rectDisplay);
		}
		else
		{
			::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rectDisplay,NULL);
		}
#if(defined(GetMonitorInfo) && defined(MonitorFromPoint))
	}
#else
	UNREFERENCED_PARAMETER(ptHitTest);
#endif
	
	return rectDisplay;
}
