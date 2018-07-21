#if !defined(AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_)
#define AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualFaultShorten.h : header file
//
typedef CMapStringToPtr SECOBJ_MAP;
/////////////////////////////////////////////////////////////////////////////
// CManualFaultShorten dialog
#include "ViewFault.h"

class CManualFaultShorten : public CDialog
{
// Construction
public:
	CManualFaultShorten(CWnd* pParent = NULL);   // standard constructor
    ~CManualFaultShorten();
// Dialog Data
	//{{AFX_DATA(CManualFaultShorten)
	enum { IDD = IDD_DLG_MANUAL_FAULTSHORTEN };
	CListCtrlEx	m_List;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	// Attributes
public:
	/** @brief           列表的列信息*/
	//##ModelId=49B87B870267
	CArray<SColumn, SColumn&> m_arrColum;
	SECOBJ_MAP		m_SECMap;
	/** @brief           动作故障量事件数组*/
	CPtrArray		m_listChr;
	/** @brief           动作信号量事件数组*/
	CPtrArray		m_listSign;
	CViewFault*      pView;
private:
	CImageList m_imageList;
	CBitmap    m_bitmap;
	int        m_nMainRecID;
	CXJEventManager	m_EventManager;

public:

/*
 *  @brief   	FillListData	 填充列表数据 
 *  @return 	int 失败返回-1, 成功返回>=0
 */
	//##ModelId=49B87B87029F
	int FillListData();

/*
 *  @brief   	AddEventToList	 把一条事件的信息加入到列表 
 *  @param 		[In]a param of Type  CXJEventAction *  要加入的事件
 *  @param 		[In]a param of Type  int  加入的位置
 *  @return 	BOOL	加入成功返回TRUE,失败返回FALSE 
 */
	//##ModelId=49B87B8702EF
	BOOL AddEventToList(CXJEventAction * pEvent, int i);

/*
*  @brief   	InitListCtrl	 初始化列表 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	int InitListCtrl();

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
 *  @brief   	FindNetPos	 查找地区在树中的位置 
 *  @param 		[In]a param of Type  CString  地区ID
 *  @return 	HTREEITEM 树节点结构
 */
	//##ModelId=49B87BA403BB
	HTREEITEM FindNetPos(CString sNetID);

	/*
 *  @brief   	InsertDevice	 加入设备到设备树 
 *  @param 		[In]a param of Type  HTREEITEM  指定父节点
 *	@param		[In]a param of Type	 CDeviceObj* 指定设备
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB1002E
	BOOL InsertDevice(HTREEITEM htParent, CDeviceObj* pObj);
/*
*  @brief   	InitDeviceTree	 初始化设备树 
*  @return 	int		失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87BA403B4
	int InitDeviceTree();

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
 *  @brief   	FillTree	 填充设备树 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA403CA
	BOOL FillTree();
	
	void BuildGPS( CStationObj* pStation, HTREEITEM htParent );
	
	BOOL QueryActionEvent();
	
	/*************************************************************
	函 数 名: ClearCheck()
	功能概要: 清除所有选择
	返 回 值: 
	参    数: param1 
	Param2 
	**************************************************************/
	void ClearCheck(HTREEITEM ht);
	
	
	/*************************************************************
	函 数 名：SetCheck_XJ()
	功能概要：设置指定节点的选择状态
	返 回 值: void
	参    数：param1	指定节点
	Param2	TRUE-选中. FALSE-取消选择
	**************************************************************/
	//##ModelId=49B87BB100CD
	void SetCheck_XJ( HTREEITEM hit, BOOL bCheck );
	
	/*************************************************************
	函 数 名：CycTreeDown()
	功能概要：向下遍历指定节点的所有子节点, 对所有子节点做相同的选择操作
	返 回 值: void
	参    数：param1	指定节点
	Param2	选择标志.TRUE为选中, FALSE为取消选择
	**************************************************************/
	//##ModelId=49B87BB100BB
	void CycTreeDown( HTREEITEM hRoot, BOOL bMark );
	void AddOrRemoveSecObj( HTREEITEM hRoot, BOOL bMark );
	
	/*************************************************************
	函 数 名：CycTreeUp()
	功能概要：向上遍历节点的父节点, 如果当前节点是父节点的最后一个被选择的子节点, 为父节点也加上选择标志
	返 回 值: void
	参    数：param1	指定节点
	Param2
	**************************************************************/
	void CycTreeUp( HTREEITEM hCurrent, BOOL bCheck );


/*
 *  @brief   	BreakAction	 分解动作事件为故障事件 
 *  @param 		[In]a param of Type  CXJEventAction* 指定动作事件
 *  @return 	void 
 */
	//##ModelId=49B87BA401A6
	void BreakAction(CXJEventAction* pEvent);

	void OnFaultCreate();
	
/*
 *  @brief   	CreateFaultReport	 新建故障包 
 *  @param 		[In]a param of Type  int  包ID
 *  @return 	CFaultReport* 新创建的故障包
 */
	CFaultReport* CreateFaultReport(int nID);

/*
 *  @brief   	SaveEvent	 保存被选择的事件到故障包 
 *  @return 	void 
 */
	//##ModelId=49B87BA401D9
	void SaveEvent(CFaultReport* pReport);

/*
 *  @brief   	QueryLatsetID	 查询最新的故障包ID 
 *  @return 	int 最新的故障包ID
 */
	int QueryLatsetID();


/*
 *  @brief   	AddReportToDB	 将故障包加入到数据库,事件不处理
 *  @param 		[In]a param of Type  CFaultReport*  指定故障包
 *  @return 	BOOL 加入成功返回TRUE,失败返回FALSE
 */
	BOOL AddReportToDB(CFaultReport* pReport);

/*
 *  @brief   	MultiSaveEvent	 批量保存事件 
 *  @return 	int 保存成功0,删除数据失败返回1,保存数据失败返回2
 */
	int MultiSaveEvent();

/*
 *  @brief   	ClearEvent	 删除所有事件 
 *  @return 	void 
 */
	//##ModelId=49B87BA40196
	void ClearEvent();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualFaultShorten)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualFaultShorten)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnPtosclistDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALFAULTSHORTEN_H__7FD5D525_F84A_4A6D_87B9_34A758D29738__INCLUDED_)
