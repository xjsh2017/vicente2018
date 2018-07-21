/********************************************************************
	created:	2010/09/21
	created:	21:9:2010   17:13
	filename: 	\VICENTE\CODE\COMMON\XJMsgMonitorWrapper.h
	file path:	\VICENTE\CODE\COMMON
	file base:	XJMsgMonitorWrapper
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#pragma   warning   (disable   :   4786) 
#pragma   warning   (disable   :   4275) 

#ifndef XJMsgMonitorWrapper_h__
#define XJMsgMonitorWrapper_h__

#include "define.h"
#include "xjlib.h"
#include "XJMsgMonitorAPI.h"

#ifdef OS_LINUX

/** @brief            ���ļ��ӹ�������ƶ���*/
#define LIB_MSGMONITOR_NAME              "libXJMsgMonitor.so"

#endif

#ifdef OS_WINDOWS

/** @brief            ���ļ��ӹ�������ƶ���*/
#define LIB_MSGMONITOR_NAME              "XJMsgMonitor.dll"

#endif

/** @brief            OpenUDPServer��������*/
typedef XJHANDLE (*PFUNOPENUDPSERVER)( int, int, string&);	

/** @brief            CloseUDPServer��������*/
typedef int (*PFUNCLOSEUDPSERVER)(XJHANDLE, string&);	

/** @brief            RecvMsgFrame��������*/
typedef int (*PFUNRECVMSGFRAME)(XJHANDLE, XJFRAMESTRUCT&, string&);

/** @brief            SendMsgFrame��������*/
typedef	int (*PFUNSENDMSGFRAME)(string, int, const XJFRAMESTRUCT&, string&);

/** @brief            Recv103MsgFrame��������*/
typedef	int (*PFUNRECV103MSGFRAME)(XJHANDLE, XJ103FRAMESTRUCT&, string&);

/** @brief            Send103MsgFrame��������*/
typedef	int (*PFUNSEND103MSGFRAME)(string, int, const XJ103FRAMESTRUCT&, string&);

class CXJMsgMonitorWrapper
{
public:
    /*
     *  @brief		CXJMsgMonitorWrapper	  
     *  @return		 
     */

	CXJMsgMonitorWrapper();
    /*
     *  @brief		~CXJMsgMonitorWrapper	  
     *  @return		virtual 
     */

	virtual ~CXJMsgMonitorWrapper();
    /*
     *  @brief		InitLibrary	 ���ض�̬��͸�����ָ�� 
     *  @param	[Out]pError  ������ʾ
     *  @return		virtual bool true -�ɹ� false -ʧ��
     */

	virtual bool InitLibrary(string& pError);
    /*
     *  @brief		OpenUDPServer	 ��UDP����� 
     *  @param	[In]pPort  ����˰󶨵Ķ˿�
     *  @param	[In]pTimeOut  ��ʱʱ�� ��λMS
     *  @param	[Out]pError  �������
     *  @return		virtual XJHANDLE  ����˶����� �緵��NULL���ʧ��
     */

	virtual XJHANDLE OpenUDPServer( int pPort, int pTimeOut, string& pError );	
    /*
     *  @brief		CloseUDPServer	 �ر�UDP����� 
     *  @param	[In]pHandle  ����˶�����
     *  @param	[Out]pError  �������
     *  @return		virtual int 0-�ɹ� -1-ʧ��
     */

	virtual int CloseUDPServer(XJHANDLE pHandle, string& pError);	
    /*
     *  @brief		RecvMsgFrame	 ���ܱ�������֡ 
     *  @param	[In]pHandle  ����˶�����
     *  @param	[Out]pFrameData  ���յ��ı�������֡
     *  @param	[Out]pError  �������
     *  @return		virtual int 0-�ɹ� -1-ʧ��
     */		

	virtual int RecvMsgFrame(XJHANDLE pHandle, XJFRAMESTRUCT& pFrameData, string& pError);
    /*
     *  @brief		SendMsgFrame	 ���ͱ�������֡ 
     *  @param	[In]pDistIP  ���͵�Ŀ��IP��ַ
     *  @param	[In]pPort  ���͵�Ŀ�Ķ˿�
     *  @param	[In]pFrameData  �����͵ı�������֡
     *  @param	[Out]pError  �������
     *  @return		virtual int 0-�ɹ� -1-ʧ��
     */

	virtual int SendMsgFrame(string pDistIP, int pPort, const XJFRAMESTRUCT& pFrameData, string& pError);

    /*
     *  @brief		����103��������֡
     *  @param		[In]pHandle  ����˶�����
     *  @param		[Out]pFrameData  ���յ���103��������֡
     *  @param		[Out]pError  �������
     *  @return		int 0-�ɹ� -1-ʧ��
     */

	virtual int Recv103MsgFrame(XJHANDLE pHandle, XJ103FRAMESTRUCT& pFrameData, string& pError);

    /*
     *  @brief		����103��������֡
     *  @param		[In]pDistIP  ���͵�Ŀ��IP��ַ
     *  @param		[In]pPort  ���͵�Ŀ�Ķ˿�
     *  @param		[In]pFrameData  �����͵�103��������֡
     *  @param		[Out]pError  �������
     *  @return		int 0-�ɹ� -1-ʧ��
     */

	virtual int Send103MsgFrame(string pDistIP, int pPort, const XJ103FRAMESTRUCT& pFrameData, string& pError);
protected:
private:
	/** @brief				���ļ��ӹ����handle*/
	XJHANDLE				m_hLibXJMsgMonitor;
	
	/** @brief				OpenUDPServer����ָ��*/
	PFUNOPENUDPSERVER		m_pFunOpenUDPServer;
	
	/** @brief				CloseUDPServer����ָ��*/
	PFUNCLOSEUDPSERVER		m_pFunCloseUDPServer;
	
	/** @brief				RecvMsgFrame����ָ��*/
	PFUNRECVMSGFRAME		m_pFunRecvMsgFrame;

	/** @brief				SendMsgFrame����ָ��*/
	PFUNSENDMSGFRAME		m_pFunSendMsgFrame;

	/** @brief				Recv103MsgFrame����ָ��*/
	PFUNRECV103MSGFRAME		m_pFunRecv103MsgFrame;
	
	/** @brief				Send103MsgFrame����ָ��*/
	PFUNSEND103MSGFRAME		m_pFunSend103MsgFrame;
};

#endif // XJMsgMonitorWrapper_h__