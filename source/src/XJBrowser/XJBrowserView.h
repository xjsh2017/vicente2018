// XJBrowserView.h : interface of the CXJBrowserView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XJBROWSERVIEW_H__FF42B0C9_461C_41AC_AE8F_85DCE51588EC__INCLUDED_)
#define AFX_XJBROWSERVIEW_H__FF42B0C9_461C_41AC_AE8F_85DCE51588EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ViewBase.h"
#include "DeviceObj.h"
#include "..\..\ui_common\Interface\SvgViewerInterface.h"

#include "CommWnd.h"
#include "pages/pt/HangoutWnd.h"
#include "ToolTipWnd.h"

//##ModelId=49B87B7C0261
class CXJBrowserView : public CFormView
{
protected: // create from serialization only
	//##ModelId=49B87B7C0272
	CXJBrowserView();
	DECLARE_DYNCREATE(CXJBrowserView)

public:
	//{{AFX_DATA(CXJBrowserView)
	enum{ IDD = IDD_XJBROWSER_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B7C0273
	CXJBrowserDoc* GetDocument();
//	void BlinkShape(CString strStationID, BOOL bBlink, int nTime = 1000); 
public:
	//##ModelId=49B87B7C0281
	CLSID clsid;
	//##ModelId=49B87B7C0291
	LPCLASSFACTORY pClf;
	//##ModelId=49B87B7C0296
	LPUNKNOWN pUnk;
	//##ModelId=49B87B7C029B
	/** @brief           SVG�ؼ��ӿ�ָ��*/
	ISvgViewer * pSvgViewer;
	
	/** @brief           �Ƿ���װ��ͼ��*/
	//##ModelId=49B87B7C029F
	BOOL m_bLoaded;
	/** @brief           �Ƿ��ѳ�ʼ��COM�ؼ�*/
	//##ModelId=49B87B7C02A0
	BOOL m_bInitCOM;
	/** @brief           ��ѡ�е��豸����*/
	//##ModelId=49B87B7C02A2
	DEVICE_LIST m_selected;
	/** @brief           ��ǰ��ʾ�ĳ�վID*/
	//##ModelId=49B87B7C02A6
	CString m_sStationID;
	/** @brief           ��ǰ��ʾ��վ��ָ��*/
	//##ModelId=49B87B7C02A8
	CStationObj * m_pStation;
	CStationObj* m_pSelectedStation;
	/** @brief           ��ǰ��ʾ���ļ�·��*/
	//##ModelId=49B87B7C02AF
	CString m_sCurrentPath;
	/** @brief           ͼ��ԭʼ��С*/
	//##ModelId=49B87B7C02B0
	CSize	m_szOriSize;
	/** @brief           ͼ�ε�ǰ��С*/
	//##ModelId=49B87B7C02B1
	CSize	m_szCurSize;
	/** @brief           �Ƿ��˳�ˢ��״̬�߳�*/
	//##ModelId=49B87B7C02B2
	BOOL	m_bExitRefresh;
	/** @brief           �豸ͼ��״̬ˢ���߳�*/
	//##ModelId=49B87B7C02C0
	CWinThread* m_pStatusRefresh;

	/** @brief           ��ǰ��ʾ�ļ��ͼ��*/
	CBayObj* m_pCurBay;

	/** @brief           SVGͼ������. 1-��վͼ��. 2-���ͼ��*/
	int	m_nSvgType;

	//��˸ˢ�·�Χ
	//##ModelId=49B87B7C02C4
	CRect m_rcBlink;
	//��˸ˢ���߳�
	//##ModelId=49B87B7C02C6
	CWinThread * m_pBlinkThread;
	//��˸�̱߳�־
	//##ModelId=49B87B7C02CA
	BOOL m_bExitBlink;
	/** @brief           ��˸ͼ�εļ��ʱ��*/
	//##ModelId=49B87B7C02CE
	int	 m_nBlinkTime;

	CPtrList   m_PtrList;
	
	int        m_nCount;

	/** @brief			����˸ͼ������*/
	DEVICE_LIST listBlinkDevice;
	/** @brief			������˸��ͼ��Ԫ��ID*/
	CUIntArray m_arrBlinking;

	/** @brief           ��*/
	CRITICAL_SECTION m_CriticalSection;

	CCommWnd	m_wndComm;
	CHangoutWnd	m_wndHangout;
	CSize		m_szCommWnd;
	CSize		m_szHangoutWnd;

	BOOL	m_bRefreshMark;
	BOOL	m_bRefreshNoteMark;

	CToolTipWnd	m_tooltip;

// Operations
private:
/*
 *  @brief   	OpenView	 ��ָ����SVG�ļ� 
 *  @param 		[In]a param of Type  CString  �ļ�·��
 *  @return 	BOOL �򿪳ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7C02CF
	BOOL OpenView(CString strPath);

/*
 *  @brief   	GetShapeIndex	 ȡ���豸��ͼ������ 
 *  @param 		[In]a param of Type  CDeviceObj*  ָ���豸
 *  @return 	int ͼ������
 */
	int	GetShapeIndex(CDeviceObj* pObj);

/*
 *  @brief   	FindDevicePlus	 ȡ��ָ���豸������������� 
 *  @param 		[In]a param of Type  CString  ָ���豸ID
 *  @param 		[In]a param of Type  CString  ָ���豸����
 *  @return 	CDeviceObj* �豸����
 */
	CDeviceObj* FindDevicePlus(CString sID, CString sType);

	/*
	* @brief	BlinkShapeInStation	��˸ָ����վ�´���˸��ͼ��
	* @param	[In]a param of Type CString sStationID		
	* @return   void	
	*/
	void BlinkShapeInStation(CString sStationID);

	int BlinkSec(CSecObj* pObj, CUIntArray& arrBlink);
	void StopBlink(CUIntArray& arrBlink);

	BOOL IsInBlinkArray(int nIndex);

	void BlinkShapeInOtherStation(CSecObj* pSec);

	void RefreshDeviceMark();
	void SetMark(CSecObj* pObj);

	void RefreshNoteMark();
	void SetNoteMark(CSecObj* pObj);

public:
	void AddToBlinkList(CSecObj* pObj);
/*
 *  @brief   	SetDeviceCol	 �����豸״̬����ͼ����ɫ 
 *  @param 		[In]a param of Type  CSecObj*  �豸ָ��(Ŀǰֻ֧�ֶ����豸)
 *  @return 	void 
 */
	//##ModelId=49B87B7C02DE
	void SetDeviceCol(CSecObj* pObj);

/*
 *  @brief   	AddBlinkDevice	 ʹָ�������豸��˸ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	//##ModelId=49B87B7C02E0
	void AddBlinkDevice(CSecObj* pObj);

/*
 *  @brief   	RemoveAtBlinkDevice	 ʹָ�������豸������˸ 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @return 	void 
 */
	//##ModelId=49B87B7C02E2
	void RemoveAtBlinkDevice(CDeviceObj* pObj);

/*
 *  @brief   	RemoveAllBlinkDevices	 ֹͣ�����豸��˸ 
 *  @return 	void 
 */
	//##ModelId=49B87B7C02EE
	void RemoveAllBlinkDevices();

/*
 *  @brief   	CenterDevice	 ʹָ���豸ͼ�ξ�����ʾ 
 *  @param 		[In]a param of Type  CDeviceObj*  ָ���豸
 *  @return 	void 
 */
	//##ModelId=49B87B7C02EF
	void CenterDevice(CDeviceObj* pObj);

private:
/*
 *  @brief   	InitCOM	 ��ʼ��COM�ؼ� 
 *  @return 	void 
 */
	//##ModelId=49B87B7C02F1
	void InitCOM();
public:
/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B7C02FD
	void DocToClient(CPoint & pt);
/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B7C02FF
	void DocToClient(CRect & rect);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B7C030E
	void ClientToDoc(CPoint & pt);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In/Out]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B7C031C
	void ClientToDoc(CRect & rect);
private:
/*
 *  @brief   	StringToID	 �ѷ��ص�ѡ���豸�ַ���ת��Ϊ���豸ID 
 *  @param 		[In]a param of Type  CString  ѡ���豸�ַ���
 *  @param 		[Out]a param of Type  CStringArray &  �豸ID����
 *  @return 	void 
 */
	//##ModelId=49B87B7C032C
	void StringToID(CString strID, CStringArray & arrID);

/*
 *  @brief   	TypeStringToInt	 �ַ��͵��豸����ת��Ϊ���ε��豸���� 
 *  @param 		[In]a param of Type  CString  �ַ��͵��豸����
 *  @return 	int ���ε��豸����
 */
	//##ModelId=49B87B7C032F
	int TypeStringToInt(CString sType);

/*
 *  @brief   	ReleaseTemp	 �����ʱ���� 
 *  @return 	void 
 */
	//##ModelId=49B87B7C033D
	void ReleaseTemp();

/*
 *  @brief   	GetDeviceType	 ȡ���豸����
 *  @param 		[In]a param of Type  CString  Uri
 *  @param 		[In,out]a param of Type  CString �豸����
 *  @return 	int ���ε��豸����
 */
     void GetDeviceType(CString strUri, CString& strDeviceType);

public:
/*
 *  @brief   	CloseCurrentView	 �رյ�ǰ��ʾ�ĳ�վͼ�� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7C033E
	BOOL CloseCurrentView();

/*
 *  @brief   	OpenStationView	 ��ָ����վ��ͼ�� 
 *  @param 		[In]a param of Type  CString  ��վID
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B7C034B
	BOOL OpenStationView(CString strID);

/*
 *  @brief   	OpenBayView	 ��ָ�������ͼ��
 *  @param 		[In]a param of Type  CBayObj*  ָ�����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL OpenBayView(CBayObj* pObj);

/*
 *  @brief   	EndOwnerThread	 �˳������Լ���ĳ����߳�
 *  @return 	void 
 */
	//##ModelId=49B87B7C034D
	void EndOwnerThread();

/*
 *  @brief   	RefreshDeviceStatus	 ˢ���豸״̬ 
 *  @return 	void 
 */
	//##ModelId=49B87B7C034E
	void RefreshDeviceStatus();

	void RePositionCommWnd();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXJBrowserView)
	public:
	//##ModelId=49B87B7C034F
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//##ModelId=49B87B7C035D
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//##ModelId=49B87B7C036D
	virtual void OnInitialUpdate(); // called first time after construct
	//##ModelId=49B87B7C037B
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//##ModelId=49B87B7C038A
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//##ModelId=49B87B7C039A
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//##ModelId=49B87B7C03AA
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//##ModelId=49B87B7C03C8
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//##ModelId=49B87B7C03D8
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B7D0000
	virtual ~CXJBrowserView();
#ifdef _DEBUG
	//##ModelId=49B87B7D0002
	virtual void AssertValid() const;
	//##ModelId=49B87B7D0010
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXJBrowserView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPtSignallist();
	afx_msg void OnQueryEvent();
	afx_msg void OnMapRefresh();
	afx_msg void OnMapStopalart();
	afx_msg void OnMapZoomfull();
	afx_msg void OnMapZoomin();
	afx_msg void OnMapZoomout();
	afx_msg void OnMapZoomOld();
	afx_msg void OnUpdateMapZoomOld(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapZoomfull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMapZoomout(CCmdUI* pCmdUI);
	afx_msg void OnMENUITEMDevTreePTRunUpdate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMarkDevice();
	afx_msg void OnMarkPtdevice();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnDraft();
	afx_msg void OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseLeave( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	//##ModelId=49B87B7D00AE
	afx_msg void OnPTProp(UINT nID);
	afx_msg void OnWRProp(UINT nID);
	//##ModelId=49B87B7D00BB
	afx_msg void OnSTTP20002(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B7D00BF
	afx_msg void OnSTTP20079(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B7D00CC
	afx_msg void OnAllClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20018(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSTTP20022(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStationChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowTooltip(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in XJBrowserView.cpp
inline CXJBrowserDoc* CXJBrowserView::GetDocument()
   { return (CXJBrowserDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XJBROWSERVIEW_H__FF42B0C9_461C_41AC_AE8F_85DCE51588EC__INCLUDED_)
