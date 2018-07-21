#if !defined(AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_)
#define AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStatusHis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis dialog

class CDlgStatusHis : public CDialog
{
// Construction
public:
	CDlgStatusHis(CWnd* pParent = NULL);   // standard constructor
	~CDlgStatusHis();

// Dialog Data
	//{{AFX_DATA(CDlgStatusHis)
	enum { IDD = IDD_PTSTATUS_HISTORY };
	CListCtrlEx	m_List;
	//}}AFX_DATA
	/** @brief           类型。1表示通讯状态。2表示运行状态*/
	int		m_nType;
	/** @brief           事件管理器*/
	CXJEventManager		m_EventManager;
	/** @brief           对应二次设备*/
	CSecObj*	m_pSec;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatusHis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
/*
 *  @brief   	QueryCommHis	 查询历史通讯状态 
 *  @return 	void 
 */
	void QueryCommHis();

/*
 *  @brief   	QueryRunHis	 查询历史运行状态 
 *  @return 	void 
 */
	void QueryRunHis();

/*
 *  @brief   	QueryData	 查询数据 
 *  @return 	void 
 */
	void QueryData();

/*
 *  @brief   	FillList	 填充列表框 
 *  @return 	void 
 */
	void FillList();

/*
 *  @brief   	AddPTCommToList	 将指定事件填充到列表框 
 *  @param 		[In]a param of Type  CXJEventPTComm*  指定事件
 *  @param 		[In]a param of Type  int  指定在列表框中的位置
 *  @return 	BOOL 填充成功返回TRUE,否则返回FALSE
 */
	BOOL AddPTCommToList(CXJEventPTComm* pEvent, int i);

/*
 *  @brief   	AddPTRunToList	 将指定事件填充到列表框 
 *  @param 		[In]a param of Type  CXJEventPTRun*  指定事件
 *  @param 		[In]a param of Type  int  指定在列表框中的位置
 *  @return 	BOOL 填充成功返回TRUE,否则返回FALSE
 */
	BOOL AddPTRunToList(CXJEventPTRun* pEvent, int i);

/*
 *  @brief   	InitListStyle	 初始化列表风格 
 *  @return 	void 
 */
	void InitListStyle();
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStatusHis)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATUSHIS_H__5AD2CA99_D46C_434B_94C8_F97ADB9A8173__INCLUDED_)
