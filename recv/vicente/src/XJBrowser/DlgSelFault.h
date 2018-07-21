#if !defined(AFX_DLGSELFAULT_H__521E90C0_8A0B_4D0F_B98E_A58F7896691B__INCLUDED_)
#define AFX_DLGSELFAULT_H__521E90C0_8A0B_4D0F_B98E_A58F7896691B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelFault.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelFault dialog

class CDlgSelFault : public CDialog
{
// Construction
public:
	CDlgSelFault(CWnd* pParent = NULL);   // standard constructor
	~CDlgSelFault();
// Dialog Data
	//{{AFX_DATA(CDlgSelFault)
	enum { IDD = IDD_DLG_SELFAULT };
	CListCtrlEx	m_List;
	CCompareCmb	m_cmbPriDev;
	CCompareCmb	m_cmbStation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelFault)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CSecObj*	m_pSec;
	CTime		m_startTime;
	CTime		m_endTime;
	CPtrList		m_listFaultReport;
	CFaultReport*	m_pFault;

public:
	void SetSecObj(CSecObj* pObj){ m_pSec = pObj;};
	void SetQueryTime(CTime s, CTime e){m_startTime = s; m_endTime=e;};
	CFaultReport*	GetFault(){return m_pFault;};

private:
	void Clear();
	int InitListCtrl();

	void FillListData();

	void AddReportToList(CFaultReport* pReport, int nIndex);

	/*
 *  @brief   	FillNet     ������������ 
 *  @return 	void 
 */
	void FillNet();

/*
 *  @brief   	FillStation	 ��䳧վ������ 
 *  @param 		[In]a param of Type  CNetObj*  һ���豸������վ,Ϊ�ձ�ʾ������
 *  @return 	void 
 */
	//##ModelId=49B87B83030D
	void FillStation(CNetObj* pNet = NULL);

/*
 *  @brief   	FillDeviceType ����豸���� 
 *  @return 	void 
 */	
	void FillDeviceType();

/*
 *  @brief   	FillPriDevice	 ���һ���豸������ 
 *  @param 		[In]a param of Type  CStationObj*  һ���豸������վ,Ϊ�ձ�ʾ������
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @param      [In]a param of Type  int  �豸����
 *  @return 	void 
 */
	//##ModelId=49B87B83030E
	void FillPriDevice(CStationObj* pStation, int nVoltage, int nDeviceType);
	
/*
 *  @brief   	FillSpecificPriDevice	 ��ָ����վ�µ�ָ�����͵�һ���豸��䵽ָ�������� 
 *  @param 		[In]a param of Type  CComboBox*  ������ָ��
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  һ���豸����
 *  @param      [In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B83031C
	void FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType, int nVoltage);

	/*
 *  @brief   	FillAllPriDevice	 �����������������һ���豸
 *  @param 		[In]a param of Type  CComboBox*  ָ��������
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void                  
 */
	void FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

/*
 *  @brief   	FillPriDeviceByType	 ����һ���豸�����������������һ���豸
 *  @param 		[In]a param of Type  CComboBox*  ָ��������
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void                  
 */
	void FillPriDeviceByType(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

	void AutoselBySec();

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelFault)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbFaultNet();
	afx_msg void OnSelchangeCmbFaultDeviceType();
	afx_msg void OnSelchangeCmbFaultStation();
	afx_msg void OnSelfaultQuery();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELFAULT_H__521E90C0_8A0B_4D0F_B98E_A58F7896691B__INCLUDED_)
