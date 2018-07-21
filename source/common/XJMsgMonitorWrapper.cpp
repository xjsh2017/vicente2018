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
 �� �� ��:    CXJMsgMonitorWrapper
 ���ܸ�Ҫ:    
 �� �� ֵ:    
 ************************************/
CXJMsgMonitorWrapper::CXJMsgMonitorWrapper()
:m_hLibXJMsgMonitor(NULL),
 m_pFunOpenUDPServer(NULL),m_pFunCloseUDPServer(NULL),
 m_pFunSendMsgFrame(NULL),m_pFunRecvMsgFrame(NULL),
 m_pFunSend103MsgFrame(NULL),m_pFunRecv103MsgFrame(NULL)
{
	
}

/************************************
 �� �� ��:    ~CXJMsgMonitorWrapper
 ���ܸ�Ҫ:    
 �� �� ֵ:    
 ************************************/
CXJMsgMonitorWrapper::~CXJMsgMonitorWrapper()
{
	xj_free_library(m_hLibXJMsgMonitor);
}

/************************************
 �� �� ��:    InitLibrary
 ���ܸ�Ҫ:    ���ض�̬��͸�����ָ��
 �� �� ֵ:    bool true -�ɹ� false -ʧ��
 ��    ��:    string & pError �������
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
 �� �� ��:    OpenUDPServer
 ���ܸ�Ҫ:    ��UDP�����
 �� �� ֵ:    XJHANDLE ����˶����� �緵��NULL���ʧ��
 ��    ��:    int pPort ����˰󶨵Ķ˿�
 ��    ��:    int pTimeOut ��ʱʱ�� ��λMS
 ��    ��:    string & pError �������
 ************************************/
XJHANDLE CXJMsgMonitorWrapper::OpenUDPServer( int pPort, int pTimeOut, string& pError )
{
	if (NULL != m_pFunOpenUDPServer)
	{
		return m_pFunOpenUDPServer(pPort,pTimeOut,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루OpenUDPServer��";
		return NULL;
	}	
}

/************************************
 �� �� ��:    CloseUDPServer
 ���ܸ�Ҫ:    �ر�UDP�����
 �� �� ֵ:    int 0-�ɹ� -1-ʧ��
 ��    ��:    XJHANDLE pHandle ����˶�����
 ��    ��:    string & pError �������
 ************************************/
int CXJMsgMonitorWrapper::CloseUDPServer( XJHANDLE pHandle, string& pError )
{
	if (NULL != m_pFunCloseUDPServer)
	{
		return m_pFunCloseUDPServer(pHandle,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루CloseUDPServer��";
		return -1;
	}
}

/************************************
 �� �� ��:    RecvMsgFrame
 ���ܸ�Ҫ:    ���ܱ�������֡
 �� �� ֵ:    int 0-�ɹ� -1-ʧ��
 ��    ��:    XJHANDLE pHandle ����˶�����
 ��    ��:    XJFRAMESTRUCT & pFrameData ���յ��ı�������֡p
 ��    ��:    string & pError �������
 ************************************/
int CXJMsgMonitorWrapper::RecvMsgFrame( XJHANDLE pHandle, XJFRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunRecvMsgFrame)
	{
		return m_pFunRecvMsgFrame(pHandle,pFrameData,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루RecvMsgFrame��";
		return -1;
	}
}

/************************************
 �� �� ��:    SendMsgFrame
 ���ܸ�Ҫ:    ���ͱ�������֡
 �� �� ֵ:    int int 0-�ɹ� -1-ʧ��
 ��    ��:    string pDistIP ���͵�Ŀ��IP��ַ
 ��    ��:    int pPort ���͵�Ŀ�Ķ˿�
 ��    ��:    const XJFRAMESTRUCT & pFrameData �����͵ı�������֡
 ��    ��:    string & pError  �������
 ************************************/
int CXJMsgMonitorWrapper::SendMsgFrame( string pDistIP, int pPort, const XJFRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunSendMsgFrame)
	{
		return m_pFunSendMsgFrame(pDistIP,pPort,pFrameData,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루SendMsgFrame��";
		return -1;
	}	
}

/************************************
 �� �� ��:    Recv103MsgFrame
 ���ܸ�Ҫ:    ����103��������֡
 �� �� ֵ:    int 0-�ɹ� -1-ʧ��
 ��    ��:    XJHANDLE pHandle ����˶�����
 ��    ��:    XJ103FRAMESTRUCT & pFrameData ���յ��ı�������֡
 ��    ��:    string & pError �������
 ************************************/
int CXJMsgMonitorWrapper::Recv103MsgFrame( XJHANDLE pHandle, XJ103FRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunRecv103MsgFrame)
	{
		return m_pFunRecv103MsgFrame(pHandle,pFrameData,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루RecvMsgFrame��";
		return -1;
	}	
}

/************************************
�� �� ��:    Convert103FrameStructToStream
���ܸ�Ҫ:    �������͵ı��Ľṹ��ת�ɱ�����
�� �� ֵ:    int int 0-�ɹ� -1-ʧ��
��    ��:    string & OutMsg ������
��    ��:    const XJ103FRAMESTRUCT & pFrameData ���Ľṹ��
************************************/
int CXJMsgMonitorWrapper::Send103MsgFrame( string pDistIP, int pPort, const XJ103FRAMESTRUCT& pFrameData, string& pError )
{
	if (NULL != m_pFunSend103MsgFrame)
	{
		return m_pFunSend103MsgFrame(pDistIP,pPort,pFrameData,pError);
	} 
	else
	{
		pError = "��Ч�ĺ���ָ�루SendMsgFrame��";
		return -1;
	}		
}
