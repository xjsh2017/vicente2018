// PTAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTAlarm.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           ����������λ*/
int g_iPTAlarmAsc;

UINT PTAlarmLoad(LPVOID pParam)
{
	CPTAlarm* ptAlarm = (CPTAlarm*)pParam;
	if(ptAlarm == NULL)
		return 0;
	int nQueryNo = ptAlarm->GetCurrentQueryNo();
	//��ȡ����
	if(ptAlarm->IsCurrentQueryNo(nQueryNo) && !ptAlarm->m_bExitThread)
	{
		if(ptAlarm->ReLoadData(nQueryNo))
			ptAlarm->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptAlarm->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}
	ptAlarm->RemoveThreadNode(nQueryNo);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPTAlarm

IMPLEMENT_DYNCREATE(CPTAlarm, CViewBase)

//##ModelId=49B87B9400AC
CPTAlarm::CPTAlarm()
	: CViewBase(CPTAlarm::IDD)
{
	//{{AFX_DATA_INIT(CPTAlarm)
	m_bQueryAcked = TRUE;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = g_PTAlarmNum;
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

//##ModelId=49B87B940261
CPTAlarm::~CPTAlarm()
{
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B940232
void CPTAlarm::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTAlarm)
	DDX_Control(pDX, IDC_BTN_PTALARM_EXCEL, m_btnExcel);
	DDX_Control(pDX, IDC_BTN_PTALARM_SIGN, m_btnSign);
	DDX_Control(pDX, IDC_BTN_PTALARM_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTALARM_DETAIL, m_btnDetail);
	DDX_Control(pDX, IDC_BTN_PTALARM_ACKALL, m_btnAckAll);
	DDX_Control(pDX, IDC_BTN_PTALARM_ACK, m_btnAck);
	DDX_Control(pDX, IDC_LIST_PTALARM, m_List);
	DDX_Check(pDX, IDC_CHECK_PTALARM_HIDE, m_bQueryAcked);
	DDX_Check(pDX, IDC_CHECK_PTALARM_DEBUG, m_bShowDebug);
	DDX_Check(pDX, IDC_CHECK_PTALARM_RETURN, m_bShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTAlarm, CViewBase)
	//{{AFX_MSG_MAP(CPTAlarm)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTALARM, OnCustomDraw)
	ON_BN_CLICKED(IDC_BTN_PTALARM_ACK, OnBtnPtalarmAck)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTALARM, OnColumnclickListPtalarm)
	ON_BN_CLICKED(IDC_BTN_PTALARM_DETAIL, OnBtnPtalarmDetail)
	ON_BN_CLICKED(IDC_BTN_PTALARM_SIGN, OnBtnPtalarmSign)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTALARM_ACKALL, OnBtnPtalarmAckall)
	ON_BN_CLICKED(IDC_BTN_PTALARM_PRINT, OnBtnPtalarmPrint)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_HIDE, OnCheckPtalarmHide)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_DEBUG, OnCheckPtalarmDebug)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_RETURN, OnCheckPtalarmReturn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTALARM, OnDblclkListAction)
	ON_BN_CLICKED(IDC_BTN_PTALARM_EXCEL, OnBtnPtalarmExport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B940271
void CPTAlarm::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B940280
void CPTAlarm::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm message handlers

//##ModelId=49B87B940226
void CPTAlarm::OnInitialUpdate() 
{

	//��¼�ؼ�
	RecordRate(IDC_CHECK_PTALARM_HIDE, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTALARM_DEBUG, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTALARM_RETURN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_ACK, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_ACKALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_DETAIL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_SIGN, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_LOADING, 0, mid_client, top_client);

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
//##ModelId=49B87B940186
int CPTAlarm::InitListCtrl()
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
//##ModelId=49B87B940196
int CPTAlarm::InitListStyle()
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

			m_List.InsertColumn(i,&lvCol, bHide);
		}
	}
	else
	{
		//��������ʧ��
		//char* arColumn[5]={"���","���","�澯����ʱ��", "��Ϣ����", "��ȷ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 5 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1:
				lvCol.cx = 80; //���
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 180; // �澯����ʱ��
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3://
				lvCol.cx = 200; // ��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 4:
				lvCol.cx = 80;
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
		m_List.SetColumnHide(4, TRUE);
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
//##ModelId=49B87B9401B5
BOOL CPTAlarm::LoadListConfig()
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
			WriteLog("PTAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTAlarmConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTAlarmConfig", XJ_LOG_LV3);
				WriteLog("PTAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTAlarmConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTAlarm::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B9401C5
BOOL CPTAlarm::SaveListConfig()
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
			WriteLog("PTAlarm::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTAlarm::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTAlarmConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTAlarmConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTAlarmConfig");
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
						WriteLog("δ�ҵ�PTAlarmConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTAlarmConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B9401A5
int CPTAlarm::FillListData(int m_nQueryNo)
{
	WriteLog("CPTAlarm::FillListData ��ʼ���", XJ_LOG_LV3);
	
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	
	CXJEventAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAlarm*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTAlarm::FillListData ��ʼ���", XJ_LOG_LV3);
	
	return 0;
}

//##ModelId=49B87B9401C6
BOOL CPTAlarm::NeedSave()
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
//##ModelId=49B87B9401D4
int CPTAlarm::InitControls()
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

//##ModelId=49B87B940290
void CPTAlarm::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTALARM, left_client, IDC_CHECK_PTALARM_RETURN, right_client, bottom_client);
}

//##ModelId=49B87B9402AF
void CPTAlarm::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����LoadDataFromDB()
 ���ܸ�Ҫ�������ݿ��ж�ȡ�����¼�, �����������ʾ��������, 
			ÿһ����Ϣ�½�һ���¼����󱣴����¼����������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FLASE
**************************************************************/
//##ModelId=49B87B9401E4
BOOL CPTAlarm::LoadDataFromDB(int num, BOOL IsQueryAcked, int nQueryNo)
{
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	if(m_pObj == NULL)
		return FALSE;

	WriteLog("CPTAlarm::LoadDataFromDB, ��ʼ��ѯ");
	
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
	Field Field111;
	bzero(&Field111, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field111.FieldName, str, str.GetLength());
	Field111.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field111);


	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "alarm_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field32;
	pApp->m_DBEngine.SetField(sql, Field32, "value", EX_STTP_DATA_TYPE_INT);
	
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
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("is_ack = 0");
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

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
		
	//����������ʱ���С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLogEx("CPTAlarm::LoadDataFromDB, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTAlarm::LoadDataFromDB, ��ȡ��%d������", nCount);
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
			CXJEventAlarm * pEvent = new CXJEventAlarm;
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
			str.Format("CViewAlarm::LoadDataFromDB, Ҫ��ʾ%d������,ֻ�ҵ�%d������", m_nMaxRow, nChecked);
			WriteLog(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTAlarm::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTAlarm::LoadDataFromDB, ��ѯ���");

	return bReturn;
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�,���Ҫ�����λ���Ѿ���������, ԭ�ȵ�����
			�����Զ�����,����������ȷλ��	
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
//##ModelId=49B87B9401F4
BOOL CPTAlarm::AddEventToList(CXJEventAlarm * pEvent, int i)
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
	
	//2:����ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 2, sTime);
	//3:��Ϣ����
	m_List.SetItemText(i, 3, pEvent->m_sMsg);
	//4:��ȷ��
	m_List.SetItemText(i, 4, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ����OnSTTP20018()
 ���ܸ�Ҫ����Ӧ20018(�����澯�¼�����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9402F2
void CPTAlarm::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	if(pParam == NULL)
		return;
	if(pParam->m_pSec != m_pObj)
		return;

	//����ʾ������Ϣ�����
	if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CPTAlarm::OnSTTP20018, �����¼�����ʾ");
		return;
	}

	//����ʾ������Ϣ
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CPTAlarm::OnSTTP20018, �����¼�����ʾ");
		return;
	}

	if(GetThreadCount() > 0)
	{
		WriteLog("CPTAlarm::OnSTTP20018, ������������,��������", XJ_LOG_LV3);
		return;
	}

	//����
	CXJEventAlarm* pEvent = (CXJEventAlarm*)pParam->Clone();
	if(pEvent == NULL)
		return;

	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
}

/*************************************************************
�� �� ����OnPTFrameOpen()
���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ,ˢ������ 
�� �� ֵ: void
��    ����param1	��Ϣ����
Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B9402FD
void CPTAlarm::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;
	
	ReFillAll();
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������δȷ���¼�������ɫ 
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9402B1
void CPTAlarm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

					if (g_role == MODE_SUB)
					{
						//�õ��¼�����
						CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}

					if(nSubItem == 1)
					{
						//�õ��¼�����, Ҫ��֤EventManager����б�˳������ͼlist�е�˳��һ��
						CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
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

//##ModelId=49B87B9402C0
void CPTAlarm::OnBtnPtalarmAck() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
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
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
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
 ���ܸ�Ҫ���¼�ȷ�Ϻ�ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	ȷ�ϵ��¼�
		   Param2	�¼�λ��
**************************************************************/
//##ModelId=49B87B940203
void CPTAlarm::RefreshAck( CXJEventAlarm* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, 4, StringFromID(IDS_ACK_YES));
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
//##ModelId=49B87B940177
int CALLBACK CPTAlarm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventAlarm* pEvent1 = (CXJEventAlarm*)lParam1;
	CXJEventAlarm* pEvent2 = (CXJEventAlarm*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","���","�澯����ʱ��", "��Ϣ����", "��ȷ��"
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
	case 2: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //��Ϣ����
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //�Ƿ���ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTAlarmAsc;
	return iResult;
}

//##ModelId=49B87B9402CE
void CPTAlarm::OnColumnclickListPtalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTAlarmAsc = - g_iPTAlarmAsc;
	}
	else
	{
		g_iPTAlarmAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B9402D2
void CPTAlarm::OnBtnPtalarmDetail() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
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
		CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
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
//##ModelId=49B87B94030D
void CPTAlarm::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
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
//##ModelId=49B87B94031C
void CPTAlarm::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
	}
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ����Ӧ���¼����������¼�ȷ����Ϣ
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B94031F
void CPTAlarm::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//�����ڱ��صĵ�ַ
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
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

/*************************************************************
 �� �� ����FindLocalPoint()
 ���ܸ�Ҫ������ָ���¼��ڱ��صĵ�ַ, ��Ϊ��������ҳ����¼����¼������¼��ǲ�ͬ�ĵ�ַ
 �� �� ֵ: ��ѯ���ı��ص�ַ,δ�ҵ�����NULL
 ��    ����param1	Ҫ���ҵ��¼�
**************************************************************/
//##ModelId=49B87B940213
CXJEventAlarm* CPTAlarm::FindLocalPoint( CXJEventAlarm* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventAlarm* pReturn = NULL;
	//��Ϊ������ID��֪�������,ֻ�ܱȽ϶����豸ID, ����ʱ�����Ϣ
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventAlarm* pTemp = (CXJEventAlarm*)m_EventManager.GetAtEvent(i);
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

//##ModelId=49B87B9402DE
void CPTAlarm::OnBtnPtalarmSign() 
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
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
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
 ���ܸ�Ҫ�����������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B940215
BOOL CPTAlarm::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_SHOW);
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
		GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_HIDE);
	}
	else
		StartThread();
	return TRUE;
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ�����ڹر�ʱ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B94032D
void CPTAlarm::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���ر��Լ�������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B940216
void CPTAlarm::EndOwnerThread()
{
	WriteLog("��ʼ�˳������澯ҳ���ȡ�����߳�", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳������澯ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

//##ModelId=49B87B9402E0
BOOL CPTAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTALARM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B940251
void CPTAlarm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_FUN_ALARM));
	}
	else
 		pApp->SetAppTile( StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_FUN_ALARM));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B9402E3
void CPTAlarm::OnBtnPtalarmAckall() 
{
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
	if(AckPTAllEvent(EX_STTP_INFO_PT_ALARM_DATA, m_pObj->m_sID))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_EventManager.m_EventList.GetAt(i);
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
//##ModelId=49B87B940222
BOOL CPTAlarm::InitCOM()
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
//##ModelId=49B87B940223
void CPTAlarm::ReleaseCOM()
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
//##ModelId=49B87B940224
CString CPTAlarm::TranslateSingleKey( CString sKey )
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
		if(strHead != "$ALARM_EVENT_LIST")
		{
			//������Ƕ����¼��б�, ���ؿ�
			return "";
		}
		CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
		if(pEvent == NULL)
			return "";
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "EVENT_ALARMTYPE$")
		{
			return pEvent->GetAlarmType(); //�¼����
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
			if(sKey == "$ALARM_EVENT_LIST.GetCount()$")
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

//##ModelId=49B87B9402EE
void CPTAlarm::OnBtnPtalarmPrint() 
{
	// TODO: Add your control notification handler code here
	
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTALARM_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTALARM_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTAlarm);
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

//##ModelId=49B87B9402F0
void CPTAlarm::OnCheckPtalarmHide() 
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
void CPTAlarm::CheckLineCount()
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

void CPTAlarm::OnCheckPtalarmDebug() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAlarm::OnCheckPtalarmReturn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAlarm::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//�õ����ж�Ӧ���¼�
		CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

void CPTAlarm::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTAlarmLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("�����澯��ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTAlarm::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 1000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}

void CPTAlarm::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("��ʼ�˳������澯��ѯ�߳�,��ѯ��:%d", nNo);
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

	str.Format("�����澯��ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTAlarm::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("�����澯��ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTAlarm::GetFirstNode()
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


int CPTAlarm::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);  
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTAlarm::ReLoadData( int nQueryNo )
{
	return LoadDataFromDB(m_nMaxRow, m_bQueryAcked, nQueryNo);
}

void CPTAlarm::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_HIDE);
}


void CPTAlarm::OnBtnPtalarmExport() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s)-%s.xls", StringFromID(IDS_LATEST_ALARM),m_pObj->m_sName, t.Format("%Y%m%d%H%M%S"));
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
