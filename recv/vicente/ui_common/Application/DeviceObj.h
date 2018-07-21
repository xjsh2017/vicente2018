// ***************************************************************
//  DeviceObj   version:  1.0    date: 12:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �豸��, ��¼�����豸������, ִ���豸�����Զ�ȡ����
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_)
#define AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceObj.h : header file
//
#include "vccommon_define.h"
#include "DBEngine.h"

class CDeviceObj;
class CSecCPU;
class CNetObj;
class CStationObj;

class CBreakObj;
class CBusObj;
class CGenObj;
class CCapObj;
class CLineObj;
class CReactorObj;
class CSwitchObj;
class CTransObj;
class CPmsObj;
class CBayObj;

class CSecObj;

/** @brief           �����豸����*/
//##ModelId=49B87BA5002E
typedef CTypedPtrList<CObList , CDeviceObj*>	DEVICE_LIST;

/** @brief           ��������*/
//##ModelId=49B87BA5003E
typedef	CTypedPtrList<CObList , CNetObj*>		NET_LIST;
/** @brief           ��վ����*/
//##ModelId=49B87BA5004E
typedef CTypedPtrList<CObList , CStationObj*>	STATION_LIST;

/** @brief           ��վͨ��ԭ���*/
typedef CMap<int, int, CString, CString>        STATION_STATUS_CONFIG_MAP;  


/** @brief           �����豸����*/
//##ModelId=49B87BA500EB
typedef CTypedPtrList<CObList , CSecObj*>		SEC_LIST;

typedef CMap<int, int, CStationObj* , CStationObj* >            MAP_VOLTAGE;


/////////////////////////////////////////////////////////////////////////////
// CDeviceObj �豸����

/**
 * @defgroup 	CDeviceObj �豸������
 * @{
 */
/**
 * @brief       �豸����,���豸��Ӵ���̳�
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER
//##ModelId=49B87BA50138
class CDeviceObj : public CObject
{
public:
	//##ModelId=49B87BA50149
	CDeviceObj();
	DECLARE_DYNCREATE(CDeviceObj)

// Attributes
public:
	/** @brief           �豸ID*/
	//##ModelId=49B87BA50186
	CString		m_sID;
	/** @brief           ͳһ��Դ���*/
	CString    m_sUri;
	/** @brief           �豸����*/
	//##ModelId=49B87BA50187
	CString		m_sName;
	/** @brief           �����α�ʶ���豸����.�����stdafx.h*/
	//##ModelId=49B87BA50196
	int			m_nType;
	/** @brief           ���ַ�����ʶ���豸����. �����stdafx.h*/
	//##ModelId=49B87BA50197
	CString		m_sType;
	/** @brief           ��ѹ�ȼ�*/
	//##ModelId=49B87BA501A5
	int			m_nVoltage;
	/** @brief           ������վID*/
	//##ModelId=49B87BA501A6
	CString		m_sStationID;
	/** @brief           ����״̬,�����豸��Ӧ��ͬ����״ֵ̬*/
	//##ModelId=49B87BA501B5
	int			m_nRunStatu;
	/** @brief           ͨѶ״̬.0-����,1-�Ͽ�*/
	//##ModelId=49B87BA501C5
	int			m_nCommStatus;
	/** @brief           ���*/
	//##ModelId=49B87BA501C6
	int			m_nCode;
	/** @brief           103���*/
	//##ModelId=49B87BA501D4
	int			m_n103Group;
	/** @brief           103��Ŀ��*/
	//##ModelId=49B87BA501D5
	int			m_n103Item;
	/** @brief           ��Ӧ���ID*/
	//##ModelId=49B87BA501E4
	CString		m_sBayID;

	BOOL m_bHasSecDevice;

	/** @brief           ���豸���е�λ��*/
	HTREEITEM	m_htItem;

// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA50204
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 �����豸��Ϣ, ������ʵ��
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA50242
	virtual	BOOL Save(CDBEngine* pDBEngine);

// Implementation
public:
	//##ModelId=49B87BA50271
	virtual ~CDeviceObj();
};
//CDeviceObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CNetObj	����
/**
 * @defgroup 	CNetObj	������
 * @{
 */
/**
 * @brief       ������, ����,��¼,�洢������Ϣ
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER

//##ModelId=49B87BA502CE
class CNetObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA502DE
	CNetObj();
	//##ModelId=49B87BA5030D
	~CNetObj();
	DECLARE_DYNCREATE(CNetObj)

//Attribute
public:
	/** @brief           ����˵��*/
	//##ModelId=49B87BA5033C
	CString		m_sNote;

	/** @brief           ͨ���쳣�ĳ�վ��*/
	//##ModelId=49B87BA5034B
	int		m_nStationAbnormity;
	/** @brief           ����ͨ��״̬�쳣������*/
	//##ModelId=49B87BA5034C
	int		m_nPTCommAbnormity;
	/** @brief           ��������״̬�쳣������*/
	//##ModelId=49B87BA5035B
	int		m_nPTRunAbnormity;

// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA5035C
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 �����豸��Ϣ, ������ʵ��
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA50399
	virtual	BOOL Save(CDBEngine* pDBEngine);
};

//CNetObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CStationObj ��վ
/**
 * @defgroup 	CStationObj	��վ��
 * @{
 */
/**
 * @brief       ��վ��, ����,��¼,�洢��վ��Ϣ
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER

//##ModelId=49B87BA6009C
class CStationObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA600AC
	CStationObj();
	//##ModelId=49B87BA600DA
	~CStationObj();
	DECLARE_DYNCREATE(CStationObj)

//Attribute
	/** @brief           ��������ID*/
	//##ModelId=49B87BA60109
	CString	m_sNetID;
	/** @brief           ͨѶ��ԼID*/
	//##ModelId=49B87BA60119
	int		m_nProtocolId;
	/** @brief           ͨѶ��Լ����*/
	//##ModelId=49B87BA6011A
	CString	m_sProtocol;
	/** @brief           ͨѶ��ʽ 1��tcp 2��com(����) 3��udp 4��modem*/
	//##ModelId=49B87BA60128
	int		m_nCommMode;
	/** @brief           ��վ��Ӧ��SVG�ļ�·��(ȫ·��)*/
	//##ModelId=49B87BA60129
	CString	m_sSvgPath;
	/** @brief           ���һ��״̬��λ��ʱ��*/
	//##ModelId=49B87BA60138
	CTime	m_tmNewStatus;
	/** @brief           ���һ��״̬��λ��ԭ��*/
	//##ModelId=49B87BA60139
	CString	m_sNewStatusReason;
	/** @brief           ԭ��ı�ʱ��*/
	CTime m_tmReasonTime;

	/** @brief           IPѡ���־��Ϊ2ʱΪ����˫ͨ��*/
	int		m_nIPFlag;

	/** @brief           ͨ��1״̬*/
	int		m_nCommStatus1;
	/** @brief           ͨ��1���һ��״̬��λ��ʱ��*/
	CTime	m_tmNewStatus1;
	/** @brief           ͨ��1���һ��״̬��λ��ԭ��*/
	CString	m_sNewStatusReason1;
	/** @brief           ͨ��1ԭ��ı�ʱ��*/
	CTime m_tmReasonTime1;


	/** @brief           ͨѶ״̬�Ͽ��ı�������*/
	//##ModelId=49B87BA6014A
	SEC_LIST		m_arrCommAbnormity;
	/** @brief           ����״̬�쳣�ı�������*/
	//##ModelId=49B87BA6015B
	SEC_LIST		m_arrRunAbnormity;
	/** @brief           IP��ַ*/
	//##ModelId=49B87BA60177
	CString			m_sIP;
	/** @brief           ����������Ϣ*/
	//##ModelId=49B87BA60186
	CString			m_sManufacturer;
	/** @brief           ��ѹ�ȼ��б�*/
	CWordArray		m_arrVoltage;
	/** @brief			ͨ�Ŷ˿�*/
	CString			m_sPort;
	/** @brief			������Ϣ*/
	CString			m_sTel;

	int m_nChannel;

	/** @brief			�ڵ���ͼ�е�λ��*/
	CPoint	m_ptCenter;

	/** @brief			���ݿ���ʶ���*/
	CDBEngine*	m_pDBEngine;

//��վ�¸��豸����, ������MAP����,�����豸��LIST
private:
	/** @brief           ĸ������*/
	//##ModelId=49B87BA60197
	//BUS_LIST		m_BusList;
	CMapStringToOb	m_BusMap;
	/** @brief           ��������*/
	//##ModelId=49B87BA601A6
	CMapStringToOb	m_BreakMap;
	/** @brief           �������*/
	//##ModelId=49B87BA601B6
	CMapStringToOb	m_BayMap;
	/** @brief           ��·����*/
	//##ModelId=49B87BA601C6
	CMapStringToOb	m_LineMap;
	/** @brief           ����������*/
	//##ModelId=49B87BA601D5
	CMapStringToOb	m_CapMap;
	/** @brief           �翹������*/
	//##ModelId=49B87BA601DA
	CMapStringToOb	m_ReactorMap;
	/** @brief           ��բ����*/
	//##ModelId=49B87BA601E8
	CMapStringToOb	m_SwitchMap;
	/** @brief           �������������*/
	//##ModelId=49B87BA601F5
	CMapStringToOb	m_PmsMap;
	/** @brief           ��ѹ������*/
	//##ModelId=49B87BA60204
	CMapStringToOb	m_TransMap;
	/** @brief           ���������*/
	//##ModelId=49B87BA60214
	CMapStringToOb	m_GenMap;
	
	CMapStringToOb	m_SecMap;
	/** @brief          ��UriΪKey��Map*/
	CMapStringToOb  m_UriMap;	

	

public:
	BOOL LoadData(CDBEngine* pDBEngine);
	BOOL BuildData(CDBEngine* pDBEngine);
	void InitLock();
	/*
 *  @brief   	GetPriDeviceList	 ȡ��һ���豸���� 
 *  @param 		[In]a param of Type  DEVICE_LIST& ���ڱ���һ���豸������
 *  @param 		[In]a param of Type  int ָ��Ҫ���ҵ�һ���豸���ͣ�-1��ʾ��������
 *  @return 	
 */
	void GetPriDeviceList(DEVICE_LIST& listDevice, int nDeviceType);

/*
 *  @brief   	GetSecList	 ȡ�ö����豸���� 
 *  @param 		[In]a param of Type  DEVICE_LIST& ���ڱ�������豸������
 *  @param 		[In]a param of Type  CString ָ��Ҫһ���豸ID��Ϊ��ʱ��ʾ������һ���豸
 *  @param 		[In]a param of Type  int ָ��Ҫ���ҵĶ����豸���ͣ�-1��type_sec��ʾ��������
 *  @return 	
 */
	void GetSecList(DEVICE_LIST& listDevice, CString sPriID, int nSecType);

	/*
	* @brief	GetSecList	ȡ�ö����豸����
	* @param	[In]a param of Type DEVICE_LIST & listDevice ���ڱ�������豸������		
	* @param	[In]a param of Type int nPriType ָ��Ҫһ���豸����	,Ϊ-1ʱΪ�����豸
	* @param	[In]a param of Type int nSecType ָ��Ҫ���ҵĶ����豸���ͣ�-1��type_sec��ʾ��������		
	* @return   void	
	*/
	void GetSecList(DEVICE_LIST& listDevice, int nPriType, int nSecType);
private:
	CXJLock* m_pLock;
private:
/*
 *  @brief   	BuildBusMap	 ��ȡ����վ��ĸ�����ò�����ĸ��MAP 
 *  @return 	BOOL �ɹ�����true��ʧ�ܷ���false
 */
	BOOL BuildBusMap(CDBEngine* pDBEngine);
	BOOL BuildLineMap(CDBEngine* pDBEngine);
	BOOL BuildBreakMap(CDBEngine* pDBEngine);
	BOOL BuildBayMap(CDBEngine* pDBEngine);
	BOOL BuildCapMap(CDBEngine* pDBEngine);
	BOOL BuildReactorMap(CDBEngine* pDBEngine);
	BOOL BuildSwitchMap(CDBEngine* pDBEngine);
	BOOL BuildPmsMap(CDBEngine* pDBEngine);
	BOOL BuildTransMap(CDBEngine* pDBEngine);
	BOOL BuildGenMap(CDBEngine* pDBEngine);
	BOOL BuildSecMap(CDBEngine* pDBEngine);

	void GetGeoPosition(CString strPosition);
public:

/*
 *  @brief   	AddDevice	 �����豸������������ 
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA602AF
	BOOL AddDevice(int nType, CDeviceObj* pObj);

// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA60251
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 �����豸��Ϣ, ������ʵ��
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA60280
	virtual	BOOL Save(CDBEngine* pDBEngine);

/*
 *  @brief   	Clear	 �����վ�������豸��Ϣ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA602DE
	BOOL Clear();

/*
 *  @brief   	FindDevice	 ���ұ���վ�µ��豸 
 *  @param 		[In]a param of Type  CString  �豸ID
 *  @param 		[In]a param of Type  int  �豸����
 *  @return 	CDeviceObj* 
 */
	//##ModelId=49B87BA6030D
	CDeviceObj* FindDevice(CString sID, int nType);

/*
 *  @brief   	FindDeviceNoLock	 ���ұ���վ�µ��豸,������ 
 *  @param 		[In]a param of Type  CString  �豸ID
 *  @param 		[In]a param of Type  int  �豸����
 *  @return 	CDeviceObj* 
 */
	CDeviceObj* FindDeviceNoLock(CString sID, int nType);

/*
 *  @brief   	FindDeviceBy103	 ����103�����Ŀ�Ų����豸 
 *  @param 		[In]a param of Type  CString  103�����Ŀ��.��ʽ:103���:103��Ŀ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @return 	CDeviceObj * �豸ָ��
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceBy103(CString str103, int nType);


/*
 *  @brief   	FindDeviceBy103	 ����uri�����豸 
 *  @param 		[In]a param of Type  CString uri
 *  @return 	CDeviceObj * �豸ָ��
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceByUri(CString strUri);

/*
 *  @brief   	LoadStatusFromDB	 �����ݿ������վ����վͨѶ״̬ 
 *  @param 		[In]a param of Type  int  ͨ��
 *  @return 	void 
 */
	void LoadStatusFromDB(int nChannel, CDBEngine* pDBEngine);

	/*
	* @brief	GetDurativeTime	ȡ��״̬����ʱ��
	* @param	[In]a param of Type CTimeSpan & tsDurative1	��ͨ��״̬����ʱ��	
	* @param	[In]a param of Type CTimeSpan & tsDurative2	����ͨ��״̬����ʱ��	
	* @return   void	
	*/
	void GetDurativeTime(CTimeSpan& tsDurative1, CTimeSpan& tsDurative2);

/*
 *  @brief   	LoadStatusFromSTTP	 �ӱ����ж�������վͨѶ״̬ 
 *  @param 		[In]a param of Type  STTP_FILE_DATA*  ��������
 *  @return 	void 
 */
	//##ModelId=49B87BA603B9
	void LoadStatusFromSTTP(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	SecCommChange	 ����ͨѶ״̬�仯 
 *  @param 		[In]a param of Type  CSecObj*  ״̬�仯�ı���
 *  @param 		[In]a param of Type  int  �仯���״̬
 *  @return 	void 
 */
	//##ModelId=49B87BA603D8
	void SecCommChange(CSecObj* pObj, int nStatus);

/*
 *  @brief   	SecRunChange	 ��������״̬�仯 
 *  @param 		[In]a param of Type  CSecObj*  ״̬�仯�ı���
 *  @param 		[In]a param of Type  int  �仯���״̬
 *  @return 	void 
 */
	//##ModelId=49B87BA7001F
	void SecRunChange(CSecObj* pObj, int nStatus);

/*
 *  @brief   	GetCommAbnormityNum	 ȡ�ñ���վ��ͨѶ״̬�쳣��װ���� 
 *  @return 	int ͨѶ״̬�쳣��װ����
 */
	//##ModelId=49B87BA7003E
	int GetCommAbnormityNum();

/*
 *  @brief   	GetRunAbnormityNum	 ȡ�ñ���վ������״̬�쳣�ı����� 
 *  @return 	int ����״̬�쳣�ı�����
 */
	//##ModelId=49B87BA7004E
	int GetRunAbnormityNum();

/*
 *  @brief   	SaveSvgPath	 ����SVG�ļ�·�������ݿ� 
 *  @param 		[In]a param of Type  CString  SVG·��(���·��)
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA7005D
	BOOL SaveSvgPath(CString sPath, CDBEngine* pDBEngine);

/*
 *  @brief   	GetReason	 �õ���վ״̬�ı�ԭ������ 
 *  @param 		[In]a param of Type  int  ״̬�ı�ԭ��
 *  @return 	CString ״̬�ı�ԭ������
 */
	//##ModelId=49B87BA7007D
	CString GetReason(int nType);

/*
 *  @brief   	StatVoltage	 ͳ��վ�ڵ�ѹ�ȼ� 
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void 
 */
	void StatVoltage(int nVoltage);

/*
 *  @brief   	SetStatus	 ���ó�վ״̬ 
 *  @param 		[In]a param of Type  int  ״̬
 *  @param 		[In]a param of Type  CTime  ʱ��
 *  @param 		[In]a param of Type  int  ԭ��
 *  @param 		[In]a param of Type  int  ͨ��
 *  @return 	void 
 */
	void SetStatus(int nStatus, CTime tm, int nReason, int nChannel, CTime tmReasonTime, int nIPFlag);
};

//CStationObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBusObj	ĸ��
/**
 * @defgroup 	CBusObj	ĸ����
 * @{
 */
/**
 * @brief       ����ĸ������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA700AB
class CBusObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA700BC
	CBusObj();
	//##ModelId=49B87BA700CB
	~CBusObj();
	DECLARE_DYNCREATE(CBusObj)
//Attribute
	/** @brief           ���߷�ʽ 0��3/2���ߣ�1��˫ĸ���ߣ�2��˫ĸ������·ĸ�ߣ�3����ĸ�ߣ�4����ĸ�߷ֶΣ�*/
	//##ModelId=49B87BA700DA
	int		m_nBusType;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA700EA
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 �����豸��Ϣ, ������ʵ��
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA700FA
	virtual	BOOL Save(CDBEngine* pDBEngine);

};
//CBusObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CLineObj	��·
/**
 * @defgroup 	CLineObj	·����
 * @{
 */
/**
 * @brief       ����·������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70128
class CLineObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7012A
	CLineObj();
	//##ModelId=49B87BA70138
	~CLineObj();
	DECLARE_DYNCREATE(CLineObj)
//Attribute
public:

// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA70157
	virtual	BOOL Load(CMemSet * pMemSet);
};

//CLineObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBreakObj	����
/**
 * @defgroup 	CBreakObj	������
 * @{
 */
/**
 * @brief       ������������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70196
class CBreakObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70198
	CBreakObj();
	//##ModelId=49B87BA701A5
	~CBreakObj();
	DECLARE_DYNCREATE(CBreakObj)
//Attribute
public:
	/** @brief           0��3/2ĸ�߲࿪��1��3/2�м俪�� 2��˫ĸĸ������ 3����·���� 4����ĸ�ֶο��� 5����·����*/
	//##ModelId=49B87BA701B5
	int		m_nBreakType;

// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA701C5
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CBreakObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CGenObj	�����
/**
 * @defgroup 	CGenObj	�������
 * @{
 */
/**
 * @brief       �������������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA701F4
class CGenObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70204
	CGenObj();
	//##ModelId=49B87BA70213
	~CGenObj();
	DECLARE_DYNCREATE(CGenObj)
//Attribute
public:
	/** @brief           ����,����,��λΪMVA*/
	//##ModelId=49B87BA70222
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA70223
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CGenObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CReactorObj	��ѹ�翹��
/**
 * @defgroup 	CReactorObj	��ѹ�翹����
 * @{
 */
/**
 * @brief       ������ѹ�翹������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70261
class CReactorObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70263
	CReactorObj();
	//##ModelId=49B87BA70271
	~CReactorObj();
	DECLARE_DYNCREATE(CReactorObj)
//Attribute
public:
	/** @brief           ����, ��λMVA*/
	//##ModelId=49B87BA70280
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA70281
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CReactorObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CTransObj	��ѹ��
/**
 * @defgroup 	CTransObj	��ѹ����
 * @{
 */
/**
 * @brief       ������ѹ������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA702BF
class CTransObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA702CE
	CTransObj();
	//##ModelId=49B87BA702CF
	~CTransObj();
	DECLARE_DYNCREATE(CTransObj)
//Attribute
public:
	/** @brief           ��ѹ������ 1: ����� 0�������*/
	//##ModelId=49B87BA702DE
	int		m_nTransType;
	/** @brief           �����*/
	//##ModelId=49B87BA702DF
	int		m_nVolume;
	/** @brief           ��ѹ���ѹ*/
	//##ModelId=49B87BA702EE
	int		m_nVoltageH;
	/** @brief           ��ѹ���ѹ*/
	//##ModelId=49B87BA702EF
	int		m_nVoltageM;
	/** @brief           ��ѹ���ѹ*/
	//##ModelId=49B87BA702F0
	int		m_nVoltageL;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA702F1
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CTransObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSwitchObj	��բ
/**
 * @defgroup 	CSwitchObj	��բ��
 * @{
 */
/**
 * @brief       ������բ����
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA7032C
class CSwitchObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7032E
	CSwitchObj();
	//##ModelId=49B87BA7033C
	~CSwitchObj();
	DECLARE_DYNCREATE(CSwitchObj)
//Attribute
public:
	/** @brief           ��բ���� 0:�ǽӵص�բ 1:�ӵص�բ*/
	//##ModelId=49B87BA7034B
	int		m_nSwitchType;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA7034C
	virtual	BOOL Load(CMemSet * pMemSet);
};

//CSwitchObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CCapObj	������
/**
 * @defgroup 	CCapObj	��������
 * @{
 */
/**
 * @brief       ��������������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA7037A
class CCapObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7037C
	CCapObj();
	//##ModelId=49B87BA7038A
	~CCapObj();
	DECLARE_DYNCREATE(CCapObj)
//Attribute
public:
	/** @brief           ����,��λMVA*/
	//##ModelId=49B87BA70399
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA7039A
	virtual	BOOL Load(CMemSet * pMemSet);
};
//CCapObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CPmsObj	���������
/**
 * @defgroup 	CPmsObj	�����������
 * @{
 */
/**
 * @brief       �����������������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA703C8
class CPmsObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA703CA
	CPmsObj();
	//##ModelId=49B87BA703D8
	~CPmsObj();
	DECLARE_DYNCREATE(CPmsObj)
//Attribute
public:
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA703D9
	virtual	BOOL Load(CMemSet * pMemSet);	
};
//CPmsObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBayObj	���
/**
 * @defgroup 	CBayObj	�����
 * @{
 */
/**
 * @brief       �����������
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA8001F
class CBayObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA80021
	CBayObj();
	//##ModelId=49B87BA8002E
	~CBayObj();
	DECLARE_DYNCREATE(CBayObj)
//Attribute
public:
	/** @brief           SVGͼ��·��*/
	CString		m_sSvgPath;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA8002F
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	SaveSvgPath	 ����SVG·�������ݿ� 
 *  @param 		[In]a param of Type  CString  SVG�ļ�·��
 *  @return 	void 
 */
	BOOL SaveSvgPath(CString strPath, CDBEngine* pDBEngine);
};
//CBayObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecObj	�����豸
/**
 * @defgroup 	CSecObj	�����豸��
 * @{
 */
/**
 * @brief       ���������豸����
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA800CB
class CSecObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA800CD
	CSecObj();
	//##ModelId=49B87BA800DA
	~CSecObj();
	DECLARE_DYNCREATE(CSecObj)
//Attribute
public:
/** @brief           0����վ��1��ĸ�����2����ѹ������ 3����·������4�������鱣����
5����·��������6���翹��������8��������������9������¼������
11ĸ��������12��Ƶ����װ�ã�13�����װ�ã�14����ȫ�Զ�װ�ã�
	15��GPSװ�ã�16:���ܱ�17:��ѹװ�ã�18: ���ܼ���װ�ã�20:����װ��*/
	//##ModelId=49B87BA800DB
	int m_nSecType;	
	/** @brief           ��������	���������󱸱�����*/
	//##ModelId=49B87BA800EA
	CString	m_sKind;
	/** @brief           �豸�ͺ�*/
	//##ModelId=49B87BA800EB
	CString	m_sModel;
	/** @brief           �豸�汾��(������ʾ: ����汾��)*/
	//##ModelId=49B87BA800EC
	CString	m_sSysVer;
	/** @brief           CPU����*/
	//##ModelId=49B87BA800ED
	int		m_nCpuNum;
	/** @brief           �豸ʱ��(������ʾ: ��������ʱ��)*/
	//##ModelId=49B87BA800EE
	CString	m_sDevTime;
	/** @brief           �豸У����(������ʾ: ͨ��У����)*/
	//##ModelId=49B87BA800FA
	CString	m_sCrc;
	/** @brief           �豸վ�ڵ�ַ(������ʾ: װ�õ�ַ)*/
	//##ModelId=49B87BA800FB
	CString	m_sAddr;
	/** @brief           ͨ�ſں�(������ʾ: ͨ�ſ�)*/
	//##ModelId=49B87BA800FC
	int		m_nComID;
	/** @brief           ����һ���豸ID*/
	//##ModelId=49B87BA800FD
	CString	m_sOwnerID;
	/** @brief           ����һ���豸ָ��*/
	//##ModelId=49B87BA800FF
	CDeviceObj* m_pOwner;
	/** @brief           ����һ���豸���� ����һ���豸���ͣ�
								1;		ĸ��
								2;		��·
								3;		����
								4;		�����
								5;		��ѹ�翹��
								6;		��ѹ��
								7;		��բ
								8;		��ѹ������
								9;		��վ
								10;		����װ��
								11;		¼����	*/
	//##ModelId=49B87BA8010B
	int		m_nOwnerType;
	/** @brief           ��������ID*/
	//##ModelId=49B87BA8010C
	int		m_nPTTypeID;
	/** @brief           ��Լ���ͣ�141Ϊmodem��142Ϊmodbus��143Ϊmlink*/
	//##ModelId=49B87BA80119
	int		m_nProtocolType;
	/** @brief           ���������ID*/
	//##ModelId=49B87BA8011A
	CString	m_sPMSID;
	/** @brief           װ��103��ַ*/
	//##ModelId=49B87BA8011B
	int		m_n103Addr;

	/** @brief           ������վָ��*/
	//##ModelId=49B87BA80129
	CStationObj* m_pStation;

	/** @brief           ���һ��ͨ��״̬��λ��ʱ��*/
	//##ModelId=49B87BA8012D
	CTime	m_tmNewCommStatus;

	/** @brief           ���һ������״̬��λ��ʱ��*/
	//##ModelId=49B87BA8013A
	CTime	m_tmNewRunStatus;

	bool    m_bNewDevice;

	//�ź����б���  add by yinzhehong 2005-09020
	/** @brief           �����������б����Ա�tb_pt_Action_character*/
	//##ModelId=49B87BA8014B
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_arrActChr;
	/** @brief           �����ź����б����Ա�tb_pt_event_def*/
	//##ModelId=49B87BA80158
	CTypedPtrArray<CPtrArray,PT_Signal*>          m_arrActionSignal;
    /** @brief           �澯�ź����б����Ա�tb_pt_alarm_def*/
	//##ModelId=49B87BA80168
    CTypedPtrArray<CPtrArray,PT_Signal*>          m_arrAlarmSignal;
	/** @brief           ����CPU��Ϣ*/
	//##ModelId=49B87BA8016D
	CTypedPtrArray<CPtrArray, CSecCPU*>			  m_arrCPU;

private:
	//��ֹ���ݿ����޵����Ϣ, ��ÿ�����¼�ʱ������
	/** @brief           ���ݿ�����������*/
	//##ModelId=49B87BA8017A
	BOOL	m_bNoActChr;
	/** @brief           ���ݿ����޶����ź���*/
	//##ModelId=49B87BA8017B
	BOOL	m_bNoActSign;
	/** @brief           ���ݿ����޸澯�ź���*/
	//##ModelId=49B87BA80186
	BOOL	m_bNoAlaSign;
// Operations
public:
/*
 *  @brief   	Load	 �����豸��Ϣ, ������ʵ�� 
 *  @return 	virtual BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA80187
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	LoadSimple	 �����豸��Ϣ 
 *  @param 		[In]a param of Type  CMemSet*  ���ݿ�����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadSimple(CMemSet* pMemSet);

/*
 *  @brief   	RefreshConfig	 ˢ�±������� 
 *  @param 		[In/Out]a param of Type  BOOL  ����Ѿ��������Ƿ�ǿ��ˢ��
 *  @return 	BOOL ˢ�³ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA80198
	BOOL RefreshConfig(CDBEngine* pDBEngine, BOOL bRefresh = FALSE);

/*
 *  @brief   	SetCommStatus	 �ı�װ��ͨѶ״̬ 
 *  @param 		[In]a param of Type  int  ָ��ͨѶ״̬. 0-����.1-�Ͽ�
 *  @return 	void 
 */

	//##ModelId=49B87BA801A6
	void SetCommStatus(int nStatus);

/*
 *  @brief   	SetRunStatus	 �ı�װ������״̬ 
 *  @param 		[In]a param of Type  int  ָ������״̬
 *  @return 	void 
 */
	//##ModelId=49B87BA801A8
	void SetRunStatus(int nStatus);

/*
 *  @brief   	AddZone	 ���붨ֵ���� 
 *  @param 		[In]a param of Type  PT_ZONE*  ��ֵ���Žṹ
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA801B6
	BOOL AddZone(PT_ZONE* ptz);

/*
 *  @brief   	RefreshCommDurativeTime	 ˢ��ͨ��״̬����ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87BA801B8
	CTimeSpan GetCommDurativeTime();

/*
 *  @brief   	RefreshRunDurativeTime	 ˢ������״̬����ʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87BA801B9
	CTimeSpan GetRunDurativeTime();

/*
 *  @brief   	LoadCommStatusFromSTTP	 ��STTP�����ж�ȡͨ��״̬��Ϣ
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP��������
 *  @param 		[In]a param of Type  int  ����
 *  @return 	void 
 */
	//##ModelId=49B87BA801BA
	void LoadCommStatusFromSTTP(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	LoadRunStatusFromSTTP	 ��STTP�����ж�ȡ����״̬��Ϣ
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP��������
 *  @param 		[In]a param of Type  int  ����
 *  @return 	void 
 */
	//##ModelId=49B87BA801C6
	void LoadRunStatusFromSTTP(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	RefreshStatus	 �ı䱾�豸��״̬Ϊָ���豸��״̬
 *  @param 		[In]a param of Type  CSecObj*  ָ���豸
 *  @return 	void 
 */
	void RefreshStatus(CSecObj* pSec);

/*
 *  @brief   	FindCpu	 ָ��CPU�Ų���CPU��Ϣ 
 *  @param 		[In]a param of Type  int  ָ��CPU��
 *  @return 	CSecCPU* CPU��Ϣ
 */
	CSecCPU* FindCpu(int nCpu);

private:
/*
 *  @brief   	GetActionChr	 �����ݿ��ȡ������������ֵ 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA801C9
	BOOL GetActionChr(CDBEngine* pDBEngine);

/*
 *  @brief   	GetActionSignal	 �����ݿ��ж�ȡ���������ź��� 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA801D4
	BOOL GetActionSignal(CDBEngine* pDBEngine);

/*
 *  @brief   	GetAlarmSignal	 �����ݿ��ж�ȡ�����澯�ź��� 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA801D5
	BOOL GetAlarmSignal(CDBEngine* pDBEngine);

/*
 *  @brief   	ClearConfig	 ���������Ϣ
 *  @param 		[In]a param of Type  int  Ҫ�����Щ������Ϣ.0-ȫ��, 1-������, 2-�����ź�, 3-�澯�ź�
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA801D6
	BOOL ClearConfig(int nAim = 0);

/*
 *  @brief   	GetCPUInfo	 �����ݿ��ȡ����CPU��Ϣ 
 *  @return 	BOOL 
 */
	//##ModelId=49B87BA801E5
	BOOL GetCPUInfo(CDBEngine* pDBEngine);
};

//CSecObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecCPU �����豸CPU��
/**
 * @defgroup 	CSecCPU	�����豸CPU��
 * @{
 */
/**
 * @brief       ���������豸CPU��Ϣ
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA80213
class CSecCPU : public CDeviceObj
{
public:
	//##ModelId=49B87BA80215
	CSecCPU();
	//##ModelId=49B87BA80222
	~CSecCPU();
	DECLARE_DYNCREATE(CSecCPU)
//attribute
public:
	/** @brief           װ��ID*/
	//##ModelId=49B87BA80223
	CString	  PT_ID;
	/** @brief           CPU��*/
	//##ModelId=49B87BA80224
	int		  code;
	/** @brief           CPU����*/
	//##ModelId=49B87BA80225
	CString   des;
	/** @brief           �汾*/
	//##ModelId=49B87BA80226
	CString   ver;
	/** @brief           ��ʼ��ֵ����*/
	//##ModelId=49B87BA80227
	int       from_zone;
	/** @brief           ������ֵ����*/
	//##ModelId=49B87BA80228
	int		  to_zone;
	/** @brief           ���ܺ�*/
	//##ModelId=49B87BA80229
	int       fun;
	/** @brief           �Ƿ�ʹ�ö�ֵ��������.FALSEʱʹ��from_zone & to_zone*/
	//##ModelId=49B87BA80232
	BOOL	  m_bUseSpeZone;
	/** @brief           ��ֵ��������*/
	//##ModelId=49B87BA80234
	CTypedPtrArray<CPtrArray, PT_ZONE*>	m_arrZONE;
	/** @brief           ��CPU�¶�ֵ��������*/
	int		m_nSettingDefs;
	/** @brief           ��CPU�¿�������������*/
	int		m_nDigitalDefs;
	/** @brief           ��CPU����ѹ���������*/
	int		m_nSoftBoardDefs;
	/** @brief           ��CPU��ģ������������*/
	int		m_nAnalogDefs;
//operation
public:
/*
 *  @brief   	Load	 �����ݿ�������� 
 *  @param 		[In]a param of Type  CMemSet *  ���ݼ�ָ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA80238
	BOOL Load(CMemSet * pMemSet);
private:
/*
 *  @brief   	GetZone	 �����ݿ���붨ֵ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA8023A
	BOOL GetZone(CDBEngine* pDBEngine);

/*
 *  @brief   	ClearZone	 ��ն�ֵ�������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA80242
	BOOL ClearZone();
};
//CSecCPU	end
//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_)
