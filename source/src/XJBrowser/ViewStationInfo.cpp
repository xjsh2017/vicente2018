// ViewStationInfo.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewStationInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iStationInfoViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo

IMPLEMENT_DYNCREATE(CViewStationInfo, CFormView)

CViewStationInfo::CViewStationInfo()
	: CFormView(CViewStationInfo::IDD)
{
	//{{AFX_DATA_INIT(CViewStationInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_iStationInfoViewAsc = -1;
	m_arrColum.RemoveAll();
	m_pStation = NULL;
	m_nOldSortCol = -1;
}

CViewStationInfo::~CViewStationInfo()
{
}

void CViewStationInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStationInfo)
	DDX_Control(pDX, IDC_LIST_STATIONINFO, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStationInfo, CFormView)
	//{{AFX_MSG_MAP(CViewStationInfo)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STATIONINFO, OnColumnclickListStationinfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STATIONINFO, OnCustomDraw)
	ON_COMMAND(IDC_STATIONINFO_SEC, OnStationinfoSec)
	ON_COMMAND(IDC_STATIONINFO_COMM_H, OnStationinfoCommH)
	ON_COMMAND(IDC_STATIONINFO_RUN_H, OnStationinfoRunH)
	ON_COMMAND(IDC_STATIONINFO_EXCLEL, OnStationinfoExclel)
	ON_COMMAND(IDC_STATIONINFO_PRINT, OnStationinfoPrint)
	ON_COMMAND(IDC_STATIONINFO_REFRESH, OnStationinfoRefresh)
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STATIONINFO, OnDblclkListStationinfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStationInfoOpen)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo diagnostics

#ifdef _DEBUG
void CViewStationInfo::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewStationInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo message handlers

void CViewStationInfo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//��ʼ���б�
	InitListCtrl();
	//��ʼˢ�±���ͨ��״̬������״̬����ʱ�䶨ʱ��
	m_nTimer = SetTimer(1, g_RefreshTime*60*1000, 0);
}

/*************************************************************
 �� �� ��: InitListCtrl()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//��ʼ���б���
	InitListStyle();
	//�������
//	FillListData();
	return 0;
}

/*************************************************************
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::InitListStyle()
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
		//char* arColumn[8]={"�豸����","ͨѶ״̬", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��","����״̬", "����״̬�ı�ʱ��", "����״̬����ʱ��", "һ���豸"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 300; //�豸����
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 1://
				lvCol.cx = 100; // ͨѶ״̬
				colName = StringFromID(IDS_FUNC_COMMUSTATUS);
				break;
			case 2://
				lvCol.cx = 180; // ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 180; // ����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4://
				lvCol.cx = 100; //����״̬
				colName = StringFromID(IDS_FUNC_RUNSTATUS);
				break;
			case 5:
				lvCol.cx = 180; //ʱ��
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 6:
				lvCol.cx = 180; //����ʱ��
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 7:
				lvCol.cx = 160; // һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			default:
				lvCol.cx=50;//�п�100����
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 �� �� ��: FillListData()
 ���ܸ�Ҫ: ����б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::FillListData()
{
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();

	if(m_pStation == NULL)
		return -1;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, m_pStation->m_sID);
	//ѭ������
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		AddEventToList(pObj, 0);
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
BOOL CViewStationInfo::LoadListConfig()
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
			WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewStationInfoConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewStationInfoConfig", XJ_LOG_LV3);
				WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewStationInfoConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewStationInfo::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
BOOL CViewStationInfo::SaveListConfig()
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
			WriteLog("ViewStationInfo::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("ViewStationInfo::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewStationInfoConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewStationInfoConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewStationInfoConfig");
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
						WriteLog("δ�ҵ�ViewStationInfoConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���ViewStationInfoConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
BOOL CViewStationInfo::NeedSave()
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
 �� �� ��: RegulateControlSize()
 ���ܸ�Ҫ: �����ؼ���С,λ�� 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//�ͻ�����С
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//�����б���С,ռ�����߶ȵ�(1 - 0.618)
	float fHList = (float)rcList.Height();
	fHList = float(fHList);
	rcList.bottom = (long)fHList;
	m_List.MoveWindow(&rcList, TRUE);
}

void CViewStationInfo::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 �� �� ��: AddEventToList()
 ���ܸ�Ҫ: ��ָ��װ�õ�ͨѶ״̬,����״̬���뵽�б�����ʾ
 �� �� ֵ: ����ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ�����װ��
		   Param2 �����λ��
**************************************************************/
BOOL CViewStationInfo::AddEventToList( CSecObj * pSec, int i )
{
	//�ж�������Ч��
	if(pSec == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;

	//ˢ�³���ʱ��
	pSec->RefreshCommDurativeTime();
	pSec->RefreshRunDurativeTime();
	
	//"�豸����","ͨѶ״̬", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��","����״̬", "����״̬�ı�ʱ��", "����״̬����ʱ��", "һ���豸"
	
	//0:�豸����
	CString str;
	str = pSec->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	//1:ͨѶ״̬
	m_List.SetItemText(i, 1, GetCommStatusString(pSec->m_nCommStatus));
	//2:ͨѶ״̬�ı�ʱ��
	m_List.SetItemText(i, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
	//3:ͨѶ״̬����ʱ��
	m_List.SetItemText(i, 3, pSec->m_sCommDurativeTime);
	//4:����״̬
	m_List.SetItemText(i, 4, GetRunStatusString(pSec->m_nRunStatu));
	//5:����״̬�ı�ʱ��
	m_List.SetItemText(i, 5, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
	//6:����״̬����ʱ��
	m_List.SetItemText(i, 6, pSec->m_sRunDurativeTime);
	//7:һ���豸
	str = "";
	if(pSec->m_pOwner != NULL)
	{
		str = pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(i, 7, str);
	
	//��������
	m_List.SetItemData(i, (DWORD)pSec);
	return TRUE;
}


void CViewStationInfo::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ��: OnStationInfoOpen()
 ���ܸ�Ҫ: ��Ӧҳ��򿪻�����������Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::OnStationInfoOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//ȡ���³�վָ��
	m_pStation = (CStationObj*)lParam;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	if(m_pStation != NULL)
	{
		CString str;
		str.Format("[%s]%s", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO));
		pApp->SetAppTile(str);
	}
	//��������
	FillListData();
}

void CViewStationInfo::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_pStation != NULL)
	{
		CString str;
		str.Format("[%s]%s", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO));
		pApp->SetAppTile(str);
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ��: OnSTTP20002()
 ���ܸ�Ҫ: ��ӦͨѶ״̬�ı䱨��
 �� �� ֵ: 
 ��    ��: param1 �豸����ָ��
		   Param2 
**************************************************************/
void CViewStationInfo::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj == NULL)
		return;
	if(m_pStation == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	//�ı��б��е���ʾ
	ChangeCommStatusInList(pObj);
}

/*************************************************************
 �� �� ��: OnSTTP20079()
 ���ܸ�Ҫ: ��Ӧ����״̬�ı䱨��
 �� �� ֵ: 
 ��    ��: param1 �豸����ָ��
		   Param2 
**************************************************************/
void CViewStationInfo::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj == NULL)
		return;
	if(m_pStation == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	//�ı��б��е���ʾ
	ChangeRunStatusInList(pObj);
}

/*************************************************************
 �� �� ��: ChangeCommStatusInList()
 ���ܸ�Ҫ: �ı�ָ�������豸��ͨ��״̬��ͨ��״̬�б���е���ʾ
 �� �� ֵ: 
 ��    ��: param1 ָ�������豸
		   Param2 
**************************************************************/
void CViewStationInfo::ChangeCommStatusInList( CSecObj* pSec )
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
	
	if(nIndex < 0)
	{
		//δ�ҵ�������ʾ
		return;
	}
	else
	{
		//�ı�ֵ
		//1:ͨѶ״̬
		m_List.SetItemText(nIndex, 1, GetCommStatusString(pSec->m_nCommStatus));
		//2:ͨѶ״̬�ı�ʱ��
		m_List.SetItemText(nIndex, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		//3:ͨѶ״̬����ʱ��
		m_List.SetItemText(nIndex, 3, pSec->m_sCommDurativeTime);
		//ˢ��
		m_List.Update(nIndex);
	}
}

/*************************************************************
 �� �� ��: ChangeRunStatusInList()
 ���ܸ�Ҫ: �ı�ָ�������豸������״̬��ͨ��״̬�б���е���ʾ
 �� �� ֵ: 
 ��    ��: param1 ָ�������豸
		   Param2 
**************************************************************/
void CViewStationInfo::ChangeRunStatusInList( CSecObj* pSec )
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
	
	if(nIndex < 0)
	{
		//δ�ҵ�������ʾ
		return;
	}
	else
	{
		//�ı�ֵ
		//4:����״̬
		m_List.SetItemText(nIndex, 4, GetRunStatusString(pSec->m_nRunStatu));
		//5:����״̬�ı�ʱ��
		m_List.SetItemText(nIndex, 5, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		//6:����״̬����ʱ��
		m_List.SetItemText(nIndex, 6, pSec->m_sRunDurativeTime);
		//ˢ��
		m_List.Update(nIndex);
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
int CALLBACK CViewStationInfo::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CSecObj* pObj1 = (CSecObj*)lParam1;
	CSecObj* pObj2 = (CSecObj*)lParam2;
	if(pObj2 == NULL || pObj1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"�豸����","ͨѶ״̬", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��","����״̬", "����״̬�ı�ʱ��", "����״̬����ʱ��", "һ���豸"
	switch(nCol)
	{
	case 0: //��������
		str1 = pObj1->m_sName;
		str2 = pObj2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //ͨѶ״̬
		iResult = pObj1->m_nCommStatus - pObj2->m_nCommStatus;
		break;
	case 2: //ͨѶ״̬�ı�ʱ��
		str1.Format("%s", pObj1->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //ͨѶ״̬����ʱ��
		iResult = pObj1->m_lCommDurative - pObj2->m_lCommDurative;
		break;
	case 4: //����״̬
		iResult = pObj1->m_nRunStatu - pObj2->m_nRunStatu;
		break;
	case 5: //����״̬�ı�ʱ��
		str1.Format("%s", pObj1->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //����״̬����ʱ��
		iResult = pObj1->m_lRunDurative - pObj2->m_lRunDurative;
		break;
	case 7: //һ���豸��
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
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iStationInfoViewAsc;
	return iResult;
}

void CViewStationInfo::OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iStationInfoViewAsc = - g_iStationInfoViewAsc;
	}
	else
	{
		g_iStationInfoViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewStationInfo::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

					//�õ��¼�����
					CSecObj * pSec = (CSecObj*)m_List.GetItemData(nItem);
					if(pSec == NULL)
						break;
					if(nSubItem == 1) //ͨѶ״̬
					{
						if(pSec->m_nCommStatus == 1)
						{
							//�Ͽ�
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}
					if(nSubItem == 4) //����״̬
					{
						switch(pSec->m_nRunStatu)
						{
						case 0: //����
							lplvcd->clrText = XJ_STATUS_RUN_CHECK;
							break;
						case 1: //ͣ��
							lplvcd->clrText = XJ_STATUS_RUN_OFF;
							break;
// 						case 2: //Ͷ��
// 							lplvcd->clrTextBk = XJ_STATUS_RUN_ON;
// 							break;
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

void CViewStationInfo::OnStationinfoSec() 
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
	//��ʾ���¼��Ķ����豸����
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pObj->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pObj, 1);
	else
		pDoc->ShowSecPropPage(pObj);
}

void CViewStationInfo::OnStationinfoCommH() 
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

void CViewStationInfo::OnStationinfoRunH() 
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

void CViewStationInfo::OnStationinfoExclel() 
{
	// TODO: Add your command handler code here
	if(m_pStation == NULL)
		return;
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("[%s]%s%s.xls", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO),t.Format("%Y%m%d%H%M%S"));
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

void CViewStationInfo::OnStationinfoPrint() 
{
	// TODO: Add your command handler code here
	
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, STATIONINFO_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),STATIONINFO_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInStationInfo);
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

void CViewStationInfo::OnStationinfoRefresh() 
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

void CViewStationInfo::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_STATIONINFO))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_STATIONINFO_SEC, MF_ENABLED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

/*************************************************************
 �� �� ��: WriteResultToExcel()
 ���ܸ�Ҫ: �����������Excel
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 �ļ�·��
		   Param2 
**************************************************************/
BOOL CViewStationInfo::WriteResultToExcel( CString strFullPath )
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
				//"�豸����","ͨѶ״̬", "ͨѶ״̬�ı�ʱ��", "ͨѶ״̬����ʱ��","����״̬", "����״̬�ı�ʱ��", "����״̬����ʱ��", "һ���豸"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_FUNC_COMMUSTATUS), StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_FUNC_RUNSTATUS), StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION), StringFromID(IDS_MODEL_PRIMARY));
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
 �� �� ��: AddResultToExcel()
 ���ܸ�Ҫ: ��ָ���Ľ����д�뵽ָ����Excel�ļ���
 �� �� ֵ: 
 ��    ��: param1 ���ݿ����
		   Param2 �豸����
**************************************************************/
void CViewStationInfo::AddResultToExcel( CDatabase* pDatabase, CSecObj* pSec )
{
	//���������Ч��
	if(pSec == NULL)
		return;
	if(pDatabase == NULL)
		return;
	
	//һ���豸
	CString strDevice = "";
	if(pSec->m_pOwner != NULL)
	{
		strDevice = pSec->m_pOwner->m_sName;
	}
	
	//"�豸����","״̬", "״̬�ı�ʱ��", "״̬����ʱ��", "��վ","һ���豸"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s', '%s','%s')",
		pSec->m_sName, GetCommStatusString(pSec->m_nCommStatus), pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sCommDurativeTime, 
		GetRunStatusString(pSec->m_nRunStatu), pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sRunDurativeTime, strDevice);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 �� �� ��: InitCOM()
 ���ܸ�Ҫ: ��ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL CViewStationInfo::InitCOM()
{
	return InitReportComNoReg(pClf, pReportDoc);
}

/*************************************************************
 �� �� ��: ReleaseCOM()
 ���ܸ�Ҫ: ɾ��COM���
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::ReleaseCOM()
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
 �� �� ��: TranslateSingleKey()
 ���ܸ�Ҫ: ���͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ��: param1 �ؼ���
		   Param2 
**************************************************************/
CString CViewStationInfo::TranslateSingleKey( CString sKey )
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

/*************************************************************
 �� �� ��: RefreshDuravieTime()
 ���ܸ�Ҫ: ˢ��LIST��ָ��λ�õĳ���ʱ����ʾ
 �� �� ֵ: 
 ��    ��: param1 ָ����LIST�е�λ��
		   Param2 
**************************************************************/
void CViewStationInfo::RefreshDuravieTime( int nIndex )
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
	//ˢ��ͨѶ״̬����ʱ��
	pObj->RefreshCommDurativeTime();
	//������ʾ
	m_List.SetItemText(nIndex, 3, pObj->m_sCommDurativeTime);

	//ˢ������״̬����ʱ��
	pObj->RefreshRunDurativeTime();
	//������ʾ
	m_List.SetItemText(nIndex, 6, pObj->m_sRunDurativeTime);

	//ˢ��
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ��: RefreshTime()
 ���ܸ�Ҫ: ˢ�����б�����ͨ��״̬������״̬�ĳ���ʱ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::RefreshTime()
{
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
}

void CViewStationInfo::OnTimer(UINT nIDEvent) 
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

void CViewStationInfo::OnDblclkListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//�õ����ж�Ӧ���豸
		//�õ��豸
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nSelected);
		if(pObj == NULL)
			return;
		//��ʾ�ö����豸����
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		if(pObj->m_nType == TYPE_WAVEREC)
			pDoc->ShowSecPropPage(pObj, 1);
		else
			pDoc->ShowSecPropPage(pObj);
	}
	*pResult = 0;
}

/****************************************************
Date:2013/6/3  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CViewStationInfo::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	FillListData();
	WriteLog("StationInit,��վ��װ����Ϣҳ��ˢ�����", XJ_LOG_LV3);
}
