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
�� �� ��: LoadCPLib
���ܸ�Ҫ: װ��XJComtrade.dll 
�� �� ֵ: װ�سɹ�����TRUE,ʧ�ܷ���FALSE
��    ��: param1 
		  param2 
************************************/
bool ComtradeParseEngine::xjcp_LoadCPLib()
{
	m_hModule = xj_load_library(LIBNAME_COMTRADEPARSE);
	if(m_hModule != NULL)
	{
		m_pLoadFile = (int(*)(char*,char*, XJHANDLE&))xj_get_addr(m_hModule, "LoadFile"); //���ļ�
		m_pCloseFile = (void (*)(XJHANDLE))xj_get_addr(m_hModule, "CloseFile");	//�ر��ļ�
		m_pGetCFGAttr = (void (*)(XJHANDLE,ComtradeCFG&))xj_get_addr(m_hModule, "GetCFGAttr"); //�ļ���¼�Ĳ���������������
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
�� �� ��: XJLoadFile
���ܸ�Ҫ: ���벨���ļ�
�� �� ֵ: ����ɹ�����>=0, ʧ�ܷ���<0
��    ��: param1 �����ļ�·��
		  param2 ������Ϣ
		  param3 ���ɵĲ����ļ����ݶ���ָ��
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
�� �� ��: XJCloseFile
���ܸ�Ҫ: �ر��ļ�
�� �� ֵ: 
��    ��: param1 ָ�������ļ����ݶ���ָ��
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
�� �� ��: XJGetCFGAttr
���ܸ�Ҫ: ȡ��CFG�ļ���¼�Ĳ���������������
�� �� ֵ: 
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ����������������
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
�� �� ��: XJGetRatesCount
���ܸ�Ҫ: ȡ�ò���Ƶ����
�� �� ֵ: ����Ƶ����
��    ��: param1 ָ�������ļ����ݶ���ָ��
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
�� �� ��: XJGetRateSample
���ܸ�Ҫ: ȡ��ָ������Ƶ�������Ĳ���Ƶ�ʺͲ�������
�� �� ֵ: 
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ������Ƶ������.������0��ʼ
		  param3 ����Ƶ��
		  param4 ����Ƶ���µĲ��������
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
�� �� ��: XJGetRate
���ܸ�Ҫ: ȡ��ָ��������Ĳ���Ƶ��
�� �� ֵ: ����Ƶ��
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��������
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
�� �� ��: XJGetChannelCFG
���ܸ�Ҫ: ȡ��ָ��ͨ����������Ϣ
�� �� ֵ: ͨ������.1��ʾģ����ͨ����0��ʾ������ͨ��
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��ͨ������.������0��ʼ
		  param3 ͨ��������Ϣ�ṹ��
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
�� �� ��: XJGetChannelType
���ܸ�Ҫ: ȡ��ָ��ͨ��������
�� �� ֵ: ͨ������.1��ʾģ����ͨ����0��ʾ������ͨ��
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��ͨ������.������0��ʼ
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
�� �� ��: XJGetTimeRelativeTrigger
���ܸ�Ҫ: ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ��
�� �� ֵ: ����ڹ��ϴ���ʱ�̵�ʱ�䣨���룩
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ������������.������0��ʼ
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
�� �� ��: XJGetTimeRelativeStart
���ܸ�Ҫ: ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ��
�� �� ֵ: ����ڲ�����ʼʱ�̵�ʱ�䣨���룩
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ������������.������0��ʼ
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
�� �� ��: XJGetRealValue
���ܸ�Ҫ: ȡ��ָ��ͨ��ָ�����������ʵֵ
�� �� ֵ: ָ��ͨ��ָ�����������ʵֵ
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��ͨ������.������0��ʼ
		  param3 ָ������������.������0��ʼ
		  param4 ָ����Ҫ��ֵ�����ͣ�Ĭ��Ϊ����ֵ.0Ϊ����ֵ��1Ϊһ��ֵ
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
�� �� ��: XJGetTotalSamples
���ܸ�Ҫ: ȡ�ò���������
�� �� ֵ: ����������
��    ��: param1 ָ�������ļ����ݶ���ָ��
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
�� �� ��: XJGetTriggerTime
���ܸ�Ҫ: ȡ�ù��ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ��
�� �� ֵ: ���ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ�䣨���룩
��    ��: param1 ָ�������ļ����ݶ���ָ��
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
�� �� ��: XJGetTotalTime
���ܸ�Ҫ: ȡ����ʱ��
�� �� ֵ: ��ʱ�䣨���룩
��    ��: param1 ָ�������ļ����ݶ���ָ��
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
�� �� ��: XJGetMaxValue
���ܸ�Ҫ: ȡ��ָ��ͨ�����ֵ
�� �� ֵ: ���ֵ
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��ͨ������.������0��ʼ
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
�� �� ��: XJGetRecordValue
���ܸ�Ҫ: ȡ��ָ��ͨ��ָ��������ļ�¼ֵ
�� �� ֵ: ָ��ͨ��ָ��������ļ�¼ֵ
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 ָ��ͨ������.������0��ʼ
		  param3 ָ������������.������0��ʼ
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
�� �� ��: XJGetCTPT
���ܸ�Ҫ: ȡ��CTPT���
�� �� ֵ: 
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 CT���
		  param3 PT���
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
�� �� ��: XJSetCTPT
���ܸ�Ҫ: ����CTPT���
�� �� ֵ: 
��    ��: param1 ָ�������ļ����ݶ���ָ��
		  param2 CT���
		  param3 PT���
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
������:   XJGetTimeRelativeTriggerMSEC	
����ֵ:   double	����ڹ��ϴ���ʱ�̵�ʱ�䣨΢�룩
���ܸ�Ҫ: ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ�䣬����΢��ֵ
����: XJHANDLE hComtrade	ָ�������ļ����ݶ���ָ��
����: int nSampleNo	
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
������:   XJGetTimeRelativeStartMSEC	
����ֵ:   double	����ڲ�����ʼʱ�̵�ʱ�䣨΢�룩
���ܸ�Ҫ: ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ�䣬����΢��ֵ
����: XJHANDLE hComtrade	ָ�������ļ����ݶ���ָ��
����: int nSampleNo	����ڹ��ϴ���ʱ�̵�ʱ�䣨΢�룩
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
