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
	/** @brief			������������*/
	CPtrList m_listFullData;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	BOOL m_bExit;
public:
	/*
	* @brief	AddSttpData	��ӱ�������
	* @param	[In]a param of Type STTP_FULL_DATA * pSttpData	����ӵı�������	
	* @return   int	��Ӳ�����ɺ������еı�����������
	*/
	int AddSttpData(STTP_FULL_DATA* pSttpData);

	/*
	* @brief	TranslateSttpData ִ��һ��Sttp���Ľ�������	
	* @return   void	
	*/
	void TranslateSttpData();

	/*
	* @brief	ThreadContinueRun	�߳��Ƿ�Ҫ��������
	* @return   BOOL	
	*/
	BOOL ThreadContinueRun(){return !m_bExit;};

	void Quit(){m_bExit = TRUE;};

	BOOL IsAutoPushMsg(int uMsgID);

	BOOL IsAutoNotifyMsg(int uMsgID);
private:
	/*
	* @brief	RemoveFirstSttpData	ɾ�������е�һ����������		
	* @return   int	ɾ��������ɺ������еı�����������
	*/
	int RemoveFirstSttpData();
	/*
	* @brief	GetFirstSttpData ȡ�������е�һ����������	
	* @return   STTP_FULL_DATA*	�����е�һ����������
	*/
	STTP_FULL_DATA* GetFirstSttpData();
	/*
	* @brief	ClearSttpData ������б�������	
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
