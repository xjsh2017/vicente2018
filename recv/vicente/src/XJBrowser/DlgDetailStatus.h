#if !defined(AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_)
#define AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDetailStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus dialog

class DlgDetailStatus : public CDialog
{
public:
	CommStatusList* pList;
	POSITION startPos; 
	/** @brief           ��¼֮ǰ���������*/
	int		m_nOldSortCol;
// Construction
public:
	DlgDetailStatus(CWnd* pParent = NULL);   // standard constructor
	DlgDetailStatus(CommStatusList* pCommList, 	POSITION pos, CWnd* pParent = NULL);  

// Dialog Data
	//{{AFX_DATA(DlgDetailStatus)
	enum { IDD = IDD_DLG_DETAIL_STATUS };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDetailStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDetailStatus)
	virtual BOOL OnInitDialog();
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListDetailStatus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

   /*
    *  @brief   	InitListCtrl	 ��ʼ���б� 
    *  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	void InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int	InitListStyle();

   /*
    *  @brief   	LoadListConfig	��������Ϣ����	  
    *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
    */
	BOOL LoadListConfig();

   /*
    *  @brief   	SaveListConfig	 ��������Ϣ���� 
    *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
    */
	BOOL SaveListConfig();

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	void RegulateControlSize();

	/*
	*  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
	*  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 ɾ��COM��� 
	*  @return 	void 
	*/
	void ReleaseCOM();

/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	BOOL GetStatus(Record* pRecord);
	
	void SetRecordInfo(Record* pRecord, int nItem);
	
	void FillDataList();

	BOOL NeedSave();

private:
	/** @brief           �б������Ϣ*/
	CArray<SColumn, SColumn&> m_arrColum;

	/** @brief           ��ӡģ���ʶ*/
	CLSID clsid;

	/** @brief           COM�๤��ָ��*/
	LPCLASSFACTORY pClf;

	/** @brief           COMָ��*/
	LPUNKNOWN pUnk;

	/** @brief           ��ӡģ��ӿ�ָ��*/
	IXJReport * pReportDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_)
