#if !defined(AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_)
#define AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAnalog.h : header file
//
#include "ViewBase.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B930157
class CPTAnalog : public CViewBase
{
protected:
	//##ModelId=49B87B930159
	CPTAnalog();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAnalog)

// Form Data
public:
	//{{AFX_DATA(CPTAnalog)
	enum { IDD = IDD_PT_ANALOG };
	CButton	m_btnExcel;
	CPictureEx			m_gif;
	CButton	m_btnStyle;
	CXPStyleButtonST	m_btnPrint;
	CButton	m_btnCall;
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B930191
	CSecObj *	m_pObj; //��Ӧ�ı���ָ��
	//##ModelId=49B87B930196
	CString		m_sCPU;	//��ѡCPU
	/** @brief           ������ʱ��ʱ��*/
	//##ModelId=49B87B930197
	int			m_nTimer;
	/** @brief           �߳��˳���־*/
	//##ModelId=49B87B9301A5
	BOOL		m_bExitThread;
	/** @brief           ��¼�����߳�ָ��*/
	//##ModelId=49B87B9301A7
	CWinThread*	m_pLoadThread;

private:
	//##ModelId=49B87B9301B6
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B9301BB
	CTypedPtrArray<CPtrArray, PT_ANALOG*> m_arrAnalog; //�洢�����������б�
	/** @brief           ����ֵ��*/
	//##ModelId=49B87B9301C8
	int m_nNewValueIndex;
	/** @brief			ʵʱֵ��*/
	int m_nRealtimeValueIndex;
	UINT m_nRealtimeValueTimer; //ʵʱֵˢ�¶�ʱ��


	/** @brief           ��ǰ״̬. 0-����״̬ 1-�����ٻ�ģ����*/
	//##ModelId=49B87B9301D4
	int	m_nCurrentStatus;

	/** @brief           �Ƿ��һ֡*/
	BOOL m_bFirstFrame;

	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B9301D6
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B9301E5
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B9301EA
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B9301F7
	IXJReport * pReportDoc;

	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;

	CHisValueMgr	m_HisValueMgr;

private:
	CPtrList	m_listThread;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();
	int GetThreadCount();
// Operations
public:
	//##ModelId=49B87B930204
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B930208
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B930222
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B930232
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B930242
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B930251
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B930261
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B930271
	int InitControls();

/*
 *  @brief   	ClearAnalogConfig	 ���ģ����������Ϣ 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B930290
	BOOL ClearAnalogConfig();

/*
 *  @brief   	AddAnalogValue	 ����ĳģ����ֵ���б� 
 *  @param 		[In]a param of Type  CString  Ҫ�����ģ������ID
 *  @param 		[In]a param of Type  CString  Ҫ�����ģ������ֵ
 *  @param 		[In]a param of Type  int  ָ��Ҫ���뵽�ڼ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B93029F
	BOOL AddAnalogValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	LoadPTHistoryAnalogData	 ����ģ������ʷֵ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9302AF
	BOOL LoadPTHistoryAnalogData();

/*
 *  @brief   	LoadDataFromSTTP	 ��STTP�����ж�ȡģ����ֵ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9302B0
	BOOL LoadDataFromSTTP(STTP_FULL_DATA* sttpData);

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ�������
 *  @return 	void 
 */
	//##ModelId=49B87B9302C0
	void UpdateControlsEnable();

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B9302C1
	void RefreshControls();

/*
 *  @brief   	SaveSelCPU	 ���浱ǰѡ���CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU������ָ��
 *  @return 	BOOL ֵ�иı䷵��TRUE,ֵδ�ı䷵��FALSE 
 */
	//##ModelId=49B87B9302CE
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 ɾ���¼ӵ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B9302D0
	void ClearDataCol();


/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B9302DE
	void CancelCall();

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B9302DF
	void EndOwnerThread();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B9302E0
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	IsVaild20125	 �ж��Ƿ��Լ����ܵ�20125���� 
 *  @param 		[In]a param of Type  int  ��������ID
 *  @return 	BOOL ���Լ����ܵı��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B9302EE
	BOOL IsVaild20125(int nMsgID);

	PT_ANALOG* FindAnalog(CString sID);

	void StartThread();

	int GetNextQueryNo();

	/*
	* @brief	AddValueCol	Ϊ�б����һ��������ʾֵ
	* @param	[In]a param of Type int nQueryNo		
	* @return   void	
	*/
	void AddValueCol(int nQueryNo);

	BOOL LoadDefaultCpu();

	void startRealtimeValueTimer();
	void stopRealtimeValueTimer();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};

	/*
 *  @brief   	LoadPTAnalogConfig	 �����ݿ��ȡģ����������Ϣ 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B930280
	BOOL LoadPTAnalogConfig(int nQueryNo);
/*
 *  @brief   	ReFillAll	 ���¶����������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9302F0
	BOOL ReFillAll();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B9302F1
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9302FD
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B9302FE
	void ReleaseCOM();
/*
 *  @brief   	ReleaseCOM	 ���������ʵ��ֵ
 *  @return 	void 
 */
	//##ModelId=49B87B9302FE
	CString SetFloatValue(PT_ANALOG* pst, CString strValue);
/*
 *  @brief   	DoPrintAnalog	 ��ӡģ���� 
 *  @param 		[In]a param of Type  CString  ��ӡģ��ȫ·��
 *  @return 	void 
 */
	//##ModelId=49B87B9302FF
	void DoPrintAnalog(CString strPath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAnalog)
	public:
	//##ModelId=49B87B930301
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B93031C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B93033C
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B930341
	virtual ~CPTAnalog();
#ifdef _DEBUG
	//##ModelId=49B87B93035B
	virtual void AssertValid() const;
	//##ModelId=49B87B93036B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAnalog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtanalogCall();
	afx_msg void OnSelchangeCmbPtanalogCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtanalogStyle();
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtanalogCall();
	afx_msg void OnBtnPtanalogExcel();
	//}}AFX_MSG
	//##ModelId=49B87B940006
	void OnSTTP20008(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B940010
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B94001F
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B94002E
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B940031
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B940040
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTANALOG_H__56BEF203_B2F4_413B_A51F_4C0A90BB9C95__INCLUDED_)
