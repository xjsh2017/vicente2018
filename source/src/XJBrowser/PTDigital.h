#if !defined(AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_)
#define AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTDigital.h : header file
//
#include "ViewBase.h"
#include "DeviceObj.h"
#include "HisValueMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CPTDigital form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B920177
class CPTDigital : public CViewBase
{
protected:
	//##ModelId=49B87B920179
	CPTDigital();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTDigital)

// Form Data
public:
	//{{AFX_DATA(CPTDigital)
	enum { IDD = IDD_PT_DIGITAL };
	//##ModelId=49B87B920197
	CPictureEx			m_gif;
	//##ModelId=49B87B9201A6
	CButton	m_btnStyle;
	//##ModelId=49B87B9201AB
	CXPStyleButtonST	m_btnPrint;
	//##ModelId=49B87B9201B9
	CButton	m_btnCall;
	//##ModelId=49B87B9201C6
	CListCtrlEx			m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B9201CB
	CSecObj *	m_pObj; //��Ӧ�ı���ָ��
	//##ModelId=49B87B9201D4
	CString		m_sCPU;	//��ѡCPU	
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B9201D5
	BOOL		m_bExitThread;
	/** @brief           ��¼�����߳�ָ��*/
	//##ModelId=49B87B9201E5
	CWinThread*	m_pLoadThread;
	
private:
	//##ModelId=49B87B9201EA
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B9201F5
	CTypedPtrArray<CPtrArray, PT_DIGITAL*> m_arrDigital; //�洢�����������б�
	/** @brief           ��ǰ״̬. 0-����״̬ 1-�����ٻ�������*/
	//##ModelId=49B87B920203
	int		m_nCurrentStatus;
	/** @brief           �Ƿ��һ֡*/
	BOOL	m_bFirstFrame;
	/** @brief           ������ʱ��ʱ������*/
	//##ModelId=49B87B920204
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B920214
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B920219
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B920225
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B920233
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
	//##ModelId=49B87B920238
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
private:
	/** @brief           ����ֵ��*/
	//##ModelId=49B87B920242
	int m_nNewValueIndex;
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B920243
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B920261
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B920271
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B920290
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B92029F
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B9202AF
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B9202BF
	int InitControls();


/*
 *  @brief   	LoadDataFromSTTP	 ��STTP���Ķ��뿪�������� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA *  ��������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B92031C
	BOOL LoadDataFromSTTP(STTP_FULL_DATA * sttpData);
/*
 *  @brief   	ClearDigitalConfig	 ���������������Ϣ 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9202EE
	BOOL ClearDigitalConfig();

/*
 *  @brief   	AddDigitalValue	 ����ĳ�ؿ���ֵ���б� 
 *  @param 		[In]a param of Type  CString  Ҫ����Ŀ�������ID
 *  @param 		[In]a param of Type  CString  Ҫ����Ŀ�������ֵ
 *  @param 		[In]a param of Type  int  ָ��Ҫ���뵽�ڼ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9202EF
	BOOL AddDigitalValue(CString ID, CString VALUE, int iCol);

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B92031E
	void UpdateControlsEnable();

/*
 *  @brief   	RefreshControls	 ˢ�¿ؼ�ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B92032C
	void RefreshControls();

/*
 *  @brief   	SaveSelCPU	 ���浱ǰѡ���CPU 
 *  @param 		[In]a param of Type  CComboBox *  CPU������ָ��
 *  @return 	BOOL ֵ�иı䷵��TRUE,ֵδ�ı䷵��FALSE 
 */
	//##ModelId=49B87B92032D
	BOOL SaveSelCPU( CComboBox * pCmb );

/*
 *  @brief   	ClearDataCol	 ɾ���¼ӵ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B92033C
	void ClearDataCol();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B92033D
	void CancelCall();

/*
 *  @brief   	IsVaild20069	 �ж��յ���20069�Ƿ��Լ���
 *  @param 		[In]a param of Type  int  ʧ�ܵĲ�������ID
 *  @return 	BOOL ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 */
	//##ModelId=49B87B92033E
	BOOL IsVaild20069(int nMsgID);

/*
 *  @brief   	DisplayValue	 �õ���������ֵ��ʾ�ַ��� 
 *  @param 		[In]a param of Type  int  ������ֵ
 *  @return 	CString ��ʾֵ
 */
	//##ModelId=49B87B92034B
	CString DisplayValue(int nValue);

	PT_DIGITAL* FindDigital(CString sID);

	void StartThread();

	int GetNextQueryNo();

	BOOL LoadDefaultCpu();

public:
	/*
 *  @brief   	LoadPTHistoryDigitalData	 ���뿪������ʷֵ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B92030D
	BOOL LoadPTHistoryDigitalData(int nQueryNo);

	/*
 *  @brief   	LoadPTDigitalConfig	 �����ݿ��ȡ������������Ϣ 
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9202DE
	BOOL LoadPTDigitalConfig(int nQueryNo);


	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B92034D
	BOOL ReFillAll();

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B92035B
	void EndOwnerThread();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B92035C
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B92035E
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B92035F
	void ReleaseCOM();

/*
 *  @brief   	DoPrintDigital	 ��ӡ������ 
 *  @param 		[In]a param of Type  CString  ��ӡģ��ȫ·��
 *  @return 	void 
 */
	//##ModelId=49B87B920360
	void DoPrintDigital(CString strPath);

	void AddValueCol( int nQueryNo );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTDigital)
	public:
	//##ModelId=49B87B92036B
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B92037A
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B920399
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B9203AD
	virtual ~CPTDigital();
#ifdef _DEBUG
	//##ModelId=49B87B9203C8
	virtual void AssertValid() const;
	//##ModelId=49B87B930000
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTDigital)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPtdiCall();
	afx_msg void OnSelchangeCmbPtdiCpu();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtdiStyle();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnRoutineprintNew1();
	afx_msg void OnRoutineprintNew2();
	afx_msg void OnRoutineprintNew3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtdiCall();
	afx_msg void OnBtnPtdiExcel();
	//}}AFX_MSG
	//##ModelId=49B87B930090
	void OnSTTP20012(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B93009C
	void OnSTTP20069(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300AB
	void OnSTTP20125(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300BB
	void OnSTTP20157(WPARAM wParam,LPARAM lParam);
	//##ModelId=49B87B9300CB
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9300CE
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9300DC
	afx_msg void OnRefreshPTDI(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTDIGITAL_H__FCEA0AD4_33FA_4497_B20E_E020E66EE97A__INCLUDED_)
