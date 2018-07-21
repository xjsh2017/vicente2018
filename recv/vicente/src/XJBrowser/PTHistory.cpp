// PTHistory.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTHistory.h"
#include "GlobalFunc.h"

#include "DlgSelFault.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: PTParse20116()
 ���ܸ�Ҫ: ����20116�����߳�
 �� �� ֵ: 
 ��    ��: param1 ¼�����ش��ڶ���
		   Param2 
**************************************************************/
UINT PTParse20116(LPVOID pParam)
{
	CPTHistory* ptHistory = (CPTHistory*)pParam;
	if(ptHistory == NULL)
		return -1;

	//ѭ���ַ�����
	while(1)
	{
		if(ptHistory->m_bExitParse)
		{
			//�˳���־ΪTRUE
			return 0;
		}

		if(ptHistory->m_bDataEnd && (ptHistory->m_listFullData.GetCount() == 0))
		{
			//֪ͨ�̹߳������
			WriteLog("PTParse20116, �������,�����˳�",XJ_LOG_LV3);
			ptHistory->PostMessage(THREAD_WORKEND);
			return 0; //��û�б��ļ��������б��Ĵ�����ɣ��˳�
		}
		
		//����
		int nSize = ptHistory->m_listFullData.GetCount();
		if(nSize == 0)
			Sleep(50);
		
		//ȡ�ö��еĵ�һ������
		POSITION pos = ptHistory->m_listFullData.GetHeadPosition();
		if(pos == NULL)
			continue;
		STTP_FULL_DATA sttpData = (STTP_FULL_DATA)ptHistory->m_listFullData.GetAt(pos);
		
		
		CString strlog;
		strlog.Format("��֡��:%d,��ǰ֡��:%d",sttpData.sttp_head.uMsgType,sttpData.sttp_head.uMsgLengthType);
		WriteLog(strlog, XJ_LOG_LV3);
		//��ȡ���ĳɹ�
		try
		{
			CString str;
			str.Format("PTParse20116, ������ing...,ʣ��:%d", nSize);
			WriteLog(str, XJ_LOG_LV3);
			int nType = sttpData.sttp_body.nEventType;
			int nEventCount = sttpData.sttp_body.variant_member.dataflat_data.record_list.size();
			if(nType == 1 || nType == 5)
			{
				//����
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//�˳���־ΪTRUE
						return 0;
					}
					ptHistory->TranslateHistoryAction(&sttpData, i);
				}
				
			}
			else if(nType == 2 || nType == 6)
			{
				//�澯
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//�˳���־ΪTRUE
						return 0;
					}
					ptHistory->TranslateHistoryAlarm(&sttpData, i);
				}
			}
			else if(nType == 3)
			{
				//������
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//�˳���־ΪTRUE
						return 0;
					}
					ptHistory->TranslateHistoryDI(&sttpData, i);
				}
			}
		}
		catch (...)
		{
			WriteLog("PTParse20116, �������쳣", XJ_LOG_LV3);
		}
		
		zero_sttp_full_data(sttpData);
		//ɾ������
		ptHistory->RemoveHead20116SttpData();
	}

	return 0;
}

/** @brief           ����������λ*/
int g_iPTHistoryAsc;
/////////////////////////////////////////////////////////////////////////////
// CPTHistory

IMPLEMENT_DYNCREATE(CPTHistory, CViewBase)

//##ModelId=49B87B9001A7
CPTHistory::CPTHistory()
	: CViewBase(CPTHistory::IDD)
{
	//{{AFX_DATA_INIT(CPTHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nSelSrc = 1;
	m_nEventType = 0;
	m_nCurrentStatus = 0;
	m_pObj = NULL;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_EventList.RemoveAll();
	m_arrDigital.RemoveAll();
	m_nNum = 1;
	m_nOldSortCol = -1;
	m_nActionIndex = 0;

	m_listFullData.RemoveAll();
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	m_pParse20116Thread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B9001C5
CPTHistory::~CPTHistory()
{
	ClearAllDI();
	ClearAllEvent();
	//�����������
	m_listFullData.RemoveAll();
	DeleteCriticalSection(&m_CriticalSection);
	WriteLog("CPTHistory::~CPTHistory", XJ_LOG_LV3);
}

//##ModelId=49B87B90032C
void CPTHistory::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTHistory)
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_NULL, m_btnSelNull);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_ALL, m_btnSelAll);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_LIST_PTHISTORY, m_List);
	DDX_Control(pDX, IDC_GIF_PTHISTORY, m_gif);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_CALL, m_btnCall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTHistory, CViewBase)
	//{{AFX_MSG_MAP(CPTHistory)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_CALL, OnBtnPthistoryCall)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CMB_PTHISTORY_SRC, OnSelchangeCmbPthistorySrc)
	ON_CBN_SELCHANGE(IDC_CMB_PTHISTORY_TYPE, OnSelchangeCmbPthistoryType)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTHISTORY, OnColumnclickListPthistory)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SAVE, OnBtnPthistorySave)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_NULL, OnBtnPthistorySelNull)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_ALL, OnBtnPthistorySelAll)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTHISTORY, OnCustomDraw)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_COPY, OnBtnPthistorySelCopy)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTHISTORY_CALL, OnDoubleclickedBtnPthistoryCall)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_FAULT, OnBtnPthistoryFault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20116, OnSTTP20116)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(THREAD_WORKEND, OnThreadWorkEnd)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTHistory diagnostics

#ifdef _DEBUG
//##ModelId=49B87B90034B
void CPTHistory::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B90035B
void CPTHistory::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTHistory message handlers

//##ModelId=49B87B90031D
void CPTHistory::OnInitialUpdate() 
{
	//��¼λ��
	//������Դ
	RecordRate(IDC_STATIC_PTHISTORY_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTHISTORY_SRC, 0, left_client, top_client);
	//�¼�����
	RecordRate(IDC_STATIC_PTHISTORY_TYPE, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTHISTORY_TYPE, 0, left_client, top_client);
	//��ʼʱ��
	RecordRate(IDC_STATIC_PTHISTORY_START, 0, left_client, top_client);
	RecordRate(IDC_DP_PTHISTORY_START, 0, left_client, top_client);
	//����ʱ��
	RecordRate(IDC_STATIC_PTHISTORY_END, 0, left_client, top_client);
	RecordRate(IDC_DP_PTHISTORY_END, 0, left_client, top_client);
	//�ٻ���ť
	RecordRate(IDC_BTN_PTHISTORY_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SAVE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_ALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_NULL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_COPY, 0, left_client, top_client);
	//gif
	RecordRate(IDC_GIF_PTHISTORY, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTHIS, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTHIS_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTHIS_P2, 0, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//��ʼ��������Դ������
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_SRC);
	if(pCmb == NULL)
		return;

	pCmb->AddString(StringFromID(IDS_MODEL_RELAY));
	pCmb->AddString(StringFromID(IDS_MODEL_SUBSTATION));
	pCmb->SetCurSel(1);

	//pCmb->EnableWindow(FALSE);
	
	//��ʼ���¼�����������
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_TYPE);
	if(pCmb == NULL)
		return;
	pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_ACTION));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_ALARM));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_DI));
	pCmb->SetCurSel(0);

	//����ʱ���
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
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

	//��ʼ����ť�ؼ�
	InitControls();
	//��ʼ���б��
	InitListCtrl();
}

//##ModelId=49B87B90037A
void CPTHistory::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTHISTORY, left_client, IDC_DP_PTHISTORY_END, right_client, bottom_client);
}

/*************************************************************
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ�������ؼ�(��List���)
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B900246
int CPTHistory::InitControls()
{
	/*
	//�ٻ���ť
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ���ʷ�¼�"));

	//���水ť
	m_btnSave.SetThemeHelper(&m_ThemeHelper);
//	m_btnSave.SetIcon(IDI_SAVE);
	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSave.SetTooltipText(_T("������ѡ�¼�"));
	if (g_role)
	{
		m_btnSave.EnableWindow(FALSE);
	}

	//ѡ������
	m_btnSelAll.SetThemeHelper(&m_ThemeHelper);
//	m_btnSelAll.SetIcon(IDI_SEARCH);
	m_btnSelAll.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSelAll.SetTooltipText(_T("ѡ�������¼�"));
	if (g_role)
	{
		m_btnSelAll.EnableWindow(FALSE);
	}

	//ȡ������ѡ��
	m_btnSelNull.SetThemeHelper(&m_ThemeHelper);
//	m_btnSelNull.SetIcon(IDI_SEARCH);
	m_btnSelNull.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSelNull.SetTooltipText(_T("ȡ������ѡ���¼�"));
	if (g_role)
	{
		m_btnSelNull.EnableWindow(FALSE);
	}
*/
	return 1;
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B900251
int CPTHistory::InitListCtrl()
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
//##ModelId=49B87B900261
int CPTHistory::InitListStyle()
{
	//��ͼ���б�, ���������и�
// 	CImageList   m_l;   
// 	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
// 	m_List.SetImageList(&m_l,LVSIL_SMALL);
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
		//char* arColumn[7]={"���","�¼�����", "����ʱ��", "�¼�����", "�¼�ֵ", "���ʱ��", "�ź�������ʱ��"};
	
		//�����п�,��С����
		for (int nCol=0; nCol < 7 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 100; // �¼�����
				colName = StringFromID(IDS_COMMON_TYPE);
				break;
			case 2://
				lvCol.cx = 200; // ����ʱ��
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3:
				lvCol.cx = 200; //�¼�����
				colName = StringFromID(IDS_EVENT_DESC);
				break;
			case 4:
				lvCol.cx = 80; //�¼�ֵ
				colName = StringFromID(IDS_EVENT_VALUE);
				break;
			case 5:
				lvCol.cx = 180; //���ʱ��
				colName = StringFromID(IDS_COMMON_FINISHTIME);
				break;
			case 6:
				lvCol.cx = 180; //�ź�������ʱ��
				colName = StringFromID(IDS_EVENT_SIGNALTIME);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
	//	m_List.SetColumnHide(0, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B900280
BOOL CPTHistory::LoadListConfig()
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
			pSelected = pDoc->selectSingleNode("//PTHistoryConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTHistoryConfig", XJ_LOG_LV3);
				WriteLog("PTHistoryList::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTHistoryConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTHistoryConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B900290
BOOL CPTHistory::SaveListConfig()
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
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("PTOSC::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTHistoryConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTHistoryConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTHistoryConfig");
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
						WriteLog("δ�ҵ�PTHistoryConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTHistoryConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B900271
int CPTHistory::FillListData()
{
	CString str;
	str.Format("һ����[%d]���¼�", m_nNum - 1);
	WriteLog(str, XJ_LOG_LV3);

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//���ListCtrl����
	POSITION pos = m_EventList.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_EventList.GetNext(pos);
		if(pEvent == NULL)
			continue;
		//������ʾ
		AddEventToList(pEvent);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B90029F
BOOL CPTHistory::NeedSave()
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

//##ModelId=49B87B90038A
void CPTHistory::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002AF
void CPTHistory::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//�ٻ���ť���ָ�Ϊ�ٻ�
		SetDlgItemText(IDC_BTN_PTHISTORY_CALL, StringFromID(IDS_CALLOPER_CALL));
		
		//������
//		GetDlgItem(IDC_CMB_PTHISTORY_SRC) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTHISTORY_TYPE) ->EnableWindow(TRUE);
		//ʱ��ѡ���
		GetDlgItem(IDC_DP_PTHISTORY_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_PTHISTORY_END) ->EnableWindow(TRUE);
		//���水ť
		if (g_role != MODE_SUB)
		{
			m_btnSave.EnableWindow(TRUE);
			m_btnCopy.EnableWindow(TRUE);
			m_btnSelAll.EnableWindow(TRUE);
			m_btnSelNull.EnableWindow(TRUE);
		}
		//ֹͣ����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		//�ٻ���ť���ָ�Ϊȡ���ٻ�
		SetDlgItemText(IDC_BTN_PTHISTORY_CALL, CANCEL_CALL);
		
		//������
//		GetDlgItem(IDC_CMB_PTHISTORY_SRC) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTHISTORY_TYPE) ->EnableWindow(FALSE);
		//ʱ��ѡ���
		GetDlgItem(IDC_DP_PTHISTORY_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_PTHISTORY_END) ->EnableWindow(FALSE);
		//���水ť
		if (g_role != MODE_SUB)
		{
			m_btnSave.EnableWindow(FALSE);
			m_btnCopy.EnableWindow(FALSE);
			m_btnSelAll.EnableWindow(FALSE);
			m_btnSelNull.EnableWindow(FALSE);
		}
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTHIS, str);
	}
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002BF
void CPTHistory::CancelCall()
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
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20105, m_pObj->m_sID, 0);
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ���ʷ�¼�������ȡ��", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B9002C0
BOOL CPTHistory::IsVaild20069( int nMsgID )
{
	if(20105 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ���ʷ�¼�
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B900399
void CPTHistory::OnBtnPthistoryCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ȡ���ٻ�
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
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
		STTP_FULL_DATA sttpData;
		
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
		if(pTime == NULL)
			return;
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
		if(pTime2 == NULL)
			return;
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//��֯20105����
		if(!pApp->m_SttpEngine.BuildDataFor20105HistoryEvent(sttpData, m_pObj->m_sID, m_nEventType, tmStart, tmEnd, m_nSelSrc))
		{
			//��֯����ʧ��
			WriteLog("��֯20105����ʧ��", XJ_LOG_LV3);
			return;
		}
		else
		{
			WriteLog("��֯20105���ĳɹ�", XJ_LOG_LV3);
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
				str.Format("����[%s]�����ٻ���ʷ�¼�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ���ʷ�¼�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
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
			int nTimeOut = pData->m_nHistoryEventTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//���ݵ�ǰ״̬���¿ؼ�������
			UpdateControlsEnable();
			
			//ɾ��֮ǰ���б�
			m_List.DeleteAllItems();
			ClearAllEvent();
			m_nNum = 1;

			//�����߳�
			StartParse20116Thread();

			CString str;
			str.Format("����[%s]�����ٻ���ʷ�¼����ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else 
	{
		//�����ٻ�������
		//�˳��߳�
		EndParse20116Thread();
		//ȡ���ٻ�
		CancelCall();
	}
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ���������Դ���Ϣ, �����豸
 �� �� ֵ: void
 ��    ����param1
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B9003C0
void CPTHistory::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;

	m_List.DeleteAllItems();
	//��������¼�
	ClearAllEvent();
	//�������������
	ClearAllDI();
	m_nNum = 1;
	m_nActionIndex = 0;

	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	
	SetDlgItemText(IDC_STATIC_TIME_PTHIS, "");
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ20069(����ʧ��)����
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9003C9
void CPTHistory::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���״̬,��������
		//WriteLog("CPTHistory::OnSTTP20069::û�д����ٻ���״̬,��������", 3);
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
        WriteLog("CPTHistory::OnSTTP20069::����Ϊ��", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//������
        WriteLog("CPTHistory::OnSTTP20069::������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
        WriteLog("CPTHistory::OnSTTP20069::���Ǳ�װ�õı���", XJ_LOG_LV3);
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ�װ�õı���
		WriteLog("CPTHistory::OnSTTP20069::IsVaild20069", XJ_LOG_LV3);
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//�����ɹ�
	}
	else
	{
		//����ʧ��
		CString str;
		str.Format("����[%s]�ٻ���ʷ�¼�ʧ��!", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}
	
	//�˳��߳�
	EndParse20116Thread();
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);

	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֹ���)����
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9003D9
void CPTHistory::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�״̬,��������
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
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		return;
	}
	
	//����Ƿ񱾹��ܵı���
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//�˳��߳�
	EndParse20116Thread();
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ���ʷ�¼�ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);
	
	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

//##ModelId=49B87B9003A9
void CPTHistory::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//�˳��߳�
			EndParse20116Thread();
			//�ı�״̬
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			CString str;
			str.Format("����[%s]�ٻ���ʷ�¼�ʧ��,ԭ��Ϊ:��ʱ", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);
			//��ʾ
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTHIS, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����OnSTTP20116()
 ���ܸ�Ҫ����Ӧ������ʷ�¼�����֪ͨ, �����¼�
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87B910000
void CPTHistory::OnSTTP20116( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTHistory::OnSTTP20116 start", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20116����
	if(pSttpData->sttp_head.uMsgID != 20116)
	{
		//������
		CString str;
		str.Format("CPTHistory::OnSTTP20116 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTHistory::OnSTTP20116 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//����
	AddTail20116SttpData(*pSttpData);
	WriteLog("CPTHistory::OnSTTP20116 neat end", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ����ָ���¼����뵽�б������ʾ
 �� �� ֵ: 
 ��    ����param1	ָ���¼�
		   Param2	ָ���б�������
**************************************************************/
//##ModelId=49B87B9002CE
void CPTHistory::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//���������Ч��
	MYASSERT(pEvent);
	if(nIndex < 0)
		return;
	//"���","�¼�����", "����ʱ��", "�¼�����", "�¼�ֵ"
	CString str = "";
	str.Format("%d", pEvent->m_nID);
	//0:���
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:�¼�����
	str = ""; 
	//1-������Ϣ 2���Լ���Ϣ 3��ѹ����Ϣ
	if(pEvent->m_nType == 1)
	{
		str = StringFromID(IDS_ACTION_CHR);
	}
	else if(pEvent->m_nType == 2)
	{
		str = StringFromID(IDS_EVENTTYPE_ALARM);
	}
	else if(pEvent->m_nType == 3)
	{
		str = StringFromID(IDS_EVENTTYPE_DI);
	}
	else if(pEvent->m_nType == 99)
	{
		str = StringFromID(IDS_ACTION_SIGNAL);
	}
	m_List.SetItemText(nIndex, 1, str);

	//2:����ʱ��
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 2, str);
	//3:�¼�����
	m_List.SetItemText(nIndex, 3, pEvent->m_sEventDefName);
	//4:�¼�ֵ
	str = pEvent->m_sEventContent;
	if(pEvent->m_nType == 1)
	{
		//����ֵ
		if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
			str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
	}
	else if(pEvent->m_nType == 3)
	{
		int nResult = atoi(pEvent->m_sEventContent);
		if(nResult == 0)
			str = "OFF";
		else
			str = "ON";
	}
	else
	{
		int nResult = atoi(pEvent->m_sEventContent);
		if(nResult == 0)
			str = StringFromID(IDS_CASE_RETURN);
		else
			str = StringFromID(IDS_CASE_ACTION);
	}
	m_List.SetItemText(nIndex, 4, str);

	if(pEvent->m_nType != 3)
	{
		//5.���ʱ��
		str.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
		m_List.SetItemText(nIndex, 5, str);
	}

	if(pEvent->m_nType == 99)
	{
		//6.�ź���ʱ��
		str.Format("%s.%03d", pEvent->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsSign);
		m_List.SetItemText(nIndex, 6, str);
	}


	//�����豸
	m_List.SetItemData(nIndex, (DWORD)pEvent);
}

/*************************************************************
 �� �� ����TranslateHistoryAction()
 ���ܸ�Ҫ��������ʷ������Ϣ
 �� �� ֵ: 
 ��    ����param1	��������
		   Param2
**************************************************************/
//##ModelId=49B87B9002DE
void CPTHistory::TranslateHistoryAction( STTP_FULL_DATA* pSttpData, int i)
{
	//���������Ч��
	MYASSERT(pSttpData);
	MYASSERT(m_pObj);

	//����װ��ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	//����ʱ��
	CString strTime1 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
	int nms1 = 0;
	CTime tmTime1 = StringToTimeSttp15(strTime1, nms1);
	//���ʱ��
	CString strTime2 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved3);
	int nms2 = 0;
	CTime tmTime2 = StringToTimeSttp15(strTime2, nms2);
	
	//ѭ������ֵ
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//�����¼�
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_nNum = m_nActionIndex;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 1;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_tmReceiveTime = tmTime2;
		pEvent->m_nmsReceive = nms2;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = ComplexExchange(XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue));
		//��������ֵ����
		pEvent->m_sEventDefName = GetChrName(atoi(pEvent->m_sEventDef), pEvent->m_strUnit);
		
		//�������
		pEvent->m_nID = m_nNum++;
		//���뵽����
		m_EventList.AddTail(pEvent);
		//���뵽�б���ʾ
		//	AddEventToList(pEvent);
	}
	
	//ѭ���ź���
	int nSignCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list.size();
	for(int m = 0; m < nSignCount; m++)
	{
		//�����¼�
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_nNum = m_nActionIndex;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 99;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_tmReceiveTime = tmTime2;
		pEvent->m_nmsReceive = nms2;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].strValue);
		pEvent->m_tmSign = StringToTimeSttp15(XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].strReserved), pEvent->m_nmsSign);
		//�����ź�������
		pEvent->m_sEventDefName = GetActionSignName(atoi(pEvent->m_sEventDef));
		
		//�������
		pEvent->m_nID = m_nNum++;
		//���뵽����
		m_EventList.AddTail(pEvent);
		//���뵽�б���ʾ
		//	AddEventToList(pEvent);
	}
	m_nActionIndex++;
}


/*************************************************************
 �� �� ����ClearAllEvent()
 ���ܸ�Ҫ��ɾ�������¼���¼
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002F0
void CPTHistory::ClearAllEvent()
{
	POSITION pos = m_EventList.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_EventList.GetNext(pos);
		if(pEvent == NULL)
			continue;
		delete pEvent;
	}
	m_EventList.RemoveAll();
}

/*************************************************************
 �� �� ����GetChrName()
 ���ܸ�Ҫ������ָ������ֵ����
 �� �� ֵ: ����ֵ����
 ��    ����param1	����ֵID
		   Param2	��λ
**************************************************************/
//##ModelId=49B87B9002F1
CString CPTHistory::GetChrName( int nID, CString& strUnit )
{
	//���������Ч��
	if(m_pObj == NULL)
		return "";
	//���뱣������
	m_pObj->RefreshConfig();
	//����
	PT_ActionCharacter* pAC = (PT_ActionCharacter*)m_pObj->FindActionCharacter(nID);
	if(pAC)
	{
		strUnit = pAC->strUnits;
		return pAC->strName;
	}
	
	//û���ҵ�, ��ͨ���в���
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp ->GetDataEngine();
	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < pData->m_arrUniversalActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) pData->m_arrUniversalActChr[i];
		if (nID == pActChr->nID)
		{
			strUnit = pActChr->strUnits;
			return pActChr->strName;
		}
	}
	return "";
}

/*************************************************************
 �� �� ����GetActionSignName()
 ���ܸ�Ҫ������ָ���ź�������
 �� �� ֵ: �ź�������
 ��    ����param1	�ź���ID
		   Param2
**************************************************************/
//##ModelId=49B87B9002FE
CString CPTHistory::GetActionSignName( int nID )
{
	//���������Ч��
	if(m_pObj == NULL)
		return "";
	//���뱣������
	m_pObj->RefreshConfig();
	//����
	PT_Signal* pAC = (PT_Signal*)m_pObj->FindActionSignal(nID);
	
	if(pAC)	
		return pAC->strName;

	return "";
}

/*************************************************************
 �� �� ����TranslateHistoryAlarm()
 ���ܸ�Ҫ��������ʷ�澯��Ϣ
 �� �� ֵ: 
 ��    ����param1 ��������
		   Param2
**************************************************************/
//##ModelId=49B87B9002E0
void CPTHistory::TranslateHistoryAlarm( STTP_FULL_DATA* pSttpData, int i )
{
	//���������Ч��
	MYASSERT(pSttpData);
	MYASSERT(m_pObj);
		
	//����װ��ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	
	//ѭ���ź���
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//�����¼�
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 2;
		CString strTime = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strReserved);
		pEvent->m_tmStart = StringToTimeSttp15(strTime, pEvent->m_nmsStart);
		strTime = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
		pEvent->m_tmReceiveTime = StringToTimeSttp15(strTime, pEvent->m_nmsReceive);
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue);
		//�����ź�������
		pEvent->m_sEventDefName = GetAlarmSignName(atoi(pEvent->m_sEventDef));
		
		pEvent->m_nID = m_nNum++;
		//���뵽����
		m_EventList.AddTail(pEvent);
		//���뵽�б���ʾ
		//	AddEventToList(pEvent);
	}
}

/*************************************************************
 �� �� ����GetAlarmSignName()
 ���ܸ�Ҫ������ָ���澯�ź���������
 �� �� ֵ: �ź�������
 ��    ����param1	�ź���ID
		   Param2
**************************************************************/
//##ModelId=49B87B900300
CString CPTHistory::GetAlarmSignName( int nID )
{
	//���������Ч��
	if(m_pObj == NULL)
		return "";
	//���뱣������
	m_pObj->RefreshConfig();
	//����
	PT_Signal* pAC = (PT_Signal*)m_pObj->FindAlarmSignal(nID);
	if(pAC)
	{
		return pAC->strName;
	}
	
	return "";
}

/*************************************************************
 �� �� ����ClearAllDI()
 ���ܸ�Ҫ��������п���������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B90030E
void CPTHistory::ClearAllDI()
{
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* ptg = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(ptg == NULL)
			continue;
		delete ptg;
	}
	m_arrDigital.RemoveAll();
}

/*************************************************************
 �� �� ����GetPTDI()
 ���ܸ�Ҫ�������ݿ����뱾���������п���������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B90030F
BOOL CPTHistory::GetPTDI(int nCPU)
{
	BOOL bReturn = TRUE;

	if(m_pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code����DI_ID,CODE_NAME,NAME,��DI_ID��С��������
		
	CString str;
		
	//�ֶ�
	//DI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "DI_ID";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);

	//code_name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "CODE_NAME";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//cpu_code
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "CPU_CODE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field4);
		
	//����
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	/*
	//CPU
	Condition CPU;
	bzero(&CPU, sizeof(Condition));
	str.Format("cpu_code = %d", nCPU);
	strncpy(CPU.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(CPU);
	*/

	//��DI_ID��С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by DI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTHistory::GetPTDI, ��ѯʧ��");
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
			//��������������
			PT_DIGITAL * digital = new PT_DIGITAL;
			digital->ID = pMemset->GetValue((UINT)0); //DI_ID
			digital->Name = pMemset->GetValue(1); //NAME
			digital->CodeName = pMemset->GetValue(2); //Code_Name
			digital->CpuCode = atoi(pMemset->GetValue(3));
			m_arrDigital.Add(digital);
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTHistory::GetPTDI,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 �� �� ����GetDIName()
 ���ܸ�Ҫ���õ�ָ��������������
 �� �� ֵ: ����������
 ��    ����param1	������ID
		   Param2
**************************************************************/
//##ModelId=49B87B900310
CString CPTHistory::GetDIName( CString sID, int nCPU )
{
	//���������Ч��
	if(m_pObj == NULL)
		return "";
	if(m_arrDigital.GetSize() == 0)
		GetPTDI(nCPU);
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* ptg = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(ptg == NULL)
			continue;
		if(ptg->ID == sID && ptg->CpuCode == nCPU)
			return ptg->Name;
	}
	return "";
}

/*************************************************************
 �� �� ����TranslateHistoryDI()
 ���ܸ�Ҫ��������ʷ��������Ϣ
 �� �� ֵ: 
 ��    ����param1	��������
		   Param2
**************************************************************/
//##ModelId=49B87B9002EE
void CPTHistory::TranslateHistoryDI( STTP_FULL_DATA* pSttpData, int i)
{
	//���������Ч��
	if(pSttpData == NULL)
		return;
	if(m_pObj == NULL)
		return;
	//����װ��ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	//����ʱ��
	CString strTime1 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
	int nms1 = 0;
	CTime tmTime1 = StringToTimeSttp15(strTime1, nms1);
	//CPU
	int nCPU = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].nReserved;
	
	//ѭ������ֵ
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//�����¼�
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 3;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_nCPU = nCPU;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue);
		//���ҿ���������
		pEvent->m_sEventDefName = GetDIName(pEvent->m_sEventDef, nCPU);
		if (pEvent->m_sEventDefName == "")
		{
			WriteLog("CPTHistory::TranslateHistoryDI::δ���ҵ����������� " + pEvent->m_sSecID + " " + pEvent->m_sEventDef, XJ_LOG_LV3);
		}
		
		//�������
		pEvent->m_nID = m_nNum++;
		//���뵽����
		m_EventList.AddTail(pEvent);
		//���뵽�б���ʾ
		//	AddEventToList(pEvent);
	}
}

/*************************************************************
 �� �� ����OnSTTP00901()
 ���ܸ�Ҫ���յ�00901����, �ٻ���ʷ�¼����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B910013
void CPTHistory::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTHistory::OnSTTP00901 start", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�00901����
	if(pSttpData->sttp_head.uMsgID != 901)
	{
		//������
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//֪ͨ�����̱߳��Ľ���
	m_bDataEnd = TRUE;

	CString str;
	str.Format("����[%s]�ٻ���ʷ�¼��ɹ�!", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_SUCCESS);

	WriteLog("CPTHistory::OnSTTP00901 neat end", XJ_LOG_LV3);
}

//##ModelId=49B87B9003AC
void CPTHistory::OnSelchangeCmbPthistorySrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_SRC);
	if(pCmb == NULL)
		return;
	m_nSelSrc = pCmb->GetCurSel();
}

//##ModelId=49B87B9003BA
void CPTHistory::OnSelchangeCmbPthistoryType() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_TYPE);
	if(pCmb == NULL)
		return;
	m_nEventType = pCmb->GetCurSel();
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B900234
int CALLBACK CPTHistory::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int type1, type2;
	int iResult = 0;
	//"���","�¼�����", "����ʱ��", "�¼�����", "�¼�ֵ"
	switch(nCol)
	{
	case 0: //���
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //�¼�����
		type1 = pEvent1->m_nType;
		type2 = pEvent2->m_nType;
		if(type1 == 99)
			type1 = 1;
		if(type2 == 99)
			type2 = 1;
		iResult = type1- type2;
		break;
	case 2: //����ʱ��
		str1 = pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		str2 = pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //�¼�����
		str1 = pEvent1->m_sEventDefName;
		str2 = pEvent2->m_sEventDefName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //�¼�ֵ
		str1 = pEvent1->m_sEventContent;
		str2 = pEvent2->m_sEventContent;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTHistoryAsc;
	return iResult;
}

//##ModelId=49B87B9003BC
void CPTHistory::OnColumnclickListPthistory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTHistoryAsc = - g_iPTHistoryAsc;
	}
	else
	{
		g_iPTHistoryAsc = 1;
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
//##ModelId=49B87B91000F
void CPTHistory::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�״̬,��������
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
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//������
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		return;
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
		//�����ٻ���ʷ�¼�				
		int nTimeOut = pData->m_nHistoryEventTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTHistory::OnBtnPthistorySave() 
{
	// TODO: Add your control notification handler code here
	//ȡ��ѡ����
	HISTORYEVENT_LIST listAction; 
	HISTORYEVENT_LIST listAlarm; 
	HISTORYEVENT_LIST listDI;
	listAction.RemoveAll();
	listAlarm.RemoveAll();
	listDI.RemoveAll();

	GetSelected(&listAction, &listAlarm, &listDI);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	MYASSERT(pApp);

	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	//�澯
	SendAlarm(&listAlarm);
	//������
	SendDI(&listDI);
	//����
	SendAction(&listAction);

	AfxMessageBox( StringFromID(IDS_TIP_SAVE_FINISHED));
}

/*************************************************************
 �� �� ��: GetSelected()
 ���ܸ�Ҫ: ȡ��ѡ����
 �� �� ֵ: 
 ��    ��: param1 ������ѡ�����¼�
		   Param2 ������ѡ�澯�¼�
		   Param3 ������ѡ�������¼�
**************************************************************/
void CPTHistory::GetSelected( HISTORYEVENT_LIST* listAction, HISTORYEVENT_LIST* listAlarm, HISTORYEVENT_LIST* listDI )
{
	UpdateData(TRUE);
	//���������Ч��
	MYASSERT(listAction);
	MYASSERT(listAlarm);
	MYASSERT(listDI);
	//ȡ��ѡ��
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		if(m_List.GetCheck(i))
		{
			//��ѡ��
			CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(i);
			MYASSERT_CONTINUE(pEvent);
			if(pEvent->m_nType == 1 || pEvent->m_nType == 99)
			{
				//����
				listAction->AddTail(pEvent);
			}
			else if(pEvent->m_nType == 2)
			{
				//�澯
				listAlarm->AddTail(pEvent);
			}
			else if(pEvent->m_nType == 3)
			{
				//������
				listDI->AddTail(pEvent);
			}
		}
	}
}

/*************************************************************
 �� �� ��: SendAlarm()
 ���ܸ�Ҫ: ���͸澯����
 �� �� ֵ: 
 ��    ��: param1 �澯�¼�����
		   Param2 
**************************************************************/
void CPTHistory::SendAlarm( HISTORYEVENT_LIST* listAlarm )
{
	//���������Ч��
	MYASSERT(listAlarm);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	//��֯����
	if(!pApp->m_SttpEngine.BuildDataFor20116FaultAlarm(sttpData, listAlarm))
	{
		//��֯����ʧ��
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
			str.Format("����%s���ͱ�����ʷ�澯�¼�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("����%s���ͱ�����ʷ�澯�¼�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == 0)
		{
			CString str;
			str.Format("����%s���ͱ�����ʷ�澯�¼�����ʧ��,ԭ��Ϊ��Ч����", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
}

/*************************************************************
 �� �� ��: SendDI()
 ���ܸ�Ҫ: ���Ϳ���������
 �� �� ֵ: 
 ��    ��: param1 �������¼�����
		   Param2 
**************************************************************/
void CPTHistory::SendDI( HISTORYEVENT_LIST* listDI )
{
	//���������Ч��
	MYASSERT(listDI);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	//��֯����
	if(!pApp->m_SttpEngine.BuildDataFor20116FaultDI(sttpData, listDI))
	{
		//��֯����ʧ��
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
			str.Format("����%s���ͱ�����ʷ�������¼�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("����%s���ͱ�����ʷ�������¼�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == 0)
		{
			CString str;
			str.Format("����%s���ͱ�����ʷ�������¼�����ʧ��,ԭ��Ϊ��Ч����", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
}

/*************************************************************
 �� �� ��: SendAction()
 ���ܸ�Ҫ: ���Ͷ�������
 �� �� ֵ: 
 ��    ��: param1 �����¼�����
		   Param2 
**************************************************************/
void CPTHistory::SendAction( HISTORYEVENT_LIST* listAction )
{
	//���������Ч��
	MYASSERT(listAction);

	//��¼�ѱ�����¼�
	CWordArray arrActionNum;
	arrActionNum.RemoveAll();
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	POSITION pos = listAction->GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)listAction->GetNext(pos);
		MYASSERT_CONTINUE(pEvent);
		//ȡ�ö�����
		int nActionNum = pEvent->m_nNum;
		//�����Ƿ��ѱ����
		BOOL bSaved = FALSE;
		for(int i = 0; i < arrActionNum.GetSize(); i++)
		{
			int nIndex = arrActionNum.GetAt(i);
			if(nIndex == nActionNum)
			{
				bSaved = TRUE;
				break;
			}
		}
		//�Ա�����Ĳ��ٴ���
		if(bSaved)
			continue;
		arrActionNum.Add(nActionNum);
		//ȡ��������ص��¼�
		HISTORYEVENT_LIST listSave;
		listSave.RemoveAll();
		POSITION pos1 = m_EventList.GetHeadPosition();
		while(pos1 != NULL)
		{
			CFaultEvent* pAction = (CFaultEvent*)m_EventList.GetNext(pos1);
			MYASSERT_CONTINUE(pAction);
			if(pAction->m_nType != 1 && pAction->m_nType != 99)
				continue;
			if(pAction->m_nNum != nActionNum)
				continue;
			listSave.AddTail(pAction);
		}

		//���뱨����������
		STTP_FULL_DATA sttpData;
		//��֯����
		if(!pApp->m_SttpEngine.BuildDataFor20116FaultAction(sttpData, &listSave))
		{
			//��֯����ʧ��
			continue;
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
				str.Format("����%s���ͱ�����ʷ�����¼�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����%s���ͱ�����ʷ�����¼�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == 0)
			{
				CString str;
				str.Format("����%s���ͱ�����ʷ�����¼�����ʧ��,ԭ��Ϊ��Ч����", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
		}
	}

}

void CPTHistory::OnBtnPthistorySelNull() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		m_List.SetCheck(i, FALSE);
	}
}

void CPTHistory::OnBtnPthistorySelAll() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		m_List.SetCheck(i, TRUE);
	}
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ�����ò���ʱlist���ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CPTHistory::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				if(nItem > -1)
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

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: AddTail20116SttpData()
 ���ܸ�Ҫ: ��20116���ļ��뵽����β
 �� �� ֵ: 
 ��    ��: param1 20116��������
		   Param2 
**************************************************************/
void CPTHistory::AddTail20116SttpData( STTP_FULL_DATA& sttpData )
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
 �� �� ��: RemoveHead20116SttpData()
 ���ܸ�Ҫ: ɾ��20116��������ĵ�һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTHistory::RemoveHead20116SttpData()
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
 �� �� ��: StartParse20116Thread()
 ���ܸ�Ҫ: ��������20026�����߳�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTHistory::StartParse20116Thread()
{
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	//�����������
	m_listFullData.RemoveAll();
	//����
	m_pParse20116Thread = AfxBeginThread(PTParse20116,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pParse20116Thread != NULL)
	{
		m_pParse20116Thread->m_bAutoDelete = FALSE;
		m_pParse20116Thread->ResumeThread();
		WriteLog("CPTHistory::StartParse20116Thread, �߳������ɹ�", XJ_LOG_LV3);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: EndParse20116Thread()
 ���ܸ�Ҫ: ǿ��ֹͣ20026��������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTHistory::EndParse20116Thread()
{
	//�˳��߳�
	if(m_pParse20116Thread != NULL)
	{
		//�̻߳�û�˳�ȥ
		m_bExitParse = TRUE;	//�����˳���־
		WaitForSingleObject(m_pParse20116Thread->m_hThread, INFINITE);
		delete m_pParse20116Thread;
		m_pParse20116Thread = NULL;
		WriteLog("CPTHistory::EndParse20116Thread, �߳��˳��ɹ�", XJ_LOG_LV3);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: OnThreadWorkEnd()
 ���ܸ�Ҫ: �߳�֪ͨ���棬���Ĵ������
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTHistory::OnThreadWorkEnd( WPARAM wParam, LPARAM lParam )
{
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//������ʾ
	FillListData();
	
	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 �� �� ��: OnPTFrameClose()
 ���ܸ�Ҫ: ��Ӧ���ڹر���Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTHistory::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	//�˳��߳�
	EndParse20116Thread();
}

void CPTHistory::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (nSBCode == SB_ENDSCROLL)
	{
		this->SendMessage(WM_SIZE);
	}
	CViewBase::OnHScroll(nSBCode, nPos, pScrollBar);
	
}

BOOL CPTHistory::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CViewBase::OnScrollBy(sizeScroll, bDoScroll);
}

void CPTHistory::OnBtnPthistorySelCopy() 
{
	// TODO: Add your control notification handler code here
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
		    CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
			for (int j = 0; j < m_List.GetHeaderCtrl()->GetItemCount(); j++)
			{
				char sz[512] = {0};
				m_List.GetItemText(nIndex, j, sz, 512);
				str += sz;
				str += "   ";
			}
		}
		str += "\r\n";
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

void CPTHistory::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTHistory::OnDoubleclickedBtnPthistoryCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTHistory::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_HISTORYEVENT));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_HISTORYEVENT));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPTHistory::OnBtnPthistoryFault() 
{
	// TODO: Add your control notification handler code here
	CDlgSelFault dlg;
	dlg.SetSecObj(m_pObj);
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
	if(pTime2 == NULL)
		return;
	CTime tmStart;
	pTime->GetTime(tmStart);
	CTime tmEnd;
	pTime2->GetTime(tmEnd);
	dlg.SetQueryTime(tmStart, tmEnd);

	int selFault = -1;

	if( dlg.DoModal() == IDOK )
	{
		CFaultReport* report = dlg.GetFault();

		if( NULL == report )
		{
			return;
		}
		report->LoadDetail();

		HISTORYEVENT_LIST listAction; 
		HISTORYEVENT_LIST listAlarm; 
		HISTORYEVENT_LIST listDI;
		listAction.RemoveAll();
		listAlarm.RemoveAll();
		listDI.RemoveAll();
		
		GetSelected(&listAction, &listAlarm, &listDI);

		POSITION pos = listAction.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listAction.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			if( 1 == pClone->m_nType )
			{
				//������
				report->m_listChr.Add(pClone);
			}
			if( pClone->m_nType ==  99 )
			{
				pClone->m_nType = 2; //�����ź���
				report->m_listSign.Add(pClone);
			}
		}
		
		pos = listAlarm.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listAlarm.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			pClone->m_nType = 3;
			report->m_listAlarm.Add(pClone);
		}

		pos = listDI.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listDI.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			pClone->m_nType = 8;
			report->m_listDIChange.Add(pClone);
		}

		//save
		if( 0 != report->MultiSaveEvent() )
		{
			AfxMessageBox( StringFromID(IDS_COMMON_UPDATEDB_FAILED));
		}else
		{
			AfxMessageBox( StringFromID(IDS_FAULTREPORT_UPDATE_SUCCESS));
		}

		report->ClearDetail();

	}
}
