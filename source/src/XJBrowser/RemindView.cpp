// RemindView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "RemindView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemindView

#define REMIND_REFRESHTIME	300

UINT RefreshObjs(LPVOID pParam)
{
	if(!pParam)
	{
		WriteLog("启动提醒窗口线程失败!", XJ_LOG_LV3);
		return -1;
	}
	WriteLog("启动提醒窗口线程成功!", XJ_LOG_LV3);
	CRemindView * pView = (CRemindView*)pParam;
	while(1)
	{
		if(pView->m_bThreadExit)
			break;
		int ntemptime = 0;
		while( ntemptime < REMIND_REFRESHTIME )
		{
			Sleep(10);
			ntemptime+=10;
			if(pView->m_bThreadExit)
				break;
		}
		if(pView->m_bThreadExit)
			break;
		pView->UpdateAllObj();
		pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	}
	WriteLog("退出提醒窗口线程成功!", XJ_LOG_LV3);
	return 0;
}


IMPLEMENT_DYNCREATE(CRemindView, CScrollView)

CRemindView::CRemindView()
{
	m_pAction = new CRemindObj(StringFromID(IDS_TOOLBAR_ACTION));
	m_pAlarm = new CRemindObj(StringFromID(IDS_TOOLBAR_ALARM));
	m_pOsc = new CRemindObj(StringFromID(IDS_TOOLBAR_OSC));
	m_pFault = new CRemindObj(StringFromID(IDS_TOOLBAR_FAULTREPORT));

	m_bThreadExit = FALSE;
	m_pThread = NULL;
	
	InitializeCriticalSection(&m_CriticalSection);
}

CRemindView::~CRemindView()
{
	DeleteCriticalSection(&m_CriticalSection);
	if(m_pAction)
	{
		delete m_pAction;
		m_pAction = NULL;
	}
	if(m_pAlarm)
	{
		delete m_pAlarm;
		m_pAlarm = NULL;
	}
	if(m_pOsc)
	{
		delete m_pOsc;
		m_pOsc = NULL;
	}
	if(m_pFault)
	{
		delete m_pFault;
		m_pFault = NULL;
	}
}


BEGIN_MESSAGE_MAP(CRemindView, CScrollView)
	//{{AFX_MSG_MAP(CRemindView)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(ADD_REMIND, OnAddRemind)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemindView drawing

void CRemindView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 90;
	SetScrollSizes(MM_TEXT, sizeTotal);
	StartThread();
}

void CRemindView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
    
	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);//检测无效区域
	
	client.InflateRect(2,2,2,2);
	CRect rcClip = client;
	DocToClient(rcClip);//LPtoDP(rect)
	
	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		//in other words , use memeory DC
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rcClip.Width(), rcClip.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;
				
				dc.OffsetViewportOrg(-rcClip.left, -rcClip.top);    
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rcClip.left % 8, rcClip.top % 8);
                
				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	CRect rect;
	GetClientRect(&rect);
	pDrawDC->FillSolidRect(rect, RGB(255, 255, 255));
	// TODO: add draw code here
	EnterCriticalSection(&m_CriticalSection);
	if(m_pAction)
	{
		m_pAction->Draw(pDrawDC);
	}
	if(m_pAlarm)
	{
		m_pAlarm->Draw(pDrawDC);
	}
	if(m_pOsc)
	{
		m_pOsc->Draw(pDrawDC);
	}
	if(m_pFault)
	{
		m_pFault->Draw(pDrawDC);
	}
	LeaveCriticalSection(&m_CriticalSection);
	
	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
        dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
			&dc, 0, 0, SRCCOPY);
		
		dc.SelectObject(pOldBitmap);
	}
	
	//释放资源
	bitmap.DeleteObject();
  	dc.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
// CRemindView diagnostics

#ifdef _DEBUG
void CRemindView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRemindView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRemindView message handlers

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   ResetObjSize	
返回值:   void	
功能概要: 
*****************************************************/
void CRemindView::ResetObjSize()
{
	CRect rect;
	GetClientRect(&rect);
	int w = ((rect.Width() - 40)/2);
	if(m_pAction)
	{
		m_pAction->SetBound(10, 10, w, 30);
	}
	if(m_pAlarm)
	{
		m_pAlarm->SetBound(w+30, 10, w, 30);
	}
	if(m_pOsc)
	{
		m_pOsc->SetBound(10, 50, w, 30);
	}
	if(m_pFault)
	{
		m_pFault->SetBound(w+30, 50, w, 30);
	}
}

void CRemindView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CScrollView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	ResetObjSize();
}

void CRemindView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnTimer(nIDEvent);
}

BOOL CRemindView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   UpdateAllObj	
返回值:   void	
功能概要: 
*****************************************************/
void CRemindView::UpdateAllObj()
{
	EnterCriticalSection(&m_CriticalSection);
	if(m_pAction)
	{
		m_pAction->Update(100);
	}
	if(m_pAlarm)
	{
		m_pAlarm->Update(100);
	}
	if(m_pOsc)
	{
		m_pOsc->Update(100);
	}
	if(m_pFault)
	{
		m_pFault->Update(100);
	}
	LeaveCriticalSection(&m_CriticalSection);
}


/*************************************************************
 函 数 名：DocToClient()
 功能概要：逻辑坐标转设备坐标
 返 回 值: void
 参    数：param1	要转换的坐标
**************************************************************/
//##ModelId=49B87B8502E6
void CRemindView::DocToClient( CPoint & pt )
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&pt);
}

/*************************************************************
 函 数 名：DocToClient()
 功能概要：逻辑坐标转设备坐标
 返 回 值: void
 参    数：param1	要转换的范围
**************************************************************/
//##ModelId=49B87B8502EE
void CRemindView::DocToClient( CRect & rect )
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

/*************************************************************
 函 数 名：ClientToDoc()
 功能概要：设备坐标转逻辑坐标
 返 回 值: void
 参    数：param1	要转换的坐标
**************************************************************/
//##ModelId=49B87B8502F0
void CRemindView::ClientToDoc( CPoint & pt )
{
	CClientDC dc(this);
	OnPrepareDC(&dc , NULL);
	dc.DPtoLP(&pt);
}

/*************************************************************
 函 数 名：ClientToDoc()
 功能概要：设备坐标转逻辑坐标
 返 回 值: void
 参    数：param1	要转换的范围
**************************************************************/
//##ModelId=49B87B8502F2
void CRemindView::ClientToDoc( CRect & rect )
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
	rect.NormalizeRect();
}

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   OnAddRemind	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CRemindView::OnAddRemind( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	EnterCriticalSection(&m_CriticalSection);
	CRemindContent* pRC = (CRemindContent*)lParam;
	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_ACTION) && m_pAction)
		m_pAction->AddContent(pRC);
	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_ALARM) && m_pAlarm)
		m_pAlarm->AddContent(pRC);
	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_OSC) && m_pOsc)
		m_pOsc->AddContent(pRC);
	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_FAULTREPORT) && m_pFault)
		m_pFault->AddContent(pRC);
	LeaveCriticalSection(&m_CriticalSection);
}

void CRemindView::UpdateActive( CPoint pt )
{
	if(m_pAction)
	{
		if(m_pAction->IsMouseOn(pt))
			m_pAction->SetActive(TRUE);
		else
			m_pAction->SetActive(FALSE);
	}
	if(m_pAlarm)
	{
		if(m_pAlarm->IsMouseOn(pt))
			m_pAlarm->SetActive(TRUE);
		else
			m_pAlarm->SetActive(FALSE);
	}
	if(m_pOsc)
	{
		if(m_pOsc->IsMouseOn(pt))
			m_pOsc->SetActive(TRUE);
		else
			m_pOsc->SetActive(FALSE);
	}
	if(m_pFault)
	{
		if(m_pFault->IsMouseOn(pt))
			m_pFault->SetActive(TRUE);
		else
			m_pFault->SetActive(FALSE);
	}
}


void CRemindView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);

	UpdateActive(ptDoc);
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CRemindView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);
	CRemindObj* pSelect = MouseOnObj(ptDoc);
	if(pSelect)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(pSelect == m_pAction)
			pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_ACTION);
		if(pSelect == m_pAlarm)
			pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_ALARM);
		if(pSelect == m_pOsc)
			pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_OSC);
		if(pSelect == m_pFault)
			pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_FAULT);
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

CRemindObj* CRemindView::MouseOnObj( CPoint pt )
{
	CRemindObj* pReturn = NULL;

	if(m_pAction && m_pAction->IsMouseOn(pt))
		pReturn = m_pAction;
	if(m_pAlarm && m_pAlarm->IsMouseOn(pt))
		pReturn = m_pAlarm;
	if(m_pOsc && m_pOsc->IsMouseOn(pt))
		pReturn = m_pOsc;
	if(m_pFault && m_pFault->IsMouseOn(pt))
		pReturn = m_pFault;

	return pReturn;
}

void CRemindView::StartThread()
{
	if (m_pThread == NULL)
	{
		m_bThreadExit = FALSE;
		//启动清理二次设备配置线程
		m_pThread = AfxBeginThread(RefreshObjs,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pThread != NULL)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

void CRemindView::EndThread()
{
	if(m_pThread != NULL)
	{
		WriteLog("开始退出提醒窗口线程", XJ_LOG_LV3);
		//线程还没退出去
		m_bThreadExit = TRUE;	//设置退出标志
		DWORD dw = WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pThread->m_hThread,NULL);
			WriteLog("提醒窗口线程无法正常退出,强制结束", XJ_LOG_LV3);
			break;
		}
		delete m_pThread;
		m_pThread = NULL;
		WriteLog("退出提醒窗口线程完毕", XJ_LOG_LV3);
	}
}

void CRemindView::OnThreadUpdate( WPARAM wParam, LPARAM lParam )
{
	Invalidate();
}
