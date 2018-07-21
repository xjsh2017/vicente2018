// PTFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTFrame.h"

#include "DeviceObj.h"
//保护各属性视图页
#include "PTAction.h"
#include "PTAlarm.h"
#include "PTAnalog.h"
#include "PTDigital.h"
#include "PTGeneral.h"
#include "PTOSC.h"
#include "PTSetting.h"
#include "PTSoftBoard.h"

#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CPTFrame

IMPLEMENT_DYNCREATE(CPTFrame, CMDIChildWnd)

//##ModelId=49B87B91033E
CPTFrame::CPTFrame()
{
	/** @brief           保护动作属性页视图指针*/
	m_pPTAction = NULL;
	/** @brief           保护告警属性页视图指针*/
	m_pPTAlarm = NULL;
	/** @brief           保护模拟量属性页视图指针*/
	m_pPTAnalog = NULL;
	/** @brief           保护开关量属性页视图指针*/
	m_pPTDigital = NULL;
	/** @brief           保护基本属性页视图指针*/
	m_pPTGeneral = NULL;
	/** @brief           保护录波属性页视图指针*/
	m_pPTOSC = NULL;
	/** @brief           保护定值属性页视图指针*/
	m_pPTSetting = NULL;
	/** @brief           保护软压板属性页视图指针*/
	m_pPTSoftBoard = NULL;
	m_pPTHistory = NULL;
	m_pPTOSCList = NULL;
	m_pPTNote = NULL;
	m_bSetBar = FALSE;
	m_pCurSec = NULL;
}

//##ModelId=49B87B91039B
CPTFrame::~CPTFrame()
{
	m_mapIndex.RemoveAll();
}


BEGIN_MESSAGE_MAP(CPTFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPTFrame)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20058, OnSTTP20058)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
	ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_STTP_20109, OnSTTP20109)
	ON_MESSAGE(WM_STTP_20111, OnSTTP20111)
	ON_MESSAGE(WM_STTP_20114, OnSTTP20114)
	ON_MESSAGE(WM_STTP_20118, OnSTTP20118)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20089, OnSTTP20089)
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20116, OnSTTP20116)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20004, OnSTTP20004)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
	ON_MESSAGE(REFRESH_PTDI, OnRefreshPTDI)
	ON_MESSAGE(REFRESH_PTSOFTBOARD, OnRefreshPTSoft)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTFrame message handlers

//##ModelId=49B87B910382
void CPTFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp *)AfxGetApp();

	nCmdShow =SW_SHOWMAXIMIZED;	
	//通知主框架隐藏设备树
	if (!m_bSetBar)
	{
		pApp->XJSetMenuInfo();
		m_bSetBar = TRUE;
	}
// 	pApp->GetMainWnd()->PostMessage(DEVICETREE_HIDE, 0, 0);
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87B91038A
BOOL CPTFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		int nIndex = 0;
		//增加视图

		//基本信息
		if(g_PTShowGeneral == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_GENERAL), RUNTIME_CLASS(CPTGeneral), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTGeneral = (CPTGeneral*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_GENERAL, nIndex);
		}

		//定值
		if(g_PTShowSetting == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_SETTING), RUNTIME_CLASS(CPTSetting), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTSetting = (CPTSetting*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_SETTING, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTSetting = m_pPTSetting;
		}
		
		//开关量
		if(g_PTShowDI == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_DIGITAL), RUNTIME_CLASS(CPTDigital), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTDigital = (CPTDigital*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_DIGITAL, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTDigital = m_pPTDigital;
		}
		
		//软压板
		if(g_PTShowSoftBoard == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_SOFT), RUNTIME_CLASS(CPTSoftBoard), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTSoftBoard = (CPTSoftBoard*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_SOFT, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTSoftBoard = m_pPTSoftBoard;
		}
		
		//模拟量
		if(g_PTShowAI == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ANALOG), RUNTIME_CLASS(CPTAnalog), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAnalog = (CPTAnalog*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ANALOG, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTAnalog = m_pPTAnalog;
		}
		
		//动作
		if(g_PTShowAction == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ACTION), RUNTIME_CLASS(CPTAction), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAction = (CPTAction*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ACTION, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTAction = m_pPTAction;
		}
		
		//告警
		if(g_PTShowAlarm == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ALARM), RUNTIME_CLASS(CPTAlarm), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAlarm = (CPTAlarm*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ALARM, nIndex);
			//记录用于回调函数的指针
			pApp->m_pPTAlarm = m_pPTAlarm;
		}
		
		//历史事件
		if(g_PTShowHistory == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_HISTORYEVENT), RUNTIME_CLASS(CPTHistory), pContext);
			if(nIndex == -1)
				return FALSE;
			//记录指针
			m_pPTHistory = (CPTHistory*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_HISTORY, nIndex);
		}

		//本地录波
		if(g_PTShowOsc == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_LOCALFILE), RUNTIME_CLASS(CPTOSC), pContext);
			if (nIndex == -1)  
				return FALSE;  
			//记录指针
			m_pPTOSC = (CPTOSC*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_OSC, nIndex);
		}
		
		//录波下载
		if(g_PTShowOscList == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_REMOTEFILE), RUNTIME_CLASS(CPTOSCList), pContext);
			if(nIndex == -1)
				return FALSE;
			//记录指针
			m_pPTOSCList = (CPTOSCList*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_OSCLIST, nIndex);
		}

		//备注页面
		if(g_PTShowNote == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_NOTE), RUNTIME_CLASS(CPTNote), pContext);
			if(nIndex == -1)
				return FALSE;
			//记录指针
			m_pPTNote = (CPTNote*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_NOTE, nIndex);
		}

		// activate the input view
		SetActiveView(pView);
	}
	return TRUE;
}

//##ModelId=49B87B91038E
BOOL CPTFrame::PreCreateWindow(CREATESTRUCT& cs) 
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
	
	//cs.style &=~ (LONG) FWS_ADDTOTITLE;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style |= WS_VISIBLE;
	
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20008()
 功能概要：转发保护模拟量采样值下载通知(20008)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103C9
void CPTFrame::OnSTTP20008( WPARAM wParam, LPARAM lParam )
{
	//保护模拟量采样值下载通知,转发给保护属性页--模拟量视图
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog->SendMessage(WM_STTP_20008, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20012()
 功能概要：转发保护开关量采样值下载通知(20012)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103CD
void CPTFrame::OnSTTP20012( WPARAM wParam, LPARAM lParam )
{
	//保护开关量值下载通知, 转发给保护属性页--开关量视图
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital->SendMessage(WM_STTP_20012, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20016()
 功能概要：转发定值下载通知(20016)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103DB
void CPTFrame::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	//保护定值下载通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20016, wParam, lParam);
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
void CPTFrame::OnSTTP20048( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20048, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20170( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20170, wParam, lParam);
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
void CPTFrame::OnSTTP20052( WPARAM wParam, LPARAM lParam )
{
	//保护定值修改通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20052, wParam, lParam);
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
void CPTFrame::OnSTTP20054( WPARAM wParam, LPARAM lParam )
{
	//保护定值修改执行通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20054, wParam, lParam);
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
void CPTFrame::OnSTTP20056( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换执行通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20056, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20172( WPARAM wParam, LPARAM lParam )
{
	//保护定值组切换执行通知, 转发给保护属性页--定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20172, wParam, lParam);
	}
}


/*************************************************************
 函 数 名：OnSTTP20058()
 功能概要：转发保护信号复归通知(20058)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92002F
void CPTFrame::OnSTTP20058( WPARAM wParam, LPARAM lParam )
{
	//保护信号复归通知, 转发给保护基本信息视图
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20058, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20060()
 功能概要：转发保护对时通知(20060)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92003E
void CPTFrame::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	//保护对时通知, 转发给保护基本信息视图
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20060, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：转发控制命令通用返回通知(20069)
 返 回 值:  void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920042
void CPTFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//控制命令通用返回通知, 转发给所有可能会发送召唤或操作命令的属性页
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//定值页
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//开关量页
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//历史事件
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20107()
 功能概要：转发召唤当前定值区通知(20107)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920050
void CPTFrame::OnSTTP20107( WPARAM wParam, LPARAM lParam )
{
	//召唤保护当前定值区通知, 转发给定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20107, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20174( WPARAM wParam, LPARAM lParam )
{
	//召唤保护当前定值区通知, 转发给定值视图
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20174, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20109()
 功能概要：转发召唤保护软压板通知(20109)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92005D
void CPTFrame::OnSTTP20109( WPARAM wParam, LPARAM lParam )
{
	//召唤保护软压板通知, 转发给软压板视图
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard->SendMessage(WM_STTP_20109, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20111()
 功能概要：转发召唤保护基本信息通知(20111)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920061
void CPTFrame::OnSTTP20111( WPARAM wParam, LPARAM lParam )
{
	//召唤保护基本信息通知, 转发给基本信息视图
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20111, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20114()
 功能概要：转发保护软压板投退执行通知(20114)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92006E
void CPTFrame::OnSTTP20114( WPARAM wParam, LPARAM lParam )
{
	//保护软压板投退执行通知, 转发给软压板视图
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20114, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20118()
 功能概要：转发保护软压板投退通知, 转发给软压板视图
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920072
void CPTFrame::OnSTTP20118( WPARAM wParam, LPARAM lParam )
{
	//保护软压板投退通知, 转发给软压板视图
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard->SendMessage(WM_STTP_20118, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：转发装置不支持功能通知
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92007E
void CPTFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//装置不支持功能通知, 转发给所有可能会发送召唤或操作命令的属性页
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//定值页
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//开关量页
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//历史事件
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20125, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20089()
 功能概要：转发保护召唤时间通知(20089)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920082
void CPTFrame::OnSTTP20089( WPARAM wParam, LPARAM lParam )
{
	//保护召唤时间通知, 转发给基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20089, wParam, lParam);
	}
}


/*************************************************************
 函 数 名：OnSTTP20004()
 功能概要：转发召唤保护基本信息通知(20111)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920061
void CPTFrame::OnSTTP20004( WPARAM wParam, LPARAM lParam )
{
	//召唤保护基本信息通知, 转发给基本信息视图
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20004, wParam, lParam);
	}
}


//##ModelId=49B87B91039D
void CPTFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//先转发给各属性性
	m_pCurSec = NULL;
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//定值页
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//开关量页
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//动作事件
	if(m_pPTAction != NULL)
	{
		m_pPTAction ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//告警事件
	if(m_pPTAlarm != NULL)
	{
		m_pPTAlarm ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//历史事件
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//录波
	if(m_pPTOSC != NULL)
	{
		m_pPTOSC ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//备注页面
	if(m_pPTNote != NULL )
	{
		m_pPTNote->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//发消息通知主框架
	AfxGetApp() ->m_pMainWnd->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页面打开消息,先转发给各属性性, 通知它们更新内容, 再把窗口推出前面
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B9200BB
void CPTFrame::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//先转发给各属性性
	CSecObj* pObj = (CSecObj*)lParam;
	m_pCurSec = pObj;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
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
	
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//激活窗口
	ActivateFrame(SW_SHOWMAXIMIZED);
	
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTGeneral->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入基本属性页数据时间", startTime);
	}

	//定值页
	if(m_pPTSetting != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTSetting ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入定值页数据时间", startTime);
	}
	
	//开关量页
	if(m_pPTDigital != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTDigital ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入开关量数据时间", startTime);
	}
	
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入软压板数据时间", startTime);
	}
	
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAnalog ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入模拟量数据时间", startTime);
	}
	
	//动作事件
	if(m_pPTAction != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAction ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入动作数据时间", startTime);
	}
	
	//告警事件
	if(m_pPTAlarm != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAlarm ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入告警数据时间", startTime);
	}
	
	//历史事件
	if(m_pPTHistory != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTHistory->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入历史事件数据时间", startTime);
	}
	
	//录波
	if(m_pPTOSC != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTOSC ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入录波数据时间", startTime);
	}
	
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTOSCList ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入远方录波数据时间", startTime);
	}

	//备注页面
	if(m_pPTNote != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTNote ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("载入备注数据时间", startTime);
	}

	//激活页面
	int nPage = (int)wParam;
	int nIndex = -1;
	m_mapIndex.Lookup(nPage, nIndex);
	if(nIndex <= PT_PROP_NOTE && nIndex >= PT_PROP_GENERAL)
		m_pTabView->ActivateView(nIndex);
	else
		m_pTabView->ActivateView(0);
}

//##ModelId=49B87B9103AA
void CPTFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：OnEventPropAckPT()
 功能概要：由保护属性页面打开的事件属性对话框发生事件确认消息, 转发
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B9200BF
void CPTFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：由事件栏打开的事件属性对话框发生事件确认消息, 转发
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B9200CC
void CPTFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnEventListAck()
 功能概要：由事件栏发生事件确认消息, 转发
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B9200DA
void CPTFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 函 数 名：OnAllClose()
 功能概要:	响应程序关闭消息, 转发给各属性页面WM_PTFRAME_CLOSE消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200DE
CPTFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//定值页
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//开关量页
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//动作事件
	if(m_pPTAction != NULL)
	{
		m_pPTAction ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//告警事件
	if(m_pPTAlarm != NULL)
	{
		m_pPTAlarm ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//历史事件
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//录波
	if(m_pPTOSC != NULL)
	{
		m_pPTOSC ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//备注页面
	if(m_pPTNote != NULL )
	{
		m_pPTNote ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
}

/*************************************************************
 函 数 名：OnSTTP20026()
 功能概要：20026(录波列表下载通知), 转发给远方录波页面
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92008F
void CPTFrame::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20026, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20043()
 功能概要：20043(录波文件下载报文), 转发给远方录波页面
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920093
void CPTFrame::OnSTTP20043( WPARAM wParam, LPARAM lparam )
{
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20043, wParam, lparam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20116()
 功能概要：20116(保护历史事件下载通知), 转发给历史事件页面
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92009F
void CPTFrame::OnSTTP20116( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20116, wParam, lParam);
	}
}

//##ModelId=49B87B9200EA
void CPTFrame::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTSetting != NULL)
		m_pPTSetting->PostMessage(REFRESH_PTSETTING, wParam, lParam);
}

//##ModelId=49B87B9200EE
void CPTFrame::OnRefreshPTDI( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTDigital != NULL)
		m_pPTDigital->PostMessage(REFRESH_PTDI, wParam, lParam);
}

//##ModelId=49B87B9200FB
void CPTFrame::OnRefreshPTSoft( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTSoftBoard != NULL)
		m_pPTSoftBoard->PostMessage(REFRESH_PTSOFTBOARD, wParam, lParam);
}

//##ModelId=49B87B910399
void CPTFrame::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::PostNcDestroy();
//	delete this;
}

//##ModelId=49B87B910385
BOOL CPTFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::DestroyWindow();
}

//##ModelId=49B87B9103AF
void CPTFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIChildWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
}

//##ModelId=49B87B9103BD
void CPTFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：OnSTTP00901()
 功能概要：收到00901报文, 转发给历史事件视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200AF
void CPTFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_00901, wParam, lParam);
	}
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应20157报文, 转发给所有可能会发送召唤或操作命令的属性页
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200AB
void CPTFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	//超时返回通知, 转发给所有可能会发送召唤或操作命令的属性页
	//基本属性页
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//定值页
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//开关量页
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//软压板
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//模拟量
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//远方录波
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//历史事件
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20157, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->SendMessage(WM_STTP_20018, wParam, lParam);
}

void CPTFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->SendMessage(WM_STTP_20022, wParam, lParam);
}
