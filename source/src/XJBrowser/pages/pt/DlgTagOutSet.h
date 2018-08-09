#ifndef _DLG_TAGOUT_H
#define _DLG_TAGOUT_H

#include "resource.h"
// DlgTagOutSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTagOutSet ���ƶԻ���

/**
 * @defgroup 	CDlgTagOutSet	���ƶԻ�������
 * @{
 */
/**
 * @brief       ��������
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
	*  @brief   	CanMark	 ��״̬������Ƿ��Ѵ��ڹ���װ�� 
	*  @return 	BOOL ���ڷ���TRUE, ���򷵻�FALSE
	*/
	BOOL CanMark();

	/*
	*  @brief   	CanUnMark	 ȡ������ǰ���״̬�� 
	*  @return 	BOOL ���ڷ���TRUE, ���򷵻�FALSE
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
	*  @brief   	InitListStyle	 ��ʼ���б���ʽ 
	*  @return 	int ʧ�ܷ���-1, �ɹ�����>=0
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
