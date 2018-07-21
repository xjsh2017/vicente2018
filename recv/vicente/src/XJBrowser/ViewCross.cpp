// ViewCross.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewCross.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iCrossViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewCross

IMPLEMENT_DYNCREATE(CViewCross, CFormView)

CViewCross::CViewCross()
	: CFormView(CViewCross::IDD)
{
	//{{AFX_DATA_INIT(CViewCross)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_listCross.RemoveAll();
	m_nOldSortCol = -1;
	g_iCrossViewAsc = -1;
}

CViewCross::~CViewCross()
{
	Clear();
}

void CViewCross::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCross)
	DDX_Control(pDX, IDC_LIST_CROSS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCross, CFormView)
	//{{AFX_MSG_MAP(CViewCross)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CROSS, OnClickListCross)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CROSS, OnItemchangedListCross)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CROSS, OnColumnclickListCross)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20159, OnSTTP20159)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCross diagnostics

#ifdef _DEBUG
void CViewCross::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewCross::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

/*************************************************************
 �� �� ��: InitListCtrl()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewCross::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	if(!LoadDataFromDB())
	{
		WriteLog("Ԥ����ͼ��ȡ����ʧ��", 1);
	}
	else
	{
		FillListData();
	}
	return 0;
}

/*************************************************************
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewCross::InitListStyle()
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
		//char* arColumn[6]={"���","Ԥ��������","��ǰ״̬","״̬�ı�ʱ��", "��������","��վ��"};
		for (int nCol=0; nCol < 6 ; nCol++)
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
				lvCol.cx = 200; //����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 80; //��ǰ״̬
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3://
				lvCol.cx = 180; //ʱ��
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 4:
				lvCol.cx = 100; //��������
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			case 5:
				lvCol.cx = 120; //��վ��
				colName = StringFromID(IDS_MODEL_SUBSTATION);
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
int CViewCross::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	CCrossDef * pEvent = NULL;
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pEvent = (CCrossDef*)m_listCross.GetNext(pos);
		AddEventToList(pEvent, 0);
	}
	
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
BOOL CViewCross::LoadListConfig()
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
			WriteLog("ViewCross::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewCross::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewCrossConfig");

			//����ViewCrossConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewCrossConfig");
				WriteLog("ViewCross::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewCrossConfig�µ�����");
					WriteLog("ViewCross::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
BOOL CViewCross::SaveListConfig()
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
			WriteLog("ViewCross::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewCross::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCrossConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewAlarmConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCrossConfig");
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
						WriteLog("δ�ҵ�ViewCrossConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewCrossConfig�µ�����, ��ɾ���½������ӽڵ�");
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
BOOL CViewCross::NeedSave()
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
 ���ܸ�Ҫ: ��ָ��Ԥ����䵽�б�
 �� �� ֵ: ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ��Ԥ��
		   Param2 ָ�����б��е�λ��
**************************************************************/
BOOL CViewCross::AddEventToList( CCrossDef * pEvent, int i /*= 0*/ )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	//"���","Ԥ��������","��ǰ״̬","״̬�ı�ʱ��", "��������","��վ��"
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:Ԥ��������
	m_List.SetItemText(i, 1, pEvent->m_sName);
	
	//2:��ǰ״̬
	m_List.SetItemText(i, 2, pEvent->GetStatusDes(pEvent->m_nCurStatus));
	//3:����ʱ��
	m_List.SetItemText(i, 3, pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	//4:��������
	m_List.SetItemText(i, 4, pEvent->GetDataTypeDes(pEvent->m_nDataType));
	//5:��վ��
	CString str = "";
	if(pEvent->m_pStation != NULL)
		str = pEvent->m_pStation->m_sName;
	m_List.SetItemText(i, 5, str);
	//��������
	m_List.SetItemData(i, (DWORD)(pEvent));
	return TRUE;
}

void CViewCross::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//��ʼ���б�ؼ�
	InitListCtrl();
}

void CViewCross::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//������λ�øı��ı�ؼ�λ��
	RegulateControlSize();
}

/*************************************************************
 �� �� ��: RegulateControlSize()
 ���ܸ�Ҫ: ������λ�øı��ı�ؼ�λ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewCross::RegulateControlSize()
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

void CViewCross::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CViewCross::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_CROSSSTATUS_ALERT));
	//GetParentFrame()->SetWindowText(" Ԥ�� ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ��: LoadDataFromDB()
 ���ܸ�Ҫ: �����ݿ��ȡ����
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewCross::LoadDataFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//���������
	Clear();
	m_List.DeleteAllItems();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SECTION_COMB, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewCross::LoadDataFromDB, ��ѯʧ��");
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
			str.Format("CViewCross::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CCrossDef * pEvent = new CCrossDef;
			if(pEvent->LoadFromDB(pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				m_listCross.AddTail(pEvent);
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
		str.Format("CViewCross::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: Clear()
 ���ܸ�Ҫ: ���������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewCross::Clear()
{
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pDef = (CCrossDef*)m_listCross.GetNext(pos);
		if(pDef != NULL)
			delete pDef;
	}
	m_listCross.RemoveAll();
}

void CViewCross::OnClickListCross(NMHDR* pNMHDR, LRESULT* pResult) 
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
		pWnd ->PostMessage(VIEWCROSS_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	*pResult = 0;
}

void CViewCross::OnItemchangedListCross(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CCrossDef * pEvent = (CCrossDef*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//�������Ƿָ��,�����ڵĸ�������ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//�򸸴��ڷ���SELECTCHANGED��Ϣ
		pWnd ->PostMessage(VIEWCROSS_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������ 
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CViewCross::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CCrossDef* pEvent1 = (CCrossDef*)lParam1;
	CCrossDef* pEvent2 = (CCrossDef*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","Ԥ��������","��ǰ״̬","״̬�ı�ʱ��", "��������","��վ��"
	switch(nCol)
	{
	case 0: //���
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //Ԥ��������
		str1 = pEvent1->m_sName;
		str2 = pEvent2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //��ǰ״̬
		iResult = pEvent1->m_nCurStatus - pEvent2->m_nCurStatus;
		break;
	case 3: //״̬�ı�ʱ��
		str1 = pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //��������
		iResult = pEvent1->m_nDataType - pEvent2->m_nDataType;
		break;
	case 5: //��վ��
		str1 = "";
		if(pEvent1->m_pStation != NULL)
			str1 = pEvent1->m_pStation->m_sName;
		str2 = "";
		if(pEvent2->m_pStation != NULL)
			str2 = pEvent2->m_pStation->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iCrossViewAsc;
	return iResult;
}

void CViewCross::OnColumnclickListCross(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iCrossViewAsc = - g_iCrossViewAsc;
	}
	else
	{
		g_iCrossViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ��: OnSTTP20159()
 ���ܸ�Ҫ: ��ӦԤ���ṹ֪ͨ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 ��������
**************************************************************/
void CViewCross::OnSTTP20159( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewCross::OnSTTP20159 start");
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20159)
		return;

	//ȡ��ID
	int nID = pSttpData->sttp_body.nFlag;
	//����Ԥ����
	CCrossDef* pDef = FindCross(nID);

	if(pDef != NULL)
	{
		//��ȡ����
		pDef->LoadFromSTTP(*pSttpData);
		//����ֵ
		pDef->ClearDetail();
		pDef->m_bLoadDetail = FALSE;
		UpdateCrossList(pDef);
	}
	else
	{
		CString str;
		str.Format("CViewCross::OnSTTP20159, �Ҳ���IDΪ%d��Ԥ����", nID);
		WriteLog(str, XJ_LOG_LV3);
	}

	WriteLog("CViewCross::OnSTTP20159 end");
}

/*************************************************************
 �� �� ��: FindCross()
 ���ܸ�Ҫ: ����ָ��ID��Ԥ����
 �� �� ֵ: Ԥ����
 ��    ��: param1 ָ��ID
		   Param2 
**************************************************************/
CCrossDef* CViewCross::FindCross( int nID )
{
	CCrossDef* pReturn = NULL;
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pDef = (CCrossDef*)m_listCross.GetNext(pos);
		if(pDef == NULL)
			continue;
		if(pDef->m_nID == nID)
		{
			pReturn = pDef;
			break;
		}
	}
	return pReturn;
}

/*************************************************************
 �� �� ��: UpdateCrossList()
 ���ܸ�Ҫ: ����Ԥ�����б��е���ʾ
 �� �� ֵ: 
 ��    ��: param1 ָ��Ԥ��
		   Param2 
**************************************************************/
void CViewCross::UpdateCrossList( CCrossDef* pDef )
{
	if(pDef == NULL)
		return;
	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pDef;
	int nIndex = m_List.FindItem(&lvFind);
	if(nIndex < 0)
		return;
	//�ı�ֵ
	//2:��ǰ״̬
	m_List.SetItemText(nIndex, 2, pDef->GetStatusDes(pDef->m_nCurStatus));
	//3:����ʱ��
	m_List.SetItemText(nIndex, 3, pDef->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.Update(nIndex);
}
