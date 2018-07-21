// ***************************************************************
//  ViewBase   version:  1.0    date: 10:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 继承自FormView, 提供动态改变控件位置功能
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_)
#define AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewBase.h : header file
//
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CViewBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B860205
struct ctrlPos{
	//##ModelId=49B87B860214
	UINT	nID;		//控件ID
	//##ModelId=49B87B860215
	float	fWidth;		//控件宽度/客户区宽度
	//##ModelId=49B87B860216
	float	fHeight;	//控件高度/客户区高度
	//##ModelId=49B87B860217
	int		nType;		//变化类型
	//##ModelId=49B87B860218
	int		nID_Left;	//左边相对哪个控件位置不变
	//##ModelId=49B87B860219
	int		Left_Dis;	//左边相对距离
	//##ModelId=49B87B860222
	int		nID_Top;	//上边相对哪个控件位置不变
	//##ModelId=49B87B860223
	int		Top_Dis;	//上边相对距离
	//##ModelId=49B87B860224
	int		nID_Right;	//右边相对哪个控件位置不变
	//##ModelId=49B87B860225
	int		Right_Dis;	//右边相对距离
	//##ModelId=49B87B860226
	int		nID_Bottom;	//下边相对哪个控件位置不变
	//##ModelId=49B87B860227
	int		Bottom_Dis;	//下边相对距离
};

//相对位置定义
int const	left_client = -1;	//客户区左边位置
int const	right_client = -2;	//客户区右边位置
int const	top_client = -3;	//客户区上边位置
int const	bottom_client = -4; //客户区下边位置
int const	mid_client = -5;	//客户区中间位置,横向
int const	null_effect = -99;	//此项无效

//宽度高度改变规则定义
int const	change_null = 0;	//宽度高度都不变
int const	change_width = 1;	//宽度改变
int const	change_height = 2;	//高度改变
int	const	change_all	= 3;	//都改变


//##ModelId=49B87B860242
class CViewBase : public CFormView
{
protected:
	//##ModelId=49B87B860244
	CViewBase();           // protected constructor used by dynamic creation
	//##ModelId=49B87B860245
	CViewBase(UINT nIDTemplate);
	DECLARE_DYNCREATE(CViewBase)

// Form Data
public:
	//{{AFX_DATA(CViewBase)
	enum { IDD = 0 };
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B860248
	CArray<ctrlPos,ctrlPos> posCtrl;
	//##ModelId=49B87B860254
	UINT m_nDlgID;
	//##ModelId=49B87B860255
	CSize szDlg;
	/** @brief           记录页面是否已载入详细信息*/
	//##ModelId=49B87B860256
	BOOL m_bLoadData;
	/** @brief           自动召唤周期*/
	CString		m_sCyc;

// Operations
public:
/*
 *  @brief   	SetCyc	 设置自动总召周期 
 *  @param 		[In]a param of Type  CString  第一次启动时间
 *  @param 		[In]a param of Type  CString  周期
 *  @return 	void 
 */
	//##ModelId=49B87BB20138
	virtual void SetCyc(CString sStartTime, CString sCyc){} ;

/*
 *  @brief   	MoveCtrl	移动控件位置,类似MFC的MoveWindow 
 *  @param 		[In]a param of Type  UINT  要移动的控件的ID
 *  @param 		[In]a param of Type  int  左方位置,如果输入的是其它控件ID,即是此控件right + 偏移量
 *  @param 		[In]a param of Type  int  上方位置,如果输入的是其它控件ID,即是此控件bottom + 偏移量
 *  @param 		[In]a param of Type  int  右方位置,如果输入的是其它控件ID,即是此控件left - 偏移量
 *  @param 		[In]a param of Type  int  下方位置,如果输入的是其它控件ID,即是此控件top - 偏移量
 *	@param		[In]a param of Type	 int  偏移量
 *  @return 	void 
 */
	//##ModelId=49B87B860257
	void MoveCtrl(UINT nID, int nLeft, int nTop, int nRight, int nBottom, int offset = 10);
/*
 *  @brief   	RegulateControlSize	 调整控件位置
 *  @return 	void 
 */
	//##ModelId=49B87B860263
	void RegulateControlSize();
/*
 *  @brief   	RecordRate	 记录控件比例 
 *  @param 		[In/]a param of Type  UINT  要记录的控件的ID
 *  @param 		[In]a param of Type  int  改变类型
 *  @param 		[In]a param of Type  int  左边相对的位置
 *  @param 		[In]a param of Type  int  上边相对的位置
 *  @param 		[In]a param of Type  int  右边相对的位置
 *  @param 		[In]a param of Type  int  下边相对的位置
 *  @return 	void 
 */
	//##ModelId=49B87B860264
	void RecordRate(UINT nID, int nType = 0, int nLeft = null_effect, 
		int nTop = null_effect, int nRight = null_effect, int nBottom = null_effect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewBase)
	public:
	//##ModelId=49B87B860271
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	//##ModelId=49B87B860275
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87B860277
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B860280
	virtual ~CViewBase();
#ifdef _DEBUG
	//##ModelId=49B87B860282
	virtual void AssertValid() const;
	//##ModelId=49B87B860284
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewBase)
	//##ModelId=49B87B860290
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87B860295
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBASE_H__454108C2_8609_4F41_9090_1A3899ABB8A1__INCLUDED_)
