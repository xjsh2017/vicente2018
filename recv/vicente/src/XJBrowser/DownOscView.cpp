// DownOscView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownOscView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownOscView

IMPLEMENT_DYNCREATE(CDownOscView, CViewBase)

CDownOscView::CDownOscView()
	: CViewBase(CDownOscView::IDD)
{
	//{{AFX_DATA_INIT(CDownOscView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	m_nTimer = -1;
	m_nDownTimer = -1;
	m_pCurrentOsc = NULL;
}

CDownOscView::~CDownOscView()
{
}

void CDownOscView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownOscView)
	DDX_Control(pDX, IDC_LIST_DOWN_OSC, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownOscView, CViewBase)
	//{{AFX_MSG_MAP(CDownOscView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_DOWNOSC_DOWN, OnDownoscDown)
	ON_COMMAND(IDC_DOWNOSC_CANCEL, OnDownoscCancel)
	ON_COMMAND(IDC_DOWNOSC_OPEN, OnDownoscOpen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DOWN_OSC, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNOSC_FRAME_OPEN, OnDownOscOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownOscView diagnostics

#ifdef _DEBUG
void CDownOscView::AssertValid() const
{
	CViewBase::AssertValid();
}

void CDownOscView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownOscView message handlers
/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA301A5
BOOL CDownOscView::LoadListConfig()
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
			WriteLog("DownloadViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("DownOscViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//DownOscViewConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�DownOscViewConfig");
				WriteLog("DownOscViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�DownOscViewConfig�µ�����");
					WriteLog("DownOscViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
// 	}	
// 	//�رմ򿪵�COM
// 	CoUninitialize();
	return bResult;
}

//##ModelId=49B87BA301B5
BOOL CDownOscView::SaveListConfig()
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
			WriteLog("DownOscViewConfig::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("DownOscViewConfig::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//DownOscViewConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�DownOscViewConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("DownOscViewConfig");
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
						WriteLog("δ�ҵ�DownOscViewConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���DownOscViewConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87BA301C5
BOOL CDownOscView::NeedSave()
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
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ��: param1 
		   Param2 
**************************************************************/
int CDownOscView::InitListStyle()
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
		//char* arColumn[5]={"���", "����","��С", "����ʱ��", "������"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 5 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80;
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 250; // ����
				colName = StringFromID(IDS_COMMON_FILENAME);
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
				lvCol.cx = 80;
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

void CDownOscView::OnInitialUpdate() 
{
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
	
	//��ʼ���б��
	InitListStyle();

	//������ʱ��
	int nTimeOut = 5;
	m_nDownTimer = SetTimer(2, nTimeOut*1000, 0);
}

void CDownOscView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_DOWN_OSC, left_client, top_client, right_client, bottom_client);
}

void CDownOscView::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ��: AddEventToList()
 ���ܸ�Ҫ: ��һ���¼�����Ϣ���뵽�б�
 �� �� ֵ: ����ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ������¼�
		   Param2 �����λ��
**************************************************************/
BOOL CDownOscView::AddEventToList( CXJEventOSC * pEvent, int i )
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
	str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(i, 2, str);
	
	//3:����ʱ��
	str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, str);
	
	//4:������
	m_List.SetItemText(i, 4, pEvent->GetDownloadString());
	
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 �� �� ��: AddEvent()
 ���ܸ�Ҫ: ���ָ���¼�����ͼ
 �� �� ֵ: 
 ��    ��: param1 ָ���¼�
		   Param2 
**************************************************************/
void CDownOscView::AddEvent( CXJEventOSC* pEvent )
{
	if(pEvent == NULL)
		return;
	//��ӵ�����
	m_EventManager.AddEvent(pEvent);
	//��ӵ��б��,���һ��
	int nCount = m_List.GetItemCount();
	AddEventToList(pEvent, nCount);
}

/*************************************************************
 �� �� ��: OnDownOscOpen()
 ���ܸ�Ҫ: ��Ӧҳ�����Ϣ������¼�����ҳ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 ¼���¼�
**************************************************************/
void CDownOscView::OnDownOscOpen( WPARAM wParam, LPARAM lParam )
{
	CXJEventOSC* pEvent = (CXJEventOSC*)lParam;
	if(pEvent == NULL)
		return;
	//�����¼�
	CXJEventOSC* pOsc = (CXJEventOSC*)pEvent->Clone();
	CXJEventOSC* pExist = NULL;
	if(NeedAdd(pOsc, pExist))
	{
		//��ӵ���ҳ��
		AddEvent(pOsc);
	}
	else
	{
		delete pOsc;
		pOsc = NULL;
		//���������Ѵ��ڵ��¼�
		if(pExist != NULL)
		{
			pExist->m_nDownloaded = 0;
			ChangeValue(pExist);
		}
	}
}

/*************************************************************
 �� �� ��: DownloadOSCFile()
 ���ܸ�Ҫ: ����ָ��¼���б��¼���ļ�
 �� �� ֵ: 
 ��    ��: param1 ָ��¼���б�
		   Param2 
**************************************************************/
void CDownOscView::DownloadOSCFile( CXJEventOSC* pEvent )
{
	if(pEvent == NULL)
		return;

	//��Ӧ��ť���²���, ����¼���ļ���ȡ���ٻ�
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	int nOffset = 0;
	
	CString strFilePath = pEvent->m_sFilePath;

	//����/
	strFilePath.Replace("\\", "/");
	if(strFilePath.Right(1) != "/")
		strFilePath += "/";

	CString strFileName;
	if (g_role != MODE_SUB)
	{
		strFileName = strFilePath + pEvent->m_sFileName;
	}
	else
	{
		strFileName = pEvent->m_sFileName;
	}
	 
	//CString strFileName = pEvent->m_sFileName;
	//��֯20042����,����վ����
	if(!pApp->m_SttpEngine.BuildDataFor20042DownloadWaveFile(sttpData, pEvent->m_sSecID, strFileName,  1, nOffset, pEvent->m_nFileNum))
	{
		//��֯����ʧ��
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		pEvent->m_nDownloaded = -1;
		ChangeValue(pEvent);
		m_pCurrentOsc = NULL;
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
			str.Format("%s�����ٻ�¼���ļ�����ʧ��,ԭ��Ϊ�����ж�", pEvent->m_sSecID);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pEvent->m_nDownloaded = -1;
			ChangeValue(pEvent);
			m_pCurrentOsc = NULL;
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("%s�����ٻ�¼���ļ�����ʧ��,ԭ��Ϊ��������", pEvent->m_sSecID);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pEvent->m_nDownloaded = -1;
			ChangeValue(pEvent);
			m_pCurrentOsc = NULL;
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
		ChangeDownloadedValue(pEvent, "0");
	}
}

/*************************************************************
 �� �� ����ChangeDownloadedValue()
 ���ܸ�Ҫ���޸��������ص��ļ������������ݴ�С
 �� �� ֵ: 
 ��    ����param1	�������ص��ļ�
		   Param2	���������ݴ�С
**************************************************************/
void CDownOscView::ChangeDownloadedValue( CXJEventOSC* pEvent, CString sValue )
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
	CString str;
	str.Format("%s K", sValue);
	//�ı�ֵ
	m_List.SetItemText(nIndex, 4, str);
	//ˢ��
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����ChangeValue()
 ���ܸ�Ҫ���ı�¼���б��Ƿ������ص���ʾ
 �� �� ֵ: 
 ��    ����param1	ָ��¼���б�
		   Param2
**************************************************************/
//##ModelId=49B87B8F0292
void CDownOscView::ChangeValue( CXJEventOSC* pEvent )
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
	//�ı��ļ���С
	CString str;
	str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(nIndex, 2, str);
	//�ı�ֵ
	m_List.SetItemText(nIndex, 4, pEvent->GetDownloadString());
	//ˢ��
	m_List.Update(nIndex);
}

void CDownOscView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//�ٻ�ʧ��
		if(m_pCurrentOsc != NULL)
		{
			m_pCurrentOsc->m_nDownloaded = -1;
			ChangeValue(m_pCurrentOsc);
			m_pCurrentOsc = NULL;
		}
	}
	if(nIDEvent == m_nDownTimer)
	{
		if(m_pCurrentOsc == NULL) //��ǰ����
		{
			//�鿴�Ƿ����ڵȴ����ص��¼�
			for(int i = 0; i < m_EventManager.GetCount(); i++)
			{
				CXJEventOSC* pOsc = (CXJEventOSC*)m_EventManager.GetAtEvent(i);
				if(pOsc == NULL)
					continue;
				if(pOsc->m_nDownloaded == 0) //δ����
				{
					m_pCurrentOsc = pOsc;
					//����
					DownloadOSCFile(m_pCurrentOsc);
					break;
				}
			}
		}
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ��: OnSTTP20069()
 ���ܸ�Ҫ: ��Ӧ20069(�ٻ�ʧ��)֪ͨ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 ��������
**************************************************************/
void CDownOscView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;

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
		str.Format("CDownOscView::OnSTTP20069 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDownOscView::OnSTTP20069 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}

	//�رռ�ʱ��
	KillTimer(m_nTimer);
	//���õ�ǰ�ٻ�ʧ��
	m_pCurrentOsc->m_nDownloaded = -1;
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;

}

/*************************************************************
 �� �� ��: OnSTTP20157()
 ���ܸ�Ҫ: ��Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDownOscView::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;

	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		WriteLog("CDownOscView::OnSTTP20157 ����Ϊ��", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20157����
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//������
		WriteLog("CDownOscView::OnSTTP20157 ���ĺŴ���, ��20157", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//���Ǳ�װ�õı���
		WriteLog("CDownOscView::OnSTTP20157 ���Ǳ�װ�ñ���", XJ_LOG_LV3);
		return;
	}
	
	//�ٻ�¼���ļ�
	if(pSttpData->sttp_body.nMsgId != 20042)
	{
		WriteLog("CDownOscView::OnSTTP20157 ���ĺŴ���, ��20042", XJ_LOG_LV3);
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
	
	//�����ٻ�¼���ļ�
	WriteLog("CDownOscView::OnSTTP20157 ˢ�³�ʱʱ��", XJ_LOG_LV3);
	int nTimeOut = pData->m_nWavFileTimeOut;
	m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	//�����������ص��ļ�������������ֵ
	CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
	ChangeDownloadedValue(m_pCurrentOsc, sValue);
}

/*************************************************************
 �� �� ��: OnSTTP20125()
 ���ܸ�Ҫ: ��Ӧװ�ò�֧��֪ͨ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDownOscView::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;
	
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
		str.Format("CDownOscView::OnSTTP20125 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDownOscView::OnSTTP20125 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	
	//�رռ�ʱ��
	KillTimer(m_nTimer);
	//���õ�ǰ�ٻ�ʧ��
	m_pCurrentOsc->m_nDownloaded = -1;
	//������ʾ
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;
}

void CDownOscView::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
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
		str.Format("CDownOscView::OnSTTP20043 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDownOscView::OnSTTP20043 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ�ǰҪ���ص��ļ�
	CString strFileName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	//ȡ�õ�һ�������ص�¼���б�
	strFileName.Replace("/", "\\");
	if(strFileName != m_pCurrentOsc->GetFileRelatePath())
	{
		//���Ǳ�¼���б���ļ�
		WriteLog("CDownOscView::OnSTTP20043 ���͵�¼���ļ����뱾¼���б��ļ���ƥ��");
		CString str;
		str.Format("����·��Ϊ%s, ����·��Ϊ%s", strFileName, m_pCurrentOsc->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		//return;
	}
	
	m_pCurrentOsc->m_nDownloaded = pSttpData->sttp_body.nStatus;
	m_pCurrentOsc->m_nOffset = 0;
	if (g_role != MODE_SUB)
	{
		if(m_pCurrentOsc->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag > 0)
		{
			m_pCurrentOsc->m_nDownloaded = 2; //��������
			m_pCurrentOsc->m_nOffset = pSttpData->sttp_body.nFlag;
		}
		else if(m_pCurrentOsc->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag <= 0)
		{
			m_pCurrentOsc->m_nDownloaded = -1; //����ʧ��
		}
		if(m_pCurrentOsc->m_nDownloaded == 1 && m_pCurrentOsc->m_nFileSize == 0)
		{
			//���سɹ�����¼�ļ���С
			m_pCurrentOsc->m_nFileSize = pSttpData->sttp_body.nFlag;
		}
	}
	else
	{
		m_pCurrentOsc->m_nDownloaded = 1;
	}
	//�رռ�ʱ��
	KillTimer(m_nTimer);
	//������ʾ
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;
	WriteLog("CDownOscView::OnSTTP20043 neat end", XJ_LOG_LV3);
}

void CDownOscView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_DOWNOSC))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{	
				pMenu->EnableMenuItem(IDC_DOWNOSC_DOWN, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_DOWNOSC_CANCEL, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//ֻѡ����һ��
				pMenu->EnableMenuItem(IDC_DOWNOSC_OPEN, MF_ENABLED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

void CDownOscView::OnDownoscDown() 
{
	// TODO: Add your command handler code here
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
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent == NULL)
				continue;
			if(pEvent == m_pCurrentOsc)
			{
				//���������ص��������
			}
			else
			{
				//�������־��Ϊδ���أ����´α���ʱ���ҵ�
				pEvent->m_nDownloaded = 0;
				ChangeValue(pEvent);
			}
		}
	}
}

void CDownOscView::OnDownoscCancel() 
{
	// TODO: Add your command handler code here
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
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent == NULL)
				continue;
			if(pEvent == m_pCurrentOsc)
			{
				//���������ص������ȡ������
				CancelCall();
				m_pCurrentOsc = NULL;
			}
			else
			{
				//�������־��Ϊȡ�����أ����´α���ʱ�Ҳ���
				if(pEvent->m_nDownloaded == 0)
				{
					pEvent->m_nDownloaded = -2;
					ChangeValue(pEvent);
				}
			}
		}
	}
}

void CDownOscView::OnDownoscOpen() 
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
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_nDownloaded  != 1)
	{
		if(AfxMessageBox( StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_OKCANCEL)==IDOK)
		{
			//���ӵ�����ҳ��
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->ShowDownOsc(pEvent);
		}
		return;
	}
	CString strFilePath = pEvent->GetFilePath();
	//���ļ�
	OpenWaveFile(strFilePath, GetSafeHwnd());
}

/*************************************************************
 �� �� ��: CancelCall()
 ���ܸ�Ҫ: ȡ���ٻ�����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDownOscView::CancelCall()
{
	//��ǰ�ǿ��е�, �˲�����Ч
	if(m_pCurrentOsc == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���뱨����������
	STTP_FULL_DATA sttpData;
	//ֹͣ��ʱ��
	KillTimer(m_nTimer);
	
	//��������¼���ļ�
	//�齨ȡ������
	pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20042, m_pCurrentOsc->m_sSecID, 0);

	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_pCurrentOsc->m_nDownloaded = -2;
	ChangeValue(m_pCurrentOsc);
}

void CDownOscView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				if(nItem%2 != 0)
				{
					lplvcd->clrTextBk = RGB(204, 253, 251);
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
 CreateDate: 19:1:2010     Author:LYH
 �� �� ��: NeedAdd()
 ���ܸ�Ҫ: �ж�ָ���¼��Ƿ�Ҫ���뵽�������б�
 �� �� ֵ: Ҫ���뷵��TRUE, ��Ҫ���뷵��FALSE
 ��    ��: param1 ָ��¼���¼�
		   Param2 
**************************************************************/
BOOL CDownOscView::NeedAdd( CXJEventOSC* pEvent, CXJEventOSC*& pExist )
{
	if(pEvent == NULL)
		return FALSE;
	pExist = NULL;
	BOOL bReturn = TRUE;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList == NULL)
		return FALSE;
	for(int i = 0; i < pList->GetSize(); i++)
	{
		CXJEventOSC* pOsc = (CXJEventOSC*)pList->GetAt(i);
		if(pOsc == NULL)
			continue;
		if(pEvent->m_lID >= 0) //��ID���Ƚ�ID
		{
			if(pOsc->m_lID == pEvent->m_lID)
			{
				pExist = pOsc;
				return FALSE;
			}
		}
		else
		{
			//���¼���û��ID���Ƚ��ļ���
			if(pOsc->m_sFileName == pEvent->m_sFileName)
			{
				pExist = pOsc;
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CDownOscView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile( StringFromID(IDS_FUNC_OSC_DOWNLOAD));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
