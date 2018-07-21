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

/** @brief            STTPConnectServer��������*/
typedef	XJHANDLE (*PFUNSTTPCONNECTSERVER)(int pClientModle , int& pConnected , const char* pLogPath , int pRunModle);

/** @brief            STTPCloseConnection��������*/
typedef int (*PFUNSTTPCLOSECONNECTION)(XJHANDLE pClientHandle);

/** @brief            STTPActiveTest��������*/
typedef int (*PFUNSTTPACTIVETEST) (XJHANDLE pClientHandle);

/** @brief            STTPRead��������*/
typedef int (*PFUNSTTPREAD) (XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);

/** @brief            STTPWrite��������*/
typedef int (*PFUNSTTPWRITE) (XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);

/** @brief            STTPSetOption��������*/
typedef int (*PFUNSTTPSETOPTION) (XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);

/** @brief            STTPRegisterHandler��������*/
typedef int (*PFUNSTTPREGISTERHANDLER) (XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );

/** @brief            STTPRegisterConnectChangerHandler��������*/
typedef int (*PFUNSTTPREGISTERCONNECTCHANGERHANDLER) (XJHANDLE pClientHandle ,PFUNONCONNECTCHANGE pOnConnectChange, void* pParam );

/** @brief            STTPSetLogPath��������*/
typedef int (*PFUNSTTPSETLOGPATH)  (XJHANDLE pClientHandle , const char* pLogPath);


typedef struct _STRUCT_SUBSTATION_STATUS{
	int  iStatus;
	string strStationID;
}XJSubstationRunStatus;

typedef struct _XJServerRunStatus{
	int								iGroup;			//���������������
	int								iOnlineStatus;	//����״̬ -1-δ֪ 0-Standby 1-Active
	int								iLoadSoccer;	//��������
	string							strServerID;	//������ID
	vector<XJSubstationRunStatus>	listStationStatus;	//��Ͻ��վ״̬;
}XJServerRunStatus;

typedef vector<XJServerRunStatus> XJServerStatusSet;

/** @brief            STTPQueryServerStatus��������*/
typedef int (*PFUNSTTPQUERYSERVERSTATUS)  (XJHANDLE pClientHandle , XJServerStatusSet& pSrvStatusSet);

#endif // XJSttpProxyAPI_h__