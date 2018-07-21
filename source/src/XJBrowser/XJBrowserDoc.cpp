// XJBrowserDoc.cpp : implementation of the CXJBrowserDoc class
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "XJBrowserDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserDoc

IMPLEMENT_DYNCREATE(CXJBrowserDoc, CDocument)

BEGIN_MESSAGE_MAP(CXJBrowserDoc, CDocument)
	//{{AFX_MSG_MAP(CXJBrowserDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserDoc construction/destruction

//##ModelId=49B87B7D00DC
CXJBrowserDoc::CXJBrowserDoc()
{
	// TODO: add one-time construction code here

}

//##ModelId=49B87B7D00F8
CXJBrowserDoc::~CXJBrowserDoc()
{
}

//##ModelId=49B87B7D00EB
BOOL CXJBrowserDoc::OnNewDocument()
{
//	if (!CDocument::OnNewDocument())
//		return FALSE;

	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	if(pFrame != NULL)
	{
		pFrame->m_pDoc = this;
	}
	//读取设备基本数据
	WriteLog("start load data", XJ_LOG_LV3);
	m_DataEngine.LoadData();
	WriteLog("end load data", XJ_LOG_LV3);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	WriteLog("start create views", XJ_LOG_LV3);
	AfxGetApp() ->m_pMainWnd ->SendMessage(WM_NEWDOCUMENTOPENED, 0, (LPARAM)this);
	WriteLog("end create views", XJ_LOG_LV3);

	TRACE("CXJBrowserDoc::OnNewDocument \n");
	pFrame->SetMenu(NULL); 
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXJBrowserDoc serialization

//##ModelId=49B87B7D00ED
void CXJBrowserDoc::Serialize(CArchive& ar)
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
// CXJBrowserDoc diagnostics

#ifdef _DEBUG
//##ModelId=49B87B7D00FB
void CXJBrowserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

//##ModelId=49B87B7D00FD
void CXJBrowserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserDoc commands

//##ModelId=49B87B7D00F0
BOOL CXJBrowserDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class

//	return CDocument::CanCloseFrame(pFrame);
	if(pFrame->IsKindOf(RUNTIME_CLASS(CPTFrame)))
	{
		//是保护属性页面,可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CWaveRecFrame)))
	{
		//录波器属性页面,可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CBatchFrame)))
	{
		//批量召唤页面, 可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CAutoCallFrame)))
	{
		//自动总召设置页面, 可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CAutoResultFrame)))
	{
		//自动总召结果查看页面, 可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CDownloadFrame)))
	{
		//通用文件下载页面,可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CStationInfoFrame)))
	{
		//厂站信息页面，可关闭
		return TRUE;
	}
	if(pFrame->IsKindOf(RUNTIME_CLASS(CDownOscFrame)))
	{
		//录波下载页面，可关闭
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B7D00F3
BOOL CXJBrowserDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: Add your specialized creation code here
	AfxGetApp() ->m_pMainWnd ->SendMessage(WM_NEWDOCUMENTOPENED, 0, (LPARAM)this);

	TRACE("CXJBrowserDoc::OnOpenDocument \n");
	
	return TRUE;
}

//##ModelId=49B87B7D00F6
void CXJBrowserDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("CXJBrowserDoc::OnCloseDocument start\n");
//	AfxGetApp()->m_pMainWnd->SendMessage(WM_DOCUMENTCLOSEED, 0, (LPARAM)this);
	
	CDocument::OnCloseDocument();
	TRACE("CXJBrowserDoc::OnCloseDocument end\n");
}

/*************************************************************
 函 数 名：ShowSecPropPage()
 功能概要：显示指定二次设备的属性窗口
 返 回 值: void
 参    数：param1	二次设备指针
**************************************************************/
//##ModelId=49B87B7D00E2
void CXJBrowserDoc::ShowSecPropPage( CSecObj* pObj, int nPage /*= 0*/ )
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OpenSecPropPage(pObj, nPage);
}



