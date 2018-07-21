// ViewPTComm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewPTComm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iPTCommViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewPTComm

IMPLEMENT_DYNCREATE(CViewPTComm, CFormView)

//##ModelId=49B87B8201F5
CViewPTComm::CViewPTComm()
	: CFormView(CViewPTComm::IDD)
{
	//{{AFX_DATA_INIT(CViewPTComm)
	m_bShowPTCommOFF = g_ShowPTCommOFF;
	m_bShowPTCommON = g_ShowPTCommON;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_PTCOMM;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	m_nTimer = 0;
	g_iPTCommViewAsc = -1;
	m_nTimer = -1;
	m_sChooseStationID = "";

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
}

//##ModelId=49B87B82033C
CViewPTComm::~CViewPTComm()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewPTComm::~CViewPTComm \n");
}

//##ModelId=49B87B82030D
void CViewPTComm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewPTComm)
	DDX_Control(pDX, IDC_CHECK_SHOWPTCOMMON, m_cBoxShowPTCommON);
	DDX_Control(pDX, IDC_CHECK_SHOWPTCOMMOFF, m_cBoxShowPTcommOFF);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATIOIN, m_cmbChooseStation);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_LIST_PTCOMM, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWPTCOMMOFF, m_bShowPTCommOFF);
	DDX_Check(pDX, IDC_CHECK_SHOWPTCOMMON, m_bShowPTCommON);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewPTComm, CFormView)
	//{{AFX_MSG_MAP(CViewPTComm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTCOMM, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTCOMM, OnClickListPtcomm)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTCOMM, OnColumnclickListPtcomm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTCOMM, OnDblclkListPtcomm)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_PTCOMM_ON, OnPtcommOn)
	ON_COMMAND(IDC_PTCOMM_OFF, OnPtcommOff)
	ON_COMMAND(IDC_PTCOMM_HISTORY, OnPtcommHistory)
	ON_WM_TIMER()
	ON_COMMAND(IDC_PTCOMM_EXPORT, OnPtcommExport)
	ON_COMMAND(IDC_PTCOMM_PRINT, OnPtcommPrint)
	ON_COMMAND(IDC_PTCOMM_REFRESH, OnPtcommRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATIOIN, OnSelchangeComboChoosestatioin)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTCOMMON, OnCheckShowptcommon)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTCOMMOFF, OnCheckShowptcommoff)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B82035B
void CViewPTComm::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B82036B
void CViewPTComm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm message handlers

//##ModelId=49B87B82032C
void CViewPTComm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_DEVICECOMMU));
	//GetParentFrame()->SetWindowText("�豸ͨ��");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B820301
void CViewPTComm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	InitComBox();
	InitListCtrl();

	//��ʼˢ�±���ͨ��״̬����ʱ�䶨ʱ��
	m_nTimer = SetTimer(1, g_RefreshPTCommTime*60*1000, 0);
}

//##ModelId=49B87B82037A
void CViewPTComm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
//##ModelId=49B87B820254
int CViewPTComm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	FillListData();

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B820271
int CViewPTComm::InitListStyle()
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
		//char* arColumn[9]={"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸", "���ܵ�λ", "��ά��λ", "���޵�λ"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 300; //�豸����
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 1://
				lvCol.cx = 100; // ״̬
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 2://
				lvCol.cx = 180; // ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 200; // ����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4://
				lvCol.cx = 140; // ��վ
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 5:
				lvCol.cx = 160; // һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 6:
				lvCol.cx = 160; // �������ܵ�λ
				colName = StringFromID(IDS_MODEL_MANAGEMENT);
				break;
			case 7:
				lvCol.cx = 160; // ��ά��λ
				colName = StringFromID(IDS_MODEL_MAINTENANCE);
				break;
			case 8:
				lvCol.cx = 160; // ���޵�λ
				colName = StringFromID(IDS_MODEL_OVERHUALUNIT);
				break;
			default:
				lvCol.cx=160;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�7��(�������ܵ�λ)
		m_List.SetColumnHide(6, TRUE);
		//Ĭ�����ص�8��(��ά��λ)
		m_List.SetColumnHide(7, TRUE);
		m_List.SetColumnHide(8, TRUE);
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
//##ModelId=49B87B820281
BOOL CViewPTComm::LoadListConfig()
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
			WriteLog("ViewPTComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewPTComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewPTCommConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewPTCommConfig", XJ_LOG_LV3);
				WriteLog("ViewPTComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewCommConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewPTComm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B82029F
BOOL CViewPTComm::SaveListConfig()
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
			WriteLog("ViewPTComm::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewPTComm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewPTCommConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewPTCommConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewPTCommConfig");
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
						WriteLog("δ�ҵ�ViewPTCommConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewPTCommConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B820280
int CViewPTComm::FillListData()
{

	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;

	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice);
	//ѭ�����г�վ
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (pObj->m_pStation->m_sID == m_sChooseStationID||m_sChooseStationID.IsEmpty())
		{
			AddEventToList(pObj, 0);
		}
		
	}
	m_cBoxShowPTcommOFF.SetCheck(m_bShowPTCommOFF);
	m_cBoxShowPTCommON.SetCheck(m_bShowPTCommON);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8202A0
BOOL CViewPTComm::NeedSave()
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
//##ModelId=49B87B8202AF
void CViewPTComm::RegulateControlSize()
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
		m_cBoxShowPTCommON.ShowWindow(FALSE);
		m_cBoxShowPTcommOFF.ShowWindow(FALSE);
	//	m_cboxShowDebug.ShowWindow(FALSE);
	//	m_cboxShowReturn.ShowWindow(FALSE);
	//	m_cboxShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	
	
	//�����б���С,ռ�����߶ȵ�(1 - 0.618)

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B820399
void CViewPTComm::OnDestroy() 
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
//##ModelId=49B87B8202BF
BOOL CViewPTComm::LoadDataFromDB(int num, BOOL IsQueryAcked)
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
	//ѡ����ʾ��վ����Ϣ
	if ( !m_sChooseStationID.IsEmpty())
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewPTComm::LoadDataFromDB, ��ѯʧ��", XJ_LOG_LV3);
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
			str.Format("CViewPTComm::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventPTComm * pEvent = new CXJEventPTComm;
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
		str.Format("CViewPTComm::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����OnSTTP20002()
 ���ܸ�Ҫ����Ӧ20002(����ͨѶ״̬����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	�豸ָ��
		   Param2	��������
**************************************************************/
//##ModelId=49B87B830021
void CViewPTComm::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewPTComm::OnSTTP20002 start", XJ_LOG_LV3);
	//ȡ���豸ָ��
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj != NULL)
	{
		if (pObj->m_sStationID != m_sChooseStationID&& !m_sChooseStationID.IsEmpty())
		{
			CString str;
			str.Format("�յ���վIDΪ%s���ģ����ǵ�ǰѡ���ĳ�վ���ѹ��ˡ�",pObj->m_sStationID);
			WriteLog(str, XJ_LOG_LV3);
			return;
		}
		//�ı���ͨ��״̬�б��е���ʾ
 		ChangeStatusInList(pObj);

		CString str;
		str.Format("װ��ID:%s,����:%s,��վID:%s,��ǰ״̬:%d", pObj->m_sID, pObj->m_sName, pObj->m_sStationID,pObj->m_nCommStatus);
		WriteLog(str);
	}
	//��˸
	if(pObj != NULL && g_BlinkDevice == 1)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView * pView = pApp->GetSVGView();
		//CViewCommMap* pCommView = pApp->GetCommView();
		//++m_nCount;
		if(pView != NULL)
		{
			WriteLog("CViewPTComm::OnSTTP20002, ��˸�豸", XJ_LOG_LV3);
			//��˸
			//pView->AddBlinkDevice(pObj);
			pView->AddToBlinkList(pObj);
		}
	}	

	WriteLog("CViewPTComm::OnSTTP20002 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ�����÷�����״̬�±���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B82039B
void CViewPTComm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					//if(nSubItem == 1)//״̬
					//{
						//�õ��¼�����
						CSecObj * pSec = (CSecObj*)m_List.GetItemData(nItem);
						if(pSec == NULL)
							break;
						if(pSec->m_nCommStatus == 1)
						{
							//�Ͽ�
						    lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					//}
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

//##ModelId=49B87B8203AA
void CViewPTComm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_PTCOMM))
				return;

			Menu.DeleteMenu(IDC_EVENT_DEVICE, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_PTCOMM_REFRESH, MF_BYCOMMAND);

			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
			}
			if(1 == g_ShowPTCommON)
			{
				//��ʾͨ��״̬Ϊ������װ����Ϣ
				pMenu->CheckMenuItem(IDC_PTCOMM_ON, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTCOMM_ON, MF_UNCHECKED);
			}
			if(1 == g_ShowPTCommOFF)
			{
				//��ʾͨ��״̬Ϊ�Ͽ���װ����Ϣ
				pMenu->CheckMenuItem(IDC_PTCOMM_OFF, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTCOMM_OFF, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}


//##ModelId=49B87B8203B9
void CViewPTComm::OnEventDevice() 
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
	//�õ��豸
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//��ʾ���¼���һ���豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pObj->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pObj->m_pOwner);
}

//##ModelId=49B87B8203BB
void CViewPTComm::OnEventSec() 
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
	//�õ��豸
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pObj);
}

//##ModelId=49B87B8203C8
void CViewPTComm::OnClickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
// 	�õ���ѡ��
// 		NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
// 		if(nSelected > -1)
// 		{
// 			//������¼�, ȷ�ϴ��¼�
// 			//�õ����ж�Ӧ���¼�
// 			CXJEventPTComm * pEvent = (CXJEventPTComm*)m_List.GetItemData(nSelected);
// 			if(pEvent == NULL)
// 				return;
// 			if(pEvent->Ack())
// 			{
// 				//ˢ�½�����ʾ, ֪ͨ�������ڸ���
// 				RefreshAck(pEvent, nSelected);
// 				TellOtherWndAck(pEvent);
// 			}
// 		}
	*pResult = 0;
}

//##ModelId=49B87B8203CC
void CViewPTComm::OnEventSecProp() 
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
	//�õ��豸
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//��ʾ���¼��Ķ����豸ͼ��
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pObj->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pObj, 1);
	else
		pDoc->ShowSecPropPage(pObj, PT_PROP_GENERAL);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,����ʱ������ 
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ����param1	�Ƚ϶���1
		   Param2	�Ƚ϶���2
		   param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B820247
int CALLBACK CViewPTComm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CSecObj* pObj1 = (CSecObj*)lParam1;
	CSecObj* pObj2 = (CSecObj*)lParam2;
	if(pObj2 == NULL || pObj1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸","�������ܵ�λ", "��ά��λ"
	switch(nCol)
	{
	case 0: //��������
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
		str1.Format("%s", pObj1->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //״̬����ʱ��
		iResult = pObj1->m_lCommDurative - pObj2->m_lCommDurative;
		break;
	case 4: //��վ��
		str1 = pObj1->m_sStationID;
		str2 = pObj2->m_sStationID;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //һ���豸��
		str1 = "";
		if(pObj1->m_pOwner != NULL)
			str1 = pObj1->m_pOwner->m_sName;
		str2 = "";
		if(pObj2->m_pOwner != NULL)
			str2 = pObj2->m_pOwner->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: // �������ܵ�λ
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nManagedepartment - pObj2->m_pStation->m_nManagedepartment;
		break;
	case 7: // ��ά��λ
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nMaintaindepartment - pObj2->m_pStation->m_nMaintaindepartment;
		break;
	case 8: //���޵�λ
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nServiceDepartment - pObj2->m_pStation->m_nServiceDepartment;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTCommViewAsc;
	return iResult;
}

//##ModelId=49B87B8203D8
void CViewPTComm::OnColumnclickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTCommViewAsc = - g_iPTCommViewAsc;
	}
	else
	{
		g_iPTCommViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B8203DC
void CViewPTComm::OnDblclkListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		
	}
	*pResult = 0;
}

//##ModelId=49B87B830000
BOOL CViewPTComm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTCOMM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ����ָ�������豸��ͨ��״̬��Ϣ���뵽�б����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ�������豸
		   Param2	ָ�����б��е�λ��
**************************************************************/
//##ModelId=49B87B8202CE
BOOL CViewPTComm::AddEventToList( CSecObj* pSec, int i )
{
	//�ж�������Ч��
	if(pSec == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	if(!IsNeedShow(pSec->m_nCommStatus))
		return FALSE;
	//ˢ�³���ʱ��
	pSec->RefreshCommDurativeTime();

	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸", "�������ܵ�λ", "��ά��λ"

	//0:�豸����
	CString str;
	str = pSec->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	//1:״̬
	m_List.SetItemText(i, 1, GetCommStatusString(pSec->m_nCommStatus));
	//2:״̬�ı�ʱ��
	m_List.SetItemText(i, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
	//3:״̬����ʱ��
	m_List.SetItemText(i, 3, pSec->m_sCommDurativeTime);
	//4:��վ��
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sName;
	}
	m_List.SetItemText(i, 4, str);
	//һ���豸
	str = "";
	if(pSec->m_pOwner != NULL)
	{
		str = pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(i, 5, str);
	
	//�������ܵ�λ
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sManagedepartment;
	}
	m_List.SetItemText(i, 6, str);
	
	//��ά��λ
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sMaintaindepartment;
	}
	m_List.SetItemText(i, 7, str);

	str = "";
	if(pSec->m_pStation != NULL)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData != NULL)
			str = pData->GetServiceDepartmentName(pSec->m_pStation->m_nServiceDepartment);
	}
	m_List.SetItemText(i, 8, str);

	//��������
	m_List.SetItemData(i, (DWORD)pSec);
	return TRUE;
}

/*************************************************************
 �� �� ����IsNeedShow()
 ���ܸ�Ҫ���жϵ�ǰͨ��״̬�ı�����Ϣ�Ƿ���Ҫ��ʾ
 �� �� ֵ: ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 ��    ����param1	ͨ��״̬
		   Param2
**************************************************************/
//##ModelId=49B87B8202DE
BOOL CViewPTComm::IsNeedShow( int nStatus )
{
	BOOL bReturn = FALSE;
	switch(nStatus)
	{
	case 0: //����
		if(1 == g_ShowPTCommON)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 1: //�Ͽ�
		if(1 == g_ShowPTCommOFF)
			bReturn = TRUE;
		else 
			bReturn = FALSE;
		break;
	default:
		bReturn = TRUE;
		break;
	}
	return bReturn;
}

//##ModelId=49B87B830003
void CViewPTComm::OnPtcommOn() 
{
	// TODO: Add your command handler code here
	//�ı�ͨ��״̬�����Ƿ�Ҫ��ʾ������
	g_ShowPTCommON = !g_ShowPTCommON;
	m_bShowPTCommON = g_ShowPTCommON;
	//�����������
	FillListData();
}

//##ModelId=49B87B830005
void CViewPTComm::OnPtcommOff() 
{
	// TODO: Add your command handler code here
	//�ı�ͨ��״̬�Ͽ��Ƿ�Ҫ��ʾ������
	g_ShowPTCommOFF = !g_ShowPTCommOFF;
	m_bShowPTCommOFF = g_ShowPTCommOFF;
	//�����������
	FillListData();
}

//##ModelId=49B87B83000F
void CViewPTComm::OnPtcommHistory() 
{
	// TODO: Add your command handler code here
	//׼���򿪲�ѯ��ͳ�ƵĲ���
	//����ͨѶ״̬��ѯ: 2$$$��վID$$$�豸����$$$�豸ID $$$ͨѶ״̬$$$�Զ���ѯ(0-���Զ���ѯ, 1-�Զ���ѯ)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount < 1)
	{
		sSQL = "2$$$ $$$99$$$ $$$99$$$0";
	}
	else
	{
		int nIndex = -1;
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex == -1)
			return;
		//�õ��豸
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
		if(pObj == NULL)
			return;
		if(pObj->m_nType == TYPE_PROTECT)
			sSQL.Format("2$$$ $$$99$$$ $$$%s$$$99$$$1", pObj->m_sID);
		else
			sSQL.Format("2$$$ $$$99$$$%s$$$ $$$99$$$1", pObj->m_sID);
	}
	//�򿪲�ѯ��ͳ��
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

/*************************************************************
 �� �� ����ChangeStatusInList()
 ���ܸ�Ҫ���ı�ָ�������豸��ͨ��״̬��ͨ��״̬�б���е���ʾ
 �� �� ֵ: void
 ��    ����param1	ָ�������豸
		   Param2
**************************************************************/
//##ModelId=49B87B8202E0
void CViewPTComm::ChangeStatusInList( CSecObj* pSec )
{
	//�ж�������Ч��
	if(pSec == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;

	//�������б��е�λ��
	LVFINDINFO lvFind;
	int nIndex = -1;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pSec;
	nIndex = m_List.FindItem(&lvFind);

	//�ж��Ǵ�״̬���Ƿ���Ҫ��ʾ�豸��Ϣ
	if(!IsNeedShow(pSec->m_nCommStatus))
	{
		if(nIndex > -1)
		{
			//�Ƴ�
			m_List.DeleteItem(nIndex);
		}
	}
	else
	{
		if(nIndex < 0)
		{
			//����
			AddEventToList(pSec, 0);
		}
		else
		{
			//�ı�ֵ
			//1:״̬
			m_List.SetItemText(nIndex, 1, GetCommStatusString(pSec->m_nCommStatus));
			//2:״̬�ı�ʱ��
			m_List.SetItemText(nIndex, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
			//3:״̬����ʱ��
			m_List.SetItemText(nIndex, 3, pSec->m_sCommDurativeTime);
			//ˢ��
			m_List.Update(nIndex);
		}
	}
}

//##ModelId=49B87B830011
void CViewPTComm::OnTimer(UINT nIDEvent) 
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
 �� �� ����RefreshTime()
 ���ܸ�Ҫ��ˢ�����б�����ͨ��״̬�ĳ���ʱ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8202F0
void CViewPTComm::RefreshTime()
{
	WriteLog("CViewPTComm::RefreshTime,��ʱˢ�¿�ʼ", XJ_LOG_LV3);
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
	WriteLog("CViewPTComm::RefreshTime,��ʱˢ�½���", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����RefreshDuravieTime()
 ���ܸ�Ҫ��ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ
 �� �� ֵ: 
 ��    ����param1	ָ����LIST�е�λ��
		   Param2
**************************************************************/
//##ModelId=49B87B8202EE
void CViewPTComm::RefreshDuravieTime( int nIndex )
{
	//����
	int nCount = m_List.GetItemCount();
	//�ж�������Ч��
	if(nIndex < 0)
		return;
	if(nIndex >= nCount)
		return;
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//ˢ�³���ʱ��
	pObj->RefreshCommDurativeTime();
	//������ʾ
	m_List.SetItemText(nIndex, 3, pObj->m_sCommDurativeTime);
	//ˢ��
	m_List.Update(nIndex);
}

//##ModelId=49B87B830014
void CViewPTComm::OnPtcommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	
	CString strtype = StringFromID(IDS_TITLE_DEV_COMMUSTATUS);
	if(m_bShowPTCommON && !m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUON);
	if(!m_bShowPTCommON && m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUOFF);
	if(!m_bShowPTCommON && !m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUUNKNOWN);
	CString sStationName = StringFromID(IDS_COMMON_ALL);
	if(!m_sChooseStationID.IsEmpty())
	{
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATIOIN);
		if(pCmb)
		{
			int nIndex = pCmb->GetCurSel();
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
			if(pStation)
				sStationName = pStation->m_sName;
		}
	}
	CString strName;
	strName.Format("%s-%s%s.xls", sStationName, strtype, t.Format("%Y%m%d%H%M%S"));
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

//##ModelId=49B87B83001F
void CViewPTComm::OnPtcommPrint() 
{
	// TODO: Add your command handler code here
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTCOMM_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTCOMM_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTComm);
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
�� �� ����InitCOM()
���ܸ�Ҫ����ʼ����ӡģ��COM���
�� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
��    ����param1
Param2
**************************************************************/
//##ModelId=49B87B8202F1
BOOL CViewPTComm::InitCOM()
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
//##ModelId=49B87B8202F2
void CViewPTComm::ReleaseCOM()
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
 �� �� ����WriteResultToExcel()
 ���ܸ�Ҫ�����б��е����ݵ�����Excel
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Excel�ļ�����λ��
		   Param2
**************************************************************/
//##ModelId=49B87B8202F3
BOOL CViewPTComm::WriteResultToExcel( CString strFullPath )
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
				//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸","�������ܵ�λ", "��ά��λ"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_COMMON_NO), StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE),
					StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_COMMU_STATUS),
					StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION));
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CSecObj* pObj = (CSecObj*)m_List.GetItemData(i);
				AddResultToExcel(&database, pObj, i+1);
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
//##ModelId=49B87B8202FE
void CViewPTComm::AddResultToExcel( CDatabase* pDatabase, CSecObj* pSec, int index )
{
	//���������Ч��
	if(pSec == NULL)
		return;
	if(pDatabase == NULL)
		return;

	//4:��վ��
	CString strStation = "";
	if(pSec->m_pStation != NULL)
	{
		strStation = pSec->m_pStation->m_sName;
	}
	//һ���豸
	CString strDevice = "";
	if(pSec->m_pOwner != NULL)
	{
		strDevice = pSec->m_pOwner->m_sName;
	}
	// �������ܵ�λ 
	CString strManagedepartment = "";
	// ��ά��λ 
	CString strMaintaindepartment = "";
	if(pSec->m_pStation != NULL)
	{
		strManagedepartment = pSec->m_pStation->m_sManagedepartment;
		strMaintaindepartment = pSec->m_pStation->m_sMaintaindepartment;
	}
	CString strServiceDepartment = "";
	if(pSec->m_pStation != NULL)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData != NULL)
			strServiceDepartment = pData->GetServiceDepartmentName(pSec->m_pStation->m_nServiceDepartment);
	}

	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸","�������ܵ�λ","��ά��λ"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES (%d, '%s','%s','%s','%s','%s','%s','%s', '%s')",
		index, strManagedepartment,strMaintaindepartment, strStation, strDevice, pSec->m_sName, GetCommStatusString(pSec->m_nCommStatus), pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sCommDurativeTime);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B820252
CString CViewPTComm::TranslateSingleKey( CString sKey )
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
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
		if(pObj == NULL)
			return "";
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->m_sName; //�豸��
		}
		if(strTail == "DEVICE_COMM$")
		{
			//ͨ��״̬
			return GetCommStatusString(pObj->m_nCommStatus);
		}
		if(strTail == "DEVICE_COMM_TIME$")
		{
			//ͨ��״̬�ı�ʱ��
			return pObj->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_RUN_TIME$")
		{
			//����״̬�ı�ʱ��
			return pObj->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_COMM_DURATIVE$")
		{
			//ͨ��״̬����ʱ��
			return pObj->m_sCommDurativeTime;
		}
		if(strTail == "DEVICE_RUN_DURATIVE$")
		{
			//����״̬����ʱ��
			return pObj->m_sRunDurativeTime;
		}
		if(strTail == "DEVICE_OWNER$")
		{
			//һ���豸
			CString strDevice = "";
			if(pObj->m_pOwner != NULL)
			{
				strDevice = pObj->m_pOwner->m_sName;
			}
			return strDevice;
		}
		if(strTail == "DEVICE_STATION$")
		{
			//��վ��
			CString strStation = "";
			if(pObj->m_pStation != NULL)
			{
				strStation = pObj->m_pStation->m_sName;
			}
			return strStation;
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
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			
		}
	}
	return "";
}

void CViewPTComm::OnPtcommRefresh() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������ж����豸
	pData->LoadAllSec();
	//ˢ�������豸��״̬
	pData->RefreshDevStatus();
	//��������б��
	FillListData();
}

/*************************************************************
 �� �� ��: OnViewRefresh()
 ���ܸ�Ҫ: ��Ӧҳ��ǿ��ˢ����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewPTComm::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//������������豸״̬ͳһ���������
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������ж����豸
//	pData->LoadAllSec();
	//ˢ�������豸��״̬
//	pData->RefreshDevStatus();
	//��������б��
	FillListData();
	WriteLog("CViewPTComm::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

CScrollBar* CViewPTComm::GetScrollBarCtrl(int nBar) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::GetScrollBarCtrl(nBar);
}
/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewPTComm::InitComBox()
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





void CViewPTComm::OnSelchangeComboChoosestatioin() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//��ȡ��ǰѡ����
	
	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATIOIN))->GetCurSel();
	
		CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)	
	{	CString sStationID=	pStationid->m_sID;
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTComm::OnCheckShowptcommon() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTCommON = !g_ShowPTCommON;
	m_bShowPTCommON = !m_bShowPTCommON;
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTComm::OnCheckShowptcommoff() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTCommOFF = !g_ShowPTCommOFF;
	m_bShowPTCommOFF = !m_bShowPTCommOFF;
	//ˢ���б���Ϣ
	FillListData();
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewPTComm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	InitComBox();//�������볧վѡ��������
	FillListData();
	WriteLog("StationInit,�����豸ͨѶ״̬ҳ��ˢ�����", XJ_LOG_LV3);
}
