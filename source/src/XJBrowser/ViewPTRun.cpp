// ViewPTRun.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewPTRun.h"
#include "MainFrm.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iPTRunViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewPTRun

IMPLEMENT_DYNCREATE(CViewPTRun, CFormView)

//##ModelId=49B87B8102F0
CViewPTRun::CViewPTRun()
	: CFormView(CViewPTRun::IDD)
{
	//{{AFX_DATA_INIT(CViewPTRun)
	m_bShowPTRunCheck = g_ShowPTRunCheck;
	m_bShowPTRunCode = g_ShowPTRunCode;
	m_bShowPTRunDebug = g_ShowPTRunDebug;
	m_bShowPTRunOFF = g_ShowPTRunOFF;
	m_bShowPTRunON = g_ShowPTRunON;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_PTRUN;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	m_nTimer = 0;
	g_iPTRunViewAsc = -1;
	m_sChooseStationID = "";

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
}

//##ModelId=49B87B82007D
CViewPTRun::~CViewPTRun()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewPTRun::~CViewPTRun \n");
}

//##ModelId=49B87B82003E
void CViewPTRun::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewPTRun)
	DDX_Control(pDX, IDC_CHECK_SHOWPTRUNCHECK, m_cboxShowPTRunCheck);
	DDX_Control(pDX, IDC_CHECK_SHOWPTRUNCODE, m_cboxShowPTRunCode);
	DDX_Control(pDX, IDC_CHECK_SHOWPTRUNDEBUG, m_cboxShowPTRunDebug);
	DDX_Control(pDX, IDC_CHECK_SHOWPTRUNOFF, m_cboxShowPTRunOFF);
	DDX_Control(pDX, IDC_CHECK_SHOWPTRUNON, m_cboxShowPTRunON);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATIOIN, m_cmbChooseStation);
	DDX_Control(pDX, IDC_LIST_PTRun, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWPTRUNCHECK, m_bShowPTRunCheck);
	DDX_Check(pDX, IDC_CHECK_SHOWPTRUNCODE, m_bShowPTRunCode);
	DDX_Check(pDX, IDC_CHECK_SHOWPTRUNDEBUG, m_bShowPTRunDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWPTRUNOFF, m_bShowPTRunOFF);
	DDX_Check(pDX, IDC_CHECK_SHOWPTRUNON, m_bShowPTRunON);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewPTRun, CFormView)
	//{{AFX_MSG_MAP(CViewPTRun)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTRun, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTRun, OnClickLISTPTRun)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTRun, OnColumnclickLISTPTRun)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTRun, OnDblclkLISTPTRun)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(IDC_PTRUN_ON, OnPtrunOn)
	ON_COMMAND(IDC_PTRUN_OFF, OnPtrunOff)
	ON_COMMAND(IDC_PTRUN_CHECK, OnPtrunCheck)
	ON_COMMAND(IDC_PTRUN_DEBUG, OnPtrunDebug)
	ON_COMMAND(IDC_PTRUN_CODE, OnPtrunCode)
	ON_COMMAND(IDC_PTRUN_HISTORY, OnPtrunHistory)
	ON_COMMAND(IDC_PTRUN_PRINT, OnPtrunPrint)
	ON_COMMAND(IDC_PTRUN_EXPORT, OnPtrunExport)
	ON_COMMAND(IDC_PTRUN_REFRESH, OnPtrunRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATIOIN, OnSelchangeComboChoosestatioin)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTRUNON, OnCheckShowptrunon)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTRUNOFF, OnCheckShowptrunoff)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTRUNCHECK, OnCheckShowptruncheck)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTRUNDEBUG, OnCheckShowptrundebug)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTRUNCODE, OnCheckShowptruncode)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPTRun diagnostics

#ifdef _DEBUG
//##ModelId=49B87B82008C
void CViewPTRun::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B82009C
void CViewPTRun::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPTRun message handlers

//##ModelId=49B87B82005D
void CViewPTRun::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_DEVICERUN));
	//GetParentFrame()->SetWindowText("�豸����״̬");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B820030
void CViewPTRun::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	InitComBox();
	InitListCtrl();
	//��ʼˢ�±�������״̬����ʱ�䶨ʱ��
	m_nTimer = SetTimer(1, g_RefreshPTCommTime*60*1000, 0);
}

//##ModelId=49B87B8200BB
void CViewPTRun::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
//##ModelId=49B87B81035D
int CViewPTRun::InitListCtrl()
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
//##ModelId=49B87B81036B
int CViewPTRun::InitListStyle()
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
				lvCol.cx = 120; // ��վ
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 5:
				lvCol.cx = 140; // һ���豸
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
//##ModelId=49B87B81038A
BOOL CViewPTRun::LoadListConfig()
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
			WriteLog("ViewPTRun::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewPTRun::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewPTRunConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewPTRunConfig", XJ_LOG_LV3);
				WriteLog("ViewPTRun::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewPTRunConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewPTRun::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B810399
BOOL CViewPTRun::SaveListConfig()
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
			WriteLog("ViewPTRun::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewPTRun::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewPTRunConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewPTRunConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewPTRunConfig");
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
						WriteLog("δ�ҵ�ViewPTRunConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewPTRunConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B81037A
int CViewPTRun::FillListData()
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
	m_cboxShowPTRunON.SetCheck(m_bShowPTRunON);
	m_cboxShowPTRunOFF.SetCheck(m_bShowPTRunOFF);
	m_cboxShowPTRunCheck.SetCheck(m_bShowPTRunCheck);
	m_cboxShowPTRunDebug.SetCheck(m_bShowPTRunDebug);
	m_cboxShowPTRunCode.SetCheck(m_bShowPTRunCode);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8103A9
BOOL CViewPTRun::NeedSave()
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
//##ModelId=49B87B8103B9
void CViewPTRun::RegulateControlSize()
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
		m_cboxShowPTRunON.ShowWindow(FALSE);
		m_cboxShowPTRunOFF.ShowWindow(FALSE);
		m_cboxShowPTRunDebug.ShowWindow(FALSE);
		m_cboxShowPTRunCode.ShowWindow(FALSE);
		m_cboxShowPTRunCheck.ShowWindow(FALSE);
		//	m_cboxShowDebug.ShowWindow(FALSE);
		//	m_cboxShowReturn.ShowWindow(FALSE);
		//	m_cboxShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B8200CB
void CViewPTRun::OnDestroy() 
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
//##ModelId=49B87B8103C8
BOOL CViewPTRun::LoadDataFromDB(int num, BOOL IsQueryAcked)
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewPTRun::LoadDataFromDB, ��ѯʧ��", XJ_LOG_LV3);
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
			str.Format("CViewPTRun::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventPTRun * pEvent = new CXJEventPTRun;
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
		str.Format("CViewPTRun::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 ���ܸ�Ҫ����ָ��װ�õ�����״̬���뵽�б�����ʾ	
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ�����װ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
//##ModelId=49B87B8103D8
BOOL CViewPTRun::AddEventToList(CSecObj * pSec, int i)
{
	//�ж�������Ч��
	if(pSec == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	if(!IsNeedShow(pSec->m_nRunStatu))
		return FALSE;
	//ˢ�³���ʱ��
	pSec->RefreshRunDurativeTime();
	
	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸", "�������ܵ�λ", "��ά��λ"

	
	//0:�豸����
	CString str;
	str = pSec->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	//1:״̬
	m_List.SetItemText(i, 1, GetRunStatusString(pSec->m_nRunStatu));
	//2:״̬�ı�ʱ��
	m_List.SetItemText(i, 2, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
	//3:״̬����ʱ��
	m_List.SetItemText(i, 3, pSec->m_sRunDurativeTime);
	//4:��վ��
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sName;
	}
	m_List.SetItemText(i, 4, str);
	//5:һ���豸
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
 �� �� ����OnSTTP20079()
 ���ܸ�Ҫ����Ӧ20079()����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B82016B
void CViewPTRun::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewPTRun::OnSTTP20079 start", XJ_LOG_LV3);
	//ȡ���豸ָ��
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj != NULL)
	{
		//�ı���ͨ��״̬�б��е���ʾ
 		ChangeStatusInList(pObj);
	}
	if (pObj->m_sStationID != m_sChooseStationID&& !m_sChooseStationID.IsEmpty())
	{
		CString str;
		str.Format("�յ���վIDΪ%s���ģ����ǵ�ǰѡ���ĳ�վ���ѹ��ˡ�",pObj->m_sStationID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	if(pObj != NULL)
	{
		CString str;
		str.Format("װ��ID:%s,����:%s,��վID:%s,��ǰ״̬:%d", pObj->m_sID, pObj->m_sName, pObj->m_sStationID,pObj->m_nRunStatu);
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
			WriteLog("CViewPTRun::OnSTTP20079, ��˸�豸", XJ_LOG_LV3);
			//��˸
			//pView->AddBlinkDevice(pObj);
			pView->AddToBlinkList(pObj);
		}
	}	

	WriteLog("CViewPTRun::OnSTTP20079 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ�����÷�����״̬�±���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8200DA
void CViewPTRun::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					if(nSubItem == 1)
					{
						//�õ��¼�����
						CSecObj * pEvent = (CSecObj*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						switch(pEvent->m_nRunStatu)
						{
						case 0: //����
							lplvcd->clrText = XJ_STATUS_RUN_CHECK;
							break;
						case 1: //ͣ��
							lplvcd->clrText = XJ_STATUS_RUN_OFF;
							break;
							// 					case 2: //Ͷ��
							// 						lplvcd->clrTextBk = XJ_STATUS_RUN_ON;
							// 						break;
						case 3: //δ����
							lplvcd->clrText = XJ_STATUS_RUN_CODE;
							break;
						case 4: //����
							lplvcd->clrText = XJ_STATUS_RUN_DEBUG;
							break;
						default:
							break;
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

//##ModelId=49B87B8200EB
void CViewPTRun::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_PTRUN))
				return;

			Menu.DeleteMenu(IDC_EVENT_DEVICE, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_PTRUN_REFRESH, MF_BYCOMMAND);
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
			}
			//����
			if(1 == g_ShowPTRunON)
			{
				//��ʾͨ��״̬Ϊ���е�װ����Ϣ
				pMenu->CheckMenuItem(IDC_PTRUN_ON, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTRUN_ON, MF_UNCHECKED);
			}
			//ͣ��
			if(1 == g_ShowPTRunOFF)
			{
				pMenu->CheckMenuItem(IDC_PTRUN_OFF, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTRUN_OFF, MF_UNCHECKED);
			}
			//����
			if(1 == g_ShowPTRunCheck)
			{
				pMenu->CheckMenuItem(IDC_PTRUN_CHECK, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTRUN_CHECK, MF_UNCHECKED);
			}
			//����
			if(1 == g_ShowPTRunDebug)
			{
				pMenu->CheckMenuItem(IDC_PTRUN_DEBUG, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTRUN_DEBUG, MF_UNCHECKED);
			}
			//�����
			if(1 == g_ShowPTRunCode)
			{
				pMenu->CheckMenuItem(IDC_PTRUN_CODE, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTRUN_CODE, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B8200FA
void CViewPTRun::OnEventDevice() 
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

//##ModelId=49B87B8200FC
void CViewPTRun::OnEventSec() 
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

//##ModelId=49B87B820109
void CViewPTRun::OnClickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
// 	�õ���ѡ��
// 		NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
// 		if(nSelected > -1)
// 		{
// 			//������¼�, ȷ�ϴ��¼�
// 			//�õ����ж�Ӧ���¼�
// 			CXJEventPTRun * pEvent = (CXJEventPTRun*)m_List.GetItemData(nSelected);
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

//##ModelId=49B87B82010D
void CViewPTRun::OnEventSecProp() 
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
//##ModelId=49B87B81034B
int CALLBACK CViewPTRun::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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
		iResult = pObj1->m_nRunStatu - pObj2->m_nRunStatu;
		break;
	case 2: //״̬�ı�ʱ��
		str1.Format("%s", pObj1->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //״̬����ʱ��
		iResult = pObj1->m_lRunDurative - pObj2->m_lRunDurative;
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
	iResult *= g_iPTRunViewAsc;
	return iResult;
}

//##ModelId=49B87B820119
void CViewPTRun::OnColumnclickLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTRunViewAsc = - g_iPTRunViewAsc;
	}
	else
	{
		g_iPTRunViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B82011D
void CViewPTRun::OnDblclkLISTPTRun(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
	}
	*pResult = 0;
}

//##ModelId=49B87B82012A
BOOL CViewPTRun::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTRun);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����IsNeedShow()
 ���ܸ�Ҫ���жϵ�ǰ����״̬�ı�����Ϣ�Ƿ���Ҫ��ʾ
 �� �� ֵ: ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 ��    ����param1	����״̬
		   Param2
**************************************************************/
//##ModelId=49B87B820002
BOOL CViewPTRun::IsNeedShow( int nStatus )
{
	BOOL bReturn = FALSE;
	switch(nStatus)
	{
	case 0: //����
		if(1 == g_ShowPTRunCheck)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 1: //ͣ��
		if(1 == g_ShowPTRunOFF)
			bReturn = TRUE;
		else 
			bReturn = FALSE;
		break;
	case 2: //Ͷ��
		if(1 == g_ShowPTRunON)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 3: //δ����
		if(1 == g_ShowPTRunCode)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 4: //����(�����)
		if(1 == g_ShowPTRunDebug)
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

/*************************************************************
 �� �� ����ChangeStatusInList()
 ���ܸ�Ҫ���ı�ָ�������豸������״̬��ͨ��״̬�б���е���ʾ
 �� �� ֵ: 
 ��    ����param1	ָ�������豸
		   Param2
**************************************************************/
//##ModelId=49B87B82000F
void CViewPTRun::ChangeStatusInList( CSecObj* pSec )
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
			if (pSec->m_sStationID == m_sChooseStationID || m_sChooseStationID.IsEmpty())
			{
				//����
				AddEventToList(pSec, 0);
			}
			
		}
		else
		{
			//�ı�ֵ
			//1:״̬
			m_List.SetItemText(nIndex, 1, GetRunStatusString(pSec->m_nRunStatu));
			//2:״̬�ı�ʱ��
			m_List.SetItemText(nIndex, 2, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
			//3:״̬����ʱ��
			m_List.SetItemText(nIndex, 3, pSec->m_sRunDurativeTime);
			//ˢ��
			m_List.Update(nIndex);
		}
	}
}

//##ModelId=49B87B820138
void CViewPTRun::OnTimer(UINT nIDEvent) 
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
 ���ܸ�Ҫ��ˢ�����б���������״̬�ĳ���ʱ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B820013
void CViewPTRun::RefreshTime()
{
	WriteLog("CVievPTRun::RefreshTime,��ʱˢ�¿�ʼ", XJ_LOG_LV3);
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
	WriteLog("CViewPTRun::RefreshTime,��ʱˢ�½���", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����RefreshDuravieTime()
 ���ܸ�Ҫ��ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ
 �� �� ֵ: 
 ��    ����param1	ָ����LIST�е�λ��
		   Param2
**************************************************************/
//##ModelId=49B87B820011
void CViewPTRun::RefreshDuravieTime( int nIndex )
{
	//����
	int nCount = m_List.GetItemCount();
	//�ж�������Ч��
	if(nIndex < 0)
		return;
	if(nIndex >= nCount)
		return;
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	MYASSERT(pObj);
	//ˢ�³���ʱ��
	pObj->RefreshRunDurativeTime();
	//������ʾ
	m_List.SetItemText(nIndex, 3, pObj->m_sRunDurativeTime);
	//ˢ��
	m_List.Update(nIndex);
}

//##ModelId=49B87B82013B
void CViewPTRun::OnPtrunOn() 
{
	// TODO: Add your command handler code here
	//�ı�����״̬�����Ƿ�Ҫ��ʾ������
	g_ShowPTRunON = !g_ShowPTRunON;
	m_bShowPTRunON = g_ShowPTRunON;
		//�����������
		FillListData();
}

//##ModelId=49B87B82013D
void CViewPTRun::OnPtrunOff() 
{
	// TODO: Add your command handler code here
	//�ı�����״̬ͣ���Ƿ�Ҫ��ʾ������
	g_ShowPTRunOFF = !g_ShowPTRunOFF;
	m_bShowPTRunOFF = g_ShowPTRunOFF;
		//�����������
		FillListData();
}

//##ModelId=49B87B820148
void CViewPTRun::OnPtrunCheck() 
{
	// TODO: Add your command handler code here
	//�ı�����״̬�����Ƿ�Ҫ��ʾ������
	g_ShowPTRunCheck = !g_ShowPTRunCheck;
	m_bShowPTRunCheck = g_ShowPTRunCheck;
		//�����������
		FillListData();
}

//##ModelId=49B87B82014A
void CViewPTRun::OnPtrunDebug() 
{
	// TODO: Add your command handler code here
	//�ı�����״̬�����Ƿ�Ҫ��ʾ������
	g_ShowPTRunDebug = !g_ShowPTRunDebug;
	m_bShowPTRunDebug = g_ShowPTRunDebug;
		//�����������
		FillListData();
}

//##ModelId=49B87B82014C
void CViewPTRun::OnPtrunCode() 
{
	// TODO: Add your command handler code here
	//�ı�����״̬������Ƿ�Ҫ��ʾ������
	g_ShowPTRunCode = !g_ShowPTRunCode;
	m_bShowPTRunCode = g_ShowPTRunCode;
		//�����������
		FillListData();
}

//##ModelId=49B87B820158
void CViewPTRun::OnPtrunHistory() 
{
	// TODO: Add your command handler code here
	//׼���򿪲�ѯ��ͳ�ƵĲ���
	//��������״̬��ѯ: 3$$$��վID$$$�豸����$$$�豸ID $$$����״̬$$$�Զ���ѯ(0-���Զ���ѯ, 1-�Զ���ѯ)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount < 1)
	{
		sSQL = "3$$$ $$$99$$$ $$$99$$$0";
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

//##ModelId=49B87B820167
void CViewPTRun::OnPtrunPrint() 
{
	// TODO: Add your command handler code here
	
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTRUN_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTRUN_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTRun);
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
//##ModelId=49B87B82001F
BOOL CViewPTRun::WriteResultToExcel( CString strFullPath )
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
					StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_COMMU_STATUS), StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE),
					StringFromID(IDS_MODEL_OVERHUALUNIT));
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CSecObj* pObj = (CSecObj*)m_List.GetItemData(i);
				AddResultToExcel(&database, pObj);
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
//##ModelId=49B87B820021
void CViewPTRun::AddResultToExcel( CDatabase* pDatabase, CSecObj* pSec )
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

	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸", "�������ܵ�λ", "��ά��λ"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s','%s','%s','%s', '%s')",
		pSec->m_sName, GetRunStatusString(pSec->m_nRunStatu), pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sRunDurativeTime, strStation, strDevice,strManagedepartment,strMaintaindepartment,strServiceDepartment);
	pDatabase->ExecuteSQL(strSQL);
}

//##ModelId=49B87B820169
void CViewPTRun::OnPtrunExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s.xls", StringFromID(IDS_TOOLBAR_DEVICERUN),t.Format("%Y%m%d%H%M%S"));
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

/*************************************************************
�� �� ����InitCOM()
���ܸ�Ҫ����ʼ����ӡģ��COM���
�� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
��    ����param1
Param2
**************************************************************/
//##ModelId=49B87B82002E
BOOL CViewPTRun::InitCOM()
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
//##ModelId=49B87B82002F
void CViewPTRun::ReleaseCOM()
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
//##ModelId=49B87B81035B
CString CViewPTRun::TranslateSingleKey( CString sKey )
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
		if(strTail == "DEVICE_RUN$")
		{
			//����״̬
			return GetRunStatusString(pObj->m_nRunStatu);
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

void CViewPTRun::OnPtrunRefresh() 
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
void CViewPTRun::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//������������豸״̬�������
	//��������б��
	FillListData();
	WriteLog("CViewPTRun::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}
/*************************************************************
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewPTRun::InitComBox()
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


void CViewPTRun::OnSelchangeComboChoosestatioin() 
{
	// TODO: Add your control notification handler code here
	//��ȡ��ǰѡ����
	
	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATIOIN))->GetCurSel();
	
		CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)	
	{
		CString sStationID=	pStationid->m_sID;
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTRun::OnCheckShowptrunon() 
{
	// TODO: Add your control notification handler code here
	m_bShowPTRunON = !m_bShowPTRunON;
	g_ShowPTRunON = !g_ShowPTRunON;
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTRun::OnCheckShowptrunoff() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTRunOFF = !g_ShowPTRunOFF;
	m_bShowPTRunOFF = !m_bShowPTRunOFF;
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTRun::OnCheckShowptruncheck() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTRunCheck = !g_ShowPTRunCheck;
	m_bShowPTRunCheck = !m_bShowPTRunCheck;
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTRun::OnCheckShowptrundebug() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTRunDebug = !g_ShowPTRunDebug;
	m_bShowPTRunDebug = !m_bShowPTRunDebug;
	//ˢ���б���Ϣ
	FillListData();
}

void CViewPTRun::OnCheckShowptruncode() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTRunCode = !g_ShowPTRunCode;
	m_bShowPTRunCode = !m_bShowPTRunCode;
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
void CViewPTRun::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	//ˢ���б���Ϣ
	InitComBox();//�������볧վѡ��������
	FillListData();
	WriteLog("StationInit,�����豸����״̬ҳ��ˢ�����", XJ_LOG_LV3);
}
