// ***************************************************************
//  ViewFaultQuery   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �¹ʱ����ѯҳ��,������ѯ��������ʾ��ѯ������б�
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_)
#define AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFaultQuery.h : header file
//
#include "ViewBase.h"
#include "FaultReport.h"

/** @brief           �¹ʱ�������*/
//##ModelId=49B87B8301F1
typedef CTypedPtrList<CObList , CFaultReport*>	FAULTREPORT_LIST;
/////////////////////////////////////////////////////////////////////////////
// CViewFaultQuery form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B830242
class CViewFaultQuery : public CViewBase
{
protected:
	//##ModelId=49B87B830244
	CViewFaultQuery();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFaultQuery)

// Form Data
public:
	//{{AFX_DATA(CViewFaultQuery)
	enum { IDD = IDD_XJ_VIEW_FAULT_QUERY };
	CCompareCmb	m_cmbPriDev;
	CCompareCmb	m_cmbSecDev;
	CCompareCmb	m_cmbStation;
	CButton	m_btnContinue;
	CButton	m_btnQuery;
	CListCtrlEx	m_List;
	CString	m_sQueryCondition;
	CString	m_sQueryResult;
	//}}AFX_DATA

// Attributes
public:
	MAP_VOLTAGE m_VoltageMap;

private:
	//##ModelId=49B87B830272
	CThemeHelperST m_ThemeHelper;
	//##ModelId=49B87B830277
	CArray<SColumn, SColumn&> m_arrColum;	//�б������Ϣ

	/** @brief           �¹ʱ�������*/
	//##ModelId=49B87B83027C
	FAULTREPORT_LIST		m_listFaultReport;
    /** @brief           ѡ��ĵ���*/
	CString m_sSelNet;
	/** @brief           ѡ��ĳ�վID*/
	//##ModelId=49B87B830280
	CString m_sSelStationID;
	/** @brief           ��ѹ�ȼ�*/
	int m_nSelVoltage;
	/** @brief           �豸����*/
	int m_nSelDeviceType;
	/** @brief           ѡ���һ���豸ID*/
	//##ModelId=49B87B830281
	CString m_sSelPriID;
	/** @brief           ѡ��Ķ����豸*/
	CString m_sSelSecDevice;
	/** @brief           ѡ��Ĺ������*/
	//##ModelId=49B87B830282
	int		m_nSelFaultType;
	/** @brief           ѡ��Ĺ�������*/
	int		m_nChooseFaultType;
	/** @brief           ѡ�����ʼʱ��*/
	//##ModelId=49B87B830290
	CTime	m_tmSelStart;
	/** @brief           ѡ��Ľ���ʱ��*/
	//##ModelId=49B87B830291
	CTime	m_tmSelEnd;
	/** @brief           �Ƿ�浵*/
	int m_nIsSaved;
	/** @brief           �Ƿ��ǵ��͹���*/
	int m_nIsTypicalcase;
	int m_nOverhaul;

	/** @brief           ֮ǰ���������*/
	//##ModelId=49B87B830292
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
	//##ModelId=49B87B830293
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

/*
 *  @brief   	IssueToWeb	 ��ָ���Ĺ��ϰ�������WEB 
 *  @param 		[In]a param of Type  CFaultReport*  ָ��Ҫ�����Ĺ��ϰ�
 *  @return 	int	������� 0-�ɹ�. 1-ͨѶ������δ����. 2-���ͱ���ʧ��
 */
	//##ModelId=49B87B850022
	int IssueToWeb(CFaultReport* pReport);
private:
		/*
*  @brief   	InitListCtrl	 ��ʼ���б� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8302A2
	int InitListCtrl();

/*
 *  @brief   	InitControls	 ��ʼ�������ؼ�
 *  @return 	int  ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B8302AF
	int InitControls();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8302BF
	void RegulateControlSize();

/*
 *  @brief   	ShowResult	 ��ʾ��ѯ��� 
 *  @param 		[In]a param of Type  BOOL  �Ƿ���ʾ��ѯ���
 *  @return 	void 
 */
	//##ModelId=49B87B8302CE
	void ShowResult(BOOL bShow);

	/*
	*  @brief   	LoadListConfig	��������Ϣ����	  
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8302DE
	BOOL LoadListConfig();
	
	/*
	*  @brief   	SaveListConfig	 ��������Ϣ���� 
	*  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
	*/
	//##ModelId=49B87B8302EE
	BOOL SaveListConfig();
	
	/*
	*  @brief   	NeedSave	 �ж��Ƿ���Ҫ�����б������Ϣ
	*  @return 	BOOL ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
	*/
	//##ModelId=49B87B8302EF
	BOOL NeedSave();

	/*
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87B8302FD
	int	InitListStyle();

/*
 *  @brief   	FillNet     ������������ 
 *  @return 	void 
 */
	void FillNet();

/*
 *  @brief   	FillStation	 ��䳧վ������ 
 *  @param 		[In]a param of Type  CNetObj*  һ���豸������վ,Ϊ�ձ�ʾ������
 *  @return 	void 
 */
	//##ModelId=49B87B83030D
	void FillStation(CNetObj* pNet = NULL);

/*
 *  @brief   	FillVoltage ����ѹ�ȼ�
 *  @param      [In]a param of Type  CStationObj*  ��վָ��
 *  @return 	void 
 */
	void FillVoltage(CStationObj* pStationObj);

/*
 *  @brief   	FillDeviceType ����豸���� 
 *  @return 	void 
 */	
	void FillDeviceType();

/*
 *  @brief   	FillPriDevice	 ���һ���豸������ 
 *  @param 		[In]a param of Type  CStationObj*  һ���豸������վ,Ϊ�ձ�ʾ������
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @param      [In]a param of Type  int  �豸����
 *  @return 	void 
 */
	//##ModelId=49B87B83030E
	void FillPriDevice(CStationObj* pStation, int nVoltage, int nDeviceType);
	
/*
 *  @brief   	FillSecDevice	 �������豸������ 
 *  @param 	    [In]a param of Type  CStationObj*  һ���豸������վ,Ϊ�ձ�ʾ������
 *  @param      [In]a param of Type  CDeviceObj*   һ���豸ָ��
 *  @param      [In]a param of Type  int           ��ѹ�ȼ�
 *  @param      [In]a param of Type  int           �豸����
 *  @return 	void 
 */	
	void FillSecDevice(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType);

/*
 *  @brief   	FillSpecificPriDevice	 ��ָ����վ�µ�ָ�����͵�һ���豸��䵽ָ�������� 
 *  @param 		[In]a param of Type  CComboBox*  ������ָ��
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  һ���豸����
 *  @param      [In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B83031C
	void FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType, int nVoltage);

/*
 *  @brief   	FillSecDeviceMap	 ��ָ����վ�µĶ����豸��䵽ָ��������
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  CDeviceObj*  һ���豸ָ��
 *  @param 		[In]a param of Type  int          ��ѹ�ȼ�
 *  @param      [In]a param of Type  int          �豸����
 *  @return 	void 
 */
	void FillSecDeviceMap(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType);	

/*
 *  @brief   	FillFaultType	 ��������������� 
 *  @return 	void 
 */
	//##ModelId=49B87B83032E
	void FillFaultType();
	
	/*
 *  @brief   	FillFaultType	 ���������������� 
 *  @return 	void 
 */
	//##ModelId=49B87B83032E
	void FillFaultType2();
/*
 *  @brief   	FillStartTime	 �����ʼʱ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B83033C
	void FillStartTime();

/*
 *  @brief   	GetAllCurSel	 ȡ�����пؼ��ĵ�ǰѡ��ֵ 
 *  @return 	void 
 */
	//##ModelId=49B87B83033D
	void GetAllCurSel();


/*
 *  @brief   	QuerySpecificPriDevice	 ��ѯһ���豸���Ϲ鵵��
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE 
 */
	BOOL QuerySpecificPriDevice();

/*
 *  @brief   	QueryFaultReport	 ��ѯ���ϱ��� 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B83035B
	BOOL QueryFaultReport();

/*
 *  @brief   	Clear	 ���ԭ�������� 
 *  @return 	void 
 */
	//##ModelId=49B87B83036B
	void Clear();

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	void 
 */
	//##ModelId=49B87B83037A
	void FillListData();

/*
 *  @brief   	AddReportToList	 �ѹ��ϱ�����뵽�б������ʾ 
 *  @param 		[In]a param of Type  CFaultReport*  Ҫ������ʾ�Ĺ��ϱ������
 *  @param 		[In]a param of Type  int  Ҫ�����λ������
 *  @return 	void 
 */
	//##ModelId=49B87B83037B
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
 *  @brief   	InitVoltageMap	 ��ʼ����ѹ�ȼ�Map
 *  @return 	void 
 */	
	void InitVoltageMap();
	void InitVoltageMap(CString strNetID, MAP_VOLTAGE* VoltageMap);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFaultQuery)
	public:
	//##ModelId=49B87B83038B
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B830399
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B8303B9
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8303D8
	virtual ~CViewFaultQuery();
#ifdef _DEBUG
	//##ModelId=49B87B84000F
	virtual void AssertValid() const;
	//##ModelId=49B87B84001F
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewFaultQuery)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnBtnFaultQuery();
	afx_msg void OnBtnFaultContinue();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbFaultStation();
	afx_msg void OnItemchangedListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaultPrint();
	afx_msg void OnFaultEdit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFaultWeb();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmbFaultNet();
	afx_msg void OnSelchangeCmbFaultOne();
	afx_msg void OnSelchangeCmbFaultDeviceType();
	afx_msg void OnSelchangeCmbFaultVoltage();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFaultExport();
	afx_msg void OnManualFaultShorten();
	afx_msg void OnFaultDelete();
	afx_msg void OnSelchangeCmbFaultType();
	afx_msg void OnSelchangeCmbFaultType2();
	//}}AFX_MSG
	afx_msg void OnFaultReportEdited(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
/*
 *  @brief   	GetVoltageCmbValue	 ȡ�õ�ѹ�ȼ���������ѡֵ
 *  @return 	int                  ��ѹ�ȼ�ֵ 
 */
	inline int GetVoltageCmbValue();

/*
 *  @brief   	FillAllPriDevice	 �����������������һ���豸
 *  @param 		[In]a param of Type  CComboBox*  ָ��������
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void                  
 */
	inline void FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

/*
 *  @brief   	FillPriDeviceByType	 ����һ���豸�����������������һ���豸
 *  @param 		[In]a param of Type  CComboBox*  ָ��������
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  int  �豸����
 *  @param 		[In]a param of Type  int  ��ѹ�ȼ�
 *  @return 	void                  
 */
	inline void FillPriDeviceByType(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage);

/*
 *  @brief   	IfHasSecDevice	 ��ѯһ���豸���Ƿ��ж����豸
 *  @param 		[In]a param of Type  CStationObj*  ��վָ��
 *  @param 		[In]a param of Type  CDeviceObj*   һ���豸ָ��
 *  @return 	void                  
 */
    inline BOOL IfHasSecDevice(CStationObj* pStation, CDeviceObj* pDeviceObj);

void DeleteFaultShortenData(CList<int, int>&  itemList);
void DeleteFaultReport(CFaultReport* pReport);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULTQUERY_H__5DCA1871_7D75_48E9_8722_03C5EA74BA79__INCLUDED_)
