// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                         
// OXTreeEdit.cpp : implementation file
//
// Product Version: 7.52


#include "stdafx.h"
#include "OXTreeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXTreeEdit

COXTreeEdit::COXTreeEdit()
{
	m_nDeltaX = 14;
	m_nDeltaY = 6;
}

COXTreeEdit::~COXTreeEdit()
{
}


BEGIN_MESSAGE_MAP(COXTreeEdit, CEdit)
	//{{AFX_MSG_MAP(COXTreeEdit)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXTreeEdit message handlers

void COXTreeEdit::KeepPos(CPoint pt,DWORD dwAlign)
{
	m_bKeepPos = TRUE;
	m_pos = pt;
	m_dwAlign = dwAlign;
	if(::IsWindow(m_hWnd))
	{
		DWORD dwStyle = WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL|dwAlign;
		::SetWindowLong(m_hWnd,GWL_STYLE,dwStyle);
	}
}

void COXTreeEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CEdit::OnWindowPosChanging(lpwndpos);
	CString sWndText;
	GetWindowText(sWndText);
	int nTextLen = GetWindowTextLength();
	if(!nTextLen)
	{
		//there are no text in edit window
		// use sample text to measure
		sWndText = _T("W");
	}
	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(GetFont());
	CSize sz = pDC->GetTextExtent(sWndText);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	sz.cx += m_nDeltaX;
	sz.cy += m_nDeltaY;
	lpwndpos->cx = sz.cx;
	lpwndpos->cy = sz.cy;
	if (m_bKeepPos)
		switch(m_dwAlign)
		{
		case ES_LEFT:
			lpwndpos->x = m_pos.x;
			lpwndpos->y = m_pos.y;
			break;
		case ES_RIGHT:
			lpwndpos->x = m_pos.x - lpwndpos->cx;
			lpwndpos->y = m_pos.y;
			break;
		case ES_CENTER:
			lpwndpos->y = m_pos.y;
			lpwndpos->x = m_pos.x - lpwndpos->cx/2;
			break;
		default:
			// unknown style found!
			ASSERT(FALSE);
			break;
		}
	if(lpwndpos->x < 0)
		lpwndpos->x = 0;
	if(m_bKeepBounds)
	{
		if(lpwndpos->x + lpwndpos->cx > m_szBounds.cx)
			lpwndpos->cx = m_szBounds.cx - lpwndpos->x;
	}
}

void COXTreeEdit::Init()
{
	m_bKeepPos = FALSE;
	m_bKeepBounds = FALSE;
	m_dwAlign = 0;
}

void COXTreeEdit::KeepBounds(CSize sz)
{
	m_bKeepBounds = TRUE;
	m_szBounds = sz;
}
