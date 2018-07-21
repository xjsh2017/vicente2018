/****************************************************************************\
Datei  : MDITabs.h
Projekt: MDITabs, a tabcontrol for switching between MDI-views
Inhalt : EMDITabStyles, CMDITabs declaration
Datum  : 03.10.2001
Autor  : Christian Rodemeyer
Hinweis: © 2001 by Christian Rodemeyer
\****************************************************************************/
#pragma once

#ifndef __MDITABS_H
#define __MDITABS_H

/****************************************************************************\
 EMDITabStyles: Styles for customizing the appeareance of CMDITabs
\****************************************************************************/
//##ModelId=49B87B97001F
enum EMDITabStyles
{
	//##ModelId=49B87B970021
  MT_BOTTOM        = 0x0000, // places tabs at bottom (default)
	//##ModelId=49B87B970022
  MT_TOP           = 0x0001, // place tabs at top
	//##ModelId=49B87B970023
  MT_IMAGES        = 0x0002, // show images
	//##ModelId=49B87B970024
  MT_HIDEWLT2VIEWS = 0x0004, // Hide Tabs when less than two views are open (default is one view)
	//##ModelId=49B87B97002E
  MT_TOOLTIPS      = 0x0008, // not implemented (a tooltip can appear about a tab) 
	//##ModelId=49B87B97002F
  MT_BUTTONS       = 0x0010, // not implemented (show tabs as buttons)
	//##ModelId=49B87B970030
  MT_AUTOSIZE      = 0x0020, // not implemented (tabs are sized to fit the entire width of the control)
	//##ModelId=49B87B970031
  MT_TASKBAR       = 0x0038  // MT_TOOLTIPS|MT_BUTTONS|MT_AUTOSIZE
};

//##ModelId=49B87B97003E
enum EWMMTGetInfo
{
	//##ModelId=49B87B970040
  WM_GETTABTIPTEXT = WM_APP + 0x0393, // send to associated view to get a tip text
  WM_GETTABSYSMENU = WM_APP + 0x0394  // if neccessary, the view can provide a different systemmenu by answering this message
};

//ÒÔÏÂÊÇtab¿Ø¼þ
#define IDD_ABOUTBOX                    100
#define IDR_MAINFRAME                   128
#define IDR_CUSTOMTYPE                  129
#define IDD_PROPERTIES                  131
#define IDB_BITMAP1                     132
#define IDC_CURSORMOVE                  133
#define IDC_LABELED                     WM_APP +1000
#define IDC_URLED                       WM_APP +1001
#define IDC_TOOLTIPED                   WM_APP +1002
#define ID_INCHEIGHT                    WM_APP +32771
#define ID_DECHEIGHT                    WM_APP +32772
#define ID_LEFT                         WM_APP +32775
#define ID_RIGHT                        WM_APP +32776
#define ID_TOP                          WM_APP +32777
#define ID_BOTTOM                       WM_APP +32778

#define WM_SETCURSELMSG	(WM_USER + 1)

/****************************************************************************\
 CMdiTabs: Deklaration
\****************************************************************************/
//##ModelId=49B87B97004E
class CMDITabs : public CTabCtrl
{
public:
	//##ModelId=49B87B970050
  CMDITabs();

	//##ModelId=49B87B970051
  void Create(CFrameWnd* pMainFrame, DWORD dwStyle = MT_BOTTOM|MT_IMAGES);
	//##ModelId=49B87B970054
  void Update(); // sync the tabctrl with all views

	//##ModelId=49B87B970055
  void SetMinViews(int minViews) {m_minViews = minViews;}

private:
	//##ModelId=49B87B97005E
  HWND       m_mdiClient;
	//##ModelId=49B87B97005F
  int        m_height;
	//##ModelId=49B87B970060
  int        m_width;
	//##ModelId=49B87B970062
  CImageList m_images;
	//##ModelId=49B87B97006F
  int        m_minViews; // minimum number of views 
	//##ModelId=49B87B970070
  bool       m_bImages;
	//##ModelId=49B87B97007D
  bool       m_bTop;

public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMDITabs)
  //}}AFX_VIRTUAL

// Implementation
public:

  // Generated message map functions
protected:
  //{{AFX_MSG(CMDITabs)
	//##ModelId=49B87B97007E
  afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//##ModelId=49B87B970082
  afx_msg void OnPaint();
	//##ModelId=49B87B97008C
  afx_msg void OnNcPaint();
	//##ModelId=49B87B97008E
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//##ModelId=49B87B970092
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	//##ModelId=49B87B97009C
  afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);

  DECLARE_MESSAGE_MAP()
};

#endif
