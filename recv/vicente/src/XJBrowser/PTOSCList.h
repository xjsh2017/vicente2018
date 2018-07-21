#if !defined(AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_)
#define AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTOSCList.h : header file
//
//##ModelId=49B87B8F011A
typedef CTypedPtrList<CObList , CXJEventOSC*>		EventOSC_LIST;
/////////////////////////////////////////////////////////////////////////////
// CPTOSCList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B8F0177
class CPTOSCList : public CViewBase
{
protected:
	//##ModelId=49B87B8F0179
	CPTOSCList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTOSCList)

// Form Data
public:
	//{{AFX_DATA(CPTOSCList)
	enum { IDD = IDD_PT_OSCLIST };
	//##ModelId=49B87B8F0187
	CButton	m_btnOpen;
	//##ModelId=49B87B8F0197
	CListCtrlEx			m_List;
	//##ModelId=49B87B8F019C
	CPictureEx			m_gif;
	//##ModelId=49B87B8F01A1
	CButton	m_btnDown;
	//##ModelId=49B87B8F01A6
	CButton	m_btnCall;
	//}}AFX_DATA

// Attributes
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B8F01AA
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	//##ModelId=49B87B8F01B0
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B8F01B5
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           ��ǰѡ���������Դ����.0-����װ��. 1-��վ���ݿ�*/
	//##ModelId=49B87B8F01B9
	int		m_nSelSrc;
	/** @brief           ��ǰ״̬. 0-����. 1-�����ٻ�¼���б�. 2-��������¼���ļ�*/
	//##ModelId=49B87B8F01BA
	int		m_nCurrentStatus;
	/** @brief           ��Ӧ�����豸*/
	//##ModelId=49B87B8F01BC
	CSecObj*	m_pObj;
	/** @brief           ���涨ʱ��*/
	//##ModelId=49B87B8F01C5
	int		m_nTimer;
	/** @brief           ������ʱ��ʱ������*/
	int			m_nRecordTimer;
	/** @brief           ������ʱ*/
	int			m_nOPTime;

	/** @brief           �����֡����ʱ���յ���֡���*/
	//##ModelId=49B87B8F01CC
	CUIntArray			m_arrFrame;
	/** @brief           ��֡������֡��*/
	//##ModelId=49B87B8F01D4
	int		m_nFrameCount;
	/** @brief           �����ص�¼���б�*/
	//##ModelId=49B87B8F01D6
	EventOSC_LIST		m_EventDownload;
	/** @brief           �ٻ�¼���ļ����. ��ʽΪ: �ٻ�"�ļ���", "���"*/
	//##ModelId=49B87B8F01E4
	CString				m_CallFileResult;

	/** @brief           �Ƿ�ʹ�öϵ�����*/
	//##ModelId=49B87B8F01E5
	BOOL	m_bUseBreakPoint;

	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;

	/** @brief           ���سɹ���*/
	int		m_nSuccess;
	/** @brief           ����ʧ����*/
	int		m_nFaild;
	/** @brief           ����������*/
	int		m_nPart;
	/** @brief           ����20026�����߳�ָ��*/
	CWinThread* m_pParse20026Thread;
	CWinThread*	m_pQueryThread;
public:
	/** @brief           20026��������*/
	CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           �˳������̱߳�־*/
	BOOL m_bExitParse;
	/** @brief           ������ȡ��ɱ�־�����߳��ڴ��������еı��ĺ��Զ��˳�*/
	BOOL m_bDataEnd;
	/** @brief           �������*/
	//##ModelId=49B87B8F01F4
	int m_nNum;
	/** @brief           �¼�������*/
	//##ModelId=49B87B8F01C7
	CXJEventManager		m_EventManager;

	BOOL m_bExitQuery; //�˳���ѯ�̱߳�־

// Operations
public:
/*
 *  @brief   	AddTail20026SttpData	 ��20026���ļ��뵽����β 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  20026��������
 *  @return 	void 
 */
	void AddTail20026SttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	RemoveHead20026SttpData	 ɾ��20026��������ĵ�һ�� 
 *  @return 	void 
 */
	void RemoveHead20026SttpData();

/*
 *  @brief   	Parse20026	 ����20026��������
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  �������ݶ���
 *  @return 	void 
 */
	void Parse20026(STTP_FULL_DATA* pSttpData);

	BOOL LoadDataFromDB();

	void NotifyThreadEnd();

private:
/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B8F0203
	int InitControls();

/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8F0213
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8F0222
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8F0232
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8F0233
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8F0242
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8F0251
	BOOL NeedSave();

/*
 *  @brief   	UpdateControlsEnable	 ���ݵ�ǰ״̬���¿ؼ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8F0261
	void UpdateControlsEnable();

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B8F0271
	void CancelCall();

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventOSC *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8F0272
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	TranslateMultiFrame	 ������֡�Ķ�ֵ���� 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *  @return 	void 
 */
	//##ModelId=49B87B8F0280
	void TranslateMultiFrame(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	DownloadOSCFile	 ����ָ��¼���б��¼���ļ� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ��¼���б�
 *  @return 	void 
 */
	//##ModelId=49B87B8F0290
	void DownloadOSCFile(CXJEventOSC* pEvent);

/*
 *  @brief   	ChangeValue	 �ı�¼���б��Ƿ������ص���ʾ
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ��¼���б�
 *  @return 	void 
 */
	//##ModelId=49B87B8F0292
	void ChangeValue(CXJEventOSC* pEvent);

/*
 *  @brief   	CheckDownload	 ���Ҫ���ص��ļ� 
 *  @return 	void 
 */
	//##ModelId=49B87B8F029F
	void CheckDownload();

/*
 *  @brief   	ChangeDownloadedValue	 �޸��������ص��ļ������������ݴ�С 
 *  @param 		[In]a param of Type  CXJEventOSC*  �������ص��ļ�
 *  @param 		[In]a param of Type  CString  ���������ݴ�С
 *  @return 	void 
 */
	void ChangeDownloadedValue(CXJEventOSC* pEvent, CString sValue);

/*
 *  @brief   	GetOSCFileDir	 �õ�¼���ļ����Ŀ¼ 
 *  @return 	CString ¼���ļ����Ŀ¼
 */
	//##ModelId=49B87B90008F
	CString	GetOSCFileDir(BOOL bNewPath);

/*
 *  @brief   	StartParse20026Thread	 ��������20026�����߳� 
 *  @return 	void 
 */
	void StartParse20026Thread();

/*
 *  @brief   	EndParse20026Thread	 ǿ��ֹͣ20026�������� 
 *  @return 	void 
 */
	void EndParse20026Thread();

	void StartQueryThread();
	void EndQueryThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTOSCList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8F02BF
	virtual ~CPTOSCList();
#ifdef _DEBUG
	//##ModelId=49B87B8F02DE
	virtual void AssertValid() const;
	//##ModelId=49B87B8F02EE
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTOSCList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCmbPtosclistSrc();
	afx_msg void OnBtnPtosclistCall();
	afx_msg void OnBtnPtosclistDown();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnPtosclistOpen();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclickListPtosclist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListPtosclist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtosclistCopy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDoubleclickedBtnPtosclistCall();
	afx_msg void OnDoubleclickedBtnPtosclistDown();
	//}}AFX_MSG
	//##ModelId=49B87B8F035C
	afx_msg void OnSTTP20026(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F036B
	afx_msg void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F036F
	afx_msg void OnCallFileEnd(WPARAM wparam, LPARAM lParam);
	//##ModelId=49B87B8F037D
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F038B
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F0399
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F039D
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8F03AA
	afx_msg void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadWorkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTOSCLIST_H__563F6F73_C9BF_48BB_B064_DB5F2055C194__INCLUDED_)
