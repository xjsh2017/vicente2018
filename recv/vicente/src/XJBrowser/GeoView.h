#if !defined(AFX_GEOVIEW_H__5C2FFA03_46EF_48B2_AF5E_FD08BFE77446__INCLUDED_)
#define AFX_GEOVIEW_H__5C2FFA03_46EF_48B2_AF5E_FD08BFE77446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeoView.h : header file
//
#include "ScrollViewEx.h"

/////////////////////////////////////////////////////////////////////////////
// CGeoView view

class CGeoView : public CScrollViewEx
{
protected:
	CGeoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGeoView)

// Attributes
private:
	//背景图片信息
	CBitmap m_Bitmap;		
	BITMAPINFO m_bmInfo;	
	int m_nImageWidth;
	int m_nImageHeight;
	BOOL m_bMouseLeave;
	BOOL m_mostMin;

	/** @brief           鼠标是否被按下*/
	BOOL	m_bMouseDown;
	/** @brief           鼠标的位置*/
	CPoint	m_ptMouse;
	/** @brief           是否可编辑*/
	BOOL	m_bCanEdit;
	/** @brief           当请被选择的厂站*/
	CStationObj* m_pSelStation;

	COleDropTarget	m_dropTarget;	//OLE Drag-Drop support

	CMapStringToOb	m_mapTime;

	int m_nBlinkTime;//闪烁间隔时间

	

// Operations
public:

	void BlinkStation(CDeviceObj* pObj);
/*
 *  @brief   	FindFigure	 查找被点击的厂站 
 *  @param 		[In]a param of Type  CPoint  点击的位置
 *  @return 	CStationObj* 被点击的厂站
 */
	CStationObj* FindFigure(CPoint pt);

/*
 *  @brief   	ClearSelected	 清空所有选择 
 *  @return 	void 
 */
	void ClearSelected();

/*
 *  @brief   	RefreshObj	 刷新指定厂站的图形 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	void RefreshObj(CStationObj* pStation);

/*
 *  @brief   	GetSelected	 根据指定位置取得被选择的厂站 
 *  @param 		[In]a param of Type  CPoint  指定位置
 *  @return 	CStationObj* 被选择的厂站
 */
	CStationObj* GetSelected(CPoint pt);

/*
 *  @brief   	SetSelected	 设置指定厂站被选中 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	void SetSelected(CStationObj* pStation);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeoView)
	public:
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGeoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGeoView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnGeoEdit();
	afx_msg void OnGeoDelete();
	afx_msg void OnGeoOpen();
	afx_msg void OnGeoSave();
	afx_msg void OnGeoZoomin();
	afx_msg void OnGeoZoomout();
	afx_msg void OnGeoZoomReturn();
	afx_msg void OnGeoLocaltree();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOVIEW_H__5C2FFA03_46EF_48B2_AF5E_FD08BFE77446__INCLUDED_)
