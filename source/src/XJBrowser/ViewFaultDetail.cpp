// ViewFaultDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewFaultDetail.h"
#include "MainFrm.h"
#include "GlobalFunc.h"
#include "ExcelOp.h"

#include "..\..\common_open_source\cairo_include\cairo-win32.h"
/////////////////////////////////////////////////////////////////////////////
// CViewFaultDetail

IMPLEMENT_DYNCREATE(CViewFaultDetail, CFormView)

//##ModelId=49B87B840159
CViewFaultDetail::CViewFaultDetail()
	: CFormView(CViewFaultDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewFaultDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pReport = NULL;
	m_szDoc.cx = 0;
	m_szDoc.cy = 0;
	m_nOldSelEle = -1;
	pReportDoc = NULL;
	pWaveShape = new WaveShapeEngine;
	pClf = NULL;
	pUnk = NULL;
	m_sCurFile = "";
	m_bLoad = FALSE;
	m_bShowReturn = TRUE;
	m_pEvent = NULL;
	m_bGetNewFault = FALSE;
	m_nMaxHeight = -1;
	m_nMaxWidth = -1;
	m_bShowDI = TRUE;
}

//##ModelId=49B87B840244
CViewFaultDetail::~CViewFaultDetail()
{
	if(pWaveShape !=NULL)
		delete pWaveShape;


	if(pClf != NULL)
		pClf->Release();
	if(pUnk != NULL)
		pUnk->Release();
	if(pReportDoc != NULL)
		pReportDoc->Release();

	m_bitmapNew.DeleteObject();
		dcNew.DeleteDC();
	TRACE("CViewFaultDetail::~CViewFaultDetail \n");
}

//##ModelId=49B87B840214
void CViewFaultDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewFaultDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewFaultDetail, CFormView)
	//{{AFX_MSG_MAP(CViewFaultDetail)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_FAULT_PRINT, OnFaultPrint)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(IDC_FAULT_RETURN, OnFaultReturn)
	ON_COMMAND(IDC_FAULT_EXPORT, OnFaultExport)
	ON_COMMAND(IDC_FAULT_REFRESH, OnFaultRefresh)
	ON_COMMAND(IDC_FAULT_SHOWDI, OnFaultShowdi)
	//}}AFX_MSG_MAP
	ON_MESSAGE(FAULTREPORT_SEL_CHANGED, OnSelChanged)
	ON_MESSAGE(FAULTREPORT_PRINT, OnPrintFaultReport)
	ON_MESSAGE(FAULTDETAIL_REFRESH, OnRefreshFaultReport)
	ON_MESSAGE(FAULTREPORT_EXPORT_EXCEL, OnExportExcelFaultReport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFaultDetail diagnostics

#ifdef _DEBUG
//##ModelId=49B87B840251
void CViewFaultDetail::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B840261
void CViewFaultDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewFaultDetail message handlers

//##ModelId=49B87B840222
void CViewFaultDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_FAULTREPORT));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B840208
void CViewFaultDetail::OnInitialUpdate() 
{
	WriteLog("CViewFaultDetail::OnInitialUpdate, 开始初始化视图");
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//初始化COM
	WriteLog("CViewFaultDetail::OnInitialUpdate, 开始初始化COM");
	InitCOM();
	WriteLog("CViewFaultDetail::OnInitialUpdate, 初始化COM完成");

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//得到系统下载路径
	m_sCurFile = FAULTREPORT_MULTI_PT;
	CSize szDoc(100, 100);
	if(pReportDoc != NULL)
	{
		szDoc = OpenReportFile(pReportDoc, m_sCurFile);
		if(szDoc.cx > 0 || szDoc.cy > 0)
			m_bLoad = TRUE;
		else
		{
			m_bLoad = FALSE;
			CString sLog;
			sLog.Format("打开故障报告模板文件失败.\r\n%s", m_sCurFile);
			WriteLog(sLog, XJ_LOG_LV1);
		}
	}
	WriteLog("CViewFaultDetail::OnInitialUpdate, 打开模板文件完毕");
	//设置回调函数
	if(pReportDoc != NULL && m_bLoad)
		pReportDoc->SetQueryFunction(TranslateKeyInFaultDetail);
	//设置滚动条，给下边和右边留点空间
	szDoc.cx += 10;
	szDoc.cy += 10;
	SetScrollSizes(MM_TEXT, szDoc);
	m_szDoc = szDoc;
// 	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
// 	pApp->m_pFaultDetail = this;
	
	EnableToolTips(TRUE);
    m_tool.Create(this);
	m_tool.AddTool(this,"");
	m_tool.SetDelayTime(50);
	m_tool.Activate(true);

	g_UseLandscapeMultiStation = CheckLandscape();

	WriteLog("CViewFaultDetail::OnInitialUpdate, 初始化视图完毕");
}

//##ModelId=49B87B840271
void CViewFaultDetail::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化COM接口
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B840197
BOOL CViewFaultDetail::InitCOM()
{
	/*HRESULT hr;
	if((hr = ::CLSIDFromProgID(L"XJReportDoc", &clsid)) != NOERROR)
	{
		TRACE("unable to find program ID -- error= %x\n", hr);
		return FALSE;
	}
	
	if((hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, NULL, 
		IID_IClassFactory, (void **)&pClf)) != NOERROR)
	{
		TRACE("unable to find CLSID -- error = %x\n", hr);
		return FALSE;
	}
	
	pClf ->CreateInstance(NULL, IID_IUnknown, (void **)&pUnk);
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);*/
	return InitReportComNoReg(pClf, pReportDoc);
	//return TRUE;
}

//##ModelId=49B87B840234
void CViewFaultDetail::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	pDC->FillSolidRect(0,0,99999,99999,RGB(255,255,255));
		CBitmap* pOldBitmapNew = NULL;
	CRect rcNew;
	CRect rcClient;
		GetClientRect(&rcClient);
	rcNew.left = 0;
	rcNew.top = 0;
	rcNew.right = max(rcClient.Width(), m_nMaxWidth);
	m_nMaxWidth = max(rcClient.Width(), m_nMaxWidth);
	int nLenth = m_nMaxHeight;
// 	if (m_pReport != NULL)
// 	{
// 		nLenth = m_szDoc.cy - (m_pReport->m_listWROSC.GetSize() + m_pReport->m_listPTOSC.GetSize() ) * 500;
// 		}
	rcNew.bottom =  max(rcClient.Height(), nLenth);
	static bool bFirst = true;
	if (bFirst)
	{
		if (!dcNew.CreateCompatibleDC(pDC))
			return;
		if (!m_bitmapNew.CreateCompatibleBitmap(pDC, rcNew.Width(), rcNew.Height()))
			return;	
		//dcNew.FillSolidRect(rcNew, RGB(255, 255, 255));
		bFirst = FALSE;
	}
	if (m_bGetNewFault)
	{
		
		m_bitmapNew.DeleteObject();
		dcNew.DeleteDC();
		if (!dcNew.CreateCompatibleDC(pDC))
			return;
		if (!m_bitmapNew.CreateCompatibleBitmap(pDC, rcNew.Width(), rcNew.Height()))
			return;	
			//dcNew.FillSolidRect(rcNew, RGB(255, 255, 255));
		//bFirst = FALSE;
	}


	
	pOldBitmapNew = dcNew.SelectObject(&m_bitmapNew);
	if (m_pReport==NULL)
	{
		dcNew.FillSolidRect(rcNew.left,rcNew.top,m_nMaxWidth,m_nMaxHeight, RGB(255, 255, 255));
	}
		

	if (m_bGetNewFault)
	{
		BuildNewBitmap(pDC);//制作打印图样
		
		
	
	}
	pDC->BitBlt(0,0,rcNew.Width(),m_nMaxHeight,&dcNew,0,0,SRCCOPY);
		dcNew.SelectObject(pOldBitmapNew);



}

/*************************************************************
 函 数 名：OnSelChanged()
 功能概要：故障报告选择改变, 更改显示
 返 回 值: void
 参    数：param1	消息参数
		   Param2	被选择的故障报告
**************************************************************/
//##ModelId=49B87B8402CE
void CViewFaultDetail::OnSelChanged( WPARAM wParam, LPARAM lParam )
{
	m_bGetNewFault = TRUE;
	m_pReport = (CFaultReport*)lParam;
	if(m_pReport == NULL)
	{
		m_pReport = NULL;
		pReportDoc->FillValueData();
		Invalidate();
		return;
	}

	g_wndWait.Show();
	//载入详细信息
	if(m_pReport->m_bLoadDetail == FALSE)
	{
		m_pReport->m_bShowReturn = m_bShowReturn;
		m_pReport->LoadDetail();
	}
	else
	{
		if(m_pReport->m_bShowReturn != m_bShowReturn)
		{
			m_pReport->m_bShowReturn = m_bShowReturn;
			m_pReport->LoadDetail(TRUE);
		}
	}

	//判断应该显示什么模板
	CString sFile = GetTemplateFile(m_pReport);

	CSize szDoc(100, 100);

	if(sFile != m_sCurFile)
	{
		m_sCurFile = sFile;
		if (pReportDoc != NULL)
		{
			szDoc = OpenReportFile(pReportDoc, m_sCurFile);
			if(szDoc.cx > 0 || szDoc.cy > 0)
			{
				m_bLoad = TRUE;
			}
			else
			{
				m_bLoad = FALSE;
				szDoc.cx = szDoc.cy = 100;
				CString sLog;
				sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_FAULTREPORT_TEMPLATE_FAIL),m_sCurFile);
				WriteLog(sLog, XJ_LOG_LV1);
				AfxMessageBox(sLog);
			}
		}
	}
	
	if(pReportDoc != NULL && m_bLoad)
	{
		szDoc = pReportDoc->FillValueData();
	}
	
//	int nAdd = (m_pReport->m_listWROSC.GetSize() + m_pReport->m_listPTOSC.GetSize() ) * 500 + 10; 
	//设置滚动条，给下边和右边留点空间
	szDoc.cx += 10;
	szDoc.cy += 10;
	m_nMaxHeight = szDoc.cy;
	if (g_FaultShowWave == 1)
	{
		szDoc.cy = GetBitMapSize(szDoc.cx,szDoc.cy);
	
	}
	SetScrollSizes(MM_TEXT, szDoc);
	m_szDoc = szDoc;
	//szDoc.cy = m_nMaxHeight;
	//刷新
	Invalidate();
	g_wndWait.Hide();
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字代表的意义
 返 回 值: 关键字代表的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8401D4
CString CViewFaultDetail::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//判断是否有.GetCount()方法
	nFind = sKey.Find(".GetCount()", 0);
	if(nFind > -1)
	{
		if( m_sCurFile != FAULTREPORT_MULTI_COLOR_LANDSCAPE && m_sCurFile != FAULTREPORT_MULTI_PT_LANDSCAPE)
			return GetValue_getcount(sKey);
		else
			return GetValue_getcount_Landscape(sKey);
	}
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//没有[, 为普通关键字
		//普通关键字
		if(sKey == "$DEVICE_NAME$") //二次设备
		{
			if(m_pReport->m_listSec.GetSize() == 1)
			{
				CSecObj* pObj = (CSecObj*)m_pReport->m_listSec.GetAt(0);
				if(pObj != NULL)
					return pObj->m_sName;
			}
			return "";
		}
		if(sKey == "$FAULT_NAME$")
			return m_pReport->m_sName;
		if(sKey == "$FAULT_DISTANCE$") //故障测距
		{
			CString sDistance = "";
			if(m_pReport->m_listSec.GetSize() == 1)
			{
				CSecObj* pObj = (CSecObj*)m_pReport->m_listSec.GetAt(0);
				if(pObj != NULL)
				{
					
					sDistance = m_pReport->GetPTFaultDistance(pObj->m_sID);
					if( sDistance != "")
						sDistance.Format("%s[%s] %s", StringFromID(IDS_COMMON_AWAYFROM),pObj->m_pStation->m_sName, m_pReport->GetPTFaultDistance(pObj->m_sID));
				}
			}else{
				sDistance = m_pReport->m_sFaultDistance;
				
			}
			sDistance.TrimLeft();
			sDistance.TrimRight();
			if( sDistance == "" )
				return "-";
			return (sDistance+" km");
		}
		if(sKey == "$FAULT_TYPE$") //故障类型
			return m_pReport->GetFaultType();
		if(sKey == "$FAULT_TIME$") //故障时间
			return m_pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		if(sKey == "$FAULT_TIME_MS$")//故障时间毫秒值
		{
			sReturn.Format("%03d", m_pReport->m_nmsFaultTime);
			return sReturn;
		}
		if(sKey == "$FAULT_STATION1$") //故障厂站1
		{
			if(m_pReport->m_pStation1 != NULL)
				return m_pReport->m_pStation1->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_STATION2$") //故障厂站2
		{
			if(m_pReport->m_pStation2 != NULL)
				return m_pReport->m_pStation2->m_sName;
			else
				return "";
		}
		if(sKey == "$FAULT_DEVICE$") //一次设备
		{
			if(m_pReport->m_pPriDevice != NULL)
				return m_pReport->m_pPriDevice->m_sName;
			else
				return "";
		}
	}
	else
	{
		int nFind2 = sKey.Find('[', nFind+1);
		if( m_sCurFile != FAULTREPORT_MULTI_COLOR_LANDSCAPE && m_sCurFile != FAULTREPORT_MULTI_PT_LANDSCAPE)
		{
			if(nFind2 == -1)
			{
				//只有一个[
				return GetValue_OneLayer(sKey);
			}
			else
			{
				nFind = sKey.Find('[', nFind2 + 1);
				if(nFind == -1)
				{
					//只有两个
					return GetValue_TwoLayer(sKey);
				}
			}
		}
		else
		{
			if(nFind2 == -1)
			{
				//只有一个[
				return GetValue_OneLayer_Landscape(sKey);
			}
			else
			{
				nFind = sKey.Find('[', nFind2 + 1);
				if(nFind == -1)
				{
					//只有两个
					return GetValue_TwoLayer_Landscape(sKey);
				}
			}
		}
	}
	return "";
}

/*************************************************************
 函 数 名：OnPrintFaultReport()
 功能概要：响应打印命令
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8402DE
void CViewFaultDetail::OnPrintFaultReport( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	if(m_pReport == NULL)
		return;
	if(pReportDoc != NULL && m_bLoad)
	{
		g_hook = 1;
		pReportDoc->Print();
		g_hook = 0;
	}
}

/*************************************************************
 函 数 名：OnContextMenu()
 功能概要：响应内容菜单消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B840281
void CViewFaultDetail::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//生成菜单
	CMenu Menu, *pMenu;
	if(!Menu.LoadMenu(IDR_MENU_FAULTREPORT))
		return;
	pMenu = Menu.GetSubMenu(0);
	
	if(m_pReport != NULL)
	{
		//只选择了一个
	//	pMenu->EnableMenuItem(IDC_FAULT_EDIT, MF_ENABLED);
		pMenu->EnableMenuItem(IDC_FAULT_PRINT, MF_ENABLED);
	//	pMenu->EnableMenuItem(IDC_FAULT_WEB, MF_ENABLED);
		pMenu->EnableMenuItem(IDC_FAULT_RETURN, MF_ENABLED);
		pMenu->EnableMenuItem(IDC_FAULT_EXPORT, MF_ENABLED);
		if(m_bShowReturn)
			pMenu->CheckMenuItem(IDC_FAULT_RETURN, MF_CHECKED);
		else
			pMenu->CheckMenuItem(IDC_FAULT_RETURN, MF_UNCHECKED);
		pMenu->EnableMenuItem(IDC_FAULT_SHOWDI, MF_ENABLED);
		if(m_bShowDI)
			pMenu->CheckMenuItem(IDC_FAULT_SHOWDI, MF_CHECKED);
		else
			pMenu->CheckMenuItem(IDC_FAULT_SHOWDI, MF_UNCHECKED);

	}
	pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
}

/*************************************************************
 函 数 名：OnFaultPrint()
 功能概要：响应打印消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B840291
void CViewFaultDetail::OnFaultPrint() 
{
	// TODO: Add your command handler code here
	//判断数据有效性
	if(m_pReport == NULL)
		return;
	if(pReportDoc != NULL && m_bLoad)
	{
		g_hook = 1;
		pReportDoc->Print();
		if (m_pReport != NULL && g_FaultShowWave == 1)
		{
			int nPTLenth = m_pReport->m_listPTOSC.GetSize();
			for(int i  = 0; i < nPTLenth; i++)
			{
				m_pEvent = (CFaultEvent*)m_pReport->m_listPTOSC.GetAt(i);
				MYASSERT_CONTINUE(m_pEvent);
				//打开录波文件
				CString strPath = m_pEvent->m_sEventDef;
				//录波简报才有
				
				// 					strPath.Replace("/", "\\");
				// 					int nPos = strPath.ReverseFind('\\');
				// 					if(nPos != -1)
				// 					{
				// 						strPath.Delete(0, nPos+1);
				// 					}
				//判断是否有后缀名
				if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
					strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
					strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
					strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
				{
					//加上CFG后缀
					strPath += ".cfg";
				}
				//得到完整路径
				strPath = GetFullPath(m_pEvent->m_pSec, strPath);
				//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
				XJHANDLE hWaveShape;
				int nWidth = 0;
				int nHeight = 0;
				char* sPath = strPath.GetBuffer(sizeof(strPath));
				char arrError[1024];
				char* sError = arrError;
				if (pWaveShape->xjws_LoadWSLib())
				{
					/*continue;*/
				}
				else
				{
					AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
					break;
				}
			
				if(pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape)>=0)
				{
					pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetCanvasRange(0,0,nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetShapeDirection(0,hWaveShape);
					pWaveShape->xjws_PrintShape(hWaveShape);
					pWaveShape->xjws_CloseWaveHandle(hWaveShape);
				}
			}
		
			int nOSCLenth = m_pReport->m_listWROSC.GetSize();
			for(int j  = 0; j < nOSCLenth; j++)
			{
				m_pEvent = (CFaultEvent*)m_pReport->m_listWROSC.GetAt(j);
				MYASSERT_CONTINUE(m_pEvent);
				//打开录波文件
				CString strPath = m_pEvent->m_sEventDef;
				//录波简报才有
				
				// 					strPath.Replace("/", "\\");
				// 					int nPos = strPath.ReverseFind('\\');
				// 					if(nPos != -1)
				// 					{
				// 						strPath.Delete(0, nPos+1);
				// 					}
				//判断是否有后缀名
				if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
					strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
					strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
					strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
				{
					//加上CFG后缀
					strPath += ".cfg";
				}
				//得到完整路径
				strPath = GetFullPath(m_pEvent->m_pSec, strPath);
				//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
				XJHANDLE hWaveShape;
				int nWidth = 0;
				int nHeight = 0;
				char* sPath = strPath.GetBuffer(sizeof(strPath));
				char arrError[1024];
				char* sError = arrError;
				if (pWaveShape->xjws_LoadWSLib())
				{
					/*continue;*/
				}
				else
				{
					AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
					break;
				}
				if(pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape)>=0)
				{
					pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetCanvasRange(0,0,nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetShapeDirection(0,hWaveShape);
					int k = pWaveShape->xjws_PrintShape(hWaveShape);
					pWaveShape->xjws_CloseWaveHandle(hWaveShape);
				}
			}
		}
		g_hook = 0;
	}
}

/*************************************************************
 函 数 名：DocToClient()
 功能概要：逻辑坐标转设备坐标
 返 回 值: void
 参    数：param1	要转换的坐标
**************************************************************/
//##ModelId=49B87B8401A5
void CViewFaultDetail::DocToClient( CPoint & pt )
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
//##ModelId=49B87B8401B5
void CViewFaultDetail::DocToClient( CRect & rect )
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
//##ModelId=49B87B8401B7
void CViewFaultDetail::ClientToDoc( CPoint & pt )
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
//##ModelId=49B87B8401C5
void CViewFaultDetail::ClientToDoc( CRect & rect )
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&rect);
	rect.NormalizeRect();
}

//##ModelId=49B87B84029F
void CViewFaultDetail::OnLButtonDown(UINT nFlags, CPoint point) 
{
	pDot=point;
	// TODO: Add your message handler code here and/or call default
	//转为逻辑坐标
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);
	//判断被点击
	int nHit = -1;
	if(pReportDoc != NULL && m_pReport != NULL && m_bLoad)
	{
		nHit = pReportDoc->GetHitElement(ptDoc);
	}
	
	if(nHit >= 0)
	{
		CString strValueCode = "";
		if(pReportDoc != NULL && m_bLoad)
			strValueCode = pReportDoc->GetValueCode(nHit);
		if(strValueCode != "")
		{
			//得到对应的事件
			CFaultEvent* pEvent = GetEventByKey(strValueCode);
			if(pEvent != NULL)
			{
				m_pEvent = pEvent;
				//是录波简报时才操作
				if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT || pEvent->m_nType == XJ_FAULT_EVENT_OSCPARSE)
				{
					//打开录波文件
					CString strPath = pEvent->m_sEventDef;
					//判断是否有后缀名
					if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
						strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
						strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
						strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
					{
						//加上CFG后缀
						strPath += ".cfg";
					}
					//得到完整路径
					strPath = GetFullPath(pEvent->m_pSec, strPath);
					//打开文件
					OpenWaveFile(strPath, GetSafeHwnd());
				}
			}
		}
	}
	
	CFormView::OnLButtonDown(nFlags, point);
}

//##ModelId=49B87B8402AF
BOOL CViewFaultDetail::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8402B2
void CViewFaultDetail::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//转为逻辑坐标
	CPoint ptDoc = point;
	ClientToDoc(ptDoc);
	//判断被点击
	int nHit = -1;
	if(pReportDoc != NULL && m_pReport != NULL && m_bLoad)
	{
		nHit = pReportDoc->GetHitElement(ptDoc);
	}

	if(nHit >= 0)
	{
		CString strValueCode = "";
		if(pReportDoc != NULL && m_bLoad)
			strValueCode = pReportDoc->GetValueCode(nHit);
		if(strValueCode != "")
		{
			//得到对应的事件
			CFaultEvent* pEvent = GetEventByKey(strValueCode);
			if(pEvent != NULL)
			{
				//是录波简报时才操作
				if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
				{
					m_tool.UpdateTipText(StringFromID(IDS_TIP_CLICK_OPENWAVE), this);
				}
			}
		}
	}


	
	if(nHit >= 0)
	{
		if(nHit != m_nOldSelEle)
		{
			//还原之前选择的元素文字颜色
			if(m_nOldSelEle >= 0)
			{
				if(pReportDoc != NULL && m_bLoad)
				{
					CRect rect = pReportDoc->RevertTextColor(m_nOldSelEle);
					DocToClient(rect);
					InvalidateRect(rect);
					m_nOldSelEle = -1;
				}
			}
		}
		CString strValueCode = "";
		if(pReportDoc != NULL && m_bLoad)
			strValueCode = pReportDoc->GetValueCode(nHit);
		if(strValueCode != "")
		{
			if(GetEventByKey(strValueCode) != NULL)
			{
				//改变鼠标
			//	SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
				//改变文字颜色
				if(m_nOldSelEle != nHit)
				{
					CRect rc = pReportDoc->SetElementTextColor(nHit, RGB(255, 0, 0));
					m_nOldSelEle = nHit;
					DocToClient(rc);
					InvalidateRect(rc);
				}
			}
			else
			{
			//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			}
		}
		else
		{
		//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
	}
	else
	{
		//还原之前选择的元素文字颜色
		if(m_nOldSelEle >= 0)
		{
			if(pReportDoc != NULL && m_bLoad)
			{
				CRect rect = pReportDoc->RevertTextColor(m_nOldSelEle);
				DocToClient(rect);
				InvalidateRect(rect);
				m_nOldSelEle = -1;
			}
		}
		//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	CFormView::OnMouseMove(nFlags, point);
}

/*************************************************************
 函 数 名：GetEventByKey()
 功能概要：由关键字得到对应的故障事件, 是对应$OSC_FILE_NAME$的才返回指针, 其它都为NULL
 返 回 值: 故障事件指针
 参    数：param1 关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8401E4
CFaultEvent* CViewFaultDetail::GetEventByKey( CString sKey )
{
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return NULL;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return NULL;
	//解释每个关键字, 查找事件, 记录尾部字符
	CStringArray arrTail;
	arrTail.RemoveAll();
	CFaultEvent* pReturnEvent = NULL;
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sTail;
		CFaultEvent* pEvent = NULL;
		if( m_sCurFile != FAULTREPORT_MULTI_COLOR_LANDSCAPE && m_sCurFile != FAULTREPORT_MULTI_PT_LANDSCAPE)
			pEvent =  GetEventBySignleKey(arrKey.GetAt(i), sTail);
		else
			pEvent =  GetEventBySignleKey_Landscape(arrKey.GetAt(i), sTail);
		if(pEvent != NULL)
		{
			pReturnEvent = pEvent;
			if(sTail == "$OSC_FILE_NAME$" || sTail == "$L2_OSC_FILE_NAME$")
				arrTail.Add(sTail);
		}
	}
	//查找完毕
	if(pReturnEvent == NULL)
		return NULL;
	if(arrTail.GetSize() < 1)
	{
		//没有尾部关键字
		return NULL;
	}
	return pReturnEvent;
}

/*************************************************************
 函 数 名：GetEventBySignleKey()
 功能概要：由单个关键字找到对应的故障事件
 返 回 值: 故障事件指针
 参    数：param1	关键字
		   Param2	尾部关键字
**************************************************************/
//##ModelId=49B87B8401E6
CFaultEvent* CViewFaultDetail::GetEventBySignleKey( CString sKey, CString & sTail)
{
	//判断数据有效性
	if(sKey == "")
		return NULL;
	if(m_pReport == NULL)
		return NULL;
	sTail = "";
	CFaultEvent* pEvent = NULL;
	int nFind = -1;
	//判断是否带有[ ]运算符
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//带有[]运算符
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//没找到结束括号
			return NULL;
		}
		int nFind3 = sKey.Find('[', nFind2);
		if(nFind3 != -1)
		{
			//有两层
			int nFind4 = sKey.Find(']', nFind3);
			if(nFind4 == -1)
				return NULL;
			//得到索引
			CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
			int nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			//得到头
			CString strHead = sKey.Left(nFind);
			if(strHead != "$DEVICE_LIST")
				return NULL;
			//设备列表, 取得设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return NULL;
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return NULL;
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return NULL;
			//建立零时事件列表
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			
			//得到索引
			strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
			nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);
			
			//得到事件
			CFaultEvent* pEvent = NULL;
			if(strMid == "FAULT_ACTION_CHR_LIST")
			{
				//动作特征值
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
			}
			if(strMid == "FAULT_ACTION_SIGN_LIST")
			{
				//动作信号
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempSign.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempSign[nIndex];
			}
			if(strMid == "FAULT_PT_OSC_LIST")
			{
				//保护录波
				if(nIndex < 0 || nIndex >=m_pReport->m_listTempPTOSC.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSC[nIndex];
			}
			//取得尾部
			CString str = sKey.Right(sKey.GetLength() - (nFind4 + 3));
			sTail = "$";
			sTail += str;
			return pEvent;
		}
		//取得索引号
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		//取得头部
		CString strHead = sKey.Left(nFind);
		//取得事件
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//动作特征值
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//动作信号
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_ LIST")
		{
			//告警
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//保护录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//录波器录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(strHead == "$OSCPARSE_LIST")
		{
			//录波分析
			if(nIndex < 0 || nIndex >=m_pReport->m_listOSCParse.GetSize())
			{
				return NULL;
			}
			pEvent = (CFaultEvent*)m_pReport->m_listOSCParse[nIndex];
		}
		if(pEvent == NULL)
			return NULL;
		//取得尾部字符
		sTail = "$";
		sTail += sKey.Right(sKey.GetLength() - (nFind2 + 3));
	}
	return pEvent;
}

//##ModelId=49B87B8402C0
BOOL CViewFaultDetail::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	GetClientRect(&rc);
	if(m_szDoc.cy < rc.Height())
		return FALSE;
	//向下滚动滚轮时zDelta为负	
	CPoint point = GetScrollPosition();
	point.y -= zDelta;
	ScrollToPosition(point);
	Invalidate();
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

/*************************************************************
 函 数 名：OnRefreshFaultReport()
 功能概要：收到强制刷新消息, 如果是正在显示的故障报告,由刷新
 返 回 值: void
 参    数：param1
		   Param2	故障报告
**************************************************************/
//##ModelId=49B87B8402EE
void CViewFaultDetail::OnRefreshFaultReport( WPARAM wParam, LPARAM lParam )
{
	CFaultReport* pReport = (CFaultReport*)lParam;
	if(pReport == NULL)
		return;
	if(pReport == m_pReport)
		OnSelChanged(wParam, lParam);
}

/*************************************************************
 函 数 名：GetFullPath()
 功能概要：得到指定二次设备的指定录波文件的完整路径
 返 回 值: 完整路径
 参    数：param1	指定二次设备
		   Param2	指定录波文件
**************************************************************/
//##ModelId=49B87B8401F4
CString CViewFaultDetail::GetFullPath( CSecObj* pSec, CString strFileName )
{
	//检查数据有效性
	if(pSec == NULL)
		return "";
	if(strFileName == "")
		return "";
	//得到基址
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath = "";
	strFullPath = strDownDir + strFileName;
	return strFullPath;
}

/*************************************************************
 函 数 名：GetValue_getcount()
 功能概要：得到关键字getcount的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8401F7
CString CViewFaultDetail::GetValue_getcount( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找是否有[
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//由单个关键字组成
		if(sKey == "$DEVICE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listSec.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ACTION_CHR_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listChr.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ACTION_SIGN_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listSign.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_ALARM_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listAlarm.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_PT_OSC_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listPTOSC.GetSize());
			return sReturn;
		}
		if(sKey == "$FAULT_WR_OSC_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWROSC.GetSize());
			return sReturn;
		}
		if(sKey == "$WAVEDISTANCE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWaveDistance.GetSize());
			return sReturn;
		}
		if(sKey == "$DISTANCE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listDistance.GetSize());
			return sReturn;
		}
		if(sKey == "$OSCPARSE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listOSCParse.GetSize());
			return sReturn;
		}
		if(sKey == "$DICHANGE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_bShowDI?m_pReport->m_listDIChange.GetSize():0);
			return sReturn;
		}
	}
	else
	{
		//得到头
		CString strHead = sKey.Left(nFind);
		if(strHead == "$DEVICE_LIST")
		{
			//得到索引
			int nFind1 = sKey.Find(']', nFind);
			if(nFind1 == -1)
				return "";
			CString sIndex = sKey.Mid(nFind+1, nFind1 - nFind -1);
			int nIndex = atoi(sIndex);
			//得到索引对应的设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return "";
			if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
				return "";
			CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
			if(pSec == NULL)
				return "";
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			//得到后段的关键字 // $DEVICE_LIST[1]::FAULT_PT_OSC_LIST.GetCount()$
			nFind = sKey.ReverseFind(':');
			if(nFind == -1)
				return "";
			CString strTail = sKey.Right(sKey.GetLength() - nFind - 1);
			if(strTail == "FAULT_ACTION_CHR_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempChr.GetSize());
				return sReturn;
			}
			if(strTail == "FAULT_ACTION_SIGN_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempSign.GetSize());
				return sReturn;
			}
			if(strTail == "FAULT_PT_OSC_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_pReport->m_listTempPTOSC.GetSize());
				return sReturn;
			}
			if(strTail == "DICHANGE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_bShowDI?m_pReport->m_listTempDIChange.GetSize():0);
				return sReturn;
			}
		}
	}
	return sReturn;
}

/*************************************************************
 函 数 名：GetValue_OneLayer()
 功能概要：得到一层深度关键字的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B840204
CString CViewFaultDetail::GetValue_OneLayer( CString sKey )
{
	//$DEVICE_LIST[11]::DEVICE_NAME$
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
	if(strHead == "$DEVICE_LIST")
	{
		//设备列表, 取得设备
		if(m_pReport->m_listSec.GetSize() < 1)
			return "";
		if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
			return "";
		CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
		if(pSec == NULL)
			return "";
		if(strTail == "DEVICE_NAME$")
		{
			return pSec->m_sName;
		}
		if(strTail == "DEVICE_STATION$")
		{
			if(pSec->m_pStation != NULL)
				return pSec->m_pStation->m_sName;
			return "";
		}
		if(strTail == "DEVICE_OWNER$")
		{
			if(pSec->m_pOwner != NULL)
				return pSec->m_pOwner->m_sName;
			return "";
		}
		if(strTail == "EVENT_TIME1_FULL$")
		{
			//得到第一个动作信息的时间
			if(pSec != m_pReport->m_pTempSec)
			{
				//组建零时数组
				m_pReport->BuildTempList(pSec);
			}
			if(m_pReport->m_listTempSign.GetSize() < 1)
				return "";
			CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listTempSign.GetAt(0);
			if(pEvent == NULL)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "FAULT_DISTANCE$")
		{
			CString sDistance = "";
			
			sDistance = m_pReport->GetPTFaultDistance(pSec->m_sID);
			if( sDistance != "")
				sDistance.Format("%s[%s] %s", StringFromID(IDS_COMMON_AWAYFROM),pSec->m_pStation->m_sName, m_pReport->GetPTFaultDistance(pSec->m_sID));
			
			sDistance.TrimLeft();
			sDistance.TrimRight();
			if( sDistance == "" )
				return "-";
			return (sDistance+" km");
		}
	}
	else
	{
		CFaultEvent* pEvent = NULL;
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//动作特征值
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return "";
			}

			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//动作信号
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_LIST")
		{
			//告警
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//保护录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//录波器录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(strHead == "$WAVEDISTANCE_LIST")
		{
			if(nIndex < 0 || nIndex >= m_pReport->m_listWaveDistance.GetSize())
				return "";
			pEvent = (CFaultEvent*)m_pReport->m_listWaveDistance[nIndex];
		}
		if(strHead == "$DISTANCE_LIST")
		{
			if(nIndex < 0 || nIndex >= m_pReport->m_listDistance.GetSize())
				return "";
			pEvent = (CFaultEvent*)m_pReport->m_listDistance[nIndex];
		}
		if(strHead == "$OSCPARSE_LIST")
		{
			if(nIndex < 0 || nIndex >= m_pReport->m_listOSCParse.GetSize())
				return "";
			pEvent = (CFaultEvent*)m_pReport->m_listOSCParse[nIndex];
		}
		if(strHead == "$DICHANGE_LIST")
		{
			if(nIndex < 0 || nIndex >= m_pReport->m_listDIChange.GetSize() || !m_bShowDI)
				return "";
			pEvent = (CFaultEvent*)m_pReport->m_listDIChange[nIndex];
		}
		if(pEvent == NULL)
			return "";
		if(strTail == "EVENT_TIME1_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME1$")
		{
			sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME1_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2$")
		{
			sReturn = pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_SEC_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_sName;
			else if(pEvent->m_nType == XJ_FAULT_EVENT_DISTANCE)
			{
				return StringFromID(IDS_FAULTREPORT_MAIN_LOCATION);
			}
		}
		if(strTail == "EVENT_STATION_NAME$")
		{
			if(pEvent->m_pSec != NULL)
			{
				if(pEvent->m_pSec->m_pStation != NULL)
					return pEvent->m_pSec->m_pStation->m_sName;
			}
		}
		if(strTail == "EVENT_OSC_TIME$")
		{
			//录波分析-故障时间
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_DURATIV$")
		{
			//录波分析-故障持续时间
			sReturn.Format("%s ms", pEvent->m_sEventContent);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_DISTANCE$")
		{
			//录波分析-测距值
			sReturn.Format("%s km", pEvent->m_sReceiveTime);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_PHASE$")
		{
			//录波分析-故障相别
			return pEvent->m_sSignaltime;
		}
		if(strTail == "EVENT_OSC_TRIGGER$")
		{
			//录波分析-跳闸相别
			return pEvent->m_sWaveDescribe;
		}
		if(strTail == "EVENT_OSC_RECLOSE$")
		{
			//录波分析-重合闸时间
			if(pEvent->m_nWaveFileNo <= 0)
				sReturn =  StringFromID(IDS_RECLOSE_NOTCLOSE);
			else
				sReturn.Format("%d ms", pEvent->m_nWaveFileNo);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_ELECTRIC$")
		{
			//录波分析-最大电流
			sReturn.Format("%s A", pEvent->m_sReverse2);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_ELECTRICPHASE$")
		{
			//录波分析-最大电流相别
			return pEvent->m_sReverse3;
		}
		if(strTail == "OSC_FILE_NAME$")
		{
			//录波分析-分析的录波文件
			CString str = pEvent->m_sEventDef;
			//得到文件名
			str.Replace("/", "\\");
			int nRFind = str.ReverseFind('\\');
			if(nRFind >= 0)
				str.Delete(0, nRFind+1);
			return str;
		}
		if(strTail == "EVENT_DEF$")
		{
			if(pEvent->m_nType == XJ_FAULT_EVENT_WAVEDISTANCE)
				return pEvent->m_sEventDef;
			return pEvent->m_sEventDefName;
		}
		if(strTail == "EVENT_CONTENT$")
		{
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//特征量
				int nEventDef = atoi(pEvent->m_sEventDef);
				if(nEventDef == 2)
				{
					//故障类型
					CString strFaultType = pEvent->GetFaultType(pEvent->m_sEventContent);
					return strFaultType;
				}
				CString v = pEvent->m_sEventContent;
				if( v.Find(".", 0) != -1 )
				{
					//认为是浮点数
					float f = atof(v);
					v.Format("%.2f", f);
				}
				CString str = v;
				if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
					str.Format("%s(%s)", v, pEvent->m_strUnit);
				return str;
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
			{
				//动作信号或告警
				if(pEvent->m_sEventContent == "1")
				{
					return StringFromID(IDS_CASE_ACTION);
				}
				else if(pEvent->m_sEventContent == "0")
				{
					return StringFromID(IDS_CASE_RETURN);
				}
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
			{
				if(pEvent->m_sEventContent == "1")
				{
					return "ON";
				}
				else if(pEvent->m_sEventContent == "0")
				{
					return "OFF";
				}
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_WAVEDISTANCE)
			{
				CString str;
				str.Format("%s km", pEvent->m_sSignaltime);
				return str;
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_DISTANCE)
			{
				CString str;
				str.Format("%s km", pEvent->m_sEventContent);
				return str;
			}
			return pEvent->m_sEventContent;
		}
		if(strTail == "OSC_FILE_NAME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			CString str = pEvent->m_sEventDef;
			//得到文件名
			str.Replace("/", "\\");
			int nRFind = str.ReverseFind('\\');
			if(nRFind >= 0)
				str.Delete(0, nRFind+1);
			return str;
		}
		if(strTail == "OSC_FILE_TIME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_RELETIVE_TIME$")
		{
			//判断是否事件
			
			int nOrgMs = 0;
			CTime tmStartTime;
			
			nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
			tmStartTime = m_pReport->m_tmFaultTime;
			
			if(pEvent->m_nType == XJ_FAULT_EVENT_SING && g_FaultActionTime == 1)
			{
				if((pEvent->m_tmStart == pEvent->m_tmSign) && (pEvent->m_nmsStart == pEvent->m_nmsSign))
				{
					nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
					tmStartTime = m_pReport->m_tmFaultTime;
				}
				else
				{
					nOrgMs = pEvent->m_nmsStart;
					tmStartTime = pEvent->m_tmStart;
				}
			}
			//int nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
			CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;
			LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
			//转为毫秒值
			LONG nMs = nSEC* 1000;
			LONG nCurMs = pEvent->m_nmsSign + nMs;
			//计算差值
			LONG nDis = nCurMs - nOrgMs;
			sReturn.Format("%d ms", nDis);
			return sReturn;
		}
	}
	return "";
}

/*************************************************************
 函 数 名：GetValue_TwoLayer()
 功能概要：得到两层深度关键字的值
 返 回 值: 值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B840206
CString CViewFaultDetail::GetValue_TwoLayer( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	if(strHead != "$DEVICE_LIST")
		return "";
	//设备列表, 取得设备
	if(m_pReport->m_listSec.GetSize() < 1)
		return "";
	if(nIndex < 0 || nIndex >= m_pReport->m_listSec.GetSize())
		return "";
	CSecObj* pSec = (CSecObj*)m_pReport->m_listSec.GetAt(nIndex);
	if(pSec == NULL)
		return "";
	//建立零时事件列表
	if(pSec != m_pReport->m_pTempSec)
	{
		//组建零时数组
		m_pReport->BuildTempList(pSec);
	}
	//取得中间的关键字
	int nFind3 = sKey.Find('[', nFind2);
	if(nFind3 == -1)
		return "";
	int nFind4 = sKey.Find(']', nFind3);
	if(nFind4 == -1)
		return "";
	//得到索引
	strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
	nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);

	//取得尾部
	CString strTail = sKey.Right(sKey.GetLength() - (nFind4 + 3));
	if(strTail == "DEVICE_NAME$")
	{
		return pSec->m_sName;
	}
	if(strTail == "DEVICE_STATION$")
	{
		if(pSec->m_pStation != NULL)
			return pSec->m_pStation->m_sName;
		else
			return "";
	}
	//得到事件
	CFaultEvent* pEvent = NULL;
	CFaultEvent* pEventFirst = NULL; //第一条事件,用来计算相对时间
	if(strMid == "FAULT_ACTION_CHR_LIST")
	{
		//动作特征值
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempChr.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempChr[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempChr[0];
	}
	if(strMid == "FAULT_ACTION_SIGN_LIST")
	{
		//动作信号
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempSign.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempSign[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempSign[0];
	}
	if(strMid == "FAULT_PT_OSC_LIST")
	{
		//保护录波
		if(nIndex < 0 || nIndex >=m_pReport->m_listTempPTOSC.GetSize())
		{
			return "";
		}
		pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSC[nIndex];
		pEventFirst = (CFaultEvent*)m_pReport->m_listTempPTOSC[0];
	}
	if(strMid == "DICHANGE_LIST")
	{
		//开关量变位
		if(nIndex < 0 || nIndex >= m_pReport->m_listTempDIChange.GetSize() || !m_bShowDI)
			return "";
		pEvent = (CFaultEvent*)m_pReport->m_listTempDIChange[nIndex];
		if(m_pReport->m_listTempSign.GetSize() > 0)
			pEventFirst = (CFaultEvent*)m_pReport->m_listTempSign[0];
		else
			pEventFirst = (CFaultEvent*)m_pReport->m_listTempDIChange[0];
	}
	if(pEvent == NULL)
		return "";

	if(strTail == "DEVICE_STATION$")
	{
		if(pSec->m_pStation != NULL)
			return pSec->m_pStation->m_sName;
		else
			return "";
	}
	
	if(strTail == "EVENT_TIME1_FULL$")
	{
		sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME1$")
	{
		sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:时%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME1_MS$")
	{
		sReturn.Format("%d", pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_FULL$")
	{
		sReturn.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2$")
	{
		sReturn = pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_MS$")
	{
		sReturn.Format("%d", pEvent->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_SEC_NAME$")
	{
		if(pEvent->m_pSec != NULL)
			return pEvent->m_pSec->m_sName;
	}
	if(strTail == "EVENT_STATION_NAME$")
	{
		if(pEvent->m_pSec != NULL)
			return pEvent->m_pSec->m_pStation->m_sName;
	}
	if(strTail == "EVENT_DEF$")
	{
		return pEvent->m_sEventDefName;
	}
	if(strTail == "EVENT_CONTENT$")
	{
		if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征量
			int nEventDef = atoi(pEvent->m_sEventDef);
			if(nEventDef == 2)
			{
				//故障类型
				CString strFaultType = pEvent->GetFaultType(pEvent->m_sEventContent);
				return strFaultType;
			}
			CString v = pEvent->m_sEventContent;
			if( v.Find(".", 0) != -1 )
			{
				//认为是浮点数
				float f = atof(v);
				v.Format("%.2f", f);
			}
			CString str = v;
			if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
				str.Format("%s(%s)", v, pEvent->m_strUnit);
			return str;
		}
		if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//动作信号或告警
			CString str="";
			if(pEvent->m_sEventContent == "1")
			{
				str = StringFromID(IDS_CASE_ACTION);
			}
			else if(pEvent->m_sEventContent == "0")
			{
				str =  StringFromID(IDS_CASE_RETURN);
			}
			return str;
		}
		if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
		{
			if(pEvent->m_sEventContent == "1")
			{
				return "ON";
			}
			else if(pEvent->m_sEventContent == "0")
			{
				return "OFF";
			}
		}
		return pEvent->m_sEventContent;
	}
	if(strTail == "OSC_FILE_NAME$")
	{
		//录波简报才有
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		//得到文件名
		CString str = pEvent->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
			str.Delete(0, nRFind+1);
		return str;
	}
	if(strTail == "OSC_FILE_TIME$")
	{
		//录波简报才有
		if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_RELETIVE_TIME$")
	{
		//判断是否事件
		if(pEventFirst == NULL)
			return "";
		int nOrgMs = 0;
		CTime tmStartTime;
		
		
		if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
		{
			nOrgMs = pEventFirst->m_nmsSign; //原始毫秒值
			tmStartTime = pEventFirst->m_tmSign;
			
			if(g_FaultActionTime == 1)
			{
				if((pEvent->m_tmStart == pEvent->m_tmSign) && (pEvent->m_nmsStart == pEvent->m_nmsSign))
				{
					//相对第一条事件
					nOrgMs = pEventFirst->m_nmsSign; //原始毫秒值
					tmStartTime = pEventFirst->m_tmSign;
				}
				else
				{
					nOrgMs = pEvent->m_nmsStart;
					tmStartTime = pEvent->m_tmStart;
				}
			}
		}
		else
		{
			nOrgMs = pEventFirst->m_nmsStart;
			tmStartTime = pEventFirst->m_tmStart;
		}
		CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;
		LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
		//转为毫秒值
		LONG nMs = nSEC* 1000;
		LONG nCurMs = pEvent->m_nmsSign + nMs;
		//计算差值
		LONG nDis = nCurMs - nOrgMs;
		sReturn.Format("%d ms", nDis);
		return sReturn;
	}
	
	return "";
}

void CViewFaultDetail::OnFaultReturn() 
{
	// TODO: Add your command handler code here
	m_bShowReturn = !m_bShowReturn;
	//重新载入数据
	if(m_pReport == NULL)
		return;
	m_pReport->m_bShowReturn = m_bShowReturn;
	m_pReport->LoadDetail(TRUE);
	SendMessage(FAULTDETAIL_REFRESH, 0, (LPARAM)m_pReport);
}

BOOL CViewFaultDetail::PreTranslateMessage(MSG* pMsg) 
{
	/*
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message = WM_MOUSEMOVE)
	{
		OnMouseMove(pMsg->wParam, pMsg->pt);
	}
	*/
	m_tool.RelayEvent(pMsg);
	return CFormView::PreTranslateMessage(pMsg);
}



void CViewFaultDetail::OnFaultExport() 
{
	// TODO: Add your command handler code here
	CFaultReport* pReport = m_pReport;
	try
	{
		
		if(pReport == NULL)
		{
			AfxMessageBox(StringFromID(IDS_RECLOSE_NOTCLOSE));
			return;
		}
		
		//ParserReport(pReport);
	}
	catch(...)
	{
		WriteLog("CViewFaultDetail::OnFaultExport() 解析故障报告出错");
	}
	
	try
	{
		// 		UpdateData();
		// 		char sOldPath[500];
		// 		GetCurrentDirectory(500,sOldPath);
		//	SetCurrentDirectory(sOldPath);
		
		MYASSERT(pReport);
		CFileDialog dlgFile(FALSE, "xls", pReport->m_sName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT/* | OFN_NOCHANGEDIR*/, NULL, NULL);
		dlgFile.m_ofn.lpstrTitle = StringFromID(IDS_FAULTREPORT_EXPORT); //对话框标题
		dlgFile.m_ofn.lpstrFilter =_T("*.xls"); //过滤
		//dlgFile.m_ofn.lpstrInitialDir = strSTDown;
		CString strFullPath = "";
		
		// 		HANDLE hMutex = CreateMutex(NULL,TRUE,NULL);
		// 		
		// 		WaitForSingleObject(hMutex.INFINITE);
		
		if(dlgFile.DoModal() == IDOK)
		{
			//得到全路径
			strFullPath = dlgFile.GetPathName();
			/*ReleaseMutex(hMutex);	*/
			// TODO: Add your command handler code here
			//ExportByExcel(strFullPath);
			if(pReportDoc != NULL)
			{
				if(pReportDoc->ExportToExcel(strFullPath) < 0)
				{
					AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_FAIL));
					return;
				}
			}
			else
			{
				AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_FAIL));
				return;
			}
			
			CString sFileName = dlgFile.GetFileName();
			CString sDir = strFullPath;
			sDir.Delete(strFullPath.GetLength()-sFileName.GetLength(),sFileName.GetLength());
			sDir += dlgFile.GetFileTitle();
			//sFileName.Delete(sFileName.Find("."),sFileName.GetLength());
			CreateDirectory(sDir,NULL);
			CString sDir1;
			CString sDir2;
			sDir1 = sDir + "\\" +pReport->m_sStation1ID + "\\";//;pReport->m_sStation1ID;//
			sDir2 = sDir + "\\" +pReport->m_sStation2ID + "\\";//;pReport->m_sStation1ID;//
			//sDir.Delete(strFullPath.GetLength()-sFileName.GetLength(), sFileName.GetLength());
			//sDir += dlgFile.GetFileTitle();
			//sDir += "-录波文件";
			CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
			CStationObj* pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pReport->m_sStation1ID,TYPE_STATION);
			if (pStation != NULL)
			{
				if(CreateDirectory(sDir1, NULL))
				{
					CString strLog;
					strLog.Format("创建目录成功:%s", sDir1);
					WriteLog(strLog, XJ_LOG_LV3);
					ExportWaveFile( sDir1, pReport);
				}
				else if(GetLastError() == ERROR_ALREADY_EXISTS)
				{
					CString strLog;
					strLog.Format("创建目录失败,目录已存在:%s", sDir1);
					WriteLog(strLog, XJ_LOG_LV1);
					ExportWaveFile( sDir1, pReport);
				}
				else
				{
					CString strLog;
					strLog.Format("创建目录失败:%s", sDir1);
					WriteLog(strLog, XJ_LOG_LV1);
				}
			}
			pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pReport->m_sStation2ID,TYPE_STATION);
			if (pStation != NULL)
			{
				if(CreateDirectory(sDir2, NULL))
				{
					CString strLog;
					strLog.Format("创建目录成功:%s", sDir2);
					WriteLog(strLog, XJ_LOG_LV3);
					ExportWaveFile( sDir2, pReport);
				}
				else if(GetLastError() == ERROR_ALREADY_EXISTS)
				{
					CString strLog;
					strLog.Format("创建目录失败,目录已存在:%s", sDir2);
					WriteLog(strLog, XJ_LOG_LV1);
					ExportWaveFile( sDir2, pReport);
				}
				else
				{
					CString strLog;
					strLog.Format("创建目录失败:%s", sDir2);
					WriteLog(strLog, XJ_LOG_LV1);
				}
			}
			//导出成功
			if(AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_SUCCESS), MB_OKCANCEL) == IDOK)
			{
				CString str = "/n,/select,";
				str += strFullPath;
				ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
			}
		}
		
		//ClearLevelList();
	}
	catch(...)
	{
		WriteLog("CViewFault::OnFaultExport() 导出故障报告出错");
	}
}

void CViewFaultDetail::OnFaultRefresh() 
{
	// TODO: Add your command handler code here
	
}

void CViewFaultDetail::BuildNewBitmap(CDC* pDC)
{
	//	pDC=&dcNew;
	dcNew.FillSolidRect(0,0,m_nMaxWidth,m_nMaxHeight,RGB(255,255,255));
		CRect rc;
		CRect rcClient;
		GetClientRect(&rcClient);
		rc.left = 0;
		rc.top = 0;
		rc.right = max(rcClient.Width(), m_nMaxWidth);
		CDC dcMemory;
		CBitmap bitmap;
		CBitmap* pOldBitmap = NULL;
		CSize szDoc(0, 0);
		if(pReportDoc != NULL && m_bLoad)
		{
			szDoc = pReportDoc->FillValueData();
		}
		int nLenth = /*m_nMaxHeight;//*/szDoc.cy+10;

			
		rc.bottom = max(rcClient.Height(), nLenth);
		


		if (!dcMemory.CreateCompatibleDC(pDC))
			return;
		
		if (!bitmap.CreateCompatibleBitmap(pDC, m_nMaxWidth, rc.Height()))
			return;	
		
		pOldBitmap = dcMemory.SelectObject(&bitmap);
		
		//画背景色
		
		dcMemory.FillSolidRect(0,0,m_nMaxWidth,rc.bottom, RGB(255, 255, 255));
		//画图形
		if(pReportDoc != NULL && m_pReport != NULL  && m_bLoad)
			pReportDoc->Draw(&dcMemory);
		//////////////////////////////////////////////////////////////////////////
		//pDC->BitBlt(0, 0, rc.Width(), rc.Height(),&dcMemory, 0, 0, SRCCOPY);
		dcNew.BitBlt(0, 0, rc.Width(), rc.Height(),&dcMemory, 0, 0, SRCCOPY);
		//////////////////////////////////////////////////////////////////////////	
		
		dcMemory.SelectObject(pOldBitmap);


		if (m_pReport!=NULL && g_FaultShowWave == 1)
		{
			int nNewTop=rc.bottom;
			int nPTLenth = m_pReport->m_listPTOSC.GetSize();
			for(int i  = 0; i < nPTLenth; i++)
			{
				m_pEvent = (CFaultEvent*)m_pReport->m_listPTOSC.GetAt(i);
				MYASSERT_CONTINUE(m_pEvent);
				//打开录波文件
				CString strPath = m_pEvent->m_sEventDef;
				//录波简报才有
				
				// 					strPath.Replace("/", "\\");
				// 					int nPos = strPath.ReverseFind('\\');
				// 					if(nPos != -1)
				// 					{
				// 						strPath.Delete(0, nPos+1);
				// 					}
				//判断是否有后缀名
				if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
					strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
					strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
					strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
				{
					//加上CFG后缀
					strPath += ".cfg";
				}
				//得到完整路径
				strPath = GetFullPath(m_pEvent->m_pSec, strPath);
				//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
				XJHANDLE hWaveShape;
				int nWidth = 0;
				int nHeight = 0;
				char* sPath = strPath.GetBuffer(sizeof(strPath));
				char arrError[1024];
				char* sError = arrError;
				if (pWaveShape->xjws_LoadWSLib())
				{
					/*continue;*/
				}
				else
				{
					AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
					break;
				}
				
				if(pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape)>=0)
				{
					
				
					
					if(m_pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
						return ;
					
					
					pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
					CDC dc;
					//	dc.FillRect(&CRect(rc.right, rc.top, rc.right + 50, rc.bottom), &CBrush(m_curPen))
					
					CBitmap bitmap2;
					if (!dc.CreateCompatibleDC(pDC))
						return;
					
					if (!bitmap2.CreateCompatibleBitmap(pDC, m_nMaxWidth, nHeight+50))
						return;	
					
					pOldBitmap = dc.SelectObject(&bitmap2);
					
					//画背景色
					dc.FillSolidRect(rc.left,rc.top,rc.right,nHeight+50, RGB(255, 255, 255));
					CRect rcNew(rc.left+80,rc.top+20,rc.right,50);
					dc.DrawText(sPath,&rcNew,NULL);
					rcNew.top-=20;
					cairo_surface_t * pSur = cairo_win32_surface_create(dc);
					pWaveShape->xjws_SetCanvasRange(rc.left,rc.top+50,nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetShapeDirection(0,hWaveShape);
					pWaveShape->xjws_DrawShape(pSur,hWaveShape);
					pWaveShape->xjws_CloseWaveHandle(hWaveShape);
					
					dcNew.BitBlt(0, nNewTop, rc.Width(), nHeight+50,&dc, 0, 0, SRCCOPY);
					nNewTop = nNewTop + nHeight +50;
					dc.SelectObject(pOldBitmap);
					
					bitmap2.DeleteObject();
					dc.DeleteDC();
				}
				
			}
		
			int nOSCLenth = m_pReport->m_listWROSC.GetSize();
			for(int j  = 0; j < nOSCLenth; j++)
			{
				m_pEvent = (CFaultEvent*)m_pReport->m_listWROSC.GetAt(j);
				MYASSERT_CONTINUE(m_pEvent);
				//打开录波文件
				CString strPath = m_pEvent->m_sEventDef;
				//录波简报才有
				
				// 					strPath.Replace("/", "\\");
				// 					int nPos = strPath.ReverseFind('\\');
				// 					if(nPos != -1)
				// 					{
				// 						strPath.Delete(0, nPos+1);
				// 					}
				//判断是否有后缀名
				if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
					strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
					strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
					strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
				{
					//加上CFG后缀
					strPath += ".cfg";
				}
				//得到完整路径
				strPath = GetFullPath(m_pEvent->m_pSec, strPath);
				//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
				XJHANDLE hWaveShape;
				int nWidth = 0;
				int nHeight = 0;
				char* sPath = strPath.GetBuffer(sizeof(strPath));
				char arrError[1024];
				char* sError = arrError;
				if (pWaveShape->xjws_LoadWSLib())
				{
					//continue;
				}
				else
				{
					AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
					break;
				}
				if(pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape)>=0)
				{
					
					
					
					if(m_pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
						return ;
					
					
					pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
					
					//画背景色
					CDC dc;
					//	dc.FillRect(&CRect(rc.right, rc.top, rc.right + 50, rc.bottom), &CBrush(m_curPen))
					
					CBitmap bitmap2;
					if (!dc.CreateCompatibleDC(pDC))
						return;
					
					if (!bitmap2.CreateCompatibleBitmap(pDC, m_nMaxWidth, nHeight+50))
						return;	
					
					pOldBitmap = dc.SelectObject(&bitmap2);
					
					dc.FillSolidRect(rc.left,rc.top,rc.right,nHeight+50, RGB(255, 255, 255));

					CRect rcNew(rc.left+80,rc.top+20,rc.right,50);
					dc.DrawText(sPath,&rcNew,NULL);
					rcNew.top-=20;
					cairo_surface_t * pSur = cairo_win32_surface_create(dc);
					pWaveShape->xjws_SetCanvasRange(rc.left,rc.top+50,nWidth,nHeight,hWaveShape);
					pWaveShape->xjws_SetShapeDirection(0,hWaveShape);
					pWaveShape->xjws_DrawShape(pSur,hWaveShape);
					pWaveShape->xjws_CloseWaveHandle(hWaveShape);
	
					dcNew.BitBlt(0, nNewTop, rc.Width(), nHeight+50,&dc, 0, 0, SRCCOPY);
					nNewTop = nNewTop + nHeight +50;
					dc.SelectObject(pOldBitmap);
					
					bitmap2.DeleteObject();
					dc.DeleteDC();
				}
				

			}
		}
		
		//释放资源
		bitmap.DeleteObject();
		dcMemory.DeleteDC();
		m_bGetNewFault = FALSE;
	
}

int CViewFaultDetail::GetBitMapSize(int x,int y)
{
	m_nMaxWidth = x;
	m_nMaxHeight = y;

	if (m_pReport != NULL)
	{
		int nPTLenth = m_pReport->m_listPTOSC.GetSize();
		for(int i  = 0; i < nPTLenth; i++)
		{
			m_pEvent = (CFaultEvent*)m_pReport->m_listPTOSC.GetAt(i);
			MYASSERT_CONTINUE(m_pEvent);
			//打开录波文件
			CString strPath = m_pEvent->m_sEventDef;
			//录波简报才有
			
			// 					strPath.Replace("/", "\\");
			// 					int nPos = strPath.ReverseFind('\\');
			// 					if(nPos != -1)
			// 					{
			// 						strPath.Delete(0, nPos+1);
			// 					}
			//判断是否有后缀名
			if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
				strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
				strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
				strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
			{
				//加上CFG后缀
				strPath += ".cfg";
			}
			//得到完整路径
			strPath = GetFullPath(m_pEvent->m_pSec, strPath);
			//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
			XJHANDLE hWaveShape;
			int nWidth = 0;
			int nHeight = 0;
			char* sPath = strPath.GetBuffer(sizeof(strPath));
			char arrError[1024];
			char* sError = arrError;
			if (pWaveShape->xjws_LoadWSLib())
			{
				/*continue;*/
			}
			else
			{
				AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
				break;
				}
			if (pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape) >= 0)
			{
				pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
				m_nMaxHeight = m_nMaxHeight + nHeight + 50;
				nWidth >= m_nMaxWidth ? m_nMaxWidth = nWidth : m_nMaxWidth = m_nMaxWidth;
			}

// 			pWaveShape->xjws_SetCanvasRange(0,0,nWidth,nHeight,hWaveShape);
// 			pWaveShape->xjws_SetShapeDirection(0,hWaveShape);
// 			pWaveShape->xjws_PrintShape(hWaveShape);
			pWaveShape->xjws_CloseWaveHandle(hWaveShape);
		}
		
		int nOSCLenth = m_pReport->m_listWROSC.GetSize();
		for(int j  = 0; j < nOSCLenth; j++)
		{
			m_pEvent = (CFaultEvent*)m_pReport->m_listWROSC.GetAt(j);
			MYASSERT_CONTINUE(m_pEvent);
			//打开录波文件
			CString strPath = m_pEvent->m_sEventDef;
			//录波简报才有
			
			// 					strPath.Replace("/", "\\");
			// 					int nPos = strPath.ReverseFind('\\');
			// 					if(nPos != -1)
			// 					{
			// 						strPath.Delete(0, nPos+1);
			// 					}
			//判断是否有后缀名
			if(strPath.Find(".cfg") == -1 && strPath.Find(".dat") == -1 && 
				strPath.Find(".CFG") == -1 && strPath.Find(".DAT") == -1 &&
				strPath.Find(".reh") == -1 && strPath.Find(".REH") == -1 &&
				strPath.Find(".rev") == -1 && strPath.Find(".REV") == -1)
			{
				//加上CFG后缀
				strPath += ".cfg";
			}
			//得到完整路径
			strPath = GetFullPath(m_pEvent->m_pSec, strPath);
			//strPath = GetFullPath(pEvent->m_pSec, pEvent->m_sEventDef);
			XJHANDLE hWaveShape;
			int nWidth = 0;
			int nHeight = 0;
			char* sPath = strPath.GetBuffer(sizeof(strPath));
			char arrError[1024];
			char* sError = arrError;
			if (pWaveShape->xjws_LoadWSLib())
			{
				/*continue;*/
			}
			else
			{
				AfxMessageBox(StringFromID(IDS_TIP_WAVESHAPE_NOTFOUND));
				break;
				}
						if (pWaveShape->xjws_LoadWaveFile(sPath,sError,hWaveShape) >= 0)
			{
				pWaveShape->xjws_GetFullSize(nWidth,nHeight,hWaveShape);
				m_nMaxHeight = m_nMaxHeight + nHeight + 50;
				nWidth >= m_nMaxWidth ? m_nMaxWidth = nWidth : m_nMaxWidth = m_nMaxWidth;
			}
			pWaveShape->xjws_CloseWaveHandle(hWaveShape);
			
		}
		}


	return m_nMaxHeight;	
}

/****************************************************
Date:2013/4/1  Author:LYH
函数名:   OnExportExcelFaultReport	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewFaultDetail::OnExportExcelFaultReport( WPARAM wParam, LPARAM lParam )
{
	OnFaultExport();
}

CString CViewFaultDetail::GetTemplateFile( CFaultReport* pReport )
{
	//默认为单保护模板
	CString sFile = FAULTREPORT_SINGLE_PT;
	if(g_UseColorFaultReport == 1)
		sFile = FAULTREPORT_SINGLE_COLOR;
	
	if(pReport == NULL)
		return sFile;
	
	if(pReport->m_listSec.GetSize() > 1)
	{
		if(pReport->m_pStation2 == NULL)
		{
			//单端多保护
			if(g_UseColorFaultReport == 1)
				sFile = FAULTREPORT_SINGLESTATION_MULTI_COLOR;
			else
				sFile = FAULTREPORT_SINGLESTATION_MULTI_PT;
		}
		else
		{
			//双端
			if(g_UseColorFaultReport == 1)
			{
				if(g_UseLandscapeMultiStation == 0)
					sFile = FAULTREPORT_MULTI_COLOR;
				else
					sFile = FAULTREPORT_MULTI_COLOR_LANDSCAPE;
			}
			else
			{
				if(g_UseLandscapeMultiStation == 0)
					sFile = FAULTREPORT_MULTI_PT;
				else
					sFile = FAULTREPORT_MULTI_PT_LANDSCAPE;
			}
		}
	}
	return sFile;
}

void CViewFaultDetail::OnFaultShowdi() 
{
	// TODO: Add your command handler code here
	m_bShowDI = !m_bShowDI;
	
	SendMessage(FAULTDETAIL_REFRESH, 0, (LPARAM)m_pReport);
}


CString CViewFaultDetail::GetValue_TwoLayer_Landscape(CString sKey)
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	if(strHead != "$DEVICE_LIST")
		return "";
	//设备列表, 取得设备
	if(m_pReport->m_listSec.GetSize() < 1)
		return "";
	CSecObj* pSecStation1 = NULL;
	CSecObj* pSecStation2 = NULL;
	if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation1.GetSize())
		pSecStation1 = (CSecObj*)m_pReport->m_listSecStation1.GetAt(nIndex);
	if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation2.GetSize())
		pSecStation2 = (CSecObj*)m_pReport->m_listSecStation2.GetAt(nIndex);
	if(pSecStation1 == NULL && pSecStation2 == NULL)
		return "";
	//建立零时事件列表
	if(pSecStation1 != m_pReport->m_pTempSecStation1 || pSecStation2 != m_pReport->m_pTempSecStation2)
	{
		//组建零时数组
		m_pReport->BuildTempListLandscape(pSecStation1, pSecStation2);
	}
	//取得中间的关键字
	int nFind3 = sKey.Find('[', nFind2);
	if(nFind3 == -1)
		return "";
	int nFind4 = sKey.Find(']', nFind3);
	if(nFind4 == -1)
		return "";
	//得到索引
	strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
	nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);

	//取得尾部
	CString strTail = sKey.Right(sKey.GetLength() - (nFind4 + 3));
	if(strTail == "DEVICE_NAME$")
	{
		if(pSecStation1 == NULL)
			return "";
		return pSecStation1->m_sName;
	}
	if(strTail == "L2_DEVICE_NAME$")
	{
		if(pSecStation2 == NULL)
			return "";
		return pSecStation2->m_sName;
	}
	if(strTail == "DEVICE_STATION$")
	{
		if(pSecStation1->m_pStation != NULL)
			return pSecStation1->m_pStation->m_sName;
		else
			return "";
	}
	if(strTail == "L2_DEVICE_STATION$")
	{
		if(pSecStation2->m_pStation != NULL)
			return pSecStation2->m_pStation->m_sName;
		else
			return "";
	}
	//得到事件
	CFaultEvent* pEventL1 = NULL;
	CFaultEvent* pEventFirstL1 = NULL; //第一条事件,用来计算相对时间
	CFaultEvent* pEventL2 = NULL;
	CFaultEvent* pEventFirstL2 = NULL; //第一条事件,用来计算相对时间
	if(strMid == "FAULT_ACTION_CHR_LIST")
	{
		//动作特征值
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempChrStation1.GetSize())
		{
			pEventL1 = (CFaultEvent*)m_pReport->m_listTempChrStation1[nIndex];
			pEventFirstL1 = (CFaultEvent*)m_pReport->m_listTempChrStation1[0];
		}
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempChrStation2.GetSize())
		{
			pEventL2 = (CFaultEvent*)m_pReport->m_listTempChrStation2[nIndex];
			pEventFirstL2 = (CFaultEvent*)m_pReport->m_listTempChrStation2[0];
		}
		
	}
	if(strMid == "FAULT_ACTION_SIGN_LIST")
	{
		//动作信号
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempSignStation1.GetSize())
		{
			pEventL1 = (CFaultEvent*)m_pReport->m_listTempSignStation1[nIndex];
			pEventFirstL1 = (CFaultEvent*)m_pReport->m_listTempSignStation1[0];
		}
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempSignStation2.GetSize())
		{
			pEventL2 = (CFaultEvent*)m_pReport->m_listTempSignStation2[nIndex];
			pEventFirstL2 = (CFaultEvent*)m_pReport->m_listTempSignStation2[0];
		}
		
	}
	if(strMid == "FAULT_PT_OSC_LIST")
	{
		//保护录波
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempPTOSCStation1.GetSize())
		{
			pEventL1 = (CFaultEvent*)m_pReport->m_listTempPTOSCStation1[nIndex];
			pEventFirstL1 = (CFaultEvent*)m_pReport->m_listTempPTOSCStation1[0];
		}
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempPTOSCStation2.GetSize())
		{
			pEventL2 = (CFaultEvent*)m_pReport->m_listTempPTOSCStation2[nIndex];
			pEventFirstL2 = (CFaultEvent*)m_pReport->m_listTempPTOSCStation2[0];
		}
		
	}
	if(strMid == "DICHANGE_LIST")
	{
		//开关量事件
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempDIChangeStation1.GetSize())
		{
			pEventL1 = (CFaultEvent*)m_pReport->m_listTempDIChangeStation1[nIndex];
			pEventFirstL1 = (CFaultEvent*)m_pReport->m_listTempDIChangeStation1[0];
		}
		if(nIndex >= 0 && nIndex < m_pReport->m_listTempDIChangeStation2.GetSize())
		{
			pEventL2 = (CFaultEvent*)m_pReport->m_listTempDIChangeStation2[nIndex];
			pEventFirstL2 = (CFaultEvent*)m_pReport->m_listTempDIChangeStation2[0];
		}
	}

	if(strTail == "DEVICE_STATION$")
	{
		if(pEventL1 == NULL)
			return "";
		if(pSecStation1->m_pStation != NULL)
			return pSecStation1->m_pStation->m_sName;
		else
			return "";
	}
	if(strTail == "L2_DEVICE_STATION$")
	{
		if(pEventL2 == NULL)
			return "";
		if(pSecStation2->m_pStation != NULL)
			return pSecStation2->m_pStation->m_sName;
		else
			return "";
	}
	
	if(strTail == "EVENT_TIME1_FULL$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn.Format("%s.%03d", pEventL1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEventL1->m_nmsStart);
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME1_FULL$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn.Format("%s.%03d", pEventL2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEventL2->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME1$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn = pEventL1->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME1$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn = pEventL2->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME1_MS$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn.Format("%d", pEventL1->m_nmsStart);
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME1_MS$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn.Format("%d", pEventL2->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_FULL$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn.Format("%s.%03d", pEventL1->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEventL1->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME2_FULL$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn.Format("%s.%03d", pEventL2->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEventL2->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_TIME2$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn = pEventL1->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME2$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn = pEventL2->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
		return sReturn;
	}
	if(strTail == "EVENT_TIME2_MS$")
	{
		if(pEventL1 == NULL)
			return "";
		sReturn.Format("%d", pEventL1->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "L2_EVENT_TIME2_MS$")
	{
		if(pEventL2 == NULL)
			return "";
		sReturn.Format("%d", pEventL2->m_nmsReceive);
		return sReturn;
	}
	if(strTail == "EVENT_SEC_NAME$")
	{
		if(pEventL1 == NULL)
			return "";
		if(pEventL1->m_pSec != NULL)
			return pEventL1->m_pSec->m_sName;
	}
	if(strTail == "L2_EVENT_SEC_NAME$")
	{
		if(pEventL2 == NULL)
			return "";
		if(pEventL2->m_pSec != NULL)
			return pEventL2->m_pSec->m_sName;
	}
	if(strTail == "EVENT_STATION_NAME$")
	{
		if(pEventL1 == NULL)
			return "";
		if(pEventL1->m_pSec != NULL)
			return pEventL1->m_pSec->m_pStation->m_sName;
	}
	if(strTail == "L2_EVENT_STATION_NAME$")
	{
		if(pEventL2 == NULL)
			return "";
		if(pEventL2->m_pSec != NULL)
			return pEventL2->m_pSec->m_pStation->m_sName;
	}
	if(strTail == "EVENT_DEF$")
	{
		if(pEventL1 == NULL)
			return "";
		return pEventL1->m_sEventDefName;
	}
	if(strTail == "L2_EVENT_DEF$")
	{
		if(pEventL2 == NULL)
			return "";
		return pEventL2->m_sEventDefName;
	}
	if(strTail == "EVENT_CONTENT$")
	{
		if(pEventL1 == NULL)
			return "";
		if(pEventL1->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征量
			int nEventDef = atoi(pEventL1->m_sEventDef);
			if(nEventDef == 2)
			{
				//故障类型
				CString strFaultType = pEventL1->GetFaultType(pEventL1->m_sEventContent);
				return strFaultType;
			}
			CString v = pEventL1->m_sEventContent;
			if( v.Find(".", 0) != -1 )
			{
				//认为是浮点数
				float f = atof(v);
				v.Format("%.2f", f);
			}
			CString str = v;
			if(pEventL1->m_strUnit != "" && pEventL1->m_sEventContent != "")
				str.Format("%s(%s)", v, pEventL1->m_strUnit);
			return str;
		}
		if(pEventL1->m_nType == XJ_FAULT_EVENT_SING || pEventL1->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//动作信号或告警
			if(pEventL1->m_sEventContent == "1")
			{
				return StringFromID(IDS_CASE_ACTION);
			}
			else if(pEventL1->m_sEventContent == "0")
			{
				return StringFromID(IDS_CASE_RETURN);
			}
		}
		if(pEventL1->m_nType == XJ_FAULT_EVENT_DICHANGE)
		{
			if(pEventL1->m_sEventContent == "1")
			{
				return "ON";
			}
			else if(pEventL1->m_sEventContent == "0")
			{
				return "OFF";
			}
		}
		return pEventL1->m_sEventContent;
	}
	if(strTail == "L2_EVENT_CONTENT$")
	{
		if(pEventL2 == NULL)
			return "";
		if(pEventL2->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征量
			int nEventDef = atoi(pEventL2->m_sEventDef);
			if(nEventDef == 2)
			{
				//故障类型
				CString strFaultType = pEventL2->GetFaultType(pEventL2->m_sEventContent);
				return strFaultType;
			}
			CString v = pEventL2->m_sEventContent;
			if( v.Find(".", 0) != -1 )
			{
				//认为是浮点数
				float f = atof(v);
				v.Format("%.2f", f);
			}
			CString str = v;
			if(pEventL2->m_strUnit != "" && pEventL2->m_sEventContent != "")
				str.Format("%s(%s)", v, pEventL2->m_strUnit);
			return str;
		}
		if(pEventL2->m_nType == XJ_FAULT_EVENT_SING || pEventL2->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//动作信号或告警
			if(pEventL2->m_sEventContent == "1")
			{
				return StringFromID(IDS_CASE_ACTION);
			}
			else if(pEventL2->m_sEventContent == "0")
			{
				return StringFromID(IDS_CASE_RETURN);
			}
		}
		if(pEventL2->m_nType == XJ_FAULT_EVENT_DICHANGE)
		{
			if(pEventL2->m_sEventContent == "1")
			{
				return "ON";
			}
			else if(pEventL2->m_sEventContent == "0")
			{
				return "OFF";
			}
		}
		return pEventL2->m_sEventContent;
	}
	if(strTail == "OSC_FILE_NAME$")
	{
		if(pEventL1 == NULL)
			return "";
		//录波简报才有
		if(pEventL1->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		//得到文件名
		CString str = pEventL1->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
			str.Delete(0, nRFind+1);
		return str;
	}
	if(strTail == "L2_OSC_FILE_NAME$")
	{
		if(pEventL2 == NULL)
			return "";
		//录波简报才有
		if(pEventL2->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		//得到文件名
		CString str = pEventL2->m_sEventDef;
		str.Replace("/", "\\");
		int nRFind = str.ReverseFind('\\');
		if(nRFind >= 0)
			str.Delete(0, nRFind+1);
		return str;
	}
	if(strTail == "OSC_FILE_TIME$")
	{
		if(pEventL1 == NULL)
			return "";
		//录波简报才有
		if(pEventL1->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		sReturn.Format("%s.%03d", pEventL1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEventL1->m_nmsStart);
		return sReturn;
	}
	if(strTail == "L2_OSC_FILE_TIME$")
	{
		if(pEventL2 == NULL)
			return "";
		//录波简报才有
		if(pEventL2->m_nType != XJ_FAULT_EVENT_OSCREPORT)
			return "";
		sReturn.Format("%s.%03d", pEventL2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEventL2->m_nmsStart);
		return sReturn;
	}
	if(strTail == "EVENT_RELETIVE_TIME$")
	{
		if(pEventL1 == NULL)
			return "";
		if(pEventFirstL1 == NULL)
			return "";
		//判断是否事件
		if(pEventL1->m_nType != XJ_FAULT_EVENT_CHR && pEventL1->m_nType != XJ_FAULT_EVENT_SING && pEventL1->m_nType != XJ_FAULT_EVENT_ALARM)
		{
			return "";
		}
		
		int nOrgMs = 0;
		CTime tmStartTime;
		if(g_FaultActionTime == 0)
		{
			nOrgMs = pEventFirstL1->m_nmsSign; //原始毫秒值
			tmStartTime = pEventFirstL1->m_tmSign;
		}
		else if(g_FaultActionTime == 1)
		{
			if((pEventL1->m_tmStart == pEventL1->m_tmSign) && (pEventL1->m_nmsStart == pEventL1->m_nmsSign))
			{
				//相对第一条事件
				nOrgMs = pEventFirstL1->m_nmsSign; //原始毫秒值
				tmStartTime = pEventFirstL1->m_tmSign;
			}
			else
			{
				nOrgMs = pEventL1->m_nmsStart;
				tmStartTime = pEventL1->m_tmStart;
			}
		}
		CTimeSpan tmSpan = pEventL1->m_tmSign - tmStartTime;
		LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
		//转为毫秒值
		LONG nMs = nSEC* 1000;
		LONG nCurMs = pEventL1->m_nmsSign + nMs;
		//计算差值
		LONG nDis = nCurMs - nOrgMs;
		sReturn.Format("%d ms", nDis);
		return sReturn;
	}
	if(strTail == "L2_EVENT_RELETIVE_TIME$")
	{
		if(pEventL2 == NULL)
			return "";
		if(pEventFirstL2 == NULL)
			return "";
		//判断是否事件
		if(pEventL2->m_nType != XJ_FAULT_EVENT_CHR && pEventL2->m_nType != XJ_FAULT_EVENT_SING && pEventL2->m_nType != XJ_FAULT_EVENT_ALARM)
		{
			return "";
		}
		
		int nOrgMs = 0;
		CTime tmStartTime;
		if(g_FaultActionTime == 0)
		{
			nOrgMs = pEventFirstL2->m_nmsSign; //原始毫秒值
			tmStartTime = pEventFirstL2->m_tmSign;
		}
		else if(g_FaultActionTime == 1)
		{
			if((pEventL2->m_tmStart == pEventL2->m_tmSign) && (pEventL2->m_nmsStart == pEventL2->m_nmsSign))
			{
				//相对第一条事件
				nOrgMs = pEventFirstL2->m_nmsSign; //原始毫秒值
				tmStartTime = pEventFirstL2->m_tmSign;
			}
			else
			{
				nOrgMs = pEventL2->m_nmsStart;
				tmStartTime = pEventL2->m_tmStart;
			}
		}
		CTimeSpan tmSpan = pEventL2->m_tmSign - tmStartTime;
		LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
		//转为毫秒值
		LONG nMs = nSEC* 1000;
		LONG nCurMs = pEventL2->m_nmsSign + nMs;
		//计算差值
		LONG nDis = nCurMs - nOrgMs;
		sReturn.Format("%d ms", nDis);
		return sReturn;
	}
	
	return "";
}

CString CViewFaultDetail::GetValue_getcount_Landscape(CString sKey)
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找是否有[
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
	{
		//由单个关键字组成
		if(sKey == "$DEVICE_LIST.GetCount()$")
		{
			sReturn.Format("%d", max( m_pReport->m_listSecStation1.GetSize(), m_pReport->m_listSecStation2.GetSize()) );
			return sReturn;
		}
		
		if(sKey == "$FAULT_WR_OSC_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWROSC.GetSize());
			return sReturn;
		}
		if(sKey == "$WAVEDISTANCE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listWaveDistance.GetSize());
			return sReturn;
		}
		if(sKey == "$DISTANCE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listDistance.GetSize());
			return sReturn;
		}
		if(sKey == "$OSCPARSE_LIST.GetCount()$")
		{
			sReturn.Format("%d", m_pReport->m_listOSCParse.GetSize());
			return sReturn;
		}
	}
	else
	{
		//得到头
		CString strHead = sKey.Left(nFind);
		if(strHead == "$DEVICE_LIST")
		{
			//得到索引
			int nFind1 = sKey.Find(']', nFind);
			if(nFind1 == -1)
				return "";
			CString sIndex = sKey.Mid(nFind+1, nFind1 - nFind -1);
			int nIndex = atoi(sIndex);
			//得到索引对应的设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return "";
			CSecObj* pSecStation1 = NULL;
			CSecObj* pSecStation2 = NULL;
			if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation1.GetSize())
				pSecStation1 = (CSecObj*)m_pReport->m_listSecStation1.GetAt(nIndex);
			if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation2.GetSize())
				pSecStation2 = (CSecObj*)m_pReport->m_listSecStation2.GetAt(nIndex);
			if(pSecStation1 == NULL && pSecStation2 == NULL)
				return "";
			//建立零时事件列表
			if(pSecStation1 != m_pReport->m_pTempSecStation1 || pSecStation2 != m_pReport->m_pTempSecStation2)
			{
				//组建零时数组
				m_pReport->BuildTempListLandscape(pSecStation1, pSecStation2);
			}
			
			//得到后段的关键字 // $DEVICE_LIST[1]::FAULT_PT_OSC_LIST.GetCount()$
			nFind = sKey.ReverseFind(':');
			if(nFind == -1)
				return "";
			CString strTail = sKey.Right(sKey.GetLength() - nFind - 1);
			if(strTail == "FAULT_ACTION_CHR_LIST.GetCount()$")
			{
				sReturn.Format("%d", max( m_pReport->m_listTempChrStation1.GetSize(), m_pReport->m_listTempChrStation2.GetSize()) );
				return sReturn;
			}
			if(strTail == "FAULT_ACTION_SIGN_LIST.GetCount()$")
			{
				sReturn.Format("%d", max( m_pReport->m_listTempSignStation1.GetSize(), m_pReport->m_listTempSignStation2.GetSize()) );
				return sReturn;
			}
			if(strTail == "FAULT_PT_OSC_LIST.GetCount()$")
			{
				sReturn.Format("%d", max( m_pReport->m_listTempPTOSCStation1.GetSize(), m_pReport->m_listTempPTOSCStation2.GetSize()) );
				return sReturn;
			}
			if(strTail == "DICHANGE_LIST.GetCount()$")
			{
				sReturn.Format("%d", max( m_pReport->m_listTempDIChangeStation1.GetSize(), m_pReport->m_listTempDIChangeStation2.GetSize()) );
				return sReturn;
			}
		}
	}
	return sReturn;
}

CString CViewFaultDetail::GetValue_OneLayer_Landscape(CString sKey)
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pReport == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//查找[]
	nFind = sKey.Find('[', 0);
	if(nFind == -1)
		return "";
	int nFind2 = sKey.Find(']', nFind);
	if(nFind2 == -1)
		return "";
	//得到索引
	CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
	int nIndex = atoi(strIndex);
	if(nIndex < 0)
		return "";
	//得到头,尾
	CString strHead = sKey.Left(nFind);
	CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
	if(strHead == "$DEVICE_LIST")
	{
		//设备列表, 取得设备
		if(m_pReport->m_listSec.GetSize() < 1)
			return "";
		CSecObj* pSecStation1 = NULL;
		CSecObj* pSecStation2 = NULL;
		if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation1.GetSize())
			pSecStation1 = (CSecObj*)m_pReport->m_listSecStation1.GetAt(nIndex);
		if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation2.GetSize())
			pSecStation2 = (CSecObj*)m_pReport->m_listSecStation2.GetAt(nIndex);
		if(strTail == "DEVICE_NAME$")
		{
			if(pSecStation1 == NULL)
				return "";
			return pSecStation1->m_sName;
		}
		if(strTail == "L2_DEVICE_NAME$")
		{
			if(pSecStation2 == NULL)
				return "";
			return pSecStation2->m_sName;
		}
		if(strTail == "DEVICE_STATION$")
		{
			if(pSecStation1 == NULL)
				return "";
			if(pSecStation1->m_pStation != NULL)
				return pSecStation1->m_pStation->m_sName;
			return "";
		}
		if(strTail == "L2_DEVICE_STATION$")
		{
			if(pSecStation2 == NULL)
				return "";
			if(pSecStation2->m_pStation != NULL)
				return pSecStation2->m_pStation->m_sName;
			return "";
		}
		if(strTail == "DEVICE_OWNER$")
		{
			if(pSecStation1 == NULL)
				return "";
			if(pSecStation1->m_pOwner != NULL)
				return pSecStation1->m_pOwner->m_sName;
			return "";
		}
		if(strTail == "L2_DEVICE_OWNER$")
		{
			if(pSecStation2 == NULL)
				return "";
			if(pSecStation2->m_pOwner != NULL)
				return pSecStation2->m_pOwner->m_sName;
			return "";
		}
		if(strTail == "EVENT_TIME1_FULL$")
		{
			//得到第一个动作信息的时间
			//建立零时事件列表
			if(pSecStation1 != m_pReport->m_pTempSecStation1 || pSecStation2 != m_pReport->m_pTempSecStation2)
			{
				//组建零时数组
				m_pReport->BuildTempListLandscape(pSecStation1, pSecStation2);
			}
			if(m_pReport->m_listTempSignStation1.GetSize() < 1)
				return "";
			CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listTempSignStation1.GetAt(0);
			if(pEvent == NULL)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "L2_EVENT_TIME1_FULL$")
		{
			//得到第一个动作信息的时间
			//建立零时事件列表
			if(pSecStation1 != m_pReport->m_pTempSecStation1 || pSecStation2 != m_pReport->m_pTempSecStation2)
			{
				//组建零时数组
				m_pReport->BuildTempListLandscape(pSecStation1, pSecStation2);
			}
			if(m_pReport->m_listTempSignStation2.GetSize() < 1)
				return "";
			CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listTempSignStation2.GetAt(0);
			if(pEvent == NULL)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "FAULT_DISTANCE$")
		{
			if(pSecStation1 == NULL)
				return "-";
			CString sDistance = "";
			
			sDistance = m_pReport->GetPTFaultDistance(pSecStation1->m_sID);
			if( sDistance != "")
				sDistance.Format("%s[%s] %s", StringFromID(IDS_COMMON_AWAYFROM),pSecStation1->m_pStation->m_sName, m_pReport->GetPTFaultDistance(pSecStation1->m_sID));
			
			sDistance.TrimLeft();
			sDistance.TrimRight();
			if( sDistance == "" )
				return "-";
			return (sDistance+" km");
		}
		if(strTail == "L2_FAULT_DISTANCE$")
		{
			if(pSecStation2 == NULL)
				return "-";
			CString sDistance = "";
			
			sDistance = m_pReport->GetPTFaultDistance(pSecStation2->m_sID);
			if( sDistance != "")
				sDistance.Format("%s[%s] %s", StringFromID(IDS_COMMON_AWAYFROM),pSecStation2->m_pStation->m_sName, m_pReport->GetPTFaultDistance(pSecStation2->m_sID));
			
			sDistance.TrimLeft();
			sDistance.TrimRight();
			if( sDistance == "" )
				return "-";
			return (sDistance+" km");
		}
	}
	else
	{
		CFaultEvent* pEvent = NULL;
		if(strHead == "$FAULT_ACTION_CHR_LIST")
		{
			//动作特征值
			if(nIndex < 0 || nIndex >=m_pReport->m_listChr.GetSize())
			{
				return "";
			}

			pEvent = (CFaultEvent*)m_pReport->m_listChr[nIndex];
		}
		if(strHead == "$FAULT_ACTION_SIGN_LIST")
		{
			//动作信号
			if(nIndex < 0 || nIndex >=m_pReport->m_listSign.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listSign[nIndex];
		}
		if(strHead == "$FAULT_ALARM_LIST")
		{
			//告警
			if(nIndex < 0 || nIndex >=m_pReport->m_listAlarm.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listAlarm[nIndex];
		}
		if(strHead == "$FAULT_PT_OSC_LIST")
		{
			//保护录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listPTOSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listPTOSC[nIndex];
		}
		if(strHead == "$FAULT_WR_OSC_LIST")
		{
			//录波器录波
			if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
			{
				return "";
			}
			pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
		}
		if(pEvent == NULL)
			return "";
		if(strTail == "EVENT_TIME1_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME1$")
		{
			sReturn = pEvent->m_tmStart.Format("%Y-%m-%d %H:时%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME1_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_FULL$")
		{
			sReturn.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_TIME2$")
		{
			sReturn = pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S");
			return sReturn;
		}
		if(strTail == "EVENT_TIME2_MS$")
		{
			sReturn.Format("%d", pEvent->m_nmsReceive);
			return sReturn;
		}
		if(strTail == "EVENT_SEC_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_sName;
		}
		if(strTail == "EVENT_STATION_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_pStation->m_sName;
		}
		if(strTail == "EVENT_OSC_TIME$")
		{
			//录波分析-故障时间
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_DURATIV$")
		{
			//录波分析-故障持续时间
			sReturn.Format("%s ms", pEvent->m_sEventContent);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_DISTANCE$")
		{
			//录波分析-测距值
			sReturn.Format("%s km", pEvent->m_sReceiveTime);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_PHASE$")
		{
			//录波分析-故障相别
			return pEvent->m_sSignaltime;
		}
		if(strTail == "EVENT_OSC_TRIGGER$")
		{
			//录波分析-跳闸相别
			return pEvent->m_sWaveDescribe;
		}
		if(strTail == "EVENT_OSC_RECLOSE$")
		{
			//录波分析-重合闸时间
			if(pEvent->m_nWaveFileNo <= 0)
				sReturn =  StringFromID(IDS_RECLOSE_NOTCLOSE);
			else
				sReturn.Format("%d ms", pEvent->m_nWaveFileNo);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_ELECTRIC$")
		{
			//录波分析-最大电流
			sReturn.Format("%s A", pEvent->m_sReverse2);
			return sReturn;
		}
		if(strTail == "EVENT_OSC_ELECTRICPHASE$")
		{
			//录波分析-最大电流相别
			return pEvent->m_sReverse3;
		}

		if(strTail == "EVENT_DEF$")
		{
			return pEvent->m_sEventDefName;
		}
		if(strTail == "EVENT_CONTENT$")
		{
			if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
			{
				//特征量
				int nEventDef = atoi(pEvent->m_sEventDef);
				if(nEventDef == 2)
				{
					//故障类型
					CString strFaultType = pEvent->GetFaultType(pEvent->m_sEventContent);
					return strFaultType;
				}
				CString v = pEvent->m_sEventContent;
				if( v.Find(".", 0) != -1 )
				{
					//认为是浮点数
					float f = atof(v);
					v.Format("%.2f", f);
				}
				CString str = v;
				if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
					str.Format("%s(%s)", v, pEvent->m_strUnit);
				return str;
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
			{
				//动作信号或告警
				if(pEvent->m_sEventContent == "1")
				{
					return StringFromID(IDS_CASE_ACTION);
				}
				else if(pEvent->m_sEventContent == "0")
				{
					return StringFromID(IDS_CASE_RETURN);
				}
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
			{
				if(pEvent->m_sEventContent == "1")
				{
					return "ON";
				}
				else if(pEvent->m_sEventContent == "0")
				{
					return "OFF";
				}
			}
			if(pEvent->m_nType == XJ_FAULT_EVENT_WAVEDISTANCE)
			{
				CString str;
				str.Format("%s km", pEvent->m_sSignaltime);
				return str;
			}
			return pEvent->m_sEventContent;
		}
		if(strTail == "OSC_FILE_NAME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			CString str = pEvent->m_sEventDef;
			//得到文件名
			str.Replace("/", "\\");
			int nRFind = str.ReverseFind('\\');
			if(nRFind >= 0)
				str.Delete(0, nRFind+1);
			return str;
		}
		if(strTail == "OSC_FILE_TIME$")
		{
			//录波简报才有
			if(pEvent->m_nType != XJ_FAULT_EVENT_OSCREPORT)
				return "";
			sReturn.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
			return sReturn;
		}
		if(strTail == "EVENT_RELETIVE_TIME$")
		{
			//判断是否事件
			if(pEvent->m_nType != XJ_FAULT_EVENT_CHR && pEvent->m_nType != XJ_FAULT_EVENT_SING && pEvent->m_nType != XJ_FAULT_EVENT_ALARM)
			{
				return "";
			}
			int nOrgMs = 0;
			CTime tmStartTime;
			if(g_FaultActionTime == 0)
			{
				nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
				tmStartTime = m_pReport->m_tmFaultTime;
			}
			else if(g_FaultActionTime == 1)
			{
				if((pEvent->m_tmStart == pEvent->m_tmSign) && (pEvent->m_nmsStart == pEvent->m_nmsSign))
				{
					nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
					tmStartTime = m_pReport->m_tmFaultTime;
				}
				else
				{
					nOrgMs = pEvent->m_nmsStart;
					tmStartTime = pEvent->m_tmStart;
				}
			}
			//int nOrgMs = m_pReport->m_nmsFaultTime; //原始毫秒值
			CTimeSpan tmSpan = pEvent->m_tmSign - tmStartTime;
			LONG nSEC = tmSpan.GetTotalSeconds(); //相差秒数
			//转为毫秒值
			LONG nMs = nSEC* 1000;
			LONG nCurMs = pEvent->m_nmsSign + nMs;
			//计算差值
			LONG nDis = nCurMs - nOrgMs;
			sReturn.Format("%d ms", nDis);
			return sReturn;
		}
	}
	return "";
}

int CViewFaultDetail::CheckLandscape()
{
	int nReturn = 0;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//得到系统下载路径
	CString strDir = pApp->GetDownDir();
	//组建全路径
	if(g_UseColorFaultReport)
		strDir += FAULTREPORT_MULTI_COLOR_LANDSCAPE;
	else
		strDir += FAULTREPORT_MULTI_PT_LANDSCAPE;
	CFileFind ff;
	return ff.FindFile( strDir);
}

CFaultEvent* CViewFaultDetail::GetEventBySignleKey_Landscape(CString sKey, CString & sTail)
{
	//判断数据有效性
	if(sKey == "")
		return NULL;
	if(m_pReport == NULL)
		return NULL;
	sTail = "";
	CFaultEvent* pEvent = NULL;
	int nFind = -1;
	//判断是否带有[ ]运算符
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//带有[]运算符
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//没找到结束括号
			return NULL;
		}
		int nFind3 = sKey.Find('[', nFind2);
		if(nFind3 != -1)
		{
			//有两层
			int nFind4 = sKey.Find(']', nFind3);
			if(nFind4 == -1)
				return NULL;
			//得到索引
			CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
			int nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			//得到头
			CString strHead = sKey.Left(nFind);
			if(strHead != "$DEVICE_LIST")
				return NULL;
			//设备列表, 取得设备
			if(m_pReport->m_listSec.GetSize() < 1)
				return NULL;
			CSecObj* pSecStation1 = NULL;
			CSecObj* pSecStation2 = NULL;
			if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation1.GetSize())
				pSecStation1 = (CSecObj*)m_pReport->m_listSecStation1.GetAt(nIndex);
			if(nIndex >= 0 && nIndex < m_pReport->m_listSecStation2.GetSize())
				pSecStation2 = (CSecObj*)m_pReport->m_listSecStation2.GetAt(nIndex);
			if(pSecStation1 == NULL && pSecStation2 == NULL)
				return NULL;
			//建立零时事件列表
			if(pSecStation1 != m_pReport->m_pTempSecStation1 || pSecStation2 != m_pReport->m_pTempSecStation2)
			{
				//组建零时数组
				m_pReport->BuildTempListLandscape(pSecStation1, pSecStation2);
			}
			
			//得到索引
			strIndex = sKey.Mid(nFind3+1, nFind4 - nFind3 -1);
			nIndex = atoi(strIndex);
			if(nIndex < 0)
				return NULL;
			CString strMid = sKey.Mid(nFind2+3, nFind3 - nFind2 - 3);

			//取得尾部
			CString str = sKey.Right(sKey.GetLength() - (nFind4 + 3));
			sTail = "$";
			sTail += str;

			if(strMid == "FAULT_PT_OSC_LIST" && sTail == "$OSC_FILE_NAME$")
			{
				//保护录波
				if(nIndex >= 0 && nIndex < m_pReport->m_listTempPTOSCStation1.GetSize())
				{
					pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSCStation1[nIndex];
				}
							
			}
			else if(strMid == "FAULT_PT_OSC_LIST" && sTail == "$L2_OSC_FILE_NAME$")
			{
				if(nIndex >= 0 && nIndex < m_pReport->m_listTempPTOSCStation2.GetSize())
				{
					pEvent = (CFaultEvent*)m_pReport->m_listTempPTOSCStation2[nIndex];
				}	
			}
		}
		else
		{
			//取得索引号
			CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
			int nIndex = atoi(strIndex);
			//取得头部
			CString strHead = sKey.Left(nFind);
			//取得事件
			if(strHead == "$FAULT_WR_OSC_LIST")
			{
				//录波器录波
				if(nIndex < 0 || nIndex >=m_pReport->m_listWROSC.GetSize())
				{
					return NULL;
				}
				pEvent = (CFaultEvent*)m_pReport->m_listWROSC[nIndex];
			}
			//取得尾部字符
			sTail = "$";
			sTail += sKey.Right(sKey.GetLength() - (nFind2 + 3));
		}
	}
	return pEvent;
}
