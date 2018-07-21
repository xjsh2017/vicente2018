// ViewSOE.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewSOE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iSOEViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewSOE

IMPLEMENT_DYNCREATE(CViewSOE, CFormView)

CViewSOE::CViewSOE()
	: CFormView(CViewSOE::IDD)
{
	//{{AFX_DATA_INIT(CViewSOE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMaxRow = 200;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_arrColum.RemoveAll();
	m_nType = 0;
	g_iSOEViewAsc = -1;
	m_nOldSortCol  = -1;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
}

CViewSOE::~CViewSOE()
{
}

void CViewSOE::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSOE)
	DDX_Control(pDX, IDC_LIST_SOE, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSOE, CFormView)
	//{{AFX_MSG_MAP(CViewSOE)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SOE, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SOE, OnColumnclickListSoe)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOE, OnClickListSoe)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SOE, OnItemchangedListSoe)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SOE, OnDblclkListSoe)
	ON_COMMAND(IDC_EVENT_FILTER_RETURN, OnEventFilterReturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSOE diagnostics

#ifdef _DEBUG
void CViewSOE::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSOE::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSOE message handlers
/*************************************************************
 �� �� ��: InitListCtrl()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewSOE::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	RefreshData();

	return 0;
}

/*************************************************************
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewSOE::InitListStyle()
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
		//char* arColumn[8]={"���","��Ϣ���","�����豸","����ʱ��", "��Ϣ����","��վ��","һ���豸","��ȷ��"};
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; //���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; //���
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 300; //�����豸
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 3://
				lvCol.cx = 180; //����ʱ��
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 4:
				lvCol.cx = 200; //��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 5:
				lvCol.cx = 140; //"��վ
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 6:
				lvCol.cx = 160; //һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 7:
				lvCol.cx = 80;//��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(7, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 �� �� ��: FillListData()
 ���ܸ�Ҫ: ����б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewSOE::FillListData()
{
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	CXJEvent * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEvent*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}

	g_iSOEViewAsc = -1;
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, 3);
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 �� �� ��: LoadListConfig()
 ���ܸ�Ҫ: ��������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewSOE::LoadListConfig()
{
		//��ȡUI�����ļ�
	BOOL bResult = TRUE;
	CString strNodeName = "ViewSOEIConfig";
	if(m_nType == 1)
		strNodeName = "ViewSOEIIConfig";
	else if(m_nType == 2)
		strNodeName = "ViewSOEIIIConfig";
	else if(m_nType == 3)
		strNodeName = "ViewSOEDebugConfig";
	
	CoInitialize(NULL); //��ʼ��COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", XJ_LOG_LV3);
			WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			CString str;
			str.Format("//%s", strNodeName);
			pSelected = pDoc->selectSingleNode((_bstr_t)str);

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
// 				WriteLog("δ�ҵ�ViewAlarmConfig");
// 				WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
// 					WriteLog("δ�ҵ�ViewAlarmConfig�µ�����");
// 					WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

/*************************************************************
 �� �� ��: SaveListConfig()
 ���ܸ�Ҫ: ��������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewSOE::SaveListConfig()
{
		//����UI�����ļ�
	BOOL bResult = TRUE;
	CString strNodeName = "ViewSOEIConfig";
	if(m_nType == 1)
		strNodeName = "ViewSOEIIConfig";
	else if(m_nType == 2)
		strNodeName = "ViewSOEIIIConfig";
	else if(m_nType == 3)
		strNodeName = "ViewSOEDebugConfig";
	
	CoInitialize(NULL); //��ʼ��COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
// 			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!");
// 			WriteLog("ViewAlarm::����UIConfigʧ��", XJ_LOG_LV3);
			bResult = FALSE;
		} 	

		//����UIConfig�ļ� 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//����ʧ��
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
// 				CString str;
// 				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
// 				WriteLog(str);
// 				WriteLog("ViewAlarm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				CString str;
				str.Format("//%s", strNodeName);
				pSelected = pDoc->selectSingleNode((_bstr_t)str);
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
				//	WriteLog("δ�ҵ�ViewAlarmConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement((_bstr_t)strNodeName);
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
					//	WriteLog("δ�ҵ�ViewAlarmConfig�µ�����, �½������ӽڵ�");
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
					//	WriteLog("�ҵ���ViewAlarmConfig�µ�����, ��ɾ���½������ӽڵ�");
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
BOOL CViewSOE::NeedSave()
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

/*************************************************************
 �� �� ��: AddEventToList()
 ���ܸ�Ҫ: ��ָ���¼����뵽�б�����ʾ
 �� �� ֵ: �ɹ�����TRUE��ʧ�ܷ���FALSE
 ��    ��: param1 ָ���¼�
		   Param2 ָ�����б��е�λ��
**************************************************************/
BOOL CViewSOE::AddEventToList( CXJEvent * pEvent, int i /*= 0*/ )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:��Ϣ����
	if(pEvent->m_nEventType == XJ_EVENT_ACTION)
	{
		//����
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_ACTION));
	}
	else if(pEvent->m_nEventType == XJ_EVENT_ALARM)
	{
		//�澯
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_ALARM));
	}
	else if(pEvent->m_nEventType == XJ_EVENT_DI)
	{
		//������
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_DI));
	}
	else
	{
		//δ֪
		m_List.SetItemText(i, 1, StringFromID(IDS_COMMON_UNKNOWN));
	}
	
	//2:��������
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 2, str);
	//3:����ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, sTime);
	//4:��Ϣ����
	if(pEvent->m_nEventType == XJ_EVENT_DI)
	{
		CXJEventDI* pDI = (CXJEventDI*)pEvent;
		str.Format("%s %s", pDI->m_sDIName, DisplayValue(pDI->m_nValue));
		m_List.SetItemText(i, 4, str);
	}
	else
		m_List.SetItemText(i, 4, pEvent->m_sMsg);
	//5:��վ��
	m_List.SetItemText(i, 5, pEvent->m_sStationName);
	//6:һ���豸��
	m_List.SetItemText(i, 6, pEvent->m_sDeviceName);
	//7:��ȷ��
	m_List.SetItemText(i, 7, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ��: RegulateControlSize()
 ���ܸ�Ҫ: �����ؼ���С,λ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSOE::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//�����б���С,ռ�����߶�
	m_List.MoveWindow(&rc, TRUE);
}

void CViewSOE::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewSOE::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//��ȡ����
	LoadConfig();
	//��ʼ���б�ؼ�
	InitListCtrl();
}

void CViewSOE::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strTitle = "";
	if(m_nType == 0)
		strTitle = StringFromID(IDS_EVENTCLASS_I);
	else if(m_nType == 1)
		strTitle = StringFromID(IDS_EVENTCLASS_II);
	else if(m_nType == 2)
		strTitle = StringFromID(IDS_EVENTCLASS_III);
	else if(m_nType == 3)
		strTitle = StringFromID(IDS_EVENTCLASS_DEBUG);
	pApp->SetAppTile(strTitle);
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ��: LoadActionFromDB()
 ���ܸ�Ҫ: �����ݿ��ȡ��������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ���ҵĸ���
		   Param2 �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
**************************************************************/
BOOL CViewSOE::LoadActionFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//��Ϊ������Ϣ��Ҫ�Ƚ�����֪���Ƿ��¼��Ƿ񷵻���Ϣ���ȶ�ȡnum*5���¼�
	num *= 5;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//Ҫ��ѯ��ȷ���¼�, �����������
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//����ѯ��ȷ���¼�, �����ֻ��ѯδȷ���¼�������
		sql.Conditionlist.push_back(condition1);
	}

	//����������,�鼸���¼�
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	if(m_nType == 3)
	{
		//������Ϣ��ͼ
		str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	}
	else
		str.Format("property=%d", m_nType); //������Ϣ��ͼ
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//�����ʾ��������
	if(!m_bShowDebug && m_nType != 3)
	{
		//�ǵ�����Ϣ��ͼ
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSOE::LoadActionFromDB, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadActionFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		//����������
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			//�ж��ѷ��������ĸ���
			if(nChecked == m_nMaxRow)
				break;
			//�����¼�
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
			{
				//��ȡ���ݳɹ�
				if(!m_bShowReturn)
				{
					//����ʾ�����¼�
					if(pEvent->m_nIsReturn == 1)
					{
						//ɾ�����¼�
						delete pEvent;
						pEvent = NULL;
						pMemset->MoveNext();
						continue;
					}
				}
				//���뵽����
				m_EventManager.AddEvent(pEvent);
				nChecked++;
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewSOE::LoadActionFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CViewSOE::LoadActionFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 �� �� ��: LoadAlarmFromDB()
 ���ܸ�Ҫ: �����ݿ��ȡ�澯����
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ���ҵĸ���
		   Param2 �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
**************************************************************/
BOOL CViewSOE::LoadAlarmFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//��Ϊ������Ϣ��Ҫ�Ƚ�����֪���Ƿ��¼��Ƿ񷵻���Ϣ���ȶ�ȡnum*5���¼�
	num *= 5;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("is_ack = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//Ҫ��ѯ��ȷ���¼�, �����������
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//����ѯ��ȷ���¼�, �����ֻ��ѯδȷ���¼�������
		sql.Conditionlist.push_back(condition1);
	}

	//����������,�鼸���¼�
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	if(m_nType == 3)
	{
		//������Ϣ��ͼ
		str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	}
	else
		str.Format("property=%d", m_nType); //������Ϣ��ͼ
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//�����ʾ��������
	if(!m_bShowDebug && m_nType != 3)
	{
		//�ǵ�����Ϣ��ͼ
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
		
	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewSOE::LoadAlarmFromDB, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadAlarmFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		//����������
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			//�ж��ѷ��������ĸ���
			if(nChecked == m_nMaxRow)
				break;
			//�����¼�
			CXJEventAlarm * pEvent = new CXJEventAlarm;
			if(pEvent->LoadFromDB(pMemset))
			{
				//��ȡ���ݳɹ�
				if(!m_bShowReturn)
				{
					//����ʾ�����¼�
					if(pEvent->m_nIsReturn == 1)
					{
						//ɾ�����¼�
						delete pEvent;
						pEvent = NULL;
						pMemset->MoveNext();
						continue;
					}
				}
				//���뵽����
				m_EventManager.AddEvent(pEvent);
				nChecked++;
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewSOE::LoadAlarmFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CViewSOE::LoadAlarmFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ��: LoadDataFromDB()
 ���ܸ�Ҫ: �����ݿ��ȡ����
 �� �� ֵ: 
 ��    ��: param1 Ҫ���ҵĸ���
		   Param2 �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
**************************************************************/
void CViewSOE::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	//���������
	m_EventManager.ClearAllEvents();
	int nRowNum = num/2;
	if(m_nType == 3)
	{
		//������Ϣ��ͼ
		nRowNum = num/3;
	}
	//���붯��
	LoadActionFromDB(nRowNum, IsQueryAcked);
	//����澯
	LoadAlarmFromDB(nRowNum, IsQueryAcked);
	if(m_nType == 3)
	{
		//������Ϣ��ͼ,���뿪����
		LoadDIFromDB(nRowNum, IsQueryAcked);
	}
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������δȷ���¼��ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CViewSOE::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

void CViewSOE::OnDestroy() 
{
	//��������
	SaveConfig();
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CViewSOE::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_EVENTVIEW))
				return;

			/*
			Menu.DeleteMenu(IDC_EVENT_COPY, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_ACK, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_ALL, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_PROP, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_Refresh, MF_BYCOMMAND);
			*/

			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{
				//��ѡ��
				pMenu->EnableMenuItem(IDC_EVENT_COPY, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_ACK, MF_ENABLED);
			//	pMenu->EnableMenuItem(IDC_EVENT_SIGN, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
			}
			if(m_bShowAcked)
			{
				//������ȷ���¼�
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_CHECKED);
			}
			else
			{
				//��ʾ
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_UNCHECKED);
			}

			//�Ƿ���ʾ����
			if(m_nType != 3)
			{
				//�ǵ�����Ϣ��ͼ
				pMenu->EnableMenuItem(IDC_EVENT_FILTER_DEBUG, MF_ENABLED);
				if(m_bShowDebug)
				{
					//��ʾ����
					pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_CHECKED);
				}
				else
				{
					pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_UNCHECKED);
				}
			}
			else
			{
				//������Ϣ��ͼ
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_CHECKED);
			}

			//�Ƿ���ʾ����
			pMenu->EnableMenuItem(IDC_EVENT_FILTER_RETURN, MF_ENABLED);
			if(m_bShowReturn)
			{
				//��ʾ����
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_RETURN, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_RETURN, MF_UNCHECKED);
			}
			
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

void CViewSOE::OnEventAck() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
		return;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//ˢ�½�����ʾ, ֪ͨ��������
 					RefreshAck(pEvent, nIndex);
 					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}

void CViewSOE::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	BOOL bAckAction = AckAllEvent(EX_STTP_INFO_PT_ACTION_DATA, m_nType);
	BOOL bAckAlarm = AckAllEvent(EX_STTP_INFO_PT_ALARM_DATA, m_nType);
	for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
	{
		CXJEvent* pEvent = (CXJEvent*)m_EventManager.m_EventList.GetAt(i);
		if(pEvent == NULL)
			continue;
		if(pEvent->m_nEventType == XJ_EVENT_ALARM && bAckAlarm)
		{
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
			TellOtherWndAck(pEvent);
		}
		else if(pEvent->m_nEventType == XJ_EVENT_ACTION && bAckAction)
		{
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
			TellOtherWndAck(pEvent);
		}
	}
}

void CViewSOE::OnEventCopy() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
		return;
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				str += pEvent->GetDescribe();
			}
		}
	}
	
	//����str��������
	if (OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, str.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,str);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
}

void CViewSOE::OnEventDevice() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//�õ��¼�
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

void CViewSOE::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//�����������
	RefreshData();
}

void CViewSOE::OnEventProp() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//�õ��¼�
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

void CViewSOE::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

void CViewSOE::OnEventSec() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//�õ��¼�
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

void CViewSOE::OnEventSecProp() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//�õ��¼�
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	//�ҿ���������ҳ
	if(pEvent->m_nEventType == XJ_EVENT_ACTION)
		pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ACTION);
	else if(pEvent->m_nEventType == XJ_EVENT_ALARM)
		pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ALARM);
	else
	{
		if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
			pDoc->ShowSecPropPage(pEvent->m_pSec, 1);
		else
			pDoc->ShowSecPropPage(pEvent->m_pSec);
	}
}

/*************************************************************
 �� �� ��: RefreshAck()
 ���ܸ�Ҫ: ȷ�ϳɹ���ˢ�½�����ʾ
 �� �� ֵ: 
 ��    ��: param1 ȷ�ϵ��¼�
		   Param2 �¼�λ��
**************************************************************/
void CViewSOE::RefreshAck( CXJEvent* pEvent, int nIndex /*= -1*/ )
{
	//�ж�������Ч��
	if(pEvent == NULL)
		return;
	if(nIndex == -1)
	{
		//�����¼����б��е�λ��
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pEvent;
		nIndex = m_List.FindItem(&lvFind);
	}
	//ʵ���Ҳ���,�˳�
	if(nIndex == -1)
		return;
	//����ID,��Ϊ���¼����Ȳ�֪��ID
	CString str;
	str.Format("%d", pEvent->m_lID);
	m_List.SetItemText(nIndex, 0, str);
	//�����Ƿ�ȷ����ʾ
	m_List.SetItemText(nIndex, 7, StringFromID(IDS_ACK_YES));
	//ˢ��
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CViewSOE::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEvent* pEvent1 = (CXJEvent*)lParam1;
	CXJEvent* pEvent2 = (CXJEvent*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","��Ϣ���","�����豸","����ʱ��", "��Ϣ����","��վ��","һ���豸","��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //��Ϣ���
		iResult = pEvent1->m_nEventType - pEvent2->m_nEventType;
		break;
	case 2: //��������
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //��Ϣ����
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //��վ��
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //һ���豸��
		str1 = pEvent1->m_sDeviceName;
		str2 = pEvent2->m_sDeviceName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iSOEViewAsc;
	return iResult;
}

void CViewSOE::OnColumnclickListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iSOEViewAsc = - g_iSOEViewAsc;
	}
	else
	{
		g_iSOEViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewSOE::OnClickListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected < 0)
	{
		//�������Ƿָ��,�����ڵĸ�������SOEFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ѡ���˿���,���Ϳ�ָ��
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	*pResult = 0;
}

void CViewSOE::OnItemchangedListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//��ʵ���������ѡ����, ���������ITEMCHANGED��Ϣ, 
		//1��A�����ֱ�����ʧ������2��Aȡ��ѡ�д�����3��B��ѡ�д���
		//Ϊ����ˢ��̫Ƶ��, ����ֻ����ѡ����Ϣ, ���������©��ѡ���˿��е���Ϣ
		//���԰�ѡ����з���NM_CLICK�д���, �����ƶ��������¼����µ�ѡ�񵽿��в�������
		
		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
		if(nSelected < 0)
			return;
		
		//�õ����ж�Ӧ���¼�
		CXJEvent * pEvent = (CXJEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//�������Ƿָ��,�����ڵĸ�������SOEFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd->PostMessage(VIEWSOE_SELECTCHANGED, 0, (LPARAM)pEvent);
	}

	*pResult = 0;
}

/*************************************************************
 �� �� ��: TellOtherWndAck()
 ���ܸ�Ҫ: ����������������ȷ�����¼�
 �� �� ֵ: 
 ��    ��: param1 ��ȷ�ϵ��¼�
		   Param2 �Ƿ񱻶�ȷ��
**************************************************************/
void CViewSOE::TellOtherWndAck( CXJEvent* pEvent, BOOL bIsPassive /*= FALSE*/ )
{
	//�������Ƿָ��,�����ڵĸ�������SOEFrame.
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
		pWnd ->PostMessage(VIEW_SOE_LIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
}

void CViewSOE::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	CXJEventAction* pParam = (CXJEventAction*)lParam;
	if(pParam == NULL)
		return;
	
	//����ʾ������Ϣ�����
	if(m_nType != 3)
	{
		//�ǵ�����Ϣ��ͼ
		if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
		{
			WriteLog("CViewSOE::OnSTTP20022, �����¼�����ʾ", XJ_LOG_LV3);
			return;
		}
	}

	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewSOE::OnSTTP20022, �����¼�����ʾ", XJ_LOG_LV3);
		return;
	}
	
	//����
	CXJEventAction* pEvent = (CXJEventAction*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�
	AddEventToList(pEvent, 0); //���뵽��һ��
	CheckLineCount();
	//�����¼����Կ�
	if(g_ShowEventDlg == 1)
	{
		CXJBrowserApp* pApp  = (CXJBrowserApp*)AfxGetApp();
		m_EventManager.m_pCurrentEvent = pEvent;
		pApp->ShowEventProp(&m_EventManager);
	}

}

/*************************************************************
 �� �� ��: CheckLineCount()
 ���ܸ�Ҫ: ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSOE::CheckLineCount()
{
	int nCount = m_List.GetItemCount();
	int nDel = nCount - m_nMaxRow;
	if(nDel <= 0)
	{
		//�б�����ʾ��û���������
		return;
	}
	//ɾ�����һ��,ֱ����������
	for(int i = 0; i < nDel; i++)
	{
		int nNewCount = m_List.GetItemCount();
		//ȡ�����һ��
		CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nNewCount - 1);
		//���б���ɾ��
		m_List.DeleteItem(nNewCount - 1);
		//���¼�������ڴ���ɾ��
		m_EventManager.RemoveAtEvent(pEvent);
	}
}

void CViewSOE::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	if(pParam == NULL)
		return;

	//����ʾ������Ϣ�����
	if(m_nType != 3)
	{
		//�ǵ�����Ϣ��ͼ
		if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
		{
			WriteLog("CViewSOE::OnSTTP20018, �����¼�����ʾ", XJ_LOG_LV3);
			return;
		}
	}

	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewSOE::OnSTTP20018, �����¼�����ʾ", XJ_LOG_LV3);
		return;
	}

	//����
	CXJEventAlarm* pEvent = (CXJEventAlarm*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	//�����¼����Կ�
	if(g_ShowEventDlg == 1)
	{
		CXJBrowserApp* pApp  = (CXJBrowserApp*)AfxGetApp();
		m_EventManager.m_pCurrentEvent = pEvent;
		pApp->ShowEventProp(&m_EventManager);
	}	
}

void CViewSOE::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//���¶�������
	RefreshData();
}

/*************************************************************
 �� �� ��: LoadDIFromDB()
 ���ܸ�Ҫ: �����ݿ��ȡ����������
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ���ҵĸ���
		   Param2 �Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
**************************************************************/
BOOL CViewSOE::LoadDIFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//������ƽ̨����DLL�����ӿ���
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//Ҫ��ѯ��ȷ���¼�, �����������
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//����ѯ��ȷ���¼�, �����ֻ��ѯδȷ���¼�������
		sql.Conditionlist.push_back(condition1);
	}

	//����������,�鼸���¼�
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	//������Ϣ��ͼ
	str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
		
	//��ID��С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by TIME DESC,MS DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CHANGE_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSOE::LoadDIFromDB, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadDIFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventDI * pEvent = new CXJEventDI;
			if(pEvent->LoadFromDB(pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewSOE::LoadDIFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 �� �� ��: DisplayValue()
 ���ܸ�Ҫ: �õ���������ֵ��ʾ�ַ���
 �� �� ֵ: ��ʾֵ
 ��    ��: param1 ������ֵ
		   Param2 
**************************************************************/
CString CViewSOE::DisplayValue( int nValue )
{
	//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��, 4:����/����.
	if(0 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "1";
		else if(0 == nValue)
			return "0";
	}
	if(1 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "ON";
		else if(0 == nValue)
			return "OFF";
	}
	if(2 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_COMMISSIONING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CEASING);
	}
	if(3 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_CLOSING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_OPENING);
	}
	if(4 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_ACTION);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_RETURN);
	}
	CString str;
	str.Format("%d", nValue);
	return str;
}

void CViewSOE::OnSTTP20010( WPARAM wParam, LPARAM lParam )
{	
	if(m_nType != 3)
		return;

	CXJEventDI* pParam = (CXJEventDI*)lParam;
	if(pParam == NULL)
		return;
	//����
	CXJEventDI* pEvent = (CXJEventDI*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
}

/*************************************************************
 �� �� ��: LoadConfig()
 ���ܸ�Ҫ: ��ȡ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSOE::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	CString strNodeName = "SOEIViewConfig";
	if(m_nType == 1)
		strNodeName = "SOEIIViewConfig";
	else if(m_nType == 2)
		strNodeName = "SOEIIIViewConfig";
	else if(m_nType == 3)
		strNodeName = "SOEDebugViewConfig";
	
	//�����ʾ����
	m_nMaxRow = GetPrivateProfileInt(strNodeName, "MaxRow", 100, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	m_bShowAcked = GetPrivateProfileInt(strNodeName, "ShowAcked", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowDebug = GetPrivateProfileInt(strNodeName, "ShowDebug", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowReturn = GetPrivateProfileInt(strNodeName, "ShowReturn", 1, strIniFile);

	if(m_nType == 3)
	{
		//������Ϣ��ͼ
		m_bShowDebug = TRUE;
	}
}

/*************************************************************
 �� �� ��: SaveConfig()
 ���ܸ�Ҫ: ��������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSOE::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString strNodeName = "SOEIViewConfig";
	if(m_nType == 1)
		strNodeName = "SOEIIViewConfig";
	else if(m_nType == 2)
		strNodeName = "SOEIIIViewConfig";
	else if(m_nType == 3)
		strNodeName = "SOEDebugViewConfig";
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString(strNodeName, "MaxRow", str, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString(strNodeName, "ShowAcked", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString(strNodeName, "ShowDebug", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowReturn);
	WritePrivateProfileString(strNodeName, "ShowReturn", str, strIniFile);
}

void CViewSOE::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	//�����������
	RefreshData();
}

void CViewSOE::OnDblclkListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEvent * pEvent = (CXJEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
	*pResult = 0;
}

void CViewSOE::OnEventFilterReturn() 
{
	// TODO: Add your command handler code here
	m_bShowReturn = !m_bShowReturn;
	//�����������
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   RefreshData	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CViewSOE::RefreshData()
{
	//���¶�������
	//�������Ƿָ��,�����ڵĸ�������SOEFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ѡ���˿���,���Ϳ�ָ��
		pWnd ->PostMessage(VIEWSOE_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	else
		return 0;
	LoadDataFromDB(m_nMaxRow, m_bShowAcked);
	FillListData();
	WriteLog("SOE��ͼˢ�����", XJ_LOG_LV3);
	return 1;
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewSOE::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	BOOL bNeedReload = FALSE;
	CStationObj* pStation = (CStationObj*)wParam;
	CXJEvent * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = pList->GetAt(i);
			if(pEvent == NULL)
				continue;
			if(pEvent->m_sStationId == pStation->m_sID)
			{
				bNeedReload = TRUE;
				break;
			}
		}
	}
	if(bNeedReload)
	{
		RefreshData();
		WriteLog("StationInit,SOE�¼�ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
		WriteLog("StationInit,SOE�¼�ҳ��ˢ�����", XJ_LOG_LV3);
}
