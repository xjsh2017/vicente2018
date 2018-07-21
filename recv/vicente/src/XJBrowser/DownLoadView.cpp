// DownLoadView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownLoadView.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/** @brief           ����������λ*/
int g_iDownloadViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView

IMPLEMENT_DYNCREATE(CDownLoadView, CViewBase)

//##ModelId=49B87BA3010B
CDownLoadView::CDownLoadView()
	: CViewBase(CDownLoadView::IDD)
{
	//{{AFX_DATA_INIT(CDownLoadView)
	m_strCondition = _T("");
	//}}AFX_DATA_INIT
	m_nConditionType = 1;
	m_nCurrentStatus = 0;
	m_sStationID = "";
	m_nTimer = -1;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bUseBreakPoint = FALSE;
	m_nNum = 1;
	m_nOldSortCol = -1;
}

//##ModelId=49B87BA30251
CDownLoadView::~CDownLoadView()
{
	ClearFileList();
}

//##ModelId=49B87BA30232
void CDownLoadView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownLoadView)
	DDX_Control(pDX, IDC_CMB_DOWN_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_BTN_FILE_CLOSE, m_btnFileClose);
	DDX_Control(pDX, IDC_GIF_DOWNLOAD, m_gif);
	DDX_Control(pDX, IDC_BTN_DOWNLOAD_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_DOWNLOAD_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_DOWN, m_List);
	DDX_Text(pDX, IDC_EDIT_DOWN_CONDITION, m_strCondition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownLoadView, CViewBase)
	//{{AFX_MSG_MAP(CDownLoadView)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CMB_DOWN_CONDITION, OnSelchangeCmbDownCondition)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_CALL, OnBtnDownloadCall)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_DOWN, OnBtnDownloadDown)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DOWN, OnColumnclickListDown)
	ON_BN_CLICKED(IDC_BTN_FILE_CLOSE, OnBtnFileClose)
	ON_BN_DOUBLECLICKED(IDC_BTN_DOWNLOAD_CALL, OnDoubleclickedBtnDownloadCall)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_OPEN, OnBtnDownloadOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOWN, OnDblclkListDown)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNLOAD_OPEN, OnDownloadOpen)
	ON_MESSAGE(WM_STTP_00204, OnSTTP00204)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_00212, OnSTTP00212)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(COMMONFILE_DOWNLOAD_END, DownloadFileEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BA30261
void CDownLoadView::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87BA30271
void CDownLoadView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView message handlers

//##ModelId=49B87BA30223
void CDownLoadView::OnInitialUpdate() 
{
	//����
	RecordRate(IDC_STATIC_DOWNLOAD_1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_4, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_5, 0, left_client, top_client);
	//
	RecordRate(IDC_CMB_DOWN_STATION, 0, left_client, top_client);
	RecordRate(IDC_CMB_DOWN_CONDITION, 0, left_client, top_client);
	RecordRate(IDC_EDIT_DOWN_CONDITION, 0, left_client, top_client);
	RecordRate(IDC_DP_DOWN_TIME1, 0, left_client, top_client);
	RecordRate(IDC_DP_DOWN_TIME2, 0, left_client, top_client);
	//����
	RecordRate(IDC_BTN_FILE_CLOSE, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_DOWN, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_OPEN, 0, left_client, top_client);
	RecordRate(IDC_GIF_DOWNLOAD, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_DOWN, 0, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//��ʼ���б��
	InitListStyle();
	//��ʼ����ť
	InitControls();
	//��ʼ����վ������
	FillStation();
	//��ʼ������������
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_CONDITION);
	if(pCmb == NULL)
		return;
	pCmb->AddString( StringFromID(IDS_COMMONFILE_CONDITION_FILENAME));
	pCmb->AddString( StringFromID(IDS_COMMONFILE_CONDITION_TIME));
	pCmb->SetCurSel(0);
	//��ʼ�������༭��
	m_strCondition = "*.*";
	//����ʱ���
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME1);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME2);
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

	UpdateData(FALSE);
	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
	m_gif.ShowWindow(SW_HIDE);
}

//##ModelId=49B87BA30280
void CDownLoadView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_DOWN, left_client, IDC_DP_DOWN_TIME2, right_client, bottom_client);
}

/*************************************************************
 �� �� ����OnDownloadOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA302CF
void CDownLoadView::OnDownloadOpen( WPARAM wParam, LPARAM lParam )
{
	//ǿ��ˢ��һ��
	RegulateControlSize();
	MoveCtrl(IDC_LIST_DOWN, left_client, IDC_DP_DOWN_TIME2, right_client, bottom_client);
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BA301A5
BOOL CDownLoadView::LoadListConfig()
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
				WriteLog("DownloadViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//DownloadViewConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�DownloadViewConfig");
				WriteLog("DownloadViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�DownloadViewConfig�µ�����");
					WriteLog("DownloadViewConfig::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
BOOL CDownLoadView::SaveListConfig()
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
			WriteLog("DownloadViewConfig::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("DownloadViewConfig::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//DownloadViewConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�DownloadViewConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("DownloadViewConfig");
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
						WriteLog("δ�ҵ�DownloadViewConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���DownloadViewConfig�µ�����, ��ɾ���½������ӽڵ�");
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
BOOL CDownLoadView::NeedSave()
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
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���ʽ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301D4
int CDownLoadView::InitListStyle()
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
		//char* arColumn[6]={"���", "��վ����", "�ļ���","�ļ���С","�ļ�ʱ��","�������"};
		for (int nCol=0; nCol < 6 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; //
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1: //��վ��
				lvCol.cx = 160;
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 2: //�ļ���
				lvCol.cx = 200;
				colName = StringFromID(IDS_COMMON_FILENAME);
				break;
			case 3: //�ļ���С
				lvCol.cx = 100;
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 4: //�ļ�ʱ��
				lvCol.cx = 180;
				colName = StringFromID(IDS_COMMON_FILETIME);
				break;
			case 5: //������
				lvCol.cx = 100;
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			default:
				lvCol.cx=100;//
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��, ���
		m_List.SetColumnHide(0, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ�������ؼ�(��List���)
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301D5
int CDownLoadView::InitControls()
{

	return 0;
}

/*************************************************************
 �� �� ����FillStation()
 ���ܸ�Ҫ����䳧վ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301E4
void CDownLoadView::FillStation()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_STATION);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//���뵽������
		int nIndex = pCmb->AddString(pStation->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pStation);
		}
	}
	pCmb->SetCurSel(0);

	m_cmbStation.OnDataFilled(TRUE, 0);
}

/*************************************************************
 �� �� ����ClearFileList()
 ���ܸ�Ҫ���������ͨ���ļ���¼
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301E5
void CDownLoadView::ClearFileList()
{
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = (CCommonFile*)m_FileList.GetNext(pos);
		if(pFile == NULL)
			continue;
		delete pFile;
	}
	m_FileList.RemoveAll();
}

//##ModelId=49B87BA3029F
void CDownLoadView::OnSelchangeCmbDownCondition() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_CONDITION);
	if(pCmb == NULL)
		return;
	m_nConditionType = pCmb->GetCurSel() + 1;
}

/*************************************************************
 �� �� ����OnBtnDownloadCall()
 ���ܸ�Ҫ������ͨ���ļ��б�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA302A1
void CDownLoadView::OnBtnDownloadCall() 
{
	// TODO: Add your control notification handler code here
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
		//����״̬, �����ť��ʾҪ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;

		//ˢ�¿ؼ�ֵ
		UpdateData(TRUE);

		//ȡ�ó�վ
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_STATION);
		if(pCmb == NULL)
			return;
		int nSel = pCmb->GetCurSel();
		if(nSel < 0)
			return;
		CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
		if(pStation == NULL)
			m_sStationID = "";
		else
			m_sStationID = pStation->m_sID;
		//ȡ��ʱ��
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME1);
		if(pTime == NULL)
			return;
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME2);
		if(pTime2 == NULL)
			return;
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//��֯00203����
		if(!pApp->m_SttpEngine.BuildDataFor00203CommonFileList(sttpData, m_sStationID, m_nConditionType, tmStart, tmEnd, m_strCondition))
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
				str = "�����ٻ�ͨ���ļ��б���ʧ��,ԭ��Ϊ�����ж�";
				WriteLog(str);
			}
			else if(nReturn == -2)
			{
				CString str;
				str = "�����ٻ�¼���б���ʧ��,ԭ��Ϊ��������";
				WriteLog(str);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ��б�
			m_nCurrentStatus = 1;
			
			//������ʱ��
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nCommFileListTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//���ݵ�ǰ״̬���¿ؼ�������
			UpdateControlsEnable();
			
			//ɾ��֮ǰ���б�
			m_List.DeleteAllItems();
			ClearFileList();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//ȡ���ٻ�
		CancelCall();
	}
}

//##ModelId=49B87BA302AF
void CDownLoadView::OnBtnDownloadDown() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �����ļ���ȡ������
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
		//ȡ�õ�ǰѡ���¼���б�
		int nCount = m_List.GetSelectedCount();
		if(nCount < 1)
		{
			AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
			return;
		}
		//��ѡ����ļ��б���뵽���ٻ�����
		m_FileDownload.RemoveAll();
		m_CallFileResult = "";
		
		int nIndex = -1;
		for(int i = 0; i < nCount; i++)
		{
			nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
			if(nIndex != -1)
			{
				CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nIndex);
				if(pFile != NULL)
				{
					//���뵽���ٻ�����
					m_FileDownload.AddTail(pFile);
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

		if(m_FileDownload.GetCount() > 0)
		{
			//��ʼ��һ������
			CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
			if(pFile != NULL)
				DownloadCommonFile(pFile);
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
	else if(m_nCurrentStatus == 2)
	{
		//ȡ���ٻ�
		CancelCall();
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301F4
void CDownLoadView::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//��ť
		m_btnCall.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_DOWNLOAD_CALL, StringFromID(IDS_FUNC_CALL_LIST));
		m_btnDown.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_DOWNLOAD_DOWN, StringFromID(IDS_FUNC_CALL_FILE));
		
		//������
		GetDlgItem(IDC_CMB_DOWN_CONDITION) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_DOWN_STATION) ->EnableWindow(TRUE);
		//ʱ��
		GetDlgItem(IDC_DP_DOWN_TIME1) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_DOWN_TIME2) ->EnableWindow(TRUE);
		//�༭��
		GetDlgItem(IDC_EDIT_DOWN_CONDITION) ->EnableWindow(TRUE);
		
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
		
		//������
		GetDlgItem(IDC_CMB_DOWN_CONDITION) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_DOWN_STATION) ->EnableWindow(FALSE);
		//ʱ��
		GetDlgItem(IDC_DP_DOWN_TIME1) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_DOWN_TIME2) ->EnableWindow(FALSE);
		//�༭��
		GetDlgItem(IDC_EDIT_DOWN_CONDITION) ->EnableWindow(FALSE);
		
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_DOWN, str);
		
		if(m_nCurrentStatus == 1)
		{
			//�ٻ��ļ��б�
			m_btnCall.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_DOWNLOAD_CALL, CANCEL_CALL);
		}
		else if(m_nCurrentStatus == 2)
		{
			//�����ļ�
			m_btnDown.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_DOWNLOAD_DOWN, CANCEL_CALL);
		}
	}
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301F5
void CDownLoadView::CancelCall()
{
	//��ǰ�ǿ��е�, �˲�����Ч
	if(m_nCurrentStatus == 0)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���뱨����������
	STTP_FULL_DATA sttpData;
	//ֹͣ��ʱ��
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ��б�
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 203, "", 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//���������ļ�
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 210, "", 0);
		//��մ������б�
		m_FileDownload.RemoveAll();
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
 �� �� ����OnSTTP00204()
 ���ܸ�Ҫ����Ӧ00204, ͨ���ļ���Ϣһ��֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87BA302DE
void CDownLoadView::OnSTTP00204( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ��б��״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�00204����
	if(pSttpData->sttp_head.uMsgID != 204)
	{
		//������
		CString str;
		str.Format("CDownLoadView::OnSTTP00204 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(m_sStationID != "")
	{
		if(strID != m_sStationID)
		{
			//������ѡ��վ�ı���
			CString str;
			str.Format("CDownLoadView::OnSTTP00204 �յ�������վ����, ��վIDΪ%s", strID);
			WriteLog(str);
			return;
		}
	}
	
	//������
	//��֡�������ļ�����
	int nFileNum = pSttpData->sttp_body.variant_member.file_list.size();
	for(int i = 0; i < nFileNum; i++)
	{
		//�½�����
		CCommonFile* pFile = new CCommonFile;
		if(!pFile->LoadFromSTTP00204(pSttpData, i))
		{
			delete pFile;
			pFile = NULL;
			continue;
		}
		//����ID
		pFile->m_nID = m_nNum++;
		//���뵽����
		m_FileList.AddTail(pFile);
	}

	//�ж��Ƿ��к���֡
	if(pSttpData->sttp_head.uMsgEndFlag == 0)
	{
		//ֹͣ��ʱ��
		KillTimer(m_nTimer);
		//ˢ��״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();

		//���һ֡, ���List
		FillListData();

		//�����˹�������־
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString str;
		str.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_COMMFILELIST);
		pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_SUCCESS);
	}
}

/*************************************************************
 �� �� ����FillListData()
 ���ܸ�Ҫ������б������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30203
void CDownLoadView::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = (CCommonFile*)m_FileList.GetNext(pos);
		if(pFile == NULL)
			continue;
		//���뵽��ʾ
		AddFileToList(pFile);
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ����AddFileToList()
 ���ܸ�Ҫ�������ļ������б������ʾ
 �� �� ֵ: void
 ��    ����param1	�ļ�����
		   Param2	�б������
**************************************************************/
//##ModelId=49B87BA30204
void CDownLoadView::AddFileToList( CCommonFile* pFile, int nIndex /*= 0*/ )
{
	//�ж�������Ч��
	if(pFile == NULL)
		return;
	if(nIndex < 0)
		return;
	//"���", "��վ����", "�ļ���","�ļ���С","�ļ�ʱ��","������"
	//0:���
	CString str;
	str.Format("%d", pFile->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:��վ��
	str = "";
	if(pFile->m_pStation != NULL)
		str = pFile->m_pStation->m_sName;
	m_List.SetItemText(nIndex, 1, str);
	//2:�ļ���
	m_List.SetItemText(nIndex, 2, pFile->m_sFilePath);
	//3:�ļ���С
	str.Format("%d Byte", pFile->m_nFileSize);
	m_List.SetItemText(nIndex, 3, str);
	//4:�ļ�ʱ��
	m_List.SetItemText(nIndex, 4, pFile->m_sFileTime);
	//5:������
	m_List.SetItemText(nIndex, 5, pFile->GetDownloadString());
	//��������
	m_List.SetItemData(nIndex, (DWORD)pFile);
}

//##ModelId=49B87BA302B1
void CDownLoadView::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ20069(����ʧ��)����
 �� �� ֵ: void
 ��    ����param1
		   Param2	��������
**************************************************************/
//##ModelId=49B87BA302EF
void CDownLoadView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//û�д��������ļ����ٻ��б��״̬,��������
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
		str.Format("CDownLoadView::OnSTTP20069 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDownLoadView::OnSTTP20069 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//�ٻ�¼���б�
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
		//ɾ������
		ClearFileList();
		//�Ǳ����ٻ���ʧ�ܱ���
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�رռ�ʱ��
		KillTimer(m_nTimer);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString str;
		str.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_COMMFILELIST);
		pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_FAILD);

		//��ʾʧ��
		AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_CALL_FAIL));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�ٻ�ͨ���ļ�ʧ��
		if(pSttpData->sttp_body.nMsgId != 210)
			return;
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		pFile->m_nIsDownload = 0;
		pFile->m_nOffset = 0;
		
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//������Ϣ
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
	}
}

//##ModelId=49B87BA302BF
void CDownLoadView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//ɾ������
			ClearFileList();
			//�ı�״̬
			m_nCurrentStatus = 0;
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString str;
			str.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_COMMFILELIST);
			pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_FAILD);
			//��ʾ
			AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_CALL_TIMEOUT));
		}
		else if(m_nCurrentStatus == 2)
		{
			//���������Ϣ
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 3);
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_DOWN, str);
	}

	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����DownloadFileEnd()
 ���ܸ�Ҫ�������ļ���Ӧ
 �� �� ֵ: void
 ��    ����param1
		   Param2 �ٻ����. 1-����ʧ��. 2-����ʧ��. 3-��ʱ. 4.-װ�ò�֧��. 5-�ɹ�. 6-��������
**************************************************************/
//##ModelId=49B87BA302FD
void CDownLoadView::DownloadFileEnd( WPARAM wParam, LPARAM lParam )
{
	//û�д�������¼���ļ���״̬, ��������Ϣ
	if(m_nCurrentStatus != 2)
		return;
	if(m_FileDownload.GetCount() < 1)
	{
		//û�д������ļ�, ���������
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	}
	CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
	//��¼��ɽ��
	m_CallFileResult += StringFromID(IDS_FUNC_CALL);
	m_CallFileResult += "[";
	m_CallFileResult += pFile->m_sFilePath;
	m_CallFileResult += "]";
	m_CallFileResult += "\t";
	int nResult = (int)lParam;

	int nOperResult = OPER_SUCCESS;
	switch(pFile->m_nIsDownload)
	{
	case 0:
		m_CallFileResult += StringFromID(IDS_COMMON_FAILED);
		nOperResult = OPER_FAILD;
		break;
	case 1:
		m_CallFileResult += StringFromID(IDS_COMMON_SUCCESS);
		break;
	case 2:
		m_CallFileResult += StringFromID(IDS_DOWNLOAD_PARTIAL);
		nOperResult = OPER_FAILD;
		break;
	default:
		break;
	}
	ChangeValue(pFile);
	m_CallFileResult += "\r\n";
	//�Ƴ���һ���������б�
	m_FileDownload.RemoveHead();


	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	str.Format("%d,%d", nOperResult, XJ_OPER_CALL_COMMFILE);
	pApp->AddNewManOperator(FUNC_QUY_DOWNLOADFILE, m_sStationID, str, XJ_OPER_CALL_COMMFILE, nOperResult);

	//����Ƿ��д������б�
	if(m_FileDownload.GetCount() < 1)
	{
		//��û�е������б�,��ʾȫ���������
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(m_CallFileResult);
	}
	else
	{
		CCommonFile* pNew = (CCommonFile*)m_FileDownload.GetHead();
		DownloadCommonFile(pNew);
	}
}

/*************************************************************
 �� �� ����ChangeValue()
 ���ܸ�Ҫ���ı��ļ��б��Ƿ������ص���ʾ
 �� �� ֵ: 
 ��    ����param1	ָ���б�
		   Param2
**************************************************************/
//##ModelId=49B87BA30213
void CDownLoadView::ChangeValue( CCommonFile* pFile )
{
	//���������Ч��
	if(pFile == NULL)
		return;
	//����λ��
	int nIndex = -1;
	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pFile;
	nIndex = m_List.FindItem(&lvFind);
	//�Ҳ���λ��
	if(nIndex < 0)
		return;
	//�ı�ֵ
	m_List.SetItemText(nIndex, 5, pFile->GetDownloadString());
	//ˢ��
	m_List.Update(nIndex);
}

/*************************************************************
 �� �� ����DownloadCommonFile()
 ���ܸ�Ҫ������ͨ���ļ�
 �� �� ֵ: 
 ��    ����param1	ָ���б�
		   Param2
**************************************************************/
//##ModelId=49B87BA30215
void CDownLoadView::DownloadCommonFile( CCommonFile* pFile )
{
	//��Ӧ��ť���²���, ����¼���ļ���ȡ���ٻ�
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	if(pFile == NULL)
	{
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
		return;
	}
	if(pFile->m_pStation == NULL)
	{
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
		return;
	}
	
	//���뱨����������
	STTP_FULL_DATA sttpData;

	int nOffset = 0;
	if(m_bUseBreakPoint)
	{
		nOffset = pFile->m_nOffset;
	}
	
	//��֯00210����
	if(!pApp->m_SttpEngine.BuildDataFor00210CommonFileDown(sttpData, pFile->m_sStationID, pFile->m_sFilePath, nOffset, pFile->m_pStation->m_sIP))
	{
		//��֯����ʧ��
		//�����ٻ������Ϣ,���Ϊ����ʧ��
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
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
			str.Format("�����ٻ�ͨ���ļ�����ʧ��,ԭ��Ϊ�����ж�", pFile->m_sFilePath);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pFile->m_nIsDownload = 0;
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("�����ٻ�ͨ���ļ�����ʧ��,ԭ��Ϊ��������", pFile->m_sFilePath);
			WriteLog(str);
			//�����ٻ������Ϣ,���Ϊ����ʧ��
			pFile->m_nIsDownload = 0;
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
			return;
		}
	}
	else
	{
		pFile->m_nIsDownload = 0;
		//������ʱ��
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nCommFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//�ı��������ݴ�С
		ChangeDownloadedValue(pFile, "0");
	}
}

/*************************************************************
 �� �� ����OnSTTP00212()
 ���ܸ�Ҫ����Ӧ00212,�ļ��������֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30301
void CDownLoadView::OnSTTP00212( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2)
	{
		//û�д����ٻ��ļ���״̬,��������
		return;
	}

	if(m_FileDownload.GetCount() < 1)
	{
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�00212����
	if(pSttpData->sttp_head.uMsgID != 212)
	{
		//������
		CString str;
		str.Format("CDownLoadView::OnSTTP00212 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}

	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	
	CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
	if(pFile == NULL)
		return;
	if(pFile->m_sStationID != strID)
		return;
	CString strName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	strName.Replace("/", "\\");
	if(pFile->GetFileRelatePath() != strName)
	{
		WriteLog("CDownLoadView::OnSTTP00212 ���͵��ļ����뱾�б��ļ���ƥ��");
		CString str;
		str.Format("����·��Ϊ%s, ����·��Ϊ%s", strName, pFile->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	pFile->m_nIsDownload = pSttpData->sttp_body.nStatus;
	pFile->m_nOffset = 0;
	if(pFile->m_nIsDownload == 0 && pSttpData->sttp_body.nFlag > 0)
	{
		pFile->m_nIsDownload = 2;
		pFile->m_nOffset = pSttpData->sttp_body.nFlag;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	//������Ϣ
	this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
}

/*************************************************************
 �� �� ����CheckDownload()
 ���ܸ�Ҫ���˶�Ҫ���ص��ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30222
void CDownLoadView::CheckDownload()
{
	if(m_FileDownload.GetCount() < 1)
		return;
	BOOL bHasSuccess = FALSE; //�Ƿ���������ص��ļ�
	BOOL bHasHalf = FALSE; //�Ƿ�����������ص��ļ�

	POSITION pos = m_FileDownload.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = m_FileDownload.GetNext(pos);
		if(pFile == NULL)
			continue;
		if(pFile->m_nIsDownload == 1)
		{
			bHasSuccess = TRUE;
		}
		if(pFile->m_nIsDownload == 2)
		{
			bHasHalf = TRUE;
		}
	}

	if(bHasSuccess)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_AGAIN), MB_YESNO) == IDYES)
		{

		}
		else
		{
			//����������, ȥ�������ص��ļ�
			POSITION pos = m_FileDownload.GetHeadPosition();
			while(pos != NULL)
			{
				POSITION oldPos = pos;
				CCommonFile* pFile = m_FileDownload.GetNext(pos);
				if(pFile == NULL)
					continue;
				if(pFile->m_nIsDownload == 1)
				{
					m_FileDownload.RemoveAt(oldPos);
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
//##ModelId=49B87BA3013A
int CALLBACK CDownLoadView::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CCommonFile* pFile1 = (CCommonFile*)lParam1;
	CCommonFile* pFile2 = (CCommonFile*)lParam2;
	if(pFile1 == NULL || pFile2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���", "��վ����", "�ļ���","�ļ���С","�ļ�ʱ��","������"
	switch(nCol)
	{
	case 0: //���
		iResult = pFile1->m_nID - pFile2->m_nID;
		break;
	case 1: //��վ����
		str1 = "";
		if(pFile1->m_pStation != NULL)
			str1 = pFile1->m_pStation->m_sName;
		str2 = "";
		if(pFile2->m_pStation != NULL)
			str2 = pFile2->m_pStation->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //�ļ���
		str1 = pFile1->m_sFileName;
		str2 = pFile2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //�ļ���С
		iResult = pFile1->m_nFileSize - pFile2->m_nFileSize;
		break;
	case 4: //�ļ�ʱ��
		str1 = pFile1->m_sFileTime;
		str2 = pFile2->m_sFileTime;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //������
		iResult = pFile1->m_nIsDownload - pFile2->m_nIsDownload;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDownloadViewAsc;
	return iResult;
}

//##ModelId=49B87BA302C2
void CDownLoadView::OnColumnclickListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iDownloadViewAsc = - g_iDownloadViewAsc;
	}
	else
	{
		g_iDownloadViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87BA3030E
void CDownLoadView::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//û�д��������ļ����ٻ��б��״̬,��������
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
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//������
		CString str;
		str.Format("CDownLoadView::OnSTTP20125 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDownLoadView::OnSTTP20125 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//�ٻ�¼���б�
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
		//ɾ������
		ClearFileList();
		//�Ǳ����ٻ���ʧ�ܱ���
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�رռ�ʱ��
		KillTimer(m_nTimer);
		//��ʾʧ��
		AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_NOTSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�ٻ�ͨ���ļ�ʧ��
		if(pSttpData->sttp_body.nMsgId != 210)
			return;
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		pFile->m_nIsDownload = 0;
		pFile->m_nOffset = 0;
		
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//������Ϣ
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
	}
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30312
void CDownLoadView::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
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
		WriteLog("CDownLoadView::OnSTTP20157 ����Ϊ��", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//������
		WriteLog("CDownLoadView::OnSTTP20157 ����!=20157", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//���Ǳ�װ�õı���
		WriteLog("CDownLoadView::OnSTTP20157 �Ǳ�װ�ñ���", XJ_LOG_LV3);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//�ٻ�ͨ���ļ��б�
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
	}
	else if(m_nCurrentStatus == 2)
	{
		//�ٻ�ͨ���ļ�
		if(pSttpData->sttp_body.nMsgId != 210)
		{
			WriteLog("CDownLoadView::OnSTTP20157 ���ٻ�ͨ���ļ���Ӧ", XJ_LOG_LV3);
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
		//�����ٻ�ͨ���ļ��б�				
		int nTimeOut = pData->m_nCommFileListTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�ͨ���ļ��б�
		WriteLog("CDownLoadView::OnSTTP20157 �������ó�ʱʱ��", XJ_LOG_LV3);
		int nTimeOut = pData->m_nCommFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//�����������ļ���С
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
		ChangeDownloadedValue(pFile, sValue);
	}
}

/*************************************************************
 �� �� ����ChangeDownloadedValue()
 ���ܸ�Ҫ�������������ص��ļ������������ݴ�С
 �� �� ֵ: 
 ��    ����param1	�������ص��ļ�
		   Param2	�������ļ���С
**************************************************************/
void CDownLoadView::ChangeDownloadedValue( CCommonFile* pFile, CString sValue )
{
	//���������Ч��
	if(pFile == NULL)
		return;
	//����λ��
	int nIndex = -1;
	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pFile;
	nIndex = m_List.FindItem(&lvFind);
	//�Ҳ���λ��
	if(nIndex < 0)
		return;
	CString str;
	str.Format("%s K", sValue);
	//�ı�ֵ
	m_List.SetItemText(nIndex, 5, str);
	//ˢ��
	m_List.Update(nIndex);
}

void CDownLoadView::OnBtnFileClose() 
{
	// TODO: Add your control notification handler code here
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CDownLoadView::OnDoubleclickedBtnDownloadCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CDownLoadView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile( StringFromID(IDS_FUNC_COMMONFILE));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDownLoadView::OnBtnDownloadOpen() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox( StringFromID(IDS_COMMON_SELECTFORWATCH));
		return;
	}
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nIndex);
	if(!pFile || pFile->m_nIsDownload != 1)
	{
		AfxMessageBox( StringFromID(IDS_COMMON_FILE_NOTDOWNLOADED));
		return;
	}
	OpenFileDir(pFile);
}

/****************************************************
Date:2013/12/23  Author:LYH
������:   OpenFileDir	
����ֵ:   void	
���ܸ�Ҫ: 
����: CCommonFile * pFile	
*****************************************************/
void CDownLoadView::OpenFileDir( CCommonFile* pFile )
{
	if(!pFile)
		return;
	CString str = "/n,/select,";
	str += pFile->GetFilePath();
	ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
}

void CDownLoadView::OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//������¼�, ȷ�ϴ��¼�
		//�õ����ж�Ӧ���¼�
		CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nSelected);
		if(!pFile || pFile->m_nIsDownload != 1)
		{
			AfxMessageBox( StringFromID(IDS_COMMON_FILE_NOTDOWNLOADED));
			return;
		}
		OpenFileDir(pFile);
	}
	*pResult = 0;
}
