// ***************************************************************
//  BatchDetailSettingView   version:  1.0    date: 3:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����ٻ��豸ѡ�񼰸��豸���ٻ�ѡ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
#define AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchDetailSettingView.h : header file
//
#include "StdAfx.h"
#include "ViewBase.h"
#include "BatchSel.h"
#include "ReportCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailSettingView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



/**
 * @defgroup 	CBatchDetailSettingView	�����ٻ��豸������
 * @{
 */
/**
 * @brief       ���������ٻ��豸ѡ�񼰸��豸���ٻ�ѡ��
*  @author      LYH
 * @version     ver1.0
 * @date        03/11/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BB103D8
class CBatchDetailSettingView : public CViewBase
{
protected:
	//##ModelId=49B87BB103DA
	CBatchDetailSettingView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBatchDetailSettingView)

// Form Data
public:
	//{{AFX_DATA(CBatchDetailSettingView)
	enum { IDD = IDD_BATCH_DETAIL_SETTING };
	CButton         	m_btnCallResult;
	CButton 	        m_btnStartCall;
	CButton         	m_btnCallStop;
	CButton         	m_btnAutoClose;
	CButton          	m_btnDeviceList;
	CButton	m_btnReset;
	CButton	m_btnSet;
	CButton	m_btnSelWR;
	CButton	m_btnSelPT;
	CButton	m_btnSave;
	
	CPictureEx	m_gif;
	CReportCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           �Զ��ٻ���һ������ʱ��*/
	//##ModelId=49B87BB20072
	CString		m_sStartTime;
	/** @brief           �Զ��ٻ�����*/
	//##ModelId=49B87BB2007D
	CString		m_sCyc;
	/** @brief           ��ͼ����, 0��ʾ���Զ����ٵ�������ͼ. 1��ʾ�������ٻ���ͼ*/
	//##ModelId=49B87BB20119
	int		m_nViewType;

private:
	//##ModelId=49B87BB2008D
	CThemeHelperST	m_ThemeHelper;
	/** @brief           �б������Ϣ*/
	//##ModelId=49B87BB20092
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           ѡ��������*/
	//##ModelId=49B87BB2009
	BATCHSEL_MAP		m_mapSel;
	/** @brief           �������*/
    BATCHSEL_MAP        m_mapSave;
	/** @brief           ��ֵ�����Ƿ��ٻ�*/
	BOOL	m_bZone;
	/** @brief           ��ֵ�Ƿ��ٻ�*/
	//##ModelId=49B87BB200AB
	BOOL	m_bSetting;
	/** @brief           �������Ƿ��ٻ�*/
	//##ModelId=49B87BB200AC
	BOOL	m_bDigital;
	/** @brief           ��ѹ���Ƿ��ٻ�*/
	//##ModelId=49B87BB200BB
	BOOL	m_bSoftBoard;
	/** @brief           ģ�����Ƿ��ٻ�*/
	//##ModelId=49B87BB200CB
	BOOL	m_bAnalog;

	/** @brief           �˵����ǰ��¼�Ƿ��ٻ���ֵ����*/
	BOOL	m_bMenuZone;
	/** @brief           �˵����ǰ��¼�Ƿ��ٻ���ֵ*/
	//##ModelId=49B87BB200CC
	BOOL	m_bMenuSetting;
	/** @brief           �˵����ǰ��¼�Ƿ��ٻ�������*/
	//##ModelId=49B87BB200DA
	BOOL	m_bMenuDigital;
	/** @brief           �˵����ǰ��¼�Ƿ��ٻ���ѹ��*/
	//##ModelId=49B87BB200DB
	BOOL	m_bMenuSoftBoard;
	/** @brief           �˵����ǰ��¼�Ƿ��ٻ�ģ����*/
	//##ModelId=49B87BB200EA
	BOOL	m_bMenuAnalog;

	/** @brief           ��ǰ״̬, 0-���ڽ����豸ѡ��. 1-���ڽ����ٻ�*/
	//##ModelId=49B87BB200EB
	int		m_nCuruentStatus;

	/** @brief           �������Զ�����ģ������Ӿ��*/
	//##ModelId=49B87BB2010A
	XJHANDLE	m_pAutoCall;

	/** @brief           ��¼֮ǰ��������� */
	//##ModelId=49B87BB2011A
	int		m_nOldSortCol;

	/** @brief           ��ǰ�ɲ�����վ*/
	CStationObj* m_pActiveStation;

	/** @brief			��ʱ��ID*/
	int m_nTimer;
// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BB20128
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

/*
 *  @brief   	SetCyc	 �����Զ��������� 
 *  @param 		[In]a param of Type  CString  ��һ������ʱ��
 *  @param 		[In]a param of Type  CString  ����
 *  @return 	void 
 */
	//##ModelId=49B87BB20138
	void SetCyc(CString sStartTime, CString sCyc);

private:
	BOOL HasSelected();
	void CheckCallSetting(CBatchSel* pBatchSel);
	void UpdateBatchSetting(int nItem, int nSubItem);
	void UpdateCheckState(CObject* pObj, BOOL bCheck);
	void InsertSingleCPU(WPARAM wParam, LPARAM lParam);
	void InsertSecDevice(WPARAM wParam, LPARAM lParam);
	void InsertCPU(WPARAM wParam, LPARAM lParam);
	void InsertWaverc(WPARAM wParam, LPARAM lParam);
	void InsertStation(WPARAM wParam, LPARAM lParam);
	void InsertDeviceType(WPARAM wParam, LPARAM lParam);
	void InsertPriDevice(WPARAM wParam, LPARAM lParam);
	void InsertNet();

	inline void InsertItem(CString strName, CSel* pBatchSel, int iCPU = -1);
	void AddBatchSel(CBatchSel *pSel);
	void AddBatchSel(CSecObj* pSecObj);
	void AddBatchSel(CStationObj* pStationObj);

	BatchSel* FindNetBatchSel(CString strNetName);
	BatchSel* FindStationBatchSel(CString strStationID, NET_MAP* pNetMap);
    BatchSel* FindStationBatchSel(CString strNetName, CString strStationID);
	BatchSel* FindDeviceTypeBatchSel(int iDeviceType, STATION_MAP* pStationMap);
    BatchSel* FindDeviceTypeBatchSel(CString strNetName, CString strStationID, int iDeviceType);
	BatchSel* FindPriDeviceBatchSel(CString strPriDeviceName, DEVICETYPE_MAP* pDeviceTypeMap);
    BatchSel* FindPriDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName);
	BatchSel* FindSecDeviceBatchSel(CString strSecDeviceName, PRIDEVICE_MAP* pPriDeviceMap);
    BatchSel* FindSecDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName);
	CBatchSel* FindSecCPUBatchSel(int iCPU, BatchSel* pBatchSel);
    CBatchSel* FindSecCPUBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName, int iCPU);

	void InsertNetBatchSel(CBatchSel *pSel);  
	void InsertStationBatchSel(CBatchSel *pSel, NET_MAP* pNetMap);
	void InsertDeviceTypeBatchSel(CBatchSel *pSel, STATION_MAP* pStationMap);
	void InsertPriDeviceBatchSel(CBatchSel *pSel, DEVICETYPE_MAP* pDeviceTypeMap);
	void InsertSecDeviceBatchSel(CBatchSel *pSel, PRIDEVICE_MAP* pPriDeviceMap);
	void InsertSecCPUBatchSel(CBatchSel* pSel, BATCH_LIST* pBatchSel);
    void SetBatchSel(int nSubItem, CSel* pSel, int nValue);
	void SetBatchSels(CSel* pSel, int nSubItem, int nValue);

	void ClearBatchSel();
    void ClearBatchSelMap(CObject* pObj);
	void ClearBatchSelArray(BATCH_LIST* pList);


/*
 *  @brief   	InitListCtrl	 ��ʼ���б�� 
 *  @return 	void 
 */
	//##ModelId=49B87BB201D4
	void InitListCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BB20203
	int	InitListStyle();


/*
 *  @brief   	AddSelToList	 ��ѡ������뵽�б������ʾ 
 *  @param 		[In]a param of Type  CBatchSel*  ָ��ѡ�������
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB20280
	BOOL AddSelToList(CBatchSel* pSel, int nIndex);

/*
 *  @brief   	UpdateControlsEnable	 ���Ŀؼ������� 
 *  @param 		[In]a param of Type  BOOL	TRUE-�ؼ�����, FALSE-�ؼ�������  
 *  @return 	void 
 */
	//##ModelId=49B87BB2035E
	void UpdateControlsEnable(BOOL bEnable);

/*
 *  @brief   	NotifySelectView	 ֪ͨ�豸��ѡ����ͼ�ÿؼ�����/���� 
 *  @param 		[In]a param of Type  BOOL  TRUE-�ؼ�����. FALSE-�ؼ�������
 *  @return 	void 
 */
	//##ModelId=49B87BB2036B
	void NotifySelectView(BOOL bEnable);

/*
 *  @brief   	SaveSelData	 �������ݵ����ݿ�
 *  @param 		[In]a param of Type  int  ���Զ��������û��������ٻ�����.0-�Զ�����. 1-�����ٻ�
 *  @return 	BOOL 
 */
	//##ModelId=49B87BB2037A
	BOOL SaveSelData(int nIs_OC);

/*
 *  @brief   	DeleteSelData	 ����Զ����������е����� 
 *  @param 		[In]a param of Type  int  0-ɾ���Զ����ٵ�����. 1-ɾ�������ٻ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB2038A
	BOOL DeleteSelData(int nIs_OC);

/*
 *  @brief   	NotifyOC	 ֪ͨ�Զ�����ģ����в��� 
 *  @param 		[In]a param of Type  int  0-�ٻ�. 1-ȡ���ٻ�
 *  @return 	BOOL ֪ͨ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB20399
	BOOL NotifyOC(int nAction);

/*
 *  @brief   	NotifyOCCall	 ֪ͨ�Զ�����ģ������ٻ����� 
 *  @return 	BOOL ֪ͨ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB203A9
	BOOL NotifyOCCall();

/*
 *  @brief   	NotifyOCCancelCall	 ֪ͨ�Զ�����ģ�����ȡ���ٻ����� 
 *  @return 	BOOL ֪ͨ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB203B9
	BOOL NotifyOCCancel();

/*
 *  @brief   	MultiSaveData	 �������ݵ����ݿ� 
 *  @return 	BOOL 
 */
	//##ModelId=49B87BB203BA
	BOOL MultiSaveData(int nIs_OC, POSITION pos);


/*
 *  @brief   	ReceiveAutoCall	 �ȴ��Զ����ٵĻ�Ӧ 
 *  @return 	BOOL �յ���Ӧ�ҿ��ٻ�����TRUE, δ�յ���Ӧ�����ڲ����ٻ�����FALSE
 */
	//##ModelId=49B87BB203D9
	BOOL ReceiveAutoCall();

/*
 *  @brief   	AddToComplete	 ��ָ��ѡ������뵽���ٻ�������� 
 *  @param 		[In]a param of Type  CBatchSel*  ָ��ѡ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB30003
	BOOL AddToComplete(CBatchSel* pSel);

/*
 *  @brief   	SaveAutoCallSet	 �����Զ��������õ�ϵͳ���ñ� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[In]a param of Type  CString  ֵ
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB30010
	BOOL SaveAutoCallSet(CString sKey, CString sValue);

/*
 *  @brief   	GetAutoCallConfig	 ��ϵͳ���ñ��в�ѯ�Զ��������� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @param 		[Out]a param of Type  CString  ֵ
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB3001F
	BOOL GetAutoCallConfig(CString sKey, CString& sValue);


/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @return 	void 
 */
	BOOL LoadDataFromDB();


/*
 *  @brief   	SetControls	 ������ʾ��������
 *  @param 		[In]a param of Type  int  ��ͼ����,0��ʾ���Զ����ٵ�������ͼ. 1��ʾ�������ٻ���ͼ
 *  @return 	void
 */
	//##ModelId=49B87BB203C9
	void SetControls(int nViewType);

	void StartBatchCall();

	void EndBatchCall();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchDetailSettingView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB3009C
	virtual ~CBatchDetailSettingView();
#ifdef _DEBUG
	//##ModelId=49B87BB300DA
	virtual void AssertValid() const;
	//##ModelId=49B87BB30119
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBatchDetailSettingView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnBatchSelNull();
	afx_msg void OnBtnBatchSelPt();
	afx_msg void OnBtnBatchSelWr();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnBatchSave();

	afx_msg void OnBtnBatchSet();
	afx_msg void OnClickListBatchDetailSetting(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnBatchSettingReset();
	afx_msg void OnBtnBatchSettingDeviceList();
	afx_msg void OnBtnAutoClose();
	afx_msg void OnBtnCallStop();
	afx_msg void OnBtnBatchStartAllCall();
	afx_msg void OnBtnAutoCallResult();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnBatchStartAllCall();
	afx_msg void OnBtnBatchProcess();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	//##ModelId=49B87BB302E4
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB3031C
	afx_msg void OnBatchCheck(WPARAM wParam, LPARAM lParam);

	//##ModelId=49B87BB3037B
	afx_msg void OnAutoCallSetChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAutoCallStationChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowList(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCheckChange( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBatchCallEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BatchDetailSettingView_H__162EFB99_54E9_45F6_BF19_F0F0F2E1F602__INCLUDED_)
