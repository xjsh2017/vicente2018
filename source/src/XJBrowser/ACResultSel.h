#if !defined(AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_)
#define AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ACResultSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CACResultSel form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87BB50203
class CACResultSel : public CFormView
{
protected:
	//##ModelId=49B87BB50205
	CACResultSel();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CACResultSel)

// Form Data
public:
	//{{AFX_DATA(CACResultSel)
	enum { IDD = IDD_ACRESULTSEL };
	//##ModelId=49B87BB50207
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

// Attributes
public:
	/** @brief           保存根节点*/
	//##ModelId=49B87BB5020B
	HTREEITEM	m_hRoot;

// Operations
public:
/*
 *  @brief   	InsertNet	 插入地区节点 
 *  @param 		[In]a param of Type  CNetObj*  要插入的地区对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB50213
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 插入厂站节点 
 *  @param 		[In]a param of Type  CStationObj*  要插入的厂站对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB50215
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	FindNetPos	 查找地区在树中的位置 
 *  @param 		[In]a param of Type  CString  地区ID
 *  @return 	HTREEITEM 树节点结构
 */
	//##ModelId=49B87BB50217
	HTREEITEM FindNetPos(CString sNetID);

	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87BB50219
	void RegulateControlSize();

/*
 *  @brief   	BuildNet	 在设备树中创建地区 
 *  @return 	void 
 */
	//##ModelId=49B87BB5021A
	void BuildNet();

/*
 *  @brief   	BuildStation	 在设备树中创建厂站 
 *  @return 	void 
 */
	//##ModelId=49B87BB5021B
	void BuildStation();

/*
 *  @brief   	InitTree	 初始化设备树 
 *  @return 	int 初始化成功返回>=0的值, 失败返回<0的值
 */
	//##ModelId=49B87BB5021C
	int InitTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACResultSel)
	public:
	//##ModelId=49B87BB50222
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BB50224
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB50227
	virtual ~CACResultSel();
#ifdef _DEBUG
	//##ModelId=49B87BB50229
	virtual void AssertValid() const;
	//##ModelId=49B87BB5022B
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CACResultSel)
	//##ModelId=49B87BB50233
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87BB50238
	afx_msg void OnSelchangedTreeAcresultsel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//##ModelId=49B87BB50242
	afx_msg void OnAutoCallResultFrameOpen(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACRESULTSEL_H__5F3FBD49_D656_4CF1_8F61_30EFDF3C3D5E__INCLUDED_)
