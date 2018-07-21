#if !defined(AFX_VIEWFAULTDETAIL_H__89322600_C7EE_4267_9617_4522642D1E7C__INCLUDED_)
#define AFX_VIEWFAULTDETAIL_H__89322600_C7EE_4267_9617_4522642D1E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewFaultDetail.h : header file
//
#include "..\..\ui_common\Interface\XJReportInterface.h"
#include "FaultReport.h"
#include "..\..\ui_common\DLLEncapsulation\WaveShapeEngine.h"
/////////////////////////////////////////////////////////////////////////////
// CViewFaultDetail form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//##ModelId=49B87B840157
class CViewFaultDetail : public CFormView
{
protected:
	//##ModelId=49B87B840159
	CViewFaultDetail();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewFaultDetail)

// Form Data
public:
	//{{AFX_DATA(CViewFaultDetail)
	enum { IDD = IDD_XJ_VIEW_FAULT_DETAIL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	//##ModelId=49B87B840168
	CLSID clsid;
	//##ModelId=49B87B840178
	LPCLASSFACTORY pClf;
	//##ModelId=49B87B84017D
	LPUNKNOWN pUnk;
	//##ModelId=49B87B840182
	IXJReport* pReportDoc;
	WaveShapeEngine *pWaveShape;
	/** @brief           ��ǰ��ʾ�Ĺ��ϱ������ָ��*/
	//##ModelId=49B87B840187
	CFaultReport* m_pReport;
	/** @brief           ��ǰ�ĵ���С*/
	//##ModelId=49B87B84018B
	CSize		  m_szDoc;
	/** @brief           ����֮ǰ��ѡ�е�Ԫ��ID*/
	//##ModelId=49B87B84018C
	int			  m_nOldSelEle;
	/** @brief           ���浱ǰ��ģ���ļ���*/
	//##ModelId=49B87B84018D
	CString		  m_sCurFile;
	/** @brief           ��¼�Ƿ��������ļ�*/
	//##ModelId=49B87B840196
	BOOL		  m_bLoad;
	/** @brief           �Ƿ���ʾ������Ϣ*/
	BOOL		  m_bShowReturn;
	/** @brief			�Ƿ���ʾ��������λ��Ϣ*/
	BOOL		  m_bShowDI;
	CToolTipCtrl  m_tool;
	CPoint pDot;
	CFaultEvent* m_pEvent;
	CBitmap m_bitmapNew;
	CDC dcNew;//��������ͼƬ
	int m_nMaxHeight;//��
	int m_nMaxWidth;//��
// Operations
public:
	int GetBitMapSize(int x,int y);
	void BuildNewBitmap(CDC* pDC);
	BOOL m_bGetNewFault;
/*
 *  @brief   	InitCOM	 ��ʼ��COM�ӿ�
 *  @return 	BOOL 
 */
	//##ModelId=49B87B840197
	BOOL InitCOM();

/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B8401A5
	void DocToClient(CPoint & pt);
/*
 *  @brief   	DocToClient	 �߼�����ת�豸���� 
 *  @param 		[In]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B8401B5
	void DocToClient(CRect & rect);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In]a param of Type  CPoint &  Ҫת��������
 *  @return 	void 
 */
	//##ModelId=49B87B8401B7
	void ClientToDoc(CPoint & pt);
/*
 *  @brief   	ClientToDoc	 �豸����ת�߼����� 
 *  @param 		[In/Out]a param of Type  CRect &  Ҫת���ķ�Χ
 *  @return 	void 
 */
	//##ModelId=49B87B8401C5
	void ClientToDoc(CRect & rect);

/*
 *  @brief   	TranslateSingleKey	 ���͵����ؼ��ֵ����� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString �ؼ��ֱ�ʾ��ֵ
 */
	//##ModelId=49B87B8401D4
	CString TranslateSingleKey(CString sKey);

/*
 *  @brief   	GetEventByKey	 �ɹؼ��ֵõ���Ӧ�Ĺ����¼� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CFaultEvent* �����¼�ָ��
 */
	//##ModelId=49B87B8401E4
	CFaultEvent* GetEventByKey(CString sKey);

/*
 *  @brief   	GetEventBySignleKey	 �ɵ����ؼ����ҵ���Ӧ�Ĺ����¼� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *	@param		[Out]a param of Type CString  β���ؼ���
 *  @return 	CFaultEvent* �����¼�ָ��
 */
	//##ModelId=49B87B8401E6
	CFaultEvent* GetEventBySignleKey(CString sKey, CString & sTail);

/*
 *  @brief   	GetFullPath	 �õ�ָ�������豸��ָ��¼���ļ�������·�� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @param 		[In]a param of Type  CString  ָ��¼���ļ�
 *  @return 	CString ����·��
 */
	//##ModelId=49B87B8401F4
	CString GetFullPath(CSecObj* pSec, CString strFileName);

/*
 *  @brief   	GetValue_getcount	 �õ��ؼ���getcount��ֵ
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B8401F7
	CString GetValue_getcount(CString sKey);

/*
 *  @brief   	GetValue_OneLayer	 �õ�һ����ȹؼ��ֵ�ֵ 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B840204
	CString GetValue_OneLayer(CString sKey);

/*
 *  @brief   	GetValue_TwoLayer	 �õ�������ȹؼ��ֵ�ֵ 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B840206
	CString GetValue_TwoLayer(CString sKey);

	CString GetTemplateFile(CFaultReport* pReport);

	CString GetValue_TwoLayer_Landscape(CString sKey);
	CString GetValue_getcount_Landscape(CString sKey);
	CString GetValue_OneLayer_Landscape(CString sKey);
	CFaultEvent* GetEventBySignleKey_Landscape(CString sKey, CString & sTail);

	/*
	 *  @brief   	CheckLandscape	����Ƿ�֧�ֺ��򲼾� 
	 *  @return 	int	 0-��֧�֣�1-֧��
	 */
	int CheckLandscape();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewFaultDetail)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87B840244
	virtual ~CViewFaultDetail();
#ifdef _DEBUG
	//##ModelId=49B87B840251
	virtual void AssertValid() const;
	//##ModelId=49B87B840261
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewFaultDetail)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaultPrint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFaultReturn();
	afx_msg void OnFaultExport();
	afx_msg void OnFaultRefresh();
	afx_msg void OnFaultShowdi();
	//}}AFX_MSG
	//##ModelId=49B87B8402CE
	afx_msg void OnSelChanged(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8402DE
	afx_msg void OnPrintFaultReport(WPARAM wParam, LPARAM lParam);
	//##ModelId=49B87B8402EE
	afx_msg void OnRefreshFaultReport(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportExcelFaultReport(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWFAULTDETAIL_H__89322600_C7EE_4267_9617_4522642D1E7C__INCLUDED_)
