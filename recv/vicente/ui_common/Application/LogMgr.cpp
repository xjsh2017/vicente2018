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

#include "LogMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CLogMgr* CLogMgr::m_instance(NULL);
CLogMgr* CLogMgr::m_instance = NULL;
/////////////////////////////////////////////////////////////////////////////
// CLogMgr
/****************************************************
Date:2011/10/17  Author:LYH
函数名:   CLogMgr	
返回值:   	
功能概要: 
参数: CString sPath	
参数: int nLogLevel	
参数: BOOL bOutPutToFile	
*****************************************************/
CLogMgr::CLogMgr( CString sPath, CString sFolder /*= "Temp_Log"*/, int nLogLevel /*= XJ_LOG_LV3*/, BOOL bOutPutToFile /*= TRUE*/ )
{
	SetLogPath(sPath, sFolder);
	SetLogLevel(nLogLevel);
	SetOutPutToFile(bOutPutToFile);
}

//##ModelId=49B87B990233
CLogMgr::~CLogMgr()
{
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
void CLogMgr::SetLogPath(CString& path, CString sFolder/* = "Temp_Log"*/)
{
	//关闭之前的日志文件
	m_File.SetLogPath(path);
	m_File.Open(sFolder);
	m_File.SetLogLevel(m_nLogLevel);
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
	if(nLV > (XJ_LOG_LV1 + (m_nLogLevel - 1)))
	{
		//大于设定的输出等级
		return;
	}

	//组织输出的字符串
	CString sOutPut = MarkLogString(sLog);

	TRACE(sOutPut);

	//输出到日志文件
	if(m_bOutPutToFile > 0)
		OutputToFile(sLog, nLV);
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

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   getInstance	
返回值:   CLogMgr*	
功能概要: 
*****************************************************/
CLogMgr* CLogMgr::getInstance()
{
	return m_instance;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   CreateInstance	
返回值:   CLogMgr*	
功能概要: 
参数: CString sPath	
参数: CString sFolder	
参数: int nLogLevel	
参数: BOOL bOutPutToFile	
*****************************************************/
CLogMgr* CLogMgr::CreateInstance( CString sPath, CString sFolder /*= "Temp_Log"*/, int nLogLevel /*= XJ_LOG_LV3*/, BOOL bOutPutToFile /*= TRUE*/ )
{
	m_instance = new CLogMgr(sPath, sFolder, nLogLevel, bOutPutToFile);
	return m_instance;
}

