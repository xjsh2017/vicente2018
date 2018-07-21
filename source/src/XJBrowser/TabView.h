// ***************************************************************
//  TabView   version:  1.0    date: 8:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 此类主要实现带TAB标签切换的类似属性页对话框的属性页视图,
//					使用时请重载此类, 用子类进行实际工作
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_) 
#define AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_ 
#if _MSC_VER > 1000 
#pragma once 
#endif // _MSC_VER > 1000 
// TabView.h : header file 
// 
#ifdef _WIN32 
#define WC_TABVIEWA "SysTabControl32" 
#define WC_TABVIEWW L"SysTabControl32" 
#ifdef UNICODE 
#define WC_TABVIEW WC_TABVIEWW 
#else 
#define WC_TABVIEW WC_TABVIEWA 
#endif 
#else 
#define WC_TABVIEW "SysTabControl" 
#endif 
#include <afxtempl.h> 
///////////////////////////////////////////////////////////////////////////// 
// CTabView view 
//##ModelId=49B87B880128
class CTabView : public CCtrlView 
{ 
DECLARE_DYNCREATE(CTabView) 
// Construction 
public: 
	//##ModelId=49B87B880139
CTabView(); 
	//##ModelId=49B87B88013A
virtual ~CTabView(); 
// Attributes 
public: 
	//##ModelId=49B87B88013C
CTabCtrl& GetTabCtrl() const; 
protected: 
	//##ModelId=49B87B88013E
void RemoveImageList(); 
// Operations 
public: 
	//##ModelId=49B87B88013F
void HidePage(int nPage); 
	//##ModelId=49B87B880141
void RemoveView(int nItem);	 // Removes a view 
	//##ModelId=49B87B880149
void RemoveView(CView * pView); // Removes a view 
	//##ModelId=49B87B88014B
UINT GetPageID(int nPage);	 // Get the ID of the page 
	//##ModelId=49B87B88014D
void ActivateView(int nItem);	// Activates a view (tab) 
	//##ModelId=49B87B88014F
CView * GetView(int nView);	 // Return the correcponsing view 
// Insert a view at a given page 
	//##ModelId=49B87B880151
int Insert(int nPage, LPCTSTR lpszItemText, CRuntimeClass * pNewWnd, 
CCreateContext * pContext = NULL); 
// Insert a view as the last page 
	//##ModelId=49B87B880158
int Insert(LPCTSTR lpszItemText, CRuntimeClass *pNewWnd, 
CCreateContext *pContext = NULL); 
// Set the height of the tab font (9 is a good value) 
	//##ModelId=49B87B88015C
void SetFontHeight(int nHeight); 
// Shows a page 
// Overrides 
// ClassWizard generated virtual function overrides 
//{{AFX_VIRTUAL(CTabView) 
public: 
	//##ModelId=49B87B88015E
virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 
	//##ModelId=49B87B880161
virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, 
LRESULT* pLResult); 
	//##ModelId=49B87B88016B
virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo); 
protected: 
	//##ModelId=49B87B880171
virtual void PreSubclassWindow(); 
	//##ModelId=49B87B880173
virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint); 
//}}AFX_VIRTUAL 
// Implementation 
protected: 
	//##ModelId=49B87B88017B
CFont m_Font; 
	//##ModelId=49B87B880180
CMapWordToPtr m_aPages; 
	//##ModelId=49B87B880281
CMap<int, int, UINT, UINT> m_aPageIDs; 
	//##ModelId=49B87B880285
UINT m_uLastID; 
// Generated message map functions 
protected: 
//	CWnd * m_pDummyWnd; 
	//##ModelId=49B87B880291
CFrameWnd * m_pMainFrame; 
	//##ModelId=49B87B880295
void RecalcLayout(CWnd *pCurWnd = NULL);	// Fits the view into the client area of the tab 
	//##ModelId=49B87B88029F
UINT GetNewPageID(int nPage);	 // Gets a new available ID for a page 
//{{AFX_MSG(CTabView) 
	//##ModelId=49B87B8802A1
afx_msg void OnDestroy(); 
	//##ModelId=49B87B8802A3
afx_msg void OnSize(UINT nType, int cx, int cy); 
	//##ModelId=49B87B8802B1
afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult); 
	//##ModelId=49B87B8802B5
afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult); 
	//##ModelId=49B87B8802C0
afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct); 
	//##ModelId=49B87B8802C3
afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//##ModelId=49B87B8802CE
afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
//}}AFX_MSG 
DECLARE_MESSAGE_MAP() 
}; 
//##ModelId=49B87B880139
inline CTabView::CTabView() : CCtrlView(WC_TABVIEW, AFX_WS_DEFAULT_VIEW) { 
m_uLastID = AFX_IDW_PANE_FIRST + 128; // Leave room for 127 other views :-)
m_Font.CreateFont(
				10,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,                 // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				"宋体");                 // lpszFacename 
} 
//##ModelId=49B87B88013C
inline CTabCtrl& CTabView::GetTabCtrl() const 
{ return *(CTabCtrl*)this; } 
///////////////////////////////////////////////////////////////////////////// 
//{{AFX_INSERT_LOCATION}} 
// Microsoft Visual C++ will insert additional declarations immediately before the previous line. 
#endif // !defined(AFX_TABVIEW_H__207AF890_DCF0_42E0_81C4_A506BE71ABC7__INCLUDED_) 
