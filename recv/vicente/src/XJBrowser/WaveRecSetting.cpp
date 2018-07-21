// WaveRecSetting.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecSetting.h"
#include "DlgSettingDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WRSettingLoad(LPVOID pParam)
{
	CWaveRecSetting* wrSetting = (CWaveRecSetting*)pParam;
	if(wrSetting == NULL)
		return 0;
	//��ȡ����
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//�����˳��߳�,�˳�
		if(wrSetting->m_bExitThread)
			return 0;
		//�������Դ���
		if(nCount < 0)
			return 0;
		//��ȡ���, �˳�
		if(bLoad)
		{
			//log
			WriteLog("¼��������--��ֵҳ��, ��ȡ���ݽ���, �����˳��߳�!");
			return 0;
		}
		if(wrSetting->ReFillAll())
		{
			bLoad = TRUE;
		}
		else
		{
			//��ȡʧ��
			CString str;
			str.Format("wrSetting, ʧ��%d��", (4 - nCount));
			WriteLog(str);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting

IMPLEMENT_DYNCREATE(CWaveRecSetting, CViewBase)

//##ModelId=49B87B7F0020
CWaveRecSetting::CWaveRecSetting()
	: CViewBase(CWaveRecSetting::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_arrSetting.RemoveAll();
	m_arrGroup.RemoveAll();
	m_nNewValueIndex = 4;
	m_nSrc = 0;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bCanGroup = FALSE;

	m_arrNewSetting.RemoveAll();
	m_arrFrame.RemoveAll();
	m_nFrameCount = 0;

	m_bExitThread = FALSE;
	m_pLoadThread = NULL;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
}

//##ModelId=49B87B7F038A
CWaveRecSetting::~CWaveRecSetting()
{
	ClearSettingConfig();
	ClearSettingGroup();
}

//##ModelId=49B87B7F033C
void CWaveRecSetting::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecSetting)
	DDX_Control(pDX, IDC_GIF_WRSET, m_gif);
	DDX_Control(pDX, IDC_TAB_WRSET, m_TabCtrl);
	DDX_Control(pDX, IDC_BTN_WRSET_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_WRSET_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_WRSET_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_WRSET, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecSetting, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecSetting)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_WRSET_CALL, OnBtnWrsetCall)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_WRSET, OnSelchangeTabWrset)
	ON_CBN_SELCHANGE(IDC_CMB_WRSET_CPU, OnSelchangeCmbWrsetCpu)
	ON_CBN_SELCHANGE(IDC_CMB_WRSET_SRC, OnSelchangeCmbWrsetSrc)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_WRSET, OnCustomDraw)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRSET_CALL, OnDoubleclickedBtnWrsetCall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WRSET, OnDblclkListWrset)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting diagnostics

#ifdef _DEBUG
//##ModelId=49B87B7F03A9
void CWaveRecSetting::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B7F03D8
void CWaveRecSetting::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting message handlers

//##ModelId=49B87B7F031C
void CWaveRecSetting::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	//����һ��
	RecordRate(IDC_STATIC_WRSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_WRSET_SRC, 0, left_client, top_client);
	
	RecordRate(IDC_STATIC_WRSET_CPU, 0, left_client, top_client);
	RecordRate(IDC_CMB_WRSET_CPU, 0, left_client, top_client);
	//TAB��
	RecordRate(IDC_TAB_WRSET, 1, left_client, top_client);
	//����һ�Ű�ť
	RecordRate(IDC_BTN_WRSET_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRSET_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRSET_STYLE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRSETTING_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRSETTING_P2, 0, left_client, top_client);
	RecordRate(IDC_GIF_WRSET, 0, null_effect, top_client, right_client, null_effect);
	RecordRate(IDC_STATIC_TIME_WRSETTING, 0, null_effect, top_client, right_client, null_effect);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();

	//��ʼ���б�
	InitListCtrl();
	//��ʼ�������ؼ�
	InitControls();
	//�ж�����/��ʾ ����TAB��
	if(g_PTSettingGroup != 1)
	{
		//����
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_HIDE);
	}
	else
	{
		//��ʾ
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_SHOW);
	}

	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

//##ModelId=49B87B80001F
void CWaveRecSetting::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(g_PTSettingGroup != 1) //��ֵ������
		MoveCtrl(IDC_LIST_WRSET, left_client, IDC_BTN_WRSET_CALL, right_client, bottom_client);
	else //����
	{
		MoveCtrl(IDC_LIST_WRSET, left_client, IDC_TAB_WRSET, right_client, bottom_client);
	
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.top -= 10;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}
	}
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B7F0109
int CWaveRecSetting::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
// 	if(g_PTSettingGroup == 1)
// 	{
// 		//��ֵ��
// 		LoadPTGroupConfig();
// 	}
// 	//��ֵ����
// 	LoadPTSettingConfig();
// 	//��ֵֵ
// 	LoadPTHistorySettingData();

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B7F0128
int CWaveRecSetting::InitListStyle()
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
		char* arColumn[4]={"���","��ֵ����", "��ֵ����", "��λ"};
		LV_COLUMN lvCol;
		lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM| LVS_SHOWSELALWAYS;
		lvCol.fmt=LVCFMT_LEFT;
		
		//�����п�,��С����
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // ���
				break;
			case 1://
				lvCol.cx = 140; // ��ֵ����
				break;
			case 2://
				lvCol.cx = 140; // ��ֵ����
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=arColumn[nCol];
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��ֵ����)
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
//##ModelId=49B87B7F0167
BOOL CWaveRecSetting::LoadListConfig()
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
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!");
			WriteLog("WaveRecSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("WaveRecSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//WaveRecSettingConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�WaveRecSettingConfig");
				WriteLog("WaveRecSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�WaveRecSettingConfig�µ�����");
					WriteLog("WaveRecSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

//##ModelId=49B87B7F0196
BOOL CWaveRecSetting::SaveListConfig()
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
			WriteLog("WaveRecSetting::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("WaveRecSetting::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//WaveRecSettingConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("WaveRecSettingConfig");
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
						WriteLog("δ�ҵ�WaveRecSettingConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���WaveRecSettingConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B7F0148
int CWaveRecSetting::FillListData()
{
	//���ListCtrl����
	m_List.DeleteAllItems();
	PT_SETTING *pts = NULL;
	for(int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(g_PTSettingGroup == 1 && m_bCanGroup)
		{
			//������ʾ
			if(pts->Group != m_nGroup)
			{
				//��������ڵ�ǰ��ѡ���
				continue;
			}
		}
		m_List.InsertItem(i, pts->ID); //ID
		m_List.SetItemText(i, 1, pts->Name); //����
		m_List.SetItemText(i, 2, pts->CodeName); //����
		m_List.SetItemText(i, 3, pts->Unit); //��λ
	//	m_List.SetItemText(i, 3, pts->OrderValue); //��׼ֵ
	}

	return 0;
}

//##ModelId=49B87B7F01B5
BOOL CWaveRecSetting::NeedSave()
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
//##ModelId=49B87B7F01D4
int CWaveRecSetting::InitControls()
{
	/*
	//�ٻ���ֵ��ť
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ���ֵ"));

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
	m_btnPrint.SetTooltipText(_T("��ӡ"));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());

	return 0;
}


//##ModelId=49B87B80004E
void CWaveRecSetting::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����ClearSettingConfig()
 ���ܸ�Ҫ�������ֵ������Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7F01E4
BOOL CWaveRecSetting::ClearSettingConfig()
{
	//�����ֵ����
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING * pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		delete pts;
		pts = NULL;
	}
	m_arrSetting.RemoveAll();

	return TRUE;
}

/*************************************************************
 �� �� ����LoadSettingConfig()
 ���ܸ�Ҫ�������ݿ����뱣����ֵ����, �����ڶ�ֵ������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7F0203
BOOL CWaveRecSetting::LoadPTSettingConfig()
{
	WriteLogEx("CWaveRecSetting::LoadPTSettingConfig,��ѯ��ʼ");
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	//���֮ǰ������
	if(!ClearSettingConfig())
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//������ƽ̨����DLL�����ӿ���
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code, zone,103group����
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//��setting_ID��С��������
		
	CString str;
		
	//�ֶ�
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Setting_ID", EX_STTP_DATA_TYPE_INT);

	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);

	//code_name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);

	//vpickList
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "vpicklist", EX_STTP_DATA_TYPE_STRING);

	//unit
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "unit", EX_STTP_DATA_TYPE_STRING);

	//s_precision
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "s_precision", EX_STTP_DATA_TYPE_STRING);

	//datatype
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "datatype", EX_STTP_DATA_TYPE_INT);
		
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	if(g_PTSettingGroup == 1 && m_bCanGroup)
	{
		//������ʾ
		Condition condition5;
		str.Format("103GROUP = %d", m_nGroup);
		pApp->m_DBEngine.SetCondition(sql, condition5, str);
	}

	//��Setting_ID��С����
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //��where����	
		
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadPTSettingConfig, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();

		CString str;
		str.Format("CWaveRecSetting::LoadPTSettingConfig,��ѯ��%d������", nCount);
		WriteLog(str);
		for(int i = 0; i < nCount; i++)
		{
			//������ֵ����
			PT_SETTING * pts = new PT_SETTING;
			//setting_id,NAME,CODE_NAME,103group,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = pMemset->GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset->GetValue(1); //NAME
			pts->CodeName = pMemset->GetValue(2); //Code_Name
			pts->VPickList = pMemset->GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = pMemset->GetValue(4); //unit
			pts->Precision = pMemset->GetValue(5); //s_precision
			str = pMemset->GetValue(6); //datatype
			pts->DataType = atoi(str);

			//��ֵ��
			if(g_PTSettingGroup == 1 && m_bCanGroup)
			{
				pts->Group = m_nGroup;
			}

			m_arrSetting.Add(pts);

			//���һ�׼ֵ,¼�����ݲ�֧��
			pts->OrderValue = "";
		//	LoadOrderValue(pts);
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadPTSettingConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
		
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//����Value����
	FillListData();

	WriteLogEx("CWaveRecSetting::LoadPTSettingConfig,��ѯ��ʼ");

	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTGroupConfig()
 ���ܸ�Ҫ�����뱣����ֵ��������Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����Ҫ����Ķ�ֵ��
**************************************************************/
//##ModelId=49B87B7F0213
BOOL CWaveRecSetting::LoadPTGroupConfig()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLogEx("CWaveRecSetting::LoadPTGroupConfig,��ѯ��ʼ");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����station_id, 103addr, cpu_code, ����103group,name
	
	CString str;	
	//�ֶ�
	//id
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);
	
	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//station_id
	Condition condition2;
	str.Format("STATION_ID = '%s'", m_pObj->m_sStationID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	//103addr
	Condition condition3;
	str.Format("103ADDR = %d", m_pObj->m_n103Addr);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
	
	//cpu
	Condition condition4;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_103GROUP_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CWaveRecSetting::LoadPTGroupConfig, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();

		CString str;
		str.Format("CWaveRecSetting::LoadPTGroupConfig,��ѯ��%d������", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			CString str = pMemset->GetValue((UINT)0); //103group
			int nGroup = atoi(str);
			str = pMemset->GetValue(1); //name
			SetGroupName(nGroup, str);
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	WriteLogEx("CWaveRecSetting::LoadPTGroupConfig,��ѯ��������");
	
	return bReturn;
}

/*************************************************************
 �� �� ����LoadOrderValue()
 ���ܸ�Ҫ�����붨ֵ��׼ֵ��Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ����Ķ�ֵ
**************************************************************/
//##ModelId=49B87B7F0232
BOOL CWaveRecSetting::LoadOrderValue(PT_SETTING* pts)
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//������ƽ̨����DLL�����ӿ���
	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	//����PT_ID, CPU, Zone, ID(��ֵID)����Value(��׼ֵ),��Time�Ӵ�С����,ȡ����һ��
		
	CString str;	
	//�ֶ�
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "value", EX_STTP_DATA_TYPE_STRING);
		
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
		
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	//id
	Condition condition4;
	str.Format("ID = %s", pts->ID);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);

	//��ʱ������
	Condition condition5;
	str = "order by TIME DESC";
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ST_ORDERSETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadOrderValue, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str = "";
		if(nCount > 0)
		{
			//ȡ��һ��
			str = pMemset->GetValue((UINT)0);
			pts->OrderValue = str;
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadOrderValue,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����AddSettingValue()
 ���ܸ�Ҫ������ĳ�ֵ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   CString		��ֵID
		   param2   CString		��ֵֵ
		   param3   int			Ҫ�������
**************************************************************/
//##ModelId=49B87B7F0251
BOOL CWaveRecSetting::AddSettingValue( CString ID, CString VALUE, int iCol )
{
	//���ҵ�Setting_ID==ID������
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	LVFINDINFO  fi;
	fi.flags = LVFI_STRING;
	fi.psz = strID;
	
	//���Ҵ�ֵ�ڵڼ���
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return FALSE;
	
	//����ֵ
	PT_SETTING *pst = m_arrSetting.GetAt(nItem);
	pst->Value = VALUE;
	
	//����ListCtrl��Value������
	bReturn = m_List.SetItemText(nItem, iCol/*������Ϊ����ֵ*/, pst->Value);
	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTHistorySettingData()
 ���ܸ�Ҫ�����뱣����ʷֵ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7F0261
BOOL CWaveRecSetting::LoadPTHistorySettingData()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	//�����ǰ��ӵ���
	CHeaderCtrl *pHC = m_List.GetHeaderCtrl();
	int iColCount = pHC->GetItemCount();
	for (int z = 0; z < iColCount - m_nNewValueIndex; z ++)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CStringArray arrTime;
	arrTime.RemoveAll();
	//������ƽ̨����DLL�����ӿ���
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
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	//��ʱ���С����
	Condition condition3;
	str.Format("order by CURTIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//��where
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadPTHistorySettingData, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		int iItem = 0;
		for(int i = 0; i < nCount; i++)
		{
			if(iItem >= g_PTSettingValues)
				break;

			CString str;
			str = pMemset->GetValue((UINT)0);
			arrTime.InsertAt(0, str);
			iItem++;
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadPTHistorySettingData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//���ÿһ��
	//�����ݿ����ÿ��Time��Ӧ�Ķ�ֵ��
	for (int i = 0; i < arrTime.GetSize(); i ++)
	{
		CString strTimeDisplay;
		CString strTime = arrTime.GetAt(i);
		if(strTime.ReverseFind('.') != -1)
		{
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		}
		else
		{
			strTimeDisplay = strTime;
		}
		
		//��ÿ��Time���һ��
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
		
		if(-1 == Index)
			continue;
		//�����ݿ����ÿ��Time��Ӧ�Ŀ�������
		//������ƽ̨����DLL�����ӿ���
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//ȡ�����ݿ�������ʷ���ݵ�Time�б�
		CString str;
			
		//�ֶ�
		//setting_id
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "setting_id", EX_STTP_DATA_TYPE_INT);

		//VALUE
		Field Field2;
		pApp->m_DBEngine.SetField(sql, Field2, "Value", EX_STTP_DATA_TYPE_STRING);
			
		//����
		//PT_ID
		Condition condition1;
		str.Format("PT_ID = '%s'", m_pObj->m_sID);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);

		//cpu_code
		Condition condition2;
		str.Format("CPU_CODE = %s", m_sCPU);
		pApp->m_DBEngine.SetCondition(sql, condition2, str);

		//CURTIME
		Condition condition4;
		str.Format("CURTIME = '%s'", strTime);
		pApp->m_DBEngine.SetCondition(sql, condition4, str);
				
		CMemSet* pMemset;
		pMemset = new CMemSet;
			
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, pMemset);
		}
		catch (...)
		{
			WriteLog("CWaveRecSetting::LoadPTHistorySettingData, ��ѯʧ��");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			int nItem = 0;
			for(int i = 0; i < nCount; i++)
			{
				CString strID, strValue;
				strID = pMemset->GetValue((UINT)0);
				strValue = pMemset->GetValue(1);

				BOOL bAddReturn = AddSettingValue(strID, strValue, Index);
				if(bAddReturn)
					nItem++;
				pMemset->MoveNext();
			}
			if(0 == nItem)
				m_List.DeleteColumn(Index);
		}
		else
		{
			CString str;
			str.Format("CWaveRecSetting::LoadPTHistorySettingData,��ѯʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
			
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	}
	return TRUE;
}

//##ModelId=49B87B80006D
void CWaveRecSetting::OnBtnWrsetCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ֵ��ȡ���ٻ�
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	if(m_nCurrentStatus == 0)
	{
		//����״̬, �����ť��ʾҪ�ٻ���ֵ
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20015����
		//��ֵ������0
		if(!pApp->m_SttpEngine.BuildDataFor20015PTSetting(sttpData, m_pObj->m_sID, atoi(m_sCPU), 0, m_nSrc))
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
				str.Format("����%s����¼������ֵ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
				WriteLog(str);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����%s����¼������ֵ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
				WriteLog(str);
			}
			AfxMessageBox("���ͱ���ʧ��");
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ���ֵ
			m_nCurrentStatus = 1;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nWRSettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			CString str;
			str.Format("%s��ʼ�ٻ���ֵ�����ó�ʱʱ��Ϊ%d", m_pObj->m_sName, nTimeOut);
			WriteLog(str);

			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ֵ
		//ȡ���ٻ�
		CancelCall();
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¿ؼ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7F0282
void CWaveRecSetting::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		SetDlgItemText(IDC_BTN_WRSET_CALL, "�ٻ���ֵ");
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_WRSET_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRSET_STYLE) ->EnableWindow(TRUE);
		
		//������
		GetDlgItem(IDC_CMB_WRSET_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_WRSET_SRC) ->EnableWindow(TRUE);
		//ֹͣ����
		m_gif.Stop();
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		SetDlgItemText(IDC_BTN_WRSET_CALL, CANCEL_CALL);
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_WRSET_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRSET_STYLE) ->EnableWindow(FALSE);
		
		//������
		GetDlgItem(IDC_CMB_WRSET_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_WRSET_SRC) ->EnableWindow(FALSE);
		//��ʼ����
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("��ʱ%d��", m_nOPTime);
		SetDlgItemText(IDC_STATIC_TIME_WRSETTING, str);
	}
}

/*************************************************************
 �� �� ����OnSTTP20016()
 ���ܸ�Ҫ����Ӧ��ֵ����֪ͨ(20016)
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8000CC
void CWaveRecSetting::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���ֵ��״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20016����
	if(pSttpData->sttp_head.uMsgID != 20016)
	{
		//������
		CString str;
		str.Format("CWaveRecSetting::OnSTTP20016 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CWaveRecSetting::OnSTTP20016 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str);
		return;
	}

	//�رն�ʱ��
//	KillTimer(m_nTimer);
	
	//�ж��ǵ�֡���Ƕ�֡
	if(pSttpData->sttp_head.uMsgType < 2)
	{
		//���CPU
		int nCpu = pSttpData->sttp_body.nCpu;
		if(nCpu != atoi(m_sCPU))
		{
			//���Ǳ�CPU�ı���
			CString str;
			str.Format("CWaveRecSetting::OnSTTP20016 �յ�����CPU����, CPUΪ%d", nCpu);
			WriteLog(str);
			return;
		}

		//��֡,ֱ�Ӵ���
		//�رն�ʱ��
		KillTimer(m_nTimer);
		
		//������
		if(!LoadDataFromSTTP(pSttpData))
		{
			WriteLog("CPTSetting::OnSTTP20016 ������ʧ��");
		}
		
		//�������,�ı�״̬�Ͱ�ť����
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
	}
	else if(pSttpData->sttp_head.uMsgType >= 2)
	{
		//��֡
		m_nFrameCount = pSttpData->sttp_head.uMsgType; //��֡��
		//��ǰ֡���
		int nFrame = pSttpData->sttp_head.uMsgLengthType;
		//�жϺϷ���
		if(nFrame > m_nFrameCount)
		{
			//�������,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
			return;
		}
		//�жϵ�ǰ֡�Ƿ����յ���
		BOOL bFound = FALSE;
		for(int i = 0; i < m_arrFrame.GetSize(); i++)
		{
			int nTempFrame = m_arrFrame.GetAt(i);
			if(nTempFrame == nFrame)
			{
				//�Ѵ���
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			//�Ѵ���, ������˱���
			CString str;
			str.Format("�յ���ֵ��֡����, ��֡��%d, ��ǰ�ǵ�%d֡, ֮ǰ���յ���,������", m_nFrameCount, nFrame);
			WriteLog(str);
			return;
		}
		else
		{
			//֮ǰ������,����
			CString str;
			str.Format("�յ���ֵ��֡����, ��֡��%d, ��ǰ�ǵ�%d֡", m_nFrameCount, nFrame);
			WriteLog(str);
			//�������
			m_arrFrame.Add(nFrame);
			//��������
			TranslateMultiFrame(pSttpData);
		}
	}
}

/*************************************************************
 �� �� ����LoadDataFromSTTP()
 ���ܸ�Ҫ: ��STTP�����ж�ȡ����
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
**************************************************************/
//##ModelId=49B87B7F0280
BOOL CWaveRecSetting::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//������ֵ����
	try
	{
		//ʱ��,ȡ��ǰϵͳʱ��
		CTime tmTime = CTime::GetCurrentTime();
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//�¼�һ��
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
		//��ȡÿ��ֵ,���뵽�б�
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddSettingValue(id, Value, Index);
		}
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadDataFromSTTP, ����", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 �� �� ����ReSetGroupTab()
 ���ܸ�Ҫ���������÷���TAB��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F0290
void CWaveRecSetting::ReSetGroupTab()
{
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_WRSET);
	if(pTab == NULL)
		return;
	//���ԭ��������
	pTab->DeleteAllItems();
	
	if(g_PTSettingGroup != 1)
		return; //��������ʾ, ������
	
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * ptg = (PT_GROUP*)m_arrGroup.GetAt(i);
		CString str; //��ʽ: ���-����
		str.Format("%d-%s", ptg->Group, ptg->Name);
		pTab->InsertItem(i, str);
	}
	
	//����Ĭ��
	if(pTab->GetItemCount() > 0)
	{
		pTab->SetCurSel(0);
		
		//���浱ǰѡ��
		SaveSelGroup(pTab);
	}
}

/*************************************************************
 �� �� ����SaveSelGroup()
 ���ܸ�Ҫ������TAB����ǰѡ�����Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F029F
void CWaveRecSetting::SaveSelGroup( CTabCtrl* pTab )
{
	int nIndex = pTab->GetCurSel();
	TCITEM tc;
	char temp[256];
	for(int j = 0; j < 256; j++)
	{
		temp[j] = '\0';
	}
	tc.mask = TCIF_TEXT;
	tc.pszText = temp;
	tc.cchTextMax = 256;
	pTab->GetItem(nIndex, &tc);
	
	CString strGroup = tc.pszText;
	int pos = strGroup.Find('-', 0);
	if(pos != -1)
	{
		strGroup = strGroup.Left(pos);
	}
	m_nGroup = atoi(strGroup);
}

/*************************************************************
 �� �� ����ReFillCPU()
 ���ܸ�Ҫ���������CPU������
 �� �� ֵ: void
 ��    ����param1	����TAB��
		   Param2
**************************************************************/
//##ModelId=49B87B7F02AF
void CWaveRecSetting::ReFillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_CPU);
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
		pCmb->SetCurSel(0);
		//���浱ǰѡ��
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 �� �� ����SaveSelCPU()
 ���ܸ�Ҫ�����浱ǰѡ���CPU����Ϣ
 �� �� ֵ: void
 ��    ����param1	CPU������
		   Param2
**************************************************************/
//##ModelId=49B87B7F02BF
void CWaveRecSetting::SaveSelCPU( CComboBox* pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return;
	CString strCPU;
	pCmb->GetLBText(nIndex, strCPU);
	//CPU��ʾ�ĸ�ʽΪ: CPU��-CPU����
	int pos = strCPU.Find('-', 0);
	if(pos != -1)
	{
		strCPU = strCPU.Left(pos);
	}
	m_sCPU = strCPU;
}

//##ModelId=49B87B80007D
void CWaveRecSetting::OnSelchangeTabWrset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//����ѡ��ֵ
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_WRSET);
	if(pTab == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	SaveSelGroup(pTab);
	
	//������������
	LoadPTSettingConfig();
	LoadPTHistorySettingData();
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();

	*pResult = 0;
}

//##ModelId=49B87B80008C
void CWaveRecSetting::OnSelchangeCmbWrsetCpu() 
{
	// TODO: Add your control notification handler code here
	//ȡ�õ�ǰֵ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_CPU);
	if(pCmb == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	SaveSelCPU(pCmb);
	
	//�������ú���ʷֵ
	if(g_PTSettingGroup == 1)
	{
		//������ʾ,������CPU�й�
		LoadSettingGroup();
		ReSetGroupTab();
	}
	LoadPTSettingConfig();
	LoadPTHistorySettingData();
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();
}

//##ModelId=49B87B80009C
void CWaveRecSetting::OnSelchangeCmbWrsetSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_SRC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	if(nIndex == 0 || nIndex == 1)
	{
		m_nSrc = nIndex;
	}
}

/*************************************************************
 �� �� ����OnWRFrameOpen()
 ���ܸ�Ҫ����Ӧ¼��������ҳ�����Ϣ, ˢ�½���ֵ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B8000DB
void CWaveRecSetting::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	
	int nIndex = (int)wParam;

	m_bLoadData = FALSE;
	if(nIndex == WR_PROP_SETTING)
	{
		ReFillAll();
	}

	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_WRSETTING, "");
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ����ʧ�ܱ���(20069)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8000FA
void CWaveRecSetting::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ���ֵ״̬,��������
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
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���ֵ
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
		}
	}
	else
	{
		//����ʧ��
		//�������,�ı�״̬�Ͱ�ť����
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���ֵ
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
				
			CString str;
			str.Format("¼���� %s �ٻ���ֵʧ��!", m_pObj->m_sID);
			WriteLog(str);
			AfxMessageBox("�ٻ���ֵʧ��!");
		}
	}
	
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧװ�ò�֧�ֹ��ܱ���(20125)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B800109
void CWaveRecSetting::OnSTTP20125( WPARAM wParam, LPARAM lParam )
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

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	//װ�ò�֧��
	//�������,�ı�״̬�Ͱ�ť����
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ֵ
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
		AfxMessageBox("��װ�ò�֧���ٻ���ֵ!");
	}
}

//##ModelId=49B87B80009E
void CWaveRecSetting::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//�ı�״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//��ʾ
		AfxMessageBox("¼�����ٻ���ֵ��ʱ!");
	}
	
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("��ʱ%d��", m_nOPTime);
		SetDlgItemText(IDC_STATIC_TIME_WRSETTING, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����ClearMultiFrame()
 ���ܸ�Ҫ���������֡����ʱ�Ļ�������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02CE
void CWaveRecSetting::ClearMultiFrame()
{
	//��֡��,��ʱ��Ҫ����֡�����ж��Ƿ����ڽ��ն�֡����
	m_nFrameCount = 0;
	//֡
	m_arrFrame.RemoveAll();
	//��ֵ����
	for(int i = 0; i < m_arrNewSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(i);
		delete pts;
	}
	m_arrNewSetting.RemoveAll();
}

/*************************************************************
 �� �� ����TranslateMultiFrame()
 ���ܸ�Ҫ��������֡�Ķ�ֵ����, �����ݱ�����m_arrNewSetting��, ��ȫ��ͳһ����
 �� �� ֵ: void
 ��    ����param1	��ǰ֡��������
		   Param2
**************************************************************/
//##ModelId=49B87B7F02CF
void CWaveRecSetting::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	
	if(pSttpData == NULL)
		return;
	
	//������ֵ����
	try
	{
		//���CPU
		int nCpu = pSttpData->sttp_body.nCpu;
		if(nCpu != atoi(m_sCPU))
		{
			//���Ǳ�CPU�ı���
			CString str;
			str.Format("CWaveRecSetting::OnSTTP20016 �յ�����CPU����, CPUΪ%d", nCpu);
			WriteLog(str);
		}
		else
		{
			//��ȡÿ��ֵ,���뵽�¶�ֵ����
			int nCount = pSttpData->sttp_body.variant_member.value_data.size();
			for(int i = 0; i < nCount; i++)
			{
				CString id;
				id.Format("%d", pSttpData->sttp_body.variant_member.value_data[i].id);
				CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
				Value = ComplexExchange(Value);
				//��������
				PT_SETTING* pts = new PT_SETTING;
				pts->ID = id;
				pts->Value = Value;
				m_arrNewSetting.Add(pts);
			}
		}
				
		//�ж��Ƿ�����ȡ��,����������Ԫ����=��֡������Ϊ����ȡ��
		if(m_arrFrame.GetSize() == m_nFrameCount)
		{
			//��ȡ���,�رն�ʱ��
			KillTimer(m_nTimer);
			//ʱ��,ȡ��ǰϵͳʱ��
			CTime tmTime = CTime::GetCurrentTime();
			CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
			//�¼�һ��
			int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
			//���뵽list��ʾ
			for(int j = 0; j < m_arrNewSetting.GetSize(); j++)
			{
				PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(j);
				AddSettingValue(pts->ID, pts->Value, Index);
			}
			//�����������
			ClearMultiFrame();
			//�������,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, ����", 3);
		return;
	}
}

/*************************************************************
 �� �� ����OnWRFrameClose()
 ���ܸ�Ҫ����Ӧ���ڹر���Ϣ,�˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B80011C
void CWaveRecSetting::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7F02FF
BOOL CWaveRecSetting::ReFillAll()
{
	m_bLoadData = TRUE;
	//�������ú���ʷֵ
	//1.��䶨ֵ��Դ������
	CComboBox * pCMB = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_SRC);
	if(pCMB == NULL)
		return FALSE;
	pCMB->ResetContent();
	pCMB->AddString("¼����װ��");
	pCMB->AddString("��վ���ݿ�");
	pCMB->SetCurSel(0);
	m_nSrc = 0;
	//2.���CPU������
	ReFillCPU();
	
	if(g_PTSettingGroup == 1)
	{
		//������ʾ
		if(!LoadSettingGroup())
			return FALSE;
		ReSetGroupTab();
	}
	else 
	{
		m_bCanGroup = FALSE;
		ClearSettingGroup();
		ReSetGroupTab();
	}
	if(!LoadPTSettingConfig())
		return FALSE;
	/*
	if(!LoadPTHistorySettingData())
		return FALSE;
	*/
	return TRUE;
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02DE
void CWaveRecSetting::CancelCall()
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
		//�����ٻ���ֵ

		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20015, m_pObj->m_sID, atoi(m_sCPU));
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B7F02DF
BOOL CWaveRecSetting::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20015 && m_nCurrentStatus == 1)
	{
		//�ٻ���ֵʧ��
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02EE
void CWaveRecSetting::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B8000AB
BOOL CWaveRecSetting::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_WRSET);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����ClearSettingGroup()
 ���ܸ�Ҫ�������ֵ������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02EF
BOOL CWaveRecSetting::ClearSettingGroup()
{
	//�����ֵ������
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		delete pg;
		pg = NULL;
	}
	m_arrGroup.RemoveAll();
	return TRUE;
}

/*************************************************************
 �� �� ����LoadSettingGroup()
 ���ܸ�Ҫ�����Ҷ�ֵ��
 �� �� ֵ: ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02F0
BOOL CWaveRecSetting::LoadSettingGroup()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLogEx("CWaveRecSetting::LoadSettingGroup, ��ʼ��ѯ");
	
	ClearSettingGroup();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����PT_ID, cpu_code,����103group
	
	CString str;

	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
	
	//�ֶ�
	//103group
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);
	
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CWaveRecSetting::LoadSettingGroup, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount <= 0)
		{
			//���Ҳ�����װ�õķ���
			m_bCanGroup = FALSE;
		}
		else
		{
			m_bCanGroup = TRUE;
		}
		CString strLog;
		strLog.Format("CWaveRecSetting::LoadSettingGroup, ��ѯ��%d������", nCount);
		WriteLogEx(strLog);
		for(int i = 0; i < nCount; i++)
		{
			//������ֵ�����
			PT_GROUP * group = new PT_GROUP;
			CString str = pMemset->GetValue((UINT)0); //103group
			
			group->Group = atoi(str);
			group->Name = ""; //name
			
			m_arrGroup.Add(group);
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	if(m_bCanGroup)
		LoadPTGroupConfig();
	WriteLogEx("CWaveRecSetting::LoadSettingGroup, ��ѯ��������");
	return bReturn;
}

/*************************************************************
 �� �� ����SetGroupName()
 ���ܸ�Ҫ�����ö�ֵ�������
 �� �� ֵ: 
 ��    ����param1	��ֵ��ID
		   Param2	��ֵ������
**************************************************************/
//##ModelId=49B87B7F02F1
void CWaveRecSetting::SetGroupName( int nGroupID, CString sName )
{
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			pg->Name = sName;
			break;
		}
	}
}

//##ModelId=49B87B800128
void CWaveRecSetting::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	//�ж�����/��ʾ ����TAB��
	if(g_PTSettingGroup != 1)
	{
		//����
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_HIDE);
	}
	else
	{
		//��ʾ
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_SHOW);
	}
	OnSize(0, 0, 0);
	ReFillAll();
}

//##ModelId=49B87B7F037A
void CWaveRecSetting::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!bActivate)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = "δ֪��վ";
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, "��ֵ");
	}
	else
 		pApp->SetAppTile("δ֪װ��", "��ֵ");

	if(!m_bLoadData)
	{
		m_bLoadData = TRUE;
		//�ڴ�������ݶ����еȴ�����
		COXSplashWnd wndWait;
		wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
		wndWait.Show(SPLASH_NOTIMER);
		//δ����, ��������
		ReFillAll();
		wndWait.Hide();
	}
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B7F030D
CString CWaveRecSetting::TranslateSingleKey( CString sKey )
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
//##ModelId=49B87B7F030F
BOOL CWaveRecSetting::InitCOM()
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
//##ModelId=49B87B7F0310
void CWaveRecSetting::ReleaseCOM()
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
 �� �� ����DoPrintWRSetting()
 ���ܸ�Ҫ����ӡ¼������ֵ
 �� �� ֵ: ��ӡģ���ļ�
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F0311
void CWaveRecSetting::DoPrintWRSetting( CString strPath )
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
			sLog.Format("�򿪴�ӡģ���ļ�ʧ��.\r\n%s", strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInWRSetting);
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

//##ModelId=49B87B8000BB
void CWaveRecSetting::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintWRSetting(WRSETTING_NEW1_TP);
}

//##ModelId=49B87B8000BD
void CWaveRecSetting::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintWRSetting(WRSETTING_NEW2_TP);
}

//##ModelId=49B87B8000BF
void CWaveRecSetting::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintWRSetting(WRSETTING_NEW3_TP);
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������LIST������ʾ���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8000C1
void CWaveRecSetting::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

				//����������
				if(m_arrSetting.GetSize() > nItem)
				{
					PT_SETTING *pts = m_arrSetting[nItem];
					if(pts != NULL)
					{
						if (pts->DataType == 2)
						{
							lplvcd->clrTextBk = RGB(220, 220, 220);
						}
					}
				}
				
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
				
				*pResult = CDRF_NEWFONT;
				break;
				/*

				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//���¼�����(I, II, III)��������ɫ
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
				*/

			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B800119
void CWaveRecSetting::OnSTTP20157( WPARAM wParam, LPARAM lParam )
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
		//�����ٻ���ֵ				
		int nTimeOut = pData->m_nWRSettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		CString str;
		str.Format("%s�ٻ���ֵ���յ�20157���ģ��������ó�ʱʱ��Ϊ%d", m_pObj->m_sName, nTimeOut);
		WriteLog(str);
	}
}

void CWaveRecSetting::OnDoubleclickedBtnWrsetCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecSetting::OnDblclkListWrset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	//ȡ�ñ�������к�
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if (nItem == -1)
		return;
	
	
	PT_SETTING *pts = NULL;
	pts = (PT_SETTING *)m_List.GetItemData(nItem);
	pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
	if (pts->DataType == 2 || pts->DataType == 4)
	{
		//��ʼ���Ի���
		CDlgSettingDetail dlg;
		dlg.m_pts = pts;
		dlg.DoModal();
	}
	
	*pResult = 0;
}
