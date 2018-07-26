// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgCheckPro.h"

#pragma   warning   (disable   :   4786)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct PT_SETTING_DATA{
	PT_SETTING* pts;
	CString reserve1;
	CString reserve2;
	BOOL bMod;

	PT_SETTING_DATA::PT_SETTING_DATA(){
		bMod = FALSE;
		pts = NULL;
		reserve1 = "";
		reserve2 = "";
	}
};



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
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSET, OnCustomdrawList)
END_MESSAGE_MAP()

void CDlgCheckPro::OnCustomdrawList ( NMHDR* pNMHDR, LRESULT* pResult )
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

		int nRowIdx = static_cast<int> (pLVCD->nmcd.dwItemSpec); //������
		int nColIdx = pLVCD->iSubItem; //������
        
        if ( 12 == nColIdx )
		{
			//ֵ
			CString strValue = m_List.GetItemText(nRowIdx, nColIdx);
			CString strOldValue = m_List.GetItemText(nRowIdx, nColIdx - 1);
			
			//ȥ�����߿ո�
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
// CDlgCheckPro message handlers

/*
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
*/
BOOL CDlgCheckPro::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitListStyle();
	
	UpdateLabels();
	
	// TODO: Add extra initialization here
	if (m_nType ==0)
	{
		SetWindowText( StringFromID(IDS_CHECK_RUNNER));
	}
	else if (m_nType ==1)
	{
		SetWindowText( StringFromID(IDS_CHECK_GUARDIAN));
	}
	else if (m_nType ==2)
	{
		SetWindowText( StringFromID(IDS_CHECK_OPERATOR));
	}
	else
	{
		SetWindowText( StringFromID(IDS_CHECK_DEFAULT));
	}
	
	LoadData();
	FillData();
	
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
	str.Format("%d, %d", data.cpu, data.code);
	//AfxMessageBox(str);
	CString sCPU = m_sCPU, sZone = m_sZone;
	m_sCPU.TrimLeft();
	m_sCPU.TrimRight();
	m_sZone.TrimLeft();
	m_sZone.TrimRight();
	if (m_sCPU.IsEmpty()){
		sCPU.Format("%d", data.cpu);
		m_sCPU = sCPU;
	}
	if (m_sZone.IsEmpty()){
		sZone.Format("%d", data.code);
		m_sZone = sZone;
	}
	
	
	CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(data.PT_ID, TYPE_SEC);
	m_strDESC.Format("װ��[%s]��[%s]��CPU[%s]�Ŷ�ֵ���ϵĶ�ֵ�������¸��ģ�"
		, pObj->m_sName, sCPU, sZone);
	//AfxMessageBox(m_strDESC);
	UpdateData(FALSE);
}

int CDlgCheckPro::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_LEFT;
	{
		//��������ʧ��
		//char* arColumn[11]={"���","��ֵ����", "��ֵ����", "��λ", "��׼ֵ", "���", "��Ŀ��","����", "��Сֵ/���ֵ", "����", "��������"};
		//�����п�,��С����
		for (int nCol=0; nCol < 13 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 140; // ��ֵ����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // ��ֵ����
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80; //��λ
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 80; //��׼ֵ
				colName = StringFromID(IDS_SETTING_ORDER);
				break;
			case 5:
				lvCol.cx = 60; //���
				colName = StringFromID(IDS_POINT_GROUP);
				break;
			case 6:
				lvCol.cx = 60; //��Ŀ��
				colName = StringFromID(IDS_POINT_ITEM);
				break;
			case 7:
				lvCol.cx = 60; //����
				colName = StringFromID(IDS_POINT_STEP);
				break;
			case 8:
				lvCol.cx = 80; //��Сֵ/���ֵ
				colName = StringFromID(IDS_POINT_MAXMIN);
				break;
			case 9:
				lvCol.cx = 60; //����
				colName = StringFromID(IDS_POINT_PRECISION);
				break;
			case 10:
				lvCol.cx = 80; //��������
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			case 11:
				lvCol.cx = 80; //ԭֵ
				colName = StringFromID(IDS_COLUMN_VALUE_0);
				break;
			case 12:
				lvCol.cx = 80; //����ֵ
				colName = StringFromID(IDS_COLUMN_VALUE_1);
				break;
			/*case 13:
				lvCol.cx = 80; //��ֵ
				colName = StringFromID(IDS_COLUMN_VALUE_2);
				break;*/
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		//m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��ֵ����)
		m_List.SetColumnHide(2, TRUE);
		//Ĭ�����ص�5��(��׼ֵ)
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
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgCheckPro::ClearData()
{
	//�����ֵ�б�
	for(int i = 0; i < m_arrSetting2.GetSize(); i++)
	{
		PT_SETTING_DATA * data = (PT_SETTING_DATA*)m_arrSetting2.GetAt(i);
		delete data->pts;
		data->pts = NULL;
		delete data;
	}
	m_arrSetting2.RemoveAll();
}

void CDlgCheckPro::LoadData()
{
	ClearData();
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	PT_ZONE data;
	CString sRecords;
	int nCurrentState = pApp->GetPTSetModState(data, sRecords);

	map<CString, CString> mapMod;

	int nPTCount = m_arrModifyList.GetSize();
// 	CString ttstr;
// 	ttstr.Format("m_arrModifyList.size() = %d", m_arrModifyList.GetSize());
// 	AfxMessageBox(ttstr);
	for(int i = 0; i < nPTCount; i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);

// 		ttstr.Format("%s, %s", strID, sttpData->str_value.c_str());
// 		AfxMessageBox(ttstr);

		mapMod.insert(make_pair(strID, sttpData->str_value.c_str()));
	}

	WriteLog("CDlgCheckPro::LoadData, ��ѯ��ʼ", XJ_LOG_LV3);

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code, zone, 103group(���Ҫ������ʾ�Ļ�)����
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//��setting_ID��С��������
		
	CString str;
		
	//�ֶ�
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
	pApp->m_DBEngine.SetField(sql, Field14, "RESERVE1", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "RESERVE3", EX_STTP_DATA_TYPE_STRING);

	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", data.PT_ID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU/*data.cpu*/);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("ZONE = %s", m_sZone/*data.id*/);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//��Setting_ID��С����
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //��where����

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
		WriteLogEx("CDlgCheckPro::LoadPTSET, ��ѯʧ��");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CDlgCheckPro::LoadPTSET,��ѯ��%d����¼", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		//EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			/*if(g_role != MODE_MAIN)
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			*/
			//������ֵ����
			PT_SETTING * pts = new PT_SETTING;
			PT_SETTING_DATA *data = new PT_SETTING_DATA;
			data->pts = pts;
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
			//pts->VPickList = str;
			data->reserve1 = str;
			str = pMemset.GetValue(14);
			data->reserve2 = str;

			data->reserve2.TrimLeft();
			data->reserve2.TrimRight();
			if(data->reserve2.IsEmpty())
				data->reserve2 = data->reserve1;

// 			if (i == 2)
// 				data->reserve2 = CString("18.8");

			if (mapMod.count(pts->ID) == 1)
				data->reserve2 = mapMod[pts->ID];

			m_arrSetting2.Add(data);
					
			pMemset.MoveNext();
		}

		// �� m_arrModifyList �����޸�ֵ

	}
	else
	{
		CString str;
		str.Format("CDlgCheckPro::LoadData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
	}
	str.Format("m_arrSetting2.size() = %d", m_arrSetting2.GetSize());
	//AfxMessageBox(str);
	WriteLog("CDlgCheckPro::LoadData,��ѯ����", XJ_LOG_LV3);

}

void CDlgCheckPro::FillData()
{
	//WriteLog("CDlgCheckPro::FillData, ��ʼ", XJ_LOG_LV3);
	
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper);  
	//int nGroupCount = m_arrGroup.GetSize();
	PT_SETTING *pts = NULL;
	PT_SETTING_DATA *data = NULL;
	int nIndex = 0;
	for(int i = 0; i < m_arrSetting2.GetSize(); i ++)
	{
		data = (PT_SETTING_DATA*)m_arrSetting2.GetAt(i);
		if (NULL == data)
			continue;
		pts = data->pts;
		if (NULL == pts)
			continue;

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
		m_List.SetItemText(nIndex, 1, strName); //����
		m_List.SetItemText(nIndex, 2, pts->CodeName); //����
		m_List.SetItemText(nIndex, 3, pts->Unit); //��λ
		m_List.SetItemText(nIndex, 4, pts->OrderValue); //��׼ֵ
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
			//0-���� 1-���� 2-������(ʮ������) 3-�ַ��� 4-������(������)
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
		m_List.SetItemText(nIndex, 11, data->reserve1);
		m_List.SetItemText(nIndex, 12, data->reserve2);
		m_List.SetItemData(nIndex, (DWORD)data);
		
		
		nIndex++;
	}
	//�ָ�ˢ��
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	WriteLog("CDlgCheckPro::FillListData, ����", XJ_LOG_LV3);
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
		m_List.SetItemText(nIndex, 1, str); //����
		str.Format("%s", v[1].c_str()); 
		m_List.SetItemText(nIndex, 11, str); //ԭֵ
		str.Format("%s", "100"); 
		m_List.SetItemText(nIndex, 12, str); //����ֵ
		
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
		m_List.SetItemText(nIndex, 1, str); //����
		str.Format("%s", v[1]); 
		m_List.SetItemText(nIndex, 11, str); //ԭֵ
		str.Format("%s", sttpData->str_value.c_str()); 
		m_List.SetItemText(nIndex, 12, str); //�޸�ֵ

		nIndex++;
	}

	delete[] sError;
	sError = NULL;
}

void CDlgCheckPro::ClearSettingConfig()
{
	//�����ֵ����
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

	WriteLog("CDlgCheckPro::LoadPTSET, ��ѯ��ʼ", XJ_LOG_LV3);

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code, zone, 103group(���Ҫ������ʾ�Ļ�)����
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//��setting_ID��С��������
		
	CString str;
		
	//�ֶ�
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
	pApp->m_DBEngine.SetField(sql, Field14, "RESERVE1", EX_STTP_DATA_TYPE_STRING);
	
	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "RESERVE3", EX_STTP_DATA_TYPE_STRING);

	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", data.PT_ID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %d", data.cpu);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	//��Setting_ID��С����
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //��where����	

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
		WriteLogEx("CDlgCheckPro::LoadPTSET, ��ѯʧ��");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CDlgCheckPro::LoadPTSET,��ѯ��%d����¼", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		//EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			/*if(g_role != MODE_MAIN)
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			*/
			//������ֵ����
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

			//��ֵ��
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
		str.Format("CDlgCheckPro::LoadPTSET,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
	}	
	
	WriteLog("CDlgCheckPro::LoadPTSET,��ѯ����", XJ_LOG_LV3);

}

void CDlgCheckPro::FillListData()
{
	//WriteLog("CDlgCheckPro::FillListData, ��ʼ", XJ_LOG_LV3);

	//�������ʱ��ֹˢ��
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
		m_List.SetItemText(nIndex, 1, strName); //����
		m_List.SetItemText(nIndex, 2, pts->CodeName); //����
		m_List.SetItemText(nIndex, 3, pts->Unit); //��λ
		m_List.SetItemText(nIndex, 4, pts->OrderValue); //��׼ֵ
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
       //0-���� 1-���� 2-������(ʮ������) 3-�ַ��� 4-������(������)
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
	//�ָ�ˢ��
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);


	WriteLog("CDlgCheckPro::FillListData, ����", XJ_LOG_LV3);
}