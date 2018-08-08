// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgCheckPro.h"

#include "XJTagOutStore.h"
#include "qptsetstatetable.h"

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
		
        COLORREF crText/*, crBkgnd*/;

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

BOOL CDlgCheckPro::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitListStyle();
	
	UpdateLabels();
	
	// TODO: Add extra initialization here
	if (m_nType ==0)
	{
		SetWindowText( StringFromID(IDS_CHECK_RUNNER));
		CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
		QPTSetDataTable *pData = pStore->GetPTSetData();
		pData->ReLoad();
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

	FillData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCheckPro::UpdateLabels()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;

	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pState = pStore->GetState();
	pStore->ReLoadState();
	
	int nPTSetState = pState->GetStateID();

	if (nPTSetState != 0){
		m_sCPU = QByteArray::number(pState->GetCPUID()).constData();
		m_sZone = QByteArray::number(pState->GetZoneID()).constData();
	}
	
	CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pState->GetPTID().constData(), TYPE_SEC);
	m_strDESC.Format("װ��[%s]��[%s]��CPU[%s]�Ŷ�ֵ���ϵĶ�ֵ�������¸��ģ�"
		, pObj->m_sName, m_sCPU, m_sZone);
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

void CDlgCheckPro::FillData()
{
	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pState = pStore->GetState();
	QPTSetDataTable *pPTSetData = pStore->GetPTSetData();
	
	//�������ʱ��ֹˢ��
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
		m_List.SetItemText(nIndex, 1, str); //����
		str = pPTSetData->GetFieldValue(i + 1, "code_name").constData();
		m_List.SetItemText(nIndex, 2, str); //����
		str = pPTSetData->GetFieldValue(i + 1, "unit").constData();
		m_List.SetItemText(nIndex, 3, str); //��λ
		//str = pPTSetData->GetFieldValue(i + 1, "unit").constData();
		m_List.SetItemText(nIndex, 4, ""); //��׼ֵ
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
		val = pPTSetData->GetFieldValue(i + 1, "reserve3").constData();
		m_List.SetItemText(nIndex, 11, val.GetFieldValue(1, 1).constData());
		m_List.SetItemText(nIndex, 12, val.GetFieldValue(1, 2).constData());
		m_List.SetItemData(nIndex, (DWORD)NULL);
		
		
		nIndex++;
	}
	//�ָ�ˢ��
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	//WriteLog("CDlgCheckPro::FillListData, ����", XJ_LOG_LV3);
}
