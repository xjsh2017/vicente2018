// XJBrowserDoc.h : interface of the CXJBrowserDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XJBROWSERDOC_H__14FCD114_4107_4A24_B701_3037072BCF14__INCLUDED_)
#define AFX_XJBROWSERDOC_H__14FCD114_4107_4A24_B701_3037072BCF14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DataEngine.h"


//##ModelId=49B87B7D00DA
class CXJBrowserDoc : public CDocument
{
protected: // create from serialization only
	//##ModelId=49B87B7D00DC
	CXJBrowserDoc();
	DECLARE_DYNCREATE(CXJBrowserDoc)

// Attributes
public:
	//##ModelId=49B87B7D00DE
	CDataEngine		m_DataEngine;	//数据引擎

	

// Operations
public:
/*
 *  @brief   	ShowSecPropPage	 显示指定二次设备的属性窗口 
 *  @param 		[In]a param of Type  CSecObj*  二次设备指针
 *  @param 		[In/Out]a param of Type  int  要激活哪个页面
 *  @return 	void 
 */
	//##ModelId=49B87B7D00E2
	void ShowSecPropPage(CSecObj* pObj, int nPage = PT_PROP_ACTION);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXJBrowserDoc)
	public:
	//##ModelId=49B87B7D00EB
	virtual BOOL OnNewDocument();
	//##ModelId=49B87B7D00ED
	virtual void Serialize(CArchive& ar);
	//##ModelId=49B87B7D00F0
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	//##ModelId=49B87B7D00F3
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//##ModelId=49B87B7D00F6
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B7D00F8
	virtual ~CXJBrowserDoc();
#ifdef _DEBUG
	//##ModelId=49B87B7D00FB
	virtual void AssertValid() const;
	//##ModelId=49B87B7D00FD
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXJBrowserDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XJBROWSERDOC_H__14FCD114_4107_4A24_B701_3037072BCF14__INCLUDED_)
