// CrossPoint.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CrossPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrossPoint

IMPLEMENT_DYNCREATE(CCrossPoint, CObject)

CCrossPoint::CCrossPoint()
{
	/** @brief           ID(序号)*/
	m_nID = -1;
	/** @brief           所属预警表达式的ID*/
	m_nCrossID = -1;
	/** @brief           数据类型. 1.定值 2.模拟量 3.开关量 4.软压板*/
	m_nDataType = -1;
	/** @brief           所属厂站ID*/
	m_sStationID = "";
	/** @brief           所属厂站指针*/
	m_pStation = NULL;
	/** @brief           所属二次设备ID*/
	m_sDevID = "";
	/** @brief           所属二次设备指针*/
	m_pSec = NULL;
	/** @brief           所属CPU*/
	m_nCPU = -1;
	/** @brief           数据点定义的ID*/
	m_nDataPointID = -1;
	/** @brief           数据点的值*/
	m_sDataPointValue = "";
	/** @brief           计算后的值*/
	m_sSectionValue = "";

	CTime tm(1971, 1, 1, 1, 1, 1);
	/** @brief           状态改变时间*/
	m_tmTime = tm;
	/** @brief           1、预警；0、复归*/
	m_nCrossType = -1;
	/** @brief           批次号*/
	m_nSaveNo = -1;

	m_sDataPointName = "";
}


CCrossPoint::~CCrossPoint()
{
}

/*************************************************************
 函 数 名: LoadFromDB()
 功能概要: 从数据库读入数据
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 数据库数据
		   Param2 
**************************************************************/
BOOL CCrossPoint::LoadFromDB( CMemSet* pMemSet )
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //ID
		m_nID = atoi(str);
		str = pMemSet->GetValue(1); //预警点id
		m_nCrossID = atoi(str);
		str = pMemSet->GetValue(2); //数据类型
		m_nDataType = atoi(str);
		m_sStationID = pMemSet->GetValue(3); //预警点所属厂站ID
		m_sDevID = pMemSet->GetValue(4); //数据点所属二次设备ID号
		str = pMemSet->GetValue(5); //数据点所属设备CPU号
		m_nCPU = atoi(str);
		str = pMemSet->GetValue(7); //数据点定义ID号
		m_nDataPointID = atoi(str);
		m_sDataPointValue = pMemSet->GetValue(8); //数据点的值
		m_sSectionValue = pMemSet->GetValue(9); //预警值
		str = pMemSet->GetValue(10); //时间
		m_tmTime = StringToTime(str);
		str = pMemSet->GetValue(11); //预警数据类型
		m_nCrossType = atoi(str);
	}
	catch (...)
	{
		WriteLog("CCrossPoint::LoadFromDB, 填入数据失败");
		return FALSE;
	}	
	
	//创建关联关系
	if(!BuildReleation(TYPE_SEC))
	{
		WriteLog("CCrossPoint::LoadFromDB, 建立与设备的关联关系失败");
		return FALSE;
	}
	QueryDataPointName();
	return TRUE;
}

/*************************************************************
 函 数 名: BuildReleation()
 功能概要: 建立与设备的关联
 返 回 值: 关联成功返回TRUE, 失败返回FALSE
 参    数: param1 设备类型
		   Param2 
**************************************************************/
BOOL CCrossPoint::BuildReleation( int nType )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//厂站指针
	m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(m_pStation == NULL)
		return FALSE;
	//先查找二次设备指针
	m_pSec = (CSecObj*)pData->FindDevice(m_sDevID, nType);
	if(m_pSec == NULL)
		return FALSE;
	
	return TRUE;
}

/*************************************************************
 函 数 名: GetStatusDes()
 功能概要: 取得预警点指定状态的描述
 返 回 值: 状态描述
 参    数: param1 指定状态
		   Param2 
**************************************************************/
CString CCrossPoint::GetStatusDes( int nType )
{
	if(nType == 0)
		return StringFromID(IDS_CROSSSTATUS_RETURN);
	else if(nType == 1)
		return StringFromID(IDS_CROSSSTATUS_ALERT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 函 数 名: QueryDataPointName()
 功能概要: 查询数据点名称
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CCrossPoint::QueryDataPointName()
{
	if(m_nDataType == 1)
	{
		//定值
		QuerySettingDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 2)
	{
		//模拟量
		QueryAIDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 3)
	{
		//开关量
		QueryDIDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	else if(m_nDataType == 4)
	{
		//软压板
		QuerySoftBoardDataPointName(m_sDevID, m_nDataPointID, m_nCPU, m_sDataPointName);
	}
	return TRUE;
}

/*************************************************************
 函 数 名: QueryAIDataPointName()
 功能概要: 查询指定模拟量点的名称
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 二次设备ID
		   Param2 模拟量ID
		   Param3 CPU
		   Param4 模拟量名称
**************************************************************/
BOOL CCrossPoint::QueryAIDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("AI_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QueryAIDataPointName, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}

	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QueryAIDataPointName,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名: QueryDIDataPointName()
 功能概要: 查询指定模开关点的名称
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 二次设备ID
		   Param2 开关量ID
		   Param3 CPU
		   Param4 开关量名称
**************************************************************/
BOOL CCrossPoint::QueryDIDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("DI_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QueryDIDataPointName, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QueryDIDataPointName,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名: QuerySettingDataPointName()
 功能概要: 查询指定模开关点的名称
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 二次设备ID
		   Param2 定值ID
		   Param3 CPU
		   Param4 定值名称
**************************************************************/
BOOL CCrossPoint::QuerySettingDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("SETTING_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QuerySettingDataPointName, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QuerySettingDataPointName,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名: QuerySoftBoardDataPointName()
 功能概要: 查询指定软压板点的名称
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 二次设备ID
		   Param2 软压板ID
		   Param3 CPU
		   Param4 软压板名称
**************************************************************/
BOOL CCrossPoint::QuerySoftBoardDataPointName( CString sPTID, int nID, int nCPU, CString & sName )
{
	BOOL bReturn = FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", sPTID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
	
	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %d", nCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
	
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("SOFT_ID = %d", nID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CCrossPoint::QuerySoftBoardDataPointName, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		sName = pMemset->GetValue((UINT)0);
	}
	else
	{
		CString str;
		str.Format("CCrossPoint::QuerySoftBoardDataPointName,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}