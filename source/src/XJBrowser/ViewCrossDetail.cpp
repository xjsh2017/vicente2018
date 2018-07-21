// ViewCrossDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewCrossDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/** @brief           ����������λ*/
int g_iCrossDetailViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail

IMPLEMENT_DYNCREATE(CViewCrossDetail, CFormView)

CViewCrossDetail::CViewCrossDetail()
	: CFormView(CViewCrossDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewCrossDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCrossDef = NULL;
	m_nOldSortCol = -1;
}

CViewCrossDetail::~CViewCrossDetail()
{
}

void CViewCrossDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCrossDetail)
	DDX_Control(pDX, IDC_LIST_CROSSDETAIL, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCrossDetail, CFormView)
	//{{AFX_MSG_MAP(CViewCrossDetail)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CROSSDETAIL, OnColumnclickListCrossdetail)
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWCROSS_SELECTCHANGED, OnViewCrossSelChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail diagnostics

#ifdef _DEBUG
void CViewCrossDetail::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewCrossDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail message handlers
/*************************************************************
 �� �� ��: LoadListConfig()
 ���ܸ�Ҫ: ��������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewCrossDetail::LoadListConfig()
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
			WriteLog("ViewCrossDetail::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("ViewCrossDetail::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewCrossDetailConfig");

			//����ViewCrossConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewCrossDetailConfig");
				WriteLog("ViewCrossDetail::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewCrossDetailConfig�µ�����");
					WriteLog("ViewCrossDetail::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
BOOL CViewCrossDetail::SaveListConfig()
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
			WriteLog("ViewCrossDetail::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewCrossDetail::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCrossDetailConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCrossDetailConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCrossDetailConfig");
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
						WriteLog("δ�ҵ�ViewCrossDetailConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���ViewCrossDetailConfig�µ�����, ��ɾ���½������ӽڵ�");
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
BOOL CViewCrossDetail::NeedSave()
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
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::InitListStyle()
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
		//char* arColumn[8]={"���","���ݵ�����", "״̬","���ݵ�ֵ","Ԥ��ֵ", "�����豸","CPU","��վ��"};
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
				lvCol.cx = 200; //����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 100; //��ǰ״̬
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3://
				lvCol.cx = 100; //ʱ��
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 4:
				lvCol.cx = 100; //���ݵ�ֵ
				colName = StringFromID(IDS_COMMON_VALUE);
				break;
			case 5:
				lvCol.cx = 100; //Ԥ��ֵ
				colName = StringFromID(IDS_CROSSSTATUS_ALERT);
				break;
			case 6:
				lvCol.cx = 300; //�����豸
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 7:
				lvCol.cx = 80; //CPU
				colName = StringFromID(IDS_MODEL_CPU);
				break;
			case 8:
				lvCol.cx = 160; //��վ��
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
//	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 �� �� ��: RegulateControlSize()
 ���ܸ�Ҫ: �����ؼ�λ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewCrossDetail::RegulateControlSize()
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

void CViewCrossDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//������λ�øı��ı�ؼ�λ��
	RegulateControlSize();
}

/*************************************************************
 �� �� ��: InitListCtrl()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	return 1;
}

void CViewCrossDetail::OnInitialUpdate() 
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

void CViewCrossDetail::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ��: FillListData()
 ���ܸ�Ҫ: ����б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	if(m_pCrossDef != NULL)
	{
		if(!m_pCrossDef->m_bLoadDetail)
		{
			//δ������ϸ��Ϣ,��������
			m_pCrossDef->LoadDetail();
		}
		POSITION pos = m_pCrossDef->m_listCross.GetHeadPosition();
		while(pos != NULL)
		{
			CCrossPoint* pEvent = (CCrossPoint*)m_pCrossDef->m_listCross.GetNext(pos);
			AddEventToList(pEvent, 0);
		}
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 �� �� ��: AddEventToList()
 ���ܸ�Ҫ: ��ָ��Ԥ�����ݵ���䵽�б�
 �� �� ֵ: ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ��Ԥ�����ݵ�
		   Param2 ָ�����б��е�λ��
**************************************************************/
BOOL CViewCrossDetail::AddEventToList( CCrossPoint * pEvent, int i /*= 0*/ )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	//"���","���ݵ�����", "״̬","���ݵ�ֵ","Ԥ��ֵ", "�����豸","CPU","��վ��"
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:���ݵ�����
	m_List.SetItemText(i, 1, pEvent->m_sDataPointName);
	
	//2:��ǰ״̬
	m_List.SetItemText(i, 2, pEvent->GetStatusDes(pEvent->m_nCrossType));
	//3:���ݵ�ֵ
	m_List.SetItemText(i, 3, pEvent->m_sDataPointValue);
	//4:Ԥ��ֵ
	m_List.SetItemText(i, 4, pEvent->m_sSectionValue);
	//5:�����豸
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 5, str);
	//6:CPU
	str = "";
	str.Format("%d", pEvent->m_nCPU);
	m_List.SetItemText(i, 6, str);
	//7:��վ
	str = "";
	if(pEvent->m_pStation != NULL)
		str = pEvent->m_pStation->m_sName;
	m_List.SetItemText(i, 7, str);
	//��������
	m_List.SetItemData(i, (DWORD)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ��: OnViewCrossSelChanged()
 ���ܸ�Ҫ: ��Ԥ����ͼѡ��ı����Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewCrossDetail::OnViewCrossSelChanged( WPARAM wParam, LPARAM lParam )
{
	CCrossDef* pDef = (CCrossDef*)lParam;
	if(pDef != m_pCrossDef)
	{
		m_pCrossDef = pDef;
		FillListData();
	}
}

/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������ 
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CViewCrossDetail::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CCrossPoint* pEvent1 = (CCrossPoint*)lParam1;
	CCrossPoint* pEvent2 = (CCrossPoint*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","���ݵ�����", "״̬","���ݵ�ֵ","Ԥ��ֵ", "�����豸","CPU","��վ��"
	switch(nCol)
	{
	case 0: //���
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //���ݵ�����
		str1 = pEvent1->m_sDataPointName;
		str2 = pEvent2->m_sDataPointName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //״̬
		iResult = pEvent1->m_nCrossType - pEvent2->m_nCrossType;
		break;
	case 3: //���ݵ�ֵ
		str1 = pEvent1->m_sDataPointValue;
		str2 = pEvent2->m_sDataPointValue;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //Ԥ��ֵ
		str1 = pEvent1->m_sSectionValue;
		str2 = pEvent2->m_sSectionValue;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //�����豸
		str1 = "";
		if(pEvent1->m_pSec != NULL)
			str1 = pEvent1->m_pSec->m_sName;
		str2 = "";
		if(pEvent2->m_pSec != NULL)
			str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //CPU
		iResult = pEvent1->m_nCPU - pEvent2->m_nCPU;
		break;
	case 7: //��վ��
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
	iResult *= g_iCrossDetailViewAsc;
	return iResult;
}

void CViewCrossDetail::OnColumnclickListCrossdetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iCrossDetailViewAsc = - g_iCrossDetailViewAsc;
	}
	else
	{
		g_iCrossDetailViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewCrossDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_CROSSSTATUS_ALERT));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
