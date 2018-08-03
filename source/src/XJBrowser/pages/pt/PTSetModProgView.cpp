// RemindView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSetModProgView.h"
#include "PTSetModStateItem.h"

#include "XJPTSetStore.h"

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
		WriteLog("�������Ѵ����߳�ʧ��!", XJ_LOG_LV3);
		return -1;
	}
	WriteLog("�������Ѵ����̳߳ɹ�!", XJ_LOG_LV3);
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
	WriteLog("�˳����Ѵ����̳߳ɹ�!", XJ_LOG_LV3);
	return 0;
}


IMPLEMENT_DYNCREATE(CPTSetModProgView, CScrollView)

CPTSetModProgView::CPTSetModProgView()
{
	//GetTypeNames();

	m_pHeadItem = new CPTSetModStateItem("װ������", 999);

	QMatrixByteArray &flow = CXJPTSetStore::GetInstance()->GetWorkFlow();
	//AfxMessageBox(flow.constData());
	for (int i = 1; i <= flow.GetRows(); i++){
		int nPTSetStateID = flow.GetFiled(i, 1).toInt();
		int nVisible = flow.GetFiled(i, 2).toInt();
		int nUserType = flow.GetFiled(i, 3).toInt();
		int nUserID = flow.GetFiled(i, 4).toInt();
		CString typeName = CXJPTSetStore::GetInstance()->GetFuncID(nPTSetStateID);
		CPTSetModStateItem *item = new CPTSetModStateItem(typeName, nPTSetStateID);
		item->SetVisible(nVisible);

		m_items.Add(item);
	}

	m_bThreadExit = FALSE;
	m_pThread = NULL;
	
	InitializeCriticalSection(&m_CriticalSection);
}

CPTSetModProgView::~CPTSetModProgView()
{
	DeleteCriticalSection(&m_CriticalSection);
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetModStateItem *item = m_items.GetAt(i);
		DELETE_POINTER(item);
	}
	m_items.RemoveAll();
	DELETE_POINTER(m_pHeadItem);
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
	
	int cy = 30 * (m_items.GetSize() + 1) + 80;
	CSize sizeTotal;
	sizeTotal.cx = 90;
	sizeTotal.cy = cy;
	SetScrollSizes(MM_TEXT, sizeTotal);
    
	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);//�����Ч����
	
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
		CPTSetModStateItem *item = m_items.GetAt(i);
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
	
	//�ͷ���Դ
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
������:   ResetObjSize	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTSetModProgView::ResetObjSize()
{
	CRect rect;
	GetClientRect(&rect);
	int w = ((rect.Width() - 20)/1);

	if(m_pHeadItem){
		m_pHeadItem->SetBound(10, 10, w, 30);
	}
	int j = 0;
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetModStateItem *item = m_items.GetAt(i);
		if(item && item->IsVisble()){
			item->SetBound(10, 50 + j*40, w, 30);
			j++;
		}
	}
}

void CPTSetModProgView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CScrollView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	ResetObjSize();
}

void CPTSetModProgView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		//KillTimer(m_nTimer);
		
		CXJPTSetStore *store = CXJPTSetStore::GetInstance();
		QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(store->GetCard()));
		QLogTable &log = *(reinterpret_cast<QLogTable *>(store->GetLog()));
		QMatrixByteArray &flow = store->GetWorkFlow();
		
// 		KillTimer(m_nTimer);
// 		AfxMessageBox(flow.constData());
// 		m_nTimer = SetTimer(501, 3*1000, 0);

		int nPTSetState = card.GetStateID();
		if (nPTSetState < 0 || card.GetPTID().isEmpty())
			return;
		
		int i, j;
		
//		KillTimer(m_nTimer);
		bool bResize = false;
		for (i = 0; i < m_items.GetSize(); i++){
			CPTSetModStateItem *item = m_items.GetAt(i);
			if (!item)
				continue;

			item->SetCurPTSetState(nPTSetState);

			int nItemState = item->GetPTSetState();
			for (int j = 1; j <= flow.GetRows(); j++){
				int nFlowState = flow.GetFiled(j, 1).toInt();
				if (nItemState != nFlowState)
					continue;

				if (!flow.GetFiled(j, 2).isEmpty()){
					int nVisible = flow.GetFiled(j, 2).toInt();
					
					if (item->IsVisble() != nVisible){
						item->SetVisible(nVisible);
						bResize = true;
					}
				}
				break;
			}
		}
//		m_nTimer = SetTimer(501, 3*1000, 0);

		if (bResize)
			ResetObjSize();

		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(card.GetPTID().constData(), TYPE_SEC);
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
			CPTSetModStateItem *item = m_items.GetAt(i);
			if (!item){
				continue;
			}
			
			item->SetContent("");
			item->SetMarked(FALSE);

			CString sContent;
			int nItemState = item->GetPTSetState();
			int nLogState = -1;
			for (j = 1; j <= nCount; j++){
				nLogState = log.GetFiled(j, 3).toInt();
				if (nLogState != nItemState)
					continue;

				item->SetMarked(TRUE);
				
				sContent.Format(" ִ��ʱ�䣺[ %s ]��ִ���û���[ %s ]"
					, log.GetFiled(j, 1).constData()
					, log.GetFiled(j, 2).constData());

				item->SetContent(sContent);
				
				break;
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
������:   UpdateAllObj	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTSetModProgView::UpdateAllObj()
{
	EnterCriticalSection(&m_CriticalSection);
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetModStateItem *item = m_items.GetAt(i);
		if (item && item->IsVisble()){
			item->Update(100);
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
}


/*************************************************************
 �� �� ����DocToClient()
 ���ܸ�Ҫ���߼�����ת�豸����
 �� �� ֵ: void
 ��    ����param1	Ҫת��������
**************************************************************/
//##ModelId=49B87B8502E6
void CPTSetModProgView::DocToClient( CPoint & pt )
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&pt);
}

/*************************************************************
 �� �� ����DocToClient()
 ���ܸ�Ҫ���߼�����ת�豸����
 �� �� ֵ: void
 ��    ����param1	Ҫת���ķ�Χ
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
 �� �� ����ClientToDoc()
 ���ܸ�Ҫ���豸����ת�߼�����
 �� �� ֵ: void
 ��    ����param1	Ҫת��������
**************************************************************/
//##ModelId=49B87B8502F0
void CPTSetModProgView::ClientToDoc( CPoint & pt )
{
	CClientDC dc(this);
	OnPrepareDC(&dc , NULL);
	dc.DPtoLP(&pt);
}

/*************************************************************
 �� �� ����ClientToDoc()
 ���ܸ�Ҫ���豸����ת�߼�����
 �� �� ֵ: void
 ��    ����param1	Ҫת���ķ�Χ
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
������:   OnAddRemind	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
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
 	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetModStateItem *item = m_items.GetAt(i);
 		if(item){
			if(item->IsMouseOn(pt))
				item->SetActive(TRUE);
			else
				item->SetActive(FALSE);
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
					//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
					//�رվɵ�,���µ�
					pSvgView->CloseCurrentView();
					pSvgView->OpenStationView(pObj->m_pStation->m_sID);
					pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
				}
				else
				{
					//����ͼ�Ƶ�ǰ��
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
	for (int i = 0; i < m_items.GetSize(); i++){
		CPTSetModStateItem* item = m_items.GetAt(i);
		if(item){
			if(item && item->IsMouseOn(pt))
				pReturn = item;
		}
 	}

	return pReturn;
}

void CPTSetModProgView::StartThread()
{
	if (m_pThread == NULL)
	{
		m_bThreadExit = FALSE;
		//������������豸�����߳�
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
		WriteLog("��ʼ�˳����Ѵ����߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		m_bThreadExit = TRUE;	//�����˳���־
		DWORD dw = WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pThread->m_hThread,NULL);
			WriteLog("���Ѵ����߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pThread;
		m_pThread = NULL;
		WriteLog("�˳����Ѵ����߳����", XJ_LOG_LV3);
	}
}

void CPTSetModProgView::OnThreadUpdate( WPARAM wParam, LPARAM lParam )
{
	Invalidate();
}
