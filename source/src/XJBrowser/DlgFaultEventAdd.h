#if !defined(AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_)
#define AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaultEventAdd.h : header file
//
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultEventAdd dialog

//##ModelId=49B87BA40128
class CDlgFaultEventAdd : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA4012A
	CDlgFaultEventAdd(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA40138
	~CDlgFaultEventAdd();

// Dialog Data
	//{{AFX_DATA(CDlgFaultEventAdd)
	enum { IDD = IDD_DLG_FAULTEVENT };
	CCompareCmb	m_cmbPriDev;
	CCompareCmb	m_cmbSec;
	CCompareCmb	m_cmbStation;
	CTabCtrl	m_Tab;
	CListCtrl	m_List;
	//}}AFX_DATA

private:
	/** @brief           �����������¼�����*/
	//##ModelId=49B87BA40144
	CPtrArray		m_listChr;
	/** @brief           �����ź����¼�����*/
	//##ModelId=49B87BA4014B
	CPtrArray		m_listSign;
	/** @brief           �澯�¼�����*/
	//##ModelId=49B87BA40150
	CPtrArray		m_listAlarm;
	/** @brief           ¼������*/
	//##ModelId=49B87BA40158
	CPtrArray		m_listOSC;
	/** @brief			��������λ�¼�*/
	CPtrArray		m_listDIChange;

	/** @brief           ������Ϣ��ID*/
	//##ModelId=49B87BA40167
	int				m_nMainRecID;

	CString m_sPridevID;
	CString m_sStationID;
	int m_nPridevType;
public:
	
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
private:
/*
 *  @brief   	FillStation	 ��䳧վ������ 
 *  @return 	void 
 */
	//##ModelId=49B87BA40168
	void FillStation();

/*
 *  @brief   	FillSec	 ���ָ����վ�µĶ����豸�������� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ,ΪNULLʱ��ʾ���г�վ
 *  @return 	void 
 */
	//##ModelId=49B87BA40169
	void FillSec(CStationObj* pStation);

/*
 *  @brief   	FillSpecificSec	 ���ָ����վ�µĶ����豸��ָ�������� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  CComboBox*  ָ��������
 *  @return 	void 
 */
	//##ModelId=49B87BA40178
	void FillSpecificSec(CStationObj* pStation, CComboBox* pCmb);

/*
 *  @brief   	SetTimeCtrl	 ����ʱ��ؼ� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40186
	void SetTimeCtrl();

/*
 *  @brief   	InitListStyle	 ��ʼ���б��� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40187
	void InitListStyle();

/*
 *  @brief   	ClearEvent	 ɾ�������¼� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40196
	void ClearEvent();

/*
 *  @brief   	QueryAction	 ��ѯ�����¼� 
 *  @param 		[In]a param of Type  CString  �����豸ID, ""�������ж����豸
 *  @param 		[In]a param of Type  CTime  ��ʼʱ��
 *  @param 		[In]a param of Type  CTime  ����ʱ��
 *  @return 	void 
 */
	//##ModelId=49B87BA40197
	void QueryAction(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakAction	 �ֽ⶯���¼�Ϊ�����¼� 
 *  @param 		[In]a param of Type  CXJEventAction* ָ�������¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA401A6
	void BreakAction(CXJEventAction* pEvent);

/*
 *  @brief   	FillListData	 ���ֵ���б�� 
 *  @param 		[In]a param of Type  int  Ҫ����������.0-��������ֵ.1-������Ϣ��.2-�澯.3-¼����
 *  @return 	void 
 */
	//##ModelId=49B87BA401A8
	void FillListData(int nType);

/*
 *  @brief   	AddEventToList	 �����¼����б� 
 *  @param 		[In]a param of Type  CFaultEvent*  �¼�ָ��
 *  @param 		[In]a param of Type  int  Ҫ�����λ������
 *  @return 	void 
 */
	//##ModelId=49B87BA401B5
	void AddEventToList(CFaultEvent* pEvent, int nIndex = 0);

/*
 *  @brief   	QueryAction	 ��ѯ�澯�¼� 
 *  @param 		[In]a param of Type  CString  �����豸ID, ""�������ж����豸
 *  @param 		[In]a param of Type  CTime  ��ʼʱ��
 *  @param 		[In]a param of Type  CTime  ����ʱ��
 *  @return 	void 
 */
	//##ModelId=49B87BA401B8
	void QueryAlarm(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakAlarm	 �ֽ�澯�¼�Ϊ�����¼�-�澯���� 
 *  @param 		[In]a param of Type  CXJEventAlarm*  ָ���澯�¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA401C8
	void BreakAlarm(CXJEventAlarm* pEvent);

/*
 *  @brief   	QueryAction	 ��ѯ¼���¼� 
 *  @param 		[In]a param of Type  CString  �����豸ID, ""�������ж����豸
 *  @param 		[In]a param of Type  CTime  ��ʼʱ��
 *  @param 		[In]a param of Type  CTime  ����ʱ��
 *  @return 	void 
 */
	//##ModelId=49B87BA401CA
	void QueryOSC(CString sSec, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BreakOSC	 �ֽ�¼���¼�Ϊ����¼�� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ��¼���¼�
 *  @return 	void 
 */
	//##ModelId=49B87BA401D7
	void BreakOSC(CXJEventOSC* pEvent);

	void QueryDIChange(CString sSec, CTime tmStart, CTime tmEnd);
	void BreakDIChange(CXJEventDI* pEvent);

/*
 *  @brief   	SaveEvent	 ���汻ѡ����¼������ϰ� 
 *  @return 	void 
 */
	//##ModelId=49B87BA401D9
	void SaveEvent();
public:
	/*
 *  @brief   	GetDevNameList	 ȡ�ø��ݳ�վ���豸���;�����һ���豸 
 *  @param 		[Out]a param of Type  DEVICE_LIST*  ����һ���豸������
 *  @return 	void 
 */
	void GetDevNameList( DEVICE_LIST& listDevice );

/*
 *  @brief   	FillDevType	����豸�����������б�
 *  @return 	void 
 */
	void FillDevType();
	/*
 *  @brief   	FillDevName	���һ���豸�������б�
 *  @return 	void 
 */
	void FillDevName(CStationObj* pStation);
	void FillSecType(CStationObj* pStation);
	/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaultEventAdd)
	protected:
	//##ModelId=49B87BA401E4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaultEventAdd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgFaulteventQuery();
	afx_msg void OnSelchangeDlgFaulteventTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeDlgFaulteventCmbStation();
	afx_msg void OnColumnclickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDlgFaulteventCmbDevice();
	afx_msg void OnSelchangeDlgFaulteventCmbSectype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFAULTEVENTADD_H__CDAC9BEE_85F4_45B3_ADA7_0E29FF3509BF__INCLUDED_)
