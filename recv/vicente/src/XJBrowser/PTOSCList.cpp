// PTOSCList.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTOSCList.h"
#include "GlobalFunc.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

#define OSCSRC_DEV	0
#define OSCSRC_SUBSTAION	1
#define OSCSRC_MAINSTATION	2

#define	COL_MAINRECEIVETIME	5
#define NUM_COL	6

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: PTParse20026()
 ���ܸ�Ҫ: ����20026�����߳�
 �� �� ֵ: 
 ��    ��: param1 ¼�����ش��ڶ���
		   Param2 
**************************************************************/
UINT PTParse20026(LPVOID pParam)
{
	CPTOSCList* ptOscList = (CPTOSCList*)pParam;
	if(ptOscList == NULL)
		return -1;

	//ѭ���ַ�����
	while(1)
	{
		if(ptOscList->m_bExitParse)
		{
			//�˳���־ΪTRUE
			return 0;
		}

		if(ptOscList->m_bDataEnd && (ptOscList->m_listFullData.GetCount() == 0))
		{
			//֪ͨ�̹߳������
			WriteLog("PTParse20026, �������,�����˳�",XJ_LOG_LV3);
			ptOscList->PostMessage(THREAD_WORKEND);
			return 0; //��û�б��ļ��������б��Ĵ�����ɣ��˳�
		}
		
		//����
		int nSize = ptOscList->m_listFullData.GetCount();
		if(nSize == 0)
			Sleep(50);
		
		//ȡ�ö��еĵ�һ������
		POSITION pos = ptOscList->m_listFullData.GetHeadPosition();
		if(pos == NULL)
			continue;
		STTP_FULL_DATA sttpData = (STTP_FULL_DATA)ptOscList->m_listFullData.GetAt(pos);
		
		//��ȡ���ĳɹ�
		try
		{
			CString str;
			str.Format("PTParse20026, ������ing...,ʣ��:%d", nSize);
			WriteLog(str, XJ_LOG_LV3);
			int nCount = sttpData.sttp_body.variant_member.file_list.size();
			for(int i = 0; i < nCount; i++)
			{
				if(ptOscList->m_bExitParse) //�˳�
					return 0;
				//��������
				CXJEventOSC* pOsc = new CXJEventOSC;
				if(!pOsc->LoadFromSttpForPT(sttpData, i))
				{
					delete pOsc;
					pOsc = NULL;
					continue;
				}
				//����ID
				pOsc->m_lID = ptOscList->m_nNum++;
				//���뵽����
				ptOscList->m_EventManager.AddEvent(pOsc);
			}
		}
		catch (...)
		{
			WriteLog("PTParse20026, �������쳣", XJ_LOG_LV3);
		}
		
		zero_sttp_full_data(sttpData);
		//ɾ������
		ptOscList->RemoveHead20026SttpData();
	}

	return 0;
}

UINT QueryData(LPVOID pParam)
{
	CPTOSCList* ptOscList = (CPTOSCList*)pParam;
	if(ptOscList == NULL)
		return -1;
	ptOscList->LoadDataFromDB();
	ptOscList->NotifyThreadEnd();
	ptOscList->PostMessage(THREAD_WORKEND);
	return 0;
}

/** @brief           ����������λ*/
int g_iPTOSCListAsc;

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList

IMPLEMENT_DYNCREATE(CPTOSCList, CViewBase)

//##ModelId=49B87B8F0179
CPTOSCList::CPTOSCList()
	: CViewBase(CPTOSCList::IDD)
{
	//{{AFX_DATA_INIT(CPTOSCList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSelSrc = 0;
	m_nCurrentStatus = 0;
	m_pObj = NULL;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_EventManager.ClearAllEvents();
	m_nFrameCount = 0;
	m_arrFrame.RemoveAll();
	m_EventDownload.RemoveAll();
	m_bUseBreakPoint = FALSE;
	m_nNum = 1;
	m_nOldSortCol = -1;
	/** @brief           ���سɹ���*/
	m_nSuccess = 0;
	/** @brief           ����ʧ����*/
	m_nFaild = 0;
	/** @brief           ����������*/
	m_nPart = 0;
	m_listFullData.RemoveAll();
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	m_pParse20026Thread = NULL;
	m_pQueryThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8F02BF
CPTOSCList::~CPTOSCList()
{
	m_EventManager.ClearAllEvents();
	//�����������
	m_listFullData.RemoveAll();
	WriteLog("CPTOSCList::~CPTOSCList", XJ_LOG_LV3);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8F02AF
void CPTOSCList::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTOSCList)
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_LIST_PTOSCLIST, m_List);
	DDX_Control(pDX, IDC_GIF_PTOSCLIST, m_gif);
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_CALL, m_btnCall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTOSCList, CViewBase)
	//{{AFX_MSG_MAP(CPTOSCList)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CMB_PTOSCLIST_SRC, OnSelchangeCmbPtosclistSrc)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_CALL, OnBtnPtosclistCall)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_DOWN, OnBtnPtosclistDown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_OPEN, OnBtnPtosclistOpen)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTOSCLIST, OnColumnclickListPtosclist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTOSCLIST, OnDblclkListPtosclist)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_COPY, OnBtnPtosclistCopy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTOSCLIST, OnCustomDraw)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTOSCLIST_CALL, OnDoubleclickedBtnPtosclistCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTOSCLIST_DOWN, OnDoubleclickedBtnPtosclistDown)
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(PTOSCLIST_CALLFILE_END, OnCallFileEnd)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(THREAD_WORKEND, OnThreadWorkEnd)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8F02DE
void CPTOSCList::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8F02EE
void CPTOSCList::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList message handlers

//##ModelId=49B87B8F02A0
void CPTOSCList::OnInitialUpdate() 
{
	//��¼λ��
	//����
	RecordRate(IDC_STATIC_PTOSCLIST_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTOSCLIST_SRC, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSCLIST_START, 0, left_client, top_client);
	RecordRate(IDC_DP_PTOSCLIST_START, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSCLIST_END, 0, left_client, top_client);
	RecordRate(IDC_DP_PTOSCLIST_END, 0, left_client, top_client);

	//��ť
	RecordRate(IDC_BTN_PTOSCLIST_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_DOWN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_OPEN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_COPY, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOL_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOL_P2, 0, left_client, top_client);
	
	RecordRate(IDC_GIF_PTOSCLIST, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTLIST, 0, left_client, top_client);
	RecordRate(IDC_STATIC_NUM_PTLIST, 1, left_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//��ʼ����ť�ؼ�
	InitControls();
	//��ʼ���б�
	InitListCtrl();
	//��ʼ��������Դ������
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTOSCLIST_SRC);
	if(pCmb != NULL)
	{
		pCmb->AddString(StringFromID(IDS_MODEL_RELAY));
		if (g_role != MODE_SUB)
		{
			pCmb->AddString( StringFromID(IDS_SOURCE_SUBSTATIONDB));
			pCmb->AddString( StringFromID(IDS_SOURCE_LOCALDB));
			pCmb->SetCurSel(1);
			m_nSelSrc = 1;
		}
		else
		{
			pCmb->SetCurSel(0);
			m_nSelSrc = 0;
		}
	}
	//����ʱ���
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	if(pTime2 == NULL)
		return;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
 	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//���÷�Χ
	CTime tmMin(1972, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//����ʱ��
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));

	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

//##ModelId=49B87B8F02FD
void CPTOSCList::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTOSCLIST, left_client, IDC_DP_PTOSCLIST_START, right_client, bottom_client);
	
}

/*************************************************************
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ����ť�ؼ�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0203
int CPTOSCList::InitControls()
{
	/*
	//¼����ť
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ�¼���б�"));
	
	//���ذ�ť
	m_btnDown.SetThemeHelper(&m_ThemeHelper);
//	m_btnDown.SetIcon(IDI_SEARCH);
	m_btnDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnDown.SetTooltipText(_T("����ָ����¼���ļ�"));

	//�򿪰�ť
	m_btnOpen.SetThemeHelper(&m_ThemeHelper);
//	m_btnOpen.SetIcon(IDI_WAVE);
	m_btnOpen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOpen.SetTooltipText(_T("��ָ����¼���ļ�"));
	*/
	return 0;
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8F0213
int CPTOSCList::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
//	FillListData();

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8F0222
int CPTOSCList::InitListStyle()
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
		//char* arColumn[NUM_COL]={"���", "����","��С", "����ʱ��", "������", "��վ����ʱ��"};
	
		//�����п�,��С����
		for (int nCol=0; nCol < NUM_COL ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; //���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 250; // ����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 100; // ��С
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 3://
				lvCol.cx = 200; // ����ʱ��
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4:
				lvCol.cx = 80; //������
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			case 5:
				lvCol.cx = 200; //��վ����ʱ��
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	m_List.SetColumnHide(COL_MAINRECEIVETIME, TRUE);
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
//##ModelId=49B87B8F0233
BOOL CPTOSCList::LoadListConfig()
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
			WriteLog("PTOSCList::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTOSCListConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTOSCListConfig");
				WriteLog("PTOSCList::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTOSCListConfig�µ�����");
					WriteLog("PTOSCList::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if(pChild->Getlength() != NUM_COL)
					{
						WriteLog("PTOSCList::�б����������,��ʹ��Ĭ���з��");
						bResult = FALSE;
					}
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

//##ModelId=49B87B8F0242
BOOL CPTOSCList::SaveListConfig()
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
			WriteLog("PTOSC::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTOSC::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTOSCListConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTOSCListConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTOSCListConfig");
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
						WriteLog("δ�ҵ�PTOSCListConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���PTOSCListConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B8F0232
int CPTOSCList::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	if(m_nSelSrc == OSCSRC_MAINSTATION)
	{
		m_List.SetColumnHide(COL_MAINRECEIVETIME, FALSE);
	}
	else
		m_List.SetColumnHide(COL_MAINRECEIVETIME, TRUE);
	//���ListCtrl����
	CXJEventOSC * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventOSC*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8F0251
BOOL CPTOSCList::NeedSave()
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

//##ModelId=49B87B8F031C
void CPTOSCList::OnSelchangeCmbPtosclistSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTOSCLIST_SRC);
	MYASSERT(pCmb);
	int nSel = pCmb->GetCurSel();
	if(nSel >= 0)
	{
		m_nSelSrc = nSel;
	}
}

//##ModelId=49B87B8F032C
void CPTOSCList::OnBtnPtosclistCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���������ȡ���ٻ�
	MYASSERT(m_pObj);
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}

	if(m_nSelSrc == OSCSRC_MAINSTATION)
	{
		if(m_nCurrentStatus == 0)
		{
			//��ѯ
			m_nCurrentStatus = 1;
			StartQueryThread();
		}
		else 
		{
			//ȡ��
			m_nCurrentStatus = 0;
			EndQueryThread();
		}
		//���ݵ�ǰ״̬���¿ؼ�������
		UpdateControlsEnable();
		return;
	}
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//����״̬, �����ť��ʾҪ�ٻ�������
		//���뱨����������
		CString str="";
		SetDlgItemText(IDC_STATIC_NUM_PTLIST, str);

		STTP_FULL_DATA sttpData;
		
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
		MYASSERT(pTime);
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
		MYASSERT(pTime2);
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//��֯20025����
		
		if (g_role != MODE_SUB)
		{
			if(!pApp->m_SttpEngine.BuildDataFor20025WaveFileList(sttpData, m_pObj->m_sID, m_nSelSrc, tmStart, tmEnd))
			{
				//��֯����ʧ��
				return;
			}
		}
		else
		{
			if(!pApp->m_SttpEngine.BuildDataFor20025WaveFileList(sttpData, m_pObj->m_sID, 0, tmStart, tmEnd))
			{
				//��֯����ʧ��
				return;
			}
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
				str.Format("����[%s]�����ٻ�¼���б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ�¼���б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ�¼���б�
			m_nCurrentStatus = 1;
			
			//������ʱ��
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nWavFileListTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//ɾ��֮ǰ���б�
			m_List.DeleteAllItems();
			m_EventManager.ClearAllEvents();
			m_nNum = 1;
			
			//���ݵ�ǰ״̬���¿ؼ�������
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����ٻ�¼���б��ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);

			//��������20026�߳�
			StartParse20026Thread();
		}
	}
	else 
	{
		//�����ٻ�������
		//ȡ���ٻ�
		CancelCall();
		//�˳�����20026�߳�
		EndParse20026Thread();
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0261
void CPTOSCList::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//��ť
		m_btnCall.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_PTOSCLIST_CALL, StringFromID(IDS_FUNC_CALL_LIST));
		m_btnDown.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_PTOSCLIST_DOWN, StringFromID(IDS_FUNC_CALL_FILE));
		m_btnOpen.EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTOSCLIST_COPY)->EnableWindow(TRUE);
		
		//������
		GetDlgItem(IDC_CMB_PTOSCLIST_SRC) ->EnableWindow(TRUE);
		//ʱ��
		GetDlgItem(IDC_DP_PTOSCLIST_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_PTOSCLIST_END) ->EnableWindow(TRUE);

		//ֹͣ����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  > 0)
	{
		//��ť
		m_btnCall.EnableWindow(FALSE);
		m_btnDown.EnableWindow(FALSE);
		m_btnOpen.EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTOSCLIST_COPY)->EnableWindow(FALSE);
		
		//������
		GetDlgItem(IDC_CMB_PTOSCLIST_SRC) ->EnableWindow(FALSE);
		//ʱ��
		GetDlgItem(IDC_DP_PTOSCLIST_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_PTOSCLIST_END) ->EnableWindow(FALSE);
		
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTLIST, str);

		if(m_nCurrentStatus == 1)
		{
			//�ٻ�¼���б�
			m_btnCall.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_PTOSCLIST_CALL, CANCEL_CALL);
		}
		else if(m_nCurrentStatus == 2)
		{
			//����¼���ļ�
			m_btnDown.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_PTOSCLIST_DOWN, CANCEL_CALL);
		}
	}
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0271
void CPTOSCList::CancelCall()
{
	//��ǰ�ǿ��е�, �˲�����Ч
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���뱨����������
	STTP_FULL_DATA sttpData;
	//ֹͣ��ʱ��
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ�¼���б�
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20025, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//��������¼���ļ�
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20042, m_pObj->m_sID, 0);
		//��մ������б�
		m_EventDownload.RemoveAll();
		//��ս��
		m_CallFileResult = "";
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����OnSTTP20026()
 ���ܸ�Ҫ����Ӧ20026(¼���б�����֪ͨ)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F035C
void CPTOSCList::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�¼���б��״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20026����
	if(pSttpData->sttp_head.uMsgID != 20026)
	{
		//������
		CString str;
		str.Format("CPTOSCList::OnSTTP20026 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTOSCList::OnSTTP20026 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	
	//���뵽��������
	WriteLog("���뱨�ĵ�����");
	AddTail20026SttpData(*pSttpData);
	WriteLog("���뱨�ĵ��������");

	CString strEnd = "";
	strEnd.Format("������־Ϊ %d", pSttpData->sttp_head.uMsgEndFlag);
	WriteLog(strEnd);
	//�ж��Ƿ��к���֡
	if(pSttpData->sttp_head.uMsgEndFlag == 0)
	{
		//ֹͣ��ʱ��
		KillTimer(m_nTimer);
		//֪ͨ�̱߳���û��
		m_bDataEnd = TRUE;

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_SUCCESS);
	}
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ����һ���¼�����Ϣ���뵽�б�
 �� �� ֵ: ����ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ����param1	Ҫ������¼�
		   Param2	�����λ��
**************************************************************/
//##ModelId=49B87B8F0272
BOOL CPTOSCList::AddEventToList( CXJEventOSC * pEvent, int i )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//"���", "����","��С", "����ʱ��", "������"

	CString str;
	str.Format("%d", pEvent->m_lID);
	//0:���
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;

	//1:����
	m_List.SetItemText(i, 1, pEvent->m_sFileName);
	
	//2:��С
	str = "-";
	if(pEvent->m_nFileSize > 0)
		str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(i, 2, str);

	//3:����ʱ��
	str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, str);

	//4:������
	m_List.SetItemText(i, 4, pEvent->GetDownloadString());

	//5.��վ����ʱ��
	m_List.SetItemText(i, 5, pEvent->m_tmMainReceiveTime.Format("%Y-%m-%d %H:%M:%S"));

	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ����TranslateMultiFrame()
 ���ܸ�Ҫ��������֡�Ķ�ֵ����
 �� �� ֵ: 
 ��    ����param1	��������
		   Param2
**************************************************************/
//##ModelId=49B87B8F0280
void CPTOSCList::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	if(pSttpData == NULL)
		return;
	
	//������ֵ����
	try
	{
		//��ȡÿ��ֵ,���뵽�¶�ֵ����
		int nCount = pSttpData->sttp_body.variant_member.file_list.size();
		for(int i = 0; i < nCount; i++)
		{
			CXJEventOSC * pEvent = new CXJEventOSC;
			if(!pEvent->LoadFromSttpForPT(*pSttpData, i)) //ָ������
			{
				//��ȡ����ʧ��
				delete pEvent;
				pEvent = NULL;
				continue;
			}
			//���뵽����
			m_EventManager.AddEvent(pEvent);
			//���뵽�б�,���뵽��һ��
			AddEventToList(pEvent, 0);
		}
		
		//�ж��Ƿ�����ȡ��,����������Ԫ����=��֡������Ϊ����ȡ��
		if(m_arrFrame.GetSize() == m_nFrameCount)
		{
			//��ȡ���,�رն�ʱ��
			KillTimer(m_nTimer);
			//������ʾ
			for(int i = 0; i < m_EventManager.GetCount(); i++)
			{
				CXJEventOSC* pEvent = (CXJEventOSC*)m_EventManager.GetAtEvent(i);
				AddEventToList(pEvent, 0);
			}
			//�������,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
		}
	}
	catch (...)
	{
		WriteLog("CPTOSCList::TranslateMultiFrame, ����", 3);
		return;
	}
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ�򿪱�������ҳ��Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F036B
void CPTOSCList::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;

	//���б����ɾ��
	m_List.DeleteAllItems();
	//���ڴ���ɾ��
	m_EventManager.ClearAllEvents();
	m_nNum = 0;
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//����ٻ���ʱ
	SetDlgItemText(IDC_STATIC_TIME_PTLIST, "");
	SetDlgItemText(IDC_STATIC_NUM_PTLIST, "");
}

//##ModelId=49B87B8F032E
void CPTOSCList::OnBtnPtosclistDown() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, ����¼���ļ���ȡ������
	MYASSERT(m_pObj);

	if( m_nSelSrc == OSCSRC_MAINSTATION )
	{
		AfxMessageBox( StringFromID(IDS_DOWNOSC_BY_LOCAL));
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
		{
			return;
		}
	}
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		/** @brief           ���سɹ���*/
		m_nSuccess = 0;
		/** @brief           ����ʧ����*/
		m_nFaild = 0;
		/** @brief           ����������*/
		m_nPart = 0;

		//ȡ�õ�ǰѡ���¼���б�
		int nCount = m_List.GetSelectedCount();
		if(nCount < 1)
		{
			AfxMessageBox( StringFromID(IDS_DOWNOSC_SELLIST));
			return;
		}
		//��ѡ���¼���б���뵽���ٻ�����
		m_EventDownload.RemoveAll();
		m_CallFileResult = "";

		int nIndex = -1;
		for(int i = 0; i < nCount; i++)
		{
			nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
			if(nIndex != -1)
			{
				CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
				if(pEvent != NULL)
				{
					//���뵽���ٻ�����
					m_EventDownload.AddTail(pEvent);
				}
			}
		}
		//��ʼ����
		//�ı�״̬
		m_nCurrentStatus = 2;
		//������ʾ
		UpdateControlsEnable();

		//����ļ�
		CheckDownload();

		//���ص�һ��
		if(m_EventDownload.GetCount() > 0)
		{
			CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
			if(pOsc != NULL)
			{
				DownloadOSCFile(pOsc);
			}
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_DOWNLOAD_FINISHED));
			//�ı�״̬
			m_nCurrentStatus = 0;
			//������ʾ
			UpdateControlsEnable();
		}
	}
	else
	{
		//ȡ���ٻ�
		CancelCall();
	}
}

/*************************************************************
 �� �� ����DownloadOSCFile()
 ���ܸ�Ҫ������ָ��¼���б��¼���ļ�
 �� �� ֵ: void
 ��    ����param1	ָ��¼���б�
		   Param2
**************************************************************/
void CPTOSCList::DownloadOSCFile( CXJEventOSC* pEvent )
{
	//��Ӧ��ť���²���, ����¼���ļ���ȡ���ٻ�
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	//���뱨����������
	STTP_FULL_DATA sttpData;

	int nOffset = 0;
	if(m_bUseBreakPoint)
	{
		nOffset = pEvent->m_nOffset;
	}
	
	CString strFileName;
	if (g_role != MODE_SUB)
	{
		strFileName = pEvent->m_sFilePath + pEvent->m_sFileName;
		if(strFileName.GetLength() >= 100)
			strFileName = pEvent->m_sFileName;
	}
	else
	{
		strFileName = pEvent->m_sFileName;
	}

	//CString strFileName = pEvent->m_sFileName;
	//��֯20042����
	if(!pApp->m_SttpEngine.BuildDataFor20042DownloadWaveFile(sttpData, m_pObj->m_sID, strFileName, pEvent->m_nDataSrc, nOffset, pEvent->m_nFileNum))
	{
		//��֯����ʧ��
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		pEvent->m_nDownloaded = 0;
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
		return;
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
			str.Format("%s�����ٻ�¼���ļ�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pEvent->m_nDownloaded = 0;
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("%s�����ٻ�¼���ļ�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pEvent->m_nDownloaded = 0;
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
			return;
		}
	}
	else
	{
		pEvent->m_nDownloaded = 0;
		//������ʱ��
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nWavFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//�޸�ֵ
		CString strValue;
		strValue.Format("%d", nOffset);
		ChangeDownloadedValue(pEvent, nOffset);
	}
}

/*************************************************************
 �� �� ����OnCallFileEnd()
 ���ܸ�Ҫ���ٻ�һ��¼���ļ����, ���ڱ��ٻ����Ǵ��ٻ�����ĵ�һ��, ��ɺ��ڴ˴��������Ƴ�
 �� �� ֵ: void
 ��    ����param1
		   Param2	�ٻ����. 1-����ʧ��. 2-����ʧ��. 3-��ʱ. 4.-װ�ò�֧��. 5-�ɹ�
**************************************************************/
//##ModelId=49B87B8F036F
void CPTOSCList::OnCallFileEnd( WPARAM wparam, LPARAM lParam )
{
	//û�д�������¼���ļ���״̬, ��������Ϣ
	if(m_nCurrentStatus != 2)
		return;
	if(m_EventDownload.GetCount() < 1)
	{
		//û�д������ļ�, ���������
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	}
	CXJEventOSC* pEvent = m_EventDownload.GetHead();
	//��¼��ɽ��
	m_CallFileResult += StringFromID(IDS_CALLOPER_CALL);
	m_CallFileResult += "[";
	m_CallFileResult += pEvent->m_sFileName;
	m_CallFileResult += "]";
	m_CallFileResult += "\t";
	int nResult = (int)lParam;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	switch(pEvent->m_nDownloaded)
	{
	case 0:
		m_CallFileResult += StringFromID(IDS_COMMON_FAILED);
		m_nFaild++;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_FAILD);
		break;
	case 1:
		m_CallFileResult += StringFromID(IDS_COMMON_SUCCESS);
		m_nSuccess++;
		strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_SUCCESS);
		break;
	case 2:
		m_CallFileResult += StringFromID(IDS_DOWNLOAD_PARTIAL);
		m_nPart++;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_FAILD);
		break;
	default:
		break;
		WriteLog("�ļ�����״̬�쳣!");
	}

	ChangeValue(pEvent);
	m_CallFileResult += "\r\n";
	//�Ƴ���һ���������б�
	m_EventDownload.RemoveHead();
	//����Ƿ��д������б�
	if(m_EventDownload.GetCount() < 1)
	{
		//��û�е������б�,��ʾȫ���������
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	//	AfxMessageBox(m_CallFileResult);
		CString str;
		str.Format("%s: %d\n%s: %d\n%s: %d", StringFromID(IDS_COMMON_SUCCESS),m_nSuccess, 
			StringFromID(IDS_COMMON_FAILED), m_nFaild, StringFromID(IDS_DOWNLOAD_PARTIAL), m_nPart);
		AfxMessageBox(str);
	}
	else
	{
		//������һ��
		CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
		DownloadOSCFile(pOsc);
	}
}

/*************************************************************
 �� �� ����OnSTTP20043()
 ���ܸ�Ҫ����Ӧ¼���ļ��������֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2)
	{
		//û�д�������¼���ļ���״̬,��������
		return;
	}

	//û�е�����, ��������
	if(m_EventDownload.GetCount() < 1)
		return;
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20043����
	if(pSttpData->sttp_head.uMsgID != 20043)
	{
		//������
		CString str;
		str.Format("CPTOSCList::OnSTTP20043 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTOSCList::OnSTTP20043 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}

	//����Ƿ�ǰҪ���ص��ļ�
	CString strFileName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	//ȡ�õ�һ�������ص�¼���б�
	CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
	MYASSERT(pEvent);
	strFileName.Replace("/", "\\");
	if(strFileName != pEvent->GetFileRelatePath())
	{
		//���Ǳ�¼���б���ļ�
		WriteLog("CPTOSCList::OnSTTP20043 ���͵�¼���ļ����뱾¼���б��ļ���ƥ��");
		CString str;
		str.Format("����·��Ϊ%s, ����·��Ϊ%s", strFileName, pEvent->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		//return;
	}

	pEvent->m_nDownloaded = pSttpData->sttp_body.nStatus;
	pEvent->m_nOffset = 0;
	CString strStatus;
	strStatus.Format("%d", pEvent->m_nDownloaded);
	WriteLog("20043 ���Ľ��Ϊ" + strStatus, 3);
	if (g_role != MODE_SUB)
	{
		if(pEvent->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag > 0)
		{
			//��¼�˵�λ��
			pEvent->m_nDownloaded = 2;
			pEvent->m_nOffset = pSttpData->sttp_body.nFlag;
		}
		else if(pEvent->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag <= 0)
		{
			//����ʧ��
			pEvent->m_nDownloaded = 0;
			WriteLog("�����ļ�ʧ��!", 3);
		}
		if(pEvent->m_nDownloaded == 1 && pEvent->m_nFileSize == 0)
		{
			//���سɹ�����¼�ļ���С
			pEvent->m_nFileSize = pSttpData->sttp_body.nFlag;
			WriteLog("�����ļ��ɹ�!", 3);
		}
	}
	else
	{
		pEvent->m_nDownloaded = 1;
	}

	//�رռ�ʱ��
	KillTimer(m_nTimer);
	//���ͳɹ���Ϣ
	this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ20069,����ʧ�ܱ���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//û�д�������¼���ļ����ٻ�¼���б��״̬,��������
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
		CString str;
		str.Format("CPTSetting::OnSTTP20069 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTOSCList::OnSTTP20069 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}

	if(m_nCurrentStatus == 1)
	{
		//�ٻ�¼���б�
		if(pSttpData->sttp_body.nMsgId != 20025)
			return;
		//�Ǳ����ٻ���ʧ�ܱ���
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�رռ�ʱ��
		KillTimer(m_nTimer);
		//�˳��߳�
		EndParse20026Thread();
		CString str;
		str.Format("����[%s]�ٻ�¼���б�ʧ��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);

		//��ʾʧ��
		AfxMessageBox( StringFromID(IDS_CALL_FAIL));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�ٻ�¼���ļ�
		if(pSttpData->sttp_body.nMsgId != 20042)
			return;
		//ȡ�õ�һ�������ص�¼���б�
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		if(pEvent == NULL)
			return;
		
		//ʧ��
		pEvent->m_nDownloaded = 0;
		pEvent->m_nOffset = 0;
		//�رռ�ʱ��
		KillTimer(m_nTimer);
		//������Ϣ
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125, װ�ò�֧�ֹ��ܱ���
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//û�д�������¼���ļ����ٻ�¼���б��״̬,��������
		return;
	}
		
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20125����
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20125 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20125 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	//�رռ�ʱ��
	KillTimer(m_nTimer);
	
	if(m_nCurrentStatus == 1)
	{
		//�ٻ�¼���б�
		if(pSttpData->sttp_body.nMsgId != 20025)
			return;
		//�Ǳ����ٻ���ʧ�ܱ���
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�رռ�ʱ��
		KillTimer(m_nTimer);
		//�˳��߳�
		EndParse20026Thread();

		CString str;
		str.Format("����[%s]�ٻ�¼���б�ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);

		//��ʾʧ��
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�ٻ�¼���ļ�
		if(pSttpData->sttp_body.nMsgId != 20042)
			return;
		//ȡ�õ�һ�������ص�¼���б�
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		if(pEvent == NULL)
			return;
		
		//ʧ��
		pEvent->m_nDownloaded = 0;
		pEvent->m_nOffset = 0;
		//�رռ�ʱ��
		KillTimer(m_nTimer);
		//������Ϣ
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
	}
}

void CPTOSCList::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//�ı�״̬
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			//�˳�����20026�߳�
			EndParse20026Thread();
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);
			//��ʾ
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
		else if(m_nCurrentStatus == 2)
		{
			//���������Ϣ
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 3);
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTLIST, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����ChangeValue()
 ���ܸ�Ҫ���ı�¼���б��Ƿ������ص���ʾ
 �� �� ֵ: 
 ��    ����param1	ָ��¼���б�
		   Param2
**************************************************************/
//##ModelId=49B87B8F0292
void CPTOSCList::ChangeValue( CXJEventOSC* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	//����λ��
	int nIndex = -1;
	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//�Ҳ���λ��
	if(nIndex < 0)
		return;
	//��С
	if(pEvent->m_nDownloaded == 1) //�ɹ�
	{ 
		CString str;
		str.Format("%d Byte", pEvent->m_nFileSize);
		m_List.SetItemText(nIndex, 2, str);
	}
	//�ı�ֵ
	m_List.SetItemText(nIndex, 4, pEvent->GetDownloadString());
	//ˢ��
	m_List.Update(nIndex);
}

//##ModelId=49B87B8F0340
void CPTOSCList::OnBtnPtosclistOpen() 
{
	// TODO: Add your control notification handler code here
	if(m_List.GetSelectedCount() != 1)
	{
		//ֻ��ѡ��һ��¼���ļ�
		AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex != -1)
	{
		CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
		if(pEvent == NULL)
			return;
		if(pEvent->m_nDownloaded  != 1)
		{
			AfxMessageBox( StringFromID(IDS_OPENFAIL_NOTDOWN));
			return;
		}
		CString strFilePath = pEvent->GetFilePath();
		//���ļ�
		OpenWaveFile(strFilePath, GetSafeHwnd());
	}
}

//##ModelId=49B87B8F03AA
void CPTOSCList::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	m_pObj = (CSecObj*)lParam;

	//���б����ɾ��
	m_List.DeleteAllItems();
	//���ڴ���ɾ��
	m_EventManager.ClearAllEvents();
	m_nNum = 0;
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//����ٻ���ʱ
	SetDlgItemText(IDC_STATIC_TIME_PTLIST, "");
}

//##ModelId=49B87B8F034B
BOOL CPTOSCList::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTOSCLIST);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8F034E
void CPTOSCList::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����CheckDownload()
 ���ܸ�Ҫ�����Ҫ���ص��ļ�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F029F
void CPTOSCList::CheckDownload()
{
	if(m_EventDownload.GetCount() < 1)
		return;
	BOOL bHasSuccess = FALSE; //�Ƿ���������ص��ļ�
	BOOL bHasHalf = FALSE; //�Ƿ�����������ص��ļ�

	POSITION pos = m_EventDownload.GetHeadPosition();
	while(pos != NULL)
	{
		CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetNext(pos);
		if(pOsc == NULL)
			continue;
		if(pOsc->m_nDownloaded == 1)
		{
			bHasSuccess = TRUE;
		}
		if(pOsc->m_nDownloaded == 2)
			bHasHalf = TRUE;
	}

	if(bHasSuccess)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_AGAIN), MB_YESNO) == IDYES)
		{
			
		}
		else
		{
			//����������, ȥ�������ص��ļ�
			POSITION pos = m_EventDownload.GetHeadPosition();
			while(pos != NULL)
			{
				POSITION oldPos = pos;
				CXJEventOSC* pFile = (CXJEventOSC*)m_EventDownload.GetNext(pos);
				if(pFile == NULL)
					continue;
				if(pFile->m_nDownloaded == 1)
				{
					m_EventDownload.RemoveAt(oldPos);
				}
			}
		}
	}
	if(bHasHalf)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_RESUME), MB_YESNO) == IDYES)
		{
			m_bUseBreakPoint = TRUE;
		}
		else
		{
			m_bUseBreakPoint = FALSE;
		}
	}
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B8F01AA
int CALLBACK CPTOSCList::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventOSC* pOSC1 = (CXJEventOSC*)lParam1;
	CXJEventOSC* pOSC2 = (CXJEventOSC*)lParam2;
	if(pOSC1 == NULL || pOSC2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���", "����","��С", "����ʱ��", "������"
	switch(nCol)
	{
	case 0: //���
		iResult = pOSC1->m_lID - pOSC2->m_lID;
		break;
	case 1: //����
		str1 = pOSC1->m_sFileName;
		str2 = pOSC2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //��С
		iResult = pOSC1->m_nFileSize - pOSC2->m_nFileSize;
		break;
	case 3: //����ʱ��
		str1 = pOSC1->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pOSC2->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //������
		iResult = pOSC1->m_nDownloaded - pOSC2->m_nDownloaded;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTOSCListAsc;
	return iResult;
}

//##ModelId=49B87B8F0350
void CPTOSCList::OnColumnclickListPtosclist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTOSCListAsc = - g_iPTOSCListAsc;
	}
	else
	{
		g_iPTOSCListAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//û�д����ٻ�״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		WriteLog("CPTOSCList::OnSTTP20157 ����Ϊ��", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//������
		WriteLog("CPTOSCList::OnSTTP20157 ���ĺŴ���, ��20157", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		WriteLog("CPTOSCList::OnSTTP20157 ���Ǳ�װ�ñ���", XJ_LOG_LV3);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//�ٻ�¼���б�
		if(pSttpData->sttp_body.nMsgId != 20025)
		{
			WriteLog("CPTOSCList::OnSTTP20157 ���ĺŴ���, ��20025", XJ_LOG_LV3);
			return;
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//¼���ļ�����Ҫ��(20042)
		if(pSttpData->sttp_body.nMsgId != 20042)
		{
			WriteLog("CPTOSCList::OnSTTP20157 ���ĺŴ���, ��20042", XJ_LOG_LV3);
			return;
		}
	}
	
	//��ʱ��Ϊ
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: ��ʼ��׼ʱ������趨���
	//�رն�ʱ��
	KillTimer(m_nTimer);
	//������ʱ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ�¼���б�				
		int nTimeOut = pData->m_nWavFileListTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�¼���ļ�
		WriteLog("CPTOSCList::OnSTTP20157 ˢ�³�ʱʱ��", XJ_LOG_LV3);
		int nTimeOut = pData->m_nWavFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//�����������ص��ļ�������������ֵ
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		MYASSERT(pEvent);
		CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
		ChangeDownloadedValue(pEvent, sValue);
	}
}

/*************************************************************
 �� �� ����ChangeDownloadedValue()
 ���ܸ�Ҫ���޸��������ص��ļ������������ݴ�С
 �� �� ֵ: 
 ��    ����param1	�������ص��ļ�
		   Param2	���������ݴ�С
**************************************************************/
void CPTOSCList::ChangeDownloadedValue( CXJEventOSC* pEvent, CString sValue )
{
	//���������Ч��
	MYASSERT(pEvent);
	//����λ��
	int nIndex = -1;
	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//�Ҳ���λ��
	if(nIndex < 0)
		return;
	CString str;
	str.Format("%s K", sValue);
	//�ı�ֵ
	m_List.SetItemText(nIndex, 4, str);
	//ˢ��
	m_List.Update(nIndex);
}

void CPTOSCList::OnDblclkListPtosclist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected > -1)
	{
		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//��¼��
		if(pEvent->m_nDownloaded  != 1)
		{
			if(AfxMessageBox(StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_YESNOCANCEL)!=IDYES)
				return;
			//��ѡ���¼���б���뵽���ٻ�����
			/** @brief           ���سɹ���*/
			m_nSuccess = 0;
			/** @brief           ����ʧ����*/
			m_nFaild = 0;
			/** @brief           ����������*/
			m_nPart = 0;
			m_EventDownload.RemoveAll();
			m_CallFileResult = "";
			//���뵽���ٻ�����
			m_EventDownload.AddTail(pEvent);
			//�ı�״̬
			m_nCurrentStatus = 2;
			//������ʾ
			UpdateControlsEnable();
			
			//����ļ�
			CheckDownload();
			//���ص�һ��
			if(m_EventDownload.GetCount() > 0)
			{
				CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
				if(pOsc != NULL)
					DownloadOSCFile(pOsc);
			}
			else
			{
				AfxMessageBox(StringFromID(IDS_DOWNLOAD_FINISHED));
				//�ı�״̬
				m_nCurrentStatus = 0;
				//������ʾ
				UpdateControlsEnable();
			}
			
		}
		else
		{
			CString strFilePath = pEvent->GetFilePath();
			//���ļ�
			OpenWaveFile(strFilePath, GetSafeHwnd());
		}
	}
	*pResult = 0;
}

void CPTOSCList::OnBtnPtosclistCopy() 
{
	// TODO: Add your control notification handler code here
		//ȡ����ѡ�ļ�
	CStringArray arrSelectFile;
	arrSelectFile.RemoveAll();
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pOSC = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pOSC == NULL)
				continue;
			if(pOSC->m_nDownloaded  != 1)
			{
				AfxMessageBox(StringFromID(IDS_OPENFAIL_NOTDOWN));
				return;
			}

			CString strName = pOSC->m_sFileName;
			//ȥ��׺��
			int nFind = strName.ReverseFind('.');
			if(nFind >= 0)
			{
				strName = strName.Left(nFind);
			}
			//���ļ������뵽����
			BOOL bExist = FALSE;
			for(int k = 0; k < arrSelectFile.GetSize(); k++)
			{
				CString strValue = arrSelectFile.GetAt(k);
				if(strValue == strName)
				{
					bExist = TRUE;
					break;
				}
			}
			if(!bExist)
			{
				//���
				arrSelectFile.Add(strName);
			}
		}
	}

	//�Ƿ����ļ�
	if(arrSelectFile.GetSize() <= 0)
		return;

	//ȡ���û�ѡ���ļ���
	CString strNewDir = GetSelectDir();
	if(strNewDir.IsEmpty())
		return;

	//�����ļ�
	for(i = 0; i<arrSelectFile.GetSize(); i++)
	{
		CFileFind finder; //�ļ�������
		CString strPath;
		strPath = GetOSCFileDir(FALSE);
		if(strPath.Right(1) != "\\")
			strPath += "\\";
		CString strFilter;
		strFilter.Format("%s.*", arrSelectFile.GetAt(i));
		strPath += strFilter; //ָ���ļ���
		BOOL bWorking = finder.FindFile(strPath);

		if (!bWorking)
		{
			strPath = GetOSCFileDir(TRUE);
			if(strPath.Right(1) != "\\")
			{
				strPath += "\\";
			}
			strFilter.Format("%s.*", arrSelectFile.GetAt(i));
			strPath += strFilter; //ָ���ļ���
			bWorking = finder.FindFile(strPath);			
		}

		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			
			// skip . and .. files;
			if (finder.IsDots())
			{
				continue;
			}
			else if(finder.IsDirectory()) // ��Ŀ¼
			{
				continue;
			}
			else //���ļ�
			{
				CString strName = finder.GetFileName(); //�ļ���,����׺
				//Ŀ��·��
				CString strNewPath = strNewDir+strName;
				//Ŀ¼
				CString strOldPath = finder.GetFilePath();
				//����
				CString strLog;
				if(CopyFile(strOldPath, strNewPath, TRUE))
				{
					strLog.Format("�����ļ��ɹ�,Ŀ��·��%s", strNewPath);
				}
				else
				{
					strLog.Format("�����ļ�ʧ��,Ŀ��·��%s", strNewPath);
				}
			}
		}
		finder.Close();
	}

	//����
	if(AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_SUCCESS), MB_OKCANCEL) == IDOK)
	{
		ShellExecute(GetSafeHwnd(), "open", "Explorer.exe", strNewDir, NULL, SW_SHOW); 
	}
}

/*************************************************************
 �� �� ��: GetOSCFileDir()
 ���ܸ�Ҫ: �õ�¼���ļ����Ŀ¼
 �� �� ֵ: ¼���ļ����Ŀ¼
 ��    ��: param1 
		   Param2 
**************************************************************/
CString CPTOSCList::GetOSCFileDir(BOOL bNewPath)
{
	MYASSERT_STRING(m_pObj);
	//�õ���ַ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//Ŀ¼���'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//��֯·��
	CString strFullPath = strDownDir + m_pObj->m_sStationID;
	
	if (bNewPath)
	{
		strFullPath += "\\Collection";
	}
	
	if (m_pObj->m_nType == TYPE_PROTECT)
	{
		strFullPath += "\\����\\";
	}
	else
	{
		strFullPath += "\\¼����\\";
	}
	
	strFullPath += m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";
	
	return strFullPath;
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: Add20026SttpData()
 ���ܸ�Ҫ: ��20026���ļ��뵽����β
 �� �� ֵ: 
 ��    ��: param1 20026��������
		   Param2 
**************************************************************/
void CPTOSCList::AddTail20026SttpData( STTP_FULL_DATA& sttpData )
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		m_listFullData.AddTail(sttpData);
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: RemoveHead20026SttpData()
 ���ܸ�Ҫ: ɾ��20026��������ĵ�һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::RemoveHead20026SttpData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		m_listFullData.RemoveHead();
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: Parse20026()
 ���ܸ�Ҫ: ����20026��������
 �� �� ֵ: 
 ��    ��: param1 �������ݶ���
		   Param2 
**************************************************************/
void CPTOSCList::Parse20026( STTP_FULL_DATA* pSttpData )
{
    //¼���ļ��б�����֪ͨ(20026)
	//�ж���Ч��
	MYASSERT(pSttpData);
	int nCount = pSttpData->sttp_body.variant_member.file_list.size();
	for(int i = 0; i < nCount; i++)
	{
		//��������
		CXJEventOSC* pOsc = new CXJEventOSC;
		if(!pOsc->LoadFromSttpForPT(*pSttpData, i))
		{
			delete pOsc;
			pOsc = NULL;
			continue;
		}
		//����ID
		pOsc->m_lID = m_nNum++;
		//���뵽����
		m_EventManager.AddEvent(pOsc);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: OnThreadWorkEnd()
 ���ܸ�Ҫ: �̹߳�����������֪ͨ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnThreadWorkEnd( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTOSCList::OnThreadWorkEnd, ��ʼ���������ʾ");
	int nCount = m_EventManager.GetCount();
	CString str;
	str.Format("%s: %d", StringFromID(IDS_EVENTCOUNT_TOTAL),nCount);
	SetDlgItemText(IDC_STATIC_NUM_PTLIST, str);
			
	//���List
	FillListData();

	//ˢ��״̬
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: StartParse20026Thread()
 ���ܸ�Ҫ: ��������20026�����߳�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::StartParse20026Thread()
{
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	//�����������
	m_listFullData.RemoveAll();
	//����
	m_pParse20026Thread = AfxBeginThread(PTParse20026,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pParse20026Thread != NULL)
	{
		m_pParse20026Thread->m_bAutoDelete = FALSE;
		m_pParse20026Thread->ResumeThread();
		WriteLog("CPTOSCList::StartParse20026Thread, �߳������ɹ�");
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: EndParse20026Thread()
 ���ܸ�Ҫ: ǿ��ֹͣ20026��������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::EndParse20026Thread()
{
	//�˳��߳�
	if(m_pParse20026Thread != NULL)
	{
		//�̻߳�û�˳�ȥ
		m_bExitParse = TRUE;	//�����˳���־
		WaitForSingleObject(m_pParse20026Thread->m_hThread, INFINITE);
		delete m_pParse20026Thread;
		m_pParse20026Thread = NULL;
		WriteLog("CPTOSCList::EndParse20026Thread, �߳��˳��ɹ�");
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: OnPTFrameClose()
 ���ܸ�Ҫ: ��Ӧ���������˳���Ϣ���˳��߳�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	//�˳��߳�
	EndParse20026Thread();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: OnWRFrameClose()
 ���ܸ�Ҫ: ��Ӧ¼���������˳���Ϣ���˳��߳�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	//�˳��߳�
	EndParse20026Thread();
}

void CPTOSCList::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

void CPTOSCList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTOSCList::OnDoubleclickedBtnPtosclistCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTOSCList::OnDoubleclickedBtnPtosclistDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTOSCList::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_REMOTEFILE));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_REMOTEFILE));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/****************************************************
Date:2014/5/20  Author:LYH
������:   LoadDataFromDB	
����ֵ:   BOOL	
���ܸ�Ҫ: 
*****************************************************/
BOOL CPTOSCList::LoadDataFromDB()
{
	if(!m_pObj)
		return FALSE;
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	MYASSERT_BOOL(pTime);
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	MYASSERT_BOOL(pTime2);
	CTime tmStart;
	pTime->GetTime(tmStart);
	CTime tmEnd;
	pTime2->GetTime(tmEnd);

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
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "filename", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "trigger_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "ms", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "savepath", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "Bfaultfile", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "fault_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "trigger_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "reclose_time", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "Fault_distance", EX_STTP_DATA_TYPE_STRING);
	
	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "Is_Ack", EX_STTP_DATA_TYPE_INT);
	
	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "Reverse2", EX_STTP_DATA_TYPE_STRING);

	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "download", EX_STTP_DATA_TYPE_INT);

	Field Field16;
	pApp->m_DBEngine.SetField(sql, Field16, "Reverse1", EX_STTP_DATA_TYPE_STRING);
	
	//����
	Condition condition1;
	CString sc;
	sc.Format("pt_id='%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, sc);
	Condition con2;
	sc.Format("trigger_time>'%s' AND trigger_time<'%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"), tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con2, sc);

	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if( m_bExitQuery )
		return FALSE;

	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CPTOSCList::LoadDataFromDB, ��ѯʧ��", XJ_LOG_LV3);
		return FALSE;
	}

	if( m_bExitQuery )
		return FALSE;
	
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTOSCList::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventOSC * pEvent = new CXJEventOSC;
			if(pEvent->LoadFromDB(&pMemset))
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

			if( i%10 == 0)
				Sleep(1);
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTOSCList::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2014/5/20  Author:LYH
������:   StartQueryThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTOSCList::StartQueryThread()
{
	m_bExitQuery = FALSE;
	
	//����
	m_pQueryThread = AfxBeginThread(QueryData,this,THREAD_PRIORITY_BELOW_NORMAL);
	
	WriteLog("CPTOSCList::StartQueryThread, �߳������ɹ�");
}

/****************************************************
Date:2014/5/20  Author:LYH
������:   EndQueryThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTOSCList::EndQueryThread()
{
	//�˳��߳�
	if(m_pQueryThread != NULL)
	{
		//�̻߳�û�˳�ȥ
		m_bExitQuery = TRUE;	//�����˳���־
		//WaitForSingleObject(m_pQueryThread->m_hThread, INFINITE);
		m_pParse20026Thread = NULL;
		WriteLog("CPTOSCList::EndQueryThread, �߳��˳��ɹ�");
	}
}

/****************************************************
Date:2014/5/20  Author:LYH
������:   NotifyThreadEnd	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTOSCList::NotifyThreadEnd()
{
	m_pQueryThread = NULL;
	m_bExitQuery = FALSE;
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	WriteLog("CPTOSCList::NotifyThreadEnd, �߳��˳��ɹ�");
}
