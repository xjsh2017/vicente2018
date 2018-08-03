// CommWnd.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "HangoutWnd.h"

#include "XJPTSetStore.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CHangoutWndPane::CHangoutWndPane( CPoint pt, CSize sz )
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

CHangoutWndPane::~CHangoutWndPane()
{
	
}

void CHangoutWndPane::Draw( CDC* pDC )
{
	return;
}

BOOL CHangoutWndPane::MouseMove( CPoint point )
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

int CHangoutWndPane::LButtonDown( CPoint point )
{
	return CHangoutWnd::OPERATOR_NONE;
}


CHangoutTitlePane::CHangoutTitlePane( CPoint pt, CSize sz ):CHangoutWndPane(pt, sz)
{
	m_sText = StringFromID(IDS_FUNC_HANGOUTSTATUS);
	m_colFill = RGB(141, 177, 226);
	m_ImageList.Create(IDB_BMP_WNDBTN, m_nIconWidth, 4, RGB(255, 0, 0));
	m_bIsMini = FALSE;
}

CHangoutTitlePane::~CHangoutTitlePane()
{
	
}

void CHangoutTitlePane::Draw( CDC* pDC )
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
int CHangoutTitlePane::LButtonDown( CPoint point )
{
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(!rcBound.PtInRect(point))
		return CHangoutWnd::OPERATOR_NONE;
// 	CRect rcText;
// 	rcText.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx-m_nIconWidth, m_ptLeftTop.y+m_szPane.cy));
// 	if(rcText.PtInRect(point))
// 		return CHangoutWnd::OPERATOR_OPEN;
	else
	{
		if(IsMini())
			return CHangoutWnd::OPERATOR_MAX;
		else
			return CHangoutWnd::OPERATOR_MINI;
	}
	return CHangoutWnd::OPERATOR_NONE;
}


CHangoutDescPane::CHangoutDescPane( CPoint pt, CSize sz ):CHangoutWndPane(pt, sz)
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

CHangoutDescPane::~CHangoutDescPane()
{
	
}

void CHangoutDescPane::Draw( CDC* pDC )
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

int CHangoutDescPane::LButtonDown( CPoint point )
{
	CRect rcBound;
	rcBound.SetRect(m_ptLeftTop, CPoint(m_ptLeftTop.x+m_szPane.cx, m_ptLeftTop.y+m_szPane.cy));
	if(rcBound.PtInRect(point))
		return CHangoutWnd::OPERATOR_OPEN;

	return CHangoutWnd::OPERATOR_NONE;
}

IMPLEMENT_DYNAMIC(CHangoutWnd, CWnd)
/////////////////////////////////////////////////////////////////////////////
// CHangoutWnd

CHangoutWnd::CHangoutWnd()
{
	m_ptWnd;
	m_szWnd;
	m_arrPane.RemoveAll();
	m_pTitlePane = NULL;
	m_bMouseDown = FALSE;
	m_bAlreadyShowOnce = FALSE;
}

CHangoutWnd::~CHangoutWnd()
{
	for(int i = 0; i < m_arrPane.GetSize(); i++)
	{
		CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
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


BEGIN_MESSAGE_MAP(CHangoutWnd, CWnd)
	//{{AFX_MSG_MAP(CHangoutWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave) 
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHangoutWnd message handlers

void CHangoutWnd::OnPaint() 
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
		CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
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

BOOL CHangoutWnd::CreateWnd( CWnd* pParent, CPoint pt, CSize sz )
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

	m_pTitlePane = new CHangoutTitlePane(CPoint(0, 0), m_szTitle);
	
	CHangoutDescPane* p1 = new CHangoutDescPane(CPoint(0, 53), m_szPane);
	if(p1 != NULL)
		m_arrPane.Add(p1);
/*	p1 = new CHangoutDescPane(CPoint(0, 125), m_szPane);
	if(p1 != NULL)
		m_arrPane.Add(p1);*/

	return TRUE;
}

void CHangoutWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		//KillTimer(m_nTimer);

		if (NULL != m_pStation){
			KillTimer(m_nTimer);
			//SetStation(m_pStation);
			RefreshData();
			m_nTimer = SetTimer(801, 3*1000, NULL);
		}
	}
}


/****************************************************
Date:2013/7/15  Author:LYH
函数名:   Show	
返回值:   void	
功能概要: 
*****************************************************/
void CHangoutWnd::Show()
{
	if(m_pTitlePane != NULL)
	{
		if(m_pTitlePane->IsMini())
			SetWindowPos(NULL, m_ptWnd.x, m_ptWnd.y, m_szTitle.cx, m_szTitle.cy, SWP_SHOWWINDOW);
		else
			SetWindowPos(NULL, m_ptWnd.x, m_ptWnd.y, m_szWnd.cx, m_szWnd.cy, SWP_SHOWWINDOW);
	}

	Invalidate();
}

/****************************************************
Date:2013/7/19  Author:LYH
函数名:   Show	
返回值:   void	
功能概要: 
参数: CPoint pt	
*****************************************************/
void CHangoutWnd::Show( CPoint pt )
{
	m_ptWnd = pt;
	Show();
}


void CHangoutWnd::OnMouseMove(UINT nFlags, CPoint point) 
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

BOOL CHangoutWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CHangoutWnd::DoMouseEvent( CPoint point )
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
			CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
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

void CHangoutWnd::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	m_bMouseDown = FALSE;
	m_bMouseTracking = FALSE;
	DoMouseEvent(CPoint(-1, -1));
}

void CHangoutWnd::OnLButtonDown(UINT nFlags, CPoint point) 
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
					CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
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

void CHangoutWnd::MiniWnd( BOOL bMini )
{
	m_pTitlePane->SetMini(bMini);
	Invalidate(FALSE);
	Show();
}

void CHangoutWnd::ShowChannelDetail( int nChannel )
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	CCJTabCtrlBar &bar = pMainFrame->m_wndGlobalMsgBar;
	CXJBrowserView * pView = pApp->GetSVGView();

	CRect rcRect;
	pView->GetWindowRect(&rcRect);
	
	if (m_bAlreadyShowOnce){
		bar.GetWindowRect(rcRect);
		m_pointPTSetModView.x = rcRect.left - 2;
		m_pointPTSetModView.y = rcRect.top - 18;

	}else{
		LONG w = 600;
		LONG h = 220;

		m_pointPTSetModView.x = rcRect.right - w;
		m_pointPTSetModView.y = rcRect.bottom - h;
		m_bAlreadyShowOnce = true;
	}
	
	bar.ShowWindow(bar.IsVisible() ? SW_HIDE : SW_SHOW);
	pMainFrame->FloatControlBar(&bar, m_pointPTSetModView, CBRS_ALIGN_LEFT);

	
	CString str;
	str.Format("x: %d; y: %d", m_pointPTSetModView.x, m_pointPTSetModView.y);
	//AfxMessageBox(str);
}

/****************************************************
Date:2013/7/18  Author:LYH
函数名:   SetStation	
返回值:   void	
功能概要: 
参数: CStationObj * pStation	
*****************************************************/
void CHangoutWnd::SetStation( CStationObj* pStation )
{
	m_pStation = pStation;
	KillTimer(m_nTimer);
	m_nTimer = SetTimer(801, 3*1000, NULL);
	//RefreshData();
}

void CHangoutWnd::RefreshData()
{
	if(m_pStation == NULL)
	{
		for(int i = 0; i < m_arrPane.GetSize(); i++)
		{
			CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
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
			CHangoutWndPane* p = (CHangoutWndPane*)m_arrPane.GetAt(i);
			if(p != NULL)
			{
				CXJPTSetStore *store = CXJPTSetStore::GetInstance();
				QPTSetCard &card = *(store->GetCard());
				QLogTable &log = *(store->GetLog());

				int nPTSetState = card.GetStateID();
				
				CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
				CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(card.GetPTID().constData(), TYPE_SEC);
				BOOL bHangout = FALSE;
				if (pObj && pObj->m_pStation && pObj->m_pStation == m_pStation && nPTSetState != XJ_OPER_UNHANGOUT)
					bHangout = TRUE;
				else
					bHangout = FALSE;
				
				switch(i)
				{
				case 0:
					if(m_pStation->m_nIPFlag == 0)
						p->m_sDetailTitle = StringFromID(IDS_HANGOUT_STATUS)+":";
					else if(m_pStation->m_nIPFlag == 1)
						p->m_sDetailTitle = StringFromID(IDS_COMMU_MAIN_MACHINE)+":";
					else if(m_pStation->m_nIPFlag == 2 || m_pStation->m_nIPFlag == 3)
						p->m_sDetailTitle = StringFromID(IDS_COMMU_MAIN_CHANNEL)+":";
					p->m_nDetailStatus = (bHangout ? 0 : 1);
					if (bHangout)
						p->m_sStatusMsg = StringFromID(IDS_HANGOUTING_STATUS);
					else
						p->m_sStatusMsg = StringFromID(IDS_HANGOUTNONE_STATUS);
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

void CHangoutWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bMouseDown = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}
