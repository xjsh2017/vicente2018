// BatchSel.cpp: implementation of the CBatchSel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchSel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////
/** @brief           升降序作用位*/
int g_iBatchViewAsc;


CString CSel::GetCallString( int nCall )
{
	CString sReturn = "-";
	switch(nCall)
	{
	case 0:
		sReturn = StringFromID(IDS_CALLOPER_NOTCALL);
		break;
	case 1:
		sReturn = StringFromID(IDS_CALLOPER_CALL);
		break;
	case 2:
		sReturn = StringFromID(IDS_CALLOPER_NOTSUPPORT);
		break;
	case SUSPEND:
		sReturn = StringFromID(IDS_CALLOPER_SUSPEND);
		break;
	default:
		break;
	}
	return sReturn;
}

//CBatchSel
//##ModelId=49B87BB101F6
CBatchSel::CBatchSel()
{
	/** @brief           对应的二次设备*/
	m_pObj = NULL;
	m_pCpu = NULL;
	m_nCallZone = 0;
	/** @brief           是否召唤定值*/
	m_nCallSetting = 0;
	/** @brief           是否召唤开关量*/
	m_nCallDigital = 0;
	/** @brief           是否召唤软压板*/
	m_nCallSoftBoard = 0;
	/** @brief           是否召唤模拟量*/
	m_nCallAnalog = 0;
	m_nCallOsc = 0;
	m_nCallHistory = 0;
	/** @brief           当前定值区号召唤结果,  0:此项不召唤 1：成功 2：失败 3:尚未召唤*/
	m_nZone = 0;
	/** @brief           定值召唤结果, 0:此项不召唤 1：成功 2：失败 3:尚未召唤*/
	m_nSetting = 0;
	/** @brief           开关量召唤结果,  0:此项不召唤 1：成功 2：失败 3:尚未召唤*/
	m_nDigital = 0;
	/** @brief           软压板召唤结果, 0:此项不召唤 1：成功 2：失败 3:尚未召唤*/
	m_nSoftBoard = 0;
	/** @brief           模拟量召唤结果, 0:此项不召唤 1：成功 2：失败 3:尚未召唤*/
	m_nAnalog = 0;
	m_nOsc = 0;
	m_nHistory = 0;

	/** @brief           定值区号结果说明*/
	m_nZoneNote = 0;
	/** @brief           定值结果说明*/
	m_nSettingNote = 0;
	/** @brief           开关量结果说明*/
	m_nDigitalNote = 0;
	/** @brief           软压板结果说明*/
	m_nSoftBoardNote = 0;
	/** @brief           模拟量结果说明*/
	m_nAnalogNote = 0;
	m_nOscNote = 0;
	m_nHistoryNote = 0;

	m_bChecked = FALSE;

	pMap = NULL;
}


//##ModelId=49B87BB10213
CBatchSel::CBatchSel( CSecObj* pObj,CSecCPU* pCpu, BOOL bZone,  BOOL bSetting, BOOL bDigital, BOOL bSoftBoard, BOOL bAnalog )
{
	/** @brief           对应的二次设备*/
	m_pObj = pObj;
	m_pCpu = pCpu;
	m_nCallZone = bZone;
	if(pObj->m_nType == TYPE_WAVEREC)
		m_nCallZone = 2;//不支持
	/** @brief           是否召唤定值*/
	m_nCallSetting = bSetting;
	if(pCpu->m_nSettingDefs <= 0)
	{
		//没有点表
		m_nCallSetting = 2;//不支持
	}
	/** @brief           是否召唤开关量*/
	m_nCallDigital = bDigital;
	if(pCpu->m_nDigitalDefs <= 0)
	{
		//没有点表
		m_nCallDigital = 2;//不支持
	}
	/** @brief           是否召唤软压板*/
	m_nCallSoftBoard = bSoftBoard;
	if(pCpu->m_nSoftBoardDefs <= 0)
	{
		//没有点表
		m_nCallSoftBoard = 2;//不支持
	}
	/** @brief           是否召唤模拟量*/
	m_nCallAnalog = bAnalog;
	if(pCpu->m_nAnalogDefs <= 0)
	{
		//没有点表
		m_nCallAnalog = 2;//不支持
	}
	
	//定值区号结果
	m_nZone = 9;
	if(m_nCallZone == 0)
	{
		//定值区号不召唤
		m_nZone = 0;
	}
	else if(m_nCallZone == 2)
	{
		//装置不支持
		m_nZone = 3;
	}
	//定值结果
	m_nSetting = 9;
	if(m_nCallSetting == 0)
	{
		//定值不召唤
		m_nSetting = 0;
	}
	else if(m_nCallSetting == 2)
	{
		//装置不支持
		m_nSetting = 3;
	}

	//开关量结果
	m_nDigital = 9;
	if(m_nCallDigital == 0)
	{
		//开关量不召唤
		m_nDigital = 0;
	}
	else if(m_nCallDigital == 2)
	{
		//装置不支持
		m_nDigital = 3;
	}

	//软压板结果
	m_nSoftBoard = 9;
	if(m_nCallSoftBoard == 0)
	{
		//软压板不召唤
		m_nSoftBoard = 0;
	}
	else if(m_nCallSoftBoard == 2)
	{
		//装置不支持
		m_nSoftBoard = 3;
	}

	//模拟量结果
	m_nAnalog = 9;
	if(m_nCallAnalog == 0)
	{
		//模拟量不召唤
		m_nAnalog = 0;
	}
	else if(m_nCallAnalog == 2)
	{
		//装置不支持
		m_nAnalog = 3;
	}

	/** @brief           定值区号结果说明*/
	m_nZoneNote = 0;
	/** @brief           定值结果说明*/
	m_nSettingNote = 0;
	/** @brief           开关量结果说明*/
	m_nDigitalNote = 0;
	/** @brief           软压板结果说明*/
	m_nSoftBoardNote = 0;
	/** @brief           模拟量结果说明*/
	m_nAnalogNote = 0;
}
//##ModelId=49B87BB10232
CBatchSel::~CBatchSel()
{
	
}

/*************************************************************
 函 数 名：SaveData()
 功能概要：保存数据到数据库 
 返 回 值: 保存数据成功返回TRUE, 失败返回FALSE
 参    数：param1	是自动总召配置还是批量召唤配置.0-自动总召. 1-批量召唤
		   Param2
**************************************************************/
//##ModelId=49B87BB10251
BOOL CBatchSel::SaveData( int nIS_OC )
{
	//检查数据有效性
	if(m_pObj == NULL)
		return FALSE;

	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//设置字段

	//厂站ID
	Field field1;
	pApp->m_DBEngine.SetField(sql, field1, "Station_ID", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sStationID);

	//保护ID
	Field field2;
	pApp->m_DBEngine.SetField(sql, field2, "PT_ID", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sID);

	//IsZone
	Field field9;
	CString str;
	str.Format("%d", m_nCallZone);
	pApp->m_DBEngine.SetField(sql, field9, "Reverse1", EX_STTP_DATA_TYPE_INT, str);
	//IsCallSetting
	Field field3;
	str.Format("%d", m_nCallSetting);
	pApp->m_DBEngine.SetField(sql, field3, "IsCallSetting", EX_STTP_DATA_TYPE_INT, str);

	//IsCallDi
	Field field4;
	str.Format("%d", m_nCallDigital);
	pApp->m_DBEngine.SetField(sql, field4, "IsCallDi", EX_STTP_DATA_TYPE_INT, str);

	//IsCallSoftboard
	Field field5;
	str.Format("%d", m_nCallSoftBoard);
	pApp->m_DBEngine.SetField(sql, field5, "IsCallSoftboard", EX_STTP_DATA_TYPE_INT, str);

	//IsCallAi
	Field field6;
	str.Format("%d", m_nCallAnalog);
	pApp->m_DBEngine.SetField(sql, field6, "IsCallAi", EX_STTP_DATA_TYPE_INT, str);

	//Is_Auto
	Field field7;
	str.Format("%d", nIS_OC);
	pApp->m_DBEngine.SetField(sql, field7, "Is_Auto", EX_STTP_DATA_TYPE_INT, str);

	//cpu_code
	Field field8;
	str.Format("%d", m_pCpu->code);
	pApp->m_DBEngine.SetField(sql, field8, "cpu_code", EX_STTP_DATA_TYPE_INT, str);

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_OC_CONFIG_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBatchSel::SaveData, 保存失败");
		delete[] sError;
		return FALSE;
	}

	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;

	delete[] sError;

	return bReturn;
}

//##ModelId=49B87BB10271
BOOL CBatchSel::MultiSaveData( STTP_DBDATA_RECORD& sttpDB, int nIS_OC)
{
	try
	{
		//厂站ID
		STTP_DBDATA_UNIT field;
		CString str;
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_pObj->m_sStationID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//保护ID
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str = m_pObj->m_sID;
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallSetting
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallSetting);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallDi
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallDigital);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
		
		//IsCallSoftboard
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallSoftBoard);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//IsCallAi
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallAnalog);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//Is_Auto
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		CString strOC;
		strOC.Format("%d", nIS_OC);
		field.strValue = strOC;
		field.nDataLength = strOC.GetLength();
		sttpDB.field_list.push_back(field);

		//cpu_code
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_pCpu->code);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//IsCallZone
		field.nDataType = EX_STTP_DATA_TYPE_INT;
		str.Format("%d", m_nCallZone);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//是否召唤历史事件
		field.nDataType = EX_STTP_DATA_TYPE_FLOAT;
		str.Format("%d", m_nCallHistory);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);

		//是否召唤录波
		field.nDataType = EX_STTP_DATA_TYPE_STRING;
		str.Format("%d", m_nCallOsc);
		field.strValue = str;
		field.nDataLength = str.GetLength();
		sttpDB.field_list.push_back(field);
	}
	catch (...)
	{
		WriteLog("CBatchSel::MultiSaveData, 出错", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名: LoadFromDB()
 功能概要: 从数据库载入信息
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 数据库指针
		   Param2 
**************************************************************/
BOOL CBatchSel::LoadFromDB( CMemSet* pMemSet )
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		CString str;
		str = pMemSet->GetValue((UINT)0); //厂站ID
		CString sPTID = pMemSet->GetValue(1);//保护ID

		str = pMemSet->GetValue(2); //是否召唤定值
		m_nCallSetting = atoi(str);
		str = pMemSet->GetValue(3); //是否召唤开关量
		m_nCallDigital = atoi(str);
		str = pMemSet->GetValue(4); //是否召唤软压板
		m_nCallSoftBoard = atoi(str);
		str = pMemSet->GetValue(5); //是否召唤模拟量
		m_nCallAnalog = atoi(str);
		str = pMemSet->GetValue(7); //CPU号
		int nCPU = atoi(str);
		str = pMemSet->GetValue(8); //是否召唤定值区号
		m_nCallZone = atoi(str);
		str = pMemSet->GetValue(9); //是否召唤历史事件
		m_nCallHistory = atoi(str);
		str = pMemSet->GetValue(10); //是否召唤录波
		m_nCallOsc = atoi(str);
	
		//定值区号结果
		m_nZone = 9;
		if(m_nCallZone == 0)
		{
			//定值区号不召唤
			m_nZone = 0;
		}
		else if(m_nCallZone == 2)
		{
			//装置不支持
			m_nZone = 3;
		}
		//定值结果
		m_nSetting = 9;
		if(m_nCallSetting == 0)
		{
			//定值不召唤
			m_nSetting = 0;
		}
		else if(m_nCallSetting == 2)
		{
			//装置不支持
			m_nSetting = 3;
		}
		
		//开关量结果
		m_nDigital = 9;
		if(m_nCallDigital == 0)
		{
			//开关量不召唤
			m_nDigital = 0;
		}
		else if(m_nCallDigital == 2)
		{
			//装置不支持
			m_nDigital = 3;
		}
		
		//软压板结果
		m_nSoftBoard = 9;
		if(m_nCallSoftBoard == 0)
		{
			//软压板不召唤
			m_nSoftBoard = 0;
		}
		else if(m_nCallSoftBoard == 2)
		{
			//装置不支持
			m_nSoftBoard = 3;
		}
		
		//模拟量结果
		m_nAnalog = 9;
		if(m_nCallAnalog == 0)
		{
			//模拟量不召唤
			m_nAnalog = 0;
		}
		else if(m_nCallAnalog == 2)
		{
			//装置不支持
			m_nAnalog = 3;
		}
		
		//查找保护
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		m_pObj = (CSecObj*)pData->FindDevice(sPTID, TYPE_SEC);
		if(m_pObj == NULL)
			return FALSE;
		m_pCpu = m_pObj->FindCpu(nCPU);
		if(m_pCpu == NULL)
			return FALSE;
		
	}
	catch (...)
	{
		WriteLog("CBatchSel::LoadFromDB, 填入数据失败");
		return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////


BatchSel::BatchSel()
{
	/** @brief           是否召唤定值区号.0-不召唤,1-召唤,2-不支持*/
	m_nCallZone = 0;
	/** @brief           是否召唤定值.0-不召唤,1-召唤,2-不支持*/
	m_nCallSetting = 0;
	/** @brief           是否召唤开关量.0-不召唤,1-召唤,2-不支持*/
	m_nCallDigital = 0;
	/** @brief           是否召唤软压板.0-不召唤,1-召唤,2-不支持*/
	m_nCallSoftBoard = 0;
	/** @brief           是否召唤模拟量.0-不召唤,1-召唤,2-不支持*/
	m_nCallAnalog = 0;	
	m_nCallOsc = 0;
	m_nCallHistory = 0;

	m_bChecked = FALSE;

	pMap = NULL;
}


