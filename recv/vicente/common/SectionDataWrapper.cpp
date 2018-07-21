/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   16:57
	filename: 	\VICENTE\CODE\COMMON\SectionDataWrapper.cpp
	file path:	\VICENTE\CODE\COMMON
	file base:	SectionDataWrapper
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "SectionDataWrapper.h"

CSectionDataWrapper::CSectionDataWrapper()
:m_hLibXJShareSD(NULL),m_pFunCloseSectionData(NULL),
 m_pFunCreateSectionData(NULL),m_pFunOpenSectionData(NULL),
 m_pFunSetFullSectionData(NULL),m_pFunGetFullSectionData(NULL),
 m_pFunSetSectionData(NULL),m_pFunGetSectionData(NULL),
 m_pFunSetPointData(NULL),m_pFunGetPointData(NULL)
{
	
}

CSectionDataWrapper::~CSectionDataWrapper()
{
	xj_free_library(m_hLibXJShareSD);		
}

bool CSectionDataWrapper::InitLibrary( char* pErrorMsg )
{
	m_hLibXJShareSD = xj_load_library(LIB_SHARESECTIONDATA_NAME);
	if (m_hLibXJShareSD != NULL)
	{
		m_pFunCreateSectionData = (PFUNCREATESECTIONDATA) xj_get_addr(m_hLibXJShareSD,
			"CreateSectionData");
		if (m_pFunCreateSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function CreateSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunOpenSectionData = (PFUNOPENSECTIONDATA) xj_get_addr(m_hLibXJShareSD,
			"OpenSectionData");
		if (m_pFunOpenSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function OpenSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunCloseSectionData = (PFUNCLOSESECTION) xj_get_addr(m_hLibXJShareSD,
			"CloseSectionData");
		if (m_pFunCloseSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function CloseSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetFullSectionData = (PFUNSETFULLSD) xj_get_addr(m_hLibXJShareSD,
			"SetFullSectionData");
		if (m_pFunSetFullSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetFullSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunGetFullSectionData = (PFUNGETFULLSD) xj_get_addr(m_hLibXJShareSD,
			"GetFullSectionData");
		if (m_pFunGetFullSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetFullSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetSectionData = (PFUNSETSD) xj_get_addr(m_hLibXJShareSD,
			"SetSectionData");
		if (m_pFunSetSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}

		m_pFunGetSectionData = (PFUNGETSD) xj_get_addr(m_hLibXJShareSD,
			"GetSectionData");
		if (m_pFunGetSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetPointData = (PFUNSETPOINTDATA) xj_get_addr(m_hLibXJShareSD,
			"SetPointData");
		if (m_pFunSetPointData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetPointData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}
		
		m_pFunGetPointData = (PFUNGETPOINTDATA) xj_get_addr(m_hLibXJShareSD,
			"GetPointData");
		if (m_pFunGetPointData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetPointData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	

	} 
	else
	{
		if (NULL != pErrorMsg)
		{	
			sprintf(pErrorMsg,"LoadShareSectionData Lib failed(reason:%s)",strerror(errno));
		}
		return false;
	}
	return true;		
}

/*************************************************************
 函 数 名:   CreateSectionData
 功能概要:   创建断面数据共享内存
 返 回 值:   bool true-成功 false-失败
 参    数:   string pSHMName 共享内存名
 参    数:   vector<string> & pStationIDList 厂站ID列表
 参    数:   string pLogPath 日志路径
 参    数:   int pLogLevel 日志级别
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::CreateSectionData( string pSHMName, vector<string>& pStationIDList, string pLogPath, int pLogLevel, string& pErrorMsg )
{
	if (m_pFunCreateSectionData != NULL)
	{
		return m_pFunCreateSectionData(pSHMName,pStationIDList,pLogPath,pLogLevel,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（CreateSectionData）";
		return false;
	}
}

/*************************************************************
 函 数 名:   OpenSectionData
 功能概要:   打开断面数据共享内存
 返 回 值:   bool true-成功 false-失败
 参    数:   string pSHMName 共享内存名
 参    数:   string pLogPath 日志路径
 参    数:   int pLogLevel 日志级别
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::OpenSectionData( string pSHMName ,string pLogPath, int pLogLevel, string& pErrorMsg )
{
	if (m_pFunOpenSectionData != NULL)
	{
		return m_pFunOpenSectionData(pSHMName,pLogPath,pLogLevel,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（OpenSectionData）";
		return false;
	}	
}

/*************************************************************
 函 数 名:   CloseSectionData
 功能概要:   关闭断面数据共享内存
 返 回 值:   bool true-成功 false-失败
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::CloseSectionData( string& pErrorMsg )
{
	if (m_pFunCloseSectionData!=NULL)
	{
		return m_pFunCloseSectionData(pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（CloseSectionData）";
		return false;
	}		
}

/*************************************************************
 函 数 名:   SetFullSectionData
 功能概要:   设置某站全部断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   const string & pSectionData 断面数据块
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::SetFullSectionData( string pStationId, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetFullSectionData!=NULL)
	{
		return m_pFunSetFullSectionData(pStationId,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（SetFullSectionData）";
		return false;
	}	
}

/*************************************************************
 函 数 名:   GetFullSectionData
 功能概要:   获取某站全部断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   string & pSectionData 断面数据块
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::GetFullSectionData( string pStationId, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetFullSectionData!=NULL)
	{
		return m_pFunGetFullSectionData(pStationId,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（GetFullSectionData）";
		return false;
	}		
}

/*************************************************************
 函 数 名:   SetSectionData
 功能概要:   设置某站部分断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   int pOffSet 断面数据块起始地址
 参    数:   int pSize 断面数据块大小
 参    数:   const string & pSectionData 断面数据块
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::SetSectionData( string pStationId, int pOffSet, int pSize, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetSectionData!=NULL)
	{
		return m_pFunSetSectionData(pStationId,pOffSet,pSize,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（SetSectionData）";
		return false;
	}
}

/*************************************************************
 函 数 名:   GetSectionData
 功能概要:   获取某站部分断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   int pOffSet 断面数据块起始地址
 参    数:   int pSize 断面数据块大小
 参    数:   string & pSectionData 断面数据块
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::GetSectionData( string pStationId, int pOffSet, int pSize, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetSectionData!=NULL)
	{
		return m_pFunGetSectionData(pStationId,pOffSet,pSize,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（GetSectionData）";
		return false;
	}	
}

/*************************************************************
 函 数 名:   SetPointData
 功能概要:   设置某点断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   int pRow 行号
 参    数:   int pColumn 列号
 参    数:   const string & pSectionData 断面数据
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::SetPointData( string pStationId, int pRow, int pColumn, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetPointData!=NULL)
	{
		return m_pFunSetPointData(pStationId,pRow,pColumn,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（SetPointData）";
		return false;
	}		
}

/*************************************************************
 函 数 名:   GetPointData
 功能概要:   获取某点断面数据
 返 回 值:   bool true-成功 false-失败
 参    数:   string pStationId 厂站编号
 参    数:   int pRow 行号
 参    数:   int pColumn 列号
 参    数:   string & pSectionData 断面数据
 参    数:   string & pErrorMsg 错误信息
**************************************************************/
bool CSectionDataWrapper::GetPointData( string pStationId, int pRow, int pColumn, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetPointData!=NULL)
	{
		return m_pFunGetPointData(pStationId,pRow,pColumn,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "无效的函数指针（GetPointData）";
		return false;
	}	
}