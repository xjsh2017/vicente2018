// ***************************************************************
//  DataEngine   version:  1.0    date: 16:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ����������,�����ȡ�ʹ洢���豸����
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************

#if !defined(AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_)
#define AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataEngine.h : header file
//
#include "DeviceObj.h"
#include "vccommon_define.h"
#include "DBEngine.h"
#include "../OXSplashWnd/OXSplashWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CDataEngine document
/**
 * @defgroup 	CDataEngine	����������
 * @{
 */
/**
 * @brief       ������豸����,������Ϣ�Ķ�ȡ�ʹ洢
*  @author      LYH
 * @version     ver1.0
 * @date        16/09/08
/** @} */ //OVER

//##ModelId=49B87BA9003E
class CDataEngine
{
public:
	CDataEngine(CDBEngine* pDBEngine);

// Attributes
public:
	/** @brief			�����ȴ�����*/
	COXSplashWnd*	m_pSplashWnd;
	/** @brief			���ݿ��������*/
	CDBEngine*		m_pDBEngine;
	/*
	* @brief	GetDBEngine			
	* @return   CDBEngine*	
	*/	
	CDBEngine* GetDBEngine() const { return m_pDBEngine; }

	/*
	* @brief	SetDBEngine	
	* @param	[In]a param of Type CDBEngine*		
	* @return   	
	*/	
	void SetDBEngine(CDBEngine* DBEngine) { m_pDBEngine = DBEngine; }
	/** @brief           ��������*/
	//##ModelId=49B87BA9004F
	NET_LIST		m_NetList;
	/** @brief           ��վ����*/
	//##ModelId=49B87BA9005E
	CMapStringToOb	m_StationMap;
	/** @brief           ��վͨ��ԭ���*/
	STATION_STATUS_CONFIG_MAP m_StationStatusConfigMap;

	/** @brief           ���ж����豸����, ����ˢ��״̬*/
	SEC_LIST		m_AllSEC;

	/** @brief           ͨ�ù����������б�����tb_pt_character*/
	//##ModelId=49B87BA90063
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_arrUniversalActChr;

	/** @brief           �����ٻ���ֵ��ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90067
	int		m_nPTSettingTimeOut;
	/** @brief           ¼�����ٻ���ֵ��ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90068
	int		m_nWRSettingTimeOut;
	/** @brief           �����ٻ�ģ������ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9006D
	int		m_nPTAiTimeOut;
	/** @brief           �����ٻ���������ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9006E
	int		m_nPTDiTimeOut;
	/** @brief           �����ٻ���ѹ�峬ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9006F
	int		m_nPTSoftBoardTimeOut;
	/** @brief           �����ٻ���ֵ���ų�ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90070
	int		m_nPTZoneTimeOut;
	/** @brief           �ٻ�װ��ʱ�䳬ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90071
	int		m_nDevTimeTimeOut;
	/** @brief           �ٻ�װ�û�����Ϣ��ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9007D
	int		m_nDevGeneralTimeOut;
	/** @brief           ��ֵ�޸ĳ�ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9007E
	int		m_nModifySettingTimeOut;
	/** @brief           ��ֵ���л���ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9007F
	int		m_nChangeZoneTimeOut;
	/** @brief           ��ѹ���л���ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90080
	int		m_nChangeSBTimeOut;
	/** @brief           װ�ö�ʱ��ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA90081
	int		m_nVerifyTimeTimeOut;
	/** @brief           �źŸ��鳬ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9008C
	int		m_nSignalResetTimeOut;
	/** @brief           Զ��������ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9008D
	int		m_nRemoteStartTimeOut;
	/** @brief           �ٻ�¼���ļ���ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9008E
	int		m_nWavFileTimeOut;
	/** @brief           �ٻ�¼���б�ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9009C
	int		m_nWavFileListTimeOut;
	/** @brief           �ٻ���ʷ�¼���ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9009D
	int		m_nHistoryEventTimeOut;
	/** @brief           �ٻ�ͨ���ļ���ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA9009E
	int		m_nCommFileTimeOut;
	/** @brief           �ٻ�ͨ���ļ��б�ʱʱ��(��λ:��)*/
	//##ModelId=49B87BA900AB
	int		m_nCommFileListTimeOut;
	/** @brief           �ٻ�����״̬ʱ��(��λ:��)*/
	int     m_nCommStatusTimeOut;

	CRITICAL_SECTION m_criSection;
// Operations
public:
	/** @brief			����Ķ����豸������*/
	int m_nSecType;
	/** @brief			���ݶ��뷽ʽ. 0-һ�ζ��룻 1-����վ���߳�*/
	int m_nLoadDataModel;
	/** @brief			�Ƿ���ص�����Ϣ*/
	BOOL m_bLoadNet;
	/** @brief			�Ƿ����20162����*/
	BOOL m_bLoad20162Config;
	/** @brief			�Ƿ���20162֪ͨ*/
	BOOL m_bSend20162;
	/** @brief			�Ƿ����CPU��Ϣ*/
	BOOL m_bLoadCPUInfo;
	/** @brief			�Ƿ���ض�ֵ������Ϣ*/
	BOOL m_bLoadZoneInfo;
private:
	/** @brief			ϵͳ����·��*/
	CString		m_sDownDir;
	/** @brief			ϵͳ��־·��*/
	CString		m_sLogDir;

private:
	void LoadDownDir();
/*
 *  @brief   	BuildNetList	 ������������ 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900AC
	BOOL BuildNetList();

/*
 *  @brief   	BuildStationList	 ������վ���� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900BB
	BOOL BuildStationList();

/*
 *  @brief   	BuildBusList	 ����ĸ������ 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900DA
	BOOL BuildBusList();

/*
 *  @brief   	BuildBreakList	 ������������
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900DB
	BOOL BuildBreakList();

/*
 *  @brief   	BuildBayList	 ����������� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900EA
	BOOL BuildBayList();

/*
 *  @brief   	BuildLineList	 ������·���� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90109
	BOOL BuildLineList();

/*
 *  @brief   	BuildCapList	 �������������� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA9010A
	BOOL BuildCapList();

/*
 *  @brief   	BuildReactorList	 �����翹������ 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90119
	BOOL BuildReactorList();

/*
 *  @brief   	BuildTransList	 ������ѹ������ 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90128
	BOOL BuildTransList();

/*
 *  @brief   	BuildGenList	�������������
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90129
	BOOL BuildGenList();

/*
 *  @brief   	BuildSwitchList	 ������բ���� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90138
	BOOL BuildSwitchList();

/*
 *  @brief   	BuildPmsList	 �����������������s 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90139
	BOOL BuildPmsList();

/*
 *  @brief   	BuildSecList	 ���������豸���� 
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90148
	BOOL BuildSecList();


/*
 *  @brief   	BuildCpuList	 ����CPU����
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90149
	BOOL BuildCpuList();

/*
 *  @brief   	BuildZoneList	 ������ֵ�������� 
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA9014A
	BOOL BuildZoneList();

/*
 *  @brief   	BuildZoneList	 ������ֵ��������,��tb_secdev_zone_base��ȡ����
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildZoneListOld();

/* 
 *  @brief   	BuildStationStatusConfig()  ������վͨ��ԭ���
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA900AC
	BOOL BuildStationStatusConfig();

/*
 *  @brief   	Clear	 ������� 
 *  @return 	void 
 */
	//##ModelId=49B87BA90158
	void Clear();

/*
 *  @brief   	ClearRefreshSec	 �������ˢ���豸״̬�Ķ����豸���� 
 *  @return 	void 
 */
	void ClearRefreshSec();


/*
 *  @brief   	BuildUniversalActChr	 ����ͨ�õĶ��������� 
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA90159
	BOOL BuildUniversalActChr();

/*
 *  @brief   	GetFuncTimeOut	 �����ݿ���ȡ�ø�ͨ�Ų������ܳ�ʱʱ�� 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA9015A
	BOOL GetFuncTimeOut();

public:
/*
 *  @brief   	QueryHasSettingConfig	 ��ѯ�������豸�Ƿ��ж�ֵ���� 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryHasSettingConfig();

/*
 *  @brief   	QueryHasDigitalConfig	 ��ѯ�������豸�Ƿ��п��������� 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryHasDigitalConfig();

/*
 *  @brief   	QueryHasSoftBoardConfig	 ��ѯ�������豸�Ƿ�����ѹ������ 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryHasSoftBoardConfig();

/*
 *  @brief   	QueryHasAnalogConfig	 ��ѯ�������豸�Ƿ���ģ�������� 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryHasAnalogConfig();


/*
 *  @brief   	LoadAllSec	 �������ж����豸 
 *  @return 	void 
 */
	void LoadAllSec();

/*
 *  @brief   	RefreshDevStatus	 ˢ���豸״̬ 
 *  @return 	void 
 */
	void RefreshDevStatus();

/*
 *  @brief   	LoadData	 �������� 
 *  @return 	BOOL ����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA9015B
	BOOL LoadData();

/*
 *  @brief   	FindDevice	 �����豸 
 *  @param 		[In]a param of Type  CString  �豸ID
 *  @param 		[In]a param of Type  int  �豸����
 *  @return 	CDeviceObj * �豸����ָ��
 */
	//##ModelId=49B87BA90167
	CDeviceObj * FindDevice(CString DeviceID, int DeviceType, CString StationID = "");

/*
 *  @brief   	FindDeviceBy103	 ͨ��103�����Ŀ�Ų����豸 
 *  @param 		[In]a param of Type  CString  103�����Ŀ��,��ʽ: ���#��Ŀ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	CDeviceObj * �豸ָ��
 */
	//##ModelId=49B87BA9016A
	CDeviceObj * FindDeviceBy103(CString str103, int DeviceType, CString strStaionID);


/*
 *  @brief   	FindDeviceBy103	 ����uri�����豸 
 *  @param 		[In]a param of Type  CString uri
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	CDeviceObj * �豸ָ��
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceByUri(CString strUri, CString strStaionID);



/*
 *  @brief   	QueryStationStatus	 ��ѯ��վ״̬ 
 *  @return 	void 
 */
	void QueryStationStatus();


	void Load20162Config();

/*
 *  @brief   	GetPriDeviceList	 ȡ��һ���豸���� 
 *  @param 		[In]a param of Type  DEVICE_LIST& ���ڱ���һ���豸������
 *  @param 		[In]a param of Type  int ָ��Ҫ���ҵ�һ���豸���ͣ�-1��ʾ��������
 *  @param 		[In]a param of Type  CString ָ��������վID��Ϊ��ʱ��ʾ���г�վ
 *  @return 	
 */
	void GetPriDeviceList(DEVICE_LIST& listDevice, int nDeviceType, CString sStationID="", CString sNetID = "");

/*
 *  @brief   	GetSecList	 ȡ�ö����豸���� 
 *  @param 		[In]a param of Type  DEVICE_LIST& ���ڱ�������豸������
 *  @param 		[In]a param of Type  CString ָ��һ���豸��Ϊ��ʱ��ʾ����
 *  @param 		[In]a param of Type  int ָ��Ҫ���ҵĶ����豸���ͣ�-1��TYPE_SEC��ʾ��������
 *  @param 		[In]a param of Type  CString ָ��������վID��Ϊ��ʱ��ʾ���г�վ
 *  @return 	
 */
	void GetSecList(DEVICE_LIST& listDevice, CString sPriID = "", int nSecType = TYPE_SEC, CString sStationID = "", CString sNetID = "");

	void GetSecList(DEVICE_LIST& listDevice, int nPriType = 0, int nSecType = TYPE_SEC, CString sStationID = "", CString sNetID = "");

/*
 *  @brief   	GetStationList	 ȡ�ó�վ���� 
 *  @param 		[In]a param of Type  DEVICE_LIST& ���ڱ��泧վ������
 *  @param 		[In]a param of Type  CString ָ������ID��Ϊ��ʱ��ʾ���е���
 *  @return 	
 */
	void GetStationList(DEVICE_LIST& listDevice, CString sNetID = "");

	/*
	* @brief	GetDownDir	ȡ��ϵͳ����·��
	* @return   CString	ϵͳ����·��
	*/
	CString GetDownDir();

	/*
	* @brief	SetSecCommStatus	���ö����豸ͨ��״̬
	* @param	[In]a param of Type CSecObj * pSec	ָ�������豸	
	* @param	[In]a param of Type int nStatus		ָ��״̬
	* @return   void	
	*/
	void SetSecCommStatus(CSecObj* pSec, int nStatus);

	/*
	* @brief	SetSecRunStatus	���ö����豸����״̬
	* @param	[In]a param of Type CSecObj * pSec	ָ�������豸	
	* @param	[In]a param of Type int nStatus	ָ��״̬	
	* @return   void	
	*/
	void SetSecRunStatus(CSecObj* pSec, int nStatus);

/*
 *  @brief   	LoadSysLogPath	 �����ݿ��ȡ��־�ļ�·�� 
 *  @return 	CString ��־�ļ�·��
 */
	CString LoadSysLogPath();

/*
 *  @brief   	GetSysLogPath	 ȡ����־�ļ�·�� 
 *  @return 	CString ��־�ļ�·��
 */
	//##ModelId=49B87B7E0167
	CString GetSysLogPath(CString sProjectName, CString sLocalPath);

// Implementation
public:
	//##ModelId=49B87BA90179
	virtual ~CDataEngine();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_)
