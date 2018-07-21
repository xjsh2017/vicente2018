// ***************************************************************
//  Event   version:  1.0    date: 14:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ���¼��ඨ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(EVENT_H)
#define EVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Deviceobj.h"
// Event.h : header file

/**
 * @defgroup 	CSTEvent	�¼�����
 * @{
 */
/**
 * @brief       ���ڸ��¼�����̳�,�ṩ�¼���ͬ������
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87B9A02CE
class CXJEvent : public CObject
{
public:	
	
	//##ModelId=49B87B9A02DE
	CXJEvent();
	//##ModelId=49B87B9A038A
	virtual ~CXJEvent();

	/** @brief           �Ƿ��ǵ���״̬*/
	//##ModelId=49B87B9B003E
	int			m_IsDebug;
	/** @brief           �¼�����Ӧ��΢��װ��ָ��(�����豸)*/
	//##ModelId=49B87B9B004F
	CSecObj *	m_pSec;
	/** @brief           ��Ӧ���ݱ��ID*/
	//##ModelId=49B87B9B005D
	long		m_lID;
	/** @brief           �¼���Ϣ*/
	//##ModelId=49B87B9B005E
	CString		m_sMsg;			//�¼���Ϣ
	/** @brief           ��������*/
	//##ModelId=49B87B9B006D
	CString		m_sNetName;
	/** @brief           ��վ����*/
	//##ModelId=49B87B9B007D
	CString		m_sStationName;     
	/** @brief           ��վID*/
	//##ModelId=49B87B9B007E
	CString		m_sStationId;
	/** @brief           һ���豸����*/
	//##ModelId=49B87B9B008C
	CString		m_sDeviceName;
	/** @brief           �����豸����*/
	//##ModelId=49B87B9B008D
	CString		m_sSecName;
	/** @brief           �����豸ID*/
	//##ModelId=49B87B9B009C
	CString		m_sSecID;
	/** @brief            һ���豸��ID,������Ϣ�Ĺ�����ѯ*/
	//##ModelId=49B87B9B00AB
	CString		m_sDeviceId;
	/** @brief           �Ƿ�ȷ�ϣ�0-δȷ�ϣ�1-��ȷ�ϣ�*/
	//##ModelId=49B87B9B00AC
	int			m_nIsAck;
	/** @brief           ����ʱ��*/
	//##ModelId=49B87B9B00BB
	CTime		m_tmTime;
	/** @brief           ����ֵ*/
	//##ModelId=49B87B9B00BC
	int			m_nms;
	/** @brief           ��������*/
	//##ModelId=49B87B9B00CB
	CString		m_sFaultType;
	/** @brief           �¼�����. 0��ʾI��, 1��ʾII��, 2��ʾIII��*/
	//##ModelId=49B87B9B00DA
	int			m_nProperty;


	/** @brief           �������ʱ��*/
	//##ModelId=49B87B9B00DB
	CTime		m_tmTime2;	 
	/** @brief           �������ʱ��ĺ���ֵ*/
	//##ModelId=49B87B9B00EA
	int			m_nms2;

	CTime		m_tmMainReceiveTime;//��վ����ʱ��

	//�¼����Ͷ���
	// XJ_EVENT_ACTION	= 0;	//��������
	// XJ_EVENT_ALARM	= 1;	//�����澯
	// XJ_EVENT_FAULT	= 2;	//�¹ʱ���
	// XJ_EVENT_OSC		= 3;	//¼��
	// XJ_EVENT_DI		= 4;	//��������λ��Ϣ
	// XJ_EVENT_PTCOMM	= 5;	//����ͨѶ״̬
	// XJ_EVENT_PTRUN	= 6;	//��������״̬
	// XJ_EVENT_SYSALARM = 7;	//ϵͳ�澯
	/** @brief           �¼�����(0:��������, 1:�����澯, 2:�¹ʱ���, 3:¼��, 4:������, 5:����ͨѶ״̬, 6:��������״̬, 7:ϵͳ�澯)*/
	//##ModelId=49B87B9B00FA
	int			m_nEventType;

	/** @brief           �Ƿ���������ϸ��Ϣ, TRUE��ʾ������, FALSE��ʾδ����*/
	//##ModelId=49B87B9B00FB
	BOOL		m_bLoadDetail;

	/** @brief           �Ƿ񷵻���Ϣ��־��0-���Ƿ�����Ϣ��1-�Ƿ�����Ϣ*/
	int			m_nIsReturn;
/*
 *  @brief   	GetDevice	 �õ��¼���Ӧ��һ���豸 
 *  @return 	�¼�����Ӧ��һ���豸ָ��
    virtual CDeviceObj* 
 */
	//##ModelId=49B87B9B0109
	virtual CDeviceObj* GetDevice()	{return (m_pSec!=NULL)?m_pSec->m_pOwner:NULL; }

/*
 *  @brief   	Serialize	 �������¼� 
 *  @param 		[In]a param of Type  CArchive &  �����������
 *  @return 	virtual void 
 */
	//##ModelId=49B87B9B010B
	virtual	void Serialize(CArchive &ar);	

/*
 *  @brief   	Ack	 �¼�ȷ�ϣ�����������ʵ��
 *  @return 	virtual BOOL  TRUE-�ɹ���FALSE-ʧ��
 */
	//##ModelId=49B87B9B01B5
	virtual	BOOL Ack(){return FALSE;};	

/*
 *  @brief   	GetDescribe	 ����¼����� 
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87B9B01B7
	virtual CString GetDescribe();	

/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87B9B0261
	virtual CXJEvent* Clone()=0;	 

/*
 *  @brief   	GetFaultType	 ���������ͽ��ͳ�����
 *  @return 	virtual CString	 ���ͺ������
 */
	//##ModelId=49B87B9B0263
	virtual CString GetFaultType();

/*
 *  @brief   	GetTriggerType	 �õ����˵�� 
 *  @param 		[In]a param of Type  CString  �����д
 *  @return 	virtual CString ���˵��
 */
	//##ModelId=49B87B9B030D
	virtual CString GetTriggerType(CString sTrigger);

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9B03A9
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9C006D
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);


/*
 *  @brief   	BuildReleation	 �����¼����豸�Ĺ�ϵ 
 *  @param 		[In/Out]a param of Type  int  �����豸����
 *  @return 	virtual BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9C0109
	virtual BOOL BuildReleation(int nType);

/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	BOOL ��ѯʧ�ܷ���FALSE, �ɹ�����TRUE
 */
	//##ModelId=49B87B9C01B5
	virtual BOOL QueryNewEventID();

/*
 *  @brief   	IsReturnEvent	 �ж��Ƿ�"����"�¼�
 *  @return 	BOOL 
 */
	virtual BOOL IsReturnEvent();
	
	
	//���������������ź�����λ��Ϊ���������
	//��ʽΪ��
	//������������Ŀ������������1ID������������1ֵ����������������N1��ID������������N1ֵ��
	//�ź�����Ŀ��������1ID��������1ֵ������������N2��ID��������N2ֵ��
	/** @brief           ���¼��漰�Ĺ����������б�*/
	//##ModelId=49B87B9C0252
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_ActChrs;
	/** @brief           ���¼��漰���ź����б�*/
	//##ModelId=49B87B9C0262
	CTypedPtrArray<CPtrArray,PT_Signal*> m_Signals;

protected:
/*
 *  @brief   	Clear	 ���������� 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B9C0271
	BOOL Clear();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventAlarm �澯�¼�
/**
 * @defgroup 	CXJEventAlarm	�澯�¼���
 * @{
 */
/**
 * @brief       �����澯�¼�
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87B9C038A
class CXJEventAlarm : public CXJEvent
{
public:
	//##ModelId=49B87B9C039A
	CXJEventAlarm();
	//##ModelId=49B87B9D004E
	CXJEventAlarm(CXJEventAlarm* pEvent);
	//##ModelId=49B87B9D00DA
	~CXJEventAlarm();
public:		 
	/** @brief           �����澯�¼���Դ*/
	//##ModelId=49B87B9D0177
	long	m_lAlarmID;
	/** @brief           �����澯�¼���Դ��ֵ��0-���أ�1-����*/
	//##ModelId=49B87B9D0178
	int		m_nValue;   
	int     m_nCount;
public:
/*
 *  @brief   	TranslateFData	 �����¼�FData�ֶεõ��¼���ϸ��Ϣ
 *  @return 	BOOL ���ͳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9D0186
	BOOL TranslateFData();

/*
 *  @brief   	TranslateSttpData	 �����澯�������� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *	@param		[In]a param of Type  ָ���Ǳ��������еĵڼ���������
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9D0203
	BOOL TranslateSttpData(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	Ack	 ȷ���¼� 
 *  @return 	virtual BOOL ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9D0290
	virtual	BOOL Ack();
	
/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87B9D031C
	virtual CString GetDescribe(); 

/*
 *  @brief   	Clone	 �����¼������������� 
 *  @return 	virtual CSTEvent* �������ɵĶ���
 */
	//##ModelId=49B87B9D03B9
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetAlarmType	 ȡ�ø澯�¼���� 
 *  @return 	CString �澯�¼����,I,II,III
 */
	//##ModelId=49B87B9E005D
	CString GetAlarmType();

/*
 *  @brief   	IsReturnEvent	 �ж��Ƿ�"����"�¼�
 *  @return 	BOOL 
 */
	virtual BOOL IsReturnEvent();
protected:
/*
 *  @brief   	GetSignalConfig	 �õ�ָ����Ϣ�� 
 *  @param 		[In/Out]a param of Type  int  ��Ϣ��ID
 *  @return 	PT_Signal * ��Ϣ�Ŷ���
 */
	//##ModelId=49B87B9E00DA
	PT_Signal * GetSignalConfig(int nID){ if(m_pSec) return m_pSec->FindAlarmSignal(nID); return NULL; }

public:
/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9E0157
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *	@param		[In]a param of Type	 ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9E01E4
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	void 
 */
	//##ModelId=49B87B9E0261
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventAction �����¼�
/**
 * @defgroup 	CSTEventAction	�����¼���
 * @{
 */
/**
 * @brief       ���������¼�
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08

/** @} */ //OVER
//##ModelId=49B87B9E033C
class CXJEventAction : public CXJEvent
{
public:
	//##ModelId=49B87B9E034B
	CXJEventAction();
	//##ModelId=49B87B9E03B9
	CXJEventAction(CXJEventAction* pEvent);
	//##ModelId=49B87B9F003E
	~CXJEventAction();
public:
	/** @brief           �������ݣ���ӦTB_PT_ACTION��FDATA�ֶ�*/
	//##ModelId=49B87B9F00AB
	CString		m_FData;
	
	int         m_nCount;

	/** @brief			���ʱ��*/
	int			m_nRelativeTime;

public:
/*
 *  @brief   	Ack	 ȷ���¼� 
 *  @return 	virtual BOOL ȷ�ϳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F00BB
	virtual	BOOL Ack();	 

/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ���� 
 *  @return 	virtual CString �¼���ϸ����������ַ���
 */
	//##ModelId=49B87B9F0128
	virtual CString GetDescribe();	 

/*
 *  @brief   	Clone	 �����¼����� 
 *  @return 	virtual CSTEvent* ���Ƶ��¼�����
 */
	//##ModelId=49B87B9F0196
	virtual CXJEvent* Clone();

/*
 *  @brief   	TranslateFData	 ����FData���� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F01F4
	BOOL TranslateFData();

/*
 *  @brief   	TranslateSttpData	 ����STTP��������Ϣ�ź�������������
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F0261
	BOOL TranslateSttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetAlarmType	 ȡ�ù�������,I,II,III�� 
 *  @return 	CString 
 */
	//##ModelId=49B87B9F02BF
	CString	GetAlarmType();

	/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F030D
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *	@param		[In]a param of Type	 int	ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F036B
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);
private:
/*
 *  @brief   	RefreshConfig	 ˢ�¶�Ӧ�����豸���ź����б� 
 *  @return 	BOOL ˢ�³ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9F03B9
	BOOL RefreshConfig(){ if(!m_pSec) return FALSE; m_pSec->RefreshConfig(); return TRUE;};

/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA0002E
	BOOL QueryNewEventID();

protected:
/*
 *  @brief   	GetActChrsConfig	 �õ�ָ�����¼������� 
 *  @param 		[In]a param of Type  int  ������ID
 *  @return 	PT_ActionCharacter * ����������
 */
	//##ModelId=49B87BA0007D
	PT_ActionCharacter * GetActChrsConfig(int nID){ if(m_pSec) return m_pSec->FindActionCharacter(nID); return NULL; };

/*
 *  @brief   	GetActChrsFromUniversal	 ��ͨ���������в������������� 
 *  @param 		[In]a param of Type  int  ������ID
 *  @return 	PT_ActionCharacter * ����������
 */
	//##ModelId=49B87BA000DA
	PT_ActionCharacter * GetActChrsFromUniversal(int nID);

/*
 *  @brief   	GetSignalConfig	 �õ�ָ�����¼��ź��� 
 *  @param 		[In]a param of Type  int  �ź���ID
 *  @return 	PT_Signal * �ź�������
 */
	//##ModelId=49B87BA00128
	PT_Signal * GetSignalConfig(int nID){ if(m_pSec) return m_pSec->FindActionSignal(nID); return NULL; };
};

//////////////////////////////////////////////////////////////////////////
//CXJEventFault �¹ʱ���

//////////////////////////////////////////////////////////////////////////
//CXJEventDI	������
/**
 * @defgroup 	CXJEventDI	��������λ�¼���
 * @{
 */
/**
 * @brief       ������������λ�¼�
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA001C5
class CXJEventDI : public CXJEvent
{
public:
	//##ModelId=49B87BA001D4
	CXJEventDI();
	//##ModelId=49B87BA00213
	CXJEventDI(CXJEventDI* pEvent);
	//##ModelId=49B87BA00261
	~CXJEventDI();

//attribute
public:
	/** @brief           CPU��*/
	//##ModelId=49B87BA0029F
	int		m_nCPU;
	/** @brief           ������ID*/
	//##ModelId=49B87BA002AF
	int		m_nDIID;
	/** @brief           ����ֵ*/
	//##ModelId=49B87BA002B0
	int		m_nValue;
	/** @brief           ����������*/
	//##ModelId=49B87BA002B1
	CString	m_sDIName;
//operation
public:
	/*
 *  @brief   	Ack	 �¼�ȷ�ϣ�����������ʵ��
 *  @return 	virtual BOOL  TRUE-�ɹ���FALSE-ʧ��
 */
	//##ModelId=49B87BA002B2
	virtual	BOOL Ack();	

/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA002FD
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87BA0033C
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA0037A
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA003B9
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	GetDIName	 ���õ����������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA1000F
	BOOL GetDIName();

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA1004E
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventOSC	¼��
/**
 * @defgroup 	CXJEventOSC	�����豸¼���¼�
 * @{
 */
/**
 * @brief       ���������豸¼���¼�
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA100FA
class CXJEventOSC : public CXJEvent
{
public:
	//##ModelId=49B87BA100FC
	CXJEventOSC();
	//##ModelId=49B87BA10128
	CXJEventOSC(CXJEventOSC* pEvent);
	//##ModelId=49B87BA10167
	~CXJEventOSC();
//attribute
public:
	/** @brief           �����ļ���*/
	//##ModelId=49B87BA10196
	CString		m_sFileName;
	/** @brief           �ļ�·��,���·��*/
	//##ModelId=49B87BA10197
	CString		m_sFilePath;
	/** @brief           �Ƿ��й��ϱ�ʶ 1-�й��� 0-�޹���*/
	//##ModelId=49B87BA101A5
	int			m_nHaveFault;
	/** @brief           ��բ���*/
	//##ModelId=49B87BA101A6
	CString		m_sTriggerType;
	/** @brief           �غ�բʱ��*/
	//##ModelId=49B87BA101A7
	int			m_nRecloseTime;
	/** @brief           ���ϲ��*/
	//##ModelId=49B87BA101A8
	CString		m_sFaultDistance;
	/** @brief           �Ƿ�������.0-��.1-��.2-��������.-1-����ʧ��.-2-ȡ������*/
	//##ModelId=49B87BA101B5
	int			m_nDownloaded;
	/** @brief           ¼���ļ���С*/
	//##ModelId=49B87BA101B6
	int			m_nFileSize;
	/** @brief           ¼���ļ���װ���еı��*/
	//##ModelId=49B87BA101B7
	int			m_nFileNum;
	/** @brief           ����Դ*/
	//##ModelId=49B87BA101B8
	int			m_nDataSrc;

	/** @brief           ƫ����*/
	//##ModelId=49B87BA101B9
	int			m_nOffset;
	
//operation
public:
	/*
 *  @brief   	Ack	 �¼�ȷ�ϣ�����������ʵ��
 *  @return 	virtual BOOL  TRUE-�ɹ���FALSE-ʧ��
 */
	//##ModelId=49B87BA101C5
	virtual	BOOL Ack();	

/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA101E4
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87BA10222
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA10242
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA10242
	virtual	BOOL LoadFromDB_(CMemSet * pMemSet);


/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA10280
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	LoadFromSttpForPT	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA102AF
	BOOL LoadFromSttpForPT(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	IsDownloaded	 �жϵ�ǰ¼���б��Ƿ������� 
 *  @return 	BOOL 0-δ����. 1-������. 2-��������
 */
	//##ModelId=49B87BA1030D
	int IsDownloaded();

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA102DE
	BOOL QueryNewEventID();

public:
/*
 *  @brief   	GetFilePath	 �õ�����¼���ļ�ȫ·�� 
 *  @return 	CString ����¼���ļ�ȫ·��
 */
	//##ModelId=49B87BA1032C
	CString GetFilePath();

/*
 *  @brief   	GetFilePathNoSuff	 �õ�������׺����·�� 
 *  @return 	CString �ļ�·��
 */
	//##ModelId=49B87BA1034B
	CString GetFilePathNoSuff();

/*
 *  @brief   	GetFileRelatePath	 �õ�������׺�������·�� 
 *  @return 	CString ���·��
 */
	//##ModelId=49B87BA1037A
	CString GetFileRelatePath();

/*
 *  @brief   	GetDownloadString	 �õ��Ƿ����ص��������� 
 *  @return 	CString �Ƿ����ص���������
 */
	//##ModelId=49B87BA10399
	CString GetDownloadString();

/*
 *  @brief   	GetFileDir	 ȡ���ļ�Ŀ¼ 
 *  @return 	CString �ļ�Ŀ¼
 */
	CString GetFileDir();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventPTComm ����ͨѶ״̬
/**
 * @defgroup 	CXJEventPTComm	����ͨѶ״̬�¼���
 * @{
 */
/**
 * @brief       ��������ͨѶ״̬��λ�¼�
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA2000F
class CXJEventPTComm : public CXJEvent
{
public:
	//##ModelId=49B87BA20011
	CXJEventPTComm();
	//##ModelId=49B87BA2002E
	CXJEventPTComm(CXJEventPTComm * pEvent);
	//##ModelId=49B87BA2004E
	~CXJEventPTComm();
//attribute
public:
	/** @brief           ״̬ 0������  1: �Ͽ�*/
	//##ModelId=49B87BA2006D
	int		m_nStatus;
//operation
public:

/*
 *  @brief   	Ack	 ȷ���¼�
 *  @return 	BOOL ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA2007D
	BOOL Ack();
/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA2008C
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87BA200AB
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA200CB
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA200EA
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87BA20109
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventPTRun	��������״̬
//##ModelId=49B87BA20167
class CXJEventPTRun : public CXJEvent
{
public:
	//##ModelId=49B87BA20169
	CXJEventPTRun();
	//##ModelId=49B87BA20186
	CXJEventPTRun(CXJEventPTRun * pEvent);
	//##ModelId=49B87BA20196
	~CXJEventPTRun();
//attribute
	/** @brief           ����״̬ 0:���� 1������/���� 2��ͣ�� 3������ 4: �����*/
	//##ModelId=49B87BA201B5
	int		m_nStatus;
//operation
/*
 *  @brief   	Ack	 ȷ���¼�
 *  @return 	BOOL ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA201B6
	BOOL Ack();

/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA201C5
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87BA201D4
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA201F4
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA20203
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA20222
	BOOL QueryNewEventID();
};


//////////////////////////////////////////////////////////////////////////
//CXJEventSysAlarm ϵͳ�쳣�澯
//##ModelId=49B87BA202CE
class CXJEventSysAlarm : public CXJEvent
{
public:
	//##ModelId=49B87BA202D0
	CXJEventSysAlarm();
	//##ModelId=49B87BA202DE
	CXJEventSysAlarm(CXJEventSysAlarm* pEvent);
	//##ModelId=49B87BA202EE
	~CXJEventSysAlarm();
//attribute
/** @brief           ����1����ֵ���ţ�2����ѹ�壻3����ǰһ�ζԱȻ���վֱ�����͵Ķ�ֵ�仯; 4-���׼ֵ�Աȶ�ֵ�仯; 5-��վ���ñ仯*/
	//##ModelId=49B87BA202FD
	int		m_nType;
	/** @brief           �澯��Ϣ����ϸ����*/
	//##ModelId=49B87BA202FE
	CString	m_sDetailMsg;
	/** @brief           �澯��Ӧ��CPU��*/
	//##ModelId=49B87BA202FF
	CString		m_sCPU;
	/** @brief           �澯��Ӧ�Ķ�ֵ����*/
	//##ModelId=49B87BA2030D
	CString		m_sZone;
	/** @brief           �����ʽ������ϸ��Ϣ*/
	//##ModelId=49B87BA2030E
	CString		m_sDetail;

	/** @brief           ���涨ֵ����*/
	//##ModelId=49B87BA20310
	CTypedPtrArray<CPtrArray, PT_SETTING*> arrSetting;
	/** @brief           ������ѹ�嶨��*/
	//##ModelId=49B87BA2031D
	CTypedPtrArray<CPtrArray, PT_SOFTBOARD*> arrSoftBoard;

	/** @brief           ��ϸ��Ϣ*/
	CTypedPtrArray<CPtrArray, SYSALARM*> m_arrDetail;
//operation
/*
 *  @brief   	Ack	 ȷ���¼�
 *  @return 	BOOL ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA20321
	BOOL Ack();
/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA2032C
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA2033C
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA2034B
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

	BOOL Load20138Sttp(STTP_FULL_DATA& sttpData);

	BOOL Load20176Sttp(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetDescribe	 ����¼����� 
 *  @return 	virtual CString �¼������ַ���
 */
	//##ModelId=49B87BA2035B
	virtual CString GetDescribe();

private:
	CString GetSubStationChangeType(int nType);
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID(
 *  @return 	��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA2036B
	BOOL QueryNewEventID();

	/*
	* @brief	GetDescribeCommOFF ȡ����վ�����豸ͨѶ�Ͽ���Խ������	
	* @return   CString	��վ�����豸ͨѶ�Ͽ���Խ����ϸ����
	*/
	CString GetDescribeCommOFF();

/*
 *  @brief   	GetDescribeSubStation	 ȡ����վ���ñ仯��ϸ���� 
 *  @return 	CString ��վ���ñ仯��ϸ����
 */
	//##ModelId=49B87BA2037A
	CString GetDescribeSubStation();

/*
 *  @brief   	GetDescribeSetting	 ȡ�ö�ֵ�仯����ϸ���� 
 *  @return 	CString ��ֵ�仯��ϸ����
 */
	//##ModelId=49B87BA2037B
	CString GetDescribeSetting();

/*
 *  @brief   	GetDescribeSoftBoard	 ȡ����ѹ��仯����ϸ����  
 *  @return 	CString ��ѹ��仯����ϸ����
 */
	//##ModelId=49B87BA2038A
	CString GetDescribeSoftBoard();

/*
 *  @brief   	GetDescribeZone	 ȡ�ö�ֵ���仯����ϸ���� 
 *  @return 	CString ��ֵ���仯����ϸ����
 */
	//##ModelId=49B87BA2038B
	CString GetDescribeZone();

/*
 *  @brief   	GetDescribeOrder	 ȡ�����׼ֵ�Աȶ�ֵ�仯����ϸ����
 *  @return 	CString ��ϸ����
 */
	//##ModelId=49B87BA2038C
	CString GetDescribeOrder();

/*
 *  @brief   	TranslateDetailMsg	 �����¼���ϸ����������m_sDetail�� 
 *  @param 		[In]a param of Type  int  �¼�����
 *  @return 	void 
 */
	//##ModelId=49B87BA2038D
	void TranslateDetailMsg(int nType);

/*
 *  @brief   	TranslateSubStation	 ������վ���ñ仯����ϸ���� 
 *  @return 	void 
 */
	//##ModelId=49B87BA2039A
	void TranslateSubStation();

/*
 *  @brief   	TranslateSetting	 ������ֵ�仯����ϸ���� 
 *  @return 	void 
 */
	//##ModelId=49B87BA203A9
	void TranslateSetting();

/*
 *  @brief   	TranslateOrder	 �������׼ֵ�Աȶ�ֵ����ϸ����
 *  @return 	void 
 */
	//##ModelId=49B87BA203B9
	void TranslateOrder();

/*
 *  @brief   	GetSettingDes	 �õ�ָ����ֵID�Ķ�ֵ���� 
 *  @param 		[In]a param of Type  CString  ��ֵID
 *  @return 	CString ��ֵ����
 */
	//##ModelId=49B87BA203BA
	CString GetSettingDes(CString sID);

/*
 *  @brief   	GetSettingConfig	 ��ȡָ������ָ��CPU�����ж�ֵ���� 
 *  @param 		[In]a param of Type  CString  ָ������
 *  @param 		[In]a param of Type  int  ָ��CPU��
 *	@param		[In]a param of Type	 int   ָ����ֵ����
 *  @return 	void 
 */
	//##ModelId=49B87BA203BC
	void GetSettingConfig(CString PT_ID, int nCPU, int nZone);

/*
 *  @brief   	TranslateZone	 ������ֵ���ű仯����ϸ���� 
 *  @return 	void 
 */
	//##ModelId=49B87BA203CB
	void TranslateZone();

/*
 *  @brief   	TranslateSoftBoard	 ������ѹ��仯����ϸ����  
 *  @return 	void 
 */
	//##ModelId=49B87BA203CC
	void TranslateSoftBoard();

/*
 *  @brief   	GetSettingDes	 �õ�ָ����ѹ��ID�Ķ�ֵ���� 
 *  @param 		[In]a param of Type  CString  ��ѹ��ID
 *  @return 	CString ��ѹ������
 */
	//##ModelId=49B87BA203D8
	CString GetSoftBoardDes(CString sID);

/*
 *  @brief   	GetSoftBoardConfig	 �õ�ָ������ָ��CPU�µ���ѹ�嶨��
 *  @param 		[In]a param of Type  CString  ����ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	void 
 */
	//##ModelId=49B87BA203DA
	void GetSoftBoardConfig(CString PT_ID, int nCPU);

/*
 *  @brief   	GetMsg	 �õ���������Ϣ 
 *  @return 	CString ��������Ϣ
 */
	//##ModelId=49B87BA30001
	CString	GetMsg();

/*
 *  @brief   	ClearSettingConfig	 �����ֵ���� 
 *  @return 	void 
 */
	//##ModelId=49B87BA30002
	void ClearSettingConfig();

/*
 *  @brief   	ClearSoftBoardConfig	 �����ѹ�嶨�� 
 *  @return 	void 
 */
	//##ModelId=49B87BA30003
	void ClearSoftBoardConfig();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventComm	����վͨѶ״̬��λ�¼�
/**
 * @defgroup 	CXJEventComm	����վͨѶ״̬��λ�¼�
 * @{
 */
/**
 * @brief       ��������ͨѶ��λ�����¼�����
*  @author      LYH
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA3002E
class CXJEventComm : public CXJEvent
{
public:
	//##ModelId=49B87BA30030
	CXJEventComm();
	//##ModelId=49B87BA3003E
	CXJEventComm(CXJEventComm* pEvent);
	//##ModelId=49B87BA30040
	~CXJEventComm();
//attribute
public:
	/** @brief           ͨѶ״̬*/
	//##ModelId=49B87BA30041
	int m_nStatus;
//operation
public:
/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	//##ModelId=49B87BA30042
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA30044
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA30047
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	BuildReleation	 �����¼����豸�Ĺ�ϵ 
 *  @param 		[In]a param of Type  int  �����豸����
 *  @return 	virtual BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA3004B
	virtual	BOOL BuildReleation(int nType);
};

class CXJEventDistance : public CXJEvent
{
public:
	CXJEventDistance();
	CXJEventDistance(CXJEventDistance* pEvent);
	~CXJEventDistance();
public:
	/** @brief			��·����*/
	CString	m_sLineName;
	/** @brief			��ѹ�ȼ�*/
	int		m_nVoltage;
	/** @brief			��·�ܳ���*/
	float	m_fLineTotalLength;
	/** @brief			���ϵ㵽M�˵ľ���*/
	float	m_fDistance;
	/** @brief			����λ��*/
	int		m_nTowerPos;
	/** @brief			ά��������*/
	CString	m_sManagerUnit;
public:
	/*
 *  @brief   	Ack	 �¼�ȷ�ϣ�����������ʵ��
 *  @return 	virtual BOOL  TRUE-�ɹ���FALSE-ʧ��
 */
	virtual	BOOL Ack();	
/*
 *  @brief   	GetDescribe	 �õ��¼���ϸ����
 *  @return 	virtual CString �¼������ַ���
 */
	virtual CString GetDescribe();
	/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CXJEvent* �������ɵ��¼�����
 */
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 ��ѯ���¼���ID
 *  @return 	void 
 */
	BOOL QueryNewEventID();
};

#endif
