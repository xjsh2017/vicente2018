// PTAction.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTAction.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

#define PTACTION_COL_NUM 8

/** @brief           ����������λ*/
int g_iPTActionAsc;

UINT PTActionLoad(LPVOID pParam)
{
	CPTAction* ptAction = (CPTAction*)pParam;
	if(ptAction == NULL)
		return 0;
	int nQueryNo = ptAction->GetCurrentQueryNo();
	//��ȡ����
	if(ptAction->IsCurrentQueryNo(nQueryNo) && !ptAction->m_bExitThread)
	{
		if(ptAction->ReLoadData(nQueryNo))
		{
			ptAction->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		}
		else
		{
			ptAction->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
		}
	}
	ptAction->RemoveThreadNode(nQueryNo);

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPTAction

IMPLEMENT_DYNCREATE(CPTAction, CViewBase)

//##ModelId=49B87B940399
CPTAction::CPTAction()
	: CViewBase(CPTAction::IDD)
{
	//{{AFX_DATA_INIT(CPTAction)
	m_bQueryAcked = TRUE;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = g_PTActionNum;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B95014A
CPTAction::~CPTAction()
{	
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B950128
void CPTAction::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTAction)
	DDX_Control(pDX, IDC_BTN_PTACTION_EXCEL, m_btnExcel);
	DDX_Control(pDX, IDC_BTN_PTACTION_SIGN, m_btnSign);
	DDX_Control(pDX, IDC_BTN_PTACTION_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTACTION_DETAIL, m_btnDetail);
	DDX_Control(pDX, IDC_BTN_PTACTION_ACKALL, m_btnAckAll);
	DDX_Control(pDX, IDC_BTN_PTACTION_ACK, m_btnAck);
	DDX_Control(pDX, IDC_LIST_PTACTION, m_List);
	DDX_Check(pDX, IDC_CHECK_PTACTION_HIDE, m_bQueryAcked);
	DDX_Check(pDX, IDC_CHECK_PTACTION_DEBUG, m_bShowDebug);
	DDX_Check(pDX, IDC_CHECK_PTACTION_RETURN, m_bShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTAction, CViewBase)
	//{{AFX_MSG_MAP(CPTAction)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTACTION, OnCustomDraw)
	ON_BN_CLICKED(IDC_BTN_PTACTION_ACK, OnBtnPtactionAck)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTACTION, OnColumnclickListPtaction)
	ON_BN_CLICKED(IDC_BTN_PTACTION_DETAIL, OnBtnPtactionDetail)
	ON_BN_CLICKED(IDC_BTN_PTACTION_SIGN, OnBtnPtactionSign)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTACTION_ACKALL, OnBtnPtactionAckall)
	ON_BN_CLICKED(IDC_BTN_PTACTION_PRINT, OnBtnPtactionPrint)
	ON_BN_CLICKED(IDC_CHECK_PTACTION_HIDE, OnCheckPtactionHide)
	ON_BN_CLICKED(IDC_CHECK_PTACTION_DEBUG, OnCheckPtactionDebug)
	ON_BN_CLICKED(IDC_CHECK_PTACTION_RETURN, OnCheckPtactionReturn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTACTION, OnDblclkListAction)
	ON_BN_CLICKED(IDC_BTN_PTACTION_EXCEL, OnBtnPtactionExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTAction diagnostics

#ifdef _DEBUG
//##ModelId=49B87B950157
void CPTAction::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B950167
void CPTAction::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTAction message handlers

//##ModelId=49B87B95010E
void CPTAction::OnInitialUpdate() 
{
	//��¼�ؼ�
	RecordRate(IDC_CHECK_PTACTION_HIDE, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTACTION_DEBUG, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTACTION_RETURN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_ACK, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_ACKALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_DETAIL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTACTION_SIGN, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTACTION_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTACTION_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTACTION_P3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTACTION_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	//��ʼ���б�
	InitListCtrl();
	//��ʼ�������ؼ�
	InitControls();
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B95006D
int CPTAction::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	LoadDataFromDB(m_nMaxRow, m_bShowAcked);
	FillListData();*/

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B95007D
int CPTAction::InitListStyle()
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
		//char* arColumn[PTACTION_COL_NUM]={"���","���", "��������", "װ������ʱ��", "��Ϣ����ʱ��", "���ʱ��","��Ϣ����", "��ȷ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < PTACTION_COL_NUM; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; // ���
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2:
				lvCol.cx = 100; //��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 3://
				lvCol.cx = 180; // װ������ʱ��
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 4:
				lvCol.cx = 180; //��Ϣ����ʱ��
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			case 5:
				lvCol.cx = 100; //���ʱ��
				colName = StringFromID(IDS_EVENT_RELATIVETIME);
				break;
			case 6:
				lvCol.cx = 200;	//��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 7:
				lvCol.cx = 80;	//��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx = 80;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(7, TRUE);
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
//##ModelId=49B87B95009C
BOOL CPTAction::LoadListConfig()
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
			WriteLog("PTAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTActionConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTActionConfig", XJ_LOG_LV3);
				WriteLog("PTAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTActionConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() != PTACTION_COL_NUM)
					{
						WriteLog("PTAction::�����������仯,����ȡUIConfig,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B9500AB
BOOL CPTAction::SaveListConfig()
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
			WriteLog("PTAction::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTAction::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTActionConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTActionConfig");
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
				else //�ҵ���PTActionConfig
				{
					//�����Ƿ����ӽڵ�
					if(!pSelected ->hasChildNodes())
					{
						//�Ҳ����ӽڵ�
						WriteLog("δ�ҵ�PTActionConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTActionConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B95008C
int CPTAction::FillListData(int nQueryNo)
{
	WriteLogEx("CPTAction::FillListData, ��ʼ����б�");
	//��m_EventManager�е���������б�
	
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);

	EnterCriticalSection(&m_CriticalOper);
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAction*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	WriteLogEx("CPTAction::FillListData, ����б���������");
	return 0;
}

//##ModelId=49B87B9500BB
BOOL CPTAction::NeedSave()
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
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ�������ؼ�, ���ó�ʼֵ, ��ʾ���
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B9500BC
int CPTAction::InitControls()
{
	/*
	//ȷ�ϰ�ť
	m_btnAck.SetThemeHelper(&m_ThemeHelper);
//	m_btnAck.SetIcon(IDI_ACK);
	m_btnAck.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnAck.SetTooltipText(_T("ȷ����ѡ�¼�"));

	//ȷ�����а�ť
	m_btnAckAll.SetThemeHelper(&m_ThemeHelper);
//	m_btnAckAll.SetIcon(IDI_ACK);
	m_btnAckAll.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnAckAll.SetTooltipText(_T("ȷ�������г����¼�"));

	//��ϸ��Ϣ��ť
	m_btnDetail.SetThemeHelper(&m_ThemeHelper);
//	m_btnDetail.SetIcon(IDI_EVENT);
	m_btnDetail.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnDetail.SetTooltipText(_T("��ʾ�¼���ϸ��Ϣ"));

	//��Ϣ�Ű�ť
	m_btnSign.SetThemeHelper(&m_ThemeHelper);
//	m_btnSign.SetIcon(IDI_SEARCH);
	m_btnSign.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSign.SetTooltipText(_T("��ʾ�¼��ź�������"));

	//��ӡ��ť
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
//	m_btnPrint.SetIcon(IDI_PRINT);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText(_T("��ӡ��ѡ�¼���ϸ��Ϣ"));
*/
	return 0;
}

//##ModelId=49B87B950186
void CPTAction::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTACTION, left_client, IDC_CHECK_PTACTION_RETURN, right_client, bottom_client);
}

//##ModelId=49B87B950197
void CPTAction::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
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
//##ModelId=49B87B9500CB
BOOL CPTAction::LoadDataFromDB(int num, BOOL IsQueryAcked, int nQueryNo)
{
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	if(m_pObj == NULL)
		return FALSE;
		
	WriteLogEx("CPTAction::LoadDataFromDB, ��ʼ��ѯ");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//��Ϊ������Ϣ��Ҫ�Ƚ�����֪���Ƿ��¼��Ƿ񷵻���Ϣ���ȶ�ȡnum*3���¼�
	num *= 3;

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


	Field Field01;
	pApp->m_DBEngine.SetField(sql, Field01, "id", EX_STTP_DATA_TYPE_INT);
	
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
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	//PT_ID
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	//����������ʱ���С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	//str.Format("order by ID DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);

	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

			
	CMemSet pMemset;

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAction::LoadDataFromDB, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTAction::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLogEx(str);
		}

		//����������
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);

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
				EnterCriticalSection(&m_CriticalOper);
				m_EventManager.AddEvent(pEvent);
				LeaveCriticalSection(&m_CriticalOper);
				nChecked++;
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
			str.Format("CPTAction::LoadDataFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
		bReturn = FALSE;

	WriteLogEx("CPTAction::LoadDataFromDB, ��ѯ���,�����˳�");

	return bReturn;
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
//##ModelId=49B87B9500DB
BOOL CPTAction::AddEventToList(CXJEventAction * pEvent, int i)
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
	//2:��������
	m_List.SetItemText(i, 2, pEvent->GetFaultType());
	//3:װ������ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, sTime);
	//4:��Ϣ����ʱ��
	sTime.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
	m_List.SetItemText(i, 4, sTime);
	//5:���ʱ��
	sTime.Format("%d %s", pEvent->m_nRelativeTime, StringFromID(IDS_COMMON_MILLISECOND));
	m_List.SetItemText(i, 5, sTime);
	//5:��Ϣ����
	m_List.SetItemText(i, 6, pEvent->m_sMsg);
	//6:��ȷ��
	m_List.SetItemText(i, 7, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������s
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ����OnSTTP20022()
 ���ܸ�Ҫ����Ӧ20022()����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9501E6
void CPTAction::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
    WriteLog("CPTAction::OnSTTP20022 start", XJ_LOG_LV3);
	CXJEventAction* pParam = (CXJEventAction*)lParam;
	if(pParam == NULL)
	{
		WriteLog("CPTAction::OnSTTP20022 pParam == NULL", XJ_LOG_LV3);
		return;
	}
	if(pParam->m_pSec != m_pObj)
	{
		WriteLog("CPTAction::OnSTTP20022 pParam->m_pSec != m_pObj", XJ_LOG_LV3);
		return;
	}
	
	//����ʾ������Ϣ�����
	if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CPTAction::OnSTTP20022, �����¼�����ʾ", XJ_LOG_LV3);
		return;
	}
	
	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CPTAction::OnSTTP20022, �����¼�����ʾ", XJ_LOG_LV3);
		return;
	}

	if(GetThreadCount() > 0)
	{
		WriteLog("CPTAction::OnSTTP20022, ����������ʷ����,��������", XJ_LOG_LV3);
		return;
	}
	
	//����
	CXJEventAction* pEvent = (CXJEventAction*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�
	m_List.SetRedraw(FALSE);
	if (!AddEventToList(pEvent, 0)) //���뵽��һ��
	{
		WriteLog("CPTAction::OnSTTP20022 ��ӵ��б�ʧ��", XJ_LOG_LV3);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	CheckLineCount();
    WriteLog("CPTAction::OnSTTP20022 end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ, ˢ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B9501F4
void CPTAction::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	int nIndex = (int)wParam;

	ReFillAll();
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������I,II,III���¼�����ɫ, δȷ���¼�������ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9501A5
void CPTAction::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
						else if (g_style == 3)
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

//##ModelId=49B87B9501B5
void CPTAction::OnBtnPtactionAck() 
{
	// TODO: Add your control notification handler code here
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
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
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
				if(pEvent->Ack())
				{
					//ȷ�ϳɹ�,������ʾ
					RefreshAck(pEvent, nIndex);
					//������Ϣ֪ͨ�����
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
					if(pFrame != NULL)
					{
						pFrame->PostMessage(WM_EVENTPT_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
					}
				}
			}
		}
	}
}

/*************************************************************
 �� �� ����RefreshAck()
 ���ܸ�Ҫ��ȷ���¼������ˢ����ʾ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9500EB
void CPTAction::RefreshAck( CXJEventAction* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, PTACTION_COL_NUM-1, StringFromID(IDS_ACK_YES));
	//ˢ��
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);	
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B950005
int CALLBACK CPTAction::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventAction* pEvent1 = (CXJEventAction*)lParam1;
	CXJEventAction* pEvent2 = (CXJEventAction*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","���", "��������", "װ������ʱ��", "��Ϣ����ʱ��", "��Ϣ����", "��ȷ��"
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
	case 3: //װ������ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //��Ϣ����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms2);
		str2.Format("%s.%03d", pEvent2->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms2);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5:
		iResult = pEvent1->m_nRelativeTime - pEvent2->m_nRelativeTime;
		break;
	case 6: //��Ϣ����
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTActionAsc;
	return iResult;
}

//##ModelId=49B87B9501B7
void CPTAction::OnColumnclickListPtaction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTActionAsc = - g_iPTActionAsc;
	}
	else
	{
		g_iPTActionAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B9501C6
void CPTAction::OnBtnPtactionDetail() 
{
	// TODO: Add your control notification handler code here
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
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	//���ѡ��ʱֻ�����һ��
	if(nIndex != -1)
	{
		CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
		if(pEvent != NULL)
		{
			m_EventManager.m_pCurrentEvent = pEvent;
			pApp->ShowEventProp(&m_EventManager, TRUE);
		}
	}
}

/*************************************************************
 �� �� ����OnEventPropAckPT()
 ���ܸ�Ҫ����Ӧ�ɱ�������ҳ��򿪵��¼����ԶԻ������¼�ȷ�ϵ���Ϣ
 �� �� ֵ:  void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B950203
void CPTAction::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
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
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ����Ӧ���¼����򿪵��¼����ԶԻ���ȷ���¼���Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B950206
void CPTAction::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
	}
}

/*************************************************************
 �� �� ����FindLocalPoint()
 ���ܸ�Ҫ������ָ���¼��ڱ��صĵ�ַ, ��Ϊ��������ҳ����¼����¼������¼��ǲ�ͬ�ĵ�ַ
 �� �� ֵ: ��ѯ���ı��ص�ַ,δ�ҵ�����NULL
 ��    ����param1	Ҫ���ҵ��¼�
**************************************************************/
//##ModelId=49B87B9500FB
CXJEventAction* CPTAction::FindLocalPoint( CXJEventAction* pEvent )
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
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ���¼����������¼�ȷ����Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B950215
void CPTAction::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		//֪ͨ�¼����ԶԻ������﷢���˱���ȷ��
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			pFrame->PostMessage(WM_EVENTPT_PASSIVE_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
		}
	}
}

//##ModelId=49B87B9501C8
void CPTAction::OnBtnPtactionSign() 
{
	// TODO: Add your control notification handler code here
	CDlgEventSignal dlg;
	//ѭ����ѡ�¼�,�����ź���
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
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
				//�����¼�
				pEvent->TranslateFData();
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
 �� �� ����ReFillAll()
 ���ܸ�Ҫ������������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9500FD
BOOL CPTAction::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTACTION_LOADING) ->ShowWindow(SW_SHOW);

	//�������
	EnterCriticalSection(&m_CriticalOper);
	m_EventManager.ClearAllEvents();
	LeaveCriticalSection(&m_CriticalOper);
	m_List.DeleteAllItems();
	//������������
	if(MODE_SUB == g_role)
	{
		if(LoadDataFromDB(m_nMaxRow, m_bQueryAcked, 0))
			FillListData(0);
		GetDlgItem(IDC_STATIC_PTACTION_LOADING) ->ShowWindow(SW_HIDE);
	}
	else
		StartThread();
	return FALSE;
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ��ҳ��ر�, ���߳��˳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B950223
void CPTAction::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ��ֹͣ�Լ�������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B950109
void CPTAction::EndOwnerThread()
{
	WriteLog("��ʼ�˳���������ҳ���ȡ�����߳�", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳���������ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

//##ModelId=49B87B9501D4
BOOL CPTAction::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTACTION);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B950138
void CPTAction::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!bActivate)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_FUN_ACTION));
	 }
	 else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_FUN_ACTION));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B9501D7
void CPTAction::OnBtnPtactionAckall() 
{
	/*
	for (int i =0 ;i <10; i++)
	{
	CPTFrame * pFrame = (CPTFrame *)this->GetParentFrame(); 
	CXJEventAction* pEvent = (CXJEventAction*)m_EventManager.GetAtEvent(0);
	pFrame->SendMessage(WM_STTP_20022, 0, (LPARAM)pEvent);
	}
	*/

	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj == NULL)
		return;
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckPTAllEvent(EX_STTP_INFO_PT_ACTION_DATA, m_pObj->m_sID))
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
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B95010A
BOOL CPTAction::InitCOM()
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
//##ModelId=49B87B95010B
void CPTAction::ReleaseCOM()
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

//##ModelId=49B87B9501D9
void CPTAction::OnBtnPtactionPrint() 
{
	// TODO: Add your control notification handler code here
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTACTION_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTACTION_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTAction);
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
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B95010C
CString CPTAction::TranslateSingleKey( CString sKey )
{
	//�ж�������Ч��
	if(sKey == "")
		return "";
	if(m_pObj == NULL)
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
		if(strHead != "$ACTION_EVENT_LIST")
		{
			//������Ƕ����¼��б�, ���ؿ�
			return "";
		}
		CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
		if(pEvent == NULL)
			return "";
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "EVENT_ALARMTYPE$")
		{
			return pEvent->GetAlarmType(); //�¼����
		}
		if(strTail == "EVENT_FAULTTYPE$")
		{
			return pEvent->GetFaultType(); //��������
		}
		if(strTail == "EVENT_TIME1_FULL$")
		{
			CString str;
			str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
			return str; //����ʱ������ֵ
		}
		if(strTail == "EVENT_TIME1$")
		{
			CString str;
			str = pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
			return str; //����ʱ��
		}
		if(strTail == "EVENT_TIME1_MS$")
		{
			CString str;
			str.Format("%03d", pEvent->m_nms);
			return str; //����ʱ�����ֵ
		}
		if(strTail == "EVENT_TIME2_FULL$")
		{
			CString str;
			str.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
			return str; //���ʱ������ֵ
		}
		if(strTail == "EVENT_TIME2$")
		{
			CString str;
			str = pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S");
			return str; //���ʱ��
		}
		if(strTail == "EVENT_TIME2_MS$")
		{
			CString str;
			str.Format("%03d", pEvent->m_nms2);
			return str; //���ʱ�����ֵ
		}
		if(strTail == "EVENT_DEF$")
		{
			return pEvent->m_sMsg; //�¼�����
		}
		if(strTail == "EVENT_ACK$")
		{
			return pEvent->m_nIsAck==1?StringFromID(IDS_ACK_YES):""; //��ȷ��
		}
		if(strTail == "EVENT_SEC_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_sName; //�����豸��
			return "";
		}
		if(strTail == "EVENT_DEVICE_NAME$")
		{
			if(pEvent->m_pSec != NULL)
			{
				if(pEvent->m_pSec->m_pOwner != NULL)
				{
					return pEvent->m_pSec->m_pOwner->m_sName; //һ���豸��
				}
			}
		}
	}
	else
	{
		//�ж��Ƿ���.GetCount()����
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//��.GetCount()����
			if(sKey == "$ACTION_EVENT_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//��ͨ�ؼ���
			if(sKey == "$PT_NAME$") //��������
			{
				return m_pObj->m_sName;
			}
			if(sKey == "$PT_MODEL$") //�����ͺ�
			{
				return m_pObj->m_sModel;
			}
			if(sKey == "$DEVICE_NAME$") //һ���豸
			{
				if(m_pObj->m_pOwner != NULL)
					return m_pObj->m_pOwner->m_sName;	
				return "";
			}
			if(sKey == "$STATION_NAME$") //��վ��
			{
				
				//���ҳ�վ
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CDataEngine* pData = pApp->GetDataEngine();
				CStationObj* pStation = (CStationObj*)pData->FindDevice(m_pObj->m_sStationID, TYPE_STATION);
				if(pStation != NULL)
				{
					return pStation->m_sName;
				}
				else
				{
					return "";
				}
				
			}
		}
	}
	return "";
}

//##ModelId=49B87B9501E4
void CPTAction::OnCheckPtactionHide() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

/*************************************************************
 �� �� ��: CheckLineCount()
 ���ܸ�Ҫ: ����б��е�Ԫ�����Ƿ�����ָ���ĸ���,�����,ɾ������� 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTAction::CheckLineCount()
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

void CPTAction::OnCheckPtactionDebug() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAction::OnCheckPtactionReturn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAction::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
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

/****************************************************
Date:2013/7/26  Author:LYH
������:   StartLoadThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTAction::StartThread()
{
	//������ȡ�߳�
	
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTActionLoad,this,THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("����������ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTAction::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 100000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}

void CPTAction::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("��ʼ�˳�����������ѯ�߳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);

	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION posOld = pos;
		THREADNODE* pNode = (THREADNODE*)m_listThread.GetNext(pos);
		if(pNode != NULL && pNode->nNo == nNo)
		{
			m_listThread.RemoveAt(posOld);
			delete pNode;
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
	
	str.Format("����������ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTAction::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("����������ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTAction::GetFirstNode()
{
	THREADNODE_PTR pReturn  = NULL;
	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	if(pos != NULL)
	{
		pReturn = (THREADNODE_PTR)m_listThread.GetNext(pos);
	}
	LeaveCriticalSection(&m_CriticalSection);
	return pReturn;
}


int CPTAction::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);  
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTAction::ReLoadData( int nQueryNo )
{
	return LoadDataFromDB(m_nMaxRow, m_bQueryAcked, nQueryNo);
}

void CPTAction::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTACTION_LOADING) ->ShowWindow(SW_HIDE);
}


void CPTAction::OnBtnPtactionExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s)-%s.xls", StringFromID(IDS_LATEST_ACTION),m_pObj->m_sName, t.Format("%Y%m%d%H%M%S"));
	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(m_List.ExportExcel(strPath))
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
