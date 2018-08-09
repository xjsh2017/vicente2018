#ifndef _DLG_TAGOUT_H
#define _DLG_TAGOUT_H

#include "resource.h"
// DlgTagOutSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTagOutSet 挂牌对话框

/**
 * @defgroup 	CDlgTagOutSet	挂牌对话框设置
 * @{
 */
/**
 * @brief       挂牌设置
*  @author      WPS
 * @version     
 * @date        2018/08/02
*/

/////////////////////////////////////////////////////////////////////////////
// CDlgTagOutSet dialog

class CSecObj;
class CDlgTagOutSet : public CDialog
{
// Construction
public:
	//对应的设备对象
	CSecObj*	m_pObj;
	//挂牌判断
	BOOL		m_bMark;
		/*
 *  @brief   	InitComBox	 比对原因
 *  @return 	void
	*/
	BOOL Comparison(CString sMarkInfo);
		/*
 *  @brief   	InitComBox	 填充combox下拉框
 *  @return 	void
	*/
	void InitComBox();
	/*
 *  @brief   	SetDeviceState	 设置挂牌状态
 *  @return 	void
	*/
	void SetDeviceState();
	/*
 *  @brief   	InsertDBMark	 更新数据库的装置挂牌信息
 *  @return 	BOOL 建立成功返回TRUE, 失败返回FALSE
 */
	BOOL InsertDBMark();

	/*
	*  @brief   	CanMark	 从状态机检查是否已存在挂牌装置 
	*  @return 	BOOL 存在返回TRUE, 否则返回FALSE
	*/
	BOOL CanMark();

	/*
	*  @brief   	CanUnMark	 取消挂牌前检查状态机 
	*  @return 	BOOL 存在返回TRUE, 否则返回FALSE
	*/
	BOOL CanUnMark();

	CDlgTagOutSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTagOutSet)
	enum { IDD = IDD_DLG_TAGOUT };
	CComboBox	m_cmbMarkReason;
	CListCtrlEx	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTagOutSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	/*
	*  @brief   	InitListStyle	 初始化列表样式 
	*  @return 	int 失败返回-1, 成功返回>=0
	*/
	//##ModelId=49B87B8C009C
	int	InitListStyle();

	void FillData();

	BOOL CheckFlow();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTagOutSet)
	afx_msg void OnBtnMark();
	afx_msg void OnBtnUnmark();
	afx_msg void OnApplySetting();
	afx_msg void OnSelchangeCmbMarkreason();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_DLG_TAGOUT_H)
