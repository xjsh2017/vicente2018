// RemindView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSetProgView.h"

#include "PTSetStateItem.h"
#include "XJTagOutStore.h"
#include "qptsetstatetable.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTSetProgView

#define REMIND_REFRESHTIME	300

UINT RefreshItems(LPVOID pParam)
{
	if(!pParam)
	{
		WriteLog("启动提醒窗口线程失败!", XJ_LOG_LV3);
		return -1;
	}
	WriteLog("启动提醒窗口线程成功!", XJ_LOG_LV3);
	CPTSetProgView * pView = (CPTSetProgView*)pParam;
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


IMPLEMENT_DYNCREATE(CPTSetProgView, CScrollView)

CPTSetProgView::CPTSetProgView()
{
	//GetTypeNames();

	m_nLastPTSetType = -1;
	m_pHeadItem = new CPTSetStateItem("装置名称", 999);

	m_bThreadExit = FALSE;
	m_pThread = NULL;
	
	InitializeCriticalSection(&m_CriticalSection);
	ResetObj();
}

CPTSetProgView::~CPTSetProgView()
{
	DeleteCriticalSection(&m_CriticalSection);
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
		DELETE_POINTER(item);
	}
	m_items.RemoveAll();
	DELETE_POINTER(m_pHeadItem);
}


BEGIN_MESSAGE_MAP(CPTSetProgView, CScrollView)
	//{{AFX_MSG_MAP(CPTSetProgView)
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
// CPTSetProgView drawing

void CPTSetProgView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 90;
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	m_nTimer = SetTimer(501, 3*1000, 0);
	//StartThread();
}

void CPTSetProgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
	
	int cy = 30 * (m_items.GetSize() + 1) + 80;
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
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
		if(item){
			item->Draw(pDrawDC);
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
// CPTSetProgView diagnostics

#ifdef _DEBUG
void CPTSetProgView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPTSetProgView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSetProgView message handlers

/****************************************************
Date:2013/10/9  Author:LYH
函数名:   ResetObjSize	
返回值:   void	
功能概要: 
*****************************************************/
void CPTSetProgView::ResetObjSize()
{
	CRect rect;
	GetClientRect(&rect);
	int w = ((rect.Width() - 20)/1);

	if(m_pHeadItem){
		m_pHeadItem->SetBound(10, 10, w, 30);
	}
	int j = 0;
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
		if(item && item->IsVisble()){
			item->SetBound(10, 50 + j*40, w, 30);
			j++;
		}

		if (item && !item->IsVisble())
			item->SetBound(10, 50, w, 0);
	}
}

void CPTSetProgView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CScrollView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	ResetObjSize();
}

void CPTSetProgView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		//KillTimer(m_nTimer);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		
		CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
		QPTSetStateTable *pState = pStore->GetState();
		
		QByteArrayMatrix &flow = pState->GetWorkFlow();
		QByteArrayMatrix &log = pState->GetLogs();

		int nTagOutState = pState->GetStateID();
		int nTagOutType = pState->GetType();
		
		if (nTagOutState < 0 || pState->GetPTID().isEmpty())
			return;
		
		if (-1 == m_nLastPTSetType){
			m_nLastPTSetType = nTagOutType;
		}
		if (m_nLastPTSetType != nTagOutType
			&& m_nLastPTSetType == XJ_TAGOUT_UNDEFINE)
		{
			KillTimer(m_nTimer);

			m_nLastPTSetType = nTagOutType;
			AfxMessageBox("ResetObj");
			ResetObj();
			ResetObjSize();

			m_nTimer = SetTimer(501, 3*1000, 0);
		}
		int n1 = m_nLastPTSetType - XJ_TAGOUT_UNDEFINE;
		int n2 = nTagOutType - XJ_TAGOUT_UNDEFINE;
		// 刷新（挂牌原因变化时刷新）
		if (n1 * n2 == 0 && n1 + n2 > 0){
			m_nLastPTSetType = nTagOutType;
		}
		
		int i, j;
		KillTimer(m_nTimer);
		bool bResize = false;
		for (i = 0; i < m_items.GetSize(); i++){
			CPTSetStateItem *item = m_items.GetAt(i);
			if (!item)
				continue;
			
			int nItemState = item->GetPTSetState();
			item->SetCurPTSetState(nTagOutState);

			if (nTagOutType != XJ_TAGOUT_UNDEFINE){
				QByteArray typeName = pStore->GetFuncID(nItemState);
				item->SetTypeName(typeName.constData());
				//AfxMessageBox(typeName.constData());
			}

			for (int j = 1; j <= flow.GetRows(); j++){
				int nFlowPTSetType = flow.GetFieldValue(j, 1).toInt();
				int nFlowState = flow.GetFieldValue(j, 2).toInt();

				if (nItemState != nFlowState)
					continue;

				if (!flow.GetFieldValue(j, 3).isEmpty()){
					int nVisible = flow.GetFieldValue(j, 3).toInt();
					
					if (item->IsVisble() != nVisible){
						item->SetVisible(nVisible);
						//AfxMessageBox("bResize");
						bResize = true;
					}
				}
				break;
			}
		}
		m_nTimer = SetTimer(501, 3*1000, 0);

		if (bResize)
			ResetObjSize();

		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pState->GetPTID().constData(), TYPE_SEC);
		if (NULL == pObj)
			return;
		
		KillTimer(m_nTimer);
		if (m_pHeadItem){
			if (pObj->m_pStation){
				str.Format(" [ %s ][ %s ]", pObj->m_pStation->m_sName, pObj->m_sName);
			}else
				str.Format("  %s", pObj->m_sName);

			m_pHeadItem->SetContent(str);
		}

		int nCount = log.GetRecordCount();
		for (i = 0; i < m_items.GetSize(); i++){
			CPTSetStateItem *item = m_items.GetAt(i);
			if (!item){
				continue;
			}
			
			item->SetContent("");
			item->SetMarked(FALSE);

			CString sContent;
			int nItemState = item->GetPTSetState();
			int nLogState = -1;
			for (j = 1; j <= nCount; j++){
				nLogState = log.GetFieldValue(j, 3).toInt();
				if (nLogState != nItemState)
					continue;

				item->SetMarked(TRUE);
				
				sContent.Format(" 执行时间：[ %s ]：执行用户：[ %s ]"
					, log.GetFieldValue(j, 1).constData()
					, log.GetFieldValue(j, 2).constData());

				item->SetContent(sContent);
				
				break;
			}
		}
		
		Invalidate();
		m_nTimer = SetTimer(501, 3*1000, 0);
	}

	CScrollView::OnTimer(nIDEvent);
}

BOOL CPTSetProgView::OnEraseBkgnd(CDC* pDC) 
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
void CPTSetProgView::UpdateAllObj()
{
	EnterCriticalSection(&m_CriticalSection);
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
		if (item && item->IsVisble()){
			item->Update(100);
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
void CPTSetProgView::DocToClient( CPoint & pt )
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
void CPTSetProgView::DocToClient( CRect & rect )
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
void CPTSetProgView::ClientToDoc( CPoint & pt )
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
void CPTSetProgView::ClientToDoc( CRect & rect )
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
void CPTSetProgView::OnAddRemind( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	EnterCriticalSection(&m_CriticalSection);
	CPTSetStateContent* pRC = (CPTSetStateContent*)lParam;
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

void CPTSetProgView::UpdateActive( CPoint pt )
{
 	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
 		if(item){
			if(item->IsMouseOn(pt))
				item->SetActive(TRUE);
			else
				item->SetActive(FALSE);
 		}
 	}
}


void CPTSetProgView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);

	//UpdateActive(ptDoc);
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CPTSetProgView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CPTSetProgView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);
	CPTSetStateItem* pSelect = MouseOnObj(ptDoc);
	if(pSelect)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(pSelect == m_pHeadItem){
			//pApp->GetMainWnd()->SendMessage(PUSHTOP_FVIEW, 0, FVIEW_ACTION);
			//AfxMessageBox("hello, world");

			CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
			QPTSetStateTable *pState = pStore->GetState();
			
			int nTagOutState = pState->GetStateID();
			int nHangoutType = pState->GetType();

			CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pState->GetPTID().constData(), TYPE_SEC);
			CMainFrame * pFrame = (CMainFrame*)pApp->m_pMainWnd;
			CDeviceView *pView = pFrame->m_pDeviceView;
			if (pObj){
				pView->LocateObjInTree(pObj);

				CXJBrowserDoc * pDoc = pApp->GetCurDocument();
				if (XJ_TAGOUT_PTVALVSET == nHangoutType || XJ_TAGOUT_PTZONESET == nHangoutType)
					pDoc->ShowSecPropPage(pObj, (ID_PT_SETTING_NEW - ID_PT_GENERAL_NEW));
				else if (XJ_TAGOUT_PTSOFTSET == nHangoutType)
					pDoc->ShowSecPropPage(pObj, (ID_PT_SOFTBOARD_NEW - ID_PT_GENERAL_NEW));
				else{
					pDoc->ShowSecPropPage(pObj, (ID_PT_ACTION_NEW - ID_PT_GENERAL_NEW));
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
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

CPTSetStateItem* CPTSetProgView::MouseOnObj( CPoint pt )
{
	CPTSetStateItem* pReturn = NULL;

	if (m_pHeadItem && m_pHeadItem->IsMouseOn(pt))
		pReturn = m_pHeadItem;
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem* item = m_items.GetAt(i);
		if(item){
			if(item && item->IsMouseOn(pt))
				pReturn = item;
		}
 	}

	return pReturn;
}

void CPTSetProgView::StartThread()
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

void CPTSetProgView::EndThread()
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

void CPTSetProgView::OnThreadUpdate( WPARAM wParam, LPARAM lParam )
{
	Invalidate();
}

void CPTSetProgView::ResetObj()
{
	EnterCriticalSection(&m_CriticalSection);
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetStateItem *item = m_items.GetAt(i);
		if(item){
			DELETE_POINTER(item);
		}
	}
	m_items.RemoveAll();

	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	pStore->ReLoadState();
	QPTSetStateTable *pState = pStore->GetState();
	
	int nTagOutState = pState->GetStateID();
	int nTagOutType = pState->GetType();
	
	QByteArrayMatrix &flow = pState->GetWorkFlow();
	for (int i = 1; i <= flow.GetRows(); i++){
		int nFlowPTSetType = flow.GetFieldValue(i, 1).toInt();
		int nFlowPTSetStateID = flow.GetFieldValue(i, 2).toInt();
		
		if (XJ_OPER_HANGOUT != nFlowPTSetStateID
			&& XJ_OPER_UNHANGOUT != nFlowPTSetStateID){
			
			if (nFlowPTSetType != nTagOutType)
				continue;
		}
		
		int nVisible = flow.GetFieldValue(i, 3).toInt();
		int nUserType = flow.GetFieldValue(i, 4).toInt();
		int nUserID = flow.GetFieldValue(i, 5).toInt();
		CString typeName = CXJTagOutStore::GetInstance()->GetFuncID(nFlowPTSetStateID);
		CPTSetStateItem *item = new CPTSetStateItem(typeName, nFlowPTSetStateID);
		item->SetVisible(nVisible);
		
		m_items.Add(item);
	}

	LeaveCriticalSection(&m_CriticalSection);
}
