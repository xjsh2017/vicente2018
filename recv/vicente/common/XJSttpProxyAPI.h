/********************************************************************
	created:	2012/06/29
	created:	29:6:2012   16:32
	filename: 	D:\VICENTE\CODE\XJSttpProxyAPI.h
	file path:	D:\VICENTE\CODE
	file base:	XJSttpProxyAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef XJSttpProxyAPI_h__
#define XJSttpProxyAPI_h__

#pragma   warning   (disable   :   4275)

#include "define.h"
#include "SttpDefine.h"

/** @brief            STTPConnectServer函数定义*/
typedef	XJHANDLE (*PFUNSTTPCONNECTSERVER)(int pClientModle , int& pConnected , const char* pLogPath , int pRunModle);

/** @brief            STTPCloseConnection函数定义*/
typedef int (*PFUNSTTPCLOSECONNECTION)(XJHANDLE pClientHandle);

/** @brief            STTPActiveTest函数定义*/
typedef int (*PFUNSTTPACTIVETEST) (XJHANDLE pClientHandle);

/** @brief            STTPRead函数定义*/
typedef int (*PFUNSTTPREAD) (XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);

/** @brief            STTPWrite函数定义*/
typedef int (*PFUNSTTPWRITE) (XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);

/** @brief            STTPSetOption函数定义*/
typedef int (*PFUNSTTPSETOPTION) (XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);

/** @brief            STTPRegisterHandler函数定义*/
typedef int (*PFUNSTTPREGISTERHANDLER) (XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );

/** @brief            STTPRegisterConnectChangerHandler函数定义*/
typedef int (*PFUNSTTPREGISTERCONNECTCHANGERHANDLER) (XJHANDLE pClientHandle ,PFUNONCONNECTCHANGE pOnConnectChange, void* pParam );

/** @brief            STTPSetLogPath函数定义*/
typedef int (*PFUNSTTPSETLOGPATH)  (XJHANDLE pClientHandle , const char* pLogPath);


typedef struct _STRUCT_SUBSTATION_STATUS{
	int  iStatus;
	string strStationID;
}XJSubstationRunStatus;

typedef struct _XJServerRunStatus{
	int								iGroup;			//服务器所属分组号
	int								iOnlineStatus;	//在线状态 -1-未知 0-Standby 1-Active
	int								iLoadSoccer;	//负载评分
	string							strServerID;	//服务器ID
	vector<XJSubstationRunStatus>	listStationStatus;	//所辖厂站状态;
}XJServerRunStatus;

typedef vector<XJServerRunStatus> XJServerStatusSet;

/** @brief            STTPQueryServerStatus函数定义*/
typedef int (*PFUNSTTPQUERYSERVERSTATUS)  (XJHANDLE pClientHandle , XJServerStatusSet& pSrvStatusSet);

#endif // XJSttpProxyAPI_h__