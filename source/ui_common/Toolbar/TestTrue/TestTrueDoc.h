// TestTrueDoc.h : interface of the CTestTrueDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTTRUEDOC_H__AD5DE745_CBD3_48D2_8DBC_B578ECF6A126__INCLUDED_)
#define AFX_TESTTRUEDOC_H__AD5DE745_CBD3_48D2_8DBC_B578ECF6A126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestTrueDoc : public CDocument
{
protected: // create from serialization only
	CTestTrueDoc();
	DECLARE_DYNCREATE(CTestTrueDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestTrueDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestTrueDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestTrueDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTTRUEDOC_H__AD5DE745_CBD3_48D2_8DBC_B578ECF6A126__INCLUDED_)
