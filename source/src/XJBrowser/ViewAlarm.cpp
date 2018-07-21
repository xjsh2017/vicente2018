// ViewAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewAlarm.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iAlarmViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewAlarm

IMPLEMENT_DYNCREATE(CViewAlarm, CFormView)

//##ModelId=49B87B86033E
CViewAlarm::CViewAlarm()
	: CFormView(CViewAlarm::IDD)
{
	//{{AFX_DATA_INIT(CViewAction)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	m_bCBoxShowReturn = TRUE;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_ALARM;
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_nOldSortCol = -1;
	m_arrColum.RemoveAll();
	g_iAlarmViewAsc = -1; //Ĭ�Ͻ�������
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	m_sChooseStationID = "";
}

//##ModelId=49B87B8700AB
CViewAlarm::~CViewAlarm()
{
	m_EventManager.ClearAllEvents();
	WriteLog("CViewAlarm::~CViewAlarm");
}

//##ModelId=49B87B87006D
void CViewAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAlarm)
	DDX_Control(pDX, IDC_LIST_ALARM, m_List);
	

	DDX_Control(pDX, IDC_CHECK_SHOWRETURN, m_cboxShowReturn);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cboxShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cboxShowDebug);
	DDX_Control(pDX, IDC_STATIC_VACHSTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_COMBO_VACHSTATION, m_cmbChooseStation);

	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	DDX_Check(pDX, IDC_CHECK_SHOWRETURN, m_bCBoxShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewAlarm, CFormView)
	//{{AFX_MSG_MAP(CViewAlarm)
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALARM, OnClickListAlarm)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALARM, OnItemchangedListAlarm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ALARM, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ALARM, OnColumnclickListAlarm)
	ON_COMMAND(IDC_EVENT_SIGN, OnEventSign)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALARM, OnDblclkListAlarm)
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_QUERYEVENT, OnEventQueryevent)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_FILTER_RETURN, OnEventFilterReturn)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	ON_BN_CLICKED(IDC_CHECK_SHOWRETURN, OnCheckShowreturn)
	ON_CBN_SELCHANGE(IDC_COMBO_VACHSTATION, OnSelchangeComboVachstation)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(REFRESH_ALARMVIEW, OnRefreshAlarmView)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAlarm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8700BB
void CViewAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B8700CB
void CViewAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewAlarm message handlers

//##ModelId=49B87B87007D
void CViewAlarm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
 	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->SetAppTile(StringFromID(IDS_SECPROP_ALARM));
	//GetParentFrame()->SetWindowText(" �澯 ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B870023
void CViewAlarm::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::OnPrepareDC(pDC, pInfo);

//	RegulateControlSize();
}

//##ModelId=49B87B87003E
void CViewAlarm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//��ȡ����
	LoadConfig();
	InitListStyle();
	//��ʼ���б�ؼ�
	InitComBox();
	
	InitListCtrl();
}

//##ModelId=49B87B87004E
BOOL CViewAlarm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�ؼ�, ��ʼ����, �������
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
//##ModelId=49B87B86037A
int CViewAlarm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	
	RefreshData();
	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B86038A
int CViewAlarm::InitListStyle()
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
		//char* arColumn[8]={"���","���","�����豸","����ʱ��", "��Ϣ����","��վ��","һ���豸","��ȷ��"};
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
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
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 4:
				lvCol.cx = 200; //��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 5:
				lvCol.cx = 140; //��վ��
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 6:
				lvCol.cx = 160; //һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 7:
				lvCol.cx = 80; //��ȷ��
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
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8603A9
BOOL CViewAlarm::LoadListConfig()
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
			WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewAlarmConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewAlarmConfig");
				WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewAlarmConfig�µ�����");
					WriteLog("ViewAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

//##ModelId=49B87B8603B9
BOOL CViewAlarm::SaveListConfig()
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
			WriteLog("ViewAlarm::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewAlarm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewAlarmConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewAlarmConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewAlarmConfig");
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
						WriteLog("δ�ҵ�ViewAlarmConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewAlarmConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B860399
int CViewAlarm::FillListData()
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
	CXJEventAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAlarm*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	m_cboxShowReady.SetCheck(m_bCBoxShowReady);
	m_cboxShowDebug.SetCheck(m_bCBoxShowDebug);
	m_cboxShowReturn.SetCheck(m_bCBoxShowReturn);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8603C8
BOOL CViewAlarm::NeedSave()
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
 ���ܸ�Ҫ�������ؼ���С,λ��.�ڴ���λ���б䶯ʱ����
 �� �� ֵ: ����ֵ˵��
 ��    ����
**************************************************************/
//##ModelId=49B87B8603C9
void CViewAlarm::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	if ( g_role == MODE_SUB )             //��������ӽ�ɫ 0:��վ 1:��վ)
	{
		//���ð�ť����
		m_staticChooseStation.ShowWindow(FALSE);
		m_cmbChooseStation.ShowWindow(FALSE);
		m_cboxShowDebug.ShowWindow(FALSE);
		m_cboxShowReturn.ShowWindow(FALSE);
		m_cboxShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	
	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B8700EA
BOOL CViewAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//	return CFormView::OnEraseBkgnd(pDC);	
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_ALARM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
}

//##ModelId=49B87B87009C
void CViewAlarm::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::CalcWindowRect(lpClientRect, nAdjustType);
}

//##ModelId=49B87B8700FA
void CViewAlarm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//������λ�øı��ı�ؼ�λ��
	RegulateControlSize();
}

//##ModelId=49B87B8700FD
void CViewAlarm::OnDestroy() 
{
	//��������
	SaveConfig();

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
//##ModelId=49B87B8603D8
BOOL CViewAlarm::LoadDataFromDB(int num, BOOL IsQueryAcked)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	m_EventManager.ClearAllEvents();

	//��Ϊ������Ϣ��Ҫ�Ƚ�����֪���Ƿ��¼��Ƿ񷵻���Ϣ���ȶ�ȡnum*5���¼�
	//num *= 2;
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);


	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);

	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "alarm_id", EX_STTP_DATA_TYPE_INT);

	Field Field32;
	pApp->m_DBEngine.SetField(sql, Field32, "value", EX_STTP_DATA_TYPE_INT);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "is_ack", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "curtime", EX_STTP_DATA_TYPE_TIME);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "ms", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "ms2", EX_STTP_DATA_TYPE_INT);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "recvtime", EX_STTP_DATA_TYPE_TIME);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);
	

		
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

	//����������
	CString strFiter = BuildOrSQL();
	if(strFiter != "")
	{
		Condition condition3;
		pApp->m_DBEngine.SetCondition(sql, condition3, strFiter);
	}

	//�����ʾ��������
	if(!m_bShowDebug)
	{
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}
	
	if(!m_bShowReturn)//����ʾ�����¼�
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("value=%d", 1);
		pApp->m_DBEngine.SetCondition(sql, condition5, strFilter);
	}
	//ѡ����ʾ��վ����Ϣ
	if ( !m_sChooseStationID.IsEmpty())
	{
		Condition condition6;
		CString strFilter;
		if( m_sChooseStationID.Find(virtualStationID, 0) != -1 )
		{
			CString sNetID = m_sChooseStationID;
			int nFind = sNetID.Find(virtualStationID, 0);
			if( nFind > 0)
			{
				sNetID = sNetID.Left(nFind);
			}
			strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where type=21");
			pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
			
			Condition condition16;
			CString sFilter1;
			sFilter1.Format("(station_id in (select station_id from tb_dev_station_base where area_id='%s'))))", sNetID);
			pApp->m_DBEngine.SetCondition(sql,condition16,sFilter1);
		}
		else
		{
			strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
			pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
		}
	}
	else if(!pApp->MgrAllStations())
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
		
	LONGLONG startTime = StartCount();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewAlarm::LoadDataFromDB, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	EndCount("��ѯ�澯�¼���ʱ��", startTime);
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewAlarm::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str);
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
				/*if(!m_bShowReturn)
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
				}*/
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
			str.Format("CViewAlarm::LoadDataFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLog(str);
		}
	}
	else
	{
		CString str;
		str.Format("CViewAlarm::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
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
//##ModelId=49B87B870000
BOOL CViewAlarm::AddEventToList(CXJEventAlarm * pEvent, int i)
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;

	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;

	//1:����
	if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		//����
		m_List.SetItemText(i, 1, StringFromID(IDS_RUNSTATUS_TESTING));
	}
	else
	{
		//����
		m_List.SetItemText(i, 1, pEvent->GetAlarmType());
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

//##ModelId=49B87B870109
void CViewAlarm::OnClickListAlarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;   
    
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��  
	if(nSelected < 0)
	{	
		//�������Ƿָ��,�����ڵĸ�������ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ,ѡ���˿���,����һ����ֵ
		pWnd ->PostMessage(VIEWALARM_SELECTCHANGED, 0, (LPARAM)NULL);
	}

	*pResult = 0;
}

//##ModelId=49B87B870119
void CViewAlarm::OnItemchangedListAlarm(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//�������Ƿָ��,�����ڵĸ�������ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWALARM_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnSTTP20018()
 ���ܸ�Ҫ����Ӧ20018(�����澯�¼�����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B87016E
void CViewAlarm::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewAlarm::OnSTTP20018 start");

	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	if(pParam == NULL)
		return;
	if( !m_sChooseStationID.IsEmpty() )
	{
		if( m_sChooseStationID.Find(virtualStationID, 0) != -1 )
		{
			CString sNetID = m_sChooseStationID;
			int nFind = sNetID.Find(virtualStationID, 0);
			if( nFind > 0)
			{
				sNetID = sNetID.Left(nFind);
			}

			if( pParam->m_pSec && pParam->m_pSec->m_pStation )
			{
				if( pParam->m_pSec->m_nSecType != 21 || pParam->m_pSec->m_pStation->m_sNetID != sNetID )
				{
					CString str;
					str.Format("�յ��Ǵ˵������װ�õı��ģ��ѹ��ˡ�",pParam->m_sStationId);
					WriteLog(str);
					return;
				}
			}
			else
				return;
		}
		else if (pParam->m_sStationId != m_sChooseStationID )
		{
			CString str;
			str.Format("�յ���վIDΪ%s���ģ����ǵ�ǰѡ���ĳ�վ���ѹ��ˡ�",pParam->m_sStationId);
			WriteLog(str);
			return;
		}
	}
	if(pParam != NULL)
	{
		CString str;
		str.Format("װ��ID:%s,����:%s,��վID:%s", pParam->m_sSecID,pParam->m_sSecName,pParam->m_sStationId);
		WriteLog(str);
	}

	//����ʾ������Ϣ�����
	if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CViewAlarm::OnSTTP20018, �����¼�����ʾ");
		return;
	}

	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewAlarm::OnSTTP20018, �����¼�����ʾ");
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

	WriteLog("CViewAlarm::OnSTTP20018 neat end");
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������δȷ���¼��ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B87011D
void CViewAlarm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

					if (g_role == MODE_SUB)
					{
						//�õ��¼�����
						CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}


					if(nSubItem == 1)
					{
						//�õ��¼�����, Ҫ��֤EventManager����б�˳������ͼlist�е�˳��һ��
						CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						
						if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
						{
							//����
							lplvcd->clrText = XJ_STATUS_RUN_CHECK;
						}
						else
						{
							CString strAlarm = pEvent->GetAlarmType();
							if(strAlarm == "I")
							{
								//I���¼�,����ɫ
								lplvcd->clrText = g_colEventI;
							}
							else if(strAlarm == "II")
							{
								//II���¼�,����ɫs
								lplvcd->clrText = g_colEventII;
							}
							else if(strAlarm == "III")
							{
								//III���¼�,����ɫ
								lplvcd->clrText = g_colEventIII;
							}
						}
					}
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

//##ModelId=49B87B87012B
void CViewAlarm::OnContextMenu(CWnd* pWnd, CPoint point) 
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

			
			
			//Menu.DeleteMenu(IDC_EVENT_COPY, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_ACK, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_ALL, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_PROP, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_Refresh, MF_BYCOMMAND);
			

			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{
				//��ѡ��
				pMenu->EnableMenuItem(IDC_EVENT_COPY, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_ACK, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SIGN, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_QUERYEVENT, MF_ENABLED);
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

//##ModelId=49B87B870139
void CViewAlarm::OnEventAck() 
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
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B87013B
void CViewAlarm::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_PT_ALARM_DATA))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B870148
void CViewAlarm::OnEventCopy() 
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
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B87014A
void CViewAlarm::OnEventDevice() 
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
	CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

//##ModelId=49B87B87014C
void CViewAlarm::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//������������
	RefreshData();
}

//##ModelId=49B87B87014E
void CViewAlarm::OnEventProp() 
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
	CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B870157
void CViewAlarm::OnEventSec() 
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
	CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B870159
void CViewAlarm::OnEventSecProp() 
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
	CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	//�ҿ������澯�¼�ҳ
	pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ALARM);
}

/*************************************************************
 �� �� ����RefreshAck()
 ���ܸ�Ҫ��ȷ�ϳɹ���ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	ȷ�ϵ��¼�	
		   Param2	�¼�λ��
**************************************************************/
//##ModelId=49B87B87000F
void CViewAlarm::RefreshAck( CXJEventAlarm* pEvent, int nIndex )
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
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B86036C
int CALLBACK CViewAlarm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventAlarm* pEvent1 = (CXJEventAlarm*)lParam1;
	CXJEventAlarm* pEvent2 = (CXJEventAlarm*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","���","�����豸","����ʱ��", "��Ϣ����","��վ��","һ���豸","��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //���
		if(pEvent1->m_IsDebug == XJ_EVENTINFO_DEBUG)
			str1 = StringFromID(IDS_RUNSTATUS_TESTING);
		else
			str1 = pEvent1->GetAlarmType();
		if(pEvent2->m_IsDebug == XJ_EVENTINFO_DEBUG)
			str2 = StringFromID(IDS_RUNSTATUS_TESTING);
		else
			str2 = pEvent2->GetAlarmType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
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
	iResult *= g_iAlarmViewAsc;
	return iResult;
}

//##ModelId=49B87B87015B
void CViewAlarm::OnColumnclickListAlarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iAlarmViewAsc = - g_iAlarmViewAsc;
	}
	else
	{
		g_iAlarmViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ, ����ˢ����ʾ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	��ȷ���¼�ָ��
**************************************************************/
//##ModelId=49B87B870178
void CViewAlarm::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

/*************************************************************
 �� �� ����TellOtherWndAck()
 ���ܸ�Ҫ����ȷ�ϳɹ���, ֪ͨ�������ڸı������ʾ
 �� �� ֵ: void
 ��    ����param1	��ȷ�ϵ��¼�
		   param2	�Ƿ񱻶�ȷ��
**************************************************************/
//##ModelId=49B87B870012
void CViewAlarm::TellOtherWndAck( CXJEventAlarm* pEvent, BOOL bIsPassive)
{
	//������Ϣ֪ͨ�¼����ԶԻ���
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		if(bIsPassive)
		{
			//ȷ�϶���ʱ�ɱ��������¼��б���ʱ, �¼���������ܷ�����ȷ����Ϣ
			//����Ϣ�������ת�����¼����ԶԻ���
			pFrame->PostMessage(WM_EVENTLIST_PASSIVE_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
		else
		{
			//�¼�����������ȷ���¼�����ʱ, ������ܷ�ȷ���¼���Ϣ
			//����Ϣ�������ת�����¼����ԶԻ���ͱ�������ҳ��
			pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
	}
	//�������Ƿָ��,�����ڵĸ�������AlarmFrame.
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
		pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
}

/*************************************************************
 �� �� ����OnEventPTAck()
 ���ܸ�Ҫ����Ӧ��������ҳ��ȷ���¼���Ϣ, ���±��ؽ�����ʾ
 �� �� ֵ: void
 ��    ����param1	�¼�����	
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B87017B
void CViewAlarm::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		//������Ϣ,��Ϊ��֪���¼����ԶԻ�����ʾ����ʲô,Ҳ��һ��
		TellOtherWndAck(pLocal, TRUE);
	}
}

/*************************************************************
 �� �� ����FindLocalPoint()
 ���ܸ�Ҫ������ָ���¼��ڱ��صĵ�ַ, ��Ϊ��������ҳ����¼����¼������¼��ǲ�ͬ�ĵ�ַ
 �� �� ֵ: ��ѯ���ı��ص�ַ,δ�ҵ�����NULL
 ��    ����param1	Ҫ���ҵ��¼�
**************************************************************/
//##ModelId=49B87B87001F
CXJEventAlarm* CViewAlarm::FindLocalPoint( CXJEventAlarm* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventAlarm* pReturn = NULL;
	//��Ϊ������ID��֪�������,ֻ�ܱȽ϶����豸ID, ����ʱ�����Ϣ
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventAlarm* pTemp = (CXJEventAlarm*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//�����豸��ͬ,�ȽϷ���ʱ��,ʱ�����ַ�����
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//����ʱ����ͬ,�ȽϷ���ʱ�����ֵ
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//��ͬ,��Ϊ��ͬһ�¼�
					pTemp->m_lID = pEvent->m_lID;
					pTemp->m_nIsAck = pEvent->m_nIsAck;
					pReturn = pTemp;
					break;
				}
				else
				{
					//����ʱ�����ֵ��ͬ,��һ��
					continue;
				}
			}
			else
			{
				//����ʱ�䲻ͬ,��һ��
				continue;
			}
		}
		else
		{
			//�����豸��ͬ, ��һ��
			continue;
		}
	}
	return pReturn;
}

/*************************************************************
 �� �� ����OnEventPropAckPT()
 ���ܸ�Ҫ����Ӧ�ɱ�������ҳ��򿪵��¼��Ի������¼�ȷ�ϵ���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B870188
void CViewAlarm::OnEventPropAckPT( WPARAM wParma, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParma;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		//������Ϣ,ֻ�����¼���ϸ������ͼ
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd != NULL)
			pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
	}
}

//##ModelId=49B87B870168
void CViewAlarm::OnEventSign() 
{
	// TODO: Add your command handler code here
	CDlgEventSignal dlg;
	dlg.m_colHide = 6;
	//ѭ����ѡ�¼�,�����ź���
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
			if (pEvent->m_Signals.GetSize() == 0)
			{
				AddSignToEvent(pEvent);
			}
			MYASSERT_CONTINUE(pEvent);
			for(int j = 0; j < pEvent->m_Signals.GetSize(); j++)
			{
				PT_Signal* sign = (PT_Signal*)pEvent->m_Signals.GetAt(j);
				dlg.AddSignal(sign);
			}
		}
	}

	//��ʾ�Ի���
	dlg.DoModal();
}

/*************************************************************
 �� �� ����CheckLineCount()
 ���ܸ�Ҫ������б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B870021
void CViewAlarm::CheckLineCount()
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

//##ModelId=49B87B87016A
void CViewAlarm::OnDblclkListAlarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;   
    
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nSelected);
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
 �� �� ����BuildOrSQL()
 ���ܸ�Ҫ���齨I,II,II����Ϣ�Ĺ��˲�ѯ���
 �� �� ֵ: ��ѯ���
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B870022
CString CViewAlarm::BuildOrSQL()
{
	CString sReturn = "";
	if(g_ShowEventI == 1)
	{
		//I����Ϣ��Ҫ��ʾ
		sReturn += "(";
		sReturn += "property=0";
	}
	if(g_ShowEventII == 1)
	{
		//II����Ϣ��Ҫ��ʾ
		if(sReturn == "")
		{
			sReturn += "(";
			sReturn += "property=1";
		}
		else
		{
			sReturn += " or ";
			sReturn += "property=1";
		}
	}
	if(g_ShowEventIII)
	{
		//III����Ϣ��Ҫ��ʾ
		if(sReturn == "")
		{
			sReturn += "(";
			sReturn += "property=2";
		}
		else
		{
			sReturn += " or ";
			sReturn += "property=2";
		}
	}
	
	if(sReturn != "")
		sReturn += ")";
	return sReturn;
}

/*************************************************************
 �� �� ����OnRefreshAlarmView()
 ���ܸ�Ҫ����Ӧǿ��ˢ�¸澯�¼���ʾ��Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B87018B
void CViewAlarm::OnRefreshAlarmView( WPARAM wParam, LPARAM lParam )
{
	//�������Ƿָ��,�����ڵĸ�������ActionFrame
	RefreshData();
	
}

void CViewAlarm::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	//������������
	RefreshData();
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
CViewAlarm::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//������������
	RefreshData();
	WriteLog("CViewAlarm::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

void CViewAlarm::OnEventQueryevent() 
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
	CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CSecObj* pObj = pEvent->m_pSec;
	if(pObj == NULL)
		return;
	//�齨��ѯ����
	CString sQuery = "";
	sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$1$$$1", pObj->m_sID);
	OpenXJQuery(sQuery, GetSafeHwnd());
}

/*************************************************************
 �� �� ��: LoadConfig()
 ���ܸ�Ҫ: ��ȡ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAlarm::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�����ʾ����
	m_nMaxRow = GetPrivateProfileInt("AlarmViewConfig", "MaxRow", 100, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	m_bShowAcked = GetPrivateProfileInt("AlarmViewConfig", "ShowAcked", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowDebug = GetPrivateProfileInt("AlarmViewConfig", "ShowDebug", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowReturn = GetPrivateProfileInt("AlarmViewConfig", "ShowReturn", 1, strIniFile);
	//�趨checkboxѡ��
	m_bCBoxShowReady = m_bShowAcked;
	m_bCBoxShowReturn = m_bShowReturn;
	m_bCBoxShowDebug = m_bShowDebug;
}

/*************************************************************
 �� �� ��: SaveConfig()
 ���ܸ�Ҫ: ��������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAlarm::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("AlarmViewConfig", "MaxRow", str, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("AlarmViewConfig", "ShowAcked", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("AlarmViewConfig", "ShowDebug", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowReturn);
	WritePrivateProfileString("AlarmViewConfig", "ShowReturn", str, strIniFile);
}

void CViewAlarm::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//������������
	RefreshData();
}

void CViewAlarm::OnEventFilterReturn() 
{
	// TODO: Add your command handler code here
	m_bShowReturn = !m_bShowReturn;
	m_bCBoxShowReturn = m_bShowReturn;
	//������������
	RefreshData();
}

void CViewAlarm::AddSignToEvent(CXJEventAlarm* pEvent)
{
	MYASSERT(pEvent);
	PT_Signal* pSignal = new PT_Signal;
	pSignal->strStation = pEvent->m_sStationName;
	pSignal->strPT_ID = pEvent->m_pSec->m_sID;
	pSignal->strPT_Model = pEvent->m_pSec->m_sModel;
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	pSignal->strTime = sTime;
	pSignal->strName = pEvent->m_sMsg;
	pSignal->nEventType = pEvent->m_nProperty;
	pSignal->strPTName = pEvent->m_pSec->m_sName;
	pEvent->m_Signals.Add(pSignal);
}
/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAlarm::InitComBox()
{

 	//�ó�վ�е���������б�
	m_cmbChooseStation.ResetContent();

	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);


 	POSITION pos = listDevice.GetHeadPosition();
	m_cmbChooseStation.InsertString(0," ("+StringFromID(IDS_COMMON_ALL)+") ");
 	while(pos != NULL)
 	{
 		 CStationObj*pStation = (CStationObj*)listDevice.GetNext(pos);
 		if(pStation == NULL)
 			continue;
		//���뵽������
 
		int nIndex = m_cmbChooseStation.AddString(pStation->m_sName);
		m_cmbChooseStation.SetItemData(nIndex,(DWORD)pStation);
	}
	m_cmbChooseStation.SetCurSel(0);
	m_cmbChooseStation.OnDataFilled(TRUE, 0);
}
/*************************************************************
 �� �� ��: OnSelchangeCOMBOChooseStation()
 ���ܸ�Ҫ: ��Ӧѡ��վ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAlarm::OnSelchangeComboVachstation() 
{
	// TODO: Add your control notification handler code here

	//��ȡ��ǰѡ����

	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_VACHSTATION))->GetCurSel();
	
	CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)
	{
		CString sStationID=	pStationid->m_sID;
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	
	
	//������������
	RefreshData();
}
void CViewAlarm::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowReady=!m_bCBoxShowReady;
	
	m_bShowAcked=!m_bShowAcked;
	//������������
	RefreshData();
	
}

void CViewAlarm::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	//������������
	RefreshData();
}

void CViewAlarm::OnCheckShowreturn() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowReturn=!m_bCBoxShowReturn;
	
	m_bShowReturn=!m_bShowReturn;
	//������������
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   RefreshData	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewAlarm::RefreshData()
{
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->SendMessage(VIEWALARM_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	else
		return;
	if(LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//��ȡ���ݳɹ�,����б�
		FillListData();
	}
	WriteLog("ˢ�¸澯��ͼ�������", XJ_LOG_LV3);
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewAlarm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	
	InitComBox();//�������볧վѡ��������
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAlarm*)pList->GetAt(i);
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
		WriteLog("StationInit,�澯�¼�ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,�澯�¼�ҳ������ˢ��", XJ_LOG_LV3);
	}
}
