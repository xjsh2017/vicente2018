#if !defined(AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_)
#define AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaultEvent.h : header file
//
#include "DeviceObj.h"
#include "DataEngine.h"
#include "vccommon_define.h"

class CFaultEvent;
/** @brief           ��ʷ�¼�����*/
//##ModelId=49B87B90014A
typedef CTypedPtrList<CObList , CFaultEvent*>	HISTORYEVENT_LIST;

/////////////////////////////////////////////////////////////////////////////
// CFaultEvent

/**
 * @defgroup 	CFaultEvent	���ϱ����¼���
 * @{
 */
/**
 * @brief       �������ϱ�����¼���Ϣ
*  @author      LYH
 * @version     ver1.0
 * @date        24/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87B9A01C5
class CFaultEvent : public CObject
{
	DECLARE_DYNCREATE(CFaultEvent)
public:
	//##ModelId=49B87B9A01C7
	CFaultEvent();           // protected constructor used by dynamic creation
	CFaultEvent(CDataEngine* pDataEngine);
	//##ModelId=49B87B9A01C8
	virtual ~CFaultEvent();
// Attributes
public:
	CDataEngine*	m_pDataEngine;
	/** @brief           ֵ�ĵ�λ*/
	CString		m_strUnit;
	/** @brief           ���ϱ����¼�����*/
	//##ModelId=49B87B9A01CA
	int			m_nType;
	/** @brief           �������е�ID*/
	//##ModelId=49B87B9A01D4
	int			m_nID;
	/** @brief           ��Ӧ�Ĺ�����Ϣ��ID*/
	//##ModelId=49B87B9A01D5
	int			m_nMainRecID;
	/** @brief           �����豸ID*/
	//##ModelId=49B87B9A01D6
	CString		m_sSecID;
	/** @brief           ����ʱ��*/
	//##ModelId=49B87B9A01D7
	CTime		m_tmStart;
	/** @brief           ����ʱ�����ֵ*/
	//##ModelId=49B87B9A01D8
	int			m_nmsStart;
	/** @brief           ����ʱ��*/
	//##ModelId=49B87B9A01D9
	CTime		m_tmReceiveTime;
	/** @brief           ����ʱ�����ֵ*/
	//##ModelId=49B87B9A01E4
	int			m_nmsReceive;
	/** @brief           �¼�����*/
	//##ModelId=49B87B9A01E5
	CString		m_sEventDef;
	/** @brief           �¼�ֵ*/
	//##ModelId=49B87B9A01E6
	CString		m_sEventContent;
	/** @brief           �¼���������*/
	//##ModelId=49B87B9A01E7
	CString		m_sEventDefName;
	/** @brief           �ź�ʱ��*/
	//##ModelId=49B87B9A01E8
	CTime		m_tmSign;
	/** @brief           �ź�ʱ�����ֵ*/
	//##ModelId=49B87B9A01E9
	int			m_nmsSign;
	/** @brief           ��Ӧ��CPU��*/
	int			m_nCPU;

	/** @brief           �¼������Ķ����豸ָ��*/
	//##ModelId=49B87B9A01F5
	CSecObj*	m_pSec;

	/** @brief           ��ѡ���־,�����¼���*/
	//##ModelId=49B87B9A01F9
	BOOL m_bSelected;

	/** @brief           �����¼���,������ʷ�¼���*/
	int		m_nNum;

	/** @brief           ¼���ļ����Ŀ¼*/
	CString m_sDir; 

	/** @brief           �ɵ�¼���ļ����Ŀ¼*/
	CString m_sOldDir; 

public:
/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  CMemSet*  ���ݼ�
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9A01FA
	BOOL LoadFromDB(CMemSet* pMemset);

/*
 *  @brief   	BuildReleation	 �������豸�Ĺ�ϵ 
 *  @return 	BOOL �����豸��������TRUE, ���ܹ�������FALSE
 */
	//##ModelId=49B87B9A01FC
	BOOL BuildReleation();

/*
 *  @brief   	FindEventDefName	 �����¼���������� 
 *  @return 	BOOL ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9A01FD
	BOOL FindEventDefName();

/*
 *  @brief   	MultiSaveData	 ��֯������������
 *  @param 		[Out]a param of Type  STTP_DBDATA_RECORD&  ���ݼ�¼
 *  @return 	BOOL ��֯�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9A01FE
	BOOL MultiSaveData(STTP_DBDATA_RECORD& sttpDB);

/*
 *  @brief   	Clone	 ���ƹ����¼� 
 *  @return 	CFaultEvent* �������ɵĹ����¼�
 */
	//##ModelId=49B87B9A0204
	CFaultEvent* Clone();

/*
 *  @brief   	GetFaultType	 ���������ͽ��ͳ����� 
 *  @param 		[In]a param of Type  CString  ��������
 *  @return 	CString ���ͺ������
 */
	CString GetFaultType(CString sType);

/*
 *  @brief   	IsReturnEvent	 �Ƿ񷵻���Ϣ 
 *  @return 	BOOL �Ƿ�����Ϣ����TRUE,���Ƿ�����Ϣ����FALSE
 */
	BOOL IsReturnEvent();

/*
 *  @brief   	GetEventProperty	 ȡ���¼��ȼ� 
 *  @return 	int �¼��ȼ�,Ĭ�Ϸ���0��0-I�࣬1-II�࣬2-III��
 */
	int GetEventProperty();

private:
/*
 *  @brief   	GetActChrsConfig	 �õ�ָ�����¼������� 
 *  @param 		[In]a param of Type  int  ������ID
 *  @return 	PT_ActionCharacter * ����������
 */
	//##ModelId=49B87B9A0205
	PT_ActionCharacter * GetActChrsConfig(int nID);

/*
 *  @brief   	GetActChrsFromUniversal	 ��ͨ���������в������������� 
 *  @param 		[In]a param of Type  int  ������ID
 *  @return 	PT_ActionCharacter * ����������
 */
	//##ModelId=49B87B9A0207
	PT_ActionCharacter * GetActChrsFromUniversal(int nID);

/*
 *  @brief   	GetSignalConfig	 �õ�ָ�����¼��ź��� 
 *  @param 		[In]a param of Type  int  �ź���ID
 *  @return 	PT_Signal * �ź�������
 */
	//##ModelId=49B87B9A0209
	PT_Signal * GetSignalConfig(int nID);

/*
 *  @brief   	GetAlarmConfig	 �õ�ָ���ĸ澯���ź��� 
 *  @param 		[In]a param of Type  int  �ź���ID
 *  @return 	PT_Signal * �ź�������
 */
	//##ModelId=49B87B9A020B
	PT_Signal * GetAlarmConfig(int nID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_)
