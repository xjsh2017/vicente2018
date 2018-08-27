// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "DlgOperHis.h"

#include "xjbrowser.h"
#include "DeviceObj.h"
#include "XJTagOutStore.h"
#include "XJUserStore.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOperHis dialog


//##ModelId=49B87BA402E1
CDlgOperHis::CDlgOperHis(CWnd* pParent /*=NULL*/, int nType)
	: CDialog(CDlgOperHis::IDD, pParent), m_sCPU(""), m_sZone("")
{
	//{{AFX_DATA_INIT(CDlgOperHis)
	m_strModify = _T("");
	m_nType = nType;
	m_pObj = NULL;
	//}}AFX_DATA_INIT
}


//##ModelId=49B87BA402E4
void CDlgOperHis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOperHis)
	DDX_Text(pDX, IDC_EDIT_CHECK, m_strModify);
	DDX_Text(pDX, IDC_STATIC_DESC, m_strDESC);
	DDX_Control(pDX, IDC_LIST_PTSET, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOperHis, CDialog)
	//{{AFX_MSG_MAP(CDlgOperHis)
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSET, OnCustomdrawList)
END_MESSAGE_MAP()

void CDlgOperHis::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
    // Take the default processing unless we set this to something else below.
    *pResult = 0;
	
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
        // This is the prepaint stage for a subitem. Here's where we set the
        // item's text and background colors. Our return value will tell 
        // Windows to draw the subitem itself, but it will use the new colors
        // we set here.
        // The text color will cycle through red, green, and light blue.
        // The background color will be light blue for column 0, red for
        // column 1, and black for column 2.
		
        COLORREF crText, crBkgnd;

		int nRowIdx = static_cast<int> (pLVCD->nmcd.dwItemSpec); //行索引
		int nColIdx = pLVCD->iSubItem; //列索引
        
        if ( 12 == nColIdx )
		{
			//值
			CString strValue = m_List.GetItemText(nRowIdx, nColIdx);
			CString strOldValue = m_List.GetItemText(nRowIdx, nColIdx - 1);
			
			//去除两边空格
			strValue.TrimLeft();
			strValue.TrimRight();
			strOldValue.TrimLeft();
			strOldValue.TrimRight();

			if (strValue != strOldValue){
				crText = RGB(255,0,0);
				//crBkgnd = RGB(128,128,255);
				
				pLVCD->clrText = crText;
				//pLVCD->clrTextBk = crBkgnd;
			}
		}

		if (nColIdx == 0){
			crBkgnd = RGB(212,207,200);
			crBkgnd = RGB(224,221,216);
			pLVCD->clrTextBk = crBkgnd;
		}else{
			crBkgnd = RGB(244,244,244);
			pLVCD->clrTextBk = crBkgnd;
		}

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
	}
}

BOOL CDlgOperHis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitListStyle();
	
	UpdateLabels();
	
	// TODO: Add extra initialization here
	if (m_nType ==0)
	{
		SetWindowText("操作历史记录");
	}
	else if (m_nType ==1)
	{
		SetWindowText("操作历史记录：定值（区）修改");
	}
	else if (m_nType ==2)
	{
		SetWindowText("操作历史记录：软压板投退");
	}
	else
	{
		SetWindowText("操作历史记录");
	}

	FillData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOperHis::UpdateLabels()
{
	if (m_pObj == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;

	m_strDESC.Format("装置：[ %s ] [ %s ] [ %s ]"
		, m_pObj->m_pStation->m_sName, m_pObj->m_sID, m_pObj->m_sName);
	UpdateData(FALSE);
}

int CDlgOperHis::InitListStyle()
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
	{
		//载入配置失败
		//char* arColumn[11]={"序号","定值名称", "定值代码", "单位", "基准值", "组号", "条目号","步长", "最小值/最大值", "精度", "数据类型"};
		//分配列宽,最小需求
		for (int nCol=0; nCol < 12 ; nCol++)
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
				lvCol.cx = 50; // 人工操作历史ID
				colName = "ID";
				break;
			case 2://
				lvCol.cx = 140; // 操作时间
				colName = "操作时间";
				break;
			case 3:
				lvCol.cx = 190; // 用户操作
				colName = "用户操作";
				break;
			case 4:
				lvCol.cx = 60; // 操作类型
				colName = "操作类型";
				break;
			case 5:
				lvCol.cx = 60; // 用户名
				colName = "用户名";
				break;
			case 6:
				lvCol.cx = 100; // 用户所有者
				colName = "所有者";
				break;
			case 7:
				lvCol.cx = 100; // 用户组
				colName = "用户组";
				break;
			case 8:
				lvCol.cx = 80; // 操作对象
				colName = "操作对象";
				break;
			case 9://
				lvCol.cx = 250; // 操作信息
				colName = "人工操作信息";
				break;
			case 10:
				lvCol.cx = 80; // 计算机名
				colName = "计算机名";
				break;
			case 11:
				lvCol.cx = 60; // 操作结果
				colName = "操作结果";
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		//m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(1, TRUE);
		m_List.SetColumnHide(4, TRUE);
		m_List.SetColumnHide(8, TRUE);
		//m_List.SetColumnHide(10, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgOperHis::FillData()
{
	WriteLog("CDlgOperHis::FillData, 开始", XJ_LOG_LV3);

	CXJUserStore *pUsrStore = CXJUserStore::GetInstance();
	pUsrStore->ReLoad();
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper);  
	//int nGroupCount = m_arrGroup.GetSize();

	CString str;
	CXJBrowserApp*  pApp = (CXJBrowserApp*)AfxGetApp();
	CDBEngine&		dbEngine = pApp->m_DBEngine;
	
	char * sError = NULL;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	CMemSet	mem;
	int nResult;

	QByteArray baSQL;
	if (m_nType == 1){
		baSQL << "SELECT 1,id,time,func,opertype,username,'','',act,msg,computer,operresult "
			<< " FROM tb_operation WHERE ACT IN ('" << m_pObj->m_sID << "')"
			<< " AND (opertype BETWEEN " << XJ_OPER_UNHANGOUT << " AND " << XJ_OPER_HANGOUT << " OR "
			<< " opertype BETWEEN " << XJ_TAGOUT_PTVALVSET << " AND " << XJ_OPER_PTZONESET_STATE_5 << " )"
			<< " AND FUNC like '定值%'"
			<< " ORDER BY time DESC";
	}else if (2 == m_nType){
		baSQL << "SELECT 1,id,time,func,opertype,username,'','',act,msg,computer,operresult "
			<< " FROM tb_operation WHERE ACT IN ('" << m_pObj->m_sID << "')"
			<< " AND (opertype BETWEEN " << XJ_OPER_UNHANGOUT << " AND " << XJ_OPER_HANGOUT << " OR "
			<< " opertype BETWEEN " << XJ_TAGOUT_PTSOFTSET << " AND " << XJ_OPER_PTSOFTSET_STATE_5 << " )"
			<< " AND FUNC like '软压板%'"
			<< " ORDER BY time DESC";
	}
	
	WriteLog(baSQL.constData());
	//AfxMessageBox(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, baSQL.constData(), baSQL.count());
	memset(sError, '\0', MAXMSGLEN);
	
	try
	{
		nResult = dbEngine.XJExecuteSql(MutiSql, sError, &mem);
	}
	catch (...)
	{
		str.Format("CDlgOperHis::FillData, 查询失败");
		WriteLog(str);

		DELETE_POINTERS(sError);
		//AfxMessageBox(str);
		
		return;
	}
	
	if(nResult == 1)
	{	
		mem.MoveFirst();
		int nCount = mem.GetMemRowNum();

		CString str;
		str.Format("CDlgOperHis::FillData, 查询到%d条记录", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		//EnterCriticalSection(&m_CriticalOper);
		int nIndex = 0;
		for(int i = 0; i < nCount; i++)
		{
			CString str;

			str.Format("%d", nCount - nIndex);
			m_List.InsertItem(nIndex, str);

			for (int j = 0; j < 12; j++){
				if (0 == j){
					str.Format("%d", nCount - nIndex);
				}else if (11 == j){
					str = mem.GetValue((UINT)j);
					if (atoi(str) == 0){
						str = "成功";
					}else{
						str = "失败";
					}
				}else{
					str = mem.GetValue((UINT)j);
				}

				m_List.SetItemText(nIndex, j, str);
			}

			QByteArray user_id = mem.GetValue((UINT)5);
			QByteArray &owner = pUsrStore->GetUserOwner(user_id.constData());
			m_List.SetItemText(nIndex, 6, owner.constData());
			QByteArray &group = pUsrStore->GetUserGroupName(user_id);
			m_List.SetItemText(nIndex, 7, group.constData());
			
			nIndex++;
			
			mem.MoveNext();
		}
	}
	else
	{
		str.Format("CDlgOperHis::FillData, 查询失败, 原因为%s", sError);
		WriteLog(str);
		//AfxMessageBox(str);
	}
	DELETE_POINTERS(sError);
	
	mem.FreeData(true);

	//恢复刷新
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	WriteLog("CDlgOperHis::FillData, 结束", XJ_LOG_LV3);
}
