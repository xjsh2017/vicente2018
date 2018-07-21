// ContextDockBar.cpp : implementation file
//

#include "stdafx.h"
//#include "STDraw.h"
#include "ContextDockBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContextDockBar

IMPLEMENT_DYNCREATE(CContextDockBar, CSubclassWnd)

//##ModelId=49B87BA9039C
CContextDockBar::CContextDockBar()
{
}

//##ModelId=49B87BA903A9
CContextDockBar::~CContextDockBar()
{
}


//##ModelId=49B87BA903AA
void CContextDockBar::SetContextMenu(UINT nMenuID)
{
	if(m_menu.GetSafeHmenu())
	{
		m_menu.DestroyMenu();
	}
	VERIFY(m_menu.LoadMenu(nMenuID));
}

//##ModelId=49B87BA903B2
LRESULT CContextDockBar::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
	case WM_RBUTTONUP:
		{			
// 			CMenu *pMenu;
// 			POINT pt;
// 			pMenu = m_menu.GetSubMenu(0);
// 			ASSERT(pMenu != NULL);
// 			pt.x = LOWORD(lp);
// 			pt.y = HIWORD(lp);
// 			::ClientToScreen(m_hWnd, &pt);
// 			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
// 				pt.x, pt.y, CWnd::FromHandle(m_hWnd)->GetParentFrame(), NULL);
		}
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}
