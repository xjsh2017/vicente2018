// PTSoftBoard.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSoftBoard.h"
#include "DlgCheck.h"
#include "DlgDataCheck.h"
#include "DlgValidateID.h"
#include "DlgValidateUser.h"
#include "GlobalFunc.h"

#include "MainFrm.h"
#include "DlgOperHis.h"

#include "XJTagOutStore.h"
#include "XJUserStore.h"
#include "XJUtilsStore.h"
#include "qptsetstatetable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int PTSETSOFT_TIMER_LEN = 2;

UINT PTSoftBoardLoad(LPVOID pParam)
{
	CPTSoftBoard* ptSoftBoard = (CPTSoftBoard*)pParam;
	if(ptSoftBoard == NULL)
		return 0;
	int nQueryNo = ptSoftBoard->GetCurrentQueryNo();
	//��ȡ����
	if(ptSoftBoard->IsCurrentQueryNo(nQueryNo) && !ptSoftBoard->m_bExitThread)
	{
		BOOL bLoad = ptSoftBoard->LoadPTSoftBoardConfig(nQueryNo);
		if(bLoad)
		{
			ptSoftBoard->LoadPTHistorySoftBoardData(nQueryNo);
			ptSoftBoard->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
		}
	}
	
	ptSoftBoard->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard

IMPLEMENT_DYNCREATE(CPTSoftBoard, CViewBase)

//##ModelId=49B87B89037C
CPTSoftBoard::CPTSoftBoard()
	: CViewBase(CPTSoftBoard::IDD)
{
	//{{AFX_DATA_INIT(CPTSoftBoard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrSoftBoard.RemoveAll();
	m_pObj = NULL;
	m_sCPU = "";
	m_nNewValueIndex = 3;
	m_nCurrentStatus = 0;
	m_nCurrentDetailStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_nOperationNum = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;

	m_bOperVerify = false;
	m_bMonVerify = false;
	m_sOperUser = "";
	m_sMonUser = "";

	m_bChecking = FALSE;
	m_bAlreadyShowOnce = false;

	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8A02B1
CPTSoftBoard::~CPTSoftBoard()
{
	ClearSoftBoardConfig();
	ClearModifyList();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8A0271
void CPTSoftBoard::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTSoftBoard)
	DDX_Control(pDX, IDC_BTN_PTSOFT_VERIFY2, m_btnVerify2);
	DDX_Control(pDX, IDC_BTN_PTSOFT_VERIFY1, m_btnVerify1);
	DDX_Control(pDX, IDC_GIF_PTSOFT, m_gif);
	DDX_Control(pDX, IDC_BTN_PTSOFTSET_VIEWPROG, m_btnViewProg);
	DDX_Control(pDX, IDC_BTN_PTSOFTSET_VIEW_HIS, m_btnViewHis);
	DDX_Control(pDX, IDC_BTN_PTSOFT_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTSOFT_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTSOFT_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BTN_PTSOFT_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTSOFT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTSoftBoard, CViewBase)
	//{{AFX_MSG_MAP(CPTSoftBoard)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTSOFT_CALL, OnBtnPtsoftCall)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_MODIFY, OnBtnPtsoftModify)
	ON_BN_CLICKED(IDC_BTN_PTSOFTSET_VIEWPROG, OnBtnViewProg)
	ON_BN_CLICKED(IDC_BTN_PTSOFTSET_VIEW_HIS, OnBtnViewHis)
	ON_CBN_SELCHANGE(IDC_CMB_PTSOFT_CPU, OnSelchangeCmbPtsoftCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSOFT, OnCustomDraw)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTSOFT, OnClickListPtsoft)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTSOFT_STYLE, OnBtnPtsoftStyle)
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSOFT_CALL, OnDoubleclickedBtnPtsoftCall)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_VERIFY1, OnBtnPtsoftVerify1)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_VERIFY2, OnBtnPtsoftVerify2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTSOFT, OnDblclkListPtsoft)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_EXCEL, OnBtnPtsoftExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20109, OnSTTP20109)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20118, OnSTTP20118)
	ON_MESSAGE(WM_STTP_20114, OnSTTP20114)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTSOFTBOARD, OnRefreshPTSoft)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8A02DE
void CPTSoftBoard::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8A02FD
void CPTSoftBoard::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard message handlers

//##ModelId=49B87B8A0246
void CPTSoftBoard::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	RecordRate(IDC_BTN_PTSOFT_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_STYLE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY1, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSOFT, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSOFT_CPU, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTSOFT, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTSB, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P3, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFTSET_VIEWPROG, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFTSET_VIEW_HIS, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSOFT_LOADING, 0, mid_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	m_btnVerify1.SetIcon(IDI_KEY1);
	m_btnVerify1.SetFlat(FALSE);
	
	m_btnVerify2.SetIcon(IDI_KEY1);
	m_btnVerify2.SetFlat(FALSE);

	if(g_PTControlModel == 1)//��ͨģʽ
	{
		m_btnVerify1.ShowWindow(SW_HIDE);
		m_btnVerify2.ShowWindow(SW_HIDE);
	}

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
//##ModelId=49B87B8A005D
int CPTSoftBoard::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	RefreshControls();
	LoadPTSoftBoardConfig();
	LoadPTHistorySoftBoardData();*/

	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8A007D
int CPTSoftBoard::InitListStyle()
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
		//char* arColumn[3]={"���","��ѹ������", "��ѹ�����"};
		
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
				lvCol.cx = 140; // ��ѹ������
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // ��ѹ�����
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��ѹ��)
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
//##ModelId=49B87B8A00CB
BOOL CPTSoftBoard::LoadListConfig()
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
			WriteLog("PTSoftBoard::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTSoftBoard::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTSoftBoardConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTSoftBoardConfig", XJ_LOG_LV3);
				WriteLog("PTSoftBoard::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTSoftBoardConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTSoftBoard::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B8A00EA
BOOL CPTSoftBoard::SaveListConfig()
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
			WriteLog("PTSoftBoard::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTSoftBoard::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTSoftBoardConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�ViewCommConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTSoftBoardConfig");
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
						WriteLog("δ�ҵ�PTSoftBoardConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTSoftBoardConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B8A00AB
int CPTSoftBoard::FillListData(INT nQueryNo)
{
	WriteLog("CPTSoftBoard::FillListData, ��ʼ�������", XJ_LOG_LV3);

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	CString strLog;
	strLog.Format("CPTSoftBoard::FillListData, ����%d����ʷֵ", m_HisValueMgr.GetCount());
	WriteLog(strLog, XJ_LOG_LV3);
	PT_SOFTBOARD *pts = NULL;
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;

		pts = m_arrSoftBoard.GetAt(i);
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
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalOper);
	WriteLog("CPTSoftBoard::FillListData, ����������", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B8A0109
BOOL CPTSoftBoard::NeedSave()
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
//##ModelId=49B87B8A0128
int CPTSoftBoard::InitControls()
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
	m_btnPrint.SetTooltipText(StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());
/*
	//��ѹ��Ͷ��
	m_btnModify.SetThemeHelper(&m_ThemeHelper);
//	m_btnModify.SetIcon(IDI_MODIFY);
	m_btnModify.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModify.SetTooltipText(_T("Ͷ����ѹ��"));
*/
	return 0;
}


//##ModelId=49B87B8A032C
void CPTSoftBoard::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTSOFT, left_client, IDC_BTN_PTSOFT_CALL, right_client, bottom_client);
}

//##ModelId=49B87B8A035B
void CPTSoftBoard::OnDestroy() 
{
	//����UI����
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 �� �� ����LoadPTSoftBoardConfig()
 ���ܸ�Ҫ�������ݿ��ж�ȡ��ѹ��������Ϣ,tb_pt_softboard_def
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8A0148
BOOL CPTSoftBoard::LoadPTSoftBoardConfig(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	WriteLog("CPTSoftBoard::LoadPTSoftBoardConfig, ��ʼ��ѯ", XJ_LOG_LV3);
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code����SOFT_ID,CODE_NAME,NAME,��DI_ID��С��������
		
	CString str;
		
	//�ֶ�
	//SOFT_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "SOFT_ID";
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
	str.Format("order by SOFT_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition3);
			
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadPTSoftBoardConfig, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//��������
			PT_SOFTBOARD * softboard = new PT_SOFTBOARD;
			softboard->ID = pMemset.GetValue((UINT)0); //DI_ID
			softboard->Name = pMemset.GetValue(1); //NAME
			softboard->CodeName = pMemset.GetValue(2); //Code_Name
			m_arrSoftBoard.Add(softboard);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSoftBoard::LoadPTSoftBoardConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTSoftBoard::LoadPTSoftBoardConfig, ��ѯ����", XJ_LOG_LV3);

	return bReturn;
}

/*************************************************************
 �� �� ����ClearSoftBoardConfig()
 ���ܸ�Ҫ�������ѹ��������Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8A0167
BOOL CPTSoftBoard::ClearSoftBoardConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD * digital = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(digital != NULL)
		{
			delete digital;
			digital = NULL;
		}
	}
	m_arrSoftBoard.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 �� �� ����AddSoftBoardValue()
 ���ܸ�Ҫ������ĳ��ѹ��ֵ���б�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ�������ѹ���ID
           Param2   Ҫ�������ѹ���ֵ
		   Param3	ָ��Ҫ���뵽�ڼ���, ��ֵӦ��>=3
**************************************************************/
//##ModelId=49B87B8A0177
BOOL CPTSoftBoard::AddSoftBoardValue(CString ID, CString VALUE, int iCol)
{
	//���ҵ�DI_ID==ID������
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_SOFTBOARD* pst = FindSoftBoard(strID);
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
	bReturn = m_List.SetItemText(nItem, iCol, DisplayValue(atoi(VALUE)));
	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTHistorySoftBoardData()
 ���ܸ�Ҫ��������ѹ����ʷֵ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8A0196
BOOL CPTSoftBoard::LoadPTHistorySoftBoardData(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	if(g_PTSoftValues <= 0)
		return TRUE;

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯtime��ʼ", XJ_LOG_LV3);
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
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,��ѯ��%d��time", nCount);
		WriteLog(strLog, XJ_LOG_LV3);

		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			
			if(i >= g_PTSoftValues)
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
		str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯtime����", XJ_LOG_LV3);

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯ��ʷֵ��ʼ", XJ_LOG_LV3);
	
	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);
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
		str = "SOFT_ID";
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
				
		CMemSet pMemset;

		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLogEx("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯʧ��");
			return FALSE;
		}
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,��ѯ%s����ʷ����,����Ϊ%d", strTime, nCount);
			WriteLog(str, XJ_LOG_LV3);

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
			str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,��ѯʧ��,ԭ��Ϊ%s", sError);
			WriteLogEx(str);
			bReturn = FALSE;
		}
	}

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, ��ѯ��ʷֵ����", XJ_LOG_LV3);
	return TRUE;
}

//##ModelId=49B87B8A037A
void CPTSoftBoard::OnBtnPtsoftCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ȡ���ٻ�
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
		
		//��֯20108����
		if(!pApp->m_SttpEngine.BuildDataFor20108PTSoftBoard(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
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
				str.Format("����[%s]�����ٻ���ѹ�屨��ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ���ѹ�屨��ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ���ѹ��
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;
			UpdateControlsEnable();

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTSoftBoardTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//�����˹�������־
			//pApp->AddManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sName);
			CString str;
			str.Format("����[%s]�����ٻ���ѹ�屨�ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ѹ��
		//ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 �� �� ����OnSTTP20109()
 ���ܸ�Ҫ����Ӧ20109(�ٻ���ѹ��֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8A03DE
void CPTSoftBoard::OnSTTP20109( WPARAM wParam,LPARAM lParam )
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
	
	//����Ƿ�20109����
	if(pSttpData->sttp_head.uMsgID != 20109)
	{
		//������
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20109 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20109 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20109 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if(m_bFirstFrame)
	{
		m_bFirstFrame = FALSE;
		//ʱ��,ȡ��ǰϵͳʱ��
		CTime tmTime = CTime::GetCurrentTime();
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//�¼�һ��
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}
	
	//������
	if(!LoadDataFromSTTP(pSttpData))
	{
		WriteLog("CPTSoftBoard::OnSTTP20109 ������ʧ��", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 ){
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);

	CString sLog;
	sLog.Format("����[%s]�ٻ���ѹ��ɹ�!", m_pObj->m_sName);
	WriteLog(sLog, XJ_LOG_LV2);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_SUCCESS);

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;

	//���¿ؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����LoadDataFromSTTP()
 ���ܸ�Ҫ����STTP�����ж�ȡ��ѹ������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	��������
**************************************************************/
//##ModelId=49B87B8A01B5
BOOL CPTSoftBoard::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//������ѹ������
	try
	{
		
		//��ȡÿ��ֵ,���뵽�б�
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddSoftBoardValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTSoftBoard::LoadDataFromSTTP, ����", XJ_LOG_LV3);
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
//##ModelId=49B87B8B000F
void CPTSoftBoard::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
	{
		//û�д����ٻ���ѹ��״̬,Ҳû�д�����ѹ��Ͷ��״̬,��������
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
		m_nCurrentStatus = 0;
		
		//���¿ؼ�������
		UpdateControlsEnable();
	}
	else
	{
		//����ʧ��
		//�������,�ı�״̬�Ͱ�ť����
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���ѹ��
				
			CString str;
			str.Format("����[%s]�ٻ���ѹ��ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���¿ؼ�������
			UpdateControlsEnable();
			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
			
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);
		}
		if(m_nCurrentStatus == 2)
		{
			//������ѹ��Ͷ��
			CString str;
			str.Format("����[%s]��ѹ��Ͷ��ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			
			m_bMonVerify = false;
			m_bOperVerify = false;

			m_nCurrentStatus = 0;

			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			
			//���¿ؼ�������
			UpdateControlsEnable();
			AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_FAIL));
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("��ѹ��Ͷ��ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("��ѹ��Ͷ��ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
		}	
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֹ��ܻ�Ӧ����)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8B001F
void CPTSoftBoard::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
	{
		//û�д����ٻ���ѹ��״̬Ҳû�д�����ѹ��Ͷ��״̬,��������
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
	//�������,�ı�״̬�Ͱ�ť����
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ѹ��
		m_nCurrentStatus = 0;
		
		//���¿ؼ�������
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	if(m_nCurrentStatus == 2)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;

		//������ѹ��Ͷ��
		m_nCurrentStatus = 0;

		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		
		//���¿ؼ�������
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			pApp->AddNewManOperator("��ѹ��Ͷ��ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			pApp->AddNewManOperator("��ѹ��Ͷ��ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
		}
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬�ı�ؼ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8A01C5
void CPTSoftBoard::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//�ٻ���ѹ��
		SetDlgItemText(IDC_BTN_PTSOFT_CALL, StringFromID(IDS_BTN_CALLSOFT));
		//��ѹ��Ͷ��
		SetDlgItemText(IDC_BTN_PTSOFT_MODIFY, StringFromID(IDS_BTN_MODIFYSOFT));
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);

		//����״̬,������
		GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
		
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTSOFT_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_STYLE) ->EnableWindow(TRUE);
		
		//������
		GetDlgItem(IDC_CMB_PTSOFT_CPU) ->EnableWindow(TRUE);
		//ֹͣ����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;

		if(g_PTControlModel == 2)//�㶫ģʽ
		{
			if(!m_bOperVerify)
			{
				GetDlgItem(IDC_BTN_PTSOFT_VERIFY2)->EnableWindow(false);
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				if (m_bMonVerify)
				{
					m_btnVerify2.SetIcon(IDI_TICK);
				}
				else
				{
					m_btnVerify2.SetIcon(IDI_KEY1);
				}
				GetDlgItem(IDC_BTN_PTSOFT_VERIFY2)->EnableWindow(true);
			}
			
			if (m_bOperVerify && m_bMonVerify)
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				m_btnVerify2.SetIcon(IDI_TICK);
				GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
				GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(FALSE);
				m_sOperUser = _T("");
				m_sMonUser = _T("");
			}
		}
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 3)
	{
		//�ٻ������״̬,�����Լ������Ķ�������
		
		GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(FALSE);
		
		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTSOFT_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_STYLE) ->EnableWindow(FALSE);
		
		//������
		GetDlgItem(IDC_CMB_PTSOFT_CPU) ->EnableWindow(FALSE);
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND), m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSB, str);
		
		//���Լ�����
		switch(m_nCurrentStatus)
		{
		case 1: //�ٻ���ѹ��
			SetDlgItemText(IDC_BTN_PTSOFT_CALL, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(TRUE);
			break;
		case 2: //��ѹ��Ͷ��
			SetDlgItemText(IDC_BTN_PTSOFT_MODIFY, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
			break;
		default:
			break;
		}
	}
}

//##ModelId=49B87B8A038A
void CPTSoftBoard::OnBtnPtsoftModify() 
{
	OnBtnPtsoftModify2();
	return;

	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ȡ���ٻ�
	m_bChecking = FALSE;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	/*if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//�ռ����޸ĵ�ֵ
			CheckModifyData(m_arrModifyList, strOut);
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			return;
		}
	}*/
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		//����״̬
		//����û��з��޸Ķ�ֵ
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//���޸Ķ�ֵ, ��ʾ���޸�
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_SOFT));
			return;
		}

		m_nOperationNum = GetOperationNum();
		
		//�齨Ҫ�޸�ֵ������
		m_strOutPut = "";
		//��¼��ֵ
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//Ҫ���û��ٴ�ȷ��
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}*/
		
		//����Աȷ��
		CDlgCheck dlg(this, 2);
		dlg.m_strModify = m_strOutPut;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//��ͨģʽ,Ҫ�����Ա��֤
				CDlgValidateID dlgID(2);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Բ���Ա�����֤�ɹ�:��ѹ��Ͷ��", m_sOperUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("����Ա�����֤ʧ��:��ѹ��Ͷ��");
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, "", -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
			
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//�໤Աȷ��
		CDlgCheck dlg1(this, 1);
		dlg1.m_strModify = m_strOutPut;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//��ͨģʽ,Ҫ��໤Ա��֤
				CDlgValidateID dlgID(1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Լ໤Ա�����֤�ɹ�:��ѹ��Ͷ��", m_sMonUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�໤Ա�����֤ʧ��:��ѹ��Ͷ��", m_sMonUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, "", -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
			
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//�齨����
		STTP_FULL_DATA sttpData;
		pApp->m_SttpEngine.BuildDataFor20117SoftBoardModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);

		//���ͱ���
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//�ж��Ƿ�ɹ�
		if(nReturn != 1)
		{
			//ʧ��
			if(nReturn == -1)
			{
				CString str;
				str.Format("����[%s]������ѹ��Ͷ�˱���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]������ѹ��Ͷ�˱���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
		}
		else
		{
			//�ɹ�,״̬��Ϊ������ѹ��Ͷ��
			m_nCurrentStatus = 2;

			m_bChecking = TRUE;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeSBTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//�����˹�����
			//pApp->AddManOperator(FUNC_OPT_SETSOFTBOARD, m_pObj->m_sID);
			
			//�ı�ؼ�������
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]������ѹ��Ͷ�˱��ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//��ѹ��Ͷ��״̬
		CancelCall();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
	}
	else 
	{
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		return;
	}
}

void CPTSoftBoard::OnBtnPtsoftModify2() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ȡ���ٻ�
	m_bChecking = FALSE;
	CString str;

	if(m_pObj == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSoftDataTable* pPTSoftData = pTagOutStore->GetPTSoftData();
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();

	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		//����״̬
		//����û��з��޸Ķ�ֵ
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//���޸Ķ�ֵ, ��ʾ���޸�
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_SOFT));
			//return;
		}

		m_nOperationNum = GetOperationNum();
		
		//�齨Ҫ�޸�ֵ������
		m_strOutPut = "";
		//��¼��ֵ
		CheckModifyData(m_arrModifyList, m_strOutPut);
//		AfxMessageBox(m_strOutPut);

		// ������ֵ
// 		str.Format("cpu: %s", m_sCPU);
// 		AfxMessageBox(str);
		pPTSoftData->ReLoad(QByteArray(m_pObj->m_sID.GetBuffer(0))
			, atoi(m_sCPU.GetBuffer(0)), m_arrModifyList, m_arrSoftBoard);
		
		//����Աȷ��
		CDlgDataCheck dlgCheck(this, XJ_USERGROUP_OPERATOR, XJ_TAGOUT_PTSOFTSET);
		dlgCheck.m_strModify = m_strOutPut;
		dlgCheck.m_sCPU = m_sCPU;
		if(dlgCheck.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//��ͨģʽ,Ҫ�����Ա��֤
				CDlgValidateUser dlgUser(XJ_USERGROUP_OPERATOR);
				//dlgUser.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				dlgUser.m_nFuncID = XJ_OPER_PTSOFTSET_STATE_2;
				QByteArray sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
				QByteArray sLogUserID = pTagOutState->GetLogUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
				dlgUser.m_strUser = sUserID.constData();
				if (!sUserID.isEmpty())
					dlgUser.m_strAuthUserID = sUserID.constData();
				else{
					dlgUser.m_strUser = sLogUserID.constData();
				}
				dlgUser.m_excludeUserList << pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT);
				if (pUserStore->hasFuncID(XJ_OPER_PTSOFTSET_STATE_2, pApp->m_User.m_strUSER_ID.GetBuffer(0)))
					dlgUser.m_strUser = pApp->m_User.m_strUSER_ID;
				if(dlgUser.DoModal() == IDOK)
				{
					if (dlgUser.m_strAuthUserID.IsEmpty()){
						pTagOutStore->ReLoadState();
						int nPTSetState = pTagOutState->GetStateID();
						
						CString str;
						if (XJ_OPER_HANGOUT != nPTSetState){
							CString sCurUserID = pTagOutState->GetOperUserID().constData();
							str.Format("�û�[%s]���ڶԸ�װ�ý�����ѹ��Ͷ�˲��� ���� ��װ����ȡ���˹��ƣ����Ժ�����"
								, sCurUserID);
							AfxMessageBox(str, MB_OK | MB_ICONWARNING);
							
							RevertModifyValue();
							return;
						}
					}
					m_sOperUser = dlgUser.m_strUser;
					
					pTagOutState->Next_PTSET_SOFT_STATE_2(atoi(m_sCPU)
						, m_sOperUser.GetBuffer(0), m_arrModifyList, m_arrSoftBoard);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();

					str.Format("�û�[%s]�Բ���Ա�����֤ʧ�ܣ�������֤����", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_2, m_sOperUser.GetBuffer(0)
						, QByteArray(str.GetBuffer(0)));

					return;
				}
			}
			
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//pPTSoftData->UnitTest_01();

		if (pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_3))
		{
			//�໤Աȷ��
			CDlgDataCheck dlg1(this, XJ_USERGROUP_MONITOR, XJ_TAGOUT_PTSOFTSET);
			dlg1.m_strModify = m_strOutPut;
			dlg1.m_sCPU = m_sCPU;
			if(dlg1.DoModal() == IDOK)
			{
				if(g_PTControlModel == 1)
				{
					//��ͨģʽ,Ҫ��໤Ա��֤
					CDlgValidateUser dlgUser(XJ_USERGROUP_MONITOR);
					//dlgUser.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
					dlgUser.m_nFuncID = XJ_OPER_PTSOFTSET_STATE_3;
					QByteArray sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_3);
					dlgUser.m_strUser = sUserID.constData();
					if (!sUserID.isEmpty())
						dlgUser.m_strAuthUserID = sUserID.constData();
					dlgUser.m_excludeUserList << pTagOutState->GetLogUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2); // ���˵����β���Ա�û�
					if (pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)){ // ����������Ա��֤���轫���ι����û����˵�
						dlgUser.m_excludeUserList.AppendField(pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT));
					}

					if(dlgUser.DoModal() == IDOK)
					{
						m_sMonUser = dlgUser.m_strUser;
						
						pTagOutState->Next_PTSET_SOFT_STATE_3(m_sMonUser.GetBuffer(0));
					}
					else
					{
						m_sMonUser = dlgUser.m_strUser;
						
						//�޲���Ȩ��
						//�ظ��޸�ǰ��ֵ
						RevertModifyValue();
						
						str.Format("�û�[%s]�Լ໤Ա�����֤ʧ�ܣ�������֤����", m_sMonUser);
						WriteLog(str, XJ_LOG_LV2);
						pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_3, m_sMonUser.GetBuffer(0)
							, QByteArray(str.GetBuffer(0)));
						
						return;
					}
				}
				
			}
			else
			{
				//��ͬ���޸�
				//�ظ��޸�ǰ��ֵ
				RevertModifyValue();
				m_bMonVerify = false;
				m_bOperVerify = false;
				UpdateControlsEnable();
				
				str.Format("�û�[%s]�Լ໤Ա�����֤ʧ�ܣ���ͬ���޸�", m_sMonUser);
				WriteLog(str, XJ_LOG_LV2);
				pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_3, m_sMonUser.GetBuffer(0)
					, QByteArray(str.GetBuffer(0)));
				
				return;
			}
		}

		// ExcutePTSet_Soft();
	}
	else if(m_nCurrentStatus == 2)
	{
		//��ѹ��Ͷ��״̬
		CancelCall();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
	}
	else 
	{
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		return;
	}
}

void CPTSoftBoard::ExcutePTSet_Soft()
{
	if (0 != m_nCurrentStatus)
		return;

	if (NULL == m_pObj){
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
		RevertModifyValue();
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	
	CString str;

	//�齨����
	STTP_FULL_DATA sttpData;
	pApp->m_SttpEngine.BuildDataFor20117SoftBoardModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		//ʧ��
		if(nReturn == -1)
		{
			str.Format("����[%s]������ѹ��Ͷ��ִ�б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			str.Format("����[%s]������ѹ��Ͷ��ִ�б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}else{
			str.Format("����������ѹ��Ͷ��ִ�б���ʧ��: [%s]", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		KillTimer(m_nPTSetTimer);
		AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		UpdateControlsEnable();

		QByteArray &sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
		pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, sUserID.constData()
			, QByteArray(str.GetBuffer(0)));
		
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}
	else
	{
		//�ɹ�,״̬��Ϊ������ѹ��Ͷ��
		m_nCurrentStatus = 2;
		
		m_bChecking = TRUE;
		
		//������ʱ��
		CDataEngine* pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nChangeSBTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		
		//�����˹�����
		//pApp->AddManOperator(FUNC_OPT_SETSOFTBOARD, m_pObj->m_sID);
		
		//�ı�ؼ�������
		UpdateControlsEnable();
		
		CString str;
		str.Format("����[%s]������ѹ��Ͷ�˱��ĳɹ�", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
	}
}

/*************************************************************
 �� �� OnBtnViewProg()
 ���ܸ�Ҫ����ѹ��Ͷ�˽��Ȳ鿴
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSoftBoard::OnBtnViewProg() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	CCJTabCtrlBar &bar = pMainFrame->m_wndGlobalMsgBar;
	
	CRect rcRect;
	GetWindowRect(&rcRect);
	
	if (m_bAlreadyShowOnce){
		bar.GetWindowRect(rcRect);
		m_pointViewProg.x = rcRect.left - 2;
		m_pointViewProg.y = rcRect.top - 18;

	}else{
		LONG w = 600;
		LONG h = 220;

		m_pointViewProg.x = rcRect.right - w;
		m_pointViewProg.y = rcRect.bottom - h;
		m_bAlreadyShowOnce = true;
	}
	
	bar.ShowWindow(bar.IsVisible() ? SW_HIDE : SW_SHOW);
	pMainFrame->FloatControlBar(&bar, m_pointViewProg, CBRS_ALIGN_LEFT);

	
	CString str;
	str.Format("x: %d; y: %d", m_pointViewProg.x, m_pointViewProg.y);
	//AfxMessageBox(str);

}

/*************************************************************
 �� �� OnBtnViewHis()
 ���ܸ�Ҫ����ѹ��Ͷ����ʷ�鿴
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSoftBoard::OnBtnViewHis() 
{
	// TODO: Add your control notification handler code here
	CDlgOperHis dlg;
	dlg.m_nType = 2;
	dlg.m_pObj = m_pObj;
	dlg.DoModal();
}

//##ModelId=49B87B8A01D4
void CPTSoftBoard::CheckModifyData( MODIFY_LIST& modifyList, CString& strOut )
{
	//Ͷ����ѹ���б�
	ClearModifyList();

	CPoint ptItem;
	CString strText, strPickList, strOutput = StringFromID(IDS_MODIFYSOFT_CHECK)+":\r\n";
	PT_SOFTBOARD *pts;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		ptItem = m_List.m_arrEdited.GetAt(i);
		strText = m_List.GetItemText(ptItem.x, ptItem.y);
		
		//ȥ��"*"
		strText = strText.Left(strText.Find("*", 0));
		TRACE0(strText + "|\n");
		
		//ȡ�ø���ѹ��Ķ���
		pts = (PT_SOFTBOARD *)m_arrSoftBoard.GetAt(ptItem.x);
		
		STTP_DATA * sttpData = new STTP_DATA;
		//д����ѹ��ID
		sttpData->id = atoi(pts->ID);
		sttpData->nCpu = atoi(m_sCPU);

		strOutput += pts->ID + "\t";
		strOutput += pts->Name + StringFromID(IDS_COMMON_FROM)+" -> ";
		strOutput += pts->Value + "\t"+StringFromID(IDS_COMMON_CHANGETO)+"-> ";
		strOutput += strText + "\r\n";
		
		//��ѹ���ֵ
		//�����ܵĲ�ͬ������ʽת����0:1
		if(!strText.CompareNoCase("OFF") || !strText.CompareNoCase(StringFromID(IDS_CASE_CEASING)))
		{
			strText = "0";
		}
		if(!strText.CompareNoCase("ON") || !strText.CompareNoCase(StringFromID(IDS_CASE_COMMISSIONING)))
		{
			strText = "1";
		}

		sttpData->str_value = strText;

		//���뵽����
		modifyList.Add(sttpData);
	}
	strOut = strOutput;
}

/*************************************************************
 �� �� ����ClearModifyList()
 ���ܸ�Ҫ������޸�����
 �� �� ֵ: void 
**************************************************************/
//##ModelId=49B87B8A01E5
void CPTSoftBoard::ClearModifyList()
{
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		delete sttpData;
	}
	m_arrModifyList.RemoveAll();
}

/*************************************************************
 �� �� ����OnSTTP20118()
 ���ܸ�Ҫ����Ӧ��ѹ��Ͷ��ԤУ֪ͨ(20118)
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8B003E
void CPTSoftBoard::OnSTTP20118( WPARAM wParam,LPARAM lparam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	
	//����Ǵ�����ѹ��Ͷ��״̬
	if(m_nCurrentStatus != 2)
	{
		//����ѹ��Ͷ��״̬
		return;
	}
	
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lparam;
	if(pSttpData == NULL)
		return;
	
	//����Ƿ�20118����
	if(pSttpData->sttp_head.uMsgID != 20118)
	{
		//������
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20118 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20118 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, pSttpData->sttp_body.nCpu, m_arrModifyList.GetSize());
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	pApp->AddNewManOperator("��ѹ��Ͷ��ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);
	WriteLog("CPTSoftBoard::OnSTTP20118 " + m_pObj->m_sName + " " + m_pObj->m_sID, XJ_LOG_LV3);

	//�ر�ԤУ��ʱ��
	KillTimer(m_nTimer);
	
	CString strOutput = m_strOutPut;
	
	CString str;
	//����ȷ��
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//�ɹ�
		strOutput += StringFromID(IDS_EXECUTE_MODIFYSOFT_CONFIRM);
		//��ʾ�û��������
		//�໤��ȷ��
// 		CDlgCheck dlg(this, 3);
// 		dlg.m_strModify = strOutput;
// 		if (dlg.DoModal() == IDOK)
		{
			
			//����ִ�б���
			//�齨����
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20113SoftBoardModifyExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);
			
			//���ͱ���
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//�ж��Ƿ�ɹ�
			if(nReturn != 1)
			{
				//ʧ��
				if(nReturn == -1)
				{
					str.Format("����[%s]������ѹ��Ͷ��ִ�б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					str.Format("����[%s]������ѹ��Ͷ��ִ�б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}else{
					str.Format("����������ѹ��Ͷ��ִ�б���ʧ��: [%]", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				KillTimer(m_nPTSetTimer);
				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

				//�ı�״̬
				m_nCurrentStatus = 0;
				//�ظ��޸�ǰ��ֵ
				RevertModifyValue();
				UpdateControlsEnable();

				pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, m_sOperUser.GetBuffer(0)
					, QByteArray(str.GetBuffer(0)));
				
				// ������ʱ
				m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);

				return;
			}
			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeSBTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
// 		else
// 		{
// 			//����ִ��
// 			//�޸�״̬Ϊ����
// 			m_nCurrentStatus = 0;
// 			m_bMonVerify = false;
// 			m_bOperVerify = false;
// 			//�ظ��޸�ǰ��ֵ
// 			RevertModifyValue();
// 		}
	}
	else
	{
		KillTimer(m_nPTSetTimer);
		//ʧ��
		CString str;
		str.Format("%s", "��ѹ��Ͷ��ԤУʧ��");
		WriteLog(str);
		AfxMessageBox(str);

		strOutput += StringFromID(IDS_VERIFY_MODIFYSOFT_FAIL);
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//�޸�״̬Ϊ����
		m_nCurrentStatus = 0;
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		//��ʾ�û��������
		//AfxMessageBox(strOutput, MB_OK);

		pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, m_sOperUser.GetBuffer(0)
			, QByteArray(str.GetBuffer(0)));
		
		// ������ʱ
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}
	UpdateControlsEnable();

	m_bChecking = FALSE;
}

/*************************************************************
 �� �� ����OnSTTP20114()
 ���ܸ�Ҫ����Ӧ��ѹ��Ͷ��ִ�б���(20114)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8B004E
void CPTSoftBoard::OnSTTP20114( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//����Ǵ�����ѹ��Ͷ��״̬
	if(m_nCurrentStatus != 2)
	{
		//����ѹ��״̬
		return;
	}
	
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//����Ƿ�20114����
	if(pSttpData->sttp_head.uMsgID != 20114)
	{
		//������
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20114 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20114 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, pSttpData->sttp_body.nCpu, m_arrModifyList.GetSize());
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	pApp->AddNewManOperator("��ѹ��Ͷ��ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	WriteLog("CPTSoftBoard::OnSTTP20114 " + m_pObj->m_sName + " " + m_pObj->m_sID, XJ_LOG_LV3);
	//�رն�ʱ��
	KillTimer(m_nTimer);
	

	//�޸�״̬
	m_nCurrentStatus = 0;
	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//�ɹ�
		//����޸ı��
		m_List.ClearEdited();
		AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_SUCCESS));
	}
	else
	{
		//ʧ��
		//�ظ��޸�ǰ��ֵ
		
		RevertModifyValue();
		AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_FAIL));
	}

	m_bMonVerify = false;
	m_bOperVerify = false;
	m_sOperUser = _T("");
	m_sMonUser = _T("");
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ, ˢ��ҳ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B8B0060
void CPTSoftBoard::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	int nIndex = (int)wParam;

	RefreshControls();
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_PTSB, "");
	//�Ƿ�ʹ�ÿ��ƹ���
	if(g_PTControl == 1)
	{
		if(g_PTControlModel)//�㶫ģʽ
		{
			m_btnVerify1.ShowWindow(SW_HIDE);
			m_btnVerify2.ShowWindow(SW_HIDE);
		}
		if(m_pObj->m_nCanControl == 0)
			m_btnModify.ShowWindow(SW_SHOW);
	}

	m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	m_btnModify.ShowWindow(SW_HIDE);

// 	CRect rcRect;
// 	((CWnd*)GetDlgItem(IDC_STATIC_PTSB_P3))->GetWindowRect(rcRect);
// 	ScreenToClient(&rcRect);
// 	GetDlgItem(IDC_BTN_PTSOFTSET_VIEWPROG)->SetWindowPos(NULL
// 		, rcRect.left + 3, rcRect.top, 0, 0
// 		,SWP_NOZORDER | SWP_NOSIZE );

	ReFillAll();
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ, �ڵ�һ�������͸��±���ʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8A01F4
void CPTSoftBoard::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return;
	//�������
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
 ���ܸ�Ҫ�����浱ǰѡ��CPU
 �� �� ֵ: void
 ��    ����param1	CPU������
**************************************************************/
//##ModelId=49B87B8A01F5
BOOL CPTSoftBoard::SaveSelCPU( CComboBox * pCmb )
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

//##ModelId=49B87B8A0399
void CPTSoftBoard::OnSelchangeCmbPtsoftCpu() 
{
	// TODO: Add your control notification handler code here
	//���浱ǰѡ���CPU
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return;

	if(SaveSelCPU(pCmb))
		ReFillAll();
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������LIST�����ı���ɫ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A039B
void CPTSoftBoard::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

				//����ֵ�б���ɫ,��ʾ���޸�
				if(nSubItem == m_nNewValueIndex)
				{
					//����ɫ����
					lplvcd->clrTextBk = RGB(200,255,255);
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

/*************************************************************
 �� �� ����OnClickListPtsoft()
 ���ܸ�Ҫ����Ӧ����¼�, �޸�����ֵ����ѹ��ֵ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A03AB
void CPTSoftBoard::OnClickListPtsoft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	//�Ƿ��ʹ�ÿ��ƹ���
	if(g_PTControl == 1)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//����ֵ��
				CString str;
				str.Format("%s;%s", DisplayValue(0), DisplayValue(1));
				m_List.ListSubItem(nItem, nSubItem, str);
			}
		}
	}
	*pResult = 0;
}

//##ModelId=49B87B8A03BA
void CPTSoftBoard::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ�� 
		KillTimer(m_nTimer);
		//��ʾ
		CString str = "";
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		switch(m_nCurrentStatus)
		{
		case 1:
			//str = "�ٻ���ѹ�峬ʱ!";
			
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
			pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);
			break;
		case 2:
			//str = "��ѹ��Ͷ�˳�ʱ!";
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("��ѹ��Ͷ��ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("��ѹ��Ͷ��ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		default:
			break;
		}
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���ѹ��
		}
		if(m_nCurrentStatus == 2)
		{
			//������ѹ��Ͷ��

			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
		}
		//�ı�״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(str);
	}

	if (nIDEvent == m_nPTSetTimer){
		KillTimer(m_nPTSetTimer);
		
		CString str;
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		
		CXJUserStore *pUserStore = CXJUserStore::GetInstance();
		CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
		QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
		
		int nPTSetState = pTagOutState->GetStateID();
		int nHangoutType = pTagOutState->GetType();
		
		QByteArray baOperUserID;
		QByteArray baRunUserID;
		
		baOperUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
		baRunUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT);
		CString sRunnerUserID = baRunUserID.constData();
		CString sOperUserID = baOperUserID.constData();
		
		// ��ѹ��Ͷ�˰�ť�Ƿ���ã� 
// 		if (XJ_OPER_HANGOUT == nPTSetState 
// 			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
// 			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID
// 			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER))){
// 			m_btnModify.EnableWindow(TRUE);
// 		}else{
// 			m_btnModify.EnableWindow(FALSE);
// 		}
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
			&& qstrcmp(baRunUserID.constData(), pApp->m_User.m_strUSER_ID.GetBuffer(0)) != 0){
			m_btnModify.EnableWindow(TRUE);
		}else{
			m_btnModify.EnableWindow(FALSE);
		}
		
		// ��ѹ��Ͷ�˰�ť�Ƿ�ɼ�
// 		if (XJ_OPER_UNHANGOUT != nPTSetState 
// 			&& CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
// 			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
// 			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
// 			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
// 			m_btnModify.ShowWindow(SW_SHOW);
// 		else
// 			m_btnModify.ShowWindow(SW_HIDE);
		if (XJ_OPER_UNHANGOUT != nPTSetState 
			&& CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
			&& pUserStore->hasFuncID(XJ_OPER_PTSOFTSET_STATE_2, pApp->m_User.m_strUSER_ID.GetBuffer(0)))
			m_btnModify.ShowWindow(SW_SHOW);
		else
			m_btnModify.ShowWindow(SW_HIDE);
		
		// �鿴���ƽ��Ȱ�ť�Ƿ�ɼ�
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType){
			m_btnViewProg.ShowWindow(SW_SHOW);
			if (XJ_TAGOUT_PTSOFTSET != nHangoutType)
				m_btnViewProg.SetWindowText("�鿴�޸Ľ���");
		}
		else
			m_btnViewProg.ShowWindow(SW_HIDE);
		
		QByteArray &thisComputer = CXJUtilsStore::GetInstance()->GetComputerName();
		if (5 == m_pObj->m_nRunStatu/* && pApp->m_User.m_strUSER_ID == sOperUserID*/){

			QByteArray msg;
			if (XJ_OPER_PTSOFTSET_STATE_4 == nPTSetState)
				msg << "����Ա����֤��ѹ���޸����ݣ��޸����ݽ��·�����վ������<ȷ��>��ִ���޸�";
			else if (!pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)
				&& XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState)
				msg << "�໤Ա����֤��ѹ���޸����ݣ��޸����ݽ��·�����վ������<ȷ��>��ִ���޸�";
// 			if (XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState && 0 == m_nCurrentDetailStatus
// 				&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer){
			if (((XJ_OPER_PTSOFTSET_STATE_4 == nPTSetState) 
				|| (!pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)
				&& XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState)) 
				&& 0 == m_nCurrentDetailStatus
				&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer)
			{
				AfxMessageBox("�໤Ա����֤��ѹ���޸����ݣ��޸����ݽ��·�����վ������<ȷ��>��ִ���޸�"
					, MB_OK|MB_ICONINFORMATION);	
				m_nCurrentDetailStatus = 1;
				
				pTagOutStore->ReLoadState();
				nPTSetState = pTagOutState->GetStateID();
				if (XJ_OPER_UNHANGOUT == nPTSetState){
					AfxMessageBox("����Ա�Ѿ�ȡ������Ӧװ�õĹ��ƣ��˴ζ�ֵ���޸�ȡ����");
					RevertModifyValue();
					//pStore->RevertModify();
					m_nCurrentDetailStatus = 0;
				}else{
					pTagOutState->SetFlags(2);		// �������ʱȡ�����Ʋ���
					ExcutePTSet_Soft();
				}
			}
		}

		if (pTagOutState->GetFlags() == 1 && CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			//&& pApp->m_User.m_strUSER_ID == sOperUserID
			&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer){
			// ֻ���޸���ѹ����û��Ĺ���վ��ִ�н���ظ�����
			KillTimer(m_nPTSetTimer);

			RevertModifyValue();
			pTagOutState->SetFlags(0);
			pTagOutState->Save();

			// ���ö�ʱ��
			m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
		}
		
		// ���ö�ʱ��
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}

	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSB, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ����Ӧ�رմ�����Ϣ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8B006F
void CPTSoftBoard::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8A0232
BOOL CPTSoftBoard::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_SHOW);

	//�������
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearSoftBoardConfig();
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//��ȡ����
	if(MODE_SUB == g_role)
	{
		//ֱ�Ӷ�ȡ
		LoadPTSoftBoardConfig(0);
		LoadPTHistorySoftBoardData(0);
		AddValueCol(0);
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_HIDE);
	}
	else
	{
		//�����߳�
		StartThread();
	}

	return TRUE;
}

/*************************************************************
 �� �� ����ClearDataCol()
 ���ܸ�Ҫ������¼ӵ�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0203
void CPTSoftBoard::ClearDataCol()
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
//##ModelId=49B87B8A0204
void CPTSoftBoard::CancelCall()
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
		//�����ٻ���ѹ��

		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20108, m_pObj->m_sID, atoi(m_sCPU));
	}
	else if(m_nCurrentStatus == 2)
	{
		//������ѹ��Ͷ��

		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20117, m_pObj->m_sID, atoi(m_sCPU));
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20113, m_pObj->m_sID, atoi(m_sCPU));
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����SetSoftBoardValue()
 ���ܸ�Ҫ������ָ������ѹ��ֵ
 �� �� ֵ: void
 ��    ����param1	��ѹ��ID
		   Param2	��ѹ��ֵ
		   Param3	������
**************************************************************/
//##ModelId=49B87B8A0213
void CPTSoftBoard::SetSoftBoardValue( CString ID, CString VALUE, int iCol )
{
	//���ҵ�DI_ID==ID������
	CString strID = ID;
	strID.TrimLeft();
	strID.TrimRight();
	LVFINDINFO  fi;
	fi.flags = LVFI_STRING;
	fi.psz = strID;
	
	//���Ҵ�ֵ�ڵڼ���
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return;
	
	//����ֵ
	PT_SOFTBOARD *pst = m_arrSoftBoard.GetAt(nItem);
	
	//����ListCtrl��Value������
	m_List.SetItemText(nItem, m_nNewValueIndex, DisplayValue(atoi(pst->Value)));
}

/*************************************************************
 �� �� ����RevertModifyValue()
 ���ܸ�Ҫ���ظ��޸���ѹ��ֵʱ�޸ĵ�ֵ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0222
void CPTSoftBoard::RevertModifyValue()
{
	if(m_arrModifyList.GetSize() < 0)
		return;
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);
		SetSoftBoardValue(strID, "", 0);
	}
	//����޸ı��
	m_List.ClearEdited();
	m_nCurrentDetailStatus = 0;
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0233
void CPTSoftBoard::EndOwnerThread()
{
	WriteLog("��ʼ�˳�������ѹ��ҳ���ȡ�����߳�", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳�������ѹ��ҳ���ȡ�����߳����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B8A0223
BOOL CPTSoftBoard::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20108 && m_nCurrentStatus == 1)
	{
		//�ٻ���ѹ��
		return TRUE;
	}
	if(nMsgID == 20113 && m_nCurrentStatus == 2)
	{
		//��ѹ��Ͷ��
		return TRUE;
	}
	if(nMsgID == 20117 && m_nCurrentStatus == 2)
	{
		//��ѹ��Ͷ��
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B8A03C8
BOOL CPTSoftBoard::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTSOFT);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����DisplayValue()
 ���ܸ�Ҫ���õ���ѹ���ֵ��ʾ�ַ���
 �� �� ֵ: ��ʾֵ
 ��    ����param1	��ѹ��ֵ
		   Param2
**************************************************************/
//##ModelId=49B87B8A0225
CString CPTSoftBoard::DisplayValue( int nValue )
{
	//��ѹ��ֵ��ʾ���: 0: 1/0, 1: ON/OFF, 3: Ͷ/��.
	if(0 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return "1";
		else if(0 == nValue)
			return "0";
	}
	if(1 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return "ON";
		else if(0 == nValue)
			return "OFF";
	}
	if(2 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_COMMISSIONING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CEASING);
	}
	CString str;
	str.Format("%d", nValue);
	return str;
}

//##ModelId=49B87B8B007F
void CPTSoftBoard::OnRefreshPTSoft( WPARAM wParam, LPARAM lParam )
{
	ReFillAll();	
}

//##ModelId=49B87B8A03CB
void CPTSoftBoard::OnBtnPtsoftStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B8A029F
void CPTSoftBoard::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_SOFT));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_SOFT));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B8A0234
CString CPTSoftBoard::TranslateSingleKey( CString sKey )
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
//##ModelId=49B87B8A0242
BOOL CPTSoftBoard::InitCOM()
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
//##ModelId=49B87B8A0243
void CPTSoftBoard::ReleaseCOM()
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
 �� �� ����DoPrintSoftBoard()
 ���ܸ�Ҫ����ӡ��ѹ��
 �� �� ֵ: 
 ��    ����param1	��ӡģ���ļ�
		   Param2
**************************************************************/
//##ModelId=49B87B8A0244
void CPTSoftBoard::DoPrintSoftBoard( CString strPath )
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
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTSoftBoard);
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

//##ModelId=49B87B8A03D8
void CPTSoftBoard::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSoftBoard(SOFTBOARD_NEW1_TP);
}

//##ModelId=49B87B8A03DA
void CPTSoftBoard::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSoftBoard(SOFTBOARD_NEW2_TP);
}

//##ModelId=49B87B8A03DC
void CPTSoftBoard::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSoftBoard(SOFTBOARD_NEW3_TP);
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8B005D
void CPTSoftBoard::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
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
		//�����ٻ���ѹ��				
		int nTimeOut = pData->m_nPTSoftBoardTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//����Ͷ����ѹ��
		int nTimeOut = pData->m_nChangeSBTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTSoftBoard::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTSoftBoard::OnDoubleclickedBtnPtsoftCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSoftBoard::OnBtnPtsoftVerify1() 
{
		m_nOperationNum = GetOperationNum();
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(2);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{
		m_sOperUser = dlgID.m_strUser;
		m_bOperVerify = true;
		m_btnVerify1.SetIcon(IDI_TICK);
		m_btnVerify2.EnableWindow(TRUE);
	}
}

void CPTSoftBoard::OnBtnPtsoftVerify2() 
{
		m_nOperationNum = GetOperationNum();
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
	if(dlgID.DoModal() == IDOK)
	{
		m_sMonUser = dlgID.m_strUser;
		if (!m_sMonUser.CompareNoCase(m_sOperUser))
		{
			AfxMessageBox(StringFromID(IDS_VALIDATE_MULTI_IDENTITY));
			return;
		}
		m_bMonVerify = true;
		m_btnVerify2.SetIcon(IDI_TICK);
		if (m_bOperVerify)
		{
			m_btnModify.EnableWindow(TRUE);
		}
	}
}

/****************************************************
Date:2012/3/13  Author:LYH
������:   FindNewValue	
����ֵ:   CString	
���ܸ�Ҫ: 
����: int nID	
*****************************************************/
CString CPTSoftBoard::FindNewValue( int nID )
{
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* pts = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(pts == NULL)
			continue;
		if(atoi(pts->ID) == nID)
			return pts->Value;
	}
	return "";
}

void CPTSoftBoard::OnDblclkListPtsoft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	//�Ƿ��ʹ�ÿ��ƹ���
	if(g_PTControl == 1)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//����ֵ��
				CString str;
				str.Format("%s;%s", DisplayValue(0), DisplayValue(1));
				m_List.ListSubItem(nItem, nSubItem, str);
			}
		}
	}
	*pResult = 0;
}

/****************************************************
Date:2012/12/6  Author:LYH
������:   FindSoftBoard	
����ֵ:   PT_SOFTBOARD*	
���ܸ�Ҫ: 
����: CString sID	
*****************************************************/
PT_SOFTBOARD* CPTSoftBoard::FindSoftBoard( CString sID )
{
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* ps = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(ps == NULL)
			continue;
		if(ps->ID == sID)
			return ps;
	}
	return NULL;
}



void CPTSoftBoard::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTSoftBoardLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("������ѹ���ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTSoftBoard::GetNextQueryNo()
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

void CPTSoftBoard::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("��ʼ�˳�������ѹ���ѯ�߳�,��ѯ��:%d", nNo);
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
	
	str.Format("������ѹ���ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTSoftBoard::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("������ѹ���ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTSoftBoard::GetFirstNode()
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

void CPTSoftBoard::AddValueCol( int nQueryNo )
{
	if(g_PTDIValues <= 0)
		return;
	if(m_arrSoftBoard.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//����ֵ��
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTSoftBoard::AddValueCol, ��ʼ����ֵ��", XJ_LOG_LV3);
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
	
	WriteLog("CPTSoftBoard::AddValueCol, ����ֵ�����", XJ_LOG_LV3);
}

void CPTSoftBoard::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	AddValueCol(wParam);
	FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_HIDE);
}

int CPTSoftBoard::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTSoftBoard::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTSoftBoard::LoadDefaultCpu, ��ѯ��ʼ", XJ_LOG_LV3);
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadDefaultCpu, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSoftBoard::LoadDefaultCpu,��ѯ��%d������", nCount);
		WriteLogEx(str);
		if( nCount >= 1 )
		{
			str = pMemset.GetValue((UINT)0); //ID
			int c = atoi(str);
			for( int k = 0; k < pCmb->GetCount(); k++ )
			{
				CSecCPU * cpu = (CSecCPU *)pCmb->GetItemData(k);
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
		str.Format("CPTSoftBoard::LoadDefaultCpu,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTSoftBoard::LoadDefaultCpu, ��ѯ����", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTSoftBoard::OnBtnPtsoftExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_SOFT),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
