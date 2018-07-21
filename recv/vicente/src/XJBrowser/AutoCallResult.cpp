// AutoCallResult.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AutoCallResult.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
/** @brief           ����������λ*/
int g_iResultViewAsc;

CString g_sRootDirPath;

UINT ExportByStation(LPVOID pParam)
{
	CAutoCallResult* pView = (CAutoCallResult*)pParam;
	if( !pView )
		return -1;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	Sleep(10);
	DEVICE_LIST	listStation;
	pData->GetStationList( listStation );

	POSITION pos = listStation.GetHeadPosition();
	int total = listStation.GetCount();
	int count = 0;
	int success = 0;
	int fail = 0;
	while(pos != NULL)
	{
		CStationObj*pStation = (CStationObj*)listStation.GetNext(pos);
		if(pStation == NULL)
			continue;
		CString strDir = g_sRootDirPath + "\\" + pStation->m_sNetID;
		CFileFind ifind;
		if( !ifind.FindFile( strDir ) )
			CreateDirectory( strDir, 0 );
		CString sFilePath = strDir + "\\" + pStation->m_sName + ".xls" ;

		if( pView->WriteResultToExcel( sFilePath, pStation->m_sID ) )
			success++;
		else
		{
			fail++;
			CString strLog;
			strLog.Format("[%s] %s", pStation->m_sID, IDS_AUTOCALLRESULT_EXPORT_FAIL );
			WriteLog( strLog, XJ_LOG_LV1 );
		}
		
		count++;
		if( pView->m_pExportProcess )
			pView->m_pExportProcess->PostMessage( EXPORT_PROCESS, total, count );
		Sleep( 1 );
	}
	if( pView->m_pExportProcess )
		pView->m_pExportProcess->PostMessage( EXPORT_RESULT, success, fail );
	return 0;
}

#define CALLRELUST_LIST_COLUMNS	25
//////////////////////////////////////////////////////////////////////////
//CCallResult
//##ModelId=49B87BB303CA
CCallResult::CCallResult()
{
	/** @brief           ��ֵ���Ž��*/
	m_nZone = 0;
	/** @brief           ��ֵ���*/
	m_nSetting = 0;
	/** @brief           ���������*/
	m_nDigital = 0;
	/** @brief           ��ѹ����*/
	m_nSoftBoard = 0;
	/** @brief           ģ�������*/
	m_nAnalog = 0;

	/** @brief           ��ֵ���Ž��˵��*/
	m_sZoneNote = ERROR_NULL;
	/** @brief           ��ֵ���˵��*/
	m_sSettingNote = ERROR_NULL;
	/** @brief           ���������˵��*/
	m_sDigitalNote = ERROR_NULL;
	/** @brief           ��ѹ����˵��*/
	m_sSoftBoardNote = ERROR_NULL;
	/** @brief           ģ�������˵��*/
	m_sAnalogNote = ERROR_NULL;
	
	/** @brief           ��������*/
	m_nSaveNo = 0;

	/** @brief           �����豸ID*/
	m_sSecID = "";
	/** @brief           �����豸ָ��*/
	m_pSec = NULL;
	/** @brief           CPU��*/
	m_nCPU = -1;
	/** @brief           ���ʱ��*/
	CTime tm(1972, 1, 1, 1, 1, 1);
	m_tmEnd = tm;
	/** @brief           ��վID*/
	m_sStationId = "";
	/** @brief           ��վָ��*/
	m_pStation = NULL;

	m_sSituation = "";
	m_sDispose = "";
}

//##ModelId=49B87BB40000
CCallResult::~CCallResult()
{
	
}

/*************************************************************
 �� �� ����LoadDataFromDB()
 ���ܸ�Ҫ�������ݿ����������Ϣ
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݿ����
		   Param2
**************************************************************/
//##ModelId=49B87BB4006D
BOOL CCallResult::LoadDataFromDB( CMemSet* pMemset )
{
	//���������Ч��
	if(pMemset == NULL)
		return FALSE;
	try
	{
		m_sSecID = pMemset->GetValue((UINT)0); //����ID
		CString str;
		str = pMemset->GetValue(1); //CPU
		m_nCPU = atoi(str);
		str = pMemset->GetValue(2); //���ʱ��
		m_tmEnd = StringToTime(str);
		m_sStationId = pMemset->GetValue(3); //��վID
		str = pMemset->GetValue(4);
		m_nZone = atoi(str); //��ֵ�����ٻ����
		str = pMemset->GetValue(5);
		m_nSetting = atoi(str); //��ֵ�ٻ����
		str = pMemset->GetValue(6);
		m_nSoftBoard = atoi(str); //��ѹ���ٻ����
		str = pMemset->GetValue(7);
		m_nAnalog = atoi(str); //ģ�����ٻ����
		str = pMemset->GetValue(8);
		m_nDigital = atoi(str); //�������ٻ����
		str = pMemset->GetValue(10);
		m_nHistory = atoi(str);//��ʷ�¼��ٻ����
		str = pMemset->GetValue(11);
		m_nOsc = atoi(str);//¼���ٻ����

		if(m_nHistory == 2)
			m_sHistoryNote = pMemset->GetValue(12);
		if(m_nOsc == 2)
			m_sOscNote = pMemset->GetValue(13);
		if(m_nZone == 2)
			m_sZoneNote = pMemset->GetValue(14); //��ֵ���Ž��˵��
		if(m_nSetting == 2)
			m_sSettingNote = pMemset->GetValue(15); //��ֵ���˵��
		if(m_nDigital == 2)
			m_sDigitalNote = pMemset->GetValue(16); //���������˵��
		if(m_nSoftBoard == 2)
			m_sSoftBoardNote = pMemset->GetValue(17); //��ѹ����˵��
		if(m_nAnalog == 2)
			m_sAnalogNote = pMemset->GetValue(18); //ģ�������˵��
		str = pMemset->GetValue(19);
		m_nSaveNo = atoi(str); //����
		m_sSituation = pMemset->GetRealDataValue( "call_notes" ); //���˵��
		m_sDispose = pMemset->GetRealDataValue( "call_suggest" ); //�������
	
	}
	catch (...)
	{
		WriteLog("CCallResult::LoadDataFromDB ��ȡ���ݳ���", 3);
	}
	
	if(!BuildReleation())
	{
		WriteLog("CCallResult::LoadDataFromDB, ���豸����ʧ��");
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildReleation()
 ���ܸ�Ҫ���������豸�Ĺ���
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4008C
BOOL CCallResult::BuildReleation()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;
	//�Ȳ��Ҷ����豸ָ��
	m_pSec = (CSecObj*)pData->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL)
		return FALSE;
	
	//��վID
	CStationObj * pStation = (CStationObj*)pData->FindDevice(m_sStationId, TYPE_STATION);
	m_pStation = pStation;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult

IMPLEMENT_DYNCREATE(CAutoCallResult, CFormView)

//##ModelId=49B87BB40158
CAutoCallResult::CAutoCallResult()
	: CFormView(CAutoCallResult::IDD)
{
	//{{AFX_DATA_INIT(CAutoCallResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_listResult.RemoveAll();
	m_arrSaveNo.RemoveAll();

	g_iResultViewAsc = -1;

	m_nNewSaveNo = 0;
	m_nOldSaveNo = 0;
	m_nMaxSaveNo = 10;
	m_nMinSaveNo = 1;

	m_ZoneStat.nFail = 0;
	m_ZoneStat.nSuccess = 0;

	m_SettingStat.nSuccess =0;
	m_SettingStat.nFail = 0;

	m_DigitalStat.nFail = 0;
	m_DigitalStat.nSuccess = 0;

	m_SoftBoardStat.nSuccess = 0;
	m_SoftBoardStat.nFail = 0;

	m_AnalogStat.nFail = 0;
	m_AnalogStat.nSuccess = 0;

	CTime tm(1971, 1, 1, 1, 1, 1);
	m_tmStart = tm;
	m_tmEnd = tm;

	m_nOldSortCol = -1;

	m_pStation = NULL;

	m_nSelSaveNo = 0;

	m_bShowNoFailed = TRUE;

	m_bSelectRoot = FALSE;
}

//##ModelId=49B87BB403C8
CAutoCallResult::~CAutoCallResult()
{
	Clear();
	ClearSaveNo();
}

//##ModelId=49B87BB403A9
void CAutoCallResult::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoCallResult)
	DDX_Control(pDX, IDC_LIST_AUTOCALL_SUMMARY, m_ListSummary);
	DDX_Control(pDX, IDC_LIST_AUTOCALL_RESULT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoCallResult, CFormView)
	//{{AFX_MSG_MAP(CAutoCallResult)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_AUTOCALL_RESULT, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_AUTOCALL_RESULT, OnColumnclickListAutocallResult)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_RESULT_EXPORT, OnResultExport)
	ON_COMMAND(IDC_RESULT_PRINT, OnResultPrint)
	ON_COMMAND(IDC_RESULT_PREV, OnResultPrev)
	ON_COMMAND(IDC_RESULT_NEXT, OnResultNext)
	ON_COMMAND(IDC_RESULT_SHOWFAILD, OnResultShowfaild)
	ON_COMMAND(IDC_WIN_CLOSE, OnWinClose)
	ON_COMMAND(IDC_RESULT_STATIONEXPORT, OnResultStationexport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(AUTOCALL_RESULT_OPEN, OnAutoCallResultFrameOpen)
	ON_MESSAGE(AUTOCALL_RESULT_CHANGE, OnAutoCallResultChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB50000
void CAutoCallResult::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB5000F
void CAutoCallResult::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult message handlers

//##ModelId=49B87BB4038A
void CAutoCallResult::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//���ÿɹ�����ͼ�ı�Ҫ����
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);

	//��ʼ���б����ʽ
	InitListStyle();
	InitSummaryListStyle();
	
}

//##ModelId=49B87BB5002E
void CAutoCallResult::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BB40232
BOOL CAutoCallResult::LoadListConfig()
{
	//��ȡUI�����ļ�
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //��ʼ��COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", 3);
			WriteLog("AutoCallResultConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("AutoCallResultConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//AutoCallResultConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�AutoCallResultConfig");
				WriteLog("AutoCallResultConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�AutoCallResultConfig�µ�����");
					WriteLog("AutoCallResultConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() == CALLRELUST_LIST_COLUMNS)
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
					else
						bResult = FALSE;
				}
			}
		}		
// 	}	
// 	//�رմ򿪵�COM
// 	CoUninitialize();
	return bResult;
}

//##ModelId=49B87BB40242
BOOL CAutoCallResult::SaveListConfig()
{
	//����UIConfigʧ��", 3
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //��ʼ��COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", 3);
			WriteLog("AutoCallResultConfig::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("AutoCallResultConfig::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//AutoCallResultConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�AutoCallResultConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("AutoCallResultConfig");
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
						WriteLog("δ�ҵ�BatchDetailViewConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���AutoCallResultConfig�µ�����, ��ɾ���½������ӽڵ�");
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
// 	}
// 	//�رմ򿪵�COM
// 	CoUninitialize();
	return bResult;
}

/*************************************************************
 �� �� ����NeedSave()
 ���ܸ�Ҫ���ж��Ƿ�����Ϣ�иı���Ҫ����
 �� �� ֵ: ��Ҫ���淵��TRUE, ����Ҫ���淵��FALSE
**************************************************************/
//##ModelId=49B87BB40261
BOOL CAutoCallResult::NeedSave()
{
	BOOL bReturn = FALSE;

	//�ȶ�ȡ��������Ϣ
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
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С,λ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40271
void CAutoCallResult::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(m_ListSummary.GetSafeHwnd() == NULL)
		return;
	//�����ĸ߶�Ϊ60, �����Ķ�Ϊ��ϸ
	CRect rc;
	GetClientRect(&rc); //�ͻ�����С
	
	CRect rcSummary = rc;
	rcSummary.bottom = rcSummary.top + 90;
	m_ListSummary.MoveWindow(rcSummary);
	
	CRect rect = rc;
	rect.top  = rcSummary.bottom;
	m_List.MoveWindow(rect);
}

/*************************************************************
 �� �� ����OnAutoCallResultFrameOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ, �����ؼ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5009E
void CAutoCallResult::OnAutoCallResultFrameOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//��ʼ������
// 	LoadDataFromDB();
// 	FillListData();
//  	GetOCTime("OC_LSTARTTIME", m_tmStart);
//  	GetOCTime("OC_LENDTIME", m_tmEnd);
// 	FillSummaryListData();
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40280
int CAutoCallResult::InitListStyle()
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
		
		CString colName="";
		for (int nCol=0; nCol < CALLRELUST_LIST_COLUMNS; nCol++)
		{
			colName="";
			lvCol.iSubItem=nCol;
			lvCol.cx = 100;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; //"����"
				colName.LoadString(IDS_AUTOCALLRESULT_NUMBER);
				break;
			case 1:
				lvCol.cx = 300;
				colName.LoadString(IDS_MODEL_SECONDARY);
				break;
			case 2:
				colName.LoadString(IDS_MODEL_CPU);
				break;
			case 3:
				colName.LoadString(IDS_AUTOCALLRESULT_ZONE);
				break;
			case 4:
				colName.LoadString(IDS_AUTOCALLRESULT_ZONE_NOTE);
				break;
			case 5:
				colName.LoadString(IDS_AUTOCALLRESULT_SETTING);
				break;
			case 6:
				colName.LoadString(IDS_AUTOCALLRESULT_SETTING_NOTE);
				break;
			case 7:
				colName.LoadString(IDS_AUTOCALLRESULT_DIGITAL);
				break;
			case 8:
				colName.LoadString(IDS_AUTOCALLRESULT_DIGITAL_NOTE);
				break;
			case 9:
				colName.LoadString(IDS_AUTOCALLRESULT_SOFT);
				break;
			case 10:
				colName.LoadString(IDS_AUTOCALLRESULT_SOFT_NOTE);
				break;
			case 11:
				colName.LoadString(IDS_AUTOCALLRESULT_ANALOG);
				break;
			case 12:
				colName.LoadString(IDS_AUTOCALLRESULT_ANALOG_NOTE);
				break;
			case 13:
				colName.LoadString(IDS_AUTOCALLRESULT_OSC);
				break;
			case 14:
				colName.LoadString(IDS_AUTOCALLRESULT_OSC_NOTE);
				break;
			case 15:
				colName.LoadString(IDS_AUTOCALLRESULT_HISEVENT);
				break;
			case 16:
				colName.LoadString(IDS_AUTOCALLRESULT_HISEVENT_NOTE);
				break;
			case 17:
				colName.LoadString(IDS_COMMON_FINISHTIME);
				break;
			case 18:
				colName.LoadString(IDS_AUTOCALLRESULT_OVERVIEW);
				break;
			case 19:
				colName.LoadString(IDS_AUTOCALLRESULT_RESOLUTION);
				break;
			case 20:
				colName.LoadString(IDS_MODEL_SUBSTATION);
				break;
			case 21:
				colName.LoadString(IDS_MODEL_PRIMARY);
				break;
			case 22:
				colName.LoadString(IDS_MODEL_MANUFACTURER);
				break;
			case 23:
				colName.LoadString(IDS_MODEL_MANAGEMENT);
				break;
			case 24:
				colName.LoadString(IDS_MODEL_MAINTENANCE);
				break;
			default:
				lvCol.cx=100;//�п�100����
			}
			lvCol.pszText = colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
 		}
 	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	//	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 �� �� ����InitSummaryListStyle()
 ���ܸ�Ҫ����ʼ�������б��ķ��
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40290
int CAutoCallResult::InitSummaryListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,16,TRUE|ILC_COLOR32,1,0);   
	m_ListSummary.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;

	CString colName="";
	for (int nCol=0; nCol < 9 ; nCol++)
	{
		colName="";
		lvCol.iSubItem=nCol;
		lvCol.cx = 100;
		switch(nCol)
		{
		case 0://
			lvCol.cx = 70; //�п�50����
			colName.LoadString(IDS_COMMON_ITEM);
			break;
		case 1:
			colName.LoadString(IDS_SECPROP_ZONE);
			break;
		case 2:
			colName.LoadString(IDS_SECPROP_SETTING);
			break;
		case 3:
			colName.LoadString(IDS_SECPROP_DIGITAL);
			break;
		case 4:
			colName.LoadString(IDS_SECPROP_SOFT);
			break;
		case 5:
			colName.LoadString(IDS_SECPROP_ANALOG);
			break;
		case 6:
			colName.LoadString(IDS_SECPROP_OSC);
			break;
		case 7:
			colName.LoadString(IDS_SECPROP_HISTORYEVENT);
			break;
		case 8:
			lvCol.cx = 240;
			colName.LoadString(IDS_AUTOCALLRESULT_SUMMARY);
			break;
		default:
			lvCol.cx=100;//�п�100����
			break;
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_ListSummary.InsertColumn(nCol,&lvCol);
	}

	//���÷��
	m_ListSummary.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);

	return 1;
}

/*************************************************************
 �� �� ����LoadDataFromDB()
 ���ܸ�Ҫ�������ݿ��������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4029F
BOOL CAutoCallResult::LoadDataFromDB(int nSaveNo)
{
	//���������
	Clear();
	//���ͳ�ƽ��
	ClearStat();
	
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;		
	
	//����
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (m_pStation != NULL)
	{
		Condition con2;
		str.Format("station_id = '%s'", m_pStation->m_sID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}
	else if( !pApp->MgrAllStations() )
	{
		Condition con2;
		str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//���κ�
	Condition Con3;
	str.Format("SaveNo = %d", nSaveNo);
	pApp->m_DBEngine.SetCondition(sql, Con3, str);

	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::LoadDataFromDB, ��ѯʧ��");
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
			//��������
			CCallResult* pResult = new CCallResult;
			if(pResult->LoadDataFromDB(pMemset))
			{
				//���뵽ͳ�ƽ��
				StatResult(pResult);
				//���뵽����
				m_listResult.AddTail(pResult);
			}
			else
			{
				delete pResult;
				pResult = NULL;
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����Clear()
 ���ܸ�Ҫ���������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB402BF
void CAutoCallResult::Clear()
{
	POSITION pos = m_listResult.GetHeadPosition();
	while(pos != NULL)
	{
		CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
		if(pResult == NULL)
			continue;
		delete pResult;
		pResult = NULL;
	}
	m_listResult.RemoveAll();
}

/*************************************************************
 �� �� ����StatResult()
 ���ܸ�Ҫ����ָ��������뵽���ͳ����
 �� �� ֵ: void
 ��    ����param1	ָ�����
		   Param2
**************************************************************/
//##ModelId=49B87BB402C0
void CAutoCallResult::StatResult( CCallResult* pResult )
{
	//���������Ч��
	if(pResult == NULL)
		return;

	if(pResult->m_tmEnd < m_tmStart)
		m_tmStart = pResult->m_tmEnd;
	if(pResult->m_tmEnd > m_tmEnd)
		m_tmEnd = pResult->m_tmEnd;
	//��ֵ����
	if(1 == pResult->m_nZone)
	{
		//�ɹ�
		m_ZoneStat.nSuccess++;
	}
	else if(2 == pResult->m_nZone)
	{
		//ʧ��
		m_ZoneStat.nFail++;
	}

	//��ֵ
	if(1 == pResult->m_nSetting)
	{
		//�ɹ�
		m_SettingStat.nSuccess++;
	}
	else if(2 == pResult->m_nSetting)
	{
		//ʧ��
		m_SettingStat.nFail++;
	}

	//������
	if(1 == pResult->m_nDigital)
	{
		//�ɹ�
		m_DigitalStat.nSuccess++;
	}
	else if(2 == pResult->m_nDigital)
	{
		//ʧ��
		m_DigitalStat.nFail++;
	}

	//��ѹ��
	if(1 == pResult->m_nSoftBoard)
	{
		//�ɹ�
		m_SoftBoardStat.nSuccess++;
	}
	else if(2 == pResult->m_nSoftBoard)
	{
		//ʧ��
		m_SoftBoardStat.nFail++;
	}

	//ģ����
	if(1 == pResult->m_nAnalog)
	{
		//�ɹ�
		m_AnalogStat.nSuccess++;
	}
	else if(2 == pResult->m_nAnalog)
	{
		//ʧ��
		m_AnalogStat.nFail++;
	}
	//¼��
	if(1 == pResult->m_nOsc)
		m_OscStat.nSuccess++;
	else if( 2== pResult->m_nOsc)
		m_OscStat.nFail++;
	//��ʷ�¼�
	if(1 == pResult->m_nHistory)
		m_nHistoryStat.nSuccess++;
	else if( 2 == pResult->m_nHistory)
		m_nHistoryStat.nFail++;
}

/*************************************************************
 �� �� ����FillListData()
 ���ܸ�Ҫ���������ٻ�������뵽�б������ʾ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB402DE
void CAutoCallResult::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	POSITION pos = m_listResult.GetHeadPosition();
	while(pos != NULL)
	{
		CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
		if(pResult == NULL)
			continue;
		//���뵽��ʾ
		AddResultToList(pResult);
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ����AddResultToList()
 ���ܸ�Ҫ����ָ��������뵽�б�����ʾ
 �� �� ֵ: void
 ��    ����param1	ָ�����
		   Param2	Ҫ�����λ��
**************************************************************/
//##ModelId=49B87BB402DF
void CAutoCallResult::AddResultToList( CCallResult* pResult, int nIndex)
{
	//���������Ч��
	if(pResult == NULL)
		return;
	//�ж��Ƿ���Ҫ��ʾ
	if(!m_bShowNoFailed)
	{
		//����ʾ��ʧ�ܵ���Ŀ
		if(!HasFaildItem(pResult))
		{
			//û��ʧ����Ŀ
			return;
		}
	}
	if(nIndex < 0)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(pResult->m_pSec == NULL)
		return;
	
	CString str;
	//0: ����
	str.Format("%d", pResult->m_nSaveNo);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1: �����豸��
	str = pResult->m_pSec->m_sName;
	m_List.SetItemText(nIndex, 1, str);
	//2: CPU��
	CSecCPU * pCpu = GetCPU(pResult->m_pSec, pResult->m_nCPU);
	if(pCpu != NULL)
	{
		str.Format("%d(%s)", pCpu->code, pCpu->des);
	}
	else
	{
		str.Format("%d", pResult->m_nCPU);
	}
	m_List.SetItemText(nIndex, 2, str);
	//3:��ֵ���Ž��
	str = GetResultString(pResult->m_nZone);
	m_List.SetItemText(nIndex, 3, str);
	//4:��ֵ���Ž��˵��
	if(pResult->m_nZone == 2)//ʧ��
		m_List.SetItemText(nIndex, 4, pResult->m_sZoneNote);
	else
		m_List.SetItemText(nIndex, 4, "-");
	//5:��ֵ���
	str = GetResultString(pResult->m_nSetting);
	m_List.SetItemText(nIndex, 5, str);
	//6:��ֵ���˵��
	if(pResult->m_nSetting == 2)//ʧ��
		m_List.SetItemText(nIndex, 6, pResult->m_sSettingNote);
	else
		m_List.SetItemText(nIndex, 6, "-");
	//7:���������
	str = GetResultString(pResult->m_nDigital);
	m_List.SetItemText(nIndex, 7, str);
	//8:���������˵��
	if(pResult->m_nDigital == 2)//ʧ��
		m_List.SetItemText(nIndex, 8, pResult->m_sDigitalNote);
	else
		m_List.SetItemText(nIndex, 8, "-");
	//9:��ѹ����
	str = GetResultString(pResult->m_nSoftBoard);
	m_List.SetItemText(nIndex, 9, str);
	//10:��ѹ����˵��
	if(pResult->m_nSoftBoard == 2)//ʧ��
		m_List.SetItemText(nIndex, 10, pResult->m_sSoftBoardNote);
	else
		m_List.SetItemText(nIndex, 10, "-");
	//11:ģ�������
	str = GetResultString(pResult->m_nAnalog);
	m_List.SetItemText(nIndex, 11, str);
	//12:ģ�������˵��
	if(pResult->m_nAnalog == 2)
		m_List.SetItemText(nIndex, 12, pResult->m_sAnalogNote);
	else
		m_List.SetItemText(nIndex, 12, "-");
	//13:¼�����
	str = GetResultString(pResult->m_nOsc);
	m_List.SetItemText(nIndex, 13, str);
	//14:¼�����˵��
	if(pResult->m_nOsc == 2)
		m_List.SetItemText(nIndex, 14, pResult->m_sOscNote);
	else
		m_List.SetItemText(nIndex, 14, "-");
	//15:��ʷ�¼����
	str = GetResultString(pResult->m_nHistory);
	m_List.SetItemText(nIndex, 15, str);
	//16:��ʷ�¼����˵��
	if(pResult->m_nHistory == 2)
		m_List.SetItemText(nIndex, 16, pResult->m_sHistoryNote);
	else
		m_List.SetItemText(nIndex, 16, "-");
	//17:����ʱ��
	str = pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 17, str);
	//18:���˵��
	m_List.SetItemText( nIndex, 18, GetDetailSituation(pResult->m_sSituation));
	//19:�������
	m_List.SetItemText( nIndex, 19, GetDetailDispose(pResult->m_sDispose));
	//20:��վ��
	str = "";
	if(pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sName;
	}
	m_List.SetItemText(nIndex, 20, str);
	//21:һ���豸��
	str = "";
	if(pResult->m_pSec->m_pOwner != NULL)
	{
		str = pResult->m_pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(nIndex, 21, str);


	//22:��������
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sManufacturer;
	}
	m_List.SetItemText(nIndex, 22, str);

	//23:�������ܵ�λ
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sManagedepartment;
	}
	m_List.SetItemText(nIndex, 23, str);

	//24:��ά��λ
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sMaintaindepartment;
	}
	m_List.SetItemText(nIndex, 24, str);

	//��������
	m_List.SetItemData(nIndex, (DWORD)pResult);
}

/*************************************************************
 �� �� ����GetResultString()
 ���ܸ�Ҫ��ȡ�ý���ַ���
 �� �� ֵ: ����ַ���
 ��    ����param1	�����־
		   Param2
**************************************************************/
//##ModelId=49B87BB402EF
CString CAutoCallResult::GetResultString( int nResult )
{
	CString ret = "";
	//0:δ�ٻ� 1���ɹ� 2��ʧ��
	if(0 == nResult)
	{
		//δ�ٻ�
		ret.LoadString(IDS_CALLRESULT_NOTCALL);
	}
	if(1 == nResult)
	{
		//�ɹ�
		ret.LoadString(IDS_CALLRESULT_SUCCESS);
	}
	if(2 == nResult)
	{
		//ʧ��
		ret.LoadString(IDS_CALLRESULT_FAILED);
	}
	if(3 == nResult)
	{
		//װ�ò�֧��
		ret.LoadString(IDS_CALLRESULT_NOSUPPORT);
	}
	if(4 == nResult)
	{
		ret.LoadString(IDS_CALLRESULT_NODATA);
	}
	return ret;
}

/*************************************************************
 �� �� ����GetCPU()
 ���ܸ�Ҫ��ȡ��ָ�������豸��ָ��CPU�ŵ�CPU����
 �� �� ֵ: �ҵ�����CPU�����ָ��, δ�ҵ�����NULL
 ��    ����param1	ָ�������豸
		   Param2	ָ��CPU��
**************************************************************/
//##ModelId=49B87BB402FE
CSecCPU* CAutoCallResult::GetCPU( CSecObj* pObj, int nCpu )
{
	//���������Ч��
	if(pObj == NULL)
		return NULL;
	//����
	CSecCPU* pCpu = NULL;
	for(int i = 0; i < pObj->m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pTemp = (CSecCPU*)pObj->m_arrCPU.GetAt(i);
		if(pTemp->code == nCpu)
		{
			//�ҵ�, �˳�ѭ��
			pCpu = pTemp;
			break;
		}
	}
	return pCpu;
}

/*************************************************************
 �� �� ����FillSummaryListData()
 ���ܸ�Ҫ���������б������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4031C
void CAutoCallResult::FillSummaryListData()
{
	if(m_ListSummary.GetSafeHwnd() == NULL)
		return;
	//���������
	m_ListSummary.DeleteAllItems();
	if(m_pStation == NULL)
		return;

	//"��Ŀ", "��ֵ����","��ֵ","������","��ѹ��","ģ����","����"
	CString str;
	CString stemp = "";
	CString sNotstart = stemp.LoadString(IDS_AUTOCALL_NOTSTART);
	//��һ��: ����
	//0: ��Ŀ
	str.LoadString(IDS_AUTOCALLRESULT_TOTAL);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 0, str, 0, 0, 0, 0) == -1)
		return;
	//1:��ֵ����
	str.Format("%d", m_ZoneStat.nFail+ m_ZoneStat.nSuccess);
	m_ListSummary.SetItemText(0, 1, str);
	//2:��ֵ
	str.Format("%d", m_SettingStat.nFail + m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(0, 2, str);
	//3:������
	str.Format("%d", m_DigitalStat.nFail + m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(0, 3, str);
	//4:��ѹ��
	str.Format("%d", m_SoftBoardStat.nFail + m_SoftBoardStat.nSuccess);
	m_ListSummary.SetItemText(0, 4, str);
	//5:ģ����
	str.Format("%d", m_AnalogStat.nFail + m_AnalogStat.nSuccess);
	m_ListSummary.SetItemText(0, 5, str);
	//6:¼��
	str.Format("%d", m_OscStat.nFail + m_OscStat.nSuccess);
	m_ListSummary.SetItemText(0, 6, str);
	//7:��ʷ�¼�
	str.Format("%d", m_nHistoryStat.nFail + m_nHistoryStat.nSuccess);
	m_ListSummary.SetItemText(0, 7, str);
	//8:����
	stemp.LoadString(IDS_AUTOCALL_STARTTIME);
	str.Format("%s:%s", stemp ,m_tmStart.Format("%Y-%m-%d %H:%M:%S"));
	m_ListSummary.SetItemText(0, 8, str);

	//�ڶ���: �ɹ���
	//0: ��Ŀ
	str.LoadString(IDS_AUTOCALLRESULT_SUCCESS_NUM);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 1, str, 0, 0, 0, 1) == -1)
		return;
	//1:��ֵ����
	str.Format("%d", m_ZoneStat.nSuccess);
	m_ListSummary.SetItemText(1, 1, str);
	//2:��ֵ
	str.Format("%d", m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(1, 2, str);
	//3:������
	str.Format("%d", m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(1, 3, str);
	//4:��ѹ��
	str.Format("%d", m_SoftBoardStat.nSuccess);
	m_ListSummary.SetItemText(1, 4, str);
	//5:ģ����
	str.Format("%d", m_AnalogStat.nSuccess);
	m_ListSummary.SetItemText(1, 5, str);
	//6:¼��
	str.Format("%d", m_OscStat.nSuccess);
	m_ListSummary.SetItemText(1, 6, str);
	//7:��ʷ�¼�
	str.Format("%d", m_nHistoryStat.nSuccess);
	m_ListSummary.SetItemText(1, 7, str);
	//6:����
	CTime tm(1971, 1, 1, 1, 1, 1);
	stemp.LoadString(IDS_AUTOCALL_ENDTIME);
	str.Format("%s:%s",  stemp, m_tmEnd==tm?sNotstart:m_tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	m_ListSummary.SetItemText(1, 8, str);

	//������: ʧ����
	//0: ��Ŀ
	str.LoadString(IDS_AUTOCALLRESULT_FAILED_NUM);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 2, str, 0, 0, 0, 2) == -1)
		return;
	//1:��ֵ����
	str.Format("%d", m_ZoneStat.nFail);
	m_ListSummary.SetItemText(2, 1, str);
	//2:��ֵ
	str.Format("%d", m_SettingStat.nFail);
	m_ListSummary.SetItemText(2, 2, str);
	//3:������
	str.Format("%d", m_DigitalStat.nFail);
	m_ListSummary.SetItemText(2, 3, str);
	//4:��ѹ��
	str.Format("%d", m_SoftBoardStat.nFail);
	m_ListSummary.SetItemText(2, 4, str);
	//5:ģ����
	str.Format("%d", m_AnalogStat.nFail);
	m_ListSummary.SetItemText(2, 5, str);
	//6:¼��
	str.Format("%d", m_OscStat.nFail);
	m_ListSummary.SetItemText(2, 6, str);
	//7:��ʷ�¼�
	str.Format("%d", m_nHistoryStat.nFail);
	m_ListSummary.SetItemText(2, 7, str);
	//6:����
	CTimeSpan tmSpan;
	tmSpan = m_tmEnd - m_tmStart;
	stemp.LoadString(IDS_AUTOCALLRESULT_TOTALTIME);
	CString sformat = GetDurationFormatStr();
	str.Format("%s:%s", stemp, tmSpan<0?stemp.LoadString(IDS_AUTOCALL_NOTSTART):tmSpan.Format(sformat));
	m_ListSummary.SetItemText(2, 8, str);
}

//##ModelId=49B87BB40399
BOOL CAutoCallResult::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::DestroyWindow();
}

//##ModelId=49B87BB5004E
void CAutoCallResult::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ����Ӧ�б���Ի���ʾ��Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5005D
void CAutoCallResult::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//"����", "�����豸", "CPU��", "��ֵ���Ž��", "��ֵ���Ž��˵��", "��ֵ���", "��ֵ���˵��", "���������", "���������˵��", "��ѹ����", "��ѹ����˵��", "ģ�������", "ģ�������˵��", "���ʱ��", "��վ��", "һ���豸", "�������ܵ�λ", "��ά��λ"
				//�����
				if(nSubItem == 3 || nSubItem == 5 || nSubItem == 7 || nSubItem == 9 || nSubItem == 11)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					CString stemp;

					if(stemp.LoadString(IDS_CALLRESULT_SUCCESS) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchSuccess;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_FAILED) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchFail;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_NOTCALL) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchNULL;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_NOSUPPORT) && str.CompareNoCase(stemp) == 0 )
					{
						lplvcd->clrText = g_BatchNotSupport;
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
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��.1, 0, -1
 ��    ����param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   param3 �Ƚϲ���
**************************************************************/
//##ModelId=49B87BB40214
int CALLBACK CAutoCallResult::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CCallResult* pResult1 = (CCallResult*)lParam1;
	CCallResult* pResult2 = (CCallResult*)lParam2;
	if(pResult2 == NULL || pResult1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"����", "�����豸", "CPU��", "��ֵ���Ž��", "��ֵ���Ž��˵��", 
	//"��ֵ���", "��ֵ���˵��", "���������", "���������˵��", 
	//"��ѹ����", "��ѹ����˵��", "ģ�������", "ģ�������˵��", 
	//"���ʱ��", "��վ��", "һ���豸", "�������ܵ�λ", "��ά��λ"
	switch(nCol)
	{
	case 0: //����
		iResult = 0;
		break;
	case 1: //�����豸
		str1 = pResult1->m_pSec->m_sName;
		str2 = pResult2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //CPU
		iResult = pResult1->m_nCPU - pResult2->m_nCPU;
		break;
	case 3: //��ֵ���Ž��
		iResult = pResult1->m_nZone - pResult2->m_nZone;
		break;
	case 4: //��ֵ���Ž��˵��
		str1 = pResult1->m_sZoneNote;
		str2 = pResult2->m_sZoneNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5:	//��ֵ���
		iResult = pResult1->m_nSetting - pResult2->m_nSetting;
		break;
	case 6: //��ֵ���˵��
		str1 = pResult1->m_sSettingNote;
		str2 = pResult2->m_sSettingNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //���������
		iResult = pResult1->m_nDigital - pResult2->m_nDigital;
		break;
	case 8: //���������˵��
		str1 = pResult1->m_sDigitalNote;
		str2 = pResult2->m_sDigitalNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9: //��ѹ����
		iResult = pResult1->m_nSoftBoard - pResult2->m_nSoftBoard;
		break;
	case 10: //��ѹ����˵��
		str1 = pResult1->m_sSoftBoardNote;
		str2 = pResult2->m_sSoftBoardNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 11: //ģ�������
		iResult = pResult1->m_nAnalog - pResult2->m_nAnalog;
		break;
	case 12: //ģ�������˵��
		str1 = pResult1->m_sAnalogNote;
		str2 = pResult2->m_sAnalogNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 13: //¼�����
		iResult = pResult1->m_nOsc - pResult2->m_nOsc;
		break;
	case 14: //¼�����˵��
		str1 = pResult1->m_sOscNote;
		str2 = pResult2->m_sOscNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 15: //��ʷ�¼����
		iResult = pResult1->m_nHistory - pResult2->m_nHistory;
		break;
	case 16: //��ʷ�¼����˵��
		str1 = pResult1->m_sHistoryNote;
		str2 = pResult2->m_sHistoryNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 17: //����ʱ��
		str1 = pResult1->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		str2 = pResult2->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 18: //��վ��
		if(pResult1->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pStation != NULL)
		{
			str2 = pResult2->m_pStation->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 19: //һ���豸
		if(pResult1->m_pSec->m_pOwner != NULL)
		{
			str1 = pResult1->m_pSec->m_pOwner->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pSec->m_pOwner != NULL)
		{
			str2 = pResult2->m_pSec->m_pOwner->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 20: //��������
		if (pResult1->m_pStation != NULL && pResult2->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sManufacturer;
			str2 = pResult2->m_pStation->m_sManufacturer;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	case 21: //���ܵ�λ
		if (pResult1->m_pStation != NULL && pResult2->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sManagedepartment;
			str2 = pResult2->m_pStation->m_sManagedepartment;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	case 22: //��ά��λ
		{
			str1 = pResult1->m_pStation->m_sMaintaindepartment;
			str2 = pResult2->m_pStation->m_sMaintaindepartment;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iResultViewAsc;
	return iResult;
}

//##ModelId=49B87BB5006D
void CAutoCallResult::OnColumnclickListAutocallResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iResultViewAsc = - g_iResultViewAsc;
	}
	else
	{
		g_iResultViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����GetOCTime()
 ���ܸ�Ҫ�������ݿ��ѯ�Զ����ٵ�ʱ��
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��Ӧtb_sys_config�е�KEY
		   Param2	ʱ��
**************************************************************/
//##ModelId=49B87BB4031D
BOOL CAutoCallResult::GetOCTime( CString sKey, CTime& tmTime )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����:KeyName,��ѯValue
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);
	
	Condition condition1;
	CString str;
	str.Format("KEYNAME='%s'", sKey);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CAutoCallResult::GetOCTime, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			str = pMemset->GetValue(UINT(0));
			tmTime = StringToTime(str);
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::GetOCTime, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		return FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return TRUE;
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ������������������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4032D
void CAutoCallResult::ReFillAll(int nSaveNo)
{
//	g_wndWait.Show(SPLASH_NOTIMER);
	//��ѯ��������������
	if(m_pStation != NULL && nSaveNo == -1)
	{
		QuerySaveNo(m_pStation->m_sID);
		QueryNewSaveNo(m_pStation->m_sID);
	}
	else
	{
		QuerySaveNo("");
		QueryNewSaveNo("");
	}
	if(nSaveNo < 1)
		m_nSelSaveNo = m_nNewSaveNo;
	else
		m_nSelSaveNo = nSaveNo;
	LoadDataFromDB(m_nSelSaveNo);
	FillListData();
	FillSummaryListData();
//	g_wndWait.Hide();
}

/*************************************************************
 �� �� ����OnAutoCallResultChange()
 ���ܸ�Ҫ����Ӧѡ��ı���Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB500AB
void CAutoCallResult::OnAutoCallResultChange( WPARAM wParam, LPARAM lParam )
{
	//ȡ��ѡ����豸
	CDeviceObj* pDevice = (CDeviceObj*)lParam;
	if(pDevice == NULL)
	{
		//ȫ����ʾ
		ReFillAll(-1);
		m_bSelectRoot = TRUE;
		return;
	}
	m_bSelectRoot = FALSE;
	m_nSelSaveNo = 0;
	//�ж��ǲ��ǳ�վ
	if(pDevice->m_nType != TYPE_STATION)
	{

		m_pStation = NULL;
		return;
	}
	else
	{
		CStationObj* pStation = (CStationObj*)pDevice;
		if(pStation != m_pStation)
		{
			m_pStation = pStation;
		}
		else
			return;
	}

	//ˢ������
	ReFillAll(-1);
	//m_pStation = NULL;
}

/*************************************************************
 �� �� ����ClearStat()
 ���ܸ�Ҫ�����ͳ�ƽ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4033C
void CAutoCallResult::ClearStat()
{
	m_ZoneStat.nFail = 0;
	m_ZoneStat.nSuccess = 0;
	
	m_SettingStat.nSuccess =0;
	m_SettingStat.nFail = 0;
	
	m_DigitalStat.nFail = 0;
	m_DigitalStat.nSuccess = 0;
	
	m_SoftBoardStat.nSuccess = 0;
	m_SoftBoardStat.nFail = 0;
	
	m_AnalogStat.nFail = 0;
	m_AnalogStat.nSuccess = 0;

	m_OscStat.nFail = 0;
	m_OscStat.nSuccess = 0;

	m_nHistoryStat.nFail = 0;
	m_nHistoryStat.nSuccess = 0;

	CTime tm(1971, 1, 1, 1, 1, 1);
	m_tmEnd = tm;
	CTime tm1(2029, 1, 1, 1, 1, 1);
	m_tmStart = tm1;
}

//##ModelId=49B87BB5007E
void CAutoCallResult::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_RESULT))
				return;
			pMenu = Menu.GetSubMenu(0);

			if(GetPrevSaveNo(m_nSelSaveNo) > 0)
				pMenu->EnableMenuItem(IDC_RESULT_PREV, MF_ENABLED);
			if(GetNextSaveNo(m_nSelSaveNo) > 0)
				pMenu->EnableMenuItem(IDC_RESULT_NEXT, MF_ENABLED);
			if( !m_bSelectRoot )
				pMenu->EnableMenuItem(IDC_RESULT_STATIONEXPORT, MF_DISABLED);

			if(!m_bShowNoFailed)
			{
				//������ʧ����Ŀ
				pMenu->CheckMenuItem(IDC_RESULT_SHOWFAILD, MF_CHECKED);
			}
			else
			{
				//��ʾ
				pMenu->CheckMenuItem(IDC_RESULT_SHOWFAILD, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87BB5008D
void CAutoCallResult::OnResultExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName = "";

	if(m_pStation != NULL)
	{
		strName.Format("[%s]%s-%s.xls", m_pStation->m_sName, StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));
	}
	else
	{
		strName.Format("%s-%s.xls", StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));
	}

	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//�����ɹ�
		if(AfxMessageBox(IDS_COMMON_EXPORT_SUCCESS, MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//����ʧ��
		AfxMessageBox(IDS_COMMON_EXPORT_FAIL);
	}
}

/*************************************************************
 �� �� ����AddResultToExcel()
 ���ܸ�Ҫ����ָ���Ľ����д�뵽ָ����Excel�ļ���
 �� �� ֵ: void
 ��    ����param1	���ݿ����
		   Param2	�����
**************************************************************/
//##ModelId=49B87BB4033D
void CAutoCallResult::AddResultToExcel( CDatabase* pDatabase, CCallResult* pResult )
{
	if(pResult == NULL)
		return;

	//�ж��Ƿ���Ҫ��ʾ
	if(!m_bShowNoFailed)
	{
		//����ʾ��ʧ�ܵ���Ŀ
		if(!HasFaildItem(pResult))
		{
			//û��ʧ����Ŀ
			return;
		}
	}

	if(pResult->m_pSec == NULL)
		return;

	CString strSaveNO;
	strSaveNO.Format("%d", pResult->m_nSaveNo);

	CString strSQL;
	
	CString strCPU = "";
	CSecCPU * pCpu = GetCPU(pResult->m_pSec, pResult->m_nCPU);
	if(pCpu != NULL)
	{
		strCPU.Format("%d(%s)", pCpu->code, pCpu->des);
	}
	else
	{
		strCPU.Format("%d", pResult->m_nCPU);
	}
	//8:��վ��
	CString strStation = "";
	if(pResult->m_pStation != NULL)
	{
		strStation = pResult->m_pStation->m_sName;
	}
	//9:һ���豸��
	CString strDevice = "";
	if(pResult->m_pSec->m_pOwner != NULL)
	{
		strDevice = pResult->m_pSec->m_pOwner->m_sName;
	}

	CString strManufacturer = "";
	if (pResult->m_pStation != NULL)
	{
		strManufacturer = pResult->m_pStation->m_sManufacturer;
	}

	//17:�������ܵ�λ
	CString sManagedepartment = "";
	if (pResult->m_pStation != NULL)
	{
		sManagedepartment = pResult->m_pStation->m_sManagedepartment;
	}
	
	//18:��ά��λ
	CString sMaintaindepartment = "";
	if (pResult->m_pStation != NULL)
	{
		sMaintaindepartment = pResult->m_pStation->m_sMaintaindepartment;
	}

	strSQL.Format("INSERT INTO Log VALUES ('%s', '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		strSaveNO, pResult->m_pSec->m_sName,  strCPU, 
		GetResultString(pResult->m_nZone), pResult->m_nZone==2?pResult->m_sZoneNote:"-", 
		GetResultString(pResult->m_nSetting), pResult->m_nSetting==2?pResult->m_sSettingNote:"-", 
		GetResultString(pResult->m_nDigital), pResult->m_nDigital==2?pResult->m_sDigitalNote:"-", 
		GetResultString(pResult->m_nSoftBoard), pResult->m_nSoftBoard==2?pResult->m_sSoftBoardNote:"-", 
		GetResultString(pResult->m_nAnalog), pResult->m_nAnalog==2?pResult->m_sAnalogNote:"-",
		GetResultString(pResult->m_nOsc), pResult->m_nOsc==2?pResult->m_sOscNote:"-",
		GetResultString(pResult->m_nHistory), pResult->m_nHistory==2?pResult->m_sHistoryNote:"-",
		pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S"),GetDetailSituation(pResult->m_sSituation),
		GetDetailDispose(pResult->m_sDispose),
		strStation, strDevice, strManufacturer, sManagedepartment, sMaintaindepartment);

	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 �� �� ����WriteResultToExcel()
 ���ܸ�Ҫ�������������Excel
 �� �� ֵ: 
 ��    ����param1	�ļ�·��
		   Param2
**************************************************************/
//##ModelId=49B87BB4034C
BOOL CAutoCallResult::WriteResultToExcel( CString strFullPath )
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
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_AUTOCALLRESULT_NUMBER), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_MODEL_CPU), StringFromID(IDS_AUTOCALLRESULT_ZONE), 
					StringFromID(IDS_AUTOCALLRESULT_ZONE_NOTE), StringFromID(IDS_AUTOCALLRESULT_SETTING), StringFromID(IDS_AUTOCALLRESULT_SETTING_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_DIGITAL), StringFromID(IDS_AUTOCALLRESULT_DIGITAL_NOTE), StringFromID(IDS_AUTOCALLRESULT_SOFT), StringFromID(IDS_AUTOCALLRESULT_SOFT_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_ANALOG), StringFromID(IDS_AUTOCALLRESULT_ANALOG_NOTE), StringFromID(IDS_AUTOCALLRESULT_OSC), StringFromID(IDS_AUTOCALLRESULT_OSC_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_HISEVENT), StringFromID(IDS_AUTOCALLRESULT_HISEVENT_NOTE), StringFromID(IDS_COMMON_FINISHTIME), StringFromID(IDS_AUTOCALLRESULT_OVERVIEW), 
					StringFromID(IDS_AUTOCALLRESULT_RESOLUTION), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_MANUFACTURER),
					StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE));

				database.ExecuteSQL(strSQL);
			}

			POSITION pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(pResult == NULL)
					continue;
				AddResultToExcel(&database, pResult);
			}
		}
		else
			return FALSE;

		database.Close();
	}
	catch(CDBException * e)
	{
		WriteLog("����Excel�����: " + e->m_strError, XJ_LOG_LV1);
		
		return FALSE;
	}
	return TRUE;
}

BOOL CAutoCallResult::WriteResultToExcel(CString strFullPath, CString sStationID)
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
			
			POSITION pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(pResult == NULL)
					continue;
				if( pResult->m_sStationId == sStationID )
				{
					if(!IsFind)
					{
						//�½�
						strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
							StringFromID(IDS_AUTOCALLRESULT_NUMBER), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_MODEL_CPU), StringFromID(IDS_AUTOCALLRESULT_ZONE), 
							StringFromID(IDS_AUTOCALLRESULT_ZONE_NOTE), StringFromID(IDS_AUTOCALLRESULT_SETTING), StringFromID(IDS_AUTOCALLRESULT_SETTING_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_DIGITAL), StringFromID(IDS_AUTOCALLRESULT_DIGITAL_NOTE), StringFromID(IDS_AUTOCALLRESULT_SOFT), StringFromID(IDS_AUTOCALLRESULT_SOFT_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_ANALOG), StringFromID(IDS_AUTOCALLRESULT_ANALOG_NOTE), StringFromID(IDS_AUTOCALLRESULT_OSC), StringFromID(IDS_AUTOCALLRESULT_OSC_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_HISEVENT), StringFromID(IDS_AUTOCALLRESULT_HISEVENT_NOTE), StringFromID(IDS_COMMON_FINISHTIME), StringFromID(IDS_AUTOCALLRESULT_OVERVIEW), 
							StringFromID(IDS_AUTOCALLRESULT_RESOLUTION), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_MANUFACTURER),
							StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE));
						database.ExecuteSQL(strSQL);
						IsFind = TRUE;
					}
					AddResultToExcel(&database, pResult);
				}
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
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87BB4037C
CString CAutoCallResult::TranslateSingleKey( CString sKey )
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
		//ȡ��ͷ��
		CString strHead = sKey.Left(nFind);
		//ȡ���¼�
		CCallResult* pResult = NULL;
		if(strHead == "$AUTORESULT_LIST")
		{
			int nCount = 0;
			POSITION pos = NULL;
			pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(nCount == nIndex)
					break;
				nCount++;
			}
		}
		if(pResult == NULL)
			return "";
		if(pResult->m_pSec == NULL)
			return "";
		
		CString str;
		//ȡ��β���ַ�
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_NAME$")
		{
			//�����豸��
			return pResult->m_pSec->m_sName;
		}
		if(strTail == "PT_MODEL$")
		{
			//�����豸�ͺ�
			return pResult->m_pSec->m_sModel;
		}
		if(strTail == "PT_CPU$")
		{
			//CPU��
			str = "";
			str.Format("%d", pResult->m_nCPU);
			return str;
		}
		if(strTail == "ZONE_RESULT$")
		{
			//��ֵ���Ž��
			return GetResultString(pResult->m_nZone);
		}
		if(strTail == "SETTING_RESULT$")
		{
			//��ֵ���
			return GetResultString(pResult->m_nSetting);
		}
		if(strTail == "DIGITAL_RESULT$")
		{
			//���������
			return GetResultString(pResult->m_nDigital);
		}
		if(strTail == "SOFTBOARD_RESULT$")
		{
			//��ѹ����
			return GetResultString(pResult->m_nSoftBoard);
		}
		if(strTail == "ANALOG_RESULT$")
		{
			//ģ�������
			return GetResultString(pResult->m_nAnalog);
		}
		if(strTail == "COMPLETE_TIME$")
		{
			//����ʱ��
			return pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "STATION_NAME$")
		{
			//��վ��
			str = "";
			if(pResult->m_pStation != NULL)
			{
				str = pResult->m_pStation->m_sName;
			}
			return str;
		}
		if(strTail == "DEVICE_NAME$")
		{
			//9:һ���豸��
			str = "";
			if(pResult->m_pSec->m_pOwner != NULL)
			{
				str = pResult->m_pSec->m_pOwner->m_sName;
			}
			return str;
		}
	}
	else
	{
		//�ж��Ƿ���.GetCount()����
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//��.GetCount()����
			if(sKey == "$AUTORESULT_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			if(sKey == "$STATION_NAME$")
			{
				//��վ��
				if(m_pStation == NULL)
					return "";
				return m_pStation->m_sName;
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
//##ModelId=49B87BB4035B
BOOL CAutoCallResult::InitCOM()
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
//##ModelId=49B87BB4035C
void CAutoCallResult::ReleaseCOM()
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

//##ModelId=49B87BB5008F
void CAutoCallResult::OnResultPrint() 
{
	// TODO: Add your command handler code here
	
	//��ӡ
	InitCOM();
	//��ģ���ļ�
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, AUTORESULT_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
		{
			bOpened = TRUE;
		}
		else
		{
			CString sLog;
			sLog.Format("�򿪴�ӡģ���ļ�ʧ��.\r\n%s", AUTORESULT_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInAutoCallResult);
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
 �� �� ����QuerySaveNo()
 ���ܸ�Ҫ����ѯָ��վ�����������
 �� �� ֵ: void
 ��    ����param1	վID
		   Param2
**************************************************************/
//##ModelId=49B87BB4035D
void CAutoCallResult::QuerySaveNo( CString sStationID )
{
	ClearSaveNo();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SaveNo", EX_STTP_DATA_TYPE_INT);

	
	//����
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (sStationID != "")
	{
		Condition con2;
		str.Format("station_id = '%s'", sStationID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//��ʱ������
// 	Condition condition3;
// 	str.Format("order by TIME DESC");
// 	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//��where
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::QuerySaveNo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//���浽������
			SaveNumber* sn = new SaveNumber;
			CString str = pMemset->GetValue(UINT(0));
			sn->nSaveNo = atoi(str);
// 			str = pMemset->GetValue(1);
// 			sn->tmTime = StringToTime(str);
			m_arrSaveNo.Add(sn);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::QuerySaveNo,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//����
	SortSaveNo();
}

/*************************************************************
 �� �� ����QueryNewSaveNo()
 ���ܸ�Ҫ����ѯָ��վ������µ��������κ�
 �� �� ֵ: 
 ��    ����param1	վID
		   Param2
**************************************************************/
//##ModelId=49B87BB4035F
void CAutoCallResult::QueryNewSaveNo( CString sStationID )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SaveNo", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	
	//����
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (sStationID != "")
	{
		Condition con2;
		str.Format("station_id = '%s'", sStationID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//��ʱ������
	Condition condition3;
	str.Format("order by TIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//��where
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::QueryNewSaveNo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//���浽������
			CString str = pMemset->GetValue(UINT(0));
			m_nNewSaveNo = atoi(str);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::QueryNewSaveNo,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 �� �� ����GetPrevSaveNo()
 ���ܸ�Ҫ��ȡ����һ���ε����κ�
 �� �� ֵ: ��һ�������κ�
 ��    ����param1 ��ǰ���κ�
		   Param2
**************************************************************/
//##ModelId=49B87BB4036C
int CAutoCallResult::GetPrevSaveNo( int nCurSaveNo )
{
	if(nCurSaveNo == m_nOldSaveNo)
	{
		//�����ϵ�
		return -1;
	}

	int nSaveNo = nCurSaveNo - 1;
	if(nSaveNo < m_nMinSaveNo)
	{
		nSaveNo = m_nMaxSaveNo;
	}

	return nSaveNo;
}

/*************************************************************
 �� �� ����GetNextSaveNo()
 ���ܸ�Ҫ��ȡ����һ���ε����κ�
 �� �� ֵ: ��һ�������κ�
 ��    ����param1	��ǰ���κ�
		   Param2
**************************************************************/
//##ModelId=49B87BB4036E
int CAutoCallResult::GetNextSaveNo( int nCurSaveNo )
{
	if(nCurSaveNo == m_nNewSaveNo)
		return -1; //�����µ�

	int nSaveNo = nCurSaveNo + 1;
	if(nSaveNo > m_nMaxSaveNo)
	{
		nSaveNo = m_nMinSaveNo;
	}
	
	return nSaveNo;
}

//##ModelId=49B87BB50091
void CAutoCallResult::OnResultPrev() 
{
	// TODO: Add your command handler code here
	ReFillAll(GetPrevSaveNo(m_nSelSaveNo));
}

//##ModelId=49B87BB5009C
void CAutoCallResult::OnResultNext() 
{
	// TODO: Add your command handler code here
	ReFillAll(GetNextSaveNo(m_nSelSaveNo));
}

/*************************************************************
 �� �� ����ClearSaveNo()
 ���ܸ�Ҫ��������μ�¼
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4037A
void CAutoCallResult::ClearSaveNo()
{
	for(int i = 0; i < m_arrSaveNo.GetSize(); i++)
	{
		SaveNumber * sn = (SaveNumber*)m_arrSaveNo.GetAt(i);
		if(sn != NULL)
			delete sn;
	}
	m_arrSaveNo.RemoveAll();
}

/*************************************************************
 �� �� ����SortSaveNo()
 ���ܸ�Ҫ�������ΰ�ʱ��Ӿɵ��½�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4037B
void CAutoCallResult::SortSaveNo()
{
	int nMax = 0;
	int nMin = 99;
	//�������,��С���κ�
	for(int i = 0; i < m_arrSaveNo.GetSize(); i++)
	{
		SaveNumber* sn = (SaveNumber*)m_arrSaveNo.GetAt(i);
		if(sn == NULL)
			continue;
		if(sn->nSaveNo > nMax)
			nMax = sn->nSaveNo;
		if(sn->nSaveNo < nMin)
			nMin = sn->nSaveNo;
	}
	m_nMaxSaveNo = nMax;
	m_nMinSaveNo = nMin;

	if(m_nNewSaveNo == nMax)
	{
		//���µ����κŵ����������κ�, ����С�����κ�Ϊ���ϵ�
		m_nOldSaveNo = nMin;
	}
	else if(m_nNewSaveNo < nMax)
	{
		//���µ����κ�С���������κ�, �����ϵ����κ�Ϊ���µ����κ�+1
		m_nOldSaveNo = m_nNewSaveNo+1;
	}
}

/*************************************************************
 �� �� ��: HasFaildItem()
 ���ܸ�Ҫ: �ж�һ���Զ����ٵĽ�����Ƿ���ʧ����Ŀ
 �� �� ֵ: ��ʧ����Ŀ����TRUE, ��ʧ����Ŀ����FALSE
 ��    ��: param1 ָ���Զ����ٽ�������
		   Param2 
**************************************************************/
BOOL CAutoCallResult::HasFaildItem( CCallResult* pResult )
{
	if(pResult == NULL)
		return TRUE;
	//0:δ�ٻ� 1���ɹ� 2��ʧ�� 3: װ�ò�֧��
	if(2 == pResult->m_nZone) //��ֵ����
		return TRUE;
	if(2 == pResult->m_nSetting) //��ֵ
		return TRUE;
	if(2 == pResult->m_nDigital) //������
		return TRUE;
	if(2 == pResult->m_nAnalog) //ģ����
		return TRUE;
	if(2 == pResult->m_nSoftBoard) //��ѹ��
		return TRUE;
	if(2 == pResult->m_nOsc)//¼��
		return TRUE;
	if(2 == pResult->m_nHistory)//��ʷ�¼�
		return TRUE;
	return FALSE;
}

void CAutoCallResult::OnResultShowfaild() 
{
	// TODO: Add your command handler code here
	m_bShowNoFailed = !m_bShowNoFailed;
	//��������б�
	FillListData();
}

void CAutoCallResult::OnWinClose() 
{
	// TODO: Add your command handler code here
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CAutoCallResult::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_FUNC_AUTOCALLRESULT));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CAutoCallResult::OnResultStationexport() 
{
	// TODO: Add your command handler code here
	m_pExportProcess = NULL;

	CTime t = CTime::GetCurrentTime();
	CString strName = "";
	
	strName.Format("%s-%s", StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));

	
	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;

	//�����ļ���
	CreateDirectory(strPath,NULL);

	g_sRootDirPath = strPath;

	CDlgExportProcess dlg;
	m_pExportProcess = &dlg;
	AfxBeginThread(ExportByStation,this,THREAD_PRIORITY_BELOW_NORMAL);
	dlg.DoModal();
}
