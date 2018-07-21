#if !defined(AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_)
#define AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownOscView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownOscView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDownOscView : public CViewBase
{
protected:
	CDownOscView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownOscView)

// Form Data
public:
	//{{AFX_DATA(CDownOscView)
	enum { IDD = IDD_DOWNLOAD_OSC };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
private:
	/** @brief           �б������Ϣ*/
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           �¼�������*/
	CXJEventManager		m_EventManager;
	/** @brief           �����ٻ���ʱ��*/
	int		m_nTimer;
	/** @brief           ���涨ʱ���ض�ʱ��*/
	int		m_nDownTimer;
	/** @brief           ��ǰ�������ص�¼���ļ�*/
	CXJEventOSC*	m_pCurrentOsc;

// Operations
public:
private:
/*
 *  @brief   	LoadListConfig	��������Ϣ����	  
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA301A5
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 ��������Ϣ���� 
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA301B5
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
 *  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 */
	//##ModelId=49B87BA301C5
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 ��ʼ���б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87BA301D4
	int	InitListStyle();

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventOSC *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8F0272
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	AddEvent	 ���ָ���¼�����ͼ 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ���¼�
 *  @return 	void 
 */
	void AddEvent(CXJEventOSC* pEvent);

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
 *  @brief   	ChangeDownloadedValue	 �޸��������ص��ļ������������ݴ�С 
 *  @param 		[In]a param of Type  CXJEventOSC*  �������ص��ļ�
 *  @param 		[In]a param of Type  CString  ���������ݴ�С
 *  @return 	void 
 */
	void ChangeDownloadedValue(CXJEventOSC* pEvent, CString sValue);

/*
 *  @brief   	CancelCall	 ȡ���ٻ�����
 *  @return 	void 
 */
	//##ModelId=49B87B8F0271
	void CancelCall();

/*
 *  @brief   	NeedAdd	 �ж�ָ���¼��Ƿ�Ҫ���뵽�������б� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ָ��¼���¼�
 *  @param		[Out]a param of Type CXJEventOSC*  �����Ѵ��ڵĶ�Ӧ��
 *  @return 	BOOL Ҫ���뷵��TRUE, ��Ҫ���뷵��FALSE
 */
	BOOL NeedAdd(CXJEventOSC* pEvent, CXJEventOSC*& pExist);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownOscView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDownOscView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDownOscView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDownoscDown();
	afx_msg void OnDownoscCancel();
	afx_msg void OnDownoscOpen();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnDownOscOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_)
