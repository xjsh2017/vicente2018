// PTAnalog.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTAnalog.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

UINT PTAnalogLoad(LPVOID pParam)
{
	CPTAnalog* ptAnalog = (CPTAnalog*)pParam;
	if(ptAnalog == NULL)
		return 1;
	int nQueryNo = ptAnalog->GetCurrentQueryNo();
	
	//�����˳��߳�,�˳�
	if(!ptAnalog->m_bExitThread && ptAnalog->IsCurrentQueryNo(nQueryNo))
	{
		if(ptAnalog->LoadPTAnalogConfig(nQueryNo))
		{
			ptAnalog->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		}
		else
		{
			ptAnalog->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
		}
	}
	
	ptAnalog->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog

IMPLEMENT_DYNCREATE(CPTAnalog, CViewBase)

//##ModelId=49B87B930159
CPTAnalog::CPTAnalog()
	: CViewBase(CPTAnalog::IDD)
{
	//{{AFX_DATA_INIT(CPTAnalog)
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_sCPU = "";
	m_nRealtimeValueIndex = 4;
	m_nNewValueIndex = 5;
	m_nRealtimeValueTimer = 0;
	m_nTimer = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();
	m_nCurrentStatus = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B930341
CPTAnalog::~CPTAnalog()
{
	ClearAnalogConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B93031C
void CPTAnalog::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTAnalog)
	DDX_Control(pDX, IDC_BTN_PTANALOG_EXCEL, m_btnExcel);
	DDX_Control(pDX, IDC_GIF_PTANALOG, m_gif);
	DDX_Control(pDX, IDC_BTN_PTANALOG_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTANALOG_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTANALOG_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTANALOG, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTAnalog, CViewBase)
	//{{AFX_MSG_MAP(CPTAnalog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTANALOG_CALL, OnBtnPtanalogCall)
	ON_CBN_SELCHANGE(IDC_CMB_PTANALOG_CPU, OnSelchangeCmbPtanalogCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTANALOG, OnCustomDraw)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTANALOG_STYLE, OnBtnPtanalogStyle)
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTANALOG_CALL, OnDoubleclickedBtnPtanalogCall)
	ON_BN_CLICKED(IDC_BTN_PTANALOG_EXCEL, OnBtnPtanalogExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog diagnostics

#ifdef _DEBUG
//##ModelId=49B87B93035B
void CPTAnalog::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B93036B
void CPTAnalog::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog message handlers

//##ModelId=49B87B930301
void CPTAnalog::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	RecordRate(IDC_BTN_PTANALOG_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_STYLE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTANALOG, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTANALOG_CPU, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTANALOG, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTAI, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTAI_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTAI_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTANALOG_LOADING, 0, mid_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	//��ʼ���б�
	InitListCtrl();
	//��ʼ�������ؼ�
	InitControls();
	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B930208
int CPTAnalog::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	RefreshControls();
	LoadPTAnalogConfig();
	LoadPTHistoryAnalogData();*/

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B930222
int CPTAnalog::InitListStyle()
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
		lvCol.fmt=LVCFMT_LEFT;
	if(LoadListConfig() && m_arrColum.GetSize() >= 5)
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
		//char* arColumn[5]={"���","ģ��������", "ģ��������", "��λ", "ʵʱֵ"};
	
		
		//�����п�,��С����
		for (int nCol=0; nCol < 5 ; nCol++)
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
				lvCol.cx = 140; // ģ��������
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // ģ��������
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80;
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 160;
				colName = StringFromID(IDS_COMMON_REALTIME_VALUE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(ģ��������)
		m_List.SetColumnHide(2, TRUE);
	}
	if( !g_PTAIRealtimeValue )
	{
		//ʵʱֵ
		m_List.SetColumnHide(m_nRealtimeValueIndex, TRUE);
	}
	else
	{
		m_List.SetColumnHide(m_nRealtimeValueIndex, FALSE);
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
//##ModelId=49B87B930242
BOOL CPTAnalog::LoadListConfig()
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
			WriteLog("PTAnalog::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTAnalog::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTAnalogConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTAnalogConfig", XJ_LOG_LV3);
				WriteLog("PTAnalog::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTAnalogConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTAnalog::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B930251
BOOL CPTAnalog::SaveListConfig()
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
			WriteLog("PTAnalog::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTAnalog::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTAnalogConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTAnalogConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTAnalogConfig");
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
						WriteLog("δ�ҵ�PTAnalogConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTAnalogConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B930232
int CPTAnalog::FillListData(int nQueryNo)
{
	WriteLog("CPTAnalog::FillListData,��ʼ�������", XJ_LOG_LV3);

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	PT_ANALOG *pts = NULL;
	EnterCriticalSection(&m_CriticalOper);
	int nValueCount = m_HisValueMgr.GetCount();
	for(int i = 0; i < m_arrAnalog.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;
		pts = m_arrAnalog.GetAt(i);
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", i+1);
		}
		m_List.InsertItem(i, sID);
		m_List.SetItemText(i, 1, pts->Name);
		m_List.SetItemText(i, 2, pts->CodeName); //
		m_List.SetItemText(i, 3, pts->Unit); //��λ
		m_List.SetItemText(i, 4, SetFloatValue(pts, pts->Value));
		
		if(g_PTAIValues)
		{
			for(int k = 0; k < nValueCount; k++)
			{
				CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
				m_List.SetItemText(i, m_nNewValueIndex+k, SetFloatValue(pts, sHisValue));
			}
		}
		m_List.SetItemData(i, (DWORD)pts);
	}
	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTAnalog::FillListData,����������", XJ_LOG_LV3);

	return 0;
}

//##ModelId=49B87B930261
BOOL CPTAnalog::NeedSave()
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
		for(int i = 0; i < m_nNewValueIndex; i++)
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
//##ModelId=49B87B930271
int CPTAnalog::InitControls()
{
	/*
	//�ٻ���ť
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ�ģ����"));

	//��ʾ���ð�ť
	m_btnStyle.SetThemeHelper(&m_ThemeHelper);
//	m_btnStyle.SetIcon(IDI_SEARCH);
	m_btnStyle.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStyle.SetTooltipText(_T("������ʾ���"));
*/
	//��ӡ��ť
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
//	m_btnPrint.SetIcon(IDI_PRINT);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText( StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());
	return 0;
}

//##ModelId=49B87B93038A
void CPTAnalog::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTANALOG, left_client, IDC_BTN_PTANALOG_CALL, right_client, bottom_client);
}

//##ModelId=49B87B9303A9
void CPTAnalog::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����LoadPTAnalogConfig()
 ���ܸ�Ҫ�������ݿ��ȡģ����������Ϣ,ģ������ʷֵֻ����һ��,
			���ұ��������ñ���,����ֱ�Ӷ���ֵ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B930280
BOOL CPTAnalog::LoadPTAnalogConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	BOOL bReturn = TRUE;

	stopRealtimeValueTimer();

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code����DI_ID,CODE_NAME,NAME,��DI_ID��С��������
		
	CString str;
		
	//�ֶ�
	//AI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "AI_ID";
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

	//VALUE
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "VALUE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_FLOAT;
	sql.Fieldlist.push_back(Field4);

	//curtime
	Field Field7;
	bzero(&Field7, sizeof(Field));
	str = "CURTIME";
	strncpy(Field7.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field7);

	//unit
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "UNIT";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field5);

	//ai_precision
	Field Field6;
	bzero(&Field6, sizeof(Field));
	str = "AI_PRECISION";
	strncpy(Field6.FieldName, str, str.GetLength());
	Field6.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field6);
	
	//����
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %s", m_sCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	//��AI_ID��С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by AI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);
			
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadPTAnalogConfig, ��ѯʧ��");
		return FALSE;
	}

	CString strCurTime = "";
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadPTAnalogConfig,��ȡ��%d������", nCount);
		WriteLogEx(str);

		EnterCriticalSection(&m_CriticalOper);
		int nValueNo = m_HisValueMgr.CreateValue("");
		BOOL bHasTime = FALSE;
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(g_role != MODE_SUB && i%20 == 0)
				Sleep(1);
			//��������������
			PT_ANALOG * analog = new PT_ANALOG;
			analog->ID = pMemset.GetValue((UINT)0); //DI_ID
			analog->Name = pMemset.GetValue(1); //NAME
			analog->CodeName = pMemset.GetValue(2); //Code_Name
			analog->Value = pMemset.GetValue(3); //value
			float fValue = atof(analog->Value);
			analog->Value.Format("%.3f", fValue);
			m_HisValueMgr.AddValue(nValueNo, atoi(analog->ID), analog->Value);
			if(!bHasTime)
			{
				if(analog->Value != "")
				{
					bHasTime = TRUE;
					m_HisValueMgr.SetTime(nValueNo, pMemset.GetValue(4));
				}
			}
			analog->Unit = "";
			analog->Unit = pMemset.GetValue(5); //unit
			analog->Precision = pMemset.GetValue(6); //ai_precision
			m_arrAnalog.Add(analog);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadPTAnalogConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTAnalog::LoadPTAnalogConfig,��ѯ�������", XJ_LOG_LV3);

	startRealtimeValueTimer();

	return bReturn;
}

/*************************************************************
 �� �� ����ClearAnalogConfig()
 ���ܸ�Ҫ�����ģ����������Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B930290
BOOL CPTAnalog::ClearAnalogConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrAnalog.GetSize(); i++)
	{
		PT_ANALOG * analog = (PT_ANALOG*)m_arrAnalog.GetAt(i);
		delete analog;
		analog = NULL;
	}
	m_arrAnalog.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 �� �� ����AddAnalogValue()
 ���ܸ�Ҫ������ĳģ����ֵ���б�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ�����ģ������ID
           Param2   Ҫ�����ģ������ֵ
		   Param3	ָ��Ҫ���뵽�ڼ���
**************************************************************/
//##ModelId=49B87B93029F
BOOL CPTAnalog::AddAnalogValue(CString ID, CString VALUE, int iCol)
{
	//���ҵ�DI_ID==ID������
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();

	PT_ANALOG* pst = FindAnalog(strID);
	if(pst == NULL)
		return FALSE;
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//���Ҵ�ֵ�ڵڼ���
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return FALSE;
	//����ֵ
	pst->Value = VALUE;

	//����ListCtrl��Value������
	bReturn = m_List.SetItemText(nItem, iCol, SetFloatValue(pst, pst->Value));
	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTHistoryAnalogData()
 ���ܸ�Ҫ������ģ������ʷֵ 
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9302AF
BOOL CPTAnalog::LoadPTHistoryAnalogData()
{
		if(m_pObj == NULL)
		return FALSE;
	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code����DI_ID,CODE_NAME,NAME,��DI_ID��С��������
		
	CString str;
		
	//�ֶ�
	//AI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "AI_ID";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//VALUE
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "VALUE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_FLOAT;
	sql.Fieldlist.push_back(Field4);

	//curtime
	Field Field7;
	bzero(&Field7, sizeof(Field));
	str = "CURTIME";
	strncpy(Field7.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field7);
	
	//����
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %s", m_sCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadPTAnalogConfig, ��ѯʧ��");
		return FALSE;
	}

	CString strCurTime = "";
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadPTHistoryAnalogData,��ȡ��%d������", nCount);
		WriteLogEx(str);

		
		for(int i = 0; i < nCount; i++)
		{
			if( 0 == i )
			{
				CString strTime;
				strTime.Format("%s:%s", StringFromID(IDS_COMMON_REALTIME_VALUE),pMemset.GetValue(2));
				LVCOLUMN m_vcolumn;
				m_vcolumn.mask = LVCF_TEXT;
				m_vcolumn.pszText =strTime.GetBuffer(0);
				m_vcolumn.cchTextMax = strTime.GetLength();
				m_List.SetColumn(m_nRealtimeValueIndex, &m_vcolumn);
			}
			AddAnalogValue(pMemset.GetValue((UINT)0), pMemset.GetValue(1), m_nRealtimeValueIndex);
					
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadPTHistoryAnalogData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTAnalog::LoadPTHistoryAnalogData,��ѯ�������", XJ_LOG_LV3);

	return bReturn;
}

//##ModelId=49B87B9303B9
void CPTAnalog::OnBtnPtanalogCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ�ٻ���ť������Ϣ,�����ٻ�ģ�������Ļ�ȡ���ٻ�
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
		//����״̬, �����ť��ʾҪ�ٻ�ģ����
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯�ٻ�ģ����20007����
		if(!pApp->m_SttpEngine.BuildDataFor20007PTAnalog(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
		{
			//��֯����ʧ��
			return;
		}
		
		//���ͱ���
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);

		zero_sttp_full_data(sttpData);
		//�ж��Ƿ�ɹ�
		if(nReturn != 1)
		{
			//ʧ��
			if(nReturn == -1)
			{
				CString str;
				str.Format("����[%s]�����ٻ�ģ��������ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ�ģ��������ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ�ģ����
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTAiTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//�����˹�������־
			//pApp->AddManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sName);
			
			//���ݵ�ǰ״̬���¿ؼ�������
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����ٻ�ģ�������ĳɹ�,�ȴ���Ӧ", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else
	{
		//ȡ���ٻ�
		CancelCall();
	}
}


/*************************************************************
 �� �� ����OnSTTP20008()
 ���ܸ�Ҫ����ӦWM_STTP_20008(ģ��������֪ͨ)��Ϣ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�������ݽṹָ��
**************************************************************/
//##ModelId=49B87B940006
void CPTAnalog::OnSTTP20008( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�ģ������״̬,��������
		return;
	}

	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}

	//����Ƿ�20008����
	if(pSttpData->sttp_head.uMsgID != 20008)
	{
		//������
		CString str;
		str.Format("CPTAnalog::OnSTTP20008 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTAnalog::OnSTTP20008 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20008 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if( m_bFirstFrame )
	{
		m_bFirstFrame = FALSE;

		//ʱ��
		CString strTime = pSttpData->sttp_body.ch_time_15_BIT1;
		//ʱ��ת��
		int nms;
		CTime tmTime = StringToTimeSttp15(strTime, nms);
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//�¼�һ��
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}

	//������
	if(!LoadDataFromSTTP(pSttpData))
	{
		WriteLog("CPTAnalog::OnSTTP20008 ������ʧ��", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 )
	{
		//���к���֡
		return;
	}

	//�յ���ȷ����, ֹͣ��ʱ��ʱ��
	KillTimer(m_nTimer);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_SUCCESS);
	//�������,�ı�״̬
	m_nCurrentStatus = 0;

	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ�ģ�����ɹ�", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

/*************************************************************
 �� �� ����LoadDataFromSTTP()
 ���ܸ�Ҫ����STTP�����ж�ȡģ��������,��������Ϊ����ı����Ѿ�����ȷ����,�����ж�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
**************************************************************/
//##ModelId=49B87B9302B0
BOOL CPTAnalog::LoadDataFromSTTP( STTP_FULL_DATA* sttpData )
{
	if(sttpData == NULL)
		return FALSE;

	//����ģ��������
	try
	{		
		//��ȡÿ��ֵ,���뵽�б�
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddAnalogValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTAnalog::LoadDataFromSTTP, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ20069(���Ʊ���ͨ�÷���֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B940010
void CPTAnalog::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�ģ������״̬,��������
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
		//�������,�ı�״̬�Ͱ�ť����
		CString str;
		str.Format("����[%s]�ٻ�ģ����ʧ��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}

	m_nCurrentStatus = 0;

	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֹ��ܻ�Ӧ����)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B94001F
void CPTAnalog::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ�ģ������״̬,��������
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
	if(!IsVaild20125(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
			
	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ�ģ����ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
	
	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9302C0
void CPTAnalog::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//�ٻ���ť���ָ�Ϊȡ���ٻ�
		SetDlgItemText(IDC_BTN_PTANALOG_CALL, StringFromID(IDS_BTN_CALLANALOG));

		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTANALOG_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTANALOG_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTANALOG_STYLE) ->EnableWindow(TRUE);
		
		//������
		GetDlgItem(IDC_CMB_PTANALOG_CPU) ->EnableWindow(TRUE);
		//����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		//�ٻ���ť���ָ�Ϊȡ���ٻ�
		SetDlgItemText(IDC_BTN_PTANALOG_CALL, CANCEL_CALL);

		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTANALOG_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTANALOG_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTANALOG_STYLE) ->EnableWindow(FALSE);

		//������
		GetDlgItem(IDC_CMB_PTANALOG_CPU) ->EnableWindow(FALSE);
		//����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTAI, str);
	}
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ����Ϣ, ˢ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B940031
void CPTAnalog::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//����ٻ���ʱ
	SetDlgItemText(IDC_STATIC_TIME_PTAI, "");
	//ˢ�¿ؼ�ֵ
	RefreshControls();
	
	ReFillAll();
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ, �ڵ�һ����������Ķ�Ӧ����ʱ��Ҫ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9302C1
void CPTAnalog::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return;

	//���
	pCmb->ResetContent();

	if(m_pObj == NULL)
		return;

	//CPU������, �������Ϊ"CPU��-����"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//ȡ��CPU����
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d-%s", pCpu->code, pCpu->des); //cpu��-����
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//����Ĭ��ѡ��
	if(pCmb->GetCount() > 0)
	{
		if( pCmb->GetCount() == 1 )
		{
			pCmb->SetCurSel(0);
		}
		else
		{
			if( !LoadDefaultCpu())
			{			
				pCmb->SetCurSel(0);
			}
		}

		//���浱ǰѡ��
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 �� �� ����SaveSelCPU()
 ���ܸ�Ҫ�����浱ǰѡ���CPU
 �� �� ֵ: void
 ��    ����param1	CPU������
**************************************************************/
//##ModelId=49B87B9302CE
BOOL CPTAnalog::SaveSelCPU( CComboBox * pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CString strCPU;
	pCmb->GetLBText(nIndex, strCPU);
	//CPU��ʾ�ĸ�ʽΪ: CPU��-CPU����
	int pos = strCPU.Find('-', 0);
	if(pos != -1)
	{
		strCPU = strCPU.Left(pos);
	}
	if(m_sCPU != strCPU)
	{
		m_sCPU = strCPU;
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303BB
void CPTAnalog::OnSelchangeCmbPtanalogCpu() 
{
	// TODO: Add your control notification handler code here
	//���浱ǰѡ��
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return;

	if(SaveSelCPU(pCmb))
	{
		//������������
		ReFillAll();
	}	
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������List�е�ֵ�б仯ʱ������ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9303C8
void CPTAnalog::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//ֵ�ĵ�һ����ڶ��в���
				if(nSubItem == m_nNewValueIndex)
				{
					//ֵ
					CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					
					//ȥ�����߿ո�
					strValue.TrimLeft();
					strValue.TrimRight();
					strOldValue.TrimLeft();
					strOldValue.TrimRight();

					//�����ı���ɫ
					if(strValue != strOldValue)
						lplvcd->clrText = XJ_LIST_ONE_TWO;
				}
				//�ڶ���������в���
				if(nSubItem == m_nNewValueIndex + 1)
				{
					//ֵ
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex + 2);

					//ȥ���ո�
					strOldValue.TrimLeft();
					strOldValue.TrimRight();
					strOlderValue.TrimLeft();
					strOlderValue.TrimRight();

					//�����ı���ɫ
					if(strOldValue != strOlderValue)
						lplvcd->clrText = XJ_LIST_TWO_THREE;
				}

				//���
				if(nItem%2 != 0 && nSubItem < m_nNewValueIndex)
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
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

//##ModelId=49B87B9303CC
void CPTAnalog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//ֹͣʱ��
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//�ı�״̬
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			CString str;
			str.Format("����[%s]�ٻ�ģ����ʧ��,ԭ��Ϊ:��ʱ", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
			pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
			//��ʾ
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTAI, str);
	}
	if( nIDEvent == m_nRealtimeValueTimer )
	{
		LoadPTHistoryAnalogData();
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶���������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9302F0
BOOL CPTAnalog::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_SHOW);

	//�������
	ClearAnalogConfig();
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//��ȡ����
	if(g_role == MODE_SUB)
	{
		//ֱ�Ӷ�ȡ
		LoadPTAnalogConfig(0);
		//������ʷֵ��
		AddValueCol(0);
		//�������
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_HIDE);
	}
	else
	{
		//�����߳�
		StartThread();
	}
	
	
	return TRUE;
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ�����ڹر�ʱ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B940040
void CPTAnalog::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ClearDataCol()
 ���ܸ�Ҫ��ɾ���¼ӵ�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302D0
void CPTAnalog::ClearDataCol()
{
	//���������Ч��
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302DE
void CPTAnalog::CancelCall()
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
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20007, m_pObj->m_sID, atoi(m_sCPU));
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

//##ModelId=49B87B9302DF
void CPTAnalog::EndOwnerThread()
{
	WriteLog("��ʼ�˳�����ģ����ҳ���ȡ�����߳�", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while( GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳�����ģ����ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B9302E0
BOOL CPTAnalog::IsVaild20069( int nMsgID )
{
	if(20007 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ�ģ����
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303D8
BOOL CPTAnalog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTANALOG);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����IsVaild20125()
 ���ܸ�Ҫ���ж��Ƿ��Լ����ܵ�20125���� 
 �� �� ֵ: ���Լ����ܵı��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	��������ID
		   Param2
**************************************************************/
//##ModelId=49B87B9302EE
BOOL CPTAnalog::IsVaild20125( int nMsgID )
{
	if(20007 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ�ģ����
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303DB
void CPTAnalog::OnBtnPtanalogStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B93033C
void CPTAnalog::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_ANALOG));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_ANALOG));


	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B9302F1
CString CPTAnalog::TranslateSingleKey( CString sKey )
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
		if(strHead != "$PT_ROUTINE_LIST")
		{
			//������Ǳ�������ֵ, ���ؿ�
			return "";
		}
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_ROUTINE_ID$")
		{
			return m_List.GetItemText(nIndex, 0); //����ֵID
		}
		if(strTail == "PT_ROUTINE_NAME$")
		{
			return m_List.GetItemText(nIndex, 1); //����ֵ����
		}
		if(strTail == "PT_ROUTINE_CODE$")
		{
			return m_List.GetItemText(nIndex, 2); //����ֵ����
		}
		if(strTail == "PT_ROUTINE_UNIT$")
		{
			return m_List.GetItemText(nIndex, 3); //��λ
		}
		//���϶�����,������$PT_ROUTINE_ VALUE#N$����ʽ
		int nValueItem = -1;
		nValueItem = strTail.Find('#', 0);
		if(nValueItem != -1)
		{
			//����#����ʽ, �ж��ǲ���PT_ROUTINE_ VALUE#N
			CString strTemp = strTail;
			strTemp = strTemp.Left(nValueItem);
			if(strTemp != "PT_ROUTINE_VALUE")
			{
				return "";
			}
			//�õ�ֵ����
			strTemp = strTail;
			strTemp.Delete(0, nValueItem+1);
			nValueItem = atoi(strTemp);
			if(nValueItem < 1)
				nValueItem = 1;
			int nSubItem = m_nNewValueIndex + nValueItem - 1;
			return m_List.GetItemText(nIndex, nSubItem);
		}
	}
	else
	{
		//�ж��Ƿ���.GetCount()����
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//��.GetCount()����
			if(sKey == "$PT_ROUTINE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//��ͨ�ؼ���
			if(sKey == "$PT_SEL_CPU$") //��ǰ��ѡCPU
				return m_sCPU;
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
			//���϶�����, �п�����$PT_ROUTINE_TIME#N$�����
			int nValueItem = -1;
			nValueItem = sKey.Find('#', 0);
			if(nValueItem != -1)
			{
				//����#����ʽ, �ж��ǲ���PT_ROUTINE_TIME#N
				CString strTemp = sKey;
				strTemp = strTemp.Left(nValueItem);
				if(strTemp != "$PT_ROUTINE_TIME")
				{
					return "";
				}
				//�õ�ֵ����
				strTemp = sKey;
				strTemp.Delete(0, nValueItem+1);
				nValueItem = atoi(strTemp);
				if(nValueItem < 1)
					nValueItem = 1;
				int nSubItem = m_nNewValueIndex + nValueItem - 1;
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				char temp[256];
				for(int j = 0; j < 256; j++)
				{
					temp[j] = '\0';
				}
				col.cchTextMax = 256;
				col.pszText = temp;
				m_List.GetColumn(nSubItem, &col);
				CString strReturn = temp;
				return strReturn;
			}
		}
	}
	return "";
}

/*************************************************************
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302FD
BOOL CPTAnalog::InitCOM()
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
//##ModelId=49B87B9302FE
void CPTAnalog::ReleaseCOM()
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
 �� �� ����DoPrintAnalog()
 ���ܸ�Ҫ����ӡģ����
 �� �� ֵ: 
 ��    ����param1	��ӡģ��ȫ·��
		   Param2
**************************************************************/
//##ModelId=49B87B9302FF
void CPTAnalog::DoPrintAnalog( CString strPath )
{
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strPath);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRAnalog);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTAnalog);
	}
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

//##ModelId=49B87B940000
void CPTAnalog::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintAnalog(ANALOG_NEW1_TP);
}

//##ModelId=49B87B940002
void CPTAnalog::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintAnalog(ANALOG_NEW2_TP);
}

//##ModelId=49B87B940004
void CPTAnalog::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintAnalog(ANALOG_NEW3_TP);
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ20157, ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B94002E
void CPTAnalog::OnSTTP20157( WPARAM wParam,LPARAM lParam )
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
	
	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
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
		//�����ٻ�ģ����				
		int nTimeOut = pData->m_nPTAiTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTAnalog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTAnalog::OnDoubleclickedBtnPtanalogCall() 
{
	// TODO: Add your control notification handler code here
	
}
/****************************************************
Date:2012/12/3  Author:LY
������:   SetFloatValue	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString strVPickList
*****************************************************/
CString CPTAnalog::SetFloatValue(PT_ANALOG *pst, CString strValue)
{
	if (pst==NULL)
	{
		return strValue;
	}
	CString sValue = strValue;
	if (sValue != "")
	{
		CString sPrecision = pst->Precision;
		
		int nDouhao = sPrecision.Find(",");
		sPrecision.Delete(0,nDouhao+1);
		int nAdd = 3;
		sPrecision.TrimLeft();
		sPrecision.TrimRight();
		if(!sPrecision.IsEmpty())
		{
			nAdd = atoi(sPrecision);			//����
		}
		
		float fValue = atof(sValue);
		CString sFormat;
		sFormat.Format("%%.0%df", nAdd);
		sValue.Format(sFormat, fValue);
	}
	return sValue;
}

/****************************************************
Date:2012/12/6  Author:LYH
������:   FindAnalog	
����ֵ:   PT_ANALOG*	
���ܸ�Ҫ: 
����: CString sID	
*****************************************************/
PT_ANALOG* CPTAnalog::FindAnalog( CString sID )
{
	for(int i = 0; i < m_arrAnalog.GetSize(); i++)
	{
		PT_ANALOG* pa = (PT_ANALOG*)m_arrAnalog.GetAt(i);
		if(pa == NULL)
			continue;
		if(pa->ID == sID)
			return pa;
	}
	return NULL;
}

void CPTAnalog::StartThread()
{	
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTAnalogLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("ģ������ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTAnalog::GetNextQueryNo()
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


void CPTAnalog::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("�˳�����ģ������ѯ�߳�,��ѯ��:%d", nNo);
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
	str.Format("����ģ������ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTAnalog::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("����ģ������ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTAnalog::GetFirstNode()
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


int CPTAnalog::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

/****************************************************
Date:2014/1/9  Author:LYH
������:   OnThreadFillData	
����ֵ:   void	
���ܸ�Ҫ: ����һ���̷߳�������������ʾ��֪ͨ
����: WPARAM wParam	��ѯ����
����: LPARAM lParam	
*****************************************************/
void CPTAnalog::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	if( lParam == 1)
	{
		AddValueCol(wParam);
		FillListData(wParam);
	}
	GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_HIDE);
}

/****************************************************
Date:2014/1/9  Author:LYH
������:   AddValueCol	
����ֵ:   void	
���ܸ�Ҫ: Ϊ�б����һ��������ʾֵ
����: int nQueryNo	
*****************************************************/
void CPTAnalog::AddValueCol( int nQueryNo )
{
	if(!g_PTAIValues)
		return;
	if(m_arrAnalog.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//����ֵ��
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTAnalog::AddValueCol, ��ʼ����ֵ��", XJ_LOG_LV3);
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_HisValueMgr.GetCount(); i++)
	{
		strTime = m_HisValueMgr.GetTime(i);
		if(strTime.ReverseFind('.') != -1)
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		else
			strTimeDisplay = strTime;
		if(IsCurrentQueryNo(nQueryNo) || MODE_SUB == g_role)
			m_List.InsertColumn(m_nNewValueIndex+i, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}
	LeaveCriticalSection(&m_CriticalOper);

	WriteLog("CPTAnalog::AddValueCol, ����ֵ�����", XJ_LOG_LV3);
}

BOOL CPTAnalog::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTAnalog::LoadDefaultCpu, ��ѯ��ʼ", XJ_LOG_LV3);
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
	
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "cpu_code", EX_STTP_DATA_TYPE_INT);
	
	//����
	CString str = "";
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//��cpu����
	Condition condition5;
	str.Format("order by cpu_code");
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);//��where
	
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadDefaultCpu, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadDefaultCpu,��ѯ��%d������", nCount);
		WriteLogEx(str);
		if( nCount >= 1 )
		{
			str = pMemset.GetValue((UINT)0); //ID
			int c = atoi(str);
			for( int k = 0; k < pCmb->GetCount(); k++ )
			{
				CSecCPU * cpu =  (CSecCPU *)pCmb->GetItemData(k);
				if( cpu != NULL && cpu->code == c)
				{
					pCmb->SetCurSel(k);
					bReturn = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadDefaultCpu,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTAnalog::LoadDefaultCpu, ��ѯ����", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTAnalog::startRealtimeValueTimer()
{
	if( g_PTAIRealtimeValue )
		m_nRealtimeValueTimer = SetTimer(99, g_PTAIRealtimeRefresh*1000, 0);
}

void CPTAnalog::stopRealtimeValueTimer()
{
	if( g_PTAIRealtimeValue && m_nRealtimeValueTimer)
		KillTimer(m_nRealtimeValueTimer);
}

void CPTAnalog::OnBtnPtanalogExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_ANALOG),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
