#if !defined(AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_)
#define AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoCallStop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop dialog

//##ModelId=49B87BA402FD
class CDlgAutoCallStop : public CDialog
{
// Construction
public:
	//##ModelId=49B87BA402FF
	CDlgAutoCallStop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoCallStop)
	enum { IDD = IDD_AUTOCALL_STOP };
	//##ModelId=49B87BA4030E
	CListCtrl	m_List;
	//##ModelId=49B87BA40313
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	/** @brief           保存根节点*/
	//##ModelId=49B87BA40317
	HTREEITEM	m_hRoot;

	/** @brief           厂站总个数*/
	//##ModelId=49B87BA4031C
	int m_nStationNum;

	/** @brief           保存与自动总召模块的连接句柄*/
	//##ModelId=49B87BA4031E
	XJHANDLE	m_pAutoCall;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoCallStop)
	protected:
	//##ModelId=49B87BA4032C
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

/*
 *  @brief   	InitTree	 初始化设备树 
 *  @return 	int 初始化成功返回>=0的值, 失败返回<0的值
 */
	//##ModelId=49B87BA4032F
	int InitTree();

/*
 *  @brief   	BuildNet	 在设备树中创建地区 
 *  @return 	void 
 */
	//##ModelId=49B87BA40330
	void BuildNet();

/*
 *  @brief   	InsertNet	 插入地区节点 
 *  @param 		[In]a param of Type  CNetObj*  要插入的地区对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4033C
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 插入厂站节点 
 *  @param 		[In]a param of Type  CStationObj*  要插入的厂站对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA4033E
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	BuildStation	 在设备树中创建厂站 
 *  @return 	void 
 */
	//##ModelId=49B87BA40340
	void BuildStation();

/*
 *  @brief   	FindNetPos	 查找地区在树中的位置 
 *  @param 		[In]a param of Type  CString  地区ID
 *  @return 	HTREEITEM 树节点结构
 */
	//##ModelId=49B87BA4034B
	HTREEITEM FindNetPos(CString sNetID);

/*
 *  @brief   	CycTreeDown	 向下遍历指定节点的所有子节点 
 *  @param 		[In]a param of Type  HTREEITEM  指定节点
 *  @param 		[In]a param of Type  BOOL  选择标志.TRUE为选中, FALSE为取消选择
 *  @return 	void 
 */
	//##ModelId=49B87BA4034D
	void CycTreeDown(HTREEITEM hRoot, BOOL bMark);

/*
 *  @brief   	CycTreeUp	 向上遍历节点的父节点 
 *  @param 		[In]a param of Type  HTREEITEM  指定节点
 *  @return 	void 
 */
	//##ModelId=49B87BA40350
	void CycTreeUp(HTREEITEM hCurrent);

/*
 *  @brief   	HitItemHelp	 处理节点被点击事件 
 *  @param 		[In]a param of Type  HTREEITEM  被点击的节点
 *  @return 	void 
 */
	//##ModelId=49B87BA4035C
	void HitItemHelp(HTREEITEM hit);

/*
 *  @brief   	CheckDevice	 对某设备进行了Check操作, 如果是保护或录波器, 通知详细视图
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @param 		[In]a param of Type  int  1表示选中, 0表示取消选择
 *  @return 	void 
 */
	//##ModelId=49B87BA4035E
	void CheckDevice(CDeviceObj* pObj, int nSelected);

/*
 *  @brief   	ListAddStation	 把厂站加入到列表中显示 
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @return 	void 
 */
	//##ModelId=49B87BA4036B
	void ListAddStation(CDeviceObj* pObj);

/*
 *  @brief   	ListRemoveStation	 把厂站从列表中移除 
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @return 	void 
 */
	//##ModelId=49B87BA4036D
	void ListRemoveStation(CDeviceObj* pObj);

/*
 *  @brief   	ConnectAutoCall	 与自动总召模块建立连接 
 *  @return 	int 连接成功返回1, 失败返回0
 */
	//##ModelId=49B87BA4036F
	int	 ConnectAutoCall();

/*
 *  @brief   	DisConnectAutoCall	 关闭与自动总召的连接 
 *  @return 	void 
 */
	//##ModelId=49B87BA40370
	void DisConnectAutoCall();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoCallStop)
	//##ModelId=49B87BA4037A
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BA4037C
	afx_msg void OnClickTreeAutocalStop(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87BA40380
	afx_msg void OnBtnAutocallStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOCALLSTOP_H__DA1AE6BA_01A5_40E3_A316_1FBC5EECF92D__INCLUDED_)
