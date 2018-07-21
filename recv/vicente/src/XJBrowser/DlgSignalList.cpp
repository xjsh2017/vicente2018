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

/** @brief           ����������λ*/
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

	//����TAB
	m_Tab.InsertItem(0, StringFromID(IDS_SIGNAL_ACTION));
	m_Tab.InsertItem(1, StringFromID(IDS_SIGNAL_ALARM));

	//���ô�������
	CString strPTName;
	strPTName = m_pObj->m_sName;
	CString strCaption;
	strCaption = strPTName + StringFromID(IDS_SIGNAL_LIST);
	SetWindowText(strCaption);
	//list
	InitListStyle();
	//����ʾ�����ź���
	InitActionSignalData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ��������,����,������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA303B9
void CDlgSignalList::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	//char* arColumn[4] = {"���","�ź�������","�ź�������","�ź������"};//"strCodeName"
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
			lvCol.cx = 40;//�п�100����
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_List.InsertColumn(iCol,&lvCol);
	}
	
	//Ĭ���������
	m_List.SetColumnHide(0, TRUE);
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}

/*************************************************************
 �� �� ����AddSignalToList()
 ���ܸ�Ҫ�����ź������뵽�б�����ʾ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ������ʾ���ź���
		   param2	Ҫ�����λ��
**************************************************************/
//##ModelId=49B87BA303BA
BOOL CDlgSignalList::AddSignalToList( PT_Signal* sign, int nIndex)
{
	//�ж�������Ч��
	if(sign == NULL)
		return FALSE;
	if(sign < 0)
		return FALSE;
	//"���","�ź�������","�ź�������","�ź������"
	//0:���
	CString sID;
	sID.Format("%d", sign->nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, sID, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:�ź�������
	m_List.SetItemText(nIndex, 1, sign->strName);
	//2:�ź�������
	m_List.SetItemText(nIndex, 2, sign->strCodeName);
	//3:�ź������
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
	m_List.SetItemText(nIndex, 3, sID);
	//��������
	m_List.SetItemData(nIndex, (DWORD)sign);
	return TRUE;
}

/*************************************************************
 �� �� ����InitActionSignalData()
 ���ܸ�Ҫ����ʼ�������ź�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA303BD
void CDlgSignalList::InitActionSignalData()
{
	//���������Ч��
	if(m_pObj == NULL)
		return;
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//���ԭ������
	m_List.DeleteAllItems();
	m_pObj->RefreshConfig();
	//ѭ�������ź���������������ʾ
	for(int i = 0; i < m_pObj->GetActionSignalCount(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pObj->QueryActionSignal(i);
		AddSignalToList(pSign);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ����InitAlarmSignalData()
 ���ܸ�Ҫ����ʼ���澯�����ź�����ʾ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BA303BE
void CDlgSignalList::InitAlarmSignalData()
{
	//���������Ч��
	if(m_pObj == NULL)
		return;
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//���ԭ������
	m_List.DeleteAllItems();
	//ѭ�������ź���������������ʾ
	for(int i = 0; i < m_pObj->GetAlarmSignalCount(); i++)
	{
		PT_Signal* pSign = (PT_Signal*)m_pObj->QueryAlarmSignal(i);
		AddSignalToList(pSign);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

//##ModelId=49B87BA303CB
void CDlgSignalList::OnSelchangeTabSignalList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_Tab.GetCurSel();
	if(nIndex == 0)
	{
		//�����ź�
		InitActionSignalData();
	}
	else if(nIndex == 1)
	{
		//�澯�ź���
		InitAlarmSignalData();
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������I,II,III���ź���������ɫ
 �� �� ֵ: void
 ��    ����param1
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				if(nItem  != -1)
				{
					//�õ�����
					PT_Signal * sign = (PT_Signal*)m_List.GetItemData(nItem);
					//����������ɫ
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
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87BA303AF
int CALLBACK CDlgSignalList::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	PT_Signal* pSign1 = (PT_Signal*)lParam1;
	PT_Signal* pSign2 = (PT_Signal*)lParam2;
	if(pSign1 == NULL || pSign2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","�ź�������","�ź�������","�ź������"
	switch(nCol)
	{
	case 0: //ID
		iResult = pSign1->nID - pSign2->nID;
		break;
	case 1: //�ź�������
		str1 = pSign1->strName;
		str2 = pSign2->strName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //�ź�������
		str1 = pSign1->strCodeName;
		str2 = pSign2->strCodeName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //���
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
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
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
