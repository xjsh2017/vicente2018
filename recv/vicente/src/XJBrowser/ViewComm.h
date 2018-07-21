// ***************************************************************
//  ViewComm   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ����վͨѶ״̬��ϸ�б���ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_)
#define AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewComm.h : header file
//

class Record : public CObject
{
public:
	CString m_sName;
	int     m_nCommStatus;
	CString sTime;
    CString m_sDurativeTime;
    CString m_sReason;
	CString m_sReasonTime;
	int     m_nCommStatus1;
	CString sTime1;
    CString m_sDurativeTime1;
	CString m_sReason1;
	CString m_sReasonTime1;
	CString m_sManufacturer;
	int     m_nIPFlag;
    Record(CStationObj* pObj);
};

typedef CTypedPtrList< CObList, Record* > RecordList; 
class CommStatusList : public CObject
{
public:
	HTREEITEM hItem;
	CStationObj* pStationObj;
	RecordList recordList;
	CommStatusList();
};

typedef CMapStringToPtr CStationPosMap;

//�ٻ���վͨѶ״̬
typedef struct StationStauts_Call
{
	/** @brief           ��վ*/
	CStationObj* pStation;
	/** @brief           ͨ��*/
	int nChannel;
	/** @brief           0:�ɹ�. -1:���ͱ���ʧ��. -2:�յ�20069. -3:��ʱ*/
	int nRelust;
}StationStauts_Call;

/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87B860033
int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

/////////////////////////////////////////////////////////////////////////////
// CViewComm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EventManager.h"

//##ModelId=49B87B8503C8
class CViewComm : public CFormView
{
protected:
	//##ModelId=49B87B8503CA
	CViewComm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewComm)

// Form Data
public:
	//{{AFX_DATA(CViewComm)
	enum { IDD = IDD_XJ_VIEW_COMM };
	//##ModelId=49B87B860001
	COXTreeCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
    /** @brief           ��վλ��Map*/
    CStationPosMap       m_mapStationPos;
	/** @brief           �б������Ϣ*/
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           �˳��̱߳�־*/
	BOOL	m_bExitThread;

	CMenu * pMenu;
private:
	/** @brief           �¼������ʾ����*/
	int		m_nMaxRow;
	/** @brief           �¼�����*/
	CXJEventManager	m_EventManager;
	/** @brief           ��¼֮ǰ���������*/
	int		m_nOldSortCol;
	/** @brief           ��¼��ʱ��*/
	int		m_nTimer;

	/** @brief           ��ӡģ���ʶ*/
	CLSID clsid;
	/** @brief           COM�๤��ָ��*/
	LPCLASSFACTORY pClf;
	/** @brief           COMָ��*/
	LPUNKNOWN pUnk;
	/** @brief           ��ӡģ��ӿ�ָ��*/
	IXJReport * pReportDoc;

	/** @brief           ��ѯ��ʱ��*/
	int m_nQueryTimer;
	/** @brief           �ٻ��б�*/
	CTypedPtrArray<CPtrArray, StationStauts_Call*> m_arrCall;
	/** @brief           �ٻ����*/
	CString m_sResult;

	CWinThread*	m_pLoadDataThread;
	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;
// Operations


private:
   /*
    *  @brief   	GetStatus ȡ��ĳ����¼��ͨ��״̬
    *  @return 	BOOL 
	*/
	int GetStatus(Record* pRecord, int* nCut);
    int GetStatus(Record* pRecord);
    int GetStatus(CStationObj* pRecord);
    int GetStatus(int nCommStatus, int nCommStatus1, int nIPFlag);
	
	/*
	*  @brief   	InitListCtrl	 ��ʼ���б� 
	*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int	InitListStyle();
	
	
	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	BOOL NeedSave();
	
   /*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	void RegulateControlSize();
	
   /*
	*  @brief   	AddStationToList	 ��һ����վ��״̬��Ϣ���뵽�б� 
	*  @param 		[In]a param of Type  CStationObj *  Ҫ����ĳ�վ
	*  @return 	BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE 
	*/
	BOOL AddStationToList(CStationObj* pStation);

	
   /*
	*  @brief   	InsertStationInfo       ���볧վ�ڵ���Ϣ 
	*  @param 		[In]a param of Type  HTREEITEM ������Ϣ�ڵ�
	*  @return     void 
	*/
    BOOL InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, POSITION pos);
    void InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, int nLastRecordStatus, POSITION pos);
	
   /*
	*  @brief   	SetStationInfo       ���ó�վ�ڵ���Ϣ 
	*  @param 		[In]a param of Type  HTREEITEM ������Ϣ�ڵ�
	*  @return     void 
	*/
	void SetStationInfo(CStationObj* pStation, HTREEITEM hItem, BOOL bChildItem);
	
   /*
	*  @brief   	SetStationInfo       ���ó�վ�ڵ���Ϣ 
	*  @param 		[In]a param of Type  HTREEITEM ������Ϣ�ڵ�
	*  @return     void 
	*/
    void SetStationInfo(Record* record, HTREEITEM hItem, BOOL bChildItem);
	
	/*
	*  @brief   	SetStationInfo       ���ó�վ�ڵ���Ϣ 
	*  @param 		[In]a param of Type  HTREEITEM ������Ϣ�ڵ�
	*  @return     void 
	*/	
	BOOL SetStationInfo(CommStatusList* pList);
	
	/*
	*  @brief   	IsNeedShow	 �жϵ�ǰͨ��״̬�ĳ�վ��Ϣ�Ƿ���Ҫ��ʾ 
	*  @param 		[In]a param of Type  int  ͨ��״̬
	*  @return 	BOOL ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
	*/
	BOOL IsNeedShow(int nStatus);
	
	/*
	*  @brief   	WriteResultToExcel	�����������Excel 
	*  @param 		[In]a param of Type  CString  �ļ�·��
	*  @return 	void 
	*/
	BOOL WriteResultToExcel(CString strFullPath);
	
	/*
	*  @brief   	AddResultToExcel	 ��ָ���Ľ����д�뵽ָ����Excel�ļ��� 
	*  @param 		[In]a param of Type  CString  ���ݿ����
	*  @param 		[In]a param of Type  CStationObj*  �豸����
	*  @return 	void 
	*/
	void AddResultToExcel(CDatabase* pDatabase, CStationObj* pStation);
	
	/*
	*  @brief   	InitCOM	 ��ʼ����ӡģ��COM��� 
	*  @return 	BOOL ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 ɾ��COM��� 
	*  @return 	void 
	*/
	void ReleaseCOM();
	
	/*
	*  @brief   	QueryStationStatus	 ��ѯ��վͨѶ״̬ 
	*  @param 		[In]a param of Type  CStationObj*  ָ����վ
	*  @param 		[In]a param of Type  int  ָ��ͨ��
	*  @return 	int ʧ�ܷ���<0���������ɹ�����0 
	*/
	int QueryStationStatus(CStationObj* pObj, int nChannel = 0);
	
	void TestStationStatus(CStationObj* pStation);
	
	/*
	*  @brief   	ClearCallList	 ����ٻ��б� 
	*  @return 	void 
	*/
	void ClearCallList();
	
	/*
	*  @brief   	AddToCallList	 ��ָ����վָ��ͨ�����뵽���ٻ��б� 
	*  @param 		[In]a param of Type  CStationObj*  ָ����վ
	*  @param 		[In]a param of Type  int  ָ��ͨ��
	*  @return 	void 
	*/
	void AddToCallList(CStationObj* pObj, int nChannel);
	
	/*
	*  @brief   	StartCall	 ���ٻ�������ٻ� 
	*  @param 		[In]a param of Type  StationStauts_Call*  ָ���ٻ���
	*  @return 	void 
	*/
	void StartCall(StationStauts_Call* pCall);
	
	/*
	*  @brief   	ClearStationPosMap	 ���StationPosMap
	*  @param 		
	*  @return 	void 
	*/
	void ClearStationPosMap();
	
    
	/*
	*  @brief   	ReadDetailReason	 ��ȡ�Ͽ���ϸԭ��
	*  @param 		
	*  @return 	void 
	*/
	void ReadStatusData(CStationObj* pStationObj, CommStatusList** pList);

	/*
	*  @brief   	ClearList	 �����¼
	*  @param 		
	*  @return 	void 
	*/
	void ClearRecord(CommStatusList* pList);
    void ClearRecord(CStationObj* pStationObj, BOOL bClearAll);
	void ClearRecord(CommStatusList* pList, BOOL bCon);
	/*
	*  @brief   	AddRecordToList	 ��Ӽ�¼
	*  @param 		
	*  @return 	void 
	*/	
	BOOL AddRecordToList(CStationObj* pStationObj, CommStatusList** pList);
	BOOL AddRecordToList(CommStatusList* pList);
	
	/*
	*  @brief   	SetListData	 �����б�����
	*  @param 		
	*  @return 	void 
	*/
	void SetListData();

	void StartThread();
	void EndThread();
	int FillListData_Multi();
	
public:
	void NotifyThreadEnd();

	/*
	*  @brief   	FillListData	 ����б����� 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	int FillListData();
   /*
    *  @brief   	RefreshDuravieTime	 ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ 
    *  @param 		[In]a param of Type  HTREEITEM  ָ����LIST�е�λ��
    *  @return 	void 
	*/
	void RefreshDuravieTime(HTREEITEM hItem, CStationObj* pStationObj);
	
   /*
	*  @brief   	RefreshTime	 ˢ�����г�վ�ĳ���ʱ�� 
	*  @return 	void 
	*/
	void RefreshTime();
	
	/*
	*  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
	*  @param 		[In]a param of Type  CString  �ؼ���
	*  @return 	CString �ؼ��ֱ�ʾ��ֵ
	*/
	CString TranslateSingleKey(CString sKey);
	void LoadStatusFromDB(CStationObj* pObj, CommStatusList** pList);
	
	void RefreshStatusDetail();
	
	void LoadDetailDataFromDB();

	BOOL LoadAllDataFromDB();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewComm)
public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL IsSelected(const CObject* pDocItem) const;
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewComm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewComm)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnStationHistory();
	afx_msg void OnStationMap();
	afx_msg void OnStationTree();
	afx_msg void OnColumnclickListComm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnClickListComm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCommOn();
	afx_msg void OnCommOff();
	afx_msg void OnCommExport();
	afx_msg void OnCommPrint();
	afx_msg void OnCommandItem(UINT nID);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParma, LPARAM lParam);
	afx_msg void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndCallStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTestStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReFillData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCOMM_H__8DBA9078_DE47_4B80_9790_8B6E5DE39136__INCLUDED_)
