// DlgSignalList.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgSignalList.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           升降序作用位*/
int g_iSignalListAsc;
/////////////////////////////////////////////////////////////////////////////
// CDlgSignalList dialog


//##ModelId=49B87BA3039B
CDlgSignalList::CDlgSignalList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSignalList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSignalList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOldSortCol = -1;
}


//##ModelId=49B87BA303BF
void CDlgSignalList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSignalList)
	DDX_Control(pDX, IDC_TAB_SIGNAL_LIST, m_Tab);
	DDX_Control(pDX, IDC_LIST_SIGNAL_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSignalList, CDialog)
	//{{AFX_MSG_MAP(CDlgSignalList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SIGNAL_LIST, OnSelchangeTabSignalList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SIGNAL_LIST, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SIGNAL_LIST, OnColumnclickListSignalList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSignalList message handlers

//##ModelId=49B87BA303C9
BOOL CDlgSignalList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pObj == NULL)
		return FALSE;

	//设置TAB
	m_Tab.InsertItem(0, StringFromID(IDS_SIGNAL_ACTION));
	m_Tab.InsertItem(1, StringFromID(IDS_SIGNAL_ALARM));

	//设置窗口文字
	CString strPTName;
	strPTName = m_pObj->m_sName;
	CString strCaption;
	strCaption = strPTName + StringFromID(IDS_SIGNAL_LIST);
	SetWindowText(strCaption);
	//list
	InitListStyle();
	//先显示动作信号量
	InitActionSignalData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括列数,列名,列属性
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA303B9
void CDlgSignalList::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	//char* arColumn[4] = {"序号","信号量名称","信号量代码","信号量类别"};//"strCodeName"
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	for (int iCol = 0; iCol < 4 ; iCol++)
	{
		lvCol.iSubItem = iCol;
		CString colName = "";
		switch(iCol)
		{
		case 0:
			lvCol.cx = 50;
			colName = StringFromID( IDS_COMMON_NO );
			break;
		case 1:
			lvCol.cx = 165;
			colName = StringFromID( IDS_SIGNAL_NAME);
			break;
		case 2:
			lvCol.cx = 160;
			colName = StringFromID( IDS_SIGNAL_CODE);
			break;
		case 3:
			lvCol.cx = 100;
			colName = StringFromID( IDS_SIGNAL_TYPE);
			break;
		default:
			lvCol.cx = 40;//列宽100象素
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_List.InsertColumn(iCol,&lvCol);
	}
	
	//默认隐藏序号
	m_List.SetColumnHide(0, TRUE);
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}

/*************************************************************
 函 数 名：AddSignalToList()
 功能概要：把信号量加入到列表中显示
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	要加入显示的信号量
		   param2	要加入的位置
**************************************************************/
//##ModelId=49B87BA303BA
BOOL CDlgSignalList::AddSignalToList( PT_Signal* sign, int nIndex)
{
	//判断数据有效性
	if(sign == NULL)
		return FALSE;
	if(sign < 0)
		return FALSE;
	//"序号","信号量名称","信号量代码","信号量类别"
	//0:序号
	CString sID;
	sID.Format("%d", sign->nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:信号量名称
	m_List.SetItemText(nIndex, 1, sign->strName);
	//2:信号量代码
	m_List.SetItemText(nIndex, 2, sign->strCodeName);
	//3:信号量类别
	if(sign->nEventType == 0)
	{
		sID = "I";
	}
	else if(sign->nEventType == 1)
	{
		sID = "II";
	}
	else
	{
		//默认为III
		sID = "III";
	}
	m_List.SetItemText(nIndex, 3, sID);
	//关联数据
	m_List.SetItemData(nIndex, (DWORD)sign);
	return TRUE;
}

/*************************************************************
 函 数 名：InitActionSignalData()
 功能概要：初始化动作信号量数据
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA303BD
void CDlgSignalList::InitActionSignalData()
{
	//检查数据有效性
	if(m_pObj == NULL)
		return;
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//清空原有数据
	m_List.DeleteAllItems();
	m_pObj->RefreshConfig();
	//循环动作信号链表增加数据显示
	for(int i = 0; i < m_pObj->GetActionSignalCount(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pObj->QueryActionSignal(i);
		AddSignalToList(pSign);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名：InitAlarmSignalData()
 功能概要：初始化告警数据信号量显示
 返 回 值: void
**************************************************************/
//##ModelId=49B87BA303BE
void CDlgSignalList::InitAlarmSignalData()
{
	//检查数据有效性
	if(m_pObj == NULL)
		return;
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//清空原有数据
	m_List.DeleteAllItems();
	//循环动作信号链表增加数据显示
	for(int i = 0; i < m_pObj->GetAlarmSignalCount(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pObj->QueryAlarmSignal(i);
		AddSignalToList(pSign);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

//##ModelId=49B87BA303CB
void CDlgSignalList::OnSelchangeTabSignalList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_Tab.GetCurSel();
	if(nIndex == 0)
	{
		//动作信号
		InitActionSignalData();
	}
	else if(nIndex == 1)
	{
		//告警信号量
		InitAlarmSignalData();
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置I,II,III类信号量文字颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA303CF
void CDlgSignalList::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				if(nItem  != -1)
				{
					//得到对象
					PT_Signal * sign = (PT_Signal*)m_List.GetItemData(nItem);
					//设置文字颜色
					if(sign->nEventType == 0)
					{
						//I
						lplvcd->clrText = g_colEventI;
					}
					else if(sign->nEventType == 1)
					{
						//II
						lplvcd->clrText = g_colEventII;
					}
					else
					{
						//III
						lplvcd->clrText = g_colEventIII;
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

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87BA303AF
int CALLBACK CDlgSignalList::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	PT_Signal* pSign1 = (PT_Signal*)lParam1;
	PT_Signal* pSign2 = (PT_Signal*)lParam2;
	if(pSign1 == NULL || pSign2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","信号量名称","信号量代码","信号量类别"
	switch(nCol)
	{
	case 0: //ID
		iResult = pSign1->nID - pSign2->nID;
		break;
	case 1: //信号量名称
		str1 = pSign1->strName;
		str2 = pSign2->strName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //信号量代码
		str1 = pSign1->strCodeName;
		str2 = pSign2->strCodeName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //类别
		iResult = pSign1->nEventType - pSign2->nEventType;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iSignalListAsc;
	return iResult;
}

//##ModelId=49B87BA303D8
void CDlgSignalList::OnColumnclickListSignalList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iSignalListAsc = - g_iSignalListAsc;
	}
	else
	{
		g_iSignalListAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}
