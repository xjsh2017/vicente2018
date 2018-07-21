/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   16:45
	filename: 	F:\vicente\Code\src\XJBrowser\ViewAction.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	ViewAction
	file ext:	cpp
	author:		LYH
	
	purpose:	
*********************************************************************/
// ViewAction.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewAction.h"
#include "XJBrowserDoc.h"
#include "DataEngine.h"
#include "DlgEventProp.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"
#include "GlobalFunc.h"

#define VIEWACTION_COL_NUM 11

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

UINT ActionViewLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CViewAction* pView = (CViewAction*)pParam;


	//��ȡ����
	if(pView->LoadDataFromDB())
	{
		pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	}
	pView->NotifyThreadEnd();
	return 0;
}

/** @brief           ����������λ*/
int g_iActionViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewAction

IMPLEMENT_DYNCREATE(CViewAction, CFormView)


CViewAction::CViewAction()
	: CFormView(CViewAction::IDD)
{
	//{{AFX_DATA_INIT(CViewAction)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	m_bCBoxShowReturn = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_nOldSortCol = -1;
	g_iActionViewAsc = -1;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	m_bQuitThread = FALSE;
	m_sChooseStationID = "" ;
	m_pLoadDataThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}


CViewAction::~CViewAction()
{
	DeleteCriticalSection(&m_CriticalSection);
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	WriteLog("CViewAction::~CViewAction ");
}

void CViewAction::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAction)
	DDX_Control(pDX, IDC_CHECK_SHOWRETURN, m_cboxShowReturn);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cboxShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cboxShowDebug);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_StateChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATION, m_cmbChooseStation);
	DDX_Control(pDX, IDC_LIST_ACTION, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	DDX_Check(pDX, IDC_CHECK_SHOWRETURN, m_bCBoxShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewAction, CFormView)
	//{{AFX_MSG_MAP(CViewAction)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACTION, OnClickListAction)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ACTION, OnItemchangedListAction)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ACTION, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ACTION, OnColumnclickListAction)
	ON_COMMAND(IDC_EVENT_SIGN, OnEventSign)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnDblclkListAction)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_QUERYEVENT, OnEventQueryevent)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_FILTER_RETURN, OnEventFilterReturn)
	ON_COMMAND(IDC_EXPORT_EXCEL, OnExportExcel)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATION, OnSelchangeCOMBOChooseStation)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	ON_BN_CLICKED(IDC_CHECK_SHOWRETURN, OnCheckShowreturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(REFRESH_ACTIONVIEW, OnRefreshActionView)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnReFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAction diagnostics

#ifdef _DEBUG

void CViewAction::AssertValid() const
{
	CFormView::AssertValid();
}


void CViewAction::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewAction message handlers

void CViewAction::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
 	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->SetAppTile(StringFromID(IDS_SECPROP_ACTION));
	//GetParentFrame()->SetWindowText(" ���� ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CViewAction::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CFormView::OnPrepareDC(pDC, pInfo);
//	RegulateControlSize();
}


void CViewAction::OnInitialUpdate() 
{
	LONGLONG startTime = StartCount();

	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//��������
	LoadConfig();
	
	//��ʼ��combox��Ϣ
	InitComBox();
	//��ʼ���б�
	InitListCtrl();
	

	EndCount("��ʼ������ҳ��ʱ��", startTime);
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�ؼ�,������ʼ���б���ʽ������б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CViewAction::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//�����б���
	InitListStyle();
	//�����ݿ��ȡ����
	RefreshData();
	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CViewAction::InitListStyle()
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
		//char* arColumn[VIEWACTION_COL_NUM]={"���","���", "��������","�����豸", "װ������ʱ��","��Ϣ����ʱ��","���ʱ��","��Ϣ����","��վ��","һ���豸","��ȷ��"};
		for (int nCol=0; nCol < VIEWACTION_COL_NUM ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //�п�50���� ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 100; //�п�60���� ���
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 100; //�п�60���� ��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 3://
				lvCol.cx = 300; //�п�160���� �����豸
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 4: 
				lvCol.cx = 180; //װ������ʱ��
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 180; //��Ϣ����ʱ��
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			case 6:
				lvCol.cx = 80;//���ʱ��
				colName = StringFromID(IDS_EVENT_RELATIVETIME);
				break;
			case 7:
				lvCol.cx = 200; //��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 8:
				lvCol.cx = 140; //��վ��
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 9:
				lvCol.cx = 160; //һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 10:
				lvCol.cx = 80; //��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��������)
		m_List.SetColumnHide(2, TRUE);
		//Ĭ������"ȷ��"
		m_List.SetColumnHide(10, TRUE);
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
BOOL CViewAction::LoadListConfig()
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
			WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewActionConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewActionConfig");
				WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewActionConfig�µ�����");
					WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() != VIEWACTION_COL_NUM)
					{
						WriteLog("ViewAction::�����������仯,����ȡUIConfig,��ʹ��Ĭ���з��");
						bResult = FALSE;
					}
					else
					{
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
	}	
	//�رմ򿪵�COM
	CoUninitialize();
	return bResult;
}

BOOL CViewAction::SaveListConfig()
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
			WriteLog("ViewAction::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewAction::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewActionConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewActionConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewActionConfig");
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
						WriteLog("δ�ҵ�ViewActionConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewActionConfig�µ�����, ��ɾ���½������ӽڵ�");
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
int CViewAction::FillListData()
{
	if(m_bQuitThread)
		return 0;
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::FillListData, ȡ��������Ȩ");
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			if(m_bQuitThread)
			{
				LeaveCriticalSection(&m_CriticalSection);
				return 0;
			}
			pEvent = (CXJEventAction*)pList->GetAt(i);

			AddEventToList(pEvent, i);
		}
	}
	m_cboxShowReady.SetCheck(m_bCBoxShowReady);
	m_cboxShowDebug.SetCheck(m_bCBoxShowDebug);
	m_cboxShowReturn.SetCheck(m_bCBoxShowReturn);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::FillListData, �ͷ�������Ȩ");
	return 0;
}

/*************************************************************
 �� �� ����NeedSave()
 ���ܸ�Ҫ���ж��Ƿ�����Ϣ�иı���Ҫ����
 �� �� ֵ: ��Ҫ���淵��TRUE, ����Ҫ���淵��FALSE
**************************************************************/
//##ModelId=49B87B8702CE
BOOL CViewAction::NeedSave()
{
	BOOL bReturn = FALSE;

	//�ȶ�ȡ��������Ϣ
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
 ���ܸ�Ҫ�������ؼ���С, �˴��б�ؼ�ռ�����ͻ�����С
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8702DE
void CViewAction::RegulateControlSize()
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
		m_StateChooseStation.ShowWindow(FALSE);
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
	
	
	//�����б���С,ռ�����߶�
	m_List.MoveWindow(&rcList, TRUE);
}


void CViewAction::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewAction::OnDestroy() 
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
 ���ܸ�Ҫ�������ݿ��ж�ȡ�����¼�,
			ÿһ����Ϣ�½�һ���¼����󱣴����¼����������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FLASE
  ��    ����param1   Ҫ���ҵĸ���
			param2	 �Ƿ��ѯ��ȷ���ֶ�, TRUEΪ��ѯ,FALSEΪ����ѯ
**************************************************************/
BOOL CViewAction::LoadDataFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_bQuitThread)
		return FALSE;

	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::LoadDataFromDB, ȡ��������Ȩ");
	m_EventManager.ClearAllEvents();

	//��Ϊ������Ϣ��Ҫ�Ƚ�����֪���Ƿ��¼��Ƿ񷵻���Ϣ���ȶ�ȡnum*5���¼�
	
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
	str.Format("%d", m_nMaxRow*3);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);

	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);

	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);

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

	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "faulttype", EX_STTP_DATA_TYPE_STRING);

	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);

	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "fdata", EX_STTP_DATA_TYPE_STRING);

	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);


	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(m_bShowAcked)
	{
		//Ҫ��ѯ��ȷ���¼�, �����������
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//����ѯ��ȷ���¼�, �����ֻ��ѯδȷ���¼�������
		sql.Conditionlist.push_back(condition1);
	}

	//����¼����͹�������
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
	//ѡ����ʾ��վ����Ϣ
	if ( !m_sChooseStationID.IsEmpty())
	{ //ѡ�����ض���վ
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
	else if(!pApp->MgrAllStations())
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}

	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	//str.Format("order by recvtime desc");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);

			
	CMemSet pMemset;
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if(m_bQuitThread)
	{
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
		return FALSE;
	}

	int nResult;
	LONGLONG startTime = StartCount();
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogTemp("CViewAction::LoadDataFromDB, ��ѯʧ��");
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
		return FALSE;
	}
	if(m_bQuitThread)
	{
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
		return FALSE;
	}
	EndCount("��ѯ�����¼���ʱ��",startTime);
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewAction::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLogTemp(str);
		}

		//����������
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			if(m_bQuitThread)
			{
				LeaveCriticalSection(&m_CriticalSection);
				WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
				return FALSE;
			}
			Sleep(g_ThreadTime);
			//�ж��ѷ��������ĸ���
			if(nChecked == m_nMaxRow)
				break;
			//�����¼�
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(&pMemset))
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
						pMemset.MoveNext();
						continue;
					}
				}
				//���뵽����
				m_EventManager.AddEvent(pEvent);
				nChecked++;
				if(m_bQuitThread)
				{
					LeaveCriticalSection(&m_CriticalSection);
					WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
					return FALSE;
				}
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset.MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewAction::LoadDataFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLogTemp(str);
		}
	}
	else
	{
		CString str;
		str.Format("CViewAction::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::LoadDataFromDB, �ͷ�������Ȩ");
	WriteLog("CViewAction::LoadDataFromDB, ��ѯ��������");
	return bReturn;
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
BOOL CViewAction::AddEventToList(CXJEventAction * pEvent, int i)
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;

	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	//1:���
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
	//2.��������
	m_List.SetItemText(i, 2, pEvent->GetFaultType());
	//3:��������
	CString str;
	m_List.SetItemText(i, 3, pEvent->m_pSec->m_sName);
	//4:װ������ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:��Ϣ����ʱ��
	sTime.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
	m_List.SetItemText(i, 5, sTime);
	//6:���ʱ��
	str.Format("%d ms", pEvent->m_nRelativeTime);
	m_List.SetItemText(i, 6, str);
	//6:��Ϣ����
	m_List.SetItemText(i, 7, pEvent->m_sMsg);
	//7:��վ��
	m_List.SetItemText(i, 8, pEvent->m_sStationName);
	//8:һ���豸��
	m_List.SetItemText(i, 9, pEvent->m_sDeviceName);
	//9:��ȷ��
	m_List.SetItemText(i, 10, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������s
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

void CViewAction::OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected < 0)
	{
		//�������Ƿָ��,�����ڵĸ�������ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ѡ���˿���,���Ϳ�ָ��
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	*pResult = 0;
}


void CViewAction::OnItemchangedListAction(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//�������Ƿָ��,�����ڵĸ�������ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnSTTP20022()
 ���ܸ�Ҫ����Ӧ20022(���������¼�����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B88005E
void CViewAction::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewAction::OnSTTP20022 start");

	CXJEventAction* pParam = (CXJEventAction*)lParam;
	if(pParam == NULL)
		return;

	if (pParam->m_sStationId != m_sChooseStationID && !m_sChooseStationID.IsEmpty())
	{
		CString str;
		str.Format("�յ���վIDΪ%s���ģ����ǵ�ǰѡ���ĳ�վ���ѹ��ˡ�",pParam->m_sStationId);
		WriteLog(str);
		return;
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
		WriteLog("CViewAction::OnSTTP20022, �����¼�����ʾ");
		return;
	}

	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewAction::OnSTTP20022, �����¼�����ʾ");
		return;
	}

	//����
	CXJEventAction* pEvent = (CXJEventAction*)pParam->Clone();
	if(pEvent == NULL)
		return;

	if(m_pLoadDataThread)
		return;//����߳����ڶ��������򲻴���˱���

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

	WriteLog("CViewAction::OnSTTP20022 neat end");
	
	
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������I,II,III���¼�����ɫ, δȷ���¼��ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CViewAction::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
		//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//���¼�����(I, II, III)��������ɫ
				if(nItem  != -1)
				{
					if (g_role == MODE_SUB)
					{
						//�õ��¼�����
						CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}


					//�ڶ��У��¼�����
 					if(nSubItem == 1)
 					{
						//�õ��¼�����, Ҫ��֤EventManager����б�˳������ͼlist�е�˳��һ��
						CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nItem);
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


void CViewAction::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				//��ʾ��ȷ���¼�
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_CHECKED);
			
			}
			else
			{
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

/*************************************************************
 �� �� ����OnEventAck()
 ���ܸ�Ҫ��ȷ����ѡ�¼�
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventAck() 
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
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//ȷ�ϳɹ�,������ʾ
					RefreshAck(pEvent, nIndex);
					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}


void CViewAction::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_PT_ACTION_DATA))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventAction* pEvent = (CXJEventAction*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

/*************************************************************
 �� �� ����OnEventCopy()
 ���ܸ�Ҫ��������ѡ�¼�����ϸ���������а�
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventCopy() 
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
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
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

/*************************************************************
 �� �� ����OnEventDevice()
 ���ܸ�Ҫ����λ���������һ���豸����
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventDevice() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

/*************************************************************
 �� �� ����OnEventHide()
 ���ܸ�Ҫ���ı��Ƿ�������ȷ���¼�, ��������¼�
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
}

/*************************************************************
 �� �� ����OnEventProp()
 ���ܸ�Ҫ���鿴�¼�����
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventProp() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

/*************************************************************
 �� �� ����OnEventSec()
 ���ܸ�Ҫ����λ��������Ķ����豸����
 �� �� ֵ: void
**************************************************************/
void CViewAction::OnEventSec() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}


void CViewAction::OnEventSecProp() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	//�򿪱��������¼�ҳ
	pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ACTION);
}

/*************************************************************
 �� �� ����RefreshAck()
 ���ܸ�Ҫ���¼�ȷ�Ϻ�,������ʾ
 �� �� ֵ: void
 ��    ����param1	ȷ�ϵ��¼�
		   Param2	�¼�λ��
**************************************************************/
void CViewAction::RefreshAck( CXJEventAction* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, VIEWACTION_COL_NUM-1, StringFromID(IDS_ACK_YES));
	//ˢ��
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
int CALLBACK CViewAction::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventAction* pEvent1 = (CXJEventAction*)lParam1;
	CXJEventAction* pEvent2 = (CXJEventAction*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","���", "��������","��������", "װ������ʱ��","��Ϣ����ʱ��","��Ϣ����","��վ��","һ���豸","��ȷ��"
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
		str1 = pEvent1->GetFaultType();
		str2 = pEvent2->GetFaultType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //��������
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //װ������ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //��Ϣ����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms2);
		str2.Format("%s.%03d", pEvent2->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms2);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6:
		iResult = pEvent1->m_nRelativeTime - pEvent2->m_nRelativeTime;
		break;
	case 7: //��Ϣ����
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8: //��վ��
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9: //һ���豸��
		str1 = pEvent1->m_sDeviceName;
		str2 = pEvent2->m_sDeviceName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 10: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iActionViewAsc;
	return iResult;
}


void CViewAction::OnColumnclickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iActionViewAsc = - g_iActionViewAsc;
	}
	else
	{
		g_iActionViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ�¼����ԶԻ���ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
void CViewAction::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

/*************************************************************
 �� �� ����TellOtherWndAck()
 ���ܸ�Ҫ����ȷ���¼��ɹ���֪ͨ�������ڸı��������ʾ
 �� �� ֵ: void
 ��    ����param1	��ȷ�ϵ��¼�
		   param2	�Ƿ񱻶�ȷ��
**************************************************************/
void CViewAction::TellOtherWndAck( CXJEventAction* pEvent, BOOL bIsPassive)
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
	//�������Ƿָ��,�����ڵĸ�������ActionFrame.
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
		pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
}

/*************************************************************
 �� �� ����OnEventPTAck()
 ���ܸ�Ҫ����Ӧ��������ҳ��ȷ���¼���Ϣ, ˢ�½�����ʾ, Ҫ���ҵ���Ӧ���¼�,��Ϊָ���ǲ�ͬ��
 �� �� ֵ: void
 ��    ����param1	 �¼�����
		   Param2	 �¼�ָ��
**************************************************************/
void CViewAction::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
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
CXJEventAction* CViewAction::FindLocalPoint( CXJEventAction* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventAction* pReturn = NULL;
	//��Ϊ������ID��֪�������,ֻ�ܱȽ϶����豸ID, ����ʱ�����Ϣ
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventAction* pTemp = (CXJEventAction*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//�����豸��ͬ,�ȽϷ���ʱ��,ʱ�����ַ�����
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//����ʱ����ͬ,�ȽϷ���ʱ�����ֵ
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//����ʱ�����ֵ��ͬ,�ȽϽ���ʱ��
					if(pTemp->m_tmTime2 == pEvent->m_tmTime2)
					{
						//����ʱ����ͬ,�ȽϽ���ʱ�����ֵ
						if(pTemp->m_nms2 = pEvent->m_nms2)
						{
							//��ͬ,��Ϊ��ͬһ�¼�
							pTemp->m_lID = pEvent->m_lID;
							pTemp->m_nIsAck = pEvent->m_nIsAck;
							pReturn = pTemp;
							break;
						}
						else
						{
							//��ͬ,��һ��
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
 ���ܸ�Ҫ����Ӧ�ɱ�������ҳ��򿪵��¼����ԶԻ������¼�ȷ�ϵ���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
void CViewAction::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		//������Ϣ,ֻ�����¼���ϸ����
		//�������Ƿָ��,�����ڵĸ�������ActionFrame.
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd != NULL)
		pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
	}
}


void CViewAction::OnEventSign() 
{
	// TODO: Add your command handler code here
	CDlgEventSignal dlg;
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
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				for(int j = 0; j < pEvent->m_Signals.GetSize(); j++)
				{
					PT_Signal* sign = (PT_Signal*)pEvent->m_Signals.GetAt(j);
					dlg.AddSignal(sign);
				}
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
void CViewAction::CheckLineCount()
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


void CViewAction::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}


BOOL CViewAction::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_ACTION);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����BuildOrSQL()
 ���ܸ�Ҫ���齨I,II,II����Ϣ�Ĺ��˲�ѯ���
 �� �� ֵ: ��ѯ���
 ��    ����param1
		   Param2
**************************************************************/
CString CViewAction::BuildOrSQL()
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
 �� �� ����OnRefreshActionView()
 ���ܸ�Ҫ����Ӧǿ��ˢ�¶����¼��б���Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CViewAction::OnRefreshActionView( WPARAM wParam, LPARAM lParam )
{
	//�������Ƿָ��,�����ڵĸ�������ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ���Ϳ�ָ��
		pWnd ->SendMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	RefreshData();
}

void CViewAction::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	//�������Ƿָ��,�����ڵĸ�������ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ���Ϳ�ָ��
		pWnd ->SendMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	RefreshData();
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAction::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
}

void CViewAction::OnEventQueryevent() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CSecObj* pObj = pEvent->m_pSec;
	if(pObj == NULL)
		return;
	//�齨��ѯ����
	CString sQuery = "";
	sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$0$$$1", pObj->m_sID);
	OpenXJQuery(sQuery, GetSafeHwnd());
}

/*************************************************************
 �� �� ��: LoadConfig()
 ���ܸ�Ҫ: ��ȡ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAction::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�����ʾ����
	m_nMaxRow = GetPrivateProfileInt("ActionViewConfig", "MaxRow", 100, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	m_bShowAcked = GetPrivateProfileInt("ActionViewConfig", "ShowAcked", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowDebug = GetPrivateProfileInt("ActionViewConfig", "ShowDebug", 1, strIniFile);
	//�Ƿ���ʾ�����¼�
	m_bShowReturn = GetPrivateProfileInt("ActionViewConfig", "ShowReturn", 1, strIniFile);
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
void CViewAction::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("ActionViewConfig", "MaxRow", str, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("ActionViewConfig", "ShowAcked", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("ActionViewConfig", "ShowDebug", str, strIniFile);
	//�Ƿ���ʾ�����¼�
	str.Format("%d", m_bShowReturn);
	WritePrivateProfileString("ActionViewConfig", "ShowReturn", str, strIniFile);
}

void CViewAction::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
}

void CViewAction::OnEventFilterReturn() 
{
	// TODO: Add your command handler code here
	
	m_bShowReturn = !m_bShowReturn;
	m_bCBoxShowReturn = m_bShowReturn;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
}

void CViewAction::OnExportExcel() 
{
	/*
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("�����¼�%s.xls", t.Format("%Y%m%d%H%M%S"));
	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//�����ɹ�
		if(AfxMessageBox("�����ļ��ɹ�, ��[ȷ��]���ļ�Ŀ¼", MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//����ʧ��
		AfxMessageBox("�������ļ�ʧ��");
	}
	*/
}

void CViewAction::RefreshData()
{
	//�������Ƿָ��,�����ڵĸ�������ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//�򸸴��ڷ���SELECTCHANGED��Ϣ, ѡ���˿���,���Ϳ�ָ��
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	//����б�
	m_List.DeleteAllItems();
	StartThead();
	WriteLog("������ͼˢ����������", XJ_LOG_LV3);
}
/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewAction::InitComBox()
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
void CViewAction::OnSelchangeCOMBOChooseStation() 
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
	RefreshData();
}

void CViewAction::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowReady=!m_bCBoxShowReady;

	m_bShowAcked=!m_bShowAcked;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
	
}

void CViewAction::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
}

void CViewAction::OnCheckShowreturn() 
{
	// TODO: Add your control notification handler code here
	//�޸�ѡ��״̬
	m_bCBoxShowReturn=!m_bCBoxShowReturn;

	m_bShowReturn=!m_bShowReturn;
	//�����������
	//�����ݿ��ȡ����
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewAction::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	
	InitComBox();//�������볧վѡ��������
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAction*)pList->GetAt(i);
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
		WriteLog("StationInit,�����¼�ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,�����¼�ҳ������ˢ��", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   StartThead	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewAction::StartThead()
{
	EndThread();
	m_bQuitThread = FALSE;
	m_pLoadDataThread = AfxBeginThread(ActionViewLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   EndThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewAction::EndThread()
{
	if(!m_pLoadDataThread)
		return;
	m_bQuitThread = TRUE;
	DWORD dw = WaitForSingleObject(m_pLoadDataThread->m_hThread, INFINITE);
	switch(dw)
	{
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		TerminateThread(m_pLoadDataThread->m_hThread,NULL);
		WriteLog("��ȡ�����¼��߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
		break;
	}
	m_pLoadDataThread = NULL;
	m_bQuitThread = FALSE;
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   NotifyThreadEnd	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewAction::NotifyThreadEnd()
{
	m_pLoadDataThread = NULL;
	m_bQuitThread = FALSE;
	WriteLog("CViewAction::NotifyThreadEnd �߳̽���", XJ_LOG_LV3);
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   OnAllClose	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewAction::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	EndThread();
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   OnReFillData	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewAction::OnReFillData( WPARAM wParam, LPARAM lParam )
{
	FillListData();
}
