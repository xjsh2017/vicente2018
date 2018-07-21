/********************************************************************
	created:	2010/09/21
	created:	21:9:2010   17:13
	filename: 	\VICENTE\CODE\COMMON\XJMsgMonitorWrapper.cpp
	file path:	\VICENTE\CODE\COMMON
	file base:	XJMsgMonitorWrapper
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/

#include "XJMsgMonitorWrapper.h"

/************************************
 函 数 名:    CXJMsgMonitorWrapper
 功能概要:    
 返 回 值:    
 ************************************/
CXJMsgMonitorWrapper::CXJMsgMonitorWrapper()
:m_hLibXJMsgMonitor(NULL),
 m_pFunOpenUDPServer(NULL),m_pFunCloseUDPServer(NULL),
 m_pFunSendMsgFrame(NULL),m_pFunRecvMsgFrame(NULL),
 m_pFunSend103MsgFrame(NULL),m_pFunRecv103MsgFrame(NULL)
{
	
}

/************************************
 函 数 名:    ~CXJMsgMonitorWrapper
 功能概要:    
 返 回 值:    
 ************************************/
CXJMsgMonitorWrapper::~CXJMsgMonitorWrapper()
{
	xj_free_library(m_hLibXJMsgMonitor);
}

/************************************
 函 数 名:    InitLibrary
 功能概要:    加载动态库和各函数指针
 返 回 值:    bool true -成功 false -失败
 参    数:    string & pError 错误输出
 ************************************/
bool CXJMsgMonitorWrapper::InitLibrary( string& pError )
{
	m_hLibXJMsgMonitor = xj_load_library(LIB_MSGMONITOR_NAME);
	if (m_hLibXJMsgMonitor != NULL)
	{
		m_pFunOpenUDPServer = (PFUNOPENUDPSERVER) xj_get_addr(m_hLibXJMsgMonitor,
			"OpenUDPServer");
		if (m_pFunOpenUDPServer == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function OpenUDPServer failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}	
		
		m_pFunCloseUDPServer = (PFUNCLOSEUDPSERVER) xj_get_addr(m_hLibXJMsgMonitor,
			"CloseUDPServer");
		if (m_pFunCloseUDPServer == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function CloseUDPServer failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}	
		
		m_pFunRecvMsgFrame = (PFUNRECVMSGFRAME) xj_get_addr(m_hLibXJMsgMonitor,
			"RecvMsgFrame");
		if (m_pFunRecvMsgFrame == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function RecvMsgFrame failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}	
		
		m_pFunSendMsgFrame = (PFUNSENDMSGFRAME) xj_get_addr(m_hLibXJMsgMonitor,
			"SendMsgFrame");
		if (m_pFunSendMsgFrame == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function SendMsgFrame failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}

		m_pFunRecv103MsgFrame = (PFUNRECV103MSGFRAME) xj_get_addr(m_hLibXJMsgMonitor,
			"Recv103MsgFrame");
		if (m_pFunRecv103MsgFrame == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function Recv103MsgFrame failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}	
		
		m_pFunSend103MsgFrame = (PFUNSEND103MSGFRAME) xj_get_addr(m_hLibXJMsgMonitor,
			"Send103MsgFrame");
		if (m_pFunSend103MsgFrame == NULL) {
			char pErrorMsg[1024]="";
			sprintf(pErrorMsg,"(%s)load function Send103MsgFrame failed",LIB_MSGMONITOR_NAME);
			pError = pErrorMsg;
			return false;
		}
	} 
	else
	{
		char pErrorMsg[1024]="";
		sprintf(pErrorMsg,"LoadXJMsgMonitor Lib failed(reason:%s)",xj_get_liberror());
		pError = pErrorMsg;
		return false;
	}
	return true;	
}

/************************************
 函 数 名:    OpenUDPServer
 功能概要:    打开UDP服务端
 返 回 值:    XJHANDLE 服务端对象句柄 如返回NULL则打开失败
 参    数:    int pPort 服务端绑定的端口
 参    数:    int pTimeOut 超时时间 单位MS
 参    数:    string & pError 错误输出
 ************************************/
XJHANDLE CXJMsgMonitorWrapper::OpenUDPServer( int pPort, int pTimeOut, string& pError )
{
	if (NULL != m_pFunOpenUDPServer)
	{
		return m_pFunOpenUDPServer(pPort,pTimeOut,pError);
	} 
	else
	{
		pError = "无效的函数指针（OpenUDPServer）";
		return NULL;
	}	
}

/************************************
 函 数 名:    CloseUDPServer
 功能概要:    关闭UDP服务端
 返 回 值:    int 0-成功 -1-失败
 参    数:    XJHANDLE pHandle 服务端对象句柄
 参    数:    string & pError 错误输出
 ************************************/
int CXJMsgMonitorWrapper::CloseUDPServer( XJHANDLE pHandle, string& pError )
{
	if (NULL != m_pFunCloseUDPServer)
	{
		return m_pFunCloseUDPServer(pHandle,pError);
	} 
	else
	{
		pError = "无效的函数指针（CloseUDPServer）";
		return -1;
	}
}

/************************************
 函 数 名:    RecvMsgFrame
 功能概要:    接受报文数据帧
 返 回 值:    int 0-成功 -1-失败
 参    数:    XJHANDLE pHandle 服务端对象句柄
 参    数:    XJFRAMESTRUCT & pFrameData 接收到的报文数据帧p
 参    数:    string & pError 错误输出
 ************************************/
int CXJMsgMonitorWrapper::RecvMsgFrame( XJHANDLE pHandle, XJFRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunRecvMsgFrame)
	{
		return m_pFunRecvMsgFrame(pHandle,pFrameData,pError);
	} 
	else
	{
		pError = "无效的函数指针（RecvMsgFrame）";
		return -1;
	}
}

/************************************
 函 数 名:    SendMsgFrame
 功能概要:    发送报文数据帧
 返 回 值:    int int 0-成功 -1-失败
 参    数:    string pDistIP 发送的目的IP地址
 参    数:    int pPort 发送的目的端口
 参    数:    const XJFRAMESTRUCT & pFrameData 待发送的报文数据帧
 参    数:    string & pError  错误输出
 ************************************/
int CXJMsgMonitorWrapper::SendMsgFrame( string pDistIP, int pPort, const XJFRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunSendMsgFrame)
	{
		return m_pFunSendMsgFrame(pDistIP,pPort,pFrameData,pError);
	} 
	else
	{
		pError = "无效的函数指针（SendMsgFrame）";
		return -1;
	}	
}

/************************************
 函 数 名:    Recv103MsgFrame
 功能概要:    接受103报文数据帧
 返 回 值:    int 0-成功 -1-失败
 参    数:    XJHANDLE pHandle 服务端对象句柄
 参    数:    XJ103FRAMESTRUCT & pFrameData 接收到的报文数据帧
 参    数:    string & pError 错误输出
 ************************************/
int CXJMsgMonitorWrapper::Recv103MsgFrame( XJHANDLE pHandle, XJ103FRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunRecv103MsgFrame)
	{
		return m_pFunRecv103MsgFrame(pHandle,pFrameData,pError);
	} 
	else
	{
		pError = "无效的函数指针（RecvMsgFrame）";
		return -1;
	}	
}

/************************************
函 数 名:    Convert103FrameStructToStream
功能概要:    将待发送的报文结构体转成报文流
返 回 值:    int int 0-成功 -1-失败
参    数:    string & OutMsg 报文流
参    数:    const XJ103FRAMESTRUCT & pFrameData 报文结构体
************************************/
int CXJMsgMonitorWrapper::Send103MsgFrame( string pDistIP, int pPort, const XJ103FRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunSend103MsgFrame)
	{
		return m_pFunSend103MsgFrame(pDistIP,pPort,pFrameData,pError);
	} 
	else
	{
		pError = "无效的函数指针（SendMsgFrame）";
		return -1;
	}		
}
