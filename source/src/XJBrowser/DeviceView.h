// ***************************************************************
//  DeviceView   version:  1.0    date: 23:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 设备树视图
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_)
#define AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "..\..\COMMON\xjlib.h"	// Added by ClassView

/**
 * @defgroup 	CDeviceView	设备树视图类
 * @{
 */
/**
 * @brief       实现设备树, 显示设备信息 
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
class CDeviceView : public CFormView
{
protected:
	CDeviceView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDeviceView)

// Form Data
public:
	//{{AFX_DATA(CDeviceView)
	enum { IDD = IDD_XJ_DEVICE_VIEW };
	CComboBox	m_comboSelect;
	CTreeCtrl	m_Tree;
	CListCtrl	m_List;
	CString	m_strSelect;
	//}}AFX_DATA

// Attributes
public:
private:
	/** @brief           当前选中树节点*/
	//##ModelId=49B87BA403B3
	HTREEITEM htCurrent;
	CImageList m_imageList;
	COleDataSource	m_dataSource;
	CBitmap    m_bitmap;
	int		m_nListType;

	CDeviceObj *m_pSelObj;

	/** @brief           查询计时器*/
	int m_nQueryTimer;	
	/** @brief           */
	int	m_nTimer;
	int m_nDZ_MOD_State;
	/** @brief           召唤列表*/
	CTypedPtrArray<CPtrArray, StationStauts_Call*> m_arrCall;
	/** @brief           召唤结果*/
	CString m_sResult;
// Operations
public:
private:
/*
 *  @brief   	InsertDevice	 加入设备到设备树 
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *	@param		[In]a param of Type	 CDeviceObj* 指定设备
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
 *  @brief   	InsertDevice	 加入保护设备到设备树 
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *	@param		[In]a param of Type	 CDeviceObj* 指定设备
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertPTDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
*  @brief   	InitDeviceTree	 初始化设备树 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87BA403B4
	int InitDeviceTree();
	
	/*
	*  @brief   	InitListCtrl	 初始化列表 
	*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87BA403B9
	int InitListCtrl();
	
	/*
	*  @brief   	RegulateControlSize	 调整控件大小,位置 
	*  @return 	void 
	*/
	//##ModelId=49B87BA403BA
	void RegulateControlSize();

/*
 *  @brief   	FindNetPos	 查找地区在树中的位置 
 *  @param 		[In]a param of Type  CString  地区ID
 *  @return 	HTREEITEM 树节点结构
 */
	//##ModelId=49B87BA403BB
	HTREEITEM FindNetPos(CString sNetID);

/*
 *  @brief   	InsertNet	 插入地区节点 
 *  @param 		[In]a param of Type  CNetObj*  要插入的地区对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA403BD
	BOOL InsertNet(CNetObj* pObj);

/*
 *  @brief   	InsertStation	 插入厂站节点 
 *  @param 		[In]a param of Type  CStationObj*  要插入的厂站对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA403C8
	BOOL InsertStation(CStationObj* pObj);

/*
 *  @brief   	InsertStation_Voltage	 插入厂站节点，厂站下接电压等级 
 *  @param 		[In]a param of Type  CStationObj*  要插入的厂站对象
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	BOOL InsertStation_Voltage(CStationObj* pObj);

/*
 *  @brief   	InsertVoltage	 插入电压等级节点 
 *  @param 		[In]a param of Type  int  指定电压等级
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *	@param		[In]a param of Type	 CStationObj* 指定厂站
 *  @return 	BOOL 插入成功返回TRUE, 失败返回FALSE
 */
	BOOL InsertVoltage(int nVoltage, HTREEITEM htParent, CStationObj* pObj);

/*
 *  @brief   	SortVoltage	 对站内电压等级排序 
 *  @param 		[In]a param of Type  CStationObj*  指定站
 *  @return 	void 
 */
	void SortVoltage(CStationObj* pObj);

/*
 *  @brief   	BuildBreak	 把指定厂站指定电压等级的所有开关加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildBreak(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBus	 把指定厂站指定电压等级的所有母线加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildBus(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildCap	 把指定厂站指定电压等级的所有电容器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildCap(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildGen	 把指定厂站指定电压等级的所有发电机加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildGen(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildLine	 把指定厂站指定电压等级的所有线路加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildLine(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildReactor	 把指定厂站指定电压等级的所有电抗器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildReactor(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildSwitch	 把指定厂站指定电压等级的所有刀闸加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildSwitch(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildTrans	 把指定厂站指定电压等级的所有变压器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *	@param		[In]a param of Type	 int  指定间隔
 *  @return 	void 
 */
	void BuildTrans(CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildProtect	 把指定厂站的所有保护加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	//##ModelId=49B87BB100A1
	void BuildProtect(CStationObj* pStation);

/*
 *  @brief   	InsertProtect	 加入保护到设备树 
 *  @param 		[In]a param of Type  CSecObj*  指定保护
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1009C
	BOOL InsertProtect(CSecObj* pObj);

/*
 *  @brief   	BuildSecToStation	 把指定厂站的所有二次设备加入到设备树中的对应厂站下 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	void BuildSecToStation(CStationObj* pStation);

	BOOL InsertSecToStation(CSecObj* pObj);

/*
 *  @brief   	BuildWaveRec	 把指定厂站的所有录波器加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @return 	void 
 */
	//##ModelId=49B87BB1008E
	void BuildWaveRec(CStationObj* pStation, HTREEITEM htParent);

/*
 *  @brief   	InsertFastness	 添加硬节点 
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  int  指定电压等级
 *  @param 		[In]a param of Type  CString  指定间隔ID
 *  @return 	void 
 */
	void InsertFastness(HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID);

/*
 *  @brief   	BuildBay	 把指定厂站指定电压等级的所有间隔加入到设备树 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  int  指定电压等级
 *  @return 	void 
 */
	void BuildBay(CStationObj* pStation, HTREEITEM htParent, int nVoltage);

/*
 *  @brief   	InsertBay	 插入间隔到设备树 
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *	@param		[In]a param of Type	 CBayObj*	指定间隔
 *  @return 	void 
 */
	void InsertBay(HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay);

/*
 *  @brief   	ClearEmptyNodeType1	 清除设备树类型为1的设备树的空节点 
 *  @return 	void 
 */
	void ClearEmptyNodeType1();

/*
 *  @brief   	ClearEmptyNodeType2	 清除设备树类型为2的设备树的空节点 
 *  @return 	void 
 */
	void ClearEmptyNodeType2();

/*
 *  @brief   	ClearEmptyNodeType4	 清除设备树类型为4的设备树的空节点 
 *  @return 	void 
 */
	void ClearEmptyNodeType4();

public:
/*
 *  @brief   	FillTree	 填充设备树 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA403CA
	BOOL FillTree();

/*
 *  @brief   	FillList	 填充列表框内容 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA403CB
	BOOL FillList();

	void BuildGPS( CStationObj* pStation, HTREEITEM htParent );
	
	void BuildDistance(CString sNetID,  HTREEITEM htParent );

private:
	BOOL IsChinese(CString str);
	void CompareStation(CString str);

/*
 *  @brief   	LocateInTree	 在设备树中定位指定厂站 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	//##ModelId=49B87BA403CC

	/*
	*  @brief   	QueryStationStatus	 查询厂站通讯状态 
	*  @param 		[In]a param of Type  CStationObj*  指定厂站
	*  @param 		[In]a param of Type  int  指定通道
	*  @return 	int 失败返回<0的整数，成功返回0 
	*/
	int QueryStationStatus(CStationObj* pObj, int nChannel = 0);
	
	void TestStationStatus(CStationObj* pStation);
	
	/*
	*  @brief   	ClearCallList	 清空召唤列表 
	*  @return 	void 
	*/
	void ClearCallList();
	
	/*
	*  @brief   	AddToCallList	 将指定厂站指定通道加入到待召唤列表 
	*  @param 		[In]a param of Type  CStationObj*  指定厂站
	*  @param 		[In]a param of Type  int  指定通道
	*  @return 	void 
	*/
	void AddToCallList(CStationObj* pObj, int nChannel);
	
	/*
	*  @brief   	StartCall	 对召唤项进行召唤 
	*  @param 		[In]a param of Type  StationStauts_Call*  指定召唤项
	*  @return 	void 
	*/
	void StartCall(StationStauts_Call* pCall);

public:
	void LocateInTree(CStationObj* pStation);
	void LocateObjInTree(CDeviceObj *pObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceView)
	public:
	//##ModelId=49B87BA403CE
	virtual void OnInitialUpdate();
	protected:
	//##ModelId=49B87BA403D8
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BA403DB
	virtual ~CDeviceView();
#ifdef _DEBUG
	//##ModelId=49B87BA403DD
	virtual void AssertValid() const;
	//##ModelId=49B87BA403DF
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDeviceView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTreeOpenmap();
	afx_msg void OnTreeSetpath();
	afx_msg void OnPtSignallist();
	afx_msg void OnQueryEvent();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMENUITEMDevTreePTRunUpdate();
    afx_msg void OnStationConfigChange();
	afx_msg void OnEditchangeComboDevice();
	afx_msg void OnSelendokComboDevice();
	afx_msg void OnTreeStationProp();
	afx_msg void OnTreeStationAdd();
	afx_msg void OnDevTreePTADD();
	afx_msg void OnDevTreePTCONFIGCHANGED();
	afx_msg void OnMarkDevice();
	afx_msg void OnMarkPtdevice();
	afx_msg void OnTreeStationTest();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnPTProp(UINT nID);
	afx_msg void OnWRProp(UINT nID);
	afx_msg void OnStationInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationChanged( WPARAM wParam, LPARAM lParam );
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEndCallStationStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEVIEW_H__2149B055_7BCA_4713_B376_F1D908103258__INCLUDED_)
