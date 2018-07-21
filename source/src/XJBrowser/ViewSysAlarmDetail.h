#if !defined(AFX_VIEWSYSALARMDETAIL_H__B3956E06_0B9C_4E89_80B6_4AE46E68B39A__INCLUDED_)
#define AFX_VIEWSYSALARMDETAIL_H__B3956E06_0B9C_4E89_80B6_4AE46E68B39A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewSysAlarmDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarmDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B81009C
class CViewSysAlarmDetail : public CFormView
{
protected:
	//##ModelId=49B87B81009E
	CViewSysAlarmDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewSysAlarmDetail)

// Form Data
public:
	//{{AFX_DATA(CViewSysAlarmDetail)
	enum { IDD = IDD_XJ_VIEW_SYSALARM_DETAIL };
	CListCtrlEx	m_List;
	CString	m_sTime;
	CString	m_sDevice;
	CString	m_sDetail;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           保存当前页面选择的事件指针*/
	//##ModelId=49B87B8100AD
	CXJEventSysAlarm*	m_pEvent;

// Operations
public:
private:
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 		void 
	*/
	//##ModelId=49B87B8100B1
	void RegulateControlSize();

/*
 *  @brief   	RefureDetail	 刷新显示的详细内容 
 *  @return 	void 
 */
	//##ModelId=49B87B8100B2
	void RefureDetail();

/*
 *  @brief   	InitListStyle	 初始化列表 
 *  @return 	void 
 */
	void InitListStyle();

/*
 *  @brief   	AddDetailToList	 添加详细信息到列表 
 *  @param 		[In]a param of Type  SYSALARM*  详细信息数据
 *  @param 		[In]a param of Type  int  添加的位置
 *  @return 	BOOL 
 */
	BOOL AddDetailToList(SYSALARM* sa, int nIndex);

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	void 
 */
	void FillListData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewSysAlarmDetail)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B8100BC
	virtual ~CViewSysAlarmDetail();
#ifdef _DEBUG
	//##ModelId=49B87B8100BE
	virtual void AssertValid() const;
	//##ModelId=49B87B8100C0
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewSysAlarmDetail)
	//##ModelId=49B87B8100C3
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=49B87B8100CB
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	//##ModelId=49B87B8100D0
	afx_msg void OnListSelChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8100D4
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8100DA
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSYSALARMDETAIL_H__B3956E06_0B9C_4E89_80B6_4AE46E68B39A__INCLUDED_)
