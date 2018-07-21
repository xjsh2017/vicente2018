// DraftDocument.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DraftDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDraftDocument

IMPLEMENT_DYNCREATE(CDraftDocument, CDocument)

CDraftDocument::CDraftDocument()
{
	m_sStationName = "";
	m_sStationID = "";
	m_nSelect = 0;
}

BOOL CDraftDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDraftDocument::~CDraftDocument()
{
}


BEGIN_MESSAGE_MAP(CDraftDocument, CDocument)
	//{{AFX_MSG_MAP(CDraftDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftDocument diagnostics

#ifdef _DEBUG
void CDraftDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CDraftDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDraftDocument serialization

void CDraftDocument::Serialize(CArchive& ar)
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

BOOL CDraftDocument::Init(CString sStationID, CString sStationName)
{
	m_sStationID = sStationID;
	m_sStationName = sStationName;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString sdir = pApp->GetDownDir() + "draft"+"\\"+m_sStationID+"\\";
	m_arrFiles.clear();
	SearchDraftFiles( sdir );
	if( m_arrFiles.size() <= 0 )
		return FALSE;
	return TRUE;
}

BOOL CDraftDocument::SearchDraftFiles(CString path)
{
	CFileFind finder; //�ļ�������
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //�����ļ�
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		
		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // ��Ŀ¼,�������
		{
			CString str = finder.GetFilePath();
			SearchDraftFiles(str);
		}
		else //���ļ�
		{
			//�õ��ļ���׺
			CString strFileName = finder.GetFileName();
			int pos = strFileName.ReverseFind('.');
			if(pos != -1)
			{
				CString str = strFileName.Right(strFileName.GetLength() - pos -1);
				if( str.CompareNoCase("bmp") == 0)
				{
					DraftFile draftFile;
					draftFile.m_sTitle = finder.GetFileTitle();
					draftFile.m_sPath = finder.GetFilePath();

					m_arrFiles.push_back( draftFile );
				}
			}
		}
	}
	finder.Close();
	
	return TRUE;
}

