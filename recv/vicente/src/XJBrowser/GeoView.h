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
	//����ͼƬ��Ϣ
	CBitmap m_Bitmap;		
	BITMAPINFO m_bmInfo;	
	int m_nImageWidth;
	int m_nImageHeight;
	BOOL m_bMouseLeave;
	BOOL m_mostMin;

	/** @brief           ����Ƿ񱻰���*/
	BOOL	m_bMouseDown;
	/** @brief           ����λ��*/
	CPoint	m_ptMouse;
	/** @brief           �Ƿ�ɱ༭*/
	BOOL	m_bCanEdit;
	/** @brief           ���뱻ѡ��ĳ�վ*/
	CStationObj* m_pSelStation;

	COleDropTarget	m_dropTarget;	//OLE Drag-Drop support

	CMapStringToOb	m_mapTime;

	int m_nBlinkTime;//��˸���ʱ��

	

// Operations
public:

	void BlinkStation(CDeviceObj* pObj);
/*
 *  @brief   	FindFigure	 ���ұ�����ĳ�վ 
 *  @param 		[In]a param of Type  CPoint  �����λ��
 *  @return 	CStationObj* ������ĳ�վ
 */
	CStationObj* FindFigure(CPoint pt);

/*
 *  @brief   	ClearSelected	 �������ѡ�� 
 *  @return 	void 
 */
	void ClearSelected();

/*
 *  @brief   	RefreshObj	 ˢ��ָ����վ��ͼ�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
 *  @return 	void 
 */
	void RefreshObj(CStationObj* pStation);

/*
 *  @brief   	GetSelected	 ����ָ��λ��ȡ�ñ�ѡ��ĳ�վ 
 *  @param 		[In]a param of Type  CPoint  ָ��λ��
 *  @return 	CStationObj* ��ѡ��ĳ�վ
 */
	CStationObj* GetSelected(CPoint pt);

/*
 *  @brief   	SetSelected	 ����ָ����վ��ѡ�� 
 *  @param 		[In]a param of Type  CStationObj*  ָ����վ
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
