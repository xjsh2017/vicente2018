/********************************************************************
	created:	2011/12/08
	created:	8:12:2011   11:07
	filename: 	\VICENTE\CODE\COMMON\HeartbeatWrapper.cpp
	file path:	\VICENTE\CODE\COMMON
	file base:	HeartbeatWrapper
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/

#include "HeartbeatWrapper.h"

CHeartbeatWrapper::CHeartbeatWrapper(CLogFile& pLogFile)
:m_rLogFile(pLogFile),m_hLibHBHandler(NULL),
 m_pFunCreateHBHandler(NULL),m_pFunDestroyHBHandler(NULL)
{
}

CHeartbeatWrapper::~CHeartbeatWrapper()
{
	xj_free_library(m_hLibHBHandler);
}

bool CHeartbeatWrapper::InitLibrary()
{
	m_hLibHBHandler = xj_load_library(LIB_HEARTBEATHANDLER_NAME);
	if (m_hLibHBHandler != NULL)
	{
		m_pFunCreateHBHandler = (PFUNCREATHEARTBEATHANDLER) xj_get_addr(m_hLibHBHandler,
			"CreateHeartbeatHandler");
		if (m_pFunCreateHBHandler == NULL) {
			m_rLogFile.FormatAdd(CLogFile::error,"(%s)load function CreateHeartbeatHandler failed",LIB_HEARTBEATHANDLER_NAME);
			return false;
		}	
		m_pFunDestroyHBHandler = (PFUNDESTROYHEARTBEATHANDLER) xj_get_addr(m_hLibHBHandler,
			"DestroyHeartbeatHandler");
		if (m_pFunDestroyHBHandler == NULL) {
			m_rLogFile.FormatAdd(CLogFile::error,"(%s)load function DestroyHeartbeatHandler failed",LIB_HEARTBEATHANDLER_NAME);
			return false;
		}	

	} 
	else
	{
		m_rLogFile.FormatAdd(CLogFile::error,"Load XJHeartbeatHandler Lib failed,reason:%s",xj_get_liberror());
		return false;
	}	
	
	m_rLogFile.Add("Load XJHeartbeatHandler Lib and function success",CLogFile::trace);
	return true;
}

CHeartbeatInterface* CHeartbeatWrapper::CreateHeartbeatHandler( bool pIfLogMessage, CLogFile* pFlowLog ,CLogFile* pMessageLog )
{
	if (NULL!=m_pFunCreateHBHandler)
	{
		return m_pFunCreateHBHandler(pIfLogMessage,pFlowLog,pMessageLog);
	} 
	else
	{
		return NULL;
	}
}

bool CHeartbeatWrapper::DestroyHeartbeatHandler( CHeartbeatInterface* pHandler )
{
	return (NULL!=m_pFunDestroyHBHandler)?m_pFunDestroyHBHandler(pHandler):false;
}
