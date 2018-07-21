// ***************************************************************
//  ViewCommMap   version:  1.0    date: 5:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ����վͨѶ״̬ͼ����ͼ
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_VIEWCOMMMAP_H__2AA733F5_182E_44F6_B3CD_72E75DA5F313__INCLUDED_)
#define AFX_VIEWCOMMMAP_H__2AA733F5_182E_44F6_B3CD_72E75DA5F313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewCommMap.h : header file
//
#include "..\..\ui_common\Interface\SvgViewerInterface.h"
#include "..\..\ui_common\Interface\SvgOperateInterface.h"

/////////////////////////////////////////////////////////////////////////////


// CViewCommMap view

//##ModelId=49B87B8502BF
class CViewCommMap : public CScrollView
{
protected:
	//##ModelId=49B87B8502C1
	CViewCommMap();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCommMap)

// Attributes
public:
	//##ModelId=49B87B8502C3
	CLSID clsid;
	//##ModelId=49B87B8502CF
	LPCLASSFACTORY pClf;
	//##ModelId=49B87B8502D4
	LPUNKNOWN pUnk;
	/** @brief           SVG�ؼ��ӿ�ָ��*/
	ISvgViewer * pSvgViewer;
	/** @brief           SVG�ؼ��������ܽӿ�ָ��*/
	ISvgOperate* pSvgOperate;
	
	/** @brief           �Ƿ���װ��ͼ��*/
	//##ModelId=49B87B8502DD
	BOOL m_bLoaded;
	/** @brief           �Ƿ��ѳ�ʼ��COM�ؼ�*/
	//##ModelId=49B87B8502DE
	BOOL m_bInitCOM;
	/** @brief           ��ѡ�е��豸����*/
	//##ModelId=49B87B8502E0
	DEVICE_LIST m_selected;

	/** @brief           ͼ��ԭʼ��С*/
	//##ModelId=49B87B8502E4
	CSize	m_szOriSize;
	/** @brief           ͼ�ε�ǰ��С*/
	//##ModelId=49B87B8502E5
	CSize	m_szCurSize;

	CPtrList   m_PtrList;

	int        m_nCount;

	BOOL	m_bShapeRefreshed;

private:
	int m_nStationsPerNet;
	int m_nStationWidth;
	int m_nStationHeight;
	int m_nStationSpace;
	int m_nNetSpace;
	int m_nNetTextHeight;

	int StationNumPreRow;

	CUIntArray m_arrStationCount;
// Operations
public:
/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B8502E6
	void DocToClient(CPoint & pt);
/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B8502EE
	void DocToClient(CRect & rect);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B8502F0
	void ClientToDoc(CPoint & pt);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In/Out]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B8502F2
	void ClientToDoc(CRect & rect);

/*
 *  @brief   	CenterDevice	 ʹָ���豸ͼ�ξ�����ʾ 
 *  @param 		[In]a param of Type  CString  ָ��ͼ��ID
 *  @return 	void 
 */
	//##ModelId=49B87B8502FD
	void CenterDevice(CString strID);

	void AddBlinkDevice( CDeviceObj* pObj );
	void RemoveAtBlinkDevice( CDeviceObj* pObj );

private:
/*
 *  @brief   	InitCOM	 ��ʼ��COM�ؼ� 
 *  @return 	void 
 */
	//##ModelId=49B87B8502FF
	void InitCOM();

/*
 *  @brief   	OpenView	 ��ָ����SVG�ļ� 
 *  @param 		[In]a param of Type  CString  �ļ�·��
 *  @return 	BOOL �򿪳ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B850300
	BOOL OpenView(CString strPath);

/*
 *  @brief   	StringToID	 �ѷ��ص�ѡ���豸�ַ���ת��Ϊ���豸ID 
 *  @param 		[In]a param of Type  CString  ѡ���豸�ַ���
 *  @param 		[Out]a param of Type  CStringArray &  �豸ID����
 *  @return 	void 
 */
	//##ModelId=49B87B85030D
	void StringToID(CString strID, CStringArray & arrID);

/*
 *  @brief   	ReleaseTemp	 �����ʱ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B850310
	void ReleaseTemp();

/*
 *  @brief   	SetStationShapeColor	 ����ָ��״̬����ָ����վ��ɫ
 *  @param 		[In]a param of Type  CStationObj  ��վ
 *  @return 	void 
 */
	//##ModelId=49B87B850311
	void SetStationShapeColor(CStationObj* pStation);
	int GetShapeIndex( CDeviceObj* pObj );
	void RefreshStatusToShape();

	BOOL CreateCommMap();

	/*
	 *  @brief   	CreateLandscapeCommMap	ÿ�������ڵ���վ����20��ʱʹ�ã������������У������ڷ�Ϊ���У�ÿ�����10����վ 
	 *  @return 	BOOL	 
	 */
	BOOL CreateLandscapeCommMap();

	/*
	 *  @brief   	CreateVerticalCommMap �����ڵ���վ����20��ʱʹ�ã������������У���������վ�������У�ÿ����ʾ9����վ	 
	 *  @return 	BOOL	 
	 */
	BOOL CreateVerticalCommMap();

	void CreateStationFigure(CStationObj* pStation, int x, int y);

	void CreateNetFigureLandscape(CNetObj* pNet, int x, int y);
	void CraeteNetFigureVertical(CNetObj* pNet, int x, int y);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCommMap)
	protected:
	//##ModelId=49B87B85031D
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//##ModelId=49B87B850320
	virtual void OnInitialUpdate();     // first time after construct
	

	
	//##ModelId=49B87B850322
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL																																																																																																																																																																

// Implementation
protected:
	//##ModelId=49B87B85032E
	virtual ~CViewCommMap();
#ifdef _DEBUG
	//##ModelId=49B87B850330
	virtual void AssertValid() const;
	//##ModelId=49B87B850332
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewCommMap)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnComMap();
	afx_msg void OnComRefresh();
	//}}AFX_MSG
	//##ModelId=49B87B850361
	afx_msg void OnSTTP20144(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationStatusChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnstationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRefreshCommMap(WPARAM wParam, LPARAM lParam);
	
	void RecreateMap();
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCOMMMAP_H__2AA733F5_182E_44F6_B3CD_72E75DA5F313__INCLUDED_)
