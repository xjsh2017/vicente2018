// CommWnd.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CommWnd.h"
#include "DlgChannelStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWndPane::CWndPane( CPoint pt, CSize sz )
{
	m_ptLeftTop = pt;
	m_szPane = sz;
	
	m_colFill = RGB(255, 255, 255);
	
	m_sText = "";
	
	m_colText = RGB(0, 0, 0);
	m_nState = FOCUS_NONE;
	m_nIconWidth = 24;

	m_sDetailTitle = "";
	m_nDetailStatus = -1;
	m_sStatusMsg = "";

	VERIFY(m_ftFont.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,					// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					StringFromID(IDS_DEFAULT_FONT)));                 // lpszFacename
}

CWndPane::~CWndPane()
{
	
}

void CWndPane::Draw( CDC* pDC )
{
	return;
}

BOOL CWndPane::MouseMove( CPoint point )
{
	BOOL bReturn = FALSE;
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(rcBound.PtInRect(point))
	{
		switch(m_nState)
		{
		case FOCUS_NONE:
			SetState(FOCUS_ON);
			bReturn = TRUE;
			break;
		case FOCUS_ON:
			break;
		default:break;
		}
	}
	else
	{
		switch(m_nState)
		{
		case FOCUS_NONE:
			break;
		case FOCUS_ON:
			SetState(FOCUS_NONE);
			bReturn = TRUE;
			break;
		default:break;
		}
	}
	return bReturn;
}

int CWndPane::LButtonDown( CPoint point )
{
	return CCommWnd::OPERATOR_NONE;
}


CTitlePane::CTitlePane( CPoint pt, CSize sz ):CWndPane(pt, sz)
{
	m_sText = StringFromID(IDS_FUNC_COMMUSTATUS);
	m_colFill = RGB(141, 177, 226);
	m_ImageList.Create(IDB_BMP_WNDBTN, m_nIconWidth, 4, RGB(255, 0, 0));
	m_bIsMini = FALSE;
}

CTitlePane::~CTitlePane()
{
	
}

void CTitlePane::Draw( CDC* pDC )
{
	if(pDC == NULL)
		return;
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(m_nState == FOCUS_NONE)
		m_colFill = RGB(141, 177, 226);
	else if(m_nState == FOCUS_ON)
		m_colFill = RGB(120, 148, 64);
	CBrush brush(m_colFill);
	CBrush* pOldBursh = pDC->SelectObject(&brush);
	pDC->Rectangle(&rcBound);
	pDC->SelectObject(pOldBursh);

	CFont* pOldFont = pDC->SelectObject(&m_ftFont);

	//绘制文字
	CRect rcText;
	rcText.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx-m_nIconWidth, m_ptLeftTop.y+m_szPane.cy));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colText);
	pDC->DrawText(m_sText, &rcText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	pDC->SelectObject(pOldFont);

	int nIconY = rcBound.top+(rcBound.Height()-m_nIconWidth)/2;
	//绘制最小化/最大化按钮
	int nIndex = 1;
	if(m_bIsMini)
		nIndex = 0;
	m_ImageList.Draw(pDC, nIndex, CPoint(rcBound.right -(2+m_nIconWidth), nIconY), ILD_TRANSPARENT);
}

/****************************************************
Date:2013/7/17  Author:LYH
函数名:   LButtonDown	
返回值:   int	
功能概要: 
参数: CPoint point	
*****************************************************/
int CTitlePane::LButtonDown( CPoint point )
{
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(!rcBound.PtInRect(point))
		return CCommWnd::OPERATOR_NONE;
// 	CRect rcText;
// 	rcText.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx-m_nIconWidth, m_ptLeftTop.y+m_szPane.cy));
// 	if(rcText.PtInRect(point))
// 		return CCommWnd::OPERATOR_OPEN;
	else
	{
		if(IsMini())
			return CCommWnd::OPERATOR_MAX;
		else
			return CCommWnd::OPERATOR_MINI;
	}
	return CCommWnd::OPERATOR_NONE;
}


CDescripPane::CDescripPane( CPoint pt, CSize sz ):CWndPane(pt, sz)
{
	m_nIconWidth = 48;
	m_sText = "";
	m_ImageList.Create(m_nIconWidth, m_nIconWidth, ILC_COLOR32|ILC_MASK, 0, 4);
	//m_ImageList.Create(IDB_BMP_STATUSLIGHT, m_nIconWidth, 4, RGB(0, 0, 0));
	CBitmap bmp;  
	bmp.LoadBitmap(IDB_BMP_STATUSLIGHT);
	m_ImageList.Add(&bmp, RGB(255,255,255)); 
	bmp.DeleteObject();
}

CDescripPane::~CDescripPane()
{
	
}

void CDescripPane::Draw( CDC* pDC )
{
	if(pDC == NULL)
		return;
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(m_nState == FOCUS_NONE)
		m_colFill = RGB(255, 255, 255);
	else if(m_nState == FOCUS_ON)
		m_colFill = RGB(234, 112, 13);
	CBrush brush(m_colFill);
	CBrush* pOldBursh = pDC->SelectObject(&brush);
	pDC->Rectangle(&rcBound);
	pDC->SelectObject(pOldBursh);
	//绘制文字
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_colText);
	CFont* pOldFont = pDC->SelectObject(&m_ftFont);
	int nPaneRight = m_ptLeftTop.x+m_szPane.cx;
	CRect rcText;
	if(m_sText != "")
	{
		rcText.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
		pDC->DrawText(m_sText, &rcText, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		pDC->SelectObject(pOldFont);
		return;
	}

	rcText.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+(m_szPane.cx-m_nIconWidth-8)/2, m_ptLeftTop.y+m_szPane.cy));
	pDC->DrawText(m_sDetailTitle, &rcText, DT_SINGLELINE|DT_RIGHT|DT_VCENTER);

	//绘制图标
	int nIconY = rcBound.top+(rcBound.Height()-m_nIconWidth)/2;
	if(m_nDetailStatus < 0 ||m_nDetailStatus >= m_ImageList.GetImageCount())
	{
		//不绘制图标
	}
	else
		m_ImageList.Draw(pDC, m_nDetailStatus, CPoint(rcText.right + 4, nIconY), ILD_TRANSPARENT);
	//文字
	rcText.SetRect(rcText.right+4+4+m_nIconWidth, m_ptLeftTop.y, nPaneRight, m_ptLeftTop.y+m_szPane.cy);
	pDC->DrawText(m_sStatusMsg, &rcText, DT_SINGLELINE|DT_LEFT|DT_VCENTER);

	pDC->SelectObject(pOldFont);
}

int CDescripPane::LButtonDown( CPoint point )
{
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(rcBound.PtInRect(point))
		return CCommWnd::OPERATOR_OPEN;

	return CCommWnd::OPERATOR_NONE;
}

IMPLEMENT_DYNAMIC(CCommWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CCommWnd

CCommWnd::CCommWnd()
{
	m_ptWnd;
	m_szWnd;
	m_arrPane.RemoveAll();
	m_pTitlePane = NULL;
	m_bMouseDown = FALSE;
}

CCommWnd::~CCommWnd()
{
	for(int i = 0; i < m_arrPane.GetSize(); i++)
	{
		CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
		if(p != NULL)
			delete p;
	}
	m_arrPane.RemoveAll();
	if(m_pTitlePane != NULL)
	{
		delete m_pTitlePane;
		m_pTitlePane = NULL;
	}
	m_pStation = NULL;
}


BEGIN_MESSAGE_MAP(CCommWnd, CWnd)
	//{{AFX_MSG_MAP(CCommWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave) 
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommWnd message handlers

void CCommWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(dc.GetSafeHdc() == NULL)
		return;
	
	// TODO: Add your message handler code here
	CRect r;
	dc.GetClipBox(&r);
	
	CRect rect = r;
	dc.LPtoDP(rect);
	
	CDC newdc;
	CBitmap bitmap;
	CDC* pDrawDC = &dc;
	CBitmap* pOldBitmap = NULL;
	
	if(newdc.CreateCompatibleDC(&dc))
	{
		if(bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
		{
			//OnPrepareDC(&newdc, NULL);
			pDrawDC = &newdc;
			
			newdc.OffsetViewportOrg(-rect.left, -rect.top);
			pOldBitmap = newdc.SelectObject(&bitmap);
		}
	}
	
	CRect rcClient;
	GetClientRect(rcClient);
	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDrawDC->SelectObject(&brush);
	pDrawDC->Rectangle(&rcClient);
 	pDrawDC->SelectObject(pOldBrush);
	//pDrawDC ->FillSolidRect(rcClient, RGB(255, 255, 255));
	
	if(m_pTitlePane != NULL)
		m_pTitlePane->Draw(pDrawDC);
	for(int i = 0; i < m_arrPane.GetSize(); i++)
	{
		CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
		if(p != NULL)
			p->Draw(pDrawDC);
	}
	
	dc.SetViewportOrg(0, 0);
	dc.SetWindowOrg(0,0);
	dc.SetMapMode(MM_TEXT);
	newdc.SetViewportOrg(0, 0);
	newdc.SetWindowOrg(0,0);
	newdc.SetMapMode(MM_TEXT);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &newdc, 0, 0, SRCCOPY);
	newdc.SelectObject(pOldBitmap);
	newdc.DeleteDC();
}

BOOL CCommWnd::CreateWnd( CWnd* pParent, CPoint pt, CSize sz )
{
	m_ptWnd = pt;
	m_szWnd = sz;
	CRect rcWnd;
	rcWnd.SetRect(0, 0, m_szWnd.cx, m_szWnd.cy);
	if(!Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rcWnd, pParent, 123))
		return FALSE;

	m_szTitle.cx = m_szWnd.cx;
	m_szTitle.cy = 40;
	m_szPane.cx = m_szWnd.cx;
	m_szPane.cy = 60;

	m_pTitlePane = new CTitlePane(CPoint(0, 0), m_szTitle);
	
	CDescripPane* p1 = new CDescripPane(CPoint(0, 53), m_szPane);
	if(p1 != NULL)
		m_arrPane.Add(p1);
	p1 = new CDescripPane(CPoint(0, 125), m_szPane);
	if(p1 != NULL)
		m_arrPane.Add(p1);
	return TRUE;
}

/****************************************************
Date:2013/7/15  Author:LYH
函数名:   Show	
返回值:   void	
功能概要: 
*****************************************************/
void CCommWnd::Show()
{
	if(m_pTitlePane != NULL)
	{
		if(m_pTitlePane->IsMini())
			SetWindowPos(NULL, m_ptWnd.x, m_ptWnd.y, m_szTitle.cx, m_szTitle.cy, SWP_SHOWWINDOW);
		else
			SetWindowPos(NULL, m_ptWnd.x, m_ptWnd.y, m_szWnd.cx, m_szWnd.cy, SWP_SHOWWINDOW);
	}
}

/****************************************************
Date:2013/7/19  Author:LYH
函数名:   Show	
返回值:   void	
功能概要: 
参数: CPoint pt	
*****************************************************/
void CCommWnd::Show( CPoint pt )
{
	m_ptWnd = pt;
	Show();
}


void CCommWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	TRACKMOUSEEVENT tme;   
	tme.cbSize=sizeof(TRACKMOUSEEVENT); //监控鼠标离开   
	tme.dwFlags=TME_LEAVE;   
	tme.hwndTrack=this->m_hWnd;  
	
	if(::_TrackMouseEvent(&tme))   
	{   
		m_bMouseTracking=TRUE; //其他鼠标进入时的操作      
	}
	
	if(m_bMouseDown)
	{
		
	}
	else
		DoMouseEvent(point);
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CCommWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CCommWnd::DoMouseEvent( CPoint point )
{
	BOOL bChanged = FALSE;

	
	if(m_pTitlePane != NULL)
	{
		if(m_pTitlePane->MouseMove(point))
			bChanged = TRUE;
	}
	if(m_pTitlePane != NULL && !m_pTitlePane->IsMini())
	{
		for(int i = 0; i < m_arrPane.GetSize(); i++)
		{
			CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
			if(p != NULL)
			{
				if(p->MouseMove(point))
					bChanged = TRUE;
			}
		}
	}
	
	if(bChanged)
	{
		Invalidate(FALSE);
	}
}

void CCommWnd::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	m_bMouseDown = FALSE;
	m_bMouseTracking = FALSE;
	DoMouseEvent(CPoint(-1, -1));
}

void CCommWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = TRUE;
	m_ptMouseDown.x = point.x-m_ptWnd.x;
	m_ptMouseDown.y = point.y-m_ptWnd.y;
	int nReturn = OPERATOR_NONE;
	if(m_pTitlePane != NULL)
	{
		int nClicked = -1;
		switch(m_pTitlePane->LButtonDown(point))
		{
		case OPERATOR_MINI:
			MiniWnd(TRUE);
			break;
		case OPERATOR_MAX:
			MiniWnd(FALSE);
			break;
		default:
			if(!m_pTitlePane->IsMini())
			{
				for(int i = 0; i < m_arrPane.GetSize(); i++)
				{
					CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
					if(p != NULL)
					{
						if(p->LButtonDown(point) == OPERATOR_OPEN)
						{
							nClicked = i;
							break;
						}
					}
				}
				if(nClicked != -1)
				{
					ShowChannelDetail(nClicked);
				}
			}
			break;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CCommWnd::MiniWnd( BOOL bMini )
{
	m_pTitlePane->SetMini(bMini);
	Invalidate(FALSE);
	Show();
}

void CCommWnd::ShowChannelDetail( int nChannel )
{
	CDlgChannelStatus dlg;
	dlg.m_pStation = m_pStation;
	dlg.m_nChannel = nChannel;
	dlg.DoModal();
}

/****************************************************
Date:2013/7/18  Author:LYH
函数名:   SetStation	
返回值:   void	
功能概要: 
参数: CStationObj * pStation	
*****************************************************/
void CCommWnd::SetStation( CStationObj* pStation )
{
	m_pStation = pStation;
	RefreshData();
}

void CCommWnd::RefreshData()
{
	if(m_pStation == NULL)
	{
		for(int i = 0; i < m_arrPane.GetSize(); i++)
		{
			CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
			if(p != NULL)
			{
				p->m_sDetailTitle = "";
				p->m_nDetailStatus = -1;
				p->m_sStatusMsg = "";
			}
		}
	}
	else
	{
		for(int i = 0; i < m_arrPane.GetSize(); i++)
		{
			CWndPane* p = (CWndPane*)m_arrPane.GetAt(i);
			if(p != NULL)
			{
				switch(i)
				{
				case 0:
					if(m_pStation->m_nIPFlag == 0)
						p->m_sDetailTitle = StringFromID(IDS_COMMU_STATUS)+":";
					else if(m_pStation->m_nIPFlag == 1)
						p->m_sDetailTitle = StringFromID(IDS_COMMU_MAIN_MACHINE)+":";
					else if(m_pStation->m_nIPFlag == 2 || m_pStation->m_nIPFlag == 3)
						p->m_sDetailTitle = StringFromID(IDS_COMMU_MAIN_CHANNEL)+":";
					p->m_nDetailStatus = m_pStation->m_nCommStatus;
					p->m_sStatusMsg = m_pStation->GetStatusDesc(0);
					break;
				case 1:
					if(m_pStation->m_nIPFlag == 0)
					{
						p->m_sText = StringFromID(IDS_COMMU_ALONE);
					}
					else
					{
						p->m_sText = "";
						if(m_pStation->m_nIPFlag == 1)
							p->m_sDetailTitle = StringFromID(IDS_COMMU_STANDBY_MACHINE)+":";
						else if(m_pStation->m_nIPFlag == 2 || m_pStation->m_nIPFlag == 3)
							p->m_sDetailTitle = StringFromID(IDS_COMMU_STANDBY_CHANNEL)+":";
						
						p->m_nDetailStatus = m_pStation->m_nCommStatus1;
						p->m_sStatusMsg = m_pStation->GetStatusDesc(1);
					}
					
					break;
				}
				
			}
		}
	}
	Invalidate();
}

void CCommWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bMouseDown = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}
