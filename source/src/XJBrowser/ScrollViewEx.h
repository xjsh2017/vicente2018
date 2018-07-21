// uizoom.h : header file
//				  Implements Zooming functions in a CScrollView window
//
// Written by Zhang Peichao
// Copyright 1998, SJTU., all rights reserved.
// Version 1.0
#ifndef SCROLLVIEWEX_H
#define SCROLLVIEWEX_H

//#define MAX_VIEWSCALE	500		//最大缺省显示比例(%)
//#define MIN_VIEWSCALE	20		//最小缺省显示比例(%)

/////////////////////////////////////////////////////////////////////////////
// CScrollViewEx view
//##ModelId=3CE2421F028A
class CScrollViewEx : public CScrollView
{
	DECLARE_DYNCREATE(CScrollViewEx)
protected:
	//##ModelId=3CE2421F029A
	CScrollViewEx();			// protected constructor used by dynamic creation

// Operations
public:
	// Overridden CScrollView member functions
	//##ModelId=3CE2421F029B
	void SetZoomSizes(SIZE sizeTotal, const SIZE& sizePage = sizeDefault, const SIZE& sizeLine = sizeDefault);
	//##ModelId=3CE2421F02B9
	void CenterOnLogicalPoint(CPoint ptCenter);
	//##ModelId=3CE2421F02C9
	CPoint GetLogicalCenterPoint(void);
	//##ModelId=3CE2421F02CB
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	// Zooming functions
	//##ModelId=3CE242200133
	typedef enum {MODE_ZOOMOFF, MODE_ZOOMIN, MODE_ZOOMOUT} ZoomMode_;
	//##ModelId=3CE2421F02DA
	void      SetZoomMode(ZoomMode_ zoomMode);
	//##ModelId=3CE2421F02E9
	ZoomMode_ GetZoomMode() {return m_zoomMode;		};
	//##ModelId=3CE2421F02EA
	float GetZoomScale()	{return m_zoomScale;	};
	//##ModelId=3CE2421F02F8
	float GetZoomScaleMin()	{return m_fZoomScaleMin;};
	//##ModelId=3CE2421F02F9
	float GetZoomScaleMax()	{return m_fZoomScaleMax;};
	//##ModelId=3CE2421F02FA
	void SetZoomScaleMinMax(float ZoomScaleMin, float ZoomScaleMax);
	//##ModelId=3CE2421F0309
	int DoZoomIn    (CRect &rect);
	//##ModelId=3CE2421F0317
	int DoZoomIn    (float delta = 1.25, CPoint *point = NULL);	//放大x倍
	//##ModelId=3CE2421F0327
	int DoZoomOut   (float delta = 1.25, CPoint *point = NULL);	//缩小x倍
	//##ModelId=3CE2421F0336
	int DoZoomRatio (float ratio = 1.00, CPoint *point = NULL); //缩放到指定比例	
	//##ModelId=3CE2421F0346
	int DoZoomFull  ();

	// Override this to get notified of zoom scale change
	//##ModelId=3CE2421F0347
	virtual void NotifyZoom(void) {};
	//##ModelId=3CE2421F0357
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	//##ModelId=3CE2421F0366
	void DocToClient(CRect& rect);
	//##ModelId=3CE2421F0368
	void DocToClient(CPoint& point);
	//##ModelId=3CE2421F0376
	void ClientToDoc(CRect& rect);
	//##ModelId=3CE2421F0384
	void ClientToDoc(CPoint& point);

	// Zooming utility functions
	//##ModelId=3CE2421F0386
	void ViewDPtoLP (LPPOINT lpPoints, int nCount = 1);
	//##ModelId=3CE2421F0395
	void ViewLPtoDP (LPPOINT lpPoints, int nCount = 1);
	//##ModelId=3CE2421F03A5
	void ClientToDevice(CPoint &point);
	//##ModelId=3CE2421F03A7
	void NormalizeRect(CRect &rect);
	//##ModelId=3CE2421F03B4
	void DrawBox(CDC &dc, CRect &rect, BOOL xor = TRUE);
	//##ModelId=3CE2421F03C4
	void DrawLine(CDC &dc, const int &x1, const int &y1, 
						const int &x2, const int &y2, BOOL xor = TRUE);

// Implementation
protected:
	//##ModelId=3CE2421F03E2
	virtual ~CScrollViewEx();

#ifdef _DEBUG
	//##ModelId=3CE24220000A
	virtual void AssertValid() const;
#endif //_DEBUG
	//##ModelId=3CE24220000C
	virtual void OnDraw(CDC* pDC) {CScrollViewEx::OnDraw(pDC);}; // Pure Virtual

private:
	//##ModelId=3CE24220001B
	void AssertZoomScale();
	// Internally called functions
	//##ModelId=3CE242200029
	void	PersistRatio(const CSize &orig, CSize &dest, CPoint &remainder);
	//##ModelId=3CE24220003A
	void	CalcBars(void);
	
	// Private member variables
	//##ModelId=3CE242200049
	ZoomMode_	m_zoomMode;
	//##ModelId=3CE242200058
	BOOL		m_bCaptured;
	//##ModelId=3CE242200068
	CRect		m_ptDragRect;
	//##ModelId=3CE242200069
	CSize		m_origTotalDev;           // Origional total size in device units
	//##ModelId=3CE242200077
	CSize		m_origPageDev;            // Origional per page scroll size in device units
	//##ModelId=3CE242200087
	CSize		m_origLineDev;            // Origional per line scroll size in device units
	//##ModelId=3CE242200096
	float		m_zoomScale;			//缩放比例，1.0 表示不缩不放
	//##ModelId=3CE2422000A6
	float		m_fZoomScaleMin;		//缩放比例最小值
	//##ModelId=3CE2422000A7
	float		m_fZoomScaleMax;		//缩放比例最大值
				
	//HCURSOR   m_hZoomCursor;

public:
	// Generated message map functions
	//{{AFX_MSG(CScrollViewEx)
	//##ModelId=3CE2422000B6
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=3CE2422000C7
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//##ModelId=3CE2422000D7
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=3CE2422000E8
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//##ModelId=3CE2422000F7
 	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif
