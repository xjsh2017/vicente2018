// DlgStatusHis.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgStatusHis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis dialog


CDlgStatusHis::CDlgStatusHis(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatusHis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatusHis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_EventManager.ClearAllEvents();
	m_pSec = NULL;
}


void CDlgStatusHis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatusHis)
	DDX_Control(pDX, IDC_LIST_PTSTATUS_HIS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatusHis, CDialog)
	//{{AFX_MSG_MAP(CDlgStatusHis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgStatusHis::~CDlgStatusHis()
{
	//清空
	m_EventManager.ClearAllEvents();
}
/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis message handlers

BOOL CDlgStatusHis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//类型必须是有效值
	if(m_nType != 1 && m_nType != 2)
		return FALSE;
	//设备必须有效
	if(m_pSec == NULL)
		return FALSE;
	//设置窗口名称
	CString strCaption = "";
	if (m_nType == 1)
		strCaption = m_pSec->m_sName + StringFromID(IDS_COMMUSTATUS_HISTORY);
	else if (m_nType == 2)
		strCaption = m_pSec->m_sName + StringFromID(IDS_RUNSTATUS_HISTORY);
	SetWindowText(strCaption);

	//初始化列表
	InitListStyle();
	//查询数据
	QueryData();
	//填充数据
	FillList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名: QueryCommHis()
 功能概要: 查询历史通讯状态
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryCommHis()
{
	//检查数据有效性
	if(m_pSec == NULL)
		return;
	//清空旧数据
	m_EventManager.ClearAllEvents();

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段,所有字段
	
	//条件
	//PT_ID
	Condition condition1;
	str.Format("pt_id='%s'", m_pSec->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgStatusHis::QueryCommHis, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgStatusHis::QueryCommHis, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventPTComm* pEvent = new CXJEventPTComm;
			if(pEvent->LoadFromDB(pMemset))
			{
				//加入到链表
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				delete pEvent;
			}
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgStatusHis::QueryCommHis,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 函 数 名: QueryRunHis()
 功能概要: 查询历史运行状态
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryRunHis()
{
	//检查数据有效性
	if(m_pSec == NULL)
		return;
	//清空旧数据
	m_EventManager.ClearAllEvents();
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段,所有字段
	
	//条件
	//PT_ID
	Condition condition1;
	str.Format("pt_id='%s'", m_pSec->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgStatusHis::QueryRunHis, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgStatusHis::QueryRunHis, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventPTRun* pEvent = new CXJEventPTRun;
			if(pEvent->LoadFromDB(pMemset))
			{
				//加入到链表
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				delete pEvent;
			}
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgStatusHis::QueryRunHis,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 函 数 名: FillList()
 功能概要: 填充列表框
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::FillList()
{
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	if(m_nType == 1) //通讯状态
	{
		CXJEventPTComm * pEvent = NULL;
		EVENT_LIST * pList = m_EventManager.GetEventList();
		if(pList != NULL)
		{
			for(int i = 0; i < pList->GetSize(); i++)
			{
				pEvent = (CXJEventPTComm*)pList->GetAt(i);
				AddPTCommToList(pEvent, i);
			}
		}
	}
	else if(m_nType == 2) //运行状态
	{
		CXJEventPTRun * pEvent = NULL;
		EVENT_LIST * pList = m_EventManager.GetEventList();
		if(pList != NULL)
		{
			for(int i = 0; i < pList->GetSize(); i++)
			{
				pEvent = (CXJEventPTRun*)pList->GetAt(i);
				AddPTRunToList(pEvent, i);
			}
		}
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名: QueryData()
 功能概要: 查询数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryData()
{
	if(m_nType == 1) //通讯状态
	{
		QueryCommHis();
	}
	else if(m_nType == 2) //运行状态
	{
		QueryRunHis();
	}
}

/*************************************************************
 函 数 名: AddPTCommToList()
 功能概要: 将指定事件填充到列表框
 返 回 值: 填充成功返回TRUE,否则返回FALSE
 参    数: param1 指定事件
		   Param2 指定在列表框中的位置
**************************************************************/
BOOL CDlgStatusHis::AddPTCommToList( CXJEventPTComm* pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;

	//1:时间
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 1, sTime);

	//2:状态
	m_List.SetItemText(i, 2, GetCommStatusString(pEvent->m_nStatus));
	
	return TRUE;
}

/*************************************************************
 函 数 名: AddPTRunToList()
 功能概要: 将指定事件填充到列表框
 返 回 值: 填充成功返回TRUE,否则返回FALSE
 参    数: param1 指定事件
		   Param2 指定在列表框中的位置
**************************************************************/
BOOL CDlgStatusHis::AddPTRunToList( CXJEventPTRun* pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:时间
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 1, sTime);
	
	//2:状态
	m_List.SetItemText(i, 2, GetRunStatusString(pEvent->m_nStatus));
	
	return TRUE;
}

/*************************************************************
 函 数 名: InitListStyle()
 功能概要: 初始化列表风格
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	//载入配置失败
	//char* arColumn[3]={"序号", "状态改变时间", "改变后状态"};
	
	//分配列宽,最小需求
	for (int nCol=0; nCol < 3 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0:
			lvCol.cx = 60; //ID
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 180; // 时间
			colName = StringFromID(IDS_STATUS_CHANGE_TIME);
			break;
		case 2://
			lvCol.cx = 100; // 状态
			colName = StringFromID(IDS_STATUS_AFTER_CHANGE);
			break;
		default:
			lvCol.cx=50;//列宽100象素
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	//默认隐藏第一列(序号)
	m_List.SetColumnHide(0, TRUE);

	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}