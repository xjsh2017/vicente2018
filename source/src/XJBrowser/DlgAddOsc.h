#if !defined(AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_)
#define AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddOsc.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc dialog

class CDlgAddOsc : public CDialog
{
// Construction
public:
	CDlgAddOsc(CWnd* pParent = NULL);   // standard constructor
	~CDlgAddOsc();

	/** @brief           ��Ӧ�Ĺ��Ϲ鵵*/
	CFaultReport*	m_pReport;
	/** @brief           ¼���ļ��б�*/
	CPtrArray		m_listOSC;
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
// Dialog Data
	//{{AFX_DATA(CDlgAddOsc)
	enum { IDD = IDD_DLG_ADDOSC };
	CCompareCmb	m_cmbSec;
	CCompareCmb	m_cmbStation;
	CListCtrl	m_List;
	CString	m_sPath;
	int		m_nMS;
	CString m_sDir;
	CString m_sOldDir;
	int     m_nFileSize;
	//}}AFX_DATA

private:
/*
 *  @brief   	FillCMBStation	 ��䳧վ���������� 
 *  @return 	void 
 */
	void FillCMBStation();

/*
 *  @brief   	FillCMBSec	 ���ָ����վ�Ķ����豸���ݵ�������
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void FillCMBSec(CStationObj* pStation);

/*
 *  @brief   	InitList	 ��ʼ���б� 
 *  @return 	void 
 */
	void InitList();

/*
 *  @brief   	AddEventToList	 ���ָ���¼����б� 
 *  @param 		[In]a param of Type  CFaultEvent*  ָ���¼�
 *  @param 		[In]a param of Type  int  ��ӵ��б��λ��
 *  @return 	BOOL ��ӳɹ�����TRUE,ʧ�ܷ���FALSE
 */
	BOOL AddEventToList(CFaultEvent* pEvent, int nIndex);

/*
 *  @brief   	IsExist	 �ж�ָ���ļ��Ƿ����ڴ������б����Ϲ鵵��¼���б��� 
 *  @param 		[In]a param of Type  CString  ָ���ļ������ƣ�������׺
 *  @return 	BOOL �Ѵ��ڷ���TRUE,�����ڷ���FALSE
 */
	BOOL IsExist(CString sName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddOsc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
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
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddOsc)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddoscSkin();
	afx_msg void OnBtnAddoscAdd();
	afx_msg void OnBtnAddoscRemove();
	afx_msg void OnBtnAddoscModify();
	afx_msg void OnItemchangedListAddosc(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeCmbAddoscStation();
	afx_msg void OnColumnclickListAddosc(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_)
