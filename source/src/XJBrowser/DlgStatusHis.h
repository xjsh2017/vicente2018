#if !defined(AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_)
#define AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStatusHis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis dialog

class CDlgStatusHis : public CDialog
{
// Construction
public:
	CDlgStatusHis(CWnd* pParent = NULL);   // standard constructor
	~CDlgStatusHis();

// Dialog Data
	//{{AFX_DATA(CDlgStatusHis)
	enum { IDD = IDD_PTSTATUS_HISTORY };
	CListCtrlEx	m_List;
	//}}AFX_DATA
	/** @brief           ���͡�1��ʾͨѶ״̬��2��ʾ����״̬*/
	int		m_nType;
	/** @brief           �¼�������*/
	CXJEventManager		m_EventManager;
	/** @brief           ��Ӧ�����豸*/
	CSecObj*	m_pSec;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatusHis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
/*
 *  @brief   	QueryCommHis	 ��ѯ��ʷͨѶ״̬ 
 *  @return 	void 
 */
	void QueryCommHis();

/*
 *  @brief   	QueryRunHis	 ��ѯ��ʷ����״̬ 
 *  @return 	void 
 */
	void QueryRunHis();

/*
 *  @brief   	QueryData	 ��ѯ���� 
 *  @return 	void 
 */
	void QueryData();

/*
 *  @brief   	FillList	 ����б�� 
 *  @return 	void 
 */
	void FillList();

/*
 *  @brief   	AddPTCommToList	 ��ָ���¼���䵽�б�� 
 *  @param 		[In]a param of Type  CXJEventPTComm*  ָ���¼�
 *  @param 		[In]a param of Type  int  ָ�����б���е�λ��
 *  @return 	BOOL ���ɹ�����TRUE,���򷵻�FALSE
 */
	BOOL AddPTCommToList(CXJEventPTComm* pEvent, int i);

/*
 *  @brief   	AddPTRunToList	 ��ָ���¼���䵽�б�� 
 *  @param 		[In]a param of Type  CXJEventPTRun*  ָ���¼�
 *  @param 		[In]a param of Type  int  ָ�����б���е�λ��
 *  @return 	BOOL ���ɹ�����TRUE,���򷵻�FALSE
 */
	BOOL AddPTRunToList(CXJEventPTRun* pEvent, int i);

/*
 *  @brief   	InitListStyle	 ��ʼ���б��� 
 *  @return 	void 
 */
	void InitListStyle();
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStatusHis)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_)
