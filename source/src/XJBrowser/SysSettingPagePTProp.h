#if !defined(AFX_SYSSETTINGPAGEPTPROP_H__528C88B0_D4DE_4533_BD04_2122CFA579EA__INCLUDED_)
#define AFX_SYSSETTINGPAGEPTPROP_H__528C88B0_D4DE_4533_BD04_2122CFA579EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettingPagePTProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPagePTProp dialog

//##ModelId=49B87B88035B
class CSysSettingPagePTProp : public CPropertyPage
{
	DECLARE_DYNCREATE(CSysSettingPagePTProp)

// Construction
public:
	//##ModelId=49B87B88035D
	CSysSettingPagePTProp();
	//##ModelId=49B87B88035E
	~CSysSettingPagePTProp();

// Dialog Data
	//{{AFX_DATA(CSysSettingPagePTProp)
	enum { IDD = IDD_PAGE_SYS_PTPROP };
	//##ModelId=49B87B88035F
	BOOL	m_bSettingGroup;
	//##ModelId=49B87B880360
	BOOL	m_bPTAI;
	//}}AFX_DATA
	/** @brief           定值项数下拉框选择的索引*/
	//##ModelId=49B87B88036B
	int		m_nSelSettingValues;
	/** @brief           开关量项数下拉框选择的索引*/
	//##ModelId=49B87B88036C
	int		m_nSelDIValues;
	/** @brief           开关量值显示风格下拉框选择的索引*/
	//##ModelId=49B87B88036D
	int		m_nSelDIStyle;
	/** @brief           软压板项数下拉框选择的索引*/
	//##ModelId=49B87B88036E
	int		m_nSelSoftValues;
	/** @brief           软压板值显示风格下拉框选择的索引*/
	//##ModelId=49B87B88036F
	int		m_nSelSoftStyle;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSysSettingPagePTProp)
	protected:
	//##ModelId=49B87B880370
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSysSettingPagePTProp)
	//##ModelId=49B87B88037C
	virtual BOOL OnInitDialog();
	//##ModelId=49B87B88037E
	afx_msg void OnSelchangeCmbDiNum();
	//##ModelId=49B87B880380
	afx_msg void OnSelchangeCmbDiValue();
	//##ModelId=49B87B880382
	afx_msg void OnSelchangeCmbSettingNum();
	//##ModelId=49B87B880384
	afx_msg void OnSelchangeCmbSoftNum();
	//##ModelId=49B87B88038B
	afx_msg void OnSelchangeCmbSoftValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGPAGEPTPROP_H__528C88B0_D4DE_4533_BD04_2122CFA579EA__INCLUDED_)
