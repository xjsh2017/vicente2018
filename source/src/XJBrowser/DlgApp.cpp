// DlgApp.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgApp dialog


CDlgApp::CDlgApp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgApp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgApp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgApp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgApp)
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgApp, CDialog)
	//{{AFX_MSG_MAP(CDlgApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgApp message handlers

BOOL CDlgApp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgApp::InitListCtrl()
{
	InitListStyle();
	FillListData();
	return 0;
}

int CDlgApp::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;
		//载入配置失败
		char* arColumn[8]={"序号", "版本号","校检码", "维护人","维护人电话", "维护日期","维护内容","备注"};
		for (int nCol=0; nCol < 8 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 40; //列宽50象素 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; //列宽60象素 类别
				colName = StringFromID(IDS_COMMON_VERSION);
				break;
			case 2://
				lvCol.cx = 80; //列宽60象素 故障类型
				colName = StringFromID(IDS_COMMON_CHECKCODE);
				break;
			case 3://
				lvCol.cx = 100; //列宽160象素 保护名称
				colName = StringFromID(IDS_APPMAINTAIN_PERSON);
				break;
			case 4: 
				lvCol.cx = 100; //装置启动时间
				colName = StringFromID(IDS_APPMAINTAIN_TEL);
				break;
			case 5:
				lvCol.cx = 140;
				colName = StringFromID(IDS_APPMAINTAIN_DATE);
				break;
			case 6:
				lvCol.cx = 200;
				colName = StringFromID(IDS_APPMAINTAIN_CONTENT);
				break;
			case 7:
				lvCol.cx = 200; 
				colName = StringFromID(IDS_COMMON_NOTE);
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText = colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	//默认隐藏第一列(序号)
	m_List.SetColumnHide(0, TRUE);
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

int CDlgApp::FillListData()
{
	m_List.DeleteAllItems();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_MAINTENANCE_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgApp::FillListData, 查询失败");
		delete[] sError;
		delete pMemset;
		return -1;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgApp::FillListData, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			m_List.InsertItem(i, pMemset->GetValue((UINT)0));
			m_List.SetItemText(i, 1, pMemset->GetValue((UINT)1));
			m_List.SetItemText(i, 2, pMemset->GetValue((UINT)2));
			m_List.SetItemText(i, 3, pMemset->GetValue((UINT)3));
			m_List.SetItemText(i, 4, pMemset->GetValue((UINT)4));
			m_List.SetItemText(i, 5, pMemset->GetValue((UINT)5));
			m_List.SetItemText(i, 6, pMemset->GetValue((UINT)6));
			m_List.SetItemText(i, 7, pMemset->GetValue((UINT)7));
			pMemset->MoveNext();
		}
		
	}
	else
	{
		CString str;
		str.Format("CViewAction::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return 0;
}
