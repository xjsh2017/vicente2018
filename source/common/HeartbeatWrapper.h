/********************************************************************
	created:	2011/12/08
	created:	8:12:2011   11:07
	filename: 	\VICENTE\CODE\COMMON\HeartbeatWrapper.h
	file path:	\VICENTE\CODE\COMMON
	file base:	HeartbeatWrapper
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef HeartbeatWrapper_h__
#define HeartbeatWrapper_h__

#include "HeartbeatInterface.h"
#include "LogFile.h"
#include "xjlib.h"

typedef CHeartbeatInterface* (*PFUNCREATHEARTBEATHANDLER)(bool , CLogFile* ,CLogFile* );

typedef	bool (*PFUNDESTROYHEARTBEATHANDLER)(CHeartbeatInterface*);

#ifdef OS_LINUX

/** @brief            ��������������ƶ���*/
#define LIB_HEARTBEATHANDLER_NAME              "libXJHeartbeatHandler.so"

#endif

#ifdef OS_WINDOWS

/** @brief            ��������������ƶ���*/
#define LIB_HEARTBEATHANDLER_NAME              "XJHeartbeatHandler.dll"

#endif

class CHeartbeatWrapper
{
public:
	CHeartbeatWrapper(CLogFile& pLogFile);
	virtual ~CHeartbeatWrapper();
	CHeartbeatInterface* CreateHeartbeatHandler(bool pIfLogMessage, CLogFile* pFlowLog ,CLogFile* pMessageLog);
	
	bool DestroyHeartbeatHandler(CHeartbeatInterface* pHandler);
	bool InitLibrary();
private:
	/** @brief						��־����ָ��*/
	CLogFile&						m_rLogFile;

	/** @brief						�����������handle*/
	XJHANDLE						m_hLibHBHandler;
	
	/** @brief				CreateHeartbeatHandler����ָ��*/
	PFUNCREATHEARTBEATHANDLER		m_pFunCreateHBHandler;
	
	/** @brief				DestroyHeartbeatHadnler����ָ��*/
	PFUNDESTROYHEARTBEATHANDLER		m_pFunDestroyHBHandler;
};


#endif // HeartbeatWrapper_h__