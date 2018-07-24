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
 函 数 名：RefreshStatus()
 功能概要：刷新保护和录波器属性, 此线程在打开厂站图形时启动, 刷新完后自行退出.
 返 回 值: UINT
 参    数：param1	接线图类指针
**************************************************************/
UINT RefreshStatus(LPVOID pParam)
{
//	WriteLog("刷新图形状态线程启动!");
	TRACE("刷新图形状态线程启动!\n");
	//得到类对象
	CXJBrowserView* pView = (CXJBrowserView*)pParam;
	//检查有效性
	if(pView == NULL)
	{
		//无效指针
		return 0;
	}
	if(pView->m_pStation == NULL)
	{
		//无厂站数据
		return 0;
	}
	if(!pView->m_bLoaded)
	{
		//无图形
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
			//线程退出标志
			return 0;
		}
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//设置颜色
		pView->SetDeviceCol(pObj);
	}
	
	//完成工作,正常退出
//	WriteLog("刷新图形状态线程完成工作, 正常退出线程!");
	TRACE("刷新图形状态线程完成工作, 正常退出线程!\n");
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
		Sleep(pView->m_nBlinkTime); //睡
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
		//不显示施工图
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
	//GetParentFrame()->SetWindowText("主接线图");
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

	if(m_bLoaded)
	{
		CRect rect;
		GetClientRect(&rect);
		ClientToDoc(rect);
		pDrawDC ->FillSolidRect(rect, RGB(18, 69, 60));
		
		//绘制SVG图形
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
	
	//释放资源
	bitmap.DeleteObject();
  	dc.DeleteDC();
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化COM控件
 返 回 值: void
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
	//取得显示功能接口
	pUnk ->QueryInterface(IID_ISvgViewer, (void **)&pSvgViewer);*/
	m_bInitCOM = InitSVGComNoReg(pClf, pSvgViewer);
	
	if(pSvgViewer == NULL)
	{
		TRACE("unable to load SvgViewer Interface\n");
		return;
	}
	//开启闪烁功能
	pSvgViewer->CartoonControl(TRUE, this);

	//m_bInitCOM = TRUE;
}

/*************************************************************
 函 数 名：OpenView()
 功能概要：打开指定的SVG文件
 返 回 值: 打开成功返回TRUE, 失败返回FALSE
 参    数：param1	文件路径
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
	if(nResult <0)//载入失败
	{
		CString str;
		str.Format("厂站接线图载入图形失败，原因为%s", sError);
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

		WriteLog("一次接线图, open the svg sucess", XJ_LOG_LV3);
		m_bLoaded = TRUE;
		if(szSize.cx < 0)
			szSize.cx = 100;
		if(szSize.cy <= 0)
			szSize.cy = 100;
		SetScrollSizes(MM_TEXT, szSize);
		m_szOriSize = szSize;
		m_szCurSize = m_szOriSize;

		//刷新
		CRect rc(0, 0, szSize.cx, szSize.cy);
		DocToClient(rc);
		InvalidateRect(rc);

		//刷新所有
		Invalidate();

		//刷新设备状态
		RefreshDeviceStatus();

	}
	return TRUE;
}

//##ModelId=49B87B7D001F
void CXJBrowserView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_tooltip.HideToolTip();
	//无图形载入时不操作
	if(!m_bLoaded)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();

	//如果有选择多个,只显示第一个的属性
	if(m_selected.GetCount() > 0)
	{
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		if(pDevice->m_nType == TYPE_PROTECT || pDevice->m_nType == TYPE_WAVEREC || pDevice->m_nType == TYPE_SEC)
		{
			//保护或录波器
			CSecObj* pObj = (CSecObj*)pDevice;
			pDoc->ShowSecPropPage(pObj);
		}
	}
	
	CFormView::OnLButtonDblClk(nFlags, point);
}

/*************************************************************
 函 数 名：DocToClient()
 功能概要：逻辑坐标转设备坐标
 返 回 值: void
 参    数：param1	要转换的坐标
**************************************************************/
//##ModelId=49B87B7C02FD
void CXJBrowserView::DocToClient( CPoint & pt )
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
//##ModelId=49B87B7C02FF
void CXJBrowserView::DocToClient( CRect & rect )
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
//##ModelId=49B87B7C030E
void CXJBrowserView::ClientToDoc( CPoint & pt )
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
		
		//填充选择集		
		CUIntArray arrSelected;
		arrSelected.RemoveAll();
		pSvgViewer->GetCurSelected(arrSelected);

		//删除旧的选择
		m_selected.RemoveAll();
		
		for(int i = 0; i < arrSelected.GetSize(); i++)
		{
			CDeviceObj * pObj = NULL;
            CString str103ID;
			CString strUri;
			CString strType;
			
			//查找对应设备
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
				str.Format("SVG图形中,ID为%s,type为%s的图形未找到对应装置", str103ID, strType);
				WriteLog(str, XJ_LOG_LV2);
			}
		}

		//刷新设备状态
		//RefreshDeviceStatus();
	}

	if( 1 == m_selected.GetCount() )
	{
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		if(pDevice->m_nType == TYPE_PROTECT || pDevice->m_nType == TYPE_WAVEREC || pDevice->m_nType == TYPE_SEC)
		{
			//保护或录波器
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
 函 数 名：StringToID()
 功能概要：把COM控件返回的选择设备字符串转化为各设备ID
 返 回 值: void
 参    数：param1	选择设备字符串
		   Param2	设备ID数组
**************************************************************/
//##ModelId=49B87B7C032C
void CXJBrowserView::StringToID( CString strID, CStringArray & arrID )
{
	arrID.RemoveAll();
	
	if(strID.Find(',', 0) == -1) //只有一项
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
 函 数 名：TypeStringToInt()
 功能概要：字符型的设备类型转化为整形的设备类型
 返 回 值: 整形的设备类型
 参    数：param1	字符型的设备类型
**************************************************************/
//##ModelId=49B87B7C032F
int CXJBrowserView::TypeStringToInt( CString sType )
{
	/*
#define STRING_BUS		"母线"		//母线
#define STRING_LINE		"线路"		//线路
#define STRING_BREAK	"开关"		//开关
#define STRING_GEN		"发电机"	//发电机
#define STRING_CAP		"电容器"	//电容器
#define STRING_REACTOR	"高压电抗器"//高压电抗器
#define STRING_TRANS	"变压器"	//变压器
#define STRING_SWITCH	"刀闸"		//刀闸
#define STRING_PMS      "保护管理机" //保护管理机
#define STRING_BAY		"间隔"		//间隔
	
#define STRING_NET		"地区"		//地区
#define STRING_STATION	"厂站"		//厂站
	
#define	STRING_SEC		"二次设备"	//二次设备
#define STRING_PROTECT	"保护装置"	//保护装置
#define STRING_WAVEREC	"录波器"	//录波器 
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
 函 数 名：ReleaseTemp()
 功能概要：清除零时数据
 返 回 值: void
**************************************************************/
//##ModelId=49B87B7C033D
void CXJBrowserView::ReleaseTemp()
{
	//退出线程
	m_bExitRefresh = FALSE;

	if(m_bLoaded)
	{
		//停止所有闪烁
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
 函 数 名：CloseCurrentView()
 功能概要：关闭当前显示的厂站图形
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7C033E
BOOL CXJBrowserView::CloseCurrentView()
{
	//释放临时数据, 保存文件
	//退出线程
	m_bExitRefresh = TRUE;
	CString strLog;
	strLog.Format("关闭本厂站图形，停止所有图形闪烁, 当前站ID:%s", m_sStationID);
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
 函 数 名：OpenStationView()
 功能概要：打开指定厂站的图形
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	厂站ID
**************************************************************/
//##ModelId=49B87B7C034B
BOOL CXJBrowserView::OpenStationView( CString strID )
{
	CloseCurrentView();

	m_bRefreshMark = FALSE;
	m_bRefreshNoteMark = FALSE;

	m_nSvgType = 1;
	//找到对应厂站, 得到SVG文件路径
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();

	//查找厂站
	CStationObj* pObj = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
	if(pObj == NULL)
		return FALSE;

	if(g_role != MODE_SUB)
		m_wndComm.SetStation(pObj);

	if(g_role != MODE_SUB)
		m_wndHangout.SetStation(pObj);

	//文件路径
	CString strPath;
	strPath = pObj->m_sSvgPath;
	if( strPath.IsEmpty() )
	{
		//默认以子站ID作为svg文件名称
		strPath = strID+".svg";
	}
	//去除"\\",因为系统下载路径已经带了
	while(strPath.Left(1) == "\\")
	{
		strPath = strPath.Right(strPath.GetLength() - 1);
	}

	//保存数据
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


	WriteLog("打开SVG文件路径为:" + m_sCurrentPath, XJ_LOG_LV3);
	/*
#ifdef _DEBUG
//uesd for test
m_sCurrentPath = "C:\\Documents and Settings\\Administrator\\桌面\\SVG (2)\\Main.svg";
#endif
*/

	if(!OpenView(m_sCurrentPath))
	{
		CString str;
		str.Format("打开SVG文件失败,路径为: %s", m_sCurrentPath);
		WriteLog(str, XJ_LOG_LV1);
		//打开失败
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
	//无图形载入时不操作
	if(!m_bLoaded)
		return;

	UINT nMenuID;		//临时菜单ID
	CDeviceObj * pObj = NULL;
	
	if(m_selected.GetCount() == 0) //没有被选中设备
	{
		nMenuID = IDR_MENU_MAP_SVG; //画面菜单
	}
	else //有选中设备
	{
		if(m_selected.GetCount() > 0)
		{
			pObj = m_selected.GetHead();
			if(pObj != NULL)
			{
				int  nType = pObj ->m_nType;
				
				if(nType == TYPE_PROTECT) //保护
				{
					nMenuID = IDR_MENU_PROTECT;	//保护菜单
				}
				else if(nType == TYPE_WAVEREC) //录波器
				{
					nMenuID = IDR_MENU_WAVEREC; //录波器菜单
				}
				else
				{
					nMenuID = IDR_MENU_MAP_SVG;	//画面菜单
				}
			}
			else //找不出被选设备
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

	//弹出Menu
	ClientToScreen(&point);
	pSubmenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

	CFormView::OnRButtonDown(nFlags, point);
}

//##ModelId=49B87B7D00AE
void CXJBrowserView::OnPTProp( UINT nID )
{
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;

	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
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
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;
	
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
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
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;
	
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		//组建查询参数
		CString sQuery = "";
		sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$0$$$1", pObj->m_sID);
		OpenXJQuery(sQuery, GetSafeHwnd());
	}
}

//##ModelId=49B87B7D007D
void CXJBrowserView::OnMapRefresh() 
{
	// TODO: Add your command handler code here
	//启动刷新状态线程进行刷新
	if(m_bLoaded)
	{
		//关闭旧线程
		if(m_pStatusRefresh != NULL)
		{
			m_bExitRefresh = TRUE;
			WaitForSingleObject(m_pStatusRefresh->m_hThread, INFINITE);
			delete m_pStatusRefresh;
			m_pStatusRefresh = NULL;
		}
		
		//启动设置状态线程
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

	//停止所有告警
	RemoveAllBlinkDevices();
	//通知停止声音报警
	if(pApp->GetMainWnd() != NULL)
	{
		pApp->GetMainWnd()->PostMessage(ALARM_SOUND, 0, 1);
	}
}

/*************************************************************
 函 数 名：OnMapZoomfull()
 功能概要：按客户区大小显示
 返 回 值: void
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
		//放大到整个客户区大小
		pSvgViewer->ZoomView(1, CSize(0, 0), CSize(rc.Width(), rc.Height()));
		//pSvgDoc->ZoomView(1, nX, nY);
		m_szCurSize.cx = rc.Width();
		m_szCurSize.cy = rc.Height();

		SetScrollSizes(MM_TEXT, m_szCurSize);
		//刷新客户区
		DocToClient(rc);
		InvalidateRect(rc);
		
	}
}

/*************************************************************
 函 数 名：OnMapZoomin()
 功能概要：放大图形, X+100, y+75
 返 回 值: void
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
		//刷新客户区
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
 函 数 名：OnMapZoomout()
 功能概要：缩小图形, X-100, y-75
 返 回 值: void
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

		//刷新客户区
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
 函 数 名：OnMapZoomOld()
 功能概要：图形回复原始大小
 返 回 值: void
**************************************************************/
//##ModelId=49B87B7D008E
void CXJBrowserView::OnMapZoomOld() 
{
	// TODO: Add your command handler code here
	if(m_bLoaded)
	{
		int nX = m_szOriSize.cx - m_szCurSize.cx;
		int nY = m_szOriSize.cy - m_szCurSize.cy;
		
		//放大到原始大小
		pSvgViewer->ZoomView(2, CSize(-nX, -nY), m_szOriSize);
		m_szCurSize = m_szOriSize;

		//刷新客户区
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
 函 数 名：SetDeviceCol()
 功能概要：根据设备状态设置图形颜色,先判断挂牌状态,再判断运行状态, 运行状态为"运行"时判断通讯状态
 返 回 值: void
 参    数：param1	二次设备指针
**************************************************************/
//##ModelId=49B87B7C02DE
void CXJBrowserView::SetDeviceCol( CSecObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;

	COLORREF colStatus; //状态对应颜色
	if (pObj->m_nSuspendstatus == 1)
	{
		colStatus = XJ_STATUS_RUN_MARK;
	}
	else
	{
		//先判断运行状态
		switch(pObj->m_nRunStatu)
		{
		case 0: //检修
			colStatus = XJ_STATUS_RUN_CHECK;
			break;
		case 1:	//停运
			colStatus = XJ_STATUS_RUN_OFF;
			break;
		case 2: //投运
			if(pObj->m_nCommStatus == 0)
			{
				//正常 
				colStatus = XJ_STATUS_COMM_ON;
			}
			else
			{
				//断开
				colStatus = XJ_STATUS_COMM_OFF;
			}
			break;
		case 3: //未接入
			colStatus = XJ_STATUS_RUN_CODE;
			break;
		case 4: //调试(对码表)
			colStatus = XJ_STATUS_RUN_DEBUG;
			break;
		default: //默认
			colStatus = XJ_STATUS_RUN_ON;
			break;
		}
	}
	

	//取得图形索引
	int nIndex = GetShapeIndex(pObj);
	//设置
	if(nIndex >= 0)
	{
		pSvgViewer->ChangedFillColor(nIndex, colStatus, this);
	}
}

/*************************************************************
 函 数 名：OnSTTP20002()
 功能概要：响应保护通讯状态变化上载通知(20002), 改变图形颜色
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B7D00BB
void CXJBrowserView::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	return;
	WriteLog("CXJBrowserView::OnSTTP20002 start", XJ_LOG_LV3);
	//如果现在无图形打开, 不处理
	if(!m_bLoaded)
		return;
	//无对应厂站, 不处理
	if(m_pStation == NULL)
		return;
	//解析报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;

	//检查报文头
	if(pSttpData->sttp_head.uMsgID != 20002)
	{
		CString str;
		str.Format("CXJBrowserView::OnSTTP20002 收到错误报文, 报文号为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();

	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		//得到设备ID
		CString strID = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		//找设备指针,检查是否在本图形中
		CSecObj* pObj = (CSecObj*)pData->FindDevice(strID, TYPE_SEC); //可能是录波器?
		if(pObj == NULL)
			continue;
		if(pObj->m_sStationID != m_pStation->m_sID)
		{
			//同一条报文送上来的肯定是同一厂站的数据,如果不同,直接退出
			return;
		}
		//设置状态
		pObj->m_nCommStatus = pSttpData->sttp_body.variant_member.value_data[i].id;
		//设置颜色
		SetDeviceCol(pObj);
	}
	WriteLog("CXJBrowserView::OnSTTP20002 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：OnSTTP20079()
 功能概要：响应保护运行状态变化上载通知(20079), 改变图形颜色
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B7D00BF
void CXJBrowserView::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	return;
	WriteLog("CXJBrowserView::OnSTTP20079, start", XJ_LOG_LV3);
	//如果现在无图形打开, 不处理
	if(!m_bLoaded)
		return;
	//无对应厂站, 不处理
	if(m_pStation == NULL)
		return;
	//解析报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//检查报文头
	if(pSttpData->sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CXJBrowserView::OnSTTP20079 收到错误报文, 报文号为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	//得到设备ID
	CString strID = pSttpData->sttp_body.ch_pt_id;
	//找设备指针,检查是否在本图形中
	CSecObj* pObj = (CSecObj*)pData->FindDevice(strID, TYPE_SEC); //可能是录波器?
	if(pObj == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
	{
		return;
	}
	//设置状态
	pObj->m_nRunStatu = pSttpData->sttp_body.nStatus;
	//设置颜色
	SetDeviceCol(pObj);
	WriteLog("CXJBrowserView::OnSTTP20079 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：AddBlinkDevice()
 功能概要：使指定二次设备闪烁, 关联的一次设备一起闪烁
 返 回 值: void 
 参    数：param1	指定二次设备
		   Param2
**************************************************************/
//##ModelId=49B87B7C02E0
void CXJBrowserView::AddBlinkDevice( CSecObj* pObj )
{
	//检查可行条件
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

	//先停止之前的闪烁
	//RemoveAllBlinkDevices();
	
	if(!bBlinking)
	{
		int nBlinkTime = 800;
		//取得二次设备图形索引
		int nIndex = GetShapeIndex(pObj);
		if(nIndex >= 0)
		{
			//闪烁
			pSvgViewer->BlinkShape(nIndex, TRUE, nBlinkTime);
		}

		//关联的一次设备;
		if(pObj->m_pOwner != NULL && g_BlinkOwner == 1)
		{
			nIndex = GetShapeIndex(pObj->m_pOwner);
			if(nIndex >= 0)
			{
				//闪烁
				pSvgViewer->BlinkShape(nIndex, TRUE, nBlinkTime);
			}
		}
	}

}

/*************************************************************
 函 数 名：RemoveAtBlinkDevice()
 功能概要：使指定二次设备不再闪烁, 与之关联的一次设备也停止闪烁
 返 回 值: void
 参    数：param1	指定二次设备
**************************************************************/
//##ModelId=49B87B7C02E2
void CXJBrowserView::RemoveAtBlinkDevice( CDeviceObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	
	//得到索引
	int nIndex = GetShapeIndex(pObj);
	if(nIndex >= 0)
		pSvgViewer->BlinkShape(nIndex, FALSE);
	
	
	//得到关联一次设备的103ID;
	CSecObj* pSec = (CSecObj*)pObj;
	if(pSec->m_pOwner != NULL && g_BlinkOwner == 1)
	{
		nIndex = GetShapeIndex(pSec->m_pOwner);
		if(nIndex >= 0)
			pSvgViewer->BlinkShape(nIndex, FALSE);
	}
	
}

/*************************************************************
 函 数 名：RemoveAllBlinkDevices()
 功能概要：停止当前画面所有设备闪烁
 返 回 值: void
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
 函 数 名：CenterDevice()
 功能概要：使指定设备图形居中显示
 返 回 值: void
 参    数：param1	指定设备
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
	
	//判断设备的图形是否有在图上
	int nIndex = GetShapeIndex(pObj);
	if(nIndex < 0)//图形找不到此设备
		return;
	
	CRect rect;
	rect = pSvgViewer ->GetShapeSize(nIndex);
	//如果图形范围不合理
 	if(rect.left == 32766 ||rect.top == 32766 || rect.right == -32766 || rect.bottom == -32766)
	{
		return;
	}
	//转换成设备坐标
	DocToClient(rect);
	//得到图形中点坐标, 滚动滚动条
	CPoint pt;
	pt.x = rect.left + rect.Width()/2;
	pt.y = rect.top + rect.Height()/2;
	CScrollView::CenterOnPoint(pt);
	
	//设置当前设备为选择
	pSvgViewer ->SetSelected(nIndex, TRUE, this);

	//刷新
	Invalidate();
}

/*************************************************************
 函 数 名：OnAllClose()
 功能概要：响应关闭消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7D00CC
void CXJBrowserView::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//关闭线程
	EndOwnerThread();
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出所有自己起的长久线程
 返 回 值: void
 参    数：param1
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
 函 数 名：RefreshDeviceStatus()
 功能概要：刷新设备状态
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7C034E
void CXJBrowserView::RefreshDeviceStatus()
{
	//检查数据有效性
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
		//设置颜色
		SetDeviceCol(pObj);
	}
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CXJBrowserView::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshDeviceStatus();
	WriteLog("CXJBrowserView::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名: OpenBayView()
 功能概要: 打开指定间隔的图形
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数: param1 指定间隔
		   Param2 
**************************************************************/
BOOL CXJBrowserView::OpenBayView( CBayObj* pObj )
{
	//先关闭当前图形
	CloseCurrentView();

	m_nSvgType = 2;
	if(pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	
	//查找厂站
	CStationObj* pStation = (CStationObj*)pData->FindDevice(pObj->m_sStationID, TYPE_STATION);
	
	//文件路径
	CString strPath;
	strPath = pObj->m_sSvgPath;
	//去除"\\",因为系统下载路径已经带了
	while(strPath.Left(1) == "\\")
	{
		strPath = strPath.Right(strPath.GetLength() - 1);
	}
	//保存数据
	m_pCurBay = pObj;
	m_sCurrentPath = pApp->GetDownDir() + strPath;
	m_pStation = pStation;
	m_sStationID = pObj->m_sStationID;

	WriteLog("打开SVG文件路径为:" + m_sCurrentPath, XJ_LOG_LV3);
	//打开图形
	if(!OpenView(m_sCurrentPath))
	{
		CString str;
		str.Format("打开SVG文件失败,路径为: %s", m_sCurrentPath);
		WriteLog(str, XJ_LOG_LV1);
		//打开失败
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
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;
	
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//是录波器
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		pDoc->ShowSecPropPage(pObj, (nID - ID_WR_GENERAL_NEW));
	}
}

//保护运行状态上载
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
 函 数 名: GetShapeIndex()
 功能概要: 取得设备的图形索引
 返 回 值: 图形索引
 参    数: param1 指定设备
		   Param2 
**************************************************************/
int CXJBrowserView::GetShapeIndex( CDeviceObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return -1;
	if(!m_bLoaded)
		return -1;
	
	//得到103ID
	CString str103ID;
	str103ID.Format("%d#%d", pObj->m_n103Group, pObj->m_n103Item);
	int nDevIndex = -1;
	//103组号条目号关联
	int nIndex = pSvgViewer->GetIndexByDevID(str103ID);
	if(nIndex >= 0)
	{
		nDevIndex = nIndex;
		return nDevIndex;
	}
	//保护ID关联
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
 函 数 名: FindDevicePlus()
 功能概要: 取得指定设备，兼容三种情况 
 返 回 值: 设备对象
 参    数: param1 指定设备ID
		   Param2 指定设备类型
**************************************************************/
CDeviceObj* CXJBrowserView::FindDevicePlus( CString sID, CString sType )
{
	CDeviceObj * pObj = NULL;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	
	int nType = TypeStringToInt(sType);
	//判断ID
	if(sID.Find('#', 0) >= 0)
	{
		//103组号条目号
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
		//停止闪烁
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
			else//切换厂站
				BlinkShapeInOtherStation(pObj);
		}
		else
			SetTimer(99, g_FlashTime*500, 0);//当前没有闪烁
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CXJBrowserView::AddToBlinkList( CSecObj* pObj )
{
	if(pObj == NULL)
		return;

	CString strLog;
	strLog.Format("收到闪烁%s(%s)通知,加入到待闪烁列表", pObj->m_sName, pObj->m_sID);
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
			//当前站不是要打开的站,打开新站
			BlinkShapeInOtherStation(pObj);
		}
		else
		{
			//闪烁
			BlinkShapeInStation(pObj->m_sStationID);
		}
	}
	strLog.Format("收到闪烁%s(%s)通知,闪烁处理完成", pObj->m_sName, pObj->m_sID);
	WriteLog(strLog, XJ_LOG_LV3);
}

void CXJBrowserView::OnMarkDevice() 
{
	// TODO: Add your command handler code here
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;
	
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
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
	//判断是否有选择
	if(m_selected.GetCount() < 1)
		return;
	
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		DLGMarked dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
}

/****************************************************
Date:2012/12/5  Author:LYH
函数名:   BlinkShapeInStation	
返回值:   void	
功能概要: 
参数: CString sStationID	
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
		strLog.Format("开始本站所有待闪烁图形闪烁, 当前站ID:%s", m_sStationID);
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
		strLog.Format("本站所有待闪烁图形闪烁, 当前站ID:%s, 本次闪烁%d个设备", m_sStationID, m_arrBlinking.GetSize());
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
		//启动定时器
		SetTimer(99, g_FlashTime*1000, 0);
	}
	else
	{
		SetTimer(99, g_FlashTime*500, 0);//当前没有闪烁
	}
}

/****************************************************
Date:2012/12/5  Author:LYH
函数名:   BlinkSec	
返回值:   int	
功能概要: 
参数: CSecObj * pObj	
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
	strLog.Format("开始闪烁图形, %s(%s)", pObj->m_sName, pObj->m_sID);
	WriteLog(strLog, XJ_LOG_LV3);
	//取得二次设备图形索引
	int nIndex = GetShapeIndex(pObj);
	if(nIndex >= 0 && !IsInBlinkArray(nIndex))
	{
		CenterDevice(pObj);
		//闪烁
		pSvgViewer->BlinkShape(nIndex, TRUE, m_nBlinkTime);
		arrBlink.Add(nIndex);
	}
	
	//关联的一次设备;
	if(pObj->m_pOwner != NULL && g_BlinkOwner == 1)
	{
		nIndex = GetShapeIndex(pObj->m_pOwner);
		if(nIndex >= 0 && !IsInBlinkArray(nIndex))
		{
			//闪烁
			pSvgViewer->BlinkShape(nIndex, TRUE, m_nBlinkTime);
			arrBlink.Add(nIndex);
		}
	}
	return nIndex;
}

/****************************************************
Date:2012/12/5  Author:LYH
函数名:   StopBlink	
返回值:   void	
功能概要: 
参数: CUIntArray arrBlink	
*****************************************************/
void CXJBrowserView::StopBlink( CUIntArray& arrBlink )
{
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	CString strLog;
	strLog.Format("停止所有图形闪烁, 当前站ID:%s", m_sStationID);
	WriteLog(strLog, XJ_LOG_LV3);
	for(int i = 0; i < arrBlink.GetSize(); i++)
	{
		//得到索引
		int nIndex = arrBlink.GetAt(i);
		if(nIndex >= 0)
			pSvgViewer->BlinkShape(nIndex, FALSE);
	}
	arrBlink.RemoveAll();
	strLog.Format("成功停止所有图形闪烁, 当前站ID:%s", m_sStationID);
	WriteLog(strLog, XJ_LOG_LV3);
}

/****************************************************
Date:2012/12/6  Author:LYH
函数名:   IsInBlinkArray	
返回值:   BOOL	
功能概要: 
参数: int nIndex	
*****************************************************/
BOOL CXJBrowserView::IsInBlinkArray( int nIndex )
{
	BOOL bReturn = FALSE;
	for(int i = 0; i < m_arrBlinking.GetSize(); i++)
	{
		//得到索引
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
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
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
函数名:   	
返回值:   void	
功能概要: 
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
		//保护或录波器
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
		//不自动切换图形,删除待闪烁列表中相同站的图形
		EnterCriticalSection(&m_CriticalSection);
		CString strLog;
		strLog.Format("不自动切换图形,移除[%s]下所有待闪烁图形", m_sStationID);
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
		strLog.Format("[%s]所有待闪烁图形移除, 本次移除%d个设备", m_sStationID, m_arrBlinking.GetSize());
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
		//取得图形索引
		int nIndex = GetShapeIndex(pObj);
		//设置
		if(nIndex >= 0)
		{
			CString sPath = g_LocalPath + "check1.png";
			pSvgViewer->AddMark(nIndex, sPath,16, this);
		}
	}
}

void CXJBrowserView::RefreshDeviceMark()
{
	//检查数据有效性
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
		//设置颜色
		SetMark(pObj);
	}
}

void CXJBrowserView::RefreshNoteMark()
{
	//检查数据有效性
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
		//设置颜色
		SetNoteMark(pObj);
	}
}

void CXJBrowserView::SetNoteMark(CSecObj* pObj)
{
	if(pObj->m_nNoteCount > 0 )
	{
		//取得图形索引
		int nIndex = GetShapeIndex(pObj);
		//设置
		if(nIndex >= 0)
		{
			CString sPath = g_LocalPath + "warning1.png";
			pSvgViewer->AddMark(nIndex, sPath,16, this);
		}
	}
}
