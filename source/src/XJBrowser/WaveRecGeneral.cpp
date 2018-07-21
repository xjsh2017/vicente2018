// WaveRecGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecGeneral.h"
#include "DlgStatusHis.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

UINT WRGeneralLoad(LPVOID pParam)
{
	CWaveRecGeneral* wrGeneral = (CWaveRecGeneral*)pParam;
	if(wrGeneral == NULL)
		return 0;
	//读取内容
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//碰到退出线程,退出
		if(wrGeneral->m_bExitThread)
			return 0;
		//超过重试次数
		if(nCount < 0)
			return 0;
		//读取完成, 退出
		if(bLoad)
		{
			//log
			WriteLog("录波器属性--基本属性页面, 读取内容结束, 正常退出线程!", XJ_LOG_LV3);
			return 0;
		}
		if(wrGeneral->ReFillAll())
		{
			wrGeneral->PostMessage(WM_WINDOW_REFRESH, 0, 0);
			bLoad = TRUE;
		}
		else
		{
			//读取失败
			CString str;
			str.Format("WRGeneral, 失败%d次", (4 - nCount));
			WriteLog(str, XJ_LOG_LV3);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral

IMPLEMENT_DYNCREATE(CWaveRecGeneral, CViewBase)

//##ModelId=49B87B800291
CWaveRecGeneral::CWaveRecGeneral()
	: CViewBase(CWaveRecGeneral::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecGeneral)
	m_strCommStatus = _T("");
	m_strRunStatus = _T("");
	m_strType = _T("");
	m_strVer = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_bMoreInfo = FALSE;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_bGetStatus = FALSE;
	m_hIcon_open= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_GREEN_LIGHT));
	m_hIcon_close= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_RED_LIGHT));
}

//##ModelId=49B87B80034E
CWaveRecGeneral::~CWaveRecGeneral()
{
}

//##ModelId=49B87B80034B
void CWaveRecGeneral::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecGeneral)
	DDX_Control(pDX, IDC_STAT_LIGHT, m_iconStatLight);
	DDX_Control(pDX, IDC_COMM_LIGHT, m_iconCommLight);
	DDX_Control(pDX, IDC_BTN_WRGEN_CALL, m_btnCall);
	DDX_Control(pDX, IDC_BTN_WRGEN_COMMHIS, m_btnCommHis);
	DDX_Control(pDX, IDC_BTN_WRGEN_RUNHIS, m_btnRunHis);
	DDX_Control(pDX, IDC_GIF_WRGEN, m_gif);
	DDX_Control(pDX, IDC_BTN_WRGEN_TIME, m_btnTime);
	DDX_Control(pDX, IDC_BTN_WRGEN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTN_WRGEN_REL, m_btnRel);
	DDX_Control(pDX, IDC_BTN_WRGEN_MORE, m_btnMore);
	DDX_Text(pDX, IDC_EDIT_WRGEN_COMM, m_strCommStatus);
	DDX_Text(pDX, IDC_EDIT_WRGEN_RUN, m_strRunStatus);
	DDX_Text(pDX, IDC_EDIT_WRGEN_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_WRGEN_VER, m_strVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecGeneral, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecGeneral)
	ON_BN_CLICKED(IDC_BTN_WRGEN_MORE, OnBtnWrgenMore)
	ON_BN_CLICKED(IDC_BTN_WRGEN_TIME, OnBtnWrgenTime)
	ON_BN_CLICKED(IDC_BTN_WRGEN_START, OnBtnWrgenStart)
	ON_CBN_SELCHANGE(IDC_CMB_WRGEN_CPU, OnSelchangeCmbWrgenCpu)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_WRGEN_COMMHIS, OnBtnWrgenCommhis)
	ON_BN_CLICKED(IDC_BTN_WRGEN_RUNHIS, OnBtnWrgenRunhis)
	ON_BN_CLICKED(IDC_BTN_WRGEN_CALL, OnBtnWrgenCall)
	ON_WM_CTLCOLOR()
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_CALL, OnDoubleclickedBtnWrgenCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_START, OnDoubleclickedBtnWrgenStart)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_TIME, OnDoubleclickedBtnWrgenTime)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20124, OnSTTP20124)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20086, OnSTTP20086)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
	ON_MESSAGE(WM_WINDOW_REFRESH, OnWindowRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral diagnostics

#ifdef _DEBUG
//##ModelId=49B87B80035B
void CWaveRecGeneral::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B80035D
void CWaveRecGeneral::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral message handlers

//##ModelId=49B87B800340
void CWaveRecGeneral::OnInitialUpdate() 
{
	//记录控件位置
	RecordRate(IDC_WRGEN_SP, 1, left_client, top_client);
	RecordRate(IDC_WRGEN_SP2, 1, left_client, top_client);
	
	//左边
	RecordRate(IDC_COMM_LIGHT, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRGEN_COMM, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_COMM, 1, left_client, top_client);

	RecordRate(IDC_BTN_WRGEN_CALL, 0, IDC_EDIT_WRGEN_COMM, top_client);
	RecordRate(IDC_BTN_WRGEN_COMMHIS, 0, IDC_BTN_WRGEN_CALL, top_client);
	
	RecordRate(IDC_STATIC_WRGEN_VER, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_VER, 1, left_client, top_client);
	
	RecordRate(IDC_STATIC_WRGEN_TYPE, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_TYPE, 1, left_client, top_client);
	
	
	//右边
	RecordRate(IDC_STAT_LIGHT, 0, mid_client, top_client);
	RecordRate(IDC_STATIC_WRGEN_RUN, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_RUN, 1, mid_client, top_client);

	RecordRate(IDC_BTN_WRGEN_RUNHIS, 0, IDC_EDIT_WRGEN_RUN, top_client);
		
	//按钮
	RecordRate(IDC_BTN_WRGEN_MORE, 0, null_effect, null_effect, right_client, bottom_client);
	RecordRate(IDC_BTN_WRGEN_REL, 0, left_client, top_client);

	//远方控制组
	RecordRate(IDC_CMB_WRGEN_CPU, 0, left_client,top_client);
	RecordRate(IDC_BTN_WRGEN_START, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRGEN_TIME, 0, left_client, top_client);
	RecordRate(IDC_GIF_WRGEN, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_WRGEN, 0, left_client, top_client);
	
	CViewBase::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();

	//初始化控件
	InitControls();
	//隐藏某些信息
	HideSomeInfo(TRUE);
	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
	RefreshControls();
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件, 设置初始值, 显示风格
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B80030E
int CWaveRecGeneral::InitControls()
{
	/*
	//查看更多按钮
	m_btnMore.SetThemeHelper(&m_ThemeHelper);
//	m_btnMore.SetIcon(IDI_SEARCH);
	m_btnMore.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnMore.SetTooltipText(_T("查看更多相关信息"));

	//远方启动
	m_btnStart.SetThemeHelper(&m_ThemeHelper);
//	m_btnStart.SetIcon(IDI_CONTROL);
	m_btnStart.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStart.SetTooltipText(_T("控制远方装置启动"));

	//查看关联设备按钮
	m_btnRel.SetThemeHelper(&m_ThemeHelper);
//	m_btnRel.SetIcon(IDI_SEARCH);
	m_btnRel.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRel.SetTooltipText(_T("查看关联设备属性"));
	
	//强制对时
	m_btnTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnTime.SetIcon(IDI_SEARCH);
	m_btnTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnTime.SetTooltipText(_T("强制装置对时"));

	//通讯状态历史
	m_btnCommHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommHis.SetTooltipText(_T("查看历史"));

	//运行状态历史
	m_btnRunHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnRunHis.SetIcon(IDI_HISTORY);
	m_btnRunHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRunHis.SetTooltipText(_T("查看历史"));

	//召唤通讯状态
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
	//	m_btnCall.SetIcon(IDI_HISTORY);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤"));
	*/

	/*
	LONG style = GetWindowLong(m_iconCommLight.GetSafeHwnd(), GWL_STYLE);
	style |= SS_ICON;   
	style |= SS_CENTERIMAGE;    
	SetWindowLong(m_iconCommLight.GetSafeHwnd(),GWL_STYLE,style); 
    SetWindowLong(m_iconStatLight.GetSafeHwnd(),GWL_STYLE,style); 
*/

	return 0;
}

/*************************************************************
 函 数 名：OnWRFrameOpen()
 功能概要：响应录波器属性页面打开消息, 刷新界面显示
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B80038D
void CWaveRecGeneral::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = pObj;

	int nIndex = (int)wParam;

	//退出旧线程
// 	EndOwnerThread();
// 
// 	//启动读取线程
// 	if(m_pLoadThread == NULL)
// 	{
// 		m_bExitThread = FALSE;
// 		m_pLoadThread = AfxBeginThread(WRGeneralLoad, this, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
// 		if(m_pLoadThread != NULL)
// 		{
// 			m_pLoadThread->m_bAutoDelete = FALSE;
// 			m_pLoadThread->ResumeThread();
// 		}
// 	}

// 	else
// 	{
// 		//刷新
 		RefreshControls();
		//第一次打开时第一页的控件刷新有问题, 原因未明, 现强制刷新一次
		RegulateControlSize();
// 	}
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_WRGEN, "");
}

/*************************************************************
 函 数 名：RefreshControls()
 功能概要：刷新控件值
 返 回 值: void
**************************************************************/
//##ModelId=49B87B80031C
void CWaveRecGeneral::RefreshControls()
{
	//检查数据有效性
	if(m_pObj == NULL)
		return;
	m_strType = m_pObj->m_sModel; //型号
	m_strVer = m_pObj->m_sSysVer; //版本号
	
	//通讯状态
	m_strCommStatus = GetCommStatusString(m_pObj->m_nCommStatus);
	
	//运行状态
	m_strRunStatus = GetRunStatusString(m_pObj->m_nRunStatu);

/*
	if (m_pObj->m_nCommStatus == 0)
	{
		m_iconCommLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconCommLight.SetIcon(m_hIcon_close);
	}
	
	//运行状态
	if (m_pObj->m_nRunStatu == 2)
	{
		m_iconStatLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconStatLight.SetIcon(m_hIcon_close);
	}
	*/

	FillCPU();

	UpdateData(FALSE);
}

//##ModelId=49B87B80036C
void CWaveRecGeneral::OnBtnWrgenMore() 
{
	// TODO: Add your control notification handler code here
	if(m_bMoreInfo)
	{
		//目前正在显示更多信息,隐藏这些信息
		SetDlgItemText(IDC_BTN_WRGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	else
	{
		//目前没有显示更多信息, 显示这些信息
		SetDlgItemText(IDC_BTN_WRGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	HideSomeInfo(m_bMoreInfo);
	m_bMoreInfo = !m_bMoreInfo;
}

/*************************************************************
 函 数 名：HideSomeInfo()
 功能概要：隐藏/显示某些信息
 返 回 值: void
 参    数：param1	指定是否隐藏
**************************************************************/
//##ModelId=49B87B80031D
void CWaveRecGeneral::HideSomeInfo( BOOL bHide )
{
	UINT nCode;
	if(bHide)
	{
		nCode= SW_HIDE;
	}
	else
	{
		nCode = SW_SHOW;
	}
	CWnd * pWnd = NULL;
	//设备版本号
	pWnd = GetDlgItem(IDC_STATIC_WRGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_WRGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	if(g_PTControl == 1 && nCode == SW_SHOW)
		nCode = SW_SHOW;
	else
		nCode = SW_HIDE;
	//远方控制组
	pWnd = GetDlgItem(IDC_WRGEN_SP2);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_STATIC_WRGEN_CPU);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	m_btnStart.ShowWindow(nCode);
	m_btnTime.ShowWindow(nCode);
	m_gif.ShowWindow(nCode);
}

//##ModelId=49B87B80037A
void CWaveRecGeneral::OnBtnWrgenTime() 
{
	// TODO: Add your control notification handler code here
	//按下强制对时按钮, 强制对时或取消
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20059报文
		if(!pApp->m_SttpEngine.BuildDataFor20059PTSetTime(sttpData, m_pObj->m_sID, m_nCPU))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("录波器%s发送强制对时报文失败,原因为连接中断", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("录波器%s发送强制对时报文失败,原因为缓存已满", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变状态和按钮文字
			m_nCurrentStatus = 1;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nVerifyTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//正在召唤状态, 取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据页面当前状态, 改变控件可用度
 返 回 值: void
**************************************************************/
//##ModelId=49B87B80032C
void CWaveRecGeneral::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		SetDlgItemText(IDC_BTN_WRGEN_TIME, StringFromID(IDS_BTN_FORCE_SYNCTIME));
		SetDlgItemText(IDC_BTN_WRGEN_START, StringFromID(IDS_BTN_REMOTESTART));
		SetDlgItemText(IDC_BTN_WRGEN_CALL, StringFromID(IDS_BTN_CALL));
		//空闲状态,都可用
		GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_MORE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_WRGEN_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(TRUE);
		//停止动画
		m_gif.Stop();
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 6)
	{
		//召唤或操作状态,除了自己其它的都不可用
		
		GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_MORE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_WRGEN_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(FALSE);
		//开始动画
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_WRGEN, str);
		
		//让自己可用
		switch(m_nCurrentStatus)
		{
		case 1: //强制对时
			SetDlgItemText(IDC_BTN_WRGEN_TIME, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(TRUE);
			break;
		case 2: //远方启动
			SetDlgItemText(IDC_BTN_WRGEN_START, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(TRUE);
			break;
		case 5: //通讯状态召唤
			SetDlgItemText(IDC_BTN_WRGEN_CALL, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(TRUE);
		default:
			break;
		}
	}
}

//##ModelId=49B87B80037C
void CWaveRecGeneral::OnBtnWrgenStart() 
{
	// TODO: Add your control notification handler code here
	//按下远方启动按钮, 执行远方启动或取消
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20123报文
		if(!pApp->m_SttpEngine.BuildDataFor20123WaveRecRun(sttpData, m_pObj->m_sID, m_nCPU))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("录波器%s发送远方启动报文失败,原因为连接中断", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("录波器%s发送远方启动报文失败,原因为缓存已满", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变状态和按钮文字
			m_nCurrentStatus = 2;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nRemoteStartTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤状态, 取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 函 数 名：FillCPU()
 功能概要：填充CPU下拉框
 返 回 值: void
**************************************************************/
//##ModelId=49B87B80032D
void CWaveRecGeneral::FillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pCmb == NULL)
		return;
	//清空
	pCmb->ResetContent();
	
	if(m_pObj == NULL)
		return;
	
	//CPU下拉框, 填充内容为"CPU号"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU对象
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d", pCpu->code); //cpu号
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//设置默认选择
	if(pCmb->GetCount() > 0)
	{
		pCmb->SetCurSel(0);
		//保存当前选择
		int nIndex = pCmb->GetCurSel();
		CString sCPU;
		pCmb->GetLBText(nIndex, sCPU);
		m_nCPU = atoi(sCPU);
		return;
	}
}

//##ModelId=49B87B80037E
void CWaveRecGeneral::OnSelchangeCmbWrgenCpu() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pCmb == NULL)
		return;
	//保存当前选择
	int nIndex = pCmb->GetCurSel();
	CString sCPU;
	pCmb->GetLBText(nIndex, sCPU);
		m_nCPU = atoi(sCPU);
}

/*************************************************************
 函 数 名：OnSTTP20060()
 功能概要：响应装置对时通知(20060)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B800399
void CWaveRecGeneral::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于对时的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20060报文
	if(pSttpData->sttp_head.uMsgID != 20060)
	{
		//错误报文
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20060 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20060 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU号是否对应
	int nCPU = pSttpData->sttp_body.nCpu;
	if(nCPU != m_nCPU)
	{
		//报文错误
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20060 收到其它板卡的报文, 板卡号为%d", nCPU);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nReturn = pSttpData->sttp_body.nStatus;

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;	
	//更改控件可用性
	UpdateControlsEnable();
	if(nReturn == 0)
	{
		//成功
		AfxMessageBox(StringFromID(IDS_SYNCTIME_SUCCESS));
	}
	else
	{
		//失败 
		AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
	}
	
}

/*************************************************************
 函 数 名：OnSTTP20124()
 功能概要：录波器远方触发通知
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B80039C
void CWaveRecGeneral::OnSTTP20124( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//没有处于远方启动的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20060报文
	if(pSttpData->sttp_head.uMsgID != 20124)
	{
		//错误报文
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU号是否对应
	int nCPU = pSttpData->sttp_body.nCpu;
	if(nCPU != m_nCPU)
	{
		//报文错误
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 收到其它板卡的报文, 板卡号为%d", nCPU);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,XJ_OPER_RemoteStart,%d", pSttpData->sttp_body.nStatus, pSttpData->sttp_body.nCpu);

	pApp->AddNewManOperator(FUNC_OPT_RUNWAVEREC, m_pObj->m_sID, strMsg, XJ_OPER_RemoteStart, pSttpData->sttp_body.nStatus);

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nReturn = pSttpData->sttp_body.nStatus;

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;	
	//更改控件可用性
	UpdateControlsEnable();
	if(nReturn == 1)
	{
		//成功
		AfxMessageBox(StringFromID(IDS_REMOTESTART_SUCCESS));
	}
	else
	{
		//失败 
		AfxMessageBox(StringFromID(IDS_REMOTESTART_FAIL));
	}
}

//##ModelId=49B87B8003AB
void CWaveRecGeneral::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 5)
	{
		//没有处于操作状态, 不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		m_nCurrentStatus = 0;
			
		//更改控件可用度
		UpdateControlsEnable();
	}
	else
	{
		//操作失败
		//处理完毕,改变状态和按钮文字
		if(m_nCurrentStatus == 1)
		{
			//正在强制对时

			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
				
			CString str;
			str.Format("录波器[%s]强制对时失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//正在远方启动

			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
				
			CString str;
			str.Format("录波器[%s]远方启动失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			AfxMessageBox(StringFromID(IDS_REMOTESTART_FAIL));
		}	
	}
}

//##ModelId=49B87B8003B9
void CWaveRecGeneral::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 5)
	{
		//没有处于召唤开关量的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20125报文
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//处理完毕,改变状态和按钮文字
	if(m_nCurrentStatus == 1)
	{
		//正在强制对时
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤时间
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}

	
}

//##ModelId=49B87B80038A
void CWaveRecGeneral::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		//提示
		CString str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		
		//改变状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(str);
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_WRGEN, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：OnWRFrameClose()
 功能概要：响应窗口关闭消息, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8003C8
void CWaveRecGeneral::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：OnWindowRefresh()
 功能概要：响应窗口刷新消息, 更新控件值
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8003CB
void CWaveRecGeneral::OnWindowRefresh( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B80033F
BOOL CWaveRecGeneral::ReFillAll()
{
	//检查数据有效性
	if(m_pObj == NULL)
		return FALSE;
	m_strType = m_pObj->m_sModel; //型号
	m_strVer = m_pObj->m_sSysVer; //版本号
	
	//通讯状态
	m_strCommStatus = GetCommStatusString(m_pObj->m_nCommStatus);
	
	//运行状态
	m_strRunStatus = GetRunStatusString(m_pObj->m_nRunStatu);
	

	FillCPU();

	return TRUE;
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B80032E
void CWaveRecGeneral::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//正在强制对时
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20069, m_pObj->m_sID, m_nCPU);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在远方启动
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20123, m_pObj->m_sID, m_nCPU);
	}
	else if(m_nCurrentStatus == 5)
	{
		//正在远方启动
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20003, m_pObj->m_sID, m_nCPU);
	}

	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B80033C
BOOL CWaveRecGeneral::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20059 && m_nCurrentStatus == 1)
	{
		//强制对时
		return TRUE;
	}
	if(nMsgID == 20123 && m_nCurrentStatus == 2)
	{
		//远方启动
		return TRUE;
	}
	if (nMsgID == 20085 && m_nCurrentStatus == 5)
	{
		//录波器通讯状态下载
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B80033E
void CWaveRecGeneral::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B8003BC
void CWaveRecGeneral::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}
	
	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	if(m_nCurrentStatus == 1)
	{
		//正在强制对时				
		int nTimeOut = pData->m_nVerifyTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		////正在远方启动
		int nTimeOut = pData->m_nRemoteStartTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}


void CWaveRecGeneral::OnSTTP20086( WPARAM wparam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 5)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	

	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	
	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	int nOSCStatus = pSttpData->sttp_body.nStatus;

    if (nOSCStatus  == 0)
	{
		m_strCommStatus = StringFromID(IDS_COMMUSTATUS_ON);
	}
	else if (nOSCStatus == 1)
	{
		m_strCommStatus = StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else if (nOSCStatus == 2)
	{
		m_strCommStatus = StringFromID(IDS_COMMON_UNKNOWN);
	}

	WriteLog("CWaveRecGeneral::OnSTTP20086, 收到报文状态为" +  m_strCommStatus, XJ_LOG_LV3);
	//刷新界面
	UpdateData(FALSE);
	
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();
}

void CWaveRecGeneral::OnBtnWrgenCommhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 1;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CWaveRecGeneral::OnBtnWrgenRunhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 2;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CWaveRecGeneral::OnBtnWrgenCall() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp *)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20085报文,从子站下载
		if(!pApp->m_SttpEngine.BuildDataFor20085CommStatus(sttpData, m_pObj->m_sID))
		{
			//组织报文失败
			//发送召唤完成消息,结果为发送失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("%s发送20086报文失败, 原因为连接中断", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("%s发送20086报文失败,原因为缓存已满", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
		}
		else
		{
			//成功,改变按钮文字,修改当前状态
			m_nCurrentStatus = 5;
			
			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nCommStatusTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 5)
	{
		//正在召唤状态, 取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

void CWaveRecGeneral::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_GENERAL));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_GENERAL));

	/*
	if (!m_bGetStatus)
	{
		m_strCommStatus = GetCommStatusString(LoadOSCStatus());
		m_bGetStatus = true;
		UpdateData(FALSE);
	}
	*/
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



int CWaveRecGeneral::LoadOSCStatus()
{
	if(m_pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//与数据平台访问DLL的连接可用
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
	
	//字段
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT);

	//字段
	//value
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "curtime", EX_STTP_DATA_TYPE_TIME);
	
	CString str;
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	
	//按时间排序
	Condition condition5;
	str = "order by curtime DESC";
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecGeneral::LoadOSCStatus(), 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str = "";
		if(nCount > 0)
		{
			//取第一个
			str = pMemset->GetValue((UINT)0);
			m_pObj->m_nCommStatus = atoi(str);
			str = pMemset->GetValue((UINT)1);
			WriteLog("更新录波器通讯状态为" + GetCommStatusString(m_pObj->m_nCommStatus) + " 时间:" + str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecGeneral::LoadOSCStatus(),查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		m_pObj->m_nCommStatus = 2;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return m_pObj->m_nCommStatus;
}


HBRUSH CWaveRecGeneral::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	/*
	if(nCtlColor == CTLCOLOR_EDIT)
	{
       // pDC->SetTextColor(RGB(63, 203, 70));//字体背景色******
	   pDC->SetBkColor(RGB(98, 213, 103));//字体背景色******
		HBRUSH B = CreateSolidBrush(RGB(98, 213, 103)); //背景色
		return B;
	}
	*/

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_WRGEN_COMM)
	{
        HBRUSH B;
		if (m_pObj->m_nCommStatus == 0)
		{
			pDC->SetBkColor(RGB(131, 222, 135));//字体背景色******
			B = CreateSolidBrush(RGB(131, 222, 135)); //背景色
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//字体背景色******
			B = CreateSolidBrush(RGB(255, 104, 104)); //背景色
		}
		
		return B;
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_WRGEN_RUN)
	{
		HBRUSH B;
		if (m_pObj->m_nRunStatu == 2)
		{
			pDC->SetBkColor(RGB(131, 222, 135));//字体背景色******
			B = CreateSolidBrush(RGB(131, 222, 135)); //背景色
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//字体背景色******
			B = CreateSolidBrush(RGB(255, 104, 104)); //背景色
		}
		
		return B;
	}
	
	return hbr;
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenStart() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenTime() 
{
	// TODO: Add your control notification handler code here
	
}
