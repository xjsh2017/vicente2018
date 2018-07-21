// ***************************************************************
//  LogMgr   version:  1.0    date: 3:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ��־������ʵ��
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
������:   CLogMgr	
����ֵ:   	
���ܸ�Ҫ: 
����: CString sPath	
����: int nLogLevel	
����: BOOL bOutPutToFile	
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
 �� �� ����SetLogPath()
 ���ܸ�Ҫ��������־�ļ�·��
 �� �� ֵ: void
 ��    ����param1	CString & path   �ļ�·��(ȫ·��)
           
**************************************************************/
//##ModelId=49B87B990214
void CLogMgr::SetLogPath(CString& path, CString sFolder/* = "Temp_Log"*/)
{
	//�ر�֮ǰ����־�ļ�
	m_File.SetLogPath(path);
	m_File.Open(sFolder);
	m_File.SetLogLevel(m_nLogLevel);
}


/*************************************************************
 �� �� ����WriteLog()
 ���ܸ�Ҫ��д��־���, ���ⲿ����.�ж��Ƿ�Ҫ���, ��֯�����־,�����������
 �� �� ֵ: void
 ��    ����param1   CString&	��־�ַ���
           Param2   const int&	��־�ȼ�
**************************************************************/
//##ModelId=49B87B990218
void CLogMgr::WriteLog(CString& sLog, const int& nLV)
{
	//�ж���־�ַ����Ƿ�Ϊ��
	if(sLog.IsEmpty())
	{
		TRACE("CLogMgr::WriteLog, ������ַ���Ϊ��, ��д��־\n");
		return;
	}
	//�ж���־�ȼ��Ƿ���ȷ
	if((nLV != XJ_LOG_LV1) && (nLV != XJ_LOG_LV2) && (nLV != XJ_LOG_LV3))
	{
		TRACE("CLogMgr::WriteLog, ��־�ȼ�������׼, ��д��־\n");
		return;
	}

	//�ж���־�Ƿ������
	if(nLV > (XJ_LOG_LV1 + (m_nLogLevel - 1)))
	{
		//�����趨������ȼ�
		return;
	}

	//��֯������ַ���
	CString sOutPut = MarkLogString(sLog);

	TRACE(sOutPut);

	//�������־�ļ�
	if(m_bOutPutToFile > 0)
		OutputToFile(sLog, nLV);
}

/*************************************************************
 �� �� ����MarkLogString()
 ���ܸ�Ҫ����ʽ��Ҫ������ַ���
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   CString&	Ҫ������ַ���
**************************************************************/
//##ModelId=49B87B990226
CString CLogMgr::MarkLogString(CString& sLog)
{
	CString sReturn = "";
	//ȡ�õ�ǰʱ��
	CTime tm=CTime::GetCurrentTime();
	//��ʽ��ʱ��
	CString sTime;
	sTime = tm.Format("  [%Y��%m��%d�� %Hʱ%M��%S��] ");
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
������:   getInstance	
����ֵ:   CLogMgr*	
���ܸ�Ҫ: 
*****************************************************/
CLogMgr* CLogMgr::getInstance()
{
	return m_instance;
}

/****************************************************
Date:2011/10/19  Author:LYH
������:   CreateInstance	
����ֵ:   CLogMgr*	
���ܸ�Ҫ: 
����: CString sPath	
����: CString sFolder	
����: int nLogLevel	
����: BOOL bOutPutToFile	
*****************************************************/
CLogMgr* CLogMgr::CreateInstance( CString sPath, CString sFolder /*= "Temp_Log"*/, int nLogLevel /*= XJ_LOG_LV3*/, BOOL bOutPutToFile /*= TRUE*/ )
{
	m_instance = new CLogMgr(sPath, sFolder, nLogLevel, bOutPutToFile);
	return m_instance;
}

