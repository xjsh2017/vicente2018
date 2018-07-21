#if !defined(AFX_WAVERECFRAME_H__92D3F03C_6E2C_4627_ABE0_AEA387CBBE98__INCLUDED_)
#define AFX_WAVERECFRAME_H__92D3F03C_6E2C_4627_ABE0_AEA387CBBE98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRecFrame.h : header file
//
#include "PTSheet.h"
#include "WaveRecGeneral.h"
#include "WaveRecSetting.h"
#include "WaveRecOSC.h"
#include "PTOSCList.h"
#include "PTDigital.h"
#include "PTAnalog.h"
#include "PTSetting.h"
#include "PTOSC.h"
#include "PTNote.h"


/////////////////////////////////////////////////////////////////////////////
// CWaveRecFrame frame
//##ModelId=49B87B810000
class CWaveRecFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWaveRecFrame)
public:
	//##ModelId=49B87B810002
	CWaveRecFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	/** @brief           SHEET页面指针*/
	//##ModelId=49B87B810004
	CPTSheet *		m_pTabView;
	/** @brief           录波器基本属性页视图指针*/
	//##ModelId=49B87B810009
	CWaveRecGeneral*	m_pWRGeneral;
	/** @brief           录波器录波属性页视图指针*/
	//##ModelId=49B87B810010
	CPTOSC*		m_pWROSC;
	/** @brief           录波器定值属性页视图指针*/
	//##ModelId=49B87B810015
	CPTSetting*	m_pWRSetting;
	/** @brief           录波器录波列表视图指针*/
	//##ModelId=49B87B810023
	CPTOSCList*			m_pWROSCList;
	/** @brief           录波器开关量属性页视图指针*/
	CPTDigital*			m_pWRDigital;
	/** @brief           录波器模拟量属性页视图指针*/
	CPTAnalog*			m_pWRAnalog;
	CPTNote*			m_pWRNote;
	BOOL                m_bSetMenu;

	CMap<int, int, int, int>	m_mapIndex;
	
	CSecObj*			m_pCurSec;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveRecFrame)
	public:
	//##ModelId=49B87B810027
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	//##ModelId=49B87B810030
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//##ModelId=49B87B810033
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B81003F
	virtual ~CWaveRecFrame();

	// Generated message map functions
	//{{AFX_MSG(CWaveRecFrame)
	//##ModelId=49B87B810041
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87B810046
	afx_msg void OnClose();
	//}}AFX_MSG
	//##ModelId=49B87B81004F
	void OnSTTP20016(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810052
	void OnSTTP20060(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810055
	void OnSTTP20124(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B81005E
	void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810061
	void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B81006E
	void OnSTTP20026(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810071
	void OnSTTP20043(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B81007D
	void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	void OnSTTP20004(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810080
	void OnOpenWRProp(WPARAM wParam, LPARAM lParam);
	void OnSTTP20012(WPARAM wParam,LPARAM lParam);
	void OnSTTP20008(WPARAM wParam, LPARAM lParam);
	void OnSTTP20086(WPARAM wParam, LPARAM lParam);
	void OnSTTP20107(WPARAM wParam, LPARAM lParam);
	void OnSTTP20048(WPARAM wParam, LPARAM lParam);
	void OnSTTP20052(WPARAM wParam, LPARAM lParam);
	void OnSTTP20054(WPARAM wParam, LPARAM lParam);
	void OnSTTP20056(WPARAM wParam, LPARAM lParam);
	void OnSTTP20170(WPARAM wParam, LPARAM lParam);
	void OnSTTP20172(WPARAM wParam, LPARAM lParam);
	void OnSTTP20174(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B810083
	afx_msg OnAllClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B81008D
	afx_msg OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERECFRAME_H__92D3F03C_6E2C_4627_ABE0_AEA387CBBE98__INCLUDED_)
