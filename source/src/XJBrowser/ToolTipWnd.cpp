// ToolTipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd

CToolTipWnd::CToolTipWnd()
{

	lpWndCls	= AfxRegisterWndClass(0);
	
	//Defaults
	m_clrBkColor = RGB(249,254,188); //light yellow
	m_clrFrameColor = RGB(0,0,255);  //blue
	m_clrTextColor = RGB(0,0,0);	 //black

	m_iFontHeight = 14;
	m_strFontName = "Arial";

	m_nCellPadding = 4;

	m_arrText.RemoveAll();
}

CToolTipWnd::~CToolTipWnd()
{
}


BEGIN_MESSAGE_MAP(CToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CToolTipWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd message handlers
BOOL CToolTipWnd::Create(CWnd* pParentWnd) 
{

	BOOL bRet = CWnd::CreateEx(NULL, lpWndCls, NULL,
		WS_POPUP| TTS_NOPREFIX, 0, 0, 50, 50,
		pParentWnd->GetSafeHwnd(), NULL, NULL);

	if(bRet)
		SetOwner(pParentWnd);

	return bRet;

}

void CToolTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrTextColor);

	CPen pen(PS_SOLID, 1, m_clrFrameColor);
	CBrush brush(m_clrBkColor);

	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	CFont *pFont = dc.SelectObject(&m_fontText);

	dc.Rectangle(0,0,m_szSize.cx, m_szSize.cy);
	
	for( int i = 0; i < m_arrText.GetSize(); i++ )
	{
		CString str = m_arrText.GetAt(i);
		
		CRect rcText( m_nCellPadding, m_szCellSize.cy*i+m_nCellPadding, m_szCellSize.cx - m_nCellPadding, m_szCellSize.cy*(i+1)-m_nCellPadding);
		dc.DrawText(str, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		dc.MoveTo(0, rcText.bottom+m_nCellPadding );
		dc.LineTo(m_szCellSize.cx, rcText.bottom+m_nCellPadding);
	}

	dc.SelectObject(pFont);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}


int CToolTipWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fontText.CreateFont(m_iFontHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,m_strFontName);
	return 0;
}

void CToolTipWnd::ShowToolTip(CPoint point, CStringArray& texts)
{
	CPoint pt = point;

	m_arrText.RemoveAll();
	m_arrText.Copy(texts);

	if( 0 == m_arrText.GetSize() )
	{
		m_szSize = CSize(10,20);
	}
	else
	{
		CDC* pDC = GetDC();
		CFont *pFont = pDC->SelectObject(&m_fontText);
		m_szCellSize = CSize(0,0);
		
		for( int i = 0; i < m_arrText.GetSize(); i++ )
		{
			CString str = m_arrText.GetAt(i);
			CSize czText = pDC->GetTextExtent(str);
			if( czText.cx > m_szCellSize.cx )
				m_szCellSize.cx = czText.cx;
			if( czText.cy > m_szCellSize.cy )
				m_szCellSize.cy = czText.cy;
		}
		pDC->SelectObject(pFont);

		m_szCellSize.cx += m_nCellPadding*2;
		m_szCellSize.cy += m_nCellPadding*2;

		m_szSize.cx = m_szCellSize.cx;
		m_szSize.cy = m_szCellSize.cy * m_arrText.GetSize();
	}
	
	MoveWindow(pt.x+10, pt.y, m_szSize.cx, m_szSize.cy, TRUE);
	ShowWindow(SW_SHOWNOACTIVATE);
}

void CToolTipWnd::HideToolTip()
{
	if(IsWindowVisible())
	{
		ShowWindow(SW_HIDE);
	}
}


void CToolTipWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	
}

void CToolTipWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	
}


