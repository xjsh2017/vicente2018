#if !defined(AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_)
#define AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSignalList.h : header file
//
#include "DeviceObj.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSignalList dialog

//##ModelId=49B87BA30399
class CDlgSignalList : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA3039B
	CDlgSignalList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSignalList)
	enum { IDD = IDD_SIGNAL_LIST };
	//##ModelId=49B87BA3039E
	CTabCtrl	m_Tab;
	//##ModelId=49B87BA303A3
	CListCtrlEx	m_List;
	//}}AFX_DATA
	/** @brief           �����豸ָ��*/
	//##ModelId=49B87BA303AA
	CSecObj*	m_pObj;
private:
	/** @brief           ֮ǰ���������*/
	//##ModelId=49B87BA303AE
	int		m_nOldSortCol;

public:
	/*
 *  @brief   	CompareFunction	 �ȽϺ���,����ʱ������ 
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���1
 *  @param 		[In]a param of Type  LPARAM  �Ƚ϶���2
 *  @param 		[In]a param of Type  LPARAM  �Ƚϲ���
 *  @return 	static int CALLBACK �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 */
	//##ModelId=49B87BA303AF
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

private:
/*
 *  @brief   	InitListStyle	 ��ʼ���б��� 
 *  @return 	void 
 */
	//##ModelId=49B87BA303B9
	void InitListStyle();

/*
 *  @brief   	AddSignalToList	 ���ź������뵽�б�����ʾ 
 *  @param 		[In]a param of Type  PT_Signal*  Ҫ������ź���
 *	@param		[In]a param of Type	 int	Ҫ�����λ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA303BA
	BOOL AddSignalToList(PT_Signal* sign, int nIndex = 0);

/*
 *  @brief   	InitActionSignalData	 ��ʼ�������ź������� 
 *  @return 	void 
 */
	//##ModelId=49B87BA303BD
	void InitActionSignalData();

/*
 *  @brief   	InitAlarmSignalData	 ��ʼ���澯�ź������� 
 *  @return 	void 
 */
	//##ModelId=49B87BA303BE
	void InitAlarmSignalData();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSignalList)
	protected:
	//##ModelId=49B87BA303BF
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSignalList)
	//##ModelId=49B87BA303C9
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA303CB
	afx_msg void OnSelchangeTabSignalList(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA303CF
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA303D8
	afx_msg void OnColumnclickListSignalList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSIGNALLIST_H__B401E4BB_4116_4697_9B1E_DDA46FFB7791__INCLUDED_)
