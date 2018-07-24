#if !defined(AFX_HANGOUTWND_H__3C2943E7_E7B0_4AAC_96E8_607575A5EB0F__INCLUDED_)
#define AFX_HANGOUTWND_H__3C2943E7_E7B0_4AAC_96E8_607575A5EB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommWnd.h : header file
//

class CHangoutWndPane
{
public:
	CHangoutWndPane(CPoint pt, CSize sz);
	virtual ~CHangoutWndPane();
public:
	/** @brief			面板大小*/
	CSize	m_szPane;
	/** @brief			相对于父窗口的位置*/
	CPoint	m_ptLeftTop;
	/** @brief			背景填充色*/
	COLORREF	m_colFill;

	/** @brief			显示文字*/
	CString	m_sText;
	/** @brief			字体*/
	CFont	m_ftFont;
	/** @brief			文字颜色*/
	COLORREF m_colText;

	/** @brief			焦点状态*/
	int		m_nState;
	/** @brief			焦点状态枚举*/
	enum FocusState{
		FOCUS_NONE	= 0, //未获得焦点
		FOCUS_ON	= 1  //正获得焦点
	};

	CImageList	m_ImageList;
	/** @brief			图标大小*/
	int m_nIconWidth;

	CString m_sDetailTitle;
	int		m_nDetailStatus;
	CString	m_sStatusMsg;

public:
	virtual void Draw(CDC* pDC);
	inline void SetState(int nState){m_nState = nState;};
	inline int GetState(){return m_nState;};
	/*
	* @brief	MouseMove	判断鼠标是否在本对象范围中，并相应的改变焦点状态
	* @param	[In]a param of Type CPoint point		
	* @return   BOOL 如果有状态改变需要重新绘制返回TRUE,否则返回FALSE
	*/
	virtual BOOL MouseMove(CPoint point);
	/*
	* @brief	LButtonDown	判断是否被点击，并作出相应操作
	* @param	[In]a param of Type CPoint point		
	* @return   int 需要外部框架执行的操作
	*/
	virtual int LButtonDown(CPoint point);
};

class CHangoutTitlePane:public CHangoutWndPane
{
public:
	CHangoutTitlePane(CPoint pt, CSize sz);
	virtual ~CHangoutTitlePane();
private:
	/** @brief			是否MINI状态*/
	BOOL m_bIsMini;
public:
	virtual void Draw(CDC* pDC);
	inline void SetMini(BOOL bMini){m_bIsMini = bMini;};
	inline BOOL IsMini(){return m_bIsMini;};
	/*
	* @brief	LButtonDown	判断是否被点击，并作出相应操作
	* @param	[In]a param of Type CPoint point		
	* @return   int 需要外部框架执行的操作
	*/
	virtual int LButtonDown(CPoint point);
};

class CHangoutDescPane:public CHangoutWndPane
{
public:
	CHangoutDescPane(CPoint pt, CSize sz);
	virtual ~CHangoutDescPane();
public:
	virtual void Draw(CDC* pDC);
		/*
	* @brief	LButtonDown	判断是否被点击，并作出相应操作
	* @param	[In]a param of Type CPoint point		
	* @return   int 需要外部框架执行的操作
	*/
	virtual int LButtonDown(CPoint point);
};


/////////////////////////////////////////////////////////////////////////////
// CHangoutWnd window

class CHangoutWnd : public CWnd
{
DECLARE_DYNAMIC(CHangoutWnd)
// Construction
public:
	CHangoutWnd();

// Attributes
public:
	/** @brief			窗口左上角位置*/
	CPoint m_ptWnd;
	/** @brief			窗口大小*/
	CSize	m_szWnd;

	/** @brief			标题面板*/
	CHangoutTitlePane*	m_pTitlePane;
	/** @brief			保存面板*/
	CPtrArray	m_arrPane;

	/** @brief			标题面板大小*/
	CSize	m_szTitle;
	/** @brief			其他面板大小*/
	CSize	m_szPane;

	CPoint  m_pointPTSetModView;
	bool	m_bAlreadyShowOnce;

	int		m_nTimer;

	enum ClickOperator{
		OPERATOR_NONE = 0, //无需操作
		OPERATOR_MINI = 1, //将窗口设为MINI状态
		OPERATOR_MAX = 2, //将窗口取消MINI状态
		OPERATOR_OPEN = 3 //打开相应的详细信息
	};
private:
	BOOL m_bMouseTracking;
	CStationObj* m_pStation;
	BOOL m_bMouseDown;
	CPoint m_ptMouseDown;

// Operations
public:
	BOOL CreateWnd(CWnd* pParent, CPoint pt, CSize sz);
	void Show();
	void Show(CPoint pt);
	void SetStation(CStationObj* pStation);

private:
	void DoMouseEvent( CPoint point );
	void MiniWnd(BOOL bMini);
	void ShowChannelDetail(int nChannel);
	void RefreshData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHangoutWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHangoutWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHangoutWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANGOUTWND_H__3C2943E7_E7B0_4AAC_96E8_607575A5EB0F__INCLUDED_)
