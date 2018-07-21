/********************************************************************
	created:	2011/11/17
	created:	17:11:2011   20:22
	filename: 	\VICENTE\CODE\COMMON\HeartbeatInterface.h
	file path:	\VICENTE\CODE\COMMON
	file base:	HeartbeatInterface
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#pragma   warning   (disable   :   4275) 

#ifndef HeartbeatInterface_h__
#define HeartbeatInterface_h__

#include "define.h"
#include "XJHeartbeatHandlerAPI.h"




class CHeartbeatInterface
{
public:
	CHeartbeatInterface(){}
	virtual ~CHeartbeatInterface(){}
	//���ڷ���
	virtual int RegisterBroadcastAddress(string pAddress, int pPort) = 0;
	virtual int BroatcastHeartbeat(StHeartbeatInfo& pInfo) = 0;
	//���ڽ���
	virtual int StartHeartbeatListening(vector<string>& pAddresses, int pPort, int pTimeout) = 0;
	virtual int EndHeartbeatListening() = 0;
	virtual int RegisterHeartbeatListener(PFUNRECVHEARTBEAT pOnRecv, void* pParam) = 0;
protected:
private:
};


#endif // HeartbeatInterface_h__