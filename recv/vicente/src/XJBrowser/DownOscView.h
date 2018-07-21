#if !defined(AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_)
#define AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownOscView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownOscView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDownOscView : public CViewBase
{
protected:
	CDownOscView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownOscView)

// Form Data
public:
	//{{AFX_DATA(CDownOscView)
	enum { IDD = IDD_DOWNLOAD_OSC };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Attributes
public:
private:
	/** @brief           列表的列信息*/
	CArray<SColumn, SColumn&> m_arrColum;
	/** @brief           事件管理器*/
	CXJEventManager		m_EventManager;
	/** @brief           保存召唤定时器*/
	int		m_nTimer;
	/** @brief           保存定时下载定时器*/
	int		m_nDownTimer;
	/** @brief           当前正在下载的录波文件*/
	CXJEventOSC*	m_pCurrentOsc;

// Operations
public:
private:
/*
 *  @brief   	LoadListConfig	载入列信息配置	  
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA301A5
	BOOL LoadListConfig();

/*
 *  @brief   	SaveListConfig	 保存列信息配置 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA301B5
	BOOL SaveListConfig();

/*
 *  @brief   	NeedSave	 判断是否需要保存列表的列信息
 *  @return 	BOOL 需要保存返回TRUE,不需要保存返回FALSE
 */
	//##ModelId=49B87BA301C5
	BOOL NeedSave();

/*
 *  @brief   	InitListStyle	 初始化列表样式 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87BA301D4
	int	InitListStyle();

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventOSC *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8F0272
	BOOL AddEventToList(CXJEventOSC * pEvent, int i);

/*
 *  @brief   	AddEvent	 添加指定事件到视图 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定事件
 *  @return 	void 
 */
	void AddEvent(CXJEventOSC* pEvent);

/*
 *  @brief   	DownloadOSCFile	 下载指定录波列表的录波文件 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波列表
 *  @return 	void 
 */
	//##ModelId=49B87B8F0290
	void DownloadOSCFile(CXJEventOSC* pEvent);

/*
 *  @brief   	ChangeValue	 改变录波列表是否已下载的显示
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波列表
 *  @return 	void 
 */
	//##ModelId=49B87B8F0292
	void ChangeValue(CXJEventOSC* pEvent);

/*
 *  @brief   	ChangeDownloadedValue	 修改正在下载的文件的已下载内容大小 
 *  @param 		[In]a param of Type  CXJEventOSC*  正在下载的文件
 *  @param 		[In]a param of Type  CString  已下载内容大小
 *  @return 	void 
 */
	void ChangeDownloadedValue(CXJEventOSC* pEvent, CString sValue);

/*
 *  @brief   	CancelCall	 取消召唤操作
 *  @return 	void 
 */
	//##ModelId=49B87B8F0271
	void CancelCall();

/*
 *  @brief   	NeedAdd	 判断指定事件是否要加入到待下载列表 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定录波事件
 *  @param		[Out]a param of Type CXJEventOSC*  返回已存在的对应项
 *  @return 	BOOL 要加入返回TRUE, 不要加入返回FALSE
 */
	BOOL NeedAdd(CXJEventOSC* pEvent, CXJEventOSC*& pExist);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownOscView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDownOscView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDownOscView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDownoscDown();
	afx_msg void OnDownoscCancel();
	afx_msg void OnDownoscOpen();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnDownOscOpen(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNOSCVIEW_H__0E969589_61D2_42B6_A525_F4BFA7314140__INCLUDED_)
