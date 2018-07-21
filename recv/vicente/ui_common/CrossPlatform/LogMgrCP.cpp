// LogMgrCP.cpp: implementation of the LogMgrCP class.
//
//////////////////////////////////////////////////////////////////////
#include "LogMgrCP.h"

/****************************************************
Date:2012/7/3  Author:LYH
������:   WriteLog	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sLog	
����: int nLogLevel	
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
	printf("������־�������ɹ�\n");
}

LogMgrCP::~LogMgrCP()
{

}

/****************************************************
Date:2012/7/3  Author:LYH
������:   getInstance	
����ֵ:   LogMgrCP*	LogMgrCP����ָ��
���ܸ�Ҫ: ȡ��LogMgrCP����ָ��
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
������:   SetLogPath	
����ֵ:   void	
���ܸ�Ҫ: ������־�ļ�·��
����: CXJString sPath	·��
����: CXJString sFolder	��־�ļ�������
*****************************************************/
void LogMgrCP::SetLogPath( CXJString sPath, CXJString sFolder /*= "Temp_Log"*/ )
{
	m_File.SetLogPath(sPath);
	m_File.Open(sFolder);
	m_File.SetLogLevel(CLogFile::trace);
}

/****************************************************
Date:2012/7/3  Author:LYH
������:   WriteLog	
����ֵ:   void	
���ܸ�Ҫ: д��־����,���ⲿ����
����: CXJString sLog	��־�ַ���
����: int nLogLevel	��־�ȼ�
*****************************************************/
void LogMgrCP::WriteLog( CXJString sLog, int nLogLevel )
{
	if(sLog.IsEmpty())
		return;
	m_File.Add(sLog, nLogLevel);
}

/****************************************************
Date:2012/7/3  Author:LYH
������:   SetLogLevel	
����ֵ:   void	
���ܸ�Ҫ: ������־�ļ��ļ�¼�ȼ�
����: int nLogLevel	��־�ȼ�
*****************************************************/
void LogMgrCP::SetLogLevel( int nLogLevel )
{
	m_File.SetLogLevel(nLogLevel);
}
