#if !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
#define AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaultReport.h : header file
//
#include "XJNotify.h"
#include "FaultEvent.h"
#include "vccommon_define.h"
#include "DataEngine.h"
#include "DecisionReport.h"

/** @brief           �����¼�����*/

typedef CTypedPtrList<CObList , CFaultEvent*>	FAULTEVENT_LIST;

/////////////////////////////////////////////////////////////////////////////
// CFaultReport command target

class CFaultReport : public CXJNotify
{
public:
	CFaultReport(CDataEngine* pDataEngine);
	virtual ~CFaultReport();
// Attributes
public:
	/** @brief           ���ϰ�ID*/
	int			m_nID;
	/** @brief           �鵵����*/
	CString		m_sName;
	/** @brief           һ���豸ID*/
	CString		m_sPriDeviceID;
	/** @brief           ���ϰ�����ʱ��*/
	CTime       m_tmCreateTime;
	/** @brief           ����ʱ��*/
	CTime		m_tmFaultTime;
	/** @brief           ����ʱ�����ֵ*/
	int			m_nmsFaultTime;
	/** @brief           ���ϲ��*/
	CString		m_sFaultDistance;
	/** @brief           ��������*/
	int			m_nFaultType;
	/** @brief           ��վ1ID*/
	CString		m_sStation1ID;
	/** @brief           ��վ2ID*/
	CString		m_sStation2ID;
	/** @brief           �¼��ȼ�*/
	int			m_nEventLevel;

	/** @brief			���߰�����*/
	CDecisionReport*	m_pDecisionReport;
	/** @brief			�Ƿ����������Ϣ*/
	BOOL m_bLoadDecision;

	/** @brief           ��վ1ָ��*/
	CStationObj*	m_pStation1;
	/** @brief           ��վ2ָ��*/
	CStationObj*	m_pStation2;
	/** @brief           һ���豸ָ��*/
	CDeviceObj*		m_pPriDevice;

	/** @brief           ��ϸ��Ϣ�Ƿ��ѱ�����*/
	BOOL			m_bLoadDetail;

	/** @brief           �����������¼�����*/
	CPtrArray		m_listChr;
	/** @brief           �����ź����¼�����*/
	CPtrArray		m_listSign;
	/** @brief           �澯�¼�����*/
	CPtrArray		m_listAlarm;
	/** @brief           ����¼������*/
	CPtrArray		m_listPTOSC;
	/** @brief           ¼����¼�������*/
	CPtrArray		m_listWROSC;
	/** @brief           ¼���б�����*/
	CPtrArray		m_listOSCList;

	/** @brief           �����豸����*/
	CPtrArray		m_listSec;
	/** @brief           ��ʱ�����ź�������*/
	CPtrArray		m_listTempSign;
	/** @brief           ��ʱ����������*/
	CPtrArray		m_listTempChr;
	/** @brief           ��ʱ����¼������*/
	CPtrArray		m_listTempPTOSC;
	/** @brief           ��¼��ʱ����ʱ��������ĸ��豸*/
	CSecObj*		m_pTempSec;

	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL			m_bShowReturn;
	/** @brief           �Ƿ�浵*/
	BOOL            m_bIsSaved;
    /** @brief           ����ԭ��*/    
	CString         m_sFaultReason;

	/** @brief			�������й�������Ƿ����*/
	BOOL			m_bFilterNULL;
	/** @brief			�Ƿ���˹��ϲ��=0*/
	BOOL			m_bFilterDistance;
	/** @brief			�Ƿ����III����Ϣ*/
	BOOL			m_bFilterIII;
// Operations
public:
	
	virtual int GetNotifyType();
/*
 *  @brief   	LoadFromDB	 �����ݿ���������ϰ����� 
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
 *  @brief   	LoadDetail	 ������ϱ�����ϸ��Ϣ 
 *  @param 		[In]a param of Type  BOOL  �Ƿ�������û�б����붼��������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDetail(BOOL bForce = FALSE);

/*
 *  @brief   	ClearDetail	 �����ϸ��Ϣ
 *  @return 	void 
 */
	void ClearDetail();

/*
 *  @brief   	GetFaultType	 �õ��������͵��������� 
 *  @return 	CString ����������������
 */
	CString GetFaultTypeString(int nFaultType);

/*
 *  @brief   	SaveToDB	 �������ݵ����ݿ� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL SaveToDB();

/*
 *  @brief   	MultiSaveEvent	 ���������¼� 
 *  @return 	int ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 */
	int MultiSaveEvent();

/*
 *  @brief   	DeleteAllEventFromDB	 �����ݿ���ɾ�������¼� 
 *  @return 	BOOL ɾ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL DeleteAllEventFromDB();

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL LoadFromSTTP(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetFaultTypeInt	 ���������ʹ��ַ���תΪ���� 
 *  @param 		[In]a param of Type  CString  ���������ַ���
 *  @return 	int �������ͱ�ʶ
 */
	int GetFaultTypeInt(CString sType);

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

/*
 *  @brief   	BuildTempList	 ָ�������豸������ʱ���� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	void BuildTempList(CSecObj* pSec);

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
	* @brief	LoadDecision	���븨��������Ϣ
	* @return   BOOL	
	*/
	BOOL LoadDecision();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
