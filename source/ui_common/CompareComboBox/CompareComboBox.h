#ifndef _COMPARECOMBOBOX_H_
#define	_COMPARECOMBOBOX_H_

#include <afxwin.h>
#include <afxtempl.h>


typedef struct CMBDATA_TYP
{
	CString	name;
	DWORD	data;
	CString	letter;//ƴ����ĸ
}CMBDATA;

class CCompareCmb : public CComboBox
{
public:
	CCompareCmb();
	DECLARE_DYNCREATE(CCompareCmb)
public:
	// Operations

	/*
	* @brief	OnDataFilled	ʼ���������Ϻ���ô˺����������ѯ����Ϊ�պ������������
	* @param	[In]a param of Type BOOL bAutoSelect �������ʱ�Ƿ��Զ�ѡ��ָ����		
	* @param	[In]a param of Type int index	ָ���Զ�ѡ�񵽵��-1��ʾ���һ����ڵ���0��ֵ��ֱ�Ӷ�Ӧ�����������	
	* @return   void	
	*/
	void OnDataFilled(BOOL bAutoSelect = TRUE, int index = -1);

private:
	CList<CMBDATA, CMBDATA> m_datas;
	BOOL m_bAutoSelect;
	int	m_nSelectIndex;

	void ReFillAllData();
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompareCmb)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCompareCmb();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCompareCmb)
	afx_msg void OnEditchange();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif
