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
	/** @brief           页面所对应事件*/
	CXJEvent*	m_pEvent;

// Operations
public:
		/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87B8602B7
	void RegulateControlSize();

/*
 *  @brief   	RefureDetail	 刷新显示的详细内容 
 *  @return 	void 
 */
	//##ModelId=49B87B8602B8
	void RefureDetail();
private:
/*
 *  @brief   	InitSignListStyle	 初始化信号量列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B870290
	int	InitSignListStyle();

/*
 *  @brief   	InitSignListStyle	 初始化特征量列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B870290
	int	InitChrListStyle();

/*
 *  @brief   	AddSignToList	 将信号量添加到列表 
 *  @param 		[In]a param of Type  PT_Signal*  指定信号量
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	BOOL AddSignToList(PT_Signal* pSign, int nIndex = 0);

/*
 *  @brief   	AddChrToList	 将特征量添加到列表 
 *  @param 		[In]a param of Type  PT_ActionCharacter*  指定特征量
 *  @param 		[In]a param of Type  int  指定在列表中的位置
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	BOOL AddChrToList(PT_ActionCharacter* pChr, int nIndex = 0);

/*
 *  @brief   	FillListData	 填充列表数据 
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
