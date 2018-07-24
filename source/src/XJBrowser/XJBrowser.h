// XJBrowser.h : main header file for the XJBROWSER application
//

#if !defined(AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_)
#define AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "XJBrowserDoc.h"
#include "LogMgr.h"			//��־����
#include "DataEngine.h"
#include "SttpEngine.h"		//STTP����
#include "DBEngine.h"		//���ݿ�����
#include "User.h"		//�û���
#include "XJBrowserView.h"
#include "EventManager.h"

#include "ViewFaultDetail.h"	//���ϱ�����ϸҳ��
#include "XJBrowserView.h"		//һ�ν���ͼҳ��
#include "PTSetting.h"			//������ֵҳ��
#include "PTAnalog.h"			//����ģ����ҳ��
#include "PTDigital.h"			//����������ҳ��
#include "PTSoftBoard.h"		//������ѹ��ҳ��
#include "WaveRecSetting.h"		//¼������ֵҳ��
#include "PTAction.h"			//��������ҳ��
#include "PTAlarm.h"			//�����澯ҳ��
#include "AutoCallResult.h"		//�Զ����ٽ��ҳ��
#include "ViewPTComm.h"			//����ͨѶ״̬��ͼ
#include "ViewPTRun.h"			//��������״̬��ͼ
#include "ViewComm.h"			//����վͨѶ״̬��ͼ
#include "GeoView.h"			//����ͼҳ��
#include "ViewStationInfo.h"	//��վ��Ϣҳ��
#include "DownOscView.h"		//¼������ҳ��
#include "ViewCommMap.h"

#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp:
// See XJBrowser.cpp for the implementation of this class
//
//##ModelId=49B87B7D0222
class CXJBrowserApp : public CWinApp
{
public:
	//##ModelId=49B87B7D0224
	CXJBrowserApp();
	//##ModelId=49B87B7D0271
	~CXJBrowserApp();
	
public:
	CBrush   m_brush;
	HANDLE   m_hPostEvent;
	HANDLE   m_hReceiveEvent;
	MENUINFO  lpcmi;  
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           �����ź�*/
	//##ModelId=49B87B7D0280
	HANDLE		hSem;
	/** @brief           LOG�������*/
	//##ModelId=49B87B7D0282
	CLogMgr		m_LogMgr;

	/** @brief           ���ݿ�����*/
	//##ModelId=49B87B7D0287
	CDBEngine	m_DBEngine;

	/** @brief           STTP�������*/
	//##ModelId=49B87B7D0294
	CSttpEngine m_SttpEngine;

	/** @brief           STTP�����Ӿ��*/
	//##ModelId=49B87B7D02A0
	XJHANDLE		m_SttpHandle;

	/** @brief           ��ǰ��¼���û�*/
	//##ModelId=49B87B7D02A5
	CUser  m_User;

	/** @brief           �˳������������̱߳�־, ��ֵΪTRUEʱ�˳��߳�*/
	//##ModelId=49B87B7D02AF
	BOOL	m_bExitClearConfig;

	/** @brief           �˳����ձ����߳�, ��ֵΪTRUEʱ�˳��߳�*/
	//##ModelId=49B87B7D02B0
	BOOL	m_bExitReceiveMsg;

	/** @brief           �����������߳�ָ��*/
	//##ModelId=49B87B7D02C0
	CWinThread* m_pClearConfigThread;
	/** @brief           ���ձ����߳�ָ��*/
	//##ModelId=49B87B7D02CF
	CWinThread* m_pReceiveMsgThread;

	/** @brief           �ַ������߳�ָ��*/
	CWinThread* m_pPostMsgThread;

	CWinThread*	m_pPushMsgThread;

	/** @brief           ���ϱ�����ϸ��Ϣ��ͼָ��,���ڴ�ӡ���ϱ���ʱ�Ļص�����*/
	//##ModelId=49B87B7D02D4
	CViewFaultDetail*  m_pFaultDetail;

	/** @brief           һ�ν���ͼ��ͼָ��, ������˸ͼ��ʱ�Ļص�����*/
	//##ModelId=49B87B7D02DF
	CXJBrowserView*		m_pMapView;

	/** @brief           ������ֵҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
	//##ModelId=49B87B7D02E4
	CPTSetting*			m_pPTSetting;

	/** @brief           ����ģ����ҳ��ָ��, ���ڴ�ӡģ�����Ļص�����*/
	//##ModelId=49B87B7D02F2
	CPTAnalog*			m_pPTAnalog;

	/** @brief           ����������ҳ��ָ��, ���ڴ�ӡ�������Ļص�����*/
	//##ModelId=49B87B7D02FE
	CPTDigital*			m_pPTDigital;

	/** @brief           ������ѹ��ҳ��ָ��, ���ڴ�ӡ��ѹ��Ļص�����*/
	//##ModelId=49B87B7D030E
	CPTSoftBoard*		m_pPTSoftBoard;

	/** @brief           ¼������ֵҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
	//##ModelId=49B87B7D0313
	CPTSetting*	m_pWRSetting;
		/** @brief           ¼����������ҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
		//##ModelId=49B87B7D0313
	CPTDigital*	m_pWRDigital;
	/** @brief           ¼����ģ����ҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
	//##ModelId=49B87B7D0313
	CPTAnalog*	m_pWRAnalog;
	/** @brief           ��������ҳ��ָ��, ���ڴ�ӡ�����¼��б�Ļص�����*/
	//##ModelId=49B87B7D031D
	CPTAction*			m_pPTAction;

	/** @brief           �����澯ҳ��ָ��, ���ڴ�ӡ�澯�¼��б�Ļص�����*/
	//##ModelId=49B87B7D0322
	CPTAlarm*			m_pPTAlarm;

	/** @brief           �Զ����ٽ��ҳ��ָ��, ���ڴ�ӡ�Զ����ٽ���б�Ļص�����*/
	//##ModelId=49B87B7D0330
	CAutoCallResult*	m_pAutoCallResult;

	/** @brief           ����վͨ��״̬ҳ��ָ��, ���ڴ�ӡ����վͨ��״̬�б�Ļص�����*/
	//##ModelId=49B87B7D033D
	CViewComm*			m_pComm;

	/** @brief           ����ͨ��״̬ҳ��ָ��, ���ڴ�ӡ�Ļص�����*/
	//##ModelId=49B87B7D0342
	CViewPTComm*		m_pPTComm;

	/** @brief           ��������״̬ҳ��ָ��, ���ڴ�ӡ�Ļص�����*/
	//##ModelId=49B87B7D034F
	CViewPTRun*			m_pPTRun;

	/** @brief           ��վ��Ϣҳ��ָ��, ���ڴ�ӡ�Ļص�����*/
	CViewStationInfo*	m_pStationInfo;

	/** @brief           ��ͨѶ�������������.TRUE-��������.FALSE-�����ж�*/
	//##ModelId=49B87B7D0360
	BOOL m_bConnect;
	/** @brief           ���鱣��ͨ�����*/
	int	m_ConnectStatus[2];

	BOOL m_bGetSysLogPath;

	//�ĵ����ڹ���	
	//##ModelId=49B87B7E030D
	enum WindowType
	{
		//##ModelId=49B87B7E031C
		wtMap			= 0,	//�����߻��洰��
		//##ModelId=49B87B7E031D
		wtCommuView		= 1,    //����վͨѶ״̬����
		//##ModelId=49B87B7E031E
		wtAction		= 2,	//�����¼�����
		//##ModelId=49B87B7E031F
		wtAlarm			= 3,	//�澯�¼�����
		//##ModelId=49B87B7E032C
		wtFault			= 4,	//�¹ʱ��洰��
		//##ModelId=49B87B7E032D
		wtOSC			= 5,	//¼�����洰��
		//##ModelId=49B87B7E032E
		wtDI			= 6,	//�������¼�����
		//##ModelId=49B87B7E032F
		wtPTCommStatus	= 7,	//����ͨѶ״̬����
		//##ModelId=49B87B7E0330
		wtPTRunStatus	= 8,	//��������״̬����
		//##ModelId=49B87B7E033C
		wtSysAlarm		= 9,	//ϵͳ�澯���¼�����
		//##ModelId=49B87B7E033D
		wtPTSheet		= 10,	//�������Դ���
		//##ModelId=49B87B7E033E
		wtWaveRecSheet	= 11,	//¼�������Դ���
		//##ModelId=49B87B7E033F
		wtBatchCall		= 12,	//�����ٻ�����
		//##ModelId=49B87B7E034B
		wtAutoCall		= 13,	//�Զ��������ô���
		//##ModelId=49B87B7E034C
		wtAutoCallResult= 14,	//�Զ����ٽ������
		//##ModelId=49B87B7E034D
		wtDownload		= 15,	//ͨ���ļ����ش���
		wtCross			= 16,	//Ԥ������
		wtSOEI			= 17,	//I���¼�����
		wtSOEII			= 18,	//II���¼�����
		wtSOEIII		= 19,	//III���¼�����
		wtSOEDebug		= 20,	//������Ϣ����
		wtGeo			= 21,	//����ͼ����
		wtStationInfo	= 22,	//��վ��Ϣ����
		wtDownOsc		= 23,	//¼������ҳ��
		wtStaInfo       = 24,    //��վ��Ϣ����
		wtDistance		= 25,	//С�������Ϣ����
		wtDisDevSheet	= 26,	//���װ������ҳ��
		wtDraft			= 27	//ʩ��ͼҳ��
	};
	enum { maxDocCount=28 };	//����ĵ���
	//##ModelId=49B87B7D0354
	CMultiDocTemplate *m_pDocTempl[maxDocCount];	//�����б�
	//##ModelId=49B87B7D035C
	CDocument* m_pDoc[maxDocCount];					//�ĵ��б�

	//CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	CPtrList m_listFullData;

private:
	/** @brief           �ļ�����·��*/
	//##ModelId=49B87B7D036B
	CString	m_sDownDir;
	/** @brief           ϵͳ��־�ļ�·��*/
	//##ModelId=49B87B7D037A
	CString m_sSysLogPath;

	/** @brief           ����洢���û���*/
	CString m_sNetworkUser;
	/** @brief           ����洢������*/
	CString m_sNetworkPassword;

	BOOL	m_bCanLock;
	BOOL	m_bIsLocked;

	
public:
	void LockUI();

	/*
	 *  @brief   	MgrAllStations ��ǰ�û��Ƿ�ɹ���������վ	 
	 *  @return 	BOOL	 
	 */
	BOOL MgrAllStations(){return m_User.m_bAllStation;};

	/*
	 *  @brief   	MgrStation �ж�ָ����վ�Ƿ��ڵ�ǰ�û��Ĺ���Χ��	 
	 *	@param 		[In]a param of Type  CString stationID	
	 *  @return 	BOOL	 
	 */
	BOOL MgrStation(CString stationID);

private:

	void QueryUserStation();

	void StartClearConfigThread();
	
/*
*  @brief   	InitDocTemplates	 ��ʼ���ĵ����� 
*  @return 	void 
	*/
	//##ModelId=49B87B7D037B
	void InitDocTemplates();

/*
 *  @brief   	DoLogin	 ����û����,��¼ϵͳ 
 *  @return 	BOOL ��¼�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7D0399
	BOOL DoLogin();

/*
 *  @brief   	DOLogOut	 �ǳ�ϵͳ,�����û���¼ 
 *  @return 	BOOL �ٵ�¼�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7D03B9
	BOOL DoLogOut();

	/*
	* @brief	JudgeOperTableType	�ж���־������
	* @return   void	
	*/
	void JudgeOperTableType();
public:

	void XJSetMenuInfo();
	/*
 *  @brief   	TryEnter	 ����ĳ�û��Ƿ���н���ĳ���ܵ�Ȩ��,Ŀǰֻ֧�ֲ�ѯ��ǰ�û�����
 *  @param 		[In]a param of Type  LPCTSTR  ����ID
 *  @param 		[In]a param of Type  LPCTSTR  �û�ID, NULLʱ�õ�ǰ�û�����
 *  @param 		[In]a param of Type  BOOL  �Ƿ���ʾ���洰
 *  @return 	BOOL �ɽ���ĳ���ܵ�Ȩ�޷���TRUE,�����÷���FALSE
 */
	//##ModelId=49B87B7D03D8
	BOOL TryEnter(LPCTSTR strFuncID, LPCTSTR strUserID = NULL, BOOL bShowAlarm = TRUE);

/*
*  @brief   	GetCurDocument	 �õ���ǰ�ĵ� 
*  @return 	CXJBrowserDoc*	 �ĵ���ָ��
	*/
	//##ModelId=49B87B7E000F
	CXJBrowserDoc* GetCurDocument();

	void SetAppTile(CString sTitle, CString sDetail = "");
	
	/*
	*  @brief   	GetCurView	 �õ���ǰ��ͼ 
	*  @return 	CView*		 ��ͼ��ָ��
	*/
	//##ModelId=49B87B7E002E
	CView* GetCurView();

/*
 *  @brief   	GetDataEngine	 �õ���������ָ�� 
 *  @return 	CDataEngine * ��������ָ��
 */
	//##ModelId=49B87B7E004E
	CDataEngine * GetDataEngine();
	
/*
 *  @brief   	WriteLog	 д��־ 
 *  @param 		[In]a param of Type  CString  ��־����
 *  @param 		[In]a param of Type  int  ��־����ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B7E005D
	void WriteLog(CString sLog, int nLV = XJ_LOG_LV3);

/*
 *  @brief   	WriteLog	 д��־�ļ�����־�ļ�, ��Ϊ�����߳���д��־����ڵȴ��̹߳ر�ʱ���������� 
 *  @param 		[In/Out]a param of Type  CString  ��־����
 *  @param 		[In/Out]a param of Type  int  ��־����ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B7E008C
	void WriteLogEx(CString sLog, int nLV = XJ_LOG_LV3);

/*
 *  @brief   	STTPConnect	 ������ͨѶ������������
 *	@param		[Out]a param of Type int&	������ӽ�� 0-ʧ��, 1-�ɹ�
 *  @param 		[In]a param of Type  int  ����ģʽ. 0��ͬ��ģʽ��Ĭ�ϣ�, 1���첽ģʽ
 *  @return 	XJHANDLE ������Sttp�ͻ��˶���ľ����ʧ��ʱ����NULL
 */
	//##ModelId=49B87B7E009C
	XJHANDLE STTPConnect(int& pConnected, int pRunModle = 0);

/*
 *  @brief   	ConnectSTTPServer	 ������ͨѶ������������, ���������ձ����߳� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7E00BB
	BOOL ConnectSTTPServer();
	BOOL ConnectSTTPServerB();
/*
 *  @brief   	DisConnectSTTPServer	 �Ͽ���ͨѶ������������, ���رս��ձ����߳� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7E00DA
	BOOL DisConnectSTTPServer();

/*
 *  @brief   	SetSTTPStatus	 ������ͨѶ����������״̬��־ 
 *  @param 		[In]a param of Type  BOOL  TRUE��ʾ��������, FALSE��ʾ����ʧ��
 *  @return 	void 
 */
	//##ModelId=49B87B7E00EA
	void SetSTTPStatus(BOOL bConnected, int group);

/*
 *  @brief   	GetSTTPStatus	 ȡ�õ�ǰ��ͨѶ������������״̬ 
 *  @return 	BOOL ͨѶ��������TRUE, ͨѶ�жϷ���FALSE
 */
	//##ModelId=49B87B7E0109
	BOOL GetSTTPStatus();

/*
 *  @brief   	AddManOperator	 �����˹�������־ 
 *  @param 		[In]a param of Type  CString  ����ģ��ID
 *  @param 		[In]a param of Type  CString  ������������
 *  @return 	void
 */
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult  = 0, int num = -1);
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult  = 0, int num  = -1);

	BOOL NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sRecords = CString());
	int  GetPTSetModState(PT_ZONE &data, CString &sRecords = CString(), CString &sFlag = CString());
	BOOL RevertPTSetModState(int nRevertStateID, int nFlag = 0);
	BOOL SetRevertModifyValueFlag(int nFlag);
	CString GetUserIDByState(int nState, CString &sRecords = CString());
	BOOL SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag = 0);
	int CheckKeyNameForPTSet();

	CString GetUserGroupNameByID(CString sUserGroupID);
	BOOL SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));
	vector<CString> SplitCString(CString &str,const CString find, int limit=0);

/*
 *  @brief   	GetSVGView	 �õ�����ͼָ�� 
 *  @return 	CXJBrowserView * ������ͼָ��
 */
	//##ModelId=49B87B7E0128
	CXJBrowserView * GetSVGView();
	CViewCommMap * GetCommView();
	CGeoView* GetGeoView();
/*
 *  @brief   	LoadDownDir	  �����ݿ��ж�ȡ�ļ�����·��,������m_sDownDir��
 *  @return 	void 
 */
	//##ModelId=49B87B7E0148
	void LoadDownDir();

/*
 *  @brief   	GetDownDir	 ȡ��ϵͳ����·�� 
 *  @return 	CString ϵͳ����·��
 */
	//##ModelId=49B87B7E0157
	CString GetDownDir();

/*
 *  @brief   	LoadSysLogPath	 �����ݿ��ж�ȡ��־�ļ�·��,������m_sSysLogPath��
 *  @return 	void 
 */
	//##ModelId=49B87B7E0158
	void LoadSysLogPath();

/*
 *  @brief   	GetSysLogPath	 ȡ����־�ļ�·�� 
 *  @return 	CString ��־�ļ�·��
 */
	//##ModelId=49B87B7E0167
	CString GetSysLogPath();

/*
 *  @brief   	ShowDevice	 ��ʾĳ�豸ͼ�λ��� 
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @return 	void 
 */
	//##ModelId=49B87B7E0168
	void ShowDevice(CDeviceObj* pObj, BOOL bMoveFirst = TRUE);

/*
 *  @brief   	ShowStationInComm	 �ڳ�վ״̬ͼ������ʾ��վͼ�� 
 *  @param 		[In]a param of Type  CStationObj* ָ����վ
 *  @return 	void 
 */
	void ShowStationInComm(CStationObj* pObj, BOOL bMoveFirst = TRUE);

/*
 *  @brief   	LocateInTree	 ���豸���ж�λ��վ 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87B7E0177
	void LocateInTree(CStationObj* pObj);

/*
 *  @brief   	ShowEventProp	 ��ʾ�¼����ԶԻ��� 
 *  @param 		[In]a param of Type  CXJEventManager*  �¼�����ָ��
 *	@param		[In]a param of Type BOOL	�Ƿ��Ǳ�������ҳ��򿪵ĶԻ���
 *  @return 	void 
 */
	//##ModelId=49B87B7E0186
	void ShowEventProp(CXJEventManager* pEventManager, BOOL bPT = FALSE);

/*
 *  @brief   	EndOwnerThread	 ֹͣ�Լ���������߳� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01A5
	void EndOwnerThread();

/*
 *  @brief   	LoadConfig	 �������������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B5
	void LoadConfig();

/*
 *  @brief   	SaveConfig	 ������������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B6
	void SaveConfig();

/*
 *  @brief   	LoadSysConfig	 �������ļ��ж���ϵͳ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B7
	void LoadSysConfig();

/*
 *  @brief   	SaveSysConfig	 ����ϵͳ���õ������ļ�
 *  @return 	void 
 */
	//##ModelId=49B87B7E01C5
	void SaveSysConfig();

/*
 *  @brief   	LoadEventConfig	 �������ļ������¼����� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01D4
	void LoadEventConfig();

/*
 *  @brief   	SaveEventConfig	 �����¼����õ������ļ�
 *  @return 	void 
 */
	//##ModelId=49B87B7E01D5
	void SaveEventConfig();

/*
 *  @brief   	LoadCommStatus	 �������ļ���������վͨѶ״̬ҳ������
 *  @return 	void 
 */
	//##ModelId=49B87B7E01E4
	void LoadCommStatusConfig();

/*
 *  @brief   	SaveCommStatus	 ��������վͨѶ״̬ҳ�����õ������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01E5
	void SaveCommStatusConfig();

/*
 *  @brief   	LoadActionViewConfig	 �������ļ����붯���¼�ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01F4
	void LoadActionViewConfig();

/*
 *  @brief   	SaveActionViewConfig	 ���涯���¼�ҳ�����õ������ļ�
 *  @return 	void 
 */
	//##ModelId=49B87B7E01F5
	void SaveActionViewConfig();

/*
 *  @brief   	LoadAlarmViewConfig	 �������ļ�����澯�¼�ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0203
	void LoadAlarmViewConfig();

/*
 *  @brief   	SaveAlarmViewConfig	 ����澯�¼�ҳ�����õ������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0204
	void SaveAlarmViewConfig();

/*
 *  @brief   	LoadSysAlarmViewConfig	 �������ļ�����ϵͳ�澯ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0213
	void LoadSysAlarmViewConfig();

/*
 *  @brief   	SaveSysAlarmViewConfig	 ����ϵͳ�澯ҳ�����õ������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0214
	void SaveSysAlarmViewConfig();

/*
 *  @brief   	LoadBatchConfig	 ���������ٻ����ô������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0222
	void LoadBatchConfig();

/*
 *  @brief   	SaveBatchConfig	 ���������ٻ����õ������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0223
	void SaveBatchConfig();

/*
 *  @brief   	LoadPTCommViewConfig	 ���뱣��ͨѶ״̬ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0224
	void LoadPTCommViewConfig();

/*
 *  @brief   	SavePTCommViewConfig	 ���汣��ͨѶ״̬ҳ�����õ������ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0232
	void SavePTCommViewConfig();

/*
 *  @brief   	LoadPTRunViewConfig	 ���뱣������״̬ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0233
	void LoadPTRunViewConfig();

/*
 *  @brief   	SavePTRunViewConfig	 ���汣������״̬ҳ������
 *  @return 	void 
 */
	//##ModelId=49B87B7E0234
	void SavePTRunViewConfig();

/*
 *  @brief   	LoadPTPropConfig	 ���뱣������ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0242
	void LoadPTPropConfig();

/*
 *  @brief   	SavePTPropConfig	 ���汣������ҳ������ 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0243
	void SavePTPropConfig();

/*
 *  @brief   	LoadWndConfig	 ���봰����ʾ���� 
 *  @return 	void 
 */
	void LoadWndConfig();

/*
 *  @brief   	SaveWndConfig	 ���洰����ʾ���� 
 *  @return 	void 
 */
	void SaveWndConfig();

/*
 *  @brief   	LoadSplitterWndConfig	 ����ָ����ڴ�С���� 
 *  @return 	void 
 */
	void LoadSplitterWndConfig();

/*
 *  @brief   	SaveSplitterWndConfig	 ����ָ����ڴ�С���� 
 *  @return 	void 
 */
	void SaveSplitterWndConfig();

/*
 *  @brief   	OpenDefaultStation	 ��Ĭ�ϵĳ�վ 
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	void 
 */
	//##ModelId=49B87B7E0244
	void OpenDefaultStation(CString strID);
	void OpenDefaultStation();

/*
 *  @brief   	QueryUserGroup	 ָ���û��������û����� 
 *  @param 		[In]a param of Type  CString  �û���
 *  @return 	CString �û�����
 */
	//##ModelId=49B87B7E0251
	CString QueryUserGroup(CString sUserID);

/*
 *  @brief   	DoSysSetting	 ����ϵͳ����ҳ��
 *  @param 		[In]a param of Type  int  ��ʼ���ĸ�SHEETҳ. SYS_SETTING_GENERALΪ��������ҳ��. SYS_SETTING_INFOCASEΪ��Ϣ����ҳ��. const SYS_SETTING_STATUSΪ״̬����ҳ��. SYS_SETTING_PTPTOPΪ������������ҳ��
 *  @return 	void 
 */
	//##ModelId=49B87B7E0253
	void DoSysSetting(int nPage);

/*
 *  @brief   	ClearAllTempView	 ����������ʱ��ͼָ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0261
	void ClearAllTempView();

/*
 *  @brief   	AddNetworkDisk	 ��������洢�� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddNetworkDisk();

/*
 *  @brief   	ShowStationInfo	 ��ָ����վ�ĳ�վ��Ϣ��ͼ 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void ShowStationInfo(CStationObj* pStation);

/*
 *  @brief   	ShowDownOsc	 ��¼�����ش��ڣ�������ָ��¼�� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ�����ص�¼��
 *  @return 	void 
 */
	void ShowDownOsc(CXJEventOSC* pOsc);

/*
 *  @brief   	AddSTTPData	 ��STTP���뵽���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  sttp����
 *  @return 	void 
 */
	void AddSTTPData(STTP_FULL_DATA* sttpData);

	STTP_FULL_DATA* GetFirstSTTPData();

/*
 *  @brief   	RemoveSTTPData	 ��STTP���еĵ�һ������ɾ�� 
 *  @return 	void 
 */
	void RemoveSTTPData();

	void RemoveAllSTTPData();

/*
 *  @brief   	QueryPtRunStatus	 ��ѯ�����豸����״̬
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In,out]a param of Type  int  ״ֵ̬
 *  @param 		[In,out]a param of Type  CString  �����豸����ʱ��
 *  @return 	void 
 */
	BOOL QueryPtRunStatus(CString strPtID,  int& nRunStatus, CString& strRunTime);

/*
 *  @brief   	UpdatePtRunStatus	 ���¶����豸����״̬
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In]a param of Type  ״ֵ̬
 *  @return 	void 
 */
	void UpdatePtRunStatus(CString strPtID, int nRunStatus);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXJBrowserApp)
	public:
	virtual BOOL InitInstance();
	
	
	
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXJBrowserApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnSttpConnect();
	afx_msg void OnUpdateSttpConnect(CCmdUI* pCmdUI);
	afx_msg void OnSttpDisconnect();
	afx_msg void OnUpdateSttpDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnSysLogout();
	afx_msg void OnUpdateSysLogout(CCmdUI* pCmdUI);
	afx_msg void OnSysExit();
	afx_msg void OnUpdateSysExit(CCmdUI* pCmdUI);
	afx_msg void OnHelpContent();
	afx_msg void OnUpdateHelpContent(CCmdUI* pCmdUI);
	afx_msg void OnSysSetting();
	afx_msg void OnUpdateSysSetting(CCmdUI* pCmdUI);
	afx_msg void OnAppCall();
	afx_msg void OnCallAll();
	afx_msg void OnFileDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


typedef struct
{
	CDeviceObj* pPoint;
	int   nId;
}TimerTick;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_)
