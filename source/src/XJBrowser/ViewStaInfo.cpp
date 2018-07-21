// ViewStaInfo.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"

#include "ViewStaInfo.h"
#include "MainFrm.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/** @brief           ����������λ*/
int g_iStationInfoViewDesc;

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo

IMPLEMENT_DYNCREATE(CViewStaInfo, CFormView)

CViewStaInfo::CViewStaInfo()
	: CFormView(CViewStaInfo::IDD)
{
	g_ShowCommOFF = 1;
	g_ShowCommON = 1;
	g_iStationInfoViewDesc = -1;
	m_nTimer = 0;
	//{{AFX_DATA_INIT(CViewStaInfo)
	//}}AFX_DATA_INIT
}

CViewStaInfo::~CViewStaInfo()
{
}

void CViewStaInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStaInfo)
	DDX_Control(pDX, IDC_LIST_STAINFO, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStaInfo, CFormView)
	//{{AFX_MSG_MAP(CViewStaInfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STAINFO, OnColumnclickListStationinfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STAINFO, OnCustomDraw)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_COMM_ON, OnCommOn)
	ON_COMMAND(IDC_COMM_OFF, OnCommOff)
	ON_COMMAND(IDC_STATION_HISTORY, OnStationHistory)
	ON_COMMAND(IDC_COMM_EXPORT, OnCommExport)
	ON_COMMAND(IDC_COMM_PRINT, OnCommPrint)
	ON_COMMAND(IDC_STATION_TREE, OnStationTree)
	ON_COMMAND(IDC_STATION_MAP, OnStationMap)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStaInfoOpen)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo diagnostics

#ifdef _DEBUG
void CViewStaInfo::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewStaInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo message handlers
/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B850169
int CViewStaInfo::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//��ʼ���б���
	InitListStyle();
	if (LoadProNameFromDB()==FALSE)
	{
		return false;
	}

// 	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
// 	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
// 	if(pData == NULL)
// 		return -1;
// 	
// 	DEVICE_LIST listDevice;
// 	listDevice.RemoveAll();
// 	pData->GetStationList(listDevice);
// 	POSITION pos = listDevice.GetHeadPosition();
// 	
// 	while(pos != NULL)
// 	{
// 		//ȡ�ó�վָ��
// 		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
// 		LoadStatusFromDB(pObj);
// 		
// 	}//FillListData();
	return 0;
}
/*************************************************************
�� �� ����InitListStyle()
���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
�� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B850177
int CViewStaInfo::InitListStyle()
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
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			lvCol.iSubItem = i;
			lvCol.cx = m_arrColum[i].nItemWidth;
			CString sName = m_arrColum[i].sName;
			char * name = sName.GetBuffer(sName.GetLength());
			lvCol.pszText = name;
			BOOL bHide = m_arrColum[i].bHide;
			
			m_List.InsertColumn(i,&lvCol, bHide);
		}
	}
	else
	{
		//��������ʧ��
		//char* arColumn[14]={"����","��վ����","ͨѶ״̬","��������", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��", "�Ͽ�ԭ��","��վ����","��վIP","��վ����","��վ�ͺ�","��վ�汾","ͨѶ��Լ","��ʷ��¼"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 14; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; // ����
				colName = StringFromID(IDS_MODEL_AREA);
				break;
			case 1:
				lvCol.cx = 100; // ��վ����
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 2://
				lvCol.cx = 100; // ͨѶ״̬
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3:
				lvCol.cx = 120; // ��������
				colName = StringFromID(IDS_ORDER_STRATEGY);
				break;
			case 4://
				lvCol.cx = 180; // ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 5:
				lvCol.cx = 200; // ����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 6:
				lvCol.cx = 200; // �Ͽ�ԭ��
				colName = StringFromID(IDS_DISCONNECT_REASON);
				break;
			case 7:
				lvCol.cx = 120; // ��վ����
				colName = StringFromID(IDS_MODEL_MANUFACTURER);
				break;
			case 8:
				lvCol.cx = 180; // ip
				colName = StringFromID(IDS_COMMON_IP);
				break;
			case 9:
				lvCol.cx = 160; // ����
				colName = StringFromID(IDS_COMMON_GATEWAY);
				break;
			case 10:
				lvCol.cx = 100; // ��վ�ͺ�
				colName = StringFromID(IDS_COMMON_MODEL);
				break;
			case 11:
				lvCol.cx = 100; // ��վ�汾
				colName = StringFromID(IDS_COMMON_VERSION);
				break;
			case 12:
				lvCol.cx = 160; // ͨѶ��Լ
				colName = StringFromID(IDS_COMMON_PROTOCOL);
				break;
			case 13:
				lvCol.cx = 0; //��ʷ��¼
				colName = StringFromID(IDS_HISTORY_RECORD);
				break;
			default:
				lvCol.cx=50;//�п�100����
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}
/*************************************************************
 �� �� ����FillListData()
 ���ܸ�Ҫ������б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B850178
int CViewStaInfo::FillListData()
{
	
	
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	//if(m_pStation == NULL)
	//	return -1;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
	//ѭ������
	
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		//LoadStatusFromDB(pObj);
		CDeviceObj* pDev = pData->FindDevice(pObj->m_sNetID,TYPE_NET);
		if(pObj == NULL)
			continue;
		if ((!pObj->IsCommStatusON()&&0==g_ShowCommOFF)||(pObj->IsCommStatusON()&&0==g_ShowCommON))
		{
		}
		else
			AddStationInfoToList(pObj,pDev, 0);
	}


	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return true;

}
/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ�λ��
 �� �� ֵ: ����ֵ˵��
**************************************************************/
//##ModelId=49B87B8501A5
void CViewStaInfo::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//�����б���С,ռ�����߶ȵ�(1 - 0.618)
	float fHList = (float)rcList.Height();
	fHList = float(fHList);
	rcList.bottom = (long)fHList;
	m_List.MoveWindow(&rcList, TRUE);
}

void CViewStaInfo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//��ʼ���б�
	InitListCtrl();
	
	m_nTimer = SetTimer(1, g_RefreshTime*60*1000, 0);

	FillListData();
	//FillListData();
}


/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B850186
BOOL CViewStaInfo::LoadListConfig()
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
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", XJ_LOG_LV3);
			WriteLog("ViewStaInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("ViewStaInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewStaInfoConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewStaInfoConfig", XJ_LOG_LV3);
				WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewStaInfoConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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


void CViewStaInfo::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}
/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�,���Ҫ�����λ���Ѿ���������, ԭ�ȵ�����
			�����Զ�����,����������ȷλ��	
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
//##ModelId=49B87B8501B7
BOOL CViewStaInfo::AddStationInfoToList(CStationObj* pObj,CDeviceObj* pDev, int i)
{
	//�ж�������Ч��
	if(pObj == NULL)
		return FALSE;
	//�ж�������Ч��
	if(pDev == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	
	//ˢ�³���ʱ��
//	pSec->RefreshCommDurativeTime();
//	pSec->RefreshRunDurativeTime();
	pObj->RefreshDurativeTime();

	//"����","��վ����","ͨѶ״̬","��������", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��", "�Ͽ�ԭ��","��վ����","��վIP","��վ����","��վ�ͺ�","��վ�汾","ͨѶ��Լ","��ʷ��¼"
	//0:����
	CString str;
//	str.Format("%s(%s)", pObj->m_sName, pObj->m_sModel);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, pDev->m_sName, 0, 0, 0, i) == -1)
		return FALSE;
	//1:��վ����
	m_List.SetItemText(i, 1, pObj->m_sName);
	//2:ͨѶ״̬
	if (pObj->IsCommStatusON())
	{
		m_List.SetItemText(i, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}
	else
	{
		m_List.SetItemText(i, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}

	//3:��������
	if (pObj->m_nOrderStrategy!=0)
	{
		m_List.SetItemText(i, 3, StringFromID(IDS_ORDERSTRATEGY_SMART));
	}
	else
	{
		m_List.SetItemText(i,3,StringFromID(IDS_ORDERSTRATEGY_ALL));
	}
	
	//4:ͨѶ״̬�ı�ʱ��
	m_List.SetItemText(i, 4, pObj->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"));
	//5:ͨѶ״̬����ʱ��
	m_List.SetItemText(i, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));
	//6:�Ͽ�ԭ��
	m_List.SetItemText(i, 6, pObj->GetStatusReason());
	//7:��վ����
	m_List.SetItemText(i,7,pObj->m_sManufacturer);
	//8:ip
	m_List.SetItemText(i, 8, pObj->m_sIP);
	//9:����
	m_List.SetItemText(i, 9, pObj->m_sTel);
	//10:��վ�ͺ�
	m_List.SetItemText(i, 10, pObj->m_sStationModel);
	//11:��վ�汾
	m_List.SetItemText(i, 11, pObj->m_sStationVer);
	//12:ͨѶ��Լ
	m_List.SetItemText(i, 12, pObj->m_sProtocol);
// 	m_List.SetColumnWidth(0,80);
// 	m_List.SetColumnWidth(7,80);
// 	m_List.SetColumnWidth(10,0);
// 	m_List.SetColumnWidth(11,0);
// 	m_List.SetColumnWidth(13,0);
	//��������
	m_List.SetItemData(i, (DWORD)pObj);
	return TRUE;
}
/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CViewStaInfo::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����

	CStationObj* pObj3 = (CStationObj*)lParam1;
	CStationObj* pObj4 = (CStationObj*)lParam2;

//	if((pObj2 == NULL || pObj1 == NULL)&&(!(pObj2 == NULL && pObj1 == NULL)))
//		return 0;
	if ((pObj3 == NULL || pObj4 == NULL)&&(!(pObj3 == NULL && pObj4 == NULL)))
		return 0;
	
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"����","��վ����","ͨѶ״̬","��������", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��", "�Ͽ�ԭ��","��վ����","��վIP","��վ����","��վ�ͺ�","��վ�汾","ͨѶ��Լ","��ʷ��¼"
	switch(nCol)
	{
	case 0: //����
 		str1.Format("%s",pObj3->m_sNetID);
 		str2.Format("%s",pObj4->m_sNetID);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1://��վ����
		str1.Format("%s(%s)",pObj3->m_sNetID,pObj3->m_sName);
		str2.Format("%s(%s)",pObj4->m_sNetID,pObj4->m_sName);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2://ͨѶ״̬
		iResult = pObj3->IsCommStatusON() - pObj4->IsCommStatusON();

		break;

	case 3://��������
		str1.Format("%d",pObj3->m_nOrderStrategy);
		str2.Format("%d",pObj4->m_nOrderStrategy);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4://ͨѶ״̬�ı�ʱ��
		str1 = pObj3->GetStatusTime().Format("%Y-%m-%d %H:%M:%S");
		str2 = pObj4->GetStatusTime().Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 5: //ͨѶ״̬����ʱ��
		iResult = pObj3->GetStatusDurative().GetTotalSeconds() - pObj4->GetStatusDurative().GetTotalSeconds();
		break;
	case 6://�Ͽ�ԭ��

		str1 = pObj3->GetStatusReason();
		str2 = pObj4->GetStatusReason();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
 		break;
	case 7://��վ����
		str1 = "";
		if(pObj3->m_sManufacturer != "")
			str1 = pObj3->m_sManufacturer;
		str2 = "";
		if(pObj4->m_sManufacturer != "")
			str2 = pObj4->m_sManufacturer;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://��վIP
		str1 = "";
		if(pObj3->m_sIP != "")
			str1 = pObj3->m_sIP;
		str2 = "";
		if(pObj4->m_sIP != "")
			str2 = pObj4->m_sIP;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9://��վ����
		str1 = "";
		if(pObj3->m_sTel != "")
			str1 = pObj3->m_sTel;
		str2 = "";
		if(pObj4->m_sTel != "")
			str2 = pObj4->m_sTel;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 10://��վ�ͺ�"
		break;
	case 11://��վ�汾
		break;
	case 12://ͨѶ��Լ
		str1 = "";
		if(pObj3->m_sProtocol != "")
			str1 = pObj3->m_sProtocol;
		str2 = "";
		if(pObj4->m_sProtocol != "")
			str2 = pObj4->m_sProtocol;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 13://��ʷ��¼
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iStationInfoViewDesc;
	return iResult;
}
/*************************************************************
 �� �� ��: OnStationInfoOpen()
 ���ܸ�Ҫ: ��Ӧҳ��򿪻�����������Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStaInfo::OnStaInfoOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//ȡ���³�վָ��
	m_pStation = (CStationObj*)lParam;
	//��������
	FillListData();
}



/*************************************************************
 �� �� ����LoadStatusFromDB()
 ���ܸ�Ҫ����Ӳ�����ݿ����ͨѶ��Լ����
 �� �� ֵ: 	
**************************************************************/
BOOL CViewStaInfo::LoadProNameFromDB()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)
	
	//�����ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//��������
// 	Condition Condition0;
 	CString str;
// 	str.Format("ID = '%s'", pObj->m_sID);
// 	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	// 	Condition Condition1;
	// 	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC", EX_STTP_WHERE_ABNORMALITY);
	// 	
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_PROTOCOL_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewStaInfo::LoadStatusFromDB, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
			//ѭ������
			
			POSITION pos = listDevice.GetHeadPosition();
			while(pos != NULL)
			{
				CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
				if (pObj->m_nProtocolId == atoi(pMemset.GetValue((UINT)0)))
				{
					pObj->m_sProtocol = pMemset.GetValue(1);
				}
			}
			pMemset.MoveNext();		
		}
		
	}
	else
	{
		str.Format("CViewStaInfo::LoadStatusFromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
	//	pMemset = NULL;
	return TRUE;
}
void CViewStaInfo::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//��״̬��������ɫ
				if(nItem  != -1)
				{
					//���
					if(nItem%2 != 0)
					{
						if (g_style == 1)
						{
							lplvcd->clrTextBk = g_ListSpaceColor2;
						}
						else if(g_style == 2)
						{
							lplvcd->clrTextBk = g_ListSpaceColor3;
						}
						else if(g_style == 3)
						{
							lplvcd->clrTextBk = g_ListSpaceColor4;
						}
						else
						{
							lplvcd->clrTextBk = g_ListSpaceColor;
						}
					}
					
					//�õ��¼�����
					CStationObj * pObj = (CStationObj*)m_List.GetItemData(nItem); 
					if(pObj == NULL)
						break;
					if(nSubItem == 2) //ͨѶ״̬
					{
						if(!pObj->IsCommStatusON())
						{
							//�Ͽ�
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
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
void CViewStaInfo::OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iStationInfoViewDesc = - g_iStationInfoViewDesc;
	}
	else
	{
		g_iStationInfoViewDesc = 1;
		m_nOldSortCol = iCol;
	}
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ��: SaveListConfig()
 ���ܸ�Ҫ: ��������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewStaInfo::SaveListConfig()
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
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", XJ_LOG_LV3);
			WriteLog("ViewStaInfo::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("ViewStaInfo::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewStaInfoConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewStaInfoConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewStaInfoConfig");
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
				else //�ҵ���ViewActionConfig
				{
					//�����Ƿ����ӽڵ�
					if(!pSelected ->hasChildNodes())
					{
						//�Ҳ����ӽڵ�
						WriteLog("δ�ҵ�ViewStaInfoConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewStaInfoConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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

/*************************************************************
 �� �� ��: NeedSave()
 ���ܸ�Ҫ: �ж��Ƿ���Ҫ�����б������Ϣ
 �� �� ֵ: ��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewStaInfo::NeedSave()
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


void CViewStaInfo::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void CViewStaInfo::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//CListCtrlEx�Ƚ�ȡ��WM_CONTEXTMENU��Ϣ, ���������ת��������ߴ���
	//���������ȵõ���Ϣ�����ٴ�����Ϣ
	LPMSG  msg;
	msg = (LPMSG)GetCurrentMessage();
	UINT wParam = (UINT)msg ->wParam;
	if(wParam == LISTCTRLEX_MARK)
	{
		CPoint * ptTemp = (CPoint*)msg->lParam;
		point = *ptTemp;
		
		//�ж����һ�����ͷ�����б�,ֻ�����б�,��ͷ��CListCtrlEx�д���
		CPoint pt = point;
		m_List.ScreenToClient(&pt);
		
		CWnd* pChildWnd = m_List.ChildWindowFromPoint(pt);
		if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() != m_List.GetSafeHwnd())
		{
			char szClass [50];
			
			// Verify that this window handle is indeed the header
			// control's by checking its classname.
			GetClassName(pChildWnd->GetSafeHwnd(), szClass, 50);
			if (!lstrcmp (szClass, "SysHeader32"))
			{
			}
		}
		else if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() == m_List.GetSafeHwnd())
		{//�������ListCtrl�ķ���ͷ����
			//���ɲ˵�
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_STATION))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_STATION_MAP, MF_GRAYED);
				pMenu->EnableMenuItem(IDC_STATION_TREE, MF_GRAYED);
			}
			
			if(1 == g_ShowCommON)
			{
				//��ʾͨ��״̬Ϊ������װ����Ϣ
				pMenu->CheckMenuItem(IDC_COMM_ON, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_COMM_ON, MF_UNCHECKED);
			}
			if(1 == g_ShowCommOFF)
			{
				//��ʾͨ��״̬Ϊ�Ͽ���װ����Ϣ
				pMenu->CheckMenuItem(IDC_COMM_OFF, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_COMM_OFF, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

void CViewStaInfo::OnCommOn() 
{
	// TODO: Add your command handler code here
	g_ShowCommON == 0 ? g_ShowCommON = 1 :	g_ShowCommON = 0;

	FillListData();
}

void CViewStaInfo::OnCommOff() 
{
	// TODO: Add your command handler code here
	g_ShowCommOFF == 0 ? g_ShowCommOFF = 1 :  g_ShowCommOFF = 0;
	
	FillListData();
}

void CViewStaInfo::OnStationHistory() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	//׼���򿪲�ѯ��ͳ�ƵĲ���
	//����վͨѶ״̬��ѯ: 1$$$��վID$$$ͨѶ״̬$$$�Զ���ѯ(0-���Զ���ѯ, 1-�Զ���ѯ)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
	{
		sSQL = "1$$$ $$$99$$$0";
	}
	else if(nCount == 1)
	{
	//	int nIndex=((CListCtrl*)GetDlgItem(IDC_LIST_STAINFO))->GetItemPosition();
		
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetStationList(listDevice);
		POSITION pos = NULL;
		for(pos = listDevice.GetHeadPosition(); pos != NULL;)
		{
			CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
//			if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(nIndex)))
		//	{
				//
				sSQL.Format("1$$$%s$$$99$$$1", pStation->m_sID);
				break;
		//	}		
		}
	}
	//�򿪲�ѯ��ͳ��
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

void CViewStaInfo::OnCommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s.xls", StringFromID(IDS_TOOLBAR_STATIONINFO),t.Format("%Y%m%d%H%M%S"));
	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//�����ɹ�
		if(AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_SUCCESS), MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//����ʧ��
		AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_FAIL));
	}
}

void CViewStaInfo::OnCommPrint() 
{
	// TODO: Add your command handler code here
	
}

void CViewStaInfo::OnStationTree() 
{
	// TODO: Add your command handler code here
	
}

void CViewStaInfo::OnStationMap() 
{
	// TODO: Add your command handler code here
	
}
/*************************************************************
 �� �� ����WriteResultToExcel()
 ���ܸ�Ҫ�����б��е����ݵ�����Excel
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Excel�ļ�����λ��
		   Param2
**************************************************************/
BOOL CViewStaInfo::WriteResultToExcel( CString strFullPath )
{
	CDatabase database;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString strSQL,str;
	
	strSQL.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		strDriver, strFullPath, strFullPath);
	
	CFileFind find;
	BOOL IsFind = FALSE;
	IsFind = find.FindFile(strFullPath, 0);
	
	try
	{
		if( database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{
			if(!IsFind)
			{
				//�½�
					//"����","��վ����","ͨѶ״̬","��������", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��", "�Ͽ�ԭ��","��վ����","��վIP","��վ����","��վ�ͺ�","��վ�汾","ͨѶ��Լ","��ʷ��¼"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, ͨѶ��Լ TEXT)",
					StringFromID(IDS_MODEL_AREA), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_COMMU_STATUS), StringFromID(IDS_ORDER_STRATEGY),
					StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION),StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_MODEL_MANUFACTURER),
					StringFromID(IDS_COMMON_IP), StringFromID(IDS_COMMON_GATEWAY), StringFromID(IDS_COMMON_MODEL), StringFromID(IDS_COMMON_VERSION),
					StringFromID(IDS_COMMON_PROTOCOL));
				database.ExecuteSQL(strSQL);
			}

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CStationObj* pObj = (CStationObj*)m_List.GetItemData(i);
				CDeviceObj* pDev = pData->FindDevice(pObj->m_sNetID,TYPE_NET);
				if(pObj == NULL)
					continue;
				AddResultToExcel(&database, pObj,pDev);
			}
		}
		else
			return FALSE;
		database.Close();
	}
	catch(CDBException * e)
	{
		TRACE("���ݿ����: " + e->m_strError);
		return FALSE;
	}
	return TRUE;

}
	/*************************************************************
 �� �� ����AddResultToExcel()
 ���ܸ�Ҫ����ָ���Ľ����д�뵽ָ����Excel�ļ���
 �� �� ֵ: void
 ��    ����param1	���ݿ����
		   Param2	�豸����
**************************************************************/
void CViewStaInfo::AddResultToExcel(CDatabase* pDatabase,CStationObj* pStation,CDeviceObj* pDev)
{
	//���������Ч��
	MYASSERT(pDatabase);
	MYASSERT(pStation);

		//"����","��վ����","ͨѶ״̬","��������", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��", "�Ͽ�ԭ��","��վ����","��վIP","��վ����","��վ�ͺ�","��վ�汾","ͨѶ��Լ","��ʷ��¼"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s','%s','%s','%s', '%s', '%s', '%s', '%s')",
		pDev->m_sName,pStation->m_sName, (pStation->IsCommStatusON())?StringFromID(IDS_COMMUSTATUS_ON):StringFromID(IDS_COMMUSTATUS_OFF),
		(pStation->m_nOrderStrategy==0)?StringFromID(IDS_ORDERSTRATEGY_ALL):StringFromID(IDS_ORDERSTRATEGY_SMART), 
		pStation->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"), pStation->GetStatusDurative().Format(GetDurationFormatStr()), 
		pStation->GetStatusReason(), pStation->m_sManufacturer, pStation->m_sIP, pStation->m_sTel, pStation->m_sStationModel,  
		pStation->m_sStationVer,pStation->m_sProtocol);
	pDatabase->ExecuteSQL(strSQL);
}


void CViewStaInfo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//ˢ�³�վ����ʱ��
		if(!g_bSuspend)
			RefreshTime();
	}
	
	CFormView::OnTimer(nIDEvent);
}
/*************************************************************
 �� �� ��: RefreshTime()
 ���ܸ�Ҫ: ˢ�����б�����ͨ��״̬������״̬�ĳ���ʱ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStaInfo::RefreshTime()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	int nCount = m_List.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		if(g_bSuspend)
			break;
		RefreshDuravieTime(i);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ��: RefreshDuravieTime()
 ���ܸ�Ҫ: ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ
 �� �� ֵ: 
 ��    ��: param1 ָ����LIST�е�λ��
		   Param2 
**************************************************************/
void CViewStaInfo::RefreshDuravieTime( int nIndex )
{
	//����
	int nCount = m_List.GetItemCount();
	//�ж�������Ч��
	if(nIndex < 0)
		return;
	if(nIndex >= nCount)
		return;
	CStationObj* pObj = (CStationObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;


	//ˢ������״̬����ʱ��
	pObj->RefreshDurativeTime();
	//������ʾ
	m_List.SetItemText(nIndex, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));


	//ˢ��
	m_List.Update(nIndex);
}
/*************************************************************
 �� �� ��: OnSTTP20002()
 ���ܸ�Ҫ: ��ӦͨѶ״̬�ı䱨��
 �� �� ֵ: 
 ��    ��: param1 �豸����ָ��
		   Param2 
**************************************************************/
void CViewStaInfo::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewStaInfo::OnSTTP20144 start", XJ_LOG_LV3);
	
	//���ҳ�վ
	CStationObj * pObj = (CStationObj*)lParam;
	if(!pObj)
		return;
	
	ChangeStationStatusInList(pObj);

    //SetTimer(1, g_RefreshStationTime*60*1000, 0);
	WriteLog("CViewStaInfo::OnSTTP20144 neat end", XJ_LOG_LV3);
}
void CViewStaInfo::ChangeStationStatusInList(CStationObj* pObj)
{
//�ж�������Ч��
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	//�������б��е�λ��
	LVFINDINFO lvFind;
	int nIndex = -1;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
	nIndex = m_List.FindItem(&lvFind);
	
	if(nIndex < 0)
	{
		//δ�ҵ�������ʾ
		return;
	}
	else
	{
		//�ı�ֵ
		//1:ͨѶ״̬
			//2:ͨѶ״̬
	if (pObj->IsCommStatusON())
	{
		m_List.SetItemText(nIndex, 2, StringFromID(IDS_COMMUSTATUS_ON));
	}
	else
	{
		m_List.SetItemText(nIndex, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}
	
	//4:ͨѶ״̬�ı�ʱ��
	m_List.SetItemText(nIndex, 4, pObj->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"));
	//5:ͨѶ״̬����ʱ��
	m_List.SetItemText(nIndex, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));
	//6:�Ͽ�ԭ��
	m_List.SetItemText(nIndex, 6, pObj->GetStatusReason());
	}
}

void CViewStaInfo::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONINFO));
//	GetParentFrame()->SetWindowText("��վ��Ϣ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewStaInfo::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	FillListData();
	WriteLog("StationInit,��վ��Ϣҳ��ˢ�����", XJ_LOG_LV3);
}

void CViewStaInfo::OnViewRefresh(WPARAM wParam, LPARAM lParam)
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		ChangeStationStatusInList(pObj);
	}
	
	WriteLog("CViewStaInfo::OnViewRefresh, ��վ��Ϣҳ��ǿ��ˢ�����", XJ_LOG_LV3);
}
