// ViewCommMap.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewCommMap.h"
#include "GlobalFunc.h"
#include "MainFrm.h"

#include "../../ui_common/Interface/svg_constant.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CViewCommMap

IMPLEMENT_DYNCREATE(CViewCommMap, CScrollView)

//##ModelId=49B87B8502C1
CViewCommMap::CViewCommMap()
{
	m_bInitCOM = FALSE;
	m_bLoaded = FALSE;
	pClf = NULL;
	pUnk = NULL;
	pSvgViewer = NULL;
	pSvgOperate = NULL;

	m_szOriSize.cx = 0;
	m_szOriSize.cy = 0;
	m_szCurSize.cx = 0;
	m_szCurSize.cy = 0;

	m_selected.RemoveAll();
	m_nCount = 100;
	m_bShapeRefreshed = FALSE;

	m_nStationsPerNet = 12;
	m_nStationWidth = 150;
	m_nStationHeight = 50;
	m_nStationSpace = 20;
	m_nNetSpace = 30;
	m_nNetTextHeight = 60;

	StationNumPreRow = 8;

	//初始化COM控件
	InitCOM();
}

//##ModelId=49B87B85032E
CViewCommMap::~CViewCommMap()
{
	WriteLog("start end viewcommmap");
	ReleaseTemp();
	TRACE("ReleaseTemp end\n");
	if(pSvgOperate!= NULL)
	{
		pSvgOperate->Release();
		pSvgOperate = NULL;
	}
	if(pSvgViewer != NULL)
	{
		pSvgViewer->Release();
		pSvgViewer = NULL;
	}
	if(pUnk != NULL)
		pUnk->Release();
	if(pClf != NULL)
		pClf->Release();
	TRACE("Is error here\n");

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
	
	WriteLog("CViewCommMap::~CViewCommMap");
}


BEGIN_MESSAGE_MAP(CViewCommMap, CScrollView)
	//{{AFX_MSG_MAP(CViewCommMap)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COM_MAP, OnComMap)
	ON_COMMAND(ID_COM_REFRESH, OnComRefresh)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(STATION_CHANGED, OnStationStatusChanged)
	ON_MESSAGE(STATION_INFO_CHANGED, OnStationInfoChanged)
	ON_MESSAGE(STATIONINIT, OnstationInit)
	ON_MESSAGE(REFRESH_COMMMAP, OnRefreshCommMap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCommMap drawing

//##ModelId=49B87B850320
void CViewCommMap::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	RecreateMap();
}

//##ModelId=49B87B85031D
void CViewCommMap::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
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
		pDrawDC ->FillSolidRect(rect, /*RGB(18, 69, 60)*/g_CommMapBK);

		//绘制SVG图形
		//////////////////////////////////////////////////////////////////////////
		if(pSvgViewer != NULL)
		{
			pSvgViewer->DrawShapes(pDrawDC);
			if( !m_bShapeRefreshed )
			{
				//设置图形颜色
				RefreshStatusToShape();
				m_bShapeRefreshed = TRUE;
				Invalidate();
			}
		}
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		pDrawDC ->FillSolidRect(rect, RGB(255, 255, 255));
		CString str = StringFromID(IDS_STATIONMAP_SHOWERROR);
		pDrawDC ->TextOut(20, 20, str);
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

/////////////////////////////////////////////////////////////////////////////
// CViewCommMap diagnostics

#ifdef _DEBUG
//##ModelId=49B87B850330
void CViewCommMap::AssertValid() const
{
	CScrollView::AssertValid();
}

//##ModelId=49B87B850332
void CViewCommMap::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCommMap message handlers

//##ModelId=49B87B850322
void CViewCommMap::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONCOMMU));
	//GetParentFrame()->SetWindowText("厂站通断");
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化COM控件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8502FF

typedef HRESULT (__stdcall * pfnHello)(REFCLSID,REFIID,void**);
void CViewCommMap::InitCOM()
{
	pfnHello fnHello= NULL;
	HINSTANCE hdllInst = LoadLibrary("XJSVGEX.dll");
	fnHello=(pfnHello)GetProcAddress(hdllInst,"DllGetClassObject");
	if (fnHello != 0)
	{
		pClf = NULL;
		HRESULT hr=(fnHello)(IID_ISvgViewer,IID_IClassFactory,(void**)&pClf);
		if(SUCCEEDED(hr) && (pClf != NULL))
		{
			hr = pClf->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);
			if(SUCCEEDED(hr) && (pUnk != NULL))
			{
				//取得显示功能接口
				pUnk ->QueryInterface(IID_ISvgViewer, (void **)&pSvgViewer);
				//取得操作功能接口
				pUnk->QueryInterface(IID_ISvgOperate, (void **)&pSvgOperate);
			}
		}
	}

	if(pSvgViewer == NULL)
	{
		WriteLog("ViewCommMap:unable to load SvgViewer Interface", XJ_LOG_LV1);
		return ;
	}
	
	if(pSvgOperate == NULL)
	{
		WriteLog("ViewCommMap:unable to load SvgOperate Interface", XJ_LOG_LV1);
		return ;
	}
	if(pSvgViewer)
	{
		//开启闪烁功能
		pSvgViewer->CartoonControl(TRUE, this);
	}
	m_bInitCOM = TRUE;
}

/*************************************************************
 函 数 名：OpenView()
 功能概要：打开指定的SVG文件
 返 回 值: 打开成功返回TRUE, 失败返回FALSE
 参    数：param1	文件路径
		   Param2
**************************************************************/
//##ModelId=49B87B850300
BOOL CViewCommMap::OpenView( CString strPath )
{
	if(!m_bInitCOM)
		return FALSE;
	if(strPath == "")
		return FALSE;

	ReleaseTemp();
	
	CString sError = "";
	CRect rcBound;
	int nResult = pSvgViewer->LoadFromFile(strPath, rcBound, sError);
	CSize szSize(rcBound.Width(), rcBound.Height());
	
	if(szSize.cx == 0 && szSize.cy ==0)
	{
		CString str;
		str.Format("主子站通讯图载入失败，原因为%s", sError);
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
		WriteLog("主子站通讯图, open the svg sucess");
		m_bLoaded = TRUE;
		if(szSize.cx < 0)
			szSize.cx = 100;
		if(szSize.cy <= 0)
			szSize.cy = 100;
		SetScrollSizes(MM_TEXT, szSize);
		m_szOriSize = szSize;
		m_szCurSize = m_szOriSize;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：DocToClient()
 功能概要：逻辑坐标转设备坐标
 返 回 值: void
 参    数：param1	要转换的坐标
**************************************************************/
//##ModelId=49B87B8502E6
void CViewCommMap::DocToClient( CPoint & pt )
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
void CViewCommMap::DocToClient( CRect & rect )
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
void CViewCommMap::ClientToDoc( CPoint & pt )
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
void CViewCommMap::ClientToDoc( CRect & rect )
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
	rect.NormalizeRect();
}

//##ModelId=49B87B85033D
BOOL CViewCommMap::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CScrollView::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B850340
void CViewCommMap::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//为载入图形，不操作
	if(!m_bLoaded)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	
	//如果有选择多个,只显示第一个的属性
	if(m_selected.GetCount() > 0)
	{
		CDeviceObj * pDevice = (CDeviceObj*)m_selected.GetHead();
		if(pDevice->m_nType == TYPE_STATION)
		{
			if(g_bShowDeviceMapWnd)
			{
				//打开厂站接线图
				pApp->ShowDevice(pDevice);
			}
			else
			{
				pApp->ShowStationInfo((CStationObj*)pDevice);
			}
			//在设备上定位
			pApp->LocateInTree((CStationObj*)pDevice);
		}
		else if(pDevice->m_nType == TYPE_BAY)
		{
			if(g_bShowDeviceMapWnd)
				pApp->ShowDevice(pDevice);
		}
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

//##ModelId=49B87B85034C
void CViewCommMap::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLoaded)
	{
		if(pSvgViewer != NULL)
			pSvgViewer ->DoLButtonDown(this, point, nFlags, 0);	
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

//##ModelId=49B87B850350
void CViewCommMap::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	
	if(m_bLoaded)
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
			//查找对应设备
			int nIndex = arrSelected.GetAt(i);
			CString strID = pSvgViewer->GetDevID(nIndex);
			CString strType = pSvgViewer ->GetDevType(nIndex);
//			int nType = TypeStringToInt(strType);
			CDeviceObj * pObj = pData->FindDevice(strID, TYPE_STATION);
			if(pObj != NULL)
			{
				m_selected.AddTail(pObj);
			}
			else
			{
				//加入对间隔的支持
				pObj = pData->FindDevice(strID, TYPE_BAY);
				if(pObj != NULL)
					m_selected.AddTail(pObj);
			}
		}
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

/*************************************************************
 函 数 名：StringToID()
 功能概要：把COM控件返回的选择设备字符串转化为各设备ID
 返 回 值: void
 参    数：param1	选择设备字符串
		   Param2	设备ID数组
**************************************************************/
//##ModelId=49B87B85030D
void CViewCommMap::StringToID( CString strID, CStringArray & arrID )
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
 函 数 名：ReleaseTemp()
 功能概要：清除零时数据
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B850310
void CViewCommMap::ReleaseTemp()
{
	if(m_bLoaded)
	{	
		//停止所有闪烁
		pSvgViewer->BlinkShape(-1, FALSE);

		m_szCurSize.cx = 0;
		m_szCurSize.cy = 0;
		m_szOriSize.cx = 0;
		m_szOriSize.cy = 0;
	}
}

/*************************************************************
 函 数 名：OnSTTP20144()
 功能概要：响应20144报文, 厂站状态改变报文
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B850361
void CViewCommMap::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewCommMap::OnSTTP20144 start", XJ_LOG_LV3);
	//取得报文数据
	CStationObj* pObj = (CStationObj*)lParam;
	if(pObj == NULL)
		return;
	
	//改变图形颜色
	if(m_bLoaded)
	{
		//状态
		SetStationShapeColor(pObj);
		//图形居中
		//CenterDevice(pObj->m_sID);
	}
	
	WriteLog("CViewCommMap::OnSTTP20144 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：SetStationShapeColor()
 功能概要：根据指定状态设置指定厂站颜色
 返 回 值: void
 参    数：param1	厂站
		   Param2	
**************************************************************/
//##ModelId=49B87B850311
void CViewCommMap::SetStationShapeColor( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	if(!m_bLoaded)
		return;

	COLORREF colStatus = XJ_STATUS_COMM_OFF;
	if(pStation->IsCommStatusON())
	{
		colStatus = XJ_STATUS_COMM_ON;
	}

	//取得图形索引
	int nIndex = pSvgViewer->GetIndexByDevID(pStation->m_sID);
	//设置
	if(nIndex >= 0)
	{
		pSvgViewer->ChangedFillColor(nIndex, colStatus, this);
		if(g_ShowDevStatusInStation)
		{
			if( pStation->IsCommStatusON() && pStation->GetCommAbnormityNum() > 0 )
				pSvgViewer->AddMark(nIndex, "warning1.png", 16, this);
			else
				pSvgViewer->AddMark(nIndex, "", 16, this); //取消标记			
		}
	}
	else
	{
		CString str;
		str.Format("在厂站状态图中未找到厂站[%s]的图形",pStation->m_sID);
		WriteLog(str, XJ_LOG_LV2);
	}
	
}

//##ModelId=49B87B85035C
BOOL CViewCommMap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	if(m_szCurSize.cy < rc.Height())
		return FALSE;
	//向下滚动滚轮时zDelta为负	
	CPoint point = GetScrollPosition();
	point.y -= zDelta;
	ScrollToPosition(point);
	Invalidate();
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

//##ModelId=49B87B8502FD
void CViewCommMap::CenterDevice( CString strID )
{
	if(strID == "")
		return;
	if(!m_bLoaded)
		return;
	
	//判断设备的图形是否有在图上
	int nIndex = pSvgViewer->GetIndexByDevID(strID);
	if(nIndex < 0)
		return;
	CRect rect = pSvgViewer ->GetShapeSize(nIndex);
	//如果图形范围不合理
	if(rect.left == 32766 ||rect.top == 32766 || rect.right == -32766 || rect.bottom == -32766)
		return;
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
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewCommMap::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		SetStationShapeColor(pObj);
	}
	WriteLog("CViewCommMap::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

void CViewCommMap::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLoaded)
	{
		if(pSvgViewer != NULL)
			pSvgViewer ->DoMouseMove(this, point, nFlags, 0);
	}
	CScrollView::OnMouseMove(nFlags, point);
}



void CViewCommMap::AddBlinkDevice( CDeviceObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	
	//先停止之前的闪烁
	//RemoveAllBlinkDevices();

	TimerTick* pTick = NULL;
	POSITION oldPos;
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
		KillTimer(pTick->nId);
		//RemoveAtBlinkDevice(pTime->pPoint);
		delete pTick;
		pTick = NULL;
		m_PtrList.RemoveAt(oldPos);
		bBlinking = TRUE;
	}
	
	//AddBlinkDevice(pObj);
	TimerTick* pTime = new TimerTick;
	pTime->nId = ++m_nCount;
	pTime->pPoint = pObj;
	m_PtrList.AddTail(pTime);
	SetTimer(pTime->nId, g_FlashTime*1000, 0);
	
	if(!bBlinking)
	{
		int nBlinkTime = 800;
		//取得图形索引
		int nIndex = GetShapeIndex(pObj);
		if(nIndex >= 0)
		{
			//闪烁
			pSvgViewer->BlinkShape(nIndex, TRUE, nBlinkTime);
		}
	}

	WriteLog("CViewCommMap::AddBlinkDevice, 闪烁厂站");
}


void CViewCommMap::RemoveAtBlinkDevice( CDeviceObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return;
	if(!m_bLoaded)
		return;
	if(pSvgViewer == NULL)
		return;
	
	int nBlinkTime = 800;
	//取得二次设备图形索引
	int nIndex = GetShapeIndex(pObj);
	if(nIndex >= 0)
	{
		//闪烁
		pSvgViewer->BlinkShape(nIndex, FALSE, nBlinkTime);
	}
}


int CViewCommMap::GetShapeIndex( CDeviceObj* pObj )
{
	//检查可行条件
	if(pObj == NULL)
		return -1;
	if(!m_bLoaded)
		return -1;
	
	/*
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
	*/
	int nDevIndex = -1;
	//保护ID关联
	int nIndex = pSvgViewer->GetIndexByDevID(pObj->m_sID);
	if(nIndex >= 0)
	{
		nDevIndex = nIndex;
		return nDevIndex;
	}
	return nDevIndex;
}


void CViewCommMap::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	POSITION pos = m_PtrList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION oldPos = pos;
		TimerTick* pTime = (TimerTick*)m_PtrList.GetNext(pos);
		MYASSERT(pTime);
		if (pTime->nId == nIDEvent)
		{
			KillTimer(nIDEvent);
			MYASSERT(pTime->pPoint);
			RemoveAtBlinkDevice((CDeviceObj*)pTime->pPoint);
			delete pTime;
			pTime = NULL;
			m_PtrList.RemoveAt(oldPos);
			break;
		}
	}
	
	CScrollView::OnTimer(nIDEvent);
}


void CViewCommMap::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

		CMenu Menu, *pMenu;
		if(!Menu.LoadMenu(IDR_MENU_MAP))
		{
			return;
		}
		pMenu = Menu.GetSubMenu(0);
		if (g_bShowGeoWnd)
			pMenu->EnableMenuItem(ID_COM_MAP, MF_ENABLED);
		pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

}


void CViewCommMap::OnComMap()
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame->pGeoFrame != NULL)
		pFrame->pGeoFrame->ActivateFrame(SW_SHOWMAXIMIZED);
}

/****************************************************
Date:2012/5/23  Author:LYH
函数名:   OnStationStatusChanged	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewCommMap::OnStationStatusChanged( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	CStationObj* pObj = (CStationObj*)lParam;
	SetStationShapeColor(pObj);
	//图形居中
	//CenterDevice(pObj->m_sID);
}

/****************************************************
Date:2013/10/8  Author:LYH
函数名:   OnStationInfoChanged	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewCommMap::OnStationInfoChanged( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	CStationObj* pObj = (CStationObj*)lParam;
	SetStationShapeColor(pObj);
	//图形居中
	//CenterDevice(pObj->m_sID);
}

void CViewCommMap::RefreshStatusToShape()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		SetStationShapeColor(pObj);
	}
}

BOOL CViewCommMap::CreateCommMap()
{
	if(pSvgViewer == NULL || pSvgOperate == NULL)
		return FALSE;
	m_arrStationCount.RemoveAll();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	POSITION pos = pData->m_NetList.GetHeadPosition();
	m_nStationsPerNet = 0;
	while( pos != NULL )
	{
		CNetObj* pNet = (CNetObj*)pData->m_NetList.GetNext(pos);
		DEVICE_LIST stations;
		pData->GetStationList(stations, pNet->m_sID);
		int size = stations.GetCount();
		if( size > m_nStationsPerNet )
			m_nStationsPerNet = size;
		m_arrStationCount.Add(size);
	}

	if(m_nStationsPerNet > 20)
		CreateVerticalCommMap();
	else
		CreateLandscapeCommMap();

	return TRUE;
}

BOOL CViewCommMap::CreateLandscapeCommMap()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	POSITION pos = pData->m_NetList.GetHeadPosition();

	int netWidth = m_nStationSpace*3+m_nStationWidth*2;
	int netHeight = 0;

	int countPreRow = 3;

	int startX = 160;
	int netCount = pData->m_NetList.GetCount();
	if( netCount < countPreRow ){
		int maxWidth = netWidth * countPreRow + m_nNetSpace * (countPreRow-1);
		int middleX = startX + maxWidth/2;
		int curWidth = netWidth * netCount + m_nNetSpace*(netCount -1);
		startX = middleX - (curWidth/2);
	}
	int nCount = 0;
	int x = startX;
	int y = 180;
	
	while(pos != NULL)
	{
		if( nCount == 0 || nCount%countPreRow == 0 )
		{
			m_nStationsPerNet = 0;
			for(int i = nCount; i < nCount+countPreRow; i++)
			{
				if( i >= m_arrStationCount.GetSize() )
					break;
				if( m_nStationsPerNet < m_arrStationCount.GetAt(i))
					m_nStationsPerNet = m_arrStationCount.GetAt(i);
			}
			if(m_nStationsPerNet%2 != 0)
				m_nStationsPerNet++;
			netHeight = m_nNetTextHeight + (m_nStationsPerNet/2 * (m_nStationSpace+m_nStationHeight)) + m_nStationSpace;
		}
		CNetObj* pNet = (CNetObj*)pData->m_NetList.GetNext(pos);
		CreateNetFigureLandscape(pNet, x, y);
		nCount++;
		if(nCount%countPreRow == 0)
		{
			y += ( netHeight + m_nNetSpace ); 
			x = startX;
		}
		else
		{
			x += ( netWidth + m_nNetSpace ); 
		}
	}
	return TRUE;
}

BOOL CViewCommMap::CreateVerticalCommMap()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	POSITION pos = pData->m_NetList.GetHeadPosition();
	
	int nCount = 0;
	int x = 90;
	int y = 180;
	
	int netWidth = 0;
	int netHeight = 0;
	while(pos != NULL)
	{
		int stationNum = m_arrStationCount.GetAt(nCount);
		if( stationNum < StationNumPreRow )
			stationNum = StationNumPreRow;
		netWidth = m_nNetTextHeight + ((StationNumPreRow) * (m_nStationSpace+m_nStationWidth)) + m_nStationSpace;
		netHeight = ((stationNum%StationNumPreRow==0)?(stationNum/StationNumPreRow):(stationNum/StationNumPreRow + 1)) * (m_nStationSpace+m_nStationHeight) + m_nStationSpace;


		CNetObj* pNet = (CNetObj*)pData->m_NetList.GetNext(pos);
		CraeteNetFigureVertical(pNet, x, y);
		nCount++;
		
		y += ( netHeight + m_nNetSpace ); 
		x = 90;
	}
	return TRUE;
}


void CViewCommMap::CreateStationFigure(CStationObj* pStation, int x, int y)
{
	if(pSvgViewer == NULL || pSvgOperate == NULL || pStation == NULL)
		return;
	int indexG = pSvgOperate->AddElement(-1, ElementName_Group);
	if(indexG >= 0)
	{
		int indexRect = pSvgOperate->AddElement(indexG, ElementName_Rect);
		CString str;
		if(indexRect >= 0)
		{
			pSvgOperate->SetAttributeValue(indexRect, attrPaint_Fill, "RGB(0, 128, 128)", this);
			pSvgOperate->SetAttributeValue(indexRect, attrPaint_Stroke, "RGB(255, 255, 255)", this);
			pSvgOperate->SetAttributeValue(indexRect, attrPaint_StrokeWidth, "2", this);
			str.Format("%d", x);
			pSvgOperate->SetAttributeValue(indexRect, attrSpecific_X, str, this);
			str.Format("%d", y);
			pSvgOperate->SetAttributeValue(indexRect, attrSpecific_Y, str, this);
			str.Format("%d", m_nStationWidth);
			pSvgOperate->SetAttributeValue(indexRect, attrSpecific_Width, str, this);
			str.Format("%d", m_nStationHeight);
			pSvgOperate->SetAttributeValue(indexRect, attrSpecific_Height, str, this);
		}
		int indexText = pSvgOperate->AddElement(indexG, ElementName_Text);
		if(indexText >= 0)
		{
			int textsize = g_CommMapFontSize;
			if(pStation->m_sName.GetLength() >= 18)
				textsize = 14;
			else if(pStation->m_sName.GetLength() <= 12 )
				textsize = g_CommMapFontSize;
			else
				textsize = 16;
			pSvgOperate->SetAttributeValue(indexText, attrPaint_Fill, g_CommMapFontColor, this);
			pSvgOperate->SetAttributeValue(indexText, attrPaint_StrokeWidth, "1", this);
			str.Format("%d", x+m_nStationWidth/2-(pStation->m_sName.GetLength()*textsize/2)/2);
			pSvgOperate->SetAttributeValue(indexText, attrSpecific_X, str, this);
			str.Format("%d", y+m_nStationHeight/2 + textsize/2);
			pSvgOperate->SetAttributeValue(indexText, attrSpecific_Y, str, this);
			pSvgOperate->SetAttributeValue(indexText, attrFont_Family, g_CommMapFont, this);
			str.Format("%d", textsize);
			pSvgOperate->SetAttributeValue(indexText, attrFont_Size, str, this);
			pSvgOperate->SetElementValue(indexText, pStation->m_sName, this);
			pSvgOperate->SetAttributeValue(indexText, attrFont_Weight, "bold", this);
		}
		pSvgOperate->SetAttributeValue(indexG, attrPsr_ObjectID, pStation->m_sID, this);
	}
}

void CViewCommMap::CreateNetFigureLandscape(CNetObj* pNet, int x, int y)
{
	if(pSvgViewer == NULL || pSvgOperate == NULL || pNet == NULL)
		return;
	int netWidth = m_nStationSpace*3+m_nStationWidth*2;
	int netHeight = m_nNetTextHeight + (m_nStationsPerNet/2 * (m_nStationSpace+m_nStationHeight)) + m_nStationSpace;

	CString str;
	CString lineStroke = "RGB(255, 128, 128)";
	int index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y+m_nNetTextHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+m_nNetTextHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}

	int indexText = pSvgOperate->AddElement(-1, ElementName_Text);
	if(indexText >= 0)
	{
		pSvgOperate->SetAttributeValue(indexText, attrPaint_Fill, "RGB(255, 128, 0)", this);
		pSvgOperate->SetAttributeValue(indexText, attrPaint_StrokeWidth, "1", this);
		str.Format("%d", x+netWidth/2-(pNet->m_sName.GetLength()*14)/2);
		pSvgOperate->SetAttributeValue(indexText, attrSpecific_X, str, this);
		str.Format("%d", y+m_nNetTextHeight/2 + 12 );
		pSvgOperate->SetAttributeValue(indexText, attrSpecific_Y, str, this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Family, StringFromID(IDS_HEAD_FONT), this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Size, "24", this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Weight, "bold", this);
		pSvgOperate->SetElementValue(indexText, pNet->m_sName, this);
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listStation;
	pData->GetSortStationList(listStation, pNet->m_sID);
	POSITION pos = listStation.GetHeadPosition();
	int nx = x + m_nStationSpace;
	int ny = y + m_nNetTextHeight + m_nStationSpace; 
	int nCount = 0;
	while(pos != NULL)
	{
		nCount++;
		CStationObj* pStation = (CStationObj*)listStation.GetNext(pos);
		CreateStationFigure(pStation, nx, ny);
		if( nCount%2 == 0 )
		{
			nx = x + m_nStationSpace; 
			ny += (m_nStationHeight + m_nStationSpace); 
		}
		else
		{
			nx += (m_nStationWidth+m_nStationSpace);
		}
	}
}

void CViewCommMap::CraeteNetFigureVertical(CNetObj* pNet, int x, int y)
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listStation;
	pData->GetSortStationList(listStation, pNet->m_sID);

	int stationNum = listStation.GetCount();
	if( stationNum < StationNumPreRow )
		stationNum = StationNumPreRow;
	int netWidth = m_nNetTextHeight + ((StationNumPreRow) * (m_nStationSpace+m_nStationWidth)) + m_nStationSpace;
	int netHeight = ((stationNum%StationNumPreRow==0)?(stationNum/StationNumPreRow):(stationNum/StationNumPreRow + 1)) * (m_nStationSpace+m_nStationHeight) + m_nStationSpace;

	CString str;
	CString lineStroke = "RGB(255, 128, 128)";
	int index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x+m_nNetTextHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+m_nNetTextHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	index = pSvgOperate->AddElement(-1, ElementName_Line);
	if(index >= 0)
	{
		pSvgOperate->SetAttributeValue(index, attrPaint_Fill, "RGB(0, 128, 128)", this);
		pSvgOperate->SetAttributeValue(index, attrPaint_Stroke, lineStroke, this);
		pSvgOperate->SetAttributeValue(index, attrPaint_StrokeWidth, "2", this);
		str.Format("%d", x);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X1, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y1, str, this);
		str.Format("%d", x+netWidth);
		pSvgOperate->SetAttributeValue(index, attrSpecific_X2, str, this);
		str.Format("%d", y+netHeight);
		pSvgOperate->SetAttributeValue(index, attrSpecific_Y2, str, this);
	}
	
	
	int indexText = pSvgOperate->AddElement(-1, ElementName_Text);
	if(indexText >= 0)
	{
		pSvgOperate->SetAttributeValue(indexText, attrPaint_Fill, "RGB(255, 128, 0)", this);
		pSvgOperate->SetAttributeValue(indexText, attrPaint_StrokeWidth, "1", this);
		str.Format("%d", x+m_nNetTextHeight/2-14);
		pSvgOperate->SetAttributeValue(indexText, attrSpecific_X, str, this);
		str.Format("%d", y+netHeight/2 - (pNet->m_sName.GetLength()/2 * 24)/2 + 20);
		pSvgOperate->SetAttributeValue(indexText, attrSpecific_Y, str, this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Family, StringFromID(IDS_HEAD_FONT), this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Size, "24", this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_Weight, "bold", this);
		pSvgOperate->SetAttributeValue(indexText, attrFont_WriteMode, "tb", this);
		pSvgOperate->SetElementValue(indexText, pNet->m_sName, this);
	}
	POSITION pos = listStation.GetHeadPosition();
	int nx = x + m_nNetTextHeight + m_nStationSpace;
	int ny = y + m_nStationSpace; 
	if( listStation.GetCount() <= StationNumPreRow )
	{
		ny = y + netHeight/2 - m_nStationHeight/2;
	}
	int nCount = 0;
	while(pos != NULL)
	{
		nCount++;
		CStationObj* pStation = (CStationObj*)listStation.GetNext(pos);
		CreateStationFigure(pStation, nx, ny);
		if( nCount%StationNumPreRow == 0 )
		{
			nx = x + m_nNetTextHeight + m_nStationSpace; 
			ny += (m_nStationHeight + m_nStationSpace); 
		}
		else
		{
			nx += (m_nStationWidth+m_nStationSpace);
		}
	}
}

void CViewCommMap::OnstationInit(WPARAM wParam, LPARAM lParam)
{
	RecreateMap();
}

void CViewCommMap::OnComRefresh() 
{
	// TODO: Add your command handler code here
	m_bShapeRefreshed = FALSE;
}

void CViewCommMap::OnRefreshCommMap(WPARAM wParam, LPARAM lParam)
{
	RecreateMap();
}

void CViewCommMap::RecreateMap()
{
	m_bShapeRefreshed = FALSE;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if( g_AutoStationCommMap > 0 )
	{
		CString strFullPath = g_LocalPath + g_strStationSvg;
		WriteLog(strFullPath, XJ_LOG_LV3);
		//打开图形
		if( OpenView(strFullPath) )
		{
			CreateCommMap();
			Invalidate();
		}
	}
	else
	{
		CString strFullPath = pApp->GetDownDir() + g_strStationSvg;
		WriteLog(strFullPath, XJ_LOG_LV3);
		//打开图形
		if( !OpenView(strFullPath) )
		{
			strFullPath = g_LocalPath + g_strStationSvg;
			WriteLog(strFullPath, XJ_LOG_LV3);
			//打开图形
			OpenView(strFullPath);
		}
		Invalidate();
	}
	
	m_bShapeRefreshed = FALSE;
}
