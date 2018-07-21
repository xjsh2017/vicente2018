// ***************************************************************
//  ViewFault   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����¹ʱ���ҳ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_)
#define AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFault.h : header file
//
#include "FaultReport.h"

/** @brief           �¹ʱ�������*/
typedef CTypedPtrList<CObList , CFaultReport*>	FAULTREPORT_LIST;
/////////////////////////////////////////////////////////////////////////////
// CViewFault form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



//##ModelId=49B87B84033C
class CViewFault : public CFormView
{
protected:
	//##ModelId=49B87B84033E
	CViewFault();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFault)

// Form Data
public:
	//{{AFX_DATA(CViewFault)
	enum { IDD = IDD_XJ_VIEW_FAULT };
	//##ModelId=49B87B84034C
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B84035C
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ
	/** @brief           �¹ʱ�������*/
	//##ModelId=49B87B840361
	FAULTREPORT_LIST		m_listFaultReport;
	/** @brief           �����ʾ��*/
	//##ModelId=49B87B84036B
	int		m_nMaxRow;
	/** @brief           ֮ǰ���������*/
	//##ModelId=49B87B84036C
	int		m_nOldSortCol;

	/** @brief			�Ƿ�ֻ��ʾ�Ǽ��޹��ϰ�*/
	BOOL	m_bOnlyShowNoOverhual;
	/** @brief			�Ƿ�ֻ��ʾ�浵���ϰ�*/
	BOOL	m_bOnlyShowSaved;
	/** @brief			�Ƿ�ֻ��ʾ���͹���*/
	BOOL	m_bOnlyShowTypical;

public:
/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B84037A
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

// Operations
public:
/*
 *  @brief   	IssueToWeb	 ��ָ���Ĺ��ϰ�������WEB 
 *  @param 		[In]a param of Type  CFaultReport*  ָ��Ҫ�����Ĺ��ϰ�
 *  @return 	int	������� 0-�ɹ�. 1-ͨѶ������δ����. 2-���ͱ���ʧ��
 */
	//##ModelId=49B87B850022
	int IssueToWeb(CFaultReport* pReport);
private:

void DeleteFaultReport(CFaultReport* pReport);
BOOL DeleteFaultShortenData(CFaultReport* pReport);
void DeleteFaultShortenData(CList<int, int>&  itemList);
BOOL DeleteFaultShortenEvents(CFaultReport* pReport);
BOOL DeleteFaultAnalyData(CFaultReport* pReport);

	/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B84038A
	int InitListCtrl();

/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B84038B
	int	InitListStyle();
	
	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B840399
	int FillListData();
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8403A9
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8403B9
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8403C8
	BOOL NeedSave();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8403C9
	void RegulateControlSize();

/*
 *  @brief   	LoadDataFromDB	 �����ݿ�����������
 *  @param 		[In]a param of Type  int  �������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8403D8
	BOOL LoadDataFromDB(int nNum);

/*
 *  @brief   	Clear	 ���֮ǰ�Ĺ��ϱ�����Ϣ 
 *  @return 	void 
 */
	//##ModelId=49B87B8403DA
	void Clear();

/*
 *  @brief   	AddReportToList	 �ѹ��ϱ�����뵽�б������ʾ 
 *  @param 		[In]a param of Type  CFaultReport*  Ҫ������ʾ�Ĺ��ϱ������
 *  @param 		[In]a param of Type  int  Ҫ�����λ������
 *  @return 	void 
 */
	//##ModelId=49B87B850000
	void AddReportToList(CFaultReport* pReport, int nIndex);

/*
 *  @brief   	ChangedValueInList	 �ı�ָ�����ϱ������б��е���ʾ 
 *  @param 		[In]a param of Type  CFaultReport*  ָ�����ϱ���
 *  @param 		[In]a param of Type  int  ָ������
 *  @return 	void 
 */
	//##ModelId=49B87B850003
	void ChangedValueInList(CFaultReport* pReport, int nIndex = -1);

/*
 *  @brief   	FindFaultReport	 ����ָ���Ĺ��ϱ����Ƿ������ڴ��� 
 *  @param 		[In]a param of Type  int  ָ��ID
 *  @return 	CFaultReport* ���ҳɹ�����ָ��, ʧ�ܷ���NULL
 */
	//##ModelId=49B87B850010
	CFaultReport* FindFaultReport(int nID);

/*
 *  @brief   	CheckLineCount	 ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 *  @return 	void 
 */
	//##ModelId=49B87B850012
	void CheckLineCount();

/*
 *  @brief   	RemoveFromMem	 ��ָ�����ϱ�����ڴ��������ɾ�� 
 *  @param 		[In]a param of Type  CFaultReport*  ָ�����ϱ���
 *  @return 	void 
 */
	//##ModelId=49B87B850013
	void RemoveFromMem(CFaultReport* pReport);

/*
 *  @brief   	RemoveFromList	 ��ָ�����ϱ�����б����ɾ�� 
 *  @param 		[In]a param of Type  CFaultReport*  ָ�����ϱ���
 *  @return 	void 
 */
	//##ModelId=49B87B850020
	void RemoveFromList(CFaultReport* pReport);

/*
 *  @brief   	QueryLatsetID	 ��ѯ���µĹ��ϰ�ID 
 *  @return 	int ���µĹ��ϰ�ID
 */
	int QueryLatsetID();

/*
 *  @brief   	CreateFaultReport	 �½����ϰ� 
 *  @param 		[In]a param of Type  int  ��ID
 *  @return 	CFaultReport* �´����Ĺ��ϰ�
 */
	CFaultReport* CreateFaultReport(int nID);

/*
 *  @brief   	AddReportToDB	 �����ϰ����뵽���ݿ�,�¼�������
 *  @param 		[In]a param of Type  CFaultReport*  ָ�����ϰ�
 *  @return 	BOOL ����ɹ�����TRUE,ʧ�ܷ���FALSE
 */
	BOOL AddReportToDB(CFaultReport* pReport);

	int RefreshData();

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

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFault)
	public:
	//##ModelId=49B87B850024
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B85002F
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B85004E
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B85005D
	virtual ~CViewFault();
#ifdef _DEBUG
	//##ModelId=49B87B85006D
	virtual void AssertValid() const;
	//##ModelId=49B87B85007D
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewFault)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaultPrint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFaultEdit();
	afx_msg void OnFaultWeb();
	afx_msg void OnFaultRefresh();
	afx_msg void OnFaultCreate();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFaultDelete();
	afx_msg void OnFaultExport();
	afx_msg void OnManualFaultShorten();
	afx_msg void OnFaultOnlyshowoverhaul();
	afx_msg void OnFaultOnlyshowsaved();
	afx_msg void OnFaultOnlyshowtypical();
	//}}AFX_MSG
	//##ModelId=49B87B8500DC
	afx_msg void OnFaultReportEdited(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8500E0
	afx_msg void OnSTTP20151(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

		friend class CManualFaultShorten;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULT_H__B9A0A541_9CD9_488A_AD03_51D1DB272B83__INCLUDED_)
