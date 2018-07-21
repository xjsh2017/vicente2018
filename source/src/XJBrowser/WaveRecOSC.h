#if !defined(AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_)
#define AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRecOSC.h : header file
//

#include "ViewBase.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B800167
class CWaveRecOSC : public CViewBase
{
protected:
	//##ModelId=49B87B800169
	CWaveRecOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CWaveRecOSC)

// Form Data
public:
	//{{AFX_DATA(CWaveRecOSC)
	enum { IDD = IDD_WAVEREC_OSC };
	//##ModelId=49B87B80016B
	CListCtrlEx	m_List;
	//##ModelId=49B87B80017B
	CButton	m_btnRefur;
	//##ModelId=49B87B800180
	CButton	m_btnOpen;
	//##ModelId=49B87B800184
	CString	m_sDirPath;
	//##ModelId=49B87B800185
	BOOL	m_bOnlyCFG;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B800187
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B80018C
	CSecObj*	m_pObj;	//��Ӧ¼����
	/** @brief           �߳��˳���־*/
	//##ModelId=49B87B800190
	BOOL		m_bExitThread;
	/** @brief           ��¼���������߳�ָ��*/
	//##ModelId=49B87B800192
	CWinThread*	m_pLoadThread;
private:
	//##ModelId=49B87B800197
	CThemeHelperST	m_ThemeHelper;
	//##ModelId=49B87B80019B
	int		m_nMaxRow;	//�¼������ʾ����
	//##ModelId=49B87B80019D
	CTypedPtrArray<CPtrArray, PT_OSC*> m_arrOSC; //�洢¼���ļ������б�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B8001A5
	int		m_nOldSortCol;

// Operations
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B8001A6
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8001AB
	int InitListCtrl();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8001B5
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8001B6
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8001C5
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8001C6
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8001C7
	BOOL NeedSave();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�(��List���)
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */	
	//##ModelId=49B87B8001D4
	int InitControls();

/*
 *  @brief   	GetOSCFileDir	 �õ�¼���ļ����Ŀ¼ 
 *  @return 	CString ¼���ļ����Ŀ¼
 */
	//##ModelId=49B87B8001D5
	CString	GetOSCFileDir();
	
/*
 *  @brief   	SearchOSCFiles	 ̽��Ŀ¼����¼���ļ� 
 *  @param 		[In]a param of Type  CString&  Ŀ¼��,���'\'
 *  @return 	BOOL �����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8001D6
	BOOL SearchOSCFiles(CString& path);

/*
 *  @brief   	IsValidOSC	 �ж��ļ��Ƿ�¼���ļ� 
 *  @param 		[In]a param of Type  CString  �ļ���׺��
 *  @return 	BOOL ��¼���ļ�����TRUE, ����¼���ļ�����FALSE
 */
	//##ModelId=49B87B8001D8
	BOOL IsValidOSC(CString strName);

/*
 *  @brief   	ClearOSCMsg	 ���¼���ļ���Ϣ 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8001E5
	BOOL ClearOSCMsg();

	/*
	*  @brief   	EndOwnerThread	 �˳������Լ�����߳�
	*  @return 	void 
	*/
	//##ModelId=49B87B8001E6
	void EndOwnerThread();

/*
 *  @brief   	OpenWaveFile	 �򿪲����ļ� 
 *  @param 		[In]a param of Type  PT_OSC*  �����ļ��ṹ
 *  @return 	void 
 */
	//##ModelId=49B87B9000AC
	void OpenOSCWaveFile(PT_OSC* pts);

public:
/*
 *  @brief   	ReFillAll	 ���¶�ȡ������������� 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B8001E7
	BOOL ReFillAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveRecOSC)
	public:
	//##ModelId=49B87B8001E8
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8001F4
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8001F7
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B800207
	virtual ~CWaveRecOSC();
#ifdef _DEBUG
	//##ModelId=49B87B800213
	virtual void AssertValid() const;
	//##ModelId=49B87B800215
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWaveRecOSC)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckWroscShow();
	afx_msg void OnBtnWroscRf();
	afx_msg void OnColumnclickListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBtnWroscOpen();
	afx_msg void OnDblclkListWrosc(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87B800255
	void OnWRFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B800258
	void OnWRFrameClose(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERECOSC_H__B87FCD05_3E9A_4F63_84BD_9D68391091B5__INCLUDED_)
