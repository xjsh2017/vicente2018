#if !defined(AFX_PTFRAME_H__F92351A9_CC94_4ABF_9762_6D7D800F027B__INCLUDED_)
#define AFX_PTFRAME_H__F92351A9_CC94_4ABF_9762_6D7D800F027B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTFrame.h : header file
//
#include "PTSheet.h"
#include "PTAction.h"
#include "PTAlarm.h"
#include "PTAnalog.h"
#include "PTDigital.h"
#include "PTGeneral.h"
#include "PTOSC.h"
#include "PTSetting.h"
#include "PTSoftBoard.h"
#include "PTOSCList.h"
#include "PTHistory.h"
#include "PTNote.h"

/////////////////////////////////////////////////////////////////////////////
// CPTFrame frame

//##ModelId=49B87B91033C
class CPTFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CPTFrame)
public:
	//##ModelId=49B87B91033E
	CPTFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	/** @brief           TAB视图窗口*/
	//##ModelId=49B87B91034C
	CPTSheet * m_pTabView;

	//记录各视图指针
	/** @brief           保护动作属性页视图指针*/
	//##ModelId=49B87B910351
	CPTAction*	m_pPTAction;
	/** @brief           保护告警属性页视图指针*/
	//##ModelId=49B87B910356
	CPTAlarm*	m_pPTAlarm;
	/** @brief           保护模拟量属性页视图指针*/
	//##ModelId=49B87B91035B
	CPTAnalog*	m_pPTAnalog;
	/** @brief           保护开关量属性页视图指针*/
	//##ModelId=49B87B910360
	CPTDigital*	m_pPTDigital;
	/** @brief           保护基本属性页视图指针*/
	//##ModelId=49B87B910365
	CPTGeneral*	m_pPTGeneral;
	/** @brief           保护录波属性页视图指针*/
	//##ModelId=49B87B91036A
	CPTOSC*		m_pPTOSC;
	/** @brief           保护定值属性页视图指针*/
	//##ModelId=49B87B91036F
	CPTSetting*	m_pPTSetting;
	/** @brief           保护软压板属性页视图指针*/
	//##ModelId=49B87B910374
	CPTSoftBoard*	m_pPTSoftBoard;
	/** @brief           保护远方录波属性页视图指针*/
	//##ModelId=49B87B910379
	CPTOSCList*	m_pPTOSCList;
	/** @brief           历史事件属性页视图指针*/
	//##ModelId=49B87B91037E
	CPTHistory*	m_pPTHistory;
	CPTNote*	m_pPTNote;

	BOOL        m_bSetBar;

	CMap<int, int, int, int>	m_mapIndex;

	/** @brief			记录当前显示的保护装置*/
	CSecObj*		m_pCurSec;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTFrame)
	public:
	//##ModelId=49B87B910382
	virtual void ActivateFrame(int nCmdShow = -1);
	//##ModelId=49B87B910385
	virtual BOOL DestroyWindow();
	protected:
	//##ModelId=49B87B91038A
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//##ModelId=49B87B91038E
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//##ModelId=49B87B910399
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B91039B
	virtual ~CPTFrame();

	// Generated message map functions
	//{{AFX_MSG(CPTFrame)
	//##ModelId=49B87B91039D
	afx_msg void OnClose();
	//##ModelId=49B87B9103AA
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//##ModelId=49B87B9103AF
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//##ModelId=49B87B9103BD
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	//##ModelId=49B87B9103C9
	afx_msg void OnSTTP20008(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9103CD
	afx_msg void OnSTTP20012(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9103DB
	afx_msg void OnSTTP20016(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920001
	afx_msg void OnSTTP20048(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20170(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92000F
	afx_msg void OnSTTP20052(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920013
	afx_msg void OnSTTP20054(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920021
	afx_msg void OnSTTP20056(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20172(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92002F
	afx_msg void OnSTTP20058(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92003E
	afx_msg void OnSTTP20060(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920042
	afx_msg void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920050
	afx_msg void OnSTTP20107(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20174(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92005D
	afx_msg void OnSTTP20109(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920061
	afx_msg void OnSTTP20111(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92006E
	afx_msg void OnSTTP20114(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920072
	afx_msg void OnSTTP20118(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92007E
	afx_msg void OnSTTP20125(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920082
	afx_msg void OnSTTP20089(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B92008F
	afx_msg void OnSTTP20026(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B920093
	afx_msg void OnSTTP20043(WPARAM wParam, LPARAM lparam);
	//##ModelId=49B87B92009F
	afx_msg void OnSTTP20116(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200AB
	afx_msg void OnSTTP20157(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200AF
	afx_msg void OnSTTP00901(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20004(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200BB
	afx_msg void OnPTFrameOpen(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200BF
	afx_msg void OnEventPropAckPT(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200CC
	afx_msg void OnEventPropAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200DA
	afx_msg void OnEventListAck(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200DE
	afx_msg OnAllClose(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200EA
	afx_msg void OnRefreshPTSetting(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200EE
	afx_msg void OnRefreshPTDI(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B9200FB
	afx_msg void OnRefreshPTSoft(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTFRAME_H__F92351A9_CC94_4ABF_9762_6D7D800F027B__INCLUDED_)
