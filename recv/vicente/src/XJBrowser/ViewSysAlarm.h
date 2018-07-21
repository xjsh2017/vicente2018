// ***************************************************************
//  ViewSysAlarm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ϵͳ�澯�¼���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWSYSALARM_H__CA04F73C_892F_4662_93C7_2E3BF7623D70__INCLUDED_)
#define AFX_VIEWSYSALARM_H__CA04F73C_892F_4662_93C7_2E3BF7623D70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSysAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B810119
class CViewSysAlarm : public CFormView
{
protected:
	//##ModelId=49B87B810128
	CViewSysAlarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSysAlarm)

// Form Data
public:
	//{{AFX_DATA(CViewSysAlarm)
	enum { IDD = IDD_XJ_VIEW_SYSALARM };
	//##ModelId=49B87B810139
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B81013E
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B810143
	CXJEventManager		m_EventManager; //�¼�������
	//##ModelId=49B87B810147
	int					m_nMaxRow;	//��ʾ����
	//##ModelId=49B87B810148
	BOOL				m_bShowAcked; //�Ƿ���ʾ��ȷ���¼�
private:
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B810149
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
	//##ModelId=49B87B81014A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B81014F
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B810157
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B810167
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B810177
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B810178
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B810186
	BOOL NeedSave();
	
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B810187
	void RegulateControlSize();

	/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B810196
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventSysAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B810199
	BOOL AddEventToList(CXJEventSysAlarm * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventPTSysAlarm*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8101A6
	void RefreshAck(CXJEventSysAlarm* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventSysAlarm*  ��ȷ�ϵ��¼�
 *  @return 	void 
 */
	//##ModelId=49B87B8101A9
	void TellOtherWndAck( CXJEventSysAlarm* pEvent );

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B8101B6
	void CheckLineCount();

	int RefreshData();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSysAlarm)
	public:
	//##ModelId=49B87B8101B7
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B8101C5
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8101D4
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8101E4
	virtual ~CViewSysAlarm();
#ifdef _DEBUG
	//##ModelId=49B87B8101F4
	virtual void AssertValid() const;
	//##ModelId=49B87B810203
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSysAlarm)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventDevice();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnClickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSysalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListSysalarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	//}}AFX_MSG
	//##ModelId=49B87B81027B
	void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810282
	afx_msg void OnSTTP20155(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20138(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20176(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSYSALARM_H__CA04F73C_892F_4662_93C7_2E3BF7623D70__INCLUDED_)
