// CommonFile.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CommonFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonFile

//##ModelId=49B87BA903C8
CCommonFile::CCommonFile()
{
	/** @brief           �ļ����*/
	m_nID = 0;
	/** @brief           ��վID*/
	m_sStationID = "";
	/** @brief           ��Ӧ�ĳ�վָ��*/
	m_pStation = NULL;
	/** @brief           �ļ�·��*/
	m_sFilePath = "";
	/** @brief           �ļ���*/
	m_sFileName = "";
	/** @brief           �ļ�ʱ��*/
	m_sFileTime = "";
	/** @brief           �ļ���С*/
	m_nFileSize = 0;
	/** @brief           �Ƿ�������, 0-δ����,1-������*/
	m_nIsDownload = 0;
	m_nOffset = 0;
}

//##ModelId=49B87BAA0006
CCommonFile::~CCommonFile()
{
}

/*************************************************************
 �� �� ����LoadFromSTTP00204()
 ���ܸ�Ҫ����00204�����еõ��ļ���Ϣ
 �� �� ֵ: ��ȡ��Ϣ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
		   Param2	ѭ��������
**************************************************************/
//##ModelId=49B87BA903DC
BOOL CCommonFile::LoadFromSTTP00204( STTP_FULL_DATA* pSttpData, int nIndex )
{
	//���������Ч��
	if(pSttpData == NULL)
		return FALSE;
	//��֡�������ļ�����
	int nFileNum = pSttpData->sttp_body.variant_member.file_list.size();
	if(nIndex < 0 || nIndex >= nFileNum)
		return FALSE;
	//��������
	try
	{
		m_sStationID = pSttpData->sttp_body.ch_pt_id; //��վID
		m_sFilePath = XJToMS(pSttpData->sttp_body.variant_member.file_list[nIndex].strFielName); //�ļ�·��
		CString str;
		m_sFileTime = pSttpData->sttp_body.variant_member.file_list[nIndex].ch_time;//�ļ�ʱ��
		m_nFileSize = pSttpData->sttp_body.variant_member.file_list[nIndex].nFileSize;//�ļ���С

		//���Ҷ�Ӧ��վ
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
		if(m_pStation == NULL)
			return FALSE;
		//�õ��ļ���
		m_sFileName = GetFileNameByPath(m_sFilePath);
		//�жϱ��ļ��Ƿ����ع�
		m_nIsDownload = IsCommFileDownloaded(m_sStationID, m_sFileName);
	}
	catch (...)
	{
		WriteLog("CCommonFile::LoadFromSTTP00204 �������ĳ���", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����GetFileNameByPath()
 ���ܸ�Ҫ�����ļ�·���õ��ļ���(����׺) 
 �� �� ֵ: �ļ���
 ��    ����param1	�ļ�·��
		   Param2
**************************************************************/
//##ModelId=49B87BA903DF
CString CCommonFile::GetFileNameByPath( CString strPath )
{
	if(strPath == "")
		return "";
	//�Ȱ�/��ת��'\'
	strPath.Replace("/", "\\");
	//�ж��Ƿ���"\\"
	int nFind = strPath.ReverseFind('\\');
	if(nFind != -1)
	{
		//�ҵ�
		CString sTemp = strPath;
		sTemp.Delete(0, nFind+1);
		return sTemp;
	}
	return strPath;
}

/*************************************************************
 �� �� ����IsCommFileDownloaded()
 ���ܸ�Ҫ���ж�ָ��ͨ���ļ��Ƿ�������
 �� �� ֵ: 0-δ����, 1-������, 2-��������
 ��    ����param1	ָ����վ
		   Param2	ָ���ļ���
**************************************************************/
//##ModelId=49B87BAA0001
int CCommonFile::IsCommFileDownloaded( CString sStationID, CString sFileName )
{
	//�ж�������Ч��
	if(sStationID == "")
		return 0;
	if(sFileName == "")
		return 0;
	//��֯����·��
	CString strDir = GetFilePath();
	//�����Ƿ��д��ļ�
	CFileStatus   status;
	CString strLog;   
	if(CFile::GetStatus(strDir ,status) != TRUE)
	{
		//������,�ж��Ƿ������ʱ�ļ�
		CString str = strDir;
		str += ".tmp";
		if(CFile::GetStatus(str, status) == TRUE)
		{
			//�ҵ����ļ�
			m_nOffset = status.m_size;
			strLog.Format("ͨ���ļ�,��������,%s", strDir);
			WriteLog(strLog, XJ_LOG_LV3);
			return 2;
		}
	}
	else
	{
		strLog.Format("ͨ���ļ�,������,%s", strDir);
		WriteLog(strLog, XJ_LOG_LV3);
		return 1;
	}
	strLog.Format("ͨ���ļ�,δ����,%s", strDir);
	WriteLog(strLog, XJ_LOG_LV3);
	return 0;
}

/*************************************************************
 �� �� ����GetDownloadString()
 ���ܸ�Ҫ���õ��Ƿ���������
 �� �� ֵ: �Ƿ���������
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BAA0004
CString CCommonFile::GetDownloadString()
{
	if(0 == m_nIsDownload)
		return StringFromID(IDS_DOWNLOAD_NOTSTART);
	if(1 == m_nIsDownload)
		return StringFromID(IDS_DOWNLOAD_COMPLETE);
	if(2 == m_nIsDownload)
		return StringFromID(IDS_DOWNLOAD_PARTIAL);
	return "";
}

/*************************************************************
 �� �� ����GetFileRelatePath()
 ���ܸ�Ҫ���õ��ļ����·����
 �� �� ֵ: ���·��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BAA0005
CString CCommonFile::GetFileRelatePath()
{
	CString str = "";
	str = m_sStationID + "\\" + StringFromID(IDS_FUNC_COMMONFILE) + "\\" + m_sFileName;
	return str;
}

/****************************************************
Date:2013/12/23  Author:LYH
������:   GetFilePath	
����ֵ:   CString	
���ܸ�Ҫ: ȡ���ļ�ȫ·��
*****************************************************/
CString CCommonFile::GetFilePath()
{
	//��֯����·��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDir = pApp->GetDownDir();
	if(strDir == "")
		return "";
	strDir += GetFileRelatePath();
	return strDir;
}
/////////////////////////////////////////////////////////////////////////////
// CCommonFile message handlers
