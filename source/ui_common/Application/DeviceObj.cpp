/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   17:37
	filename: 	F:\vicente\Code\src\XJBrowser\DeviceObj.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	DeviceObj
	file ext:	cpp
	author:		LYH
	
	purpose:	各设备类详细
*********************************************************************/
// DeviceObj.cpp : implementation file
//
#include "DeviceObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)//屏蔽4786错误

/////////////////////////////////////////////////////////////////////////////
// CDeviceObj
UINT StationLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CStationObj* pStation = (CStationObj*)pParam;
	pStation->BuildData(pStation->m_pDBEngine);
	return 0;
}
IMPLEMENT_DYNCREATE(CDeviceObj, CObject)

//##ModelId=49B87BA50149
CDeviceObj::CDeviceObj()
{
	m_sID		= "";	
	m_sName		= "";
	m_nType		= -1;	
	m_sType		= "";
	m_nVoltage	= 0;
	m_n103Group = -1;
	m_n103Item	= -1;
	m_sStationID = "";
	m_nRunStatu = 0;
	m_nCommStatus = 0;
	m_nCode		= -1;

	m_htItem = NULL;
	m_bHasSecDevice = FALSE;
}

//##ModelId=49B87BA50271
CDeviceObj::~CDeviceObj()
{
}

/*************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息,各子类实现
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA50204
BOOL CDeviceObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置

	//子类实现
	return TRUE;
}

/*************************************************************
 函 数 名：Save()
 功能概要：保存设备信息到数据库, 各子类实现
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA50242
BOOL CDeviceObj::Save(CDBEngine* pDBEngine)
{
	//子类实现
	return TRUE;
}
//CDeviceObj end
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//CNetObj 地区
IMPLEMENT_DYNCREATE(CNetObj, CDeviceObj)
//##ModelId=49B87BA502DE
CNetObj::CNetObj()
{
	m_nType		= TYPE_NET;	
	m_sType		= STRING_NET;

	m_sNote		= "";

	m_nStationAbnormity = 0;
	m_nPTCommAbnormity = 0;
	m_nPTRunAbnormity = 0;
}

//##ModelId=49B87BA5030D
CNetObj::~CNetObj()
{

}

/*************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA5035C
BOOL CNetObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sName = pMemSet->GetValue(1); //名称
	m_sNote = pMemSet->GetValue(2); //备注
	return TRUE;
}

/*************************************************************
 函 数 名：Save()
 功能概要：保存设备信息到数据库
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA50399
BOOL CNetObj::Save(CDBEngine* pDBEngine)
{
	return TRUE;
}

//CNetObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CStationObj 厂站
IMPLEMENT_DYNCREATE(CStationObj, CDeviceObj)

//##ModelId=49B87BA600AC
CStationObj::CStationObj()
{
	m_nType = TYPE_STATION;
	m_sType = STRING_STATION;

	m_sNetID = "";
	m_nProtocolId = -1;
	m_sProtocol = "";
	m_nCommMode	= -1;

	m_sSvgPath = "";
	m_sManufacturer = "";

	m_sNewStatusReason = "";


	/** @brief           通道0状态*/
	m_nCommStatus = -1;
	/** @brief           通道1状态*/
	m_nCommStatus1 = -1;
	/** @brief           通道1最后一次状态变位的原因*/
	m_sNewStatusReason1 = "";

	m_ptCenter.x = 0;
	m_ptCenter.y = 0;

	m_sIP = "";
	m_nIPFlag = 0;

	m_arrCommAbnormity.RemoveAll();
	m_arrRunAbnormity.RemoveAll();

	m_arrVoltage.RemoveAll();

	m_nChannel = 0;

	m_pLock = NULL;

	m_pDBEngine = NULL;
}

//##ModelId=49B87BA600DA
CStationObj::~CStationObj()
{
	if(m_pLock != NULL)
	{
		delete m_pLock;
		m_pLock = NULL;
	}
	Clear();
	m_arrCommAbnormity.RemoveAll();
	m_arrRunAbnormity.RemoveAll();
	m_arrVoltage.RemoveAll();
}

/*************************************************************
 函 数 名：AddDevice()
 功能概要：增加设备到各自链表
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   设备类型
           Param2   设备指针
**************************************************************/
//##ModelId=49B87BA602AF
BOOL CStationObj::AddDevice(int nType, CDeviceObj* pObj)
{
	if(pObj == NULL)
		return FALSE;
	CSecObj* pSec = NULL;
	//统计电压等级
	if(pObj->m_nType == TYPE_TRANS)
	{
		CTransObj* pTrans = (CTransObj*)pObj;
		StatVoltage(pTrans->m_nVoltageH);
	}
	else
		StatVoltage(pObj->m_nVoltage);

	switch(nType)
	{
	case TYPE_BAY: //间隔
		m_BayMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_BREAK: //开关
		m_BreakMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_BUS: //母线
		m_BusMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_CAP: //电容器
		m_CapMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_GEN: //发电机
		m_GenMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_LINE: //线路 
		m_LineMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_PMS: //保护管理机
		m_PmsMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_REACTOR: //电抗器
		m_ReactorMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_SWITCH: //刀闸
		m_SwitchMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_TRANS: //变压器
		m_TransMap.SetAt(pObj->m_sID, pObj);
		break;

	case TYPE_SEC: //其它二次设备
	case TYPE_PROTECT: //保护 
	case TYPE_WAVEREC: //录波器
		pSec = (CSecObj*)pObj;
		pSec->m_pOwner = FindDeviceNoLock(pSec->m_sOwnerID, pSec->m_nOwnerType);
		if(pSec->m_pOwner != NULL)
			pSec->m_pOwner->m_bHasSecDevice = TRUE;
		m_SecMap.SetAt(pObj->m_sID, pObj);
		m_UriMap.SetAt(pObj->m_sUri, (CSecObj*)pObj);
		break;

	default:
		break;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA60251
BOOL CStationObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //厂站ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	m_sIP = pMemSet->GetValue(6);//IP
	m_sPort = pMemSet->GetValue(7);//port
	CString str;
	str = pMemSet->GetValue(int(8)); //通信规约
	m_nProtocolId = atoi(str);
	str = pMemSet->GetValue(int(9)); //通信方式
	m_nCommMode = atoi(str);
	m_sTel = pMemSet->GetValue(10); //telphone
	m_sNetID = pMemSet->GetValue(int(12)); //所属地区ID
	str = pMemSet->GetValue((int)16);//在地理图中的位置
	GetGeoPosition(str);
	str = pMemSet->GetValue((int)19); //IPFalg
	m_nIPFlag = atoi(str);
	m_sSvgPath = pMemSet->GetValue(20); //SVG文件路径
	m_sManufacturer = pMemSet->GetValue(21);
	return TRUE;
}

/*************************************************************
 函 数 名：Clear()
 功能概要：清除厂站下所有设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA602DE
BOOL CStationObj::Clear()
{
	POSITION pos;
	CDeviceObj* pObj = NULL;
	CString sID;

	//二次设备
	for(pos = m_SecMap.GetStartPosition(); pos != NULL; )
	{
		m_SecMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_SecMap.RemoveAll();
	m_UriMap.RemoveAll();

	//开关
	for(pos = m_BreakMap.GetStartPosition(); pos != NULL; )
	{
		m_BreakMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BreakMap.RemoveAll();
	
	//母线
	for(pos = m_BusMap.GetStartPosition(); pos != NULL; )
	{
		m_BusMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BusMap.RemoveAll();
	
	//电容器
	for(pos = m_CapMap.GetStartPosition(); pos != NULL; )
	{
		m_CapMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_CapMap.RemoveAll();
	
	//发电机
	for(pos = m_GenMap.GetStartPosition(); pos != NULL; )
	{
		m_GenMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_GenMap.RemoveAll();
	
	//线路 
	for(pos = m_LineMap.GetStartPosition(); pos != NULL; )
	{
		m_LineMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_LineMap.RemoveAll();

	//保护管理机
	for(pos = m_PmsMap.GetStartPosition(); pos != NULL; )
	{
		m_PmsMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_PmsMap.RemoveAll();

	//电抗器
	for(pos = m_ReactorMap.GetStartPosition(); pos != NULL; )
	{
		m_ReactorMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_ReactorMap.RemoveAll();
	
	//刀闸
	for(pos = m_SwitchMap.GetStartPosition(); pos != NULL; )
	{
		m_SwitchMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_SwitchMap.RemoveAll();
		
	//变压器
	for(pos = m_TransMap.GetStartPosition(); pos != NULL; )
	{
		m_TransMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_TransMap.RemoveAll();

	//间隔
	for(pos = m_BayMap.GetStartPosition(); pos != NULL; )
	{
		m_BayMap.GetNextAssoc(pos, sID, (CObject *&)pObj);
		if(pObj != NULL)
			delete pObj;
	}
	m_BayMap.RemoveAll();

	return TRUE;
}

/*************************************************************
 函 数 名：Save()
 功能概要：保存设备信息到数据库
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA60280
BOOL CStationObj::Save(CDBEngine* pDBEngine)
{
	return TRUE;
}

//##ModelId=49B87BA6030D
CDeviceObj* CStationObj::FindDevice(CString sID, int nType)
{
	if(m_pLock != NULL)
	{
		CLockUp lockup(m_pLock);
	}
	
	return FindDeviceNoLock(sID, nType);
}

/*************************************************************
 函 数 名：FindDeviceBy103()
 功能概要：根据103组号条目号查找设备
 返 回 值: 设备指针
 参    数：param1	103组号条目号.格式:组号#条目号
		   Param2	设备类型
**************************************************************/
//##ModelId=49B87BA6035B
CDeviceObj * CStationObj::FindDeviceBy103( CString str103, int nType )
{
	//判断传进来的是否103组号和条目号
	int nFind = str103.Find('#', 0);
	if(nFind == -1)
	{
		return FindDevice(str103, nType);
	}
	
	//得到103组号和条目号
	CString strGroup = str103.Left(nFind);
	CString strItem = str103.Right(str103.GetLength() - nFind - 1);

	CDeviceObj * pReturn = NULL;
	POSITION pos;
	CDeviceObj * pObj;
	CString strID;
	switch(nType)
	{
	case TYPE_BAY: //间隔
		for(pos = m_BayMap.GetStartPosition(); pos != NULL;)
		{
			m_BayMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_BREAK: //开关
		for(pos = m_BreakMap.GetStartPosition(); pos != NULL;)
		{
			m_BreakMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_BUS: //母线
		for(pos = m_BusMap.GetStartPosition(); pos != NULL;)
		{
			m_BusMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_CAP: //电容器
		for(pos = m_CapMap.GetStartPosition(); pos != NULL;)
		{
			m_CapMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_GEN: //发电机
		for(pos = m_GenMap.GetStartPosition(); pos != NULL;)
		{
			m_BayMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_LINE: //线路 
		for(pos = m_LineMap.GetStartPosition(); pos != NULL;)
		{
			m_LineMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_PMS: //保护管理机
		for(pos = m_PmsMap.GetStartPosition(); pos != NULL;)
		{
			m_PmsMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_REACTOR: //电抗器
		for(pos = m_ReactorMap.GetStartPosition(); pos != NULL;)
		{
			m_ReactorMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_SWITCH: //刀闸
		for(pos = m_SwitchMap.GetStartPosition(); pos != NULL;)
		{
			m_SwitchMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_TRANS: //变压器
		for(pos = m_TransMap.GetStartPosition(); pos != NULL;)
		{
			m_TransMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
		
	case TYPE_SEC: //其它二次设备	
	case TYPE_PROTECT: //保护 		
	case TYPE_WAVEREC: //录波器
		for(pos = m_SecMap.GetStartPosition(); pos != NULL;)
		{
			m_SecMap.GetNextAssoc(pos, strID, (CObject *&)pObj);
			if(pObj == NULL)
				continue;
			if(pObj->m_n103Group == atoi(strGroup) && pObj->m_n103Item == atoi(strItem))
			{
				pReturn = pObj;
				break;
			}
		}
		break;
	default:
		break;
	}

	return pReturn;
}


/*
 *  @brief   	FindDeviceBy103	 根据uri查找设备 
 *  @param 		[In]a param of Type  CString uri
 *  @return 	CDeviceObj * 设备指针
 */
 CDeviceObj * CStationObj::FindDeviceByUri(CString strUri)
{		
	 CDeviceObj * pObj = NULL;
	 m_UriMap.Lookup(strUri, (CObject*&)pObj);
	 return pObj;
}



/*************************************************************
 函 数 名：LoadStatusFromDB()
 功能概要：从硬盘数据库读入子站与主站通讯状态,只查询最新一次
 返 回 值: void 状态
 参    数：param1	通道
**************************************************************/
//##ModelId=49B87BA6038A
void CStationObj::LoadStatusFromDB(int nChannel, CDBEngine* pDBEngine)
{
	MYASSERT(pDBEngine);
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//通过Station_ID查询最新一次值(Status, Time, Reason)

	//设置字段
	Field Field0;
	pDBEngine->SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pDBEngine->SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pDBEngine->SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pDBEngine->SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);

	//设置条件
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", m_sID);
	pDBEngine->SetCondition(sql, Condition0, str);

	Condition Condition2;
	str.Format("Reserve1 = %d", nChannel);
	pDBEngine->SetCondition(sql, Condition2, str);

	Condition Condition1;
	pDBEngine->SetCondition(sql, Condition1, "order by TIME DESC, ID DESC", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet pMemset;
//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CStationObj::LoadStatusFromDB, 查询失败");
		delete[] sError;
//		delete pMemset;
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount == 1)
		{
			//只应该有一条记录
			if(nChannel == 0) //0号通道
			{
				CString str = pMemset.GetValue((UINT)0); //status
				m_nCommStatus = atoi(str);
				str = pMemset.GetValue((UINT)1); //time
				m_tmNewStatus = StringToTime(str);
				str = pMemset.GetValue((UINT)2); //reason
				int nReason = atoi(str);
				m_sNewStatusReason = GetReason(nReason);
			}
			else if(nChannel == 1) //1号通道
			{	
				CString str = pMemset.GetValue((UINT)0); //status
				m_nCommStatus1 = atoi(str);
				str = pMemset.GetValue((UINT)1); //time
				m_tmNewStatus1 = StringToTime(str);
				str = pMemset.GetValue((UINT)2); //reason
				int nReason = atoi(str);
				m_sNewStatusReason1 = GetReason(nReason);	
			}
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::LoadStatusFromDB,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	sError = NULL;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   GetDurativeTime	
返回值:   void	
功能概要: 取得状态持续时间
参数: CTimeSpan & tsDurative1	主通道状态持续时间
参数: CTimeSpan & tsDurative2	备用通道状态持续时间
*****************************************************/
void CStationObj::GetDurativeTime(CTimeSpan& tsDurative1, CTimeSpan& tsDurative2)
{
	//取得当前时间
	CTime t = CTime::GetCurrentTime();
	tsDurative1 = t - m_tmNewStatus;

	if(tsDurative1 < 0)
	{
		tsDurative1 = 0;
		CString str;
		str.Format("厂站[%s] 0号通道时间刷新出现负值,时间:%s", m_sID, m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}

	tsDurative2 = t - m_tmNewStatus1;

	if(tsDurative2 < 0)
	{
		tsDurative2 = 0;
		CString str;
		str.Format("厂站[%s] 1号通道时间刷新出现负值, 时间:%s", m_sID, m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}
}

/*************************************************************
 函 数 名：LoadStatusFromSTTP()
 功能概要：从报文中读取主子站通讯状态
 返 回 值: void
 参    数：param1	报文内容
**************************************************************/
//##ModelId=49B87BA603B9
void CStationObj::LoadStatusFromSTTP( STTP_FULL_DATA* pSttpData )
{
	MYASSERT(pSttpData);
	
	//判断报文头
	if(pSttpData->sttp_head.uMsgID != 20144)
	{
		return;
	}
	
	//得到对应装置
	CString strID = pSttpData->sttp_body.ch_station_id;
	//判断是否本厂站的报文
	if(strID != m_sID)
	{
		return;
	}

	//通道
	int nChannel = pSttpData->sttp_body.nFlag;
	
	//原因
	int nReason = pSttpData->sttp_body.nEventType;
	CString  strLog;
	strLog.Format("收到报文定义ID为%d", nReason);
	WriteLog(strLog);
	//时间
	CString strStationTime = pSttpData->sttp_body.ch_time_12_BIT1;
	CString strReasonTime = pSttpData->sttp_body.ch_time_12_BIT2;
	CTime tmReason;
	if (strReasonTime.IsEmpty())
	{
		tmReason = CTime::GetCurrentTime();
	}
	else
	{
		tmReason = StringToTimeSttp12(strReasonTime);
	}
	CTime tmStation = StringToTimeSttp12(strStationTime);
 
	SetStatus(pSttpData->sttp_body.nStatus, tmStation, nReason, nChannel, tmReason, m_nIPFlag);
}

/*************************************************************
 函 数 名：SecCommChange()
 功能概要：保护通讯状态变化, 此处需维护保护通讯状态异常链表
 返 回 值: void
 参    数：param1	状态变化的保护
		   Param2	变化后的状态
**************************************************************/
//##ModelId=49B87BA603D8
void CStationObj::SecCommChange( CSecObj* pObj, int nStatus )
{
	//判断数据有效性
	if(pObj == NULL)
		return;
	if(nStatus<0 || nStatus>1)
		return;
	//判断链表中是否已存在此装置
	BOOL bExit = FALSE;
	POSITION pos, posOld;
	pos = m_arrCommAbnormity.GetHeadPosition();
	while(pos != NULL)
	{
		//记录老位置
		posOld = pos;
		CSecObj* pSec = (CSecObj*)m_arrCommAbnormity.GetNext(pos);
		if(pSec->m_sID == pObj->m_sID)
		{
			//已存在
			bExit = TRUE;
			//判断现在的状态是否是正常,是的话从链表删除此保护
			if(nStatus == 0)
			{
				//通讯状态正常
				m_arrCommAbnormity.RemoveAt(posOld);
			}
			break;
		}
	}
	//链表中不存在此装置
	if(!bExit)
	{
		//如果状态是断开,则加入到链表
		if(nStatus == 1)
		{
			m_arrCommAbnormity.AddTail(pObj);
		}
	}
}

/*************************************************************
 函 数 名：SecRunChange()
 功能概要：保护运行状态变化,此处维护运行状态异常链表
 返 回 值: void
 参    数：param1	状态变化的保护
		   Param2	变化后的状态
**************************************************************/
//##ModelId=49B87BA7001F
void CStationObj::SecRunChange( CSecObj* pObj, int nStatus )
{
	//判断数据有效性
	if(pObj == NULL)
		return;
	if(nStatus<0 || nStatus>4)
		return;
	//判断链表中是否已存在此装置
	BOOL bExit = FALSE;
	POSITION pos, posOld;
	pos = m_arrRunAbnormity.GetHeadPosition();
	while(pos != NULL)
	{
		//记录老位置
		posOld = pos;
		CSecObj* pSec = (CSecObj*)m_arrRunAbnormity.GetNext(pos);
		if(pSec->m_sID == pObj->m_sID)
		{
			//已存在
			bExit = TRUE;
			//判断现在的状态是否是运行,是的话从链表删除此保护
			if(nStatus == 2)
			{
				//通讯状态正常
				m_arrRunAbnormity.RemoveAt(posOld);
			}
		}
	}
	//链表中不存在此装置
	if(!bExit)
	{
		//如果状态是非运行,则加入到链表
		if(nStatus != 2)
		{
			m_arrRunAbnormity.AddTail(pObj);
		}
	}
}

/*************************************************************
 函 数 名：GetCommAbnormityNum()
 功能概要: 得到本厂站下通讯状态异常的保护个数
 返 回 值: 通讯状态正常的保护数
**************************************************************/
//##ModelId=49B87BA7003E
int CStationObj::GetCommAbnormityNum()
{
	int nCount = 0;
	nCount = m_arrCommAbnormity.GetCount();
	return nCount;
}

/*************************************************************
 函 数 名：GetRunAbnormityNum()
 功能概要：得到本厂站下运行状态异常的保护个数
 返 回 值: 运行状态异常的保护数
**************************************************************/
//##ModelId=49B87BA7004E
int CStationObj::GetRunAbnormityNum()
{
	int nCount = 0;
	nCount = m_arrRunAbnormity.GetCount();
	return nCount;
}

/*************************************************************
 函 数 名：SaveSvgPath()
 功能概要：保存SVG文件路径到数据库
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数：param1	SVG路径(相对路径)
		   Param2
**************************************************************/
//##ModelId=49B87BA7005D
BOOL CStationObj::SaveSvgPath( CString sPath, CDBEngine* pDBEngine )
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//把路径的\转换成/
	sPath.Replace("\\", "/");
	
	//指定station_id,更新SVG_Path
	
	CString str;	
	//字段
	//SVG_Path
	Field Field1;
	pDBEngine->SetField(sql, Field1, "SVG_Path", EX_STTP_DATA_TYPE_STRING, sPath);
	
	//条件
	//station_id
	Condition condition2;
	str.Format("station_id = '%s'", m_sID);
	pDBEngine->SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJUpdateData(EX_STTP_INFO_STATION_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CStationObj::SaveSvgPath, 更新失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("CStationObj::SaveSvgPath,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：GetReason()
 功能概要：得到厂站状态改变原因描述
 返 回 值: 状态改变原因描述
 参    数：param1	状态改变原因
		   Param2
**************************************************************/
//##ModelId=49B87BA7007D
CString CStationObj::GetReason( int nType )
{

	/*CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	CString strReason = _T("");
	if(pData->m_StationStatusConfigMap.Lookup(nType, strReason))
	{
		return strReason;
	}
	else
	{
		//	0: 软件异常  1: 硬件通道异常  其它:其它原因待定
		if(0 == nType)
			return "无故障";
		else if(1 == nType)
			return "硬件故障";
		else if(2 == nType)
			return "子站软件异常";
		else if(3 == nType)
			return "主站主动断开";
		else
			return "未知原因";
	}*/
	return "";
}

/*************************************************************
 函 数 名: StatVoltage()
 功能概要: 统计站内电压等级,保存在m_arrVoltage中
 返 回 值: 
 参    数: param1 电压等级
		   Param2 
**************************************************************/
void CStationObj::StatVoltage( int nVoltage )
{
	//判断数据有效性
	if(nVoltage <= 0)
		return;
	BOOL bFind = FALSE;
	//查找是否已记录此电压
	for(int i = 0; i < m_arrVoltage.GetSize(); i++)
	{
		int nV = m_arrVoltage.GetAt(i);
		if(nV == nVoltage)
		{
			bFind = TRUE;
			break;
		}
	}

	//增加新电压等级
	if(!bFind)
	{
		m_arrVoltage.Add(nVoltage);
	}
}

/*************************************************************
 函 数 名: SetStatus()
 功能概要: 设置厂站状态
 返 回 值: 
 参    数: param1 状态
		   Param2 时间
		   Param3 原因
		   Param4 通道
**************************************************************/
void CStationObj::SetStatus( int nStatus, CTime tm, int nReason, int nChannel, CTime tmReasonTime, int nIPFlag)
{
	m_nChannel = nChannel;
	if(nChannel == 0)
	{
		//0号通道
		m_nCommStatus = nStatus;
		m_tmNewStatus = tm;
		m_sNewStatusReason = GetReason(nReason);
		m_tmReasonTime = tmReasonTime;
		if (nIPFlag != 2)
		{
			m_nCommStatus1 = -1;
		}
	}
	else
	{
		//1号通道
		m_nCommStatus1 = nStatus;
		m_tmNewStatus1 = tm;
		m_sNewStatusReason1 = GetReason(nReason);
		m_tmReasonTime1 = tmReasonTime;
		if (nIPFlag != 2)
		{
			m_nCommStatus = -1;
		}
	}
}

/************************************
Date:2011/7/19  Author:LYH
函数名:    BuildBusMap
返回值:   BOOL 成功返回true，失败返回false
功能概要: 读取本厂站下母线配置并填入母线MAP
************************************/
BOOL CStationObj::BuildBusMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_BusMap.RemoveAll();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult =pDBEngine->XJSelectRealData(EX_STTP_INFO_BUS_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBusMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);
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
			str.Format("CStationObj::BuildBusMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CBusObj * pObj = new CBusObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_BusMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBusMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::LoadData(CDBEngine* pDBEngine)
{
	m_pDBEngine = pDBEngine;
	AfxBeginThread(StationLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
	return TRUE;
}

void CStationObj::InitLock()
{
	m_pLock = new CXJLock(m_sID);
}

BOOL CStationObj::BuildData(CDBEngine* pDBEngine)
{
	CLockUp lockup(m_pLock);
	BuildBusMap(pDBEngine);
	BuildBayMap(pDBEngine);
	BuildBreakMap(pDBEngine);
	BuildCapMap(pDBEngine);
	BuildGenMap(pDBEngine);
	BuildLineMap(pDBEngine);
	BuildPmsMap(pDBEngine);
	BuildSwitchMap(pDBEngine);
	BuildTransMap(pDBEngine);
	BuildSecMap(pDBEngine);
	return TRUE;
}

BOOL CStationObj::BuildLineMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_LineMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_LINE_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildLineMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);
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
			str.Format("CStationObj::BuildLineMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CLineObj * pObj = new CLineObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_LineMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildLineMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildBreakMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_BreakMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_BREAKER_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBreakMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);
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
			str.Format("CStationObj::BuildBreakMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CBreakObj * pObj = new CBreakObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_BreakMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBreakMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildBayMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_BayMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_BAY_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildBayMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);
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
			str.Format("CStationObj::BuildBayMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CBayObj * pObj = new CBayObj;
			if(pObj->Load(pMemset))
			{
				//读取数据成功,加入map
				m_BayMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildBayMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildCapMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_CapMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_CAPACITOR_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildCapMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);
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
			str.Format("CStationObj::BuildCapMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CCapObj * pObj = new CCapObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_CapMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildCapMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildReactorMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_ReactorMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_REACTOR_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildReactorMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildReactorMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CReactorObj * pObj = new CReactorObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_ReactorMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildReactorMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildSwitchMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;

	m_SwitchMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_SWITCH_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildSwitchMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildSwitchMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CSwitchObj * pObj = new CSwitchObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_SwitchMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildSwitchMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildPmsMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_PmsMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_PMS_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildPmsMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildPmsMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CPmsObj * pObj = new CPmsObj;
			if(pObj->Load(pMemset))
			{
				//读取数据成功,加入map
				m_PmsMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildPmsMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildTransMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_TransMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_TRANS_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildTransMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildTransMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CTransObj * pObj = new CTransObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltageH);
				//读取数据成功,加入map
				m_TransMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildTransMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildGenMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	m_GenMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_GEN_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildGenMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildGenMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CGenObj * pObj = new CGenObj;
			if(pObj->Load(pMemset))
			{
				StatVoltage(pObj->m_nVoltage);
				//读取数据成功,加入map
				m_GenMap.SetAt(pObj->m_sID, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildGenMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

BOOL CStationObj::BuildSecMap(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);
	
	BOOL bReturn = TRUE;
	
	m_SecMap.RemoveAll();
	m_UriMap.RemoveAll();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	Field Field1;
	pDBEngine->SetField(sql, Field1, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field2;
	pDBEngine->SetField(sql, Field2, "Uri", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pDBEngine->SetField(sql, Field3, "Name", EX_STTP_DATA_TYPE_STRING);
	
	Field Field4;
	pDBEngine->SetField(sql, Field4, "Type", EX_STTP_DATA_TYPE_INT);
	
	Field Field41;
	pDBEngine->SetField(sql, Field41, "Kind", EX_STTP_DATA_TYPE_STRING);
	
	Field Field5;
	pDBEngine->SetField(sql, Field5, "Model", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	pDBEngine->SetField(sql, Field6, "Sysversion", EX_STTP_DATA_TYPE_STRING);
	
	Field Field7;
	pDBEngine->SetField(sql, Field7, "cpunum", EX_STTP_DATA_TYPE_INT);
	
	Field Field8;
	pDBEngine->SetField(sql, Field8, "dev_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field9;
	pDBEngine->SetField(sql, Field9, "Crc", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pDBEngine->SetField(sql, Field10, "Addr", EX_STTP_DATA_TYPE_STRING);
	
	Field Field11;
	pDBEngine->SetField(sql, Field11, "com_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pDBEngine->SetField(sql, Field12, "run_status", EX_STTP_DATA_TYPE_INT);
	
	Field Field13;
	pDBEngine->SetField(sql, Field13, "station_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field14;
	pDBEngine->SetField(sql, Field14, "primdev_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	pDBEngine->SetField(sql, Field15, "primdev_type", EX_STTP_DATA_TYPE_INT);
	
	Field Field16;
	pDBEngine->SetField(sql, Field16, "pt_type_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field17;
	pDBEngine->SetField(sql, Field17, "Protocoltype", EX_STTP_DATA_TYPE_INT);
	
	Field Field18;
	pDBEngine->SetField(sql, Field18, "103addr", EX_STTP_DATA_TYPE_INT);
	
	Field Field19;
	pDBEngine->SetField(sql, Field19, "103group", EX_STTP_DATA_TYPE_INT);
	
	Field Field20;
	pDBEngine->SetField(sql, Field20, "103item", EX_STTP_DATA_TYPE_INT);
	
	Field Field21;
	pDBEngine->SetField(sql, Field21, "Commu_status", EX_STTP_DATA_TYPE_INT);
	
	Field Field22;
	pDBEngine->SetField(sql, Field22, "CommStatusTime", EX_STTP_DATA_TYPE_TIME);
	
	Field Field23;
	pDBEngine->SetField(sql, Field23, "RunStatusTime", EX_STTP_DATA_TYPE_TIME);
	
	REALDATA_CONDITION realCon;
	realCon.IsUse = TRUE;
	realCon.station_id = m_sID;
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectRealData(EX_STTP_INFO_SECDEV_CFG, sql, realCon, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		CString strLog;
		strLog.Format("CStationObj::BuildSecMap, 查询失败,厂站ID:%s", m_sID);
		WriteLogTemp(strLog);

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
			str.Format("CStationObj::BuildSecMap, 读取到%d条数据, 厂站ID:%s", nCount, m_sID);
			WriteLogTemp(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CSecObj * pObj = new CSecObj;
			if(pObj->Load(pMemset))
			{
				//设置一次设备
				pObj->m_pOwner = FindDeviceNoLock(pObj->m_sOwnerID, pObj->m_nOwnerType);
				//设置厂站
				pObj->m_pStation = this;
				//读取数据成功,加入map
				m_SecMap.SetAt(pObj->m_sID, pObj);
				m_UriMap.SetAt(pObj->m_sUri, pObj);
			}
			else
			{
				//读取数据失败, delete
				delete pObj;
				pObj = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CStationObj::BuildSecMap,查询失败,厂站ID:%s,原因为%s", m_sID, sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    FindDeviceNoLock
返回值:   CDeviceObj*
功能概要: 查找本厂站下的设备,无锁版 
参数: CString sID 设备ID
参数: int nType 设备类型
************************************/
CDeviceObj* CStationObj::FindDeviceNoLock( CString sID, int nType )
{
	//去空格
	sID.TrimLeft();
	sID.TrimRight();
	CDeviceObj * pReturn = NULL;
	
	switch(nType)
	{
	case TYPE_BAY: //间隔
		m_BayMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_BREAK: //开关
		m_BreakMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_BUS: //母线
		m_BusMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_CAP: //电容器
		m_CapMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_GEN: //发电机
		m_GenMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_LINE: //线路 
		m_LineMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_PMS: //保护管理机
		m_PmsMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_REACTOR: //电抗器
		m_ReactorMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_SWITCH: //刀闸
		m_SwitchMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_TRANS: //变压器
		m_TransMap.Lookup(sID, (CObject*&)pReturn);
		break;
		
	case TYPE_SEC: //其它二次设备
	case TYPE_PROTECT: //保护 
	case TYPE_WAVEREC: //录波器
		m_SecMap.Lookup(sID, (CObject*&)pReturn);
		break;
	default:
		break;
	}
	return pReturn;
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    GetPriDeviceList
返回值:   void
功能概要: 取得一次设备链表
参数: DEVICE_LIST & listDevice 用于保存一次设备的链表
参数: int nDeviceType 指定要查找的一次设备类型，-1表示所有类型
************************************/
void CStationObj::GetPriDeviceList( DEVICE_LIST& listDevice, int nDeviceType )
{
	POSITION pos = NULL;
	CString sID;
	CDeviceObj* pObj = NULL;
	//间隔
	if(nDeviceType == TYPE_BAY || nDeviceType == -1)
	{
		for(pos = m_BayMap.GetStartPosition(); pos != NULL; )
		{
			m_BayMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//开关
	if(nDeviceType == TYPE_BREAK || nDeviceType == -1)
	{
		for(pos = m_BreakMap.GetStartPosition(); pos != NULL; )
		{
			m_BreakMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//母线
	if(nDeviceType == TYPE_BUS || nDeviceType == -1)
	{
		for(pos = m_BusMap.GetStartPosition(); pos != NULL; )
		{
			m_BusMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//电容器
	if(nDeviceType == TYPE_CAP || nDeviceType == -1)
	{
		for(pos = m_CapMap.GetStartPosition(); pos != NULL; )
		{
			m_CapMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//发电机
	if(nDeviceType == TYPE_GEN || nDeviceType == -1)
	{
		for(pos = m_GenMap.GetStartPosition(); pos != NULL; )
		{
			m_GenMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//线路
	if(nDeviceType == TYPE_LINE || nDeviceType == -1)
	{
		for(pos = m_LineMap.GetStartPosition(); pos != NULL; )
		{
			m_LineMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//保护管理机
	if(nDeviceType == TYPE_PMS || nDeviceType == -1)
	{
		for(pos = m_PmsMap.GetStartPosition(); pos != NULL; )
		{
			m_PmsMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//电抗器
	if(nDeviceType == TYPE_REACTOR || nDeviceType == -1)
	{
		for(pos = m_ReactorMap.GetStartPosition(); pos != NULL; )
		{
			m_ReactorMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//刀闸
	if(nDeviceType == TYPE_SWITCH || nDeviceType == -1)
	{
		for(pos = m_SwitchMap.GetStartPosition(); pos != NULL; )
		{
			m_SwitchMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
	//变压器
	if(nDeviceType == TYPE_TRANS || nDeviceType == -1)
	{
		for(pos =  m_TransMap.GetStartPosition(); pos != NULL; )
		{
			m_TransMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
			if(pObj != NULL)
				listDevice.AddTail(pObj);
		}
	}
}

/************************************
Date:2011/7/21  Author:LYH
函数名:    GetSecList
返回值:   void
功能概要: 取得二次设备链表
参数: DEVICE_LIST & listDevice 用于保存二次设备的链表
参数: int nDeviceType 指定要查找的二次设备类型，-1和type_sec表示所有类型
************************************/
void CStationObj::GetSecList( DEVICE_LIST& listDevice,CString sPriID, int nSecType )
{
	POSITION pos = NULL;
	CString sID;
	CDeviceObj* pObj = NULL;
	for(pos = m_SecMap.GetStartPosition(); pos != NULL; )
	{
		m_SecMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj == NULL)
			continue;
		CSecObj* pSec = (CSecObj*)pObj;
		if(sPriID.IsEmpty() || sPriID == pSec->m_sOwnerID)
		{
			if(nSecType == pSec->m_nType || nSecType == -1 || nSecType == TYPE_SEC)
				listDevice.AddTail(pObj);
		}
	}
}

/****************************************************
Date:2011/12/30  Author:LYH
函数名:   GetSecList	
返回值:   void	
功能概要: 
参数: DEVICE_LIST & listDevice	
参数: int nPriType	
参数: int nSecType	
*****************************************************/
void CStationObj::GetSecList( DEVICE_LIST& listDevice, int nPriType, int nSecType )
{
	POSITION pos = NULL;
	CString sID;
	CDeviceObj* pObj = NULL;
	for(pos = m_SecMap.GetStartPosition(); pos != NULL; )
	{
		m_SecMap.GetNextAssoc(pos, sID, (CObject*&)pObj);
		if(pObj == NULL)
			continue;
		CSecObj* pSec = (CSecObj*)pObj;
		if(pSec->m_nOwnerType == nPriType || nPriType == -1)
		{
			if(nSecType == pSec->m_nType || nSecType == -1 || nSecType == TYPE_SEC)
				listDevice.AddTail(pObj);
		}
	}
}

/****************************************************
Date:2012/8/30  Author:LYH
函数名:   GetGeoPosition	
返回值:   void	
功能概要: 
参数: CString strPosition	
*****************************************************/
void CStationObj::GetGeoPosition( CString strPosition )
{
	if(strPosition == "")
		return;
	//例：1:100,200
	int nFind = -1;
	//查找:
	nFind = strPosition.Find(':', 0);
	if(nFind == -1)
		return;
	CString str = strPosition.Left(nFind);
	int nIsShow = atoi(str);
	strPosition.Delete(0, nFind+1);
	//查找,
	nFind = strPosition.Find(',', 0);
	if(nFind == -1)
		return;
	str = strPosition.Left(nFind);
	m_ptCenter.x = atoi(str);
	strPosition.Delete(0, nFind+1);
	m_ptCenter.y = atoi(strPosition);
}

//CStationObj End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBusObj 母线
IMPLEMENT_DYNCREATE(CBusObj, CDeviceObj)
//##ModelId=49B87BA700BC
CBusObj::CBusObj()
{
	m_nType = TYPE_BUS;
	m_sType = STRING_BUS;

	m_nBusType = -1;
}

//##ModelId=49B87BA700CB
CBusObj::~CBusObj()
{
	
}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA700EA
BOOL CBusObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)1); //ID
	m_sName = pMemSet->GetValue(2); //名称
	CString str;
	str = pMemSet->GetValue(3); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(5); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(6); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(7); //接线方式
	m_nBusType = atoi(str);
	str = pMemSet->GetValue(8); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(9); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(10); //厂站ID
	m_sBayID = pMemSet->GetValue(14);
	
	return TRUE;
}

/*************************************************************
 函 数 名：Save()
 功能概要：保存设备信息到数据库
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA700FA
BOOL CBusObj::Save(CDBEngine* pDBEngine)
{
	return TRUE;
}
//CBusObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CLineObj	线路
IMPLEMENT_DYNCREATE(CLineObj, CDeviceObj)
//##ModelId=49B87BA7012A
CLineObj::CLineObj()
{
	m_nType = TYPE_LINE;
	m_sType = STRING_LINE;
}

//##ModelId=49B87BA70138
CLineObj::~CLineObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA70157
BOOL CLineObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sName = pMemSet->GetValue(1); //名称
	CString str;
	str = pMemSet->GetValue(2); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(3); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(4); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(5); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(6); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(7); //厂站ID
	m_sBayID = pMemSet->GetValue(8); //间隔ID

	return TRUE;
}

//CLineObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBreakObj 开关,断路器
IMPLEMENT_DYNCREATE(CBreakObj, CDeviceObj)
//##ModelId=49B87BA70198
CBreakObj::CBreakObj()
{
	m_nType = TYPE_BREAK;
	m_sType = STRING_BREAK;

	m_nBreakType = -1;
}

//##ModelId=49B87BA701A5
CBreakObj::~CBreakObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA701C5
BOOL CBreakObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(6)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(7)); //开关类型
	m_nBreakType = atoi(str);
	str = pMemSet->GetValue(int(8)); //组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(12)); //间隔ID
	
	return TRUE;
}

//CBreakObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CGenObj	发电机
IMPLEMENT_DYNCREATE(CGenObj, CDeviceObj)
//##ModelId=49B87BA70204
CGenObj::CGenObj()
{
	m_nType = TYPE_GEN;
	m_sType = STRING_GEN;

	m_nVolume = -1;
}

//##ModelId=49B87BA70213
CGenObj::~CGenObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA70223
BOOL CGenObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(7)); //容量
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(12)); //间隔ID
	
	return TRUE;
}

//CGenObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CReactorObj	高压电抗器
IMPLEMENT_DYNCREATE(CReactorObj, CDeviceObj)
//##ModelId=49B87BA70263
CReactorObj::CReactorObj()
{
	m_nType = TYPE_REACTOR;
	m_sType = STRING_REACTOR;

	m_nVolume = -1;
}

//##ModelId=49B87BA70271
CReactorObj::~CReactorObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA70281
BOOL CReactorObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(7); //容量
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(12)); //间隔ID
	
	return TRUE;
}
//CReactorObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CTransObj		变压器
IMPLEMENT_DYNCREATE(CTransObj, CDeviceObj)
//##ModelId=49B87BA702CE
CTransObj::CTransObj()
{
	m_nType = TYPE_TRANS;
	m_sType = STRING_TRANS;

	m_nTransType = -1;
	m_nVolume = -1;
	m_nVoltageH = -1;
	m_nVoltageM = -1;
	m_nVoltageL = -1;
}

//##ModelId=49B87BA702CF
CTransObj::~CTransObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA702F1
BOOL CTransObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //容量
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(6)); //高压侧电压
	m_nVoltageH = atoi(str);
	str = pMemSet->GetValue(int(7)); //低压侧电压
	m_nVoltageL = atoi(str);
	str = pMemSet->GetValue(int(8)); //中压侧电压
	m_nVoltageM = atoi(str);
	str = pMemSet->GetValue(int(9)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(10)); //变压器类型
	m_nTransType = atoi(str);
	str = pMemSet->GetValue(int(11)); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(12)); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(13)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(15)); //间隔ID

	return TRUE;
}


//CTransObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSwitchObj	刀闸
IMPLEMENT_DYNCREATE(CSwitchObj, CDeviceObj)
//##ModelId=49B87BA7032E
CSwitchObj::CSwitchObj()
{
	m_nType = TYPE_SWITCH;
	m_sType = STRING_SWITCH;

	m_nSwitchType = -1;
}

//##ModelId=49B87BA7033C
CSwitchObj::~CSwitchObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA7034C
BOOL CSwitchObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(6)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(7)); //刀闸类型
	m_nSwitchType = atoi(str);
	str = pMemSet->GetValue(int(8)); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(12)); //间隔ID
	return TRUE;
}

//CSwitchObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CCapObj	电容器
IMPLEMENT_DYNCREATE(CCapObj, CDeviceObj)
//##ModelId=49B87BA7037C
CCapObj::CCapObj()
{
	m_nType = TYPE_CAP;
	m_sType = STRING_CAP;

	m_nVolume = -1;
}

//##ModelId=49B87BA7038A
CCapObj::~CCapObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA7039A
BOOL CCapObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue(int(1)); //ID
	m_sName = pMemSet->GetValue(int(2)); //名称
	CString str;
	str = pMemSet->GetValue(int(3)); //编号
	m_nCode = atoi(str);
	str = pMemSet->GetValue(int(5)); //运行状态
	m_nRunStatu = atoi(str);
	str = pMemSet->GetValue(int(6)); //电压等级
	m_nVoltage = atoi(str);
	str = pMemSet->GetValue(int(7)); //容量
	m_nVolume = atoi(str);
	str = pMemSet->GetValue(int(8)); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(int(9)); //103条目号
	m_n103Item = atoi(str);
	m_sStationID = pMemSet->GetValue(int(10)); //厂站ID
	m_sBayID = pMemSet->GetValue(int(12)); //间隔ID
	
	return TRUE;
}
//CCapObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CPmsObj		保护管理机
IMPLEMENT_DYNCREATE(CPmsObj, CDeviceObj)
//##ModelId=49B87BA703CA
CPmsObj::CPmsObj()
{
	m_nType = TYPE_PMS;
	m_sType = STRING_PMS;
}

//##ModelId=49B87BA703D8
CPmsObj::~CPmsObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA703D9
BOOL CPmsObj::Load(CMemSet * pMemSet)
{
	return TRUE;
}

//CPmsObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBayObj	间隔
IMPLEMENT_DYNCREATE(CBayObj, CDeviceObj)
//##ModelId=49B87BA80021
CBayObj::CBayObj()
{
	m_nType = TYPE_BAY;	
	m_sType = STRING_BAY;
	m_sSvgPath = "";
}

//##ModelId=49B87BA8002E
CBayObj::~CBayObj()
{

}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据库载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA8002F
BOOL CBayObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)1);	//ID
	m_sName = pMemSet->GetValue(2); //名称
	CString str;
	str = pMemSet->GetValue(3); //编号
	m_nCode = atoi(str); //编号
	str = pMemSet->GetValue(5); //电压等级
	m_nVoltage = atoi(str);//电压等级
	m_sStationID = pMemSet->GetValue(6); //所属厂站ID
	m_sSvgPath = pMemSet->GetValue(7); //SVG path
	
	return TRUE;
}


/*************************************************************
 函 数 名: SaveSvgPath()
 功能概要: 保存SVG路径到数据库
 返 回 值: SVG文件路径
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CBayObj::SaveSvgPath( CString strPath, CDBEngine* pDBEngine )
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//把路径的\转换成/
	strPath.Replace("\\", "/");
	
	//指定station_id,更新SVG_Path
	
	CString str;	
	//字段
	//SVG_Path
	Field Field1;
	pDBEngine->SetField(sql, Field1, "SVG_Path", EX_STTP_DATA_TYPE_STRING, strPath);
	
	//条件
	//bay_id
	Condition condition2;
	str.Format("bay_id = '%s'", m_sID);
	pDBEngine->SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pDBEngine->XJUpdateData(EX_STTP_INFO_BAY_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBayObj::SaveSvgPath, 更新失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("CBayObj::SaveSvgPath,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}
//CBayObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecObj	二次设备
IMPLEMENT_DYNCREATE(CSecObj, CDeviceObj)
//##ModelId=49B87BA800CD
CSecObj::CSecObj()
{
	m_nType = TYPE_SEC;
	m_sType = STRING_SEC;

	m_nSecType = -1;
	m_sKind = "";
	m_sModel = "";
	m_sSysVer = "";
	m_nCpuNum = -1;
//	m_tDevTime;	//设备时间(界面显示: 程序生成时间)
	m_sCrc = "";
	m_sAddr = "";
	m_nComID = -1;
	m_sOwnerID = "";
	m_nOwnerType = -1;
	m_nPTTypeID = -1;
	m_nProtocolType = -1;
	m_sPMSID = "";	
	m_n103Addr = -1;

	m_pOwner = NULL;
	m_pStation = NULL;

	m_bNoActChr = FALSE;
	m_bNoActSign = FALSE;
	m_bNoAlaSign = FALSE;

	CTime tmTemp(1971, 1, 1, 1, 1, 1);
	m_tmNewCommStatus = tmTemp;
	m_tmNewRunStatus = tmTemp;

	m_arrActChr.RemoveAll();
	m_arrActionSignal.RemoveAll();
	m_arrAlarmSignal.RemoveAll();
	m_arrCPU.RemoveAll();
	m_bNewDevice = false;
}

//##ModelId=49B87BA800DA
CSecObj::~CSecObj()
{
	ClearConfig();
	//清除CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		delete m_arrCPU[i];
	}
	m_arrCPU.RemoveAll();
}

/**************************************************************
 函 数 名：Load()
 功能概要：从数据加载入设备信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA80187
BOOL CSecObj::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sUri = pMemSet->GetValue(1); //统一资源编号
	m_sName = pMemSet->GetValue(2); //名称
	CString str;
	str = pMemSet->GetValue(3); //设备类型
	m_nSecType = atoi(str);
	m_sKind = pMemSet->GetValue(4); //保护种类
	m_sModel = pMemSet->GetValue(5); //设备型号
	m_sSysVer = pMemSet->GetValue(6); //设备版本号
	str = pMemSet->GetValue(7); //CPU个数
	m_nCpuNum = atoi(str); 
	m_sDevTime = pMemSet->GetValue(8);//设备时间
	m_sCrc = pMemSet->GetValue(9); //设备校检码
	m_sAddr = pMemSet->GetValue(10); //设备地址
	str = pMemSet->GetValue(11); //网口号
	m_nComID = atoi(str);
	str = pMemSet->GetValue(12); //运行状态
	m_nRunStatu = atoi(str);
	m_sStationID = pMemSet->GetValue(13); //厂站ID
	m_sOwnerID = pMemSet->GetValue(14); //一次设备ID
	str = pMemSet->GetValue(15); //一次设备类型
	m_nOwnerType = atoi(str);
	str = pMemSet->GetValue(16); //二次设备类型ID
	m_nPTTypeID = atoi(str);
	str = pMemSet->GetValue(17); //规约类型
	m_nProtocolType = atoi(str);
	str = pMemSet->GetValue(18);	//103地址
	m_n103Addr = atoi(str);
	str = pMemSet->GetValue(19); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(20); //103条目号
	m_n103Item = atoi(str);
	str = pMemSet->GetValue(21); //通讯状态
	m_nCommStatus = atoi(str);
	str = pMemSet->GetValue(22); //当前通信状态改变的时间
	m_tmNewCommStatus = StringToTime(str);
	str = pMemSet->GetValue(23); //当前运行状态改变的时间
	m_tmNewRunStatus = StringToTime(str);

	//确定设备类型
	switch(m_nSecType)
	{
	case 1: //母差保护
	case 2: //变压器保护
	case 3: //线路保护
	case 4: //发变组保护
	case 5: //断路器保护
	case 6: //电抗器保护
	case 8: //电容器保护
	case 11: //母联保护
		m_nType = TYPE_PROTECT;
		break;
	case 9:
		m_nType = TYPE_WAVEREC;
		break;
	default:
		m_nType = 15;
		break;
	}

	//读取CPU信息
//	GetCPUInfo();
	//设置通讯状态
	SetCommStatus(m_nCommStatus);
//	RefreshCommDurativeTime();
	//设置运行状态
	SetRunStatus(m_nRunStatu);
//	RefreshRunDurativeTime();
	//与一次设备关联放在DataEngine里做
	//配置信息和通讯状态放在另外的线程进行读取操作
	
	return TRUE;
}


//##ModelId=49B87BA801D6
BOOL CSecObj::ClearConfig(int nAim /* = 0 */)
{
	int i = 0;
	try
	{
		if(nAim == 0 || nAim == 1)
		{
			//清除特征量
			for(i=0;i<m_arrActChr.GetSize();i++)
			{		
				delete m_arrActChr[i];
			}
			m_arrActChr.RemoveAll();
		}
		
		if(nAim == 0 || nAim == 2)
		{
			//清除动作信号量
			for(i=0;i<m_arrActionSignal.GetSize();i++)
			{
				delete m_arrActionSignal[i];
			}
			m_arrActionSignal.RemoveAll();
		}
		
		if(nAim == 0 || nAim == 3)
		{
			for(i=0;i<m_arrAlarmSignal.GetSize();i++)
			{
				delete m_arrAlarmSignal[i];
			}
			m_arrAlarmSignal.RemoveAll();
		}
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：RefreshConfig()
 功能概要：刷新保护配置
 返 回 值: 成功返回TRUE, 失败返回FALSES
**************************************************************/
//##ModelId=49B87BA80198
BOOL CSecObj::RefreshConfig(CDBEngine* pDBEngine, BOOL bRefresh)
{
	if(bRefresh == FALSE)
	{
		//不强制刷新, 检查是否有配置存在
		//如果没有数据且数据库中无数据的标志为FALSE(表示程序认为数据库中是有数据的)的话,
		//说明数据库中有数据但还没读上来, 强制刷新一次
		if(m_arrActChr.GetSize() <= 0 && !m_bNoActChr)
		{
			GetActionChr(pDBEngine);
		}
		if(m_arrActionSignal.GetSize() <= 0 && !m_bNoActSign)
		{	
			GetActionSignal(pDBEngine);
		}
		if(m_arrAlarmSignal.GetSize() <= 0 && !m_bNoAlaSign)
		{
			GetAlarmSignal(pDBEngine);
		}
	}
	else
	{
		//强制刷新所有
		GetActionChr(pDBEngine);
		GetActionSignal(pDBEngine);
		GetAlarmSignal(pDBEngine);
	}
	return TRUE;
}

/*************************************************************
 函 数 名：GetActionChr()
 功能概要：从数据库读取保护动作特征值,tb_pt_character_def
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA801C9
BOOL CSecObj::GetActionChr(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
		
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//char_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "char_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);
	//Unit
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "Unit";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field5);
	//Reserve1(事件类型)
	Field Field6;
	bzero(&Field6, sizeof(Field));
	str = "Reserve1";
	strncpy(Field6.FieldName, str, str.GetLength());
	Field6.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field6);
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(EX_STTP_INFO_PT_CHARCTOR_DEF_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecObj::GetActionChr, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetActionChr, 读取到%d条数据", nCount);
			WriteLog(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoActChr = TRUE;
			CString str;
			str.Format("ID为%s的保护没有指定动作特征量信息", m_sID);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			PT_ActionCharacter * sign = new PT_ActionCharacter;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //action_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			sign->strUnits = pMemset->GetValue(4); //Unit
			str = pMemset->GetValue(5);//事件类型
			sign->nEventType = atoi(str);

			//单位去空格
			sign->strUnits.TrimLeft();
			sign->strUnits.TrimRight();

			m_arrActChr.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetActionChr,查询失败,原因为%s", sError);
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
 函 数 名：GetActionSignal()
 功能概要：从数据库读取保护动作信号量,tb_pt_action_def
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA801D4
BOOL CSecObj::GetActionSignal(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//action_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "action_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);

	//property
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "property";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field5);
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(EX_STTP_INFO_PT_ACTION_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecObj::GetActionSignal, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetActionSignal, 读取到%d条数据", nCount);
			WriteLog(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoActSign = TRUE;
			CString str;
			str.Format("ID为%s的保护没有动作信号量信息", m_sID);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			PT_Signal * sign = new PT_Signal;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //action_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			str = pMemset->GetValue(4); //property
			sign->nEventType = atoi(str);

			//加入列表
			sign->strPT_ID     = m_sID;
			sign->strPTName    = m_sName;	        //保护名称	
			sign->strPT_Model  = m_sModel;			//保护型号
			sign->strStationId = m_sStationID;		//保护所在厂站ID

			m_arrActionSignal.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetActionSignal,查询失败,原因为%s", sError);
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
 函 数 名：GetAlarmSignal()
 功能概要：从数据库读取保护告警信号量,tb_pt_alarm_def
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA801D5
BOOL CSecObj::GetAlarmSignal(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
		
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "cpu_code";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//alarm_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "alarm_id";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);

	//name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "name";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//code_name
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "code_name";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field4);

	//property
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "property";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field5);
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(EX_STTP_INFO_PT_ALARM_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecObj::GetAlarmSignal, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			CString str;
			str.Format("CSecObj::GetAlarmSignal, 读取到%d条数据", nCount);
			WriteLog(str, 3);
		}
		if(nCount == 0)
		{
			m_bNoAlaSign = TRUE;
			CString str;
			str.Format("ID为%s的保护没有告警信号量信息", m_sID);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			PT_Signal * sign = new PT_Signal;
			sign->strPT_ID = m_sID;
			CString str;
			str = pMemset->GetValue((UINT)0); //cpu_code
			sign->nCPU = atoi(str);
			str = pMemset->GetValue(1); //alarm_id
			sign->nID = atoi(str);
			sign->strName = pMemset->GetValue(2); //name
			sign->strCodeName = pMemset->GetValue(3); //code_name
			str = pMemset->GetValue(4); //property
			sign->nEventType = atoi(str);

			//加入列表
			sign->strPT_ID     = m_sID;
			sign->strPTName    = m_sName;	        //保护名称	
			sign->strPT_Model  = m_sModel;			//保护型号
			sign->strStationId = m_sStationID;		//保护所在厂站ID
		//	sign->strStation   = m_sSTATIONName;	//保护所在的厂站名称

			m_arrAlarmSignal.Add(sign);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetAlarmSignal,查询失败,原因为%s", sError);
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
 函 数 名：GetCPUInfo()
 功能概要：从数据库读取CPU信息,tb_secdev_cpu_base
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
//##ModelId=49B87BA801E5
BOOL CSecObj::GetCPUInfo(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(EX_STTP_INFO_PT_CPU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecObj::GetCPUInfo, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			CString str;
			str.Format("ID为%s的保护没有CPU信息", m_sID);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			CSecCPU * cpu = new CSecCPU;
			if(cpu->Load(pMemset))
			{
				//读取成功
				m_arrCPU.Add(cpu);
			}
			else
			{
				delete cpu;
				cpu = NULL;
			}

			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CSecObj::GetCPUInfo,查询失败,原因为%s", sError);
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
 函 数 名：SetCommStatus()
 功能概要：改变装置通讯状态
 返 回 值: void
 参    数：param1	指定通讯状态.0-正常, 1-断开
		   Param2
**************************************************************/
//##ModelId=49B87BA801A6
void CSecObj::SetCommStatus( int nStatus )
{
	//判断数据有效性
	if(nStatus<0 || nStatus>1)
		return;
	m_nCommStatus = nStatus;
}

/*************************************************************
 函 数 名：SetRunStatus()
 功能概要：改变状态运行状态
 返 回 值: void
 参    数：param1	指定运行状态 0:运行 1：检修/挂牌 2：停用 3：调试  4: 对码表
**************************************************************/
//##ModelId=49B87BA801A8
void CSecObj::SetRunStatus( int nStatus )
{
	//判断数据有效性
	if(nStatus<0 || nStatus>4)
		return;
	m_nRunStatu = nStatus;
}

/*************************************************************
 函 数 名：AddZone()
 功能概要：加入定值区号
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1	定值区号结构
		   Param2
**************************************************************/
//##ModelId=49B87BA801B6
BOOL CSecObj::AddZone( PT_ZONE* ptz )
{
	//检查数据有效性
	if(ptz == NULL)
		return FALSE;
	//查找对应的CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pCpu = (CSecCPU*)m_arrCPU.GetAt(i);
		if(pCpu->code == ptz->cpu)
		{
			//加入
			pCpu->m_arrZONE.Add(ptz);
			pCpu->m_bUseSpeZone = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

/*************************************************************
 函 数 名：RefreshCommDurativeTime()
 功能概要：刷新通信状态持续时间
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA801B8
CTimeSpan CSecObj::GetCommDurativeTime()
{
	//取得当前时间
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewCommStatus;

	if (tmSpan < 0)
	{
		tmSpan = 0;
		CString str;
		str.Format("[%s]通信状态持续时间出现负数,状态变化时间为%s", m_sID, m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}
	
	return tmSpan;
}

/*************************************************************
 函 数 名：RefreshRunDurativeTime()
 功能概要：刷新运行状态持续时间
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA801B9
CTimeSpan CSecObj::GetRunDurativeTime()
{
	//取得当前时间
	CTime t = CTime::GetCurrentTime();
	CTimeSpan tmSpan;
	tmSpan = t - m_tmNewRunStatus;
	
	if (tmSpan < 0)
	{
		tmSpan = 0;
		CString str;
		str.Format("[%s]运行状态持续时间出现负数,状态变化时间为%s", m_sID, m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		WriteLog(str);
	}

	return tmSpan;
}

/*************************************************************
 函 数 名：LoadCommStatusFromSTTP()
 功能概要：从STTP报文中读取通信状态信息
 返 回 值: 
 参    数：param1	STTP报文数据
		   Param2	索引
**************************************************************/
//##ModelId=49B87BA801BA
void CSecObj::LoadCommStatusFromSTTP( STTP_FULL_DATA& sttpData, int nIndex )
{
	//先判断报文ID
	int nMsgID = sttpData.sttp_head.uMsgID;
	if(nMsgID != 20002 && nMsgID != 20084)
	{
		CString str;
		str.Format("CSecObj::LoadCommStatusFromSTTP, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//判断索引
	int nCount = sttpData.sttp_body.variant_member.value_data.size();
	if(nIndex >= nCount || nIndex < 0)
	{
		//非法索引
		return;
	}
	
	//状态
	int nStatus = -1;
	if(nMsgID == 20002)
		nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].id;
	else if(nMsgID == 20084)
		nStatus = sttpData.sttp_body.variant_member.value_data[nIndex].int_reserved;
	
	//改变状态
	SetCommStatus(nStatus);

	//时间
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmNewCommStatus = StringToTimeSttp12(strTime);

	//刷新状态持续时间
	GetCommDurativeTime();
	
}

/*************************************************************
 函 数 名：LoadRunStatusFromSTTP()
 功能概要：从STTP报文中读取运行状态信息
 返 回 值: 
 参    数：param1	STTP报文数据
		   Param2	索引
**************************************************************/
//##ModelId=49B87BA801C6
void CSecObj::LoadRunStatusFromSTTP( STTP_FULL_DATA& sttpData, int nIndex )
{
	//先判断报文ID
	if(sttpData.sttp_head.uMsgID != 20079)
	{
		CString str;
		str.Format("CSecObj::LoadRunStatusFromSTTP, 收到错误报文, 报文为%d", sttpData.sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//状态
	int nStatus = sttpData.sttp_body.nStatus;

	//改变状态
	SetRunStatus(nStatus);
	
	//时间
	CString strTime = sttpData.sttp_body.ch_time_12_BIT1;
	m_tmNewRunStatus = StringToTimeSttp12(strTime);

	//刷新运行状态持续时间
	GetRunDurativeTime();
}

/*************************************************************
 函 数 名: RefreshStatus()
 功能概要: 改变本设备的状态为指定设备的状态
 返 回 值: 
 参    数: param1 指定设备
		   Param2 
**************************************************************/
void CSecObj::RefreshStatus( CSecObj* pSec )
{
	if(pSec == NULL)
		return;
	//通讯状态
	SetCommStatus(pSec->m_nCommStatus);
	m_tmNewCommStatus = pSec->m_tmNewCommStatus;
	GetCommDurativeTime();
	//运行状态
	SetRunStatus(pSec->m_nRunStatu);
	m_tmNewRunStatus = pSec->m_tmNewRunStatus;
	GetRunDurativeTime();
}

/*************************************************************
 函 数 名: LoadSimple()
 功能概要: 载入设备信息,只做简单的载入,不做其它操作
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数: param1 数据库数据
		   Param2 
**************************************************************/
BOOL CSecObj::LoadSimple( CMemSet* pMemSet )
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	m_sID = pMemSet->GetValue((UINT)0); //ID
	m_sName = pMemSet->GetValue(2); //名称
	CString str;
	str = pMemSet->GetValue(3); //设备类型
	m_nSecType = atoi(str);
	m_sKind = pMemSet->GetValue(4); //保护种类
	m_sModel = pMemSet->GetValue(8); //设备型号
	m_sSysVer = pMemSet->GetValue(9); //设备版本号
	str = pMemSet->GetValue(10); //CPU个数
	m_nCpuNum = atoi(str); 
	m_sDevTime = pMemSet->GetValue(11);//设备时间
	m_sCrc = pMemSet->GetValue(12); //设备校检码
	m_sAddr = pMemSet->GetValue(13); //设备地址
	str = pMemSet->GetValue(14); //网口号
	m_nComID = atoi(str);
	str = pMemSet->GetValue(19); //运行状态
	m_nRunStatu = atoi(str);
	m_sStationID = pMemSet->GetValue(20); //厂站ID
	m_sOwnerID = pMemSet->GetValue(21); //一次设备ID
	str = pMemSet->GetValue(22); //一次设备类型
	m_nOwnerType = atoi(str);
	str = pMemSet->GetValue(23); //二次设备类型ID
	m_nPTTypeID = atoi(str);
	str = pMemSet->GetValue(24); //规约类型
	m_nProtocolType = atoi(str);
	str = pMemSet->GetValue(26);	//103地址
	m_n103Addr = atoi(str);
	str = pMemSet->GetValue(27); //103组号
	m_n103Group = atoi(str);
	str = pMemSet->GetValue(28); //103条目号
	m_n103Item = atoi(str);
	str = pMemSet->GetValue(36); //通讯状态
	m_nCommStatus = atoi(str);
	str = pMemSet->GetValue(37); //当前通信状态改变的时间
	m_tmNewCommStatus = StringToTime(str);
	str = pMemSet->GetValue(38); //当前运行状态改变的时间
	m_tmNewRunStatus = StringToTime(str);
	
	//确定设备类型
	switch(m_nSecType)
	{
	case 1: //母差保护
	case 2: //变压器保护
	case 3: //线路保护
	case 4: //发变组保护
	case 5: //断路器保护
	case 6: //电抗器保护
	case 8: //电容器保护
	case 11: //母联保护
		m_nType = TYPE_PROTECT;
		break;
	case 9:
		m_nType = TYPE_WAVEREC;
		break;
	default:
		break;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名: FindCpu()
 功能概要: 指定CPU号查找CPU信息
 返 回 值: CPU信息
 参    数: param1 指定CPU号
		   Param2 
**************************************************************/
CSecCPU* CSecObj::FindCpu( int nCpu )
{
	CSecCPU* cpu = NULL;
	//查找对应的CPU
	for(int i = 0; i < m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pCpu = (CSecCPU*)m_arrCPU.GetAt(i);
		if(pCpu->code == nCpu)
		{
			cpu = pCpu;
			break;
		}
	}
	return cpu;
}
//CSecObj	end
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSecCPU, CDeviceObj)
//////////////////////////////////////////////////////////////////////////
//CSecCPU	CPU类
//##ModelId=49B87BA80215
CSecCPU::CSecCPU()
{
	PT_ID = "";
	code = -1;
	des = "";
	ver = "";
	from_zone = -1;
	to_zone = -1;
	fun = -1;
	m_bUseSpeZone = FALSE;
	m_nType = TYPE_CPU;
	m_arrZONE.RemoveAll();
	/** @brief           本CPU下定值的配置数*/
	m_nSettingDefs = 0;
	/** @brief           本CPU下开关量的配置数*/
	m_nDigitalDefs = 0;
	/** @brief           本CPU下软压板的配置数*/
	m_nSoftBoardDefs = 0;
	/** @brief           本CPU下模拟量的配置数*/
	m_nAnalogDefs = 0;
}

//##ModelId=49B87BA80222
CSecCPU::~CSecCPU()
{
	//清空ZONE链表
	ClearZone();
}

/*************************************************************
 函 数 名：ClearZone()
 功能概要：清空定值区号链表
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA80242
BOOL CSecCPU::ClearZone()
{
	for(int i = 0; i < m_arrZONE.GetSize(); i++)
	{
		delete m_arrZONE[i];
	}
	m_arrZONE.RemoveAll();
	
	return TRUE;
}

/*************************************************************
 函 数 名：Load()
 功能概要：从数据库中读入数据, tb_secdev_cpu_base
 返 回 值: 读入成功返回TRUE, 失败返回FALSE
 参    数：param1   数据集指针
**************************************************************/
//##ModelId=49B87BA80238
BOOL CSecCPU::Load(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	if(pMemSet == NULL)
		return FALSE;
	try
	{
		PT_ID = pMemSet->GetValue((UINT)0);
		CString str;
		str = pMemSet->GetValue(1); //cpu_code
		code = atoi(str);
		des = pMemSet->GetValue(2); //cpu_des
		ver = pMemSet->GetValue(3); //cpu_ver
		str = pMemSet->GetValue(4); //from_zone
		from_zone = atoi(str);
		str = pMemSet->GetValue(5); //to_zone
		to_zone = atoi(str);
		str = pMemSet->GetValue(6); //fun
		fun = atoi(str);
	}
	catch (CException* e)
	{
		e->Delete();
		return FALSE;
	}
// 	if(!GetZone())
// 	{
// 		//读取定值区号失败或没有指定的定值区号
// 		m_bUseSpeZone = FALSE;
// 	}
// 	else
// 	{
// 		m_bUseSpeZone = TRUE;
// 	}
	return TRUE;
}

/*************************************************************
 函 数 名：GetZone()
 功能概要：从数据库查找对应的定值区号
 返 回 值: 查找成功返回TRUE, 失败或无数据返回FALSE
**************************************************************/
//##ModelId=49B87BA8023A
BOOL CSecCPU::GetZone(CDBEngine* pDBEngine)
{
	MYASSERT_BOOL(pDBEngine);

	BOOL bReturn = TRUE;
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//cpu_code
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "Zone_CODE";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//char_id
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "Zone_State";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", PT_ID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_Code = %d", code);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pDBEngine->XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CSecCPU::GetZone, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			bReturn = FALSE;
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建
			PT_ZONE * sign = new PT_ZONE;
			CString str;
			str = pMemset->GetValue((UINT)0); //zone_code
			sign->code = atoi(str);
			str = pMemset->GetValue(1); //zone_state
			sign->state = atoi(str);

			m_arrZONE.Add(sign);

			pMemset->MoveNext();
		}
	}
	else
	{
				
		CString str;
		str.Format("CSecCPU::GetZone,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

//SecCPU end
//////////////////////////////////////////////////////////////////////////
