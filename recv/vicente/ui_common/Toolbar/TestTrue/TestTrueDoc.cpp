// TestTrueDoc.cpp : implementation of the CTestTrueDoc class
//

#include "stdafx.h"
#include "TestTrue.h"

#include "TestTrueDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestTrueDoc

IMPLEMENT_DYNCREATE(CTestTrueDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestTrueDoc, CDocument)
	//{{AFX_MSG_MAP(CTestTrueDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestTrueDoc construction/destruction

CTestTrueDoc::CTestTrueDoc()
{
	// TODO: add one-time construction code here

}

CTestTrueDoc::~CTestTrueDoc()
{
}

BOOL CTestTrueDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestTrueDoc serialization

void CTestTrueDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestTrueDoc diagnostics

#ifdef _DEBUG
void CTestTrueDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestTrueDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestTrueDoc commands
