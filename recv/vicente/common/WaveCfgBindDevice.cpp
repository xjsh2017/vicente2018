/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:12
	filename: 	WaveCfgBindDevice.cpp
	file path:	\VICENTE\Code\common
	file base:	WaveCfgBindDevice
	file ext:	cpp
	author:		rwp
	
	purpose:	用于封装对录波Comtrade格式CFG文件关联设备动态库使用类
*********************************************************************/
#include "WaveCfgBindDevice.h"

/**
* @brief	CWaveCfgBindDevice 构造函数
* @return	无
* @notes	无
* @sample	无
*/
CWaveCfgBindDevice::CWaveCfgBindDevice():pfun_XJWaveCFGBindDevice(NULL),m_hLibHandle(NULL)
{

}

/**
* @brief	~CWaveCfgBindDevice 析构函数
* @return	virtual
* @notes	无
* @sample	无
*/
CWaveCfgBindDevice::~CWaveCfgBindDevice()
{
	FreeLib();
}

/**
* @brief	LoadLib 引导转换库
* @return	bool true:成功 false:失败
* @notes	无
* @sample	无
*/
bool CWaveCfgBindDevice::LoadLib()
{
	bool bReturn(false);
	m_hLibHandle = xj_load_library(LIBNAME_WAVECFGBINDDEVICE);
	if (m_hLibHandle)	
	{
		pfun_XJWaveCFGBindDevice = (XJWaveCFGBindDevice)xj_get_addr(m_hLibHandle,FUNC_NAME_BINDDEVICE);
		bReturn = (pfun_XJWaveCFGBindDevice)?true:false;
	}
	return bReturn;
}

/**
* @brief	BindWaveCfgToDevice 修改录波CFG文件首行内容使之与保信系统中的设备关联
* @param 	[In]const char* pSrcWaveCFGName 录波CFG文件名
* @param 	[In]const char* pStationID 站ID
* @param 	[In]const char* pPTID 设备ID 
* @param 	[In]string& pError 错误消息
* @return	int 0：成功 其它：失败
* @notes	无
* @sample	无
*/
int CWaveCfgBindDevice::BindWaveCfgToDevice(const char* pSrcWaveCFGName,const char* pStationID,const char* pPTID,char* pError)
{
	int iReturn(-1);
	if (pfun_XJWaveCFGBindDevice)
	{
		iReturn = pfun_XJWaveCFGBindDevice(pSrcWaveCFGName,pStationID,pPTID,pError);
	}
	return iReturn;
}



/**
* @brief	FreeLib 释放动态库
* @return	void 无
* @notes	无
* @sample	无
*/
void CWaveCfgBindDevice::FreeLib()
{
	if (m_hLibHandle)
	{
		xj_free_library(m_hLibHandle);
		m_hLibHandle = NULL;
		pfun_XJWaveCFGBindDevice = NULL;
	}
}