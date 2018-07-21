#if !defined(AFX_DRAFTDOCUMENT_H__5A42B068_BC3A_44CD_BB34_17879F4A5E7D__INCLUDED_)
#define AFX_DRAFTDOCUMENT_H__5A42B068_BC3A_44CD_BB34_17879F4A5E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DraftDocument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDraftDocument document

class DraftFile
{
public:
	DraftFile(){};
	~DraftFile(){};
public:
	CString m_sPath;//全路径
	CString m_sTitle;//文件名，不带后缀
};

class CDraftDocument : public CDocument
{
public:
	CDraftDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDraftDocument)

// Attributes
private:
	CString m_sStationID;
	
	int	m_nSelect;

	
public:
	CString m_sStationName;
	vector<DraftFile>	m_arrFiles;
	BOOL Init( CString sStationID, CString sStationName );
	CString GetSelectedDraftTitle(){ return m_arrFiles[m_nSelect].m_sTitle; };
	CString GetSelectedDraftPath(){ return m_arrFiles[m_nSelect].m_sPath; };
	void SetSelected(int nselected){m_nSelect = nselected;};
private:
	BOOL SearchDraftFiles( CString path );
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraftDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDraftDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDraftDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAFTDOCUMENT_H__5A42B068_BC3A_44CD_BB34_17879F4A5E7D__INCLUDED_)
