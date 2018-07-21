// ViewOSC.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewOSC.h"
#include "MainFrm.h"
#include "GlobalFunc.h"
/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

int g_iOSCViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewOSC

IMPLEMENT_DYNCREATE(CViewOSC, CFormView)

//##ModelId=49B87B83006F
CViewOSC::CViewOSC()
	: CFormView(CViewOSC::IDD)
{
	//{{AFX_DATA_INIT(CViewOSC)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	m_bCBoxShowFault = TRUE;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_OSC;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	g_iOSCViewAsc = -1;
	m_bShowDebug = TRUE;
	m_bShowFault = TRUE;
	m_sChooseStationID = "";
}

//##ModelId=49B87B830138
CViewOSC::~CViewOSC()
{
	m_EventManager.ClearAllEvents();
}

//##ModelId=49B87B830119
void CViewOSC::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewOSC)
	DDX_Control(pDX, IDC_CHECK_FAULT, m_cboxShowFault);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATION, m_cmbChooseStation);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cboxShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cboxShowDebug);
	DDX_Control(pDX, IDC_LIST_OSC, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	DDX_Check(pDX, IDC_CHECK_FAULT, m_bCBoxShowFault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewOSC, CFormView)
	//{{AFX_MSG_MAP(CViewOSC)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_OSC, OnClickListOsc)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_OSC, OnColumnclickListOsc)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OSC, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OSC, OnDblclkListOsc)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_QUERYEVENT, OnEventQueryevent)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_FILTER_FAULT, OnEventFilterFault)
	ON_COMMAND(IDC_EVENT_OPENOSC, OnEventOpenosc)
	ON_COMMAND(IDC_EVENT_DOWNOSC, OnEventDownosc)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATION, OnSelchangeCOMBOChooseStation)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_FAULT, OnCheckShowFault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20132, OnSTTP20132)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B830148
void CViewOSC::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B830157
void CViewOSC::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewOSC message handlers

//##ModelId=49B87B830128
void CViewOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_OSC));
	//GetParentFrame()->SetWindowText(" ¼�� ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B83010A
void CViewOSC::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//��ȡ����
	LoadConfig();

	//��ʼ���ؼ�
	InitComBox();


	//��ʼ���б�
	InitListStyle();
	InitListCtrl();
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B83009E
int CViewOSC::InitListCtrl()
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
//##ModelId=49B87B8300AB
int CViewOSC::InitListStyle()
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
		//char* arColumn[10]={"���","��������", "�����豸","��վ��","��ʼʱ��","¼�������ļ�", "��բ���", "�غ�բʱ��", "���ϲ��", "��ȷ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 10; nCol++)
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
				lvCol.cx = 100; // ��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 2://
				lvCol.cx = 300; // ¼�������� 
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 3://
				lvCol.cx = 100; // ��վ��
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 4:
				lvCol.cx = 220; // ��ʼʱ��
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 240; // ¼�������ļ�
				colName = StringFromID(IDS_COMMON_FILENAME);
				break;
			case 6:
				lvCol.cx = 100; // ��բ���
				colName = StringFromID(IDS_TRIP_PHASE);
				break;
			case 7:
				lvCol.cx = 100; //�غ�բʱ��
				colName = StringFromID(IDS_FAULT_RECLOSE_TIME);
				break;
			case 8:
				lvCol.cx = 100; //���ϲ��
				colName = StringFromID(IDS_FAULT_LOCATION);
				break;
			case 9:
				lvCol.cx = 80; //��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=80;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(9, TRUE);
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
//##ModelId=49B87B8300BB
BOOL CViewOSC::LoadListConfig()
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
			WriteLog("ViewOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewOSCConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewOSCConfig", XJ_LOG_LV3);
				WriteLog("ViewOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewOSCConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B8300CB
BOOL CViewOSC::SaveListConfig()
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
			WriteLog("ViewOSC::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewOSC::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewOSCConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewOSCConfig");
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
						WriteLog("δ�ҵ�ViewOSCConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewOSCConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B8300AC
int CViewOSC::FillListData()
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
	CXJEventOSC * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventOSC*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//����checkbox��״̬

	m_cboxShowReady.SetCheck(m_bCBoxShowReady);
	m_cboxShowDebug.SetCheck(m_bCBoxShowDebug);
	m_cboxShowFault.SetCheck(m_bCBoxShowFault);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8300CC
BOOL CViewOSC::NeedSave()
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
//##ModelId=49B87B8300DA
void CViewOSC::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList;
	
	if ( g_role == MODE_SUB )             //��������ӽ�ɫ 0:��վ 1:��վ)
	{
		//���ð�ť����
		m_staticChooseStation.ShowWindow(FALSE);
		m_cmbChooseStation.ShowWindow(FALSE);
		m_cboxShowDebug.ShowWindow(FALSE);
		m_cboxShowReady.ShowWindow(FALSE);
		m_cboxShowFault.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B830167
void CViewOSC::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

//##ModelId=49B87B83016A
void CViewOSC::OnDestroy() 
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
//##ModelId=49B87B8300DB
BOOL CViewOSC::LoadDataFromDB(int num, BOOL IsQueryAcked)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	m_EventManager.ClearAllEvents();
	m_List.DeleteAllItems();
	//������ƽ̨����DLL�����ӿ���
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
	pApp->m_DBEngine.SetField(sql, Field2, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "filename", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "trigger_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "ms", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "savepath", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "Bfaultfile", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "fault_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "trigger_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "reclose_time", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "Fault_distance", EX_STTP_DATA_TYPE_STRING);

	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "Is_Ack", EX_STTP_DATA_TYPE_INT);

	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "Reverse2", EX_STTP_DATA_TYPE_STRING);

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

	//�����ʾ��������
	if(!m_bShowDebug)
	{
		Condition condition4;
		CString strFilter;
		strFilter.Format("Reverse2='%d'", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	//ѡ����ʾ��վ����Ϣ
	if (!m_sChooseStationID.IsEmpty() )
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
	else if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	//��������Ϣ�¼�
	if (m_bShowFault==FALSE)
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("fault_type<>'NULL'");
		pApp->m_DBEngine.SetCondition(sql, condition6, strFilter);

		bzero(&condition6, sizeof(Condition));
		strFilter.Format("fault_type<>'NON'");
		pApp->m_DBEngine.SetCondition(sql, condition6, strFilter);
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
	LONGLONG startTime = StartCount();
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewOSC::LoadDataFromDB, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	EndCount("��ѯ¼���¼���ʱ��",startTime);

	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewOSC::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventOSC * pEvent = new CXJEventOSC;
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
		str.Format("CViewOSC::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
//##ModelId=49B87B8300EC
BOOL CViewOSC::AddEventToList(CXJEventOSC * pEvent, int i)
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;

	//"���","��������", "¼��������","��վ��","��ʼʱ��","¼�������ļ�", "��բ���", "�غ�բʱ��", "���ϲ��", "��ȷ��"
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:��������
	if(pEvent->m_nHaveFault == 1)
		m_List.SetItemText(i, 1, pEvent->GetFaultType());
	else
		m_List.SetItemText(i, 1, StringFromID(IDS_FAULT_NOFAULT));
	//2:¼��������
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 2, str);
	//3:��վ��
	m_List.SetItemText(i, 3, pEvent->m_sStationName);
	//4:����ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:¼�������ļ�
	m_List.SetItemText(i, 5, pEvent->m_sFileName);
	//6:��բ���
	if(pEvent->m_nHaveFault == 1)
		m_List.SetItemText(i, 6, pEvent->GetTriggerType(pEvent->m_sTriggerType));
	else
		m_List.SetItemText(i, 6, "--");
	//7:�غ�բʱ��	
	if(pEvent->m_nHaveFault == 1)
	{
		if(pEvent->m_nRecloseTime == 0)
			m_List.SetItemText(i, 7, StringFromID(IDS_RECLOSE_NOTCLOSE));
		else
		{
			sTime.Format("%d", pEvent->m_nRecloseTime);
			m_List.SetItemText(i, 7, sTime);
		}
	}
	else
		m_List.SetItemText(i, 7, "--");
	//8:���ϲ��
	if(pEvent->m_nHaveFault == 1)
	{
		m_List.SetItemText(i, 8, pEvent->m_sFaultDistance);
	}
	else
		m_List.SetItemText(i, 8, "--");
	//9:��ȷ��
	m_List.SetItemText(i, 9, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

//##ModelId=49B87B830177
void CViewOSC::OnClickListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
// 	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
// 	if(nSelected > -1)
// 	{
// 		//������¼�, ȷ�ϴ��¼�
// 		//�õ����ж�Ӧ���¼�
// 		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
// 		if(pEvent == NULL)
// 			return;
// 		if(pEvent->Ack())
// 		{
// 			//ˢ�½�����ʾ, ֪ͨ�������ڸ���
// 			RefreshAck(pEvent, nSelected);
// 			TellOtherWndAck(pEvent);
// 		}
// 	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����RefreshAck()
 ���ܸ�Ҫ��ȷ���¼������ˢ����ʾ
 �� �� ֵ: void
 ��    ����param1	ȷ�ϵ��¼�
		   Param2	�¼����б��е�λ��
**************************************************************/
//##ModelId=49B87B8300FA
void CViewOSC::RefreshAck( CXJEventOSC* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, 9, StringFromID(IDS_ACK_YES));
	//ˢ��
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

//##ModelId=49B87B83017B
void CViewOSC::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				pMenu->EnableMenuItem(IDC_EVENT_DOWNOSC, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//ֻѡ����һ��
			//	pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_QUERYEVENT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_OPENOSC, MF_ENABLED);
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
			//�Ƿ���ʾ�޹�����Ϣ
		    pMenu->EnableMenuItem(IDC_EVENT_FILTER_FAULT, MF_ENABLED);
			if (m_bShowFault)
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_FAULT,MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_FAULT,MF_UNCHECKED);
			}
			

			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B830188
void CViewOSC::OnEventAck() 
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
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B83018A
void CViewOSC::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_OSC_LIST))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B83018C
void CViewOSC::OnEventCopy() 
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
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B830197
void CViewOSC::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//�����������
	RefreshData();
}

//##ModelId=49B87B830199
void CViewOSC::OnEventProp() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B83019B
void CViewOSC::OnEventSec() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B8301A5
void CViewOSC::OnEventSecProp() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸����ҳ
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	nIndex = PT_PROP_OSCLIST;
	if(pEvent->m_pSec != NULL)
	{
		if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
			nIndex = WR_PROP_OSC;
	}
	pDoc->ShowSecPropPage(pEvent->m_pSec, nIndex);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ����param1	�Ƚ϶���1
		   Param2	�Ƚ϶���2
		   param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B83008F
int CALLBACK CViewOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventOSC* pEvent1 = (CXJEventOSC*)lParam1;
	CXJEventOSC* pEvent2 = (CXJEventOSC*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int nPos1 = -1;
	int nPos2 = -1;
	int iResult = 0;
	//"���","��������", "¼��������","��վ��","��ʼʱ��","¼�������ļ�", "��բ���", "�غ�բʱ��", "���ϲ��", "��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //��������
		if(pEvent1->m_nHaveFault == 1)
			str1 = pEvent1->GetFaultType();
		else
			str1 = StringFromID(IDS_FAULT_NOFAULT);
		if(pEvent2->m_nHaveFault == 1)
			str2 = pEvent2->GetFaultType();
		else
			str2 = StringFromID(IDS_FAULT_NOFAULT);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //¼��������
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //��վ��
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //��ʼʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //¼�������ļ�
		str1 = pEvent1->m_sFileName;
		str2 = pEvent2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //��բ���
		if(pEvent1->m_nHaveFault)
			str1 = pEvent1->GetTriggerType(pEvent1->m_sTriggerType);
		else 
			str1 = "--";
		if(pEvent2->m_nHaveFault)
			str2 = pEvent2->GetTriggerType(pEvent2->m_sTriggerType);
		else
			str2 = "--";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //�غ�բʱ��
		if(pEvent1->m_nHaveFault == 0)
			pEvent1 ->m_nRecloseTime = 0;
		if(pEvent2->m_nHaveFault == 0)
			pEvent2->m_nRecloseTime = 0;
		iResult = pEvent1->m_nRecloseTime - pEvent2->m_nRecloseTime;
	case 8: //���ϲ��
		if(pEvent1->m_nHaveFault == 0)
			str1 = "--";
		else
			str1 = pEvent1->m_sFaultDistance;
		if(pEvent2->m_nHaveFault == 0)
			str2 = "--";
		else
			str2 = pEvent2->m_sFaultDistance;
		nPos1 = str1.Find('+', 0);
		nPos2 = str2.Find('+', 0);
		if(nPos1 == -1 && nPos2 == -1)
		{
			//����ʵ��
			float dis1 = atof(str1);
			float dis2 = atof(str2);
			iResult = dis1 - dis2;
		}
		else if(nPos1 == -1 && nPos2 > -1)
		{	
			//1Ϊʵ��,2Ϊ����,��Ϊʵ���ȸ���С
			iResult = -1;
		}
		else if(nPos1 > -1 && nPos2 == -1)
		{
			//1Ϊ����,2Ϊʵ��.��Ϊʵ���ȸ���С
			iResult = 1;
		}
		else if(nPos1 > -1 && nPos2 > -1)
		{
			iResult = CompareComplex(str1, str2);
		}
		break;
	case 9: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iOSCViewAsc;
	return iResult;
}

/*************************************************************
 �� �� ����OnColumnclickListOsc()
 ���ܸ�Ҫ����Ӧ�����ͷ�¼�,�Ա�����н�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8301A7
void CViewOSC::OnColumnclickListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iOSCViewAsc = - g_iOSCViewAsc;
	}
	else
	{
		g_iOSCViewAsc = 1;
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
//##ModelId=49B87B8301BC
void CViewOSC::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_OSC)
		return;
	CXJEventOSC* pEvent = (CXJEventOSC*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

/*************************************************************
 �� �� ����TellOtherWndAck()
 ���ܸ�Ҫ���¼�ȷ�ϳɹ���, ֪ͨ��������ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	��ȷ�ϵ��¼�
**************************************************************/
//##ModelId=49B87B8300FD
void CViewOSC::TellOtherWndAck( CXJEventOSC* pEvent )
{
	//������Ϣ֪ͨ�¼����ԶԻ���
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}

/*************************************************************
 �� �� ����OnSTTP20132()
 ���ܸ�Ҫ����Ӧ20132, ¼���ļ��Զ�����֪ͨ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8301C6
void CViewOSC::OnSTTP20132( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewOSC::OnSTTP20132 start", XJ_LOG_LV3);

	CXJEventOSC * pEvent = (CXJEventOSC *)lParam;
	if(!pEvent)
		return;
	
	if (pEvent->m_sStationId != m_sChooseStationID&& !m_sChooseStationID.IsEmpty())
	{
		CString str;
		str.Format("�յ���վIDΪ%s���ģ����ǵ�ǰѡ���ĳ�վ���ѹ��ˡ�",pEvent->m_sStationId);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//����ʾ������Ϣ�����
	if(!m_bShowDebug  && pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CViewDI::OnSTTP20132, �����¼�����ʾ", XJ_LOG_LV3);
		return;
	}
	//����ʾ�޹�����Ϣ�����
	if(!m_bShowFault  && pEvent->m_nHaveFault == 1)
	{
		WriteLog("CViewDI::OnSTTP20132, �޹����¼�����ʾ", XJ_LOG_LV3);
		return;
	}

	CXJEventOSC* pNew = (CXJEventOSC*)pEvent->Clone();
	if(!pNew)
		return;
	//���뵽����
	m_EventManager.AddEvent(pNew);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pNew, 0);
	CheckLineCount();
	
	WriteLog("CViewOSC::OnSTTP20132 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8301AB
void CViewOSC::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

/*************************************************************
 �� �� ����CheckLineCount()
 ���ܸ�Ҫ������б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B830109
void CViewOSC::CheckLineCount()
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

//##ModelId=49B87B8301B5
void CViewOSC::OnDblclkListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

//##ModelId=49B87B8301B9
BOOL CViewOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_OSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

void CViewOSC::OnEVENTRefresh() 
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
void CViewOSC::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewOSC::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

void CViewOSC::OnEventQueryevent() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CSecObj* pObj = pEvent->m_pSec;
	if(pObj == NULL)
		return;
	//�齨��ѯ����
	CString sQuery = "";
	if(pObj->m_nType == TYPE_PROTECT)
		sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$2$$$1", pObj->m_sID);
	else
		sQuery.Format("0$$$ $$$99$$$%s$$$ $$$99$$$2$$$1", pObj->m_sID);
	OpenXJQuery(sQuery, GetSafeHwnd());
}

/*************************************************************
 �� �� ��: LoadConfig()
 ���ܸ�Ҫ: ��ȡ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewOSC::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�����ʾ����
	m_nMaxRow = GetPrivateProfileInt("OSCViewConfig", "MaxRow", 100, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	m_bShowAcked = GetPrivateProfileInt("OSCViewConfig", "ShowAcked", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowDebug = GetPrivateProfileInt("OSCViewConfig", "ShowDebug", 1, strIniFile);
	//�Ƿ���ʾ�޹����¼�
	m_bShowFault = GetPrivateProfileInt("OSCViewConfig", "ShowFault", 1, strIniFile);
	//�趨checkboxѡ��
	m_bCBoxShowReady = m_bShowAcked;
	m_bCBoxShowDebug = m_bShowDebug;
	m_bCBoxShowFault = m_bShowFault;
}

/*************************************************************
 �� �� ��: SaveConfig()
 ���ܸ�Ҫ: ��������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewOSC::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("OSCViewConfig", "MaxRow", str, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("OSCViewConfig", "ShowAcked", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("OSCViewConfig", "ShowDebug", str, strIniFile);
	//�Ƿ���ʾ�޹����¼�
	str.Format("%d", m_bShowFault);
	WritePrivateProfileString("OSCViewConfig", "ShowFault", str, strIniFile);
}

void CViewOSC::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//�����������
	RefreshData();
}
void CViewOSC::OnEventFilterFault()
{
	// TODO: Add your command handler code here
	m_bShowFault = !m_bShowFault;
	m_bCBoxShowFault = m_bShowFault;
	//�����������
	RefreshData();

}
void CViewOSC::OnEventOpenosc() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->IsDownloaded() != 1)
	{
		if(AfxMessageBox(StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_OKCANCEL)==IDOK)
		{
			//���ӵ�����ҳ��
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->ShowDownOsc(pEvent);
		}
		return;
	}
	CString strFilePath = pEvent->GetFilePath();
	//���ļ�
	OpenWaveFile(strFilePath, GetSafeHwnd());
}

void CViewOSC::OnEventDownosc() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
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
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				//���ӵ�����ҳ��
				pApp->ShowDownOsc(pEvent);
			}
		}
	}
}
/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewOSC::InitComBox()
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
	m_cmbChooseStation.InsertString(0," "+StringFromID(IDS_COMMON_ALL)+" ");
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
void CViewOSC::OnSelchangeCOMBOChooseStation() 
{
	// TODO: Add your control notification handler code here
	//��ȡ��ǰѡ����
	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATION))->GetCurSel();
	
	CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)	
	{
		CString sStationID=	pStationid->m_sID;
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	
	//ˢ���б���Ϣ
	InitListCtrl();
}



void CViewOSC::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	
	//�����������
	RefreshData();
}




void CViewOSC::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowReady=!m_bCBoxShowReady;
	
	m_bShowAcked=!m_bShowAcked;

	//�����������
	RefreshData();
	
}

void CViewOSC::OnCheckShowFault() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowFault=!m_bCBoxShowFault;
	
	m_bShowFault=!m_bShowFault;
	
	//�����������
	RefreshData();
	
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   RefreshData	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CViewOSC::RefreshData()
{
	if(!LoadDataFromDB(m_nMaxRow,m_bShowAcked))
	{
		//��ȡ����ʧ��
		WriteLog("¼���¼���ͼ��ȡ����ʧ��", XJ_LOG_LV2);
		return 0;
	}
	else
	{
		//�ɹ�
		FillListData();
	}
	WriteLog("¼���¼���ͼˢ�����", XJ_LOG_LV3);
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
void CViewOSC::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	InitComBox();//�������볧վѡ��������
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventOSC * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventOSC*)pList->GetAt(i);
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
		WriteLog("StationInit,¼����ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
		WriteLog("StationInit,¼����ҳ������ˢ��", XJ_LOG_LV3);
}

