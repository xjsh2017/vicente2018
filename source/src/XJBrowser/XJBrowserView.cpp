// XJBrowserView.cpp : implementation of the CXJBrowserView class
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "XJBrowserDoc.h"
#include "XJBrowserView.h"
#include "DlgSignalList.h"
#include "Dlg_DevRunStatusUpdate.h"
#include "GlobalFunc.h"
#include "DLGMarked.h"
#include "DraftDocument.h"
#include "DlgDraft.h"
/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/*************************************************************
 �� �� ����RefreshStatus()
 ���ܸ�Ҫ��ˢ�±�����¼��������, ���߳��ڴ򿪳�վͼ��ʱ����, ˢ����������˳�.
 �� �� ֵ: UINT
 ��    ����param1	����ͼ��ָ��
**************************************************************/
UINT RefreshStatus(LPVOID pParam)
{
//	WriteLog("ˢ��ͼ��״̬�߳�����!");
	TRACE("ˢ��ͼ��״̬�߳�����!\n");
	//�õ������
	CXJBrowserView* pView = (CXJBrowserView*)pParam;
	//�����Ч��
	if(pView == NULL)
	{
		//��Чָ��
		return 0;
	}
	if(pView->m_pStation == NULL)
	{
		//�޳�վ����
		return 0;
	}
	if(!pView->m_bLoaded)
	{
		//��ͼ��
		return 0;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return 0;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pView->m_pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		if(pView->m_bExitRefresh)
		{
			//�߳��˳���־
			return 0;
		}
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//������ɫ
		pView->SetDeviceCol(pObj);
	}
	
	//��ɹ���,�����˳�
//	WriteLog("ˢ��ͼ��״̬�߳���ɹ���, �����˳��߳�!");
	TRACE("ˢ��ͼ��״̬�߳���ɹ���, �����˳��߳�!\n");
	return 0;
}

UINT BlinkShape(LPVOID pParam)
{
	CXJBrowserView * pView = (CXJBrowserView *)pParam;
	
	while(1)
	{
		if(pView ->m_bExitBlink)
		{
			pView ->m_bExitBlink = FALSE;
			return 0;
		}
		Sleep(pView->m_nBlinkTime); //˯
		if(pView ->m_bExitBlink)
		{
			pView ->m_bExitBlink = FALSE;
			return 0;
		}
		
		pView ->InvalidateRect(pView ->m_rcBlink, FALSE);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserView

IMPLEMENT_DYNCREATE(CXJBrowserView, CFormView)

BEGIN_MESSAGE_MAP(CXJBrowserView, CFormView)
	//{{AFX_MSG_MAP(CXJBrowserView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PT_SIGNALLIST, OnPtSignallist)
	ON_COMMAND(ID_QUERY_EVENT, OnQueryEvent)
	ON_COMMAND(ID_MAP_REFRESH, OnMapRefresh)
	ON_COMMAND(ID_MAP_STOPALART, OnMapStopalart)
	ON_COMMAND(ID_MAP_ZOOMFULL, OnMapZoomfull)
	ON_COMMAND(ID_MAP_ZOOMIN, OnMapZoomin)
	ON_COMMAND(ID_MAP_ZOOMOUT, OnMapZoomout)
	ON_COMMAND(ID_MAP_ZOOM_OLD, OnMapZoomOld)
	ON_UPDATE_COMMAND_UI(ID_MAP_ZOOM_OLD, OnUpdateMapZoomOld)
	ON_UPDATE_COMMAND_UI(ID_MAP_ZOOMFULL, OnUpdateMapZoomfull)
	ON_UPDATE_COMMAND_UI(ID_MAP_ZOOMIN, OnUpdateMapZoomin)
	ON_UPDATE_COMMAND_UI(ID_MAP_ZOOMOUT, OnUpdateMapZoomout)
    ON_COMMAND(ID_MENUITEM_DevTree_PTRunUpdate, OnMENUITEMDevTreePTRunUpdate)
	ON_WM_TIMER()
	ON_COMMAND(ID_MARK_DEVICE, OnMarkDevice)
	ON_COMMAND(ID_MARK_PTDEVICE, OnMarkPtdevice)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_DRAFT, OnBtnDraft)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_PT_GENERAL_NEW,ID_PT_OSC_LIST_NEW,OnPTProp)
	ON_COMMAND_RANGE(ID_WR_GENERAL_NEW,ID_WR_OSC_LIST_NEW,OnWRProp)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20022)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(STATION_CHANGED, OnStationChanged)
	ON_MESSAGE(SHOWTOOPTIP, OnShowTooltip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserView construction/destruction

//##ModelId=49B87B7C0272
CXJBrowserView::CXJBrowserView()
	: CFormView(CXJBrowserView::IDD)
{
	//{{AFX_DATA_INIT(CXJBrowserView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
//	m_nViewType = XJ_VIEW_MAP;
	m_bInitCOM = FALSE;
	m_bLoaded = FALSE;
	pClf = NULL;
	pUnk = NULL;
	pSvgViewer = NULL;
	m_sStationID = "";
	m_sCurrentPath = "";
	m_szOriSize.cx = 0;
	m_szOriSize.cy = 0;
	m_szCurSize.cx = 0;
	m_szCurSize.cy = 0;
	m_pStation = NULL;
	m_bExitRefresh = FALSE;
	m_pStatusRefresh = NULL;
	m_pSelectedStation = NULL;

	m_rcBlink.SetRect(32766, 32766, -32766, -32766);
	m_bExitBlink = FALSE;
	m_pBlinkThread = NULL;
	m_nBlinkTime = 500;

	m_pCurBay = NULL;
	m_nSvgType = 0;
	m_nCount = 100;

	m_bRefreshMark = FALSE;
	m_bRefreshNoteMark = FALSE;

	listBlinkDevice.RemoveAll();

	InitializeCriticalSection(&m_CriticalSection);

	InitCOM();
}

//##ModelId=49B87B7D0000
CXJBrowserView::~CXJBrowserView()
{
	WriteLog("CXJBrowserView::~CXJBrowserView start", XJ_LOG_LV3);
	ReleaseTemp();
	if(pSvgViewer != NULL)
	{
		pSvgViewer->Release();
		pSvgViewer = NULL;
	}
	if(pUnk != NULL)
		pUnk->Release();
	if(pClf != NULL)
		pClf->Release();

	DeleteCriticalSection(&m_CriticalSection);


	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos != NULL)
	{
		TimerTick* pTime = (TimerTick*)m_PtrList.GetNext(pos);
		if (pTime != NULL)
		{
			delete pTime;
			pTime = NULL;
		}
	}
	m_PtrList.RemoveAll();

	WriteLog("CXJBrowserView::~CXJBrowserView", XJ_LOG_LV3);
}

//##ModelId=49B87B7C035D
void CXJBrowserView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXJBrowserView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//##ModelId=49B87B7C034F
BOOL CXJBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

//##ModelId=49B87B7C036D
void CXJBrowserView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
//	ResizeParentToFit();
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	m_szCommWnd.cx = 220;
	m_szCommWnd.cy = 200;
	if(g_role != MODE_SUB)
	{
		m_wndComm.CreateWnd(this, CPoint(100, 100), m_szCommWnd);
	
		m_wndComm.Show();
	}

	m_szHangoutWnd.cx = 220;
	m_szHangoutWnd.cy = 120;
	if(g_role != MODE_SUB)
	{
		m_wndHangout.CreateWnd(this, CPoint(100, 200), m_szHangoutWnd);
		
		m_wndHangout.Show();
	}

	if( !g_bShowDraft )
	{
		//����ʾʩ��ͼ
		CWnd* pBtn = (CWnd*)GetDlgItem(IDC_BTN_DRAFT);
		if(pBtn)
		{
			pBtn->ShowWindow( SW_HIDE );
			pBtn->EnableWindow( FALSE );
		}
	}

	m_tooltip.Create(this);
}

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserView printing

//##ModelId=49B87B7C037B
BOOL CXJBrowserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

//##ModelId=49B87B7C038A
void CXJBrowserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

//##ModelId=49B87B7C039A
void CXJBrowserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

//##ModelId=49B87B7C03AA
void CXJBrowserView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserView diagnostics

#ifdef _DEBUG
//##ModelId=49B87B7D0002
void CXJBrowserView::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B7D0010
void CXJBrowserView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

//##ModelId=49B87B7C0273
CXJBrowserDoc* CXJBrowserView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXJBrowserDoc)));
	return (CXJBrowserDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserView message handlers

//##ModelId=49B87B7C03C8
void CXJBrowserView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONGRPHA));
	//GetParentFrame()->SetWindowText("������ͼ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B7C03D8
void CXJBrowserView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap = NULL;
    
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

	if(m_bLoaded)
	{
		CRect rect;
		GetClientRect(&rect);
		ClientToDoc(rect);
		pDrawDC ->FillSolidRect(rect, RGB(18, 69, 60));
		
		//����SVGͼ��
		//////////////////////////////////////////////////////////////////////////
		if(pSvgViewer != NULL)
		{
			pSvgViewer->DrawShapes(pDrawDC);
			if( g_MarkDownOscPT && !m_bRefreshMark )
			{
				RefreshDeviceMark();
				m_bRefreshMark = TRUE;
				Invalidate();
			}
			if( g_PTShowNote && !m_bRefreshNoteMark)
			{
				RefreshNoteMark();
				m_bRefreshNoteMark = TRUE;
				Invalidate();
			}
		}
	//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		pDrawDC ->FillSolidRect(rect, RGB(255, 255, 255));
		CString str = StringFromID(IDS_STATIONMAP_SHOWERROR);
		pDrawDC ->TextOut(20, 20, str);
		CRect rc;
		rc.left = 20;
		rc.top = 50;
		rc.right = rect.Width()/2;
		rc.bottom = 53;
		pDrawDC->FillSolidRect(rc, RGB(128, 128, 128));
		str = StringFromID(IDS_NOGRAPH_REASON1);
		pDrawDC ->TextOut(30, 60, str);
		str = StringFromID(IDS_NOGRAPH_REASON2);
		pDrawDC ->TextOut(30, 90, str);
		str = StringFromID(IDS_NOGRAPH_REASON3);
		pDrawDC ->TextOut(30, 120, str);
	}

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

/*************************************************************
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ��COM�ؼ�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7C02F1
void CXJBrowserView::InitCOM()
{
	/*HRESULT hr;
	if((hr = ::CLSIDFromProgID(L"XJSvgViewer", &clsid)) != NOERROR)
	{
		TRACE("unable to find program ID -- error= %x\n", hr);
		return;
	}
	
	if((hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, NULL, 
		IID_IClassFactory, (void **)&pClf)) != NOERROR)
	{
		TRACE("unable to find CLSID -- error = %x\n", hr);
		return;
	}
	
	pClf ->CreateInstance(NULL, IID_IUnknown, (void **)&pUnk);
	//ȡ����ʾ���ܽӿ�
	pUnk ->QueryInterface(IID_ISvgViewer, (void **)&pSvgViewer);*/
	m_bInitCOM = InitSVGComNoReg(pClf, pSvgViewer);
	
	if(pSvgViewer == NULL)
	{
		TRACE("unable to load SvgViewer Interface\n");
		return;
	}
	//������˸����
	pSvgViewer->CartoonControl(TRUE, this);

	//m_bInitCOM = TRUE;
}

/*************************************************************
 �� �� ����OpenView()
 ���ܸ�Ҫ����ָ����SVG�ļ�
 �� �� ֵ: �򿪳ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�ļ�·��
**************************************************************/
//##ModelId=49B87B7C02CF
BOOL CXJBrowserView::OpenView( CString strPath )
{
	if(!m_bInitCOM)
		return FALSE;
	if(strPath == "")
		return FALSE;
	if(pSvgViewer == NULL)
		return FALSE;
	CString sError = "";
	CRect rcBound;
	int nResult = pSvgViewer->LoadFromFile(strPath, rcBound, sError);
	CSize szSize(rcBound.Width(), rcBound.Height());
	if(nResult <0)//����ʧ��
	{
		CString str;
		str.Format("��վ����ͼ����ͼ��ʧ�ܣ�ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV1);
		m_bLoaded = FALSE;
		szSize.cx = 100;
		szSize.cy = 100;
		SetScrollSizes(MM_TEXT, szSize);
		Invalidate();
		return FALSE;
	}
	else
	{

		WriteLog("һ�ν���ͼ, open the svg sucess", XJ_LOG_LV3);
		m_bLoaded = TRUE;
		if(szSize.cx < 0)
			szSize.cx = 100;
		if(szSize.cy <= 0)
			szSize.cy = 100;
		SetScrollSizes(MM_TEXT, szSize);
		m_szOriSize = szSize;
		m_szCurSize = m_szOriSize;

		//ˢ��
		CRect rc(0, 0, szSize.cx, szSize.cy);
		DocToClient(rc);
		InvalidateRect(rc);

		//ˢ������
		Invalidate();

		//ˢ���豸״̬
		RefreshDeviceStatus();

	}
	return TRUE;
}

//##ModelId=49B87B7D001F
void CXJBrowserView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_tooltip.HideToolTip();
	//��ͼ������ʱ������
	if(!m_bLoaded)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();

	//�����ѡ����,ֻ��ʾ��һ��������
	if(m_selected.GetCount() > 0)
	{
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		if(pDevice->m_nType == TYPE_PROTECT || pDevice->m_nType == TYPE_WAVEREC || pDevice->m_nType == TYPE_SEC)
		{
			//������¼����
			CSecObj* pObj = (CSecObj*)pDevice;
			pDoc->ShowSecPropPage(pObj);
		}
	}
	
	CFormView::OnLButtonDblClk(nFlags, point);
}

/*************************************************************
 �� �� ����DocToClient()
 ���ܸ�Ҫ���߼�����ת�豸����
 �� �� ֵ: void
 ��    ����param1	Ҫת��������
**************************************************************/
//##ModelId=49B87B7C02FD
void CXJBrowserView::DocToClient( CPoint & pt )
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
//##ModelId=49B87B7C02FF
void CXJBrowserView::DocToClient( CRect & rect )
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
//##ModelId=49B87B7C030E
void CXJBrowserView::ClientToDoc( CPoint & pt )
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
//##ModelId=49B87B7C031C
void CXJBrowserView::ClientToDoc( CRect & rect )
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
	rect.NormalizeRect();
}

//##ModelId=49B87B7D002E
void CXJBrowserView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_tooltip.HideToolTip();

	if(m_bLoaded)
	{
		if(pSvgViewer != NULL)
			pSvgViewer ->DoLButtonDown(this, point, nFlags, 0);	
	}

	
	
	CFormView::OnLButtonDown(nFlags, point);
}

//##ModelId=49B87B7D003E
void CXJBrowserView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();

	if(m_bLoaded && pSvgViewer != NULL)
	{
		pSvgViewer->DoLButtonUp(this, point, nFlags, 0);
		
		//���ѡ��		
		CUIntArray arrSelected;
		arrSelected.RemoveAll();
		pSvgViewer->GetCurSelected(arrSelected);

		//ɾ���ɵ�ѡ��
		m_selected.RemoveAll();
		
		for(int i = 0; i < arrSelected.GetSize(); i++)
		{
			CDeviceObj * pObj = NULL;
            CString str103ID;
			CString strUri;
			CString strType;
			
			//���Ҷ�Ӧ�豸
			int nIndex = arrSelected.GetAt(i);
			str103ID = pSvgViewer->GetDevID(nIndex);
			strType = pSvgViewer ->GetDevType(nIndex);
			int nType = TypeStringToInt(strType);
			pObj = FindDevicePlus(str103ID, strType);
			
			if(pObj != NULL)
			{
				m_selected.AddTail(pObj);
			}
			else
			{
				CString str;
				str.Format("SVGͼ����,IDΪ%s,typeΪ%s��ͼ��δ�ҵ���Ӧװ��", str103ID, strType);
				WriteLog(str, XJ_LOG_LV2);
			}
		}

		//ˢ���豸״̬
		//RefreshDeviceStatus();
	}

	if( 1 == m_selected.GetCount() )
	{
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		if(pDevice->m_nType == TYPE_PROTECT || pDevice->m_nType == TYPE_WAVEREC || pDevice->m_nType == TYPE_SEC)
		{
			//������¼����
			CSecObj* pObj = (CSecObj*)pDevice;
			if( pObj->m_nNoteCount > 0 )
			{
				this->PostMessage(SHOWTOOPTIP, 0, 0);
			}
		}
	}
	
	CFormView::OnLButtonUp(nFlags, point);
}

//##ModelId=49B87B7D004E
void CXJBrowserView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	if(m_bLoaded)
	{
		if(pSvgViewer != NULL)
			pSvgViewer ->DoMouseMove(this, point, nFlags, 0);
	}

	CFormView::OnMouseMove(nFlags, point);
}

/*************************************************************
 �� �� ����StringToID()
 ���ܸ�Ҫ����COM�ؼ����ص�ѡ���豸�ַ���ת��Ϊ���豸ID
 �� �� ֵ: void
 ��    ����param1	ѡ���豸�ַ���
		   Param2	�豸ID����
**************************************************************/
//##ModelId=49B87B7C032C
void CXJBrowserView::StringToID( CString strID, CStringArray & arrID )
{
	arrID.RemoveAll();
	
	if(strID.Find(',', 0) == -1) //ֻ��һ��
	{
		arrID.Add(strID);
	}
	else
	{
		int nResult = 0;
		int nOldResult = 0;
		int count = 1;
		CString str;
		
		while(nResult != -1)
		{
			nResult = strID.Find(',', nResult + 1);
			if(nResult != -1)
				count ++;
		}
		
		for(int i = 0; i < count; i++)
		{
			if(i == 0)
			{
				nResult = strID.Find(',', nOldResult);
				str = strID.Mid(nOldResult, (nResult -nOldResult));
				nOldResult = nResult;
				arrID.Add(str);
			}
			else if(i == count -1)
			{
				nResult = strID.Find(',', nOldResult);
				str = strID.Right(strID.GetLength() - (nResult+1));
				arrID.Add(str);
			}
			else
			{
				nResult = strID.Find(',', nOldResult + 1);
				str = strID.Mid(nOldResult + 1, nResult - nOldResult -1);
				nOldResult = nResult;
				arrID.Add(str);
			}
		}
		
	}
}

/*************************************************************
 �� �� ����TypeStringToInt()
 ���ܸ�Ҫ���ַ��͵��豸����ת��Ϊ���ε��豸����
 �� �� ֵ: ���ε��豸����
 ��    ����param1	�ַ��͵��豸����
**************************************************************/
//##ModelId=49B87B7C032F
int CXJBrowserView::TypeStringToInt( CString sType )
{
	/*
#define STRING_BUS		"ĸ��"		//ĸ��
#define STRING_LINE		"��·"		//��·
#define STRING_BREAK	"����"		//����
#define STRING_GEN		"�����"	//�����
#define STRING_CAP		"������"	//������
#define STRING_REACTOR	"��ѹ�翹��"//��ѹ�翹��
#define STRING_TRANS	"��ѹ��"	//��ѹ��
#define STRING_SWITCH	"��բ"		//��բ
#define STRING_PMS      "���������" //���������
#define STRING_BAY		"���"		//���
	
#define STRING_NET		"����"		//����
#define STRING_STATION	"��վ"		//��վ
	
#define	STRING_SEC		"�����豸"	//�����豸
#define STRING_PROTECT	"����װ��"	//����װ��
#define STRING_WAVEREC	"¼����"	//¼���� 
*/
	if(sType == STRING_BAY)
		return TYPE_BAY;
	if(sType == STRING_BREAK)
		return TYPE_BREAK;
	if(sType == STRING_BUS)
		return TYPE_BUS;
	if(sType == STRING_LINE)
		return TYPE_LINE;
	if(sType == STRING_GEN)
		return TYPE_GEN;
	if(sType == STRING_CAP)
		return TYPE_CAP;
	if(sType == STRING_REACTOR)
		return TYPE_REACTOR;
	if(sType == STRING_TRANS)
		return TYPE_TRANS;
	if(sType == STRING_SWITCH)
		return TYPE_SWITCH;
	if(sType == STRING_PMS)
		return TYPE_PMS;
	if(sType == STRING_SEC)
		return TYPE_SEC;
	if(sType == STRING_PROTECT)
		return TYPE_PROTECT;
	if(sType == STRING_WAVEREC)
		return TYPE_WAVEREC;
	return 0;
}

//##ModelId=49B87B7D005D
BOOL CXJBrowserView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����ReleaseTemp()
 ���ܸ�Ҫ�������ʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7C033D
void CXJBrowserView::ReleaseTemp()
{
	//�˳��߳�
	m_bExitRefresh = FALSE;

	if(m_bLoaded)
	{
		//ֹͣ������˸
		pSvgViewer->BlinkShape(-1, FALSE);

		m_sCurrentPath = "";
		m_sStationID = "";
		m_pStation = NULL;
		m_pCurBay = NULL;
		m_szCurSize.cx = 0;
		m_szCurSize.cy = 0;
		m_szOriSize.cx = 0;
		m_szOriSize.cy = 0;
		m_bLoaded = FALSE;
	}
}

/*************************************************************
 �� �� ����CloseCurrentView()
 ���ܸ�Ҫ���رյ�ǰ��ʾ�ĳ�վͼ��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7C033E
BOOL CXJBrowserView::CloseCurrentView()
{
	//�ͷ���ʱ����, �����ļ�
	//�˳��߳�
	m_bExitRefresh = TRUE;
	CString strLog;
	strLog.Format("�رձ���վͼ�Σ�ֹͣ����ͼ����˸, ��ǰվID:%s", m_sStationID);
	WriteLog(strLog, XJ_LOG_LV3);
	KillTimer(99);
	StopBlink(m_arrBlinking);
	if(m_bLoaded)
	{
		ReleaseTemp();
		//	SaveCurrentView();
	}
	return TRUE;
}

/*************************************************************
 �� �� ����OpenStationView()
 ���ܸ�Ҫ����ָ����վ��ͼ��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��վID
**************************************************************/
//##ModelId=49B87B7C034B
BOOL CXJBrowserView::OpenStationView( CString strID )
{
	CloseCurrentView();

	m_bRefreshMark = FALSE;
	m_bRefreshNoteMark = FALSE;

	m_nSvgType = 1;
	//�ҵ���Ӧ��վ, �õ�SVG�ļ�·��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();

	//���ҳ�վ
	CStationObj* pObj = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
	if(pObj == NULL)
		return FALSE;

	if(g_role != MODE_SUB)
		m_wndComm.SetStation(pObj);

	if(g_role != MODE_SUB)
		m_wndHangout.SetStation(pObj);

	//�ļ�·��
	CString strPath;
	strPath = pObj->m_sSvgPath;
	if( strPath.IsEmpty() )
	{
		//Ĭ������վID��Ϊsvg�ļ�����
		strPath = strID+".svg";
	}
	//ȥ��"\\",��Ϊϵͳ����·���Ѿ�����
	while(strPath.Left(1) == "\\")
	{
		strPath = strPath.Right(strPath.GetLength() - 1);
	}

	//��������
	m_pStation = pObj;
	m_pSelectedStation = pObj;
	if (g_role == MODE_SUB)
	{
		m_sCurrentPath = strPath;
	}
	else
	{
		m_sCurrentPath = pApp->GetDownDir() + strPath;
	}
	m_sStationID = strID;


	WriteLog("��SVG�ļ�·��Ϊ:" + m_sCurrentPath, XJ_LOG_LV3);
	/*
#ifdef _DEBUG
//uesd for test
m_sCurrentPath = "C:\\Documents and Settings\\Administrator\\����\\SVG (2)\\Main.svg";
#endif
*/

	if(!OpenView(m_sCurrentPath))
	{
		CString str;
		str.Format("��SVG�ļ�ʧ��,·��Ϊ: %s", m_sCurrentPath);
		WriteLog(str, XJ_LOG_LV1);
		//��ʧ��
		m_pStation = NULL;
		//m_sStationID = "";
		m_sCurrentPath = "";
		return FALSE;
	}

	
	BlinkShapeInStation(strID);
	return TRUE;
}

//##ModelId=49B87B7D0060
void CXJBrowserView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_tooltip.HideToolTip();
	//��ͼ������ʱ������
	if(!m_bLoaded)
		return;

	UINT nMenuID;		//��ʱ�˵�ID
	CDeviceObj * pObj = NULL;
	
	if(m_selected.GetCount() == 0) //û�б�ѡ���豸
	{
		nMenuID = IDR_MENU_MAP_SVG; //����˵�
	}
	else //��ѡ���豸
	{
		if(m_selected.GetCount() > 0)
		{
			pObj = m_selected.GetHead();
			if(pObj != NULL)
			{
				int  nType = pObj ->m_nType;
				
				if(nType == TYPE_PROTECT) //����
				{
					nMenuID = IDR_MENU_PROTECT;	//�����˵�
				}
				else if(nType == TYPE_WAVEREC) //¼����
				{
					nMenuID = IDR_MENU_WAVEREC; //¼�����˵�
				}
				else
				{
					nMenuID = IDR_MENU_MAP_SVG;	//����˵�
				}
			}
			else //�Ҳ�����ѡ�豸
			{
				nMenuID = IDR_MENU_MAP_SVG;
			}
		}
	}

	CMenu mnuPop,*pSubmenu;
	mnuPop.LoadMenu(nMenuID);
    if (g_role != MODE_SUB)
	{
		mnuPop.DeleteMenu(ID_MENUITEM_DevTree_PTRunUpdate, MF_BYCOMMAND);
	}
	mnuPop.DeleteMenu(ID_DevTree_PTCONFIGCHANGED, MF_BYCOMMAND);
	mnuPop.DeleteMenu(ID_DevTree_PTADD, MF_BYCOMMAND);

	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	//CString strTemp;
	//strTemp.Format("UserID: %s, UserGroupID: %s",pApp->m_User.m_strUSER_ID, pApp->m_User.m_strGROUP_ID);
	//AfxMessageBox(strTemp);
	if (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_RUNNER)
		|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)){
	}else{
		mnuPop.DeleteMenu(ID_MARK_DEVICE, MF_BYCOMMAND);
	}

	pSubmenu=mnuPop.GetSubMenu(0);

	//����Menu
	ClientToScreen(&point);
	pSubmenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

	CFormView::OnRButtonDown(nFlags, point);
}

//##ModelId=49B87B7D00AE
void CXJBrowserView::OnPTProp( UINT nID )
{
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;

	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();

		pDoc->ShowSecPropPage(pObj, (nID - ID_PT_GENERAL_NEW));
	}
}

//##ModelId=49B87B7D006E
void CXJBrowserView::OnPtSignallist() 
{
	// TODO: Add your command handler code here
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;
	
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		CDlgSignalList dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
}

//##ModelId=49B87B7D0070
void CXJBrowserView::OnQueryEvent() 
{
	// TODO: Add your command handler code here
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;
	
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		//�齨��ѯ����
		CString sQuery = "";
		sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$0$$$1", pObj->m_sID);
		OpenXJQuery(sQuery, GetSafeHwnd());
	}
}

//##ModelId=49B87B7D007D
void CXJBrowserView::OnMapRefresh() 
{
	// TODO: Add your command handler code here
	//����ˢ��״̬�߳̽���ˢ��
	if(m_bLoaded)
	{
		//�رվ��߳�
		if(m_pStatusRefresh != NULL)
		{
			m_bExitRefresh = TRUE;
			WaitForSingleObject(m_pStatusRefresh->m_hThread, INFINITE);
			delete m_pStatusRefresh;
			m_pStatusRefresh = NULL;
		}
		
		//��������״̬�߳�
		m_bExitRefresh = FALSE;
		m_pStatusRefresh = AfxBeginThread(RefreshStatus,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pStatusRefresh != NULL)
		{
			m_pStatusRefresh->m_bAutoDelete = FALSE;
			m_pStatusRefresh->ResumeThread();
		}
	}
}

//##ModelId=49B87B7D007F
void CXJBrowserView::OnMapStopalart() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//ֹͣ���и澯
	RemoveAllBlinkDevices();
	//ֹ֪ͨͣ��������
	if(pApp->GetMainWnd() != NULL)
	{
		pApp->GetMainWnd()->PostMessage(ALARM_SOUND, 0, 1);
	}
}

/*************************************************************
 �� �� ����OnMapZoomfull()
 ���ܸ�Ҫ�����ͻ�����С��ʾ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7D0081
void CXJBrowserView::OnMapZoomfull() 
{
	// TODO: Add your command handler code here
	if(m_bLoaded)
	{
		CRect rc;
		GetClientRect(&rc);
		int nX = rc.Width() - m_szCurSize.cx;
		int nY = rc.Height() - m_szCurSize.cy;
		//�Ŵ������ͻ�����С
		pSvgViewer->ZoomView(1, CSize(0, 0), CSize(rc.Width(), rc.Height()));
		//pSvgDoc->ZoomView(1, nX, nY);
		m_szCurSize.cx = rc.Width();
		m_szCurSize.cy = rc.Height();

		SetScrollSizes(MM_TEXT, m_szCurSize);
		//ˢ�¿ͻ���
		DocToClient(rc);
		InvalidateRect(rc);
		
	}
}

/*************************************************************
 �� �� ����OnMapZoomin()
 ���ܸ�Ҫ���Ŵ�ͼ��, X+100, y+75
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7D0083
void CXJBrowserView::OnMapZoomin() 
{
	// TODO: Add your command handler code here
	if(m_bLoaded)
	{
		pSvgViewer->ZoomView(0, CSize(-100, -75), m_szCurSize);
		//pSvgDoc->ZoomView(1, 100, 75);
		m_szCurSize.cx += 100;
		m_szCurSize.cy += 75;

		SetScrollSizes(MM_TEXT, m_szCurSize);
		//ˢ�¿ͻ���
		CRect rc(0, 0, m_szCurSize.cx, m_szCurSize.cy);
		CRect rect;
		GetClientRect(&rect);
		if(rect.Width() > rc.Width() || rect.Height() > rc.Height())
		{
			DocToClient(rect);
			InvalidateRect(rect);
		}
		else 
		{
			DocToClient(rc);
			InvalidateRect(rc);
		}
	}
}

/*************************************************************
 �� �� ����OnMapZoomout()
 ���ܸ�Ҫ����Сͼ��, X-100, y-75
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7D008C
void CXJBrowserView::OnMapZoomout() 
{
	// TODO: Add your command handler code here
	if(m_bLoaded)
	{
		pSvgViewer->ZoomView(0, CSize(100, 75), m_szCurSize);
		//pSvgDoc->ZoomView(0, 100, 75);
		m_szCurSize.cx -= 100;
		m_szCurSize.cy -= 75;

		//ˢ�¿ͻ���
		SetScrollSizes(MM_TEXT, m_szCurSize);
		CRect rc(0, 0, m_szCurSize.cx, m_szCurSize.cy);
		CRect rect;
		GetClientRect(&rect);
		if(rect.Width() > rc.Width() || rect.Height() > rc.Height())
		{
			DocToClient(rect);
			InvalidateRect(rect);
		}
		else 
		{
			DocToClient(rc);
			InvalidateRect(rc);
		}
	}
}

/*************************************************************
 �� �� ����OnMapZoomOld()
 ���ܸ�Ҫ��ͼ�λظ�ԭʼ��С
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7D008E
void CXJBrowserView::OnMapZoomOld() 
{
	// TODO: Add your command handler code here
	if(m_bLoaded)
	{
		int nX = m_szOriSize.cx - m_szCurSize.cx;
		int nY = m_szOriSize.cy - m_szCurSize.cy;
		
		//�Ŵ�ԭʼ��С
		pSvgViewer->ZoomView(2, CSize(-nX, -nY), m_szOriSize);
		m_szCurSize = m_szOriSize;

		//ˢ�¿ͻ���
		SetScrollSizes(MM_TEXT, m_szCurSize);
		CRect rc(0, 0, m_szCurSize.cx, m_szCurSize.cy);
		CRect rect;
		GetClientRect(&rect);
		if(rect.Width() > rc.Width() || rect.Height() > rc.Height())
		{
			DocToClient(rect);
			InvalidateRect(rect);
		}
		else 
		{
			DocToClient(rc);
			InvalidateRect(rc);
		}
	}
}

//##ModelId=49B87B7D0090
void CXJBrowserView::OnUpdateMapZoomOld(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bUI = TRUE;
	if(m_szOriSize.cx == m_szCurSize.cx && m_szCurSize.cy == m_szOriSize.cy)
		bUI = FALSE;
	pCmdUI->Enable(bUI);
}

//##ModelId=49B87B7D009C
void CXJBrowserView::OnUpdateMapZoomfull(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CRect rc;
	GetClientRect(&rc);
	BOOL bUI = TRUE;
	if(m_szCurSize.cx == rc.Width() && m_szCurSize.cy == rc.Height())
		bUI = FALSE;
	pCmdUI->Enable(bUI);
}

//##ModelId=49B87B7D009F
void CXJBrowserView::OnUpdateMapZoomin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI ->Enable(TRUE);
}

//##ModelId=49B87B7D00AB
void CXJBrowserView::OnUpdateMapZoomout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bUI = TRUE;
	if(m_szCurSize.cx <= 100 || m_szCurSize.cy <= 75)
		bUI = FALSE;
	pCmdUI ->Enable(bUI);
}

/*************************************************************
 �� �� ����SetDeviceCol()
 ���ܸ�Ҫ�������豸״̬����ͼ����ɫ,���жϹ���״̬,���ж�����״̬, ����״̬Ϊ"����"ʱ�ж�ͨѶ״̬
 �� �� ֵ: void
 ��    ����param1	�����豸ָ��
**************************************************************/
//##ModelId=49B87B7C02DE
void CXJBrowserView::SetDeviceCol( CSecObj* pObj )
{
	//����������
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;

	COLORREF colStatus; //״̬��Ӧ��ɫ
	if (pObj->m_nSuspendstatus == 1)
	{
		colStatus = XJ_STATUS_RUN_MARK;
	}
	else
	{
		//���ж�����״̬
		switch(pObj->m_nRunStatu)
		{
		case 0: //����
			colStatus = XJ_STATUS_RUN_CHECK;
			break;
		case 1:	//ͣ��
			colStatus = XJ_STATUS_RUN_OFF;
			break;
		case 2: //Ͷ��
			if(pObj->m_nCommStatus == 0)
			{
				//���� 
				colStatus = XJ_STATUS_COMM_ON;
			}
			else
			{
				//�Ͽ�
				colStatus = XJ_STATUS_COMM_OFF;
			}
			break;
		case 3: //δ����
			colStatus = XJ_STATUS_RUN_CODE;
			break;
		case 4: //����(�����)
			colStatus = XJ_STATUS_RUN_DEBUG;
			break;
		default: //Ĭ��
			colStatus = XJ_STATUS_RUN_ON;
			break;
		}
	}
	

	//ȡ��ͼ������
	int nIndex = GetShapeIndex(pObj);
	//����
	if(nIndex >= 0)
	{
		pSvgViewer->ChangedFillColor(nIndex, colStatus, this);
	}
}

/*************************************************************
 �� �� ����OnSTTP20002()
 ���ܸ�Ҫ����Ӧ����ͨѶ״̬�仯����֪ͨ(20002), �ı�ͼ����ɫ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B7D00BB
void CXJBrowserView::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	return;
	WriteLog("CXJBrowserView::OnSTTP20002 start", XJ_LOG_LV3);
	//���������ͼ�δ�, ������
	if(!m_bLoaded)
		return;
	//�޶�Ӧ��վ, ������
	if(m_pStation == NULL)
		return;
	//������������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;

	//��鱨��ͷ
	if(pSttpData->sttp_head.uMsgID != 20002)
	{
		CString str;
		str.Format("CXJBrowserView::OnSTTP20002 �յ�������, ���ĺ�Ϊ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();

	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		//�õ��豸ID
		CString strID = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		//���豸ָ��,����Ƿ��ڱ�ͼ����
		CSecObj* pObj = (CSecObj*)pData->FindDevice(strID, TYPE_SEC); //������¼����?
		if(pObj == NULL)
			continue;
		if(pObj->m_sStationID != m_pStation->m_sID)
		{
			//ͬһ�������������Ŀ϶���ͬһ��վ������,�����ͬ,ֱ���˳�
			return;
		}
		//����״̬
		pObj->m_nCommStatus = pSttpData->sttp_body.variant_member.value_data[i].id;
		//������ɫ
		SetDeviceCol(pObj);
	}
	WriteLog("CXJBrowserView::OnSTTP20002 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnSTTP20079()
 ���ܸ�Ҫ����Ӧ��������״̬�仯����֪ͨ(20079), �ı�ͼ����ɫ
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B7D00BF
void CXJBrowserView::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	return;
	WriteLog("CXJBrowserView::OnSTTP20079, start", XJ_LOG_LV3);
	//���������ͼ�δ�, ������
	if(!m_bLoaded)
		return;
	//�޶�Ӧ��վ, ������
	if(m_pStation == NULL)
		return;
	//������������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//��鱨��ͷ
	if(pSttpData->sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CXJBrowserView::OnSTTP20079 �յ�������, ���ĺ�Ϊ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	//�õ��豸ID
	CString strID = pSttpData->sttp_body.ch_pt_id;
	//���豸ָ��,����Ƿ��ڱ�ͼ����
	CSecObj* pObj = (CSecObj*)pData->FindDevice(strID, TYPE_SEC); //������¼����?
	if(pObj == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
	{
		return;
	}
	//����״̬
	pObj->m_nRunStatu = pSttpData->sttp_body.nStatus;
	//������ɫ
	SetDeviceCol(pObj);
	WriteLog("CXJBrowserView::OnSTTP20079 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����AddBlinkDevice()
 ���ܸ�Ҫ��ʹָ�������豸��˸, ������һ���豸һ����˸
 �� �� ֵ: void 
 ��    ����param1	ָ�������豸
		   Param2
**************************************************************/
//##ModelId=49B87B7C02E0
void CXJBrowserView::AddBlinkDevice( CSecObj* pObj )
{
	//����������
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;


	TimerTick* pTick = NULL;
	POSITION oldPos = NULL;
	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos != NULL)
	{
		oldPos = pos;
		TimerTick* pTime = (TimerTick*)m_PtrList.GetNext(pos);
		MYASSERT(pTime);
		if (pTime->pPoint == pObj)
		{
			pTick = pTime;
			break;
		}
	}

	BOOL bBlinking = FALSE;
	if(pTick != NULL)
	{
		bBlinking = TRUE;
		KillTimer(pTick->nId);
		//RemoveAtBlinkDevice(pTime->pPoint);
		delete pTick;
		pTick = NULL;
		m_PtrList.RemoveAt(oldPos);
	}
	
	TimerTick* pTime = new TimerTick;
	pTime->nId = ++m_nCount;
	pTime->pPoint = pObj;
	m_PtrList.AddTail(pTime);
	SetTimer(pTime->nId, g_FlashTime*1000, 0);

	//��ֹ֮ͣǰ����˸
	//RemoveAllBlinkDevices();
	
	if(!bBlinking)
	{
		int nBlinkTime = 800;
		//ȡ�ö����豸ͼ������
		int nIndex = GetShapeIndex(pObj);
		if(nIndex >= 0)
		{
			//��˸
			pSvgViewer->BlinkShape(nIndex, TRUE, nBlinkTime);
		}

		//������һ���豸;
		if(pObj->m_pOwner != NULL && g_BlinkOwner == 1)
		{
			nIndex = GetShapeIndex(pObj->m_pOwner);
			if(nIndex >= 0)
			{
				//��˸
				pSvgViewer->BlinkShape(nIndex, TRUE, nBlinkTime);
			}
		}
	}

}

/*************************************************************
 �� �� ����RemoveAtBlinkDevice()
 ���ܸ�Ҫ��ʹָ�������豸������˸, ��֮������һ���豸Ҳֹͣ��˸
 �� �� ֵ: void
 ��    ����param1	ָ�������豸
**************************************************************/
//##ModelId=49B87B7C02E2
void CXJBrowserView::RemoveAtBlinkDevice( CDeviceObj* pObj )
{
	//����������
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	
	//�õ�����
	int nIndex = GetShapeIndex(pObj);
	if(nIndex >= 0)
		pSvgViewer->BlinkShape(nIndex, FALSE);
	
	
	//�õ�����һ���豸��103ID;
	CSecObj* pSec = (CSecObj*)pObj;
	if(pSec->m_pOwner != NULL && g_BlinkOwner == 1)
	{
		nIndex = GetShapeIndex(pSec->m_pOwner);
		if(nIndex >= 0)
			pSvgViewer->BlinkShape(nIndex, FALSE);
	}
	
}

/*************************************************************
 �� �� ����RemoveAllBlinkDevices()
 ���ܸ�Ҫ��ֹͣ��ǰ���������豸��˸
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7C02EE
void CXJBrowserView::RemoveAllBlinkDevices()
{
	if(m_bLoaded && pSvgViewer != NULL)
	{
		pSvgViewer->BlinkShape(-1, FALSE);
	}
}

/*************************************************************
 �� �� ����CenterDevice()
 ���ܸ�Ҫ��ʹָ���豸ͼ�ξ�����ʾ
 �� �� ֵ: void
 ��    ����param1	ָ���豸
**************************************************************/
//##ModelId=49B87B7C02EF
void CXJBrowserView::CenterDevice( CDeviceObj* pObj )
{
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	
	//�ж��豸��ͼ���Ƿ�����ͼ��
	int nIndex = GetShapeIndex(pObj);
	if(nIndex < 0)//ͼ���Ҳ������豸
		return;
	
	CRect rect;
	rect = pSvgViewer ->GetShapeSize(nIndex);
	//���ͼ�η�Χ������
 	if(rect.left == 32766 ||rect.top == 32766 || rect.right == -32766 || rect.bottom == -32766)
	{
		return;
	}
	//ת�����豸����
	DocToClient(rect);
	//�õ�ͼ���е�����, ����������
	CPoint pt;
	pt.x = rect.left + rect.Width()/2;
	pt.y = rect.top + rect.Height()/2;
	CScrollView::CenterOnPoint(pt);
	
	//���õ�ǰ�豸Ϊѡ��
	pSvgViewer ->SetSelected(nIndex, TRUE, this);

	//ˢ��
	Invalidate();
}

/*************************************************************
 �� �� ����OnAllClose()
 ���ܸ�Ҫ����Ӧ�ر���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7D00CC
void CXJBrowserView::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//�ر��߳�
	EndOwnerThread();
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳������Լ���ĳ����߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7C034D
void CXJBrowserView::EndOwnerThread()
{
	if(m_pStatusRefresh != NULL)
	{
		m_bExitRefresh = TRUE;
		WaitForSingleObject(m_pStatusRefresh->m_hThread, INFINITE);
		delete m_pStatusRefresh;
		m_pStatusRefresh = NULL;
	}

	if(m_pBlinkThread != NULL)
	{
		m_bExitBlink = TRUE;
		WaitForSingleObject(m_pBlinkThread->m_hThread, INFINITE);
		delete m_pBlinkThread;
		m_pBlinkThread = NULL;
	}
}

/*************************************************************
 �� �� ����RefreshDeviceStatus()
 ���ܸ�Ҫ��ˢ���豸״̬
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7C034E
void CXJBrowserView::RefreshDeviceStatus()
{
	//���������Ч��
	if(m_pStation == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, m_pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//������ɫ
		SetDeviceCol(pObj);
	}
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CXJBrowserView::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshDeviceStatus();
	WriteLog("CXJBrowserView::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ��: OpenBayView()
 ���ܸ�Ҫ: ��ָ�������ͼ��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����
		   Param2 
**************************************************************/
BOOL CXJBrowserView::OpenBayView( CBayObj* pObj )
{
	//�ȹرյ�ǰͼ��
	CloseCurrentView();

	m_nSvgType = 2;
	if(pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	
	//���ҳ�վ
	CStationObj* pStation = (CStationObj*)pData->FindDevice(pObj->m_sStationID, TYPE_STATION);
	
	//�ļ�·��
	CString strPath;
	strPath = pObj->m_sSvgPath;
	//ȥ��"\\",��Ϊϵͳ����·���Ѿ�����
	while(strPath.Left(1) == "\\")
	{
		strPath = strPath.Right(strPath.GetLength() - 1);
	}
	//��������
	m_pCurBay = pObj;
	m_sCurrentPath = pApp->GetDownDir() + strPath;
	m_pStation = pStation;
	m_sStationID = pObj->m_sStationID;

	WriteLog("��SVG�ļ�·��Ϊ:" + m_sCurrentPath, XJ_LOG_LV3);
	//��ͼ��
	if(!OpenView(m_sCurrentPath))
	{
		CString str;
		str.Format("��SVG�ļ�ʧ��,·��Ϊ: %s", m_sCurrentPath);
		WriteLog(str, XJ_LOG_LV1);
		//��ʧ��
		m_sCurrentPath = "";
		m_pCurBay = NULL;
		m_pStation = NULL;
		m_sStationID = "";
		return FALSE;
	}
	return TRUE;
}

void CXJBrowserView::OnWRProp( UINT nID )
{
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;
	
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//��¼����
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		pDoc->ShowSecPropPage(pObj, (nID - ID_WR_GENERAL_NEW));
	}
}

//��������״̬����
void CXJBrowserView::OnMENUITEMDevTreePTRunUpdate() 
{
	// TODO: Add your command handler code here
	
	try
	{
		CString strTemp;
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		CString m_strNodeName = pDevice->m_sName;
		//1 get pt name from node name
		if (m_strNodeName.IsEmpty())
			return;
	
		bool bGet = FALSE;
        CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetStationList(listDevice);
		if (listDevice.GetCount() > 1)
		{
			AfxMessageBox(StringFromID(IDS_TIP_NOT_SUBSTATION_MODE));
			return;
		}
		POSITION stationPos = listDevice.GetHeadPosition();
		CStationObj* pStationObj = (CStationObj*)listDevice.GetAt(stationPos);
		if(pStationObj == NULL)
			return;

		CSecObj* pSecObj = NULL;

		pData->GetSecList(listDevice, "", TYPE_SEC, pStationObj->m_sID);
		POSITION pos = listDevice.GetHeadPosition();
		while (pos != NULL)
		{	
			CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
			CString str;
			str.Format("%s", pObj->m_sName);
			if (!m_strNodeName.CompareNoCase(str))
			{
				bGet = true;
				pSecObj = pObj;
				break;
			}
		}

		if(!bGet)
		{
			AfxMessageBox(StringFromID(IDS_TIP_DEVICE_NOTFOUND));
			return;
		}
	
		//2 dlg
		CDlg_DevRunStatusUpdate dlg;
		dlg.m_cstrPTID = pSecObj->m_sID;
		dlg.m_cstrPTName = pSecObj->m_sName;
		dlg.DoModal();
		pSecObj->m_nRunStatu = dlg.m_nStatus;
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_TIP_EXCEPTION));
	}
}



void CXJBrowserView::GetDeviceType(CString strUri, CString& strDeviceType)
 {
	 int iIndex = strUri.ReverseFind('.');
	 CString strCode = strUri.Right(strUri.GetLength() - iIndex - 1);
	 if ((strCode.GetAt(4) - 48) == 2)
	 {
		 strDeviceType = TYPE_SEC;
	 }
	 else
	 {
		 int iDeviceType = (strCode.GetAt(4) - 48) * 100 + (strCode.GetAt(5) - 48) * 10 + (strCode.GetAt(6) - 48);
		 switch(iDeviceType)
		 {
		 case 101:
			 strDeviceType = TYPE_BUS;
			 break;
		 case 102:
			 strDeviceType = TYPE_TRANS;
			 break;
		 case 103:
			 strDeviceType = TYPE_LINE;
			 break;
		 case 104:
			 strDeviceType = TYPE_GEN;
			 break;
		 case 105:
			 strDeviceType = TYPE_SWITCH;
			 break;
		 case 106:
			 strDeviceType = TYPE_TRANS;
			 break;
		 case 107:
			 strDeviceType = TYPE_TRANS;
			 break;
		 case 108:
			 strDeviceType = TYPE_CAP;
			 break;
		 case 109:
			 strDeviceType = TYPE_WAVEREC;
			 break;
		 }
	 }
}

/*************************************************************
 CreateDate: 20:5:2010     Author:LYH
 �� �� ��: GetShapeIndex()
 ���ܸ�Ҫ: ȡ���豸��ͼ������
 �� �� ֵ: ͼ������
 ��    ��: param1 ָ���豸
		   Param2 
**************************************************************/
int CXJBrowserView::GetShapeIndex( CDeviceObj* pObj )
{
	//����������
	if(pObj == NULL)
		return -1;
	if(!m_bLoaded)
		return -1;
	
	//�õ�103ID
	CString str103ID;
	str103ID.Format("%d#%d", pObj->m_n103Group, pObj->m_n103Item);
	int nDevIndex = -1;
	//103�����Ŀ�Ź���
	int nIndex = pSvgViewer->GetIndexByDevID(str103ID);
	if(nIndex >= 0)
	{
		nDevIndex = nIndex;
		return nDevIndex;
	}
	//����ID����
	nIndex = pSvgViewer->GetIndexByDevID(pObj->m_sID);
	if(nIndex >= 0)
	{
		nDevIndex = nIndex;
		return nDevIndex;
	}
	return nDevIndex;
}

/*************************************************************
 CreateDate: 20:5:2010     Author:LYH
 �� �� ��: FindDevicePlus()
 ���ܸ�Ҫ: ȡ��ָ���豸������������� 
 �� �� ֵ: �豸����
 ��    ��: param1 ָ���豸ID
		   Param2 ָ���豸����
**************************************************************/
CDeviceObj* CXJBrowserView::FindDevicePlus( CString sID, CString sType )
{
	CDeviceObj * pObj = NULL;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	
	int nType = TypeStringToInt(sType);
	//�ж�ID
	if(sID.Find('#', 0) >= 0)
	{
		//103�����Ŀ��
		pObj = pData->FindDeviceBy103(sID, nType, m_sStationID);
	}
	else if(sID.Find('.', 0) >= 0)//uri
	{
		//uri
		pObj = pData->FindDeviceByUri(sID, m_sStationID);
	}
	else
	{
		CString strlog;
		strlog.Format("Find device, device_id:%s, type:%s(%d) station_id:%s", sID, sType, nType, m_sStationID);
		WriteLog(strlog);
		//PTID
		pObj = pData->FindDevice(sID, nType, m_sStationID);
	}
	return pObj;
}


void CXJBrowserView::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	MYASSERT(pParam);

}


void CXJBrowserView::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	CXJEventAction* pParam = (CXJEventAction*)lParam;
	MYASSERT(pParam);
}

/*
void CXJBrowserView::BlinkShape(CString strStationID, BOOL bBlink, int nTime) 
{
	// TODO: Add your message handler code here and/or call default	
	int nEleIndex;

	if(pSvgViewer != NULL)
	{
			pSvgViewer->BlinkShape(nEleIndex, bBlink, nTime);
	}
}
*/

void CXJBrowserView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 99)
	{
		
		KillTimer(99);
		//ֹͣ��˸
		StopBlink(m_arrBlinking);
		
		POSITION pos = NULL;
		CSecObj* pObj = NULL;
		
		try
		{
			EnterCriticalSection(&m_CriticalSection);
			pos = listBlinkDevice.GetHeadPosition();
			if(pos != NULL)
				pObj = (CSecObj*)listBlinkDevice.GetAt(pos);
			LeaveCriticalSection(&m_CriticalSection);
		}
		catch(...)
		{
			LeaveCriticalSection(&m_CriticalSection);
		}
		if(pos != NULL && pObj != NULL)
		{
			if(pObj->m_sStationID == m_sStationID)
				BlinkShapeInStation(pObj->m_sStationID);
			else//�л���վ
				BlinkShapeInOtherStation(pObj);
		}
		else
			SetTimer(99, g_FlashTime*500, 0);//��ǰû����˸
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CXJBrowserView::AddToBlinkList( CSecObj* pObj )
{
	if(pObj == NULL)
		return;

	CString strLog;
	strLog.Format("�յ���˸%s(%s)֪ͨ,���뵽����˸�б�", pObj->m_sName, pObj->m_sID);
	WriteLog(strLog, XJ_LOG_LV3);
	try
	{
		EnterCriticalSection(&m_CriticalSection);
		listBlinkDevice.AddTail(pObj);
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...)
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
	
	if(m_arrBlinking.GetSize() == 0)
	{
		if(m_sStationID != pObj->m_sStationID)
		{
			//��ǰվ����Ҫ�򿪵�վ,����վ
			BlinkShapeInOtherStation(pObj);
		}
		else
		{
			//��˸
			BlinkShapeInStation(pObj->m_sStationID);
		}
	}
	strLog.Format("�յ���˸%s(%s)֪ͨ,��˸�������", pObj->m_sName, pObj->m_sID);
	WriteLog(strLog, XJ_LOG_LV3);
}

void CXJBrowserView::OnMarkDevice() 
{
	// TODO: Add your command handler code here
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;
	
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		DLGMarked dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
}

void CXJBrowserView::OnMarkPtdevice() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//�ж��Ƿ���ѡ��
	if(m_selected.GetCount() < 1)
		return;
	
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		DLGMarked dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
}

/****************************************************
Date:2012/12/5  Author:LYH
������:   BlinkShapeInStation	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sStationID	
*****************************************************/
void CXJBrowserView::BlinkShapeInStation( CString sStationID )
{
	try
	{
		if(sStationID.IsEmpty())
			return;
		if(listBlinkDevice.GetCount() <= 0)
			return;
		EnterCriticalSection(&m_CriticalSection);
		CString strLog;
		strLog.Format("��ʼ��վ���д���˸ͼ����˸, ��ǰվID:%s", m_sStationID);
		WriteLog(strLog, XJ_LOG_LV3);
		m_arrBlinking.RemoveAll();
		POSITION pos = listBlinkDevice.GetHeadPosition();
		POSITION posTail = listBlinkDevice.GetTailPosition();
		while(pos != NULL)
		{
			POSITION posOld = pos;
			CDeviceObj* pObj = (CDeviceObj*)listBlinkDevice.GetNext(pos);
			if(pObj == NULL)
			{
				if(posOld == posTail)
					break;
				else
					continue;
			}
			if(pObj->m_sStationID == sStationID)
			{
				BlinkSec((CSecObj*)pObj, m_arrBlinking);
				listBlinkDevice.RemoveAt(posOld);
			}
			if(posOld == posTail)
				break;
		}
		strLog.Format("��վ���д���˸ͼ����˸, ��ǰվID:%s, ������˸%d���豸", m_sStationID, m_arrBlinking.GetSize());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...)
	{
		LeaveCriticalSection(&m_CriticalSection);
	}

	KillTimer(99);

	if(m_arrBlinking.GetSize() > 0)
	{
		//������ʱ��
		SetTimer(99, g_FlashTime*1000, 0);
	}
	else
	{
		SetTimer(99, g_FlashTime*500, 0);//��ǰû����˸
	}
}

/****************************************************
Date:2012/12/5  Author:LYH
������:   BlinkSec	
����ֵ:   int	
���ܸ�Ҫ: 
����: CSecObj * pObj	
*****************************************************/
int CXJBrowserView::BlinkSec( CSecObj* pObj, CUIntArray& arrBlink )
{
	if(pObj == NULL)
		return -1;
	if(!m_bLoaded)
		return -1;
	if(pSvgViewer == NULL)
		return -1;
	CString strLog;
	strLog.Format("��ʼ��˸ͼ��, %s(%s)", pObj->m_sName, pObj->m_sID);
	WriteLog(strLog, XJ_LOG_LV3);
	//ȡ�ö����豸ͼ������
	int nIndex = GetShapeIndex(pObj);
	if(nIndex >= 0 && !IsInBlinkArray(nIndex))
	{
		CenterDevice(pObj);
		//��˸
		pSvgViewer->BlinkShape(nIndex, TRUE, m_nBlinkTime);
		arrBlink.Add(nIndex);
	}
	
	//������һ���豸;
	if(pObj->m_pOwner != NULL && g_BlinkOwner == 1)
	{
		nIndex = GetShapeIndex(pObj->m_pOwner);
		if(nIndex >= 0 && !IsInBlinkArray(nIndex))
		{
			//��˸
			pSvgViewer->BlinkShape(nIndex, TRUE, m_nBlinkTime);
			arrBlink.Add(nIndex);
		}
	}
	return nIndex;
}

/****************************************************
Date:2012/12/5  Author:LYH
������:   StopBlink	
����ֵ:   void	
���ܸ�Ҫ: 
����: CUIntArray arrBlink	
*****************************************************/
void CXJBrowserView::StopBlink( CUIntArray& arrBlink )
{
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	CString strLog;
	strLog.Format("ֹͣ����ͼ����˸, ��ǰվID:%s", m_sStationID);
	WriteLog(strLog, XJ_LOG_LV3);
	for(int i = 0; i < arrBlink.GetSize(); i++)
	{
		//�õ�����
		int nIndex = arrBlink.GetAt(i);
		if(nIndex >= 0)
			pSvgViewer->BlinkShape(nIndex, FALSE);
	}
	arrBlink.RemoveAll();
	strLog.Format("�ɹ�ֹͣ����ͼ����˸, ��ǰվID:%s", m_sStationID);
	WriteLog(strLog, XJ_LOG_LV3);
}

/****************************************************
Date:2012/12/6  Author:LYH
������:   IsInBlinkArray	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: int nIndex	
*****************************************************/
BOOL CXJBrowserView::IsInBlinkArray( int nIndex )
{
	BOOL bReturn = FALSE;
	for(int i = 0; i < m_arrBlinking.GetSize(); i++)
	{
		//�õ�����
		int nTemp = m_arrBlinking.GetAt(i);
		if(nTemp == nIndex )
		{
			bReturn = TRUE;
			break;
		}
	}
	return bReturn;
}

/****************************************************
Date:2013/6/5  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CXJBrowserView::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	if(pStation->m_sID == m_sStationID)
	{
		OpenStationView("");
		m_sStationID = "";
		if(g_role != MODE_SUB){
			m_wndComm.SetStation(NULL);
			m_wndHangout.SetStation(NULL);
		}
		Invalidate();
	}
}

void CXJBrowserView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RePositionCommWnd();
	
}

/****************************************************
Date:2013/7/19  Author:LYH
������:   	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CXJBrowserView::RePositionCommWnd()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CPoint pt(rcClient.right - m_szCommWnd.cx-10, rcClient.top+10);
	if(g_role != MODE_SUB)
		m_wndComm.Show(pt);
	
	CRect rcTmp;
	m_wndComm.GetWindowRect(&rcTmp);
	CPoint pt2(rcClient.right - m_szCommWnd.cx-10, rcClient.top+10 + rcTmp.Height() + 6);
	if(g_role != MODE_SUB)
		m_wndHangout.Show(pt2);

	CWnd* pBtn = (CWnd*)GetDlgItem(IDC_BTN_DRAFT);
	if( pBtn )
	{
		CRect rcBtn;
		pBtn->GetClientRect( &rcBtn );
		pBtn->MoveWindow( rcClient.right - rcBtn.Width() - 10, rcClient.bottom - rcBtn.Height() - 10, rcBtn.Width(), rcBtn.Height(), TRUE );
	}
}

void CXJBrowserView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	RePositionCommWnd();
}

BOOL CXJBrowserView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL bReturn = CFormView::OnMouseWheel(nFlags, zDelta, pt);
	RePositionCommWnd();
	return bReturn;
}

void CXJBrowserView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	RePositionCommWnd();
}

void CXJBrowserView::OnStationChanged( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	CStationObj* pStation = (CStationObj*)lParam;
	if(pStation->m_sID == m_sStationID)
	{
		if(g_role != MODE_SUB){
			m_wndComm.SetStation(pStation);
			m_wndHangout.SetStation(pStation);
		}
	}
}

void CXJBrowserView::OnShowTooltip(WPARAM wParam, LPARAM lParam)
{
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice->m_nType == TYPE_PROTECT || pDevice->m_nType == TYPE_WAVEREC || pDevice->m_nType == TYPE_SEC)
	{
		//������¼����
		CSecObj* pObj = (CSecObj*)pDevice;
		if( pObj->m_nNoteCount > 0 )
		{
			CPoint ptWindow;
			GetCursorPos(&ptWindow);

			CPtrArray arr;
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			pData->QueryPTNotes(pObj->m_sID, arr);


			CStringArray arrText;
			for( int i = 0; i < arr.GetSize(); i++ )
			{
				PT_NOTE* note = (PT_NOTE* )arr.GetAt(i);
				CString str;
				str.Format("%s : %s", note->note_name, note->note_content );
				arrText.Add(str);
				delete note;
			}
			arr.RemoveAll();
			m_tooltip.ShowToolTip(ptWindow, arrText);
		}
	}
}

void CXJBrowserView::BlinkShapeInOtherStation( CSecObj* pSec )
{
	if(!pSec)
		return;
	if(!g_AutoChangeMap)
	{
		//���Զ��л�ͼ��,ɾ������˸�б�����ͬվ��ͼ��
		EnterCriticalSection(&m_CriticalSection);
		CString strLog;
		strLog.Format("���Զ��л�ͼ��,�Ƴ�[%s]�����д���˸ͼ��", m_sStationID);
		WriteLog(strLog, XJ_LOG_LV3);
		m_arrBlinking.RemoveAll();
		POSITION pos = listBlinkDevice.GetHeadPosition();
		POSITION posTail = listBlinkDevice.GetTailPosition();
		while(pos != NULL)
		{
			POSITION posOld = pos;
			CDeviceObj* pObj = (CDeviceObj*)listBlinkDevice.GetNext(pos);
			if(pObj == NULL)
			{
				if(posOld == posTail)
					break;
				else
					continue;
			}
			if(pObj->m_sStationID == pSec->m_sStationID)
			{
				listBlinkDevice.RemoveAt(posOld);
			}
			if(posOld == posTail)
				break;
		}
		strLog.Format("[%s]���д���˸ͼ���Ƴ�, �����Ƴ�%d���豸", m_sStationID, m_arrBlinking.GetSize());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
		OpenStationView(pSec->m_sStationID);
}

void CXJBrowserView::OnBtnDraft() 
{
	// TODO: Add your control notification handler code here
	if( m_pSelectedStation )
	{
		CDlgDraft dlg;
		if(dlg.Init(m_pSelectedStation->m_sID, m_pSelectedStation->m_sName))
			dlg.DoModal();
		else
		{
			AfxMessageBox(StringFromID(IDS_TIP_DRAFT_NOTFOUND));
		}	
	}
}

void CXJBrowserView::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	
}

void CXJBrowserView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_tooltip.HideToolTip();
}

void CXJBrowserView::SetMark(CSecObj* pObj)
{
	if(pObj->m_nCanCallHistory)
	{
		//ȡ��ͼ������
		int nIndex = GetShapeIndex(pObj);
		//����
		if(nIndex >= 0)
		{
			CString sPath = g_LocalPath + "check1.png";
			pSvgViewer->AddMark(nIndex, sPath,16, this);
		}
	}
}

void CXJBrowserView::RefreshDeviceMark()
{
	//���������Ч��
	if(m_pStation == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, m_pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//������ɫ
		SetMark(pObj);
	}
}

void CXJBrowserView::RefreshNoteMark()
{
	//���������Ч��
	if(m_pStation == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, m_pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//������ɫ
		SetNoteMark(pObj);
	}
}

void CXJBrowserView::SetNoteMark(CSecObj* pObj)
{
	if(pObj->m_nNoteCount > 0 )
	{
		//ȡ��ͼ������
		int nIndex = GetShapeIndex(pObj);
		//����
		if(nIndex >= 0)
		{
			CString sPath = g_LocalPath + "warning1.png";
			pSvgViewer->AddMark(nIndex, sPath,16, this);
		}
	}
}
