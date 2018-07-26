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
	/** @brief			����С*/
	CSize	m_szPane;
	/** @brief			����ڸ����ڵ�λ��*/
	CPoint	m_ptLeftTop;
	/** @brief			�������ɫ*/
	COLORREF	m_colFill;

	/** @brief			��ʾ����*/
	CString	m_sText;
	/** @brief			����*/
	CFont	m_ftFont;
	/** @brief			������ɫ*/
	COLORREF m_colText;

	/** @brief			����״̬*/
	int		m_nState;
	/** @brief			����״̬ö��*/
	enum FocusState{
		FOCUS_NONE	= 0, //δ��ý���
		FOCUS_ON	= 1  //����ý���
	};

	CImageList	m_ImageList;
	/** @brief			ͼ���С*/
	int m_nIconWidth;

	CString m_sDetailTitle;
	int		m_nDetailStatus;
	CString	m_sStatusMsg;

public:
	virtual void Draw(CDC* pDC);
	inline void SetState(int nState){m_nState = nState;};
	inline int GetState(){return m_nState;};
	/*
	* @brief	MouseMove	�ж�����Ƿ��ڱ�����Χ�У�����Ӧ�ĸı佹��״̬
	* @param	[In]a param of Type CPoint point		
	* @return   BOOL �����״̬�ı���Ҫ���»��Ʒ���TRUE,���򷵻�FALSE
	*/
	virtual BOOL MouseMove(CPoint point);
	/*
	* @brief	LButtonDown	�ж��Ƿ񱻵������������Ӧ����
	* @param	[In]a param of Type CPoint point		
	* @return   int ��Ҫ�ⲿ���ִ�еĲ���
	*/
	virtual int LButtonDown(CPoint point);
};

class CHangoutTitlePane:public CHangoutWndPane
{
public:
	CHangoutTitlePane(CPoint pt, CSize sz);
	virtual ~CHangoutTitlePane();
private:
	/** @brief			�Ƿ�MINI״̬*/
	BOOL m_bIsMini;
public:
	virtual void Draw(CDC* pDC);
	inline void SetMini(BOOL bMini){m_bIsMini = bMini;};
	inline BOOL IsMini(){return m_bIsMini;};
	/*
	* @brief	LButtonDown	�ж��Ƿ񱻵������������Ӧ����
	* @param	[In]a param of Type CPoint point		
	* @return   int ��Ҫ�ⲿ���ִ�еĲ���
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
	* @brief	LButtonDown	�ж��Ƿ񱻵������������Ӧ����
	* @param	[In]a param of Type CPoint point		
	* @return   int ��Ҫ�ⲿ���ִ�еĲ���
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
	/** @brief			�������Ͻ�λ��*/
	CPoint m_ptWnd;
	/** @brief			���ڴ�С*/
	CSize	m_szWnd;

	/** @brief			�������*/
	CHangoutTitlePane*	m_pTitlePane;
	/** @brief			�������*/
	CPtrArray	m_arrPane;

	/** @brief			��������С*/
	CSize	m_szTitle;
	/** @brief			��������С*/
	CSize	m_szPane;

	CPoint  m_pointPTSetModView;
	bool	m_bAlreadyShowOnce;

	int		m_nTimer;

	enum ClickOperator{
		OPERATOR_NONE = 0, //�������
		OPERATOR_MINI = 1, //��������ΪMINI״̬
		OPERATOR_MAX = 2, //������ȡ��MINI״̬
		OPERATOR_OPEN = 3 //����Ӧ����ϸ��Ϣ
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
