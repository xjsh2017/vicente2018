// BatchSel.h: interface for the CBatchSel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_)
#define AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSel : public CObject
{
public:
	/** @brief           �Ƿ��ٻ���ֵ����.0-���ٻ�,1-�ٻ�,2-��֧��, 3-װ���ѹ���*/
	int	m_nCallZone;
	/** @brief           �Ƿ��ٻ���ֵ.0-���ٻ�,1-�ٻ�,2-��֧��, 3-װ���ѹ���*/
	//##ModelId=49B87BB1029F
	int	m_nCallSetting;
	/** @brief           �Ƿ��ٻ�������.0-���ٻ�,1-�ٻ�,2-��֧��, 3-װ���ѹ���*/
	//##ModelId=49B87BB102A0
	int	m_nCallDigital;
	/** @brief           �Ƿ��ٻ���ѹ��.0-���ٻ�,1-�ٻ�,2-��֧��, 3-װ���ѹ���*/
	//##ModelId=49B87BB102AF
	int	m_nCallSoftBoard;
	/** @brief           �Ƿ��ٻ�ģ����.0-���ٻ�,1-�ٻ�,2-��֧��, 3-װ���ѹ���*/
	//##ModelId=49B87BB102B0
	int	m_nCallAnalog;
	int	m_nCallOsc;
	int	m_nCallHistory;

	BOOL m_bChecked;

    CObject* pMap;

	enum CallStatus{
		DONT_CALL = 0,//���ٻ�
		CALL = 1, //�ٻ�
		UNSUPPORT = 2,//��֧��
		SUSPEND = 3,//����
	};

/*
 *  @brief   	GetCallString	 ȡ���Ƿ��ٻ����� 
 *  @param 		[In]a param of Type  int  �Ƿ��ٻ�
 *  @return 	CString �Ƿ��ٻ�����
 */
	CString GetCallString(int nCall);
};

class CBatchSel : public CSel  
{
public:
	//##ModelId=49B87BB101F6
	CBatchSel();
	//##ModelId=49B87BB10213
	CBatchSel(CSecObj* pObj, CSecCPU* pCpu, BOOL bZone, BOOL bSetting, BOOL bDigital, BOOL bSoftBoard, BOOL bAnalog);
	//##ModelId=49B87BB10232
	~CBatchSel();

/*
 *  @brief   	SaveData	 �������ݵ����ݿ� 
 *  @param 		[In]a param of Type  int ���Զ��������û��������ٻ�����.0-�Զ�����. 1-�����ٻ�
 *  @return 	BOOL �������ݳɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB10251
	BOOL SaveData(int nIS_OC);

	//##ModelId=49B87BB10271
	BOOL MultiSaveData(STTP_DBDATA_RECORD& sttpDB, int nIS_OC);

/*
 *  @brief   	LoadFromDB	 �����ݿ�������Ϣ 
 *  @param 		[In]a param of Type  CMemSet*  ���ݿ�ָ��
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);


public:
	/** @brief           ��Ӧ�Ķ����豸*/
	//##ModelId=49B87BB10291
	CSecObj* m_pObj;
	/** @brief           ��Ӧ�Ķ����豸��CPU*/
	//##ModelId=49B87BB10296
	CSecCPU* m_pCpu;

	/** @brief           ��ǰ��ֵ�����ٻ����,  0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�*/
	//##ModelId=49B87BB102BF
	int		m_nZone;
	/** @brief           ��ֵ�ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�*/
	//##ModelId=49B87BB102C0
	int		m_nSetting;
	/** @brief           �������ٻ����,  0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�*/
	//##ModelId=49B87BB102C1
	int		m_nDigital;
	/** @brief           ��ѹ���ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�*/
	//##ModelId=49B87BB102CE
	int		m_nSoftBoard;
	/** @brief           ģ�����ٻ����, 0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�*/
	//##ModelId=49B87BB102CF
	int		m_nAnalog;

	int		m_nOsc;
	int		m_nHistory;

	/** @brief           ��ֵ���Ž��˵��*/
	//##ModelId=49B87BB102DE
	int	    m_nZoneNote;
	/** @brief           ��ֵ���˵��*/
	//##ModelId=49B87BB102DF
	int		m_nSettingNote;
	/** @brief           ���������˵��*/
	//##ModelId=49B87BB102E0
	int		m_nDigitalNote;
	/** @brief           ��ѹ����˵��*/
	//##ModelId=49B87BB102EE
	int		m_nSoftBoardNote;
	/** @brief           ģ�������˵��*/
	//##ModelId=49B87BB102EF
	int		m_nAnalogNote;
	int		m_nOscNote;
	int		m_nHistoryNote;
};

class BatchSel : public CSel
{
public:
	
	BatchSel();
};


/** @brief           ѡ��������*/
//##ModelId=49B87BB102FD
typedef CPtrList  BATCH_LIST;
typedef CTypedPtrList<CObList , CBatchSel*>	BATCHSEL_LIST;
typedef CMapStringToPtr PRIDEVICE_MAP;
typedef CMapStringToPtr DEVICETYPE_MAP;
typedef CMapWordToPtr   STATION_MAP;
typedef CMapStringToPtr NET_MAP;
typedef CMapStringToPtr BATCHSEL_MAP;
typedef CMapStringToPtr STRING_MAP;
typedef CMapWordToPtr   WORD_MAP;

#endif // !defined(AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_)
