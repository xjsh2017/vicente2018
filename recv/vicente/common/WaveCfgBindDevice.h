/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:13
	filename: 	D:\WORKSPACE\VICENTE\Code\common\WaveCfgBindDevice.h
	file path:	D:\WORKSPACE\VICENTE\Code\common
	file base:	WaveCfgBindDevice
	file ext:	h
	author:		rwp
	
	purpose:	用于封装对录波Comtrade格式CFG文件关联设备动态库使用类
*********************************************************************/
#if !defined(WAVECFGBINDDEVICE_H)
#define WAVECFGBINDDEVICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef OS_WINDOWS
#define LIBNAME_WAVECFGBINDDEVICE "XJWaveCFGAssociate.dll"
#else 
#define LIBNAME_WAVECFGBINDDEVICE "libXJWaveCFGAssociate.so"
#endif

#include <string>
#include "xjlib.h"

using namespace std;

/**
* @brief	XJWaveCFGBindDevice 把Commtrade格式录波文件CFG文件中的第一行内容修改为与保信系统中的设备名
* @param 	[In]const char* pSrcWaveCFGName 全路径Commtrade格式录波文件CFG文件名
* @param 	[In]const char* pStationID 保信系统中站ID
* @param 	[In]const char* pPTID 保信系统中的装置ID
* @return	int 0: 成功，其它：错误
* @notes	   本函数用于修改录波文件之CFG的第一行内容为保信系统中的设备ID、
		    以便文件内容与设备关联、主要用于录波联网项目
* @sample	无
*/
typedef int (*XJWaveCFGBindDevice)(const char* pSrcWaveCFGName, const char* pStationID,const char* pPTID, char* pError);

#define FUNC_NAME_BINDDEVICE "XJWaveCFGBindDevice"


/**
 * @brief       class name: CWaveCfgBindDevice
 * @use			封装对动态库(libXJWaveCFGAssociate.so)的使用类
 * @author      rwp
 * @version     ver1.0
 * @date        
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */
 
class CWaveCfgBindDevice  
{
public:
	/**
	* @brief	CWaveCfgBindDevice 构造函数
	* @return	无
	* @notes	无
	* @sample	无
	*/
	CWaveCfgBindDevice();
	/**
	* @brief	~CWaveCfgBindDevice 析构函数
	* @return	virtual
	* @notes	无
	* @sample	无
	*/
	virtual ~CWaveCfgBindDevice();

public:
	/**
	* @brief	LoadLib 引导转换库
	* @return	bool true:成功 false:失败
	* @notes	无
	* @sample	无
	*/
	bool LoadLib();

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
	int BindWaveCfgToDevice(const char* pSrcWaveCFGName,const char* pStationID,const char* pPTID,char* pError);
	/**
	* @brief	FreeLib 释放动态库
	* @return	void 无
	* @notes	无
	* @sample	无
	*/
	void FreeLib();
private:
	/**	\brief	动态库句柄 */
	XJHANDLE m_hLibHandle;
	/**	\brief	utf8转换gbk函数指针 */
	XJWaveCFGBindDevice pfun_XJWaveCFGBindDevice;
};

#endif // !defined(WAVECFGBINDDEVICE_H)
