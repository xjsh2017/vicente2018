// BatchDetailView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchDetailView.h"
#include "DlgOCSet.h"
#include "GlobalFunc.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

extern int g_iBatchViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView

IMPLEMENT_DYNCREATE(CBatchDetailView, CViewBase)

//##ModelId=49B87BB103DA
CBatchDetailView::CBatchDetailView()
	: CViewBase(CBatchDetailView::IDD)
{
	//{{AFX_DATA_INIT(CBatchDetailView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();

	g_iBatchViewAsc = -1;
	
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	
	m_nOldSortCol = -1;

}

//##ModelId=49B87BB3009C
CBatchDetailView::~CBatchDetailView()
{
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	TRACE("CBatchDetailView::~CBatchDetailView \n");
}

//##ModelId=49B87BB3005D
void CBatchDetailView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchDetailView)

	DDX_Control(pDX, IDC_LIST_BATCH_DETAIL, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchDetailView, CViewBase)
	//{{AFX_MSG_MAP(CBatchDetailView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BATCH_DETAIL, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BATCH_DETAIL, OnColumnclickListBatchDetail)
	
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	
	ON_MESSAGE(WM_STTP_20154, OnSTTP20154)
	
	ON_MESSAGE(SHOW_RESULT, OnShowResult)
	ON_COMMAND(ID_BATCH_CLOSE, OnBatchClose)
	
	ON_COMMAND(ID_BATCH_EXPORT, OnBatchExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB300DA
void CBatchDetailView::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87BB30119
void CBatchDetailView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView message handlers

//##ModelId=49B87BB3002E
void CBatchDetailView::OnInitialUpdate() 
{
	//��¼�ؼ�λ��

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
	//��ʼ���б�
	InitListCtrl();
	//
}

//##ModelId=49B87BB30157
void CBatchDetailView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_BATCH_DETAIL, left_client, top_client, right_client, bottom_client);
	RegulateControlSize();
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87BB2013B
BOOL CBatchDetailView::LoadListConfig()
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
			WriteLog("BatchDetailView::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("BatchDetailView::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			
			pSelected = pDoc->selectSingleNode("//BatchDetailViewConfig");
			
			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�BatchDetailViewConfig");
				WriteLog("BatchDetailView::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�BatchDetailViewConfig�µ�����");
					WriteLog("BatchDetailView::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() == 25 )
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

//##ModelId=49B87BB20177
BOOL CBatchDetailView::SaveListConfig()
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
			WriteLog("BatchDetailView::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("BatchDetailView::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				
				pSelected = pDoc->selectSingleNode("//BatchDetailViewConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�BatchDetailViewConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					
					pNew = pDoc ->createElement("BatchDetailViewConfig");
					
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
						WriteLog("�ҵ���BatchDetailViewConfig�µ�����, ��ɾ���½������ӽڵ�");
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
//##ModelId=49B87BB201A5
BOOL CBatchDetailView::NeedSave()
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
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB201D4
void CBatchDetailView::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	InitListStyle();
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB20203
int CBatchDetailView::InitListStyle()
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
		char* arColumn[25]={"�����豸", "CPU��", "��ֵ����", "��ֵ���Ž��", "��ֵ���Ž��˵��", 
			"��ֵ", "��ֵ���", "��ֵ���˵��", "������","���������", "���������˵��", 
			"��ѹ��","��ѹ����", "��ѹ����˵��", "ģ����","ģ�������", "ģ�������˵��",
			"¼���ļ�","¼���ļ����","¼���ļ����˵��","��ʷ�¼�","��ʷ�¼����","��ʷ�¼����˵��",
			"��վ��", "һ���豸"};
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 100; //�п�50����
				break;
			case 1://
				lvCol.cx = 100; //�п�60����
				break;
			case 2://
				lvCol.cx = 100; //�п�60����
				break;
			default:
				lvCol.cx=100;//�п�100����
			}
			lvCol.pszText=arColumn[nCol];
			m_List.InsertColumn(nCol,&lvCol);
		}
	}

	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);

	return 0;
}

//##ModelId=49B87BB30196
void CBatchDetailView::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnBatchFrameOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ, ˢ����ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB302E4
void CBatchDetailView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//LPARAM ��ʾ��ͼ����
	
	//ˢ��һ��
	RegulateControlSize();
	MoveCtrl(IDC_LIST_BATCH_DETAIL, left_client, top_client, right_client, bottom_client);
}

/*************************************************************
 �� �� ����AddSelToList()
 ���ܸ�Ҫ����ѡ������뵽�б������ʾ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ��ѡ�������
		   Param2	ָ�����б��е�λ������
**************************************************************/
//##ModelId=49B87BB20280
BOOL CBatchDetailView::AddSelToList( CBatchSel* pSel, int nIndex )
{
	//���������Ч��
	if(pSel == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	if(pSel->m_pObj == NULL)
		return FALSE;
	if(pSel->m_pCpu == NULL)
		return FALSE;

	CString str;
	//0: �����豸��
	str = pSel->m_pObj->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1: CPU��
	str.Format("%d(%s)", pSel->m_pCpu->code, pSel->m_pCpu->des);
	m_List.SetItemText(nIndex, 1, str);
	//2: ��ֵ����
	m_List.SetItemText(nIndex, 2, pSel->GetCallString(pSel->m_nCallZone));
	//3: ��ֵ���Ž��
	str = GetResult(pSel->m_nZone);
	m_List.SetItemText(nIndex, 3, str);
	//4:��ֵ���Ž��˵��
	if(pSel->m_nZone == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 4, str);
	//5: ��ֵ
	m_List.SetItemText(nIndex, 5, pSel->GetCallString(pSel->m_nCallSetting));
	//6: ��ֵ���
	str = GetResult(pSel->m_nSetting);
	m_List.SetItemText(nIndex, 6, str);
	//7:��ֵ���˵��
	if(pSel->m_nSetting == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 7, str);
	//8: ������
	m_List.SetItemText(nIndex, 8, pSel->GetCallString(pSel->m_nCallDigital));
	//9: ���������
	str = GetResult(pSel->m_nDigital);
	m_List.SetItemText(nIndex, 9, str);
	//10:���������˵��
	if(pSel->m_nDigital == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 10, str);
	//11: ��ѹ��
	m_List.SetItemText(nIndex, 11, pSel->GetCallString(pSel->m_nCallSoftBoard));
	//12: ��ѹ����
	str = GetResult(pSel->m_nSoftBoard);
	m_List.SetItemText(nIndex, 12, str);
	//13:��ѹ����˵��
	if(pSel->m_nSoftBoard == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 13, str);
	//14: ģ����
	m_List.SetItemText(nIndex, 14, pSel->GetCallString(pSel->m_nCallAnalog));
	//15: ģ�������
	str = GetResult(pSel->m_nAnalog);
	m_List.SetItemText(nIndex, 15, str);
	//16:ģ�������˵��
	if(pSel->m_nAnalog == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 16, str);
	//17: ¼��
	m_List.SetItemText(nIndex, 17, pSel->GetCallString(pSel->m_nCallOsc));
	//18: ¼�����
	str = GetResult(pSel->m_nOsc);
	m_List.SetItemText(nIndex, 18, str);
	//19:¼�����˵��
	if(pSel->m_nOsc == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 19, str);

	//20: ��ʷ�¼�
	m_List.SetItemText(nIndex, 20, pSel->GetCallString(pSel->m_nCallHistory));
	//21: ��ʷ�¼����
	str = GetResult(pSel->m_nHistory);
	m_List.SetItemText(nIndex, 21, str);
	//22:��ʷ�¼����˵��
	if(pSel->m_nHistory == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 22, str);

	//23: ��վ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pSel->m_pObj->m_sStationID, TYPE_STATION);
	str = "";
	if(pStation != NULL)
		str = pStation->m_sName;
	m_List.SetItemText(nIndex, 23, str);
	//24: һ���豸��
	str = "";
	if(pSel->m_pObj->m_pOwner  != NULL)
		str = pSel->m_pObj->m_pOwner->m_sName;
	m_List.SetItemText(nIndex, 24, str);
	//��������
	m_List.SetItemData(nIndex, (DWORD)pSel);
	return TRUE;
}

/*************************************************************
 �� �� ����GetResult()
 ���ܸ�Ҫ���õ��ٻ������������
 �� �� ֵ: �ٻ������������
 ��    ����param1	�ٻ����
		   Param2
**************************************************************/
//##ModelId=49B87BB202AF
CString CBatchDetailView::GetResult( int nResult )
{
	//���������Ч��
	if(nResult < 0 || nResult >9)
		return "";
	CString sResult = "";
	//0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�
	if(nResult == 0)
	{
		//����ٻ�
		sResult = "����ٻ�";
	}
	else if(nResult == 1)
	{
		//�ɹ�
		sResult = "�ٻ��ɹ�";
	}
	else if(nResult == 2)
	{
		//�ٻ�ʧ��
		sResult = "�ٻ�ʧ��";
	}
	else if(nResult == 3)
	{
		//װ�ò�֧��
		sResult = "װ�ò�֧��";
	}
	else if(nResult == 4)
	{
		//ָ��ʱ�������¼�
		sResult = "�ٻ��ɹ���ָ��ʱ����������";
	}
	else
	{
		sResult = "��δ�ٻ�";
	}
	return sResult;
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ����Ӧ�б���Ի���ʾ��Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB301C5
void CBatchDetailView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//ѡ����
				if(nSubItem == 2 || nSubItem == 5 || nSubItem == 8 || nSubItem == 11 || nSubItem == 14 || nSubItem == 17 || nSubItem == 20)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					
					if(str == "���ٻ�")
					{
						lplvcd->clrText = g_BatchNotCall;
					}
					if(str == "�ٻ�")
					{
						lplvcd->clrText = g_BatchCall;
					}
					if(str == "��֧��")
					{
						lplvcd->clrText = g_BatchNotSupport;
					}
				}
				//�����
				if(nSubItem == 3 || nSubItem == 6 || nSubItem == 9 || nSubItem == 12 || nSubItem == 15 || nSubItem == 18 || nSubItem == 21)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);

					if(str == "�ٻ��ɹ�")
					{
						lplvcd->clrText = g_BatchSuccess;
					}
					else if(str == "�ٻ�ʧ��")
					{
						lplvcd->clrText = g_BatchFail;
					}
					else if(str == "����ٻ�")
					{
						lplvcd->clrText = g_BatchNULL;
					}
					else if(str == "��δ�ٻ�")
					{
						lplvcd->clrText = g_BatchLater;
					}
					else if(str == "װ�ò�֧��")
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

//##ModelId=49B87BB301F4
void CBatchDetailView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_BATCH))
				return;
			pMenu = Menu.GetSubMenu(0);
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
		}
	}
}

/*************************************************************
 �� �� ����OnSTTP20154()
 ���ܸ�Ҫ���յ�20154, �����ٻ���Ӧ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB3036B
void CBatchDetailView::OnSTTP20154( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CBatchDetailView::OnSTTP20154 start");

	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//�жϱ���ͷ
	if(pSttpData->sttp_head.uMsgID != 20154)
		return;

	//PT_ID
	CString PT_ID = pSttpData->sttp_body.ch_pt_id;
	//CPU
	int nCPU = pSttpData->sttp_body.nCpu;

	//���Ҷ�Ӧ��ѡ����
	CBatchSel* pSel = FindMapBatchSel(PT_ID, nCPU);
	if(pSel == NULL)
	{
		CString str;
		str.Format("�����ٻ�--���Ҳ���%sװ��%dCPU��Ӧ����", PT_ID, nCPU);
		WriteLog(str);
		return;
	}
	CString str;
	str = pSel->m_pObj->m_sName;
	CString strOut;
	if(!AddToComplete(pSel))
	{
		strOut.Format("�����ٻ�---װ��%s��%d��CPU�ظ�����, ������", str, nCPU);
		return;
	}
	WriteLog("20154��������");
	//����ֵ
	pSel->m_nZone = pSttpData->sttp_body.nZone;
	pSel->m_nSetting = pSttpData->sttp_body.nMsgId;
	pSel->m_nDigital = pSttpData->sttp_body.nStatus;
	pSel->m_nSoftBoard = pSttpData->sttp_body.nChangeConfigType;
	pSel->m_nAnalog = pSttpData->sttp_body.nChangeInfoType;
	pSel->m_nZoneNote = pSttpData->sttp_body.nEventType;
	pSel->m_nSettingNote = pSttpData->sttp_body.nCmdSource;
	pSel->m_nDigitalNote = pSttpData->sttp_body.nFlag;
	pSel->m_nAnalogNote = pSttpData->sttp_body.nSource;
	pSel->m_nOsc = atoi( pSttpData->sttp_body.ch_station_id );
	WriteLog(pSttpData->sttp_body.ch_station_id);
	pSel->m_nOscNote = atoi( pSttpData->sttp_body.ch_HandlerName );
	WriteLog(pSttpData->sttp_body.ch_HandlerName);
	pSel->m_nHistory = atoi( pSttpData->sttp_body.ch_version );
	WriteLog(pSttpData->sttp_body.ch_version);
	pSel->m_nHistoryNote = atoi( pSttpData->sttp_body.ch_time_20_BIT1 );
	WriteLog(pSttpData->sttp_body.ch_time_20_BIT1);

	str = pSttpData->sttp_body.ch_check_code;
	pSel->m_nSoftBoardNote = atoi(str);
	//�ı�ֵ��ʾ
	ChangeValueCalling(pSel);
	
	str = pSel->m_pObj->m_sName;
	strOut.Format("�����ٻ�---װ��%s��%d��CPU�ٻ����", str, nCPU);
	WriteLog(strOut);

	if (m_mapComplete.GetCount() == m_mapSave.GetCount())
	{
		WriteLog("CBatchDetailView::OnSTTP20154 �����ٻ����");
		//֪ͨ����ҳ��
		GetParentFrame()->PostMessage(BATCHCALL_END);
		AfxMessageBox("�����ٻ����");
	}
}

/*************************************************************
 �� �� ����ChangeValueCalling()
 ���ܸ�Ҫ�����ٻ�ʱ�ı�ѡ�������ʾ
 �� �� ֵ: 
 ��    ����param1	ָ��ѡ����
		   Param2	ָ�����б���е�λ��,��Ϊ-1,��ʾδ֪λ��,�����Լ������
**************************************************************/
//##ModelId=49B87BB2035B
int CBatchDetailView::ChangeValueCalling( CBatchSel* pSel, int nIndex /*= -1*/ )
{
	//���������Ч��
	if(pSel == NULL)
		return -1;
	if(nIndex < 0)
	{
		//�������б���е�λ��
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pSel;
		nIndex = m_List.FindItem(&lvFind);
		if(nIndex < 0)
			return nIndex;
	}
	//����ֵ
	//0:����ٻ� 1���ɹ� 2��ʧ�� 3:װ�ò�֧�� 9:��δ�ٻ�
	//3.��ֵ����
	m_List.SetItemText(nIndex, 3, GetResult(pSel->m_nZone));
	//4.��ֵ���Ž��˵��
	CString str;
	if(pSel->m_nZone == 2) //ʧ��
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote);
		m_List.SetItemText(nIndex, 4, str);

	}
	else
	{
		m_List.SetItemText(nIndex, 4, "-");
	}
	//6.��ֵ
	m_List.SetItemText(nIndex, 6, GetResult(pSel->m_nSetting));
	//7:��ֵ���˵��
	if(pSel->m_nSetting == 2) //ʧ��
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote);
		m_List.SetItemText(nIndex, 7, str);
	}
	else
	{
		m_List.SetItemText(nIndex, 7, "-");
	}
	//9.������
	m_List.SetItemText(nIndex, 9, GetResult(pSel->m_nDigital));
	//10: ���������˵��
	if(pSel->m_nDigital == 2) //ʧ��
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote);
		m_List.SetItemText(nIndex, 10, str);
	}
	else
		m_List.SetItemText(nIndex, 10, "-");
	//12.��ѹ��
	m_List.SetItemText(nIndex, 12, GetResult(pSel->m_nSoftBoard));
	//13:��ѹ����˵��
	if(pSel->m_nSoftBoard == 2)//ʧ��
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote);
		m_List.SetItemText(nIndex, 13, str);
	}
	else
		m_List.SetItemText(nIndex, 13, "-");
	
	//15.ģ����
	m_List.SetItemText(nIndex, 15, GetResult(pSel->m_nAnalog));
	//16:ģ�������˵��
	if(pSel->m_nAnalog == 2) //ʧ��
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote);
		m_List.SetItemText(nIndex, 16, str);
	}
	else
		m_List.SetItemText(nIndex, 16, "-");
	
	//18: ¼�����
	str = GetResult(pSel->m_nOsc);
	m_List.SetItemText(nIndex, 18, str);
	//19:¼�����˵��
	if(pSel->m_nOsc == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 19, str);
	
	
	//21: ��ʷ�¼����
	str = GetResult(pSel->m_nHistory);
	m_List.SetItemText(nIndex, 21, str);
	//22:��ʷ�¼����˵��
	if(pSel->m_nHistory == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 22, str);

	
	m_List.Update(nIndex);

	return nIndex;
}

/*************************************************************
 �� �� ����AddToComplete()
 ���ܸ�Ҫ����ָ��ѡ������뵽���ٻ��������
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ��ѡ����
		   Param2
**************************************************************/
//##ModelId=49B87BB30003
BOOL CBatchDetailView::AddToComplete( CBatchSel* pSel )
{
	//���������Ч��
	if(pSel == NULL)
		return FALSE;
	//�����Ƿ�����������

	CBatchSel* pFind = NULL;
	CString strKey;
	strKey.Format(pSel->m_pObj->m_sID + "cpu:%d", pSel->m_pCpu->code);
	m_mapComplete.Lookup(strKey, (void*&)pFind);
	if( pFind)
	{
		return FALSE;
	}
	m_mapComplete.SetAt(strKey, pSel);
	
	return TRUE;
}


/*************************************************************
 �� �� ����CompareFunction()
 ���ܸ�Ҫ���ȽϺ���,�����б�����
 �� �� ֵ: �ȽϽ��.1, 0, -1
 ��    ����param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   param3 �Ƚϲ���
**************************************************************/
//##ModelId=49B87BB20128
int CALLBACK CBatchDetailView::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CBatchSel* pResult1 = (CBatchSel*)lParam1;
	CBatchSel* pResult2 = (CBatchSel*)lParam2;
	if(pResult2 == NULL || pResult1 == NULL)
		return 0;
	if(pResult1->m_pObj == NULL || pResult2->m_pObj == NULL)
		return 0;
	int nCol = (int)lParamData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation1 = NULL;
	
	CStationObj* pStation2 = NULL;
	
	
	CString str1, str2;
	int iResult = 0;
	//"�����豸", "CPU��", "��ֵ���Ž��", "��ֵ���Ž��˵��", "��ֵ", "��ֵ���", "��ֵ���˵��", 
	//"������","���������", "���������˵��", "��ѹ��","��ѹ����", "��ѹ����˵��", 
	//"ģ����","ģ�������", "ģ�������˵��", "��վ��", "һ���豸"
	switch(nCol)
	{
	case 0: //�����豸
		str1 = pResult1->m_pObj->m_sName;
		str2 = pResult2->m_pObj->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //CPU
		if(pResult1->m_pCpu != NULL && pResult2->m_pCpu != NULL)
			iResult = pResult1->m_pCpu->code - pResult2->m_pCpu->code;
		else
			iResult = 0;
		break;
	case 2: //��ֵ�����Ƿ��ٻ�
		iResult = (pResult1->m_nCallZone)-(pResult2->m_nCallZone);
		break;
	case 3: //��ֵ���Ž��
		iResult = pResult1->m_nZone - pResult2->m_nZone;
		break;
	case 4: //��ֵ���Ž��˵��
		iResult = pResult1->m_nZoneNote - pResult2->m_nZoneNote;
		break;
	case 5: //��ֵ�Ƿ��ٻ�
		iResult = (pResult1->m_nCallSetting)-(pResult2->m_nCallSetting);
		break;
	case 6:	//��ֵ���
		iResult = pResult1->m_nSetting - pResult2->m_nSetting;
		break;
	case 7: //��ֵ���˵��
		iResult = pResult1->m_nSettingNote - pResult2->m_nSettingNote;
		break;
	case 8: //�������Ƿ��ٻ�
		iResult = (pResult1->m_nCallDigital)-(pResult2->m_nCallDigital);
		break;
	case 9: //���������
		iResult = pResult1->m_nDigital - pResult2->m_nDigital;
		break;
	case 10: //���������˵��
		iResult = pResult1->m_nDigitalNote - pResult2->m_nDigitalNote;
		break;
	case 11: //��ѹ���Ƿ��ٻ�
		iResult = (pResult1->m_nCallSoftBoard)-(pResult2->m_nCallSoftBoard);
		break;
	case 12: //��ѹ����
		iResult = pResult1->m_nSoftBoard - pResult2->m_nSoftBoard;
		break;
	case 13://��ѹ����˵��
		iResult = pResult1->m_nSoftBoardNote - pResult2->m_nSoftBoardNote;
		break;
	case 14: //ģ�����Ƿ��ٻ�
		iResult = (pResult1->m_nCallAnalog)-(pResult2->m_nCallAnalog);
		break;
	case 15: //ģ�������
		iResult = pResult1->m_nAnalog - pResult2->m_nAnalog;
		break;
	case 16: //ģ�������˵��
		iResult = pResult1->m_nAnalogNote - pResult2->m_nAnalogNote;
		break;
	case 17: //¼���Ƿ��ٻ�
		iResult = (pResult1->m_nCallOsc)-(pResult2->m_nCallOsc);
		break;
	case 18: //¼�����
		iResult = pResult1->m_nOsc - pResult2->m_nOsc;
		break;
	case 19: //¼�����˵��
		iResult = pResult1->m_nOscNote - pResult2->m_nOscNote;
		break;
	case 20: //��ʷ�¼��Ƿ��ٻ�
		iResult = (pResult1->m_nCallHistory)-(pResult2->m_nCallHistory);
		break;
	case 21: //��ʷ�¼����
		iResult = pResult1->m_nHistory - pResult2->m_nHistory;
		break;
	case 22: //��ʷ�¼����˵��
		iResult = pResult1->m_nHistoryNote - pResult2->m_nHistoryNote;
		break;
	case 23: //��վ��
		pStation1 = (CStationObj*)pApp->GetDataEngine()->FindDevice(pResult1->m_pObj->m_sStationID, TYPE_STATION);
		if(pStation1 != NULL)
		{
			str1 = pStation1->m_sName;
		}
		else
			str1 = "";
		pStation2 = (CStationObj*)pApp->GetDataEngine()->FindDevice(pResult2->m_pObj->m_sStationID, TYPE_STATION);
		if(pStation2 != NULL)
		{
			str2 = pStation2->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 24: //һ���豸
		if(pResult1->m_pObj->m_pOwner != NULL)
		{
			str1 = pResult1->m_pObj->m_pOwner->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pObj->m_pOwner != NULL)
		{
			str2 = pResult2->m_pObj->m_pOwner->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iBatchViewAsc;
	return iResult;
}

//##ModelId=49B87BB302E0
void CBatchDetailView::OnColumnclickListBatchDetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iBatchViewAsc = - g_iBatchViewAsc;
	}
	else
	{
		g_iBatchViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);

	*pResult = 0;
}

void CBatchDetailView::OnShowResult( WPARAM wParam, LPARAM lParam )
{
	if (m_mapComplete.GetCount() != m_mapSave.GetCount())
	{
		if (AfxMessageBox("�Ƿ�ˢ�������ٻ���Ŀ?", MB_YESNO) == IDNO)
		{
			return;
		}
	}
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	BATCHSEL_MAP* pMap = (BATCHSEL_MAP*)lParam;
	POSITION pos = pMap->GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = "";
	while (pos != NULL)
	{
		 pMap->GetNextAssoc(pos, strTemp, (void*&)pSel);
		 if (pSel == NULL)
		 {
			 continue;
		 }
		 if(pSel->m_bChecked)
		 {
			 ChangeSetting(pSel);
			 m_mapSave.SetAt(strTemp, pSel);
		 }
	}
	FillMap();
}


/*************************************************************
 �� �� ��: FillList()
 ���ܸ�Ҫ: ����б�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CBatchDetailView::FillMap()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	POSITION pos = m_mapSave.GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = "";
	while (pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		if(pSel == NULL)
		{
			continue;
		}
		AddSelToList(pSel, 0);
	}
	m_List.SetRedraw(TRUE);
}


CBatchSel* CBatchDetailView::FindMapBatchSel( CString PT_ID, int nCpu )
{
	//û��ѡ����,����NULL
	if(m_mapSave.GetCount() < 1)
		return NULL;

	CBatchSel* pSel = NULL;
	CString strKey;
	strKey.Format(PT_ID + "cpu:%d",  nCpu);
	m_mapSave.Lookup(strKey, (void*&)pSel);
	MYASSERT_NULL(pSel);
	//���ҵ�
	return pSel;
}

void CBatchDetailView::ChangeSetting(CBatchSel* pSel)
{
	if (pSel->m_nZone != 3)
	{
		pSel->m_nZone = 9;
	}
	if (pSel->m_nSetting != 3)
	{
		pSel->m_nSetting = 9;
	}
	if (pSel->m_nDigital != 3)
	{
		pSel->m_nDigital = 9;
	}
	if (pSel->m_nSoftBoard != 3)
	{
		pSel->m_nSoftBoard = 9;
	}
	if (pSel->m_nAnalog != 3)
	{
		pSel->m_nAnalog = 9;
	}
	if (pSel->m_nOsc != 3 )
	{
		pSel->m_nOsc = 9;
	}
	if (pSel->m_nHistory != 3 )
	{
		pSel->m_nHistory = 9;
	}
}

void CBatchDetailView::OnBatchClose() 
{
	// TODO: Add your command handler code here
	//OnBtnClose();
	GetParentFrame()->SendMessage(BATCHCALL_END);
	GetParentFrame()->SendMessage(WM_CLOSE);
}


void CBatchDetailView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile("�����ٻ����");
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CBatchDetailView::OnBatchExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName = "�����ٻ�";
	strName.Format("�����ٻ�%s.xls", t.Format("%Y%m%d%H%M%S"));
	
	//ȡ���û�ѡ���ļ���
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//�����ɹ�
		if(AfxMessageBox("�����ļ��ɹ�, ��[ȷ��]���ļ�Ŀ¼", MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//����ʧ��
		AfxMessageBox("�������ļ�ʧ��");
	}
}

/****************************************************
Date:2014/1/2  Author:LYH
������:   WriteResultToExcel	
����ֵ:   BOOL	
���ܸ�Ҫ: �����������Excel
����: CString strFullPath	
*****************************************************/
BOOL CBatchDetailView::WriteResultToExcel( CString strFullPath )
{
	CDatabase database;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString strSQL,str;
	
	strSQL.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		strDriver, strFullPath, strFullPath);
	
	CFileFind find;
	BOOL IsFind = FALSE;
	IsFind = find.FindFile(strFullPath, 0);
	//"�����豸", "CPU��", "��ֵ���Ž��", "��ֵ���Ž��˵��", "��ֵ���", "��ֵ���˵��", "���������", "���������˵��", "��ѹ����", "��ѹ����˵��", "ģ�������", "ģ�������˵��", "��վ��", "һ���豸"
	try
	{
		if( database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{
			if(!IsFind)
			{
				//�½�
				strSQL = "CREATE TABLE Log(�����豸 TEXT, CPU�� TEXT, ��ֵ���Ž�� TEXT, ��ֵ���Ž��˵�� TEXT, ��ֵ��� TEXT, ��ֵ���˵�� TEXT, ��������� TEXT, ���������˵�� TEXT, ��ѹ���� TEXT, ��ѹ����˵�� TEXT, ģ������� TEXT, ģ�������˵�� TEXT, ¼���ļ���� TEXT,¼���ļ����˵�� TEXT,��ʷ�¼���� TEXT,��ʷ�¼����˵�� TEXT,��վ�� TEXT, һ���豸 TEXT)";
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CBatchSel* pSel = (CBatchSel*)m_List.GetItemData(i);
				if(pSel)
				{
					AddSelToExcel(&database, pSel);
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

/****************************************************
Date:2014/1/2  Author:LYH
������:   AddSelToExcel	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: CDatabase * pDatabase	
����: CBatchSel * pSel	
*****************************************************/
BOOL CBatchDetailView::AddSelToExcel( CDatabase* pDatabase, CBatchSel* pSel )
{
	if(!pDatabase || !pSel)
		return FALSE;
	if(!pSel->m_pObj || !pSel->m_pCpu)
		return FALSE;
	CString strCPU = "";
	strCPU.Format("%d(%s)", pSel->m_pCpu->code, pSel->m_pCpu->des);
	
	//17: ��վ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pSel->m_pObj->m_sStationID, TYPE_STATION);
	CString strStation = "";
	if(pStation != NULL)
		strStation = pStation->m_sName;
	//18: һ���豸��
	CString strDevice = "";
	if(pSel->m_pObj->m_pOwner  != NULL)
		strDevice = pSel->m_pObj->m_pOwner->m_sName;

	CString strSQL;
	strSQL.Format("INSERT INTO Log (�����豸, CPU��, ��ֵ���Ž��, ��ֵ���Ž��˵��, ��ֵ���, ��ֵ���˵��, ���������, ���������˵��, ��ѹ����, ��ѹ����˵��, ģ�������, ģ�������˵��,¼���ļ����,¼���ļ����˵��,��ʷ�¼����,��ʷ�¼����˵��,��վ��, һ���豸) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		pSel->m_pObj->m_sName, strCPU, 
		GetResult(pSel->m_nZone), pSel->m_nZone == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote):"-", 
		GetResult(pSel->m_nSetting), pSel->m_nSetting == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote):"-", 
		GetResult(pSel->m_nDigital), pSel->m_nDigital == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote):"-", 
		GetResult(pSel->m_nSoftBoard), pSel->m_nSoftBoard == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote):"-", 
		GetResult(pSel->m_nAnalog), pSel->m_nAnalog == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote):"-", 
		GetResult(pSel->m_nOsc), pSel->m_nOsc == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote):"-", 
		GetResult(pSel->m_nHistory), pSel->m_nHistory == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote):"-", 
		strStation, strDevice);
	pDatabase->ExecuteSQL(strSQL);
	return TRUE;
}
