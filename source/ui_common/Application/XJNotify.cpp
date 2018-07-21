#include "XJNotify.h"

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   CXJNotify	
返回值:   	
功能概要: 
参数: CDataEngine * pDataEngine	
*****************************************************/
CXJNotify::CXJNotify( CDataEngine* pDataEngine )
{
	m_pDataEngine = pDataEngine;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   ~CXJNotify	
返回值:   	
功能概要: 
*****************************************************/
CXJNotify::~CXJNotify()
{

}

/*************************************************************
 函 数 名：GetDescribe()
 功能概要：得到事件详细描述,各子类实现
 返 回 值: 事件描述
**************************************************************/
CString CXJNotify::GetDescribe()
{
	CString str("这里是事件描述");
	return str;
}

/*************************************************************
 函 数 名：LoadFromDB()
 功能概要：读取数据库中的一条记录, 并写入到本事件对象中,
			调用此函数之前的查询需查询所字段,即用select * from table where XXX
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CXJNotify::LoadFromDB(CMemSet * pMemSet)
{
	//使用此函数需保证两个前提
	//1. 查询时用的是select *查询了所有字段
	//2. Memset的移动在函数外进行, 函数内部假设MemSet已经在正确的位置
	return TRUE;
}

/*************************************************************
 函 数 名：LoadFromSttp()
 功能概要：读取一条STTP报文, 并写入到本事件对象中
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数：param1	STTP报文数据结构
		   Param2	指定是报文数据中的第几个数据项
**************************************************************/
BOOL CXJNotify::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//子类中各自实现
	return TRUE;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   Clone	
返回值:   CXJNotify*	
功能概要: 
*****************************************************/
CXJNotify* CXJNotify::Clone()
{
	return NULL;
}

/*************************************************************
 函 数 名：GetFaultType()
 功能概要：将strFaultType解释成文字
 返 回 值: 解释后的文字
**************************************************************/
CString CXJNotify::GetFaultType(CString sFaultType)
{
	CString strTemp;
	sFaultType.MakeUpper();
	if ((sFaultType == "AG") || (sFaultType == "AN") || (sFaultType == "A"))
	{
		strTemp = "A相接地";
	}
	else if ((sFaultType == "BG") || (sFaultType == "BN") || (sFaultType == "B"))
	{
		strTemp = "B相接地";
	}
	else if ((sFaultType == "CG") || (sFaultType == "CN") || (sFaultType == "C"))
	{
		strTemp = "C相接地";
	}
	else if ((sFaultType == "AB") || (sFaultType == "BA"))
	{
		strTemp = "AB两相短路";
	}
	else if ((sFaultType == "BC") || (sFaultType == "CB"))
	{
		strTemp = "BC两相短路";
	}
	else if ((sFaultType == "CA") || (sFaultType == "AC"))
	{
		strTemp = "CA两相短路";
	}
	else if ((sFaultType == "ABG") || (sFaultType == "ABN") || (sFaultType == "BAG") || (sFaultType == "BAN"))
	{
		strTemp = "AB两相短路接地";
	}
	else if ((sFaultType == "BCG") || (sFaultType == "BCN") || (sFaultType == "CBG") || (sFaultType == "CBN"))
	{
		sFaultType = "BC两相短路接地";
	}
	else if ((sFaultType == "CAG") || (sFaultType == "CAN") || (sFaultType == "ACG") || (sFaultType == "ACN"))
	{
		strTemp = "CA两相短路接地";
	}
	else if (sFaultType == "ABC")
	{
		strTemp = "ABC三相短路";
	}
	else if ((sFaultType == "ABCG") || (sFaultType == "ABCN"))
	{
		strTemp = "ABC三相接地";
	}	
	else
	{
		strTemp = "未知故障";
		strTemp += sFaultType;
	}

	return strTemp;
}

/*************************************************************
 函 数 名：GetTriggerType()
 功能概要：得到相别说明
 返 回 值: 相别说明
 参    数：param1	相别简写
		   Param2
**************************************************************/
CString CXJNotify::GetTriggerType( CString sTrigger )
{
	if(sTrigger == "")
		return "";
	CString sReturn;
	sReturn = sTrigger;
	if(sTrigger.GetLength() == 1)
	{
		sReturn += "相";
	}
	else if(sTrigger.GetLength() == 2)
	{
		sReturn += "两相";
	}
	else if(sTrigger.GetLength() == 3)
	{
		sReturn += "三相";
	}
	return sReturn;
}

/****************************************************
Date:2011/10/19  Author:LYH
函数名:   BuildReleation	
返回值:   BOOL	成功返回TRUE, 失败返回FALSE
功能概要: 建立与设备的关联
*****************************************************/
BOOL CXJNotify::BuildReleation()
{
	return FALSE;
}

/****************************************************
Date:2011/10/31  Author:LYH
函数名:   GetNotifyType	
返回值:   int	
功能概要: 
*****************************************************/
int CXJNotify::GetNotifyType()
{
	return -1;
}

/****************************************************
Date:2011/11/8  Author:LYH
函数名:   GetSimpleDescribe	
返回值:   CString	
功能概要: 
*****************************************************/
CString CXJNotify::GetSimpleDescribe()
{
	return "简要描述";
}
