// GeoView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "GeoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeoView

IMPLEMENT_DYNCREATE(CGeoView, CScrollViewEx)

CGeoView::CGeoView()
{
	m_pSelStation = NULL;
	m_bMouseDown = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_bCanEdit = FALSE;
	m_bMouseLeave = FALSE;
	m_mostMin = FALSE;
	m_mapTime.RemoveAll();
	m_nBlinkTime = 800;
}

CGeoView::~CGeoView()
{
}


BEGIN_MESSAGE_MAP(CGeoView, CScrollViewEx)
	//{{AFX_MSG_MAP(CGeoView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GEO_EDIT, OnGeoEdit)
	ON_COMMAND(ID_GEO_DELETE, OnGeoDelete)
	ON_COMMAND(ID_GEO_OPEN, OnGeoOpen)
	ON_COMMAND(ID_GEO_SAVE, OnGeoSave)
	ON_COMMAND(ID_GEO_ZOOMIN, OnGeoZoomin)
	ON_COMMAND(ID_GEO_ZOOMOUT, OnGeoZoomout)
	ON_COMMAND(ID_GEO_ZOOM_RETURN, OnGeoZoomReturn)
	ON_COMMAND(ID_GEO_LOCALTREE, OnGeoLocaltree)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeoView drawing

void CGeoView::OnInitialUpdate()
{
// 	CScrollViewEx::OnInitialUpdate();
// 
// 	CSize sizeTotal;
// 	// TODO: calculate the total size of this view
// 	sizeTotal.cx = sizeTotal.cy = 100;
// 	SetScrollSizes(MM_TEXT, sizeTotal);

	SetZoomScaleMinMax(0.2f, 5.0f); //显示范围0.2 to 5 

	//注册OLE
	m_dropTarget.Register(this);

	HANDLE hBitmap = LoadImage(NULL, g_ImageFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(hBitmap != NULL)
	{
		m_Bitmap.Attach(hBitmap);
		BITMAP bm;    
		m_Bitmap.GetObject(sizeof(bm), &bm);
		m_nImageWidth = bm.bmWidth;    
		m_nImageHeight = bm.bmHeight;
		SetZoomSizes(CSize(m_nImageWidth,m_nImageHeight));
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_TIP_GEOGRAPH_NOTEXIST));
	}

	//定时器
	SetTimer(1, m_nBlinkTime, 0);
	CScrollViewEx::OnInitialUpdate();
}

void CGeoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CRect r;
	GetClientRect(&r);
	pDC->GetClipBox(&r);
	
	//Draw the bitmap
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);
	pDC->BitBlt(0, 0, m_nImageWidth, m_nImageHeight, &memDC, 0, 0, SRCCOPY);					
	memDC.SelectObject(pOldBitmap);

	//绘制所有Station
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj *pObj = (CStationObj*)listDevice.GetNext(pos);
		if(!pObj->IsShowInGeo())
			continue;
		pObj->DrawObj(pDC);
	}

	memDC.DeleteDC(); 
}

/////////////////////////////////////////////////////////////////////////////
// CGeoView diagnostics

#ifdef _DEBUG
void CGeoView::AssertValid() const
{
	CScrollViewEx::AssertValid();
}

void CGeoView::Dump(CDumpContext& dc) const
{
	CScrollViewEx::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGeoView message handlers

void CGeoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile( StringFromID(ID_TOOL_GEO));
//	GetParentFrame()->SetWindowText("地理图");
	CScrollViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名: FindFigure()
 功能概要: 查找被点击的厂站
 返 回 值: 被点击的厂站
 参    数: param1 点击的位置
		   Param2 
**************************************************************/
CStationObj* CGeoView::FindFigure( CPoint pt )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return NULL;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj *pObj = (CStationObj *)listDevice.GetNext(pos);
		if(!pObj->IsShowInGeo())
			continue;
		if(pObj->HitTest(pt))
			return pObj;
	}
	return NULL;
}

void CGeoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint local = point;
	ClientToDoc(local);
	//取得选择
	GetSelected(local);
	RefreshObj(m_pSelStation);
	if(m_pSelStation != NULL)
	{
		//打开图形
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(g_bShowDeviceMapWnd)
			pApp->ShowDevice(m_pSelStation);
		else
			pApp->ShowStationInfo(m_pSelStation);
	}
	CScrollViewEx::OnLButtonDblClk(nFlags, point);
}

void CGeoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint local = point;
	ClientToDoc(local);
	m_bMouseDown = TRUE;
	m_ptMouse = local;
	if(GetSelected(local) != NULL)
	{
		//改变鼠标
		SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
	}
	else
	{
		//改变鼠标
		SetCursor(AfxGetApp()->LoadCursor(IDC_PAN));
	}
	//刷新
	RefreshObj(m_pSelStation);
	CScrollViewEx::OnLButtonDown(nFlags, point);
}

void CGeoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = FALSE;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	CScrollViewEx::OnLButtonUp(nFlags, point);
}

void CGeoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (!m_bMouseLeave)
	{
		// 鼠标第一次移入窗口时， 请求一个WM_MOUSELEAVE 消息
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE;
        _TrackMouseEvent(&tme);
        m_bMouseLeave = TRUE;
	}
	
	CPoint local = point;
	ClientToDoc(local);
	if(m_bMouseDown)
	{
		
		if(m_pSelStation != NULL && m_bCanEdit)//移动图形
		{
			RefreshObj(m_pSelStation);
			m_pSelStation->MoveObj(local);
			RefreshObj(m_pSelStation);
		}
		if(m_pSelStation == NULL) //移动画面
		{
			CSize szScroll;
			szScroll.cx = local.x - m_ptMouse.x;
			szScroll.cy = local.y - m_ptMouse.y;
			
			CPoint ptScroll = GetScrollPosition();
			ptScroll.x -= szScroll.cx;
			ptScroll.y -= szScroll.cy;
			
			ScrollToPosition(ptScroll);
		}
	}
	CScrollViewEx::OnMouseMove(nFlags, point);
}

/*************************************************************
 函 数 名: ClearSelected()
 功能概要: 清空所有选择
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CGeoView::ClearSelected()
{
	if(m_pSelStation == NULL)
		return;
	//选择标志置为FALSE
	m_pSelStation->SetSelected(FALSE);
	//刷新图形
	RefreshObj(m_pSelStation);
	//把当请选择置为空
	m_pSelStation = NULL;
}

void CGeoView::RefreshObj( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//取得范围
	CRect rc = pStation->GetBound();
	DocToClient(rc);
	InvalidateRect(rc, TRUE);
}

DROPEFFECT CGeoView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	DROPEFFECT dropEffect = DROPEFFECT_NONE;
	if(pDataObject->IsDataAvailable(g_dwESDataFormat))
	{
		//we only have copy effect.
		dropEffect = DROPEFFECT_COPY;
	}	
	return dropEffect;
//	return CScrollViewEx::OnDragOver(pDataObject, dwKeyState, point);
}

BOOL CGeoView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!m_bCanEdit)
	{
		AfxMessageBox( StringFromID(IDS_TIP_GEO_EDIT));
		return FALSE;
	}
	FORMATETC fmt = {g_dwESDataFormat, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
	HGLOBAL hGlobal = pDataObject->GetGlobalData(g_dwESDataFormat, &fmt);
	if(hGlobal == NULL)
		return FALSE;
	//get a pointer to the pointer
	CDeviceObj **pVoid = (CDeviceObj**)GlobalLock(hGlobal);
	if(AfxIsValidAddress(*pVoid, sizeof(CDeviceObj*)))
	{
		CDeviceObj *pObj= (CDeviceObj*)(*pVoid);
		if(pObj != NULL)
		{	
			if( pObj->m_nType != TYPE_STATION )
			{
				AfxMessageBox(StringFromID(IDS_TIP_GEO_STATIONONLY));
				return TRUE;
			}
			CStationObj* pStation = (CStationObj*)pObj;
			CPoint local=point;
			ClientToDoc(local);
			if(pStation->IsShowInGeo())	//如果图元已经在图上，则移动到新位置 
			{
				if( AfxMessageBox(StringFromID(IDS_TIP_GEO_CHANGEPOSITION),MB_YESNO|MB_ICONQUESTION) == IDNO)
					return TRUE;
				RefreshObj(pStation);
				//放到鼠标位置平移
				pStation->MoveObj(local);
				RefreshObj(pStation);
			}
			else
			{
				//在地理图中显示
				pStation->ShowInGeo(local);
				//刷新
				RefreshObj(pStation);
			}
			//让新增加的图元被选中
			SetSelected(pStation);
			RefreshObj(pStation);//更新区域 
		}
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return TRUE;

//	return CScrollViewEx::OnDrop(pDataObject, dropEffect, point);
}

/*************************************************************
 函 数 名: GetSelected()
 功能概要: 根据指定位置取得被选择的厂站
 返 回 值: 被选择的厂站
 参    数: param1 指定位置
		   Param2 
**************************************************************/
CStationObj* CGeoView::GetSelected(CPoint pt)
{
	ClearSelected();
	//找到新的选择
	CStationObj* pStation = FindFigure(pt);
	if(pStation == NULL)
		return NULL;
	//改变鼠标
	pStation->SetSelected(TRUE);
	m_pSelStation = pStation;
	return m_pSelStation;
}

/*************************************************************
 函 数 名: SetSelected()
 功能概要: 设置指定厂站被选中
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 
**************************************************************/
void CGeoView::SetSelected( CStationObj* pStation )
{
	ClearSelected();
	pStation->SetSelected(TRUE);
	m_pSelStation = pStation;
}

void CGeoView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint local = point;
	ScreenToClient(&local);
	ClientToDoc(local);
	//取得选择
	GetSelected(local);
	RefreshObj(m_pSelStation);
	//生成菜单
	CMenu Menu, *pMenu;
	if(!Menu.LoadMenu(IDR_MENU_GEO))
		return;
	pMenu = Menu.GetSubMenu(0);
	
	if(m_pSelStation != NULL)
	{
		//有选择
		if(m_bCanEdit)
		{
			pMenu->EnableMenuItem(ID_GEO_DELETE, MF_ENABLED);
		}
		pMenu->EnableMenuItem(ID_GEO_OPEN, MF_ENABLED);
		pMenu->EnableMenuItem(ID_GEO_LOCALTREE, MF_ENABLED);
	}
	//可编辑?
	if(m_bCanEdit)
	{
		pMenu->CheckMenuItem(ID_GEO_EDIT, MF_CHECKED);
		pMenu->EnableMenuItem(ID_GEO_SAVE, MF_ENABLED);
	}
	else
		pMenu->CheckMenuItem(ID_GEO_EDIT, MF_UNCHECKED);
	pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
}

void CGeoView::OnGeoEdit() 
{
	// TODO: Add your command handler code here
	m_bCanEdit = !m_bCanEdit;
}

void CGeoView::OnGeoDelete() 
{
	// TODO: Add your command handler code here
	//设置被选择的图形为不显示
	if(m_pSelStation != NULL)
	{
		m_pSelStation->HideInGeo();
		RefreshObj(m_pSelStation);
	}
}

void CGeoView::OnGeoOpen() 
{
	// TODO: Add your command handler code here
	if(m_pSelStation == NULL)
		return;
	//打开一次接线图
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(m_pSelStation);
}

void CGeoView::OnGeoSave() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj *pObj = (CStationObj *)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		pObj->Save();
	}
	AfxMessageBox( StringFromID(IDS_TIP_SAVE_FINISHED));
}

void CGeoView::OnGeoZoomin() 
{
	// TODO: Add your command handler code here
	if (m_mostMin)
	{
		m_mostMin = FALSE;
	}
	DoZoomIn(1.25);
//	Invalidate();
}

void CGeoView::OnGeoZoomout() 
{
	// TODO: Add your command handler code here
	if (m_mostMin)
	{
		AfxMessageBox( StringFromID(IDS_TIP_GEO_MINIMUM));
		return;
	}
	DoZoomOut(1.25);
//	Invalidate();
}

void CGeoView::OnGeoZoomReturn() 
{
	// TODO: Add your command handler code here
	DoZoomFull();
	m_mostMin = TRUE;
	//	Invalidate();
}

void CGeoView::OnGeoLocaltree() 
{
	// TODO: Add your command handler code here
	if(m_pSelStation == NULL)
		return;
	//打开一次接线图
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->LocateInTree(m_pSelStation);
}

BOOL CGeoView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	return CScrollViewEx::OnEraseBkgnd(pDC);
}

void CGeoView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseDown = FALSE;
	m_bMouseLeave = FALSE;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void CGeoView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	POSITION pos = m_mapTime.GetStartPosition();
	while(pos != NULL)
	{
		CString strID = "";
		int	nTime = 0;
		m_mapTime.GetNextAssoc(pos, strID, (CObject*&)nTime);
		CStationObj* pStation = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
		if(pStation == NULL)
			continue;
		
		//时间
		nTime -= m_nBlinkTime;
		if(nTime > 0)
		{
			//还要闪烁
			pStation->SetVisibility(!pStation->GetVisibility());
			m_mapTime.SetAt(pStation->m_sID, (CObject*)nTime);
			RefreshObj(pStation);
		}
		else
		{
			//停止闪烁
			pStation->SetVisibility(TRUE);
			m_mapTime.RemoveKey(pStation->m_sID);
			RefreshObj(pStation);
		}
	}
	CScrollViewEx::OnTimer(nIDEvent);
}

void CGeoView::BlinkStation( CDeviceObj* pObj )
{
	if(pObj == NULL)
		return;
	CStationObj* pStation = (CStationObj*)pObj;
	CenterOnPoint(pStation->GetPositionInGeo());
	Invalidate();
	m_mapTime.SetAt(pObj->m_sID, (CObject*)(g_FlashTime*1000));
}
