#if !defined(AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_)
#define AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTOSC.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPTOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B900000
class CPTOSC : public CViewBase
{
protected:
	//##ModelId=49B87B900002
	CPTOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPTOSC)

// Form Data
public:
	//{{AFX_DATA(CPTOSC)
	enum { IDD = IDD_PT_OSC };
	//##ModelId=49B87B900010
	CButton	m_btnRefur;
	//##ModelId=49B87B900015
	CButton	m_btnOpen;
	//##ModelId=49B87B90001A
	CListCtrlEx			m_List;
	//##ModelId=49B87B900022
	CString	m_sDirPath;
	//##ModelId=49B87B900023
	BOOL	m_bOnlyCFG;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B900025
	CSecObj*	m_pObj;	//��Ӧ����
	//##ModelId=49B87B90002A
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           �˳��̱߳�־*/
	//##ModelId=49B87B900031
	BOOL		m_bExitThread;
	/** @brief           ��¼���������̵߳�ָ��*/
	//##ModelId=49B87B900033
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B900041
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B90004E
	int		m_nMaxRow;	//�¼������ʾ����
	//##ModelId=49B87B900050
	CTypedPtrArray<CPtrArray, PT_OSC*> m_arrOSC; //�洢¼���ļ������б�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B90005D
	int		m_nOldSortCol;

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
	int GetThreadCount();
// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B90005E
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B90006F
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B900070
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B90007D
	int FillListData(int nQueryNo);
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B90007E
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B90008C
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B90008D
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B90008E
	int InitControls();

/*
 *  @brief   	GetOSCFileDir	 �õ�¼���ļ����Ŀ¼ 
 *  @return 	CString ¼���ļ����Ŀ¼
 */
	//##ModelId=49B87B90008F
	CString	GetOSCFileDir(BOOL bNewPath);
	
/*
 *  @brief   	SearchOSCFiles	 ̽��Ŀ¼����¼���ļ� 
 *  @param 		[In]a param of Type  CString&  Ŀ¼��,���'\'
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B90009C
	BOOL SearchOSCFiles(CString& path, int nQueryNo);

/*
 *  @brief   	IsValidOSC	 �ж��ļ��Ƿ�¼���ļ� 
 *  @param 		[In]a param of Type  CString  �ļ���׺��
 *  @return 	BOOL ��¼���ļ�����TRUE, ����¼���ļ�����FALSE
 */
	//##ModelId=49B87B90009E
	BOOL IsValidOSC(CString strName);

/*
 *  @brief   	ClearOSCMsg	 ���¼���ļ���Ϣ 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9000AB
	BOOL ClearOSCMsg();

/*
 *  @brief   	OpenWaveFile	 �򿪲����ļ� 
 *  @param 		[In]a param of Type  PT_OSC*  �����ļ��ṹ
 *  @return 	void 
 */
	//##ModelId=49B87B9000AC
	void OpenOSCWaveFile(PT_OSC* pts);

	void StartThread();

	int GetNextQueryNo();
public:
	BOOL IsCurrentQueryNo(int nNo){return nNo == m_nQueryNo;};
	int GetCurrentQueryNo(){return m_nQueryNo;};
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9000AE
	BOOL ReFillAll();

	BOOL ReLoadData(int nQueryNo);

/*
 *  @brief   	EndOwnerThread	 �˳������Լ�����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B9000AF
	void EndOwnerThread();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
//	void RegulateControlSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTOSC)
	public:
	//##ModelId=49B87B9000B0
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B9000BB
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B9000CB
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B9000D0
	virtual ~CPTOSC();
#ifdef _DEBUG
	//##ModelId=49B87B9000DA
	virtual void AssertValid() const;
	//##ModelId=49B87B9000DC
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPTOSC)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckPtoscShow();
	afx_msg void OnBtnPtoscRf();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnPtoscOpen();
	afx_msg void OnDblclkListPtosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtoscCopy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87B90013A
	void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B90013D
	void OnPTFrameClose(WPARAM wParam, LPARAM lParam);
	void OnThreadFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTOSC_H__9E3C6522_A4FC_48FA_B99C_76A11A22D4B8__INCLUDED_)
