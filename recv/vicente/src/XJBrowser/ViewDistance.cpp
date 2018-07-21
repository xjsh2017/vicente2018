// ViewDistance.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewDistance.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iDistanceViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewDistance

IMPLEMENT_DYNCREATE(CViewDistance, CFormView)

CViewDistance::CViewDistance()
	: CFormView(CViewDistance::IDD)
{
	//{{AFX_DATA_INIT(CViewDistance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
}

CViewDistance::~CViewDistance()
{
}

void CViewDistance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDistance)
	DDX_Control(pDX, IDC_LIST_DISTANCE, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDistance, CFormView)
	//{{AFX_MSG_MAP(CViewDistance)
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DISTANCE, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DISTANCE, OnDblclkListDistance)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DISTANCE, OnColumnclickListDistance)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDistance diagnostics

#ifdef _DEBUG
void CViewDistance::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewDistance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}



#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDistance message handlers
/****************************************************
Date:2013/7/9  Author:LYH
������:   InitListCtrl	
����ֵ:   int	ʧ�ܷ���<=0, �ɹ�����>0
���ܸ�Ҫ: ��ʼ���б��
*****************************************************/
int CViewDistance::InitListCtrl()
{
	RefreshData();
	return 1;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   InitListStyle	
����ֵ:   int	ʧ�ܷ���<=0, �ɹ�����>0
���ܸ�Ҫ: ��ʼ���б���ʽ
*****************************************************/
int CViewDistance::InitListStyle()
{
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
		//char* arColumn[9]={"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 9; nCol++)
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
				lvCol.cx = 180; // ʱ��
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 2://
				lvCol.cx = 220; // ��·���� 
				colName = StringFromID(IDS_MODEL_LINE);
				break;
			case 3://
				lvCol.cx = 80; // ��ѹ�ȼ�
				colName = StringFromID(IDS_MODEL_VOLTAGE);
				break;
			case 4:
				lvCol.cx = 100; // ��·�ܳ���
				colName = StringFromID(IDS_LINE_TOTALLENGTH);
				break;
			case 5:
				lvCol.cx = 100; //���ϵ㵽M�˾���
				colName = StringFromID(IDS_FAULTPOINT_M);
				break;
			case 6:
				lvCol.cx = 80; //����λ��
				colName = StringFromID(IDS_TOWER_LOCATION);
				break;
			case 7:
				lvCol.cx = 160; //ά��������
				colName = StringFromID(IDS_MAINTAIN_DEPARTMENT);
				break;
			case 8:
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
		//Ĭ������"ȷ��"
		m_List.SetColumnHide(8, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   LoadListConfig	
����ֵ:   BOOL	����ɹ�����TRUE, ʧ�ܷ���FALSE
���ܸ�Ҫ: ��������Ϣ����
*****************************************************/
BOOL CViewDistance::LoadListConfig()
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
			WriteLog("ViewDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewDistanceConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("ViewDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewDistanceConfig�µ�����");
					WriteLog("ViewDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

/****************************************************
Date:2013/7/9  Author:LYH
������:   SaveListConfig	
����ֵ:   BOOL	����ɹ�����TRUE, ʧ�ܷ���FALSE
���ܸ�Ҫ: ��������Ϣ����
*****************************************************/
BOOL CViewDistance::SaveListConfig()
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
			WriteLog("ViewDistance::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewDistance::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewDistanceConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewDistanceConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewDistanceConfig");
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
						WriteLog("δ�ҵ�ViewDistanceConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewDistanceConfig�µ�����, ��ɾ���½������ӽڵ�");
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

/****************************************************
Date:2013/7/9  Author:LYH
������:   NeedSave	
����ֵ:   BOOL	��Ҫ���淵��TRUE,����Ҫ���淵��FALSE
���ܸ�Ҫ: �ж��Ƿ���Ҫ�����б������Ϣ
*****************************************************/
BOOL CViewDistance::NeedSave()
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

/****************************************************
Date:2013/7/9  Author:LYH
������:   RegulateControlSize	
����ֵ:   void	
���ܸ�Ҫ: �����ؼ���С,λ��
*****************************************************/
void CViewDistance::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList;
		
	rcList=rc;
	
	m_List.MoveWindow(&rcList, TRUE);
}

void CViewDistance::OnDestroy() 
{
	SaveConfig();
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CViewDistance::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewDistance::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//��ȡ����
	LoadConfig();

	InitListStyle();
	//��ʼ���б�
	InitListCtrl();
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   LoadDataFromDB	
����ֵ:   BOOL	��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
���ܸ�Ҫ: �����ݿ��ȡ����
����: int num	Ҫ���ҵĸ���
����: BOOL IsQueryAcked	�Ƿ�Ҫ��ѯ��ȷ�ϵ��¼�
*****************************************************/
BOOL CViewDistance::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
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

	//����
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_DISTANCE_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewDistance::LoadDataFromDB, ��ѯʧ��");
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
			str.Format("CViewDistance::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventDistance * pEvent = new CXJEventDistance;
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
		str.Format("CViewDistance::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   AddEventToList	
����ֵ:   BOOL	����ɹ�����TRUE,ʧ�ܷ���FALSE
���ܸ�Ҫ: ��һ���¼�����Ϣ���뵽�б�
����: CXJEventDistance * pEvent	Ҫ������¼�
����: int i	�����λ��
*****************************************************/
BOOL CViewDistance::AddEventToList( CXJEventDistance * pEvent, int i )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	//"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"
	//0:���
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 1, sTime);
	//2:��·����
	m_List.SetItemText(i, 2, pEvent->m_sLineName);
	//3:��ѹ�ȼ�
	str.Format("%dkV", pEvent->m_nVoltage);
	m_List.SetItemText(i, 3, str);
	//4:��·�ܳ���
	str.Format("%.2fkm", pEvent->m_fLineTotalLength);
	m_List.SetItemText(i, 4, str);
	//5:���ϵ㵽M�˾���
	str.Format("%.2fkm", pEvent->m_fDistance);
	m_List.SetItemText(i, 5, str);
	//6:����λ��
	str.Format("%d", pEvent->m_nTowerPos);
	m_List.SetItemText(i, 6, str);
	//7:ά��������
	m_List.SetItemText(i, 7, pEvent->m_sManagerUnit);
	//8:��ȷ��
	m_List.SetItemText(i, 8, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   FillListData	
����ֵ:   int	ʧ�ܷ���<=0, �ɹ�����>0
���ܸ�Ҫ: ����б�����
*****************************************************/
int CViewDistance::FillListData()
{
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	CXJEventDistance * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDistance *)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 1;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   RefreshData	
����ֵ:   int	ʧ�ܷ���<=0, �ɹ�����>0
���ܸ�Ҫ: ˢ�����ݣ�����������ݡ������ݿ���롢��䵽�б��
*****************************************************/
int CViewDistance::RefreshData()
{
	if(!LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//��ȡ����ʧ��
		WriteLog("��ȡС������¼�ʧ��", XJ_LOG_LV1);
		return 0;
	}
	else
	{
		//�ɹ�
		FillListData();
	}
	WriteLog("ˢ��С������¼�ҳ�����", XJ_LOG_LV3);
	return 1;
}

void CViewDistance::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
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

void CViewDistance::OnEventAck() 
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
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
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

void CViewDistance::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_DISTANCE_CFG))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

void CViewDistance::OnEventCopy() 
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
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
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

void CViewDistance::OnEventProp() 
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
	CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//���õ�ǰ�¼�
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

void CViewDistance::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

void CViewDistance::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//�����������
	RefreshData();
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   RefreshAck	
����ֵ:   void	
���ܸ�Ҫ: ȷ�ϳɹ���ˢ�½�����ʾ
����: CXJEventDistance * pEvent	ȷ�ϵ��¼�
����: int nIndex	�¼�λ��
*****************************************************/
void CViewDistance::RefreshAck( CXJEventDistance* pEvent, int nIndex /*= -1*/ )
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
	m_List.SetItemText(nIndex, 8, StringFromID(IDS_ACK_YES));
	
	m_List.Update(nIndex);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   TellOtherWndAck	
����ֵ:   void	
���ܸ�Ҫ: ����������������ȷ�����¼�
����: CXJEventDistance * pEvent	��ȷ�ϵ��¼�
*****************************************************/
void CViewDistance::TellOtherWndAck( CXJEventDistance* pEvent )
{
	//������Ϣ֪ͨ�¼����ԶԻ���
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}
void CViewDistance::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

void CViewDistance::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewDistance::OnViewRefresh, ǿ��ˢ�����", XJ_LOG_LV3);
}

void CViewDistance::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					//�õ��¼�����
					CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nItem);
					if(pEvent == NULL)
						break;
					
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

void CViewDistance::OnDblclkListDistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

void CViewDistance::OnColumnclickListDistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iDistanceViewAsc = - g_iDistanceViewAsc;
	}
	else
	{
		g_iDistanceViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   CompareFunction	
����ֵ:   int CALLBACK	�ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
���ܸ�Ҫ: �ȽϺ���,����ʱ������
����: LPARAM lParam1	�Ƚ϶���1
����: LPARAM lParam2	�Ƚ϶���2
����: LPARAM lParamData	�Ƚϲ���
*****************************************************/
int CALLBACK CViewDistance::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventDistance* pEvent1 = (CXJEventDistance*)lParam1;
	CXJEventDistance* pEvent2 = (CXJEventDistance*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //��·����
		str1 = pEvent1->m_sLineName;
		str2 = pEvent2->m_sLineName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3:	//��ѹ�ȼ�
		iResult = pEvent1->m_nVoltage - pEvent2->m_nVoltage;
		break;
	case 4: //��·�ܳ���
		if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 5: //���ϵ㵽M�˾���
		if(pEvent1->m_fDistance - pEvent2->m_fDistance < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fDistance - pEvent2->m_fDistance > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 6://����λ��
		iResult = pEvent1->m_nTowerPos - pEvent2->m_nTowerPos;
		break;
	case 7: //ά��������
		str1 = pEvent1->m_sManagerUnit;
		str2 = pEvent2->m_sManagerUnit;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://��ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDistanceViewAsc;
	return iResult;
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   CheckLineCount	
����ֵ:   void	
���ܸ�Ҫ: ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ�������
*****************************************************/
void CViewDistance::CheckLineCount()
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
		CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nNewCount - 1);
		//���б���ɾ��
		m_List.DeleteItem(nNewCount - 1);
		//���¼�������ڴ���ɾ��
		m_EventManager.RemoveAtEvent(pEvent);
	}
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   LoadConfig	
����ֵ:   void	
���ܸ�Ҫ: ��ȡ����
*****************************************************/
void CViewDistance::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�����ʾ����
	m_nMaxRow = GetPrivateProfileInt("DistanceViewConfig", "MaxRow", 100, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	m_bShowAcked = GetPrivateProfileInt("DistanceViewConfig", "ShowAcked", 1, strIniFile);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   SaveConfig	
����ֵ:   void	
���ܸ�Ҫ: ��������
*****************************************************/
void CViewDistance::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ʾ����
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("DistanceViewConfig", "MaxRow", str, strIniFile);
	//�Ƿ���ʾ��ȷ���¼�
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("DistanceViewConfig", "ShowAcked", str, strIniFile);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   OnSTTP20168	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewDistance::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewDistance::OnSTTP20168 start");
	
	CXJEventDistance* pParam = (CXJEventDistance*)lParam;
	if(pParam == NULL)
		return;

	//����
	CXJEventDistance* pEvent = (CXJEventDistance*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	WriteLog("CViewDistance::OnSTTP20168 neat end");
}

void CViewDistance::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	//GetParentFrame()->SetWindowText("С�����");
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_LOCATION));
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewDistance::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
			pFrame->PostMessage(WM_EVENTLIST_PASSIVE_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pLocal);
	}
}

void CViewDistance::OnEventPropAckPT( WPARAM wParma, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParma;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
	}
}

CXJEventDistance* CViewDistance::FindLocalPoint( CXJEventDistance* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventDistance* pReturn = NULL;
	//��Ϊ������ID��֪�������,ֻ�ܱȽ϶����豸ID, ����ʱ�����Ϣ
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventDistance* pTemp = (CXJEventDistance*)m_EventManager.GetAtEvent(i);
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
