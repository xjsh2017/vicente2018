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
	char chServerID[41];					//������ID
	char szLog_path[FILE_NAME_MAX_LEN];		//��־���Ŀ¼
	int  iLog_level;						//��־����,0:����¼��־���ļ�1:������־2:������־3:������־
	int  iLogDay;							//��־��¼����
	bool bRecordMsg;						//�Ƿ��¼����
	bool bReserved;							//��������
	int  iReserved;							//��������
	char reservedStr[256];					//��������
}stXJSrvOnlineManager;

/** @brief            StartSrvOnLineManager��������*/
typedef int (*PFUNSTARTSRVONLINEMANAGER)(const stXJSrvOnlineManager&);


/** @brief            StopSrvOnLineManager��������*/
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

/** @brief  ����������״̬-δ֪*/
const int	SRV_ONLINE_UNKNOWN = -1;
/** @brief  ����������״̬-����*/
const int	SRV_ONLINE_STANDBY = 0;
/** @brief  ����������״̬-����*/
const int	SRV_ONLINE_ACTIVE = 1;

/** @brief            �������л��ص���������*/
typedef int (*PFUNONSRVSWITCH)( void* pParam , int pStatus , vector<stXJSubstation>& pStationList);

/** @brief            RegisterSrvSwitchCallback��������*/
typedef int (*PFUNREGISTERSRVSWITCHCALLBACK)(PFUNONSRVSWITCH pOnSrvSwitch, void* pParam);

/** @brief  ��վ����״̬-δ֪*/
const int SUB_LOADED_UNKNOWN = -1;
/** @brief  ��վ����״̬-δ����*/
const int SUB_LOADED_OFF = 0;
/** @brief  ��վ����״̬-�ѹ���*/
const int SUB_LOADED_ON = 1;

typedef struct _STRUCT_SUBSTATION_LOAD_STATUS{
	char chStationID[13];
	int  iStatus;
}stXJSubstationLoadStatus;

/** @brief            SetStationLoadStatus��������*/
typedef int (*PFUNSETSTATIONLOADSTATUS)(stXJSubstationLoadStatus& pStatus);







#endif // XJSrvOnlineManagerAPI_h__



