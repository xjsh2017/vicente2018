// PTOSC.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTOSC.h"
#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           ����������λ*/
int g_iPTOSCAsc;

UINT PTOSCLoad(LPVOID pParam)
{
	CPTOSC* ptOSC = (CPTOSC*)pParam;
	if(ptOSC == NULL)
		return 0;
	int nQueryNo = ptOSC->GetCurrentQueryNo();
	//��ȡ����
	if(ptOSC->IsCurrentQueryNo(nQueryNo) && !ptOSC->m_bExitThread)
	{
		ptOSC->ReLoadData(nQueryNo);
		ptOSC->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
	}
	ptOSC->RemoveThreadNode(nQueryNo);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPTOSC

IMPLEMENT_DYNCREATE(CPTOSC, CViewBase)

//##ModelId=49B87B900002
CPTOSC::CPTOSC()
	: CViewBase(CPTOSC::IDD)
{
	//{{AFX_DATA_INIT(CPTOSC)
	m_sDirPath = _T("");
	m_bOnlyCFG = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B9000D0
CPTOSC::~CPTOSC()
{
	m_arrColum.RemoveAll();
	ClearOSCMsg();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B9000BB
void CPTOSC::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTOSC)
	DDX_Control(pDX, IDC_BTN_PTOSC_RF, m_btnRefur);
	DDX_Control(pDX, IDC_BTN_PTOSC_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_LIST_PTOSC, m_List);
	DDX_Text(pDX, IDC_STATIC_PTOSC_DIR, m_sDirPath);
	DDX_Check(pDX, IDC_CHECK_PTOSC_SHOW, m_bOnlyCFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTOSC, CViewBase)
	//{{AFX_MSG_MAP(CPTOSC)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTOSC, OnClickListPtosc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PTOSC, OnItemchangedListPtosc)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTOSC, OnColumnclickListPtosc)
	ON_BN_CLICKED(IDC_CHECK_PTOSC_SHOW, OnCheckPtoscShow)
	ON_BN_CLICKED(IDC_BTN_PTOSC_RF, OnBtnPtoscRf)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTOSC_OPEN, OnBtnPtoscOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTOSC, OnDblclkListPtosc)
	ON_BN_CLICKED(IDC_BTN_PTOSC_COPY, OnBtnPtoscCopy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTOSC, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9000DA
void CPTOSC::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B9000DC
void CPTOSC::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTOSC message handlers

//##ModelId=49B87B9000B0
void CPTOSC::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	//�ļ�·���ı�
	RecordRate(IDC_STATIC_PTOSC_DIR, 1, left_client, top_client);
	//ˢ�°�ť
	RecordRate(IDC_BTN_PTOSC_RF, 0, left_client, top_client);
	//��ʾ���е�ѡ��
	RecordRate(IDC_CHECK_PTOSC_SHOW, 0, left_client, top_client);
	//�鿴���ΰ�ť
	RecordRate(IDC_BTN_PTOSC_OPEN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSC_COPY, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTOSC_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSC_P2, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTOSC_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

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
//##ModelId=49B87B90006F
int CPTOSC::InitListCtrl()
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
//##ModelId=49B87B900070
int CPTOSC::InitListStyle()
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
		//char* arColumn[4]={"���","����","��С", "�޸�ʱ��"};
	
		//�����п�,��С����
		for (int nCol=0; nCol < 4; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 50; //���
				colName = StringFromID( IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 200; // ����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 80; // ��С
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 3://
				lvCol.cx = 200; // �޸�ʱ��
				colName = StringFromID(IDS_COMMON_MODIFYTIME);
				break;
			default:
				lvCol.cx = 50;
				break;
			}
			lvCol.pszText = colName.GetBuffer(1);
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
//##ModelId=49B87B90007E
BOOL CPTOSC::LoadListConfig()
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
			WriteLog("PTOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("PTOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTOSCConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTOSCConfig");
				WriteLog("PTOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTOSCConfig�µ�����");
					WriteLog("PTOSC::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

//##ModelId=49B87B90008C
BOOL CPTOSC::SaveListConfig()
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
				pSelected = pDoc->selectSingleNode("//PTOSCConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTOSCConfig");
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
						WriteLog("δ�ҵ�PTOSCConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���PTOSCConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87B90007D
int CPTOSC::FillListData(int nQueryNo)
{
	WriteLog("CPTOSC::FillListData, ��ʼ�������");
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	//���ListCtrl����
	PT_OSC *pts = NULL;
	for(int i = 0; i < m_arrOSC.GetSize(); i ++)
	{
		if(m_bExitThread)
			break;
		if(!IsCurrentQueryNo(nQueryNo))
			break;

		CString str;
		pts = m_arrOSC.GetAt(i);
		pts->Code = i;
		str.Format("%d", pts->Code);
		//0.���
		if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
			continue;
		//�ļ���
		m_List.SetItemText(i, 1, pts->FileName);
		//2.��С
		str.Format("%.0f Byte", pts->FileSize);
		m_List.SetItemText(i, 2, str);
		//3.ʱ��
		str = pts->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		m_List.SetItemText(i, 3, str);
		m_List.SetItemData(i, (DWORD)pts);
	}

	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	WriteLog("CPTOSC::FillListData, ����������");
	
	return 0;
}

//##ModelId=49B87B90008D
BOOL CPTOSC::NeedSave()
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
//##ModelId=49B87B90008E
int CPTOSC::InitControls()
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

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ�λ��
 �� �� ֵ: ����ֵ˵��
**************************************************************/
/*void CPTOSC::RegulateControlSize()
{	

}*/


//##ModelId=49B87B9000EB
void CPTOSC::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
//	RegulateControlSize();
}

//##ModelId=49B87B9000FA
void CPTOSC::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTOSC, left_client, IDC_BTN_PTOSC_RF, right_client, bottom_client);
}

//##ModelId=49B87B9000FF
void CPTOSC::OnDestroy() 
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
//##ModelId=49B87B90008F
CString CPTOSC::GetOSCFileDir(BOOL bNewPath)
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

	if (m_pObj->m_nType == TYPE_WAVEREC)
	{
		strFullPath += "\\¼����\\";
	}
	else
	{
		strFullPath += "\\����\\";
	}

	strFullPath += m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";

	return strFullPath;
}

/*************************************************************
 �� �� ����SearchOSCFiles()
 ���ܸ�Ҫ������Ŀ¼������¼���ļ�
 �� �� ֵ: �����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ŀ¼·��
**************************************************************/
//##ModelId=49B87B90009C
BOOL CPTOSC::SearchOSCFiles(CString& path, int nQueryNo)
{
	if(!IsCurrentQueryNo(nQueryNo) || m_bExitThread)
		return FALSE;
	CFileFind finder; //�ļ�������
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //�����ļ�
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		if(!IsCurrentQueryNo(nQueryNo) || m_bExitThread)
			break;
		bWorking = finder.FindNextFile();

		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // ��Ŀ¼,�������
		{
			CString str = finder.GetFilePath();
			SearchOSCFiles(str, nQueryNo);
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
			osc->FileSize = fSize;
			//����޸�ʱ��
			CTime	tm;
			finder.GetLastWriteTime(tm);
			osc->ModifyTime = tm;
			//Ŀ¼
			CString sDirPath = finder.GetFilePath();
			osc->DirPath = sDirPath.Left(sDirPath.GetLength() - osc->FileName.GetLength());

			EnterCriticalSection(&m_CriticalOper);
			m_arrOSC.Add(osc);
			LeaveCriticalSection(&m_CriticalOper);
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
//##ModelId=49B87B90009E
BOOL CPTOSC::IsValidOSC(CString strName)
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

//##ModelId=49B87B9000AB
BOOL CPTOSC::ClearOSCMsg()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrOSC.GetSize(); i++)
	{
		PT_OSC * osc = (PT_OSC*)m_arrOSC.GetAt(i);
		delete osc;
		osc = NULL;
	}
	m_arrOSC.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);

	return TRUE;
}

//##ModelId=49B87B900109
void CPTOSC::OnClickListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//##ModelId=49B87B90010D
void CPTOSC::OnItemchangedListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
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
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ, ˢ��ҳ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B90013A
void CPTOSC::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	MYASSERT(pObj);
	if(m_pObj == pObj)
	{
		return;
	}
	m_pObj = pObj;

	int nIndex = (int)wParam;

	//�˳��ղŵ��߳�
// 	EndOwnerThread();
// 	m_bLoadData = FALSE;
// 	if(nIndex == PT_PROP_OSC)
// 	{
// 		ReFillAll();
// 	}

	if(m_pObj != NULL)
		m_sDirPath = GetOSCFileDir(FALSE);
	else
		m_sDirPath = "";
	UpdateData(FALSE);
	
	ReFillAll();
}

/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��
 ��    ����param1	Ҫ�ȽϵĶ���1
		   Param2	Ҫ�ȽϵĶ���2
		   Param3	�Ƚϲ���
**************************************************************/
//##ModelId=49B87B90005E
int CALLBACK CPTOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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
	case 0://���
		iResult = pOSC1->Code - pOSC2->Code;
		break;
	case 1: //����
		str1 = pOSC1->FileName;
		str2 = pOSC2->FileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //��С
		if(pOSC1->FileSize - pOSC2->FileSize < -0.00001)
			iResult = -1;
		else if(pOSC1->FileSize - pOSC2->FileSize > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 3: //�޸�ʱ��
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
	iResult *= g_iPTOSCAsc;
	return iResult;
}

//##ModelId=49B87B90011A
void CPTOSC::OnColumnclickListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTOSCAsc = - g_iPTOSCAsc;
	}
	else
	{
		g_iPTOSCAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B90011E
void CPTOSC::OnCheckPtoscShow() 
{
	// TODO: Add your control notification handler code here
	//��������
	UpdateData(TRUE);
	//ɾ������,��ȡ����,�������
	ReFillAll();
}

//##ModelId=49B87B900128
void CPTOSC::OnBtnPtoscRf() 
{
	// TODO: Add your control notification handler code here
	//��������
	UpdateData(TRUE);
	//ɾ������,��ȡ����,�������
	ReFillAll();
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ����Ϣ���ڹر���Ϣ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSC::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
BOOL CPTOSC::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTOSC_LOADING)->ShowWindow(SW_SHOW);


	m_List.DeleteAllItems();
	ClearOSCMsg();

	StartThread();
	
	return TRUE;
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
void CPTOSC::EndOwnerThread()
{
	WriteLog("��ʼ�˳�����¼��ҳ���ȡ�����߳�", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳�����¼��ҳ���ȡ�����߳����", XJ_LOG_LV3);

}

//##ModelId=49B87B90012A
BOOL CPTOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTOSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B90012D
void CPTOSC::OnBtnPtoscOpen() 
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
		AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
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
 �� �� ����OpenWaveFile()
 ���ܸ�Ҫ���򿪲����ļ�
 �� �� ֵ: �����ļ��ṹ
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9000AC
void CPTOSC::OpenOSCWaveFile( PT_OSC* pts )
{
	//�ж���Ч��
	if(pts == NULL)
	{
		return;
	}
	if(pts->FileName == "")
	{
		return;
	}
	//�ϲ��ļ���
	CString sFullPath = GetOSCFileDir(FALSE) + pts->FileName;
	//���ļ�
	OpenWaveFile(sFullPath, GetSafeHwnd());
}

//##ModelId=49B87B90012F
void CPTOSC::OnDblclkListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
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

//##ModelId=49B87B9000CB
void CPTOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_LOCALFILE));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_LOCALFILE));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPTOSC::OnBtnPtoscCopy() 
{
	//ȡ����ѡ�ļ�
	CStringArray arrSelectFile;
	arrSelectFile.RemoveAll();
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			PT_OSC* pOSC = (PT_OSC*)m_List.GetItemData(nIndex);
			MYASSERT_CONTINUE(pOSC);
			CString strName = pOSC->FileName;
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
	{
		return;
	}

	int total = 0;
	int fail = 0;
	//�����ļ�
	for(i = 0; i < arrSelectFile.GetSize(); i++)
	{
		CFileFind finder; //�ļ�������
		CString strPath;
		strPath = GetOSCFileDir(FALSE);
		if(strPath.Right(1) != "\\")
		{
			strPath += "\\";
		}
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
				CString strLog;
				total++;
				CString strName = finder.GetFileName(); //�ļ���,����׺
				
				//Ŀ��·��
				CString strNewPath = strNewDir+strName;
				//Ŀ¼
				CString strOldPath = finder.GetFilePath();
				
				//����				
				
				if(CopyFile(strOldPath, strNewPath, TRUE))
				{
					strLog.Format("�����ļ��ɹ�,Դ·��:%s, Ŀ��·��:%s", strOldPath, strNewPath);
					WriteLog(strLog);
				}
				else
				{
					DWORD errcode = GetLastError();
					strLog.Format("�����ļ�ʧ��,Դ·��:%s, Ŀ��·��%s, errorcode:%d", strOldPath, strNewPath, errcode);
					WriteLog(strLog);
					fail++;
				}
			}
		}
		finder.Close();
	}

	CString strTip;
	strTip.Format("%s, %s:%d, %s:%d, %s:%d.\r\n%s", StringFromID(IDS_EXPORT_FINISHED), StringFromID(IDS_EVENTCOUNT_TOTAL),
		total, StringFromID(IDS_COMMON_SUCCESS),total-fail, StringFromID(IDS_COMMON_FAILED),fail, StringFromID(IDS_TIP_OPENDIR));
	//����
	if(AfxMessageBox(strTip, MB_OKCANCEL) == IDOK)
	{
		ShellExecute(GetSafeHwnd(), "open", "Explorer.exe", strNewDir, NULL, SW_SHOW); 
	}
}

void CPTOSC::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

void CPTOSC::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTOSCLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("����¼����ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTOSC::GetNextQueryNo()
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

void CPTOSC::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("��ʼ�˳�����¼����ѯ�߳�,��ѯ��:%d", nNo);
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
	
	str.Format("����¼����ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTOSC::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("����¼����ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTOSC::GetFirstNode()
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

BOOL CPTOSC::ReLoadData( int nQueryNo )
{
	CString sPath = GetOSCFileDir(FALSE);
	CString sNewPath = GetOSCFileDir(TRUE);

	CString strLog;
	strLog.Format("CPTOSC::ReLoadData, ��ʼ����, Ŀ¼:%s", sPath);
	WriteLog(strLog);
	SearchOSCFiles(sPath, nQueryNo);
	strLog.Format("CPTOSC::ReLoadData, �������, Ŀ¼:%s", sPath);
	WriteLog(strLog);
	strLog.Format("CPTOSC::ReLoadData, ��ʼ����, Ŀ¼:%s", sNewPath);
	WriteLog(strLog);
	SearchOSCFiles(sNewPath, nQueryNo);
	strLog.Format("CPTOSC::ReLoadData, �������, Ŀ¼:%s", sNewPath);
	WriteLog(strLog);

	return TRUE;
}

void CPTOSC::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
		return;
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTOSC_LOADING)->ShowWindow(SW_HIDE);
}

/****************************************************
Date:2014/1/11  Author:LYH
������:   GetThreadCount	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CPTOSC::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}
