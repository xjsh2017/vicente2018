// ***************************************************************
//  LogMgr   version:  1.0    date: 3:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 日志管理类实现
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
// LogMgr.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "LogMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogMgr

IMPLEMENT_DYNCREATE(CLogMgr, CObject)

//##ModelId=49B87B990205
CLogMgr::CLogMgr()
{
	m_pLogView	= NULL;
	m_bLogOpened = FALSE;
}

//##ModelId=49B87B990233
CLogMgr::~CLogMgr()
{
//	if(m_bLogOpened)
//		CloseLogFile();
	m_File.Close();
}

/////////////////////////////////////////////////////////////////////////////
// CLogMgr message handlers
/*************************************************************
 函 数 名：SetLogPath()
 功能概要：设置日志文件路径
 返 回 值: void
 参    数：param1	CString & path   文件路径(全路径)
           
**************************************************************/
//##ModelId=49B87B990214
void CLogMgr::SetLogPath(CString& path)
{
	//关闭之前的日志文件
	m_File.SetLogPath(path);
	m_File.Open("XJBrowser");
	m_File.SetLogLevel(g_LogLevel);
}

/*************************************************************
 函 数 名：SetLogView()
 功能概要：设置日志输出窗口指针
 返 回 值: void
 参    数：param1   CLogView *	日志输出窗口指针
**************************************************************/
//##ModelId=49B87B990216
void CLogMgr::SetLogView(CLogView* pView)
{
	//判断传入的指针不为空
	if(pView == NULL)
	{
		TRACE("CLogMgr::SetLogView, 传入的指针为空, 设置日志输出窗口失败\n");
		return;
	}
	m_pLogView = pView;
}

/*************************************************************
 函 数 名：WriteLog()
 功能概要：写日志入口, 供外部调用.判断是否要输出, 组织输出日志,调用输出函数
 返 回 值: void
 参    数：param1   CString&	日志字符串
           Param2   const int&	日志等级
**************************************************************/
//##ModelId=49B87B990218
void CLogMgr::WriteLog(CString& sLog, const int& nLV)
{
	//判断日志字符串是否为空
	if(sLog.IsEmpty())
	{
		TRACE("CLogMgr::WriteLog, 传入的字符串为空, 不写日志\n");
		return;
	}
	//判断日志等级是否正确
	if((nLV != XJ_LOG_LV1) && (nLV != XJ_LOG_LV2) && (nLV != XJ_LOG_LV3))
	{
		TRACE("CLogMgr::WriteLog, 日志等级不符标准, 不写日志\n");
		return;
	}

	//判断日志是否需输出
	if(nLV > (XJ_LOG_LV1 + (g_LogLevel - 1)))
	{
		//大于设定的输出等级
		return;
	}

	//组织输出的字符串
	CString sOutPut = MarkLogString(sLog);

	TRACE(sOutPut);

	//输出到日志输出窗口
	if(g_OutPutToWnd > 0 && !g_Exiting)
	{
		if(g_LogLevel == 3)
		{
			if(m_pLogView != NULL && nLV <= XJ_LOG_LV2)
			{
				m_pLogView ->AppendString(sOutPut);
			}
		}
		else
		{
			if(m_pLogView != NULL && nLV <= g_LogLevel)
			{
				m_pLogView ->AppendString(sOutPut);
			}
		}
	}

	//输出到日志文件
	if(g_PutOutToLogFile > 0)
		OutputToFile(sLog);
}

/*************************************************************
 函 数 名：MarkLogString()
 功能概要：格式化要输出的字符串
 返 回 值: 返回值说明
 参    数：param1   CString&	要输出的字符串
**************************************************************/
//##ModelId=49B87B990226
CString CLogMgr::MarkLogString(CString& sLog)
{
	CString sReturn = "";
	//取得当前时间
	CTime tm=CTime::GetCurrentTime();
	//格式化时间
	CString sTime;
	sTime = tm.Format("  [%Y年%m月%d日 %H时%M分%S秒] ");
 	sReturn = sTime+ sLog+ "\r\n";
	return sReturn;
}

//##ModelId=49B87B990228
void CLogMgr::OutputToFile(CString& sLog, int nLv/* = 2*/)
{
	m_File.Add(sLog, nLv);
}

/*************************************************************
 函 数 名：WriteLogEx()
 功能概要：写日志到日志文件
 返 回 值: 
 参    数：param1	日志内容
		   Param2	日志等级
**************************************************************/
//##ModelId=49B87B990223
void CLogMgr::WriteLogEx( CString sLog, int nLV )
{
	WriteLog(sLog, nLV);
}
