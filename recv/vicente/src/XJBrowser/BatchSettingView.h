// ***************************************************************
//  BatchSettingView   version:  1.0    date: 2:11:2008     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 批量召唤选择视图, 包括设备树和召唤选项设置
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_BatchSettingView_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_)
#define AFX_BatchSettingView_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchSettingView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchSettingView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/**
 * @defgroup 	CBatchSettingView	批量召唤选择视图类
 * @{
 */
/**
 * @brief       视图中包括一个设备树,一个列表(用来设置召唤选项)
*  @author      LYH
 * @version     ver1.0
 * @date        02/11/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BB0038A
class CBatchSettingView : public CFormView
{
protected:
	//##ModelId=49B87BB0038C
	CBatchSettingView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBatchSettingView)

// Form Data
public:
	//{{AFX_DATA(CBatchSettingView)
	enum { IDD = IDD_BATCH_SETTING };
	//##ModelId=49B87BB0039A
	CMyTreeListCtrl	m_Tree;
    CImageList	m_imageList;
	CBitmap     m_bitmap;
	//}}AFX_DATA

// Attributes
private:
	/** @brief           定值区号是否召唤*/
	BOOL	m_bZone;
	/** @brief           定值是否召唤*/
	//##ModelId=49B87BB003AE
	BOOL	m_bSetting;
	/** @brief           开关量是否召唤*/
	//##ModelId=49B87BB003AF
	BOOL	m_bDigital;
	/** @brief           软压板是否召唤*/
	//##ModelId=49B87BB003B0
	BOOL	m_bSoftBoard;
	/** @brief           模拟量是否召唤*/
	//##ModelId=49B87BB003B9
	BOOL	m_bAnalog;
	/** @brief           保存根节点*/
	//##ModelId=49B87BB003BA
	CTreeListItem*	m_hRoot;
	/** @brief           页面类型, 0为自动总召设置. 1为批量召唤*/
	//##ModelId=49B87BB003BB
	int		m_nViewType;
	/** @brief           当前可操作的厂站，NULL表示所有*/
	CStationObj*	m_pActiveStation;

// Operations
public:
private:
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87BB003C8
	void RegulateControlSize();

/*
 *  @brief   	InitTree	 初始化设备树 
 *  @return 	int 初始化成功返回>=0的值, 失败返回<0的值
 */
	//##ModelId=49B87BB003D8
	int InitTree();

/*
 *  @brief   	BuildNet	 在设备树中创建地区 
 *  @return 	void 
 */
	//##ModelId=49B87BB10000
	void BuildNet();

/*
 *  @brief   	FindNetPos	 查找地区在树中的位置 
 *  @param 		[In]a param of Type  CString  地区ID
 *  @return 	CTreeListItem* 树节点结构
 */
	//##ModelId=49B87BB10001
	CTreeListItem* FindNetPos(CString sNetID);

/*
 *  @brief   	InsertNet	 插入地区节点 
 *  @param 		[In]a param of Type  CNetObj*  要插入的地区对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1000F
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 插入厂站节点 
 *  @param 		[In]a param of Type  CStationObj*  要插入的厂站对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1001F
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	BuildStation	 在设备树中创建厂站 
 *  @return 	void 
 */
	//##ModelId=49B87BB10021
	void BuildStation();

/*
 *  @brief   	InsertDevice	 加入设备到设备树 
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *	@param		[In]a param of Type	 CDeviceObj* 指定设备
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(CTreeListItem* htParent, CDeviceObj* pObj);

/*
 *  @brief   	BuildBus	 把指定厂站的所有母线加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB1003E
	void BuildBus(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildLine	 把指定厂站的所有线路加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB10041
	void BuildLine(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildGen	 把指定厂站的所有发电机加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  输入参数说明
 *  @param 		[In]a param of Type  CTreeListItem*  输入参数说明
 *  @return 	void 
 */
	//##ModelId=49B87BB10050
	void BuildGen(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildTrans	 把指定厂站的所有变压器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  输入参数说明
 *  @param 		[In]a param of Type  CTreeListItem*  输入参数说明
 *  @return 	void 
 */
	//##ModelId=49B87BB1005E
	void BuildTrans(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildCap	 把指定厂站的所有电容器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB1006D
	void BuildCap(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildReactor	 把指定厂站的所有电抗器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB10070
	void BuildReactor(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildBreak	 把指定厂站的所有开关加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB1007E
	void BuildBreak(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildSwitch	 把指定厂站的所有刀闸加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB10081
	void BuildSwitch(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	BuildWaveRec	 把指定厂站的所有录波器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  CTreeListItem*  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB1008E
	void BuildWaveRec(CStationObj* pStation, CTreeListItem* htParent);

/*
 *  @brief   	InsertProtect	 加入保护到设备树 
 *  @param 		[In]a param of Type  CSecObj*  指定保护
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1009C
	BOOL InsertProtect(CSecObj* pObj);

/*
 *  @brief   	InsertCPU	 加入CPU到设备树 
 *  @param 		[In]a param of Type  CSecCPU*  指定CPU
 *	@param		[In]a param of Type	 CTreeListItem* 父节点位置
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1009E
	BOOL InsertCPU(CSecCPU* pCpu, CTreeListItem* htParent);

/*
 *  @brief   	BuildProtect	 把指定厂站的所有保护加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	//##ModelId=49B87BB100A1
	void BuildProtect(CStationObj* pStation);

/*
 *  @brief   	ClearEmptyNode	 去除无子节点的节点 
 *  @return 	void 
 */
	//##ModelId=49B87BB100AC
	void ClearEmptyNode();

/*
 *  @brief   	CheckDevice	 对某设备进行了Check操作, 如果是保护或录波器, 通知详细视图
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @param 		[In]a param of Type  int  1表示选中, 0表示取消选择
 *  @return 	void 
 */
	//##ModelId=49B87BB100AD
	void CheckDevice(CDeviceObj* pObj, int nSelected);

/*
 *  @brief   	CycTreeDown	 向下遍历指定节点的所有子节点 
 *  @param 		[In]a param of Type  CTreeListItem*  指定节点
 *  @param 		[In]a param of Type  BOOL  选择标志.TRUE为选中, FALSE为取消选择
 *  @return 	void 
 */
	//##ModelId=49B87BB100BB
	void CycTreeDown(CTreeListItem* hRoot, BOOL bMark);

/*
 *  @brief   	CycTreeUp	 向上遍历节点的父节点 
 *  @param 		[In]a param of Type  CTreeListItem*  指定节点
 *  @return 	void 
 */
	//##ModelId=49B87BB100BE
	void CycTreeUp(CTreeListItem* hCurrent);

/*
 *  @brief   	HitItemHelp	 处理节点被点击事件 
 *  @param 		[In]a param of Type  CTreeListItem*  被点击的节点
 *  @return 	void 
 */
	//##ModelId=49B87BB100CB
	void HitItemHelp(CTreeListItem* hit);

/*
 *  @brief   	SetCheck_XJ	 设置指定节点的选择状态 
 *  @param 		[In]a param of Type  CTreeListItem*  指定节点
 *  @param 		[In]a param of Type  BOOL  TRUE-选中. FALSE-取消选择
 *  @return 	void 
 */
	//##ModelId=49B87BB100CD
	void SetCheck_XJ(CTreeListItem* hit, BOOL bCheck);

/*
 *  @brief   	IsEnableItem	 判断指定节点是否可操作 
 *  @param 		[In]a param of Type  CTreeListItem*  指定节点
 *  @return 	BOOL 可操作返回TRUE,不可操作返回FALSE
 */
	BOOL IsEnableItem(CTreeListItem* ht);

/*
 *  @brief   	ClearCheck	 清除所有选择 
 *  @return 	void 
 */
	void ClearCheck(CTreeListItem* ht);

/*
 *  @brief   	PostMessageToDetailSettingView 响应树形控件选择消息 
 *  @return 	void 
 */
    void PostMessageToDetailSettingView(CTreeListItem* hItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchSettingView)
	public:
	//##ModelId=49B87BB100DA
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BB100DC
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB100EA
	virtual ~CBatchSettingView();
#ifdef _DEBUG
	//##ModelId=49B87BB100FA
	virtual void AssertValid() const;
	//##ModelId=49B87BB10109
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBatchSettingView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAutocallActive();
	afx_msg void OnAutocallAll();
	afx_msg void ShowDeviceList();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	//##ModelId=49B87BB10148
	afx_msg void OnBatchFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1014C
	afx_msg void OnBatchDetailUncheck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB10157
	afx_msg void OnBatchUnselectAll(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1015B
	afx_msg void OnBatchSelectAllPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB1015F
	afx_msg void OnBatchSelectAllWR(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BB10169
	afx_msg void OnBatchEnableControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickTree(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BatchSettingView_H__5F112E91_6E77_4D9C_BFE9_F9797747A093__INCLUDED_)
