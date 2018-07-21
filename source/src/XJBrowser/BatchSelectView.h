// ***************************************************************
//  BatchSelectView   version:  1.0    date: 2:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: �����ٻ�ѡ����ͼ, �����豸�����ٻ�ѡ������
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_BATCHSELECTVIEW_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_)
#define AFX_BATCHSELECTVIEW_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchSelectView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchSelectView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/**
 * @defgroup 	CBatchSelectView	�����ٻ�ѡ����ͼ��
 * @{
 */
/**
 * @brief       ��ͼ�а���һ���豸��,һ���б�(���������ٻ�ѡ��)
*  @author      LYH
 * @version     ver1.0
 * @date        02/11/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BB0038A
class CBatchSelectView : public CFormView
{
protected:
	//##ModelId=49B87BB0038C
	CBatchSelectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBatchSelectView)

// Form Data
public:
	//{{AFX_DATA(CBatchSelectView)
	enum { IDD = IDD_BATCH_SELECT };
	//##ModelId=49B87BB0039A
	CTreeCtrl	m_Tree;
	//##ModelId=49B87BB003AA
	CListCtrl	m_List;
	//}}AFX_DATA

// Attributes
private:
	/** @brief           ��ֵ�����Ƿ��ٻ�*/
	BOOL	m_bZone;
	/** @brief           ��ֵ�Ƿ��ٻ�*/
	//##ModelId=49B87BB003AE
	BOOL	m_bSetting;
	/** @brief           �������Ƿ��ٻ�*/
	//##ModelId=49B87BB003AF
	BOOL	m_bDigital;
	/** @brief           ��ѹ���Ƿ��ٻ�*/
	//##ModelId=49B87BB003B0
	BOOL	m_bSoftBoard;
	/** @brief           ģ�����Ƿ��ٻ�*/
	//##ModelId=49B87BB003B9
	BOOL	m_bAnalog;
	/** @brief           ������ڵ�*/
	//##ModelId=49B87BB003BA
	HTREEITEM	m_hRoot;
	/** @brief           ҳ������, 0Ϊ�Զ���������. 1Ϊ�����ٻ�*/
	//##ModelId=49B87BB003BB
	int		m_nViewType;
	/** @brief           ��ǰ�ɲ����ĳ�վ��NULL��ʾ����*/
	CStationObj*	m_pActiveStation;

// Operations
public:
private:
/*
 *  @brief   	InitList	 ��ʼ���ٻ�ѡ���б�� 
 *  @return 	int ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 */
	//##ModelId=49B87BB003BC
	int InitList();

	/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87BB003C8
	void RegulateControlSize();

/*
 *  @brief   	InitTree	 ��ʼ���豸�� 
 *  @return 	int ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 */
	//##ModelId=49B87BB003D8
	int InitTree();

/*
 *  @brief   	BuildNet	 ���豸���д������� 
 *  @return 	void 
 */
	//##ModelId=49B87BB10000
	void BuildNet();

/*
 *  @brief   	FindNetPos	 ���ҵ��������е�λ�� 
 *  @param 		[In]a param of Type  CString  ����ID
 *  @return 	HTREEITEM ���ڵ�ṹ
 */
	//##ModelId=49B87BB10001
	HTREEITEM FindNetPos(CString sNetID);

/*
 *  @brief   	InsertNet	 ��������ڵ� 
 *  @param 		[In]a param of Type  CNetObj*  Ҫ����ĵ�������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1000F
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 ���볧վ�ڵ� 
 *  @param 		[In]a param of Type  CStationObj*  Ҫ����ĳ�վ����
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1001F
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	BuildStation	 ���豸���д�����վ 
 *  @return 	void 
 */
	//##ModelId=49B87BB10021
	void BuildStation();

/*
 *  @brief   	InsertDevice	 �����豸���豸�� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *	@param		[In]a param of Type	 CDeviceObj* ָ���豸
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(HTREEITEM htParent, CDeviceObj* pObj);

/*
 *  @brief   	BuildBus	 ��ָ����վ������ĸ�߼��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1003E
	void BuildBus(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildLine	 ��ָ����վ��������·���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB10041
	void BuildLine(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildGen	 ��ָ����վ�����з�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  �������˵��
 *  @param 		[In]a param of Type  HTREEITEM  �������˵��
 *  @return 	void 
 */
	//##ModelId=49B87BB10050
	void BuildGen(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildTrans	 ��ָ����վ�����б�ѹ�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  �������˵��
 *  @param 		[In]a param of Type  HTREEITEM  �������˵��
 *  @return 	void 
 */
	//##ModelId=49B87BB1005E
	void BuildTrans(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildCap	 ��ָ����վ�����е��������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1006D
	void BuildCap(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildReactor	 ��ָ����վ�����е翹�����뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB10070
	void BuildReactor(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildBreak	 ��ָ����վ�����п��ؼ��뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1007E
	void BuildBreak(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildSwitch	 ��ָ����վ�����е�բ���뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB10081
	void BuildSwitch(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	BuildWaveRec	 ��ָ����վ������¼�������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @param 		[In]a param of Type  HTREEITEM  ָ�����ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB1008E
	void BuildWaveRec(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	InsertProtect	 ���뱣�����豸�� 
 *  @param 		[In]a param of Type  CSecObj*  ָ������
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1009C
	BOOL InsertProtect(CSecObj* pObj);

/*
 *  @brief   	InsertCPU	 ����CPU���豸�� 
 *  @param 		[In]a param of Type  CSecCPU*  ָ��CPU
 *	@param		[In]a param of Type	 HTREEITEM ���ڵ�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BB1009E
	BOOL InsertCPU(CSecCPU* pCpu, HTREEITEM htParent);

/*
 *  @brief   	BuildProtect	 ��ָ����վ�����б������뵽�豸�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	//##ModelId=49B87BB100A1
	void BuildProtect(CStationObj* pStation);

/*
 *  @brief   	ClearEmptyNode	 ȥ�����ӽڵ�Ľڵ� 
 *  @return 	void 
 */
	//##ModelId=49B87BB100AC
	void ClearEmptyNode();

/*
 *  @brief   	CheckDevice	 ��ĳ�豸������Check����, ����Ǳ�����¼����, ֪ͨ��ϸ��ͼ
 *  @param 		[In]a param of Type  CDeviceObj*  �豸ָ��
 *  @param 		[In]a param of Type  int  1��ʾѡ��, 0��ʾȡ��ѡ��
 *  @return 	void 
 */
	//##ModelId=49B87BB100AD
	void CheckDevice(CDeviceObj* pObj, int nSelected);

/*
 *  @brief   	CycTreeDown	 ���±���ָ���ڵ�������ӽڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @param 		[In]a param of Type  BOOL  ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
 *  @return 	void 
 */
	//##ModelId=49B87BB100BB
	void CycTreeDown(HTREEITEM hRoot, BOOL bMark);

/*
 *  @brief   	CycTreeUp	 ���ϱ����ڵ�ĸ��ڵ� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB100BE
	void CycTreeUp(HTREEITEM hCurrent);

/*
 *  @brief   	HitItemHelp	 ����ڵ㱻����¼� 
 *  @param 		[In]a param of Type  HTREEITEM  ������Ľڵ�
 *  @return 	void 
 */
	//##ModelId=49B87BB100CB
	void HitItemHelp(HTREEITEM hit);

/*
 *  @brief   	SetCheck_XJ	 ����ָ���ڵ��ѡ��״̬ 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @param 		[In]a param of Type  BOOL  TRUE-ѡ��. FALSE-ȡ��ѡ��
 *  @return 	void 
 */
	//##ModelId=49B87BB100CD
	void SetCheck_XJ(HTREEITEM hit, BOOL bCheck);

/*
 *  @brief   	IsEnableItem	 �ж�ָ���ڵ��Ƿ�ɲ��� 
 *  @param 		[In]a param of Type  HTREEITEM  ָ���ڵ�
 *  @return 	BOOL �ɲ�������TRUE,���ɲ�������FALSE
 */
	BOOL IsEnableItem(HTREEITEM ht);

/*
 *  @brief   	ClearCheck	 �������ѡ�� 
 *  @return 	void 
 */
	void ClearCheck(HTREEITEM ht);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchSelectView)
	public:
	//##ModelId=49B87BB100DA
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BB100DC
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB100EA
	virtual ~CBatchSelectView();
#ifdef _DEBUG
	//##ModelId=49B87BB100FA
	virtual void AssertValid() const;
	//##ModelId=49B87BB10109
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBatchSelectView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickTreeBatchSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListBatchSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAutocallActive();
	afx_msg void OnAutocallAll();
	afx_msg void OnCustomDrawTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87BB10148
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1014C
	afx_msg void OnBatchDetailUncheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB10157
	afx_msg void OnBatchUnselectAll(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1015B
	afx_msg void OnBatchSelectAllPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1015F
	afx_msg void OnBatchSelectAllWR(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB10169
	afx_msg void OnBatchEnableControls(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHSELECTVIEW_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_)
