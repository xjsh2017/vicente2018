// DlgDetailStatus.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgDetailStatus.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus dialog
/** @brief           ����������λ*/
int g_iDetailStatusAsc;

DlgDetailStatus::DlgDetailStatus(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDetailStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDetailStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_arrColum.RemoveAll();
	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	pList = NULL;
	startPos = NULL;
	g_iDetailStatusAsc = -1;
    m_nOldSortCol = -1;
}

DlgDetailStatus::DlgDetailStatus(CommStatusList* pCommList, POSITION pos, CWnd* pParent /*=NULL*/)
: CDialog(DlgDetailStatus::IDD, pParent)
{
	m_arrColum.RemoveAll();
	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	pList = pCommList;
	startPos = pos;
	g_iDetailStatusAsc = -1;
	m_nOldSortCol = -1;
}


void DlgDetailStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDetailStatus)
	DDX_Control(pDX, IDC_LIST_DETAIL_STATUS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDetailStatus, CDialog)
	//{{AFX_MSG_MAP(DlgDetailStatus)
    ON_WM_WINDOWPOSCHANGED()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DETAIL_STATUS, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DETAIL_STATUS, OnColumnclickListDetailStatus)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus message handlers


BOOL DlgDetailStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	FillDataList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void DlgDetailStatus::InitListCtrl()
{
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
	
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	if(LoadListConfig())
	{
		//�������óɹ�
		int IDX = 0;
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			lvCol.iSubItem = i;
			lvCol.cx = m_arrColum[i].nItemWidth;
			CString sName = m_arrColum[i].sName;
			char * name = sName.GetBuffer(sName.GetLength());
			lvCol.pszText = name;
			m_List.InsertColumn(IDX++, &lvCol);
			if (m_arrColum[i].bHide)
			{
				m_List.SetColumnWidth(i, 0);
			}
		}
		
	}
	else
	{
		//��������ʧ��
		char* arColumn[6]={
			"״̬",
			"�Ͽ�ԭ��",
			"ԭ��ı�ʱ��",
			"��ͨ��״̬",
			"��ͨ���Ͽ�ԭ��",
			"��ͨ��ԭ��ı�ʱ��"
			};		
		//�����п�,��С����
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 1://
				lvCol.cx = 80; // ״̬
				colName = StringFromID( IDS_COMMU_STATUS);
				break; 
			case 2://
				lvCol.cx = 160; // �Ͽ�ԭ��
				colName = StringFromID( IDS_DISCONNECT_REASON);
				break;
			case 3://
				lvCol.cx = 160; // ԭ��ı�ʱ��
				colName = StringFromID( IDS_DISCONNECT_REASON_TIME);
				break;
			case 4:
				lvCol.cx = 80;  //��ͨ��״̬
				colName = StringFromID( IDS_STANDBYCHL_COMMU_STATUS);
				break;
			case 5:
				lvCol.cx = 160;  //��ͨ���Ͽ�ԭ��
				colName = StringFromID( IDS_STANDBYCHL_DISCONNECT_REASON);
				break;
			case 6:
				lvCol.cx = 160; //��ͨ��ԭ��ı�ʱ��
				colName = StringFromID( IDS_STANDBYCHL_DISCONNECT_REASON_TIME);
				break;
			default:
				lvCol.cx=100;//�п�100����
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol, &lvCol);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
}



/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B86006D
BOOL DlgDetailStatus::LoadListConfig()
{
	//��ȡUI�����ļ�
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //��ʼ��COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!");
			WriteLog("ViewComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
			bResult = FALSE;
		} 	
		//����UIConfig�ļ� 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//����ʧ��
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str);
				WriteLog("ViewComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewDetailStatusConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewCommConfig");
				WriteLog("ViewComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewCommConfig�µ�����");
					WriteLog("ViewComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					//ѭ����ȡ�ӽڵ���Ϣ
					for(int i = 0; i < pChild ->Getlength(); i++)
					{
						//�õ��ӽڵ�
						MSXML2::IXMLDOMNodePtr pNode;
						pNode = pChild ->Getitem(i);
						//�õ��ӽڵ������б�ָ��
						MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
						pAttrMap = pNode ->Getattributes();
						
						//�����Խṹ
						SColumn	col;
						
						//ѭ����ȡ����
						for(int j = 0; j < pAttrMap ->Getlength(); j++)
						{
							MSXML2::IXMLDOMNodePtr			pItem;
							pItem = pAttrMap ->Getitem(j);
							CString strValue = (char *)(_bstr_t)pItem ->GetnodeValue();
							int nHide = 0;
							int nWidth = 0;
							switch(j)
							{
							case 0: //����
								col.sName = strValue;
								break;
							case 1: //�п�
								nWidth = atoi(strValue);
								col.nItemWidth = nWidth;
								break;
							case 2:	//�Ƿ�����
								nHide = atoi(strValue);
								if(0 == nHide)
								{
									col.bHide = FALSE;
								}
								else
								{
									col.bHide = TRUE;
								}
								break;
							default:
								break;
							}
						}
						m_arrColum.Add(col);
					}
				}
			}
		}		
	}	
	//�رմ򿪵�COM
	CoUninitialize();
	return bResult;
}

void DlgDetailStatus::RegulateControlSize()
{
	MYASSERT(m_List.GetSafeHwnd());
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	rc.top += 50;
	//�����б���С,ռ�����߶�
	m_List.MoveWindow(&rc, TRUE);
}

void  DlgDetailStatus::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
BOOL DlgDetailStatus::InitCOM()
{
	/*HRESULT hr;
	if((hr = ::CLSIDFromProgID(L"XJReportDoc", &clsid)) != NOERROR)
	{
		TRACE("unable to find program ID -- error= %x\n", hr);
		return FALSE;
	}
	
	if((hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, NULL, 
		IID_IClassFactory, (void **)&pClf)) != NOERROR)
	{
		TRACE("unable to find CLSID -- error = %x\n", hr);
		return FALSE;
	}
	
	pClf ->CreateInstance(NULL, IID_IUnknown, (void **)&pUnk);
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);*/
	return InitReportComNoReg(pClf, pReportDoc);
	
	//return TRUE;
}

/*************************************************************
 �� �� ����ReleaseCOM()
 ���ܸ�Ҫ��ɾ��COM���
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
void DlgDetailStatus::ReleaseCOM()
{
	if(pClf != NULL)
	{
		pClf->Release();
		pClf = NULL;
	}
	if(pUnk != NULL)
	{
		pUnk->Release();
		pUnk = NULL;
	}
	if(pReportDoc != NULL)
	{
		pReportDoc->Release();
		pReportDoc = NULL;
	}
}

BOOL DlgDetailStatus::GetStatus(Record* pRecord)
{
	MYASSERT_BOOL(pRecord);
	if ((pRecord->m_nIPFlag == 2 && pRecord->m_nCommStatus == 0 && pRecord->m_nCommStatus1 == 0) || (pRecord->m_nIPFlag != 2 && pRecord->m_nCommStatus == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void DlgDetailStatus::SetRecordInfo(Record* pRecord, int nItem)
{
	MYASSERT(pRecord);
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	//1:״̬
	result = m_List.SetItemText(nItem, IDX, pRecord->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pRecord->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)));
	
	if (pRecord->m_nCommStatus  != -1)
	{
		//2:�Ͽ�ԭ��
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReason);
		
		//3:ԭ��ı�ʱ��
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReasonTime);
	}
	else
	{
		IDX += 2;
	}
	
	//4:��ͨ��״̬
	result = m_List.SetItemText(nItem, ++IDX, pRecord->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pRecord->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)));	
	
	if (pRecord->m_nCommStatus1 != -1)
	{
 		//5:��ͨ���Ͽ�ԭ��
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReason1);
		
		//6:��ͨ��ԭ��ı�ʱ��
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReasonTime1);
	}
		
	//��������s
	m_List.SetItemData(nItem, (DWORD)pRecord);
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if (g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}


void  DlgDetailStatus::FillDataList()
{
	MYASSERT(pList);
	MYASSERT(startPos);
	CWnd* pWnd = GetDlgItem(IDC_STATION_STATIC);
	MYASSERT(pWnd);
	MYASSERT(pList->pStationObj); 
	pWnd->SetWindowText(pList->pStationObj->m_sName);
	
	pWnd = GetDlgItem(IDC_IP_MODE_STATIC);
    MYASSERT(pWnd);
	switch(pList->pStationObj->m_nIPFlag)
	{
	case 0://����
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_ALONE));
		break;
	case 1://˫��
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_DOUBLE_MACHINES));
		break;
	case 2:
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_DOUBLE_CHANNELS));
		break;
	}

	if(pList != NULL)
	{
		CString str;
		str.Format("��վ[%s]״̬��Ϊ��¼��Ϊ%d", pList->pStationObj->m_sID, pList->recordList.GetCount());
		WriteLog(str);
	}

	MYASSERT(startPos);
    Record* pRecord = pList->recordList.GetAt(startPos);
	pList->recordList.GetPrev(startPos);

	do
	{
		int nItem = m_List.InsertItem(0, "");
		SetRecordInfo(pRecord, nItem);
		MYASSERT(startPos);
	    pRecord = pList->recordList.GetPrev(startPos);
		MYASSERT(pRecord);
	}while(GetStatus(pRecord));
}


void  DlgDetailStatus::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
			
				if (nSubItem == 0 || nSubItem == 3)
				{
					CString strStatus = m_List.GetItemText(nItem, nSubItem);
					if (!strStatus.CompareNoCase( StringFromID(IDS_COMMUSTATUS_OFF)))
					{
						lplvcd->clrText = RGB(255, 0, 0);
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
 ���ܸ�Ҫ���ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ����param1	�Ƚ϶���1
		   Param2	�Ƚ϶���2
		   param3	�Ƚϲ���
**************************************************************/
int CALLBACK DlgDetailStatus::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//ȡ�ñȽ�����
	Record* pRecord1 = (Record*)lParam1;
	Record* pRecord2 = (Record*)lParam2;
	int nCol = (int)lParamData;
	
	int iResult = 0;
	switch(nCol)
	{
	case 2: //ԭ��ı�ʱ��
		iResult = lstrcmpi( pRecord1->m_sReasonTime, pRecord2->m_sReasonTime );
		break;
	case 5: //��ͨ��ԭ��ı�ʱ��
		iResult = lstrcmpi( pRecord1->m_sReasonTime1, pRecord2->m_sReasonTime1 );
		break;	
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDetailStatusAsc;
	return iResult;
}

void DlgDetailStatus::OnColumnclickListDetailStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iDetailStatusAsc = -g_iDetailStatusAsc;
	}
	else
	{
		g_iDetailStatusAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

BOOL DlgDetailStatus::SaveListConfig()
{
	//����UI�����ļ�
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //��ʼ��COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!");
			WriteLog("ViewDetailStatusConfig::����UIConfigʧ��", XJ_LOG_LV3);
			bResult = FALSE;
		} 	

		//����UIConfig�ļ� 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//����ʧ��
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str);
				WriteLog("ViewDetailStatusConfig::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewDetailStatusConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewDetailStatusConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewDetailStatusConfig");
					if(pNew != NULL)
					{
						//�����½ڵ�
						pRoot ->appendChild(pNew);
						//�½������ӽڵ�
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //����
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							//�����ӽڵ�
							pNew ->appendChild(pNewChild);
						}
					}
				}
				else //�ҵ���ViewDetailStatusConfig
				{
					//�����Ƿ����ӽڵ�
					if(!pSelected ->hasChildNodes())
					{
						//�Ҳ����ӽڵ�
						WriteLog("δ�ҵ�ViewDetailStatusConfig�µ�����, �½������ӽڵ�");
						//�½������ӽڵ�
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //����
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							pSelected ->appendChild(pNewChild);
						}
					}
					else
					{
						//�ҵ����ӽڵ�, ɾ�������ӽڵ�,���½�
						WriteLog("�ҵ���ViewDetailStatusConfig�µ�����, ��ɾ���½������ӽڵ�");
						//�ҵ��ӽڵ��б�ָ��
						MSXML2::IXMLDOMNodeListPtr pChild;
						pChild = pSelected ->GetchildNodes();
						
						//ɾ���ӽڵ�
						MSXML2::IXMLDOMNodePtr pNode = NULL;
						pNode = pChild ->Getitem(0);
						while(pNode != NULL)
						{
							pSelected ->removeChild(pNode);
							pNode = pChild ->Getitem(0);
						}
						
						//�½��ӽڵ�
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //����
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							pSelected ->appendChild(pNewChild);
						}
					}
				}
				//���������ļ�
				pDoc ->save(_variant_t(g_UIConfig));
				bResult = TRUE;
			}
		}	
	}
	//�رմ򿪵�COM
	CoUninitialize();
	return bResult;
}


BOOL DlgDetailStatus::NeedSave()
{
	BOOL bReturn = FALSE;
	
	//�ȱ�������Ϣ
	int nCount = m_List.m_hideHeader.m_aColum.GetSize();
	for(int j = 0; j < nCount; j++)
	{
		if(!m_List.m_hideHeader.m_aColum[j].bHide)
		{
			//ֻ����������е��п�
			m_List.m_hideHeader.m_aColum[j].nItemWidth = m_List.GetColumnWidth(j);
		}
	}
	
	if(m_arrColum.GetSize() == 0)
	{
		//m_arrColumΪ��, ��Ҫ����
		bReturn = TRUE;
	}
	else if(m_arrColum.GetSize() != nCount)
	{
		//������������,��Ҫ����
		bReturn = TRUE;
	}
	else
	{
		//�Աȸ���,����һ��Ⱦ���Ҫ����
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			if(m_arrColum[i].bHide != m_List.m_hideHeader.m_aColum[i].bHide)
			{
				bReturn = TRUE;
				break;
			}
			if(m_arrColum[i].nItemWidth != m_List.m_hideHeader.m_aColum[i].nItemWidth)
			{
				bReturn = TRUE;
				break;
			}
			if(m_arrColum[i].sName != m_List.m_hideHeader.m_aColum[i].sName)
			{
				bReturn = TRUE;
				break;
			}
		}
	}
	if(bReturn)
	{
		//�����Ҫ����, ����дm_arrColum
		m_arrColum.RemoveAll();
		for(int i = 0; i < nCount; i++)
		{
			SColumn col;
			col.sName = m_List.m_hideHeader.m_aColum[i].sName;
			col.nItemWidth = m_List.m_hideHeader.m_aColum[i].nItemWidth;
			col.bHide = m_List.m_hideHeader.m_aColum[i].bHide;
			m_arrColum.Add(col);
		}
	}
	return bReturn;
}

void DlgDetailStatus::OnDestroy() 
{	
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CDialog::OnDestroy();
	
	//TODO: Add your message handler code here
	
}
