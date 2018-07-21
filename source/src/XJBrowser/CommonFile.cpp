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
	/** @brief           文件序号*/
	m_nID = 0;
	/** @brief           厂站ID*/
	m_sStationID = "";
	/** @brief           对应的厂站指针*/
	m_pStation = NULL;
	/** @brief           文件路径*/
	m_sFilePath = "";
	/** @brief           文件名*/
	m_sFileName = "";
	/** @brief           文件时间*/
	m_sFileTime = "";
	/** @brief           文件大小*/
	m_nFileSize = 0;
	/** @brief           是否已下载, 0-未下载,1-已下载*/
	m_nIsDownload = 0;
	m_nOffset = 0;
}

//##ModelId=49B87BAA0006
CCommonFile::~CCommonFile()
{
}

/*************************************************************
 函 数 名：LoadFromSTTP00204()
 功能概要：从00204报文中得到文件信息
 返 回 值: 读取信息成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
		   Param2	循环体索引
**************************************************************/
//##ModelId=49B87BA903DC
BOOL CCommonFile::LoadFromSTTP00204( STTP_FULL_DATA* pSttpData, int nIndex )
{
	//检查数据有效性
	if(pSttpData == NULL)
		return FALSE;
	//本帧包含的文件个数
	int nFileNum = pSttpData->sttp_body.variant_member.file_list.size();
	if(nIndex < 0 || nIndex >= nFileNum)
		return FALSE;
	//解析报文
	try
	{
		m_sStationID = pSttpData->sttp_body.ch_pt_id; //厂站ID
		m_sFilePath = XJToMS(pSttpData->sttp_body.variant_member.file_list[nIndex].strFielName); //文件路径
		CString str;
		m_sFileTime = pSttpData->sttp_body.variant_member.file_list[nIndex].ch_time;//文件时间
		m_nFileSize = pSttpData->sttp_body.variant_member.file_list[nIndex].nFileSize;//文件大小

		//查找对应厂站
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
		if(m_pStation == NULL)
			return FALSE;
		//得到文件名
		m_sFileName = GetFileNameByPath(m_sFilePath);
		//判断本文件是否下载过
		m_nIsDownload = IsCommFileDownloaded(m_sStationID, m_sFileName);
	}
	catch (...)
	{
		WriteLog("CCommonFile::LoadFromSTTP00204 解析报文出错", 3);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：GetFileNameByPath()
 功能概要：由文件路径得到文件名(带后缀) 
 返 回 值: 文件名
 参    数：param1	文件路径
		   Param2
**************************************************************/
//##ModelId=49B87BA903DF
CString CCommonFile::GetFileNameByPath( CString strPath )
{
	if(strPath == "")
		return "";
	//先把/都转成'\'
	strPath.Replace("/", "\\");
	//判断是否有"\\"
	int nFind = strPath.ReverseFind('\\');
	if(nFind != -1)
	{
		//找到
		CString sTemp = strPath;
		sTemp.Delete(0, nFind+1);
		return sTemp;
	}
	return strPath;
}

/*************************************************************
 函 数 名：IsCommFileDownloaded()
 功能概要：判断指定通用文件是否已下载
 返 回 值: 0-未下载, 1-已下载, 2-部分下载
 参    数：param1	指定厂站
		   Param2	指定文件名
**************************************************************/
//##ModelId=49B87BAA0001
int CCommonFile::IsCommFileDownloaded( CString sStationID, CString sFileName )
{
	//判断数据有效性
	if(sStationID == "")
		return 0;
	if(sFileName == "")
		return 0;
	//组织本地路径
	CString strDir = GetFilePath();
	//查找是否有此文件
	CFileStatus   status;
	CString strLog;   
	if(CFile::GetStatus(strDir ,status) != TRUE)
	{
		//不存在,判断是否存在零时文件
		CString str = strDir;
		str += ".tmp";
		if(CFile::GetStatus(str, status) == TRUE)
		{
			//找到此文件
			m_nOffset = status.m_size;
			strLog.Format("通用文件,部分下载,%s", strDir);
			WriteLog(strLog, XJ_LOG_LV3);
			return 2;
		}
	}
	else
	{
		strLog.Format("通用文件,已下载,%s", strDir);
		WriteLog(strLog, XJ_LOG_LV3);
		return 1;
	}
	strLog.Format("通用文件,未下载,%s", strDir);
	WriteLog(strLog, XJ_LOG_LV3);
	return 0;
}

/*************************************************************
 函 数 名：GetDownloadString()
 功能概要：得到是否下载描述
 返 回 值: 是否下载描述
 参    数：param1
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
 函 数 名：GetFileRelatePath()
 功能概要：得到文件相对路径名
 返 回 值: 相对路径
 参    数：param1
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
函数名:   GetFilePath	
返回值:   CString	
功能概要: 取得文件全路径
*****************************************************/
CString CCommonFile::GetFilePath()
{
	//组织本地路径
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDir = pApp->GetDownDir();
	if(strDir == "")
		return "";
	strDir += GetFileRelatePath();
	return strDir;
}
/////////////////////////////////////////////////////////////////////////////
// CCommonFile message handlers
