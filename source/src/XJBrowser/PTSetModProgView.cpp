// RemindView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSetModProgView.h"
#include "PTSetModStateItem.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTSetModProgView

#define REMIND_REFRESHTIME	300

UINT RefreshItems(LPVOID pParam)
{
	if(!pParam)
	{
		WriteLog("启动提醒窗口线程失败!", XJ_LOG_LV3);
		return -1;
	}
	WriteLog("启动提醒窗口线程成功!", XJ_LOG_LV3);
	CPTSetModProgView * pView = (CPTSetModProgView*)pParam;
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


IMPLEMENT_DYNCREATE(CPTSetModProgView, CScrollView)

CPTSetModProgView::CPTSetModProgView()
{
	GetTypeNames();

	m_pHeadItem = new CPTSetModStateItem("装置名称", 999);
	for (int i = 0; i < _countof(m_arrTypeNames); i++){
		m_pItems[i] = new CPTSetModStateItem(m_arrTypeNames[i], i);
	}

	m_bThreadExit = FALSE;
	m_pThread = NULL;
	
	InitializeCriticalSection(&m_CriticalSection);
}

CPTSetModProgView::~CPTSetModProgView()
{
	DeleteCriticalSection(&m_CriticalSection);
	for (int i = 0; i < _countof(m_pItems); i++){
		delete m_pItems[i];
		m_pItems[i] = NULL;
	}

	delete m_pHeadItem;
	m_pHeadItem = NULL;
}


BEGIN_MESSAGE_MAP(CPTSetModProgView, CScrollView)
	//{{AFX_MSG_MAP(CPTSetModProgView)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(ADD_REMIND, OnAddRemind)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSetModProgView drawing

void CPTSetModProgView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 90;
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	m_nTimer = SetTimer(501, 3*1000, 0);
	//StartThread();
}

void CPTSetModProgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	
	int cy = 30 * (_countof(m_pItems) + 1) + 80;
	CSize sizeTotal;
	sizeTotal.cx = 90;
	sizeTotal.cy = cy;
	SetScrollSizes(MM_TEXT, sizeTotal);
    
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
	rect.InflateRect(0, 0, 0, cy);
	pDrawDC->FillSolidRect(rect, RGB(255, 255, 255));
	// TODO: add draw code here
	EnterCriticalSection(&m_CriticalSection);
	if (m_pHeadItem){
		m_pHeadItem->Draw(pDrawDC);
	}
	for (int i = 0; i < _countof(m_pItems); i++){
		if(m_pItems[i]){
			m_pItems[i]->Draw(pDrawDC);
		}
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
// CPTSetModProgView diagnostics

#ifdef _DEBUG
void CPTSetModProgView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPTSetModProgView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSetModProgView message handlers

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   ResetObjSize	
返回值:   void	
功能概要: 
*****************************************************/
void CPTSetModProgView::ResetObjSize()
{
	CRect rect;
	GetClientRect(&rect);
	int w = ((rect.Width() - 20)/1);

	if(m_pHeadItem){
		m_pHeadItem->SetBound(10, 10, w, 30);
	}
	for (int i = 0; i < _countof(m_pItems); i++){
		if(m_pItems[i]){
			m_pItems[i]->SetBound(10, 50 + i*40, w, 30);
		}
	}

}

void CPTSetModProgView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CScrollView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	ResetObjSize();
}

void CPTSetModProgView::GetTypeNames()
{
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	m_arrUserGroups[0] = pApp->GetUserGroupNameByID(StringFromID(IDS_USERGROUP_RUNNER));
	m_arrUserGroups[1] = pApp->GetUserGroupNameByID(StringFromID(IDS_USERGROUP_OPERATOR));
	m_arrUserGroups[2] = pApp->GetUserGroupNameByID(StringFromID(IDS_USERGROUP_MONITOR));

	CString c_dz_mod_state[] = {
		StringFromID(IDS_DZ_MOD_STATE_1),
			StringFromID(IDS_DZ_MOD_STATE_2),
			StringFromID(IDS_DZ_MOD_STATE_3),
			StringFromID(IDS_DZ_MOD_STATE_4),
			StringFromID(IDS_DZ_MOD_STATE_5)
			//, StringFromID(IDS_DZ_MOD_STATE_0)
	};

	int i;
	CString str;
	int arrGroups[5] = {0, 1, 2, 0, 1};
	CString arrTypeNames[5];
	for (i = 0; i < _countof(arrTypeNames); i++){
		str.Format("%s: %s", m_arrUserGroups[arrGroups[i]], c_dz_mod_state[i]);
		m_arrTypeNames[i] = str;
		//AfxMessageBox(str);
		//if (m_pItems[i]){
		//	m_pItems[i]->SetTypeName(str);
		//}
	}
}

void CPTSetModProgView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		//KillTimer(m_nTimer);
		
		PT_ZONE zone;
		CString sRecords;
		int nCurState = pApp->GetPTSetModState(zone, sRecords);
		if (nCurState < 0 || zone.PT_ID.IsEmpty())
			return;
		
		CString c_dz_mod_state[] = {
				StringFromID(IDS_DZ_MOD_STATE_1),
				StringFromID(IDS_DZ_MOD_STATE_2),
				StringFromID(IDS_DZ_MOD_STATE_3),
				StringFromID(IDS_DZ_MOD_STATE_4),
				StringFromID(IDS_DZ_MOD_STATE_5)
				//, StringFromID(IDS_DZ_MOD_STATE_0)
		};

		int i;
		for (i = 0; i < _countof(m_pItems); i++){
			if (m_pItems[i] && nCurState > 0){
				m_pItems[i]->SetCurIndex(nCurState - 1);
			}
		}

		str.Format("nCurState = %d", nCurState);
		//AfxMessageBox(str);
		
		if (sRecords.IsEmpty())
			return;
		
		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(zone.PT_ID, TYPE_SEC);
		if (NULL == pObj)
			return;

		if (m_pHeadItem){
			if (pObj->m_pStation){
				str.Format("  [%s]%s", pObj->m_pStation->m_sName, pObj->m_sName);
			}else
				str.Format("  %s", pObj->m_sName);

			m_pHeadItem->SetContent(str);
		}

		KillTimer(m_nTimer);

		vector<CString> arrRecords = pApp->SplitCString(sRecords, ';');
		int nCount = arrRecords.size();
		int nRows = _countof(m_pItems);
		str.Format("records = %s, nCount = %d, rows = %d", sRecords, nCount, nRows);
		//AfxMessageBox(str);
		for (i = 0; i < nRows; i++){
			CString sContent;
			if (i > nCount - 1){
				sContent = "";
			}else{
				vector<CString> arrItems = pApp->SplitCString(arrRecords[i], ',');

				if (arrItems.size() != 3)
					continue;
				sContent.Format(" 执行时间：[%s]：执行用户：[%s]", arrItems[0], arrItems[1]);
				//CPTSetModStateContent * pRC = new CPTSetModStateContent(c_dz_mod_state[i], sContent);
			}

			if (m_pItems[i]){
				m_pItems[i]->SetContent(sContent);
			}
		}
		
		Invalidate();
		m_nTimer = SetTimer(501, 3*1000, 0);
	}

	CScrollView::OnTimer(nIDEvent);
}

BOOL CPTSetModProgView::OnEraseBkgnd(CDC* pDC) 
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
void CPTSetModProgView::UpdateAllObj()
{
	EnterCriticalSection(&m_CriticalSection);
	for (int i = 0; i < _countof(m_pItems); i++){
		if(m_pItems[i]){
			m_pItems[i]->Update(100);
		}
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
void CPTSetModProgView::DocToClient( CPoint & pt )
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
void CPTSetModProgView::DocToClient( CRect & rect )
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
void CPTSetModProgView::ClientToDoc( CPoint & pt )
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
void CPTSetModProgView::ClientToDoc( CRect & rect )
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
void CPTSetModProgView::OnAddRemind( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	EnterCriticalSection(&m_CriticalSection);
	CPTSetModStateContent* pRC = (CPTSetModStateContent*)lParam;
// 	for (int i = 0; i < _countof(m_pItems); i++){
// 		if(m_pItems[i]){
// 			m_pItems[i]->Update(100);
// 		}
// 	}
// 	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_ACTION) && m_pAction)
// 		m_pAction->AddContent(pRC);
// 	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_ALARM) && m_pAlarm)
// 		m_pAlarm->AddContent(pRC);
// 	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_OSC) && m_pOsc)
// 		m_pOsc->AddContent(pRC);
// 	if(pRC->GetTypeName() == StringFromID(IDS_TOOLBAR_FAULTREPORT) && m_pFault)
// 		m_pFault->AddContent(pRC);
	LeaveCriticalSection(&m_CriticalSection);
}

void CPTSetModProgView::UpdateActive( CPoint pt )
{
 	for (int i = 0; i < _countof(m_pItems); i++){
 		if(m_pItems[i]){
			if(m_pItems[i]->IsMouseOn(pt))
				m_pItems[i]->SetActive(TRUE);
			else
				m_pItems[i]->SetActive(FALSE);
 		}
 	}
}


void CPTSetModProgView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);

	//UpdateActive(ptDoc);
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CPTSetModProgView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CPTSetModProgView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);
	CPTSetModStateItem* pSelect = MouseOnObj(ptDoc);
	if(pSelect)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(pSelect == m_pHeadItem){
			//pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_ACTION);
			//AfxMessageBox("hello, world");

			PT_ZONE zone;
			pApp->GetPTSetModState(zone);

			CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(zone.PT_ID, TYPE_SEC);
			CMainFrame * pFrame = (CMainFrame*)pApp->m_pMainWnd;
			CDeviceView *pView = pFrame->m_pDeviceView;
			if (pObj){
				pView->LocateObjInTree(pObj);

				CXJBrowserDoc * pDoc = pApp->GetCurDocument();
				pDoc->ShowSecPropPage(pObj, (ID_PT_SETTING_NEW - ID_PT_GENERAL_NEW));
				
				/*CXJBrowserView* pSvgView = pApp->GetSVGView();
				if(pSvgView == NULL)
					return;
				if(pObj->m_pStation && pObj->m_pStation->m_sID != pSvgView->m_sStationID || pSvgView->m_nSvgType != 1)
				{
					//现在选择的厂站与当前打开的图形不是同一厂站
					//关闭旧的,打开新的
					pSvgView->CloseCurrentView();
					pSvgView->OpenStationView(pObj->m_pStation->m_sID);
					pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
				}
				else
				{
					//把视图推到前面
					pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
				}*/
			}
		}
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

CPTSetModStateItem* CPTSetModProgView::MouseOnObj( CPoint pt )
{
	CPTSetModStateItem* pReturn = NULL;

	if (m_pHeadItem && m_pHeadItem->IsMouseOn(pt))
		pReturn = m_pHeadItem;
	for (int i = 0; i < _countof(m_pItems); i++){
		if(m_pItems[i]){
			if(m_pItems[i] && m_pItems[i]->IsMouseOn(pt))
				pReturn = m_pItems[i];
		}
 	}

	return pReturn;
}

void CPTSetModProgView::StartThread()
{
	if (m_pThread == NULL)
	{
		m_bThreadExit = FALSE;
		//启动清理二次设备配置线程
		m_pThread = AfxBeginThread(RefreshItems,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pThread != NULL)
		{
			m_pThread->m_bAutoDelete = FALSE;
			m_pThread->ResumeThread();
		}
	}
}

void CPTSetModProgView::EndThread()
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

void CPTSetModProgView::OnThreadUpdate( WPARAM wParam, LPARAM lParam )
{
	Invalidate();
}
