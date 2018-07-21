// DlgAddOsc.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgAddOsc.h"
#include "DlgFaultReport.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc dialog

extern int g_iStationInfoViewAsc;
CDlgAddOsc::CDlgAddOsc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddOsc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddOsc)
	m_sPath = _T("");
	m_sDir = _T("");
	m_sOldDir = _T("");
	m_nMS = 0;
	//}}AFX_DATA_INIT
	m_pReport = NULL;
	m_nOldSortCol = -1;
	g_iStationInfoViewAsc = -1;
	m_listOSC.RemoveAll();
}


void CDlgAddOsc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddOsc)
	DDX_Control(pDX, IDC_CMB_ADDOSC_SEC, m_cmbSec);
	DDX_Control(pDX, IDC_CMB_ADDOSC_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_LIST_ADDOSC, m_List);
	DDX_Text(pDX, IDC_EDIT_ADDOSC_PATH, m_sPath);
	DDX_Text(pDX, IDC_EDIT_ADDOSC_MS, m_nMS);
	DDV_MinMaxInt(pDX, m_nMS, 0, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddOsc, CDialog)
	//{{AFX_MSG_MAP(CDlgAddOsc)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_SKIN, OnBtnAddoscSkin)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_ADD, OnBtnAddoscAdd)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_REMOVE, OnBtnAddoscRemove)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_MODIFY, OnBtnAddoscModify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ADDOSC, OnItemchangedListAddosc)
	ON_CBN_SELCHANGE(IDC_CMB_ADDOSC_STATION, OnSelchangeCmbAddoscStation)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ADDOSC, OnColumnclickListAddosc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc message handlers

BOOL CDlgAddOsc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    MYASSERT_BOOL(m_pReport);
	//列表
	InitList();
	//厂站
	FillCMBStation();
	//取得厂站
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	CStationObj* pStation = NULL;
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	}

	if ( pStation == NULL || (DWORD)pStation == -1 )
	{
		AfxMessageBox( StringFromID(IDS_COMMON_NEEDSTATION) );
		return FALSE;
	}

	//二次设备
	FillCMBSec(pStation);
	//时间
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
	if(pTime == NULL)
		return FALSE;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	//设置时间
	CTime tm = m_pReport->m_tmFaultTime;
	VERIFY(pTime->SetTime(&tm));
	//毫秒
	m_nMS = m_pReport->m_nmsFaultTime;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: FillCMBStation()
 功能概要: 填充厂站下拉框数据,只填充对应故障归档中的厂站
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgAddOsc::FillCMBStation()
{
	//检查数据有效性
	if(m_pReport == NULL)
		return;
	//清空旧数据
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	if(pCmb == NULL)
		return;
	pCmb->ResetContent();
	//填充厂站1
	if(m_pReport->m_pStation1 != NULL)
	{
		int nIndex = pCmb->AddString(m_pReport->m_pStation1->m_sName);
		pCmb->SetItemData(nIndex, (DWORD)m_pReport->m_pStation1);
	}
	//填充厂站2
	if(m_pReport->m_pStation2 != NULL)
	{
		int nIndex = pCmb->AddString(m_pReport->m_pStation2->m_sName);
		pCmb->SetItemData(nIndex, (DWORD)m_pReport->m_pStation2);
	}
	//默认选择第一个
	if(pCmb->GetCount() > 0)
		pCmb->SetCurSel(0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: FillCMBSec()
 功能概要: 填充指定厂站的二次设备数据到下拉框
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 
**************************************************************/
void CDlgAddOsc::FillCMBSec( CStationObj* pStation )
{
	//检查数据有效性
	MYASSERT(pStation);
	//清空旧数据
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	MYASSERT(pCmb);
	pCmb->ResetContent();
	//填充数据
	//循环厂站下的保护和录波器
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj * pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			continue;
		//加入到下拉框
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex < 0)
			continue;
		//设置关联
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
	
	//默认选择第一个
	if(pCmb->GetCount() > 0)
		pCmb->SetCurSel(0);
	m_cmbSec.OnDataFilled(TRUE, 0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: InitList()
 功能概要: 初始化列表
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDlgAddOsc::InitList()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	
	//分配列宽,最小需求
	for (int nCol=0; nCol < 3 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 300; // 二次设备
			colName = StringFromID(IDS_MODEL_SECONDARY);
			break;
		case 1://
			lvCol.cx = 160; // 时间
			colName = StringFromID(IDS_COMMON_FAULTTIME);
			break;
		case 2://
			lvCol.cx = 180; // 文件名
			colName = StringFromID(IDS_COMMON_FILENAME);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: AddEventToList()
 功能概要: 添加指定事件到列表
 返 回 值: 添加成功返回TRUE,失败返回FALSE
 参    数: param1 指定事件
		   Param2 添加到列表的位置
**************************************************************/
BOOL CDlgAddOsc::AddEventToList( CFaultEvent* pEvent, int nIndex )
{
	//检查数据有效性
	if(pEvent == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	//"二次设备","故障时间","文件名称"
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str.Format("%s", pEvent->m_pSec->m_sName);
	//0:二次设备
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:时间
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 1, str);
	//2:事件描述
	m_List.SetItemText(nIndex, 2, pEvent->m_sEventDef);
	//关联设备
	m_List.SetItemData(nIndex, (DWORD)pEvent);

	return TRUE;
}

CDlgAddOsc::~CDlgAddOsc()
{
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		delete pEvent;
	}
	m_listOSC.RemoveAll();	
}

void CDlgAddOsc::OnBtnAddoscSkin() 
{
	// TODO: Add your control notification handler code here
	//取得当前选择的二次设备
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	MYASSERT(pCmb);
	int nIndex = pCmb->GetCurSel();
	CSecObj* pSec = (CSecObj*)pCmb->GetItemData(nIndex);
	MYASSERT(pSec);
	if(pSec->m_nType != TYPE_PROTECT && pSec->m_nType != TYPE_WAVEREC)
		return;
	//取得当前系统路径
	DWORD cchCurDir = MAX_PATH; 
    LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH];
	for(int i = 0; i < MAX_PATH; i++)
	{
		tchBuffer[i] = '\0';
	}
	lpszCurDir = tchBuffer; 
    GetCurrentDirectory(cchCurDir, lpszCurDir);

	//指定路径
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strSTDown = pApp ->GetDownDir();

	m_sDir = "";
	m_sOldDir = "";

	if(pSec->m_nType == TYPE_PROTECT)
	{
		m_sDir = pSec->m_sStationID + "\\保护\\" + pSec->m_sName + "(" + pSec->m_sID + ")" + "\\";
        strSTDown += m_sDir;
	}
	else if(pSec->m_nType == TYPE_WAVEREC)
	{
		m_sDir += pSec->m_sStationID + "\\录波器\\" + pSec->m_sName + "(" + pSec->m_sID + ")" + "\\";
		strSTDown += m_sDir;
	}

	//打开选择文件对话框,让用户选择文件
	static char szFilter[] = "CFG Files(*.cfg)|*.cfg|All Files(*.*)|*.*||";
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlgFile.m_ofn.lpstrTitle = StringFromID(IDS_ADDOSC_SELECTFILE); //对话框标题
	//dlgFile.m_ofn.lpstrFilter =_T("*.cfg"); //过滤
	dlgFile.m_ofn.lpstrInitialDir = strSTDown;
	if(dlgFile.DoModal() == IDOK)
	{
		//得到全路径
		CString strFullPath = dlgFile.GetPathName();
		//把两个路径都转换成小写再判断
		CString tempFullPath = strFullPath;
		CString tempSTDown = strSTDown;
		tempFullPath.MakeLower();
		tempSTDown.MakeLower();
	
	
#ifdef  NDEBUG
		if(tempFullPath.Find(tempSTDown, 0) == -1) //判断选择的文件是否在STDown目录下
		{
			
			CString str;
			str.Format("%s:\n%s", StringFromID(IDS_ADDOSC_PATHERROR),strSTDown);
			AfxMessageBox(str);
	
		}
		else //在STDown目录下
		{
#endif
			m_sPath = dlgFile.GetFileName();
			CFile file;
			if ( file.Open(dlgFile.GetPathName(), CFile::modeRead) ) 
			{
				//得到文件长度
				m_nFileSize = file.GetLength();
				CString strLog;
				strLog.Format(_T("CDlgAddOsc::OnBtnAddoscSkin 读取文件长度成功，长度为%d"), m_nFileSize);
				WriteLog(strLog, 3);
			}
 
			//去后缀
			int nFind = m_sPath.ReverseFind('.');
			if(nFind >= 0)
			{
				m_sPath = m_sPath.Left(nFind);
			}

			if (g_bSend20162)
			{
				SYSTEMTIME tNow; 
				GetLocalTime(&tNow);
				CString strTime;
				strTime.Format("%d-%02d-%02d %02d.%02d.%02d.%03d", tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
				//旧的相对路径
				m_sOldDir = m_sDir + m_sPath;
				//新的相对路径
				m_sDir += m_sPath;
				if (m_sDir.Find("Collection") == -1)
				{
					m_sDir += strTime;
					m_sDir = "Collection\\" + m_sDir;
				}
			}
			else
			{
				m_sPath = m_sDir + m_sPath;
			}
#ifdef NDEBUG
		}
#endif
	}
	
	//设置当前目录
	SetCurrentDirectory(lpszCurDir);
	UpdateData(FALSE);
}

void CDlgAddOsc::OnBtnAddoscAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//判断是否已存在
	/*
	if(IsExist(m_sPath))
	{
		//已存在
		AfxMessageBox("指定文件已存在于故障归档或待加入队列中!");
		return;
	}
	*/
	//二次设备
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	CSecObj* pObj = (CSecObj*)pCmb->GetItemData(nIndex);
	if(pObj == NULL)
		return;
	if(m_pReport == NULL)
		return;
	//时间
	CDateTimeCtrl* pTM = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
	if(pTM == NULL)
		return;
	CTime tm;
	pTM->GetTime(tm);
	//新建事件
	CFaultEvent* pEvent = new CFaultEvent;
	if (g_bSend20162)
	{
		pEvent->m_sEventDef = m_sOldDir;
	}
	else
	{
		CString sEventDef = m_sPath;
		sEventDef.Replace("/", "\\");
		pEvent->m_sEventDef = sEventDef;
	}
	pEvent->m_nMainRecID = m_pReport->m_nID;
	pEvent->m_tmStart = tm;
	pEvent->m_nmsStart = m_nMS;
	pEvent->m_sSecID = pObj->m_sID;
	pEvent->m_pSec = pObj;
	pEvent->m_nType = 4;
	pEvent->m_nmsSign = m_nMS;
	pEvent->m_nmsReceive = m_nMS;
	pEvent->m_tmSign = tm;
	pEvent->m_tmReceiveTime = tm;
	pEvent->m_sDir = m_sDir;
	pEvent->m_sOldDir = m_sOldDir;
	pEvent->m_sEventContent.Format("%d", m_nFileSize);
	//加入到列表
	m_listOSC.Add(pEvent);
	AddEventToList(pEvent, 0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: IsExist()
 功能概要: 判断指定文件是否已在待加入列表或故障归档的录波列表中
 返 回 值: 已存在返回TRUE,不存在返回FALSE
 参    数: param1 指定文件的名称，不带后缀
		   Param2 
**************************************************************/
BOOL CDlgAddOsc::IsExist( CString sName )
{
	//判断文件是否已加入到待加入列表
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		if(pEvent->m_sEventDef == sName)
		{
			//已存在
			return TRUE;
		}
	}
	if(m_pReport == NULL)
		return FALSE;
	//判断是否在故障归档中存在
	//PTOSC
	for(i = 0; i < m_pReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		CString str = pEvent->m_sEventDef;
		if(str.Find(sName, 0) >= 0)
		{
			//已存在
			return TRUE;
		}
	}
	//WROSC
	for(i = 0; i < m_pReport->m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		CString str = pEvent->m_sEventDef;
		if(str.Find(sName, 0) >= 0)
		{
			//已存在
			return TRUE;
		}
	}
	return FALSE;
}

void CDlgAddOsc::OnBtnAddoscRemove() 
{
	// TODO: Add your control notification handler code here
	//找到在LIST中的选择
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
	//在LIST中删除
	m_List.DeleteItem(nIndex);
	if(pEvent == NULL)
		return;
	//在数组中删除
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pObj = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pObj == NULL)
			continue;
		//比较
		if(pObj == pEvent)
		{
			m_listOSC.RemoveAt(i);
			delete pEvent;
			pEvent = NULL;
			return;
		}
	}
}

void CDlgAddOsc::OnBtnAddoscModify() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgAddOsc::OnItemchangedListAddosc(NMHDR* pNMHDR, LRESULT* pResult) 
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
		
		//得到该行对应的事件
		CFaultEvent * pEvent = (CFaultEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//变更控件值
		//二次设备
		if(pEvent->m_pSec != NULL)
		{
			CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
			if(pCmb != NULL)
			{
				pCmb->SelectString(-1, pEvent->m_pSec->m_sName);
			}
			//厂站
			if(pEvent->m_pSec->m_pStation != NULL)
			{
				pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
				if(pCmb != NULL)
				{
					pCmb->SelectString(-1, pEvent->m_pSec->m_pStation->m_sName);
				}
			}
		}
		//时间
		CDateTimeCtrl* pTM = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
		if(pTM != NULL)
		{
			pTM->SetTime(&pEvent->m_tmStart);
		}
		//毫秒
		m_nMS = pEvent->m_nmsStart;
		//路径
		m_sPath = pEvent->m_sEventDef;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDlgAddOsc::OnOK() 
{
	// TODO: Add extra validation here
	try
	{
		//检查数据有效性
		CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
		if(pParent != NULL)
		{
			//添加
			for(int i = 0; i < m_listOSC.GetSize(); i++)
			{
				CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
				if(pEvent == NULL)
					continue;
				CFaultEvent* pNew = pEvent->Clone();
				if(pNew->m_pSec == NULL)
					continue;
				if(pNew->m_pSec->m_nType == TYPE_PROTECT)
				{
					//保护
					pParent->m_listPTOSC.Add(pNew);
				}
				else if(pNew->m_pSec->m_nType == TYPE_WAVEREC)
				{
					//录波器
					pParent->m_listWROSC.Add(pNew);
				}
				pParent->m_bEventNeedSave = TRUE;
			}
		}
		CDialog::OnOK();
	}
	catch(...)
	{
		WriteLog("CDlgAddOsc::OnOK(), 添加录波文件出错");
		return;
	}
}

void CDlgAddOsc::OnSelchangeCmbAddoscStation() 
{
	// TODO: Add your control notification handler code here
	//取得厂站
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	CStationObj* pStation = NULL;
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	}
	
	if ( pStation == NULL || (DWORD)pStation == -1 )
	{
		AfxMessageBox(StringFromID(IDS_COMMON_NEEDSTATION));
		return;
	}
	
	//二次设备
	FillCMBSec(pStation);
}

void CDlgAddOsc::OnColumnclickListAddosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iStationInfoViewAsc = - g_iStationInfoViewAsc;
	}
	else
	{
		g_iStationInfoViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}
/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CDlgAddOsc::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"二次设备","故障时间","文件名称"
	switch(nCol)
	{
	case 0:	//二次设备

		if(pEvent1->m_pSec != NULL)
			str1.Format("%s", pEvent1->m_pSec->m_sName);
		if(pEvent2->m_pSec != NULL)
			str2.Format("%s", pEvent2->m_pSec->m_sName);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 1: //启动时间
		str1.Format("%s.%03d", pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsStart);
		str2.Format("%s.%03d", pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsStart);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 2: //事件描述
		str1.Format("%s", pEvent1->m_sEventDef);
		str2.Format("%s", pEvent2->m_sEventDef);
// 		if(pEvent1->m_nType == 4 && pEvent2->m_nType == 4)
// 		{
// 			str1 = pEvent1->m_sEventDef;
// 			str2 = pEvent2->m_sEventDef;
// 		}
// 		else
// 		{
// 			str1 = pEvent1->m_sEventDefName;
// 			str2 = pEvent2->m_sEventDefName;
// 		}
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	default:
		iResult = 0;
		break;
	}

	iResult *= g_iStationInfoViewAsc;
	return iResult;
}
