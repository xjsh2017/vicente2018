/********************************************************************
	created:	2012/05/19
	created:	19:5:2012   15:16
	filename: 	CD5kMsgBusFacade.cpp
	file path:	
	file base:	CD5kMsgBusFacade
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "CD5kMsgBusFacade.h"

CD5kMsgBusFacade::CD5kMsgBusFacade(string pProcName, int pEventSet, CLogFile& pLogFile)
:m_bExit(false),m_iEventSet(pEventSet),m_procKey(-1),
 m_pFunOnPushMessage(NULL),m_pOnPushMessageObject(NULL),
 m_pFunOnPullMessage(NULL),m_pOnPullMessageObject(NULL),
 m_RecvHandleThreadId(0),m_hRecvHandleThreadHandle(INVALID_THREAD),
 m_SendHandleThreadId(0),m_hSendHandleThreadHandle(INVALID_THREAD),
 m_rLogFile(pLogFile)
{
	bzero(m_chProcessName,25);
	strncpy(m_chProcessName,pProcName.c_str(),24);
}

CD5kMsgBusFacade::~CD5kMsgBusFacade()
{
	
}

void CD5kMsgBusFacade::WriteLog( const char * pLog,int nLevel )
{
	if (NULL != pLog)
	{
		m_rLogFile.FormatAdd(nLevel,"[CD5kMsgBusFacade] %s",pLog);
	}	
}

bool CD5kMsgBusFacade::Start( void )
{
	m_procKey = m_MsgBusObjecj.messageInit("realtime","scada_relay",m_chProcessName);
	if (0 > m_procKey){
		WriteLog("消息总线初始化失败",CLogFile::error);
		return false;
	}
	
	if (0 > m_MsgBusObjecj.messageSubscribe(m_iEventSet)){
		WriteLog("订阅事件集失败",CLogFile::error);
		return false;
	}
	
	//启动线程
	
	if(!StartRecvHandleThread())
		return false;
	
	if(!StartSendHandleThread())
		return false;
	
	return true;	
}

void CD5kMsgBusFacade::End( void )
{
	m_bExit = true;
	SendExitToRecvPort();
	
	MySleep(200);
	
	EndSendHandleThread();
	EndRecvHandleThread();
	
	
	m_MsgBusObjecj.messageUnSubscribe(m_iEventSet);
	
	m_MsgBusObjecj.messageExit(m_procKey);
		
}

int CD5kMsgBusFacade::RegisterPushMessageCallback( PFUNPUSHMESSAGE pFunction, LPVOID pParam )
{
	int iResult(-1);
	
	if (( NULL != pFunction ) && (NULL != pParam))
	{
		m_pFunOnPushMessage = pFunction;	
		m_pOnPushMessageObject = pParam;
		iResult = 0;
	}
	
	return iResult;		
}

int CD5kMsgBusFacade::RegisterPullMessageCallback( PFUNPULLMESSAGE pFunction, LPVOID pParam )
{
	int iResult(-1);
	
	if (( NULL != pFunction ) && (NULL != pParam))
	{
		m_pFunOnPullMessage = pFunction;	
		m_pOnPullMessageObject = pParam;
		iResult = 0;
	}
	
	return iResult;		
}

bool CD5kMsgBusFacade::PushMessage( const Message& pMsg )
{
	if ((NULL != m_pFunOnPushMessage) && (NULL != m_pOnPushMessageObject))
	{
		return m_pFunOnPushMessage(pMsg,m_pOnPushMessageObject);
	}
	return true;		
}

bool CD5kMsgBusFacade::PullMessage( Message& pMsg )
{
	if ((NULL != m_pFunOnPullMessage) && (NULL != m_pOnPullMessageObject))
	{
		return m_pFunOnPullMessage(pMsg,m_pOnPullMessageObject);
	}
	return true;	
}

bool CD5kMsgBusFacade::StartRecvHandleThread( void )
{
	// thread return code
	int nRet =0;
	
	// begine create  thread
	nRet = xj_thread_create(&m_hRecvHandleThreadHandle,
		&m_RecvHandleThreadId,
		RecvHandleThreadFunc,
		this);
	
	// assert create success
	if(nRet !=0)
	{
		// save log des
		char cLog[MAX_LINE_LENGTH]="";
		sprintf(cLog,"创建接收处理线程失败，原因为:%s",strerror(errno));
		WriteLog(cLog,CLogFile::error);
		return false;
	}
	
	// trace 
	WriteLog("创建接收处理线程成功",CLogFile::error);
	
	return true;				
}

void CD5kMsgBusFacade::EndRecvHandleThread( void )
{
	//停止接收线程
#ifdef OS_LINUX
	if(m_RecvHandleThreadId != 0){
		int nRet=xj_thread_join(m_RecvHandleThreadId,NULL);
		if(nRet != 0){
			// save log des
			char cLog[MAX_LINE_LENGTH]="";
			sprintf(cLog,"退出接收处理线程异常，原因为:%s",strerror(errno));
			WriteLog(cLog,CLogFile::error);
			return;
		}
	}
	
#endif
	
#ifdef OS_WINDOWS
	time_t tBegin;
	time_t tNow;
	time(&tBegin);
	do 
	{
		if (m_hRecvHandleThreadHandle == INVALID_THREAD)
		{
			break;
		}
		MySleep(50);
		time(&tNow);
	} while ((tNow-tBegin)<30);
	
	if (m_hRecvHandleThreadHandle != INVALID_THREAD)
	{
		xj_thread_detach(m_hRecvHandleThreadHandle);
	}
	
#endif
	WriteLog("退出接收处理线程成功",CLogFile::error);			
}

THREAD_FUNC WINAPI CD5kMsgBusFacade::RecvHandleThreadFunc( LPVOID pParam )
{
	// save log des
	char cLog[MAX_LINE_LENGTH]="";
	int nRet =0;
	
	CD5kMsgBusFacade* pThis = (CD5kMsgBusFacade*)pParam;
	
	try{
		nRet=pThis->RecvHandleLoop();
	}
	catch(...)
	{
		sprintf(cLog,"RecvHandleThreadFunc() exception,error code=%d",errno);
		pThis->WriteLog(cLog,CLogFile::error);
		pThis->SetRecvHandleThreadHandle(INVALID_THREAD);
		return THREAD_RETURN;
	}
	
	sprintf(cLog,"RecvHandleThreadFunc() exit with ret code =%d",nRet);
	pThis->WriteLog(cLog,CLogFile::trace);
	pThis->SetRecvHandleThreadHandle(INVALID_THREAD);
	
	return THREAD_RETURN;	
}


void CD5kMsgBusFacade::SetRecvHandleThreadHandle( THREAD_HANDLE pHandle )
{
	m_hRecvHandleThreadHandle = pHandle;	
}

int CD5kMsgBusFacade::RecvHandleLoop()
{
	WriteLog("enter MsgBus RecvHandleLoop()",
		CLogFile::trace);
	
	Message msg; //消息

	char chLog[256] = "";
	
    // recv data loop
	while(!m_bExit)
	{
		if(0 < m_MsgBusObjecj.messageReceive(&msg)){
			bzero(chLog,256);
			sprintf(chLog,"收到消息 Channel:%d EventID:%d Length:%d",msg.header.serv,msg.header.event,msg.header.len);
			WriteLog(chLog,CLogFile::trace);
			PushMessage(msg);
		}
		MySleep(50);
	}
	
	WriteLog("exit MsgBus RecvHandleLoop()",
		CLogFile::trace);
	
	return 0;	
}

bool CD5kMsgBusFacade::StartSendHandleThread( void )
{
	// thread return code
	int nRet =0;
	
	// begine create  thread
	nRet = xj_thread_create(&m_hSendHandleThreadHandle,
		&m_SendHandleThreadId,
		SendHandleThreadFunc,
		this);
	
	// assert create success
	if(nRet !=0)
	{
		// save log des
		char cLog[MAX_LINE_LENGTH]="";
		sprintf(cLog,"创建发送处理线程失败，原因为:%s",strerror(errno));
		WriteLog(cLog,CLogFile::error);
		return false;
	}
	
	// trace 
	WriteLog("创建发送处理线程成功",CLogFile::error);
	
	return true;	
}

void CD5kMsgBusFacade::EndSendHandleThread( void )
{
	//停止发送线程
#ifdef OS_LINUX
	if(m_SendHandleThreadId != 0){
		int nRet=xj_thread_join(m_SendHandleThreadId,NULL);
		if(nRet != 0){
			// save log des
			char cLog[MAX_LINE_LENGTH]="";
			sprintf(cLog,"退出发送处理线程异常，原因为:%s",strerror(errno));
			WriteLog(cLog,CLogFile::error);
			return;
		}
	}
	
#endif
	
#ifdef OS_WINDOWS
	time_t tBegin;
	time_t tNow; 
	time(&tBegin);
	do 
	{
		if (m_hSendHandleThreadHandle == INVALID_THREAD)
		{
			break;
		}
		MySleep(50);
		time(&tNow);
	} while ((tNow-tBegin)<30);
	
	if (m_hSendHandleThreadHandle != INVALID_THREAD)
	{
		xj_thread_detach(m_hSendHandleThreadHandle);
	}
	
#endif
	WriteLog("退出发送处理线程成功",CLogFile::error);			
}

THREAD_FUNC WINAPI CD5kMsgBusFacade::SendHandleThreadFunc( LPVOID pParam )
{
	// save log des
	char cLog[MAX_LINE_LENGTH]="";
	int nRet =0;
	
	CD5kMsgBusFacade* pThis = (CD5kMsgBusFacade*)pParam;
	
	try{
		nRet=pThis->SendHandleLoop();
	}
	catch(...)
	{
		sprintf(cLog,"SendHandleThreadFunc() exception,error code=%d",errno);
		pThis->WriteLog(cLog,CLogFile::error);
		pThis->SetSendHandleThreadHandle(INVALID_THREAD);
		return THREAD_RETURN;
	}
	
	sprintf(cLog,"SendHandleThreadFunc() exit with ret code =%d",nRet);
	pThis->WriteLog(cLog,CLogFile::trace);
	pThis->SetSendHandleThreadHandle(INVALID_THREAD);
	
	return THREAD_RETURN;		
}

void CD5kMsgBusFacade::SetSendHandleThreadHandle( THREAD_HANDLE pHandle )
{
	m_hSendHandleThreadHandle = pHandle;	
}

int CD5kMsgBusFacade::SendHandleLoop()
{
	WriteLog("enter MsgBus SendHandleLoop()",
		CLogFile::trace);
	
	Message msg; //消息
	char chLog[256] = "";

    // send data loop
	while(!m_bExit)
	{
		if(PullMessage(msg)){
			if (0>m_MsgBusObjecj.messageSend((Message*)&msg, msg.header.len, NULL))
			{
				bzero(chLog,256);
				sprintf(chLog,"发送消息失败 Channel:%d EventID:%d Length:%d",msg.header.serv,msg.header.event,msg.header.len);
				WriteLog(chLog,CLogFile::error);
			} 
			else
			{
				bzero(chLog,256);
				sprintf(chLog,"发送消息 Channel:%d EventID:%d Length:%d",msg.header.serv,msg.header.event,msg.header.len);
				WriteLog(chLog,CLogFile::trace);
			}
		}
		MySleep(50);
	}
	
	WriteLog("exit MsgBus SendHandleLoop()",
		CLogFile::trace);
	
	return 0;	
}

void CD5kMsgBusFacade::SendExitToRecvPort()
{
	Message msg; //消息
	msg.header.serv = m_iEventSet;
	msg.header.event = 621;
	msg.header.len = 27;
	char* pBuf = msg.Msg_buf;
	sprintf(pBuf,"Bye msgbus, I'm gonna exit!");
	m_MsgBusObjecj.messageSend((Message*)&msg, msg.header.len, NULL);	
}
