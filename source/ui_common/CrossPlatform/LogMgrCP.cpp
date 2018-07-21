// LogMgrCP.cpp: implementation of the LogMgrCP class.
//
//////////////////////////////////////////////////////////////////////
#include "LogMgrCP.h"

/****************************************************
Date:2012/7/3  Author:LYH
函数名:   WriteLog	
返回值:   void	
功能概要: 
参数: CString sLog	
参数: int nLogLevel	
*****************************************************/
void WriteLog_CP( CXJString sLog, int nLogLevel /*= 3*/ )
{
	LogMgrCP* pMgr = LogMgrCP::getInstance();
	if(pMgr != NULL)
		pMgr->WriteLog(sLog, nLogLevel);
}

LogMgrCP* LogMgrCP::m_instance = NULL;
CXJLock* LogMgrCP::m_InstanceLock = new CXJLock("LogMgrCPInstanceLock");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogMgrCP::LogMgrCP()
{
	SetLogPath();
	printf("创建日志管理对象成功\n");
}

LogMgrCP::~LogMgrCP()
{

}

/****************************************************
Date:2012/7/3  Author:LYH
函数名:   getInstance	
返回值:   LogMgrCP*	LogMgrCP对象指针
功能概要: 取得LogMgrCP对象指针
*****************************************************/
LogMgrCP* LogMgrCP::getInstance()
{
	if(m_InstanceLock != NULL)
		m_InstanceLock->Lock();
	else
		return NULL;
	if(m_instance == NULL)
	{
		m_instance = new LogMgrCP();
	}
	if(m_InstanceLock != NULL)
		m_InstanceLock->Unlock();
	else
		return NULL;
	return m_instance;
}

/****************************************************
Date:2012/7/3  Author:LYH
函数名:   SetLogPath	
返回值:   void	
功能概要: 设置日志文件路径
参数: CXJString sPath	路径
参数: CXJString sFolder	日志文件夹名称
*****************************************************/
void LogMgrCP::SetLogPath( CXJString sPath, CXJString sFolder /*= "Temp_Log"*/ )
{
	m_File.SetLogPath(sPath);
	m_File.Open(sFolder);
	m_File.SetLogLevel(CLogFile::trace);
}

/****************************************************
Date:2012/7/3  Author:LYH
函数名:   WriteLog	
返回值:   void	
功能概要: 写日志操作,供外部调用
参数: CXJString sLog	日志字符串
参数: int nLogLevel	日志等级
*****************************************************/
void LogMgrCP::WriteLog( CXJString sLog, int nLogLevel )
{
	if(sLog.IsEmpty())
		return;
	m_File.Add(sLog, nLogLevel);
}

/****************************************************
Date:2012/7/3  Author:LYH
函数名:   SetLogLevel	
返回值:   void	
功能概要: 设置日志文件的记录等级
参数: int nLogLevel	日志等级
*****************************************************/
void LogMgrCP::SetLogLevel( int nLogLevel )
{
	m_File.SetLogLevel(nLogLevel);
}
