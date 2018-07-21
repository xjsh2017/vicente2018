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
 �� �� ����SetLogPath()
 ���ܸ�Ҫ��������־�ļ�·��
 �� �� ֵ: void
 ��    ����param1	CString & path   �ļ�·��(ȫ·��)
           
**************************************************************/
//##ModelId=49B87B990214
void CLogMgr::SetLogPath(CString& path)
{
	//�ر�֮ǰ����־�ļ�
	m_File.SetLogPath(path);
	m_File.Open("XJBrowser");
	m_File.SetLogLevel(g_LogLevel);
}

/*************************************************************
 �� �� ����SetLogView()
 ���ܸ�Ҫ��������־�������ָ��
 �� �� ֵ: void
 ��    ����param1   CLogView *	��־�������ָ��
**************************************************************/
//##ModelId=49B87B990216
void CLogMgr::SetLogView(CLogView* pView)
{
	//�жϴ����ָ�벻Ϊ��
	if(pView == NULL)
	{
		TRACE("CLogMgr::SetLogView, �����ָ��Ϊ��, ������־�������ʧ��\n");
		return;
	}
	m_pLogView = pView;
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
	if(nLV > (XJ_LOG_LV1 + (g_LogLevel - 1)))
	{
		//�����趨������ȼ�
		return;
	}

	//��֯������ַ���
	CString sOutPut = MarkLogString(sLog);

	TRACE(sOutPut);

	//�������־�������
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

	//�������־�ļ�
	if(g_PutOutToLogFile > 0)
		OutputToFile(sLog);
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

/*************************************************************
 �� �� ����WriteLogEx()
 ���ܸ�Ҫ��д��־����־�ļ�
 �� �� ֵ: 
 ��    ����param1	��־����
		   Param2	��־�ȼ�
**************************************************************/
//##ModelId=49B87B990223
void CLogMgr::WriteLogEx( CString sLog, int nLV )
{
	WriteLog(sLog, nLV);
}
