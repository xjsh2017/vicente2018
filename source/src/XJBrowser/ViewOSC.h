// ***************************************************************
//  ViewOSC   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ¼���¼���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_)
#define AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewOSC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewOSC form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Event.h"
#include "EventManager.h"

//##ModelId=49B87B83006D
class CViewOSC : public CFormView
{
public:
	//##ModelId=49B87B83006F
	CViewOSC();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewOSC)

// Form Data
public:
	//{{AFX_DATA(CViewOSC)
	enum { IDD = IDD_XJ_VIEW_OSC };
	CButton	m_cboxShowFault;
	CStatic	m_staticChooseStation;
	CCompareCmb	m_cmbChooseStation;
	CButton	m_cboxShowReady;
	CButton	m_cboxShowDebug;
	CListCtrlEx	m_List;
	BOOL	m_bCBoxShowDebug;
	BOOL	m_bCBoxShowReady;
	BOOL	m_bCBoxShowFault;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B830083
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	//##ModelId=49B87B830088
	CXJEventManager		m_EventManager;	//�¼�������
	//##ModelId=49B87B83008C
	int					m_nMaxRow;	//�������
	//##ModelId=49B87B83008D
	BOOL				m_bShowAcked; //�Ƿ���ʾ��ȷ���¼�
	/** @brief           ��¼֮ǰ���������*/
	//##ModelId=49B87B83008E
	int		m_nOldSortCol;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL	m_bShowDebug;
	/** @brief           �Ƿ��޹�����Ϣ*/
	BOOL    m_bShowFault;
	/** @brief           ��վID**/
	CString m_sChooseStationID;
public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B83008F
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
private:
	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B83009E
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8300AB
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8300AC
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8300BB
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8300CB
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8300CC
	BOOL NeedSave();

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8300DA
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ��ȡ���� 
 *  @param 		[In]a param of Type  int  Ҫ���ҵĸ���
 *  @param 		[In]a param of Type  int  �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8300DB
	BOOL LoadDataFromDB(int num, BOOL IsQueryAcked = TRUE);

/*
 *  @brief   	AddEventToList	 ��һ���¼�����Ϣ���뵽�б� 
 *  @param 		[In]a param of Type  CXJEventAlarm *  Ҫ������¼�
 *  @param 		[In]a param of Type  int  �����λ��
 *  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	//##ModelId=49B87B8300EC
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	RefreshAck	 ȷ�ϳɹ���ˢ�½�����ʾ 
 *  @param 		[In]a param of Type  CXJEventOSC*  ȷ�ϵ��¼�
 *  @param 		[In]a param of Type  int  �¼�λ��
 *  @return 	void 
 */
	//##ModelId=49B87B8300FA
	void RefreshAck(CXJEventOSC* pEvent, int nIndex = -1);
	
/*
 *  @brief   	TellOtherWndAck	 ����������������ȷ�����¼� 
 *  @param 		[In]a param of Type  CXJEventOSC*  ��ȷ�ϵ��¼�
 *  @return 	void 
 */
	//##ModelId=49B87B8300FD
	void TellOtherWndAck( CXJEventOSC* pEvent );

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B830109
	void CheckLineCount();

/*
 *  @brief   	LoadConfig	 ��ȡ���� 
 *  @return 	void 
 */
	void LoadConfig();

/*
 *  @brief   	SaveConfig	 �������� 
 *  @return 	void 
 */
	void SaveConfig();
// Operations
/*
 *  @brief   	InitComBox	 ��ʼ��combox 
 *  @param 		
 *  @param 		
 *  @return 
 */
	
	void InitComBox();

	int RefreshData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewOSC)
	public:
	//##ModelId=49B87B83010A
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B830119
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B830128
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B830138
	virtual ~CViewOSC();
#ifdef _DEBUG
	//##ModelId=49B87B830148
	virtual void AssertValid() const;
	//##ModelId=49B87B830157
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewOSC)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnClickListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEventAck();
	afx_msg void OnEventAll();
	afx_msg void OnEventCopy();
	afx_msg void OnEventHide();
	afx_msg void OnEventProp();
	afx_msg void OnEventSec();
	afx_msg void OnEventSecProp();
	afx_msg void OnColumnclickListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListOsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEVENTRefresh();
	afx_msg void OnEventQueryevent();
	afx_msg void OnEventFilterDebug();
	afx_msg void OnEventFilterFault();
	afx_msg void OnEventOpenosc();
	afx_msg void OnEventDownosc();
	afx_msg void OnCheckShowdebug();
	afx_msg void OnSelchangeCOMBOChooseStation();
	afx_msg void OnCheckShowready();
	afx_msg void OnCheckShowFault();
	//}}AFX_MSG
	//##ModelId=49B87B8301BC
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8301C6
	afx_msg void OnSTTP20132(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWOSC_H__0D2467E8_3DF8_44AE_8E01_9C46BA38D468__INCLUDED_)
