// ViewComm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewComm.h"
#include "PrintListCtrl.h"
#include "GlobalFunc.h"
#include "DlgDetailStatus.h"
#include "MainFrm.h"

Record::Record(CStationObj* pObj)
{
	m_sName = pObj->m_sName;
	m_nCommStatus = pObj->m_nCommStatus;
	sTime.Format("%s", pObj->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
	m_sDurativeTime = pObj->m_sDurativeTime;
	m_sReason = pObj->m_sNewStatusReason;
	m_sReasonTime.Format("%s", pObj->m_tmReasonTime.Format("%Y-%m-%d %H:%M:%S"));
	m_nCommStatus1 = pObj->m_nCommStatus1;
	sTime1.Format("%s", pObj->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
	m_sDurativeTime1 = pObj->m_sDurativeTime1;
	m_sReason1 = pObj->m_sNewStatusReason1;
	m_sReasonTime1.Format("%s", pObj->m_tmReasonTime1.Format("%Y-%m-%d %H:%M:%S"));
	m_sManufacturer = pObj->m_sManufacturer;
	m_nIPFlag = pObj->m_nIPFlag;
}

CommStatusList::CommStatusList()
{
	hItem = NULL;
	pStationObj = NULL;
}

UINT CommViewLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CViewComm* pView = (CViewComm*)pParam;
	//��ȡ����
	pView->LoadDetailDataFromDB();
	//��������
	pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	pView->NotifyThreadEnd();
	return 0;
}

/** @brief           ����������λ*/
int g_iCommViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CViewComm

IMPLEMENT_DYNCREATE(CViewComm, CFormView)

//##ModelId=49B87B8503CA
CViewComm::CViewComm()
	: CFormView(CViewComm::IDD)
{
	//{{AFX_DATA_INIT(CViewComm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_EventManager.ClearAllEvents();
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_nTimer = 0;
	g_iCommViewAsc = -1;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;

	m_nQueryTimer = 0;
	m_arrCall.RemoveAll();
	m_sResult = "";
	m_List.Create(WS_VISIBLE|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS,
		CRect(10,10,200,200), this, 65, TVOXS_COLUMNHDR|TVOXS_ROWSEL|TVOXS_ITEMTIPS|TVOXS_PICKANYWHERE);
	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	m_pLoadDataThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

CViewComm::~CViewComm()
{
	DeleteCriticalSection(&m_CriticalSection);
	ClearStationPosMap();
	WriteLog("CViewComm::~CViewComm");
	if(pMenu != NULL)
	{
		pMenu->DestroyMenu();
		delete pMenu;
		pMenu = NULL;
	}
}


void CViewComm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewComm)
	DDX_Control(pDX, IDC_LIST_COMM, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewComm, CFormView)
	//{{AFX_MSG_MAP(CViewComm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_STATION_HISTORY, OnStationHistory)
	ON_COMMAND(IDC_STATION_MAP, OnStationMap)
	ON_COMMAND(IDC_STATION_TREE, OnStationTree)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_COMM, OnColumnclickListComm)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMM, OnClickListComm)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(IDC_COMM_ON, OnCommOn)
	ON_COMMAND(IDC_COMM_OFF, OnCommOff)
	ON_COMMAND(IDC_COMM_EXPORT, OnCommExport)
	ON_COMMAND(IDC_COMM_PRINT, OnCommPrint)
	ON_COMMAND_RANGE(ITEM_1, ITEM_13, OnCommandItem)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20146, OnSTTP20146)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(COMMVIEW_QUERY_END, OnEndCallStationStatus)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(TEST_STATIONSTATUS, OnTestStationStatus)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnReFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewComm diagnostics

#ifdef _DEBUG


void CViewComm::AssertValid() const
{
	CFormView::AssertValid();
}


void CViewComm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewComm message handlers


void CViewComm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONCOMMU));
	//GetParentFrame()->SetWindowText("��վͨ��");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CViewComm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	InitListCtrl();
	
	//��ʼˢ�³�վ����ʱ�䶨ʱ��
	m_nTimer = SetTimer(1, g_RefreshStationTime*60*1000, 0);
}

void CViewComm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
int CViewComm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
	{
		return -1;
	}
	InitListStyle();

	LoadAllDataFromDB();
	FillListData();
	RefreshStatusDetail(); 
	
	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CViewComm::InitListStyle()
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
		//char* arColumn[9]={"��վ��", "״̬","״̬�ı�ʱ��","״̬����ʱ��","��ͨ��״̬","��ͨ��״̬�ı�ʱ��","��ͨ��״̬����ʱ��","�Ͽ�ԭ��","ͨ������"};		
		//�����п�,��С����
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem= nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 130; // ��վ��
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 1://
				lvCol.cx = 80; // ״̬
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 2://
				lvCol.cx = 160; // ״̬�ı�ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 160; // ״̬����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4:
				lvCol.cx = 80; //��ͨ��״̬
				colName = StringFromID(IDS_STANDBYCHL_COMMU_STATUS);
				break;
			case 5:
				lvCol.cx = 160; //��ͨ��״̬�ı�ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 6:
				lvCol.cx = 160; //��ͨ��״̬����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 7:
				lvCol.cx = 150; 	//�Ͽ�ԭ��
				colName = StringFromID(IDS_DISCONNECT_REASON);
				break;
			case 8:
				lvCol.cx = 100;   //ͨ������
				colName = StringFromID(IDS_COMMU_TEST);
				break;
			default:
				lvCol.cx=120;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol, &lvCol);
			SColumn	col;
			col.bHide = FALSE;
			col.nItemWidth = lvCol.cx;
			col.sName = lvCol.pszText;
			m_arrColum.Add(col);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	return 0;
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
BOOL CViewComm::LoadListConfig()
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
			pSelected = pDoc->selectSingleNode("//ViewCommConfig");

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


BOOL CViewComm::SaveListConfig()
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
			WriteLog("ViewComm::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewComm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCommConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCommConfig");
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
						WriteLog("δ�ҵ�ViewCommConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewCommConfig�µ�����, ��ɾ���½������ӽڵ�");
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
int CViewComm::FillListData()
{
	WriteLog("CViewComm::FillListData, start");
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();

	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(SetStationInfo(pList))
		{

		}
	}

	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	WriteLog("CViewComm::FillListData, end");
	return 0;
}


/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ�λ��
 �� �� ֵ: ����ֵ˵��
**************************************************************/
void CViewComm::RegulateControlSize()
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


void CViewComm::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnPrepareDC(pDC, pInfo);
}

BOOL CViewComm::IsSelected(const CObject* pDocItem) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::IsSelected(pDocItem);
}

void CViewComm::OnDestroy() 
{
	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//����UI����
	SaveListConfig();
	
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����AddStationToList()
 ���ܸ�Ҫ�����볧վ״̬��Ϣ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ����ĳ�վָ��
**************************************************************/
BOOL CViewComm::AddStationToList( CStationObj* pStation)
{
	WriteLog("CViewComm::AddStationToList start", XJ_LOG_LV3);
	//�¼�Ϊ��, ����FALSE
    MYASSERT_BOOL(pStation);
	MYASSERT_BOOL(m_List.GetSafeHwnd());
	//ˢ�³���ʱ��
	pStation->RefreshDurativeTime();
	
	//"��վ��", "״̬","״̬�ı�ʱ��","״̬����ʱ��","״̬�ı�ԭ��","��ͨ��״̬","��ͨ��״̬�ı�ʱ��","��ͨ��״̬����ʱ��","��ͨ��״̬�ı�ԭ��","����������Ϣ"
	CommStatusList* pList = NULL;
    m_mapStationPos.Lookup(pStation->m_sName, (void*&)pList);

	MYASSERT_BOOL(pList);
	Record* pRecord = pList->recordList.GetTail();
	MYASSERT_BOOL(pRecord);
	
	AddRecordToList(pList);
	
	SetStationInfo(pStation, pList->hItem, FALSE);
	
	POSITION pos = pList->recordList.GetTailPosition();
	if(pList->hItem != NULL && pos != NULL)
		m_List.SetItemData(pList->hItem, (DWORD)pos);

	return TRUE;
}

BOOL CViewComm::InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, POSITION pos)
{
	MYASSERT_BOOL(pList);
	MYASSERT_BOOL(pRecord);
	if (bShow || pList->recordList.GetCount() == 1)
	{
		pList->hItem = m_List.InsertItem("");
		SetStationInfo(pRecord, pList->hItem, FALSE);
		m_List.SetItemData(pList->hItem, (DWORD)pos);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CViewComm::InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, int nLastRecordStatus, POSITION pos)
{
	MYASSERT(pList);
	MYASSERT(pRecord);
	if (bShow && GetStatus(pRecord) != nLastRecordStatus)
	{
		HTREEITEM hChildItem = m_List.InsertItem("", pList->hItem, TVI_LAST);
		SetStationInfo(pRecord, hChildItem, TRUE);
		m_List.SetItemData(hChildItem, (DWORD)pos);
	}
}

void CViewComm::SetStationInfo(CStationObj* pStation, HTREEITEM hItem, BOOL bChildItem)
{
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	if (!bChildItem)
	{
		
		//1:��վ����
		CString sID;
		result = m_List.SetItemText(hItem, pStation->m_sName, IDX);
	}
	
	++IDX;
	//2:״̬
	result = m_List.SetItemText(hItem, pStation->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pStation->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (pStation->m_nCommStatus == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}

	++IDX;
	//3:״̬��ʼʱ��
	sTime.Format("%s", pStation->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
	result = m_List.SetItemText(hItem, sTime, IDX);
	
	++IDX;
	//4:״̬����ʱ��
	result = m_List.SetItemText(hItem, pStation->m_sDurativeTime, IDX);
	
	
	++IDX;
	//5:��ͨ��״̬
	result = m_List.SetItemText(hItem, pStation->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pStation->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (pStation->m_nCommStatus1 == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	
	++IDX;
	//6:��ͨ��״̬��ʼʱ��
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sTime = "";
	else
		sTime.Format("%s", pStation->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
	result = m_List.SetItemText(hItem, sTime, IDX);
	
	++IDX;
	//7:��ͨ��״̬����ʱ��
	result = m_List.SetItemText(hItem, pStation->m_sDurativeTime1, IDX);
	
	++IDX;
	result = m_List.SetItemText(hItem, GetStatus(pStation) != 3 ? StringFromID(IDS_DISCONNECT_REASON) : "" , IDX);
	m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	

	if (!bChildItem)
	{
		++IDX;
		//9:�����ȷ��
		result = m_List.SetItemText(hItem, StringFromID(IDS_COMMU_TEST), IDX);
		m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	}
	//��������
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if(g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}

void CViewComm::SetStationInfo(Record* record, HTREEITEM hItem, BOOL bChildItem)
{
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	
	if (!bChildItem)
	{
		//1:��վ����
		CString sID;
		result = m_List.SetItemText(hItem, record->m_sName, IDX);
	}
	
	++IDX;
	//2:״̬
	result = m_List.SetItemText(hItem, record->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((record->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (record->m_nCommStatus == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	++IDX;
	//3:״̬��ʼʱ��
	result = m_List.SetItemText(hItem, record->sTime, IDX);
	
	++IDX;
	//4:״̬����ʱ��
	result = m_List.SetItemText(hItem, record->m_sDurativeTime, IDX);
	
	++IDX;
	//5:��ͨ��״̬
	result = m_List.SetItemText(hItem, record->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((record->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (record->m_nCommStatus1 == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	++IDX;
	//6:��ͨ��״̬��ʼʱ��
	result = m_List.SetItemText(hItem, record->sTime1, IDX);
	
	++IDX;
	//7:��ͨ��״̬����ʱ��
	result = m_List.SetItemText(hItem, record->m_sDurativeTime1, IDX);
	
	++IDX;
	//8:�Ͽ�ԭ��
	result = m_List.SetItemText(hItem, GetStatus(record) != 3 ? StringFromID(IDS_DISCONNECT_REASON) : "", IDX);
	m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	
	if (!bChildItem)
	{
		//9:�����ȷ��
		++IDX;
		result = m_List.SetItemText(hItem, StringFromID(IDS_COMMU_TEST), IDX);
		m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	}
	//��������s
	//m_List.SetTextBkColor(RGB(237, 254, 253));
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if(g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}

BOOL CViewComm::SetStationInfo(CommStatusList* pList)
{
	MYASSERT_BOOL(pList);
	POSITION pos = pList->recordList.GetTailPosition();
	BOOL bSuccess = FALSE;
	int nTemp = 0;
	Record* pRecord = NULL;
	while (!bSuccess && pos != NULL)
	{
		pRecord = pList->recordList.GetAt(pos);
		MYASSERT_CONTINUE(pRecord);
		//��һ��������״̬��������ʾ
		bSuccess = InsertStationInfo(pList, pRecord, TRUE, pos);
		nTemp = GetStatus(pRecord);
		pList->recordList.GetPrev(pos);
	}

	while (pos != NULL)
	{
		pRecord = pList->recordList.GetAt(pos);
		InsertStationInfo(pList, pRecord, (GetStatus(pRecord) == 3 ? g_ShowCommON : g_ShowCommOFF), nTemp, pos);
		GetStatus(pRecord, &nTemp);
		pList->recordList.GetPrev(pos);
	}
	if(!bSuccess)
		pList->hItem = NULL;
	return bSuccess;
}


/*************************************************************
 �� �� ����OnSTTP20144()
 ���ܸ�Ҫ����Ӧ����վͨѶ״̬�仯(20144)����
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
void CViewComm::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
//	KillTimer(1);
	WriteLog("CViewComm::OnSTTP20144 start", XJ_LOG_LV3);
	
	CStationObj* pObj = (CStationObj*)lParam;
	if(!pObj)
		return;
	//���ҳ�վ
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_pLoadDataThread)
		return;

	CommStatusList* pList = NULL;
	m_mapStationPos.Lookup(pObj->m_sName, (void*&)pList);
	//�ı�LIST�е�ֵ
	AddStationToList(pObj);

	//ˢ����ʾ
	FillListData();
    //SetTimer(1, g_RefreshStationTime*60*1000, 0);
	WriteLog("CViewComm::OnSTTP20144 neat end", XJ_LOG_LV3);
}


void CViewComm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				TRACE("CListCtrlEx::OnContextMenu -- ���һ�����ͷ\n");
				if(pMenu != NULL)
				{
					//������ղŵĲ˵�
					if (pMenu != NULL)
					{
						int nCount = pMenu->GetMenuItemCount();
						for(int i = 0; i < nCount; i++)
						{
							pMenu->DeleteMenu(0, MF_BYPOSITION);
						}
					}
					//������ͷ���ָ�
					pMenu->AppendMenu(MF_STRING | MF_DISABLED, ITEM_TITLE, StringFromID(IDS_SELECT_SHOW_COLUMN));
					pMenu->AppendMenu(MF_SEPARATOR, ITEM_SEPARATOR, "");
					//�����������
					int nColCount = m_arrColum.GetSize();
					for(int i = 0; i < nColCount; i++)
					{
						SColumn  sc = m_arrColum.GetAt(i);
						if(sc.bHide)
						{//���ص�
							pMenu->AppendMenu(MF_STRING | MF_UNCHECKED, ITEM_1 + i, sc.sName);
						}
						else
						{
							pMenu->AppendMenu(MF_STRING | MF_CHECKED, ITEM_1 + i, sc.sName);
						}
					}
					
					pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
				}
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
				pMenu->EnableMenuItem(IDC_STATION_MAP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_STATION_TREE, MF_ENABLED);
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

void CViewComm::OnStationHistory() 
{
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
		HTREEITEM hItem = m_List.GetSelectedItem();
		if(hItem == NULL)
			return;
		
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetStationList(listDevice);
		POSITION pos = NULL;
		for(pos = listDevice.GetHeadPosition(); pos != NULL;)
		{
			CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
			if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
			{
				//
				sSQL.Format("1$$$%s$$$99$$$1", pStation->m_sID);
				break;
			}		
		}
	}
	//�򿪲�ѯ��ͳ��
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

void CViewComm::OnStationMap() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	HTREEITEM hItem = m_List.GetSelectedItem();
	if(hItem == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = NULL;
	for(pos = listDevice.GetHeadPosition(); pos != NULL;)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
		{
			pApp->ShowDevice(pStation);
			break;
		}		
	}
}

void CViewComm::OnStationTree() 
{
	// TODO: Add your command handler code here
	//ֻ����ѡ����һ���¼������
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	HTREEITEM hItem = m_List.GetSelectedItem();
	if(hItem == NULL)
		return;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = NULL;
	for(pos = listDevice.GetHeadPosition(); pos != NULL;)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
		{
			pApp->LocateInTree(pStation);
			break;
		}
		
	}
}


void CViewComm::OnColumnclickListComm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iCommViewAsc = - g_iCommViewAsc;
	}
	else
	{
		g_iCommViewAsc = 1;
		m_nOldSortCol = iCol;
	}

	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����RefreshDuravieTime()
 ���ܸ�Ҫ��ˢ��LIST��ָ��λ�õ�ITEM�ĳ���ʱ����ʾ
 �� �� ֵ: void
 ��    ����param1	ָ��λ������
		   Param2
**************************************************************/
void CViewComm::RefreshDuravieTime(HTREEITEM hItem, CStationObj* pStationObj)
{
	//����
	MYASSERT(hItem);
	CommStatusList* pCommStatusList = NULL;
	if (pStationObj == NULL)
	{
		//�õ���վ
		CString strStationName = m_List.GetItemText(hItem);
		m_mapStationPos.Lookup(strStationName, (void*&)pCommStatusList);
		if (pCommStatusList != NULL)
		{
			pStationObj = pCommStatusList->pStationObj;
		}
		else
		{
			return;
		}
	}

	POSITION pos = NULL;
	Record* pRecord = NULL;
	MYASSERT(pStationObj);
	//ˢ�³���ʱ��
	pStationObj->RefreshDurativeTime();


	if(pStationObj->m_nCommStatus != -1)
	{
		//������ʾ
		m_List.SetItemText(hItem, pStationObj->m_sDurativeTime, 3);
	}
	if(pStationObj->m_nCommStatus1 != -1)
	{
		m_List.SetItemText(hItem, pStationObj->m_sDurativeTime1, 6);
	}
	
	//ˢ��
	int iIndex = m_List.GetItemIndex(hItem);
	m_List.Update(iIndex);
}


BOOL CViewComm::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormView::DestroyWindow();
}

void CViewComm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnClose();
}

void CViewComm::OnClickListComm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	HTREEITEM hItem = m_List.GetItemFromIndex(nItem);
	if(nItem != -1)
	{
		int IDX = m_List.GetColumnsCount();
		if(nSubItem == IDX - 3 && (m_List.GetItemText(hItem, IDX - 3) != ""))
		{
			POSITION pos = NULL;
			pos = (POSITION)m_List.GetItemData(hItem);
			CommStatusList* pList = NULL;
			CString strStationName = _T("");
		
			strStationName = m_List.GetItemText(hItem);

			if (strStationName == _T(""))
			{
				HTREEITEM hParentItem = m_List.GetParentItem(hItem);
				strStationName = m_List.GetItemText(hParentItem);
			}

			m_mapStationPos.Lookup(strStationName, (void*&)pList);
			MYASSERT(pList);
			MYASSERT(pos);
			DlgDetailStatus detailStatusDlg(pList, pos);
			detailStatusDlg.DoModal();
		}

		if(nSubItem == IDX - 2 && (m_List.GetItemText(hItem, IDX - 2) != ""))
		{
			//�õ����ж�Ӧ�ĳ�վ
			CommStatusList* pList = NULL;
			//�õ���վ
			CString strStationName = m_List.GetItemText(hItem);
			m_mapStationPos.Lookup(strStationName, (void*&)pList);
            MYASSERT(pList);
			MYASSERT(pList->pStationObj);
			TestStationStatus(pList->pStationObj);
		}
	}
	*pResult = 0;
}


BOOL CViewComm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_COMM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
}

void CViewComm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//ˢ�³�վ����ʱ��
		RefreshTime();
	}
	if(nIDEvent == m_nQueryTimer)
	{
		if(m_arrCall.GetSize() > 0)
		{
			StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
			if(pCall != NULL)
			{
				pCall->nRelust = -3;
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
	}
	CFormView::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����RefreshTime()
 ���ܸ�Ҫ��ˢ�����г�վ�ĳ���ʱ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CViewComm::RefreshTime()
{
	WriteLog("CViewComm::RefreshTime,��ʱˢ�¿�ʼ", XJ_LOG_LV3);
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);

	if(m_List.GetSafeHwnd() == NULL)
		return;

	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	HTREEITEM hItem = NULL;
	CString strStationName = _T("");
	
	//������վmap
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStationName, (void*&)pList);
		MYASSERT(pList);
		RefreshDuravieTime(pList->hItem, NULL);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	WriteLog("CViewComm::RefreshTime,��ʱˢ�½���", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����IsNeedShow()
 ���ܸ�Ҫ���жϵ�ǰͨ��״̬�ĳ�վ��Ϣ�Ƿ���Ҫ��ʾ
 �� �� ֵ: ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 ��    ����param1	ͨ��״̬
		   Param2
**************************************************************/
BOOL CViewComm::IsNeedShow( int nStatus )
{
	BOOL bReturn = FALSE;
	switch(nStatus)
	{
	case 0: //�Ͽ�
		if(1 == g_ShowCommOFF)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 1: //����
		if(1 == g_ShowCommON)
			bReturn = TRUE;
		else 
			bReturn = FALSE;
		break;
	default:
		bReturn = FALSE;
		break;
	}
	return bReturn;
}

void CViewComm::OnCommOn() 
{
	// TODO: Add your command handler code here
	//�ı�ͨ��״̬�����Ƿ�Ҫ��ʾ������
	if(g_ShowCommON == 0)
		g_ShowCommON = 1;
	else if(g_ShowCommON == 1)
		g_ShowCommON = 0;
	//�����������
	SetListData();
}

void CViewComm::OnCommOff() 
{
	// TODO: Add your command handler code here
	//�ı�ͨ��״̬�Ͽ��Ƿ�Ҫ��ʾ������
	if(g_ShowCommOFF == 0)
		g_ShowCommOFF = 1;
	else if(g_ShowCommOFF == 1)
		g_ShowCommOFF = 0;
	//�����������
	SetListData();
}

void CViewComm::OnCommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s.xls", StringFromID(IDS_TOOLBAR_STATIONCOMMU),t.Format("%Y%m%d%H%M%S"));
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

void CViewComm::OnCommPrint() 
{
	// TODO: Add your command handler code here
	/*CPrintListCtrl print;
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
	print.SetHeadFont("����", 14);
	print.SetFootFont("����", 12);
	print.SetListFont("����", 12);
	
	// ����ֽ������ TRUE,����FALSE ����
	print.SetPrinterOrientation(FALSE);
	print.Print(GetSafeHwnd(), "����վͨѶ״̬", FALSE);*/

	//�õ�ϵͳ����·��
	CString strDir = STATIONCOMM_LIST;
	//�齨ȫ·��
	if(g_DoubleChannel == 1)
		strDir = STATIONCOMM_LIST_DOUBLE;

	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strDir);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strDir);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInStationComm);
	if(pReportDoc != NULL && bOpened)
		pReportDoc->FillValueData();
	//��ӡ
	if(pReportDoc != NULL && bOpened)
	{
		g_hook = 1;
		pReportDoc->Print();
		g_hook = 0;
	}
	//ɾ��COM
	ReleaseCOM();
}

/*************************************************************
 �� �� ����WriteResultToExcel()
 ���ܸ�Ҫ�����б��е����ݵ�����Excel
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Excel�ļ�����λ��
		   Param2
**************************************************************/
BOOL CViewComm::WriteResultToExcel( CString strFullPath )
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
				//"���", "��վ��", "״̬","״̬�ı�ʱ��","״̬����ʱ��","״̬�ı�ԭ��","��ͨ��״̬","��ͨ��״̬�ı�ʱ��","��ͨ��״̬����ʱ��","��ͨ��״̬�ı�ԭ��","����������Ϣ"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT,  \"%s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT)",
					StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_COMMU_STATUS), StringFromID(IDS_STATUS_CHANGE_TIME),StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_STANDBYCHL_COMMU_STATUS),StringFromID(IDS_STATUS_CHANGE_TIME),StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_MODEL_MANUFACTURER));
				database.ExecuteSQL(strSQL);
			}

			POSITION pos = m_mapStationPos.GetStartPosition();
			CommStatusList* pList = NULL;
			CString strStation = _T("");
			
			while (pos != NULL)
			{
				m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
				MYASSERT_CONTINUE(pList);
				if(pList->pStationObj != NULL)
					AddResultToExcel(&database, pList->pStationObj);
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
void CViewComm::AddResultToExcel( CDatabase* pDatabase, CStationObj* pStation )
{
	//���������Ч��
	MYASSERT(pDatabase);
	MYASSERT(pStation);

	CString sReason = "";
	if(pStation->m_nCommStatus == 0)
	{
		//�Ͽ�
		sReason = pStation->m_sNewStatusReason;
	}

	//6:��ͨ��״̬
	CString sStatus = "";
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sStatus = "";
	else
		sStatus = pStation->m_nCommStatus1==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON);

	//7:��ͨ��״̬��ʼʱ��
	CString sTime = "";
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sTime = "";
	else
		sTime.Format("%s", pStation->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));

	//9:״̬�ı�ԭ��
	//ֻ��״̬Ϊ�Ͽ�ʱ��ʾֵ
	CString sReason1= "";
	if(pStation->m_nCommStatus1 == 0)
	{
		sReason1 = pStation->m_sNewStatusReason1;
	}

	//"��վ��", "״̬","״̬�ı�ʱ��","״̬����ʱ��","״̬�ı�ԭ��","��ͨ��״̬","��ͨ��״̬�ı�ʱ��","��ͨ��״̬����ʱ��","��ͨ��״̬�ı�ԭ��","����������Ϣ"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s','%s','%s','%s', '%s')",
		pStation->m_sName, (pStation->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON), pStation->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"), pStation->m_sDurativeTime, sReason, sStatus, sTime, pStation->m_sDurativeTime1, sReason1,  pStation->m_sManufacturer);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
BOOL CViewComm::InitCOM()
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
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);
	
	return TRUE;*/
	return InitReportComNoReg(pClf, pReportDoc);
}

/*************************************************************
 �� �� ����ReleaseCOM()
 ���ܸ�Ҫ��ɾ��COM���
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
void CViewComm::ReleaseCOM()
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

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
CString CViewComm::TranslateSingleKey( CString sKey )
{
	//�ж�������Ч��
	if(sKey == "")
		return "";
	if(m_List.GetSafeHwnd() == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;


	//�ж��Ƿ����[ ]�����
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//����[]�����
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//û�ҵ���������
			return "";
		}
		//ȡ��������
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		if(nIndex < 0)
			return "";
		if(nIndex >= m_List.GetItemCount())
			return "";
		//ȡ��ͷ��
		CString strHead = sKey.Left(nFind);
		if(strHead != "$DEVICE_LIST")
		{
			//��������豸�б�, ���ؿ�
			return "";
		}
		int nCount = 0;

		POSITION pos = m_mapStationPos.GetStartPosition();
		CommStatusList* pList = NULL;
		CString strStation = _T("");
		
		CStationObj* pObj = NULL;
		while (pos != NULL)
		{
			m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
			MYASSERT_CONTINUE(pList);
			if(nCount == nIndex)
			{
				pObj = pList->pStationObj;
			}
			nCount++;
		}
		
        //HTREEITEM hItem = m_List.GetItemFromIndex(nIndex);
		//CStationObj* pObj = (CStationObj*)m_List.GetItemData(hItem);
		if(pObj == NULL)
			return "";
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->m_sName; //��վ��
		}
		if(strTail == "DEVICE_COMM$")
		{
			//ͨ��״̬
			return (pObj->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON);
		}
		if(strTail == "DEVICE_COMM_TIME$")
		{
			//״̬�ı�ʱ��
			return pObj->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_COMM_DURATIVE$")
		{
			//״̬����ʱ��
			return pObj->m_sDurativeTime;
		}
		if(strTail == "DEVICE_COMM_REASON$")
		{
			//״̬�ı�ԭ��
			return pObj->m_sNewStatusReason;
		}
		if(strTail == "DEVICE_COMM_RESERVE$")
		{
			//ͨ��״̬
			return pObj->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pObj->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		}
		if(strTail == "DEVICE_COMM_TIME_RESERVE$")
		{
			CString sTime;
			//״̬�ı�ʱ��
			if(pObj->m_nCommStatus1 != 0 && pObj->m_nCommStatus1 != 1)
				return "";
			else
				sTime.Format("%s", pObj->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
			return sTime;
		}
		if(strTail == "DEVICE_COMM_DURATIVE_RESERVE$")
		{
			//״̬����ʱ��
			return pObj->m_sDurativeTime1;
		}
		if(strTail == "DEVICE_COMM_REASON_RESERVE$")
		{
			//״̬�ı�ԭ��
			return pObj->m_sNewStatusReason1;
		}
	}
	else
	{
		//�ж��Ƿ���.GetCount()����
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//��.GetCount()����
			if(sKey == "$DEVICE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_mapStationPos.GetCount());
				return sReturn;
			}
		}
		else
		{
			
		}
	}
	return "";
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewComm::OnViewRefresh( WPARAM wParma, LPARAM lParam )
{
	/*CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
	
	pData->QueryStationStatus();
	ClearStationPosMap();
	//��������б��
	FillListData();*/
	RefreshStatusDetail();
	WriteLog("CViewComm::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ��: QueryStationStatus()
 ���ܸ�Ҫ: ��ѯ��վͨѶ״̬
 �� �� ֵ: int ʧ�ܷ���<0���������ɹ�����0 
 ��    ��: param1 ָ����վ
		   Param2 ָ��ͨ��
**************************************************************/
int CViewComm::QueryStationStatus( CStationObj* pObj, int nChannel /*= 0*/ )
{
	//�����ȷ��
	MYASSERT_INT(pObj);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���뱨����������
	STTP_FULL_DATA sttpData;
	//��֯20145����
	if(!pApp->m_SttpEngine.BuildDataFor20145StationStatus(sttpData, pObj->m_sID, nChannel))
	{
		//��֯����ʧ��
		if(m_arrCall.GetSize() > 0)
		{
			StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
			if(pCall != NULL)
			{
				pCall->nRelust = -2;
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
		return -1;
	}
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		//ʧ��
		if(nReturn == -1)
		{
			CString str;
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��Ϊ�����ж�", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
			SendMessage(COMMVIEW_QUERY_END);
			return -1;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��Ϊ��������", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
			SendMessage(COMMVIEW_QUERY_END);
			return -1;
		}
	}
	else
	{
		//������ʱ��
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nQueryTimer = SetTimer(2, nTimeOut*1000, 0);
	}
	return 0;
}

/*************************************************************
 �� �� ��: OnSTTP20146()
 ���ܸ�Ҫ: ��Ӧ20146(��վͨѶ״̬����֪ͨ)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewComm::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewComm::OnSTTP20146");
	if(m_arrCall.GetSize() <= 0)
	{
		//û�����ٻ���վͨѶ״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20146����
	if(pSttpData->sttp_head.uMsgID != 20146)
	{
		//������
		CString str;
		str.Format("CViewComm::OnSTTP20146 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}

	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = 0;
	}
	else
		return;
	MYASSERT(pCall->pStation); 
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_station_id;
	if(strID != pCall->pStation->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CViewComm::OnSTTP20146 �յ�������վ����, ��վIDΪ%s", strID);
		WriteLog(str);
		return;
	}

	int nChannel = pSttpData->sttp_body.nFlag;
	if(nChannel != pCall->nChannel)
	{
		//���Ǳ�ͨ���ı���
		CString str;
		str.Format("CViewComm::OnSTTP20146 �յ�����ͨ���ı���, ͨ��Ϊ%d", nChannel);
		WriteLog(str);
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nQueryTimer);
	
	//������
	
	int nStatus = pSttpData->sttp_body.nStatus;
	int nReason = pSttpData->sttp_body.nEventType;

	/*
	CString str;
	str.Format("nChannel:%d   nStatus:%d  pCall->pStation->m_nCommStatus:%d", nChannel, nStatus, pCall->pStation->m_nCommStatus);
    WriteLog(str);
	*/

	if ((nChannel == 0 && nStatus != pCall->pStation->m_nCommStatus) || (nChannel == 1 && nStatus != pCall->pStation->m_nCommStatus1))
	{
		CString strStationTime = pSttpData->sttp_body.ch_time_12_BIT1;
		CString strReasonTime = pSttpData->sttp_body.ch_time_12_BIT2;
		CTime tmStation;
		CTime tmReason;
		if (strStationTime == "" || strReasonTime == "")
		{
			//�������
			//SendMessage(COMMVIEW_QUERY_END);
			// ���ʱ��Ϊ��,��������ͨѶ״̬���˴���
			WriteLog("CViewComm::OnSTTP20146, �յ�ʱ��Ϊ��");
			tmStation =  CTime::GetCurrentTime();
			tmReason =  CTime::GetCurrentTime();
		}
		else
		{
			tmStation = StringToTimeSttp12(strStationTime);
			tmReason = StringToTimeSttp12(strReasonTime);
		}
		pCall->pStation->SetStatus(nStatus, tmStation, nReason, nChannel, tmReason, pCall->pStation->m_nIPFlag);
		
		AddStationToList(pCall->pStation);

		//�ı�LIST�е�ֵ
		//ˢ����ʾ
		FillListData();
		//֪ͨ�豸����ͼ�ν���
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame=(CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			WriteLog("CViewComm::OnSTTP20146, ״̬��λ������֪ͨ");
			pFrame->PostMessage(STATION_CHANGED, 0, (LPARAM)pCall->pStation);
		}
	}
	else
	{
		WriteLog("CViewComm::OnSTTP20146, ״̬δ��λ��������֪ͨ");
	}
	
	//�������
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 �� �� ��: OnSTTP20069()
 ���ܸ�Ҫ: ��Ӧ20069(�ٻ�ʧ��֪ͨ)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewComm::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//��ǰû�����ٻ�
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//������
		return;
	}
	
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = -2;
	}
	else
		return;

	if(pCall->pStation == NULL)
		return;
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != pCall->pStation->m_sID)
	{
		//���Ǳ�װ�õı���
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nQueryTimer);
	
	//������
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//�����ɹ�
	}
	else
	{
		//����ʧ��
		CString str;
		str.Format("��վ %s �ٻ�ͨѶ״̬ʧ��!", pCall->pStation->m_sID);
		WriteLog(str);
	}
	
	//�������
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 �� �� ��: ClearCallList()
 ���ܸ�Ҫ: ����ٻ��б�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewComm::ClearCallList()
{
	for(int i = 0; i < m_arrCall.GetSize(); i++)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(i);
		MYASSERT_CONTINUE(pCall);
		MYDELETE(pCall);
	}
	m_arrCall.RemoveAll();
}

/*************************************************************
 �� �� ��: AddToCallList()
 ���ܸ�Ҫ: ��ָ����վָ��ͨ�����뵽���ٻ��б�
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ��ͨ��
**************************************************************/
void CViewComm::AddToCallList( CStationObj* pObj, int nChannel )
{
	MYASSERT(pObj);
	StationStauts_Call* pNew = new StationStauts_Call;
	pNew->pStation = pObj;
	pNew->nChannel = nChannel;
	m_arrCall.Add(pNew);
}

/*************************************************************
 �� �� ��: StartCall()
 ���ܸ�Ҫ: ���ٻ�������ٻ�
 �� �� ֵ: 
 ��    ��: param1 ָ���ٻ���
		   Param2 
**************************************************************/
void CViewComm::StartCall( StationStauts_Call* pCall)
{
	MYASSERT(pCall);
	if(QueryStationStatus(pCall->pStation, pCall->nChannel) < 0)
	{
		//���в�ѯʧ��
		pCall->nRelust = -1;
	}
	else
	{
		//���в�ѯ�ɹ�
		//���ҳ�վ���б��е�λ��
		CommStatusList* pCommStatusList = NULL;
		m_mapStationPos.Lookup(pCall->pStation->m_sName, (void*&)pCommStatusList);
		if(pCommStatusList != NULL && pCommStatusList->hItem != NULL)
		{
			//�ı�����
			int IDX = m_List.GetColumnsCount();
			m_List.SetItemText(pCommStatusList->hItem, StringFromID(IDS_CALL_CALLING), IDX - 2);
			int iIndex = m_List.GetItemIndex(pCommStatusList->hItem);
			m_List.Update(iIndex);
		}
	}
}

/*************************************************************
 �� �� ��: OnEndCallStationStatus()
 ���ܸ�Ҫ: ��Ӧ�ٻ������Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewComm::OnEndCallStationStatus( WPARAM wParam, LPARAM lParam )
{
	//ȡ���б��еĵ�һ������
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	MYASSERT(pCall);
	CStationObj* pStation = pCall->pStation;
	MYASSERT(pStation);
	//�ٻ����
	CString str;
	if(pCall->nRelust == 0)
	{
		if(pCall->nChannel == 0)
			str.Format("%s:%s, %s:%d, %s:%s\n", 
			StringFromID(IDS_MODEL_SUBSTATION),pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL),pCall->nChannel, 
			StringFromID(IDS_COMMU_STATUS),pCall->pStation->m_nCommStatus==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		else if(pCall->nChannel == 1)
			str.Format("%s:%s, %s:%d, %s:%s\n", 
			StringFromID(IDS_MODEL_SUBSTATION),pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL),pCall->nChannel, 
			StringFromID(IDS_COMMU_STATUS), pCall->pStation->m_nCommStatus1==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
	}
	else if(pCall->nRelust == -1)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_SEND));
	}
	else if(pCall->nRelust == -2)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_RESPONSE));
	}
	else if(pCall->nRelust == -3)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_TIMEOUT));
	}
	m_sResult += str;
	m_arrCall.RemoveAt(0);
	
	//�ж��Ƿ��ٻ�ȫ������
	if(m_arrCall.GetSize() > 0)
	{
		//�����ٻ�
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		StartCall(pCall);
	}
	else
	{
		//�ٻ�����
		//���в�ѯ�ɹ�
		//���ҳ�վ���б��е�λ��
		CommStatusList* pCommStatusList = NULL;
		m_mapStationPos.Lookup(pCall->pStation->m_sName, (void*&)pCommStatusList);
		if(pCommStatusList != NULL && pCommStatusList->hItem != NULL)
		{
			//�ı�����
			int IDX = m_List.GetColumnsCount();
			m_List.SetItemText(pCommStatusList->hItem, StringFromID(IDS_COMMU_TEST), IDX - 2);
			int iIndex = m_List.GetItemIndex(pCommStatusList->hItem);
			m_List.Update(iIndex);
		}
		AfxMessageBox(m_sResult);
	}
	//ɾ����һ��
	MYDELETE(pCall);
}


/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ����param1	�Ƚ϶���1
		   Param2	�Ƚ϶���2
		   param3	�Ƚϲ���
**************************************************************/
int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//ȡ�ñȽ�����
	HTREEITEM hItem1 = (HTREEITEM) lParam1;
	HTREEITEM hItem2 = (HTREEITEM) lParam2;
	CStationObj* pObj1 = (CStationObj*)pApp->m_pComm->m_List.GetItemData(hItem1);
    CStationObj* pObj2 = (CStationObj*)pApp->m_pComm->m_List.GetItemData(hItem2);
	if(pObj1 == NULL || pObj2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	if (nCol == 0 || nCol == 4)
	{
		return 0;
	}

	CString str1, str2;
	int iResult = 0;
	//"��վ��", "״̬","״̬�ı�ʱ��","״̬����ʱ��"
	switch(nCol)
	{
	case 0: //��վ��
		str1 = pObj1->m_sName;
		str2 = pObj2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //״̬
		iResult = pObj1->m_nCommStatus - pObj2->m_nCommStatus;
		break;
	case 2: //״̬�ı�ʱ��
		str1 = pObj1->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		str2 = pObj2->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //״̬����ʱ��
		iResult = pObj1->m_lDurative - pObj2->m_lDurative;
		break;
	case 4: //״̬�ı�ԭ��
		str1 = pObj1->m_sNewStatusReason;
		str2 = pObj2->m_sNewStatusReason;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iCommViewAsc;
	return iResult;
}

/*************************************************************
 �� �� ����SetListData()
 ���ܸ�Ҫ�������б�����
 �� �� ֵ: ��
**************************************************************/
void CViewComm::SetListData()
{
	/*m_List.DeleteAllItems();
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStationName = _T("");

	//������վmap
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStationName, (void*&)pList);
		SetStationInfo(pList);
	}*/
	FillListData();
	//RefreshStatusDetail();
}

/*************************************************************
 �� �� ����ClearStationPosMap()
 ���ܸ�Ҫ�����StationPosMap
 �� �� ֵ: ��
**************************************************************/
void CViewComm::ClearStationPosMap()
{
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		ClearRecord(pList);
		MYDELETE(pList);
	}
	m_mapStationPos.RemoveAll();
}


void CViewComm::OnCommandItem(UINT nID)
{
	if(nID < ITEM_1 || nID > ITEM_13)
	{
		return;
	}
	
	int nPos = nID - (int)ITEM_1; //����,��0��ʼ

	m_arrColum[nPos].bHide = !m_arrColum[nPos].bHide;
	//�ı��е�����
	if(m_arrColum[nPos].bHide)
	{		//������
		m_List.SetColumnWidth(nPos, 0);
	}
	else 
	{		//����ʾ
		m_List.SetColumnWidth(nPos, m_arrColum[nPos].nItemWidth);
	}
	SetListData();
}


void CViewComm::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_LIST_COMM)
	{
		lpMeasureItemStruct->itemHeight = 25;
	}
	
	CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CViewComm::ReadStatusData(CStationObj* pStationObj, CommStatusList** pList)
{
	MYASSERT(pStationObj);

	if(m_bExitThread)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)
	
	//�����ֶ�
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "10", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "Reserve2", EX_STTP_DATA_TYPE_STRING);
	
	//��������
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", pStationObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	Condition Condition1;
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	//strSQL.Format("select Status, Time, Reason, Reserve1, Reserve2  from tb_pt_station_status where station_id = '%s' and Reserve2 > '%s' order by Reserve2", pStationObj->m_sID, strTime);
	//WriteLog(strSQL);
	
	//���в�ѯ
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if(m_bExitThread)
		return;

	//CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CViewComm::ReadDetailReason, ��ѯʧ��");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CViewComm::ReadDetailReason, ��վ[%s]��ȡ��%d������", pStationObj->m_sID, nCount);
		WriteLog(str);
		
		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;
		if(nCount > 0)
		{
			for(int k = 0; k < nCount - 1; k++)
			{
				if(m_bExitThread)
					return;
				pMemset.MoveNext();
			}
			for(int i = 0; i < nCount; i++)
			{
				if(m_bExitThread)
					return;
				strStatus = pMemset.GetValue((UINT)0);
				strTime = pMemset.GetValue(1);
				strReason = pMemset.GetValue(2);
				strChannel = pMemset.GetValue(3);
				strReasonTime = pMemset.GetValue(4);
				
				//pStationObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pStationObj->m_nIPFlag);
				AddRecordToList(pStationObj, pList);
				//pMemset->MoveNext();
				pMemset.MoveForward();
			}
		}

		if (0 == nCount)
		{
			CString strLog;
			strLog.Format("CViewComm::ReadStatusData, δ�ҵ�%sͨ�ϼ�¼", pStationObj->m_sID);
			WriteLog(strLog);
			//LoadStatusFromDB(pStationObj, pList);
			AddRecordToList(pStationObj, pList);
			//AddRecordToList(pObj, pList);
		}
	}
	else
	{
		CString str;
		str.Format("CViewComm::ReadDetailReason, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
}


void CViewComm::ClearRecord(CStationObj* pStationObj, BOOL bClearAll)
{
	MYASSERT(pStationObj);
	CommStatusList* pList = NULL;
	m_mapStationPos.Lookup(pStationObj->m_sName, (void*&)pList);
	MYASSERT(pList);
	ClearRecord(pList);
	if (bClearAll)
	{
		MYDELETE(pList);
		m_mapStationPos.RemoveKey(pStationObj->m_sName);
	}
}


void CViewComm::ClearRecord(CommStatusList* pList)
{
	MYASSERT(pList);
	Record* pRecord = NULL;
	POSITION posList = pList->recordList.GetHeadPosition();
	while(posList != NULL)
	{
		pRecord = pList->recordList.GetNext(posList);
		MYDELETE(pRecord);
	}
	pList->recordList.RemoveAll();
}

BOOL CViewComm::AddRecordToList(CStationObj* pStationObj, CommStatusList** pList)
{
	MYASSERT_BOOL(pStationObj);	
	m_mapStationPos.Lookup(pStationObj->m_sName, (void*&)(*pList));
	if (*pList == NULL)
	{
        *pList = new CommStatusList;
		(*pList)->pStationObj = pStationObj;
		m_mapStationPos.SetAt(pStationObj->m_sName, *pList);
	}
	
	return AddRecordToList(*pList);
}

BOOL CViewComm::AddRecordToList(CommStatusList* pList)
{	
	MYASSERT_BOOL(pList);
	MYASSERT_BOOL(pList->pStationObj);
	
	Record* pRecord = new Record(pList->pStationObj);
	pList->recordList.AddTail(pRecord);
	while(pList->recordList.GetCount() > 10 )
	{
		Record* pRecord = pList->recordList.GetHead();
		pList->recordList.RemoveHead();
		MYDELETE(pRecord);
	}
	return TRUE;
}


int CViewComm::GetStatus(Record* pRecord, int* nCut)
{
	MYASSERT_BOOL(nCut);
	*nCut = GetStatus(pRecord);
	return *nCut;
}


int CViewComm::GetStatus(Record* pRecord)
{
	MYASSERT_BOOL(pRecord);
	return GetStatus(pRecord->m_nCommStatus, pRecord->m_nCommStatus1, pRecord->m_nIPFlag);
}

int CViewComm::GetStatus(CStationObj* pStationObj)
{
	MYASSERT_BOOL(pStationObj);
	return GetStatus(pStationObj->m_nCommStatus, pStationObj->m_nCommStatus1, pStationObj->m_nIPFlag);
}


int CViewComm::GetStatus(int nCommStatus, int nCommStatus1, int nIPFlag)
{
	int nReturn = 0;
	if(nIPFlag == 0)
	{
		if(nCommStatus != 1)
			nReturn = 0;
		else
			nReturn = 3;
	}
	else
	{
		if(nCommStatus != 1 && nCommStatus1 != 1)
			nReturn = 0;
		if(nCommStatus == 1 && nCommStatus1 != 1)
			return 1;
		if(nCommStatus != 1 && nCommStatus1 == 1)
			return 2;
		if(nCommStatus == 1 && nCommStatus1 == 1)
			return 3;
	}
	return nReturn;
}



/*************************************************************
 �� �� ����LoadStatusFromDB()
 ���ܸ�Ҫ����Ӳ�����ݿ������վ����վͨѶ״̬,ֻ��ѯ����һ��
 �� �� ֵ: void ״̬
 ��    ����param1	ͨ��
**************************************************************/
//##ModelId=49B87BA6038A
void CViewComm::LoadStatusFromDB(CStationObj* pObj, CommStatusList** pList)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//ͨ��Station_ID��ѯ����һ��ֵ(Status, Time, Reason)

	//�����ֶ�
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "Reserve2", EX_STTP_DATA_TYPE_STRING);

	//��������
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);

	Condition Condition1;
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet pMemset;
//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewComm::LoadStatusFromDB, ��ѯʧ��");
		delete[] sError;
//		delete pMemset;
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;			
		for(int i = 0; i < nCount; i++)
		{
			strStatus = pMemset.GetValue((UINT)0);
			strTime = pMemset.GetValue(1);
			strReason = pMemset.GetValue(2);
			strChannel = pMemset.GetValue(3);
			strReasonTime = pMemset.GetValue(4);
			
			pObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pObj->m_nIPFlag);
			AddRecordToList(pObj, pList);
			pMemset.MoveNext();		
		}
		if (0 == nCount)
		{
			str.Format("CViewComm::LoadStatusFromDB, δ�ҵ�%s������ͨ�ϼ�¼", pObj->m_sID);
			WriteLog(str);
			AddRecordToList(pObj, pList);
		}
	}
	else
	{
		str.Format("CViewComm::LoadStatusFromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
//	delete pMemset;
	sError = NULL;
//	pMemset = NULL;
}

void CViewComm::RefreshStatusDetail()
{
	ClearStationPosMap();

	StartThread();
}

BOOL CViewComm::LoadAllDataFromDB()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	//�齨��ѯ����
	CString str = "";
	CString strSQL;
	strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	//strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	WriteLog(strSQL);
	
	//���в�ѯ
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewComm::LoadAllDataFromDB, ��ѯʧ��");
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		str.Format("CViewComm::LoadAllDataFromDB, ��ѯ��%d������", nCount);
		WriteLog(str);
		
		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;			
		for(int i = 0; i < nCount; i++)
		{
			strStatus = pMemset.GetValue((UINT)0);
			strTime = pMemset.GetValue(1);
			strReason = pMemset.GetValue(2);
			strChannel = pMemset.GetValue(3);
			strReasonTime = pMemset.GetValue(4);
			CString strID = pMemset.GetValue(5);
			CStationObj* pObj = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
			if(pObj != NULL)
			{
				pObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pObj->m_nIPFlag);
				CommStatusList* pList = NULL;
				AddRecordToList(pObj, &pList);
			}
			pMemset.MoveNext();		
		}
	}
	else
	{
		str.Format("CViewComm::LoadAllDataFromDB, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
//	pMemset = NULL;
	return TRUE;
}

void CViewComm::LoadDetailDataFromDB() 
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;

	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::LoadDetailDataFromDB, ȡ��������Ȩ");

	while(pos != NULL)
	{
		if(m_bExitThread)
		{
			LeaveCriticalSection(&m_CriticalSection);
			WriteLog("CViewComm::LoadDetailDataFromDB, �ͷ�������Ȩ");
			return;
		}
		Sleep(10);
		//ȡ�ó�վָ��
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		//��ȡ����
		MYASSERT_CONTINUE(pObj);
		CommStatusList* pList = NULL;
		ReadStatusData(pObj, &pList);
	}
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::LoadDetailDataFromDB, �ͷ�������Ȩ");
}

/****************************************************
Date:2013/6/4  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewComm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	return; //2015/3/10 for test

	ClearStationPosMap();
	LoadAllDataFromDB();
	FillListData();
	RefreshStatusDetail();
	WriteLog("StationInit,��վͨѶ״̬ˢ�����", XJ_LOG_LV3);
}

/****************************************************
Date:2013/10/8  Author:LYH
������:   TestStationStatus	
����ֵ:   void	
���ܸ�Ҫ: 
����: CStationObj * pStation	
*****************************************************/
void CViewComm::TestStationStatus( CStationObj* pStation )
{
	if(!pStation)
		return;

	int IDX = m_List.GetColumnsCount();
	//���ҳ�վ���б��е�λ��
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
				
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(pList->pStationObj == pStation)
			break;
		pList = NULL;
	}
	if(pList == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall->pStation != pStation)
		{
			AfxMessageBox(StringFromID(IDS_TESTSTATION_BUSY));
		}
		else
		{
			//ȡ���ٻ�
			if(AfxMessageBox(StringFromID(IDS_TESTSTATION_CANCEL), MB_YESNOCANCEL)==IDYES)
			{
				//���뱨����������
				STTP_FULL_DATA sttpData;
				//ֹͣ��ʱ��
				KillTimer(m_nQueryTimer);
				//�齨ȡ������
				pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20145, pStation->m_sID, 0);
				//���ͱ���
				pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
				//����ٻ��б�
				ClearCallList();
				
				
				//�ı�����
				if(pList != NULL)
				{
					int IDX = m_List.GetColumnsCount();
					m_List.SetItemText(pList->hItem, StringFromID(IDS_COMMU_TEST), IDX - 2);
					m_List.Update(m_List.GetItemIndex(pList->hItem));
				}
				
			}
		}
		return;
	}
	
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	m_sResult = "";
	m_List.SetItemText(pList->hItem, StringFromID(IDS_BTN_CANCEL_CALL), IDX - 2);
	//��ѯ��վ״̬
	if(pList->pStationObj->m_nIPFlag == 0) //��ͨ��
	{
		AddToCallList(pList->pStationObj, 0);
	}
	else //˫ͨ��
	{
		AddToCallList(pList->pStationObj, 0);
		AddToCallList(pList->pStationObj, 1);
	}
	
	//��ʼ�ٻ�
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall != NULL)
		{
			StartCall(pCall);
		}
	}
}

/****************************************************
Date:2013/10/8  Author:LYH
������:   OnTestStationStatus	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewComm::OnTestStationStatus( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	CStationObj* pStation = (CStationObj*)lParam;
	TestStationStatus(pStation);
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   StartThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewComm::StartThread()
{
	EndThread();
	m_bExitThread = FALSE;
	m_pLoadDataThread = AfxBeginThread(CommViewLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   EndThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewComm::EndThread()
{
	if(!m_pLoadDataThread)
		return;
	m_bExitThread = TRUE;
	DWORD dw = WaitForSingleObject(m_pLoadDataThread->m_hThread, INFINITE);
	switch(dw)
	{
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		TerminateThread(m_pLoadDataThread->m_hThread,NULL);
		WriteLog("���ձ����߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
		break;
	}
	m_pLoadDataThread = NULL;
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   NotifyThreadEnd	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewComm::NotifyThreadEnd()
{
	m_pLoadDataThread = NULL;
	m_bExitThread = FALSE;
	WriteLog("CViewComm::NotifyThreadEnd �߳̽���", XJ_LOG_LV3);
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   OnAllClose	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewComm::OnAllClose( WPARAM wParam, LPARAM lParam )
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
void CViewComm::OnReFillData( WPARAM wParam, LPARAM lParam )
{
	FillListData_Multi();
}

/****************************************************
Date:2014/1/3  Author:LYH
������:   FillListData_Multi	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CViewComm::FillListData_Multi()
{
	if(m_bExitThread)
		return 0;
	WriteLog("CViewComm::FillListData_Multi, start");
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::FillListData_Multi, ȡ��������Ȩ");
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		if(m_bExitThread)
		{
			break;
		}
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(SetStationInfo(pList))
		{
			
		}
		//	AddStationToList(pList->pStationObj);
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::FillListData_Multi, �ͷ�������Ȩ");
	WriteLog("CViewComm::FillListData_Multi, end");
	return 0;
}
