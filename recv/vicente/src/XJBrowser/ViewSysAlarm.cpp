// ViewSysAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewSysAlarm.h"
#include "MainFrm.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/** @brief           ����������λ*/
int g_iSysAlarmViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm

IMPLEMENT_DYNCREATE(CViewSysAlarm, CFormView)

//##ModelId=49B87B810128
CViewSysAlarm::CViewSysAlarm()
	: CFormView(CViewSysAlarm::IDD)
{
	//{{AFX_DATA_INIT(CViewSysAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_SYSALARM;
	m_EventManager.ClearAllEvents();
	//�����ʾ����
	m_nMaxRow = 100;
	CString strIniFile = g_SysConfig;
	m_nMaxRow = GetPrivateProfileInt("SysAlarmViewConfig", "MaxRow", 100, strIniFile);
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	g_iSysAlarmViewAsc = -1;
}

//##ModelId=49B87B8101E4
CViewSysAlarm::~CViewSysAlarm()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewSysAlarm::~CViewSysAlarm \n");
}

//##ModelId=49B87B8101C5
void CViewSysAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSysAlarm)
	DDX_Control(pDX, IDC_LIST_SYSALARM, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSysAlarm, CFormView)
	//{{AFX_MSG_MAP(CViewSysAlarm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SYSALARM, OnClickListSysalarm)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SYSALARM, OnColumnclickListSysalarm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SYSALARM, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SYSALARM, OnDblclkListSysalarm)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SYSALARM, OnItemchangedListSysalarm)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20155, OnSTTP20155)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20138, OnSTTP20138)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_STTP_20176, OnSTTP20176)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8101F4
void CViewSysAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B810203
void CViewSysAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm message handlers

//##ModelId=49B87B8101D4
void CViewSysAlarm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_SYSALARM));
	//GetParentFrame()->SetWindowText("ϵͳ�澯");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B8101B7
void CViewSysAlarm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	InitListCtrl();
}

//##ModelId=49B87B810213
void CViewSysAlarm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B81014F
int CViewSysAlarm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	RefreshData();

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B810157
int CViewSysAlarm::InitListStyle()
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
		//char* arColumn[6]={"���", "�����豸", "����ʱ��", "��Ϣ����", "��վ", "��ȷ��"};
		LV_COLUMN lvCol;
		lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;
		
		//�����п�,��С����
		for (int nCol=0; nCol < 6; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 300; // �����豸
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 2://
				lvCol.cx = 180; // ����ʱ��
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3://
				lvCol.cx = 240; // ��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 4:
				lvCol.cx = 100; //��վ
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 5: 
				lvCol.cx = 100; //��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=50;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}

		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(5, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B810177
BOOL CViewSysAlarm::LoadListConfig()
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
			WriteLog("ViewSysAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewSysAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewSysAlarmConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewSysAlarmConfig", XJ_LOG_LV3);
				WriteLog("ViewSysAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewSysAlarmConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewSysAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B810178
BOOL CViewSysAlarm::SaveListConfig()
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
			WriteLog("ViewSysAlarm::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewSysAlarm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewSysAlarmConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewSysAlarmConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewSysAlarmConfig");
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
						WriteLog("δ�ҵ�ViewSysAlarmConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewSysAlarmConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
 �� �� ����FillListData()
 ���ܸ�Ҫ������б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B810167
int CViewSysAlarm::FillListData()
{
/*	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	
	for(int i = 0; i < 10; i++)
	{
		CString temp;
		temp.Format("%d", i);
		m_List.InsertItem(LVIF_TEXT, i, temp, 0, 0, 0, 0);
		for(int j = 1; j < nCount; j++)
		{
			CString str;
			str.Format("%d%d", i, j);
			m_List.SetItemText(i, j, str);
		}
	}*/

	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	CXJEventSysAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventSysAlarm*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B810186
BOOL CViewSysAlarm::NeedSave()
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
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ�λ��
 �� �� ֵ: ����ֵ˵��
**************************************************************/
//##ModelId=49B87B810187
void CViewSysAlarm::RegulateControlSize()
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

//##ModelId=49B87B810222
void CViewSysAlarm::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����LoadDataFromDB()
 ���ܸ�Ҫ�������ݿ��ж�ȡ�����¼�, �����������ʾ��������, 
			ÿһ����Ϣ�½�һ���¼����󱣴����¼����������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FLASE
**************************************************************/
//##ModelId=49B87B810196
BOOL CViewSysAlarm::LoadDataFromDB(int num, BOOL IsQueryAcked)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	m_EventManager.ClearAllEvents();
	m_List.DeleteAllItems();
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

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("( (host_name in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s'))) or (station_name in(select station_id from tb_user_related where user_id='%s')) )",pApp->m_User.m_strUSER_ID,pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
		
	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_ALARM_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSysAlarm::LoadDataFromDB, ��ѯʧ��", XJ_LOG_LV3);
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
			str.Format("CViewSysAlarm::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventSysAlarm * pEvent = new CXJEventSysAlarm;
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
		str.Format("CViewSysAlarm::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�,���Ҫ�����λ���Ѿ���������, ԭ�ȵ�����
			�����Զ�����,����������ȷλ��	
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
//##ModelId=49B87B810199
BOOL CViewSysAlarm::AddEventToList(CXJEventSysAlarm * pEvent, int i)
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;

	//"���", "�����豸", ����ʱ��","��Ϣ����", "��վ","��ȷ��"
	//0:���
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;

	//1: �����豸
	str = "";
	if(pEvent->m_pSec != NULL)
	{
		str =  pEvent->m_pSec->m_sName;
	}
	m_List.SetItemText(i, 1, str);

	//2:����ʱ��
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 2, sTime);
	//3:��Ϣ���� 
//	if (pEvent->m_nType == 99)
//	{
	m_List.SetItemText(i, 3, pEvent->m_sMsg);
//	}
//	else
//	{
//		m_List.SetItemText(i, 3, pEvent->m_sDetailMsg);
//	}

	//4:��վ
// 	if (pEvent->m_nType == 5)
// 	{
// 		m_List.SetItemText(i, 4, pEvent->m_sStationId);
// 	}
// 	else
// 	{
		m_List.SetItemText(i, 4, pEvent->m_sStationName);
	//}
	//5:��ȷ��
	m_List.SetItemText(i, 5, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}


//##ModelId=49B87B810224
void CViewSysAlarm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			}
			if(nCount == 1)
			{
				//ֻѡ����һ��
			//	pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
			//	pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
			//	pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
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
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B810234
void CViewSysAlarm::OnEventAck() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
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
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//ˢ�½�����ʾ, ֪ͨ�������ڸ���
					RefreshAck(pEvent, nIndex);
					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}

//##ModelId=49B87B810236
void CViewSysAlarm::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_SYS_ALARM_CFG))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B810243
void CViewSysAlarm::OnEventCopy() 
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
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B810245
void CViewSysAlarm::OnEventDevice() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

//##ModelId=49B87B810247
void CViewSysAlarm::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//�����������
	RefreshData();
}

//##ModelId=49B87B810251
void CViewSysAlarm::OnEventProp() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B810253
void CViewSysAlarm::OnEventSec() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B810255
void CViewSysAlarm::OnClickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
 	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
 	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
 	if(nSelected < 0)
 	{
 		//ȡ��������ѡ��
		//�������Ƿָ��,�����ڵĸ�������SysAlarmFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)NULL);
 	}
	*pResult = 0;
}

//##ModelId=49B87B810261
void CViewSysAlarm::OnEventSecProp() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pEvent->m_pSec,1);
	else
		pDoc->ShowSecPropPage(pEvent->m_pSec);
}

/*************************************************************
 �� �� ����RefreshAck()
 ���ܸ�Ҫ���¼�ȷ�Ϻ�ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	ȷ�ϵ��¼�
		   Param2	�¼�λ��
**************************************************************/
//##ModelId=49B87B8101A6
void CViewSysAlarm::RefreshAck( CXJEventSysAlarm* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, 5, StringFromID(IDS_ACK_YES));
	//ˢ��
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ����param1	�Ƚ϶���1
		   Param2	�Ƚ϶���2
		   param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B81014A
int CALLBACK CViewSysAlarm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventSysAlarm* pEvent1 = (CXJEventSysAlarm*)lParam1;
	CXJEventSysAlarm* pEvent2 = (CXJEventSysAlarm*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���", "�����豸", ����ʱ��","��Ϣ����", "��վ","��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //�����豸
		str1 = "";
		str2 = "";
		if(pEvent1->m_pSec != NULL)
			str1 = pEvent1->m_pSec->m_sName;
		if(pEvent2->m_pSec != NULL)
			str2 = pEvent2->m_pSec->m_sName;
	case 2: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //��Ϣ����
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
	case 4: //��վ��
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iSysAlarmViewAsc;
	return iResult;
}

//##ModelId=49B87B810263
void CViewSysAlarm::OnColumnclickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iSysAlarmViewAsc = - g_iSysAlarmViewAsc;
	}
	else
	{
		g_iSysAlarmViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ, ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B81027B
void CViewSysAlarm::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

/*************************************************************
 �� �� ����TellOtherWndAck()
 ���ܸ�Ҫ�����¼�ȷ�ϳɹ���, ֪ͨ�������ڸ�����ʾ
 �� �� ֵ: void
 ��    ����param1	��ȷ�ϵ��¼�
**************************************************************/
//##ModelId=49B87B8101A9
void CViewSysAlarm::TellOtherWndAck( CXJEventSysAlarm* pEvent )
{
	//������Ϣ֪ͨ�¼����ԶԻ���
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		//�¼�����������ȷ���¼�����ʱ, ������ܷ�ȷ���¼���Ϣ
		//����Ϣ�������ת�����¼����ԶԻ���ͱ�������ҳ��
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
	//�������Ƿָ��,�����ڵĸ�������ActionFrame.
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		pWnd->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}

/*************************************************************
 �� �� ����CheckLineCount()
 ���ܸ�Ҫ������б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8101B6
void CViewSysAlarm::CheckLineCount()
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

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ��δȷ���¼�����������ɫ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B810267
void CViewSysAlarm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

//##ModelId=49B87B81026B
void CViewSysAlarm::OnDblclkListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEventSysAlarm * pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnSTTP20155()
 ���ܸ�Ҫ���յ�20115,�����ٻ���Ϣ�仯�澯֪ͨ, �������¼�
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87B810282
void CViewSysAlarm::OnSTTP20155( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20155 start", XJ_LOG_LV3);
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20155 neat end", XJ_LOG_LV3);
}

//##ModelId=49B87B810274
void CViewSysAlarm::OnItemchangedListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CXJEventSysAlarm * pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//�������Ƿָ��,�����ڵĸ�������SysAlarmFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	*pResult = 0;
}

//##ModelId=49B87B810278
BOOL CViewSysAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_SYSALARM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

void CViewSysAlarm::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewSysAlarm::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewSysAlarm::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

/****************************************************
Date:2012/5/7  Author:LYH
������:   OnSTTP20138	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewSysAlarm::OnSTTP20138( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20138 start", XJ_LOG_LV3);
	
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20138 neat end", XJ_LOG_LV3);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   RefreshData	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CViewSysAlarm::RefreshData()
{
	//�������Ƿָ��,�����ڵĸ�������SysAlarmFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	else
		return 0;
	if(LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//��ȡ���ݳɹ�,����б�
		FillListData();
	}
	else
		return 0;
	WriteLog("ϵͳ�澯ҳ��ˢ�����", XJ_LOG_LV3);
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
void CViewSysAlarm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventSysAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
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
		WriteLog("StationInit,ϵͳ�澯�¼�ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,ϵͳ�澯�¼�ҳ��ˢ�����", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2013/6/7  Author:LYH
������:   OnSTTP20176	
����ֵ:   void	
���ܸ�Ҫ: ����ϵͳ�澯(20176)����
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewSysAlarm::OnSTTP20176( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20176 start", XJ_LOG_LV3);
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20176 neat end", XJ_LOG_LV3);
}
