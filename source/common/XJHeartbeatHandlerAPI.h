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
	WORD			wSerialNo;		//���к�
	int				iType;			//�������� 0��������Ϣ 1���ֶ��л�
	int				iGroup;			//���������������
	int				iRunStatus;		//����������״̬ 0-������ 1-����
	int				iOnlineStatus;	//����״̬ -1-δ֪ 0-Standby 1-Active
	int				iLoadSoccer;	//��������
	time_t			tTimed;			//ʱ��
	string			strServerID;	//������ID
	string			strHBAddr;		//������IP��ַ
	string			strNetAAddr;	//a��IP��ַ
	string			strNetBAddr;	//b��IP��ַ
	vector<StHBSubstationStatus>	listStationID;	//��Ͻ��վ״̬;
}StHeartbeatInfo;


typedef void (*PFUNRECVHEARTBEAT)(LPVOID, StHeartbeatInfo&);

#endif // XJHeartbeatHandlerAPI_h__
