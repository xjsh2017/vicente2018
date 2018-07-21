// DlgEventSignal.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgEventSignal.h"
#include "DataEngine.h"
#include "DeviceObj.h"
#include "PrintListCtrl.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           升降序作用位*/
int	g_iEventSignalAsc;
/////////////////////////////////////////////////////////////////////////////
// CDlgEventSignal dialog


//##ModelId=49B87BA40223
CDlgEventSignal::CDlgEventSignal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEventSignal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEventSignal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrSignalsOutput.RemoveAll();
	m_nOldSortCol = -1;
	m_colHide = -1;
}


//##ModelId=49B87BA40246
void CDlgEventSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEventSignal)
	DDX_Control(pDX, IDC_LIST_EVENTSIGN, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEventSignal, CDialog)
	//{{AFX_MSG_MAP(CDlgEventSignal)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_EVENTSIGN, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_EVENTSIGN, OnColumnclickListEventsign)
	ON_BN_CLICKED(IDC_BTN_EVENTSIGN_COPY, OnBtnEventsignCopy)
	ON_BN_CLICKED(IDC_BTN_EVENTSIGN_PRINT, OnBtnEventsignPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//##ModelId=49B87BA40225
CDlgEventSignal::~CDlgEventSignal()
{
	m_arrSignalsOutput.RemoveAll();
}

/*************************************************************
 函 数 名：AddSignal()
 功能概要：加入信号量到信号量列表
 返 回 值: void
 参    数：param1	要加入的信号量
		   Param2
**************************************************************/
//##ModelId=49B87BA40249
void CDlgEventSignal::AddSignal( PT_Signal* sign )
{
	//判断数据有效性
	if(sign == NULL)
		return;
	//检查是已存在
	BOOL bExist = FALSE;
// 	for(int i = 0; i < m_arrSignalsOutput.GetSize(); i++)
// 	{
// 		PT_Signal* pExist = (PT_Signal*)m_arrSignalsOutput.GetAt(i);
// 		if(pExist->nID == sign->nID)
// 		{
// 			//已存在
// 			bExist = TRUE;
// 			break;
// 		}
// 	}

	if(bExist == FALSE)
	{
		//不存在,加入
		m_arrSignalsOutput.Add(sign);
	}
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格和列
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA4024B
BOOL CDlgEventSignal::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
  
	for (int iCol = 0; iCol < 7 ; iCol++)
	{
		lvCol.iSubItem = iCol;
		CString colName = "";
		switch(iCol)
		{
		case 0:
			lvCol.cx = 50;
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1:
			lvCol.cx = 50;
			colName = StringFromID(IDS_COMMON_TYPE);
			break;
		case 2:
			lvCol.cx = 120;
			colName = StringFromID(IDS_MODEL_SUBSTATION);
			break;
		case 3:
			lvCol.cx = 200;
			colName = StringFromID(IDS_MODEL_RELAY);
			break;
		case 4:
			lvCol.cx = 150;
			colName = StringFromID(IDS_COMMON_TIME);
			break;
		case 5:
			lvCol.cx = 150;
			colName = StringFromID(IDS_FUN_ACTION);
			break;
		case 6://
			lvCol.cx = 80; //列宽60象素
			colName = StringFromID(IDS_ACTION_CASE);
			break;
		default:
			lvCol.cx = 40;//列宽100象素
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_List.InsertColumn(iCol,&lvCol);
	}

	//默认隐藏序号
	m_List.SetColumnHide(0, TRUE);
	if (m_colHide != -1)
	{
		m_List.SetColumnHide(m_colHide, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return TRUE;
}

/*************************************************************
 函 数 名：InitListData()
 功能概要：初始化列表数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA40251
BOOL CDlgEventSignal::InitListData()
{
	//检查是否有数据
	if(m_arrSignalsOutput.GetSize() <= 0)
	{
		return TRUE;
	}
	//清空列表原有数据
	m_List.DeleteAllItems();
	//加入数据
	for(int i = 0; i < m_arrSignalsOutput.GetSize(); i++)
	{
		PT_Signal* sign = (PT_Signal*)m_arrSignalsOutput.GetAt(i);
		AddSignalToList(sign);
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CDlgEventSignal message handlers

/*************************************************************
 函 数 名：AddSignalToList()
 功能概要：把单项信号量数据加入到列表
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	信号量
		   Param2	要加入的位置
**************************************************************/
//##ModelId=49B87BA40252
BOOL CDlgEventSignal::AddSignalToList( PT_Signal* sign, int nIndex /*= 0*/ )
{
	//检查数据有效性
	if(sign == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	//"序号", "类型","厂站名称","保护名称","时间","动作名称","动作情况"
	//0:序号
	CString sID;
	sID.Format("%d", sign->nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:类别
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
	m_List.SetItemText(nIndex, 1, sID);
	//2:厂站名称
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData != NULL)
	{
		CStationObj* pStation = (CStationObj*)pData->FindDevice(sign->strStationId, TYPE_STATION);
		if(pStation != NULL)
		{
			//取得厂站名
			sign->strStation = pStation->m_sName;
		}
	}
	m_List.SetItemText(nIndex, 2, sign->strStation);
	//3:保护名称
	//sID.Format("%s(%s)", sign->strPT_ID, sign->strPT_Model);
	m_List.SetItemText(nIndex, 3, sign->strPTName);
	//4:时间
	m_List.SetItemText(nIndex, 4, sign->strTime);
	//5:动作名称
	m_List.SetItemText(nIndex, 5, sign->strName);
	//6:动作情况
	if(sign->nValue == 1)
	{
		sID = StringFromID(IDS_CASE_ACTION);
	}
	else
	{
		sID = StringFromID(IDS_CASE_RETURN);
	}
	m_List.SetItemText(nIndex, 6, sID);
	//关联数据
	m_List.SetItemData(nIndex, (DWORD)sign);

	return TRUE;
}

//##ModelId=49B87BA40257
BOOL CDlgEventSignal::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListStyle();
	if(!InitListData())
		return FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：信号动作为1时文字为蓝色
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40261
void CDlgEventSignal::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					//按是否确认, 设置文字颜色
					if(sign->nValue == 1)
					{
						//
						lplvcd->clrText = g_colNotAcked;
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
//##ModelId=49B87BA40238
int CALLBACK CDlgEventSignal::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	PT_Signal* pSign1 = (PT_Signal*)lParam1;
	PT_Signal* pSign2 = (PT_Signal*)lParam2;
	if(pSign1 == NULL || pSign2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","类型","厂站名称","保护名称","时间","动作名称","动作情况"
	switch(nCol)
	{
	case 0: //ID
		iResult = pSign1->nID - pSign2->nID;
		break;
	case 1: //类别
		iResult = pSign1->nEventType - pSign2->nEventType;
		break;
	case 2: //厂站名称
		str1 = pSign1->strStation;
		str2 = pSign2->strStation;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //保护名称
		str1.Format("%s(%s)", pSign1->strPT_ID, pSign1->strPT_Model);
		str2.Format("%s(%s)", pSign2->strPT_ID, pSign2->strPT_Model);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //时间
		str1 = pSign1->strTime;
		str2 = pSign2->strTime;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //动作名称
		str1 = pSign1->strName;
		str2 = pSign2->strName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //动作情况
		iResult = pSign1->nValue - pSign2->nValue;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iEventSignalAsc;
	return iResult;
}

//##ModelId=49B87BA40265
void CDlgEventSignal::OnColumnclickListEventsign(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iEventSignalAsc = - g_iEventSignalAsc;
	}
	else
	{
		g_iEventSignalAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：GetSignalDescribe()
 功能概要：得到信号量的文字描述
 返 回 值: 文字描述
 参    数：param1	信号量
**************************************************************/
//##ModelId=49B87BA40255
CString CDlgEventSignal::GetSignalDescribe( PT_Signal* sign )
{
	//判断数据有效性
	if(sign == NULL)
		return "";
	CString sReturn = "";
	//"类型","厂站名称","保护名称","时间","事件名称","动作情况"
	CString str;
	//类型
	if(sign->nEventType == 0)
		str = "I";
	else if(sign->nEventType == 1)
		str = "II";
	else
		str = "III";
	sReturn += str;
	sReturn += "\t";
	//厂站名称
	sReturn += sign->strStation;
	sReturn += "\t";
	//保护名称
	str.Format("%s(%s)", sign->strPT_ID, sign->strPT_Model);
	sReturn += sign->strPTName;
	sReturn += "\t\t";
	//时间
	sReturn += sign->strTime;
	sReturn += "\t\t";
	//事件名称
	sReturn += sign->strName;
	sReturn += "\t";
	//动作情况
	if(m_colHide != 6)
	{
		if(sign->nValue == 1)
			str = StringFromID(IDS_CASE_ACTION);
		else
			str = StringFromID(IDS_CASE_RETURN);
		sReturn += str;
	}
	return sReturn;
}

//##ModelId=49B87BA40269
void CDlgEventSignal::OnBtnEventsignCopy() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	CString str;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			PT_Signal* pSign = (PT_Signal*)m_List.GetItemData(nIndex);
			str += GetSignalDescribe(pSign);
			str += "\r\n";
		}
	}

	if(OpenClipboard())
	{
		//复制str到剪贴板
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, str.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);		
		lstrcpy(pszDst, str);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}
}

void CDlgEventSignal::OnBtnEventsignPrint() 
{
	// TODO: Add your control notification handler code here
	CPrintListCtrl print;
	print.SetListCtrl(m_List.GetSafeHwnd());
	
	// 调整每列的位置
	print.ResizeColumnsToFitContent(TRUE);
	
	// 是否裁减每行的长度 FALSE 否， TRUE 是
	print.TruncateColumn(TRUE);
	
	// 打印列之间的分割线
	print.PrintColumnSeparator(TRUE);
	
	// 打印行之间的分割线
	print.PrintLineSeparator(TRUE);
	
	// 打印表的名称
	print.PrintHead(TRUE);
	
	// 设置各种字体
	print.SetHeadFont(StringFromID(IDS_HEAD_FONT), 14);
	print.SetFootFont(StringFromID(IDS_DEFAULT_FONT), 12);
	print.SetListFont(StringFromID(IDS_DEFAULT_FONT), 12);
	
	// 设置纸的走向 TRUE,横向；FALSE 纵向
	print.SetPrinterOrientation(FALSE);
    
	BOOL bHide = m_List.GetColumnIsHide(0);
    CString strCa;
	GetWindowText(strCa);
	print.Print(GetSafeHwnd(), strCa, FALSE);
	m_List.SetColumnHide(0, bHide);
}




