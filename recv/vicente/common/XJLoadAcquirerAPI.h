/********************************************************************
	created:	2011/11/11
	created:	11:11:2011   14:03
	filename: 	\VICENTE\CODE\COMMON\XJLoadAcquirerAPI.h
	file path:	\VICENTE\CODE\COMMON
	file base:	XJLoadAcquirerAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef XJLoadAcquirerAPI_h__
#define XJLoadAcquirerAPI_h__

#pragma   warning   (disable   :   4275)

#include "define.h"
#include "LogFile.h"

typedef	struct _STRUCT_XJLOADACQUIRER{
	int	 iProThreadWeight;					//进程线程资源权重
	int  iProHandlerWeight;					//进程句柄资源权重
	int  iProCPUWeight;						//进程CPU资源权重
	int  iProMemoryWeight;					//进程内存资源权重
	int	 iSrvThreadWeight;					//服务器线程资源权重
	int  iSrvHandlerWeight;					//服务器句柄资源权重
	int  iSrvCPUWeight;						//服务器CPU资源权重
	int  iSrvMemoryWeight;					//服务器内存资源权重
	bool bReserved;							//保留备用
	int  iReserved;							//保留备用
	char reservedStr[256];					//保留备用
}stXJLoadAcquirer;


#ifdef OS_LINUX

/** @brief            共享库名称定义*/
#define LIB_LOADACQUIRER_NAME              "libXJLoadAcquirer.so"

#endif

#ifdef OS_WINDOWS

/** @brief            共享库名称定义*/
#define LIB_LOADACQUIRER_NAME              "XJLoadAcquirer.dll"

#endif

    /*
     *  @brief   	启动负载采集器
     *  @param 		[In]const stXJLoadAcquirer&  接口参数
     *  @return 	int 0-成功 -1-失败
     */

typedef	int (*PStartLoadAcquirer)(const stXJLoadAcquirer& , CLogFile& );
    /*
     *  @brief   	停止负载采集器
     *  @return 	int 0-成功 -1-失败 
     */

typedef	int (*PStopLoadAcquirer)();	

    /*
     *  @brief   	请求负载得分
     *  @return 	int 
	*/
typedef	int (*PAcquireLoadSoccer)();

#endif // XJLoadAcquirerAPI_h__