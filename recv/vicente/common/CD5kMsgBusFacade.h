/********************************************************************
	created:	2012/05/19
	created:	19:5:2012   15:16
	filename: 	CD5kMsgBusFacade.h
	file path:	
	file base:	CD5kMsgBusFacade
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#ifndef CD5kMsgBusFacade_h__
#define CD5kMsgBusFacade_h__

#include "../../common/LogFile.h"
#include "msg_api/message_inv.h"

typedef bool (*PFUNPUSHMESSAGE)(const Message& ,LPVOID);

typedef bool (*PFUNPULLMESSAGE)(Message& ,LPVOID);

class CD5kMsgBusFacade
{
public:
	CD5kMsgBusFacade(string pProcName, int pEventSet, CLogFile& pLogFile);
	~CD5kMsgBusFacade(); 
	/**
	* @brief			启动服务
	* @param[in]		void	 
	* @return			bool true-成功 false-失败
	*/
	virtual bool Start(void);
	/**
	* @brief			结束服务
	* @param[in]		void
	* @return   	   void
	*/
	virtual void End(void);
    /*
     *  @brief		RegisterPushMessageCallback	 注册推送消息回调函数 
     *  @param	[In]pFunction  回调函数指针
     *  @param	[In]pParam  回调函数通知对象指针
     *  @return		virtual int 0-成功 -1-失败
     */

	virtual int RegisterPushMessageCallback(PFUNPUSHMESSAGE pFunction, LPVOID pParam);
	
    /*
     *  @brief		RegisterPullMessageCallback	 注册拉取消息回调函数 
     *  @param	[In]pFunction  回调函数指针
     *  @param	[In]pParam  回调函数通知对象指针
     *  @return		virtual int 0-成功 -1-失败
     */

	virtual int RegisterPullMessageCallback(PFUNPULLMESSAGE pFunction, LPVOID pParam);	
protected:
	/*
	*  @brief   	日志记录 供外部调用
	*  @param 		[In]a param of Type  char *  日志描述
	*  @param 		[In]a param of Type  int  日志级别
	*  @return 		void 
		*/
    void			WriteLog(const char * pLog,int nLevel);
private:
	bool	PushMessage(const Message& pMsg);
	bool	PullMessage(Message& pMsg);

	bool	StartRecvHandleThread( void );
	void	EndRecvHandleThread( void );
	static THREAD_FUNC WINAPI RecvHandleThreadFunc( LPVOID pParam );
	void	SetRecvHandleThreadHandle( THREAD_HANDLE pHandle );
	int		RecvHandleLoop();
	
	bool	StartSendHandleThread( void );
	void	EndSendHandleThread( void );
	static THREAD_FUNC WINAPI SendHandleThreadFunc( LPVOID pParam );
	void	SetSendHandleThreadHandle( THREAD_HANDLE pHandle );
	int		SendHandleLoop();
	void	SendExitToRecvPort();
private:
	/** @brief  			    退出标志 */
	bool						m_bExit;

	/** @brief  			    事件集 */
	int							m_iEventSet;

	/** @brief  			    进程标识 */
	int                         m_procKey;

	/** @brief  				PushMessage回调函数指针 */
	PFUNPUSHMESSAGE				m_pFunOnPushMessage;
	
	/** @brief  				PushMessage回调函数通知对象指针 */
	LPVOID						m_pOnPushMessageObject;
	
	/** @brief  				PullMessage回调函数指针 */
	PFUNPULLMESSAGE				m_pFunOnPullMessage;
	
	/** @brief  				PullMessage回调函数通知对象指针 */
	LPVOID						m_pOnPullMessageObject;	
	
	/** @brief					接收处理线程Id*/
	THREAD_ID					m_RecvHandleThreadId;
	
	/** @brief					接收处理线程句柄*/
	THREAD_HANDLE				m_hRecvHandleThreadHandle;
	
	/** @brief					发送处理线程Id*/
	THREAD_ID					m_SendHandleThreadId;
	
	/** @brief					发送处理线程句柄*/
	THREAD_HANDLE				m_hSendHandleThreadHandle;

	/** @brief  			    进程名 */
	char						m_chProcessName[25];
	
	/** @brief  				流程日志对象 */
	CLogFile&					m_rLogFile;
	
	/** @brief  				消息总线接口对象 */
	message_invocation          m_MsgBusObjecj; 
};


#endif // CD5kMsgBusFacade_h__