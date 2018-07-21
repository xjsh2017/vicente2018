// WaveRecFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecFrame.h"

// #include "WaveRecGeneral.h"
// #include "WaveRecOSC.h"
// #include "WaveRecSetting.h"
// #include "PTOSCList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveRecFrame

IMPLEMENT_DYNCREATE(CWaveRecFrame, CMDIChildWnd)

//##ModelId=49B87B810002
CWaveRecFrame::CWaveRecFrame()
{
	/** @brief           录波器基本属性页视图指针*/
	m_pWRGeneral = NULL;
	/** @brief           录波器录波属性页视图指针*/
	m_pWROSC = NULL;
	/** @brief           录波器定值属性页视图指针*/
	m_pWRSetting = NULL;
	m_pWROSCList = NULL;
	m_pWRDigital = NULL;
	m_pWRAnalog = NULL;
	m_bSetMenu = FALSE;
	m_pWRNote = NULL;

	m_pCurSec = NULL;
}

//##ModelId=49B87B81003F
CWaveRecFrame::~CWaveRecFrame()
{
}


BEGIN_MESSAGE_MAP(CWaveRecFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CWaveRecFrame)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20124, OnSTTP20124)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20086, OnSTTP20086)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnOpenWRProp)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecFrame message handlers

//##ModelId=49B87B810027
void CWaveRecFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_bSetMenu)
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->XJSetMenuInfo();
		m_bSetMenu = TRUE;
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87B810030
BOOL CWaveRecFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::PreCreateWindow(cs);
	//防止闪烁
	//得到屏幕分辨率
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //宽   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //高 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		/*| FWS_ADDTOTITLE*/ | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //把窗口样式设置为最大化，但先不显示
	
	cs.cx = nSx;   //把窗口大小设置为整个屏幕大小
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //创建完成只之后再显示窗口
	
	return TRUE;
}

//##ModelId=49B87B810041
void CWaveRecFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

//##ModelId=49B87B810033
BOOL CWaveRecFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(!CMDIChildWnd::OnCreateClient(lpcs, pContext))
	{
		return FALSE;
	}

	m_mapIndex.RemoveAll();

	//找到SHEET页
	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE); 
	if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CPTSheet))) 
	{ 
		CPTSheet* pView = (CPTSheet*)pWnd; 
		m_pTabView = pView; 
		m_pTabView->m_nType = 1;

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		
		int nIndex = 0;
		//增加视图
		if(g_WRShowGeneral == 1)
		{
			nIndex = pView->Insert(WR_PROP_GENERAL, StringFromID(IDS_SECPROP_GENERAL), RUNTIME_CLASS(CWaveRecGeneral), pContext); 
			if(nIndex == -1)
			{ 
				return FALSE; 
			}
			//保存视图指针
			m_pWRGeneral = (CWaveRecGeneral*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_GENERAL, nIndex);
		}
		
		if(g_WRShowSetting == 1)
		{
			nIndex = pView->Insert(WR_PROP_SETTING, StringFromID(IDS_SECPROP_SETTING), RUNTIME_CLASS(CPTSetting), pContext); 
			if(nIndex == -1)
			{ 
				return FALSE; 
			}
			//保存视图指针
			m_pWRSetting = (CPTSetting*)pView->GetView(nIndex);
			//记录用于回调函数的指针
			pApp->m_pWRSetting = m_pWRSetting;
			m_mapIndex.SetAt(WR_PROP_SETTING, nIndex);
		}

		if(g_WRShowDI == 1)
		{
			nIndex = pView->Insert(WR_PROP_DIGITAL, StringFromID(IDS_SECPROP_DIGITAL), RUNTIME_CLASS(CPTDigital), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//保存视图指针
			m_pWRDigital = (CPTDigital*)pView->GetView(nIndex);
			//记录用于回调函数的指针
			pApp->m_pWRDigital = m_pWRDigital;
			m_mapIndex.SetAt(WR_PROP_DIGITAL,nIndex);
		}

		if(g_WRShowAI == 1)
		{
			nIndex = pView->Insert(WR_PROP_ANALOG, StringFromID(IDS_SECPROP_ANALOG), RUNTIME_CLASS(CPTAnalog), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//保存视图指针
			m_pWRAnalog = (CPTAnalog*)pView->GetView(nIndex);
			//记录用于回调函数的指针
			pApp->m_pWRAnalog = m_pWRAnalog;
			m_mapIndex.SetAt(WR_PROP_ANALOG, nIndex);
		}

		if(g_WRShowOsc == 1)
		{
			nIndex = pView->Insert(WR_PROP_OSC, StringFromID(IDS_SECPROP_LOCALFILE), RUNTIME_CLASS(CPTOSC), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//保存视图指针
			m_pWROSC = (CPTOSC*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_OSC, nIndex);
		}

		if(g_WRShowOscList == 1)
		{
			nIndex = pView->Insert(WR_PROP_OSCLIST, StringFromID(IDS_SECPROP_REMOTEFILE), RUNTIME_CLASS(CPTOSCList), pContext);
			if(nIndex == -1)
			{
				return FALSE;
			}
			//保存视图指针
			m_pWROSCList = (CPTOSCList*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_OSCLIST, nIndex);
		}

		//备注页面
		if(g_WRShowNote == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_NOTE), RUNTIME_CLASS(CPTNote), pContext);
			if(nIndex == -1)
				return FALSE;
			//记录指针
			m_pWRNote = (CPTNote*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_NOTE, nIndex);
		}

		// activate the input view
		SetActiveView(pView);
	}
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20016()
 功能概要：转发定值下载通知报文(20016)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B81004F
void CWaveRecFrame::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	//定值下载通知, 转发给定值属性页
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20016, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20060()
 功能概要：转发保护对时通知(20060)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B810052
void CWaveRecFrame::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	//对时通知, 转发给基本属性页
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20060, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20124()
 功能概要：转发录波器远方触发下载通知(20124)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B810055
void CWaveRecFrame::OnSTTP20124( WPARAM wParam, LPARAM lParam )
{
	//录波器远方触发下载通知, 转发给基本属性页
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20124, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：转发控制命令通用返回通知
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B81005E
void CWaveRecFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//控制命令通用返回通知, 转发给所有有可能会召唤或控制的页面
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：转发装置不支持功能通知
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B810061
void CWaveRecFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//装置不支持功能通知, 转发给所有可能会召唤或控制的页面
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20125, wParam, lParam);
	}
}

//##ModelId=49B87B810046
void CWaveRecFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
// 	//基本属性页
// 	if(m_pWRGeneral != NULL)
// 	{
// 		m_pWRGeneral ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	
// 	//定值
// 	if(m_pWRSetting != NULL)
// 	{
// 		m_pWRSetting ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 
// 	if(m_pWRDigital != NULL)
// 	{
// 		m_pWRDigital->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	if(m_pWRAnalog != NULL)
// 	{
// 		m_pWRAnalog->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	
// 	//录波
// 	if(m_pWROSC != NULL)
// 	{
// 		m_pWROSC ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
	m_pCurSec = NULL;
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	if(m_pWRNote != NULL)
	{
		m_pWRNote->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 函 数 名：OnOpenWRProp()
 功能概要：响应打开录波器属性消息, 更改窗口标题, 转发消息给各子页
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B810080
void CWaveRecFrame::OnOpenWRProp( WPARAM wParam, LPARAM lParam )
{
	CXJBrowserApp* pApp = (CXJBrowserApp *)AfxGetApp();

	//先转发给各属性性
	CSecObj* pObj = (CSecObj*)lParam;
	m_pCurSec = pObj;	

	if(m_pCurSec != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pCurSec->m_pStation != NULL)
			strStation = m_pCurSec->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pCurSec->m_sName);
		pApp->SetAppTile(str);
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV));

	pApp->GetMainWnd()->SendMessage(WM_MDITAB_REFRESH, 0, 0);
	//基本属性页
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral ->SendMessage(WM_WRFRAME_OPEN, wParam, lParam);
	}

	//定值
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	//录波
	if(m_pWROSC != NULL)
	{
		m_pWROSC ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	//录波列表
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	if(m_pWRNote != NULL )
	{
		m_pWRNote->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	
	//激活页面
	int nPage = (int)wParam;
	int nIndex = -1;
	m_mapIndex.Lookup(nPage, nIndex);
	if(nIndex <= WR_PROP_OSCLIST && nIndex >= WR_PROP_GENERAL)
		m_pTabView->ActivateView(nIndex);
	else
		m_pTabView->ActivateView(0);
	
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B810083
CWaveRecFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//基本属性页
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	
	//定值
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	
	//录波
	if(m_pWROSC != NULL)
	{
		m_pWROSC ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_WRFRAME_CLOSE, wParam, lParam);
	}

	if(m_pWRNote != NULL )
	{
		m_pWRNote->SendMessage(WM_WRFRAME_CLOSE, wParam, lParam);
		m_pWRNote->SendMessage(WM_PTFRAME_CLOSE, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20026()
 功能概要：转发20026报文给远方录波视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B81006E
void CWaveRecFrame::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20026, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20043()
 功能概要：转发20043报文给远方录波视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B810071
void CWaveRecFrame::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20043, wParam, lParam);
	}
}

//##ModelId=49B87B81008D
CWaveRecFrame::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->PostMessage(REFRESH_PTSETTING, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B81007D
void CWaveRecFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20157, wParam, lParam);
	}
}

/*************************************************************
 函 数 名: OnSTTP20008()
 功能概要: 转发模拟量下载通知(20008)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20008( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRAnalog != NULL)
		m_pWRAnalog->SendMessage(WM_STTP_20008, wParam, lParam);
}

/*************************************************************
 函 数 名: OnSTTP20012()
 功能概要: 转发(20012)开关量下载通知
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20012( WPARAM wParam,LPARAM lParam )
{
	if(m_pWRDigital != NULL)
		m_pWRDigital->SendMessage(WM_STTP_20012, wParam, lParam);
}

/*************************************************************
 函 数 名: OnSTTP20086()
 功能概要: 转发模拟量下载通知(20086)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20086( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRGeneral != NULL)
		m_pWRGeneral->SendMessage(WM_STTP_20086, wParam, lParam);
}

/****************************************************
Date:2011/9/27  Author:LYH
函数名:   OnSTTP20107	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CWaveRecFrame::OnSTTP20107( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20107, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20048()
 功能概要：转发定值组切换通知(20048)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920001
void CWaveRecFrame::OnSTTP20048( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20048, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20170( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20170, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20052()
 功能概要：转发保护定值修改通知(20052)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92000F
void CWaveRecFrame::OnSTTP20052( WPARAM wParam, LPARAM lParam )
{
	//保护定值修改通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20052, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20054()
 功能概要：转发保护定值修改执行通知(20054)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920013
void CWaveRecFrame::OnSTTP20054( WPARAM wParam, LPARAM lParam )
{
	//保护定值修改执行通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20054, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20056()
 功能概要：转发保护定值组切换执行通知(20056)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920021
void CWaveRecFrame::OnSTTP20056( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换执行通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20056, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20172( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换执行通知, 转发给保护属性页--定值视图
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20172, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20174(WPARAM wParam, LPARAM lParam)
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20174, wParam, lParam);
	}
}
