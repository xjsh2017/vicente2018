// ViewFault.cpp : implementation file
//
#include "ExcelOp.h"
#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewFault.h"
#include "DlgFaultReport.h"
#include "GlobalFunc.h"
#include "ManualFaultShorten.h"
#include "RemindObj.h"

//#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iFaultViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CViewFault

IMPLEMENT_DYNCREATE(CViewFault, CFormView)

//##ModelId=49B87B84033E
CViewFault::CViewFault()
	: CFormView(CViewFault::IDD)
{
	//{{AFX_DATA_INIT(CViewFault)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_FAULT;
	m_listFaultReport.RemoveAll();
	m_nMaxRow = 100;
	/** @brief           ֮ǰ���������*/
	m_nOldSortCol = -1;

	m_bOnlyShowNoOverhual = FALSE;
	m_bOnlyShowSaved = FALSE;
	m_bOnlyShowTypical = FALSE;
}

//##ModelId=49B87B85005D
CViewFault::~CViewFault()
{
	Clear();
	TRACE("CViewFault::~CViewFault \n");
}

//##ModelId=49B87B85002F
void CViewFault::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewFault)
	DDX_Control(pDX, IDC_LIST_FAULT_NEW, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewFault, CFormView)
	//{{AFX_MSG_MAP(CViewFault)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FAULT_NEW, OnItemchangedListFaultNew)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FAULT_NEW, OnColumnclickListFaultNew)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FAULT_NEW, OnClickListFaultNew)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_FAULT_PRINT, OnFaultPrint)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_FAULT_EDIT, OnFaultEdit)
	ON_COMMAND(IDC_FAULT_WEB, OnFaultWeb)
	ON_COMMAND(IDC_FAULT_REFRESH, OnFaultRefresh)
	ON_COMMAND(IDC_FAULT_CREATE, OnFaultCreate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FAULT_NEW, OnCustomDraw)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDC_FAULT_DELETE, OnFaultDelete)
	ON_COMMAND(IDC_FAULT_EXPORT, OnFaultExport)
	ON_COMMAND(ID_MANUAL_FAULTSHORTENN, OnManualFaultShorten)
	ON_COMMAND(IDC_FAULT_ONLYSHOWOVERHAUL, OnFaultOnlyshowoverhaul)
	ON_COMMAND(IDC_FAULT_ONLYSHOWSAVED, OnFaultOnlyshowsaved)
	ON_COMMAND(IDC_FAULT_ONLYSHOWTYPICAL, OnFaultOnlyshowtypical)
	//}}AFX_MSG_MAP
	ON_MESSAGE(FAULTREPORT_EDITED, OnFaultReportEdited)
	ON_MESSAGE(WM_STTP_20151, OnSTTP20151)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFault diagnostics

#ifdef _DEBUG
//##ModelId=49B87B85006D
void CViewFault::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B85007D
void CViewFault::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewFault message handlers

//##ModelId=49B87B85004E
void CViewFault::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_FAULTREPORT));
//	GetParentFrame()->SetWindowText("�¹ʱ���");
	//CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	//pWnd->SendMessage(WM_PAINTMYCAPTION, NULL, NULL);
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B850024
void CViewFault::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	LoadConfig();

	//��ʼ���ؼ�
	InitListCtrl();
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B84038A
int CViewFault::InitListCtrl()
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
//##ModelId=49B87B84038B
int CViewFault::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
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

			if ( sName == StringFromID(IDS_FAULTREPORT_ISTRUEFALUT))
			{
				g_Temp = 1;
			}

			m_List.InsertColumn(i,&lvCol, bHide);
		}
	}
	else
	{
		//��������ʧ��
		//char* arColumn[14]={"���", "�¹ʰ�����","����ʱ��","����ʱ��","����Ԫ��","��վ1","��վ2","��������","¼�����","�Ƿ����", "�Ƿ��Ѵ浵","���͹���","��ϸԭ��",""};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 14 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 240; // �¹ʰ�����
				colName = StringFromID(IDS_FAULTREPORT_NAME);
				break;
			case 2:
				lvCol.cx = 180; //����ʱ��
				colName = StringFromID(IDS_COMMON_CREATETIME);
				break;
			case 3://
				lvCol.cx = 180; // ����ʱ��
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4://
				lvCol.cx = 150; // ����Ԫ��
				colName = StringFromID(IDS_FAULT_DEVICE);
				break;
			case 5://
				lvCol.cx = 100; // ��վ1
				colName = StringFromID(IDS_FAULT_STATION1);
				break;
			case 6:
				lvCol.cx = 100; //��վ2
				colName = StringFromID(IDS_FAULT_STATION2);
				break;
			case 7:
				lvCol.cx = 100; //��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 8:
				lvCol.cx = 80; //���ϲ��
				colName = StringFromID(IDS_FAULT_LOCATION);
				break;
			case 9:
				lvCol.cx = 120; //�Ƿ����
				colName = StringFromID(IDS_FAULTREPORT_ISOVERHAUL);
				break;
			case 10:
				lvCol.cx = 90; //�Ƿ��Ѵ浵
				colName = StringFromID(IDS_FAULTREPORT_ISSAVED);
				break;
			case 11:
				lvCol.cx = 90; //���͹���
				colName = StringFromID(IDS_FAULTREPORT_ISTYPICAL);
				break;
			case 12:
				lvCol.cx = 90; //��ϸԭ��
				colName = StringFromID(IDS_COMMON_DETAILREASON);
				break;
			case 13:
				lvCol.cx = 90; //""
				colName = "";
				break;
			default:
				lvCol.cx=90;//�п�100����
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(13, TRUE);
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
//##ModelId=49B87B8403A9
BOOL CViewFault::LoadListConfig()
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
			WriteLog("ViewFault::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewFault::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewFaultConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewFaultConfig");
				WriteLog("ViewFault::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewFaultConfig�µ�����");
					WriteLog("ViewFault::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

//##ModelId=49B87B8403B9
BOOL CViewFault::SaveListConfig()
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
			WriteLog("ViewFault::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewFault::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewFaultConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewFaultConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewFaultConfig");
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
						WriteLog("δ�ҵ�ViewFaultConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewFaultConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B840399
int CViewFault::FillListData()
{

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	POSITION pos = m_listFaultReport.GetHeadPosition();
	int nIndex = 0;
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		AddReportToList(pReport, nIndex);
		nIndex++;
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8403C8
BOOL CViewFault::NeedSave()
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
//##ModelId=49B87B8403C9
void CViewFault::RegulateControlSize()
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

//##ModelId=49B87B85008C
void CViewFault::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//�����ؼ�λ��
	RegulateControlSize();
}

//##ModelId=49B87B85009C
void CViewFault::OnDestroy() 
{
	SaveConfig();
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����LoadDataFromDB()
 ���ܸ�Ҫ�������ݿ�����������
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ����ļ�¼��
		   Param2
**************************************************************/
//##ModelId=49B87B8403D8
BOOL CViewFault::LoadDataFromDB( int nNum )
{
	//���������
	Clear();

	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��ѯ�����nNum����¼
	CString str;		
		
	//top
	Field Field1;
	str.Format("%d", nNum);
	pApp->m_DBEngine.SetField(sql, Field1, str, EX_STTP_DATA_TYPE_TOP);
	//����

	if(m_bOnlyShowNoOverhual)
	{
		Condition condition2;
		CString strFiter = "overhaul_flag = 0";
		pApp->m_DBEngine.SetCondition(sql, condition2, strFiter);
	}
	if(m_bOnlyShowSaved)
	{
		Condition condition3;
		CString strFiter = "reverse2 > 0";
		pApp->m_DBEngine.SetCondition(sql, condition3, strFiter);
	}
	if(m_bOnlyShowTypical)
	{
		Condition condition4;
		CString strFiter = "typicalcase = 1";
		pApp->m_DBEngine.SetCondition(sql, condition4, strFiter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("((station1_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID,pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	Condition Condition1;
	str.Format("order by ID DESC");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewFault::LoadDataFromDB, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CFaultReport* pReport = new CFaultReport();
			if(!pReport->LoadFromDB(pMemset))
			{
				delete pReport;
				pReport = NULL;
			}
			else
			{
				//���뵽����
				m_listFaultReport.AddTail(pReport);
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewFault::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Clear()
 ���ܸ�Ҫ�����֮ǰ�Ĺ��ϱ�����Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8403DA
void CViewFault::Clear()
{
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pReport != NULL)
			delete pReport;
	}
	m_listFaultReport.RemoveAll();
}

/*************************************************************
 �� �� ����AddReportToList()
 ���ܸ�Ҫ������ָ���Ĺ��ϱ��浽�б������ʾ
 �� �� ֵ: void
 ��    ����param1	ָ�����ϱ���
		   Param2	ָ��λ��
**************************************************************/
//##ModelId=49B87B850000
void CViewFault::AddReportToList( CFaultReport* pReport, int nIndex )
{
	//���������Ч��
	if(pReport == NULL)
		return;
	if(nIndex < 0)
		return;
	//"���", "�¹ʰ�����", "����ʱ��","����Ԫ��","��վ1","��վ2","��������","���ϲ��"
	CString str;
	//0:���
	str.Format("%d", pReport->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:�¹ʰ�����
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:����ʱ��
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:����ʱ��
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:����Ԫ��
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:��վ1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:��վ2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:��������
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	//8:���ϲ��
	str = "";
	if( !pReport->m_sFaultDistance.IsEmpty() )
	{
		str.Format("%s km", pReport->m_sFaultDistance);
	}
	m_List.SetItemText(nIndex, 8, str);
	//9:�Ƿ����
	m_List.SetItemText(nIndex, 9, pReport->m_nOverhaul == 0?StringFromID(IDS_OVERHUAL_NO):StringFromID(IDS_OVERHUAL_YES));
	//10:�Ƿ�浵
	m_List.SetItemText(nIndex, 10, pReport->m_bIsSaved?StringFromID(IDS_COMMON_YES):StringFromID(IDS_COMMON_NO));
	//11:�Ƿ����
	m_List.SetItemText(nIndex, 11, pReport->m_nTypicalcase == 0?StringFromID(IDS_COMMON_NO):StringFromID(IDS_COMMON_YES));
	
    m_List.SetItemText(nIndex, 12, StringFromID(IDS_COMMON_EDIT));
	m_List.SetItemText(nIndex, 13, StringFromID(IDS_COMMON_PLAYBACK));
	//������DATA
	m_List.SetItemData(nIndex, (DWORD)pReport);
}

//##ModelId=49B87B85009E
void CViewFault::OnItemchangedListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//AfxMessageBox("item change");
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//��ʵ���������ѡ����, ���������ITEMCHANGED��Ϣ, 
		//1��A�����ֱ�����ʧ������2��Aȡ��ѡ�д�����3��B��ѡ�д���
		
		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
		if(nSelected < 0)
			return;
		
		//�õ����ж�Ӧ���¼�
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nSelected);
		if(pReport == NULL)
			return;

		//������Ϣ����ϸ��ʾ����
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd != NULL)
		{
			pWnd->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)pReport);
		}
	}
	*pResult = 0;
	
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��.1, 0, -1
 ��    ����param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   param3 �Ƚϲ���
**************************************************************/
//##ModelId=49B87B84037A
int CALLBACK CViewFault::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ��Ҫ�Ƚϵ�����
	CFaultReport* pReport1 = (CFaultReport*)lParam1;
	CFaultReport* pReport2 = (CFaultReport*)lParam2;
	if(pReport1 == NULL || pReport2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	float fTemp1;
	float fTemp2;
	int iResult = 0;
	//"���", "�¹ʰ�����", "����ʱ��","����Ԫ��","��վ1","��վ2","��������","���ϲ��"
	switch(nCol)
	{
	case 0: //ID
		iResult = pReport1->m_nID - pReport2->m_nID;
		break;
	case 1: //�¹ʰ�����
		str1 = pReport1->m_sName;
		str2 = pReport2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //����ʱ��
		str1 = pReport1->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pReport2->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //����ʱ��
		str1 = pReport1->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pReport2->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4:	//����Ԫ��
		str1 = "";
		if(pReport1->m_pPriDevice != NULL)
			str1 = pReport1->m_pPriDevice->m_sName;
		str2 = "";
		if(pReport2->m_pPriDevice != NULL)
			str2 = pReport2->m_pPriDevice->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //��վ1
		str1 = "";
		if(pReport1->m_pStation1 != NULL)
			str1 = pReport1->m_pStation1->m_sName;
		str2 = "";
		if(pReport2->m_pStation1 != NULL)
			str2 = pReport2->m_pStation1->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //��վ2
		str1 = "";
		if(pReport1->m_pStation2 != NULL)
			str1 = pReport1->m_pStation2->m_sName;
		str2 = "";
		if(pReport2->m_pStation2 != NULL)
			str2 = pReport2->m_pStation2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //��������
		str1 = pReport1->GetFaultType();
		str2 = pReport2->GetFaultType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8: //���ϲ��
		fTemp1 = atof(pReport1->m_sFaultDistance);
		fTemp2 = atof(pReport2->m_sFaultDistance);
		if(fTemp1 - fTemp2 < -0.00001)
			iResult = -1;
		else if(fTemp1 - fTemp2 > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 9://�Ƿ����
		iResult = pReport1->m_nOverhaul - pReport2->m_nOverhaul;
		break;
	case 10://�Ƿ�浵
		iResult = pReport1->m_bIsSaved - pReport2->m_bIsSaved;
		break;
	case 11://�Ƿ����
		iResult = pReport1->m_nTypicalcase - pReport2->m_nTypicalcase;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iFaultViewAsc;
	return iResult;
}

/*************************************************************
 �� �� ����OnColumnclickListDi()
 ���ܸ�Ҫ����Ӧ�����ͷ�¼�, �Ե�����н�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8500AB
void CViewFault::OnColumnclickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	AfxMessageBox("��Ӧ�����ͷ�¼�, �Ե�����н�������");
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iFaultViewAsc = - g_iFaultViewAsc;
	}
	else
	{
		g_iFaultViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnClickListFaultNew()
 ���ܸ�Ҫ����Ӧ����б��¼�, ��ѡ��Ϊ��ʱ������Ϣ����ϸ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8500BB
void CViewFault::OnClickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
{

	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	int nSubItem = pNMListView->iSubItem;

	if(nSelected < 0)
	{
		//�������Ƿָ��
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ�����Ϣ, ѡ���˿���,���Ϳ�ָ��
		pWnd ->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)NULL);
		return;
	}
	if (nSubItem == 9)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?",StringFromID(IDS_TIP_FAULTREPORT_SWITCH),(pFaultReport->m_nOverhaul == 0 ? StringFromID(IDS_OVERHUAL_YES):StringFromID(IDS_OVERHUAL_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{			
			if (pFaultReport->m_nOverhaul == 1)
			{
				pFaultReport->m_nOverhaul = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_OVERHUAL_NO));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 1;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
			else
			{
				pFaultReport->m_nOverhaul = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_OVERHUAL_YES));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 0;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
		}
	}
	if (nSubItem == 10)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?", StringFromID(IDS_TIP_FAULTREPORT_SWITCH),(pFaultReport->m_bIsSaved == 0 ? StringFromID(IDS_ARCHIVE_YES):StringFromID(IDS_ARCHIVE_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{
			CString strTemp;
			m_List.GetItemText(nSelected, nSubItem, strTemp.GetBuffer(2), 2);
           
			if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			{
				pFaultReport->m_bIsSaved = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_NO));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 1;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
			else
			{
				pFaultReport->m_bIsSaved = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_YES));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 0;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
		}
	}
	if (nSubItem == 11)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?",(pFaultReport->m_nTypicalcase == 0 ? StringFromID(IDS_TYPICAL_YES):StringFromID(IDS_TYPICAL_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{
			CString strTemp;
			m_List.GetItemText(nSelected, nSubItem, strTemp.GetBuffer(2), 2);
			
			if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			{
				pFaultReport->m_nTypicalcase = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_NO));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_nTypicalcase = 1;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
			else
			{
				pFaultReport->m_nTypicalcase = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_YES));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_nTypicalcase = 0;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
		}
	}
	if (nSubItem == 12)
	{
		int nCount = m_List.GetSelectedCount();
		if(nCount != 1)
			return;
		int nIndex = -1;
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex == -1)
			return;
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
		if(pReport == NULL)
			return;
		CDlgFaultReport dlg;
		LV_COLUMN lvCol;
		lvCol.mask   =   LVCF_TEXT;
		char temp[256];
		for(int j = 0; j < 256; j++)
		{
			temp[j] = '\0';
		}
		lvCol.cchTextMax = 256;
		lvCol.pszText = temp;
		m_List.GetColumn(9, &lvCol);
		dlg.m_sTitle = lvCol.pszText;
		dlg.m_pWnd = this;
		dlg.m_pFaultReport = pReport;
		dlg.m_nCurTabSel = 5;
		dlg.DoModal();
	}
	if(nSubItem == 13)
	{
		//���ϻط�
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString sPath = pFaultReport->GetFisPath();
		if(!sPath.IsEmpty())
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString sDir = pApp->GetDownDir();
			CString sFullPath = sDir + sPath;
			OpenXJFaultInversion(sFullPath, GetSafeHwnd());
			CString sLog;
			sLog.Format("ִ�й��ϻطŽű�,�ű�·��:%s", sFullPath);
			WriteLog(sLog);
		}
		else
			AfxMessageBox(StringFromID(IDS_TIP_FAULTREPORT_NO_PLAYBACK_SCRIPT));
	}
	*pResult = 0;
	
}

//##ModelId=49B87B8500BF
void CViewFault::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_FAULTREPORT))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_FAULT_EDIT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_PRINT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_WEB, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_EXPORT, MF_ENABLED);
			}
			pMenu->EnableMenuItem(IDC_FAULT_DELETE, MF_ENABLED);
			pMenu->EnableMenuItem(IDC_FAULT_REFRESH, MF_ENABLED);
			pMenu->EnableMenuItem(IDC_FAULT_CREATE, MF_ENABLED);
			pMenu->EnableMenuItem(ID_MANUAL_FAULTSHORTENN, MF_ENABLED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWOVERHAUL, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWOVERHAUL, m_bOnlyShowNoOverhual?MF_CHECKED:MF_UNCHECKED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWSAVED, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWSAVED, m_bOnlyShowSaved?MF_CHECKED:MF_UNCHECKED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWTYPICAL, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWTYPICAL, m_bOnlyShowTypical?MF_CHECKED:MF_UNCHECKED);
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B8500CB
void CViewFault::OnFaultPrint() 
{
	// TODO: Add your command handler code here
	//�������Ƿָ��
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//�򸸴��ڷ�����Ϣ
	pWnd ->PostMessage(FAULTREPORT_PRINT, 0, 0);
}

//##ModelId=49B87B8500CD
BOOL CViewFault::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_FAULT_NEW);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8500D0
void CViewFault::OnFaultEdit() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
	if(pReport == NULL)
		return;
	CDlgFaultReport dlg;
	LV_COLUMN lvCol;
	lvCol.mask   =   LVCF_TEXT;
	char temp[256];
	for(int j = 0; j < 256; j++)
	{
		temp[j] = '\0';
	}
	lvCol.cchTextMax = 256;
	lvCol.pszText = temp;
	m_List.GetColumn(9, &lvCol);
	dlg.m_sTitle = lvCol.pszText;
	dlg.m_pWnd = this;
	dlg.m_pFaultReport = pReport;
	dlg.DoModal();
}

/*************************************************************
 �� �� ����OnFaultReportChanged()
 ���ܸ�Ҫ����Ӧ���ϱ�����Ϣ���༭��Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2	���ϱ���ָ��
**************************************************************/
void CViewFault::OnFaultReportEdited( WPARAM wParam, LPARAM lParam )
{
	CFaultReport* pReport = (CFaultReport*)lParam;
	if(pReport != NULL)
	{
		//������Ϣ�����¼���������
		pReport->SortActionSign();
		ChangedValueInList(pReport);
		//������Ϣ����ϸ��ʾ����
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd != NULL)
		{
			pWnd->SendMessage(FAULTDETAIL_REFRESH, 0, (LPARAM)pReport);
		}
	}
}

/*************************************************************
 �� �� ����ChangedValueInList()
 ���ܸ�Ҫ���ı�ָ�����ϱ������б��е���ʾ
 �� �� ֵ: 
 ��    ����param1	ָ�����ϱ���
		   Param2	ָ������
**************************************************************/
//##ModelId=49B87B850003
void CViewFault::ChangedValueInList( CFaultReport* pReport, int nIndex /*= -1*/ )
{
	//���������Ч��
	MYASSERT(pReport);
	if(nIndex == -1)
	{
		//�������б��е�λ��
		//�����¼����б��е�λ��
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pReport;
		nIndex = m_List.FindItem(&lvFind);
	}
	if(nIndex < 0)
		return;
	//
	//1:�¹ʰ�����
	CString str;
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:����ʱ��
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:����ʱ��
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:����Ԫ��
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:��վ1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:��վ2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:��������
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	//8:���ϲ��
	m_List.SetItemText(nIndex, 8, pReport->m_sFaultDistance);
	//9:�Ƿ����
	m_List.SetItemText(nIndex, 9, pReport->m_nOverhaul == 0?StringFromID(IDS_OVERHUAL_NO):StringFromID(IDS_OVERHUAL_YES));
	//10:�Ƿ�浵
	m_List.SetItemText(nIndex, 10, pReport->m_bIsSaved?StringFromID(IDS_COMMON_YES):StringFromID(IDS_COMMON_NO));
	//11:�Ƿ����
	m_List.SetItemText(nIndex, 11, pReport->m_nTypicalcase == 0?StringFromID(IDS_COMMON_NO):StringFromID(IDS_COMMON_YES));
	//ˢ��
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����OnSTTP20151()
 ���ܸ�Ҫ����Ӧ20151,�Զ��鵵��Ϣ���仯֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8500E0
void CViewFault::OnSTTP20151( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewFault::OnSTTP20151 start");

	CFaultReport* pParam = (CFaultReport*)lParam;
	if(!pParam)
		return;

	//�ж������¼����Ǿ��¼����
	if(pParam->m_nRportType == 1 || (pParam->m_nRportType == 4 && !m_bOnlyShowNoOverhual))
	{
		//����Ϣ��
		//�������¼�
		CFaultReport* pReport = pParam->Clone();
		if(!pReport)
			return;
		if(pParam->m_nRportType == 4)
			pReport->m_nOverhaul = 1;
		//���뵽����
		m_listFaultReport.AddTail(pReport);
		//�������ʱ��ֹˢ��
		m_List.SetRedraw(FALSE);
		//���뵽��ʾ, ��һ��
		AddReportToList(pReport, 0);
		//�������
		CheckLineCount();
		m_List.SetRedraw(TRUE);

		CString str;
		str.Format("������Ϣ��, ��IDΪ%d", pReport->m_nID);
		WriteLog(str, XJ_LOG_LV3);

		CString sContent = "";
		sContent.Format("{%s [%s][%s][%s] %s}", 
			pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S"), 
			pReport->m_pStation1 == NULL?"":pReport->m_pStation1->m_sName, pReport->m_pStation2 == NULL?"":pReport->m_pStation2->m_sName, 
			pReport->m_pPriDevice == NULL?"":pReport->m_pPriDevice->m_sName, 
			pReport->m_sName);
		CRemindContent * pRC = new CRemindContent(StringFromID(IDS_TOOLBAR_FAULTREPORT), sContent);
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->GetMainWnd()->SendMessage(ADD_REMIND, 0, (LPARAM)pRC);
	}
	else if(pParam->m_nRportType == 2 || (pParam->m_nRportType == 5 && !m_bOnlyShowNoOverhual))
	{
		//��Ϣ���
		//�Ȳ����Ƿ����������ڴ���
		int nID = pParam->m_nID;
		CFaultReport* pReport = FindFaultReport(nID);
		if(pReport != NULL)
		{
			//���������ڴ���
			pReport->UpdateFrom(pParam);
			if(pParam->m_nRportType == 5)
				pReport->m_nOverhaul = 1;
			//�������б��е���ʾ
			//�����¼����б��е�λ��
			LVFINDINFO lvFind;
			lvFind.flags = LVFI_PARAM;
			lvFind.lParam = (LPARAM)pReport;
			int nIndex = m_List.FindItem(&lvFind);
			if(nIndex > -1)
			{
				//�������ʱ��ֹˢ��
				m_List.SetRedraw(FALSE);
				m_List.Update(nIndex);
				m_List.SetRedraw(TRUE);
			}
			
			CString str;
			str.Format("��Ϣ�����, ��IDΪ%d", pReport->m_nID);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		WriteLog("δ֪���ϰ�����");
		return;
	}

	WriteLog("CViewFault::OnSTTP20151 neat end");
}

/*************************************************************
 �� �� ����FindFaultReport()
 ���ܸ�Ҫ������ָ���Ĺ��ϱ����Ƿ������ڴ���
 �� �� ֵ: ���ҳɹ�����ָ��, ʧ�ܻ�δ�ҵ�����NULL
 ��    ����param1	ָ��ID
		   Param2
**************************************************************/
//##ModelId=49B87B850010
CFaultReport* CViewFault::FindFaultReport( int nID )
{
	//�ҵ��������е�λ��
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pTemp->m_nID == nID)
			return pTemp;
	}
	return NULL;
}

void CViewFault::DeleteFaultReport( CFaultReport* pReport )
{
	//�ҵ��������е�λ��
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetAt(pos);
		if(pTemp == pReport)
		{
			MYDELETE(pTemp);
			m_listFaultReport.RemoveAt(pos);
			return;
		}
		m_listFaultReport.GetNext(pos);
	}
}


/*************************************************************
 �� �� ����CheckLineCount()
 ���ܸ�Ҫ������б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B850012
void CViewFault::CheckLineCount()
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
		CFaultReport* pEvent = (CFaultReport*)m_List.GetItemData(nNewCount - 1);
		//���б���ɾ��
		m_List.DeleteItem(nNewCount - 1);
		//���¼�������ڴ���ɾ��
		RemoveFromMem(pEvent);
	}
}

/*************************************************************
 �� �� ����RemoveFromMem()
 ���ܸ�Ҫ����ָ�����ϱ�����ڴ��������ɾ��
 �� �� ֵ: void
 ��    ����param1	ָ�����ϱ���
		   Param2
**************************************************************/
//##ModelId=49B87B850013
void CViewFault::RemoveFromMem( CFaultReport* pReport )
{
	//���������Ч��
	if(pReport == NULL)
		return;
	//�ҵ��������е�λ��
	POSITION pos = m_listFaultReport.GetHeadPosition();
	POSITION posOld = pos;
	while(pos != NULL)
	{
		posOld = pos;
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pTemp == pReport)
			break;
	}
	//��������ɾ��
	if(posOld != NULL)
		m_listFaultReport.RemoveAt(posOld);
	//ɾ��ָ��
	delete pReport;
	pReport = NULL;
}

/*************************************************************
 �� �� ����RemoveFromList()
 ���ܸ�Ҫ����ָ�����ϱ�����б����ɾ��
 �� �� ֵ: void
 ��    ����param1	ָ�����ϱ���
		   Param2
**************************************************************/
//##ModelId=49B87B850020
void CViewFault::RemoveFromList( CFaultReport* pReport )
{
	//�����Ч��
	if(pReport == NULL)
		return;

	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pReport;
	int nIndex = m_List.FindItem(&lvFind);

	//δ�ҵ�
	if(nIndex < 0)
		return;

	//�Ƴ�
	m_List.DeleteItem(nIndex);
}

//##ModelId=49B87B8500DA
void CViewFault::OnFaultWeb() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
			if(pReport != NULL)
			{
				int nResult = IssueToWeb(pReport);
				CString strResult;
				strResult.Format("%s %s -- %s\n", pReport->m_sName, StringFromID(IDS_PUBLICTO_WEB),
					nResult == 0?StringFromID(IDS_COMMON_SUCCESS):StringFromID(IDS_COMMON_FAILED));				
				str += strResult;
			}
		}
	}	
	AfxMessageBox(str);
}

/*************************************************************
 �� �� ����IssueToWeb()
 ���ܸ�Ҫ����ָ���Ĺ��ϰ�������WEB
 �� �� ֵ: �������. -1- ��Ч�Ĺ��ϰ�. 0-�ɹ�. 1-ͨѶ������δ����. 2-��֯����ʧ��. 3-���ͱ���ʧ��. 
 ��    ����param1	ָ��Ҫ�����Ĺ��ϰ�
		   Param2
**************************************************************/
//##ModelId=49B87B850022
int CViewFault::IssueToWeb( CFaultReport* pReport )
{
	if(pReport == NULL)
		return -1;
	//������Ϣ��ͨ�ŷ�����
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		CString str;
		str.Format("%s ���͵�WEB, %s", pReport->m_sName, STTP_DISCONNECT);
		WriteLog(str);
		return 1;
	}
	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	//��֯20151����
	if(!pApp->m_SttpEngine.BuildDataFor20151FaultPacket(sttpData, 3, pReport))
	{
		//��֯����ʧ��
		CString str;
		str.Format("%s ���͵�WEB, ��֯����ʧ��", pReport->m_sName);
		WriteLog(str);
		return 2;
	}
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		CString str;
		str.Format("%s ���͵�WEB, ���ͱ���ʧ��", pReport->m_sName);
		WriteLog(str);
		return 3;
	}

	return 0;
}

void CViewFault::OnFaultRefresh() 
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
void CViewFault::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewFault::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

void CViewFault::OnFaultCreate() 
{
	// TODO: �½����ϰ�
	//1.��ѯ���ݿ������µİ�ID
	int nID = QueryLatsetID();
	if(nID < 0)
	{
		WriteLog("δ��ѯ���Ϸ���ID", XJ_LOG_LV3);
		nID = 0;
		//return;
	}
	//2.�½���
	CFaultReport* pReport = CreateFaultReport(++nID);
	if(pReport == NULL)
		return;
	//4.���浽���ݿ�
	if(!AddReportToDB(pReport))
	{
		//����ʧ��
		AfxMessageBox(StringFromID(IDS_PACK_SAVEPACKAGE_FAIL));
		delete pReport;
		return;
	}
	//3.��ӵ������LIST��ʾ
	m_listFaultReport.AddTail(pReport);
	AddReportToList(pReport, 0); //��һ��
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: QueryLatsetID()
 ���ܸ�Ҫ: ��ѯ���µĹ��ϰ�ID
 �� �� ֵ: ���µĹ��ϰ�ID
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewFault::QueryLatsetID()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	int nReturn = -1;
	
	//��ѯ�����1����¼
	CString str;		
	
	//top 1
	Field Field1;
	str.Format("%d", 1);
	pApp->m_DBEngine.SetField(sql, Field1, str, EX_STTP_DATA_TYPE_TOP);
	//����
	Condition Condition1;
	str.Format("order by ID DESC");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewFault::QueryLatsetID, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return -1;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//ֻӦ�ò�ѯ��һ��
			CString strID = pMemset->GetValue(UINT(0));
			nReturn = atoi(strID);
		}
	}
	else
	{
		CString str;
		str.Format("CViewFault::QueryLatsetID,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return nReturn;
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: CreateFaultReport()
 ���ܸ�Ҫ: �½����ϰ�
 �� �� ֵ: �´����Ĺ��ϰ�
 ��    ��: param1 ��ID
		   Param2 
**************************************************************/
CFaultReport* CViewFault::CreateFaultReport(int nID)
{
	CFaultReport* pReport = new CFaultReport;
	pReport->m_nID = nID;
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s", StringFromID(IDS_FAULTREPORT_NEW),t.Format("%Y%m%d%H%M%S"));
	pReport->m_sName = strName;
	pReport->m_tmCreateTime = t;
	pReport->m_tmFaultTime = t;

	return pReport;
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: AddReportToDB()
 ���ܸ�Ҫ: �����ϰ����뵽���ݿ�,�¼�������
 �� �� ֵ: ����ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����ϰ�
		   Param2 
**************************************************************/
BOOL CViewFault::AddReportToDB( CFaultReport* pReport )
{
	BOOL bReturn = TRUE;

	if(pReport == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��ѯ�����nNum����¼
	CString str;		
	
	//top
	//�ֶ�
	//ID
	Field Field1;
	str.Format("%d", pReport->m_nID);
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT, str);

	//Name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "CaseName", EX_STTP_DATA_TYPE_STRING, pReport->m_sName);
	
	//Name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "FaultMsTime", EX_STTP_DATA_TYPE_INT, "0");


	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "reverse4", EX_STTP_DATA_TYPE_STRING, pReport->m_tmCreateTime.Format("%Y%m%d%H%M%S"));

	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "FaultTime", EX_STTP_DATA_TYPE_STRING, pReport->m_tmFaultTime.Format("%Y%m%d%H%M%S"));
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::AddReportToDB, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CViewFault::AddReportToDB,ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

void CViewFault::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

				if (nSubItem >= 9)
				{
					lplvcd->clrText = RGB(0, 0, 255);
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}	
}



void CViewFault::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CViewFault::OnFaultDelete() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
		{
			return;
		}
	}
	CList<int, int>  itemList;
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int index = m_List.GetNextSelectedItem(pos);
		if (index < 0)
		{
			continue;
		}
		itemList.AddTail(index);
	}

	if (itemList.GetCount() == 0)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
	}

	if(AfxMessageBox(StringFromID(IDS_TIP_DELETE_FAULTREPORT), MB_OKCANCEL) != IDOK)
	{
		return;
	}

	DeleteFaultShortenData(itemList);
}

void CViewFault::DeleteFaultShortenData(CList<int, int>&  itemList)
{
	POSITION pos = itemList.GetHeadPosition();
	while (pos != NULL)
	{
		int index = itemList.GetNext(pos);
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(index);
		MYASSERT_CONTINUE(pReport);
		if (pReport->DeleteFaultShortenData())
		{
			DeleteFaultReport(pReport);
		}
	}
	FillListData();
}

BOOL CViewFault::DeleteFaultShortenData(CFaultReport* pReport)
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("casename = '%s' and id = %d", pReport->m_sName, pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);

	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultShortenData, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		DeleteFaultShortenEvents(pReport);
		DeleteFaultAnalyData(pReport);
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

void CViewFault::OnFaultExport() 
{
	//�������Ƿָ��
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//�򸸴��ڷ�����Ϣ
	pWnd ->PostMessage(FAULTREPORT_EXPORT_EXCEL, 0, 0);
}


void CViewFault::OnManualFaultShorten()
{
	CManualFaultShorten manDlg;
	manDlg.pView = this;
	manDlg.DoModal();
}


BOOL CViewFault::DeleteFaultShortenEvents(CFaultReport* pReport)
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultShortenEvents, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/****************************************************
Date:2012/2/6  Author:LYH
������:   DeleteFaultAnalyData	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: CFaultReport * pReport	
*****************************************************/
BOOL CViewFault::DeleteFaultAnalyData( CFaultReport* pReport )
{
	BOOL bReturn = TRUE;

	if(pReport == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��������
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_ANALYFAULT_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultAnalyData, ʧ��");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   RefreshData	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CViewFault::RefreshData()
{
	//�������Ƿָ��
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return 0;
	//�򸸴��ڷ�����Ϣ, ѡ���˿���,���Ϳ�ָ��
	pWnd ->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)NULL);
	if(!LoadDataFromDB(m_nMaxRow))
		return 0;
	FillListData();
	WriteLog("���ϱ�����ͼˢ���������", XJ_LOG_LV3);
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
void CViewFault::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pReport == NULL)
			continue;
		if(pReport->m_sStation1ID == pStation->m_sID || pReport->m_sStation2ID == pStation->m_sID)
		{
			bNeedReload = TRUE;
			break;
		}
	}
	if(bNeedReload)
	{
		RefreshData();
		WriteLog("StationInit,���ϱ���ҳ��ˢ�����", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,���ϱ���ҳ������ˢ��", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2013/8/8  Author:LYH
������:   LoadConfig	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewFault::LoadConfig()
{
	CString strIniFile = g_SysConfig;

	m_nMaxRow = GetPrivateProfileInt("FaultViewConfig", "MaxRow", 100, strIniFile);
	m_bOnlyShowNoOverhual = GetPrivateProfileInt("FaultViewConfig", "OnlyShowOverhual", 0, strIniFile);
	m_bOnlyShowSaved = GetPrivateProfileInt("FaultViewConfig", "OnlyShowSaved", 0, strIniFile);
	m_bOnlyShowTypical = GetPrivateProfileInt("FaultViewConfig", "OnlyShowTypical", 0, strIniFile);
}

/****************************************************
Date:2013/8/8  Author:LYH
������:   SaveConfig	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CViewFault::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("FaultViewConfig", "MaxRow", str, strIniFile);
	str.Format("%d", m_bOnlyShowNoOverhual);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowOverhual", str, strIniFile);
	str.Format("%d", m_bOnlyShowSaved);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowSaved", str, strIniFile);
	str.Format("%d", m_bOnlyShowTypical);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowTypical", str, strIniFile);
}

void CViewFault::OnFaultOnlyshowoverhaul() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowNoOverhual = !m_bOnlyShowNoOverhual;
	RefreshData();
}

void CViewFault::OnFaultOnlyshowsaved() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowSaved = !m_bOnlyShowSaved;
	RefreshData();
}

void CViewFault::OnFaultOnlyshowtypical() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowTypical = !m_bOnlyShowTypical;
	RefreshData();
}
