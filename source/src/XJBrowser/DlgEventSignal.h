#if !defined(AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_)
#define AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEventSignal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEventSignal dialog

//##ModelId=49B87BA40214
class CDlgEventSignal : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40223
	CDlgEventSignal(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA40225
	~CDlgEventSignal();
// Dialog Data
	//{{AFX_DATA(CDlgEventSignal)
	enum { IDD = IDD_DLG_EVENT_SIGNAL };
	//##ModelId=49B87BA40227
	CListCtrlEx	m_List;
	//}}AFX_DATA
private:
	/** @brief           �¼������ź����б�*/
	//##ModelId=49B87BA40233
	CTypedPtrArray<CPtrArray,PT_Signal*> m_arrSignalsOutput;
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87BA40237
	int		m_nOldSortCol;
public:
	int     m_colHide;
public:
	/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BA40238
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEventSignal)
	protected:
	//##ModelId=49B87BA40246
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
/*
 *  @brief   	AddSignal	 �����ź������ź����б� 
 *  @param 		[In]a param of Type  PT_Signal*  �����ź������ź����б�
 *  @return 	void 
 */
	//##ModelId=49B87BA40249
	void AddSignal(PT_Signal* sign);
private:
/*
 *  @brief   	InitListStyle	 ��ʼ���б� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4024B
	BOOL InitListStyle();

/*
 *  @brief   	InitListData	 ��ʼ���б����� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA40251
	BOOL InitListData();

/*
 *  @brief   	AddSignalToList	 �ѵ����ź������ݼ��뵽�б� 
 *  @param 		[In]a param of Type  PT_Signal*  �ź���
 *	@param		[In]a param of Type	 int	λ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA40252
	BOOL AddSignalToList(PT_Signal* sign, int nIndex = 0);

/*
 *  @brief   	GetSignalDescribe	 �õ��ź������������� 
 *  @param 		[In]a param of Type  PT_Signal*  �ź���
 *  @return 	CString ��������
 */
	//##ModelId=49B87BA40255
	CString GetSignalDescribe(PT_Signal* sign);

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEventSignal)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListEventsign(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnEventsignCopy();
	afx_msg void OnBtnEventsignPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_)
