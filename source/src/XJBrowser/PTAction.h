#if !defined(AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_)
#define AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTAction.h : header file
//
#include "ViewBase.h"
#include "Event.h"
#include "EventManager.h"

/////////////////////////////////////////////////////////////////////////////
// CPTAction form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B94038A
class CPTAction : public CViewBase
{
protected:
	//##ModelId=49B87B940399
	CPTAction();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTAction)

// Form Data
public:
	//{{AFX_DATA(CPTAction)
	enum { IDD = IDD_PT_ACTION };
	CButton	m_btnExcel;
	CButton	m_btnSign;
	CButton	m_btnPrint;
	CButton	m_btnDetail;
	CButton	m_btnAckAll;
	CButton	m_btnAck;
	CListCtrlEx	m_List;
	BOOL	m_bQueryAcked;
	BOOL	m_bShowDebug;
	BOOL	m_bShowReturn;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B9403CC
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B9403D9
	CSecObj*	m_pObj;
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B9403DD
	BOOL	m_bExitThread;
	/** @brief           ��ȡ�����߳�ָ��*/
	//##ModelId=49B87B950001
	CWinThread*		m_pLoadThread;
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B950005
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
	//##ModelId=49B87B950012
	CThemeHelperST	m_ThemeHelper;

	//##ModelId=49B87B95001F
	int		m_nMaxRow;	//�¼������ʾ����
	//##ModelId=49B87B950021
	CXJEventManager	m_EventManager; //�¼�����
	//##ModelId=49B87B950031
	BOOL	m_bShowAcked;	//�Ƿ���ʾ��ȷ���¼�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B95003E
	int		m_nOldSortCol;

	/** @brief           ��ӡģ���ʶ*/
	//##ModelId=49B87B950040
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	//##ModelId=49B87B95004F
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	//##ModelId=49B87B95005E
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	//##ModelId=49B87B950063
	IXJReport * pReportDoc;

	/** @brief			��ѯ��,���ֵ1000������1000���0��ʼ����*/
	int			m_nQueryNo;
private:
	CPtrList	m_listThread;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_CriticalOper;
public:
	void RemoveThreadNode(int nNo);
	void AddThreadNode(int nNo, HANDLE hThread);
	THREADNODE_PTR GetFirstNode();
	int	GetThreadCount();

// Operations
public:
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B95006D
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B95007D
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B95008C
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B95009C
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B9500AB
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B9500BB
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B9500BC
	int InitControls();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9500CB
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE, int nQueryNo = 0);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAction *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B9500DB
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ��Ҫ�ĺ�������˵������ 
 *  @param 		[In]a param of Type  CXJEventAction*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B9500EB
	void RefreshAck(CXJEventAction* pEvent, int nIndex = -1);

/*
 *  @brief   	FindLocalPoint	 ����ĳ���¼��ڱ��صĵ�ַ 
 *  @param 		[In]a param of Type  CXJEventAction*  Ҫ���ҵ��¼�
 *  @return 	CXJEventAction* ���ص�ַ
 */
	//##ModelId=49B87B9500FB
	CXJEventAction* FindLocalPoint(CXJEventAction* pEvent);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B870312
	void CheckLineCount();

	void StartThread();

	int GetNextQueryNo();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 ����������������  
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9500FD
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B950109
	void EndOwnerThread();

/*
 *  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
 *  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B95010A
	BOOL InitCOM();

/*
 *  @brief   	ReleaseCOM	 ɾ��COM��� 
 *  @return 	void 
 */
	//##ModelId=49B87B95010B
	void ReleaseCOM();

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B95010C
	CString TranslateSingleKey(CString sKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTAction)
	public:
	//##ModelId=49B87B95010E
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B950128
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B950138
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B95014A
	virtual ~CPTAction();
#ifdef _DEBUG
	//##ModelId=49B87B950157
	virtual void AssertValid() const;
	//##ModelId=49B87B950167
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTAction)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionAck();
	afx_msg void OnColumnclickListPtaction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionDetail();
	afx_msg void OnBtnPtactionSign();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtactionAckall();
	afx_msg void OnBtnPtactionPrint();
	afx_msg void OnCheckPtactionHide();
	afx_msg void OnCheckPtactionDebug();
	afx_msg void OnCheckPtactionReturn();
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtactionExcel();
	//}}AFX_MSG
	//##ModelId=49B87B9501E6
	void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9501F4
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950203
	void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950206
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950215
	void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B950223
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTACTION_H__638667D2_D5A5_4ED9_B881_F55D86E2C4FB__INCLUDED_)
