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

/** @brief            报文监视共享库名称定义*/
#define LIB_MSGMONITOR_NAME              "libXJMsgMonitor.so"

#endif

#ifdef OS_WINDOWS

/** @brief            报文监视共享库名称定义*/
#define LIB_MSGMONITOR_NAME              "XJMsgMonitor.dll"

#endif

/** @brief            OpenUDPServer函数定义*/
typedef XJHANDLE (*PFUNOPENUDPSERVER)( int, int, string&);	

/** @brief            CloseUDPServer函数定义*/
typedef int (*PFUNCLOSEUDPSERVER)(XJHANDLE, string&);	

/** @brief            RecvMsgFrame函数定义*/
typedef int (*PFUNRECVMSGFRAME)(XJHANDLE, XJFRAMESTRUCT&, string&);

/** @brief            SendMsgFrame函数定义*/
typedef	int (*PFUNSENDMSGFRAME)(string, int, const XJFRAMESTRUCT&, string&);

/** @brief            Recv103MsgFrame函数定义*/
typedef	int (*PFUNRECV103MSGFRAME)(XJHANDLE, XJ103FRAMESTRUCT&, string&);

/** @brief            Send103MsgFrame函数定义*/
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
     *  @brief		InitLibrary	 加载动态库和各函数指针 
     *  @param	[Out]pError  出错提示
     *  @return		virtual bool true -成功 false -失败
     */

	virtual bool InitLibrary(string& pError);
    /*
     *  @brief		OpenUDPServer	 打开UDP服务端 
     *  @param	[In]pPort  服务端绑定的端口
     *  @param	[In]pTimeOut  超时时间 单位MS
     *  @param	[Out]pError  错误输出
     *  @return		virtual XJHANDLE  服务端对象句柄 如返回NULL则打开失败
     */

	virtual XJHANDLE OpenUDPServer( int pPort, int pTimeOut, string& pError );	
    /*
     *  @brief		CloseUDPServer	 关闭UDP服务端 
     *  @param	[In]pHandle  服务端对象句柄
     *  @param	[Out]pError  错误输出
     *  @return		virtual int 0-成功 -1-失败
     */

	virtual int CloseUDPServer(XJHANDLE pHandle, string& pError);	
    /*
     *  @brief		RecvMsgFrame	 接受报文数据帧 
     *  @param	[In]pHandle  服务端对象句柄
     *  @param	[Out]pFrameData  接收到的报文数据帧
     *  @param	[Out]pError  错误输出
     *  @return		virtual int 0-成功 -1-失败
     */		

	virtual int RecvMsgFrame(XJHANDLE pHandle, XJFRAMESTRUCT& pFrameData, string& pError);
    /*
     *  @brief		SendMsgFrame	 发送报文数据帧 
     *  @param	[In]pDistIP  发送的目的IP地址
     *  @param	[In]pPort  发送的目的端口
     *  @param	[In]pFrameData  待发送的报文数据帧
     *  @param	[Out]pError  错误输出
     *  @return		virtual int 0-成功 -1-失败
     */

	virtual int SendMsgFrame(string pDistIP, int pPort, const XJFRAMESTRUCT& pFrameData, string& pError);

    /*
     *  @brief		接受103报文数据帧
     *  @param		[In]pHandle  服务端对象句柄
     *  @param		[Out]pFrameData  接收到的103报文数据帧
     *  @param		[Out]pError  错误输出
     *  @return		int 0-成功 -1-失败
     */

	virtual int Recv103MsgFrame(XJHANDLE pHandle, XJ103FRAMESTRUCT& pFrameData, string& pError);

    /*
     *  @brief		发送103报文数据帧
     *  @param		[In]pDistIP  发送的目的IP地址
     *  @param		[In]pPort  发送的目的端口
     *  @param		[In]pFrameData  待发送的103报文数据帧
     *  @param		[Out]pError  错误输出
     *  @return		int 0-成功 -1-失败
     */

	virtual int Send103MsgFrame(string pDistIP, int pPort, const XJ103FRAMESTRUCT& pFrameData, string& pError);
protected:
private:
	/** @brief				报文监视共享库handle*/
	XJHANDLE				m_hLibXJMsgMonitor;
	
	/** @brief				OpenUDPServer函数指针*/
	PFUNOPENUDPSERVER		m_pFunOpenUDPServer;
	
	/** @brief				CloseUDPServer函数指针*/
	PFUNCLOSEUDPSERVER		m_pFunCloseUDPServer;
	
	/** @brief				RecvMsgFrame函数指针*/
	PFUNRECVMSGFRAME		m_pFunRecvMsgFrame;

	/** @brief				SendMsgFrame函数指针*/
	PFUNSENDMSGFRAME		m_pFunSendMsgFrame;

	/** @brief				Recv103MsgFrame函数指针*/
	PFUNRECV103MSGFRAME		m_pFunRecv103MsgFrame;
	
	/** @brief				Send103MsgFrame函数指针*/
	PFUNSEND103MSGFRAME		m_pFunSend103MsgFrame;
};

#endif // XJMsgMonitorWrapper_h__