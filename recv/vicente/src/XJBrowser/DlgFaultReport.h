#if !defined(AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_)
#define AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaultReport.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultReport dialog

//##ModelId=49B87BA4002E
class CDlgFaultReport : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40030
	CDlgFaultReport(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA4003E
	~CDlgFaultReport();

// Dialog Data
	//{{AFX_DATA(CDlgFaultReport)
	enum { IDD = IDD_DLG_FAULT };
	CCompareCmb	m_cmbStation2;
	CCompareCmb	m_cmbStation1;
	CCompareCmb	m_cmbDevice;
	CEdit	m_FaultReasonEdit;
	CTabCtrl	m_Tab;
	CListCtrl	m_List;
	CString	m_sName;
	CString	m_sDistance;
	CString	m_sMs;
	//}}AFX_DATA
public:
	/** @brief           �����Ӧ�Ĺ��ϱ���ָ��*/
	//##ModelId=49B87BA40051
	CFaultReport* m_pFaultReport;
	/** @brief           ��¼�Ƿ���Ҫ�����¼�*/
	//##ModelId=49B87BA40055
	BOOL		  m_bEventNeedSave;
	/** @brief           ���������ָ��*/
	//##ModelId=49B87BA40057
	CWnd*		  m_pWnd;
	CString       m_sTitle;
	
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
public:
	/** @brief           �����������¼�����*/
	//##ModelId=49B87BA40060
	CPtrArray		m_listChr;
	/** @brief           �����ź����¼�����*/
	//##ModelId=49B87BA40065
	CPtrArray		m_listSign;
	/** @brief           �澯�¼�����*/
	//##ModelId=49B87BA4006A
	CPtrArray		m_listAlarm;
	/** @brief           ����¼������*/
	//##ModelId=49B87BA4006F
	CPtrArray		m_listPTOSC;
	/** @brief           ¼����¼�������*/
	//##ModelId=49B87BA40074
	CPtrArray		m_listWROSC;
	/** @brief			��������λ����*/
	CPtrArray		m_listDIChange;
	/** @brief			С���������*/
	CPtrArray		m_listDistance;
	/** @brief			¼����������*/
	CPtrArray		m_listOSCParse;

	int             m_nCurTabSel;
	CString         m_sFaultReason;
private:
/*
 *  @brief   	InitCMBFaultType	 ��ʼ���������������� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4007E
	BOOL InitCMBFaultType();

/*
 *  @brief   	InitCMBStation	 ��ʼ����վ������ 
 *  @param 		[In]a param of Type  CString  Ԥѡ��ĳ�վID
 *	@param		[In]a param of Type	 UINT	������ID
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4007F
	BOOL InitCMBStation(CString sID, UINT nID);

/*
 *  @brief   	InitCMBDevice	 ��ʼ��һ���豸������ 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4008E
	BOOL InitCMBDevice();

/*
 *  @brief   	InitCMBSaved     ��ʼ���Ƿ�浵������
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4007E
	BOOL InitCMBIsSaved();

/*
 *  @brief   	FillSpecificPriDevice	 ��ָ����վ�µ�ָ�����͵�һ���豸��䵽ָ�������� 
 *  @param 		[In]a param of Type  CComboBox*  ������ָ��
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  һ���豸����
 *  @return 	void 
 */
	//##ModelId=49B87BA4009C
	void FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType);

/*
 *  @brief   	InitListStyle	 ��ʼ���б��� 
 *  @return 	void 
 */
	//##ModelId=49B87BA400AB
	void InitListStyle();

/*
 *  @brief   	ClearDetail	 �����ϸ��Ϣ
 *  @return 	void 
 */
	//##ModelId=49B87BA400AC
	void ClearDetail();

/*
 *  @brief   	ReadDetail	 ������ϸ��Ϣ 
 *  @return 	void 
 */
	//##ModelId=49B87BA400AD
	void ReadDetail();

/*
 *  @brief   	FillListData	 ���ֵ���б�� 
 *  @param 		[In]a param of Type  int  Ҫ����������.0-��������ֵ.1-������Ϣ��.2-�澯.3-¼����
 *  @return 	void 
 */
	//##ModelId=49B87BA400AE
	void FillListData(int nType);

/*
 *  @brief   	AddEventToList	 �����¼����б� 
 *  @param 		[In]a param of Type  CFaultEvent*  �¼�ָ��
 *  @param 		[In]a param of Type  int  Ҫ�����λ������
 *  @return 	void 
 */
	//##ModelId=49B87BA400BC
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	NeedSaveReport	 �жϱ�������Ϣ�������Ƿ���Ҫ���� 
 *  @return 	BOOL ��Ҫ���淵��TRUE, ����Ҫ���淵��FALSE
 */
	//##ModelId=49B87BA400CB
	BOOL NeedSaveReport();

/*
 *  @brief   	RemoveFromList	 ���б�����Ƴ��¼� 
 *  @param 		[In]a param of Type  CFaultEvent*  ָ��Ҫ�Ƴ����¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA400CC
	void RemoveFromList(CFaultEvent* pEvent);

/*
 *  @brief   	RemoveFromArray	 ���¼�������ɾ���¼� 
 *  @param 		[In]a param of Type  CFaultEvent*  ָ��Ҫ�Ƴ����¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA400CE
	void RemoveFromArray(CFaultEvent* pEvent);

/*
 *  @brief   	SaveEvent	 ��������¼� 
 *  @return 	int ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 */
	//##ModelId=49B87BA400DA
	int SaveEvent();

/*
 *  @brief   	RenameOscFile	 ¼���ļ������� 
 *  @return 	CFaultEvent*     ¼�����¼�����
 */
	void RenameOscFile( CFaultEvent* pEvent );

public:
	/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
/*
 *  @brief   	UpdateList	 ˢ���б����ʾ 
 *  @return 	void 
 */
	//##ModelId=49B87BA400DB
	void UpdateList();

	void SelCurTab(int nSel);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaultReport)
	protected:
	//##ModelId=49B87BA400DC
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaultReport)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDlgFaultTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDlgFaultRemove();
	afx_msg void OnDlgFaultAdd();
	afx_msg void OnDlgFaultAddosc();
	afx_msg void OnColumnclickDlgFaultList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFAULTREPORT_H__C8B7FA23_67A4_40C9_A29A_F4AD156FC8BD__INCLUDED_)
