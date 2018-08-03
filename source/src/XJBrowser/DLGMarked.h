#if !defined(AFX_DLGMARKED_H__DE5A0E73_208F_4C1A_8015_B0BE2F9940AB__INCLUDED_)
#define AFX_DLGMARKED_H__DE5A0E73_208F_4C1A_8015_B0BE2F9940AB__INCLUDED_

#include "..\..\COMMON\xjlib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGMarked.h : header file
//
//DLGMarked	���ƶԻ���
/**
 * @defgroup 	DLGMarked	���ƶԻ�������
 * @{
 */
/**
 * @brief       ���������豸����
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
	//��Ӧ���豸����
	CSecObj*	m_pObj;
	//�����ж�
	BOOL		m_bMark;
		/*
 *  @brief   	InitComBox	 �ȶ�ԭ��
 *  @return 	void
	*/
	BOOL Comparison(CString sMarkInfo);
		/*
 *  @brief   	InitComBox	 ���combox������
 *  @return 	void
	*/
	void InitComBox();
	/*
 *  @brief   	SetDeviceState	 ���ù���״̬
 *  @return 	void
	*/
	void SetDeviceState();
	/*
 *  @brief   	InsertDBMark	 �������ݿ��װ�ù�����Ϣ
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL InsertDBMark();

	/*
	*  @brief   	CheckStateBeforeMark	 ��״̬������Ƿ��Ѵ��ڹ���װ�� 
	*  @return 	BOOL ���ڷ���TRUE, ���򷵻�FALSE
	*/
	BOOL CheckStateBeforeMark();

	/*
	*  @brief   	CheckStateBeforeUnMark	 ȡ������ǰ���״̬�� 
	*  @return 	BOOL ���ڷ���TRUE, ���򷵻�FALSE
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
