#if !defined(AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_)
#define AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEventSignal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEventSignal dialog

//##ModelId=49B87BA40214
class CDlgEventSignal : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA40223
	CDlgEventSignal(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=49B87BA40225
	~CDlgEventSignal();
// Dialog Data
	//{{AFX_DATA(CDlgEventSignal)
	enum { IDD = IDD_DLG_EVENT_SIGNAL };
	//##ModelId=49B87BA40227
	CListCtrlEx	m_List;
	//}}AFX_DATA
private:
	/** @brief           事件所有信号量列表*/
	//##ModelId=49B87BA40233
	CTypedPtrArray<CPtrArray,PT_Signal*> m_arrSignalsOutput;
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87BA40237
	int		m_nOldSortCol;
public:
	int     m_colHide;
public:
	/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87BA40238
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEventSignal)
	protected:
	//##ModelId=49B87BA40246
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
/*
 *  @brief   	AddSignal	 加入信号量到信号量列表 
 *  @param 		[In]a param of Type  PT_Signal*  加入信号量到信号量列表
 *  @return 	void 
 */
	//##ModelId=49B87BA40249
	void AddSignal(PT_Signal* sign);
private:
/*
 *  @brief   	InitListStyle	 初始化列表 
 *  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4024B
	BOOL InitListStyle();

/*
 *  @brief   	InitListData	 初始化列表数据 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA40251
	BOOL InitListData();

/*
 *  @brief   	AddSignalToList	 把单项信号量数据加入到列表 
 *  @param 		[In]a param of Type  PT_Signal*  信号量
 *	@param		[In]a param of Type	 int	位置
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA40252
	BOOL AddSignalToList(PT_Signal* sign, int nIndex = 0);

/*
 *  @brief   	GetSignalDescribe	 得到信号量的文字描述 
 *  @param 		[In]a param of Type  PT_Signal*  信号量
 *  @return 	CString 文字描述
 */
	//##ModelId=49B87BA40255
	CString GetSignalDescribe(PT_Signal* sign);

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEventSignal)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListEventsign(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnEventsignCopy();
	afx_msg void OnBtnEventsignPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEVENTSIGNAL_H__AE0E0164_8B00_4B75_926D_577808C56E5A__INCLUDED_)
