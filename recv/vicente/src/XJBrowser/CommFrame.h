// ***************************************************************
//  CommFrame   version:  1.0    date: 5:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ����վͨ��״̬�Ӵ��ڿ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_COMMFRAME_H__AE1D4F50_6676_4E2C_A573_8A2BD8F1F575__INCLUDED_)
#define AFX_COMMFRAME_H__AE1D4F50_6676_4E2C_A573_8A2BD8F1F575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommFrame.h : header file
//
#include "ViewComm.h"
#include "ViewCommMap.h"

/////////////////////////////////////////////////////////////////////////////
// CCommFrame frame

/**
 * @defgroup 	CCommFrame	����վͨѶ״̬������
 * @{
 */
/**
 * @brief       ����վͨѶ״̬���ڿ��
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BAA000F
class CCommFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCommFrame)
protected:
	//##ModelId=49B87BAA0011
	CCommFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	/** @brief           ��������վ״̬�б���ͼָ��*/
	//##ModelId=49B87BAA0013
	CViewComm* m_pComm;
	/** @brief           ��������վ״̬ͼ��ͼָ��*/
	//##ModelId=49B87BAA0018
	CViewCommMap* m_pCommMap;
protected:
	/** @brief           �ָ�����*/
	//##ModelId=49B87BAA0020
	CSplitterWnd m_wndSplitter;
	/** @brief           �����Ƿ��Ѿ������ʾ*/
	//##ModelId=49B87BAA0024
	BOOL m_bMaximized;
	/** @brief           ֮ǰ�Ĵ��ڸ߶�*/
	int				m_nOldHeight;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BAA0035
	virtual ~CCommFrame();

	// Generated message map functions
	//{{AFX_MSG(CCommFrame)
	//##ModelId=49B87BAA003E
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	//##ModelId=49B87BAA0041
	void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	void OnSTTP20146(WPARAM wParam, LPARAM lParam);
	void OnSTTP20069(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87BAA0044
	void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTestStationStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRefreshCommMap(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMFRAME_H__AE1D4F50_6676_4E2C_A573_8A2BD8F1F575__INCLUDED_)
