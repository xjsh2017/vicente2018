// XJBrowser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "XJBrowserDoc.h"

#include "ViewAction.h"	//动作事件视图
#include "ViewAlarm.h"	//告警事件视图
#include "ViewDI.h"		//开关量事件视图
#include "ViewFault.h"	//事故报告视图
#include "ViewOSC.h"	//录波视图


#include "SysAlarmFrame.h"	//系统告警事件框架
#include "ViewSysAlarmDetail.h"	//系统告警事件详细视图
#include "ViewSysAlarm.h"	//系统告警事件视图

#include "CommFrame.h"	//主子站通信状态子框架
#include "ViewCommMap.h" //主子站通讯状态图形视图

#include "ActionFrame.h" //动作事件子框架
#include "AlarmFrame.h" //告警事件子框架

#include "FaultFrame.h"	 //事故报告子框架
#include "ViewFaultSheet.h" //事故报告SHEET视图

#include "PTFrame.h"	//保护属性窗口
#include "PTSheet.h"	//属性页视图
#include "WaveRecFrame.h"//录波器属性窗口

#include "DlgValidateID.h" //身份验证对话框

#include "BatchSelectView.h" //批量召唤选择视图
#include "BatchFrame.h" //批量召唤框架
#include "AutoCallFrame.h"//自动总召框架
#include "AutoCallResult.h"//自动总召视图
#include "AutoResultFrame.h"//自动总召结果框架
#include "ACResultSel.h" //自动总召结果厂站选择视图

#include "AboutDlgEx.h"
#include "SysSettingSheet.h"	//系统设置窗口

#include "DownloadFrame.h" //通用文件下载框架
#include "DownLoadView.h"  //通用文件下载视图

#include "CrossFrame.h" //预警窗口框架
#include "ViewCross.h" //预警列表视图

#include "SoeFrame.h" //SOE事件窗口框架
#include "ViewSOE.h"  //SOE事件列表视图

#include "DraftFrame.h"
#include "DraftView.h"
#include "DraftDocument.h"
#include "DraftList.h"

#include "ViewDistance.h"

#include "StationInfoFrame.h"
#include <winnetwk.h>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include "GlobalFunc.h"
#include "CustomStyleSetting.h"
#include "MtVerify.h"
#include "PushMsgProcess.h"

#include "XJRootStore.h"
#include "XJUserStore.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/


CPushMsgProcess	g_PushMsgProcess;

#define CLEARCONFIG_SPACE	600000	//清理二次设备现场运行的间隔时间,10分钟

/*************************************************************
 函 数 ClearSecConfig()
 功能概要：清理二次设备配置线程(动作特征值配置, 动作信号号配置, 告警信号号配置)
 返 回 值: UINT
 参    数：param1	
**************************************************************/
UINT ClearSecConfig(LPVOID pParam)
{
	WriteLog("启动清理二次设备配置线程成功!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	while(1)
	{
		if(g_bSuspend)
		{
			Sleep(10);
			continue;
		}
		if(pApp->m_bExitClearConfig)
			break;
		int ntemptime = 0;
		while( ntemptime < CLEARCONFIG_SPACE )
		{
			Sleep(10);
			ntemptime+=10;
			if(pApp->m_bExitClearConfig)
				break;
		}
		if(g_bSuspend)
			continue;
		if(pApp->m_bExitClearConfig)
			break;
		CDataEngine * pData = pApp->GetDataEngine();
		if(pData == NULL)
		{
			continue;
		}
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetSecList(listDevice, "", TYPE_SEC);
		POSITION pos;
		pos = listDevice.GetHeadPosition();

		WriteLog("开始清理二次设备配置!", XJ_LOG_LV3);
		int ncount = 0;
		int nclearcount = 0;
		while(pos != NULL)
		{
			if(pApp->m_bExitClearConfig || g_bSuspend)
			{
				break;
			}

			CSecObj * pObj = (CSecObj*)listDevice.GetNext(pos);
			if(pObj == NULL)
				continue;
			if(pObj->CanClear())
			{
				pObj->ClearConfig();
				nclearcount++;
				CString str;
				str.Format("二次设备配置清理线程，成功清理[%s]的配置", pObj->m_sID);
				WriteLog(str, XJ_LOG_LV3);
			}

			ncount++;
			if( ncount%50 == 0 )
				Sleep(2);
		}
		CString str;
		str.Format("二次设备配置清理线程，成功清理%d个装置的配置", nclearcount);
		WriteLog(str, XJ_LOG_LV3);

	}
	WriteLog("退出清理二次设备配置线程成功!", XJ_LOG_LV3);
	return 0;
}

/*************************************************************
 函 数 名: PostMessageThread()
 功能概要: 从队列中取出报文转发给主框架, 主框架再转发给各子框架
 返 回 值: UINT
 参    数: param1 主程序指针
		   Param2 
**************************************************************/
UINT PostMessageThread(LPVOID pParam)
{
	WriteLog("启动分发报文线程成功!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//循环分发报文
	while(1)
	{
		//WriteLog("PostMessageThread");
		if(pApp->m_bExitReceiveMsg && pApp->m_listFullData.GetCount() == 0)
		{
			//退出标志为TRUE
			return 0;
		}

		while(g_bSuspend)
		{
			//暂停分发线程
			Sleep(10);
		}

		//休眠
		if(pApp->m_listFullData.GetCount() == 0)
			Sleep(2000);
		else
			Sleep(g_PostTime);

		//取得队列的第一个数据
		STTP_FULL_DATA* sttpData = pApp->GetFirstSTTPData();
		if(sttpData == NULL)
			continue;

		if(pApp->m_bExitReceiveMsg )
		{
			//退出标志为TRUE
			return 0;
		}
		
		//读取报文成功
		try
		{
			int nMsgID = sttpData->sttp_head.uMsgID;
			if(nMsgID > 20000 && nMsgID < 30000)
			{
				//报文合法,计算报文对应的消息常量值
				int nMessage = (nMsgID - 20000) + 2000 + WM_USER;
				int nNotify = nMsgID - 20000 + 2000;
				CString sNotify;
				sNotify.Format("(WM_USER + %d)", nNotify);
				
				//得到主框架指针
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("收到%d报文, 发送%s消息到主框架", nMsgID, sNotify);
					WriteLog(str, XJ_LOG_LV3);

					if(pApp->m_bExitReceiveMsg )
					{
						//退出标志为TRUE
						return 0;
					}

					pFrame->SendMessage(nMessage, 0, (LPARAM)sttpData);
				}
			}
			else if(901 == nMsgID)
			{
				//得到主框架指针
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("收到00901报文, 发送WM_STTP_00901消息到主框架");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//退出标志为TRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00901, 0, (LPARAM)sttpData);
				}
			}
			else if(204 == nMsgID)
			{
				//得到主框架指针
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("收到00204报文, 发送WM_STTP_00204消息到主框架");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//退出标志为TRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00204, 0, (LPARAM)sttpData);
				}
			}
			else if(212 == nMsgID)
			{
				//得到主框架指针
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("收到00212报文, 发送WM_STTP_00212消息到主框架");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//退出标志为TRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00212, 0, (LPARAM)sttpData);
				}
			}
			else
			{
				CString str;
				str.Format("收到未知报文, 报文ID为%d", nMsgID);
				WriteLog(str, XJ_LOG_LV3);
			}
		}
		catch (...)
		{
			WriteLog("PostMessageThread, 处理报文异常", XJ_LOG_LV3);
		}
		
		//zero_sttp_full_data(*sttpData);
		//删除数据
		pApp->RemoveSTTPData();

		//WaitForSingleObject(theApp.m_hPostEvent, INFINITE);
		//----------------------****************************--------------
		//WriteLog("PostMessageThread.............");
	}
	return 0;
}

/*************************************************************
 函 数 名：ReceiveMessageThread()
 功能概要：接收STTP报文线程, 保存到队列
 返 回 值: UINT
 参    数：param1	主程序指针
**************************************************************/
UINT ReceiveMessageThread(LPVOID pParam)
{
	WriteLog("启动读取报文线程成功!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//循环收取报文
	while(1)
	{
		//WriteLog("ReceiveMessageThread");
		if(pApp->m_bExitReceiveMsg)
		{
			//退出标志为TRUE
			return 0;
		}
		//申请存放报文地址
		STTP_FULL_DATA*  sttpData = new STTP_FULL_DATA();
		if(sttpData == NULL)
			continue;
		zero_sttp_full_data(*sttpData);
		//读取报文
		int nResult = 0;
		try
		{
			if( pApp->GetSTTPStatus() )
			{
				nResult = pApp->m_SttpEngine.XJSTTPRead(pApp->m_SttpHandle, *sttpData);
			}		
		}
		catch (...)
		{
			WriteLog("ReceiveMessageThread 报文读取失败", XJ_LOG_LV3);
		}
		
		if(1 == nResult)
		{
			if(pApp->m_bExitReceiveMsg)
			{
				//退出标志为TRUE
				if(sttpData != NULL)
					delete sttpData;
				return 0;
			}

			//读取报文成功
			try
			{
				//保存到队列
				if(g_PushMsgProcess.IsAutoPushMsg(sttpData->sttp_head.uMsgID))
				{
					g_PushMsgProcess.AddSttpData(sttpData);
					CString str;
					str.Format("ReceiveMessageThread, 接收到自动上送报文%d,保存至上送报文处理中心", sttpData->sttp_head.uMsgID);
					WriteLog(str, XJ_LOG_LV3);
				}
				else
				{
					if(g_PushMsgProcess.IsAutoNotifyMsg(sttpData->sttp_head.uMsgID))
					{
						CString str;
						str.Format("ReceiveMessageThread, 接收到上载要求报文%d,不处理", sttpData->sttp_head.uMsgID);
						WriteLog(str, XJ_LOG_LV3);
						if(sttpData)
						{
							zero_sttp_full_data(*sttpData);
							delete sttpData;
							sttpData = NULL;
						}
					}
					else
					{
						pApp->AddSTTPData(sttpData);
						CString str;
						str.Format("ReceiveMessageThread, 接收到回应报文%d,保存至缓存", sttpData->sttp_head.uMsgID);
						WriteLog(str, XJ_LOG_LV3);
					}
				}
				Sleep(2);
				//WaitForSingleObject(theApp.m_hReceiveEvent, INFINITE);
				continue;
			}
			catch (...)
			{
				WriteLog("ReceiveMessageThread, 保存报文数据至缓存失败", XJ_LOG_LV3);
			}	
		}
		else if(nResult == -1)
		{
			//网络中断
			WriteLog("ReceiveMessageThread, 接收报文数据失败,原因为网络中断", XJ_LOG_LV3);
			Sleep(500);
		}
		else if(0 == nResult)
		{
			//WriteLog("ReceiveMessageThread, 接收到报文数据失败,原因为当前无数据", XJ_LOG_LV3);
			Sleep(10);
		}
		else
		{
			WriteLog("ReceiveMessageThread, 接收报文数据失败,原因未知", XJ_LOG_LV3);
			Sleep(500);
		}
		
		if(sttpData != NULL) //异常或无数据时
		{
			zero_sttp_full_data(*sttpData);
			delete sttpData;
			sttpData = NULL;
		}

		//WaitForSingleObject(theApp.m_hReceiveEvent, INFINITE);
		//-------*********************---------------------
		//WriteLog("ReceiveMessageThread..............");
	}

	return 0;
}

UINT PushMsgThread(LPVOID pParam)
{
	WriteLog("启动自动上送报文线程成功!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//循环分发报文
	while(1)
	{
		if(pApp->m_bExitReceiveMsg)
		{
			//退出标志为TRUE
			return 0;
		}

		while(g_bSuspend)
		{
			//暂停分发线程
			Sleep(10);
		}

		//休眠
		Sleep(g_PostTime);

		if(pApp->m_bExitReceiveMsg )
		{
			//退出标志为TRUE
			return 0;
		}

		g_PushMsgProcess.TranslateSttpData();
	}
	return 0;
}
 
BOOL CheckSystemProcess()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
	DWORD dCurrentProcessId = GetCurrentProcessId();
	
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;
	
    // Calculate how many process identifiers were returned.
	
    cProcesses = cbNeeded / sizeof(DWORD);
	
    // Print the name and process identifier for each process.
	
    for ( i = 0; i < cProcesses; i++ )
	{
        if( aProcesses[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			CString strModuleName = _T("");
			DWORD processID = aProcesses[i];
			// Get a handle to the process.
			
			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processID );
			if ( processID == dCurrentProcessId)
			{
				continue;
			}
			
			// Get the process name.
			
			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;
				
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
					&cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, 
						sizeof(szProcessName)/sizeof(TCHAR) );
					strModuleName = szProcessName;
					if (strModuleName.Find("XJBrowser") != -1)
					{ 
						if ( AfxMessageBox(StringFromID(IDS_TIP_APP_EXIST), MB_YESNO) == IDYES)
						{
							if ( !::TerminateProcess(hProcess, 0))
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}
					}
				}
			}	
		}
	}     

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXJBrowserApp object

CXJBrowserApp theApp;

/*************************************************************
 函 数 名：ComplexExchange()
 功能概要：复数转换, 判断传入的字符串是否表示复数,如果是,转换成复数形式表示
 返 回 值: void
 参    数：param1	需转换的字符串	
		   Param2
**************************************************************/
CString	ComplexExchange(CString sStr)
{
	CString sReturn = sStr;
	//复数的字符串形式为: 实部&&&虚部
	int nPos = sStr.Find("&&", 0);
	if(nPos == -1)
	{
		//非复数
		return sReturn;
	}
	//复数
	CString strR = sStr.Left(nPos); //实部
	CString strC = sStr;
	strC.Delete(0, nPos + 2); //虚部
	sReturn.Format("%s+j%s", strR, strC);
	return sReturn;
}

/*************************************************************
 函 数 名：CompareComplex()
 功能概要：比较两个复数的大小
 返 回 值: 如果1比2大, 返回1. 1=2,返回0. 1<2,返回-1
 参    数：param1	复数1
		   Param2	复数2
**************************************************************/
int	CompareComplex(CString str1, CString str2)
{
	//判断数据合法,复数必然是: 实部+j虚部 形式
	int nPos1 = str1.Find('+', 0);
	int nPos2 = str2.Find('+', 0);

	if(nPos1 == -1 || nPos2 == -1)
	{
		//不合法,退出
		return 0;
	}
	
	CString strR1 = str1.Left(nPos1); //复数1的实部
	CString strC1 = str1;
	strC1.Delete(0, nPos1+1); //复数1的虚部
	CString strR2 = str2.Left(nPos2); //复数2的实部
	CString strC2 = str2;
	strC2.Delete(0, nPos2+1); //复数2的虚部
	//先比实部
	int R1 = atoi(strR1);
	int R2 = atoi(strR2);
	if(R1 > R2)
	{
		return 1;
	}
	else if(R1 < R2)
	{
		return -1;
	}
	else
	{
		//实部相等,比虚部
		int C1 = atoi(strC1);
		int C2 = atoi(strC2);
		if(C1 > C2)
		{
			return 1;
		}
		else if(C1 < C2)
		{
			return -1;
		}
		else 
		{
			return 0;
		}
	}
	return 0;
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    WriteLogTemp
返回值:   void
功能概要: 短连接版写日志，只写文件
参数: CString sLog
参数: int nLV
************************************/
void WriteLogTemp(CString sLog, int nLV)
{
	//设置日志文件路径
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);
	logMgr.WriteLog(sLog, nLV);
}

/*************************************************************
 函 数 名：WriteLog()
 功能概要：写日志, 包括程序的输出栏和日志文件(可设置是否输出日志文件)
 返 回 值: void
 参    数：param1	日志文本内容
		   Param2	日志等级, 1-重要. 2-一般. 3-调试用. 默认为2
**************************************************************/
void WriteLog(CString sLog, int nLV)
{
	theApp.WriteLog(sLog, nLV);
}

/*************************************************************
 函 数 名：WriteLog()
 功能概要：写日志, 写到日志文件(可设置是否输出日志文件)
 返 回 值: void
 参    数：param1	日志文本内容
		   Param2	日志等级, 1-重要. 2-一般. 3-调试用. 默认为2
**************************************************************/
void WriteLogEx(CString sLog, int nLV)
{
	theApp.WriteLogEx(sLog, nLV);
}

/*************************************************************
 函 数 名：StringToTimeSttp15()
 功能概要：将15位的STTP时间字符串转化为CTime
 返 回 值: 转化成的CTime对象
 参    数：param1 时间字符串
		   Parsm2 返回毫秒
**************************************************************/
CTime StringToTimeSttp15(CString & sTime, int& nms)
{
	//字符串格式为YYMMDDhhmmss+3位毫秒数,如080803095654123
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() < 12 && strTime.GetLength() > 16)
		return tmTime;
	CString strTemp;

	int nt[6];

	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}

	//毫秒
	nms = atoi(strTime);

	//年加上2000,变成20XX年
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//判断日期
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：StringToTimeSttp12()
 功能概要：将12位的STTP时间字符串转化为CTime,12位的时间不带毫秒值
 返 回 值: 转化成的CTime对象
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTimeSttp12(CString & sTime)
{
	//字符串格式为YYMMDDhhmmss,如080803095654
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() != 12)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}
	
	//年加上2000,变成20XX年
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：StringToTimeSttp26()
 功能概要：将26位的STTP时间字符串转化为CTime,毫秒值占6位
 返 回 值: CTime类型时间
 参    数：param1	时间字符串
		   Param2	毫秒值
**************************************************************/
CTime StringToTimeSttp26(CString & sTime, int& nms)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMMMM 如:2008-05-30 13:44:51.120005
	CString strTime = sTime;
	if(strTime.GetLength() < 23 && strTime.GetLength() >27)
		return tmTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//判断秒钟合法
		if(nSec < 0 || nSec > 60)
		return tmTime;

	//毫秒
	strTemp = strTime;
	strTemp.Delete(0, 20);
	if(strTemp.GetLength() > 3)
	{
		//去掉后面3位
		strTemp.Delete(strTemp.GetLength() - 3, 3);
	}
	//剩下6位毫秒值
	nms = atoi(strTemp);
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 函 数 名：StringToTime14()
 功能概要：将14数的时间字符串转换成时间对象
 返 回 值: CTime类型时间
 参    数：param1 时间字符串
		   Param2
**************************************************************/
CTime StringToTime14(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//时间格式为:YYYYMMDDhhmmss, 如:20080808010100
	CString strTime = sTime;
	if(strTime.GetLength() != 14)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//每次读取前两位,然后删除前两位
		int nLength = 2;
		if(i == 0)
			nLength = 4;
		strTemp = strTime.Left(nLength);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, nLength);
	}

	//判断年份
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//判断月份
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//判断日期
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//判断小时
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//判断分钟
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//判断秒
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 函 数 名：XJToMS()
 功能概要：CXJString对象字符串转化为CString字符串
 返 回 值: 转化成CString对象
 参    数：param1	要转化的CXJString对象
**************************************************************/
CString XJToMS(string xjStr)
{
	int nLength = xjStr.length();
	char* a = new char[nLength+2];
	memset(a, 0, nLength+2);
	memcpy(a, xjStr.c_str(), nLength);
	CString str = a;
	delete[] a;
	return str;
}

string WChar2Ansi(LPCWSTR pwszSrc) 
{ 
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL); 
	if (nLen <= 0) return std::string(""); 
	char* pszDst = new char[nLen]; 
	if (NULL == pszDst) return std::string(""); 
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL); 
	pszDst[nLen -1] = 0; 
	std::string strTemp(pszDst); 
	delete [] pszDst; 
	return strTemp; 
}	
/*************************************************************
 函 数 名：MSToXJ()
 功能概要：CString对象字符串转化为CXJString字符串
 返 回 值: 转化成CXJString对象
 参    数：param1	要转化的CString对象
**************************************************************/
string MSToXJ(CString str)
{
	return (LPCSTR)str;
}

CTime StringToTimeWithMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMM 如:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//判断秒钟合法
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}
CTime StringToTimeWithoutMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//19位字符串格式为YYYY-MM-DD HH:MM:SS 如:2008-05-30 13:44:51
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//年
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//未找到
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//判断年份
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//月
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//判断月份合法
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//日
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//判断日期合法
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//时
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//判断小时合法
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//分
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//判断分钟合法
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//秒
	int nSec;
	strTemp = strTime;
	strTemp.Delete(0, pos+1);
	nSec = atoi(strTemp);
	//判断秒钟合法
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 函 数 名：StringToTime()
 功能概要：数据库中的时间字符串转化为时间.字符串格式为YYYY-MM-DD HH:MM:SS.MMMM
 返 回 值: 转化成的时间
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTime(CString& sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//字符串格式为YYYY-MM-DD HH:MM:SS.MMMM 如:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	strTime.TrimLeft();
	strTime.TrimRight();
	int nLength = strTime.GetLength();
	if(nLength <= 19)
	{
		//19位字符串格式为YYYY-MM-DD HH:MM:SS
		return StringToTimeWithoutMillisecond(strTime);
	}
	else if(nLength > 19)
	{
		return StringToTimeWithMillisecond(strTime);
	}
	return tmTime;
}

/*************************************************************
函 数 名：IsValidKeyString()
功能概要：判断一个带有关键字的字符串是否有效字符串
返 回 值: 有效返回TRUE, 无效返回FALSE
参    数：param1	字符串
		  Param2
**************************************************************/
BOOL IsValidKeyString(CString sKey)
{
	//判断数据有效性
	if(sKey == "")
		return FALSE;
	//判断是否有关键字
	int nFind1 = sKey.Find('$', 0);
	int nFind2 = -1;
	//没有关键字, 返回sKey
	if(nFind1 == -1)
		return FALSE;
	//检查$的个数是否双数
	int nCount = 1;
	while(nFind1 != -1)
	{
		nFind1 = sKey.Find('$', nFind1 + 1);
		if(nFind1 == -1)
			break;
		nCount++;
	}
	if(nCount < 2)
		return FALSE;
	int Residue = nCount%2;
	if(Residue != 0)
		return FALSE;
	return TRUE;
}

/*************************************************************
函 数 名：FindOutAllKey()
功能概要：找到语句中所有的关键字,并把关键字替换成%s
返 回 值: 格式化的语句
参    数：param1	关键字字符串
	      Param2	关键字数组
**************************************************************/
CString FindOutAllKey( CString sKey, CStringArray& arrString )
{
	//清空数组
	arrString.RemoveAll();
	CString strReturn = "";
	int nFind1 = -1;
	int nFind2 = -1;
	CString sTempKey = sKey;
	nFind1 = sTempKey.Find('$', 0);
	while(nFind1 != -1)
	{
		//查找第一个$位置
		nFind1 = sTempKey.Find('$', 0);
		if(nFind1 == -1)
			break;
		//查找与之对应的$的位置
		nFind2 = sTempKey.Find('$', nFind1 + 1);
		if(nFind2 == -1)
			break;
		//保存关键字到数组
		CString sTemp = sTempKey.Mid(nFind1, nFind2 - nFind1 + 1);
		arrString.Add(sTemp);
		//把关键字替换成%s
		sTempKey.Delete(nFind1, nFind2 - nFind1 + 1);
		sTempKey.Insert(nFind1, "%s");
	}
	strReturn = sTempKey;
	return strReturn;
}


/*************************************************************
 函 数 名：GetOscFilePath()
 功能概要：取得录波文件路径
 返 回 值: void
 参    数：param1	录波文件全路径
		   Param2
**************************************************************/
void GetOscFilePath(CString& sFullPath)
{
	CFile file;
	if ( file.Open(sFullPath, CFile::modeRead) == 0 )
	{
		if ( sFullPath.Find("Collection\\", 0) != -1 )
		{
			sFullPath.Replace("Collection\\", "");
		}
		else if ( sFullPath.Find("Collection/", 0) != -1 )
		{
			sFullPath.Replace("Collection/", "");
		}
		else
		{
			CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
			CString strDownDir = pApp->GetDownDir();
			int iIndex = sFullPath.Find(strDownDir, 0);
			if ( iIndex != -1 )
			{
				sFullPath.Insert(iIndex + strDownDir.GetLength(), "Collection\\");
			}
		}

		if ( file.Open(sFullPath, CFile::modeRead) == 0 )
		{
			sFullPath = _T("");
		}
		else
		{
			file.Close();
		}
	}
}

/*************************************************************
 函 数 名：OpenWaveFile()
 功能概要：打开录波文件
 返 回 值: void
 参    数：param1	录波文件全路径
		   Param2
**************************************************************/
void OpenWaveFile(CString sFullPath, HWND hWnd)
{
	sFullPath.Replace("/", "\\");
	CString strPath;
	strPath.Format("\"%s\"", sFullPath);
	//把文件名复制到剪贴板
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, strPath.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,strPath);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	else
	{
		WriteLog("打开剪贴板失败", XJ_LOG_LV2);
	}
	
	//查找是否已经有STWave在运行,如果已经运行则发送消息WM_OPEN_WAVEFILE
	if ( !EnumWindows(EnumWindowsProc, EW_STWAVE) )
	{
		WriteLog("已经有STWave在运行, 发送消息WM_OPEN_WAVEFILE", XJ_LOG_LV3);
		return;
	}

	/*
	GetOscFilePath(sFullPath);

	if (sFullPath.IsEmpty())
	{
		if ( AfxMessageBox("未能找到录波文件，是否手动查找?", MB_YESNO) == IDYES )
		{
			//取得当前系统路径
			DWORD cchCurDir = MAX_PATH; 
			LPTSTR lpszCurDir; 
			TCHAR tchBuffer[MAX_PATH];
			for(int i = 0; i < MAX_PATH; i++)
			{
				tchBuffer[i] = '\0';
			}
			lpszCurDir = tchBuffer; 
			GetCurrentDirectory(cchCurDir, lpszCurDir);

			CFileDialog dlgFile(TRUE, "cfg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
			dlgFile.m_ofn.lpstrTitle = _T("选择录波文件"); //对话框标题
			dlgFile.m_ofn.lpstrFilter =_T("*.cfg"); //过滤
			CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
			dlgFile.m_ofn.lpstrInitialDir = pApp->GetDownDir();
			if(dlgFile.DoModal() == IDOK)
			{
				sFullPath = dlgFile.GetPathName(); 
				WriteLog("录波文件路径为" + sFullPath);

				//设置当前目录
				SetCurrentDirectory(lpszCurDir);
			}
			else
			{
				//设置当前目录
				SetCurrentDirectory(lpszCurDir);
				return;
			}
		}
		else
		{
			return;
		}
	}
	*/
	
	//没有STWave在运行，则启动之，并将Comtrade文件名作为启动参数
	CString strDir = g_LocalPath;
	if(g_UseXJWave)
		strDir += PATH_XJWAVE;//"\\XJWave\\";
	else
		strDir += PATH_STWAVE;
	CString strLog;
	strLog.Format("录波分析软件目录为: %s", strDir);
	WriteLog(strLog, XJ_LOG_LV3);
	HINSTANCE iRet = ShellExecute( hWnd, "open", g_strOscName, strPath, strDir, SW_SHOWNORMAL );	
	if ( (int)iRet <= 32 )
	{
		AfxMessageBox(StringFromID(IDS_TIP_FAULTAPP_NOTFOUND));
	}
	else
	{
		WriteLog("打开录波文件成功", XJ_LOG_LV2);
	}
}

/*************************************************************
 函 数 名：OpenXJQuery()
 功能概要：打开事件查询与统计模块
 返 回 值: 
 参    数：param1	启动参数
		   Param2	打开窗口
**************************************************************/
void OpenXJQuery(CString sCmd, HWND hWnd)
{
	//把文件名复制到剪贴板
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, sCmd.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,sCmd);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	
	// 用应用程序名创建信号量 
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, "XJQuery"); 
	
	// 信号量已存在？ 
	// 信号量存在，则程序已有一个实例运行 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// 关闭信号量句柄 
		CloseHandle(hSem); 
		
		// 寻找先前实例的主窗口 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// 检查窗口是否有预设的标记? 
			// 有，则是我们寻找的主窗 
			if (::GetProp(hWndPrevious, "XJQuery")) 
			{ 
				// 主窗口已最小化，则恢复其大小 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				
				// 将主窗激活 
				::SetForegroundWindow(hWndPrevious); 
				
				// 将主窗的对话框激活 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				//通知XJQuery对指定装置进行事件查询
				::PostMessage(hWndPrevious, WM_QUERY_EVENT, 0, 0);

				// 退出 
				break; 
			} 
			
			// 继续寻找下一个窗口 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
	}
	else
	{
		// 关闭信号量句柄 
		CloseHandle(hSem); 

		//命名行中加上用户名密码
		CString sUserPass;
		sUserPass.Format("@%s/%s", theApp.m_User.m_strUSER_ID, theApp.m_User.m_strPassword);
		sCmd += sUserPass;
		
		CString strLog;
		strLog.Format("打开查询统计模块,传入的参数为:%s", sCmd);
		WriteLog(strLog, XJ_LOG_LV2);

		//没有在运行，则启动
		CString strDir = g_LocalPath;
		strDir += PATH_XJQUERY;
		HINSTANCE iRet = ShellExecute( hWnd, "open", "XJQuery.exe", sCmd, strDir, SW_SHOWNORMAL );	
		if ( (int)iRet <= 32 )
		AfxMessageBox(StringFromID(IDS_TIP_QUERYAPP_NOFOUND));
	}
}

/*************************************************************
 函 数 名: OpenXJAlarmQuery()
 功能概要: 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void OpenXJAlarmQuery( CString sCmd, HWND hWnd )
{
	//把文件名复制到剪贴板
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, sCmd.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,sCmd);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	
	// 用应用程序名创建信号量 
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, "XJAlarmQuery"); 
	
	// 信号量已存在？ 
	// 信号量存在，则程序已有一个实例运行 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// 关闭信号量句柄 
		CloseHandle(hSem); 
		
		// 寻找先前实例的主窗口 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// 检查窗口是否有预设的标记? 
			// 有，则是我们寻找的主窗 
			if (::GetProp(hWndPrevious, "XJAlarmQuery")) 
			{ 
				// 主窗口已最小化，则恢复其大小 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				
				// 将主窗激活 
				::SetForegroundWindow(hWndPrevious); 
				
				// 将主窗的对话框激活 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				//通知XJQuery对指定装置进行事件查询
				::PostMessage(hWndPrevious, WM_QUERY_ALARM, 0, 0);
				
				// 退出 
				break; 
			} 
			
			// 继续寻找下一个窗口 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
	}
	else
	{
		// 关闭信号量句柄 
		CloseHandle(hSem); 
		
		//没有在运行，则启动
		CString strDir = g_LocalPath;
		strDir += PATH_XJALARMQUERY;
		HINSTANCE iRet = ShellExecute( hWnd, "open", "XJAlarmQuery.exe", sCmd, strDir, SW_SHOWNORMAL );	
		if ( (int)iRet <= 32 )
			AfxMessageBox(StringFromID(IDS_TIP_ALARMSTATAPP_NOFOUND));
	}
}

void OpenXJFaultInversion(CString sFullPath, HWND hWnd)
{
	CString sTempPath = sFullPath;
	sTempPath.Replace(" ", "\" \"");
	CString strDir = g_LocalPath;
	strDir += PATH_XJFAULTINVERSION;
	HINSTANCE iRet = ShellExecute( hWnd, "open", "XJFaultInversion.exe", sTempPath, strDir, SW_SHOWNORMAL );	
	if ( (int)iRet <= 32 )
			AfxMessageBox(StringFromID(IDS_TIP_PLAYBACKAPP_NOTFOUND));
}

CString QueryDIName( CString sPTID, int nDIID, int nCPU )
{
	CString sReturn = "";
	//组建查询条件
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//要查询的字段
	//开关量Name
	Field Name;
	bzero(&Name, sizeof(Field));
	str = "Name";
	strncpy(Name.FieldName, str, str.GetLength());
	Name.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Name);
	
	//条件
	Condition PT_ID;
	bzero(&PT_ID, sizeof(Condition));
	str.Format("pt_id = '%s'", sPTID);
	strncpy(PT_ID.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(PT_ID);
	
	Condition CPU;
	bzero(&CPU, sizeof(Condition));
	str.Format("cpu_code = %d", nCPU);
	strncpy(CPU.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(CPU);
	
	Condition DI;
	bzero(&DI, sizeof(Condition));
	str.Format("di_id = %d", nDIID);
	strncpy(DI.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(DI);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			sReturn = pMemset->GetValue((UINT)0);
		}
	}
	else
	{
		CString str;
		str.Format("QueryDIName,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return sReturn;
}


CString GetSituationDescribe(int nSituationNo)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if( !pData )
		return "";
	if( nSituationNo < 0 || nSituationNo >= pData->m_arrSituation.GetSize() )
		return "";
	return pData->m_arrSituation.GetAt(nSituationNo);
	return StringFromID(IDS_COMMON_UNKNOWN);
}


CString GetDetailSituation(CString sSituation)
{
	CString sReturn = "";
	if( sSituation.IsEmpty() )
		return "";
	int nfind = sSituation.Find( ',', 0 );
	if( nfind < 0 )
	{
		return GetSituationDescribe( atoi(sSituation) );
	}
	else
	{
		
		while( nfind > 0 )
		{
			CString stemp = sSituation;
			while( nfind > 0 )
			{
				CString str = stemp.Left( nfind );
				stemp.Delete( 0, nfind+1 );
				if( !str.IsEmpty() )
				{
					sReturn += GetSituationDescribe( atoi(str) );
					sReturn += " | ";
				}
				nfind = stemp.Find( ',', 0 );
			}
			if( !stemp.IsEmpty() )
			{
				//最后一个
				CString str = stemp;
				if( !str.IsEmpty() )
					sReturn += GetSituationDescribe( atoi(str) );
			}
		}
	}
	return sReturn;
}


CString GetDisposeOpinion(int nOpinion)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if( !pData )
		return "";
	if( nOpinion < 0 || nOpinion >= pData->m_arrDispose.GetSize() )
		return "";
	return pData->m_arrDispose.GetAt(nOpinion);
	return "";
}


CString GetDetailDispose(CString sDispose)
{
	CString sReturn = "";
	if( sDispose.IsEmpty() )
		return StringFromID(IDS_COMMON_NONEED_DEAL);
	int nfind = sDispose.Find( ',', 0 );
	if( nfind < 0 )
	{
		return GetDisposeOpinion( atoi(sDispose) );
	}
	else
	{
		CString stemp = sDispose;
		while( nfind > 0 )
		{
			CString str = stemp.Left( nfind );
			stemp.Delete( 0, nfind+1 );
			if( !str.IsEmpty() )
			{
				sReturn += GetDisposeOpinion( atoi(str) );
				sReturn += " | ";
			}
			nfind = stemp.Find( ',', 0 );
		}
		if( !stemp.IsEmpty() )
		{
			//最后一个
			CString str = stemp;
			if( !str.IsEmpty() )
				sReturn += GetDisposeOpinion( atoi(str) );
		}
	}
	return sReturn;
}

CSize OpenReportFile(IXJReport*& pDoc, CString filename)
{
	CSize szDoc;
	szDoc.cx = szDoc.cy = 0;
	if( !pDoc )
		return szDoc;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//得到系统下载路径
	CString strDir = pApp->GetDownDir();
	//组建全路径
	strDir += filename;
	
	if(strDir != "")
	{
		//把有可能的/都换成"\\"
		strDir.Replace("/", "\\");
	}
	
	
	
	
	szDoc = pDoc->OpenReportFile(strDir);
	if(szDoc.cx > 0 || szDoc.cy > 0)
	{		
	}
	else
	{
		strDir = g_LocalPath + filename;
		szDoc = pDoc->OpenReportFile(strDir);
	}
	return szDoc;
}

CString GetDurationFormatStr()
{
	CString sformat;
	CString sday;
	sday.LoadString(IDS_COMMON_DAY);
	CString shour;
	shour.LoadString(IDS_COMMON_HOUR);
	CString smin;
	smin.LoadString(IDS_COMMON_MINUTE);
	sformat.Format("%%D%s %%H%s %%M%s", sday,shour,smin);
	return sformat;
}

CString StringFromID(UINT id)
{
	HINSTANCE old = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	CString str="";
	str.LoadString(id);
	AfxSetResourceHandle(old);
	return str;
}


/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp

BEGIN_MESSAGE_MAP(CXJBrowserApp, CWinApp)
	//{{AFX_MSG_MAP(CXJBrowserApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_STTP_CONNECT, OnSttpConnect)
	ON_UPDATE_COMMAND_UI(ID_STTP_CONNECT, OnUpdateSttpConnect)
	ON_COMMAND(ID_STTP_DISCONNECT, OnSttpDisconnect)
	ON_UPDATE_COMMAND_UI(ID_STTP_DISCONNECT, OnUpdateSttpDisconnect)
	ON_COMMAND(ID_SYS_LOGOUT, OnSysLogout)
	ON_UPDATE_COMMAND_UI(ID_SYS_LOGOUT, OnUpdateSysLogout)
	ON_COMMAND(ID_SYS_EXIT, OnSysExit)
	ON_UPDATE_COMMAND_UI(ID_SYS_EXIT, OnUpdateSysExit)
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENT, OnUpdateHelpContent)
	ON_COMMAND(ID_SYS_SETTING, OnSysSetting)
	ON_UPDATE_COMMAND_UI(ID_SYS_SETTING, OnUpdateSysSetting)
//	ON_COMMAND(ID_APP_CALL, OnAppCall)
	ON_COMMAND(ID_CALL_ALL, OnCallAll)
	ON_COMMAND(ID_FILE_DOWNLOAD, OnFileDownload)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp construction

//##ModelId=49B87B7D0224
CXJBrowserApp::CXJBrowserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bExitClearConfig = FALSE;
	m_bExitReceiveMsg = FALSE;
	m_bConnect = TRUE;
	m_sDownDir = "";
	m_pClearConfigThread = NULL;
	m_pReceiveMsgThread = NULL;
	m_pPostMsgThread = NULL;
	m_pPushMsgThread = NULL;
	m_pFaultDetail = NULL;
	m_pMapView = NULL;
	hSem = NULL;
	m_pPTSetting = NULL;
	m_pPTAnalog = NULL;
	m_pPTDigital = NULL;
	m_pPTSoftBoard = NULL;
	m_pWRSetting = NULL;
	m_pPTAction = NULL;
	m_pPTAlarm = NULL;
	m_pAutoCallResult = NULL;
	m_pComm = NULL;
	m_pPTComm = NULL;
	m_pPTRun = NULL;
	m_sSysLogPath = "";
	m_sNetworkUser = "";
	m_sNetworkPassword = "";
	m_pStationInfo = NULL;
	m_bGetSysLogPath = FALSE;
	m_listFullData.RemoveAll();
	m_hPostEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hReceiveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_CriticalSection);

	m_bCanLock = FALSE;
	m_bIsLocked = FALSE;

	m_ConnectStatus[0] = -1;
	m_ConnectStatus[1] = -1;
}

/*************************************************************
 函 数 名: TranslateKeyInPTRun()
 功能概要：为主子站通信状态页面解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTRun(CString sKey, CString &sValue)
{
	if(theApp.m_pPTRun == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewPTRun* pView = theApp.m_pPTRun;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名: TranslateKeyInStationInfo()
 功能概要：为厂站信息页面解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInStationInfo(CString sKey, CString &sValue)
{
	if(theApp.m_pStationInfo == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewStationInfo* pView = theApp.m_pStationInfo;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名: TranslateKeyInPTComm()
 功能概要：为主子站通信状态页面解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTComm(CString sKey, CString &sValue)
{
	if(theApp.m_pPTComm == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewPTComm* pView = theApp.m_pPTComm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInStationComm()
 功能概要：为主子站通信状态页面解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInStationComm(CString sKey, CString &sValue)
{
	if(theApp.m_pComm == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewComm* pView = theApp.m_pComm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInPTSetting()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTAnalog(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAnalog == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAnalog* pView = theApp.m_pPTAnalog;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInPTSetting()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTDigital(CString sKey, CString &sValue)
{
	if(theApp.m_pPTDigital == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTDigital* pView = theApp.m_pPTDigital;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInPTSetting()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTSoftBoard(CString sKey, CString &sValue)
{
	if(theApp.m_pPTSoftBoard == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSoftBoard* pView = theApp.m_pPTSoftBoard;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInPTSetting()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInWRSetting(CString sKey, CString &sValue)
{
	if(theApp.m_pWRSetting == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSetting* pView = theApp.m_pWRSetting;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 函 数 名：TranslateKeyInWRDigital()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInWRDigital(CString sKey, CString &sValue)
{
	if(theApp.m_pWRDigital == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTDigital* pView = theApp.m_pWRDigital;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 函 数 名：TranslateKeyInWRAnalog()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInWRAnalog(CString sKey, CString &sValue)
{
	if(theApp.m_pWRAnalog == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAnalog* pView = theApp.m_pWRAnalog;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 函 数 TranslateKeyInPTAlarm()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTAlarm(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAlarm == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAlarm* pView = theApp.m_pPTAlarm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 TranslateKeyInPTAction()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInAutoCallResult(CString sKey, CString &sValue)
{
	if(theApp.m_pAutoCallResult == NULL)
	{
		sValue = "";
		return 0;
	}
	CAutoCallResult* pView = theApp.m_pAutoCallResult;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 TranslateKeyInPTAction()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTAction(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAction == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAction* pView = theApp.m_pPTAction;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInPTSetting()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInPTSetting(CString sKey, CString & sValue)
{
	if(theApp.m_pPTSetting == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSetting* pView = theApp.m_pPTSetting;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：TranslateKeyInFaultDetail()
 功能概要：为故障报告详细信息解释关键字的回调函数
 返 回 值: 
 参    数：param1	要解释的关键字
		   Param2	解释后的语句
**************************************************************/
int TranslateKeyInFaultDetail(CString sKey, CString & sValue)
{
	if(theApp.m_pFaultDetail == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewFaultDetail* pView = theApp.m_pFaultDetail;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//判断数据有效性
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 函 数 名：UpdateBlinkRect()
 功能概要：刷新闪烁图形区域
 返 回 值: 
 参    数：param1	闪烁图形范围
		   Param2
**************************************************************/
int UpdateBlinkRect(CRect rcBlink)
{
	//检查视图是否有效
	if(theApp.m_pMapView == NULL)
		return 0;
	//刷新
	CRect rc = rcBlink;
	theApp.m_pMapView->DocToClient(rc);
	theApp.m_pMapView->InvalidateRect(rc);
	return 1;
}

/*************************************************************
 函 数 名：ReceiveSTTPConnectStatus()
 功能概要：与通讯服务器连接状态变化处理
 返 回 值: 
 参    数：param1	状态变化的连接名柄
		   Param2	新状态
		   param3	组号
**************************************************************/
int ReceiveSTTPConnectStatus(XJHANDLE pClientHandle , int pStatus , int group)
{
	if(pClientHandle != theApp.m_SttpHandle)
	{
		//返回的不是本连接的状态
		WriteLog("得到了错误连接的状态", XJ_LOG_LV2);
	}
	CString str;
	str.Format("与通信服务器连接状态变化,状态:%d,组号:%d", pStatus, group);
	theApp.WriteLog(str, XJ_LOG_LV3);
			
	theApp.SetSTTPStatus( 1==pStatus, group);

	return 1;
}


/*************************************************************
 函 数 名：AckPTAllEvent()
 功能概要：确认指定保护的所有事件
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
 参    数：param1	指定数据表标识
		   Param2	保护ID
**************************************************************/
BOOL AckPTAllEvent(int nTable, CString sID)
{
	BOOL bReturn = TRUE;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	try
	{
		//到数据库修改Is_Ack字段
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");

		//条件
		Condition con1;
		str.Format("pt_id='%s'", sID);
		pApp->m_DBEngine.SetCondition(sql, con1, str);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJUpdateData(nTable, sql, sError);
		}
		catch (...)
		{
			WriteLog("AckPTAllEvent, 更新失败", XJ_LOG_LV3);
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
		}
		else
		{
			CString str;
			str.Format("AckPTAllEvent,更新失败,原因为%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
		delete[] sError;
		sError = NULL;
		return FALSE;
	}
	
	return bReturn;
}

/*************************************************************
 函 数 名：AckAllEvent()
 功能概要：确认数据库中所有事件
 返 回 值: 确认成功返回TRUE, 失败返回FALSE
 参    数：param1	数据库表标识
		   Param2	事件类型
**************************************************************/
BOOL AckAllEvent( int nTable, int nEventType /*= -1*/ )
{
	BOOL bReturn = TRUE;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	try
	{
		//到数据库修改Is_Ack字段
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//更新IS_ACK
		
		CString str;	
		//字段
		//is_ack
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");

		//条件
		Condition con1;
		if(nEventType >= 0 && nEventType < 3)
		{
			str.Format("property=%d", nEventType);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJUpdateData(nTable, sql, sError);
		}
		catch (...)
		{
			WriteLog("AckAllEvent, 更新失败", XJ_LOG_LV3);
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
		}
		else
		{
			CString str;
			str.Format("AckAllEvent,更新失败,原因为%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
		delete[] sError;
		sError = NULL;
		return FALSE;
	}
	
	//确认成功,停止设备闪烁
	if(bReturn)
	{
		if(g_BlinkDevice)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CXJBrowserView * pView = pApp->GetSVGView();
			if(pView != NULL)
			{
				if(EX_STTP_INFO_PT_ACTION_DATA == nTable || EX_STTP_INFO_PT_ALARM_DATA == nTable)
				{
					//动作或告警
					//停止设备闪烁
					pView->RemoveAllBlinkDevices();
				}
			}
		}
	}
	
	return bReturn;
}

/*************************************************************
 函 数 名：NeedShow()
 功能概要：判断指定类型的事件是否需要显示
 返 回 值: 需要显示返回TRUE, 不需要显示返回FALSE
 参    数：param1	指定事件类型. 0-I类事件, 1-II类事件, 2-III类事件
		   Param2
**************************************************************/
BOOL NeedShow(int nEventType)
{
	if(0 == nEventType)
	{
		//I类事件
		if(1 == g_ShowEventI)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(1 == nEventType)
	{
		//II类事件
		if(1 == g_ShowEventII)
			return TRUE;
		else
			return FALSE;
	}
	else if(2 == nEventType)
	{
		//III类事件
		if(1 == g_ShowEventIII)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

/*************************************************************
 函 数 名：NeedAlarm()
 功能概要：判断指定类型的事件是否需要报警
 返 回 值: 需要报警返回TRUE, 不需要报警返回FALSE
 参    数：param1	指定事件类型. 0-I类事件, 1-II类事件, 2-III类事件
		   Param2
**************************************************************/
BOOL NeedAlarm(int nEventType)
{
	if(0 == nEventType)
	{
		//I类事件
		if(1 == g_AlarmEventI)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(1 == nEventType)
	{
		//II类事件
		if(1 == g_AlarmEventII)
			return TRUE;
		else
			return FALSE;
	}
	else if(2 == nEventType)
	{
		//III类事件
		if(1 == g_AlarmEventIII)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}
/*************************************************************
 函 数 名: GetStationStatusString()
 功能概要: 根据状态代码取得通讯状态描述字符串
 返 回 值: 通讯状态描述字符串
 参    数: param1 状态代码
		   Param2 
**************************************************************/
CString GetStationStatusString( int nStatus )
{
	if(nStatus == 0)
	{
		//断开
		return StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else if(nStatus == 1)
	{
		//正常
		return StringFromID(IDS_COMMUSTATUS_ON);
	}
	else
	{
		return StringFromID(IDS_COMMON_UNKNOWN);
	}
}

/*************************************************************
 函 数 名: GetCommStatusString()
 功能概要: 根据状态代码取得通讯状态描述字符串
 返 回 值: 通讯状态描述字符串
 参    数: param1 状态代码
		   Param2 
**************************************************************/
CString GetCommStatusString( int nStatus )
{
	if(nStatus == 0)
	{
		//正常
		return StringFromID(IDS_COMMUSTATUS_ON);
	}
	else if(nStatus == 1)
	{
		//断开
		return StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else
	{
		return StringFromID(IDS_COMMON_UNKNOWN);
	}
}

/*************************************************************
 函 数 名: GetRunStatusString()
 功能概要: 根据状态代码取得运行状态描述字符串
 返 回 值: 运行状态描述字符串
 参    数: param1 状态代码
		   Param2 
**************************************************************/
CString GetRunStatusString( int nStatus )
{
	CString sReturn = "";
	switch(nStatus)
	{
	case 0:
		sReturn = StringFromID(IDS_RUNSTATUS_OVERHAULS);
		break;
	case 1:
		sReturn = StringFromID(IDS_RUNSTATUS_OUTAGE);
		break;
	case 2:
		sReturn = StringFromID(IDS_RUNSTATUS_SERVICE);
		break;
	case 3:
		sReturn = StringFromID(IDS_RUNSTATUS_NOACCESS);
		break;
	case 4:
		sReturn = StringFromID(IDS_RUNSTATUS_TESTING);
		break;
	default:
		sReturn = StringFromID(IDS_COMMON_UNKNOWN);
	}
	return sReturn;
}

int GetOperationNum()
{
	int Num = -1;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	CString str;
	str.Format("%d", 1);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);
	
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	//str.Format("order by CAST(Reserve1 AS int) DESC");
	str.Format("order by CAST(Reserve1 AS SIGNED) DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OPERATION_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("查询操作日志操作号, 查询失败");
		delete[] sError;
		delete pMemset;
		return 0;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str;
		//取第一个
		str = pMemset->GetValue((UINT)0); //ID
		if (str == "")
		{
			Num = 0;
		}
		else
		{
			Num = atoi(str) + 1;
		}
	}
	else
	{
		CString str;
		str.Format("查询操作日志操作号,查询失败,原因为%s", sError);
		WriteLogEx(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return Num;
}

bool IsWin7()  
{  
    OSVERSIONINFOEX osvi;  
    BOOL bOsVersionInfoEx;  
    
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));  
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
    bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);  
    
    // win7的系统版本为NT6.1 
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 0)  
    {  
        return true;      
    }  
    else  
    {  
        return false;  
    }  
}

typedef LANGID (WINAPI *FP_SetThreadUILanguage)(LANGID LangId);
LANGID SetThreadUILanguage(LANGID lcid)
{
    HMODULE hKernel32 = GetModuleHandle(_T("Kernel32.dll"));
    FARPROC pFn = GetProcAddress(hKernel32, "SetThreadUILanguage");
    
    FP_SetThreadUILanguage pSetThreadUILanguage = (FP_SetThreadUILanguage)pFn;
	
    if (pSetThreadUILanguage)
    {
        return pSetThreadUILanguage(lcid);
    }
    return 0;
} 

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp initialization

//##ModelId=49B87B7E0262
BOOL CXJBrowserApp::InitInstance()
{
	LONGLONG startTime = StartCount();
/*
	// 用应用程序名创建信号量 
	hSem = CreateSemaphore(NULL, 1, 1, "XJBrowser"); 
	
	// 信号量已存在
	// 信号量存在，则程序已有一个实例运行 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// 关闭信号量句柄 
		CloseHandle(hSem); 
		hSem = NULL;
		
		// 寻找先前实例的主窗口 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// 检查窗口是否有预设的标记? 
			// 有，则是我们寻找的主窗 
			if (::GetProp(hWndPrevious, "XJBrowser")) 
			{ 
				// 主窗口已最小化，则恢复其大小 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				// 将主窗激活 
				::SetForegroundWindow(hWndPrevious); 
				// 将主窗的对话框激活 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				// 退出本实例 
				AfxMessageBox("程序已经在运行!");
				return FALSE; 
			} 
			// 继续寻找下一个窗口 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
		// 前一实例已存在，但找不到其主窗 
		//检查是否存在僵尸进程
		if ( !CheckSystemProcess())
		{
			return FALSE;
		} 
	}
*/

	
	//连接数据库
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	if(m_DBEngine.XJConnect(STTPNET_MODLE_XJBROWSER, sError) == 0)
	{
		//连接失败
		CString str;
		str.Format("%s\r\n%s:%s", StringFromID(IDS_DBSERVER_DISCONNECT),StringFromID(IDS_REASON_FOR),sError);
		WriteLog(str);
		AfxMessageBox(str);
		delete[] sError;
		return FALSE;
	}
	memset(sError, '\0', MAXMSGLEN);
	//WriteLog("连接数据库成功");

	//必须写不然无法识别
	m_brush.CreateSolidBrush(RGB(231,229,236));//你的颜色
	memset(&lpcmi,0,sizeof(::LPCMENUINFO));
	lpcmi.cbSize=sizeof(MENUINFO);
	lpcmi.fMask=   MIM_BACKGROUND;   
	lpcmi.hbrBack=(HBRUSH)m_brush.operator HBRUSH();


	//计算路径
	char   m_szLocalFileName[255]; //得到当前执行文件全名称
	for(int i = 0; i < 255; i++)
	{
		m_szLocalFileName[i] = '\0';
	}
	::GetModuleFileName(NULL,m_szLocalFileName,255);

	CString   m_szLocalPath = CString(m_szLocalFileName,255);   
	CString   m_szTemp; 
	while(TRUE) //去掉文件名称得到文件路径
	{   
		m_szTemp=m_szLocalPath.Right(1);   
		if   (m_szTemp != "\\")   
		{   
			m_szLocalPath=m_szLocalPath.Left(m_szLocalPath.GetLength()-1);
			
		}   
		else   
		{   
			break;   
		}   
	}
	g_LocalPath = m_szLocalPath;
	g_UIConfig = g_LocalPath + UI_File;
	g_SysConfig = g_LocalPath + INI_File;
	g_SoundFile = g_LocalPath + SOUND_File;
	g_ImageFile = g_LocalPath + IMAGE_FILE;

	//日志文件根路径
	m_sSysLogPath = GetSysLogPath();
	g_LogPath = m_sSysLogPath;
	m_bGetSysLogPath = TRUE;
	char* pPath = (char*)m_sSysLogPath.GetBuffer(0);
	if(m_DBEngine.XJSetLogPath(pPath, sError) == 0)
	{
		//失败
		CString str;
		str.Format("设置日志路径失败, 路径为:%s", m_sSysLogPath);
		WriteLog(str);
	}
	
	delete[] sError;

	//用于Drag&Drop
	g_dwESDataFormat = ::RegisterClipboardFormat(_T("ESDATAFORMAT"));//注册自定义剪贴板类型

	//载入系统配置
	LoadConfig();

	//设置日志文件路径
	m_LogMgr.SetLogPath(g_LogPath);

	m_sSysLogPath.ReleaseBuffer();
	
	m_SttpEngine.LoadSttpLib();

	//判断日志表类型
	JudgeOperTableType();

	//检查UI配置文件是否存在
	CFileStatus   status;	   
	if(CFile::GetStatus(g_UIConfig ,status) != TRUE)
	{
		//UI配置文件不存在,创建一个新文件
		CFile File;
		if(File.Open(g_UIConfig, CFile::modeCreate | CFile::modeReadWrite))
		{
			CString str;
			str.Format("<?xml version=\"1.0\"?>\r\n<UIConfig>\r\n</UIConfig>");
			char * pBuffer = str.GetBuffer(0);
			File.Write(pBuffer, str.GetLength());
			str.ReleaseBuffer();
		}
		else
		{
			TRACE("open uiconfig faild or wirte faild\n");
		}		
	}



	//设置语言
	if( IsWin7() )
	{
		if( g_Language.CompareNoCase("en_US") == 0 )
			SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		if( g_Language.CompareNoCase("en_US") == 0 )
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	}


	CANCEL_CALL = StringFromID(IDS_BTN_CANCEL_CALL);
	CANCEL_NULL = StringFromID(IDS_CANCEL_NULL);
	STTP_DISCONNECT = StringFromID(IDS_STTP_DISCONNECT);
	


	if (g_bLoginVerify)
	{
		CXJUserStore::GetInstance()->ReLoad();
		//CXJUserStore::GetInstance()->Save("c:/tb_sys_user.txt");

		//用户身份验证
		if (!DoLogin())
		{
			SaveSysConfig();
			return FALSE;
		}
		QueryUserStation();
	}

	//在此添加数据读入中等待窗口
	
	g_wndWait.m_bDrawDetail = TRUE;
	if (g_style == 1)
	{	
		g_wndWait.LoadBitmapEx(/*IDB_BMP_LOGIN*/IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else if (g_style == 2)
	{
		g_wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else if (g_style == 3)
	{
		g_wndWait.LoadBitmapEx(g_strStartBmp, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else
	{
		g_wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	
	g_wndWait.Show(SPLASH_NOTIMER);
	

	//正常启动程序
	AfxEnableControlContainer();

	//初始化COM
	if(!AfxOleInit())
		return FALSE;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("XJBrowser"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.


	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_MOUNT));
	//挂载网络存储器
	AddNetworkDisk();
	//加载各种文档
	InitDocTemplates();	

	//为了启动STWave
	strcpy( g_arrStrTable[0], "IPOFAS-故障分析中心" );

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	
	//设置状态栏
	CTime tm;
	CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strTemp;
	strTemp.Format("[%s] %s[%s] %s", strTime, StringFromID(IDS_COMMON_USER),m_User.m_strUSER_ID, StringFromID(IDS_COMMON_LOGIN));
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_LOGINTIP, (LPARAM)strTemp.GetBuffer(100));
		strTemp.ReleaseBuffer();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;//新建文档
	ParseCommandLine(cmdInfo);
	
	if (g_role != MODE_SUB)
	{
		m_pDocTempl[wtCommuView]->OpenDocumentFile(NULL);
	}
	else
	{
		m_pDocTempl[wtMap]->OpenDocumentFile(NULL);
	}

	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_DEVICETREE));
	//设置设备树窗口内容
	pMainFrame->m_pDeviceView->FillTree();

	ParseCommandLine(cmdInfo);
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	//取得主子站通讯状态窗口指针

	CFrameWnd * pChild = pMainFrame->GetActiveFrame();
	if (g_role != MODE_SUB)
	{
		if(pChild != NULL)
		{
			if(pChild->IsKindOf(RUNTIME_CLASS(CCommFrame)))
			{
				pMainFrame->pCommFrame = (CCommFrame*)pChild;
			}
		}
	}
	else
	{
		if(pChild != NULL)
		{
			if(pChild->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			{
				pMainFrame->pMapFrame = (CChildFrame*)pChild;
			}
		}
	}



	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_DEFAULTSTATION));
	//打开默认厂站
	if(g_role != MODE_SUB)
	{
		OpenDefaultStation(g_DefaultStation);
	}
	else
	{
		OpenDefaultStation();
	}
	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_LOADDATA_COMPLETED));
	g_wndWait.Hide();//数据读入等待窗口关闭
	// The main window has been initialized, so show and update it.
	m_nCmdShow = SW_MAXIMIZE;XJSetMenuInfo();
	pMainFrame->ShowWindow(SW_MAXIMIZE);	
	pMainFrame->SetMenu(NULL);
	int result = ::SetForegroundWindow(GetMainWnd()->m_hWnd);

    if (g_bConnectCommServer)
    {
		//建立与通讯服务器的连接
		ConnectSTTPServer();
	}

	StartClearConfigThread();
	

	EndCount("系统启动时间为", startTime);

	m_bCanLock = TRUE;
	m_bIsLocked = FALSE;

	if(g_UILockTime > 0)
		m_pMainWnd->PostMessage(STARTLOCK);

	return TRUE;
}

/*************************************************************
 函 数 名：InitDocTemplates()
 功能概要：初始化各文档类型
 返 回 值: 
**************************************************************/
//##ModelId=49B87B7D037B
void CXJBrowserApp::InitDocTemplates()
{
	//主接线图窗口
	CMultiDocTemplate *pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWMAP,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CXJBrowserView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtMap]=pDocTemplate;

	if (g_role != MODE_SUB)
	{
		//主子站通讯状态窗口
		pDocTemplate = new CMultiDocTemplate(
			//IDR_XJBROWTYPE,
			1,
			RUNTIME_CLASS(CXJBrowserDoc),
			RUNTIME_CLASS(CCommFrame), // custom MDI child frame
			RUNTIME_CLASS(CViewCommMap));
		AddDocTemplate(pDocTemplate);
		m_pDocTempl[wtCommuView]=pDocTemplate;
	}
	
	//动作事件窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CActionFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewAction));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAction]=pDocTemplate;
	
	 //告警窗口
	 pDocTemplate = new CMultiDocTemplate(
	 //IDR_XJBROWTYPE,
	 1,
	 RUNTIME_CLASS(CXJBrowserDoc),
	 RUNTIME_CLASS(CAlarmFrame), // custom MDI child frame
	 RUNTIME_CLASS(CViewAlarm));
	 AddDocTemplate(pDocTemplate);
	 m_pDocTempl[wtAlarm]=pDocTemplate;
	  
	 if (g_role != MODE_SUB)
	 {
		 //事故报告窗口
		 pDocTemplate = new CMultiDocTemplate(
			 //IDR_XJBROWTYPE,
			 1,
			 RUNTIME_CLASS(CXJBrowserDoc),
			 RUNTIME_CLASS(CFaultFrame), // custom MDI child frame
			 RUNTIME_CLASS(CViewFaultSheet));
		 AddDocTemplate(pDocTemplate);
		 m_pDocTempl[wtFault]=pDocTemplate;
	 }
		
	//录波事件窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewOSC));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtOSC]=pDocTemplate;
		  
	//开关量事件窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewDI));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDI]=pDocTemplate;
			
	//保护通讯状窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewPTComm));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTCommStatus]=pDocTemplate;
			  
	//保护运行状态窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewPTRun));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTRunStatus]=pDocTemplate;
				
	//系统告警窗口
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CSysAlarmFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewSysAlarm));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSysAlarm]=pDocTemplate;

	//保护属性窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CPTFrame), // custom MDI child frame
		RUNTIME_CLASS(CPTSheet));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTSheet]=pDocTemplate;

	//录波器属性窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CWaveRecFrame), // custom MDI child frame
		RUNTIME_CLASS(CPTSheet));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtWaveRecSheet]=pDocTemplate;

	//批量召唤窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CBatchFrame), // custom MDI child frame
		RUNTIME_CLASS(CBatchSelectView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtBatchCall]=pDocTemplate;

	//自动总召设置窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CAutoCallFrame), // custom MDI child frame
		RUNTIME_CLASS(CBatchSelectView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAutoCall]=pDocTemplate;

	//自动总召结果窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CAutoResultFrame), // custom MDI child frame
		RUNTIME_CLASS(CACResultSel));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAutoCallResult]=pDocTemplate;

	//通用文件下载窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CDownloadFrame), // custom MDI child frame
		RUNTIME_CLASS(CDownLoadView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDownload]=pDocTemplate;

	//预警窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CCrossFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewCross));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtCross]=pDocTemplate;

	//I类事件窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEI]=pDocTemplate;

	//II类事件窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEII]=pDocTemplate;

	//III类事件窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEIII]=pDocTemplate;

	//调试信息窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEDebug]=pDocTemplate;

	//地理图窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGeoView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtGeo]=pDocTemplate;

	//厂站信息窗口
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CStationInfoFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewStationInfo));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtStationInfo]=pDocTemplate;

	//录波下载页面
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CDownOscFrame), // custom MDI child frame
		RUNTIME_CLASS(CDownOscView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDownOsc]=pDocTemplate;

	//小波测距页面
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewDistance));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDistance]=pDocTemplate;

	//施工图页面
	pDocTemplate = new CMultiDocTemplate(
		IDR_MENU_DRAFT,
		RUNTIME_CLASS(CDraftDocument),
		RUNTIME_CLASS(CDraftFrame), // custom MDI child frame
		RUNTIME_CLASS(CDraftList));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDraft]=pDocTemplate;
}

/*************************************************************
 函 数 名：GetCurDocument()
 功能概要：找到当前活动文档
 返 回 值: 当前文档指针
**************************************************************/
//##ModelId=49B87B7E000F
CXJBrowserDoc * CXJBrowserApp::GetCurDocument()
{
	//得到主框架指针
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return NULL;

	return pFrame->m_pDoc;
}

/*************************************************************
 函 数 名：GetCurView()
 功能概要：得到当前活动视图
 返 回 值: 当前活动视图指针
**************************************************************/
//##ModelId=49B87B7E002E
CView * CXJBrowserApp::GetCurView()
{
	//得到主框架指针
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return NULL;
	//得到当前活动子框架指针
	CChildFrame * pChildFrame = (CChildFrame *)pFrame ->GetActiveFrame();
	if(pChildFrame == NULL)
		return NULL;
	//得到当前活动文档指针
	CView * pView = pChildFrame->GetActiveView();
	if(pView == NULL)
		return NULL;
	return pView;
}

//##ModelId=49B87B7E005D
void CXJBrowserApp::WriteLog(CString sLog, int nLV)
{
	m_LogMgr.WriteLog(sLog, nLV);
}

//##ModelId=49B87B7E004E
CDataEngine* CXJBrowserApp::GetDataEngine()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	if(pDoc == NULL)
		return NULL;
	return &(pDoc->m_DataEngine);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//##ModelId=49B87BB603D8
class CAboutDlg : public CDialog
{
public:
	//##ModelId=49B87BB7000F
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	//##ModelId=49B87BB7003E
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//##ModelId=49B87BB7000F
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//##ModelId=49B87BB7003E
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
//##ModelId=49B87B7E0280
void CXJBrowserApp::OnAppAbout()
{
// 	CAboutDlg aboutDlg;
// 	aboutDlg.DoModal();
	CAboutDlgEx dlg;
	dlg.DoModal();
}

/*************************************************************
 函 数 名：STTPConnect()
 功能概要：建立与通讯服务器的连接
 返 回 值: 创建的Sttp客户端对象的句柄，失败时返回NULL
 参    数：param1	运行模式. 0：同步模式（默认）, 1：异步模式
**************************************************************/
//##ModelId=49B87B7E009C
XJHANDLE CXJBrowserApp::STTPConnect(int& pConnected, int pRunModle /*= 0*/ )
{
	//设置通讯日志路径
	if (!m_bGetSysLogPath)
	{
		m_sSysLogPath = GetSysLogPath();
		g_LogPath = m_sSysLogPath;
	}
	char * pPath = (char*)g_LogPath.GetBuffer(0);
	
	//客户端类型不可改
	return m_SttpEngine.XJSttpConnectServer(STTPNET_MODLE_XJBROWSER, pConnected, pPath, pRunModle);
}

/*************************************************************
 函 数 名：DoLogin()
 功能概要：登录系统, 验证用户身份
 返 回 值: 登录成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7D0399
BOOL CXJBrowserApp::DoLogin()
{
	CDlgValidateID dlg;
	//dlg.m_strUser = m_User.m_strUSER_ID;
	//dlg.m_strPassword = m_User.m_strPassword;
	if (dlg.DoModal() == IDOK)
	{
		m_bIsLocked = FALSE;

		//设置m_user变量
		m_User.m_strUSER_ID	= dlg.m_strUser;
		m_User.m_strGROUP_ID= dlg.m_strUserGroup;
		m_User.m_strPassword= dlg.m_strPassword;

		
		//设置状态栏
		CTime tm;
		CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		CString strTemp, strMsg;
		strTemp.Format("[%s] %s[%s] %s", strTime, StringFromID(IDS_COMMON_USER),m_User.m_strUSER_ID, StringFromID(IDS_COMMON_LOGIN));
		strMsg = strTemp;
		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_LOGINTIP, (LPARAM)strTemp.GetBuffer(100));
		strTemp.ReleaseBuffer();

		//添加人工操作日志
		AddNewManOperator(FUNC_XJBROWSER_BROWSER, "用户登陆", strMsg, -1);

		//SetUserLoginFlag(m_User.m_strUSER_ID, m_User.m_strGROUP_ID, CString("1"));
		CXJUserStore::GetInstance()->SetUserFlags(m_User.m_strUSER_ID.GetBuffer(0)
			, m_User.m_strGROUP_ID.GetBuffer(0), 1);
		CXJUserStore::GetInstance()->Save("c:/tb_sys_user.txt");
		
		return TRUE;
	}
	
	return FALSE;
}

/*************************************************************
 函 数 名：DOLogOut()
 功能概要：登出系统,更换用户登录
 返 回 值: 再登录成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7D03B9
BOOL CXJBrowserApp::DoLogOut()
{
	if(m_User.m_strUSER_ID.IsEmpty())
		return TRUE;
	
	//清除原先的用户名
	CString strUserID = m_User.m_strUSER_ID;
	CString strUserPWD = m_User.m_strPassword;
	m_User.m_strUSER_ID.Empty();
	m_User.m_strPassword.Empty();
	
	//弹出用户登录窗口要求重新登录
	if ( !DoLogin() )
	{
		if ( AfxMessageBox( StringFromID(IDS_TIP_LOGOUT_FAIL), MB_OKCANCEL ) == IDOK )
		{
			CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
			if(pFrame != NULL)
				pFrame->SendMessage(WM_CLOSE);
		}
		
		m_User.m_strUSER_ID = strUserID;
		m_User.m_strPassword = strUserPWD;	
	}
	return TRUE;
}

/*************************************************************
 函 数 名：TryEnter()
 功能概要：测试某用户是否具有进入某功能的权限,目前只支持查询当前用户功能
 返 回 值: 可进入某功能的权限返回TRUE,不可用返回FALSE
 参    数：param1	功能ID
		   Param2	用户ID, NULL时用当前用户测试
		   param3	是否显示警告窗
**************************************************************/
//##ModelId=49B87B7D03D8
BOOL CXJBrowserApp::TryEnter( LPCTSTR strFuncID, LPCTSTR strUserID /*= NULL*/, BOOL bShowAlarm /*= TRUE*/ )
{
	//目前只支持查询当前用户功能
	ASSERT(strFuncID!=NULL);
	BOOL bCan=FALSE;
	if(m_User.m_strUSER_ID.IsEmpty() && strUserID == NULL)
	{
		bCan = FALSE;
	}

	CString strGroup = "";

	if(strUserID != NULL)
	{
		//查找用户组名
		strGroup = QueryUserGroup(strUserID);
	}
	else
		strGroup = m_User.m_strGROUP_ID;
	//组建查询条件
	//组织SQL语句
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//组名
	Condition con1;
	CString str;
	str.Format("GROUP_ID='%s'", strGroup);
	m_DBEngine.SetCondition(sql, con1, str);
	//功能名
	Condition con2;
	str.Format("FUNC_ID='%s'", strFuncID);
	m_DBEngine.SetCondition(sql, con2, str);

	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_GROUP_FUNC_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::TryEnter, 查询失败");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			bCan = TRUE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::TryEnter, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if( (!bCan) && bShowAlarm)
	{
		CString str;
		str.Format("%s [%s]", StringFromID(IDS_VALIDATE_AUTHORITY),strFuncID);
		AfxMessageBox(str);
	}
	return bCan;
}

/*************************************************************
 函 数 名：ConnectSTTPServer()
 功能概要：建立与通讯服务器的连接, 并开启接收报文线程
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7E00BB
BOOL CXJBrowserApp::ConnectSTTPServer()
{
	//建立与通讯服务器的连接, 得到连接名柄
	int nResult = -1;
	m_SttpHandle = STTPConnect(nResult);

	SetSTTPStatus(1 == nResult, 0);

	if (m_SttpEngine.XJSTTPRegisterHandler(m_SttpHandle, ReceiveSTTPConnectStatus, STTPNET_CALLBACK_ONCONNECTCHANGE))
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() 注册回调函数ReceiveSTTPConnectStatus成功"), 3);
	}
	else
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() 注册回调函数ReceiveSTTPConnectStatus失败"), 3);
	}
	

	//启动接收线程
	if (theApp.m_pReceiveMsgThread == NULL)
	{
		m_bExitReceiveMsg = FALSE;
		m_pReceiveMsgThread = AfxBeginThread(ReceiveMessageThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pReceiveMsgThread != NULL)
		{
			m_pReceiveMsgThread->m_bAutoDelete = FALSE;
			m_pReceiveMsgThread->ResumeThread();
		}
	}


	if (m_pPostMsgThread == NULL)
	{
		//启动分发线程
		m_pPostMsgThread = AfxBeginThread(PostMessageThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pPostMsgThread != NULL)
		{
			m_pPostMsgThread->m_bAutoDelete = FALSE;
			m_pPostMsgThread->ResumeThread();
		}
	}


	if (m_pPushMsgThread == NULL)
	{
		//启动分发线程
		m_pPushMsgThread = AfxBeginThread(PushMsgThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pPushMsgThread != NULL)
		{
			m_pPushMsgThread->m_bAutoDelete = FALSE;
			m_pPushMsgThread->ResumeThread();
		}
	}

	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);


	return TRUE;
}

/*************************************************************
 函 数 名：ConnectSTTPServer()
 功能概要：建立与通讯服务器的连接, 并开启接收报文线程
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7E00BB
BOOL CXJBrowserApp::ConnectSTTPServerB()
{
	//建立与通讯服务器的连接, 得到连接名柄
	int nResult = -1;
	m_SttpHandle = STTPConnect(nResult);

	SetSTTPStatus(1 == nResult, 0);

	if (m_SttpEngine.XJSTTPRegisterHandler(m_SttpHandle, ReceiveSTTPConnectStatus, STTPNET_CALLBACK_ONCONNECTCHANGE))
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() 注册回调函数ReceiveSTTPConnectStatus成功"), 3);
	}
	else
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() 注册回调函数ReceiveSTTPConnectStatus失败"), 3);
	}

	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);

	return TRUE;
}


/*************************************************************
 函 数 名：DisConnectSTTPServer()
 功能概要：断开与通讯服务器的连接, 并关闭接收线程报文
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CXJBrowserApp::DisConnectSTTPServer()
{
	//退出接收报文线程
	if(m_pReceiveMsgThread != NULL)
	{
		ResetEvent(m_hReceiveEvent);
		WriteLog("中断接收报文线程成功");
	}

	//退出分发报文线程
	if(m_pPostMsgThread != NULL)
	{
		ResetEvent(m_hPostEvent);
		WriteLog("中断分发报文线程成功");
	}
	
	//关闭连接
	int nResult = m_SttpEngine.XJSttpCloseConnection(m_SttpHandle);
	if(1 == nResult)
	{
		//关闭成功
		m_SttpHandle = NULL;
		WriteLog("断开通信服务器连接成功");
	}
	else
	{
		WriteLog("断开与通信服务器的连接失败");
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp message handlers

//##ModelId=49B87B7E029F
void CXJBrowserApp::OnAppExit() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}

/*************************************************************
 函 数 名：SetSTTPStatus()
 功能概要：设置与通讯服务器连接状态标志, 改变状态栏信息
 返 回 值: void
 参    数：param1	连接状态
**************************************************************/
//##ModelId=49B87B7E00EA
void CXJBrowserApp::SetSTTPStatus( BOOL bConnected, int group )
{
	if( group != 1 && group != 2 ){
		group = 1;
	}
	BOOL bRefresh = FALSE;
	if( 1 == group )
	{
		//设置连接状态
		if(!bConnected)
		{
			//连接中断
			m_ConnectStatus[0] = 0;
			theApp.WriteLog(_T("常规通信服务器连接中断"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("连接常规通信服务器", "连接常规通信服务器", "与常规通信服务器连接中断", -1, OPER_FAILD);
		}
		else
		{
			bRefresh = TRUE;
			//连接正常
			m_ConnectStatus[0] = 1;
			theApp.WriteLog(_T("常规通信服务器连接成功"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("连接常规通信服务器", "连接常规通信服务器", "与常规通信服务器连接成功", -1, OPER_SUCCESS);
		}
	}
	else if( 2 == group )
	{
		//设置连接状态
		if(!bConnected)
		{
			//连接中断
			m_ConnectStatus[1] = 0;
			theApp.WriteLog(_T("61850通信服务器连接中断"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("连接61850通信服务器", "连接61850通信服务器", "与61850通信服务器连接中断", -1, OPER_FAILD);
		}
		else
		{
			bRefresh = TRUE;
			//连接正常
			m_ConnectStatus[1] = 1;
			theApp.WriteLog(_T("61850通信服务器连接成功"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("连接61850通信服务器", "连接61850通信服务器", "与61850通信服务器连接成功", -1, OPER_SUCCESS);
		}
	}

	//改变状态栏信息
	CString strTemp;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(bRefresh)
	{
		if(pFrame != NULL)
		{
			pFrame->SendMessage(COMM_STATUS_CHANGE, 0, 0);
			//取得通讯断开时间
			int nCommDisconnectTime = pFrame->GetCommDisconnectTime();
			CString str;
			str.Format("与通讯服务器断开 %d秒", nCommDisconnectTime);
			WriteLog(str, XJ_LOG_LV1);
			if(nCommDisconnectTime >= g_DisconnectTime && g_DisconnectTime > 0)
			{
				//读取保护状态
				if(GetDataEngine() != NULL)
				{
					if( g_role != MODE_SUB )
					{
						GetDataEngine()->RefreshStationStatus();
						WriteLog(_T("通信恢复,重新读取厂站状态成功"));
					}
					GetDataEngine()->LoadAllSec();
					GetDataEngine()->RefreshDevStatus();
					WriteLog(_T("通信恢复,重新读取保护状态成功"));
				}
				
				//强制刷新页面
				pFrame->SendMessage(VIEW_REFRESH, 0, 0);
				WriteLog(_T("强制刷新页面成功"));
			}else{
				//只刷新厂站通信状态
				//读取保护状态
				if(GetDataEngine() != NULL)
				{
					if( g_role != MODE_SUB )
					{
						GetDataEngine()->RefreshStationStatus();
						WriteLog(_T("通信恢复,重新读取厂站状态成功"));
					}
					//强制刷新页面
					pFrame->SendMessage(VIEW_REFRESH, 0, 1);
					WriteLog(_T("强制刷新页面成功"));
				}
			}
		}
	}
	else
	{
		//连接中断
		if(pFrame != NULL)
			pFrame->SendMessage(COMM_STATUS_CHANGE, 0, 1);
	}
	if( m_ConnectStatus[0] != -1 )
	{
		strTemp.Format("%s:%s", StringFromID(IDS_COMMUSERVER_CONVENTION),
			0 == m_ConnectStatus[0]?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
	}
	if( m_ConnectStatus[1] != -1 )
	{
		CString str = "";
		str.Format("            %s:%s", StringFromID(IDS_COMMUSERVER_61850),
			0 == m_ConnectStatus[1]?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		strTemp += str;
	}
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_STTPSTATE, (LPARAM)strTemp.GetBuffer(0));
	strTemp.ReleaseBuffer(0);
}

/*************************************************************
 函 数 名：GetSTTPStatus()
 功能概要：取得与通讯服务器的通讯状态
 返 回 值: 通讯正常返回TRUE, 通讯中断返回FALSE
**************************************************************/
//##ModelId=49B87B7E0109
BOOL CXJBrowserApp::GetSTTPStatus()
{
	return (1==m_ConnectStatus[0] || 1==m_ConnectStatus[1]);
}


/****************************************************
Date:2011/12/20  Author:LYH
函数名:   AddNewManOperator	
返回值:   void	
功能概要: 
参数: CString FunID	
参数: CString Act	
参数: CString strMsg	
参数: CString strUser	
参数: int nOperType	
参数: int nOperResult	
*****************************************************/
void CXJBrowserApp::AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult , int num)
{
		/*获得机器名，以便进行人工操作日志记录*/
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1; 
	char strComputer[MAX_COMPUTERNAME_LENGTH+1]; 
	memset( strComputer, 0, sizeof(strComputer) );
	::GetComputerName( strComputer, &nSize );

	CString sComputer = strComputer;

	//取得当前时间
	CTime tm = CTime::GetCurrentTime();
	CString sTime = tm.Format("%Y-%m-%d %H:%M:%S");
	
	//组织SQL语句
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//指定KeyName,更新Value
	
	CString str;	
	//字段
	//Msg
	BLOB_FIELD Field1;
	m_DBEngine.SetBlobField(sql, Field1, "Msg", EX_STTP_DATA_TYPE_STRING, strMsg);

	//UserName
	BLOB_FIELD Field2;
	CString sUser = strUser;
	if(strUser.IsEmpty())
		sUser = m_User.m_strUSER_ID;
	m_DBEngine.SetBlobField(sql, Field2, "UserName", EX_STTP_DATA_TYPE_STRING, sUser);

	//Computer
	BLOB_FIELD Field3;
	m_DBEngine.SetBlobField(sql, Field3, "Computer", EX_STTP_DATA_TYPE_STRING, sComputer);

	//Func
	BLOB_FIELD Field4;
	m_DBEngine.SetBlobField(sql, Field4, "Func", EX_STTP_DATA_TYPE_STRING, FunID);

	//Act
	BLOB_FIELD Field5;
	m_DBEngine.SetBlobField(sql, Field5, "Act", EX_STTP_DATA_TYPE_STRING, Act);

	//Time
	BLOB_FIELD Field6;
	m_DBEngine.SetBlobField(sql, Field6, "Time", EX_STTP_DATA_TYPE_TIME, sTime);

	if(g_OperTableType == 1)
	{
		//OperResult
		str.Format("%d", nOperResult);
		BLOB_FIELD Field7;
		m_DBEngine.SetBlobField(sql, Field7, "OperResult", EX_STTP_DATA_TYPE_INT, str);

		//OperType
		BLOB_FIELD Field8;
		str.Format("%d", nOperType);
		m_DBEngine.SetBlobField(sql, Field8, "OperType", EX_STTP_DATA_TYPE_INT, str);
		
		//opernum
		BLOB_FIELD Field9;
		str.Format("%d", num);
		m_DBEngine.SetBlobField(sql, Field9, "Reserve1", EX_STTP_DATA_TYPE_STRING, str);
	}

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJInsertBlobData(EX_STTP_INFO_OPERATION_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("保存操作日志到数据库失败");
		return;
	}
	if(nResult == 1)
	{

	}
	else
	{
		CString str;
		str.Format("保存操作日志到数据库失败,原因为%s", sError);
		WriteLog(str);
	}
}
void CXJBrowserApp::AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult /*= 0*/, int num /*= -1*/ )
{
	AddNewManOperator(FunID, Act, strMsg, "", nOperType, nOperResult, num);
}

//这后面的 limit 可以指定拆分字符串的次数哦.
vector<CString> CXJBrowserApp::SplitCString(CString &str,const CString find, int limit)
{
	vector<CString> ret;

	CString strTmp = str;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if (strTmp.IsEmpty())
		return ret;

	int start = 0;
	int pos = strTmp.Find(find,start);
	int len = find.GetLength();
	int i = 0;
	while(true){
		if(pos<0 || (limit>0 && i+1==limit)){ //NO FIND   
			ret.push_back(strTmp.Mid(start));
			break;
		}else{
			ret.push_back(strTmp.Mid(start,pos-start));
			start = pos+len;
			pos = strTmp.Find(find,start);
		}
		i++;
	}
	return ret;
}

BOOL CXJBrowserApp::NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sNewRecords)
{
	BOOL bReturn = FALSE;
	
	//CheckKeyNameForPTSet();

	PT_ZONE curZone;
	CString sRecords;
	int nCurStateID = GetPTSetModState(curZone, sRecords);

	CString str;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	str.Format(" current PT_SET_MOD_STATE = %d \n next PT_SET_MOD_STATE: %d", nCurStateID, nNextStateID);
	//AfxMessageBox(str);
	
	CString strUserID = sUserID;
	if (sUserID.IsEmpty())
		strUserID = m_User.m_strUSER_ID;

	CTime tm;
	CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");

    // 更新定值修改状态机
	// 组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	// value
	Field fld0;
	str.Format("%d", nNextStateID);
	m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_INT, str);
	
	Field fld1; // 操作装置
	str.Format("%s", data.PT_ID);
	m_DBEngine.SetField(sql, fld1, "reverse1", EX_STTP_DATA_TYPE_STRING, str);

	Field fld2; // 其它内容
	str.Format("%d,%d", data.cpu, data.code);
	m_DBEngine.SetField(sql, fld2, "reverse2", EX_STTP_DATA_TYPE_STRING, str);

	Field fld3; // 操作记录
	if (0 != nNextStateID){
		str.Format("%s,%s,%d", strTime, strUserID, nNextStateID);
		
		if (1 != nNextStateID){
			if (!sRecords.IsEmpty())
				str = sRecords + ";" + str;
		}
		if (!sNewRecords.IsEmpty())
			str = sNewRecords;
	}else{
		str = "";
	}
	m_DBEngine.SetField(sql, fld3, "reverse3", EX_STTP_DATA_TYPE_STRING, str);
	
	//条件
	//keyname
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);

		if(1 == nResult){
			bReturn = TRUE;
		}else{
			str.Format("CXJBrowserApp::NextPTSetModState, 更新失败, 原因为：%s", sError);
			WriteLog(str);
			AfxMessageBox(str);

			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::NextPTSetModState, 更新失败");
		AfxMessageBox(sError);
		delete[] sError;

		return FALSE;
	}
	
	delete[] sError;
	sError = NULL;

	return bReturn;
}

/*BOOL CXJBrowserApp::SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList)
{
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	if(arrModifyList.GetSize() < 0)
		return FALSE;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	int nResult;
	for(int i = 0; i < arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);
		
		Field fld;
		str.Format("%s", sttpData->str_value.c_str());
		m_DBEngine.SetField(sql, fld, "RESERVE3", EX_STTP_DATA_TYPE_STRING, str);
		
		//条件
		//station_id
		Condition condition;
		str.Format("CPU_CODE = '%d'", sttpData->nCpu);
		m_DBEngine.SetCondition(sql, condition, str);
		
		Condition condition1;
		str.Format("SETTING_ID = '%d'", sttpData->id);
		m_DBEngine.SetCondition(sql, condition1, str);
		
		Condition condition2;
		str.Format("pt_id = '%s'", sPTID);
		m_DBEngine.SetCondition(sql, condition2, str);
		
		
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJBrowserApp::SaveTempPTSetToDB, 更新失败");
			delete[] sError;
			sError = NULL;
			AfxMessageBox("CXJBrowserApp::SaveTempPTSetToDB, 更新失败");
			return FALSE;
		}
		
		if(1 != nResult)
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, 更新失败,原因为%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			delete[] sError;
			sError = NULL;
			return FALSE;
		}
		
		AfxMessageBox("CXJBrowserApp::SaveTempPTSetToDB excuted success.");
	}
	
	delete[] sError;
	sError = NULL;
	
	return TRUE;
}*/

BOOL CXJBrowserApp::SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList)
{
	if(arrModifyList.GetSize() < 0)
		return FALSE;

	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	CMemSet* pMemset;
	pMemset = new CMemSet;

	int nResult;
	for(int i = 0; i < arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);

		CString strSQL;
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '%s' WHERE pt_id= '%s' AND cpu_code = '%d' AND setting_id = '%d'"
						, sttpData->str_value.c_str()
						, sPTID
						, sttpData->nCpu
						, sttpData->id);
		WriteLog(strSQL);

		//进行查询
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
		memset(sError, '\0', MAXMSGLEN);
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, 查询失败");
			WriteLog(str);
			delete[] sError;
			delete pMemset;

			return FALSE;
		}

		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, 更新成功");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, 更新失败, 原因为%s", sError);
			WriteLog(str);
		}

		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag)
{
	if (zone.PT_ID.IsEmpty())
		return FALSE;

	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;

	CString strSQL;
	if (nFlag != 1){
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '', reserve2 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
			, zone.PT_ID
			, zone.cpu
		, zone.code);
	}else if (1 == nFlag){
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
			, zone.PT_ID
			, zone.cpu
		, zone.code);
	}
	strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
		, zone.PT_ID
		, zone.cpu
		, zone.code);
	//AfxMessageBox(strSQL);

	WriteLog(strSQL);
	//AfxMessageBox(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	memset(sError, '\0', MAXMSGLEN);
	int nResult;
	try
	{
		nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, 查询失败");
		WriteLog(str);
		delete[] sError;
		delete pMemset;
		//AfxMessageBox(str);
		
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{	
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, 更新成功");
		WriteLog(str);
		//AfxMessageBox(str);
	}
	else
	{
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, 更新失败, 原因为%s", sError);
		WriteLog(str);
		//AfxMessageBox(str);
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting)
{
	if(arrSetting.GetSize() < 0)
		return FALSE;
	
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	int nResult;
	for(int i = 0; i < arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)arrSetting.GetAt(i);

		CString strSQL;
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve1 = '%s' WHERE pt_id= '%s' AND cpu_code = '%s' AND setting_id = '%s'"
			, pts->Value
			, sPTID
			, sCPU
			, pts->ID);
		WriteLog(strSQL);
		//AfxMessageBox(strSQL);

		//进行查询
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, 更新失败");
			WriteLog(str);
			delete[] sError;
			delete pMemset;
			//AfxMessageBox(str);
			
			return FALSE;
		}
		
		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, 更新成功");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, 更新失败, 原因为%s", sError);
			WriteLog(str);
			//AfxMessageBox(str);
		}
		
		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;

}

int CXJBrowserApp::GetPTSetModState(PT_ZONE &data, CString &sRecords, CString &sFlag)
{
	int nReturn = -1;
	//return nReturn;
   
	CheckKeyNameForPTSet();

	CString str;

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//value
	Field fld0;
	m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	//reverse1
	Field fld1;
	m_DBEngine.SetField(sql, fld1, "reverse1", EX_STTP_DATA_TYPE_STRING);
	//reverse2
	Field fld2;
	m_DBEngine.SetField(sql, fld2, "reverse2", EX_STTP_DATA_TYPE_STRING);
	//reverse3
	Field fld3;
	m_DBEngine.SetField(sql, fld3, "reverse3", EX_STTP_DATA_TYPE_STRING);
	//note
	Field fld4;
	m_DBEngine.SetField(sql, fld4, "note", EX_STTP_DATA_TYPE_STRING);
	
	//条件
	//USER_ID
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult){
			nReturn =  -1;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::GetPTSetModState, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		return -1;
	}

	// 查询成功
	if(NULL != pMemset)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJBrowserApp::GetPTSetModState, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
            nReturn = atoi(pMemset->GetValue((UINT)0));
			data.PT_ID = pMemset->GetValue((UINT)1);
			str = pMemset->GetValue((UINT)2);

			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty()){
				//AfxMessageBox(str);
				vector<CString> v = SplitCString(str, ',');
				if (v.size() >= 2){
					
					data.cpu = atoi(v[0]);
					data.code = atoi(v[1]);
				}
			}

			sRecords = pMemset->GetValue((UINT)3);
			sFlag = pMemset->GetValue((UINT)4);
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::GetPTSetModState, 查询失败, 原因为：%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		nReturn = -1;	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}

CString CXJBrowserApp::GetUserIDByState(int nState, CString &sRecords)
{
	CString sReturn = "";

	PT_ZONE zone;
	int nCurState = -1;

	sRecords.TrimLeft();
	sRecords.TrimRight();
	if (sRecords.IsEmpty()){
		nCurState = GetPTSetModState(zone, sRecords);
	}

	CString sUserID;
	CString sTime;
	vector<CString> v = SplitCString(sRecords, ";");
	if (nState > 0 && v.size() > nState - 1){
		vector<CString> v2 = SplitCString(v[nState - 1], ",");
		sUserID = v2[1];
		sTime = v2[0];

		sReturn = sUserID;
	}

	return sReturn;
}

BOOL CXJBrowserApp::SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag)
{
	if(sUserID.IsEmpty() || sUserGroupID.IsEmpty())
		return FALSE;
	
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	int nResult;
	{
		CString strSQL;
		strSQL.Format("UPDATE tb_sys_user SET notes = '%s' WHERE user_id= '%s' AND group_id = '%s'"
			, sFlag
			, sUserID
			, sUserGroupID);
		WriteLog(strSQL);
		
		//进行查询
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		
		memset(sError, '\0', MAXMSGLEN);
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SetUserLoginFlag, 查询失败");
			WriteLog(str);
			delete[] sError;
			delete pMemset;
			
			return FALSE;
		}
		
		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SetUserLoginFlag, 更新成功");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SetUserLoginFlag, 更新失败, 原因为%s", sError);
			WriteLog(str);
		}
		
		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::RevertPTSetModState(int nRevertStateID, int nFlag)
{
	BOOL bReturn = FALSE;

	PT_ZONE zone;
	CString sRecords;
	int nCurrentState = GetPTSetModState(zone, sRecords);

	if (nCurrentState < nRevertStateID){
		//AfxMessageBox("Cannot Revert PTSet Modification State to this State.");
		return FALSE;
	}else if (nCurrentState == nRevertStateID){
		return TRUE;
	}

	if (1 == nRevertStateID)
		RevertTempPTSetToDB(zone, 1);
	else if (0 == nRevertStateID)
		RevertTempPTSetToDB(zone, 0);

	vector<CString> v = SplitCString(sRecords, ";");
	CString sRevertRecords;
	int nRows = min(v.size(), nRevertStateID);
	for (int i = 0; i < nRows; i++){
		if (i != 0)
			sRecords +=";";
		sRevertRecords += v[i];
	}
	//AfxMessageBox(sRevertRecords);

	SetRevertModifyValueFlag(nFlag);

	return NextPTSetModState(nRevertStateID, zone, CString(), sRevertRecords);
}

BOOL CXJBrowserApp::SetRevertModifyValueFlag(int nFlag)
{
	BOOL bReturn = FALSE;
	
	//CheckKeyNameForPTSet();
	
	PT_ZONE curZone;
	CString sRecords;
	int nCurStateID = GetPTSetModState(curZone, sRecords);
	
	CString str;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	//str.Format(" current PT_SET_MOD_STATE = %d \n next PT_SET_MOD_STATE: %d", nCurStateID, nNextStateID);
	//AfxMessageBox(str);
	
	// 组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	// note
	Field fld0;
	str.Format("%d", nFlag);
	m_DBEngine.SetField(sql, fld0, "note", EX_STTP_DATA_TYPE_STRING, str);
	
	//条件
	//keyname
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);
		
		if(1 == nResult){
			bReturn = TRUE;
		}else{
			str.Format("CXJBrowserApp::SetRevertModifyValueFlag, 更新失败, 原因为：%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			
			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::SetRevertModifyValueFlag, 更新失败");
		AfxMessageBox(sError);
		delete[] sError;
		
		return FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;

}

/*************************************************************
 函 数 名：GetSVGView()
 功能概要：得到主接线图指针
 返 回 值: 主接线图指针
**************************************************************/
//##ModelId=49B87B7E0128
CXJBrowserView * CXJBrowserApp::GetSVGView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();

	CXJBrowserView * pSvgView = NULL;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CXJBrowserView)))//如果是通讯状态图
		{
			pSvgView = (CXJBrowserView *)pView;
			break;
		}			
	}

	return pSvgView;
}


CViewCommMap * CXJBrowserApp::GetCommView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	CViewCommMap * pCommView = NULL;
	
	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CViewCommMap)))//如果是通讯状态图
		{
			pCommView = (CViewCommMap *)pView;
			break;
		}			
	}
	
	return pCommView;
}


/*************************************************************
 函 数 名：LoadDownDir()
 功能概要：从数据库中读取文件下载路径, 保存在m_sDownDir
 返 回 值: void
**************************************************************/
//##ModelId=49B87B7E0148
void CXJBrowserApp::LoadDownDir()
{
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//查询comtrade_path
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);

	Condition con1;
	m_DBEngine.SetCondition(sql, con1, "KeyName='DownDir'");
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadDownDir, 查询失败");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sDownDir = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("系统共享路径为:%s", m_sDownDir);
			WriteLog(str, XJ_LOG_LV2);
		}
		else
		{
			WriteLog("找不到系统下载路径", XJ_LOG_LV2);
		}
		
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadDownDir, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if(m_sDownDir != "")
	{
		//把有可能的/都换成"\\"
		m_sDownDir.Replace("/", "\\");
		if(m_sDownDir.Right(1)!="\\")
			m_sDownDir += "\\";
	}
}

/*************************************************************
 函 数 名：GetDownDir()
 功能概要：取得系统下载路径
 返 回 值: 系统下载路径
**************************************************************/
//##ModelId=49B87B7E0157
CString CXJBrowserApp::GetDownDir()
{
	if(m_sDownDir == "")
	{
		if(g_UseLocalIp == 0)
			LoadDownDir();
		else
		{
			m_sDownDir = g_LocalIp;
			if(m_sDownDir != "")
			{
				//把有可能的/都换成"\\"
				m_sDownDir.Replace("/", "\\");
				if(m_sDownDir.Right(1)!="\\")
					m_sDownDir.Format("%s\\", m_sDownDir);	
			}
		}
	}
	return m_sDownDir;
}

/*************************************************************
 函 数 名：ShowDevice()
 功能概要：显示某设备所在厂站的图形画面, 并使该设备居中
 返 回 值: void
 参    数：param1 设备指针
**************************************************************/
//##ModelId=49B87B7E0168
void CXJBrowserApp::ShowDevice( CDeviceObj* pObj, BOOL bMoveFirst /*= TRUE*/ )
{
	WriteLog(" CXJBrowserApp::ShowDevice, 开始");
	if(pObj == NULL)
		return;
	//设备为地区时不处理
	if(pObj->m_nType == TYPE_NET)
		return;
	//得到SVG画面指针
	CXJBrowserView * pView = GetSVGView();
	if(pView == NULL)
		return;

	CStationObj * pStation = NULL;
	if(pObj->m_nType == TYPE_STATION)
	{
		pStation = (CStationObj*)pObj;
	}
	else
	{
		WriteLog(" CXJBrowserApp::ShowDevice, 查找厂站");
		//查找厂站
		pStation = (CStationObj*)GetDataEngine()->FindDevice(pObj->m_sStationID, TYPE_STATION);
		if(pStation == NULL)
			return;
		WriteLog(" CXJBrowserApp::ShowDevice, 查找厂站结束");
	}
	
	
	if(pObj->m_nType == TYPE_BAY)
	{
		WriteLog(" CXJBrowserApp::ShowDevice, 设备类型为间隔");
		//关闭旧的,打开新的
		pView->CloseCurrentView();
		pView->OpenBayView((CBayObj*)pObj);
		if(bMoveFirst)
			pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
	}
	else
	{
		WriteLog(" CXJBrowserApp::ShowDevice, 打开图形画面");
		//打开图形画面
		if(pStation->m_sID == pView->m_sStationID)
		{
			WriteLog(" CXJBrowserApp::ShowDevice, 画面已打开,把画面推到前面");
			//画面已打开,把画面推到前面
			if(bMoveFirst)
				pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			WriteLog(" CXJBrowserApp::ShowDevice, 现在选择的厂站与当前打开的图形不是同一厂站");
			//现在选择的厂站与当前打开的图形不是同一厂站
			//关闭旧的,打开新的
			pView->CloseCurrentView();
			pView->OpenStationView(pStation->m_sID);
			if(bMoveFirst)
				pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
	}

	//非厂站时设备居中
	if(pObj->m_nType != TYPE_STATION && pObj->m_nType != TYPE_BAY)
	{
		WriteLog("CXJBrowserApp::ShowDevice, 非厂站时设备居中");
		pView->CenterDevice(pObj);
	}
}

/*************************************************************
 函 数 名：LocateInTree()
 功能概要：在设备树中定位厂站
 返 回 值: void
 参    数：param1	指定厂站
		   Param2
**************************************************************/
//##ModelId=49B87B7E0177
void CXJBrowserApp::LocateInTree( CStationObj* pObj )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->m_pDeviceView->LocateInTree(pObj);
}

/*************************************************************
 函 数 名：ShowEventProp()
 功能概要：调用主框架窗口的方法, 显示事件属性对话框
 返 回 值: 
 参    数：param1	事件管理类指针
		   Param2	是否是保护属性页面打开的对话框
**************************************************************/
//##ModelId=49B87B7E0186
void CXJBrowserApp::ShowEventProp( CXJEventManager* pEventManager, BOOL bPT)
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowEventProp(pEventManager, bPT);
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：停止自己起的所有线程
 返 回 值: void
**************************************************************/
//##ModelId=49B87B7E01A5
void CXJBrowserApp::EndOwnerThread()
{
	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);
	//退出读取配置线程
	if(m_pClearConfigThread != NULL)
	{
		WriteLog("开始退出清理二次设备配置线程", XJ_LOG_LV3);
		//线程还没退出去
		m_bExitClearConfig = TRUE;	//设置退出标志
		DWORD dw = WaitForSingleObject(m_pClearConfigThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pClearConfigThread->m_hThread,NULL);
			WriteLog("清理二次设备配置线程无法正常退出,强制结束", XJ_LOG_LV3);
			break;
		}
		delete m_pClearConfigThread;
		m_pClearConfigThread = NULL;
		WriteLog("退出清理二次设备配置线程完毕", XJ_LOG_LV3);
	}
	//退出接收报文线程
	if(m_pReceiveMsgThread != NULL)
	{
		WriteLog("开始退出接收报文线程", XJ_LOG_LV3);
		//线程还没退出去
		m_bExitReceiveMsg = TRUE; //设置退出标志
		DWORD dw = WaitForSingleObject(m_pReceiveMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pReceiveMsgThread->m_hThread,NULL);
			WriteLog("接收报文线程无法正常退出,强制结束", XJ_LOG_LV3);
			break;
		}
		delete m_pReceiveMsgThread;
		m_pReceiveMsgThread = NULL;
		WriteLog("退出接收报文线程完毕", XJ_LOG_LV3);
	}

	//退出分发报文线程
	if(m_pPostMsgThread != NULL)
	{
		WriteLog("开始退出分发报文线程", XJ_LOG_LV3);
		//线程还没退出去
		m_bExitReceiveMsg = TRUE; //设置退出标志
		DWORD dw = WaitForSingleObject(m_pPostMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pPostMsgThread->m_hThread,NULL);
			WriteLog("分发报文线程无法正常退出,强制结束", XJ_LOG_LV3);
			break;
		}
		delete m_pPostMsgThread;
		m_pPostMsgThread = NULL;
		WriteLog("退出分发报文线程完毕", XJ_LOG_LV3);
	}

	//退出分发报文线程
	if(m_pPushMsgThread != NULL)
	{
		WriteLog("开始退出自动上送报文线程", XJ_LOG_LV3);
		//线程还没退出去
		g_PushMsgProcess.Quit();
		m_bExitReceiveMsg = TRUE; //设置退出标志
		DWORD dw = WaitForSingleObject(m_pPushMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pPushMsgThread->m_hThread,NULL);
			WriteLog("自动上送报文线程无法正常退出,强制结束", XJ_LOG_LV3);
			break;
		}
		delete m_pPushMsgThread;
		m_pPushMsgThread = NULL;
		WriteLog("退出自动上送报文线程完毕", XJ_LOG_LV3);
	}

	RemoveAllSTTPData();
}

/*************************************************************
 函 数 名：LoadSysConfig()
 功能概要：从配置文件读入系统设置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B7
void CXJBrowserApp::LoadSysConfig()
{
	CString strIniFile = g_SysConfig;

	g_ConnectServerButton = GetPrivateProfileInt("ToolBarConfig", "ConnectServerButton", 0, strIniFile);
	g_DeviceTreeButton = GetPrivateProfileInt("ToolBarConfig", "DeviceTreeButton", 1, strIniFile);
	g_LogButton = GetPrivateProfileInt("ToolBarConfig", "LogButton", 1, strIniFile);
	g_HistoryQueryButton = GetPrivateProfileInt("ToolBarConfig", "HistoryQueryButton", 1, strIniFile);
	g_AlarmCountButton = GetPrivateProfileInt("ToolBarConfig", "AlarmCountButton", 0, strIniFile);
	g_SystemSettingButton = GetPrivateProfileInt("ToolBarConfig", "SystemSettingButton", 1, strIniFile);
	g_CallAllButton = GetPrivateProfileInt("ToolBarConfig", "CallAllButton", 1, strIniFile);
	g_CommonFileButton = GetPrivateProfileInt("ToolBarConfig", "CommonFileButton", 1, strIniFile);
    g_ExitButton = GetPrivateProfileInt("ToolBarConfig", "ExitButton", 1, strIniFile);

	////运行与监视角色 0:主站 1:子站
	g_role = GetPrivateProfileInt("SysConfig", "role", 0, strIniFile);
	//点击SVG图二次设备时查找设备方式 0:103组号 1:uri
    g_FindDeviceWay = GetPrivateProfileInt("SysConfig", "FindDeviceWay", 0, strIniFile);; 
	//是否显示输出窗口
	g_ShowOutputWnd = GetPrivateProfileInt("SysConfig", "ShowOutputWnd", 1, strIniFile);
	//是否显示设备树窗口
	g_ShowDeviceTreeWnd = GetPrivateProfileInt("SysConfig", "ShowDeviceTreeWnd", 1, strIniFile);
	//设备树类型
	g_DeviceTreeType = GetPrivateProfileInt("SysConfig", "DeviceTreeType", 0, strIniFile);
	//是否输出日志到日志文件, 0为不输出, >0为输出
	g_PutOutToLogFile = GetPrivateProfileInt("SysConfig", "OutputToLogFile", 0, strIniFile);
	//是否输出日志到日志输出窗口, 0为不输出, >0为输出
	g_OutPutToWnd = GetPrivateProfileInt("SysConfig", "OutputToWnd", 1, strIniFile);
	////输出日志的等级, 1为重要的日志才输出; 2为普通日志输出; 3为所有日志都输出(少用);
	g_LogLevel = GetPrivateProfileInt("SysConfig", "LogLevel", 1, strIniFile);
	//程序启动时默认打开的厂站
	GetPrivateProfileString("SysConfig", "DefaultStation", "", g_DefaultStation.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
	//列表框行高
	g_ListItemHeight = GetPrivateProfileInt("SysConfig", "ListItemHeight", 20, strIniFile);
	//网络存储器用户名
	GetPrivateProfileString("SysConfig", "NetworkUserName", "", m_sNetworkUser.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	//网络存储器密码
	GetPrivateProfileString("SysConfig", "NetworkPassword", "", m_sNetworkPassword.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	//断线后重读数据库的时间
	g_DisconnectTime = GetPrivateProfileInt("SysConfig", "DisconnectTime", 20, strIniFile);
    //厂站通讯状态最大记录数

	g_style  = GetPrivateProfileInt("SysConfig", "Style", 0, strIniFile);
	
	g_SecType = GetPrivateProfileInt("SysConfig", "LoadSecType", -1, strIniFile);
	GetPrivateProfileString("SysConfig", "AppName", "IPOFAS-运行与监视", g_sAppName.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	g_PostTime = GetPrivateProfileInt("SysConfig", "PostTime", 20, strIniFile);

	GetPrivateProfileString("Style", "StyleName", "", g_strStyleName.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);

	g_UseXJWave = GetPrivateProfileInt("SysConfig", "UseXJWave", 1, strIniFile);
	if(g_UseXJWave == 1)
		g_strOscName = "XJWave.exe";
	else
		g_strOscName = "STWave.exe";

	//是否使用本地配置的IP, 0否1是。为0时在数据库中读取配置,默认为0 
	g_UseLocalIp = GetPrivateProfileInt("SysConfig", "UseLocalIp", 0, strIniFile);
	//本地配置的IP
	GetPrivateProfileString("SysConfig", "LocalIp", "", g_LocalIp.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
	//选择的数据库名
	g_LoadSttpDll = GetPrivateProfileInt("SysConfig","LoadSttpDll",0,strIniFile);

	//故障报告
	g_UseColorFaultReport = GetPrivateProfileInt("FaultReportConfig", "ColorFault", 0, strIniFile);
	g_FaultReportFilter = GetPrivateProfileInt("FaultReportConfig", "FaultReportFilter", 1, strIniFile);
	g_FaultFilterIII = GetPrivateProfileInt("FaultReportConfig", "FaultFilterIII", 0, strIniFile);
	g_FaultActionTime = GetPrivateProfileInt("FaultReportConfig", "FaultActionTime", 0, strIniFile);
	g_FaultShowWave = GetPrivateProfileInt("FaultReportConfig", "FaultShowWave", 0, strIniFile);

	g_LockTime = GetPrivateProfileInt("SysConfig", "LockTime", 5, strIniFile);
	CString sTime;
	char a[15];
	for(int i = 0; i < 15; i++)
		a[i] = '\0';
	GetPrivateProfileString("SysConfig", "LastFailTime", "19710101010101", a, 15, strIniFile);
	CString str = a;
	g_LastFailTime = StringToTime14(str);
	
	g_LoginFailTimes = GetPrivateProfileInt("SysConfig", "LoginFailTimes", 0, strIniFile);
	
	CTime tmCur = CTime::GetCurrentTime();
	CTimeSpan tsTime = tmCur - g_LastFailTime;
	if(tsTime.GetTotalSeconds() > g_LockTime*60)
	{
		//解除锁定
		g_LoginFailTimes = 0;
	}
	
	g_UILockTime = GetPrivateProfileInt("SysConfig", "UILockTime", 0, strIniFile);

	GetPrivateProfileString("SysConfig", "StationCommSvg", STATIONCOMM_File, g_strStationSvg.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);

	XMLParser xmlParser;
	if (xmlParser.CheckFileExist(g_StyleConfig))
	{
		xmlParser.ParserXmlFile(g_StyleConfig);
		if (xmlParser.m_list.GetCount() == 0)
		{
			xmlParser.SaveNewStyleSetting(g_StyleConfig);
		}
		StyleSetting* pStyle = NULL;
		for(POSITION pos = xmlParser.m_list.GetHeadPosition(); pos != NULL;)
		{
			pStyle = (StyleSetting*)xmlParser.m_list.GetNext(pos);
			if (pStyle != NULL && !pStyle->m_sName.CompareNoCase(g_strStyleName))
			{
				g_strToolBarBK = pStyle->m_sToolBarBK;
				g_strStartBmp = pStyle->m_sStartBmp;
				g_TreeTextColor = atoi(pStyle->m_sDeviceTreeTextColor);
				g_TreeBkColor = atoi(pStyle->m_sDeviceTreeBk);
				g_TitleLightColor = atoi(pStyle->m_sTitleLightColor);
				g_TitleDeepColor = atoi(pStyle->m_sTitleDeepColor);
				g_ListSpaceColor4 = atoi(pStyle->m_sListSpaceColor);
				g_ListHeadColor = atoi(pStyle->m_sListHeadColor);
				break;
			}
		}
	}
	else
	{
		g_style = 0;
	}


	/*
	GetPrivateProfileString("Style", "ToolBarBK", "", g_strToolBarBK.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	GetPrivateProfileString("Style", "StartBmp", "", g_strStartBmp.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	g_ListSpaceColor = GetPrivateProfileInt("Style", "ListSpaceColor", 0, strIniFile);
	g_ListHeadColor = GetPrivateProfileInt("Style", "ListHeadColor", 0, strIniFile);
	g_TreeBkColor = GetPrivateProfileInt("Style", "DeviceTreeBk", 0, strIniFile);
	g_TreeTextColor = GetPrivateProfileInt("Style", "DeviceTreeTextColor", 0, strIniFile);
	g_TitleLightColor = GetPrivateProfileInt("Style", "TitleLightColor", 0, strIniFile);
	g_TitleDeepColor = GetPrivateProfileInt("Style", "TitleDeepColor", 0, strIniFile);
	*/

	g_RemindMaxCount = GetPrivateProfileInt("Remind", "MaxCount", 0, strIniFile);
	g_RemindTime = GetPrivateProfileInt("Remind", "RetainTime", 2, strIniFile);

	g_MarkDownOscPT = GetPrivateProfileInt("SysConfig", "MarkDownOscPT", 0, strIniFile);

	g_AutoStationCommMap = GetPrivateProfileInt("SysConfig", "AutoStationCommMap", 1, strIniFile);

	GetPrivateProfileString("SysConfig", "Language", "zh_CN", g_Language.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
}

/*************************************************************
 函 数 名：SaveSysConfig()
 功能概要：保存系统配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01C5
void CXJBrowserApp::SaveSysConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	//是否显示输出窗口
	str.Format("%d", g_ShowOutputWnd);
	WritePrivateProfileString("SysConfig", "ShowOutputWnd", str, strIniFile);
	//是否显示设备树窗口
	str.Format("%d", g_ShowDeviceTreeWnd);
	WritePrivateProfileString("SysConfig", "ShowDeviceTreeWnd", str, strIniFile);
	//设备树类型
	str.Format("%d", g_DeviceTreeType);
	WritePrivateProfileString("SysConfig", "DeviceTreeType", str, strIniFile);
	//是否输出日志到日志文件, 0为不输出, >0为输出
	str.Format("%d", g_PutOutToLogFile);
	WritePrivateProfileString("SysConfig", "OutputToLogFile", str, strIniFile);
	//是否输出日志到日志输出窗口, 0为不输出, >0为输出
	str.Format("%d", g_OutPutToWnd);
	WritePrivateProfileString("SysConfig", "OutputToWnd", str, strIniFile);
	//输出日志的等级, 1为重要的日志才输出; 2为普通日志输出; 3为所有日志都输出(少用);
	str.Format("%d", g_LogLevel);
	WritePrivateProfileString("SysConfig", "LogLevel", str, strIniFile);
	//程序启动时默认打开的厂站
	WritePrivateProfileString("SysConfig", "DefaultStation", g_DefaultStation, strIniFile);
	//列表框行高
	str.Format("%d", g_ListItemHeight);
	WritePrivateProfileString("SysConfig", "ListItemHeight", str, strIniFile);
	//网络存储器用户名
	WritePrivateProfileString("SysConfig", "NetworkUserName", m_sNetworkUser, strIniFile);
	//网络存储器密码
	WritePrivateProfileString("SysConfig", "NetworkPassword", m_sNetworkPassword, strIniFile);

	//录波软件名称
	str.Format("%d", g_UseXJWave);
	WritePrivateProfileString("SysConfig", "UseXJWave", str, strIniFile);

	str.Format("%d", g_SecType);
	WritePrivateProfileString("SysConfig", "LoadSecType", str, strIniFile);
	WritePrivateProfileString("SysConfig", "AppName", g_sAppName, strIniFile);
	str.Format("%d", g_PostTime);
	WritePrivateProfileString("SysConfig", "PostTime", str, strIniFile);

	WritePrivateProfileString("SysConfig", "Language", g_Language, strIniFile);

	//是否使用本地配置的IP, 0否1是。为0时在数据库中读取配置,默认为0
	str.Format("%d", g_UseLocalIp);
	WritePrivateProfileString("SysConfig", "UseLocalIp", str, strIniFile);
	//本地配置的IP
	WritePrivateProfileString("SysConfig", "LocalIp", g_LocalIp, strIniFile);
	//动态库配置
	str.Format("%d", g_LoadSttpDll);
	WritePrivateProfileString("SysConfig", "LoadSttpDll", str, strIniFile);
	//工具栏配置
	str.Format("%d", g_ConnectServerButton);
	WritePrivateProfileString("ToolBarConfig", "ConnectServerButton", str, strIniFile);
	str.Format("%d", g_DeviceTreeButton);
	WritePrivateProfileString("ToolBarConfig", "DeviceTreeButton", str, strIniFile);
	str.Format("%d", g_LogButton);
	WritePrivateProfileString("ToolBarConfig", "LogButton", str, strIniFile);
	str.Format("%d", g_HistoryQueryButton);
	WritePrivateProfileString("ToolBarConfig", "HistoryQueryButton", str, strIniFile);
	str.Format("%d", g_AlarmCountButton);
	WritePrivateProfileString("ToolBarConfig", "AlarmCountButton", str, strIniFile);
	str.Format("%d", g_SystemSettingButton);
	WritePrivateProfileString("ToolBarConfig", "SystemSettingButton", str, strIniFile);
	str.Format("%d", g_CallAllButton);
	WritePrivateProfileString("ToolBarConfig", "CallAllButton", str, strIniFile);
	str.Format("%d", g_CommonFileButton);
	WritePrivateProfileString("ToolBarConfig", "CommonFileButton", str, strIniFile);
	str.Format("%d", g_ExitButton);
	WritePrivateProfileString("ToolBarConfig", "ExitButton", str, strIniFile);


	//故障报告
	str.Format("%d", g_UseColorFaultReport);
	WritePrivateProfileString("FaultReportConfig", "ColorFault", str, strIniFile);
	
	str.Format("%d", g_FaultReportFilter);
	WritePrivateProfileString("FaultReportConfig", "FaultReportFilter", str, strIniFile);

	str.Format("%d", g_FaultFilterIII);
	WritePrivateProfileString("FaultReportConfig", "FaultFilterIII", str, strIniFile);
	
	str.Format("%d", g_FaultActionTime);
	WritePrivateProfileString("FaultReportConfig", "FaultActionTime", str, strIniFile);

	str.Format("%d",g_FaultShowWave);
	WritePrivateProfileString("FaultReportConfig", "FaultShowWave", str, strIniFile);

	str.Format("%d", g_LockTime);
	WritePrivateProfileString("SysConfig", "LockTime", str, strIniFile);
	
	str = g_LastFailTime.Format("%Y%m%d%H%M%S");
	WritePrivateProfileString("SysConfig", "LastFailTime", str, strIniFile);
	
	str.Format("%d", g_LoginFailTimes);
	WritePrivateProfileString("SysConfig", "LoginFailTimes", str, strIniFile);
	
	str.Format("%d", g_UILockTime);
	WritePrivateProfileString("SysConfig", "UILockTime", str, strIniFile);

	WritePrivateProfileString("SysConfig", "StationCommSvg", g_strStationSvg, strIniFile);

	str.Format("%d", g_MarkDownOscPT);
	WritePrivateProfileString("SysConfig", "MarkDownOscPT", str, strIniFile);

	str.Format("%d", g_RemindMaxCount);
	WritePrivateProfileString("Remind", "MaxCount", str, strIniFile);
	str.Format("%d", g_RemindTime);
	WritePrivateProfileString("Remind", "RetainTime", str, strIniFile);

	str.Format("%d", g_AutoStationCommMap);
	WritePrivateProfileString("SysConfig", "AutoStationCommMap", str, strIniFile);

}

//##ModelId=49B87B7D0271
CXJBrowserApp::~CXJBrowserApp()
{
	DeleteCriticalSection(&m_CriticalSection);
	// 关闭信号量句柄 
	if(hSem != NULL)
	{
		ReleaseSemaphore(hSem, 1, NULL);
		CloseHandle(hSem);
		hSem = NULL;
	}
	m_listFullData.RemoveAll();
	CloseHandle(m_hPostEvent);
	CloseHandle(m_hReceiveEvent);
}

/*************************************************************
 函 数 名：WriteLogEx()
 功能概要：写日志文件到日志文件, 因为发现线程中写日志如果在等待线程关闭时会引起死锁 
 返 回 值: 
 参    数：param1	日志内容
		   Param2	日志输出等级
**************************************************************/
//##ModelId=49B87B7E008C
void CXJBrowserApp::WriteLogEx( CString sLog, int nLV /*= XJ_LOG_LV2*/ )
{
	m_LogMgr.WriteLogEx(sLog, nLV);
}

//##ModelId=49B87B7E01D4
void CXJBrowserApp::LoadEventConfig()
{
	CString strIniFile = g_SysConfig;

	//I类事件是否显示. 0-不显示. 1-显示
	g_ShowEventI = GetPrivateProfileInt("EventConfig", "EventIShow", 1, strIniFile);
	//I类事件是否报警. 0-不报警. 1-报警
	g_AlarmEventI = GetPrivateProfileInt("EventConfig", "EventIAlarm", 1, strIniFile);		
	//I类事件显示颜色
	g_colEventI = GetPrivateProfileInt("EventConfig", "EventIColor", RGB(255,0,0), strIniFile);

	//II类事件是否显示. 0-不显示. 1-显示
	g_ShowEventII = GetPrivateProfileInt("EventConfig", "EventIIShow", 1, strIniFile);
	//II类事件是否报警. 0-不报警. 1-报警
	g_AlarmEventII = GetPrivateProfileInt("EventConfig", "EventIIAlarm", 1, strIniFile);
	//II类事件显示颜色
	g_colEventII = GetPrivateProfileInt("EventConfig", "EventIIColor", RGB(0, 255, 255), strIniFile);

	//III类事件是否显示. 0-不显示. 1-显示
	g_ShowEventIII = GetPrivateProfileInt("EventConfig", "EventIIIShow", 1, strIniFile);
	//III类事件是否报警. 0-不报警. 1-报警
	g_AlarmEventIII = GetPrivateProfileInt("EventConfig", "EventIIIAlarm", 1, strIniFile);
	//III类事件显示颜色
	g_colEventIII = GetPrivateProfileInt("EventConfig", "EventIIIColor", RGB(214, 223, 247), strIniFile);

	//未确认事件显示
	g_colNotAcked = GetPrivateProfileInt("EventConfig", "EventNotAcked", RGB(0, 0, 255), strIniFile);
	//已确认事件显示
	g_colAcked = GetPrivateProfileInt("EventConfig", "EventAcked", RGB(0, 0, 0), strIniFile);

	//收到装置的动作或告警时,是否闪烁此装置图形.0-否,1-是
	g_BlinkDevice = GetPrivateProfileInt("EventConfig", "BlinkDevice", 1, strIniFile);
	//收到装置的动作或告警时,是否把所属厂站的图形画面推到最前面.0-否,1-是
	g_MustShowDevice = GetPrivateProfileInt("EventConfig", "MustShowDevice", 1, strIniFile);
	//二次设备闪烁时,是否相关联的一次设备一起闪烁.0-否,1-是
	g_BlinkOwner = GetPrivateProfileInt("EventConfig", "BlinkOwner", 1, strIniFile);
	//收到装置的动作或告警时, 是否弹出事件属性对话框. 0-否, 1-是
	g_ShowEventDlg = GetPrivateProfileInt("EventConfig", "ShowEventDlg", 0, strIniFile);
	//是否使用声音报警. 0-否,1-是
	g_UseAlarmSound = GetPrivateProfileInt("EventConfig", "UseAlarmSound", 0, strIniFile);	
	//报警声音持续时间. >=0整数. 单位.秒
	g_SoundTime = GetPrivateProfileInt("EventConfig", "AlarmSoundTime", 60, strIniFile);
	g_FlashTime = GetPrivateProfileInt("EventConfig", "FlashTime", 120, strIniFile);
	//收到装置的动作或告警时,是否闪烁此装置所属厂站图形.0-否，1-是.此项为1并且MustShowDevice为1时将厂站图形画面前置
	g_BlinkStation = GetPrivateProfileInt("EventConfig", "BlinkStation", 0, strIniFile);
	g_AutoChangeMap = GetPrivateProfileInt("EventConfig", "AutoChangeMap", 0, strIniFile);
}

/*************************************************************
 函 数 名：SaveEventConfig()
 功能概要：保存事件配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01D5
void CXJBrowserApp::SaveEventConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;	

	//I类事件是否显示. 0-不显示. 1-显示
	str.Format("%d", g_ShowEventI);
	WritePrivateProfileString("EventConfig", "EventIShow", str, strIniFile);
	//I类事件是否报警. 0-不报警. 1-报警
	str.Format("%d", g_AlarmEventI);
	WritePrivateProfileString("EventConfig", "EventIAlarm", str, strIniFile);
	//I类事件显示颜色
	str.Format("%d", g_colEventI);
	WritePrivateProfileString("EventConfig", "EventIColor", str, strIniFile);

	//II类事件是否显示. 0-不显示. 1-显示
	str.Format("%d", g_ShowEventII);
	WritePrivateProfileString("EventConfig", "EventIIShow", str, strIniFile);
	//II类事件是否报警. 0-不报警. 1-报警
	str.Format("%d", g_AlarmEventII);
	WritePrivateProfileString("EventConfig", "EventIIAlarm", str, strIniFile);
	//II类事件显示颜色
	str.Format("%d", g_colEventII);
	WritePrivateProfileString("EventConfig", "EventIIColor", str, strIniFile);

	//III类事件是否显示. 0-不显示. 1-显示
	str.Format("%d", g_ShowEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIShow", str, strIniFile);
	//III类事件是否报警. 0-不报警. 1-报警
	str.Format("%d", g_AlarmEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIAlarm", str, strIniFile);
	//III类事件显示颜色
	str.Format("%d", g_colEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIColor", str, strIniFile);

	//未确认事件显示
	str.Format("%d", g_colNotAcked);
	WritePrivateProfileString("EventConfig", "EventNotAcked", str, strIniFile);
	//已确认事件显示
	str.Format("%d", g_colAcked);
	WritePrivateProfileString("EventConfig", "EventAcked", str, strIniFile);

	//收到装置的动作或告警时,是否闪烁此装置图形.0-否,1-是
	str.Format("%d", g_BlinkDevice);
	WritePrivateProfileString("EventConfig", "BlinkDevice", str, strIniFile);
	//收到装置的动作或告警时,是否把所属厂站的图形画面推到最前面.0-否,1-是
	str.Format("%d", g_MustShowDevice);
	WritePrivateProfileString("EventConfig", "MustShowDevice", str, strIniFile);
	//二次设备闪烁时,是否相关联的一次设备一起闪烁.0-否,1-是
	str.Format("%d", g_BlinkOwner);
	WritePrivateProfileString("EventConfig", "BlinkOwner", str, strIniFile);
	//收到装置的动作或告警时, 是否弹出事件属性对话框
	str.Format("%d", g_ShowEventDlg);
	WritePrivateProfileString("EventConfig", "ShowEventDlg", str, strIniFile);

	//是否使用声音报警. 0-否,1-是
	str.Format("%d", g_UseAlarmSound);
	WritePrivateProfileString("EventConfig", "UseAlarmSound", str, strIniFile);	
	//报警声音持续时间. >=0整数. 单位.秒
	str.Format("%d", g_SoundTime);
	WritePrivateProfileString("EventConfig", "AlarmSoundTime", str, strIniFile);
	str.Format("%d", g_FlashTime);
	WritePrivateProfileString("EventConfig", "FlashTime", str, strIniFile);
	str.Format("%d", g_BlinkDevice);
	WritePrivateProfileString("EventConfig", "BlinkOwner", str, strIniFile);
	str.Format("%d", g_BlinkStation);
	WritePrivateProfileString("EventConfig", "BlinkStation", str, strIniFile);
	str.Format("%d", g_AutoChangeMap);
	WritePrivateProfileString("EventConfig", "AutoChangeMap", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadCommStatus()
 功能概要：从配置文件读入主子站通讯状态页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01E4
void CXJBrowserApp::LoadCommStatusConfig()
{
	CString strIniFile = g_SysConfig;
	
	//刷新厂站持续时间的间隔时间,单位(分钟)
	g_RefreshStationTime = GetPrivateProfileInt("CommStatusConfig", "RefreshStationTime", 10, strIniFile);
	//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
	g_ShowCommON = GetPrivateProfileInt("CommStatusConfig", "ShowCommON", 1, strIniFile);
	//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
	g_ShowCommOFF = GetPrivateProfileInt("CommStatusConfig", "ShowCommOFF", 1, strIniFile);
	//厂站是否使用双通道通信
	g_DoubleChannel = GetPrivateProfileInt("CommStatusConfig", "DoubleChannel", 0, strIniFile);
	g_ShowDevStatusInStation = GetPrivateProfileInt("CommStatusConfig", "ShowDevStatusInStation", 1, strIniFile);
	//主子站通讯状态图形背景色
	g_CommMapBK = GetPrivateProfileInt("CommStatusConfig", "CommMapBk", RGB(0, 79, 79), strIniFile); 
	//主子站通讯状体图中厂站名的字体类型
	g_CommMapFont = StringFromID(IDS_DEFAULT_FONT);
	GetPrivateProfileString("CommStatusConfig", "CommMapFont", StringFromID(IDS_DEFAULT_FONT), g_CommMapFont.GetBuffer(MAX_PATH), MAX_PATH,strIniFile);
	//主子站通讯状体图中厂站名的字体大小
	g_CommMapFontSize = GetPrivateProfileInt("CommStatusConfig", "CommMapFontSize", 18, strIniFile); 
	g_CommMapFontColor = GetPrivateProfileInt("CommStatusConfig", "CommMapFontColor", RGB(0,0,0), strIniFile); 
}

/*************************************************************
 函 数 名：SaveCommStatus()
 功能概要：保存主子站通讯状态页面配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01E5
void CXJBrowserApp::SaveCommStatusConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_RefreshStationTime);
	//刷新厂站持续时间的间隔时间,单位(分钟)
	WritePrivateProfileString("CommStatusConfig", "RefreshStationTime", str, strIniFile);
	//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
	str.Format("%d", g_ShowCommON);
	WritePrivateProfileString("CommStatusConfig", "ShowCommON", str, strIniFile);
	//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
	str.Format("%d", g_ShowCommOFF);
	WritePrivateProfileString("CommStatusConfig", "ShowCommOFF", str, strIniFile);
	//厂站是否使用双通道通信
	str.Format("%d", g_DoubleChannel);
	WritePrivateProfileString("CommStatusConfig", "DoubleChannel", str, strIniFile);
	str.Format("%d", g_ShowDevStatusInStation);
	WritePrivateProfileString("CommStatusConfig", "ShowDevStatusInStation", str, strIniFile);

	//主子站通讯状态图形背景色
	str.Format("%d", g_CommMapBK);
	WritePrivateProfileString("CommStatusConfig", "CommMapBk", str, strIniFile); 
	//主子站通讯状体图中厂站名的字体类型
	WritePrivateProfileString("CommStatusConfig", "CommMapFont", g_CommMapFont, strIniFile); 
	//主子站通讯状体图中厂站名的字体大小
	str.Format("%d", g_CommMapFontSize);
	WritePrivateProfileString("CommStatusConfig", "CommMapFontSize", str, strIniFile); 
	str.Format("%d", g_CommMapFontColor);
	WritePrivateProfileString("CommStatusConfig", "CommMapFontColor", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadActionViewConfig()
 功能概要：从配置文件读入动作事件页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01F4
void CXJBrowserApp::LoadActionViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//是否显示动作事件详细内容
	g_ShowActionDetail = GetPrivateProfileInt("ActionViewConfig", "ShowActionDetail", 1, strIniFile);
}

/*************************************************************
 函 数 名：SaveActionViewConfig()
 功能概要：保存动作事件页面配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01F5
void CXJBrowserApp::SaveActionViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowActionDetail);
	//是否显示动作事件详细内容
	WritePrivateProfileString("ActionViewConfig", "ShowActionDetail", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadAlarmViewConfig()
 功能概要：从配置文件读入告警事件页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0203
void CXJBrowserApp::LoadAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//是否显示动作事件详细内容
	g_ShowAlarmDetail = GetPrivateProfileInt("AlarmViewConfig", "ShowAlarmDetail", 0, strIniFile);
}

/*************************************************************
 函 数 名：SaveAlarmViewConfig()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0204
void CXJBrowserApp::SaveAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowAlarmDetail);
	//是否显示动作事件详细内容
	WritePrivateProfileString("AlarmViewConfig", "ShowAlarmDetail", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadSysAlarmViewConfig()
 功能概要：从配置文件读入系统告警页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0213
void CXJBrowserApp::LoadSysAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//是否显示动作事件详细内容
	g_ShowSysAlarmDetail = GetPrivateProfileInt("SysAlarmViewConfig", "ShowSysAlarmDetail", 1, strIniFile);
}

/*************************************************************
 函 数 名：SaveSysAlarmViewConfig()
 功能概要：保存系统告警页面配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0214
void CXJBrowserApp::SaveSysAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowSysAlarmDetail);
	//是否显示动作事件详细内容
	WritePrivateProfileString("SysAlarmViewConfig", "ShowSysAlarmDetail", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadBatchConfig()
 功能概要：载入批量召唤配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0222
void CXJBrowserApp::LoadBatchConfig()
{
	CString strIniFile = g_SysConfig;
	
	//批量召唤选择中, "召唤"的文字颜色
	g_BatchCall = GetPrivateProfileInt("BatchConfig", "BatchCall", RGB(0, 0, 0), strIniFile);
	//批量召唤选择中, "不召唤"的文字颜色
	g_BatchNotCall = GetPrivateProfileInt("BatchConfig", "BatchNotCall", RGB(255, 0, 0), strIniFile);
	//批量召唤选择中, "不支持"的文字颜色
	g_BatchNotSupport = GetPrivateProfileInt("BatchConfig", "BatchNotSupport", RGB(0, 0, 255), strIniFile);
	//批量召唤结果中, "成功"的文字颜色
	g_BatchSuccess = GetPrivateProfileInt("BatchConfig", "BatchSuccess", RGB(0, 0, 0), strIniFile);
	//批量召唤结果中, "失败"的文字颜色
	g_BatchFail = GetPrivateProfileInt("BatchConfig", "BatchFail", RGB(255, 0, 0), strIniFile);
	//批量召唤结果中, "此项不召唤"的文字颜色
	g_BatchNULL = GetPrivateProfileInt("BatchConfig", "BatchNULL", RGB(80, 240, 120), strIniFile);
	//批量召唤结果中, "尚未召唤"的文字颜色
	g_BatchLater = GetPrivateProfileInt("BatchConfig", "BatchLater", RGB(121, 121, 121), strIniFile);
	//自动总召模块所在的IP地址
	GetPrivateProfileString("BatchConfig", "AutoCall_IP", "10.123.16.133", g_AutoCall_IP.GetBuffer(16), 16,strIniFile);
	//自动总召模块的监听端口
	g_AutoCall_Port  = GetPrivateProfileInt("BatchConfig", "AutoCall_Port", 8888, strIniFile);
}

/*************************************************************
 函 数 名：SaveBatchConfig()
 功能概要：保存批量召唤配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0223
void CXJBrowserApp::SaveBatchConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//批量召唤选择中, "召唤"的文字颜色
	str.Format("%d", g_BatchCall);
	WritePrivateProfileString("BatchConfig", "BatchCall", str, strIniFile);
	//批量召唤选择中, "不召唤"的文字颜色
	str.Format("%d", g_BatchNotCall);
	WritePrivateProfileString("BatchConfig", "BatchNotCall", str, strIniFile);
	//批量召唤选择中, "不支持"的文字颜色
	str.Format("%d", g_BatchNotSupport);
	WritePrivateProfileString("BatchConfig", "BatchNotSupport", str, strIniFile);
	//批量召唤结果中, "成功"的文字颜色
	str.Format("%d", g_BatchSuccess);
	WritePrivateProfileString("BatchConfig", "BatchSuccess", str, strIniFile);
	//批量召唤结果中, "失败"的文字颜色
	str.Format("%d", g_BatchFail);
	WritePrivateProfileString("BatchConfig", "BatchFail", str, strIniFile);
	//批量召唤结果中, "此项不召唤"的文字颜色
	str.Format("%d", g_BatchNULL);
	WritePrivateProfileString("BatchConfig", "BatchNULL", str, strIniFile);
	//批量召唤结果中, "尚未召唤"的文字颜色
	str.Format("%d", g_BatchLater);
	WritePrivateProfileString("BatchConfig", "BatchLater", str, strIniFile);
	//自动总召模块所在的IP地址
	WritePrivateProfileString("BatchConfig", "AutoCall_IP", g_AutoCall_IP, strIniFile);
	//自动总召模块的监听端口
	str.Format("%d", g_AutoCall_Port);
	WritePrivateProfileString("BatchConfig", "AutoCall_Port", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadPTCommViewConfig()
 功能概要：读入保护通讯状态页面配置
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0224
void CXJBrowserApp::LoadPTCommViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//断讯正常,绿色
	XJ_STATUS_COMM_ON = GetPrivateProfileInt("PTCommViewConfig", "Comm_status_on", RGB(0, 111, 0), strIniFile);
	//通讯断开,红色
	XJ_STATUS_COMM_OFF = GetPrivateProfileInt("PTCommViewConfig", "Comm_status_off", RGB(225, 0, 0), strIniFile);
	//列表中是否显示通信状态正常的装置信息.0-不显示. 1-显示
	g_ShowPTCommON = GetPrivateProfileInt("PTCommViewConfig", "ShowPTCommON", 1, strIniFile);
	//列表中是否显示通信状态断开的装置信息.0-不显示. 1-显示
	g_ShowPTCommOFF = GetPrivateProfileInt("PTCommViewConfig", "ShowPTCommOFF", 1, strIniFile);
	//刷新保护通信状态持续时间的间隔时间,单位(分钟)
	g_RefreshPTCommTime = GetPrivateProfileInt("PTCommViewConfig", "RefreshPTCommTime", 10, strIniFile);
}

/*************************************************************
 函 数 名：SavePTCommViewConfig()
 功能概要：保存保护通讯状态页面配置到配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0232
void CXJBrowserApp::SavePTCommViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", XJ_STATUS_COMM_ON);
	//断讯正常,绿色
	WritePrivateProfileString("PTCommViewConfig", "Comm_status_on", str, strIniFile);
	//断讯正常,绿色
	str.Format("%d", XJ_STATUS_COMM_OFF);
	WritePrivateProfileString("PTCommViewConfig", "Comm_status_off", str, strIniFile);
	//列表中是否显示通信状态正常的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTCommON);
	WritePrivateProfileString("PTCommViewConfig", "ShowPTCommON", str, strIniFile);
	//列表中是否显示通信状态断开的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTCommOFF);
	WritePrivateProfileString("PTCommViewConfig", "ShowPTCommOFF", str, strIniFile);
	//刷新保护通信状态持续时间的间隔时间,单位(分钟)
	str.Format("%d", g_RefreshPTCommTime);
	WritePrivateProfileString("PTCommViewConfig", "RefreshPTCommTime", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadPTRunViewConfig()
 功能概要：读入保护运行状态页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0233
void CXJBrowserApp::LoadPTRunViewConfig()
{
	CString strIniFile = g_SysConfig;

	//运行, 绿色
	XJ_STATUS_RUN_ON = GetPrivateProfileInt("PTRunViewConfig", "Run_status_on", RGB(0, 111, 0), strIniFile);
	//停用, 淡绿
	XJ_STATUS_RUN_OFF = GetPrivateProfileInt("PTRunViewConfig", "Run_status_off", RGB(162, 230, 165), strIniFile);
	//检修/挂牌, 黄色
	XJ_STATUS_RUN_CHECK = GetPrivateProfileInt("PTRunViewConfig", "Run_status_check", RGB(240, 240, 0), strIniFile);
	//定值修改挂牌
	XJ_STATUS_RUN_MARK = GetPrivateProfileInt("PTRunViewConfig", "Run_status_mark", RGB(250, 120, 0), strIniFile);
	//调试/对码表, 紫色
	XJ_STATUS_RUN_DEBUG = GetPrivateProfileInt("PTRunViewConfig", "Run_status_debug", RGB(157, 0, 183), strIniFile);
	//未接入，灰色
	XJ_STATUS_RUN_CODE = GetPrivateProfileInt("PTRunViewConfig", "Run_status_code", RGB(121, 121, 121), strIniFile);
	//列表中是否显示运行状态正常的装置信息.0-不显示. 1-显示
	g_ShowPTRunON = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunON", 1, strIniFile);
	//列表中是否显示运行状态检修的装置信息.0-不显示. 1-显示
	g_ShowPTRunCheck = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunCheck", 1, strIniFile);
	//列表中是否显示运行状态停运的装置信息.0-不显示. 1-显示
	g_ShowPTRunOFF = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunOFF", 1, strIniFile);
	//列表中是否显示运行状态调试的装置信息.0-不显示. 1-显示
	g_ShowPTRunDebug = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunDebug", 1, strIniFile);
	//列表中是否显示运行状态对码表的装置信息.0-不显示. 1-显示
	g_ShowPTRunCode = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunCode", 1, strIniFile);
	//刷新保护运行状态持续时间的间隔时间,单位(分钟)
	g_RefreshPTRunTime = GetPrivateProfileInt("PTRunViewConfig", "RefreshPTRunTime", 10, strIniFile);


	//厂站信息页面刷新时间
	g_RefreshTime = GetPrivateProfileInt("StationInfoViewConfig", "RefreshStationInfoTime", 10, strIniFile);
}

/*************************************************************
 函 数 名：SavePTRunViewConfig()
 功能概要：保存保护运行状态页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0234
void CXJBrowserApp::SavePTRunViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//运行, 绿色
	str.Format("%d", XJ_STATUS_RUN_ON);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_on", str, strIniFile);
	//停用, 淡绿
	str.Format("%d", XJ_STATUS_RUN_OFF);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_off", str, strIniFile);
	//检修/挂牌, 灰色
	str.Format("%d", XJ_STATUS_RUN_CHECK);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_check", str, strIniFile);
	//停用, 淡绿
	str.Format("%d", XJ_STATUS_RUN_DEBUG);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_debug", str, strIniFile);
	//定值修改挂牌
	str.Format("%d", XJ_STATUS_RUN_MARK);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_mark", str, strIniFile);
	//对码表, 紫色
	str.Format("%d", XJ_STATUS_RUN_CODE);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_code", str, strIniFile);
	//列表中是否显示运行状态正常的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTRunON);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunON", str, strIniFile);
	//列表中是否显示运行状态检修的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTRunCheck);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunCheck", str, strIniFile);
	//列表中是否显示运行状态停运的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTRunOFF);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunOFF", str, strIniFile);
	//列表中是否显示运行状态调试的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTRunDebug);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunDebug", str, strIniFile);
	//列表中是否显示运行状态对码表的装置信息.0-不显示. 1-显示
	str.Format("%d", g_ShowPTRunCode);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunCode", str, strIniFile);

	//刷新保护运行状态持续时间的间隔时间,单位(分钟)
	str.Format("%d", g_RefreshPTRunTime);
	WritePrivateProfileString("PTRunViewConfig", "RefreshPTRunTime", str, strIniFile);

	//厂站信息页面刷新时间
	str.Format("%d", g_RefreshTime);
	g_RefreshTime = WritePrivateProfileString("StationInfoViewConfig", "RefreshStationInfoTime", str, strIniFile);
}

/*************************************************************
 函 数 名：LoadConfig()
 功能概要：读入所有配置文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B5
void CXJBrowserApp::LoadConfig()
{
	LoadSysConfig();
	LoadEventConfig();
	LoadActionViewConfig();
	LoadAlarmViewConfig();
	LoadBatchConfig();
	LoadCommStatusConfig();
	LoadPTCommViewConfig();
	LoadPTRunViewConfig();
	LoadSysAlarmViewConfig();
	LoadPTPropConfig();
	LoadWndConfig();
	LoadSplitterWndConfig();
}

/*************************************************************
 函 数 名：SaveConfig()
 功能概要：保存所有配置
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B6
void CXJBrowserApp::SaveConfig()
{
	SaveSysConfig();
	SaveEventConfig();
	SaveActionViewConfig();
	SaveAlarmViewConfig();
	SaveBatchConfig();
	SaveCommStatusConfig();
	SavePTCommViewConfig();
	SavePTRunViewConfig();
	SaveSysAlarmViewConfig();
	SavePTPropConfig();
	SaveWndConfig();
	SaveSplitterWndConfig();
}

//##ModelId=49B87B7E02AF
void CXJBrowserApp::OnSttpConnect() 
{
	// TODO: Add your command handler code here
	if (m_bConnect)
	{
		DisConnectSTTPServer();
	}
	else
	{
		Sleep(1000);
		if(m_SttpHandle != NULL)
		{
			if(AfxMessageBox(StringFromID(IDS_TIP_RECONNECTING), MB_OKCANCEL) == IDOK)
			{
				//有连接句柄存在, 先关闭
				DisConnectSTTPServer();
				ConnectSTTPServerB();
			}
		}
		else
		{
			ConnectSTTPServerB();
		}
	}
}

//##ModelId=49B87B7E02BF
void CXJBrowserApp::OnUpdateSttpConnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(!m_bConnect);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (m_bConnect)
	{
		//pCmdUI->SetText("断开通信");
		pFrame->m_wndToolBar.SetButtonText(0, StringFromID(IDS_DISCONNECT_COMMUSERVER));
	}
	else
	{
		//pCmdUI->SetText("连接通信");
		pFrame->m_wndToolBar.SetButtonText(0, StringFromID(IDS_CONNECT_COMMUSERVER));
	}
}

//##ModelId=49B87B7E02C2
void CXJBrowserApp::OnSttpDisconnect() 
{
	// TODO: Add your command handler code here
	DisConnectSTTPServer();
}

//##ModelId=49B87B7E02C4
void CXJBrowserApp::OnUpdateSttpDisconnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bConnect);
}

//##ModelId=49B87B7E02D0
void CXJBrowserApp::OnSysLogout() 
{
	// TODO: Add your command handler code here
	DoLogOut();
}

//##ModelId=49B87B7E02D2
void CXJBrowserApp::OnUpdateSysLogout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_User.m_strUSER_ID.IsEmpty()) //匿名用户
	{
	//	pCmdUI->Enable(FALSE);
		pCmdUI->SetText(StringFromID(IDS_COMMON_WRITEOFF)+"[&Z]...");
	}
	else
	{
		CString strTemp;
		strTemp.Format ("%s %s [&Z]...", StringFromID(IDS_COMMON_WRITEOFF),m_User.m_strUSER_ID);
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText (strTemp);
	}
}

/*************************************************************
 函 数 名：OpenDefaultStation()
 功能概要：打开默认厂站图形和在设备树中选择
 返 回 值: void
 参    数：param1	厂站ID
		   Param2
**************************************************************/
//##ModelId=49B87B7E0244
void CXJBrowserApp::OpenDefaultStation( CString strID )
{
	CDataEngine* pData = GetDataEngine();
	if(pData == NULL)
		return;
	CStationObj* pStation = NULL;
	if(strID == "")	//ID为空,不处理
	{
		if( g_role == MODE_SUB ) //子站默认打开图形
		{
			//查找厂站指针 
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice);
			POSITION pos = listDevice.GetHeadPosition();
			MYASSERT(pos);
			pStation = (CStationObj*)listDevice.GetAt(pos);
		}
	}
	else
	{
		//查找厂站指针
		pStation = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
		
	}
	if(pStation == NULL) //查找不到厂站, 不处理
		return;
	
	//打开图形
	if(g_bShowDeviceMapWnd)
		ShowDevice(pStation, FALSE);
	else
		ShowStationInfo(pStation);
	//在设备树中选择
	LocateInTree(pStation);
}

void CXJBrowserApp::OpenDefaultStation()
{

	CDataEngine* pData = GetDataEngine();
	MYASSERT(pData);
	//查找厂站指针 
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	MYASSERT(pos);
	CStationObj* pStation = (CStationObj*)listDevice.GetAt(pos);
	MYASSERT(pStation);
	//打开图形
	ShowDevice(pStation, FALSE);
	//在设备树中选择
	LocateInTree(pStation);
}


//##ModelId=49B87B7E02DF
void CXJBrowserApp::OnSysExit() 
{
	// TODO: Add your command handler code here
	g_bExit = 1;
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}

//##ModelId=49B87B7E02E1
void CXJBrowserApp::OnUpdateSysExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

//##ModelId=49B87B7E02EE
void CXJBrowserApp::OnHelpContent() 
{
	// TODO: Add your command handler code here
	return;
}

//##ModelId=49B87B7E02F0
void CXJBrowserApp::OnUpdateHelpContent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

/*************************************************************
 函 数 名：QueryUserGroup()
 功能概要：指定用户名查找用户组名
 返 回 值: 用户组名
 参    数：param1	用户名
		   Param2
**************************************************************/
//##ModelId=49B87B7E0251
CString CXJBrowserApp::QueryUserGroup( CString sUserID )
{
	CString strReturn = "";

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//group_id
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "group_id", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//条件
	//USER_ID
	Condition condition2;
	str.Format("USER_ID = '%s'", sUserID);
	m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_USER_CFG, sql, sError, pMemset);
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount <= 0)
		{
			strReturn = "";
		}
		else
		{
			//只应该有一个值
			strReturn = pMemset->GetValue((UINT)0); //用户组ID
		}
	}
	else
	{
		//查询数据库失败
		strReturn = "";
	}
	
	delete[] sError;
	delete pMemset;
	return strReturn;
}

//##ModelId=49B87B7E02FD
void CXJBrowserApp::OnSysSetting() 
{
	// TODO: Add your command handler code here
	if (g_bVerify)
	{
		//验证权限
		if(!TryEnter(FUNC_XJBROWSER_SETTING))
		{
			return;
		}
	}
	DoSysSetting(SYS_SETTING_GENERAL);
	//记录操作
	//AddManOperator(FUNC_XJBROWSER_SETTING, FUNC_SYS_SETTING);
}

//##ModelId=49B87B7E02FF
void CXJBrowserApp::OnUpdateSysSetting(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

/*************************************************************
 函 数 名：DoSysSetting()
 功能概要：进入系统设置页面
 返 回 值: 
 参    数：param1	初始打开哪个SHEET页. SYS_SETTING_GENERAL为基本设置页面. SYS_SETTING_INFOCASE为信息定制页面. const SYS_SETTING_STATUS为状态设置页面. SYS_SETTING_PTPTOP为保护属性设置页面
		   Param2
**************************************************************/
//##ModelId=49B87B7E0253
void CXJBrowserApp::DoSysSetting( int nPage )
{
	CSysSettingSheet dlg(StringFromID(IDS_TOOLBAR_SYSCONF), NULL, nPage);

	BOOL bNeedRefreshActionView = FALSE; //是否需要刷新动作事件视图
	BOOL bNeedRefreshAlarmView = FALSE; //是否需要刷新告警事件视图
	BOOL bNeedRefreshPTSetting = FALSE; //是否需要刷新保护定值页面
	BOOL bNeedRefreshPTDI = FALSE; //是否需要刷新保护开关量页面
	BOOL bNeedRefreshPTSoft = FALSE; //是否需要刷新保护软压板页面
	
	//系统设置页面
	dlg.m_PageGeneral.m_bDeviceTree = (g_ShowDeviceTreeWnd == 1);
	dlg.m_PageGeneral.m_bOutPut = (g_ShowOutputWnd == 1);
	dlg.m_PageGeneral.m_bActionDetail = (g_ShowActionDetail == 1);
	dlg.m_PageGeneral.m_bSysAlarmDetail = (g_ShowSysAlarmDetail == 1);
	dlg.m_PageGeneral.m_btnBkColor.m_Color = g_CommMapBK;
	dlg.m_PageGeneral.m_btnFontColor.m_Color = g_CommMapFontColor;
	

	//信息定制页面
	//I类
	dlg.m_PageInfoCase.m_bShowViewI = (g_ShowEventI == 1);
	dlg.m_PageInfoCase.m_bAlarmViewI = (g_AlarmEventI == 1 && g_ShowEventI == 1);
	dlg.m_PageInfoCase.m_btnViewIColor.m_Color = g_colEventI;
	//II类
	dlg.m_PageInfoCase.m_bShowViewII = (g_ShowEventII == 1);
	dlg.m_PageInfoCase.m_bAlarmViewII = (g_AlarmEventII == 1 && g_ShowEventII == 1);
	dlg.m_PageInfoCase.m_btnViewIIColor.m_Color = g_colEventII;
	//III类
	dlg.m_PageInfoCase.m_bShowViewIII = (g_ShowEventIII == 1);
	dlg.m_PageInfoCase.m_bAlarmViewIII = (g_AlarmEventIII == 1 && g_ShowEventIII == 1);
	dlg.m_PageInfoCase.m_btnViewIIIColor.m_Color = g_colEventIII;
	//报警设置
	dlg.m_PageInfoCase.m_bBlinkSec = (g_BlinkDevice == 1);
	dlg.m_PageInfoCase.m_bBlinkDevice = (g_BlinkOwner == 1 && g_BlinkDevice == 1);
	dlg.m_PageInfoCase.m_bPopWnd = (g_ShowEventDlg == 1);
	dlg.m_PageInfoCase.m_bTopSec = (g_MustShowDevice == 1);
	dlg.m_PageInfoCase.m_bUseSound = (g_UseAlarmSound == 1);
	dlg.m_PageInfoCase.m_bAutoChange = (g_AutoChangeMap == 1);
	dlg.m_PageInfoCase.m_nAlarmTime = g_SoundTime;
	dlg.m_PageInfoCase.m_nFlashTime = g_FlashTime;

	//设备状态设置页面
	//通讯状态
	dlg.m_PageStatus.m_btnCommON.m_Color = XJ_STATUS_COMM_ON;
	dlg.m_PageStatus.m_btnCommOFF.m_Color = XJ_STATUS_COMM_OFF;
	//运行状态
	dlg.m_PageStatus.m_btnRunON.m_Color = XJ_STATUS_RUN_ON;
	dlg.m_PageStatus.m_btnRunOFF.m_Color = XJ_STATUS_RUN_OFF;
	dlg.m_PageStatus.m_btnRunCheck.m_Color = XJ_STATUS_RUN_CHECK;
	dlg.m_PageStatus.m_btnRunCode.m_Color = XJ_STATUS_RUN_CODE;
	dlg.m_PageStatus.m_btnRunDebug.m_Color = XJ_STATUS_RUN_DEBUG;
	dlg.m_PageStatus.m_btnRunHangout.m_Color = XJ_STATUS_RUN_MARK;

	//保护属性页面
	dlg.m_PagePTProp.m_nSelSettingValues = g_PTSettingValues;
	dlg.m_PagePTProp.m_bSettingGroup = (g_PTSettingGroup == 1);
	dlg.m_PagePTProp.m_nSelDIValues = g_PTDIValues;
	dlg.m_PagePTProp.m_nSelDIStyle = g_PTDIStyle;
	dlg.m_PagePTProp.m_nSelSoftValues = g_PTSoftValues;
	dlg.m_PagePTProp.m_nSelSoftStyle = g_PTSoftStyle;
	dlg.m_PagePTProp.m_bPTAI = (g_PTAIValues == 1);

	if(dlg.DoModal() == IDOK)
	{
		int nTemp = 0;
		COLORREF colTemp;
		//保存系统设置
		g_ShowDeviceTreeWnd = dlg.m_PageGeneral.m_bDeviceTree?1:0;
		g_ShowOutputWnd = dlg.m_PageGeneral.m_bOutPut?1:0;
		g_ShowActionDetail = dlg.m_PageGeneral.m_bActionDetail?1:0;
		g_ShowSysAlarmDetail = dlg.m_PageGeneral.m_bSysAlarmDetail?1:0;
		g_strStyleName = dlg.m_PageGeneral.m_sStyleName;
		CString str;
		if (dlg.m_PageGeneral.m_iSel != -1)
		{
			str.Format("%d", dlg.m_PageGeneral.m_iSel);
			WritePrivateProfileString("SysConfig", "Style", str, g_SysConfig);
		}
		WritePrivateProfileString("Style", "StyleName", g_strStyleName, g_SysConfig);

		g_CommMapBK = dlg.m_PageGeneral.m_btnBkColor.m_Color;
		g_CommMapFontColor = dlg.m_PageGeneral.m_btnFontColor.m_Color;
		g_CommMapFont = dlg.m_PageGeneral.m_sFont;
		g_CommMapFontSize = dlg.m_PageGeneral.m_nFontSize;

		//保存信息定制设置
		//I类
		nTemp = dlg.m_PageInfoCase.m_bShowViewI?1:0;
		if(nTemp != g_ShowEventI)
		{
			//保存, 需要刷新
			g_ShowEventI = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventI = dlg.m_PageInfoCase.m_bAlarmViewI?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIColor.m_Color;
		if(colTemp != g_colEventI)
		{
			//保存, 需要刷新
			g_colEventI = colTemp;
		}

		//II类
		nTemp = dlg.m_PageInfoCase.m_bShowViewII?1:0;
		if(nTemp != g_ShowEventII)
		{
			//保存,需要刷新
			g_ShowEventII = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventII = dlg.m_PageInfoCase.m_bAlarmViewII?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIIColor.m_Color;
		if(colTemp != g_colEventII)
		{
			//保存, 需要刷新
			g_colEventII = colTemp;
		}

		//III类
		nTemp = dlg.m_PageInfoCase.m_bShowViewIII?1:0;
		if(nTemp != g_ShowEventIII)
		{
			//保存, 需要刷新
			g_ShowEventIII = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventIII = dlg.m_PageInfoCase.m_bAlarmViewIII?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIIIColor.m_Color;
		if(colTemp != g_colEventIII)
		{
			//保存, 需要刷新
			g_colEventIII = colTemp;
		}
		//报警设置
		g_BlinkDevice = dlg.m_PageInfoCase.m_bBlinkSec?1:0;
		g_BlinkOwner = dlg.m_PageInfoCase.m_bBlinkDevice?1:0;
		g_ShowEventDlg = dlg.m_PageInfoCase.m_bPopWnd?1:0;
		g_MustShowDevice = dlg.m_PageInfoCase.m_bTopSec?1:0;
		g_UseAlarmSound = dlg.m_PageInfoCase.m_bUseSound?1:0;
		g_AutoChangeMap = dlg.m_PageInfoCase.m_bAutoChange?1:0;
		g_SoundTime = dlg.m_PageInfoCase.m_nAlarmTime;
		g_FlashTime = dlg.m_PageInfoCase.m_nFlashTime;

		//保存设备状态设置
		//通讯状态
		XJ_STATUS_COMM_ON = dlg.m_PageStatus.m_btnCommON.m_Color;
		XJ_STATUS_COMM_OFF = dlg.m_PageStatus.m_btnCommOFF.m_Color;
		//运行状态
		XJ_STATUS_RUN_ON = dlg.m_PageStatus.m_btnRunON.m_Color;
		XJ_STATUS_RUN_OFF = dlg.m_PageStatus.m_btnRunOFF.m_Color;
		XJ_STATUS_RUN_CHECK = dlg.m_PageStatus.m_btnRunCheck.m_Color;
		XJ_STATUS_RUN_CODE = dlg.m_PageStatus.m_btnRunCode.m_Color;
		XJ_STATUS_RUN_DEBUG = dlg.m_PageStatus.m_btnRunDebug.m_Color;
		XJ_STATUS_RUN_MARK = dlg.m_PageStatus.m_btnRunHangout.m_Color;

		//保存保护属性设置
		g_PTSettingValues = dlg.m_PagePTProp.m_nSelSettingValues;
		nTemp = dlg.m_PagePTProp.m_bSettingGroup?1:0;
		if(g_PTSettingGroup != nTemp)
		{
			g_PTSettingGroup = nTemp;
			bNeedRefreshPTSetting = TRUE;
		}
		g_PTDIValues = dlg.m_PagePTProp.m_nSelDIValues;
		nTemp = dlg.m_PagePTProp.m_nSelDIStyle;
		if(g_PTDIStyle != nTemp)
		{
			g_PTDIStyle = nTemp;
			bNeedRefreshPTDI = TRUE;
		}
		g_PTSoftValues = dlg.m_PagePTProp.m_nSelSoftValues;
		nTemp = dlg.m_PagePTProp.m_nSelSoftStyle;
		if(g_PTSoftStyle != nTemp)
		{
			g_PTSoftStyle = nTemp;
			bNeedRefreshPTSoft = TRUE;
		}
		g_PTAIValues = dlg.m_PagePTProp.m_bPTAI?1:0;

		if (dlg.m_PageStyle.m_bOper && g_style == 3)
		{
			AfxMessageBox(StringFromID(IDS_TIP_RESTART_APP));
		}

	}

	//刷新界面显示
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(bNeedRefreshActionView)
	{
		WriteLog("设置改变, 需要刷新动作事件显示");		
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_ACTIONVIEW);
	}
	if(bNeedRefreshAlarmView)
	{
		WriteLog("设置改变, 需要刷新告警事件显示");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_ALARMVIEW);
	}
	if(bNeedRefreshPTSetting)
	{
		WriteLog("设置改变, 需要刷新保护定值显示");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTSETTING);
	}
	if(bNeedRefreshPTDI)
	{
		WriteLog("设置改变, 需要刷新保护开关量显示");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTDI);
	}
	if(bNeedRefreshPTSoft)
	{
		WriteLog("设置改变, 需要刷新保护软压板显示");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTSOFTBOARD);
	}
	if(pFrame->pCommFrame)
	{
		pFrame->pCommFrame->PostMessage(REFRESH_COMMMAP);
	}
}

/*************************************************************
 函 数 名：LoadPTPropConfig()
 功能概要：读入保护属性页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0242
void CXJBrowserApp::LoadPTPropConfig()
{
	CString strIniFile = g_SysConfig;
	
	//定值历史值载入项数. 0-10整数
	g_PTSettingValues = GetPrivateProfileInt("PTPropConfig", "SettingValues", 5, strIniFile);
	if(g_PTSettingValues < 0 || g_PTSettingValues > 10)
		g_PTSettingValues = 5;
	//定值是否按组显示. 0-否, 1-是
	g_PTSettingGroup = GetPrivateProfileInt("PTPropConfig", "SettingGroup", 1, strIniFile);
	//定值与哪项比较, 0-与前一次值比较, 1-与基准值比较
	g_PTSettingCompare = GetPrivateProfileInt("PTPropConfig", "SettingCompare", 0, strIniFile);
	//开关量历史值载入项数. 0-10整数
	g_PTDIValues = GetPrivateProfileInt("PTPropConfig", "DIValues", 5, strIniFile);
	if(g_PTDIValues < 0 || g_PTDIValues > 10)
		g_PTDIValues = 5;
	//开关量值显示风格.0: 1/0, 1: ON/OFF, 2: 投/退, 3: 合/开.
	g_PTDIStyle = GetPrivateProfileInt("PTPropConfig", "DIStyle", 5, strIniFile);
	if(g_PTDIStyle < 0 || g_PTDIStyle > 4)
		g_PTDIStyle = 0;
	//软压板历史值载入项数. 0-10整数
	g_PTSoftValues = GetPrivateProfileInt("PTPropConfig", "SoftValues", 5, strIniFile);
	if(g_PTSoftValues < 0 || g_PTSoftValues > 10)
		g_PTSoftValues = 5;
	//软压板值显示风格: 0: 1/0, 1: ON/OFF, 3: 投/退.
	g_PTSoftStyle = GetPrivateProfileInt("PTPropConfig", "SoftStyle", 5, strIniFile);
	if(g_PTSoftStyle < 0 || g_PTSoftStyle > 3)
		g_PTSoftStyle = 0;
	//模拟量是否显示历史值 0:不显示. 1:显示
	g_PTAIValues = GetPrivateProfileInt("PTPropConfig", "AIValues", 1, strIniFile);
	g_PTAIRealtimeValue = GetPrivateProfileInt("PTPropConfig", "AIRealtimeValue", 0, strIniFile);
	g_PTAIRealtimeRefresh = GetPrivateProfileInt("PTPropConfig", "AIRealtimeRefresh", 60, strIniFile);
	//控制功能是否可用. 0:不可用. 1:可用
	g_PTControl = GetPrivateProfileInt("PTPropConfig", "ControlFunc", 0, strIniFile);
	//基本属性页面的"显示更多"按钮是否显示.0-否, 1-是
	g_PTGeneralMore = GetPrivateProfileInt("PTPropConfig", "GeneralMore", 0, strIniFile);
	//动作事件显示数
	g_PTActionNum = GetPrivateProfileInt("PTPropConfig", "PTActionNum", 100, strIniFile);
	//告警事件显示数
	g_PTAlarmNum = GetPrivateProfileInt("PTPropConfig", "PTAlarmNum", 100, strIniFile);	
	int nDefault = 1;
	if(g_role == MODE_SUB)//子站时默认不显示
		nDefault = 0;
	g_PTShowHistory = GetPrivateProfileInt("PTPropConfig", "PTShowHistory", nDefault, strIniFile);
	g_PTNewZoneTable = GetPrivateProfileInt("PTPropConfig", "PTNewZoneTable", 0, strIniFile);
	g_PTControlModel = GetPrivateProfileInt("PTPropConfig", "PTControlModel", 1, strIniFile);
	g_PTModifyZone = GetPrivateProfileInt("PTPropConfig", "PTModifyzone", 1, strIniFile);
	g_PTShowGeneral = GetPrivateProfileInt("PTPropConfig", "PTShowGeneral", 1, strIniFile);	//基本信息页面是否显示。0-否，1-是
	g_PTShowSetting = GetPrivateProfileInt("PTPropConfig", "PTShowSetting", 1, strIniFile);	//定值页面是否显示。0-否，1-是
	g_PTShowDI = GetPrivateProfileInt("PTPropConfig", "PTShowDI", 1, strIniFile);			//开关量页面是否显示。0-否，1-是
	g_PTShowSoftBoard = GetPrivateProfileInt("PTPropConfig", "PTShowSoftBoard", 1, strIniFile);	//软压板页面是否显示。0-否，1-是
	g_PTShowAI = GetPrivateProfileInt("PTPropConfig", "PTShowAI", 1, strIniFile);			//模拟量页面是否显示。0-否，1-是
	g_PTShowAction = GetPrivateProfileInt("PTPropConfig", "PTShowAction", 1, strIniFile);		//动作页面是否显示。0-否，1-是
	g_PTShowAlarm = GetPrivateProfileInt("PTPropConfig", "PTShowAlarm", 1, strIniFile);		//告警页面是否显示。0-否，1-是
	g_PTShowOsc = GetPrivateProfileInt("PTPropConfig", "PTShowOSC", 1, strIniFile);		//本地录波页面是否显示。0-否，1-是
	g_PTShowOscList = GetPrivateProfileInt("PTPropConfig", "PTShowOscList", 1, strIniFile);	//远方录波页面是否显示。0-否，1-是
	g_PTShowNote = GetPrivateProfileInt("PTPropConfig", "PTShowNote", 0, strIniFile);		//备注面是否显示。0-否，1-是
	g_PTSettingJudge = GetPrivateProfileInt("PTPropConfig", "PTSettingJudge", 0, strIniFile);
	g_PTSetting160x = GetPrivateProfileInt("PTPropConfig", "PTSetting160x", 0, strIniFile);

	g_WRShowGeneral = GetPrivateProfileInt("WRPropConfig", "WRShowGeneral", 1, strIniFile);	//基本信息页面是否显示。0-否，1-是
	g_WRShowSetting = GetPrivateProfileInt("WRPropConfig", "WRShowSetting", 1, strIniFile);	//定值页面是否显示。0-否，1-是
	g_WRShowDI = GetPrivateProfileInt("WRPropConfig", "WRShowDI", 1, strIniFile);			//开关量页面是否显示。0-否，1-是
	g_WRShowAI = GetPrivateProfileInt("WRPropConfig", "WRShowAI", 1, strIniFile);			//模拟量页面是否显示。0-否，1-是
	g_WRShowOsc = GetPrivateProfileInt("WRPropConfig", "WRShowOSC", 1, strIniFile);		//本地录波页面是否显示。0-否，1-是
	g_WRShowOscList = GetPrivateProfileInt("WRPropConfig", "WRShowOscList", 1, strIniFile);	//远方录波页面是否显示。0-否，1-是
	g_WRShowNote = GetPrivateProfileInt("WRPropConfig", "WRShowNote", 0, strIniFile);		//备注面是否显示。0-否，1-是
	g_PTIndexType = GetPrivateProfileInt("PTPropConfig", "PTIndexType", 0, strIniFile);
	g_PTNotModifyCurrentZone = GetPrivateProfileInt("PTPropConfig", "NotModifyCurrentZone", 0, strIniFile);
	g_PTOscListCompare = GetPrivateProfileInt("PTPropConfig", "OscListCompare", 1, strIniFile);
}

/*************************************************************
 函 数 名：SavePTPropConfig()
 功能概要：保存保护属性页面配置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0243
void CXJBrowserApp::SavePTPropConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;

	//定值历史值载入项数. 0-10整数
	str.Format("%d", g_PTSettingValues);
	WritePrivateProfileString("PTPropConfig", "SettingValues", str, strIniFile);
	//定值是否按组显示. 0-否, 1-是
	str.Format("%d", g_PTSettingGroup);
	WritePrivateProfileString("PTPropConfig", "SettingGroup", str, strIniFile);
	//定值与哪项比较, 0-与前一次值比较, 1-与基准值比较
	str.Format("%d", g_PTSettingCompare);
	WritePrivateProfileString("PTPropConfig", "SettingCompare", str, strIniFile);
	//开关量历史值载入项数. 0-10整数
	str.Format("%d", g_PTDIValues);
	WritePrivateProfileString("PTPropConfig", "DIValues", str, strIniFile);
	//开关量值显示风格.0: 1/0, 1: ON/OFF, 2: 投/退, 3: 合/开.
	str.Format("%d", g_PTDIStyle);
	WritePrivateProfileString("PTPropConfig", "DIStyle", str, strIniFile);
	//软压板历史值载入项数. 0-10整数
	str.Format("%d", g_PTSoftValues);
	WritePrivateProfileString("PTPropConfig", "SoftValues", str, strIniFile);
	//软压板值显示风格: 0: 1/0, 1: ON/OFF, 3: 投/退.
	str.Format("%d", g_PTSoftStyle);
	WritePrivateProfileString("PTPropConfig", "SoftStyle", str, strIniFile);
	//模拟量是否显示历史值 0:不显示. 1:显示
	str.Format("%d", g_PTAIValues);
	WritePrivateProfileString("PTPropConfig", "AIValues", str, strIniFile);

	str.Format("%d", g_PTAIRealtimeValue);
	WritePrivateProfileString("PTPropConfig", "AIRealtimeValue", str, strIniFile);

	str.Format("%d", g_PTAIRealtimeRefresh);
	WritePrivateProfileString("PTPropConfig", "AIRealtimeRefresh", str, strIniFile);
	//控制功能是否可用. 0:不可用. 1:可用
	str.Format("%d", g_PTControl);
	WritePrivateProfileString("PTPropConfig", "ControlFunc", str, strIniFile);
	//基本属性页面的"显示更多"按钮是否显示.0-否, 1-是
	str.Format("%d", g_PTGeneralMore);
	WritePrivateProfileString("PTPropConfig", "GeneralMore", str, strIniFile);
	//保护动作显示数
	str.Format("%d", g_PTActionNum);
	WritePrivateProfileString("PTPropConfig", "PTActionNum", str, strIniFile);
	//保护告警显示数
	str.Format("%d", g_PTAlarmNum);
	WritePrivateProfileString("PTPropConfig", "PTAlarmNum", str, strIniFile);
	//历史事件页面是否显示
	str.Format("%d", g_PTShowHistory);
	WritePrivateProfileString("PTPropConfig", "PTShowHistory", str, strIniFile);
	str.Format("%d", g_PTNewZoneTable);
	WritePrivateProfileString("PTPropConfig", "PTNewZoneTable", str, strIniFile);
// 	str.Format("%d", g_PTControlModel);
// 	WritePrivateProfileString("PTPropConfig", "PTControlModel", str, strIniFile);
	str.Format("%d", g_PTModifyZone);
	WritePrivateProfileString("PTPropConfig", "PTModifyzone", str, strIniFile);
	str.Format("%d", g_PTShowGeneral);
	WritePrivateProfileString("PTPropConfig", "PTShowGeneral", str, strIniFile);	//基本信息页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowSetting);
	WritePrivateProfileString("PTPropConfig", "PTShowSetting", str, strIniFile);	//定值页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowDI);
	WritePrivateProfileString("PTPropConfig", "PTShowDI", str, strIniFile);			//开关量页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowSoftBoard);
	WritePrivateProfileString("PTPropConfig", "PTShowSoftBoard", str, strIniFile);	//软压板页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowAI);
	WritePrivateProfileString("PTPropConfig", "PTShowAI", str, strIniFile);			//模拟量页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowAction);
	WritePrivateProfileString("PTPropConfig", "PTShowAction", str, strIniFile);		//动作页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowAlarm);
	WritePrivateProfileString("PTPropConfig", "PTShowAlarm", str, strIniFile);		//告警页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowOsc);
	WritePrivateProfileString("PTPropConfig", "PTShowOSC", str, strIniFile);		//本地录波页面是否显示。0-否，1-是
	str.Format("%d", g_PTShowOscList);
	WritePrivateProfileString("PTPropConfig", "PTShowOscList", str, strIniFile);	//远方录波页面是否显示。0-否，1-是
	str.Format("%d", g_PTSettingJudge);
	WritePrivateProfileString("PTPropConfig", "PTSettingJudge", str, strIniFile);
	str.Format("%d", g_PTIndexType);
	WritePrivateProfileString("PTPropConfig", "PTIndexType", str, strIniFile);
	str.Format("%d", g_PTNotModifyCurrentZone);
	WritePrivateProfileString("PTPropConfig", "NotModifyCurrentZone", str, strIniFile);
	str.Format("%d", g_PTOscListCompare);
	WritePrivateProfileString("PTPropConfig", "OscListCompare", str, strIniFile);
	str.Format("%d", g_PTSetting160x);
	WritePrivateProfileString("PTPropConfig", "PTSetting160x", str, strIniFile);
	str.Format("%d", g_PTShowNote);
	WritePrivateProfileString("PTPropConfig", "PTShowNote", str, strIniFile);	//备注页面是否显示。0-否，1-是


	str.Format("%d", g_WRShowGeneral);
	WritePrivateProfileString("WRPropConfig", "WRShowGeneral", str, strIniFile);	//基本信息页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowSetting);
	WritePrivateProfileString("WRPropConfig", "WRShowSetting", str, strIniFile);	//定值页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowDI);
	WritePrivateProfileString("WRPropConfig", "WRShowDI", str, strIniFile);			//开关量页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowAI);
	WritePrivateProfileString("WRPropConfig", "WRShowAI", str, strIniFile);			//模拟量页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowOsc);
	WritePrivateProfileString("WRPropConfig", "WRShowOSC", str, strIniFile);		//本地录波页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowOscList);
	WritePrivateProfileString("WRPropConfig", "WRShowOscList", str, strIniFile);	//远方录波页面是否显示。0-否，1-是
	str.Format("%d", g_WRShowNote);
	WritePrivateProfileString("WRPropConfig", "WRShowNote", str, strIniFile);	//备注页面是否显示。0-否，1-是
}

/*************************************************************
 函 数 名：ClearAllTempView()
 功能概要：清除保存的零时视图指针
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0261
void CXJBrowserApp::ClearAllTempView()
{
	/** @brief           故障报告详细信息视图指针,用于打印故障报告时的回调函数*/
	m_pFaultDetail = NULL;
	
	/** @brief           一次接线图视图指针, 用于闪烁图形时的回调函数*/
	m_pMapView = NULL;
	
	/** @brief           保护定值页面指针, 用于打印定值的回调函数*/
	m_pPTSetting = NULL;
	
	/** @brief           保护模拟量页面指针, 用于打印模拟量的回调函数*/
	m_pPTAnalog = NULL;
	
	/** @brief           保护开关量页面指针, 用于打印开关量的回调函数*/
	m_pPTDigital = NULL;
	
	/** @brief           保护软压板页面指针, 用于打印软压板的回调函数*/
	m_pPTSoftBoard = NULL;
	
	/** @brief           录波器定值页面指针, 用于打印定值的回调函数*/
	m_pWRSetting = NULL;

	m_pPTAction = NULL;
	m_pPTAlarm = NULL;
}

/*************************************************************
 函 数 名：LoadSysLogPath()
 功能概要：从数据库中读取日志文件路径,保存在m_sSysLogPath中
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0158
void CXJBrowserApp::LoadSysLogPath()
{
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询comtrade_path
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "log_path", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_LOCAL_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadSysLogPath, 查询失败");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sSysLogPath = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("CXJBrowserApp::LoadSysLogPath, 路径为:%s", m_sSysLogPath);
			WriteLog(str);
		}
		else
		{
			WriteLog("找不到系统下载路径");
		}
		
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadSysLogPath, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	if(m_sSysLogPath != "")
	{
		//把有可能的"\\"都换成/
		m_sSysLogPath.Replace("\\", "/");
		if(m_sSysLogPath.Right(1)!="/")
			m_sSysLogPath += "/";
		if (m_sSysLogPath.Find(":") == -1)
		{
			if(m_sSysLogPath.Left(1) == ".")
			{
				CString strLocal = g_LocalPath;
				strLocal.Replace("\\", "/");
				m_sSysLogPath = strLocal + m_sSysLogPath;
			}
			else
			{
				if (m_sSysLogPath.Left(1) != "/")
				{
					m_sSysLogPath = "/" + m_sSysLogPath;
					m_sSysLogPath = "." + m_sSysLogPath;
					CString strLocal = g_LocalPath;
					strLocal.Replace("\\", "/");
					m_sSysLogPath = strLocal + m_sSysLogPath;
				}
				else
				{
					CString strLocal = g_LocalPath;
					int nFind = strLocal.Find(":", 0);
					if(nFind != -1)
					{
						CString sRoot = strLocal.Left(nFind+1);
						m_sSysLogPath = sRoot+m_sSysLogPath;
					}
				}
				
			}
		}
		
		//加上模块名
		m_sSysLogPath += "XJBrowser/";
	}
}

/*************************************************************
 函 数 名：GetSysLogPath()
 功能概要：取得日志文件路径
 返 回 值: 日志文件路径
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0167
CString CXJBrowserApp::GetSysLogPath()
{
	if(m_sSysLogPath == "")
		LoadSysLogPath();
	return m_sSysLogPath;
}

/*************************************************************
 函 数 名：AddNetworkDisk()
 功能概要：挂载网络存储器
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL CXJBrowserApp::AddNetworkDisk()
{
	BOOL bReturn = FALSE;

	//得到共享目录名
	CString strDir = GetDownDir();
	//去除后面的\\号
	while(strDir.Right(1) == "\\")
	{
		strDir.Delete(strDir.GetLength() - 1, 1);
	}

	NETRESOURCE nr;
	nr.dwType = RESOURCETYPE_ANY;
	nr.lpLocalName = NULL;
	nr.lpRemoteName = (LPSTR)(LPCTSTR)strDir;
	nr.lpProvider = NULL;

	CString str = "";
	DWORD dwReturn = 1;
	
	if(m_sNetworkPassword == "" && m_sNetworkUser == "")
	{
		dwReturn = WNetAddConnection2(&nr, (LPSTR)NULL, (LPSTR)NULL, FALSE);
	}
	else
	{
		dwReturn = WNetAddConnection2(&nr, m_sNetworkPassword, m_sNetworkUser, FALSE);
	}

	if(dwReturn == NO_ERROR)
	{
		bReturn = TRUE;
		CString strOut="";
		strOut.Format("挂载网络驱动器成功, 地址: %s", strDir);
		WriteLog(strOut, XJ_LOG_LV1);
	}
	else
	{
		bReturn = FALSE;
		CString strOut="";
		strOut.Format("挂载网络驱动器失败, 地址: %s", strDir);
		WriteLog(strOut, XJ_LOG_LV1);
		if(dwReturn == ERROR_ACCESS_DENIED)
		{
			WriteLog("Access to the network resource was denied", 3);
		}
		if(dwReturn == ERROR_ALREADY_ASSIGNED)
		{
			WriteLog("The local device specified by the lpLocalName member is already connected to a network resource", 3);
		}
		if(dwReturn == ERROR_BAD_DEV_TYPE)
		{
			WriteLog("The type of local device and the type of network resource do not match", 3);
		}
		if(dwReturn == ERROR_BAD_DEVICE)
		{
			WriteLog("The value specified by lpLocalName is invalid", 3);
		}
		if(dwReturn == ERROR_BAD_NET_NAME)
		{
			WriteLog("The value specified by the lpRemoteName member is not acceptable to any network resource provider, either because the resource name is invalid, or because the named resource cannot be located", 3);
		}
		if(dwReturn == ERROR_BAD_PROFILE)
		{
			WriteLog("The user profile is in an incorrect format", 3);
		}
		if(dwReturn == ERROR_BAD_PROVIDER)
		{
			WriteLog("The value specified by the lpProvider member does not match any provider", 3);
		}
		if(dwReturn == ERROR_BUSY)
		{
			WriteLog("The router or provider is busy, possibly initializing. The caller should retry", 3);
		}
		if(dwReturn == ERROR_CANCELLED)
		{
			WriteLog("The attempt to make the connection was cancelled by the user through a dialog box from one of the network resource providers, or by a called resource", 3);
		}
		if(dwReturn == ERROR_CANNOT_OPEN_PROFILE)
		{
			WriteLog("The system is unable to open the user profile to process persistent connections", 3);
		}
		if(dwReturn == ERROR_DEVICE_ALREADY_REMEMBERED)
		{
			WriteLog("An entry for the device specified by lpLocalName is already in the user profile", 3);
		}
		if(dwReturn == ERROR_EXTENDED_ERROR)
		{
			WriteLog("A network-specific error occurred. Call the WNetGetLastError function to obtain a description of the error", 3);
		}
		if(dwReturn == ERROR_INVALID_PASSWORD)
		{
			WriteLog("The specified password is invalid and the CONNECT_INTERACTIVE flag is not set", 3);
		}
		if(dwReturn == ERROR_NO_NET_OR_BAD_PATH)
		{
			WriteLog("The operation cannot be performed because a network component is not started or because a specified name cannot be used", 3);
		}
		if(dwReturn == ERROR_NO_NETWORK)
		{
			WriteLog("The network is unavailable", 3);
		}
	}
	return bReturn;
}

void CXJBrowserApp::LoadWndConfig()
{
	CString strIniFile = g_SysConfig;
	g_bShowActionWnd = GetPrivateProfileInt("WndConfig", "ActionWnd", 1, strIniFile);
	g_bShowAlarmWnd = GetPrivateProfileInt("WndConfig", "AlarmWnd", 1, strIniFile);
	g_bShowDIWnd = GetPrivateProfileInt("WndConfig", "DIWnd", 1, strIniFile);
	g_bShowFlautWnd = GetPrivateProfileInt("WndConfig", "FaultWnd", 1, strIniFile);
	g_bShowOSCWnd = GetPrivateProfileInt("WndConfig", "OSCWnd", 1, strIniFile);
	g_bShowPTCommWnd = GetPrivateProfileInt("WndConfig", "PTCommWnd", 1, strIniFile);
	g_bShowPTRunWnd = GetPrivateProfileInt("WndConfig", "PTRunWnd", 1, strIniFile);
	g_bShowSysAlarmWnd = GetPrivateProfileInt("WndConfig", "SysAlarmWnd", 1, strIniFile);
	g_bShowCrossWnd = GetPrivateProfileInt("WndConfig", "CrossWnd", 0, strIniFile);
	g_bShowSOEIWnd = GetPrivateProfileInt("WndConfig", "SOEIWnd", 0, strIniFile);			//是否显示I类事件窗口
	g_bShowSOEIIWnd = GetPrivateProfileInt("WndConfig", "SOEIIWnd", 0, strIniFile);		//是否显示II类事件窗口
	g_bShowSOEIIIWnd = GetPrivateProfileInt("WndConfig", "SOEIIIWnd", 0, strIniFile);		//是否显示III类事件窗口
	g_bShowSOEDebugWnd = GetPrivateProfileInt("WndConfig", "SOEDebugWnd", 0, strIniFile);		//是否显示调试事件窗口
	g_bShowGeoWnd = GetPrivateProfileInt("WndConfig", "GeoMapWnd", 0, strIniFile);		//是否显示地理图窗口
	g_bShowDeviceMapWnd = GetPrivateProfileInt("WndConfig", "DeviceMapWnd", 1, strIniFile);		//是否显示一次接线图窗口
	g_bShowDistanceWnd = GetPrivateProfileInt("WndConfig", "DistanceWnd", 0, strIniFile);		//是否显示小波测距窗口
	g_bShowStationInfoWnd = GetPrivateProfileInt("WndConfig", "StationInfoWnd", 0, strIniFile);		//是否显示厂站信息窗口
	g_bShowRemindWnd = GetPrivateProfileInt("WndConfig", "RemindWnd", 1, strIniFile);		//是否显示厂站信息窗口	
	g_bShowDraft = GetPrivateProfileInt("WndConfig", "ShowDraft", 0, strIniFile);
}

void CXJBrowserApp::SaveWndConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	str.Format("%d", g_bShowActionWnd);
	WritePrivateProfileString("WndConfig", "ActionWnd", str, strIniFile);
	str.Format("%d", g_bShowAlarmWnd);
	WritePrivateProfileString("WndConfig", "AlarmWnd", str, strIniFile);
	str.Format("%d", g_bShowDIWnd);
	WritePrivateProfileString("WndConfig", "DIWnd", str, strIniFile);
	str.Format("%d", g_bShowFlautWnd);
	WritePrivateProfileString("WndConfig", "FaultWnd", str, strIniFile);
	str.Format("%d", g_bShowOSCWnd);
	WritePrivateProfileString("WndConfig", "OSCWnd", str, strIniFile);
	str.Format("%d", g_bShowPTCommWnd);
	WritePrivateProfileString("WndConfig", "PTCommWnd", str, strIniFile);
	str.Format("%d", g_bShowPTRunWnd);
	WritePrivateProfileString("WndConfig", "PTRunWnd", str, strIniFile);
	str.Format("%d", g_bShowSysAlarmWnd);
	WritePrivateProfileString("WndConfig", "SysAlarmWnd", str, strIniFile);
	str.Format("%d", g_bShowCrossWnd);
	WritePrivateProfileString("WndConfig", "CrossWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIWnd);
	WritePrivateProfileString("WndConfig", "SOEIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIIWnd);
	WritePrivateProfileString("WndConfig", "SOEIIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIIIWnd);
	WritePrivateProfileString("WndConfig", "SOEIIIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEDebugWnd);
	WritePrivateProfileString("WndConfig", "SOEDebugWnd", str, strIniFile);
	str.Format("%d", g_bShowGeoWnd);
	WritePrivateProfileString("WndConfig", "GeoMapWnd", str, strIniFile);
	str.Format("%d", g_bShowDeviceMapWnd);
	WritePrivateProfileString("WndConfig", "DeviceMapWnd", str, strIniFile);
	str.Format("%d", g_bShowDistanceWnd);
	WritePrivateProfileString("WndConfig", "DistanceWnd", str, strIniFile);
	str.Format("%d", g_bShowStationInfoWnd);
	WritePrivateProfileString("WndConfig", "StationInfoWnd", str, strIniFile);
	str.Format("%d", g_bShowRemindWnd);
	WritePrivateProfileString("WndConfig", "RemindWnd", str, strIniFile);
	str.Format("%d", g_bShowDraft);
	WritePrivateProfileString("WndConfig", "ShowDraft", str, strIniFile);
}

int CXJBrowserApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
// 	CString str= "";
// 	m_DBEngine.XJCloseSession(str);
	CXJRootStore::Release();
	return CWinApp::ExitInstance();
}

/*************************************************************
 函 数 名: ShowStationInfo()
 功能概要: 打开指定厂站的厂站信息视图
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 
**************************************************************/
void CXJBrowserApp::ShowStationInfo( CStationObj* pStation )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowStationInfo(pStation);
}

/*************************************************************
 函 数 名: ShowDownOsc()
 功能概要: 打开录波下载窗口，并下载指定录波
 返 回 值: 
 参    数: param1 指定下载的录波
		   Param2 
**************************************************************/
void CXJBrowserApp::ShowDownOsc( CXJEventOSC* pOsc )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowDownLoadOsc(pOsc);
}

/*************************************************************
 函 数 名: AddSTTPData()
 功能概要: 将STTP加入到队列
 返 回 值: 
 参    数: param1 sttp报文
		   Param2 
**************************************************************/
void CXJBrowserApp::AddSTTPData( STTP_FULL_DATA* sttpData )
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  

		CString strLog;
		strLog.Format("开始加入报文到缓存，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		m_listFullData.AddTail(sttpData);

		strLog.Format("加入报文到缓存成功，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("加入报文到缓存失败，当前缓存大小:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}	
}


/****************************************************
Date:2012/3/5  Author:LYH
函数名:   GetFirstSTTPData	
返回值:   STTP_FULL_DATA*	
功能概要: 
*****************************************************/
STTP_FULL_DATA* CXJBrowserApp::GetFirstSTTPData()
{
	STTP_FULL_DATA* pSttp = NULL;
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		CString strLog;
		strLog.Format("开始从缓存取报文，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		POSITION pos = m_listFullData.GetHeadPosition();
		if(pos != NULL)
			pSttp = (STTP_FULL_DATA*)m_listFullData.GetAt(pos);
		
		strLog.Format("从缓存取报文完毕，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("从缓存取报文失败，当前缓存大小:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}
	return pSttp;
}


/*************************************************************
 函 数 名: RemoveSTTPData()
 功能概要: 将STTP队列的第一个数据删除
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::RemoveSTTPData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  

		CString strLog;
		strLog.Format("开始从缓存移除报文，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		POSITION pos = m_listFullData.GetHeadPosition();
		STTP_FULL_DATA* pSttp = NULL;
 		if(pos != NULL)
 		{
			pSttp = (STTP_FULL_DATA*)m_listFullData.GetHead();
			m_listFullData.RemoveHead();
		}
		strLog.Format("从缓存移除报文成功，当前缓存大小:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);

		if(pSttp != NULL)
		{
			zero_sttp_full_data(*pSttp);
			delete pSttp;
		}
		strLog.Format("从缓存移除报文成功，删除数据成功");
		//WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("从缓存移除报文失败，当前缓存大小:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}	
}

/*************************************************************
 CreateDate: 29:1:2010     Author:LYH
 函 数 名: LoadSplitterWndConfig()
 功能概要: 读入分隔窗口大小配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::LoadSplitterWndConfig()
{
	CString strIniFile = g_SysConfig;
	//厂站通断窗口分隔大小
	g_nCommSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "CommSplitterWnd", 350, strIniFile);
	//动作窗口分隔大小
	g_nActionSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "ActionSplitterWnd", 350, strIniFile);
	//告警窗口分隔大小
	g_nAlarmSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "AlarmSplitterWnd", 350, strIniFile);
	//故障归档窗口分隔大小
	g_nFaultSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "FaultSplitterWnd", 200, strIniFile);
	//系统告警窗口分隔大小
	g_nSysAlarmSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "SysAlarmSplitterWnd", 350, strIniFile);
	//SOE信息窗口分隔大小
	g_nSOESplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "SOESplitterWnd", 350, strIniFile);
	//预警窗口分隔大小
	g_nCrossSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "CrossSplitterWnd", 350, strIniFile);

	g_nDraftWndWidth = GetPrivateProfileInt("SplitterWndConfig", "DraftWndWidth", 1024, strIniFile);
	g_nDraftWndHeight = GetPrivateProfileInt("SplitterWndConfig", "DraftWndHeight", 600, strIniFile);
}

/*************************************************************
 CreateDate: 29:1:2010     Author:LYH
 函 数 名: SaveSplitterWndConfig()
 功能概要: 保存分隔窗口大小配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::SaveSplitterWndConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//定值历史值载入项数. 0-10整数
	str.Format("%d", g_PTSettingValues);
	WritePrivateProfileString("PTPropConfig", "SettingValues", str, strIniFile);
	//厂站通断窗口分隔大小
	str.Format("%d", g_nCommSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "CommSplitterWnd", str, strIniFile);
	//动作窗口分隔大小
	str.Format("%d", g_nActionSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "ActionSplitterWnd", str, strIniFile);
	//告警窗口分隔大小
	str.Format("%d", g_nAlarmSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "AlarmSplitterWnd", str, strIniFile);
	//故障归档窗口分隔大小
	str.Format("%d", g_nFaultSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "FaultSplitterWnd", str, strIniFile);
	//系统告警窗口分隔大小
	str.Format("%d", g_nSysAlarmSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "SysAlarmSplitterWnd", str, strIniFile);
	//SOE信息窗口分隔大小
	str.Format("%d", g_nSOESplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "SOESplitterWnd", str, strIniFile);
	//预警窗口分隔大小
	str.Format("%d", g_nCrossSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "CrossSplitterWnd", str, strIniFile);

	str.Format("%d", g_nDraftWndWidth);
	WritePrivateProfileString("SplitterWndConfig", "DraftWndWidth", str, strIniFile);
	str.Format("%d", g_nDraftWndHeight);
	WritePrivateProfileString("SplitterWndConfig", "DraftWndHeight", str, strIniFile);
}


/*************************************************************
 CreateDate: 18:3:2010     Author:YYH
 函 数 名: QueryPtRunStatus()
 功能概要: 查询二次设备运行状态
 返 回 值: void
 参    数: param1 二次设备ID
		   Param2 运行状态
		   Param3 设备运行时间
**************************************************************/
BOOL CXJBrowserApp::QueryPtRunStatus(CString strPtID, int& nRunStatus, CString& strRunTime)
{
	BOOL nReturn = TRUE;
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	
	//字段
	//status
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT);
	//curtime
	Field Field2;
	m_DBEngine.SetField(sql, Field2, "curtime", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//条件
	//USER_ID
	Condition condition1;
	str.Format("pt_id = '%s'", strPtID);
	m_DBEngine.SetCondition(sql, condition1, str);

	Condition condition2;
	str = "order by curtime desc";
	m_DBEngine.SetCondition(sql, condition2, str, EX_STTP_WHERE_ABNORMALITY);
		
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
		if (nResult)
		{
			nReturn = TRUE;
		}
		else
		{
			nReturn = FALSE;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::QueryPtRunStatus, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		nReturn = FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
            nRunStatus = atoi(pMemset->GetValue((UINT)0));
			strRunTime = pMemset->GetValue((UINT)1);
			CString str;
			str.Format("CDataEngine::QueryPtRunStatus, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
			nReturn = TRUE;
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::BuildNetList,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		nReturn = FALSE;	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return nReturn;
}

/*************************************************************
 CreateDate: 18:3:2010     Author:YYH
 函 数 名: UpdatePtRunStatus()
 功能概要: 查询二次设备运行状态
 返 回 值: void
 参    数: param1 二次设备ID
           param2 状态值
**************************************************************/
void CXJBrowserApp::UpdatePtRunStatus(CString strPtID, int nRunStatus)
{
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	
	CString str;	
	//字段
	//status
	Field Field1;
	str.Format("%d", nRunStatus);
	m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT, str);
	
	//条件
	//USER_ID
	Condition condition1;
	str.Format("pt_id = '%s'", strPtID);
	m_DBEngine.SetCondition(sql, condition1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::UpdatePtRunStatus, 更新失败", XJ_LOG_LV3);
		delete[] sError;
	}
	if(nResult == 1)
	{
		str.Format("CXJBrowserApp::UpdatePtRunStatus,更新成功");
	}
	else
	{
		str.Format("CXJBrowserApp::UpdatePtRunStatus,更新失败,原因为%s", sError);
	}
	
	WriteLog(str, XJ_LOG_LV3);
	
	delete[] sError;
	sError = NULL;
}


void CXJBrowserApp::XJSetMenuInfo()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if (pWnd != NULL && pWnd->GetMenu() != NULL && g_style)
	{
		::SetMenuInfo(AfxGetMainWnd()->GetMenu()->m_hMenu, &lpcmi);
	}
}

/*
void CXJBrowserApp::OnAppCall() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolAutocall();
	pFrame->OnToolAutocallResult();
}
*/

void CXJBrowserApp::OnCallAll() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolAutocall();
}

void CXJBrowserApp::OnFileDownload() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolDownloadFile();
}

/************************************
Date:2011/6/7  Author:LYH
函数名:    ShowStationInComm
返回值:   void
功能概要: 在厂站状态图形中显示厂站图形
参数: CStationObj * pObj 指定厂站
参数: BOOL bMoveFirst 是否前置
************************************/
void CXJBrowserApp::ShowStationInComm( CStationObj* pObj, BOOL bMoveFirst /*= TRUE*/ )
{
	WriteLog(" CXJBrowserApp::ShowStationInComm, 开始", XJ_LOG_LV3);
	if(pObj == NULL)
		return;
	
	//得到画面指针
	CViewCommMap * pView = GetCommView();
	if(pView == NULL)
		return;
	
	//前置
	if(bMoveFirst)
		pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
	
	//居中
	pView->CenterDevice(pObj->m_sID);
}

void CXJBrowserApp::RemoveAllSTTPData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		POSITION pos = m_listFullData.GetHeadPosition();
		while(pos != NULL)
		{
			STTP_FULL_DATA* sttpData = (STTP_FULL_DATA*)m_listFullData.GetNext(pos);
			if(sttpData != NULL)
				delete sttpData;	
		}
		m_listFullData.RemoveAll();
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

CGeoView* CXJBrowserApp::GetGeoView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	
	CGeoView * pGeoView = NULL;
	
	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CGeoView)))//如果是通讯状态图
		{
			pGeoView = (CGeoView *)pView;
			break;
		}			
	}
	
	return pGeoView;
}

/****************************************************
Date:2011/12/5  Author:LYH
函数名:   LockUI	
返回值:   void	
功能概要: 
*****************************************************/
void CXJBrowserApp::LockUI()
{
	if(!m_bCanLock)
		return;
	if(m_bIsLocked)
		return;
	m_bIsLocked = TRUE;
	CString str;
	str.Format("系统空闲%d分钟,锁定界面", g_UILockTime);
	WriteLog(str, XJ_LOG_LV1);
	if(!DoLogin())
		m_pMainWnd->PostMessage(WM_CLOSE);
}

/****************************************************
Date:2012/3/13  Author:LYH
函数名:   JudgeOperTableType	
返回值:   void	
功能概要: 判断日志表类型
*****************************************************/
void CXJBrowserApp::JudgeOperTableType()
{
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field2;
	m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_OPERATION_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::JudgeOperTableType, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		int nFieldCount = pMemset->GetMemFieldNum();
		if(nFieldCount > 7)
			g_OperTableType = 1;
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::JudgeOperTableType, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	CString strLog;
	strLog.Format("使用%s日志表", g_OperTableType==0?"旧":"新");
	WriteLog(strLog, XJ_LOG_LV2);
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   SetAppTile	
返回值:   void	
功能概要: 
参数: CString sTitle	
*****************************************************/
void CXJBrowserApp::SetAppTile( CString sTitle, CString sDetail /*= ""*/ )
{
	//得到主框架指针
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	CString str;
	if(sDetail != "")
		str.Format("%s - [%s] - [%s]", g_sAppName, sTitle, sDetail);
	else
		str.Format("%s - [%s]", g_sAppName, sTitle);
	pFrame->SetTitle(str);
	pFrame->UpdateFrameTitleForDocument("");
}

/****************************************************
Date:2014/5/22  Author:LYH
函数名:   	
返回值:   void	
功能概要: 
*****************************************************/
void CXJBrowserApp::StartClearConfigThread()
{
	if (m_pClearConfigThread == NULL)
	{
		//启动清理二次设备配置线程
		m_pClearConfigThread = AfxBeginThread(ClearSecConfig,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pClearConfigThread != NULL)
		{
			m_pClearConfigThread->m_bAutoDelete = FALSE;
			m_pClearConfigThread->ResumeThread();
		}
	}
}

void CXJBrowserApp::QueryUserStation()
{
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//group_id
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "station_id", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//条件
	//USER_ID
	Condition condition2;
	str.Format("user_id = '%s'", m_User.m_strUSER_ID);
	m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(157/*tb_user_related*/, sql, sError, &pMemset);
	if(nResult == 1)
	{
		int nCount = pMemset.GetMemRowNum();
		if(nCount > 0)
		{
			m_User.m_bAllStation = FALSE;
			CString strLog;
			strLog.Format("用户[%s]一共管理%d个子站", m_User.m_strUSER_ID, nCount);
			WriteLog(strLog, XJ_LOG_LV2);
		}
	}
}

BOOL CXJBrowserApp::MgrStation(CString stationID)
{
	if(MgrAllStations())
		return TRUE;

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//条件
	//USER_ID
	Condition condition2;
	str.Format("user_id = '%s'", m_User.m_strUSER_ID);
	m_DBEngine.SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("station_id = '%s'", stationID);
	m_DBEngine.SetCondition(sql, condition3, str);
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(157/*tb_user_related*/, sql, sError, &pMemset);
	if(nResult == 1)
	{
		int nCount = pMemset.GetMemRowNum();
		if(nCount > 0)
		{
			CString strLog;
			strLog.Format("子站[%s]在用户[%s]的管理范围内", stationID, m_User.m_strUSER_ID);
			WriteLog(strLog, XJ_LOG_LV2);
			return TRUE;
		}
	}
	return FALSE;
}

CString CXJBrowserApp::GetUserGroupNameByID(CString sUserGroupID)
{
	CString sReturn;

	if (sUserGroupID.IsEmpty())
		return sReturn;

	CString str;
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//NAME
	Field fld0;
	m_DBEngine.SetField(sql, fld0, "NAME", EX_STTP_DATA_TYPE_STRING);
	
	//条件
	//USER_ID
	Condition cond0;
	str.Format("GROUP_ID = '%s'", sUserGroupID);
	m_DBEngine.SetCondition(sql, cond0, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_GROUP_CFG, sql, sError, pMemset);
		
		if (1 != nResult){
			sReturn = "";	
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::GetUserGroupNameByID, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		sReturn = "";
		
		return sReturn;
	}
	
	// 查询成功
	if(NULL != pMemset)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJBrowserApp::GetUserGroupNameByID, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
			CString strVal = pMemset->GetValue((UINT)0);
			if (!strVal.IsEmpty()){
				sReturn = strVal;
			}
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::GetUserGroupNameByID, 查询失败, 原因为：%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		sReturn = "";	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
	return sReturn;
}

int CXJBrowserApp::CheckKeyNameForPTSet()
{
	int nReturn = -1;

    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//value
	Field fld0;
	pApp->m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	
	//条件
	//keyname
	Condition condition1;
	str.Format("keyname = 'DZ_MOD_STATE'");
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult)
		{
			nReturn = -1;
			//AfxMessageBox("CXJBrowserApp::CheckKeyNameForPTSet, 查询失败");
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::CheckKeyNameForPTSet, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		sError = NULL;

		nReturn = -1;
	}

	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CXJBrowserApp::CheckKeyNameForPTSet, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
			return 0;

		// 不存在记录 DZ_MOD_STATE
		//组建查询条件
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//设置字段
		//挂牌状态关键字
		str = "DZ_MOD_STATE";
		Field fld1;
		pApp->m_DBEngine.SetField(sql1, fld1, "keyname", EX_STTP_DATA_TYPE_STRING, str);
		Field fld2;
		pApp->m_DBEngine.SetField(sql1, fld2, "value", EX_STTP_DATA_TYPE_STRING, "0");
		
		memset(sError, '\0', MAXMSGLEN);
		
		//int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_TBSYSCONFIG, sql1, sError);

			if(nResult == 1)
			{
				//成功
				nReturn = 0;
			}
		}
		catch (...)
		{
			WriteLog("CXJBrowserApp::CheckKeyNameForPTSet.XJInsertData, 保存失败");
			delete[] sError;
			sError = NULL;
			
			nReturn = -1;
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::CheckKeyNameForPTSet,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);

		//AfxMessageBox(str);
		nReturn = -1;	
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}
