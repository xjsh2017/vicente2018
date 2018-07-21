// PushMsgProcess.h: interface for the CPushMsgProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUSHMSGPROCESS_H__BF2AD384_F6C0_4B45_BFE3_8114CDC91F81__INCLUDED_)
#define AFX_PUSHMSGPROCESS_H__BF2AD384_F6C0_4B45_BFE3_8114CDC91F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPushMsgProcess  
{
public:
	CPushMsgProcess();
	~CPushMsgProcess();
private:
	/** @brief			报文数据链表*/
	CPtrList m_listFullData;
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	BOOL m_bExit;
public:
	/*
	* @brief	AddSttpData	添加报文数据
	* @param	[In]a param of Type STTP_FULL_DATA * pSttpData	新添加的报文数据	
	* @return   int	添加操作完成后链表中的报文数据数量
	*/
	int AddSttpData(STTP_FULL_DATA* pSttpData);

	/*
	* @brief	TranslateSttpData 执行一次Sttp报文解析操作	
	* @return   void	
	*/
	void TranslateSttpData();

	/*
	* @brief	ThreadContinueRun	线程是否还要继续运行
	* @return   BOOL	
	*/
	BOOL ThreadContinueRun(){return !m_bExit;};

	void Quit(){m_bExit = TRUE;};

	BOOL IsAutoPushMsg(int uMsgID);

	BOOL IsAutoNotifyMsg(int uMsgID);
private:
	/*
	* @brief	RemoveFirstSttpData	删除链表中第一个报文数据		
	* @return   int	删除操作完成后链表中的报文数据数量
	*/
	int RemoveFirstSttpData();
	/*
	* @brief	GetFirstSttpData 取得链表中第一个报文数据	
	* @return   STTP_FULL_DATA*	链表中第一个报文数据
	*/
	STTP_FULL_DATA* GetFirstSttpData();
	/*
	* @brief	ClearSttpData 情况所有报文数据	
	* @return   void	
	*/
	void ClearSttpData();

	void Translate20002(STTP_FULL_DATA* pSttpData);
	void Translate20010(STTP_FULL_DATA* pSttpData);
	void Translate20018(STTP_FULL_DATA* pSttpData);
	void Translate20022(STTP_FULL_DATA* pSttpData);
	void Translate20079(STTP_FULL_DATA* pSttpData);
	void Translate20084(STTP_FULL_DATA* pSttpData);
	void Translate20132(STTP_FULL_DATA* pSttpData);
	void Translate20138(STTP_FULL_DATA* pSttpData);
	void Translate20144(STTP_FULL_DATA* pSttpData);
	void Translate20151(STTP_FULL_DATA* pSttpData);
	void Translate20155(STTP_FULL_DATA* pSttpData);
	void Translate20168(STTP_FULL_DATA* pSttpData);
	void Translate20176(STTP_FULL_DATA* pSttpData);
};

#endif // !defined(AFX_PUSHMSGPROCESS_H__BF2AD384_F6C0_4B45_BFE3_8114CDC91F81__INCLUDED_)
