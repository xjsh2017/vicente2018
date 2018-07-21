// PTDistance.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTDistance.h"
#include "MainFrm.h"
#include "PrintListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           ����������λ*/
int g_iPTDistanceAsc;

UINT PTDistanceLoad(LPVOID pParam)
{
	CPTDistance* ptDistance = (CPTDistance*)pParam;
	if(ptDistance == NULL)
		return 0;
	int nQueryNo = ptDistance->GetCurrentQueryNo();
	//��ȡ����
	if(ptDistance->IsCurrentQueryNo(nQueryNo) && !ptDistance->m_bExitThread)
	{
		if(ptDistance->ReLoadData(nQueryNo))
			ptDistance->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptDistance->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}
	ptDistance->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTDistance

IMPLEMENT_DYNCREATE(CPTDistance, CViewBase)

CPTDistance::CPTDistance()
	: CViewBase(CPTDistance::IDD)
{
	//{{AFX_DATA_INIT(CPTDistance)
	m_bShowAcked = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_EventManager.ClearAllEvents();
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

CPTDistance::~CPTDistance()
{
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

void CPTDistance::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTDistance)
	DDX_Control(pDX, IDC_LIST_PTDISTANCE, m_List);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_DETAIL, m_btnDetail);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_ACKALL, m_btnAckAll);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_ACK, m_btnAck);
	DDX_Check(pDX, IDC_CHECK_PTDISTANCE_HIDE, m_bShowAcked);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPTDistance, CViewBase)
	//{{AFX_MSG_MAP(CPTDistance)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_ACK, OnBtnPtdistanceAck)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_ACKALL, OnBtnPtdistanceAckall)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_DETAIL, OnBtnPtdistanceDetail)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_PRINT, OnBtnPtdistancePrint)
	ON_BN_CLICKED(IDC_CHECK_PTDISTANCE_HIDE, OnCheckPtdistanceHide)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTDISTANCE, OnDblclkListPtdistance)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTDISTANCE, OnColumnclickListPtdistance)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTDISTANCE, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPTDistance::AssertValid() const
{
	CViewBase::AssertValid();
}

void CPTDistance::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTDistance message handlers

void CPTDistance::OnInitialUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	//��¼�ؼ�
	RecordRate(IDC_CHECK_PTDISTANCE_HIDE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_ACK, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_ACKALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_DETAIL, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	
	//��ʼ���б�
	InitListCtrl();
	//��ʼ�������ؼ�
	InitControls();
}

void CPTDistance::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTDISTANCE, left_client, IDC_BTN_PTDISTANCE_DETAIL, right_client, bottom_client);
}

//##ModelId=49B87B9402AF
void CPTDistance::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CPTDistance::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("��ʼ�˳���������ѯ�߳�,��ѯ��:%d", nNo);
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
	
	str.Format("��������ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTDistance::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("��������ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTDistance::GetFirstNode()
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

int CPTDistance::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);  
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

int CALLBACK CPTDistance::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CXJEventDistance* pEvent1 = (CXJEventDistance*)lParam1;
	CXJEventDistance* pEvent2 = (CXJEventDistance*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"
	switch(nCol)
	{
	case 0: //�¼�ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //��·����
		str1 = pEvent1->m_sLineName;
		str2 = pEvent2->m_sLineName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3:	//��ѹ�ȼ�
		iResult = pEvent1->m_nVoltage - pEvent2->m_nVoltage;
		break;
	case 4: //��·�ܳ���
		if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 5: //���ϵ㵽M�˾���
		if(pEvent1->m_fDistance - pEvent2->m_fDistance < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fDistance - pEvent2->m_fDistance > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 6://����λ��
		iResult = pEvent1->m_nTowerPos - pEvent2->m_nTowerPos;
		break;
	case 7: //ά��������
		str1 = pEvent1->m_sManagerUnit;
		str2 = pEvent2->m_sManagerUnit;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://��ȷ��
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTDistanceAsc;
	return iResult;
}

int CPTDistance::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();

	return 0;
}

int CPTDistance::InitListStyle()
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
		//char* arColumn[9]={"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"};
		
		//�����п�,��С����
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 180; // ʱ��
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 2://
				lvCol.cx = 220; // ��·���� 
				colName = StringFromID(IDS_MODEL_LINE);
				break;
			case 3://
				lvCol.cx = 80; // ��ѹ�ȼ�
				colName = StringFromID(IDS_MODEL_VOLTAGE);
				break;
			case 4:
				lvCol.cx = 100; // ��·�ܳ���
				colName = StringFromID(IDS_LINE_TOTALLENGTH);
				break;
			case 5:
				lvCol.cx = 100; //���ϵ㵽M�˾���
				colName = StringFromID(IDS_FAULTPOINT_M);
				break;
			case 6:
				lvCol.cx = 80; //����λ��
				colName = StringFromID(IDS_TOWER_LOCATION);
				break;
			case 7:
				lvCol.cx = 160; //ά��������
				colName = StringFromID(IDS_MAINTAIN_DEPARTMENT);
				break;
			case 8:
				lvCol.cx = 80; //��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=80;//�п�100����
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ������"ȷ��"
		m_List.SetColumnHide(8, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

int CPTDistance::FillListData( int nQueryNo )
{
	WriteLog("CPTDistance::FillListData ��ʼ���", XJ_LOG_LV3);
	
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	
	CXJEventDistance * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDistance*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTDistance::FillListData ��ʼ���", XJ_LOG_LV3);
	
	return 0;
}

BOOL CPTDistance::LoadListConfig()
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
			WriteLog("CPTDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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
				WriteLog("CPTDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTDistanceConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("CPTDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTDistanceConfig�µ�����");
					WriteLog("CPTDistance::��ȡUIConfigʧ��,��ʹ��Ĭ���з��");
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

BOOL CPTDistance::SaveListConfig()
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
			WriteLog("CPTDistance::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("CPTDistance::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTDistanceConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTDistanceConfig, ���½��ڵ���ӽڵ�");	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTDistanceConfig");
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
						WriteLog("δ�ҵ�PTDistanceConfig�µ�����, �½������ӽڵ�");
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
						WriteLog("�ҵ���PTDistanceConfig�µ�����, ��ɾ���½������ӽڵ�");
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

BOOL CPTDistance::NeedSave()
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

int CPTDistance::InitControls()
{
	return 0;
}

BOOL CPTDistance::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/, int nQueryNo /*= 0*/ )
{
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;
	
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTDistance::LoadDataFromDB, ��ʼ��ѯ");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);

		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//Ҫ��ѯ��ȷ���¼�, �����������
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//����ѯ��ȷ���¼�, �����ֻ��ѯδȷ���¼�������
		sql.Conditionlist.push_back(condition1);
	}

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);

	//����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by TIME DESC,MS DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
	
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_DISTANCE_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CPTDistance::LoadDataFromDB, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTDistance::LoadDataFromDB, ��ȡ��%d������", nCount);
			WriteLog(str);
		}

		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//�����¼�
			CXJEventDistance * pEvent = new CXJEventDistance;
			if(pEvent->LoadFromDB(&pMemset))
			{
				//��ȡ���ݳɹ�,���뵽����
				//���뵽����
				EnterCriticalSection(&m_CriticalOper);
				m_EventManager.AddEvent(pEvent);
				LeaveCriticalSection(&m_CriticalOper);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTDistance::LoadDataFromDB,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	WriteLog("CPTDistance::LoadDataFromDB, ��ѯ���");
	return bReturn;
}

void CPTDistance::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	
	ReFillAll();
}

void CPTDistance::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

void CPTDistance::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_HIDE);
}

BOOL CPTDistance::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_SHOW);
	//�������
	EnterCriticalSection(&m_CriticalOper);
	m_EventManager.ClearAllEvents();
	LeaveCriticalSection(&m_CriticalOper);
	m_List.DeleteAllItems();
	//������������
	if(MODE_SUB == g_role)
	{
		if(LoadDataFromDB(m_nMaxRow, m_bShowAcked, 0))
			FillListData(0);
		GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_HIDE);
	}
	else
		StartThread();
	return TRUE;
}

int CPTDistance::GetNextQueryNo()
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

void CPTDistance::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTDistanceLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("װ�ò���ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

BOOL CPTDistance::ReLoadData( int nQueryNo )
{
	return LoadDataFromDB(m_nMaxRow, m_bShowAcked, nQueryNo);
}

void CPTDistance::EndOwnerThread()
{
	WriteLog("��ʼ�˳��������ҳ���ȡ�����߳�", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳��������ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

BOOL CPTDistance::AddEventToList( CXJEventDistance * pEvent, int i )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	//"���","ʱ��", "��·����","��ѹ�ȼ�", "��·�ܳ���", "���ϵ㵽M�˾���", "����λ��", "ά��������", "��ȷ��"
	//0:���
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 1, sTime);
	//2:��·����
	m_List.SetItemText(i, 2, pEvent->m_sLineName);
	//3:��ѹ�ȼ�
	str.Format("%dkV", pEvent->m_nVoltage);
	m_List.SetItemText(i, 3, str);
	//4:��·�ܳ���
	str.Format("%.2fkm", pEvent->m_fLineTotalLength);
	m_List.SetItemText(i, 4, str);
	//5:���ϵ㵽M�˾���
	str.Format("%.2fkm", pEvent->m_fDistance);
	m_List.SetItemText(i, 5, str);
	//6:����λ��
	str.Format("%d", pEvent->m_nTowerPos);
	m_List.SetItemText(i, 6, str);
	//7:ά��������
	m_List.SetItemText(i, 7, pEvent->m_sManagerUnit);
	//8:��ȷ��
	m_List.SetItemText(i, 8, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//��������
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

void CPTDistance::OnBtnPtdistanceAck() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					RefreshAck(pEvent, nIndex);
					//������Ϣ֪ͨ�����
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
					if(pFrame != NULL)
					{
						pFrame->PostMessage(WM_EVENTPT_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
					}
				}
			}
		}
	}
}

void CPTDistance::OnBtnPtdistanceAckall() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj == NULL)
		return;
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckPTAllEvent(EX_STTP_INFO_DISTANCE_CFG, m_pObj->m_sID))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

void CPTDistance::OnBtnPtdistanceDetail() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//��Ȩ��
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	//���ѡ��ʱֻ�����һ��
	if(nIndex != -1)
	{
		CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
		if(pEvent != NULL)
		{
			m_EventManager.m_pCurrentEvent = pEvent;
			pApp->ShowEventProp(&m_EventManager, TRUE);
		}
	}
}

void CPTDistance::OnBtnPtdistancePrint() 
{
	// TODO: Add your control notification handler code here
	CPrintListCtrl print;
	print.SetListCtrl(m_List.GetSafeHwnd());
	
	// ����ÿ�е�λ��
	print.ResizeColumnsToFitContent(TRUE);
	
	// �Ƿ�ü�ÿ�еĳ��� FALSE �� TRUE ��
	print.TruncateColumn(TRUE);
	
	// ��ӡ��֮��ķָ���
	print.PrintColumnSeparator(TRUE);
	
	// ��ӡ��֮��ķָ���
	print.PrintLineSeparator(TRUE);
	
	// ��ӡ�������
	print.PrintHead(TRUE);
	
	// ���ø�������
	print.SetHeadFont(StringFromID(IDS_HEAD_FONT), 14);
	print.SetFootFont(StringFromID(IDS_DEFAULT_FONT), 12);
	print.SetListFont(StringFromID(IDS_DEFAULT_FONT), 12);
	
	// ����ֽ������ TRUE,����FALSE ����
	print.SetPrinterOrientation(FALSE);
    
	BOOL bHide = m_List.GetColumnIsHide(0);
	BOOL bHide8 = m_List.GetColumnIsHide(8);
    CString strCa;
	strCa.Format("%s %s", m_pObj->m_sName, StringFromID(IDS_SECPROP_DISTANCE));
	print.Print(GetSafeHwnd(), strCa, FALSE);
	m_List.SetColumnHide(0, bHide);
	m_List.SetColumnHide(8, bHide8);
}

void CPTDistance::OnCheckPtdistanceHide() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTDistance::OnDblclkListPtdistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected >= 0)
	{
		//�õ����ж�Ӧ���¼�
		CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//���õ�ǰ�¼�
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager, TRUE);
	}
	*pResult = 0;
}

void CPTDistance::OnColumnclickListPtdistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iPTDistanceAsc = - g_iPTDistanceAsc;
	}
	else
	{
		g_iPTDistanceAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CPTDistance::RefreshAck( CXJEventDistance* pEvent, int nIndex /*= -1*/ )
{
	//�ж�������Ч��
	if(pEvent == NULL)
		return;
	if(nIndex == -1)
	{
		//�����¼����б��е�λ��
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pEvent;
		nIndex = m_List.FindItem(&lvFind);
	}
	//ʵ���Ҳ���,�˳�
	if(nIndex == -1)
		return;
	//����ID,��Ϊ���¼����Ȳ�֪��ID
	CString str;
	str.Format("%d", pEvent->m_lID);
	m_List.SetItemText(nIndex, 0, str);
	//�����Ƿ�ȷ����ʾ
	m_List.SetItemText(nIndex, 8, StringFromID(IDS_ACK_YES));
	//ˢ��
	m_List.Update(nIndex);
}

void CPTDistance::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					
					if (g_role == MODE_SUB)
					{
						//�õ��¼�����
						CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}
						
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
						else if (g_style == 3)
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

void CPTDistance::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//ˢ�½�����ʾ
	RefreshAck(pEvent);
}

void CPTDistance::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;

	//�����ڱ��صĵ�ַ
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
	}
}

void CPTDistance::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	
	//�����ڱ��صĵ�ַ
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//�ҵ��˱��ص�ַ,������ʾ
		RefreshAck(pLocal);
		//֪ͨ�¼����ԶԻ������﷢���˱���ȷ��
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			pFrame->PostMessage(WM_EVENTPT_PASSIVE_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
		}
	}
}

CXJEventDistance* CPTDistance::FindLocalPoint( CXJEventDistance* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventDistance* pReturn = NULL;
	//��Ϊ������ID��֪�������,ֻ�ܱȽ϶����豸ID, ����ʱ�����Ϣ
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventDistance* pTemp = (CXJEventDistance*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//�����豸��ͬ,�ȽϷ���ʱ��,ʱ�����ַ�����
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//����ʱ����ͬ,�ȽϷ���ʱ�����ֵ
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//��ͬ,��Ϊ��ͬһ�¼�
					pTemp->m_lID = pEvent->m_lID;
					pTemp->m_nIsAck = pEvent->m_nIsAck;
					pReturn = pTemp;
					break;
				}
				else
				{
					//����ʱ�����ֵ��ͬ,��һ��
					continue;
				}
			}
			else
			{
				//����ʱ�䲻ͬ,��һ��
				continue;
			}
		}
		else
		{
			//�����豸��ͬ, ��һ��
			continue;
		}
	}
	return pReturn;
}

void CPTDistance::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTDistance::OnSTTP20168 start");
	
	CXJEventDistance* pParam = (CXJEventDistance*)lParam;
	if(pParam == NULL)
		return;
	
	//����
	CXJEventDistance* pEvent = (CXJEventDistance*)pParam->Clone();
	if(pEvent == NULL)
		return;

	if(pEvent->m_sSecID != m_pObj->m_sID)
		return;
	
	//���뵽����
	m_EventManager.AddEvent(pEvent);
	//���뵽�б�,���뵽��һ��
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	WriteLog("CPTDistance::OnSTTP20168 neat end");
}

void CPTDistance::CheckLineCount()
{
	int nCount = m_List.GetItemCount();
	int nDel = nCount - m_nMaxRow;
	if(nDel <= 0)
	{
		//�б�����ʾ��û���������
		return;
	}
	//ɾ�����һ��,ֱ����������
	for(int i = 0; i < nDel; i++)
	{
		int nNewCount = m_List.GetItemCount();
		//ȡ�����һ��
		CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nNewCount - 1);
		//���б���ɾ��
		m_List.DeleteItem(nNewCount - 1);
		//���¼�������ڴ���ɾ��
		m_EventManager.RemoveAtEvent(pEvent);
	}
}
