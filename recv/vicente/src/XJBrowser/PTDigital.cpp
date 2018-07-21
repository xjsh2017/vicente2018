// PTDigital.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTDigital.h"
#include "DataEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT PTDigitalLoad(LPVOID pParam)
{
	CPTDigital* ptDigital = (CPTDigital*)pParam;
	if(ptDigital == NULL)
		return 1;
	int nQueryNo = ptDigital->GetCurrentQueryNo();
	
	if(ptDigital->IsCurrentQueryNo(nQueryNo) && !ptDigital->m_bExitThread)
	{
		BOOL bLoad = ptDigital->LoadPTDigitalConfig(nQueryNo);
		if(bLoad)
		{
			bLoad = ptDigital->LoadPTHistoryDigitalData(nQueryNo);
		}
		if(bLoad)
			ptDigital->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptDigital->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}

	ptDigital->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTDigital

IMPLEMENT_DYNCREATE(CPTDigital, CViewBase)

//##ModelId=49B87B920179
CPTDigital::CPTDigital()
	: CViewBase(CPTDigital::IDD)
{
	//{{AFX_DATA_INIT(CPTDigital)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_sCPU = -1;
	m_arrDigital.RemoveAll();
	m_nNewValueIndex = 3;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B9203AD
CPTDigital::~CPTDigital()
{
	ClearDigitalConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B92037A
void CPTDigital::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTDigital)
	DDX_Control(pDX, IDC_GIF_PTDIGITAL, m_gif);
	DDX_Control(pDX, IDC_BTN_PTDI_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTDI_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTDI_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTDI, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTDigital, CViewBase)
	//{{AFX_MSG_MAP(CPTDigital)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTDI_CALL, OnBtnPtdiCall)
	ON_CBN_SELCHANGE(IDC_CMB_PTDI_CPU, OnSelchangeCmbPtdiCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTDI, OnCustomDraw)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTDI_STYLE, OnBtnPtdiStyle)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTDI_CALL, OnDoubleclickedBtnPtdiCall)
	ON_BN_CLICKED(IDC_BTN_PTDI_EXCEL, OnBtnPtdiExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTDI, OnRefreshPTDI)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTDigital diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9203C8
void CPTDigital::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B930000
void CPTDigital::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTDigital message handlers

//##ModelId=49B87B92036B
void CPTDigital::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	RecordRate(IDC_BTN_PTDI_CALL, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_PRINT, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_EXCEL, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_STYLE, 0, left_client, null_effect, null_effect, bottom_client);
	RecordRate(IDC_STATIC_PTDI, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_CMB_PTDI_CPU, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_GIF_PTDIGITAL, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTDI, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTDI_P1, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_STATIC_PTDI_P2, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_STATIC_PTDIGITAL_LOADING, 0, mid_client, top_client);

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
//##ModelId=49B87B920243
int CPTDigital::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;

	InitListStyle();
/*	RefreshControls();
	LoadPTDigitalConfig();
	LoadPTHistoryDigitalData();*/

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B920261
int CPTDigital::InitListStyle()
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
		//char* arColumn[3]={"���","����������", "����������"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 3 ; nCol++)
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
				lvCol.cx = 140; // ����������
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // ����������
				colName = StringFromID(IDS_COMMON_CODE);
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
		//Ĭ�����ص�3��(����������)
		m_List.SetColumnHide(2, TRUE);
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
//##ModelId=49B87B920290
BOOL CPTDigital::LoadListConfig()
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
			WriteLog("PTDigital::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTDigital::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTDigitalConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTDigitalConfig", XJ_LOG_LV3);
				WriteLog("PTDigital::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTDigitalConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTDigital::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B92029F
BOOL CPTDigital::SaveListConfig()
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
			WriteLog("PTDigital::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTDigital::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTDigitalConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTDigitalConfig");
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
						WriteLog("δ�ҵ�PTDigitalConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTDigitalConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B920271
int CPTDigital::FillListData(int nQueryNo)
{
	WriteLog("CPTDigital::FillListData, ��ʼ�������", XJ_LOG_LV3);
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	CString strLog;
	strLog.Format("CPTDigital::FillListData, ����%d����ʷֵ", m_HisValueMgr.GetCount());
	WriteLog(strLog, XJ_LOG_LV3);
	PT_DIGITAL *pts = NULL;
	for(int i = 0; i < m_arrDigital.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;

		pts = m_arrDigital.GetAt(i);
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", i+1);
		}
		m_List.InsertItem(i, sID);
		m_List.SetItemText(i, 1, pts->Name);
		m_List.SetItemText(i, 2, pts->CodeName);
		for(int k = 0; k < m_HisValueMgr.GetCount(); k++)
		{
			CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
			m_List.SetItemText(i, m_nNewValueIndex+k, DisplayValue(atoi(sHisValue)));
		}
		m_List.SetItemData(i, (DWORD)pts);
	}
	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	WriteLog("CPTDigital::FillListData, ����������", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B9202AF
BOOL CPTDigital::NeedSave()
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
//##ModelId=49B87B9202BF
int CPTDigital::InitControls()
{
	/*
	//�ٻ���ť
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ�������"));

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

//##ModelId=49B87B93001F
void CPTDigital::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTDI, left_client, IDC_BTN_PTDI_CALL, right_client, bottom_client);
}

//##ModelId=49B87B93003E
void CPTDigital::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����LoadPTDigitalConfig()
 ���ܸ�Ҫ�������ݿ��ж�ȡ������������Ϣ,tb_pt_di_def
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9202DE
BOOL CPTDigital::LoadPTDigitalConfig(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	WriteLogEx("CPTDigital::LoadPTDigitalConfig, ��ʼ��ѯ");

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

	//��DI_ID��С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by DI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);

	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
			
	CMemSet pMemset;		
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadPTDigitalConfig, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTDigital::LoadPTDigitalConfig,��ѯ��%d������", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//��������������
			PT_DIGITAL * digital = new PT_DIGITAL;
			digital->ID = pMemset.GetValue((UINT)0); //DI_ID
			digital->Name = pMemset.GetValue(1); //NAME
			digital->CodeName = pMemset.GetValue(2); //Code_Name
			m_arrDigital.Add(digital);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTDigital::LoadPTDigitalConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLogEx("CPTDigital::LoadPTDigitalConfig, ��ѯ����");

	return bReturn;
}

/*************************************************************
 �� �� ����ClearDigitalConfig()
 ���ܸ�Ҫ�����������������Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9202EE
BOOL CPTDigital::ClearDigitalConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL * digital = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		delete digital;
		digital = NULL;
	}
	m_arrDigital.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 �� �� ����AddDigitalValue()
 ���ܸ�Ҫ������ĳ�ؿ���ֵ���б�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ����Ŀ�������ID
           Param2   Ҫ����Ŀ�������ֵ
		   Param3	ָ��Ҫ���뵽�ڼ���, ��ֵӦ��>=3
**************************************************************/
//##ModelId=49B87B9202EF
BOOL CPTDigital::AddDigitalValue(CString ID, CString VALUE, int iCol)
{
	//���ҵ�DI_ID==ID������
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_DIGITAL* pst = FindDigital(strID);
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
	bReturn = m_List.SetItemText(nItem, iCol/*������Ϊ����ֵ*/, DisplayValue(atoi(VALUE)));
	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTHistoryDigitalData()
 ���ܸ�Ҫ�����뿪������ʷֵ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B92030D
BOOL CPTDigital::LoadPTHistoryDigitalData(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_PTDIValues <= 0)
		return TRUE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;	

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, ��ʼ��ѯtime");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//ȡ�����ݿ�������ʷ���ݵ�Time�б�
	CString str;
		
	//�ֶ�

	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);

	//CURTIME
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "CURTIME";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field1);
		
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

	//��CURTIME��С����
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by CURTIME DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);

	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
				
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTDigital::LoadPTHistoryDigitalData,��ѯ��%d��time", nCount);
		WriteLog(strLog, XJ_LOG_LV3);

		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(i >= g_PTDIValues)
				break;
			CString str;
			str = pMemset.GetValue((UINT)0);
			m_HisValueMgr.CreateValue(str);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTDigital::LoadPTHistoryDigitalData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	if(!bReturn)
		return FALSE;

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, ��ѯtime����");
	
	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, ��ʼ��ѯ��ʷֵ");

	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);

	//���ÿһ��
	//�����ݿ����ÿ��Time��Ӧ�Ķ�ֵ��
	for (int i = 0; i < nHisCount; i ++)
	{
		if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(MODE_SUB != g_role)
			Sleep(1);
		EnterCriticalSection(&m_CriticalOper);
		CString strTime = m_HisValueMgr.GetTime(i);
		LeaveCriticalSection(&m_CriticalOper);
		
		//�����ݿ����ÿ��Time��Ӧ�Ŀ�������

		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//ȡ�����ݿ�������ʷ���ݵ�Time�б�
		CString str;
			
		//�ֶ�
		//CURTIME
		Field Field1;
		bzero(&Field1, sizeof(Field));
		str = "DI_ID";
		strncpy(Field1.FieldName, str, str.GetLength());
		Field1.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field1);

		Field Field2;
		bzero(&Field2, sizeof(Field));
		str = "VALUE";
		strncpy(Field2.FieldName, str, str.GetLength());
		Field2.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field2);
			
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

		//CURTIME
		Condition condition3;
		bzero(&condition3, sizeof(Condition));
		str.Format("CURTIME = '%s'", strTime);
		strncpy(condition3.ConditionContent, str, str.GetLength());
		sql.Conditionlist.push_back(condition3);

		if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
				
		CMemSet pMemset;
			
		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLog("CPTDigital::LoadPTHistoryDigitalData, ��ѯʧ��", XJ_LOG_LV3);
			return FALSE;
		}
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTDigital::LoadPTHistoryDigitalData,��ѯ%s����ʷ����,����Ϊ%d", strTime, nCount);
			WriteLog(str, XJ_LOG_LV3);
			int nItem = 0;
			EnterCriticalSection(&m_CriticalOper);
			for(int k = 0; k < nCount; k++)
			{
				if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
					break;
				if(MODE_SUB != g_role && k%20 == 0)
					Sleep(1);

				CString strID, strValue;
				strID = pMemset.GetValue((UINT)0);
				strValue = pMemset.GetValue(1);

				m_HisValueMgr.AddValue(i, atoi(strID), strValue);
				pMemset.MoveNext();
			}
			LeaveCriticalSection(&m_CriticalOper);
		}
		else
		{
			CString str;
			str.Format("CPTDigital::LoadPTHistoryDigitalData,��ѯʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
	}

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, ��ѯ��ʷֵ����", XJ_LOG_LV3);
	return bReturn;
}

/*************************************************************
 �� �� ����OnSTTP20012()
 ���ܸ�Ҫ����Ӧ20012(��������������ֵ����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B930090
void CPTDigital::OnSTTP20012( WPARAM wParam,LPARAM lParam )
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
	
	//����Ƿ�20012����
	if(pSttpData->sttp_head.uMsgID != 20012)
	{
		//������
		CString str;
		str.Format("CPTDigital::OnSTTP20012 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTDigital::OnSTTP20012 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20012 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if(m_bFirstFrame)
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
		WriteLog("CPTDigital::OnSTTP20012 ������ʧ��", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 )
	{
		//���к���֡
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_SUCCESS);
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;

	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();
	CString str;
	str.Format("����[%s]�ٻ��������ɹ�", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

//##ModelId=49B87B93004E
void CPTDigital::OnBtnPtdiCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���������ȡ���ٻ�
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

		//��֯20011����
		if(!pApp->m_SttpEngine.BuildDataFor20011PTDigital(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
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
				str.Format("����[%s]�����ٻ�����������ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ�����������ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ�������
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;

			//������ʱ��
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTDiTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//���ݵ�ǰ״̬���¿ؼ�������
			UpdateControlsEnable();
			CString str;
			str.Format("����[%s]�·��������ٻ�����ɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else 
	{
		//�����ٻ�������
		//ȡ���ٻ�
		CancelCall();
	}
}

/*************************************************************
 �� �� ����LoadDataFromSTTP()
 ���ܸ�Ҫ����STTP���Ķ��뿪��������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1 ��������
**************************************************************/
//##ModelId=49B87B92031C
BOOL CPTDigital::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//��������������
	try
	{
		
		//��ȡÿ��ֵ,���뵽�б�
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddDigitalValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTDigital::LoadDataFromSTTP, ����", XJ_LOG_LV3);
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
//##ModelId=49B87B93009C
void CPTDigital::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���������״̬,��������
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
		//���Ǳ�װ�õı���
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
		str.Format("����[%s]�ٻ�������ʧ��!", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;

	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();
	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֹ��ܻ�Ӧ����)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9300AB
void CPTDigital::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���������״̬,��������
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
	
	//������
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
			
	//���ݵ�ǰ״̬���¿ؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ�������ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);

	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B92031E
void CPTDigital::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//�ٻ���ť���ָ�Ϊ�ٻ�
		SetDlgItemText(IDC_BTN_PTDI_CALL, StringFromID(IDS_BTN_CALLDIGITAL));
		
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTDI_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTDI_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTDI_STYLE) ->EnableWindow(TRUE);
		
		//������
		GetDlgItem(IDC_CMB_PTDI_CPU) ->EnableWindow(TRUE);
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
		SetDlgItemText(IDC_BTN_PTDI_CALL, CANCEL_CALL);
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTDI_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTDI_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTDI_STYLE) ->EnableWindow(FALSE);
		
		//������
		GetDlgItem(IDC_CMB_PTDI_CPU) ->EnableWindow(FALSE);
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTDI, str);
	}
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ, ˢ��ҳ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B9300CB
void CPTDigital::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	RefreshControls();
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//����ٻ���ʱ
	SetDlgItemText(IDC_STATIC_TIME_PTDI, "");

	ReFillAll();
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ, �ڵ�һ�������͸��¶�Ӧ����ʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B92032C
void CPTDigital::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
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
//##ModelId=49B87B92032D
BOOL CPTDigital::SaveSelCPU( CComboBox * pCmb )
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

//##ModelId=49B87B930050
void CPTDigital::OnSelchangeCmbPtdiCpu() 
{
	// TODO: Add your control notification handler code here
	//���浱ǰѡ���CPU
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
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
 ���ܸ�Ҫ�����ò���ʱlist���ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B93005D
void CPTDigital::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

//##ModelId=49B87B930061
void CPTDigital::OnTimer(UINT nIDEvent) 
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
			CString str;
			str.Format("����[%s]�ٻ�������ʧ��,ԭ��Ϊ:��ʱ", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);
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
		SetDlgItemText(IDC_STATIC_TIME_PTDI, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B92034D
BOOL CPTDigital::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_SHOW);
	
	//�������
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearDigitalConfig();
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//��ȡ����
	if(MODE_SUB == g_role)
	{
		//ֱ�Ӷ�ȡ
		LoadPTDigitalConfig(0);
		LoadPTHistoryDigitalData(0);
		AddValueCol(0);
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_HIDE);
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
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9300CE
void CPTDigital::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87B92033C
void CPTDigital::ClearDataCol()
{
	//���������Ч��
	if(m_List.GetSafeHwnd() == NULL)
		return;
	EnterCriticalSection(&m_CriticalOper);
	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
	LeaveCriticalSection(&m_CriticalOper);
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92033D
void CPTDigital::CancelCall()
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
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20011, m_pObj->m_sID, atoi(m_sCPU));
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92035B
void CPTDigital::EndOwnerThread()
{
	WriteLog("��ʼ�˳�����������ҳ���ȡ�����߳�", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳�����������ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B92033E
BOOL CPTDigital::IsVaild20069( int nMsgID )
{
	if(20011 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ�������
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B93006E
BOOL CPTDigital::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTDI);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����DisplayValue()
 ���ܸ�Ҫ���õ���������ֵ��ʾ�ַ���
 �� �� ֵ: ��ʾֵ
 ��    ����param1	������ֵ
		   Param2
**************************************************************/
//##ModelId=49B87B92034B
CString CPTDigital::DisplayValue( int nValue )
{
	//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��, 4:����/����.
	if(0 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "1";
		else if(0 == nValue)
			return "0";
	}
	if(1 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "ON";
		else if(0 == nValue)
			return "OFF";
	}
	if(2 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_COMMISSIONING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CEASING);
	}
	if(3 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_OPENING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CLOSING);
	}
	if(4 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_ACTION);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_RETURN);
	}
	CString str;
	str.Format("%d", nValue);
	return str;
}

//##ModelId=49B87B9300DC
void CPTDigital::OnRefreshPTDI( WPARAM wParam, LPARAM lParam )
{
	//ReFillAll();
	StartThread();
}

//##ModelId=49B87B93007D
void CPTDigital::OnBtnPtdiStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B920399
void CPTDigital::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_DIGITAL));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_DIGITAL));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B93007F
void CPTDigital::OnShowWindow(BOOL bShow, UINT nStatus) 
{

	CViewBase::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B92035C
CString CPTDigital::TranslateSingleKey( CString sKey )
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
//##ModelId=49B87B92035E
BOOL CPTDigital::InitCOM()
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
//##ModelId=49B87B92035F
void CPTDigital::ReleaseCOM()
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
 ���ܸ�Ҫ����ӡ������
 �� �� ֵ: ��ӡģ��ȫ·��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920360
void CPTDigital::DoPrintDigital( CString strPath )
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
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL), strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRDigital);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTDigital);
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

//##ModelId=49B87B930083
void CPTDigital::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintDigital(DIGITAL_NEW1_TP);
}

//##ModelId=49B87B93008C
void CPTDigital::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintDigital(DIGITAL_NEW2_TP);
}

//##ModelId=49B87B93008E
void CPTDigital::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintDigital(DIGITAL_NEW3_TP);
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9300BB
void CPTDigital::OnSTTP20157( WPARAM wParam,LPARAM lParam )
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
		//�����ٻ�������				
		int nTimeOut = pData->m_nPTDiTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTDigital::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTDigital::OnDoubleclickedBtnPtdiCall() 
{
	// TODO: Add your control notification handler code here
	
}

/****************************************************
Date:2012/12/6  Author:LYH
������:   FindDigital	
����ֵ:   PT_DIGITAL*	
���ܸ�Ҫ: 
����: CString sID	
*****************************************************/
PT_DIGITAL* CPTDigital::FindDigital( CString sID )
{
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* pd = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(pd == NULL)
			continue;
		if(pd->ID == sID)
			return pd;
	}
	return NULL;
}

void CPTDigital::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTDigitalLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("ģ������ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTDigital::GetNextQueryNo()
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

void CPTDigital::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("�˳�������������ѯ�߳�,��ѯ��:%d", nNo);
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
	str.Format("������������ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTDigital::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("������������ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTDigital::GetFirstNode()
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

void CPTDigital::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	
	AddValueCol(wParam);
	FillListData(wParam);

	GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_HIDE);
}

void CPTDigital::AddValueCol( int nQueryNo )
{
	if(g_PTDIValues <= 0)
		return;
	if(m_arrDigital.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//����ֵ��
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTDigital::AddValueCol, ��ʼ����ֵ��", XJ_LOG_LV3);
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
	
	WriteLog("CPTDigital::AddValueCol, ����ֵ�����", XJ_LOG_LV3);
}

int CPTDigital::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTDigital::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTDigital::LoadDefaultCpu, ��ѯ��ʼ", XJ_LOG_LV3);
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadDefaultCpu, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTDigital::LoadDefaultCpu,��ѯ��%d������", nCount);
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
		str.Format("CPTDigital::LoadDefaultCpu,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTDigital::LoadDefaultCpu, ��ѯ����", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTDigital::OnBtnPtdiExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_DIGITAL),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
