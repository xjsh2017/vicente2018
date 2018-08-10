// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgDataCheck.h"

#include "XJTagOutStore.h"
#include "qptsetstatetable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDataCheck dialog


//##ModelId=49B87BA402E1
CDlgDataCheck::CDlgDataCheck(CWnd* pParent /*=NULL*/, int nType, int nTagOutType)
	: CDialog(CDlgDataCheck::IDD, pParent), m_sCPU(""), m_sZone("")
{
	//{{AFX_DATA_INIT(CDlgDataCheck)
	m_strModify = _T("");
	m_nType = nType;
	//}}AFX_DATA_INIT

	m_nTagOutType = nTagOutType;
}


//##ModelId=49B87BA402E4
void CDlgDataCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataCheck)
	DDX_Text(pDX, IDC_EDIT_CHECK, m_strModify);
	DDX_Text(pDX, IDC_STATIC_DESC, m_strDESC);
	DDX_Control(pDX, IDC_LIST_PTSET, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataCheck, CDialog)
	//{{AFX_MSG_MAP(CDlgDataCheck)
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSET, OnCustomdrawList)
END_MESSAGE_MAP()

void CDlgDataCheck::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTZoneDataTable *pPTZoneData = pTagOutStore->GetPTZoneData();
	QPTSetDataTable *pPTSetData = pTagOutStore->GetPTSetData();
	
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
		
        COLORREF crText/*, crBkgnd*/;

		int nRowIdx = static_cast<int> (pLVCD->nmcd.dwItemSpec); //行索引
		int nColIdx = pLVCD->iSubItem; //列索引
        
		int nCols = m_List.GetHeaderCtrl()->GetItemCount();
        if ( nColIdx == nCols - 1 )
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

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgDataCheck message handlers

BOOL CDlgDataCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTZoneDataTable *pPTZoneData = pTagOutStore->GetPTZoneData();
	QPTSetDataTable *pPTSetData = pTagOutStore->GetPTSetData();
	
	// TODO: Add extra initialization here
	
	UpdateLabels1();
	InitListStyle1();
	FillData1();

	if (m_nType ==0)
	{
		SetWindowText( StringFromID(IDS_CHECK_RUNNER));
		if (m_nTagOutType == XJ_TAGOUT_PTVALVSET)
			pPTSetData->ReLoad();
		else if (m_nTagOutType == XJ_TAGOUT_PTZONESET)
			pPTZoneData->ReLoad();
	}
	else if (m_nType ==1)
	{
		SetWindowText( StringFromID(IDS_CHECK_GUARDIAN));
		if (m_nTagOutType == XJ_TAGOUT_PTVALVSET)
			pPTSetData->ReLoad();
		else if (m_nTagOutType == XJ_TAGOUT_PTZONESET)
			pPTZoneData->ReLoad();
	}
	else if (m_nType ==2)
	{
		SetWindowText( StringFromID(IDS_CHECK_OPERATOR));
	}
	else
	{
		SetWindowText( StringFromID(IDS_CHECK_DEFAULT));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgDataCheck::UpdateLabels1()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTZoneDataTable *pPTZoneData = pTagOutStore->GetPTZoneData();
	QPTSetDataTable *pPTSetData = pTagOutStore->GetPTSetData();
	pTagOutStore->ReLoadState();
	
	int nPTSetState = pTagOutState->GetStateID();
	
	if (m_sCPU.IsEmpty())
		m_sCPU = QByteArray::number(pTagOutState->GetCPUID()).constData();

	if (m_sZone.IsEmpty())
		m_sZone = QByteArray::number(pTagOutState->GetZoneID()).constData();
	
	CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pTagOutState->GetPTID().constData(), TYPE_SEC);

	if (m_nTagOutType == XJ_TAGOUT_PTVALVSET){
		m_strDESC.Format("装置[%s]在[%s]号CPU[%s]号定值区上的定值将做如下更改："
		, pObj->m_sName, m_sCPU, m_sZone);
	}else if (m_nTagOutType == XJ_TAGOUT_PTZONESET){
		m_strDESC.Format("装置[%s]在[%s]号CPU上的定值区将做如下更改："
		, pObj->m_sName, m_sCPU);
	}
	//AfxMessageBox(m_strDESC);
	UpdateData(FALSE);
}

int CDlgDataCheck::InitListStyle1()
{
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTZoneDataTable *pPTZoneData = pTagOutStore->GetPTZoneData();
	QPTSetDataTable *pPTSetData = pTagOutStore->GetPTSetData();
	
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
		int nCols = 0;
		QByteArrayMatrix showCols;
		QByteArrayMatrix lenCols;
		if (m_nTagOutType == XJ_TAGOUT_PTVALVSET){
			nCols = pPTSetData->GetFieldCount() + 3;	// 增加3列： 序号、原值、新值
			
			showCols << "0,3,7"
				<< "," << nCols - 2 
				<< "," << nCols - 1;

			lenCols << "50,60,180,80,80";
			
			for (int iCol = 0; iCol < nCols; iCol++){
				lvCol.cx = 80;
				
				int nShowColsIdx = showCols.valueIndexOf(iCol);
				if (nShowColsIdx > 0){
					lvCol.cx = lenCols.GetFieldValue(1, nShowColsIdx).toInt();
				}
				
				if (iCol == 0){
					lvCol.pszText = "序号";
				}else if (iCol == nCols - 2){
					lvCol.pszText = "原值";
				}else if (iCol == nCols - 1){
					lvCol.pszText = "新值";
				}else{
					lvCol.pszText = pPTSetData->GetFieldName(iCol, XJ::LANG_CHS).data();
				}
				int result = m_List.InsertColumn(iCol, &lvCol);
				m_List.SetColumnHide(iCol, TRUE);

				if (showCols.valueContains(iCol))
					m_List.SetColumnHide(iCol, FALSE);
			}
		}else if (m_nTagOutType == XJ_TAGOUT_PTZONESET){
			nCols = pPTZoneData->GetFieldCount() + 3;  // 增加3列： 序号、原区号、新区号
			
			showCols << "0,3,4"
				<< "," << nCols - 2 
				<< "," << nCols - 1;
			
			lenCols << "50,80,180,80,80";

			//AfxMessageBox(showCols.constData());

			for (int iCol = 0; iCol < nCols; iCol++){
				lvCol.cx = 80;
				
				int nShowColsIdx = showCols.valueIndexOf(iCol);
				if (nShowColsIdx > 0){
					lvCol.cx = lenCols.GetFieldValue(1, nShowColsIdx).toInt();
				}

				if (iCol == 0){
					lvCol.pszText = "序号";
				}else if (iCol == nCols - 2){
					lvCol.pszText = "原区号";
				}else if (iCol == nCols - 1){
					lvCol.pszText = "新区号";
				}else{
					lvCol.pszText = pPTZoneData->GetFieldName(iCol, XJ::LANG_CHS).data();
				}
				int result = m_List.InsertColumn(iCol, &lvCol);
				m_List.SetColumnHide(iCol, TRUE);

				if (showCols.valueContains(iCol))
					m_List.SetColumnHide(iCol, FALSE);
			}
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgDataCheck::FillData1()
{
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTZoneDataTable *pPTZoneData = pTagOutStore->GetPTZoneData();
	QPTSetDataTable *pPTSetData = pTagOutStore->GetPTSetData();
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper);  
	int nIndex = 0;
	int nRowCount = 0;
	int nFieldCount = 0;
	CString str;
	if(m_nTagOutType == XJ_TAGOUT_PTVALVSET){
		nRowCount = pPTSetData->GetRowCount();
		nFieldCount = pPTSetData->GetFieldCount() + 3;
		
		nIndex = 0;
		for(int i = 1; i <= nRowCount; i ++)
		{
			str.Format("%d", nIndex + 1);
			m_List.InsertItem(nIndex, str); //ID
			
			for (int j = 1; j < nFieldCount; j++){
				str = pPTSetData->GetFieldValue(i, j).constData();
				m_List.SetItemText(nIndex, j, str);
				
				if (j == nFieldCount - 2){
					QByteArrayMatrix val = pPTSetData->GetFieldValue(i, "reserve3");
					m_List.SetItemText(nIndex, j, val.GetFieldValue(1, 1).constData());
				}else if(j == nFieldCount - 1){
					QByteArrayMatrix val = pPTSetData->GetFieldValue(i, "reserve3");
					QByteArray s = val.GetFieldValue(1, 2);
					if (s.isEmpty())
						s = val.GetFieldValue(1, 1);
					m_List.SetItemText(nIndex, j, s.constData());
				}
			}
			
			m_List.SetItemData(nIndex, (DWORD)NULL);
			
			
			nIndex++;
		}
	}else if (m_nTagOutType == XJ_TAGOUT_PTZONESET){
		nRowCount = pPTZoneData->GetRowCount();
		nFieldCount = pPTZoneData->GetFieldCount() + 3;

		nIndex = 0;
		for(int i = 1; i <= nRowCount; i ++)
		{
			str.Format("%d", nIndex + 1);
			m_List.InsertItem(nIndex, str); //ID

			for (int j = 1; j < nFieldCount; j++){
				str = pPTZoneData->GetFieldValue(i, j).constData();
				m_List.SetItemText(nIndex, j, str);

				if (j == nFieldCount - 2){
					QByteArrayMatrix val = pPTZoneData->GetFieldValue(i, "reserve3");
					m_List.SetItemText(nIndex, j, val.GetFieldValue(1, 1).constData());
				}else if(j == nFieldCount - 1){
					QByteArrayMatrix val = pPTZoneData->GetFieldValue(i, "reserve3");
					QByteArray s = val.GetFieldValue(1, 2);
					if (s.isEmpty())
						s = val.GetFieldValue(1, 1);
					m_List.SetItemText(nIndex, j, s.constData());
				}
			}

			m_List.SetItemData(nIndex, (DWORD)NULL);
			
			
			nIndex++;
		}
	}
	
	//恢复刷新
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	//WriteLog("CDlgDataCheck::FillListData, 结束", XJ_LOG_LV3);
}

void CDlgDataCheck::UpdateLabels()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;

	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	pTagOutStore->ReLoadState();
	
	int nPTSetState = pTagOutState->GetStateID();

	if (nPTSetState != 0){
		m_sCPU = QByteArray::number(pTagOutState->GetCPUID()).constData();
		m_sZone = QByteArray::number(pTagOutState->GetZoneID()).constData();
	}
	
	CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pTagOutState->GetPTID().constData(), TYPE_SEC);
	m_strDESC.Format("装置[%s]在[%s]号CPU[%s]号定值区上的定值将做如下更改："
		, pObj->m_sName, m_sCPU, m_sZone);
	//AfxMessageBox(m_strDESC);
	UpdateData(FALSE);
}

int CDlgDataCheck::InitListStyle()
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
		for (int nCol=0; nCol < 13 ; nCol++)
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
				lvCol.cx = 140; // 定值名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // 定值代码
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80; //单位
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 80; //基准值
				colName = StringFromID(IDS_SETTING_ORDER);
				break;
			case 5:
				lvCol.cx = 60; //组号
				colName = StringFromID(IDS_POINT_GROUP);
				break;
			case 6:
				lvCol.cx = 60; //条目号
				colName = StringFromID(IDS_POINT_ITEM);
				break;
			case 7:
				lvCol.cx = 60; //步长
				colName = StringFromID(IDS_POINT_STEP);
				break;
			case 8:
				lvCol.cx = 80; //最小值/最大值
				colName = StringFromID(IDS_POINT_MAXMIN);
				break;
			case 9:
				lvCol.cx = 60; //精度
				colName = StringFromID(IDS_POINT_PRECISION);
				break;
			case 10:
				lvCol.cx = 80; //数据类型
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			case 11:
				lvCol.cx = 80; //原值
				colName = StringFromID(IDS_COLUMN_VALUE_0);
				break;
			case 12:
				lvCol.cx = 80; //更改值
				colName = StringFromID(IDS_COLUMN_VALUE_1);
				break;
			/*case 13:
				lvCol.cx = 80; //定值
				colName = StringFromID(IDS_COLUMN_VALUE_2);
				break;*/
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		//m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(定值代码)
		m_List.SetColumnHide(2, TRUE);
		//默认隐藏第5列(基准值)
		m_List.SetColumnHide(4, TRUE);
		//if(!g_PTControl)
		{
			m_List.SetColumnHide(3, TRUE);
			m_List.SetColumnHide(5, TRUE);
			m_List.SetColumnHide(6, TRUE);
			m_List.SetColumnHide(7, TRUE);
			m_List.SetColumnHide(8, TRUE);
			m_List.SetColumnHide(9, TRUE);
			m_List.SetColumnHide(10, TRUE);
			/*if (0 == m_nType){
				m_List.SetColumnHide(11, TRUE);
				m_List.SetColumnHide(12, TRUE);
			}else{
				m_List.SetColumnHide(13, TRUE);
			}*/
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgDataCheck::FillData()
{
	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pState = pStore->GetState();
	QPTSetDataTable *pPTSetData = pStore->GetPTSetData();
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper);  
	int nIndex = 0;
	int nRowCount = pPTSetData->GetRowCount();
	for(int i = 0; i <nRowCount; i ++)
	{
		CString str;
		str = pPTSetData->GetFieldValue(i + 1, "setting_id").constData();
		if(1 == g_PTIndexType){
			str.Format("%d", nIndex+1);
		}
		m_List.InsertItem(nIndex, str); //ID
		str = pPTSetData->GetFieldValue(i + 1, "name").constData();
		m_List.SetItemText(nIndex, 1, str); //名称
		str = pPTSetData->GetFieldValue(i + 1, "code_name").constData();
		m_List.SetItemText(nIndex, 2, str); //代码
		str = pPTSetData->GetFieldValue(i + 1, "unit").constData();
		m_List.SetItemText(nIndex, 3, str); //单位
		//str = pPTSetData->GetFieldValue(i + 1, "unit").constData();
		m_List.SetItemText(nIndex, 4, ""); //基准值
		//str.Format("%d", pts->Group);
		m_List.SetItemText(nIndex, 5, "");
		//str.Format("%d", pts->Item);
		m_List.SetItemText(nIndex,6, "");
		str = pPTSetData->GetFieldValue(i + 1, "stepvalue").constData();
		str.Format("%s", str);
		m_List.SetItemText(nIndex, 7, str);
		QByteArrayMatrix val;
		val << pPTSetData->GetFieldValue(i + 1, "minvalue") << "/"
			<< pPTSetData->GetFieldValue(i + 1, "maxvalue");
		//str.Format("%s/%s", pts->minValue, pts->maxValue);
		m_List.SetItemText(nIndex, 8, val.constData());
		str = pPTSetData->GetFieldValue(i + 1, "s_precision").constData();
		m_List.SetItemText(nIndex, 9, str);
		str = pPTSetData->GetFieldValue(i + 1, "datatype").constData();
		switch (atoi(str))
		{
			//0-浮点 1-整型 2-控制字(十六进制) 3-字符串 4-控制字(二进制)
		case 0:
			str = StringFromID(IDS_DATATYPE_FLOAT);
			break;
		case 1:
			str = StringFromID(IDS_DATATYPE_INT);
			break;
		case 2:
			str = StringFromID(IDS_DATATYPE_CONTROL16);
			break;
		case 3:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		case 4:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		default:
			str = _T("");
			break;
		}
		m_List.SetItemText(nIndex, 10, str);
		val = pPTSetData->GetFieldValue(i + 1, "reserve3").constData();
		QByteArray Val1 = val.GetFieldValue(1, 1);
		QByteArray Val2 = val.GetFieldValue(1, 2);
		if (Val2.isEmpty())
			Val2 = Val1;
		m_List.SetItemText(nIndex, 11, Val1.constData());
		m_List.SetItemText(nIndex, 12, Val2.constData());
		m_List.SetItemData(nIndex, (DWORD)NULL);
		
		
		nIndex++;
	}
	//恢复刷新
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	//WriteLog("CDlgDataCheck::FillListData, 结束", XJ_LOG_LV3);
}
