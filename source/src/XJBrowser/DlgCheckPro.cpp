// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgCheckPro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckPro dialog


//##ModelId=49B87BA402E1
CDlgCheckPro::CDlgCheckPro(CWnd* pParent /*=NULL*/, int nType)
	: CDialog(CDlgCheckPro::IDD, pParent), m_sCPU(""), m_sZone("")
{
	//{{AFX_DATA_INIT(CDlgCheckPro)
	m_strModify = _T("");
	m_nType = nType;
	//}}AFX_DATA_INIT
}


//##ModelId=49B87BA402E4
void CDlgCheckPro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCheckPro)
	DDX_Text(pDX, IDC_EDIT_CHECK, m_strModify);
	DDX_Text(pDX, IDC_STATIC_DESC, m_strDESC);
	DDX_Control(pDX, IDC_LIST_PTSET, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCheckPro, CDialog)
	//{{AFX_MSG_MAP(CDlgCheckPro)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCheckPro message handlers

BOOL CDlgCheckPro::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitListStyle();
	
	UpdateLabels();
	
	// TODO: Add extra initialization here
	if (m_nType ==0)
	{
		SetWindowText( StringFromID(IDS_CHECK_RUNNER));

		LoadPTSET();
		FillListData();
	}
	else if (m_nType ==1)
	{
		SetWindowText( StringFromID(IDS_CHECK_GUARDIAN));

		LoadPTSETMod();
	}
	else if (m_nType ==2)
	{
		SetWindowText( StringFromID(IDS_CHECK_OPERATOR));

		LoadPTSETMod();
	}
	else
	{
		SetWindowText( StringFromID(IDS_CHECK_DEFAULT));
	}
		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCheckPro::UpdateLabels()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;

	PT_ZONE data;
	CString sRecords;
	int nState = pApp->GetPTSetModState(data, sRecords);
	//str.Format("%d, %d", data.cpu, data.code);
	//AfxMessageBox(str);
	CString sCPU = m_sCPU, sZone = m_sZone;
	if (m_sCPU.IsEmpty())
		sCPU.Format("%d", data.cpu);
	if (m_sZone.IsEmpty())
		sZone.Format("%d", data.code);
	
	
	CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(data.PT_ID, TYPE_SEC);
	m_strDESC.Format("装置[%s]在[%s]号CPU[%s]号定值区上的定值将做如下更改："
		, pObj->m_sName, sCPU, sZone);
	//AfxMessageBox(m_strDESC);
	UpdateData(FALSE);
}

int CDlgCheckPro::InitListStyle()
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

void CDlgCheckPro::LoadPTSETMod()
{
	if(m_arrModifyList.GetSize() < 0)
		return;
	
	CString str;
	char *sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nPTCount = m_arrModifyList.GetSize();
	int nIndex = 0;

	// test..
	/*nPTCount = 2;
	for(int i = 0; i < nPTCount; i++)
	{
		CString strID;
		strID.Format("%d", "1004");
		
		m_List.InsertItem(nIndex, strID); //ID
		
		string cc = "hello@61.2";
		vector<string> v;
		SplitString(cc, v, "@");
		if (v.size() != 2)
			continue;
		
		str.Format("%s", v[0].c_str());
		m_List.SetItemText(nIndex, 1, str); //名称
		str.Format("%s", v[1].c_str()); 
		m_List.SetItemText(nIndex, 11, str); //原值
		str.Format("%s", "100"); 
		m_List.SetItemText(nIndex, 12, str); //更改值
		
		nIndex++;
	}*/
	for(int i = 0; i < nPTCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);
		
		m_List.InsertItem(nIndex, strID); //ID

		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		vector<CString> v = pApp->SplitCString(CString(sttpData->str_reserved.c_str()), '@');
		if (v.size() != 2)
			continue;
	
		str.Format("%s", v[0]);
		m_List.SetItemText(nIndex, 1, str); //名称
		str.Format("%s", v[1]); 
		m_List.SetItemText(nIndex, 11, str); //原值
		str.Format("%s", sttpData->str_value.c_str()); 
		m_List.SetItemText(nIndex, 12, str); //修改值

		nIndex++;
	}

	delete[] sError;
	sError = NULL;
}

void CDlgCheckPro::ClearSettingConfig()
{
	//清除定值配置
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING * pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		delete pts;
		pts = NULL;
	}
	m_arrSetting.RemoveAll();
}

void CDlgCheckPro::LoadPTSET()
{
	ClearSettingConfig();
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	PT_ZONE data;
	CString sRecords;
	int nCurrentState = pApp->GetPTSetModState(data, sRecords);

	WriteLog("CDlgCheckPro::LoadPTSET, 查询开始", XJ_LOG_LV3);

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code, zone, 103group(如果要分组显示的话)查找
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//按setting_ID从小到大排列
		
	CString str;
		
	//字段
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Setting_ID", EX_STTP_DATA_TYPE_INT);

	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);

	//code_name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);

	//vpickList
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "vpicklist", EX_STTP_DATA_TYPE_STRING);

	//unit
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "unit", EX_STTP_DATA_TYPE_STRING);

	//s_precision
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "s_precision", EX_STTP_DATA_TYPE_STRING);

	//datatype
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "datatype", EX_STTP_DATA_TYPE_INT);

	//minvalue
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "minvalue", EX_STTP_DATA_TYPE_FLOAT);

	//maxvalue
	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "maxvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "stepvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "103group", EX_STTP_DATA_TYPE_INT);
	
	//stepvalue
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "103item", EX_STTP_DATA_TYPE_INT);

	//stepvalue
	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "61850ref", EX_STTP_DATA_TYPE_INT);

	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "RESERVE2", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "RESERVE3", EX_STTP_DATA_TYPE_STRING);

	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", data.PT_ID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %d", data.cpu);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	//按Setting_ID大小排序
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //非where条件	

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CDlgCheckPro::LoadPTSET, 查询失败");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CDlgCheckPro::LoadPTSET,查询到%d条记录", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		//EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			/*if(g_role != MODE_MAIN)
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			*/
			//创建定值对象
			PT_SETTING * pts = new PT_SETTING;
			//setting_id,NAME,CODE_NAME,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = pMemset.GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset.GetValue(1); //NAME
			pts->CodeName = pMemset.GetValue(2); //Code_Name
			pts->VPickList = pMemset.GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = pMemset.GetValue(4); //unit
			pts->Precision = pMemset.GetValue(5); //s_precision
			str = pMemset.GetValue(6); //datatype
			pts->DataType = atoi(str);
			str = pMemset.GetValue(7);
			pts->minValue = str;//atof(str);
			str = pMemset.GetValue(8);
			pts->maxValue = str;//atof(str);
			str = pMemset.GetValue(9);
			pts->stepValue = str;//atof(str);
			str = pMemset.GetValue(10);
			pts->Group = atoi(str);
			str = pMemset.GetValue(11);
			pts->Item = atoi(str);
			str = pMemset.GetValue(12);
			pts->nRefType = 1;
			if(!str.IsEmpty())
			{
				str.MakeUpper();
				if(str.Find("$SP$", 0) != -1)
					pts->nRefType = 0;
			}
			else
			{
				CString sGroupName;// = GetGroupName(pts->Group);
				if(!sGroupName.IsEmpty())
				{
					if(sGroupName.Find( StringFromID(IDS_COMMON_PARAMETER), 0) != -1)
						pts->nRefType = 0;
				}
			}
			str = pMemset.GetValue(13);
			pts->ntimeMStoS = 0;
			if(!str.IsEmpty())
			{
				pts->ntimeMStoS = atoi(str);
			}
			str = pMemset.GetValue(13);
			//str.Format("99.9");
			pts->VPickList = str;
			str = pMemset.GetValue(14);
			pts->Value = str;

			//定值组
		/*	if(g_PTSettingGroup == 1 && nGroupCount > 0)
			{
				pts->Group = m_nGroup;
			}*/

			m_arrSetting.Add(pts);
					
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgCheckPro::LoadPTSET,查询失败,原因为%s", sError);
		WriteLogEx(str);
	}	
	
	WriteLog("CDlgCheckPro::LoadPTSET,查询结束", XJ_LOG_LV3);

}

void CDlgCheckPro::FillListData()
{
	//WriteLog("CDlgCheckPro::FillListData, 开始", XJ_LOG_LV3);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper);  
	//int nGroupCount = m_arrGroup.GetSize();
	PT_SETTING *pts = NULL;
	int nIndex = 0;
	for(int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", nIndex+1);
		}
		m_List.InsertItem(nIndex, sID); //ID
		CString strName;
		int z = pts->Name.Find(",", 0);
		if (z != -1)
		{
			strName = pts->Name.Left(z);
		}
		else
		{
			strName = pts->Name;
		}
		m_List.SetItemText(nIndex, 1, strName); //名称
		m_List.SetItemText(nIndex, 2, pts->CodeName); //代码
		m_List.SetItemText(nIndex, 3, pts->Unit); //单位
		m_List.SetItemText(nIndex, 4, pts->OrderValue); //基准值
		CString str;
		str.Format("%d", pts->Group);
		m_List.SetItemText(nIndex, 5, str);
		str.Format("%d", pts->Item);
		m_List.SetItemText(nIndex,6, str);
		str.Format("%s", pts->stepValue);
		m_List.SetItemText(nIndex, 7, str);
		
		str.Format("%s/%s", pts->minValue, pts->maxValue);
		m_List.SetItemText(nIndex, 8, str);
		m_List.SetItemText(nIndex, 9, pts->Precision);
		switch (pts->DataType)
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
		m_List.SetItemText(nIndex, 11, pts->VPickList);
		m_List.SetItemText(nIndex, 12, pts->Value);
		if (pts->Value.IsEmpty())
			m_List.SetItemText(nIndex, 12, pts->VPickList);

		m_List.SetItemData(nIndex, (DWORD)pts);


		nIndex++;
	}
	//恢复刷新
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);


	WriteLog("CDlgCheckPro::FillListData, 结束", XJ_LOG_LV3);
}