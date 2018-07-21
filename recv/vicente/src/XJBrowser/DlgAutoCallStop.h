#if !defined(AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_)
#define AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoCallStop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop dialog

//##ModelId=49B87BA402FD
class CDlgAutoCallStop : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA402FF
	CDlgAutoCallStop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoCallStop)
	enum { IDD = IDD_AUTOCALL_STOP };
	//##ModelId=49B87BA4030E
	CListCtrl	m_List;
	//##ModelId=49B87BA40313
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	/** @brief           ������ڵ�*/
	//##ModelId=49B87BA40317
	HTREEITEM	m_hRoot;

	/** @brief           ��վ�ܸ���*/
	//##ModelId=49B87BA4031C
	int m_nStationNum;

	/** @brief           �������Զ�����ģ������Ӿ��*/
	//##ModelId=49B87BA4031E
	XJHANDLE	m_pAutoCall;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoCallStop)
	protected:
	//##ModelId=49B87BA4032C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

/*
 *  @brief   	InitTree	 ��ʼ���豸�� 
 *  @return 	int ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 */
	//##ModelId=49B87BA4032F
	int InitTree();

/*
 *  @brief   	BuildNet	 ���豸���д������� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40330
	void BuildNet();

/*
 *  @brief   	InsertNet	 ��������ڵ� 
 *  @param 		[In]a param of Type  CNetObj*  Ҫ����ĵ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4033C
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 ���볧վ�ڵ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA4033E
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	BuildStation	 ���豸���д�����վ 
 *  @return 	void 
 */
	//##ModelId=49B87BA40340
	void BuildStation();

/*
 *  @brief   	FindNetPos	 ���ҵ��������е�λ�� 
 *  @param 		[In]a param of Type  CString  ����ID
 *  @return 	HTREEITEM ���ڵ�ṹ
 */
	//##ModelId=49B87BA4034B
	HTREEITEM FindNetPos(CString sNetID);

/*
 *  @brief   	CycTreeDown	 ���±���ָ���ڵ�������ӽڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @param 		[In]a param of Type  BOOL  ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
 *  @return 	void 
 */
	//##ModelId=49B87BA4034D
	void CycTreeDown(HTREEITEM hRoot, BOOL bMark);

/*
 *  @brief   	CycTreeUp	 ���ϱ����ڵ�ĸ��ڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BA40350
	void CycTreeUp(HTREEITEM hCurrent);

/*
 *  @brief   	HitItemHelp	 ����ڵ㱻����¼� 
 *  @param 		[In]a param of Type  HTREEITEM  ������Ľڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BA4035C
	void HitItemHelp(HTREEITEM hit);

/*
 *  @brief   	CheckDevice	 ��ĳ�豸������Check����, ����Ǳ�����¼����, ֪ͨ��ϸ��ͼ
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @param 		[In]a param of Type  int  1��ʾѡ��, 0��ʾȡ��ѡ��
 *  @return 	void 
 */
	//##ModelId=49B87BA4035E
	void CheckDevice(CDeviceObj* pObj, int nSelected);

/*
 *  @brief   	ListAddStation	 �ѳ�վ���뵽�б�����ʾ 
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @return 	void 
 */
	//##ModelId=49B87BA4036B
	void ListAddStation(CDeviceObj* pObj);

/*
 *  @brief   	ListRemoveStation	 �ѳ�վ���б����Ƴ� 
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @return 	void 
 */
	//##ModelId=49B87BA4036D
	void ListRemoveStation(CDeviceObj* pObj);

/*
 *  @brief   	ConnectAutoCall	 ���Զ�����ģ�齨������ 
 *  @return 	int ���ӳɹ�����1, ʧ�ܷ���0
 */
	//##ModelId=49B87BA4036F
	int	 ConnectAutoCall();

/*
 *  @brief   	DisConnectAutoCall	 �ر����Զ����ٵ����� 
 *  @return 	void 
 */
	//##ModelId=49B87BA40370
	void DisConnectAutoCall();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoCallStop)
	//##ModelId=49B87BA4037A
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA4037C
	afx_msg void OnClickTreeAutocalStop(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA40380
	afx_msg void OnBtnAutocallStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_)
