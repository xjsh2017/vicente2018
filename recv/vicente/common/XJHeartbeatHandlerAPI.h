/********************************************************************
	created:	2011/11/13
	created:	13:11:2011   19:07
	filename: 	\VICENTE\Code\common\XJHeartbeatHandlerAPI.h
	file path:	\VICENTE\Code\common
	file base:	XJHeartbeatHandlerAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#pragma   warning   (disable   :   4275) 

#ifndef XJHeartbeatHandlerAPI_h__
#define XJHeartbeatHandlerAPI_h__

#include "define.h"

const int HEARTBEAT_TYPE_INFO = 0;
const int HEARTBEAT_TYPE_OPERATION = 1;

typedef struct _STRUCT_HBSUBSTATION_STATUS{
	int  iStatus;
	string strStationID;
}StHBSubstationStatus;

typedef struct _Struct_Heartbeat_Info 
{
	WORD			wSerialNo;		//序列号
	int				iType;			//心跳类型 0：工况信息 1：手动切换
	int				iGroup;			//服务器所属分组号
	int				iRunStatus;		//服务器运行状态 0-不可用 1-可用
	int				iOnlineStatus;	//在线状态 -1-未知 0-Standby 1-Active
	int				iLoadSoccer;	//负载评分
	time_t			tTimed;			//时标
	string			strServerID;	//服务器ID
	string			strHBAddr;		//心跳网IP地址
	string			strNetAAddr;	//a网IP地址
	string			strNetBAddr;	//b网IP地址
	vector<StHBSubstationStatus>	listStationID;	//所辖厂站状态;
}StHeartbeatInfo;


typedef void (*PFUNRECVHEARTBEAT)(LPVOID, StHeartbeatInfo&);

#endif // XJHeartbeatHandlerAPI_h__
