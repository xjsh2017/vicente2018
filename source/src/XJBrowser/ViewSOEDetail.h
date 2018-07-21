#if !defined(AFX_VIEWSOEDETAIL_H__D21513C2_AFA3_416B_A0CE_C020A6EBCBFB__INCLUDED_)
#define AFX_VIEWSOEDETAIL_H__D21513C2_AFA3_416B_A0CE_C020A6EBCBFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSOEDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSOEDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewSOEDetail : public CFormView
{
protected:
	CViewSOEDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSOEDetail)

// Form Data
public:
	//{{AFX_DATA(CViewSOEDetail)
	enum { IDD = IDD_XJ_VIEW_SOE_DETAIL };
	CListCtrlEx	m_ListSign;
	CListCtrlEx	m_ListChr;
	CString	m_sTime;
	CString	m_sDevice;
	CString	m_sDetail;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           ҳ������Ӧ�¼�*/
	CXJEvent*	m_pEvent;

// Operations
public:
		/*
	*  @brief   	RegulateControlSize	 �����ؼ���С,λ�� 
	*  @return 	void 
	*/
	//##ModelId=49B87B8602B7
	void RegulateControlSize();

/*
 *  @brief   	RefureDetail	 ˢ����ʾ����ϸ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B8602B8
	void RefureDetail();
private:
/*
 *  @brief   	InitSignListStyle	 ��ʼ���ź����б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitSignListStyle();

/*
 *  @brief   	InitSignListStyle	 ��ʼ���������б���ʽ 
 *  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
 */
	//##ModelId=49B87B870290
	int	InitChrListStyle();

/*
 *  @brief   	AddSignToList	 ���ź�����ӵ��б� 
 *  @param 		[In]a param of Type  PT_Signal*  ָ���ź���
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddSignToList(PT_Signal* pSign, int nIndex = 0);

/*
 *  @brief   	AddChrToList	 ����������ӵ��б� 
 *  @param 		[In]a param of Type  PT_ActionCharacter*  ָ��������
 *  @param 		[In]a param of Type  int  ָ�����б��е�λ��
 *  @return 	BOOL ����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL AddChrToList(PT_ActionCharacter* pChr, int nIndex = 0);

/*
 *  @brief   	FillListData	 ����б����� 
 *  @return 	void 
 */
	void FillListData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSOEDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewSOEDetail();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSOEDetail)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	afx_msg void OnViewSoeSelectChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewSoeListAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSOEDETAIL_H__D21513C2_AFA3_416B_A0CE_C020A6EBCBFB__INCLUDED_)
