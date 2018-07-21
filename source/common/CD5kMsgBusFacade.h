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
	* @brief			��������
	* @param[in]		void	 
	* @return			bool true-�ɹ� false-ʧ��
	*/
	virtual bool Start(void);
	/**
	* @brief			��������
	* @param[in]		void
	* @return   	   void
	*/
	virtual void End(void);
    /*
     *  @brief		RegisterPushMessageCallback	 ע��������Ϣ�ص����� 
     *  @param	[In]pFunction  �ص�����ָ��
     *  @param	[In]pParam  �ص�����֪ͨ����ָ��
     *  @return		virtual int 0-�ɹ� -1-ʧ��
     */

	virtual int RegisterPushMessageCallback(PFUNPUSHMESSAGE pFunction, LPVOID pParam);
	
    /*
     *  @brief		RegisterPullMessageCallback	 ע����ȡ��Ϣ�ص����� 
     *  @param	[In]pFunction  �ص�����ָ��
     *  @param	[In]pParam  �ص�����֪ͨ����ָ��
     *  @return		virtual int 0-�ɹ� -1-ʧ��
     */

	virtual int RegisterPullMessageCallback(PFUNPULLMESSAGE pFunction, LPVOID pParam);	
protected:
	/*
	*  @brief   	��־��¼ ���ⲿ����
	*  @param 		[In]a param of Type  char *  ��־����
	*  @param 		[In]a param of Type  int  ��־����
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
	/** @brief  			    �˳���־ */
	bool						m_bExit;

	/** @brief  			    �¼��� */
	int							m_iEventSet;

	/** @brief  			    ���̱�ʶ */
	int                         m_procKey;

	/** @brief  				PushMessage�ص�����ָ�� */
	PFUNPUSHMESSAGE				m_pFunOnPushMessage;
	
	/** @brief  				PushMessage�ص�����֪ͨ����ָ�� */
	LPVOID						m_pOnPushMessageObject;
	
	/** @brief  				PullMessage�ص�����ָ�� */
	PFUNPULLMESSAGE				m_pFunOnPullMessage;
	
	/** @brief  				PullMessage�ص�����֪ͨ����ָ�� */
	LPVOID						m_pOnPullMessageObject;	
	
	/** @brief					���մ����߳�Id*/
	THREAD_ID					m_RecvHandleThreadId;
	
	/** @brief					���մ����߳̾��*/
	THREAD_HANDLE				m_hRecvHandleThreadHandle;
	
	/** @brief					���ʹ����߳�Id*/
	THREAD_ID					m_SendHandleThreadId;
	
	/** @brief					���ʹ����߳̾��*/
	THREAD_HANDLE				m_hSendHandleThreadHandle;

	/** @brief  			    ������ */
	char						m_chProcessName[25];
	
	/** @brief  				������־���� */
	CLogFile&					m_rLogFile;
	
	/** @brief  				��Ϣ���߽ӿڶ��� */
	message_invocation          m_MsgBusObjecj; 
};


#endif // CD5kMsgBusFacade_h__