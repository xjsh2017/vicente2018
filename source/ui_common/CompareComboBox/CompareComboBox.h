#ifndef _COMPARECOMBOBOX_H_
#define	_COMPARECOMBOBOX_H_

#include <afxwin.h>
#include <afxtempl.h>


typedef struct CMBDATA_TYP
{
	CString	name;
	DWORD	data;
	CString	letter;//拼音字母
}CMBDATA;

class CCompareCmb : public CComboBox
{
public:
	CCompareCmb();
	DECLARE_DYNCREATE(CCompareCmb)
public:
	// Operations

	/*
	* @brief	OnDataFilled	始数据填充完毕后调用此函数，方便查询条件为空后重新填充数据
	* @param	[In]a param of Type BOOL bAutoSelect 重新填充时是否自动选择到指定项		
	* @param	[In]a param of Type int index	指定自动选择到的项。-1表示最后一项，大于等于0的值则直接对应下拉框的索引	
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
