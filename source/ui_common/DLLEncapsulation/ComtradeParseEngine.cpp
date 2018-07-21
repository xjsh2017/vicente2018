// PEngine.cpp : implementation file
//
#include "ComtradeParseEngine.h"

////////////////////////////////////////////////////////////////
// CPEngine

ComtradeParseEngine::ComtradeParseEngine()
{
	m_hModule = NULL;
	m_pLoadFile = NULL;
	m_pCloseFile = NULL;
	m_pGetCFGAttr = NULL;
	m_pGetRatesCount = NULL;
	m_pGetRateSample = NULL;
	m_pGetRate = NULL;
	m_pGetChannelCFG = NULL;
	m_pGetChannelType = NULL;
	m_pTimeRelativeTrigger = NULL;
	m_pTimeRelativeStart = NULL;
	m_pGetRealValue = NULL;
	m_pGetTotalSamples = NULL;
	m_pGetTriggerTime = NULL;
	m_pGetHdrInfoJSON = NULL;
	m_pGetHdrInfoObject = NULL;
}

ComtradeParseEngine::~ComtradeParseEngine()
{
	if(m_hModule != NULL)
	{
		xj_free_library(m_hModule);
		m_hModule = NULL;
		m_pLoadFile = NULL;
		m_pCloseFile = NULL;
		m_pGetCFGAttr = NULL;
		m_pGetRatesCount = NULL;
		m_pGetRateSample = NULL;
		m_pGetRate = NULL;
		m_pGetChannelCFG = NULL;
		m_pGetChannelType = NULL;
		m_pTimeRelativeTrigger = NULL;
		m_pTimeRelativeStart = NULL;
		m_pGetRealValue = NULL;
		m_pGetTotalSamples = NULL;
		m_pGetTriggerTime = NULL;
		m_pGetHdrInfoJSON = NULL;
		m_pGetHdrInfoObject = NULL;
	}
}

/************************************
函 数 名: LoadCPLib
功能概要: 装载XJComtrade.dll 
返 回 值: 装载成功返回TRUE,失败返回FALSE
参    数: param1 
		  param2 
************************************/
bool ComtradeParseEngine::xjcp_LoadCPLib()
{
	m_hModule = xj_load_library(LIBNAME_COMTRADEPARSE);
	if(m_hModule != NULL)
	{
		m_pLoadFile = (int(*)(char*,char*, XJHANDLE&))xj_get_addr(m_hModule, "LoadFile"); //打开文件
		m_pCloseFile = (void (*)(XJHANDLE))xj_get_addr(m_hModule, "CloseFile");	//关闭文件
		m_pGetCFGAttr = (void (*)(XJHANDLE,ComtradeCFG&))xj_get_addr(m_hModule, "GetCFGAttr"); //文件记录的波形数据配置属性
		m_pGetRatesCount = (int (*)(XJHANDLE))xj_get_addr(m_hModule, "GetRatesCount");
		m_pGetRateSample = (void (*)(XJHANDLE,int,float&,int&))xj_get_addr(m_hModule, "GetRateSample");
		m_pGetRate = (float (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetRate");
		m_pGetChannelCFG = (int (*)(XJHANDLE,int,ComtradeChannel&))xj_get_addr(m_hModule, "GetChannelCFG");
		m_pGetChannelType = (int (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetChannelType");
		m_pTimeRelativeTrigger = (int (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetTimeRelativeTrigger");
		m_pTimeRelativeStart = (int (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetTimeRelativeStart");
		m_pGetRealValue = (float (*)(XJHANDLE,int,int,int))xj_get_addr(m_hModule, "GetRealValue");
		m_pGetTotalSamples = (int (*)(XJHANDLE))xj_get_addr(m_hModule, "GetTotalSamples");
		m_pGetTriggerTime = (int (*)(XJHANDLE))xj_get_addr(m_hModule, "GetTriggerTime");
		m_pGetTotalTime = (int (*)(XJHANDLE))xj_get_addr(m_hModule, "GetTotalTime");
		m_pGetMaxValue = (float (*)(XJHANDLE,int,int,int))xj_get_addr(m_hModule, "GetMaxValue");
		m_pGetRecordValue = (int (*)(XJHANDLE,int,int))xj_get_addr(m_hModule, "GetRecordValue");
		m_pSetCTPT = (void (*)(XJHANDLE,float,float))xj_get_addr(m_hModule, "SetCTPT");
		m_pGetCTPT = (void (*)(XJHANDLE,float&,float&))xj_get_addr(m_hModule, "GetCTPT");
		m_pGetTimeRelativeTriggerMSEC = (double (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetTimeRelativeTriggerMSEC");
		m_pGetTimeRelativeStartMSEC = (double (*)(XJHANDLE,int))xj_get_addr(m_hModule, "GetTimeRelativeStartMSEC");
		m_pGetHdrInfoObject = (HdrObj* (*)(XJHANDLE))xj_get_addr(m_hModule, "GetHdrInfoObject");
		m_pGetHdrInfoJSON = (std::string (*)(XJHANDLE))xj_get_addr(m_hModule, "GetHdrInfoJSON");
	}
	else
	{
		return false;
	}
	return true;
}

/************************************
函 数 名: XJLoadFile
功能概要: 载入波形文件
返 回 值: 载入成功返回>=0, 失败返回<0
参    数: param1 波形文件路径
		  param2 错误信息
		  param3 生成的波形文件数据对象指针
************************************/
int ComtradeParseEngine::xjcp_LoadFile( char* sFilePath, char* sError, XJHANDLE& hComtrade )
{
	if(m_hModule == NULL)
		return -1;
 	if(m_pLoadFile == NULL)
 		return -1;
 	char cError[1024]= "";
 	int nResult = m_pLoadFile(sFilePath, cError, hComtrade);
 	sError = cError;
 	return nResult;
}

/************************************
函 数 名: XJCloseFile
功能概要: 关闭文件
返 回 值: 
参    数: param1 指定波形文件数据对象指针
		  param2 
************************************/
void ComtradeParseEngine::xjcp_CloseFile( XJHANDLE hComtrade )
{
	if(m_hModule == NULL)
		return;
	if(m_pCloseFile == NULL)
		return;
	m_pCloseFile(hComtrade);
}

/************************************
函 数 名: XJGetCFGAttr
功能概要: 取得CFG文件记录的波形数据配置属性
返 回 值: 
参    数: param1 指定波形文件数据对象指针
		  param2 波形数据配置属性
************************************/
void ComtradeParseEngine::xjcp_GetCFGAttr( XJHANDLE hComtrade, ComtradeCFG& cfgAttr )
{
	if(m_hModule == NULL)
		return;
	if(m_pGetCFGAttr == NULL)
		return;
	m_pGetCFGAttr(hComtrade, cfgAttr);
}

/************************************
函 数 名: XJGetRatesCount
功能概要: 取得采样频率数
返 回 值: 采样频率数
参    数: param1 指定波形文件数据对象指针
		  param2 
************************************/
int ComtradeParseEngine::xjcp_GetRatesCount( XJHANDLE hComtrade )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetRatesCount == NULL)
		return 0;
	return m_pGetRatesCount(hComtrade);
}

/************************************
函 数 名: XJGetRateSample
功能概要: 取得指定采样频率索引的采样频率和采样点数
返 回 值: 
参    数: param1 指定波形文件数据对象指针
		  param2 指定采样频率索引.索引从0开始
		  param3 采样频率
		  param4 采样频率下的采样点个数
************************************/
void ComtradeParseEngine::xjcp_GetRateSample( XJHANDLE hComtrade, int nRateIndex, float& fSampleRate, int& nSampelCount )
{
	if(m_hModule == NULL)
		return;
	if(m_pGetRateSample == NULL)
		return;
	m_pGetRateSample(hComtrade, nRateIndex, fSampleRate, nSampelCount);
}

/************************************
函 数 名: XJGetRate
功能概要: 取得指定采样点的采样频率
返 回 值: 采样频率
参    数: param1 指定波形文件数据对象指针
		  param2 指定采样点
************************************/
float ComtradeParseEngine::xjcp_GetRate( XJHANDLE hComtrade, int nSamplePos )
{
	if(m_hModule == NULL)
		return 0.0;
	if(m_pGetRate == NULL)
		return 0.0;
	return m_pGetRate(hComtrade, nSamplePos);
}

/************************************
函 数 名: XJGetChannelCFG
功能概要: 取得指定通道的配置信息
返 回 值: 通道类型.1表示模拟量通道，0表示开关量通道
参    数: param1 指定波形文件数据对象指针
		  param2 指定通道索引.索引从0开始
		  param3 通道配置信息结构体
************************************/
int ComtradeParseEngine::xjcp_GetChannelCFG( XJHANDLE hComtrade, int nCn, ComtradeChannel& chl )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetChannelCFG == NULL)
		return 0;
	return m_pGetChannelCFG(hComtrade, nCn, chl);
}

/************************************
函 数 名: XJGetChannelType
功能概要: 取得指定通道的类型
返 回 值: 通道类型.1表示模拟量通道，0表示开关量通道
参    数: param1 指定波形文件数据对象指针
		  param2 指定通道索引.索引从0开始
************************************/
int ComtradeParseEngine::xjcp_GetChannelType( XJHANDLE hComtrade, int nCn )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetChannelType == NULL)
		return 0;
	return m_pGetChannelType(hComtrade, nCn);
}

/************************************
函 数 名: XJGetTimeRelativeTrigger
功能概要: 取得指定采样点相对于故障触发时刻的时间
返 回 值: 相对于故障触发时刻的时间（毫秒）
参    数: param1 指定波形文件数据对象指针
		  param2 指定采样点索引.索引从0开始
************************************/
int ComtradeParseEngine::xjcp_GetTimeRelativeTrigger( XJHANDLE hComtrade, int nSampleNo )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pTimeRelativeTrigger == NULL)
		return 0;
	return m_pTimeRelativeTrigger(hComtrade, nSampleNo);
}

/************************************
函 数 名: XJGetTimeRelativeStart
功能概要: 取得指定采样点相对于采样开始时刻的时间
返 回 值: 相对于采样开始时刻的时间（毫秒）
参    数: param1 指定波形文件数据对象指针
		  param2 指定采样点索引.索引从0开始
************************************/
int ComtradeParseEngine::xjcp_GetTimeRelativeStart( XJHANDLE hComtrade, int nSampleNo )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pTimeRelativeStart == NULL)
		return 0;
	return m_pTimeRelativeStart(hComtrade, nSampleNo);
}

/************************************
函 数 名: XJGetRealValue
功能概要: 取得指定通道指定采样点的真实值
返 回 值: 指定通道指定采样点的真实值
参    数: param1 指定波形文件数据对象指针
		  param2 指定通道索引.索引从0开始
		  param3 指定采样点索引.索引从0开始
		  param4 指定需要的值的类型，默认为二次值.0为二次值，1为一次值
************************************/
float ComtradeParseEngine::xjcp_GetRealValue( XJHANDLE hComtrade, int nCn, int nSampleNo, int nDataType/* = 0*/ )
{
	if(m_hModule == NULL)
		return 0.0;
	if(m_pGetRealValue == NULL)
		return 0.0;
	return m_pGetRealValue(hComtrade, nCn, nSampleNo, nDataType);
}

/************************************
函 数 名: XJGetTotalSamples
功能概要: 取得采样点总数
返 回 值: 采样点总数
参    数: param1 指定波形文件数据对象指针
		  param2 
************************************/
int ComtradeParseEngine::xjcp_GetTotalSamples( XJHANDLE hComtrade )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTotalSamples == NULL)
		return 0;
	return m_pGetTotalSamples(hComtrade);
}

/************************************
函 数 名: XJGetTriggerTime
功能概要: 取得故障触发时刻相对于开始采样时刻的相对时间
返 回 值: 故障触发时刻相对于开始采样时刻的相对时间（毫秒）
参    数: param1 指定波形文件数据对象指针
		  param2 
************************************/
int ComtradeParseEngine::xjcp_GetTriggerTime( XJHANDLE hComtrade )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTriggerTime == NULL)
		return 0;
	return m_pGetTriggerTime(hComtrade);
}

/************************************
函 数 名: XJGetTotalTime
功能概要: 取得总时间
返 回 值: 总时间（毫秒）
参    数: param1 指定波形文件数据对象指针
		  param2 
************************************/
int ComtradeParseEngine::xjcp_GetTotalTime( XJHANDLE hComtrade )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetTotalTime == NULL)
		return 0;
	return m_pGetTotalTime(hComtrade);
}

/************************************
函 数 名: XJGetMaxValue
功能概要: 取得指定通道最大值
返 回 值: 最大值
参    数: param1 指定波形文件数据对象指针
		  param2 指定通道索引.索引从0开始
************************************/
float ComtradeParseEngine::xjcp_GetMaxValue( XJHANDLE hComtrade, int nCn, int nDataType, int nGroup)
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetMaxValue == NULL)
		return 0;
	return m_pGetMaxValue(hComtrade, nCn, nGroup, nDataType);
}

/************************************
函 数 名: XJGetRecordValue
功能概要: 取得指定通道指定采样点的记录值
返 回 值: 指定通道指定采样点的记录值
参    数: param1 指定波形文件数据对象指针
		  param2 指定通道索引.索引从0开始
		  param3 指定采样点索引.索引从0开始
************************************/
int ComtradeParseEngine::xjcp_GetRecordValue( XJHANDLE hComtrade, int nCn, int nSampleNo )
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetRecordValue == NULL)
		return 0;
	return m_pGetRecordValue(hComtrade, nCn, nSampleNo);
}

/*************************************************************
Date: 2010/09/19 	Author:LYH
函 数 名: XJGetCTPT
功能概要: 取得CTPT变比
返 回 值: 
参    数: param1 指定波形文件数据对象指针
		  param2 CT变比
		  param3 PT变比
*************************************************************/
void ComtradeParseEngine::xjcp_GetCTPT( XJHANDLE hComtrade, float& fCt, float& fPT )
{
	if(m_hModule == NULL)
		return;
	if(m_pGetCTPT == NULL)
		return;
	m_pGetCTPT(hComtrade, fCt, fPT);
}

/*************************************************************
Date: 2010/09/19 	Author:LYH
函 数 名: XJSetCTPT
功能概要: 设置CTPT变比
返 回 值: 
参    数: param1 指定波形文件数据对象指针
		  param2 CT变比
		  param3 PT变比
*************************************************************/
void ComtradeParseEngine::xjcp_SetCTPT( XJHANDLE hComtrade, float fCT, float fPT )
{
	if(m_hModule == NULL)
		return;
	if(m_pSetCTPT == NULL)
		return;
	m_pSetCTPT(hComtrade, fCT, fPT);
}

/****************************************************
Date:2012/5/15  Author:LYH
函数名:   XJGetTimeRelativeTriggerMSEC	
返回值:   double	相对于故障触发时刻的时间（微秒）
功能概要: 取得指定采样点相对于故障触发时刻的时间，返回微秒值
参数: XJHANDLE hComtrade	指定波形文件数据对象指针
参数: int nSampleNo	
*****************************************************/
double ComtradeParseEngine::xjcp_GetTimeRelativeTriggerMSEC( XJHANDLE hComtrade, int nSampleNo )
{
	if(m_hModule == NULL)
		return -1;
	if(m_pGetTimeRelativeTriggerMSEC == NULL)
		return -1;
	return m_pGetTimeRelativeTriggerMSEC(hComtrade, nSampleNo);
}

/****************************************************
Date:2012/5/15  Author:LYH
函数名:   XJGetTimeRelativeStartMSEC	
返回值:   double	相对于采样开始时刻的时间（微秒）
功能概要: 取得指定采样点相对于采样开始时刻的时间，返回微秒值
参数: XJHANDLE hComtrade	指定波形文件数据对象指针
参数: int nSampleNo	相对于故障触发时刻的时间（微秒）
*****************************************************/
double ComtradeParseEngine::xjcp_GetTimeRelativeStartMSEC( XJHANDLE hComtrade, int nSampleNo )
{
	if(m_hModule == NULL)
		return -1;
	if(m_pGetTimeRelativeStartMSEC == NULL)
		return -1;
	return m_pGetTimeRelativeStartMSEC(hComtrade, nSampleNo);
}

HdrObj* ComtradeParseEngine::xjcp_GetHdrInfoObject(XJHANDLE hComtrade)
{
	if(m_hModule == NULL)
		return 0;
	if(m_pGetHdrInfoObject == NULL)
		return 0;
	return m_pGetHdrInfoObject(hComtrade);
}

std::string ComtradeParseEngine::xjcp_GetHdrInfoJSON(XJHANDLE hComtrade)
{
	if( NULL == m_hModule )
		return "";
	if( NULL == m_pGetHdrInfoJSON )
		return "";
	return m_pGetHdrInfoJSON(hComtrade);
}
