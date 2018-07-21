// PTNote.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTNote.h"
#include "DataEngine.h"

#include "DlgPtnote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTNote

IMPLEMENT_DYNCREATE(CPTNote, CViewBase)

CPTNote::CPTNote()
	: CViewBase(CPTNote::IDD)
{
	//{{AFX_DATA_INIT(CPTNote)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPTNote::~CPTNote()
{
	
}

void CPTNote::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTNote)
	DDX_Control(pDX, IDC_LIST_NOTES, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTNote, CViewBase)
	//{{AFX_MSG_MAP(CPTNote)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_NOTES, OnCustomDraw)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NOTES, OnClickListNotes)
	ON_BN_CLICKED(IDC_BTN_NOTE_ADD, OnBtnNoteAdd)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTNote diagnostics

#ifdef _DEBUG
void CPTNote::AssertValid() const
{
	CViewBase::AssertValid();
}

void CPTNote::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTNote message handlers

void CPTNote::OnInitialUpdate() 
{

	RecordRate(IDC_BTN_NOTE_ADD, 0, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	InitListStyle();
}

void CPTNote::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_NOTES, left_client, IDC_BTN_NOTE_ADD, right_client, bottom_client);
}

void CPTNote::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//按事件类型(I, II, III)给定背景色
				if(nItem  != -1)
				{
					
					//第二列，事件类型
					if(nSubItem == 5 || nSubItem ==6)
					{
						
						lplvcd->clrText = RGB(0,0,255);
					}
					
					//间隔
					if(nItem%2 != 0)
					{
						if (g_style == 1)
						{
							lplvcd->clrTextBk = g_ListSpaceColor2;
						}
						else if(g_style == 2)
						{
							lplvcd->clrTextBk = g_ListSpaceColor3;
						}
						else if (g_style == 3)
						{
							lplvcd->clrTextBk = g_ListSpaceColor4;
						}
						else
						{
							lplvcd->clrTextBk = g_ListSpaceColor;
						}
					}
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

void CPTNote::OnPTFrameOpen(WPARAM wParam, LPARAM lParam)
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
	{
		return;
	}
	m_pObj = pObj;
	WriteLogEx("CPTNote::LoadPTNoteDate, 重新载入数据");
	ReFillAll();
}

void CPTNote::OnPTFrameClose(WPARAM wParam, LPARAM lParam)
{
	ClearNoteData();
}

int CPTNote::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	if(FALSE)
	{
		//载入配置
	}
	else
	{
		//载入配置失败
		//char* arColumn[7]={"序号","备注项", "备注内容","创建时间","修改时间","编辑","修改"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 7 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 200; // 备注项
				colName = StringFromID(IDS_PTNOTE_ITEM);
				break;
			case 2://
				lvCol.cx = 300; // 备注内容
				colName = StringFromID(IDS_PTNOTE_CONTENT);
				break;
			case 3:
				lvCol.cx = 180; //创建时间
				colName = StringFromID(IDS_COMMON_CREATETIME);
				break;
			case 4:
				lvCol.cx = 180;//修改时间
				colName = StringFromID(IDS_COMMON_MODIFYTIME);
				break;
			case 5:
				lvCol.cx = 60;
				colName = StringFromID(IDS_COMMON_EDIT);
				break;
			case 6:
				lvCol.cx = 60;
				colName = StringFromID(IDS_COMMON_MODIFY);
				break;
			default:
				lvCol.cx = 60;
				break;
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CPTNote::ReFillAll()
{
	LoadPTNoteDate();
}

BOOL CPTNote::LoadPTNoteDate()
{
	if(m_pObj == NULL)
		return FALSE;

	ClearNoteData();
	m_List.DeleteAllItems();
	
	BOOL bReturn = TRUE;	

	WriteLogEx("CPTNote::LoadPTNoteDate, 开始查询");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//取得数据库内有历史数据的Time列表
	CString str;
		
	//字段
		
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//按CURTIME大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by UPDATE_TIME DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);
				
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(162, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTNote::LoadPTNoteDate, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTNote::LoadPTNoteDate,查询到%d条", nCount);
		WriteLog(strLog, XJ_LOG_LV3);

		for(int i = 0; i < nCount; i++)
		{
			PT_NOTE* note = new PT_NOTE;
			CString str;
			note->id = pMemset.GetValue((UINT)0);
			note->pt_id = pMemset.GetValue(1);
			note->note_name = pMemset.GetValue(2);
			note->note_content = pMemset.GetValue(3);
			CString sTime = pMemset.GetValue(4);
			note->create_time = StringToTime( sTime);
			sTime = pMemset.GetValue(5);
			note->update_time = StringToTime( sTime);
			
			AddNoteToList(note, i);
					
			pMemset.MoveNext();
		}
		
	}
	else
	{
		CString str;
		str.Format("CPTNote::LoadPTNoteDate,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	if(!bReturn)
		return FALSE;

	WriteLogEx("CPTNote::LoadPTNoteDate, 查询结束");
	
	return bReturn;
}

void CPTNote::AddNoteToList(PT_NOTE* note, int row)
{
	if( !note )
		return;
	PT_NOTE* pts = note;
	m_List.InsertItem(row, pts->id);
	m_List.SetItemText(row, 1, pts->note_name);
	m_List.SetItemText(row, 2, pts->note_content);
	m_List.SetItemText(row, 3, pts->create_time.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(row, 4, pts->update_time.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(row, 5, StringFromID(IDS_COMMON_EDIT));
	m_List.SetItemText(row, 6, StringFromID(IDS_COMMON_DELETE));
	m_List.SetItemData(row, (DWORD)pts);
}

void CPTNote::UpdateNoteInList(PT_NOTE* note, int row)
{
	if( !note )
		return;
	PT_NOTE* pts = note;
	m_List.SetItemText(row, 1, pts->note_name);
	m_List.SetItemText(row, 2, pts->note_content);
	m_List.SetItemText(row, 3, pts->create_time.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(row, 4, pts->update_time.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemData(row, (DWORD)pts);
}

void CPTNote::ClearNoteData()
{
	for( int i = 0; i < m_List.GetItemCount(); i++)
	{
		PT_NOTE* note = (PT_NOTE*)m_List.GetItemData(i);
		if( note )
		{
			delete note;
		}
	}
}

BOOL CPTNote::UpdateNoteInDB(PT_NOTE* note)
{
	BOOL bReturn = TRUE;
	
	if(note == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询最近的nNum个记录
	CString str;		
	
	//字段
	
	//Name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "note_name", EX_STTP_DATA_TYPE_STRING, note->note_name);
	
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "note_content", EX_STTP_DATA_TYPE_STRING, note->note_content);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "update_time", EX_STTP_DATA_TYPE_TIME, note->update_time.Format("%Y-%m-%d %H:%M:%S"));

	//条件
	//id
	Condition condition2;
	str.Format("id = %s", note->id);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(162, sql, sError);
	}
	catch (...)
	{
		WriteLog("CPTNote::UpdateNoteInDB, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CPTNote::UpdateNoteInDB,失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

BOOL CPTNote::AddNoteToDB(PT_NOTE* note)
{
	BOOL bReturn = TRUE;
	
	if(note == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询最近的nNum个记录
	CString str;		
	
	//字段
	
	//Name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "pt_id", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sID);
	
	//Name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "note_name", EX_STTP_DATA_TYPE_STRING, note->note_name);

	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "note_content", EX_STTP_DATA_TYPE_STRING, note->note_content);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "create_time", EX_STTP_DATA_TYPE_TIME, note->create_time.Format("%Y-%m-%d %H:%M:%S"));
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "update_time", EX_STTP_DATA_TYPE_TIME, note->update_time.Format("%Y-%m-%d %H:%M:%S"));
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertData(162, sql, sError);
	}
	catch (...)
	{
		WriteLog("CPTNote::AddNoteToDB, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CPTNote::AddNoteToDB,失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

BOOL CPTNote::DeleteNoteInDB(PT_NOTE* note)
{
	BOOL bReturn = TRUE;
	
	if(note == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询最近的nNum个记录
	CString str;		
	
	//字段

	
	//条件
	//id
	Condition condition2;
	str.Format("id = %s", note->id);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(162, sql, sError);
	}
	catch (...)
	{
		WriteLog("CPTNote::DeleteNoteInDB, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CPTNote::DeleteNoteInDB,失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

void CPTNote::OnClickListNotes(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	int nSubItem = pNMListView->iSubItem;
	if( nSelected < 0 )
		return;
	PT_NOTE* note = (PT_NOTE*)m_List.GetItemData(nSelected);

	if( 5 == nSubItem ) //edit
	{
		CDlgPtnote dlg;
		dlg.m_pNote = note;
		if(  dlg.DoModal() == IDOK )
		{
			if( UpdateNoteInDB(note) )
			{
				UpdateNoteInList(note, nSelected);
			}
			else
			{
				AfxMessageBox( StringFromID(IDS_PTNOTE_UPDATE_FAIL));
			}
			
		}
	}

	if( 6 == nSubItem ) //delete
	{
		if( AfxMessageBox( StringFromID(IDS_COMMON_DELETE_CONFIRM), MB_OKCANCEL) == IDOK)
		{
			if( DeleteNoteInDB(note) )
			{
				m_pObj->m_nNoteCount--;
				m_List.DeleteItem(nSelected);
			}
			else
			{
				AfxMessageBox( StringFromID(IDS_PTNOTE_DELETE_FAIL));
			}
			delete note;
		}
	}

	*pResult = 0;
}

void CPTNote::OnBtnNoteAdd() 
{
	// TODO: Add your control notification handler code here
	PT_NOTE* note = new PT_NOTE;
	note->id = "0";
	note->note_name="";
	note->note_content = "";
	note->create_time = CTime::GetCurrentTime();
	CDlgPtnote dlg;
	dlg.m_pNote = note;
	if(  dlg.DoModal() == IDOK )
	{
		//save to db
		if( AddNoteToDB(note) )
		{
			m_pObj->m_nNoteCount++;
			AddNoteToList(note, 0);
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_PTNOTE_CREATE_FAIL));
		}
		
	}
}
