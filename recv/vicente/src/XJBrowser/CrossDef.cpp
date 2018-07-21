// CrossDef.cpp : implementation file
//
#include "stdafx.h"
#include "xjbrowser.h"
#include "CrossDef.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CCrossDef

IMPLEMENT_DYNCREATE(CCrossDef, CObject)

CCrossDef::CCrossDef()
{
	/** @brief           ID*/
	m_nID = -1;
	/** @brief           所属厂站ID*/
	m_sStationID = "";
	/** @brief           所属厂站指针*/
	m_pStation = NULL;
	/** @brief           数据类型. 1.定值 2.模拟量 3.开关量 4.软压板*/
	m_nDataType = -1;
	/** @brief           预警点名称*/
	m_sName = "";
	/** @brief           该点越限告警类型编号,对应tb_pt_cross_def中的type_id*/
	m_nAlarmType = -1;
	/** @brief           当前状态. 1、预警；0、正常*/
	m_nCurStatus = -1;

	CTime tm(1971, 1, 1, 1, 1, 1);
	/** @brief           状态改变时间*/
	m_tmTime = tm;
	
	/** @brief           预警数据点链表*/
	m_listCross.RemoveAll();
	/** @brief           是否已载入详细信息*/
	m_bLoadDetail = FALSE;
}

CCrossDef::~CCrossDef()
{
	ClearDetail();
}

/*************************************************************
 函 数 名: LoadFromDB()
 功能概要: 从数据库读入数据
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 数据库数据
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadFromDB( CMemSet* pMemSet )
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
		m_sStationID = pMemSet->GetValue(1); //预警点所属厂站ID
		str = pMemSet->GetValue(2); //数据类型
		m_nDataType = atoi(str);
		m_sName = pMemSet->GetValue(3); //预警点名称
		str = pMemSet->GetValue(5); //该点越限告警类型
		m_nAlarmType = atoi(str);
		str = pMemSet->GetValue(11); //该预警点当前状态值
		m_nCurStatus = atoi(str);
		str = pMemSet->GetValue(12); //时间
		m_tmTime = StringToTime(str);
	}
	catch (...)
	{
		WriteLog("CCrossDef::LoadFromDB, 填入数据失败");
		return FALSE;
	}	
	
	//创建关联关系
	if(!BuildReleation())
	{
		WriteLog("CCrossDef::LoadFromDB, 创建关联关系失败");
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名: GetStatusDes()
 功能概要: 取得指定状态的描述
 返 回 值: 状态描述
 参    数: param1 指定状态
		   Param2 
**************************************************************/
CString CCrossDef::GetStatusDes( int nStatus )
{
	if(nStatus == 0)
		return StringFromID(IDS_CROSSSTATUS_NORMAL);
	else if(nStatus == 1)
		return StringFromID(IDS_CROSSSTATUS_ALERT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 函 数 名: GetDataTypeDes()
 功能概要: 取得数据类型描述
 返 回 值: 描述
 参    数: param1 指定类型
		   Param2 
**************************************************************/
CString CCrossDef::GetDataTypeDes( int nType )
{
	if(nType == 1)
		return StringFromID(IDS_SECPROP_SETTING);
	else if(nType == 2)
		return StringFromID(IDS_SECPROP_ANALOG);
	else if(nType == 3)
		return StringFromID(IDS_SECPROP_DIGITAL);
	else if(nType == 4)
		return StringFromID(IDS_SECPROP_SOFT);
	return StringFromID(IDS_COMMON_UNKNOWN);
}

/*************************************************************
 函 数 名: LoadDetail()
 功能概要: 从数据库读取详细信息
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadDetail()
{
	//先查最新批次
	int nSaveNo = -1;
	LoadLastSaveNo(nSaveNo);

	//清除旧数据
	ClearDetail();

	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件
	CString str;

	Condition condition1;
	str.Format("CD_BATH = %d", nSaveNo);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	Condition condition2;
	str.Format("SC_ID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CROSS_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CCrossDef::LoadDetail, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CCrossDef::LoadDetail, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			CCrossPoint* pPoint = new CCrossPoint;
			if(!pPoint->LoadFromDB(pMemset))
			{
				//读取失败,删除
				delete pPoint;
			}
			else
			{
				m_listCross.AddTail(pPoint);
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CCrossDef::LoadDetail,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	m_bLoadDetail = TRUE;
	
	return bReturn;
}

/*************************************************************
 函 数 名: LoadLastSaveNo()
 功能概要: 从数据库读取最新批次
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 最新批次
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadLastSaveNo( int& nSaveNo )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//字段
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "MAX(CD_BATH)", EX_STTP_DATA_TYPE_INT);

	//条件
	CString str;
	Condition condition2;
	str.Format("SC_ID = %d", m_nID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CROSS_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CCrossDef::LoadLastSaveNo, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CCrossDef::LoadLastSaveNo, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		CString str = pMemset->GetValue((UINT)0);
		nSaveNo = atoi(str);
	}
	else
	{
		CString str;
		str.Format("CCrossDef::LoadLastSaveNo,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名: ClearDetail()
 功能概要: 清除旧数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CCrossDef::ClearDetail()
{
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossPoint* pPoint = (CCrossPoint*)m_listCross.GetNext(pos);
		if(pPoint != NULL)
			delete pPoint;
	}
	m_listCross.RemoveAll();
}

/*************************************************************
 函 数 名: LoadFromSTTP()
 功能概要: 从STTP报文中读取事件信息
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 STTP报文数据结构
		   Param2 
**************************************************************/
BOOL CCrossDef::LoadFromSTTP( STTP_FULL_DATA& sttpData )
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20159)
	{
		CString str;
		str.Format("CCrossDef::LoadFromSTTP, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return FALSE;
	}
	//所属厂站ID
	m_sStationID = sttpData.sttp_body.ch_pt_id;
	//预警点ID
	m_nID = sttpData.sttp_body.nFlag;
	//创建关联关系
	if(!BuildReleation())
	{
		WriteLog("CCrossDef::LoadFromSTTP, 创建关联关系失败");
		return FALSE;
	}
	//预警点状态
	m_nCurStatus = sttpData.sttp_body.nStatus;
	
	//预警发生时间
	CString sTime = sttpData.sttp_body.ch_time_15_BIT1;
	int nMS = 0;
	m_tmTime = StringToTimeSttp15(sTime, nMS);
	
	return TRUE;
}

/*************************************************************
 函 数 名: BuildReleation()
 功能概要: 建立关联关系
 返 回 值: 关联成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CCrossDef::BuildReleation()
{
	//创建关联关系
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;
	
	//厂站指针
	m_pStation = NULL;
	m_pStation = (CStationObj*)pData->FindDevice(m_sStationID, TYPE_STATION);
	if(m_pStation == NULL)
	{	
		return FALSE;
	}
	return TRUE;
}