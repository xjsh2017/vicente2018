// DlgDraft.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgDraft.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDraft dialog


CDlgDraft::CDlgDraft(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDraft::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDraft)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sStationID = "";
	m_nSelect = 0;
	m_sStationName = "";
}


void CDlgDraft::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDraft)
	DDX_Control(pDX, IDC_LIST_DRAFTFILES, m_List);
	DDX_Control(pDX, IDC_DDVUECTRL1, m_ddvue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDraft, CDialog)
	//{{AFX_MSG_MAP(CDlgDraft)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DRAFTFILES, OnItemchangedListDraftfiles)
	ON_COMMAND(ID_DRAFT_ZOOMIN, OnDraftZoomin)
	ON_COMMAND(ID_DRAFT_ZOOMOUT, OnDraftZoomout)
	ON_COMMAND(ID_DRAFT_WINDOW, OnDraftWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDraft message handlers

BOOL CDlgDraft::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT,  WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS ) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_DRAFT))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
	
	m_wndToolBar.LoadTrueColorToolBar( 32, IDB_TOOLBAR_ZOOM_N, IDB_TOOLBAR_ZOOM_N, IDB_TOOLBAR_ZOOM_G);

	m_wndToolBar.GetToolBarCtrl().SetState(ID_DRAFT_REAL, TBSTATE_HIDDEN);

	m_wndToolBar.ShowWindow(SW_SHOW);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	FillList();
	OpenSelectedDraft();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDlgDraft, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgDraft)
	ON_EVENT(CDlgDraft, IDC_DDVUECTRL1, 4 /* OnMouseDown */, OnMouseDownDdvuectrl1, VTS_I4 VTS_I4 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgDraft::OnMouseDownDdvuectrl1(long lX, long lY, short nButton) 
{
	// TODO: Add your control notification handler code here
	if( nButton == 1 ) //left
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_PAN));
		m_ddvue.ContextMenuCommand( 1 );//pan
	}
}

BOOL CDlgDraft::Init(CString sStationID, CString sStationName)
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

BOOL CDlgDraft::SearchDraftFiles(CString path)
{
	CFileFind finder; //文件搜索类
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //所有文件
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		
		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // 是目录,进入遍历
		{
			CString str = finder.GetFilePath();
			SearchDraftFiles(str);
		}
		else //是文件
		{
			//得到文件后缀
			CString strFileName = finder.GetFileName();
			int pos = strFileName.ReverseFind('.');
			if(pos != -1)
			{
				CString str = strFileName.Right(strFileName.GetLength() - pos -1);
				if( str.CompareNoCase("dwg") == 0)
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

void CDlgDraft::OnItemchangedListDraftfiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//事实上鼠标点击的选择动作, 会产生三次ITEMCHANGED消息, 
		//1是A的文字背景消失触发，2是A取消选中触发，3是B被选中触发
		//为避免刷新太频繁, 这里只处理被选中消息, 这样处理会漏掉选择了空行的消息
		//所以把选择空行放在NM_CLICK中处理, 对于移动键盘上下键导致的选择到空行不做处理
		
		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
		if(nSelected < 0)
			return;
		
		m_nSelect = nSelected;

		OpenSelectedDraft();

		m_ddvue.GetFocus();
		
	}
	*pResult = 0;
}

void CDlgDraft::OpenSelectedDraft()
{
	m_ddvue.Close();

	VARIANT vPwd,vZoom;
	vPwd.vt = VT_BSTR;
	vPwd.bstrVal = L"";
	vZoom.vt = VT_BOOL;
	vZoom.boolVal = VARIANT_TRUE;
	m_ddvue.Open( m_arrFiles[m_nSelect].m_sPath, 
			vPwd, vZoom );

	m_ddvue.Display();
}

void CDlgDraft::FillList()
{
	for( int i = 0; i < m_arrFiles.size(); i++ )
	{
		m_List.InsertItem( i, m_arrFiles[i].m_sTitle );
	}
	if( m_List.GetItemCount() > 0)
			m_List.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CDlgDraft::OnDraftZoomin() 
{
	// TODO: Add your command handler code here
	m_ddvue.ZoomIn(0.9);	
}

void CDlgDraft::OnDraftZoomout() 
{
	// TODO: Add your command handler code here
	m_ddvue.ZoomOut(1.1);	
}

void CDlgDraft::OnDraftWindow() 
{
	// TODO: Add your command handler code here
	m_ddvue.ZoomExtents();
}
