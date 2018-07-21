#if !defined(AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_)
#define AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddOsc.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc dialog

class CDlgAddOsc : public CDialog
{
// Construction
public:
	CDlgAddOsc(CWnd* pParent = NULL);   // standard constructor
	~CDlgAddOsc();

	/** @brief           对应的故障归档*/
	CFaultReport*	m_pReport;
	/** @brief           录波文件列表*/
	CPtrArray		m_listOSC;
	/** @brief           记录之前被排序的列*/
	//##ModelId=49B87B8F01F5
	int		m_nOldSortCol;
// Dialog Data
	//{{AFX_DATA(CDlgAddOsc)
	enum { IDD = IDD_DLG_ADDOSC };
	CCompareCmb	m_cmbSec;
	CCompareCmb	m_cmbStation;
	CListCtrl	m_List;
	CString	m_sPath;
	int		m_nMS;
	CString m_sDir;
	CString m_sOldDir;
	int     m_nFileSize;
	//}}AFX_DATA

private:
/*
 *  @brief   	FillCMBStation	 填充厂站下拉框数据 
 *  @return 	void 
 */
	void FillCMBStation();

/*
 *  @brief   	FillCMBSec	 填充指定厂站的二次设备数据到下拉框
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	void FillCMBSec(CStationObj* pStation);

/*
 *  @brief   	InitList	 初始化列表 
 *  @return 	void 
 */
	void InitList();

/*
 *  @brief   	AddEventToList	 添加指定事件到列表 
 *  @param 		[In]a param of Type  CFaultEvent*  指定事件
 *  @param 		[In]a param of Type  int  添加到列表的位置
 *  @return 	BOOL 添加成功返回TRUE,失败返回FALSE
 */
	BOOL AddEventToList(CFaultEvent* pEvent, int nIndex);

/*
 *  @brief   	IsExist	 判断指定文件是否已在待加入列表或故障归档的录波列表中 
 *  @param 		[In]a param of Type  CString  指定文件的名称，不带后缀
 *  @return 	BOOL 已存在返回TRUE,不存在返回FALSE
 */
	BOOL IsExist(CString sName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddOsc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
			/*
 *  @brief   	CompareFunction	 比较函数,排序时被调用 
 *  @param 		[In]a param of Type  LPARAM  比较对象1
 *  @param 		[In]a param of Type  LPARAM  比较对象2
 *  @param 		[In]a param of Type  LPARAM  比较参数
 *  @return 	static int CALLBACK 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 */
	//##ModelId=49B87B820247
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddOsc)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddoscSkin();
	afx_msg void OnBtnAddoscAdd();
	afx_msg void OnBtnAddoscRemove();
	afx_msg void OnBtnAddoscModify();
	afx_msg void OnItemchangedListAddosc(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelchangeCmbAddoscStation();
	afx_msg void OnColumnclickListAddosc(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDOSC_H__876C995F_86B2_44CD_81FB_F20691708DF5__INCLUDED_)
