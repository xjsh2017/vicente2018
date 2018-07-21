/********************************************************************
	created:	2011/11/11
	created:	11:11:2011   14:20
	filename: 	\VICENTE\CODE\COMMON\XJSrvOnlineManagerAPI.h
	file path:	\VICENTE\CODE\COMMON
	file base:	XJSrvOnlineManagerAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef XJSrvOnlineManagerAPI_h__
#define XJSrvOnlineManagerAPI_h__

#pragma   warning   (disable   :   4275)
#include "define.h"

typedef	struct _STRUCT_XJSrvOnlineManager{
	char chServerID[41];					//服务器ID
	char szLog_path[FILE_NAME_MAX_LEN];		//日志存放目录
	int  iLog_level;						//日志级别,0:不记录日志到文件1:错误日志2:警告日志3:跟踪日志
	int  iLogDay;							//日志记录天数
	bool bRecordMsg;						//是否记录报文
	bool bReserved;							//保留备用
	int  iReserved;							//保留备用
	char reservedStr[256];					//保留备用
}stXJSrvOnlineManager;

/** @brief            StartSrvOnLineManager函数定义*/
typedef int (*PFUNSTARTSRVONLINEMANAGER)(const stXJSrvOnlineManager&);


/** @brief            StopSrvOnLineManager函数定义*/
typedef int (*PFUNSTOPSRVONLINEMANAGER)();


typedef struct _STRUCT_SUBSTATION{
	_STRUCT_SUBSTATION():iType(0),iAddr(0),iPort(0),iPortReserve(0),iProtocolID(0),iIPFlag(0)
	{
		memset(chStationID,0,13);
		memset(chStationName,0,51);
		memset(chIP,0,13);
		memset(chIPReserve,0,13);
		memset(chTelphone,0,51);
	}
	int  iType;
	int  iAddr;
	int  iPort;
	int  iPortReserve;
	int  iProtocolID;
	int  iIPFlag;
	char chStationID[13];
	char chStationName[51];
	char chIP[19];
	char chIPReserve[19];
	char chTelphone[51];
}stXJSubstation;

/** @brief  服务器在线状态-未知*/
const int	SRV_ONLINE_UNKNOWN = -1;
/** @brief  服务器在线状态-备用*/
const int	SRV_ONLINE_STANDBY = 0;
/** @brief  服务器在线状态-在线*/
const int	SRV_ONLINE_ACTIVE = 1;

/** @brief            服务器切换回调函数声明*/
typedef int (*PFUNONSRVSWITCH)( void* pParam , int pStatus , vector<stXJSubstation>& pStationList);

/** @brief            RegisterSrvSwitchCallback函数定义*/
typedef int (*PFUNREGISTERSRVSWITCHCALLBACK)(PFUNONSRVSWITCH pOnSrvSwitch, void* pParam);

/** @brief  子站挂载状态-未知*/
const int SUB_LOADED_UNKNOWN = -1;
/** @brief  子站挂载状态-未挂载*/
const int SUB_LOADED_OFF = 0;
/** @brief  子站挂载状态-已挂载*/
const int SUB_LOADED_ON = 1;

typedef struct _STRUCT_SUBSTATION_LOAD_STATUS{
	char chStationID[13];
	int  iStatus;
}stXJSubstationLoadStatus;

/** @brief            SetStationLoadStatus函数定义*/
typedef int (*PFUNSETSTATIONLOADSTATUS)(stXJSubstationLoadStatus& pStatus);







#endif // XJSrvOnlineManagerAPI_h__



