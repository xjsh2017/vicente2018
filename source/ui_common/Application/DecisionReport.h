#if !defined(AFX_DECISIONREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
#define AFX_DECISIONREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "XJNotify.h"
#include "vccommon_define.h"
#include "DataEngine.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CFaultReport command target

class CDecisionAppraise : public CObject
{
public:
	CDecisionAppraise(CDataEngine* pDataEngine, CString sDeviceID, CString sContent, int nDeviceType = TYPE_PROTECT);
	virtual ~CDecisionAppraise();

private:
	CDataEngine*	m_pDataEngine;
	/** @brief			�豸ID*/
	CString	m_sDeviceID;
	/** @brief			��������*/
	CString	m_sContent;
	/** @brief			�豸����*/
	int	m_nDeviceType;
	/** @brief			��Ӧ�豸*/
	CDeviceObj*	m_pDevice;

public:
	/*
	* @brief	GetDeviceName	ȡ���豸����
	* @return   CString	�豸����
	*/
	CString GetDeviceName();

	/*
	* @brief	GetAppraise	ȡ������
	* @return   CString	��������
	*/
	CString GetAppraise();

	/*
	* @brief	GetAppraiseInt	ȡ��INT���͵�����
	* @return   int	
	*/
	int GetAppraiseInt();
private:
/*
 *  @brief   	BuildReleation	 �������豸�Ĺ��� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildReleation();
};

class CDecisionReport : public CXJNotify
{
public:
	CDecisionReport(CDataEngine* pDataEngine);
	virtual ~CDecisionReport();
// Attributes
public:
	/** @brief           ���߰�ID*/
	int			m_nID;
	/** @brief			��Ӧ�Ĺ��ϰ�ID*/
	int			m_nFaultID;
	/** @brief			��ѹ�ȼ�*/
	int			m_nVoltage;
	/** @brief			��������*/
	CString		m_sFaultType;
	/** @brief			����ԭ��*/
	CString		m_sFaultReason;
	/** @brief			���ι��ϳ���ʱ�䣬��λ:��*/
	int			m_nFirstTime;
	/** @brief			�غ�բ���ɹ�ʱ���ϳ���ʱ��,��λ:��*/
	int			m_nSecondTime;
	/** @brief			���ϵ���*/
	float		m_fFaultCurrent;
	/** @brief			��������*/
	CString		m_sFaultDescribe;
	/** @brief			¼�����*/
	CString		m_sWaveDescribe;
	/** @brief			�����豸����*/
	CString		m_sProtectJudge;
	/** @brief			��·����Ϊ����*/
	CString		m_sBreakJudge;
	/** @brief			�Բ߽���*/
	CString		m_sFaultAdvise;

	/** @brief			¼����������*/
	CObArray m_listWave;
	/** @brief			¼��ȱ������*/
	CObArray m_listWaveLack;
	/** @brief			������Ϊ��������*/
	CObArray m_listProtect;
	/** @brief			��������ȱ������*/
	CObArray m_listProtectLack;
	/** @brief			��·����Ϊ��������*/
	CObArray m_listBreak;

	/** @brief           �����ź����¼�����*/
	CPtrArray		m_listSign;

	/** @brief           �����豸����*/
	CPtrArray		m_listSec;

	/** @brief           ��ʱ�����ź�������*/
	CPtrArray		m_listTempSign;
	/** @brief           ��¼��ʱ����ʱ��������ĸ��豸*/
	CSecObj*		m_pTempSec;

	/** @brief			����Ԫ����*/
	CString			m_sFaultDevice;
	/** @brief			������λ*/
	CString			m_sFaultPhase;
	/** @brief			���ϲ��*/
	CString			m_sFaultDistance;

// Operations
public:
	
	virtual int GetNotifyType();
/*
 *  @brief   	LoadFromDB	 �����ݿ�������
 *  @param 		[In]a param of Type  CMemSet*  ���ݼ�ָ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL LoadFromDB(CMemSet* pMemset);

/*
 *  @brief   	BuildReleation	 �������豸�Ĺ��� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL BuildReleation();

/*
 *  @brief   	GetDescribe	 ����¼����� 
 *  @return 	virtual CString �¼������ַ���
 */
	virtual CString GetDescribe();

/*
 *  @brief   	GetSimpleDescribe	 ����¼���Ҫ���� 
 *  @return 	virtual CString �¼���Ҫ�����ַ���
 */
	virtual CString GetSimpleDescribe();

/*
 *  @brief   	BuildTempList	 ָ�������豸������ʱ���� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	void BuildTempList(CSecObj* pSec);
private:
	/*
	* @brief	ClearWaveDescribe	���¼����������
	* @return   void	
	*/
	void ClearWaveDescribe();

	/*
	* @brief	ClearProtectJudge	��ձ�����������
	* @return   void	
	*/
	void ClearProtectJudge();

	/*
	* @brief	ClearBreakJudge	��ն�·����������
	* @return   void	
	*/
	void ClearBreakJudge();

	/*
	* @brief	ParseWaveDescribe	����¼�����
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseWaveDescribe(CString sStr);

	/*
	* @brief	ParseProtectJudge	������������
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseProtectJudge(CString sStr);

	/*
	* @brief	ParseBreakJudge	������·������
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseBreakJudge(CString sStr);

	/*
 *  @brief   	LoadDetail	 ������ϱ�����ϸ��Ϣ 
 *  @param 		[In]a param of Type  BOOL  �Ƿ�������û�б����붼��������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDetail(BOOL bForce = FALSE);

	/*
	* @brief	ClearDetail	����¼���Ϣ
	* @return   void	
	*/
	void ClearDetail();

/*
 *  @brief   	GetFaultTypeLetter	 ���������ʹ�����תΪ��ĸ��ʽ 
 *  @param 		[In]a param of Type  int  �������ʹ���
 *  @return 	CString ���������ַ���
 */
	CString GetFaultTypeLetter(int nType);

/*
 *  @brief   	SortActionSign	 �ź���ʱ��ͺ���ֵ�������ź��б� 
 *  @return 	void 
 */
	void SortActionSign();

/*
 *  @brief   	CompareSign	 �Ƚ����������¼��Ķ����ź�ʱʱ���С 
 *  @param 		[In]a param of Type  CFaultEvent*  �����¼�1
 *  @param 		[In]a param of Type  CFaultEvent*  �����¼�2
 *  @return 	int 1>2����>0, 1=2����0, 1<2����<0
 */
	int CompareSign(CFaultEvent* pEvent1, CFaultEvent* pEvent2);

/*
 *  @brief   	AddSec	 ���豸���뵽�����豸���� 
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @return 	void 
 */
	void AddSec(CDeviceObj* pObj);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
