// WaveRecOSC.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecOSC.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           ����������λ*/
int g_iWROSCAsc;

UINT WROSCLoad(LPVOID pParam)
{
	CWaveRecOSC* wrOSC = (CWaveRecOSC*)pParam;
	if(wrOSC == NULL)
		return 0;
	//��ȡ����
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//�����˳��߳�,�˳�
		if(wrOSC->m_bExitThread)
			return 0;
		//�������Դ���
		if(nCount < 0)
			return 0;
		//��ȡ���, �˳�
		if(bLoad)
		{
			//log
			WriteLog("¼��������--¼��ҳ��, ��ȡ���ݽ���, �����˳��߳�!");
			return 0;
		}
		if(wrOSC->ReFillAll())
		{
			bLoad = TRUE;
		}
		else
		{
			//��ȡʧ��
			CString str;
			str.Format("wrOSC, ʧ��%d��", (4 - nCount));
			WriteLog(str);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC

IMPLEMENT_DYNCREATE(CWaveRecOSC, CViewBase)

//##ModelId=49B87B800169
CWaveRecOSC::CWaveRecOSC()
	: CViewBase(CWaveRecOSC::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecOSC)
	m_sDirPath = _T("");
	m_bOnlyCFG = FALSE;
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_nMaxRow = 100;
	m_arrOSC.RemoveAll();
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
}

//##ModelId=49B87B800207
CWaveRecOSC::~CWaveRecOSC()
{
	ClearOSCMsg();
}

//##ModelId=49B87B8001F4
void CWaveRecOSC::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecOSC)
	DDX_Control(pDX, IDC_LIST_WROSC, m_List);
	DDX_Control(pDX, IDC_BTN_WROSC_RF, m_btnRefur);
	DDX_Control(pDX, IDC_BTN_WROSC_OPEN, m_btnOpen);
	DDX_Text(pDX, IDC_STATIC_WROSC_DIR, m_sDirPath);
	DDX_Check(pDX, IDC_CHECK_WROSC_SHOW, m_bOnlyCFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecOSC, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecOSC)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_WROSC, OnClickListWrosc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WROSC, OnItemchangedListWrosc)
	ON_BN_CLICKED(IDC_CHECK_WROSC_SHOW, OnCheckWroscShow)
	ON_BN_CLICKED(IDC_BTN_WROSC_RF, OnBtnWroscRf)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_WROSC, OnColumnclickListWrosc)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_WROSC_OPEN, OnBtnWroscOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WROSC, OnDblclkListWrosc)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B800213
void CWaveRecOSC::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B800215
void CWaveRecOSC::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC message handlers

//##ModelId=49B87B8001E8
void CWaveRecOSC::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	//�ļ�·���ı�
	RecordRate(IDC_STATIC_WROSC_DIR, 1, left_client, null_effect, null_effect, bottom_client);
	//ˢ�°�ť
	RecordRate(IDC_BTN_WROSC_RF, 0, null_effect, top_client, right_client);
	//��ʾ���е�ѡ��
	RecordRate(IDC_CHECK_WROSC_SHOW, 0, null_effect, top_client, right_client);
	//�鿴���ΰ�ť
	RecordRate(IDC_BTN_WROSC_OPEN, 0, null_effect, null_effect, right_client, bottom_client);
	CViewBase::OnInitialUpdate();

	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();
	
	// TODO: Add your specialized code here and/or call the base class
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
//##ModelId=49B87B8001AB
int CWaveRecOSC::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8001B5
int CWaveRecOSC::InitListStyle()
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
		char* arColumn[3]={"����","��С", "�޸�ʱ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 250; // ����
				break;
			case 1://
				lvCol.cx = 160; // ��С
				break;
			case 2://
				lvCol.cx = 200; // �޸�ʱ��
				break;
			}
			lvCol.pszText=arColumn[nCol];
			m_List.InsertColumn(nCol,&lvCol);
		}
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
//##ModelId=49B87B8001C5
BOOL CWaveRecOSC::LoadListConfig()
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
			WriteLog("WaveRecOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("WaveRecOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//WaveRecOSCConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�WaveRecOSCConfig");
				WriteLog("WaveRecOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�WaveRecOSCConfig�µ�����");
					WriteLog("WaveRecOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

//##ModelId=49B87B8001C6
BOOL CWaveRecOSC::SaveListConfig()
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
			WriteLog("WaveRecOSC::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("WaveRecOSC::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//WaveRecOSCConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�WaveRecOSCConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("WaveRecOSCConfig");
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
						WriteLog("δ�ҵ�WaveRecOSCConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���WaveRecOSCConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B8001B6
int CWaveRecOSC::FillListData()
{
	CString sPath = GetOSCFileDir();
	//�����
	m_List.DeleteAllItems();
	ClearOSCMsg();
	SearchOSCFiles(sPath);
	
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//���ListCtrl����
	PT_OSC *pts = NULL;
	for(int i = 0; i < m_arrOSC.GetSize(); i ++)
	{
		pts = m_arrOSC.GetAt(i);
		if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, pts->FileName, 0, 0, 0, i) == -1)
			continue;
		CString str;
		str.Format("%.0f Byte", pts->FileSize);
		m_List.SetItemText(i, 1, str);
		str = pts->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		m_List.SetItemText(i, 2, str);
		m_List.SetItemData(i, (DWORD)pts);
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8001C7
BOOL CWaveRecOSC::NeedSave()
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
//##ModelId=49B87B8001D4
int CWaveRecOSC::InitControls()
{
	/*
	//�򿪰�ť
	m_btnOpen.SetThemeHelper(&m_ThemeHelper);
//	m_btnOpen.SetIcon(IDI_WAVE);
	m_btnOpen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOpen.SetTooltipText(_T("�쿴ѡ���ļ��Ĳ���"));

	//ˢ�°�ť
	m_btnRefur.SetThemeHelper(&m_ThemeHelper);
//	m_btnRefur.SetIcon(IDI_REFRESH);
	m_btnRefur.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRefur.SetTooltipText(_T("ˢ�²������ļ�"));
*/
	return 0;
}

//##ModelId=49B87B800222
void CWaveRecOSC::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_WROSC, left_client, IDC_BTN_WROSC_RF, right_client, IDC_BTN_WROSC_OPEN);
}

//##ModelId=49B87B800232
void CWaveRecOSC::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����GetOSCFileDir()
 ���ܸ�Ҫ���õ�¼���ļ�¼��Ŀ¼
 �� �� ֵ: ¼���ļ����Ŀ¼
**************************************************************/
//##ModelId=49B87B8001D5
CString CWaveRecOSC::GetOSCFileDir()
{
	if(m_pObj == NULL)
		return "";
	//�õ���ַ
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//Ŀ¼���'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//��֯·��
	CString strFullPath;
	strFullPath = strDownDir + m_pObj->m_sStationID + "\\¼����\\" + m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";
	return strFullPath;
}

/*************************************************************
 �� �� ����SearchOSCFiles()
 ���ܸ�Ҫ������Ŀ¼������¼���ļ�
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ŀ¼·��
**************************************************************/
//##ModelId=49B87B8001D6
BOOL CWaveRecOSC::SearchOSCFiles(CString& path)
{
	CFileFind finder; //�ļ�������
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //�����ļ�
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // ��Ŀ¼,�������
		{
			CString str = finder.GetFilePath();
			SearchOSCFiles(str);
		}
		else //���ļ�
		{
			PT_OSC * osc = new PT_OSC;
			osc->FileName = finder.GetFileName(); //�ļ���,����׺
			//�õ��ļ���׺
			CString strFileName = osc->FileName;
			int pos = strFileName.ReverseFind('.');
			if(pos != -1)
			{
				CString str = strFileName.Right(strFileName.GetLength() - pos -1);
				osc->FileType = str;
				if(!IsValidOSC(str))
				{
					delete osc;
					osc = NULL;
					continue;
				}
			}
			//�õ��ļ���С
			CString strSize;
			DWORD dSize = finder.GetLength();
			float fSize = float(dSize);
		//	fSize /= 1024;
			osc->FileSize = fSize;
			//����޸�ʱ��
			CTime	tm;
			finder.GetLastWriteTime(tm);
			osc->ModifyTime = tm;
			//Ŀ¼
			CString sDirPath = finder.GetFilePath();
			osc->DirPath = sDirPath.Left(sDirPath.GetLength() - osc->FileName.GetLength());

			m_arrOSC.Add(osc);
		}
	}
	finder.Close();

	return TRUE;
}

/*************************************************************
 �� �� ����IsValidOSC()
 ���ܸ�Ҫ���ж�ĳ��׺���Ƿ�����¼���ļ��ĺ�׺��
 �� �� ֵ: ����TRUE˵����¼���ļ�,FALSE˵������
 ��    ����param1   ָ����׺��
**************************************************************/
//##ModelId=49B87B8001D8
BOOL CWaveRecOSC::IsValidOSC(CString strName)
{
	if(m_bOnlyCFG)
	{
		if(strName != "cfg" && strName != "CFG")
			return FALSE;
	}
	else 
	{
		if(strName != "cfg" && strName != "dat" && strName != "hdr" && strName != "rev" && strName != "reh")
		{
			if(strName != "CFG" && strName != "DAT" && strName != "HDR" && strName != "REV" && strName != "REH")
				return FALSE;
		}
	}
	return TRUE;
}

/*************************************************************
 �� �� ����ClearOSCMsg()
 ���ܸ�Ҫ�����¼���ļ���Ϣ
 �� �� ֵ: ��ճɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8001E5
BOOL CWaveRecOSC::ClearOSCMsg()
{
	for(int i = 0; i < m_arrOSC.GetSize(); i++)
	{
		PT_OSC * osc = (PT_OSC*)m_arrOSC.GetAt(i);
		delete osc;
		osc = NULL;
	}
	m_arrOSC.RemoveAll();

	return TRUE;
}

//##ModelId=49B87B800234
void CWaveRecOSC::OnClickListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//##ModelId=49B87B800238
void CWaveRecOSC::OnItemchangedListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//��ʵ���������ѡ����, ���������ITEMCHANGED��Ϣ, 
		//1��A�����ֱ�����ʧ������2��Aȡ��ѡ�д�����3��B��ѡ�д���
		//Ϊ����ˢ��̫Ƶ��, ����ֻ����ѡ����Ϣ, ���������©��ѡ���˿��е���Ϣ
		//���԰�ѡ����з���NM_CLICK�д���, �����ƶ��������¼����µ�ѡ�񵽿��в�������
		
		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
		if(nSelected < 0)
			return;
		
		//�õ����ж�Ӧ���¼�
		PT_OSC * pts = (PT_OSC*)m_List.GetItemData(nSelected);
		if(pts == NULL)
			return;
		//������ʾֵ
		m_sDirPath = pts->DirPath;
		UpdateData(FALSE);
	}

	*pResult = 0;
}

/*************************************************************
 �� �� ����OnWRFrameOpen()
 ���ܸ�Ҫ����Ӧ¼��������ҳ�����Ϣ, ˢ�½���
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B800255
void CWaveRecOSC::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	int nIndex = (int)wParam;

	//�˳����߳�
// 	EndOwnerThread();
// 	//������ȡ�߳�
// 	if(m_pLoadThread == NULL)
// 	{
// 		m_bExitThread = FALSE;
// 		m_pLoadThread = AfxBeginThread(WROSCLoad, this, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
// 		if(m_pLoadThread != NULL)
// 		{
// 			m_pLoadThread->m_bAutoDelete = FALSE;
// 			m_pLoadThread->ResumeThread();
// 		}
// 	}
// 	else
// 	{
	m_bLoadData = FALSE;
	if(nIndex == WR_PROP_OSC)
	{
		ReFillAll();
	}
// 	}
	
}

//##ModelId=49B87B800245
void CWaveRecOSC::OnCheckWroscShow() 
{
	// TODO: Add your control notification handler code here
	//��������
	UpdateData(TRUE);
	//ɾ������,��ȡ����,�������
	FillListData();
}

//##ModelId=49B87B800247
void CWaveRecOSC::OnBtnWroscRf() 
{
	// TODO: Add your control notification handler code here
	//��������
	UpdateData(TRUE);
	//ɾ������,��ȡ����,�������
	FillListData();
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B8001A6
int CALLBACK CWaveRecOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	PT_OSC* pOSC1 = (PT_OSC*)lParam1;
	PT_OSC* pOSC2 = (PT_OSC*)lParam2;
	if(pOSC1 == NULL || pOSC2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"����","��С", "�޸�ʱ��"
	switch(nCol)
	{
	case 0: //����
		str1 = pOSC1->FileName;
		str2 = pOSC2->FileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //��С
		if(pOSC1->FileSize - pOSC2->FileSize < -0.00001)
			iResult = -1;
		else if(pOSC1->FileSize - pOSC2->FileSize > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 2: //�޸�ʱ��
		str1 = pOSC1->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pOSC2->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iWROSCAsc;
	return iResult;
}

//##ModelId=49B87B800249
void CWaveRecOSC::OnColumnclickListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iWROSCAsc = - g_iWROSCAsc;
	}
	else
	{
		g_iWROSCAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnWRFrameClose()
 ���ܸ�Ҫ����Ӧ���ڹر���Ϣ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B800258
void CWaveRecOSC::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8001E7
BOOL CWaveRecOSC::ReFillAll()
{
	m_bLoadData = TRUE;
	FillListData();
	return TRUE;
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8001E6
void CWaveRecOSC::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B800252
BOOL CWaveRecOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_WROSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8001F7
void CWaveRecOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = "δ֪��վ";
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, "����¼��");
	}
	else
 		pApp->SetAppTile("δ֪װ��", "����¼��");

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

void CWaveRecOSC::OnBtnWroscOpen() 
{
	// TODO: Add your control notification handler code here
	WriteLog("����˲鿴���ΰ�ť, �����ѡ¼��", XJ_LOG_LV3);
	//���������Ч��
	if(m_List.GetSafeHwnd() == NULL)
	{
		WriteLog("CPTOSC::OnBtnPtoscOpen �б�������Ч", XJ_LOG_LV3);
		return;
	}
	//�ж�ѡ��
	if(m_List.GetSelectedCount() < 1)
	{
		AfxMessageBox("�����ѡ��һ����¼���ܽ��д˲�����");
		return;
	}
	//ֻ�򿪵�һ��ѡ����ļ�
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
	{
		WriteLog("CPTOSC::OnBtnPtoscOpen δ�õ�ѡ�м�¼����Ϣ", XJ_LOG_LV3);
		return;
	}
	//ȡ�ö���
	PT_OSC * pts = (PT_OSC*)m_List.GetItemData(nIndex);
	//���ļ�
	WriteLog("��ʼ��¼���ļ�", XJ_LOG_LV3);
	OpenOSCWaveFile(pts);
}

/*************************************************************
 �� �� ����OpenOSCWaveFile()
 ���ܸ�Ҫ���򿪲����ļ�
 �� �� ֵ: 
 ��    ����param1	�����ļ��ṹ
		   Param2
**************************************************************/
void CWaveRecOSC::OpenOSCWaveFile( PT_OSC* pts )
{
	//�ж���Ч��
	if(pts == NULL)
	{
		AfxMessageBox("������¼��δ��������ļ���Ϣ�������޷���ɣ�");
		return;
	}
	if(pts->FileName == "")
	{
		AfxMessageBox("������¼��δ��������ļ���Ϣ�������޷���ɣ�");
		return;
	}
	//�ϲ��ļ���
	CString sFullPath = GetOSCFileDir() + pts->FileName;
	//���ļ�
	OpenWaveFile(sFullPath, GetSafeHwnd());
}


void CWaveRecOSC::OnDblclkListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//�õ����ж�Ӧ�Ķ���
		PT_OSC* pts = (PT_OSC*)m_List.GetItemData(nSelected);
		OpenOSCWaveFile(pts);
	}
	*pResult = 0;
}
