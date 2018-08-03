#if !defined(AFX_DLGMARKED_H__DE5A0E73_208F_4C1A_8015_B0BE2F9940AB__INCLUDED_)
#define AFX_DLGMARKED_H__DE5A0E73_208F_4C1A_8015_B0BE2F9940AB__INCLUDED_

#include "..\..\COMMON\xjlib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGMarked.h : header file
//
//DLGMarked	挂牌对话框
/**
 * @defgroup 	DLGMarked	挂牌对话框设置
 * @{
 */
/**
 * @brief       描述二次设备属性
*  @author      LY
 * @version     
 * @date        06/11/12
*/

/////////////////////////////////////////////////////////////////////////////
// DLGMarked dialog

class DLGMarked : public CDialog
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
	*  @brief   	CheckStateBeforeMark	 从状态机检查是否已存在挂牌装置 
	*  @return 	BOOL 存在返回TRUE, 否则返回FALSE
	*/
	BOOL CheckStateBeforeMark();

	/*
	*  @brief   	CheckStateBeforeUnMark	 取消挂牌前检查状态机 
	*  @return 	BOOL 存在返回TRUE, 否则返回FALSE
	*/
	BOOL CheckStateBeforeUnMark();

	DLGMarked(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DLGMarked)
	enum { IDD = IDD_DLG_MARKED };
	CComboBox	m_cmbMarkReason;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLGMarked)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLGMarked)
	afx_msg void OnBtnMark();
	afx_msg void OnBtnUnmark();
	afx_msg void OnSelchangeCmbMarkreason();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMARKED_H__DE5A0E73_208F_4C1A_8015_B0BE2F9940AB__INCLUDED_)
