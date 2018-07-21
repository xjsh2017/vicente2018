#if !defined(AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_)
#define AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDetailStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus dialog

class DlgDetailStatus : public CDialog
{
public:
	CommStatusList* pList;
	POSITION startPos; 
	/** @brief           记录之前被排序的列*/
	int		m_nOldSortCol;
// Construction
public:
	DlgDetailStatus(CWnd* pParent = NULL);   // standard constructor
	DlgDetailStatus(CommStatusList* pCommList, 	POSITION pos, CWnd* pParent = NULL);  

// Dialog Data
	//{{AFX_DATA(DlgDetailStatus)
	enum { IDD = IDD_DLG_DETAIL_STATUS };
	CListCtrlEx	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDetailStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDetailStatus)
	virtual BOOL OnInitDialog();
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickListDetailStatus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

   /*
    *  @brief   	InitListCtrl	 初始化列表 
    *  @return 	int		失败返回-1, 成功返回>=0
	*/
	void InitListCtrl();
	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	int	InitListStyle();

   /*
    *  @brief   	LoadListConfig	载入列信息配置	  
    *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
    */
	BOOL LoadListConfig();

   /*
    *  @brief   	SaveListConfig	 保存列信息配置 
    *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
    */
	BOOL SaveListConfig();

	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	void RegulateControlSize();

	/*
	*  @brief   	InitCOM	 初始化打印模板COM组件 
	*  @return 	BOOL 初始化成功返回TRUE, 失败返回FALSE
	*/
	BOOL InitCOM();
	
	/*
	*  @brief   	ReleaseCOM	 删除COM组件 
	*  @return 	void 
	*/
	void ReleaseCOM();

/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

	BOOL GetStatus(Record* pRecord);
	
	void SetRecordInfo(Record* pRecord, int nItem);
	
	void FillDataList();

	BOOL NeedSave();

private:
	/** @brief           列表的列信息*/
	CArray<SColumn, SColumn&> m_arrColum;

	/** @brief           打印模板标识*/
	CLSID clsid;

	/** @brief           COM类工厂指针*/
	LPCLASSFACTORY pClf;

	/** @brief           COM指针*/
	LPUNKNOWN pUnk;

	/** @brief           打印模板接口指针*/
	IXJReport * pReportDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDETAILSTATUS_H__997DE768_34AF_49F5_AB9B_D87BE2562D1D__INCLUDED_)
