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

/** @brief           ����������λ*/
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
 �� �� ����AddSignal()
 ���ܸ�Ҫ�������ź������ź����б�
 �� �� ֵ: void
 ��    ����param1	Ҫ������ź���
		   Param2
**************************************************************/
//##ModelId=49B87BA40249
void CDlgEventSignal::AddSignal( PT_Signal* sign )
{
	//�ж�������Ч��
	if(sign == NULL)
		return;
	//������Ѵ���
	BOOL bExist = FALSE;
// 	for(int i = 0; i < m_arrSignalsOutput.GetSize(); i++)
// 	{
// 		PT_Signal* pExist = (PT_Signal*)m_arrSignalsOutput.GetAt(i);
// 		if(pExist->nID == sign->nID)
// 		{
// 			//�Ѵ���
// 			bExist = TRUE;
// 			break;
// 		}
// 	}

	if(bExist == FALSE)
	{
		//������,����
		m_arrSignalsOutput.Add(sign);
	}
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA4024B
BOOL CDlgEventSignal::InitListStyle()
{
	//��ͼ���б�, ���������и�
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
			lvCol.cx = 80; //�п�60����
			colName = StringFromID(IDS_ACTION_CASE);
			break;
		default:
			lvCol.cx = 40;//�п�100����
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_List.InsertColumn(iCol,&lvCol);
	}

	//Ĭ���������
	m_List.SetColumnHide(0, TRUE);
	if (m_colHide != -1)
	{
		m_List.SetColumnHide(m_colHide, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return TRUE;
}

/*************************************************************
 �� �� ����InitListData()
 ���ܸ�Ҫ����ʼ���б�����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA40251
BOOL CDlgEventSignal::InitListData()
{
	//����Ƿ�������
	if(m_arrSignalsOutput.GetSize() <= 0)
	{
		return TRUE;
	}
	//����б�ԭ������
	m_List.DeleteAllItems();
	//��������
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
 �� �� ����AddSignalToList()
 ���ܸ�Ҫ���ѵ����ź������ݼ��뵽�б�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	�ź���
		   Param2	Ҫ�����λ��
**************************************************************/
//##ModelId=49B87BA40252
BOOL CDlgEventSignal::AddSignalToList( PT_Signal* sign, int nIndex /*= 0*/ )
{
	//���������Ч��
	if(sign == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	//"���", "����","��վ����","��������","ʱ��","��������","�������"
	//0:���
	CString sID;
	sID.Format("%d", sign->nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:���
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
		//Ĭ��ΪIII
		sID = "III";
	}
	m_List.SetItemText(nIndex, 1, sID);
	//2:��վ����
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData != NULL)
	{
		CStationObj* pStation = (CStationObj*)pData->FindDevice(sign->strStationId, TYPE_STATION);
		if(pStation != NULL)
		{
			//ȡ�ó�վ��
			sign->strStation = pStation->m_sName;
		}
	}
	m_List.SetItemText(nIndex, 2, sign->strStation);
	//3:��������
	//sID.Format("%s(%s)", sign->strPT_ID, sign->strPT_Model);
	m_List.SetItemText(nIndex, 3, sign->strPTName);
	//4:ʱ��
	m_List.SetItemText(nIndex, 4, sign->strTime);
	//5:��������
	m_List.SetItemText(nIndex, 5, sign->strName);
	//6:�������
	if(sign->nValue == 1)
	{
		sID = StringFromID(IDS_CASE_ACTION);
	}
	else
	{
		sID = StringFromID(IDS_CASE_RETURN);
	}
	m_List.SetItemText(nIndex, 6, sID);
	//��������
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
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ���źŶ���Ϊ1ʱ����Ϊ��ɫ
 �� �� ֵ: 
 ��    ����param1
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				if(nItem  != -1)
				{
					//�õ�����
					PT_Signal * sign = (PT_Signal*)m_List.GetItemData(nItem);
					//���Ƿ�ȷ��, ����������ɫ
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
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87BA40238
int CALLBACK CDlgEventSignal::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	PT_Signal* pSign1 = (PT_Signal*)lParam1;
	PT_Signal* pSign2 = (PT_Signal*)lParam2;
	if(pSign1 == NULL || pSign2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","����","��վ����","��������","ʱ��","��������","�������"
	switch(nCol)
	{
	case 0: //ID
		iResult = pSign1->nID - pSign2->nID;
		break;
	case 1: //���
		iResult = pSign1->nEventType - pSign2->nEventType;
		break;
	case 2: //��վ����
		str1 = pSign1->strStation;
		str2 = pSign2->strStation;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //��������
		str1.Format("%s(%s)", pSign1->strPT_ID, pSign1->strPT_Model);
		str2.Format("%s(%s)", pSign2->strPT_ID, pSign2->strPT_Model);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //ʱ��
		str1 = pSign1->strTime;
		str2 = pSign2->strTime;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //��������
		str1 = pSign1->strName;
		str2 = pSign2->strName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //�������
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
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
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
 �� �� ����GetSignalDescribe()
 ���ܸ�Ҫ���õ��ź�������������
 �� �� ֵ: ��������
 ��    ����param1	�ź���
**************************************************************/
//##ModelId=49B87BA40255
CString CDlgEventSignal::GetSignalDescribe( PT_Signal* sign )
{
	//�ж�������Ч��
	if(sign == NULL)
		return "";
	CString sReturn = "";
	//"����","��վ����","��������","ʱ��","�¼�����","�������"
	CString str;
	//����
	if(sign->nEventType == 0)
		str = "I";
	else if(sign->nEventType == 1)
		str = "II";
	else
		str = "III";
	sReturn += str;
	sReturn += "\t";
	//��վ����
	sReturn += sign->strStation;
	sReturn += "\t";
	//��������
	str.Format("%s(%s)", sign->strPT_ID, sign->strPT_Model);
	sReturn += sign->strPTName;
	sReturn += "\t\t";
	//ʱ��
	sReturn += sign->strTime;
	sReturn += "\t\t";
	//�¼�����
	sReturn += sign->strName;
	sReturn += "\t";
	//�������
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
	//��ѡ��
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
		//����str��������
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
	
	// ����ÿ�е�λ��
	print.ResizeColumnsToFitContent(TRUE);
	
	// �Ƿ�ü�ÿ�еĳ��� FALSE �� TRUE ��
	print.TruncateColumn(TRUE);
	
	// ��ӡ��֮��ķָ���
	print.PrintColumnSeparator(TRUE);
	
	// ��ӡ��֮��ķָ���
	print.PrintLineSeparator(TRUE);
	
	// ��ӡ�������
	print.PrintHead(TRUE);
	
	// ���ø�������
	print.SetHeadFont(StringFromID(IDS_HEAD_FONT), 14);
	print.SetFootFont(StringFromID(IDS_DEFAULT_FONT), 12);
	print.SetListFont(StringFromID(IDS_DEFAULT_FONT), 12);
	
	// ����ֽ������ TRUE,����FALSE ����
	print.SetPrinterOrientation(FALSE);
    
	BOOL bHide = m_List.GetColumnIsHide(0);
    CString strCa;
	GetWindowText(strCa);
	print.Print(GetSafeHwnd(), strCa, FALSE);
	m_List.SetColumnHide(0, bHide);
}




