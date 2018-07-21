// ***************************************************************
//  DeviceView   version:  1.0    date: 23:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �豸����ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_)
#define AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "..\..\COMMON\xjlib.h"	// Added by ClassView

/**
 * @defgroup 	CDeviceView	�豸����ͼ��
 * @{
 */
/**
 * @brief       ʵ���豸��, ��ʾ�豸��Ϣ 
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
class CDeviceView : public CFormView
{
protected:
	CDeviceView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDeviceView)

// Form Data
public:
	//{{AFX_DATA(CDeviceView)
	enum { IDD = IDD_XJ_DEVICE_VIEW };
	CComboBox	m_comboSelect;
	CTreeCtrl	m_Tree;
	CListCtrl	m_List;
	CString	m_strSelect;
	//}}AFX_DATA

// Attributes
public:
private:
	/** @brief           ��ǰѡ�����ڵ�*/
	//##ModelId=49B87BA403B3
	HTREEITEM htCurrent;
	CImageList m_imageList;
	COleDataSource	m_dataSource;
	CBitmap    m_bitmap;
	int		m_nListType;

	CDeviceObj *m_pSelObj;

	/** @brief           ��ѯ��ʱ��*/
	int m_nQueryTimer;	
	/** @brief           */
	int	m_nTimer;
	int m_nDZ_MOD_State;
	/** @brief           �ٻ��б�*/
	CTypedPtrArray<CPtrArray, StationStauts_Call*> m_arrCall;
	/** @brief           �ٻ����*/
	CString m_sResult;
// Operations
public:
private:
/*
 *  @brief   	InsertDevice	 �����豸���豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CDeviceObj* ָ���豸
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
 *  @brief   	InsertDevice	 ���뱣���豸���豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CDeviceObj* ָ���豸
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertPTDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
*  @brief   	InitDeviceTree	 ��ʼ���豸�� 
*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87BA403B4
	int InitDeviceTree();
	
	/*
	*  @brief   	InitListCtrl	 ��ʼ���б� 
	*  @return 	int		ʧ�ܷ���-1, �ɹ�����>=0
	*/
	//##ModelId=49B87BA403B9
	int InitListCtrl();
	
	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87BA403BA
	void RegulateControlSize();

/*
 *  @brief   	FindNetPos	 ���ҵ��������е�λ�� 
 *  @param 		[In]a param of Type  CString  ����ID
 *  @return 	HTREEITEM ���ڵ�ṹ
 */
	//##ModelId=49B87BA403BB
	HTREEITEM FindNetPos(CString sNetID);

/*
 *  @brief   	InsertNet	 ��������ڵ� 
 *  @param 		[In]a param of Type  CNetObj*  Ҫ����ĵ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403BD
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 ���볧վ�ڵ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403C8
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	InsertStation_Voltage	 ���볧վ�ڵ㣬��վ�½ӵ�ѹ�ȼ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL InsertStation_Voltage(CStationObj* pObj);

/*
 *  @brief   	InsertVoltage	 �����ѹ�ȼ��ڵ� 
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CStationObj* ָ����վ
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL InsertVoltage(int nVoltage, HTREEITEM htParent, CStationObj* pObj);

/*
 *  @brief   	SortVoltage	 ��վ�ڵ�ѹ�ȼ����� 
 *  @param 		[In]a param of Type  CStationObj*  ָ��վ
 *  @return 	void 
 */
	void SortVoltage(CStationObj* pObj);

/*
 *  @brief   	BuildBreak	 ��ָ����վָ����ѹ�ȼ������п��ؼ��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildBreak(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBus	 ��ָ����վָ����ѹ�ȼ�������ĸ�߼��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildBus(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildCap	 ��ָ����վָ����ѹ�ȼ������е��������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildCap(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildGen	 ��ָ����վָ����ѹ�ȼ������з�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildGen(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildLine	 ��ָ����վָ����ѹ�ȼ���������·���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildLine(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildReactor	 ��ָ����վָ����ѹ�ȼ������е翹�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildReactor(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildSwitch	 ��ָ����վָ����ѹ�ȼ������е�բ���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildSwitch(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildTrans	 ��ָ����վָ����ѹ�ȼ������б�ѹ�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *	@param		[In]a param of Type	 int  ָ�����
 *  @return 	void 
 */
	void BuildTrans(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildProtect	 ��ָ����վ�����б������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87BB100A1
	void BuildProtect(CStationObj* pStation);

/*
 *  @brief   	InsertProtect	 ���뱣�����豸�� 
 *  @param 		[In]a param of Type  CSecObj*  ָ������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1009C
	BOOL InsertProtect(CSecObj* pObj);

/*
 *  @brief   	BuildSecToStation	 ��ָ����վ�����ж����豸���뵽�豸���еĶ�Ӧ��վ�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void BuildSecToStation(CStationObj* pStation);

	BOOL InsertSecToStation(CSecObj* pObj);

/*
 *  @brief   	BuildWaveRec	 ��ָ����վ������¼�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1008E
	void BuildWaveRec(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	InsertFastness	 ���Ӳ�ڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @param 		[In]a param of Type  CString  ָ�����ID
 *  @return 	void 
 */
	void InsertFastness(HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBay	 ��ָ����վָ����ѹ�ȼ������м�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  int  ָ����ѹ�ȼ�
 *  @return 	void 
 */
	void BuildBay(CStationObj* pStation, HTREEITEM htParent, int nVoltage);

/*
 *  @brief   	InsertBay	 ���������豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *	@param		[In]a param of Type	 CBayObj*	ָ�����
 *  @return 	void 
 */
	void InsertBay(HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay);

/*
 *  @brief   	ClearEmptyNodeType1	 ����豸������Ϊ1���豸���Ŀսڵ� 
 *  @return 	void 
 */
	void ClearEmptyNodeType1();

/*
 *  @brief   	ClearEmptyNodeType2	 ����豸������Ϊ2���豸���Ŀսڵ� 
 *  @return 	void 
 */
	void ClearEmptyNodeType2();

/*
 *  @brief   	ClearEmptyNodeType4	 ����豸������Ϊ4���豸���Ŀսڵ� 
 *  @return 	void 
 */
	void ClearEmptyNodeType4();

public:
/*
 *  @brief   	FillTree	 ����豸�� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403CA
	BOOL FillTree();

/*
 *  @brief   	FillList	 ����б������ 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA403CB
	BOOL FillList();

	void BuildGPS( CStationObj* pStation, HTREEITEM htParent );
	
	void BuildDistance(CString sNetID,  HTREEITEM htParent );

private:
	BOOL IsChinese(CString str);
	void CompareStation(CString str);

/*
 *  @brief   	LocateInTree	 ���豸���ж�λָ����վ 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87BA403CC

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

public:
	void LocateInTree(CStationObj* pStation);
	void LocateObjInTree(CDeviceObj *pObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceView)
	public:
	//##ModelId=49B87BA403CE
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BA403D8
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BA403DB
	virtual ~CDeviceView();
#ifdef _DEBUG
	//##ModelId=49B87BA403DD
	virtual void AssertValid() const;
	//##ModelId=49B87BA403DF
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDeviceView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTreeOpenmap();
	afx_msg void OnTreeSetpath();
	afx_msg void OnPtSignallist();
	afx_msg void OnQueryEvent();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMENUITEMDevTreePTRunUpdate();
    afx_msg void OnStationConfigChange();
	afx_msg void OnEditchangeComboDevice();
	afx_msg void OnSelendokComboDevice();
	afx_msg void OnTreeStationProp();
	afx_msg void OnTreeStationAdd();
	afx_msg void OnDevTreePTADD();
	afx_msg void OnDevTreePTCONFIGCHANGED();
	afx_msg void OnMarkDevice();
	afx_msg void OnMarkPtdevice();
	afx_msg void OnTreeStationTest();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnPTProp(UINT nID);
	afx_msg void OnWRProp(UINT nID);
	afx_msg void OnStationInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationChanged( WPARAM wParam, LPARAM lParam );
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndCallStationStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_)
