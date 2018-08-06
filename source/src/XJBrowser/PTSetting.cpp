// PTSetting.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSetting.h"

#include "DlgCheck.h"
#include "DlgCheckPro.h"
#include "DlgValidateID.h"
#include "GlobalFunc.h"
#include "DlgSettingDetail.h"
#include "ListCellEdit.h"

#include "MainFrm.h"
#include "DlgOperHis.h"

#include "XJPTSetStore.h"
#include "qptsetstatetable.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

#define CURRENT_ZONE	99

UINT PTSettingLoad(LPVOID pParam)
{
	CPTSetting* ptSetting = (CPTSetting*)pParam;
	if(ptSetting == NULL)
	{
		return 0;
	}
	int nQueryNo = ptSetting->GetCurrentQueryNo();
	int nLoadType = ptSetting->GetLoadType();
	

	if(!ptSetting->m_bExitThread && ptSetting->IsCurrentQueryNo(nQueryNo))
	{
		BOOL bLoad = ptSetting->ReLoadData(nQueryNo, nLoadType);
		
		ptSetting->PostMessage(THREAD_FILL_DATA, nQueryNo, nLoadType);
	}
		
	ptSetting->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTSetting

IMPLEMENT_DYNCREATE(CPTSetting, CViewBase)

//##ModelId=49B87B8B0234
CPTSetting::CPTSetting()
	: CViewBase(CPTSetting::IDD)
{
	//{{AFX_DATA_INIT(CPTSetting)
	m_sCurrentZone = _T("0");
	m_sEditZone = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_arrSetting.RemoveAll();
	m_arrGroup.RemoveAll();
	m_nNewValueIndex = 11;
	m_nCurrentStatus = 0;
	m_nCurrentDetailStatus = 0;
	m_arrModifyList.RemoveAll();
	m_nSrc = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_bCanGroup = FALSE;
	m_nNewZoneIndex = 6;
	m_nOperationNum = -1;
	m_arrNewSetting.RemoveAll();
	m_arrFrame.RemoveAll();
	m_nFrameCount = 0;

	m_arrColum.RemoveAll();

	m_bExitThread = FALSE;
	m_pLoadThread = NULL;

	m_nQueryNo = 0;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	m_sOperUser = _T("");
	m_sMonUser = _T("");
	m_sRunUser = _T("");
	m_sOldZoneValue = _T("");
	m_bOperVerify = false;
	m_bMonVerify = false;
	m_bCallZoneSuccess = false;
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	m_bAlreadyShowOnce = false;

	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8D03C9
CPTSetting::~CPTSetting()
{
	ClearModifyData();
	ClearSettingGroup();
	ClearSettingConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8D033C
void CPTSetting::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTSetting)
	DDX_Control(pDX, IDC_LIST_PTZONE, m_ListZone);
	DDX_Control(pDX, IDC_BTN_PTSET_VERIFY2, m_btnVerify2);
	DDX_Control(pDX, IDC_BTN_PTSET_VERIFY1, m_btnVerify1);
	DDX_Control(pDX, IDC_GIF_PTSET, m_gif);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY_VIEWPROG, m_btnViewPTSetProg);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY_VIEW_HIS, m_btnViewPTSetHis);
	DDX_Control(pDX, IDC_BTN_PTSET_ZONE_MODIFY, m_btnModifyZone);
	DDX_Control(pDX, IDC_BTN_PTSET_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTSET_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY, m_btnModifySetting);
	DDX_Control(pDX, IDC_BTN_PTSET_CALLZONE, m_btnCallZone);
	DDX_Control(pDX, IDC_BTN_PTSET_CALL, m_btnCallSetting);
	DDX_Control(pDX, IDC_LIST_PTSET, m_List);
	DDX_Control(pDX, IDC_TAB_PTSET, m_TabCtrl);
	DDX_Text(pDX, IDC_EDIT_PTSET_CURZONE, m_sCurrentZone);
	DDX_Text(pDX, IDC_EDIT_PTSET_EDITZONE, m_sEditZone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTSetting, CViewBase)
	//{{AFX_MSG_MAP(CPTSetting)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTSET_CALL, OnBtnPtsetCall)
	ON_BN_CLICKED(IDC_BTN_PTSET_CALLZONE, OnBtnPtsetCallzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY, OnBtnPtsetModify)
	ON_BN_CLICKED(IDC_BTN_PTSET_ZONE_MODIFY, OnBtnPtsetZoneModify)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY_VIEWPROG, OnBtnViewProg)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY_VIEW_HIS, OnBtnViewHis)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PTSET, OnSelchangeTabPtset)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_CPU, OnSelchangeCmbPtsetCpu)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_SRC, OnSelchangeCmbPtsetSrc)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_SELZONE, OnSelchangeCmbPtsetSelzone)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSET, OnCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTZONE, OnCustomDraw1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTSET, OnClickListPtset)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTSET_STYLE, OnBtnPtsetStyle)
	ON_BN_CLICKED(IDC_BTN_PTSET_PRINT, OnBtnPtsetPrint)
	ON_COMMAND(ID_SETTINGPRINT_NEW1, OnSettingprintNew1)
	ON_COMMAND(ID_SETTINGPRINT_NEW2, OnSettingprintNew2)
	ON_COMMAND(ID_SETTINGPRINT_NEW3, OnSettingprintNew3)
	ON_COMMAND(ID_SETTINGPRINT_ORDER_1, OnSettingprintOrder1)
	ON_COMMAND(ID_SETTINGPRINT_ORDER_2, OnSettingprintOrder2)
	ON_COMMAND(ID_SETTINGPRINT_ORDER3, OnSettingprintOrder3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSET_CALL, OnDoubleclickedBtnPtsetCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSET_CALLZONE, OnDoubleclickedBtnPtsetCallzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_VERIFY1, OnBtnPtsetVerify1)
	ON_BN_CLICKED(IDC_BTN_PTSET_VERIFY2, OnBtnPtsetVerify2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTSET, OnDblclkListPtset)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTZONE, OnClickListPtzone)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTZONE, OnDblclkListPtzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_EXCEL, OnBtnPtsetExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
    ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
	//ON_MESSAGE(WM_GETTIP, ShowTip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSetting diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8E004E
void CPTSetting::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8E00BB
void CPTSetting::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSetting message handlers

//##ModelId=49B87B8D02D0
void CPTSetting::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	//����һ��
	RecordRate(IDC_STATIC_PTSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSET_CURZONE, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTSET_CURZONE, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSET_SELZONE, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_SELZONE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSET_CPU, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_CPU, 0, left_client, top_client);

	
	//TAB��
	RecordRate(IDC_TAB_PTSET, 1, left_client, top_client);
	//����һ�Ű�ť
	RecordRate(IDC_BTN_PTSET_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_CALLZONE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_SETTING_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_SETTING_P2, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_STYLE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY1, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY2, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_ZONE_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTSET, 0, left_client, top_client);
    RecordRate(IDC_REMIND, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTSETTING, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_EXCEL, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSETTING_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	m_btnVerify1.SetIcon(IDI_KEY1);
	m_btnVerify1.SetFlat(FALSE);
	
	m_btnVerify2.SetIcon(IDI_KEY1);
	m_btnVerify2.SetFlat(FALSE);

	if(g_PTControlModel == 1)
	{
		//������֤Ȩ�ް�ť
		m_btnVerify1.ShowWindow(SW_HIDE);
		m_btnVerify2.ShowWindow(SW_HIDE);
	}
	//��ʼ���б�
	InitListCtrl();
	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		InitZoneListCtrl();
	}
	//��ʼ�������ؼ�
	InitControls();
	//�ж�����/��ʾ ����TAB��
	if(g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//��ʾ
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_SHOW);
	}
	else
	{
		//����
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_HIDE);
	}

	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}

	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		m_List.ShowWindow(false);
		m_ListZone.ShowWindow(true);
	}
	else
	{
		m_ListZone.ShowWindow(false);
	}
}

//##ModelId=49B87B8E0138
void CPTSetting::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(g_PTSettingGroup != 1 && g_PTControl != 1) //��ֵ������
	{
		MoveCtrl(IDC_LIST_PTSET, left_client, IDC_BTN_PTSET_EXCEL, right_client, bottom_client);
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}
	}
	if(g_PTSettingGroup == 1 || g_PTControl == 1) //��ֵ����
	{
		MoveCtrl(IDC_LIST_PTSET, left_client, IDC_TAB_PTSET, right_client, bottom_client);
		MoveCtrl(IDC_LIST_PTZONE, left_client, IDC_TAB_PTSET, right_client, bottom_client);
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}

		if(m_ListZone.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_ListZone.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_ListZone.MoveWindow(rc);
		}
	}
}

/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�,ָ��Ĭ���п�
 �� �� ֵ: ʧ�ܷ���-1,�ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8C002E
int CPTSetting::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();

	return 0;
}

int CPTSetting::InitZoneListCtrl()
{
	if(m_ListZone.GetSafeHwnd() == NULL)
		return -1;
	InitZoneListStyle();
	
	return 0;
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B8C009C
int CPTSetting::InitListStyle()
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
		//char* arColumn[11]={"���","��ֵ����", "��ֵ����", "��λ", "��׼ֵ", "���", "��Ŀ��","����", "��Сֵ/���ֵ", "����", "��������"};
		//�����п�,��С����
		for (int nCol=0; nCol < 11 ; nCol++)
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
				lvCol.cx = 140; // ��ֵ����
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // ��ֵ����
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80; //��λ
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 80; //��׼ֵ
				colName = StringFromID(IDS_SETTING_ORDER);
				break;
			case 5:
				lvCol.cx = 60; //���
				colName = StringFromID(IDS_POINT_GROUP);
				break;
			case 6:
				lvCol.cx = 60; //��Ŀ��
				colName = StringFromID(IDS_POINT_ITEM);
				break;
			case 7:
				lvCol.cx = 60; //����
				colName = StringFromID(IDS_POINT_STEP);
				break;
			case 8:
				lvCol.cx = 80; //��Сֵ/���ֵ
				colName = StringFromID(IDS_POINT_MAXMIN);
				break;
			case 9:
				lvCol.cx = 60; //����
				colName = StringFromID(IDS_POINT_PRECISION);
				break;
			case 10:
				lvCol.cx = 80; //��������
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��ֵ����)
		m_List.SetColumnHide(2, TRUE);
		//Ĭ�����ص�5��(��׼ֵ)
		m_List.SetColumnHide(4, TRUE);
		if(!g_PTControl)
		{
			m_List.SetColumnHide(5, TRUE);
			m_List.SetColumnHide(6, TRUE);
			m_List.SetColumnHide(7, TRUE);
			m_List.SetColumnHide(8, TRUE);
			m_List.SetColumnHide(9, TRUE);
		}
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}


int CPTSetting::InitZoneListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListZone.SetImageList(&m_l,LVSIL_SMALL);
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	
	//��������ʧ��
	//char* arColumn[7]={"����","���", "��Ŀ��", "����", "����", "����", "��ǰֵ"};
	//�����п�,��С����
	for (int nCol=0; nCol < 7 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 140; //����
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 80; //���
			colName = StringFromID(IDS_POINT_GROUP);
			break;
		case 2://
			lvCol.cx = 80;  //��Ŀ��
			colName = StringFromID(IDS_POINT_ITEM);
			break;
		case 3:
			lvCol.cx = 80; //����
			colName = StringFromID(IDS_POINT_LOWERLIMIT);
			break;
		case 4:
			lvCol.cx = 80; //����
			colName = StringFromID(IDS_POINT_UPPERLIMIT);
			break;
		case 5:
			lvCol.cx = 80; //����
			colName = StringFromID(IDS_POINT_STEP);
			break;
		case 6:
			lvCol.cx = 100; //��ǰֵ
			colName = StringFromID(IDS_POINT_CURRENT_VALUE);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		int result = m_ListZone.InsertColumn(nCol,&lvCol);
	}
	
	//���÷��
	m_ListZone.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8C0157
BOOL CPTSetting::LoadListConfig()
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
			WriteLog("PTSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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
				WriteLog("PTSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTSettingConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�PTSettingConfig", XJ_LOG_LV3);
				WriteLog("PTSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�PTSettingConfig�µ�����", XJ_LOG_LV3);
					WriteLog("PTSetting::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
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

//##ModelId=49B87B8C01B5
BOOL CPTSetting::SaveListConfig()
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
			WriteLog("PTSetting::����UIConfigʧ��", XJ_LOG_LV3);
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
				WriteLog("PTSetting::����UIConfigʧ��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//�����ļ��ɹ�
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTSettingConfig");
				
				//����ViewActionConfig
				if(pSelected == NULL)
				{	
					//δ�ҵ�,�½��ڵ�
					WriteLog("δ�ҵ�PTSettingConfig, ���½��ڵ���ӽڵ�", XJ_LOG_LV3);	
					//�½��ڵ�
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTSettingConfig");
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
						WriteLog("δ�ҵ�VPTSettingConfig�µ�����, �½������ӽڵ�", XJ_LOG_LV3);
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
						WriteLog("�ҵ���PTSettingConfig�µ�����, ��ɾ���½������ӽڵ�", XJ_LOG_LV3);
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
//##ModelId=49B87B8C00FA
int CPTSetting::FillListData(int nQueryNo)
{
	WriteLog("CPTSetting::FillListData, ��ʼ", XJ_LOG_LV3);

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);  
	int nGroupCount = m_arrGroup.GetSize();
	PT_SETTING *pts = NULL;
	int nIndex = 0;
	for(int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(g_PTSettingGroup == 1 && nGroupCount > 0)
		{
			//������ʾ
			if(pts->Group != m_nGroup)
			{
				//��������ڵ�ǰ��ѡ���
				continue;
			}
		}
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", nIndex+1);
		}
		m_List.InsertItem(nIndex, sID); //ID
		CString strName;
		int z = pts->Name.Find(",", 0);
		if (z != -1)
		{
			strName = pts->Name.Left(z);
		}
		else
		{
			strName = pts->Name;
		}
		m_List.SetItemText(nIndex, 1, strName); //����
		m_List.SetItemText(nIndex, 2, pts->CodeName); //����
		m_List.SetItemText(nIndex, 3, pts->Unit); //��λ
		m_List.SetItemText(nIndex, 4, pts->OrderValue); //��׼ֵ
		CString str;
		str.Format("%d", pts->Group);
		m_List.SetItemText(nIndex, 5, str);
		str.Format("%d", pts->Item);
		m_List.SetItemText(nIndex,6, str);
		str.Format("%s", pts->stepValue);
		m_List.SetItemText(nIndex, 7, str);
		
		str.Format("%s/%s", pts->minValue, pts->maxValue);
		m_List.SetItemText(nIndex, 8, str);
		m_List.SetItemText(nIndex, 9, pts->Precision);
		switch (pts->DataType)
		{
       //0-���� 1-���� 2-������(ʮ������) 3-�ַ��� 4-������(������)
		case 0:
			str = StringFromID(IDS_DATATYPE_FLOAT);
			break;
		case 1:
			str = StringFromID(IDS_DATATYPE_INT);
			break;
		case 2:
			str = StringFromID(IDS_DATATYPE_CONTROL16);
			break;
		case 3:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		case 4:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		default:
			str = _T("");
			break;
		}
		m_List.SetItemText(nIndex, 10, str);
		m_List.SetItemData(nIndex, (DWORD)pts);
		nIndex++;
	}
	//�ָ�ˢ��
	LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);

	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//���ListCtrl����
		m_ListZone.DeleteAllItems();
		//�������ʱ��ֹˢ��
		m_ListZone.SetRedraw(FALSE);
		CSecCPU* pCpu = NULL;
		for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
		{
			pCpu = m_pObj->m_arrCPU[k];
			if (pCpu->code == atoi(m_sCPU))
			{
				break;
			}
		}
		MYASSERT_INT(pCpu);
		CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
		if(m_pObj->m_bNewDevice)
		{
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				//	char* arColumn[7]={"����","���", "��Ŀ��", "����", "����", "����", "��ǰֵ"};
				PT_ZONE * ptz = arrZONE[i];
				CString str;
				m_ListZone.InsertItem(i, ptz->zone_name); //ID
				str.Format("%d", ptz->group);
				m_ListZone.SetItemText(i, 1, str);
				str.Format("%d", ptz->item);
				m_ListZone.SetItemText(i, 2, str);
				str.Format("%d", ptz->minValue);
				m_ListZone.SetItemText(i, 3, str); 
				str.Format("%d", ptz->maxValue);
				m_ListZone.SetItemText(i, 4, str);
				str.Format("%d", ptz->stepValue);
				m_ListZone.SetItemText(i, 5, str);
				str.Format("%d", ptz->zoneValue);
				m_ListZone.SetItemText(i, 6, str);
				m_ListZone.SetItemData(i, (DWORD)ptz);
			}
		}
		else
		{
			int nMin = 10000;
			int nMax = -1;
			
			nMin = pCpu->from_zone<=pCpu->to_zone?pCpu->from_zone:pCpu->to_zone;
			nMax = pCpu->from_zone>=pCpu->to_zone?pCpu->from_zone:pCpu->to_zone;

			CString str;
			m_ListZone.InsertItem(0, StringFromID(IDS_SETTING_CURRENTZONE)); //ID
			m_ListZone.SetItemText(0, 1, "-");
			m_ListZone.SetItemText(0, 2, "-");
			str.Format("%d", nMin);
			m_ListZone.SetItemText(0, 3, str); 
			str.Format("%d", nMax);
			m_ListZone.SetItemText(0, 4, str);
			m_ListZone.SetItemText(0, 5, "-");
			m_ListZone.SetItemText(0, 6, m_sCurrentZone);
			//m_ListZone.SetItemData(i, (DWORD)ptz);
		}
		
		//�ָ�ˢ��
		m_ListZone.SetRedraw(TRUE);
	}

	WriteLog("CPTSetting::FillListData, ����", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B8C0222
BOOL CPTSetting::NeedSave()
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
//##ModelId=49B87B8C0271
int CPTSetting::InitControls()
{
	/*
	//�ٻ���ֵ��ť
	m_btnCallSetting.SetThemeHelper(&m_ThemeHelper);
	m_btnCallSetting.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallSetting.SetTooltipText(_T("�ٻ���ֵ"));

	//�ٻ���ֵ����
	m_btnCallZone.SetThemeHelper(&m_ThemeHelper);
	m_btnCallZone.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallZone.SetTooltipText(_T("�ٻ���ֵ����"));

	//��ʾ���ð�ť
	m_btnStyle.SetThemeHelper(&m_ThemeHelper);
	m_btnStyle.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStyle.SetTooltipText(_T("������ʾ���"));
*/
	//��ӡ��ť
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText( StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_SETTING_PRINT, GetSafeHwnd());
/*
	//�л���ֵ���Ű�ť
	m_btnModifyZone.SetThemeHelper(&m_ThemeHelper);
	m_btnModifyZone.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModifyZone.SetTooltipText(_T("�л���ֵ��"));

	//�޸Ķ�ֵ��ť
	m_btnModifySetting.SetThemeHelper(&m_ThemeHelper);
	m_btnModifySetting.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModifySetting.SetTooltipText(_T("�޸Ķ�ֵ"));
*/
	return 0;
}


//##ModelId=49B87B8E01B5
void CPTSetting::OnDestroy() 
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
//##ModelId=49B87B8C02CE
BOOL CPTSetting::ClearSettingConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	//�����ֵ����
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING * pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		delete pts;
		pts = NULL;
	}
	m_arrSetting.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);

	//�����ֵ������
// 	for(i = 0; i < m_arrGroup.GetSize(); i++)
// 	{
// 		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
// 		delete pg;
// 		pg = NULL;
// 	}
// 	m_arrGroup.RemoveAll();

	return TRUE;
}

/*************************************************************
 �� �� ����LoadSettingConfig()
 ���ܸ�Ҫ�������ݿ����뱣����ֵ����, �����ڶ�ֵ������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8C031D
BOOL CPTSetting::LoadPTSettingConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	WriteLog("CPTSetting::LoadPTSettingConfig,��ѯ��ʼ", XJ_LOG_LV3);
	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����PT_ID, cpu_code, zone, 103group(���Ҫ������ʾ�Ļ�)����
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

	//minvalue
	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "minvalue", EX_STTP_DATA_TYPE_FLOAT);

	//maxvalue
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "maxvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "stepvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "103group", EX_STTP_DATA_TYPE_INT);
	
	//stepvalue
	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "103item", EX_STTP_DATA_TYPE_INT);

	//stepvalue
	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "61850ref", EX_STTP_DATA_TYPE_INT);

	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "Reserve2", EX_STTP_DATA_TYPE_STRING);

	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

/*	int nGroupCount = 0;
	//103group
	if(g_PTSettingGroup == 1)
	{
		EnterCriticalSection(&m_CriticalOper);
		nGroupCount = m_arrGroup.GetSize();
		LeaveCriticalSection(&m_CriticalOper);
		//���������ʾ
		if(nGroupCount > 0)
		{
			Condition condition5;
			str.Format("103GROUP = %d", m_nGroup);
			pApp->m_DBEngine.SetCondition(sql, condition5, str);
		}
	}*/

	//��Setting_ID��С����
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //��where����	
		
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTSettingConfig, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTSetting::LoadPTSettingConfig,��ѯ��%d����¼", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//������ֵ����
			PT_SETTING * pts = new PT_SETTING;
			//setting_id,NAME,CODE_NAME,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = pMemset.GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset.GetValue(1); //NAME
			pts->CodeName = pMemset.GetValue(2); //Code_Name
			pts->VPickList = pMemset.GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = pMemset.GetValue(4); //unit
			pts->Precision = pMemset.GetValue(5); //s_precision
			str = pMemset.GetValue(6); //datatype
			pts->DataType = atoi(str);
			str = pMemset.GetValue(7);
			pts->minValue = str;//atof(str);
			str = pMemset.GetValue(8);
			pts->maxValue = str;//atof(str);
			str = pMemset.GetValue(9);
			pts->stepValue = str;//atof(str);
			str = pMemset.GetValue(10);
			pts->Group = atoi(str);
			str = pMemset.GetValue(11);
			pts->Item = atoi(str);
			str = pMemset.GetValue(12);
			pts->nRefType = 1;
			if(!str.IsEmpty())
			{
				str.MakeUpper();
				if(str.Find("$SP$", 0) != -1)
					pts->nRefType = 0;
			}
			else
			{
				CString sGroupName = GetGroupName(pts->Group);
				if(!sGroupName.IsEmpty())
				{
					if(sGroupName.Find( StringFromID(IDS_COMMON_PARAMETER), 0) != -1)
						pts->nRefType = 0;
				}
			}
			str = pMemset.GetValue(13);
			pts->ntimeMStoS = 0;
			if(!str.IsEmpty())
			{
				pts->ntimeMStoS = atoi(str);
			}

			//��ֵ��
		/*	if(g_PTSettingGroup == 1 && nGroupCount > 0)
			{
				pts->Group = m_nGroup;
			}*/

			m_arrSetting.Add(pts);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTSettingConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	WriteLog("CPTSetting::LoadPTSettingConfig,��ѯ����", XJ_LOG_LV3);

	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTGroupConfig()
 ���ܸ�Ҫ�����뱣����ֵ�����,������Ϣ
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8C036B
BOOL CPTSetting::LoadPTGroupConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;

	WriteLogEx("CPTSetting::LoadPTGroupConfig,��ѯ��ʼ");

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
		
	
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_103GROUP_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTGroupConfig, ��ѯʧ��");
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,��ѯ��%d������", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			CString str = pMemset.GetValue((UINT)0); //103group
			int nGroup = atoi(str);
			str = pMemset.GetValue(1); //name
			SetGroupName(nGroup, str);

			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLogEx("CPTSetting::LoadPTGroupConfig,��ѯ��������");
	
	return bReturn;
}

/*************************************************************
 �� �� ����LoadOrderValue()
 ���ܸ�Ҫ�����붨ֵ��׼ֵ��Ϣ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	
**************************************************************/
//##ModelId=49B87B8C03B9
BOOL CPTSetting::LoadOrderValue(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	WriteLog("CPTSetting::LoadOrderValue, ��ѯ��ʼ", XJ_LOG_LV3);
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�Ȳ������µ�һ�λ�׼ֵʱ��
	CString strTime = LoadNewOrderTime();
	if(strTime == "")
	{
		//û������ʱ��, ��ʾû�л�׼ֵ
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	//����PT_ID, CPU, Zone,Time ����Value(��׼ֵ)
	
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "ID", EX_STTP_DATA_TYPE_INT);
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "value", EX_STTP_DATA_TYPE_STRING);
		
	//����
	CString str = "";
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
		
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	//zone
	Condition condition3;
	int nzone = atoi(m_sZone);
	if( nzone == CURRENT_ZONE )
		nzone = atoi(m_sCurrentZone);
	str.Format("ZONE = %d", nzone);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//ʱ��
	Condition condition5;
	str.Format("TIME = '%s'", strTime);
	pApp->m_DBEngine.SetCondition(sql, condition5, str);
			
	
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ST_ORDERSETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadOrderValue, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadOrderValue,��ѯ��%d������", nCount);
		WriteLogEx(str);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(g_role != MODE_SUB && i%20 == 0)
				Sleep(1);
			//ȡ��һ��
			str = pMemset.GetValue((UINT)0); //ID
			PT_SETTING* pts = FindSetting(str);
			if(pts != NULL)
			{
				pts->OrderValue = pMemset.GetValue(1);
			}
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadOrderValue,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTSetting::LoadOrderValue, ��ѯ����", XJ_LOG_LV3);
	
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
//##ModelId=49B87B8D0021
BOOL CPTSetting::AddSettingValue( CString ID, CString VALUE, int iCol )
{
	EnterCriticalSection(&m_CriticalOper);
	//���ҵ�Setting_ID==ID������
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_SETTING* pst = (PT_SETTING*)FindSetting(strID);
	if(pst == NULL)
	{
		LeaveCriticalSection(&m_CriticalOper);
		return FALSE;
	}
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//���Ҵ�ֵ�ڵڼ���
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
	{
		LeaveCriticalSection(&m_CriticalOper);
		return FALSE;
	}
	
	//����ֵ
	pst->Value = VALUE;
	
	bReturn = m_List.SetItemText(nItem, iCol/*����ֵ*/, GetDisplayValue(pst, pst->Value));
	LeaveCriticalSection(&m_CriticalOper);
	return bReturn;
}

/*************************************************************
 �� �� ����LoadPTHistorySettingData()
 ���ܸ�Ҫ�����뱣����ʷֵ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B8D006D
BOOL CPTSetting::LoadPTHistorySettingData(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	WriteLog("CPTSetting::LoadPTHistorySettingData, ��ѯtime��ʼ", XJ_LOG_LV3);
	BOOL bReturn = TRUE;

	if(g_PTSettingValues <= 0)
		return TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//ȡ�����ݿ�������ʷ���ݵ�Time�б�
	CString str = _T("");
		
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

	int nzone = atoi(m_sZone);
	if( nzone == CURRENT_ZONE )
		nzone = atoi(m_sCurrentZone);
	Condition condition4;
	str.Format("ZONE = %d", nzone);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);

	//��ʱ���С����
	Condition condition5;
	str.Format("order by CURTIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);//��where
			
	
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTHistorySettingData, ��ѯʧ��");
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadPTHistorySettingData,��ѯtime,��ѯ��%d������", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			if(i >= g_PTSettingValues)
				break;

			CString str;
			str = pMemset.GetValue((UINT)0);
			EnterCriticalSection(&m_CriticalOper);
			m_HisValueMgr.CreateValue(str);
			LeaveCriticalSection(&m_CriticalOper);
					
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTHistorySettingData,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTSetting::LoadPTHistorySettingData, ��ѯtime����", XJ_LOG_LV3);
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	WriteLog("CPTSetting::LoadPTHistorySettingData, ��ѯ��ʷ���ݿ�ʼ", XJ_LOG_LV3);
	//���ÿһ��
	//�����ݿ����ÿ��Time��Ӧ�Ķ�ֵ��
	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);
	for (int i = 0; i < nHisCount; i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(g_role != MODE_SUB)
			Sleep(1);

		EnterCriticalSection(&m_CriticalOper);
		CString strTime = m_HisValueMgr.GetTime(i);
		LeaveCriticalSection(&m_CriticalOper);
		
		//�����ݿ����ÿ��Time��Ӧ����ʷֵ
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

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

		//zone
		Condition condition3;
		str.Format("ZONE = %d", nzone);
		pApp->m_DBEngine.SetCondition(sql, condition3, str);

		//CURTIME
		Condition condition4;
		str.Format("CURTIME = '%s'", strTime);
		pApp->m_DBEngine.SetCondition(sql, condition4, str);
				
		
		CMemSet pMemset;
			
		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLogEx("CPTSetting::LoadPTHistorySettingData, ��ѯʧ��");
			continue;
		}
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTSetting::LoadPTHistorySettingData,��ѯ%s����ʷ����,����Ϊ%d", strTime, nCount);
			WriteLogEx(str);

			EnterCriticalSection(&m_CriticalOper);
			for(int k = 0; k < nCount; k++)
			{
				if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
					break;
				if(g_role != MODE_SUB && k%20 == 0)
					Sleep(1);
				CString strID, strValue;
				strID = pMemset.GetValue((UINT)0);
				strValue = pMemset.GetValue(1);
				strValue.TrimLeft();
				strValue.TrimRight();

				m_HisValueMgr.AddValue(i, atoi(strID), strValue);

				pMemset.MoveNext();
			}
			LeaveCriticalSection(&m_CriticalOper);	
		}
		else
		{
			CString str;
			str.Format("CPTSetting::LoadPTHistorySettingData,��ѯʧ��,ԭ��Ϊ%s", sError);
			WriteLogEx(str);
			continue;
		}
	}
	WriteLog("CPTSetting::LoadPTHistorySettingData, ��ѯ��ʷ���ݽ���", XJ_LOG_LV3);
	return TRUE;
}

/*************************************************************
 �� �� ����OnBtnPtsetCall()
 ���ܸ�Ҫ���ٻ�������ֵ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0203
void CPTSetting::OnBtnPtsetCall() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ֵ��ȡ���ٻ�
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

	if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox( StringFromID(IDS_ZONE_UNKNOWN_EDITZONE));
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//����״̬, �����ť��ʾҪ�ٻ���ֵ
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20015����

		int nSendZone = atoi(m_sZone);
		//if(nSendZone == atoi(m_sCurrentZone))
		//	nSendZone = CURRENT_ZONE;
		
		if(!pApp->m_SttpEngine.BuildDataFor20015PTSetting(sttpData, m_pObj->m_sID, atoi(m_sCPU), nSendZone, m_nSrc))
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
				str.Format("����[%s]�����ٻ���ֵ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ���ֵ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ���ֵ
			m_nCurrentStatus = 1;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTSettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//�����˹�������־
			//pApp->AddManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sName);

			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����ٻ���ֵ���ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ֵ
		//ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 �� �� ����OnSTTP20016()
 ���ܸ�Ҫ����Ӧ20016(������ֵ����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8E034B
void CPTSetting::OnSTTP20016( WPARAM wParam,LPARAM lParam )
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
		str.Format("CPTSetting::OnSTTP20016 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20016 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20016 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//��鶨ֵ����
	int nZone = pSttpData->sttp_body.nZone;
	if(nZone != atoi(m_sZone) && nZone != CURRENT_ZONE && atoi(m_sZone) != CURRENT_ZONE)
	{
		//����ָ����ֵ���ŵĶ�ֵ
		CString str;
		str.Format("ָ���Ķ�ֵ����Ϊ%s, ���ͱ��ĵĶ�ֵ����Ϊ%d", m_sZone, nZone);
		WriteLog(str, XJ_LOG_LV3);
	}

	//�ж��ǵ�֡���Ƕ�֡
	if(pSttpData->sttp_head.uMsgType < 2)
	{
		//��֡,ֱ�Ӵ���
		//�رն�ʱ��
		KillTimer(m_nTimer);
		
		//������
		if(!LoadDataFromSTTP(pSttpData))
		{
			WriteLog("CPTSetting::OnSTTP20016 ������ʧ��", XJ_LOG_LV3);
		}
		
		//�������,�ı�״̬�Ͱ�ť����
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CString sLog;
		sLog.Format("����[%s]�ٻ���ֵ�ɹ�!", m_pObj->m_sName);
		WriteLog(sLog, XJ_LOG_LV2);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_SUCCESS);

		// ��������һ�ζ�ֵ�� �����
		//CXJPTSetStore::GetInstance()->SaveRecallToDB(m_sCPU, m_pObj->m_sID, m_arrSetting);
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

			CString sLog;
			sLog.Format("����[%s]�ٻ���ֵʧ��!", m_pObj->m_sName);
			WriteLog(sLog, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

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
			WriteLog(str, XJ_LOG_LV3);
			return;
		}
		else
		{
			//֮ǰ������,����
			CString str;
			str.Format("�յ���ֵ��֡����, ��֡��%d, ��ǰ�ǵ�%d֡", m_nFrameCount, nFrame);
			WriteLog(str, XJ_LOG_LV3);
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
//##ModelId=49B87B8D00BB
BOOL CPTSetting::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
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
			Value.TrimLeft();
			Value.TrimRight();
			Value = ComplexExchange(Value);
			AddSettingValue(id, Value, Index);
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, ����", XJ_LOG_LV3);
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
//##ModelId=49B87B8E038A
void CPTSetting::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4)
	{
		//û�д����ٻ���ֵ״̬,Ҳû�д����ٻ���ֵ����״̬,
		//Ҳû�д����޸Ķ�ֵ״̬, Ҳû�д��ڶ�ֵ���л�״̬,��������
		CString str;
		str.Format("CPTSetting::OnSTTP20069 ��ǰ״̬Ϊ%d", m_nCurrentStatus);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		WriteLog("CPTSetting::OnSTTP20069 ����Ϊ��", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//������
		WriteLog("CPTSetting::OnSTTP20069 ������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		WriteLog("CPTSetting::OnSTTP20069 ���Ǳ�װ�õı���", XJ_LOG_LV3);
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		CString str;
		str.Format("CPTSetting::OnSTTP20069 ��ǰ�ظ�����Ϊ%d,��ǰ״̬Ϊ%d", pSttpData->sttp_body.nMsgId, m_nCurrentStatus);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
	}
	else
	{
		//����ʧ��
		//�������,�ı�״̬�Ͱ�ť����
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���ֵ				
			CString str;
			str.Format("����[%s]�ٻ���ֵʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

			AfxMessageBox( StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//�����ٻ���ֵ����				
			CString str;
			str.Format("����[%s]�ٻ���ֵ����ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			m_bCallZoneSuccess = false;
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
			pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);

			if (m_bOperTow)
			{
				AfxMessageBox( StringFromID(IDS_CALLZONG_FAIL_BEFORE_MODIFY));
				m_bOperTow = false;
				m_bOperVerify = false;
				m_bMonVerify = false;
				UpdateControlsEnable();
			}
			else
			{
				AfxMessageBox( StringFromID(IDS_CALL_FAIL));
			}
		}
		else if(m_nCurrentStatus == 3)
		{
			//�����޸Ķ�ֵ
			m_bMonVerify = false;
			m_bOperVerify = false;
			CString str;
			str.Format("����[%s]�޸Ķ�ֵʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;

			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();


			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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
				
				pApp->AddNewManOperator("��ֵ�޸�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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

				pApp->AddNewManOperator("��ֵ�޸�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			AfxMessageBox( StringFromID(IDS_MODIFY_SETTING_FAIL));
		}
		else if(m_nCurrentStatus == 4)
		{
			//�����л���ֵ��
			m_bMonVerify = false;
			m_bOperVerify = false;
			CString str;
			str.Format("����[%s]��ֵ���л�ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
			AfxMessageBox( StringFromID(IDS_MODIFY_ZONE_FAIL));
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue(2);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)	
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("��ֵ���л�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("��ֵ���л�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
		}	
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֹ���)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8E03C8
void CPTSetting::OnSTTP20125( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
        WriteLog("CPTSetting::OnSTTP20125 ����Ϊ��,��������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20125����
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
 		//������
        WriteLog("CPTSetting::OnSTTP20125 ������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
        WriteLog("CPTSetting::OnSTTP20125 ���Ǳ�װ�õı���", XJ_LOG_LV3);
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		CString strInfo;
		strInfo.Format("pSttpData->sttp_body.nMsgId: %d   m_nCurrentStatus: %d", pSttpData->sttp_body.nMsgId, m_nCurrentStatus);
        WriteLog("CPTSetting::OnSTTP20125 ���Ǳ����ܵı���" + strInfo, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	//�������,�ı�״̬�Ͱ�ť����
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ���ֵ
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ���ֵ����
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
		pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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
			
			pApp->AddNewManOperator("��ֵ�޸�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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
			pApp->AddNewManOperator("��ֵ�޸�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
		}

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����л���ֵ��
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			if(g_OperTableType == 0)
				strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
			else if(g_OperTableType == 1)	
				strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
			pApp->AddNewManOperator("��ֵ���л�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			if(g_OperTableType == 0)
				strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
			else if(g_OperTableType == 1)
				strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
			pApp->AddNewManOperator("��ֵ���л�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
		}

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
}

/*************************************************************
 �� �� ����OnBtnPtsetCallzone()
 ���ܸ�Ҫ���ٻ���ǰ��ֵ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0242
void CPTSetting::OnBtnPtsetCallzone() 
{
	// TODO: Add your control notification handler code here
	//��Ӧ��ť���²���, �ٻ���ֵ���Ż�ȡ���ٻ�
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
		//����״̬, �����ť��ʾҪ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		if (m_pObj->m_bNewDevice)
		{
			//��֯20106����
			if(!pApp->m_SttpEngine.BuildDataFor20173PTZone(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
			{
				//��֯����ʧ��
				return;
			}
		}
		else
		{
			//��֯20106����
			if(!pApp->m_SttpEngine.BuildDataFor20106PTZone(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
			{
				//��֯����ʧ��
				return;
			}
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
				str.Format("����[%s]�����ٻ���ֵ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ���ֵ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����ٻ���ֵ����
			m_nCurrentStatus = 2;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//�����˹�������־
			//pApp->AddManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sName);

			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����ٻ���ֵ���ű��ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ���ֵ����
		//ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}

	
}

/*************************************************************
 �� �� ����OnSTTP20107()
 ���ܸ�Ҫ����Ӧ20107(�ٻ���ֵ����֪ͨ)����
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8F000F
void CPTSetting::OnSTTP20107( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//û�д����ٻ���ֵ���ŵ�״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20107����
	if(pSttpData->sttp_head.uMsgID != 20107)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20107 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	} 
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20107 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20107 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	int nZone = pSttpData->sttp_body.nZone;
	m_sCurrentZone.Format("%d", nZone);

	if (m_ListZone.GetItemCount() != 0)
	{
		if(!m_pObj->m_bNewDevice)
		{
			//ȷ�ϵ�һ��������ʾ��ǰ��ֵ��
			CString str;
			str.Format("%d", nZone);
			m_ListZone.SetItemText(0, m_nNewZoneIndex, str);
		}
	}

	//SetZoneCmbToCurrentZone();

	CString sLog;
	sLog.Format("����[%s]�ٻ���ֵ���ųɹ�!", m_pObj->m_sName);
	WriteLog(sLog, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ZONE, m_sCPU);			
	pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_SUCCESS);
	
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;

	//ˢ�½�����ʾ
	UpdateData(FALSE);

	//���Ŀؼ����ö�
	UpdateControlsEnable();
}


void CPTSetting::OnSTTP20174( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		WriteLog("CPTSetting::OnSTTP20174 û�д����ٻ���ֵ���ŵ�״̬,��������", XJ_LOG_LV3);
		//û�д����ٻ���ֵ���ŵ�״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20174����
	if(pSttpData->sttp_head.uMsgID != 20174)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20174 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20174 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//���CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//���Ǳ�CPU�ı���
		CString str;
		str.Format("CPTSetting::OnSTTP20174 �յ�����CPU����, CPUΪ%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	CSecCPU* pCpu = NULL;
	for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
	{
		pCpu = m_pObj->m_arrCPU[k];
		if (pCpu->code == nCpu)
		{
			break;
		}
	}
	MYASSERT(pCpu);

	bool bHasEditZone = false;
	CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;

	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		int id;
		id = pSttpData->sttp_body.variant_member.value_data[i].id;
		CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		for (int j = 0; j < arrZONE.GetSize(); j++)
		{
			PT_ZONE * ptz = arrZONE[j];
			if (ptz->code == id)
			{
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = Value;
					ptz->zoneValue = atoi(Value);
					LVFINDINFO lvFind;
					lvFind.flags = LVFI_PARAM;
					lvFind.lParam = (LPARAM)ptz;
					int nIndex = m_ListZone.FindItem(&lvFind);
					if(nIndex != -1 && m_nModifySettingOrModifyZone == 0)
					{
						m_ListZone.SetItemText(nIndex, m_nNewZoneIndex, Value);
					}
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = Value;
					ptz->zoneValue = atoi(Value);
					LVFINDINFO lvFind;
					lvFind.flags = LVFI_PARAM;
					lvFind.lParam = (LPARAM)ptz;
					int nIndex = m_ListZone.FindItem(&lvFind);
					if(nIndex != -1 && m_nModifySettingOrModifyZone == 0)
					{
						m_ListZone.SetItemText(nIndex, m_nNewZoneIndex, Value);
					}
					bHasEditZone = true;
				}
				break;
			}
		}
	}


	/*
	//������
	int nZone = pSttpData->sttp_body.nZone;
	m_sCurrentZone.Format("%d", nZone);
	*/
	if (!bHasEditZone)
	{
		AfxMessageBox( StringFromID(IDS_CALL_EDITZONE_FAIL));
		m_bCallZoneSuccess = false;
	}
	else
	{
		m_bCallZoneSuccess = true;
	}
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	//ˢ�½�����ʾ
	UpdateData(FALSE);
	
	//���Ŀؼ����ö�
	UpdateControlsEnable();
	WriteLog("CPTSetting::OnSTTP20174 �������", XJ_LOG_LV3);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ZONE, m_sCPU);			
	pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_SUCCESS);

	if (m_bOperTow)
	{
		if (!m_bCallZoneSuccess)
		{
			AfxMessageBox(StringFromID(IDS_CALLZONG_FAIL_BEFORE_MODIFY));
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}
		else
		{
			CString str;
			str.Format("%s, %s(%s), %s(%s)", StringFromID(IDS_ZONE_AUTOCALL_SUCCESS),
				StringFromID(IDS_ZONE_RUN),m_sCurrentZone, StringFromID(IDS_ZONE_EDIT),m_sEditZone);
			AfxMessageBox(str);
			if (m_nModifySettingOrModifyZone == 0)
			{
				return;
			}
			else if(m_nModifySettingOrModifyZone == 1)
			{
				OnBtnPtsetModify1();
			}
			else if(m_nModifySettingOrModifyZone == 2)
			{
				OnBtnPtsetZoneModify1();
			}
			
		}
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ�����ݵ�ǰ״̬���¸��ؼ�������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8D00EA
void CPTSetting::UpdateControlsEnable()
{
	SetDlgItemText(IDC_REMIND, "");
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		//�ٻ���ֵ
		SetDlgItemText(IDC_BTN_PTSET_CALL, StringFromID(IDS_BTN_CALLSETTING));
		//�ٻ���ֵ����
		SetDlgItemText(IDC_BTN_PTSET_CALLZONE, StringFromID(IDS_BTN_CALLZONE));
		//�޸Ķ�ֵ
		SetDlgItemText(IDC_BTN_PTSET_MODIFY, StringFromID(IDS_MODIFY_SETTING));
		//�л���ֵ��
		SetDlgItemText(IDC_BTN_PTSET_ZONE_MODIFY, StringFromID(IDS_MODIFY_ZONE));

		//����״̬,������
		GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
		if(g_PTControlModel == 2)
		{
			//�㶫ģʽ
			if(!m_bOperVerify)
			{
				GetDlgItem(IDC_BTN_PTSET_VERIFY2)->EnableWindow(false);
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
				GetDlgItem(IDC_BTN_PTSET_VERIFY2)->EnableWindow(true);
			}

			if (m_bOperVerify && m_bMonVerify)
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				m_btnVerify2.SetIcon(IDI_TICK);
				GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
				GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(FALSE);
				m_sOperUser = _T("");
				m_sMonUser = _T("");
			}
		}

		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTSET_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_STYLE) ->EnableWindow(TRUE);

		//������
		GetDlgItem(IDC_CMB_PTSET_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTSET_SELZONE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTSET_SRC) ->EnableWindow(TRUE);
		//ֹͣ����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 5)
	{
		//�ٻ������״̬,�����Լ������Ķ�������

		GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(FALSE);

		//��ӡ�ͷ�����ð�ť
		GetDlgItem(IDC_BTN_PTSET_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_STYLE) ->EnableWindow(FALSE);

		//������
		GetDlgItem(IDC_CMB_PTSET_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTSET_SELZONE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTSET_SRC) ->EnableWindow(FALSE);

		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();

		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSETTING, str);

		//���Լ�����
		switch(m_nCurrentStatus)
		{
		case 1: //�ٻ���ֵ
			SetDlgItemText(IDC_BTN_PTSET_CALL, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(TRUE);
			break;
		case 2: //�ٻ���ֵ����
			SetDlgItemText(IDC_BTN_PTSET_CALLZONE, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(TRUE);
			break;
		case 3: //�޸Ķ�ֵ
			SetDlgItemText(IDC_BTN_PTSET_MODIFY, StringFromID(IDS_BTN_CANCEL_MODIFY));
			GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
			break;
		case 4: //��ֵ���л�
			SetDlgItemText(IDC_BTN_PTSET_ZONE_MODIFY, StringFromID(IDS_CANCEL_SWITCH));
			GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
			break;
		default:
			break;
		}
	}
}

/*************************************************************
 �� �� ����OnBtnPtsetModify()
 ���ܸ�Ҫ����ֵ�޸�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnPtsetModify() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.m_arrEdited.GetSize();
	CString str;
	str.Format("CPTSetting::OnBtnPtsetModify() �޸ĵ�ֵ������Ϊ%d", nCount);
	WriteLog(str, XJ_LOG_LV3);
	if (m_pObj->m_bNewDevice && m_nCurrentStatus == 0)
	{
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//���޸Ķ�ֵ, ��ʾ���޸�
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}
		m_nModifySettingOrModifyZone = 1;
		m_bOperTow = true;
		OnBtnPtsetCallzone();
	}
	else
	{
		//OnBtnPtsetModify1();
		OnBtnPtsetModify2();
	}
}


void CPTSetting::OnBtnPtsetModify1()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	//��Ӧ��ť���²���, �޸Ķ�ֵ��ȡ���޸�
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	/*
	if (g_bVerify)
	{
		//��֤Ȩ��,�޸Ķ�ֵ��Ҫ����Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//�ռ����޸ĵ�ֵ
			CheckModifyData(m_arrModifyList, strOut);
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			return;
		}
	}
	*/

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

	/*if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox("��װ�ñ༭��ֵ����δ֪!");
		return;
	}*/
	
	/*if (m_pObj->m_bNewDevice && !m_sCurrentZone.CompareNoCase(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("���ж�ֵ������༭��ֵ������ͬ��ֹ�޸Ķ�ֵ!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) == atoi(m_sCurrentZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("��ֹ�޸���������ֵ!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) != atoi(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		CString str;
		str.Format("ѡ��Ķ�ֵ����(%s)��༭��ֵ����(%s)��һ��,��ֹ�޸�!", m_sZone, m_sEditZone);
		AfxMessageBox(str);
		return;
	}*/


	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//�����ڿ���״̬, ��ʼ�޸Ķ�ֵ
		//����û��з��޸Ķ�ֵ
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//AfxMessageBox("CPTSetting::OnBtnPtsetModify1.DEBUG");
			//���޸Ķ�ֵ, ��ʾ���޸�
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}

		//�齨Ҫ�޸�ֵ������
		m_strOutPut = "";
		//��¼��ֵ
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//Ҫ���û��ٴ�ȷ��
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}*/

		m_nOperationNum = GetOperationNum();

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
					str.Format("�û�%s�Բ���Ա�����֤�ɹ�:�޸Ķ�ֵ", m_sOperUser);
					WriteLog(str);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS,m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Բ���Ա�����֤ʧ��:�޸Ķ�ֵ", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD,m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
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
					str.Format("�û�%s�Լ໤Ա�����֤�ɹ�:�޸Ķ�ֵ", m_sMonUser);
					WriteLog(str);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS,m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Լ໤Ա�����֤ʧ��:�޸Ķ�ֵ", m_sMonUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD,m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}

		WriteLog("�齨�޸Ķ�ֵ����");
		//�齨����
		STTP_FULL_DATA sttpData;
		pApp->m_SttpEngine.BuildDataFor20051SettingModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
		WriteLog("�齨�޸Ķ�ֵ�������");

		WriteLog("�����޸Ķ�ֵ����");
		//���ͱ���
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//�ж��Ƿ�ɹ�
		if(nReturn != 1)
		{
			//ʧ��
			if(nReturn == -1)
			{
				CString str;
				str.Format("����[%s]�����޸Ķ�ֵ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����޸Ķ�ֵ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			//m_btnModifySetting.EnableWindow(false);
			//m_btnModifyZone.EnableWindow(false);
			//m_btnVerify2.EnableWindow(false);
			m_sOperUser = _T("");
			m_sMonUser = _T("");
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����޸Ķ�ֵ
			m_bChecking = TRUE;
			m_nCurrentStatus = 3;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nModifySettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//�����˹�����
			//pApp->AddManOperator(FUNC_OPT_SETSETTING, m_pObj->m_sName);

			//�ı�ؼ�������
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����޸Ķ�ֵ���ĳɹ�", m_pObj->m_sName);
			WriteLog(str);
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ״̬
		//ȡ������
		CancelCall();
	}
	else
	{
		//�������������ڽ���
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		return;
	}
}

void CPTSetting::OnBtnPtsetModify2()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	//��Ӧ��ť���²���, �޸Ķ�ֵ��ȡ���޸�
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	/*
	if (g_bVerify)
	{
		//��֤Ȩ��,�޸Ķ�ֵ��Ҫ����Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//�ռ����޸ĵ�ֵ
			CheckModifyData(m_arrModifyList, strOut);
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			return;
		}
	}
	*/

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

	/*if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox("��װ�ñ༭��ֵ����δ֪!");
		return;
	}*/
	
	/*if (m_pObj->m_bNewDevice && !m_sCurrentZone.CompareNoCase(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("���ж�ֵ������༭��ֵ������ͬ��ֹ�޸Ķ�ֵ!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) == atoi(m_sCurrentZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("��ֹ�޸���������ֵ!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) != atoi(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		CString str;
		str.Format("ѡ��Ķ�ֵ����(%s)��༭��ֵ����(%s)��һ��,��ֹ�޸�!", m_sZone, m_sEditZone);
		AfxMessageBox(str);
		return;
	}*/


	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//�����ڿ���״̬, ��ʼ�޸Ķ�ֵ
		//����û��з��޸Ķ�ֵ
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//AfxMessageBox("CPTSetting::OnBtnPtsetModify1.DEBUG");
			//���޸Ķ�ֵ, ��ʾ���޸�
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}

		//�齨Ҫ�޸�ֵ������
		m_strOutPut = "";
		//��¼��ֵ
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//Ҫ���û��ٴ�ȷ��
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}*/

		m_nOperationNum = GetOperationNum();

		//����Աȷ��
		CDlgCheckPro dlg(this, 2);
		dlg.m_strModify = m_strOutPut;
		dlg.m_arrModifyList.Copy(m_arrModifyList);
		dlg.m_sZone = m_sZone;
		dlg.m_sCPU = m_sCPU;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//��ͨģʽ,Ҫ�����Ա��֤
				CDlgValidateID dlgID(2, 1);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					
					CXJPTSetStore *pStore = CXJPTSetStore::GetInstance();
					QPTSetStateTable *pState = pStore->GetState();
					pStore->ReLoadState();
					
					int nPTSetState = pState->GetStateID();

					CString str;
					CString sRecords;
					int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
					if (XJ_OPER_HANGOUT != nPTSetState){
						CString sCurUserID = pState->GetOperUserID().constData();
						str.Format("�û�[%s]���ڶԸ�װ�ý��ж�ֵ�޸Ĳ�����ҵ ���� ��װ����ȡ���˹��ƣ����Ժ�����", sCurUserID);
						AfxMessageBox(str, MB_OK | MB_ICONWARNING);

						RevertModifyValue(1, 1);
						return;
					}

					CXJPTSetStore::GetInstance()->GetState()->Next_PTSet_State_2(atoi(m_sCPU), atoi(m_sZone)
						, m_sOperUser.GetBuffer(0), m_arrModifyList, m_arrSetting);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ

					CString sRecords;
					int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
					if (1 != nState){
						CString sCurUserID = pApp->GetUserIDByState(2, sRecords);
						str.Format("�û�[%s]���ڶԸ�װ�ý��ж�ֵ�޸Ĳ�����ҵ ���� ��װ����ȡ���˹��ƣ����Ժ�����", sCurUserID);
						AfxMessageBox(str, MB_OK | MB_ICONWARNING);
						
						RevertModifyValue(1, 1);
						m_nCurrentDetailStatus = 0;
						return;
					}
					
					RevertModifyValue();
					str.Format("�û�%s�Բ���Ա�����֤ʧ��:�޸Ķ�ֵ", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD,m_nOperationNum);
					
					//pApp->RevertPTSetModState(1);
					m_nCurrentDetailStatus = 0;

					return;
				}
			}
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			UpdateControlsEnable();

			CString str;
			CString sRecords;
			int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
			if (1 != nState){
				CString sCurUserID = pApp->GetUserIDByState(2, sRecords);
				str.Format("�û�[%s]���ڶԸ�װ�ý��ж�ֵ�޸Ĳ�����ҵ ���� ��װ����ȡ���˹��ƣ����Ժ�����", sCurUserID);
				AfxMessageBox(str, MB_OK | MB_ICONWARNING);
				
				RevertModifyValue(1, 1);
				m_nCurrentDetailStatus = 0;
				return;
			}
			
			RevertModifyValue();
			//pApp->RevertPTSetModState(1);
			m_nCurrentDetailStatus = 0;

			return;
		}

		//�໤Աȷ��
		CDlgCheckPro dlg1(this, 1);
		dlg1.m_strModify = m_strOutPut;
		dlg1.m_arrModifyList.Copy(m_arrModifyList);
		dlg1.m_sZone = m_sZone;
		dlg1.m_sCPU = m_sCPU;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//��ͨģʽ,Ҫ��໤Ա��֤
				CDlgValidateID dlgID(1, 1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Լ໤Ա�����֤�ɹ�:�޸Ķ�ֵ", m_sMonUser);
					WriteLog(str);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS,m_nOperationNum);
				
				    // �໤��Աȷ�Ϻ��޸�״̬��
					//pApp->ModifyDZModState(3, m_sMonUser, m_pObj);
					PT_ZONE zone;
					pApp->GetPTSetModState(zone);
					pApp->NextPTSetModState(3, zone, m_sMonUser);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Լ໤Ա�����֤ʧ��:�޸Ķ�ֵ", m_sMonUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD,m_nOperationNum);
					
					pApp->RevertPTSetModState(1);
					m_nCurrentDetailStatus = 0;
					return;
				}
			}
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();
			UpdateControlsEnable();
			pApp->RevertPTSetModState(1);
			m_nCurrentDetailStatus = 0;
			
			return;
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ״̬
		//ȡ������
		CancelCall();
	}
	else
	{
		//�������������ڽ���
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
		CString strOut;
		//�ռ����޸ĵ�ֵ
		CheckModifyData(m_arrModifyList, strOut);
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		return;
	}
}

BOOL CPTSetting::ExcutePTSet()
{
	BOOL bReturn = TRUE;

	if (NULL == m_pObj){
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
		m_nCurrentDetailStatus = 0;
		return FALSE;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	WriteLog("�齨�޸Ķ�ֵ����");
	//�齨����
	STTP_FULL_DATA sttpData;
	pApp->m_SttpEngine.BuildDataFor20051SettingModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
	WriteLog("�齨�޸Ķ�ֵ�������");
	
	WriteLog("�����޸Ķ�ֵ����");
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		//ʧ��
		if(nReturn == -1)
		{
			CString str;
			str.Format("����[%s]�����޸Ķ�ֵ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("����[%s]�����޸Ķ�ֵ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();

		m_bMonVerify = false;
		m_bOperVerify = false;
		//m_btnModifySetting.EnableWindow(false);
		//m_btnModifyZone.EnableWindow(false);
		//m_btnVerify2.EnableWindow(false);
		m_sOperUser = _T("");
		m_sMonUser = _T("");

		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);

		bReturn = FALSE;
	}
	else
	{
		//�ɹ�,״̬��Ϊ�����޸Ķ�ֵ
		m_bChecking = TRUE;
		m_nCurrentStatus = 3;
		
		//������ʱ��
		CDataEngine* pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nModifySettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//�����˹�����
		//pApp->AddManOperator(FUNC_OPT_SETSETTING, m_pObj->m_sName);
		
		//�ı�ؼ�������
		UpdateControlsEnable();
		
		CString str;
		str.Format("����[%s]�����޸Ķ�ֵ���ĳɹ�", m_pObj->m_sName);
		WriteLog(str);
		
		//AfxMessageBox(str);
	}

	return bReturn;
}

/*************************************************************
 �� �� OnBtnViewProg()
 ���ܸ�Ҫ����ֵ�޸Ľ��Ȳ鿴
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnViewProg() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	CCJTabCtrlBar &bar = pMainFrame->m_wndGlobalMsgBar;
	
	CRect rcRect;
	GetWindowRect(&rcRect);
	
	if (m_bAlreadyShowOnce){
		bar.GetWindowRect(rcRect);
		m_pointPTSetModView.x = rcRect.left - 2;
		m_pointPTSetModView.y = rcRect.top - 18;

	}else{
		LONG w = 600;
		LONG h = 220;

		m_pointPTSetModView.x = rcRect.right - w;
		m_pointPTSetModView.y = rcRect.bottom - h;
		m_bAlreadyShowOnce = true;
	}
	
	bar.ShowWindow(bar.IsVisible() ? SW_HIDE : SW_SHOW);
	pMainFrame->FloatControlBar(&bar, m_pointPTSetModView, CBRS_ALIGN_LEFT);

	
	CString str;
	str.Format("x: %d; y: %d", m_pointPTSetModView.x, m_pointPTSetModView.y);
	//AfxMessageBox(str);

}

/*************************************************************
 �� �� OnBtnViewHis()
 ���ܸ�Ҫ����ֵ�޸���ʷ�鿴
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnViewHis() 
{
	// TODO: Add your control notification handler code here
	CDlgOperHis dlg;
	dlg.m_nType = 1;
	dlg.m_pObj = m_pObj;
	dlg.DoModal();
}

/*************************************************************
 �� �� ����CheckModifyData()
 ���ܸ�Ҫ���齨Ҫ�޸ĵĶ�ֵ�б�
 �� �� ֵ: void
 ��    ����param1	Ҫ�޸ĵĶ�ֵ�б�
		   Param2	�������
**************************************************************/
//##ModelId=49B87B8D0119
void CPTSetting::CheckModifyData( MODIFY_LIST& modifyList, CString& strOut )
{
	//����޸�ֵ�б�
	ClearModifyData();

	//��ֵֵ��
	CPoint ptItem;
	CString strText, strPickList;
	strOut = StringFromID(IDS_VERIFY_MODIFYSETTING_CONFIRM)+":\r\n";
	CString str;
	str.Format("%s:%s\r\n", StringFromID(IDS_SECPROP_ZONE),m_sZone);
	strOut += str;
	PT_SETTING *pts;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		ptItem = m_List.m_arrEdited.GetAt(i);
		if (ptItem.y != m_nNewValueIndex)
		{
			continue;
		}

		STTP_DATA * sttpData = new STTP_DATA;

		strText = m_List.GetItemText(ptItem.x, ptItem.y);
		
		//ȥ��"*"
		strText = strText.Left(strText.Find("*", 0));
		if (strText.IsEmpty())
		{
			continue;
		}
		
		//ȡ�ø��ֵ�Ķ���
		pts = (PT_SETTING *)m_arrSetting.GetAt(ptItem.x);
		strPickList = pts->VPickList;
		strPickList.TrimLeft();
		strPickList.TrimRight();
		
		//ID
		sttpData->id = atoi(pts->ID);
		// CPU_CODE
		sttpData->nCpu = atoi(m_sCPU);

		//����ֶ�
		strOut += pts->ID + "\t";
		CString strName = _T("");
		int z = pts->Name.Find(",", 0);
		if (z != -1)
		{
			strName = pts->Name.Left(z);
		}
		else
		{
			strName = pts->Name;
		}
		strOut += strName + " "+StringFromID(IDS_COMMON_FROM)+" ";
		strOut += GetDisplayValue(pts, pts->Value) + "\t"+StringFromID(IDS_COMMON_CHANGETO)+"-> ";
		strOut += strText + "\r\n";

		CString strTmp;
		strTmp.Format("%s@%s", strName, GetDisplayValue(pts, pts->Value)); // name@value
		sttpData->str_reserved = strTmp; // ԭֵ
		
		if (strPickList.IsEmpty())
		{//strText����ֱ��д�뱨��
			//ȥ�������еĵ�λ
			pts->Unit.TrimRight();
			if (!pts->Unit.IsEmpty())
			{
				int nSub = strText.Find(pts->Unit, 0);
				if (nSub != -1)
					strText = strText.Left(nSub);
			}
		}
		else
		{//strText������ת��
			//��ֵ��ֵ
			strText = GetIDFromValue(pts->VPickList, strText);
		}
		if(pts->ntimeMStoS == 1)
		{
			double fValue = atof(strText);
			fValue *= 1000;
			//int nValue = (int)fValue;
			strText.Format("%.0f", fValue);
		}
		//��ֵ��ֵ
//		sttpData->str_value = MSToXJ(strText);
		sttpData->str_value = strText;

		//���뵽����
		modifyList.Add(sttpData);
	}
}

/*************************************************************
 �� �� ����GetTranslatedValue()
 ���ܸ�Ҫ��ȡ�þ�������VPickList���Value
 �� �� ֵ: ��������VPickList���Value
 ��    ����param1	��ֵ����
		   Param2	Ҫ�����ֵ
**************************************************************/
//##ModelId=49B87B8D0148
CString CPTSetting::GetTranslatedValue( PT_SETTING *pst,const CString strValue )
{
	//����VPickList���Value����,VPickList����ʱ��Valueֵ�Ľ��͸���VPivkListȷ�� 
	//��VPickList="0:on;1:off" �� Value=1 ʱ��strValueΪ"off"
	//��VPickListΪ�գ���Value=1��Unit="s" ��strValue = "1s"  
	CString strTemp;
	CString strUnit		= pst->Unit;
	CString strVPickList= pst->VPickList;
	CString strReturn	= strValue;//pst->Value;
	
	//ȥ������Ŀո�
	strUnit.TrimLeft();
	strUnit.TrimRight();
	strReturn.TrimLeft();
	strReturn.TrimRight();
	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (strVPickList.IsEmpty())
	{
	}
	else
	{
		if (CheckVPicklist(strVPickList))
		{
			int nFenhao =0;	//�ֺŵ�λ��
			int nMaohao =0; //ð�ŵ�λ��
			BOOL bFound = FALSE;
			
			//ֻҪstrVPickList���գ��������ֺ�֮����ַ������������ð����ߵ�ֵ�Ƿ���strReturn��ͬ
			//�����ͬ��stValue��ð���ұߵ��ִ�ȡ��
			while (!strVPickList.IsEmpty()) 	 
			{
				nFenhao = strVPickList.Find(';', 0);
				if(nFenhao>0)
					strTemp = strVPickList.Left(nFenhao);
				else
					strTemp = strVPickList;
				
				nMaohao = strTemp.Find(':', 0);			 
				if (nMaohao>0)
				{
					if (strTemp.Left(nMaohao) == strReturn)
					{
						strTemp.Delete(0, nMaohao+1);
						strReturn = strTemp;
						bFound = TRUE;
						break;
					}
				}
				
				if (nFenhao>0)
					strVPickList.Delete(0, nFenhao+1);
				else
					break;
			}
		}
		else
		{
			CString str;
			str.Format("CPTSetting::CheckVPicklist,��ѯʧ��,ԭ��Ϊ���ݿ�vpicklistΪ%s,��ʽ����",strVPickList);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	return strReturn;
}

/*************************************************************
 �� �� ����ClearModifyData()
 ���ܸ�Ҫ������޸�ֵ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8D0167
void CPTSetting::ClearModifyData()
{
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		delete sttpData;
	}

	m_arrModifyList.RemoveAll();
}

/*************************************************************
 �� �� ����OnSTTP20052()
 ���ܸ�Ҫ����ֵ�޸�ԤУ֪ͨ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8F004E
void CPTSetting::OnSTTP20052( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//����Ǵ����޸Ķ�ֵ״̬
	if(m_nCurrentStatus != 3)
	{
		//���޸Ķ�ֵ״̬
		return;
	}

	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;

	//����Ƿ�20052����
	if(pSttpData->sttp_head.uMsgID != 20052)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20052 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20052 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CString strMsg;
	strMsg.Format("%d,%d,%d,%d,%d", 
		pSttpData->sttp_body.nStatus, 
		XJ_OPER_MODIFYSETTING_PRECORRECTION,
		pSttpData->sttp_body.nCpu, 
		pSttpData->sttp_body.nZone, 
		m_arrModifyList.GetSize());
	
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
	
	pApp->AddNewManOperator("��ֵ�޸�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);

	//�ر�ԤУ��ʱ  
	KillTimer(m_nTimer);


	CString strOutput = m_strOutPut;

	//����ȷ��
	if(pSttpData->sttp_body.nStatus == 0)
	{
		WriteLog("��ֵ�޸�ԤУ�ɹ�,��ʼ�·�ִ������");
		//�ɹ�
		strOutput += StringFromID(IDS_EXECUTE_MODIFYSETTING_CONFIRM);
		/*CDlgCheckPro dlg(this,2);
		dlg.m_strModify = strOutput;
		dlg.m_arrModifyList.Copy(m_arrModifyList);
		dlg.m_sZone = m_sZone;
		dlg.m_sCPU = m_sCPU;
		//��ʾ�û��������
		if (dlg.DoModal() == IDOK)*/
		{
			//����ִ�б���
			//�齨����
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20053SettingModifyExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
			
			//���ͱ���
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//�ж��Ƿ�ɹ�
			if(nReturn != 1)
			{
				//ʧ��
				if(nReturn == -1)
				{
					CString str;
					str.Format("����[%s]�����޸Ķ�ִֵ�б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("����[%s]�����޸Ķ�ִֵ�б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}

				//�޸�״̬
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				//�ظ��޸�ǰ��ֵ
				RevertModifyValue();

				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
				
				// ������ʱ
				KillTimer(m_nPTSetTimer);
				m_nCurrentDetailStatus = 0;
				m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);

				return;
			}
			
			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nModifySettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
/*		else
		{
			//����ִ��
			//�޸�״̬Ϊ����
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue();

			// ������ʱ
			KillTimer(m_nPTSetTimer);
			m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
		}*/
	}
	else
	{
		WriteLog("��ֵ�޸�ԤУʧ��");
		m_bMonVerify = false;
		m_bOperVerify = false;
		//�޸�״̬Ϊ����
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		//ʧ��
		strOutput += "\n";
		strOutput += StringFromID(IDS_VERIFY_MODIFYSETTING_FAIL);
		//��ʾ�û��������
		AfxMessageBox(strOutput, MB_OK);
		
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;

		// ������ʱ
		KillTimer(m_nPTSetTimer);
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	}

	m_bChecking = FALSE;
}

/*************************************************************
 �� �� ����GetSettingObject()
 ���ܸ�Ҫ�����ݶ�ֵID�ҵ���ֵ����
 �� �� ֵ: ��ֵ����
 ��    ����param1	��ֵID
**************************************************************/
//##ModelId=49B87B8D0186
PT_SETTING * CPTSetting::GetSettingObject( CString strID )
{
	strID.TrimLeft();
	strID.TrimRight();

	PT_SETTING *p = NULL;
	for (int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		p = m_arrSetting.GetAt(i);
		
		if (p->ID == strID)
			return p;
	}
	
	return NULL; 
}

/*************************************************************
 �� �� ����GetIDFromValue()
 ���ܸ�Ҫ������ֵ����ֵ�Ķ���
 �� �� ֵ: ֵ����
 ��    ����param1	ֵ˵��
		   Param2	ֵ
**************************************************************/
//##ModelId=49B87B8D01A5
CString CPTSetting::GetIDFromValue( CString strPickList, CString strValue )
{
	CString strID, strTmp;
	strID = strValue;
	int nPos1 = 0, nPos2 = 0;
	
	while (1) 
	{
		nPos1 = strPickList.Find(":", nPos2);
		nPos2 = strPickList.Find(";", nPos1);
		if (nPos1 == -1)
		{
			//û�ҵ�
			return strID;
		}
		
		if (nPos2 == -1)
		{
			nPos2 = strPickList.GetLength();
		}
		
		strTmp = strPickList.Mid(nPos1 + 1, nPos2 - nPos1 - 1);
		if (strValue == strTmp)
		{//�ҵ���
			strPickList = strPickList.Left(nPos1);
			nPos2 = strPickList.ReverseFind(';');
			if (nPos2 != -1)
				strID = strPickList.Right(strPickList.GetLength() - nPos2 - 1);
			else
				strID = strPickList.Left(nPos1);
			
			break;
		}
	}
	
	return strID;
}

/*************************************************************
 �� �� ����OnSTTP20054()
 ���ܸ�Ҫ����Ӧ������ֵ�޸�ִ��֪ͨ(20054)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8F006D
void CPTSetting::OnSTTP20054( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//����Ǵ����޸Ķ�ֵ״̬
	if(m_nCurrentStatus != 3)
	{
		//���޸Ķ�ֵ״̬
		return;
	}
	
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//����Ƿ�20054����
	if(pSttpData->sttp_head.uMsgID != 20054)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20054 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20054 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSETTING_EXECUTE,pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_arrModifyList.GetSize());
	WriteLog(strMsg);
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
	WriteLog(strMsg);

	pApp->AddNewManOperator("��ֵ�޸�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	//�رն�ʱ��
	KillTimer(m_nTimer);

	//�޸�״̬
	m_nCurrentStatus = 0;
	UpdateControlsEnable();

	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//�ɹ�
		//����޸ı��
		m_List.ClearEdited();

		// �޸�״̬��
		//pApp->ModifyDZModState(5, m_sOperUser, m_pObj);
		PT_ZONE zone;
		pApp->GetPTSetModState(zone);
		pApp->NextPTSetModState(5, zone, m_sOperUser);
		pApp->SetRevertModifyValueFlag(0);

		//��ʾ
		AfxMessageBox( StringFromID(IDS_EXECUTE_MODIFYSETTING_SUCCESS), MB_OK|MB_ICONINFORMATION);
		m_nCurrentDetailStatus = 0;
		m_btnModifySetting.EnableWindow(FALSE);
		m_btnModifyZone.EnableWindow(FALSE);

		// �����޸ĺ��һ�ζ�ֵ�� �����
		//CXJPTSetStore::GetInstance()->SaveRecallToDB(m_sCPU, m_pObj->m_sID, m_arrSetting);
	}
	else
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//ʧ��
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		AfxMessageBox( StringFromID(IDS_EXECUTE_MODIFYSETTING_FAIL));

		// �޸�״̬��
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
	}

	m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
}

/*************************************************************
 �� �� ����OnBtnPtsetZoneModify()
 ���ܸ�Ҫ����ֵ���л�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8E0290
void CPTSetting::OnBtnPtsetZoneModify() 
{
		
	// TODO: Add your control notification handler code here
	int nCount = m_List.m_arrEdited.GetSize();
	CString str;
	str.Format("CPTSetting::OnBtnPtsetZoneModify() �޸ĵ�ֵ������Ϊ%d", nCount);
	WriteLog(str, XJ_LOG_LV3);
	if (m_pObj->m_bNewDevice && m_nCurrentStatus == 0)
	{
		if(m_ListZone.m_arrEdited.GetSize() == 0)
		{
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_ZONE));
			return;
		}
		m_nModifySettingOrModifyZone = 2;
		m_bOperTow = true;
		OnBtnPtsetCallzone();
	}
	else
	{
		OnBtnPtsetZoneModify1();
	}
}

void CPTSetting::OnBtnPtsetZoneModify1()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;

	if(m_pObj == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	/*
	if (g_bVerify)
	{
		//��֤Ȩ��,�޸Ķ�ֵ��Ҫ����Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	*/
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//����״̬
		//���¿ؼ��󶨵ı���ֵ
		UpdateData(TRUE);

		if(m_ListZone.m_arrEdited.GetSize() == 0)
		{
			AfxMessageBox( StringFromID(IDS_TIP_SELECT_ZONE));
			return;
		}
		CPoint ptItem = m_ListZone.m_arrEdited.GetAt(0);
		CString strText = m_ListZone.GetItemText(ptItem.x, ptItem.y);
		strText = strText.Left(strText.Find("*", 0));

		m_sModifyZone = strText;

		m_nOperationNum = GetOperationNum();

		//Ҫ���û�ȷ��
		CString str;
		str.Format("%s:%s, %s:%s, %s %s %s %s %s?", StringFromID(IDS_MODEL_RELAY),m_pObj->m_sName, StringFromID(IDS_MODEL_CPU),m_sCPU, 
			StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_COMMON_FROM), m_sOldZoneValue, StringFromID(IDS_COMMON_CHANGETO),strText);
		//m_sOldZoneValue = _T("");
		/*if (AfxMessageBox(str, MB_OKCANCEL) != IDOK)
		{
			UpdateControlsEnable();
			return;
		}*/

		//����Աȷ��
		CDlgCheck dlg(this, 2);
		dlg.m_strModify = str;
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
					str.Format("�û�%s�Բ���Ա�����֤�ɹ�:��ֵ���л�", m_sOperUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue(2);
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Բ���Ա�����֤ʧ��:��ֵ���л�", m_sOperUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}			
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue(2);
			UpdateControlsEnable();
			return;
		}

		//�໤Աȷ��
		CDlgCheck dlg1(this, 1);
		dlg1.m_strModify = str;
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
					str.Format("�û�%s�Լ໤Ա�����֤�ɹ�:��ֵ���л�", m_sMonUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//�޲���Ȩ��
					//�ظ��޸�ǰ��ֵ
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("�û�%s�Լ໤Ա�����֤ʧ��:��ֵ���л�", m_sMonUser);
					pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//��ͬ���޸�
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue(2);
			UpdateControlsEnable();
			return;
		}

		//�齨ԤУ����
		STTP_FULL_DATA sttpData;
		if (m_pObj->m_bNewDevice)
		{
			bool bRun = false;
			bool bEdit = false;
			for (int j = 0; j < m_ListZone.GetItemCount(); j++)
			{
				CString str = m_ListZone.GetItemText(j, 6);
				if (str.Find("*") != -1)
				{
					CString strItem = m_ListZone.GetItemText(j, 0);
					if (strItem.Find(StringFromID(IDS_COMMON_RUN)) != -1)
					{
						bRun = true;
					}
					else if (strItem.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
					{
						bEdit = true;
					}
				}
			}

			int nId = -1;
			CSecCPU* pCpu = NULL;
			for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
			{
				pCpu = m_pObj->m_arrCPU[k];
				if (pCpu->code == atoi(m_sCPU))
				{
					break;
				}
			}
			MYASSERT(pCpu);
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1 && bEdit)
				{
					nId = ptz->code;
					break;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1 && bRun)
				{
					nId = ptz->code;
					break;
				}
			}

			if (nId == -1)
			{
				AfxMessageBox( StringFromID(IDS_TIP_ZONEID_ERROR));
				return;
			}
			pApp->m_SttpEngine.BuildDataFor20169SettingChange(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText), nId);
		}
		else
		{
			pApp->m_SttpEngine.BuildDataFor20047SettingChange(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText));
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
				str.Format("����[%s]���Ͷ�ֵ���л�����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]���Ͷ�ֵ���л�����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue(2);
			m_bMonVerify = false;
			m_bOperVerify = false;
			//m_btnModifySetting.EnableWindow(false);
			//m_btnModifyZone.EnableWindow(false);
			//m_btnVerify2.EnableWindow(false);
			m_sOperUser = _T("");
			m_sMonUser = _T("");
		}
		else
		{
			//�ɹ�,״̬��Ϊ�����޸Ķ�ֵ��
			m_bChecking = TRUE;

			m_nCurrentStatus = 4;
			UpdateControlsEnable();

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//�����˹�������־
			//pApp->AddManOperator(FUNC_OPT_CHANGEZONE, m_pObj->m_sName);

			//�޸Ŀؼ�������
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]���Ͷ�ֵ���л����ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����޸Ķ�ֵ��, ȡ��
		CancelCall();
	}
	else
	{
		//������������
		return;
	}
} 

/*************************************************************
 �� �� ����OnSTTP20048()
 ���ܸ�Ҫ����Ӧ��ֵ���л�ԤУ֪ͨ(20048)
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F008C
void CPTSetting::OnSTTP20048( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		//û�д��ڶ�ֵ���л���״̬,��������
		return;
	}
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20048����
	if(pSttpData->sttp_head.uMsgID != 20048)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20048 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20048 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//�ر�ԤУ��ʱ  
	KillTimer(m_nTimer);
	
	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone);
	else if(g_OperTableType == 1)	
		strMsg.Format("%d,%d,%d,,1,,%d,%s", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_sCurrentZone);
	pApp->AddNewManOperator("��ֵ���л�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	
	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//�ɹ�
		CString str;
		str.Format("%s! %s:%s %s:(%d) %s:(%d)", StringFromID(IDS_VERIFY_SUCCESS), StringFromID(IDS_MODEL_RELAY),strID, StringFromID(IDS_MODEL_CPU),pSttpData->sttp_body.nCpu, 
			StringFromID(IDS_ZONE_SWITCHTO),pSttpData->sttp_body.nZone);

		CDlgCheck dlg(this,3);
		dlg.m_strModify = str;
		//��ʾ�û��������
		if (dlg.DoModal() == IDOK)
		{
			//�齨ִ�б���
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20055SettingChangeExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(/*m_sZone*/m_sModifyZone));

			//���ͱ���
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//�ж��Ƿ�ɹ�
			if(nReturn != 1)
			{
				//ʧ��
				if(nReturn == -1)
				{
					CString str;
					str.Format("����[%s]���Ͷ�ֵ�л�ִ�б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("����[%s]���Ͷ�ֵ�л�ִ�б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));

				//�ı�״̬
				//�ظ��޸�ǰ��ֵ
				RevertModifyValue(2);
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				return;
			}
			//�����޸ĳ�ʱ��ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
		else
		{
			//�޸�״̬Ϊ����
			//�ظ��޸�ǰ��ֵ
			RevertModifyValue(2);
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
		}
	}
	else
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//ʧ��
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue(2);
		//�޸�״̬Ϊ����
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox( StringFromID(IDS_VERIFY_FAIL));
	}

	m_bChecking = FALSE;
}


void CPTSetting::OnSTTP20170( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		WriteLog("CPTSetting::OnSTTP20170 û�д��ڶ�ֵ���л���״̬,��������", XJ_LOG_LV3);
		//û�д��ڶ�ֵ���л���״̬,��������
		return;
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20048����
	if(pSttpData->sttp_head.uMsgID != 20170)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20170 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20170 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�ر�ԤУ��ʱ  
	KillTimer(m_nTimer);
	

	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
	else if(g_OperTableType == 1)	
		strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
	pApp->AddNewManOperator("��ֵ���л�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_SUCCESS, m_nOperationNum);
	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
	
		//�ɹ�
		CPoint ptItem = m_ListZone.m_arrEdited.GetAt(0);
		CString strText = m_ListZone.GetItemText(ptItem.x, ptItem.y);
		strText = strText.Left(strText.Find("*", 0));
		CString str;
		str.Format("%s!%s:%s %s:(%d) %s:(%d)", StringFromID(IDS_VERIFY_SUCCESS), StringFromID(IDS_MODEL_RELAY),strID, StringFromID(IDS_MODEL_CPU),pSttpData->sttp_body.nCpu, 
			StringFromID(IDS_ZONE_SWITCHTO),pSttpData->sttp_body.nZone);
		if (AfxMessageBox(str, MB_OKCANCEL) == IDOK)
		{
			//�齨ִ�б���
			STTP_FULL_DATA sttpData;
			int nId = -1;

			CSecCPU* pCpu = NULL;
			for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
			{
				pCpu = m_pObj->m_arrCPU[k];
				if (pCpu->code == pSttpData->sttp_body.nCpu)
				{
					break;
				}
			}
			MYASSERT(pCpu);
			
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					nId = ptz->code;
					break;
				}
			}
			
			if (nId == -1)
			{
				AfxMessageBox(StringFromID(IDS_TIP_ZONEID_ERROR));
				return;
			}

			pApp->m_SttpEngine.BuildDataFor20171SettingChangeExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText), nId);

			//���ͱ���
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//�ж��Ƿ�ɹ�
			if(nReturn != 1)
			{
				//ʧ��
				if(nReturn == -1)
				{
					CString str;
					str.Format("����[%s]���Ͷ�ֵ�л�ִ�б���ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("����[%s]���Ͷ�ֵ�л�ִ�б���ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

				//�ı�״̬
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				return;
			}
			//�����޸ĳ�ʱ��ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
		else
		{
			//�޸�״̬Ϊ����
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
		}
	}
	else
	{
		//ʧ��
		//�޸�״̬Ϊ����
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_VERIFY_SWITCHZONE_FAIL));
	}
}

/*************************************************************
 �� �� ����OnSTTP20056()
 ���ܸ�Ҫ����Ӧ������ֵ�л�ִ��֪ͨ(20056)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B8F00AB
void CPTSetting::OnSTTP20056( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//����Ǵ��ڶ�ֵ�л�״̬
	if(m_nCurrentStatus != 4)
	{
		//�Ƕ�ֵ�л�״̬
		return;
	}
	
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//����Ƿ�20054����
	if(pSttpData->sttp_head.uMsgID != 20056)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20056 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20056 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone);
	else if(g_OperTableType == 1)
		strMsg.Format("%d,%d,%d,,1,,%d,%s", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_sCurrentZone);
	pApp->AddNewManOperator("��ֵ���л�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
		m_sCurrentZone.Format("%d", pSttpData->sttp_body.nZone);
		m_ListZone.SetItemText(0, 6, m_sCurrentZone);
		UpdateData(FALSE);
		m_ListZone.ClearEdited();
		//�ɹ�
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_SUCCESS));
	}
	else
	{
		//ʧ��
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue(2);
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_FAIL));
	}

	//�޸�״̬
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

void CPTSetting::OnSTTP20172( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//����Ǵ��ڶ�ֵ�л�״̬
	if(m_nCurrentStatus != 4)
	{
		WriteLog("CPTSetting::OnSTTP20172 �Ƕ�ֵ�л�״̬", XJ_LOG_LV3);
		//�Ƕ�ֵ�л�״̬
		return;
	}
	
	//ȡ�ñ�������
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//����Ƿ�20172����
	if(pSttpData->sttp_head.uMsgID != 20172)
	{
		//������
		CString str;
		str.Format("CPTSetting::OnSTTP20172 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTSetting::OnSTTP20172 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	CSecCPU* pCpu = NULL;
	for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
	{
		pCpu = m_pObj->m_arrCPU[k];
		if (pCpu->code == pSttpData->sttp_body.nCpu)
		{
			break;
		}
	}
	MYASSERT(pCpu);
	
	CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		int id;
		id = pSttpData->sttp_body.variant_member.value_data[i].id;
		CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		for (int j = 0; j < arrZONE.GetSize(); j++)
		{
			PT_ZONE * ptz = arrZONE[j];
			if (ptz->code == id)
			{
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = Value;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = Value;
				}
				break;
			}
		}
	}
	
	CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("��ֵ���л�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_SUCCESS, m_nOperationNum);
	//�رն�ʱ��
	KillTimer(m_nTimer);
		
	//������
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//�ɹ�
		m_ListZone.ClearEdited();
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_SUCCESS));
	}
	else
	{
		//ʧ��
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_FAIL));
	}

	//�޸�״̬
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ, ˢ��ҳ������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B8F00DC
void CPTSetting::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	m_bOperVerify = false;
	m_bMonVerify = false;
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	if (m_pObj->m_bNewDevice)
	{
		m_sEditZone = "";
	}
	else
	{
		m_sEditZone = StringFromID(IDS_CALLOPER_NOTSUPPORT);
	}
	
	int nIndex = (int)wParam;

	m_bLoadData = FALSE;
	if(m_pObj->m_bNewDevice)
	{
		CSecCPU* pCpu = NULL;
		for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
		{
			pCpu = m_pObj->m_arrCPU[k];
			MYASSERT_CONTINUE(pCpu);
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				MYASSERT_CONTINUE(ptz);
				GetZone(ptz->PT_ID, ptz->cpu, ptz->code, ptz);
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = ptz->zoneValue;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = ptz->zoneValue;
				}
			}	
		}
	}

	if(nIndex == PT_PROP_SETTING)
	{
 		//ReFillAll(TRUE);
		//StartThread();
	}

	RefreshControls();
	
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_PTSETTING, "");
	//�ж��Ƿ���ʾ���ƹ���
	if(g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		if(g_PTControlModel == 2)//�㶫ģʽ
		{
			m_btnVerify1.ShowWindow(SW_SHOW);
			m_btnVerify2.ShowWindow(SW_SHOW);
		}
		if(m_pObj->m_nCanControl == 0)
		{
			m_btnModifySetting.ShowWindow(SW_SHOW);
			m_btnModifyZone.ShowWindow(SW_SHOW);
			if(g_PTModifyZone != 1)
				m_btnModifyZone.ShowWindow(SW_HIDE);
		}
	}

	m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	m_btnModifySetting.ShowWindow(SW_HIDE);
	m_btnModifyZone.ShowWindow(SW_HIDE);
	m_btnViewPTSetProg.ShowWindow(SW_HIDE);
	
	CRect rcRect;
	m_btnVerify2.GetWindowRect(rcRect);
	ScreenToClient(&rcRect);

	m_btnViewPTSetHis.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_PTSET_MODIFY_VIEW_HIS)->SetWindowPos(NULL
		, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()
		,SWP_NOZORDER/* | SWP_NOSIZE*/ );

	m_btnVerify1.GetWindowRect(rcRect);
	ScreenToClient(&rcRect);
	GetDlgItem(IDC_BTN_PTSET_MODIFY_VIEWPROG)->SetWindowPos(NULL
		, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()
		,SWP_NOZORDER/* | SWP_NOSIZE*/ );

	//m_List.ShowWindow(false);
	//m_ListZone.ShowWindow(true);
	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
	}
	else
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
	}

	m_nLoadType = 0;
	ReFillAll();
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ, �ڵ�һ�������͸��¶�Ӧ����ʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8D01D4
void CPTSetting::RefreshControls()
{
	//1.��䶨ֵ��Դ������
	CComboBox * pCMB = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SRC);
	if(pCMB == NULL)
		return;
	pCMB->ResetContent();
	pCMB->AddString(StringFromID(IDS_MODEL_RELAY));
	pCMB->AddString(StringFromID(IDS_SOURCE_SUBSTATIONDB));
	pCMB->AddString(StringFromID(IDS_COMMON_OTHER));
	pCMB->SetCurSel(0);
	m_nSrc = 0;
	//2.���CPU������
	ReFillCPU();
	//3.��䶨ֵ����������
	ReFillZone();

	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����ReFillCPU()
 ���ܸ�Ҫ���������CPU������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8D01E4
void CPTSetting::ReFillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
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
			pCmb->SetCurSel(0);
		else
		{
			if( !LoadDefaultCpu() )
			pCmb->SetCurSel(0);
		}
		
		//���浱ǰѡ��
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 �� �� ����ReFillZone()
 ���ܸ�Ҫ�����ݵ�ǰ��ѡCPU��䶨ֵ����������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0203
void CPTSetting::ReFillZone()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	if(nIndex == -1)
		return;

	CSecCPU * pCpu = (CSecCPU*)pCmb->GetItemData(nIndex);
	if(pCpu == NULL)
		return;

	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb == NULL)
		return;

	//���
	pCmb->ResetContent();

	//�ж�����From-to������ָ��
	if(m_pObj->m_bNewDevice)
	{
		//ʹ��ָ��
		for(int i = 0; i < pCpu->m_arrZONE.GetSize(); i++)
		{
			PT_ZONE * ptz = (PT_ZONE*)pCpu->m_arrZONE.GetAt(i);
			
			
			if(ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
			{	
				for(int i = ptz->minValue; i <= ptz->maxValue; i++)
				{
					CString str;
					str.Format("%d", i);
					pCmb->AddString(str);
				}
				CString str1;
				str1.Format("%d", ptz->zoneValue);
				m_sCurrentZone = str1; 
			}
			
			if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
			{
				CString str2;
				str2.Format("%d", ptz->zoneValue);
				m_sEditZone = str2;
			}
			
			if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) == -1 && ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) == -1)
			{
				for(int i = ptz->minValue; i <= ptz->maxValue; i++)
				{
					CString str;
					str.Format("%d", i);
					pCmb->AddString(str);
				}
				CString str1;
				str1.Format("%d", ptz->zoneValue);
				m_sCurrentZone = str1; 
			}
		}
	}
	else
	{
		//ʹ��from-to
		if(pCpu->from_zone <= pCpu->to_zone)
		{
			for(int i = pCpu->from_zone; i <= pCpu->to_zone; i++)
			{
				CString str;
				str.Format("%d", i);
				pCmb->AddString(str);
			}
		}
		else
		{
			for(int i = pCpu->to_zone; i <= pCpu->from_zone; i++)
			{
				CString str;
				str.Format("%d", i);
				pCmb->AddString(str);
			}
		}
	}
	int nCurrent = pCmb->AddString(StringFromID(IDS_SETTING_CURRENTZONE));
	if(nCurrent >= 0)
	{
		pCmb->SetCurSel(nCurrent);
		//���浱ǰѡ��
		SaveSelZone(pCmb);
	}

	//����Ĭ��ѡ��
// 	if(pCmb->GetCount() > 0)
// 	{
// 		pCmb->SetCurSel(0);
// 		pCmb->GetLBText(0, m_sCurrentZone);
// 		//���浱ǰѡ��
// 		SaveSelZone(pCmb);
// 		UpdateData(FALSE);
// 		return;
// 	}
}

/*************************************************************
 �� �� ����ResetGroupTab()
 ���ܸ�Ҫ���������÷���TAB, �ڵ�һ�����������¹�������ʱ����
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B8D0222
void CPTSetting::ResetGroupTab()
{
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_PTSET);
	if(pTab == NULL)
		return;
	//���ԭ��������
	pTab->DeleteAllItems();

	if(m_pObj == NULL)
		return;

	if(g_PTSettingGroup != 1 && g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		pTab->InsertItem(0, StringFromID(IDS_SECPROP_ZONE));
		pTab->InsertItem(1, StringFromID(IDS_SECPROP_SETTING));
	}

	if(g_PTSettingGroup == 1)
	{
		pTab->InsertItem(0, StringFromID(IDS_SECPROP_SETTING));
		for(int i = 0; i < m_arrGroup.GetSize(); i++)
		{
			PT_GROUP * ptg = (PT_GROUP*)m_arrGroup.GetAt(i);
			CString str; //��ʽ: ���-����
			str.Format("%d-%s", ptg->Group, ptg->Name);
			int ii = i;
			pTab->InsertItem(++ii, str);
		}
	}
	

	//����Ĭ��
	if(pTab->GetItemCount() > 1)
	{
		pTab->SetCurSel(1);

		//���浱ǰѡ��
		SaveSelGroup(pTab);
	}
}

/*************************************************************
 �� �� ����SaveSelGroup()
 ���ܸ�Ҫ�����浱ǰѡ������
 �� �� ֵ: void
 ��    ����param1	����TAB
**************************************************************/
//##ModelId=49B87B8D0232
BOOL CPTSetting::SaveSelGroup( CTabCtrl * pTab )
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
	int nGroup = atoi(strGroup);
	if(nGroup != m_nGroup)
	{
		m_nGroup = atoi(strGroup);
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 �� �� ����SaveSelZone()
 ���ܸ�Ҫ�����浱ǰѡ��Ķ�ֵ����
 �� �� ֵ: void
 ��    ����param1	��ֵ����������
**************************************************************/
//##ModelId=49B87B8D0213
BOOL CPTSetting::SaveSelZone( CComboBox * pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CString sZone;
	pCmb->GetLBText(nIndex, sZone);
	if( sZone == StringFromID(IDS_SETTING_CURRENTZONE) )
	{
		sZone.Format("%d", CURRENT_ZONE);
	}
	if(sZone != m_sZone)
	{
		m_sZone = sZone;
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 �� �� ����SaveSelCPU()
 ���ܸ�Ҫ�����浱ǰѡ���CPU��
 �� �� ֵ: void
 ��    ����param1	CPU������
**************************************************************/
//##ModelId=49B87B8D01F4
BOOL CPTSetting::SaveSelCPU( CComboBox * pCmb )
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

//##ModelId=49B87B8E02AF
void CPTSetting::OnSelchangeTabPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//����ѡ��ֵ
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_PTSET);
	if(pTab == NULL)
		return;

	int nCurrentSel = pTab->GetCurSel();
	if (nCurrentSel == 0)
	{
		m_List.ShowWindow(false);
		m_ListZone.ShowWindow(true);		
	}
	else
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
		if(SaveSelGroup(pTab))
		{
			m_nLoadType = 2;
			ReFillAll();
		}
	}

	*pResult = 0;
}

//##ModelId=49B87B8E02BF
void CPTSetting::OnSelchangeCmbPtsetCpu() 
{
	// TODO: Add your control notification handler code here
	//ȡ�õ�ǰֵ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	if(SaveSelCPU(pCmb))
	{
		ReFillZone();	
		m_nLoadType = 0;
		ReFillAll();
	}
}

//##ModelId=49B87B8E02CE
void CPTSetting::OnSelchangeCmbPtsetSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SRC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	
	m_nSrc = nIndex;
}

//##ModelId=49B87B8E02DE
void CPTSetting::OnSelchangeCmbPtsetSelzone() 
{
	// TODO: Add your control notification handler code here
	//ȡ�õ�ǰֵ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb == NULL)
		return;

	if(SaveSelZone(pCmb))
	{
		m_nLoadType = 1;
		ReFillAll();
	}
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������LIST������ʾ���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02EE
void CPTSetting::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				//����������
				PT_SETTING *pts = (PT_SETTING *)m_List.GetItemData(nItem);
				if(pts != NULL)
				{
					if (pts->DataType == 2 || pts->DataType ==4)
					{
						lplvcd->clrTextBk = RGB(220, 220, 220);
					}
				}

				if(g_PTSettingCompare == 0)
				{
					//��ǰһ��ֵ�Ƚ�
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
				}
				else
				{
					//���׼ֵ�Ƚ�
					//ֵ�ĵ�һ�����׼ֵ����
					if(nSubItem == m_nNewValueIndex)
					{
						//ֵ
						CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex - 1);
						
						//ȥ�����߿ո�
						strValue.TrimLeft();
						strValue.TrimRight();
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						
						//�����ı���ɫ
						if(strValue != strOldValue)
							lplvcd->clrText = XJ_LIST_ONE_TWO;
					}
					//�ڶ������׼ֵ����
					if(nSubItem == m_nNewValueIndex + 1)
					{
						//ֵ
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
						CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex - 1);
						
						//ȥ���ո�
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						strOlderValue.TrimLeft();
						strOlderValue.TrimRight();
						
						//�����ı���ɫ
						if(strOldValue != strOlderValue)
							lplvcd->clrText = XJ_LIST_TWO_THREE;
					}
				}
				
				//����ֵ�б���ɫ,��ʾ���޸�
				if(nSubItem == m_nNewValueIndex)
				{
					//����ɫ����
					lplvcd->clrTextBk = RGB(200,255,255);
					if(m_arrSetting.GetSize() > nItem)
					{
						PT_SETTING *pts = m_arrSetting[nItem];
						if(pts != NULL)
						{
							if (pts->nRefType == 0)
							{
								lplvcd->clrTextBk = RGB(132, 132, 132);
							}
						}
					}
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
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ������LIST������ʾ���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02EE
void CPTSetting::OnCustomDraw1( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//����ֵ�б���ɫ,��ʾ���޸�
				if(nSubItem == 6)
				{
					//����ɫ����
					lplvcd->clrTextBk = RGB(200,255,255);
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
 �� �� ����OnClickListPtset()
 ���ܸ�Ҫ����Ӧ����¼�, ����ֵ�пɽ����޸Ĳ���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02FD
void CPTSetting::OnClickListPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	if (nSubItem == m_nNewValueIndex && nItem != -1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		//SetDlgItemText(IDC_REMIND, "˫������˳��༭״̬");
	}
	//�Ƿ��ʹ�ÿ��ƹ���
	if(g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//����ֵ��
				
				PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
				MYASSERT(pts);
				int nTextLength = -1;
				if(pts->nRefType != 0)
				{
					if(pts->DataType == 2 || pts->DataType == 4)
					{
						int nValueCount = 0;
						//������
						CString sName = pts->Name;
						if(!sName.IsEmpty())
						{
							int nFind = sName.Find(",", 0);
							if(nFind != -1)
							{
								sName.Delete(0, nFind+1);
								//��������
								nFind = sName.Find(",", 0);
								if(nFind != -1)
									nValueCount = atoi(sName.Left(nFind));
							}
						}
						if(nValueCount > 0)
						{
							if(pts->DataType == 2)
							{
								//16����
								int nTemp = nValueCount%4;
								nTextLength = (nValueCount+4-nTemp)/4;
							}
							if(pts->DataType == 4)
							{
								//2����
								nTextLength = nValueCount;
							}
						}
					}
					m_List.EditSubItem(nItem, nSubItem, atof(pts->minValue), atof(pts->maxValue),3,pts->DataType, atof(pts->stepValue), nTextLength,g_PTSettingJudge, 1==pts->ntimeMStoS);
				}
				
				/*
				CString str = m_List.GetItemText(nItem, nSubItem);
				str = str.Left(str.Find("*", 0));
				int nValue = atoi(str);
				PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
				MYASSERT(pts);
				if (nValue > pts->maxValue)
				{
					AfxMessageBox("�޸ĺ�Ķ�ֵ�������ֵ!");
				}
				*/
			}
		}
	}
	*pResult = 0;
}

//##ModelId=49B87B8E030D
void CPTSetting::OnTimer(UINT nIDEvent) 
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
			//str = "�ٻ���ֵ��ʱ!";
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);
			break;
		case 2:
			//str = "�ٻ���ֵ���ų�ʱ!";
			m_nCurrentStatus = 0;
			m_bCallZoneSuccess = false;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
			pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);
			break;
		case 3:
			//str = "�޸Ķ�ֵ��ʱ!";
			if(m_bChecking)
			{
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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
				
				pApp->AddNewManOperator("��ֵ�޸�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
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
				pApp->AddNewManOperator("��ֵ�޸�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		case 4:
			//str = "��ֵ���л���ʱ!";
			if(m_bChecking)
			{
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)	
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("��ֵ���л�ԤУ", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("��ֵ���л�ִ��", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		default:
			str = "";
			break;
		}
		if(m_nCurrentStatus == 1 && m_nFrameCount > 0)
		{
			//���ڽ��յ��Ƕ�֡����, ��ջ���
			ClearMultiFrame();
		}

		//�ı�״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue(2);
		AfxMessageBox(str);
	}
	
	if (nIDEvent == m_nPTSetTimer){
		KillTimer(m_nPTSetTimer);
		
		CString str;
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();

		CXJPTSetStore *pStore = CXJPTSetStore::GetInstance();
		QPTSetStateTable *pState = pStore->GetState();
		
		int nPTSetState = pState->GetStateID();
		int nHangoutType = pState->GetType();

		CString sRunnerUserID = pState->GetRunnerUserID().constData();
		CString sOperUserID = pState->GetOperUserID().constData();
		
		// ��ֵ�޸İ�ť�Ƿ���ã� 
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_OPER_PTSET == nHangoutType
			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID)){
			m_btnModifySetting.EnableWindow(TRUE);
		}else{
			m_btnModifySetting.EnableWindow(FALSE);
		}

		// ��ֵ���л���ť�Ƿ���ã�
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_OPER_PTZONESET == nHangoutType
			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID)){
			m_btnModifyZone.EnableWindow(TRUE);
		}else{
			m_btnModifyZone.EnableWindow(FALSE);
		}

		// ��ֵ�޸İ�ť�Ƿ�ɼ�
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_OPER_PTSET == nHangoutType
			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
			m_btnModifySetting.ShowWindow(SW_SHOW);
		else
			m_btnModifySetting.ShowWindow(SW_HIDE);

		// ��ֵ���л���ť�Ƿ�ɼ�
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_OPER_PTZONESET == nHangoutType
			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
			m_btnModifyZone.ShowWindow(SW_SHOW);
		else
			m_btnModifyZone.ShowWindow(SW_HIDE);

		// �鿴���ƽ��Ȱ�ť�Ƿ�ɼ�
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& (XJ_OPER_PTSET == nHangoutType || XJ_OPER_PTZONESET ==nHangoutType)){
			m_btnViewPTSetProg.ShowWindow(SW_SHOW);
			if (XJ_OPER_PTZONESET == nHangoutType)
				m_btnViewPTSetProg.SetWindowText("��ֵ���޸Ľ���");
			else if (XJ_OPER_PTSET == nHangoutType)
				m_btnViewPTSetProg.SetWindowText("��ֵ�޸Ľ���");
			else
				m_btnViewPTSetProg.SetWindowText("�鿴�޸Ľ���");
		}
		else
			m_btnViewPTSetProg.ShowWindow(SW_HIDE);

		if (5 == m_pObj->m_nRunStatu && pApp->m_User.m_strUSER_ID == sOperUserID){
			if (4 == nPTSetState && 0 == m_nCurrentDetailStatus){	
				m_nCurrentDetailStatus = 1;

				AfxMessageBox("������Ա����֤��ֵ�����ݣ���ֵ�޸����ݽ��·�����վ������<ȷ��>��ִ�ж�ֵ�޸�"
					, MB_OK|MB_ICONINFORMATION);

				pStore->ReLoadState();
				nPTSetState = pState->GetStateID();
				if (0 == nPTSetState){
					AfxMessageBox("������Ա�Ѿ�ȡ������Ӧװ�õĹ��ƣ��˴ζ�ֵ�޸�ȡ����");
					RevertModifyValue();
					pStore->RevertModify();
					m_nCurrentDetailStatus = 0;
				}else{
					pApp->SetRevertModifyValueFlag(2);	// �������ʱȡ�����Ʋ���
					ExcutePTSet();
				}
			}
		}
		
		if (pState->GetFlags() == 1 && CString(pState->GetPTID().constData()) == m_pObj->m_sID){
			RevertModifyValue();
			pState->SetFlags(0);
			pStore->Save();
		}
		
		// ���ö�ʱ��
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	}

	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSETTING, str);
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
//##ModelId=49B87B8D0242
void CPTSetting::ClearMultiFrame()
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
//##ModelId=49B87B8D0251
void CPTSetting::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	if(pSttpData == NULL)
		return;
	
	//������ֵ����
	try
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

			CString sLog;
			sLog.Format("����[%s]�ٻ���ֵ�ɹ�!", m_pObj->m_sName);
			WriteLog(sLog, XJ_LOG_LV2);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_SUCCESS);
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, ����", XJ_LOG_LV3);
		return;
	}
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ����Ӧ�رմ�����Ϣ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F00FA
void CPTSetting::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87B8D02AF
BOOL CPTSetting::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTSETTING_LOADING)->ShowWindow(SW_SHOW);
	WriteLog("CPTSetting::ReFillAll,��ʼ", XJ_LOG_LV3);
	
	//�������
	ClearDataCol();
	if(2 != m_nLoadType)
	{
		EnterCriticalSection(&m_CriticalOper);
		m_HisValueMgr.ResetContent();
		LeaveCriticalSection(&m_CriticalOper);
	}
	if(0 == m_nLoadType)
	{
		ClearSettingGroup();
		ClearSettingConfig();
	}
	if(0 == m_nLoadType || 2 == m_nLoadType)
	{
		m_List.DeleteAllItems();
	}

	if(MODE_SUB == g_role)
	{
		//��������
		ReLoadData(0, m_nLoadType);
		//�������
		FillData(0, m_nLoadType);
	}
	else
		StartThread();

	WriteLog("CPTSetting::ReFillAll,����", XJ_LOG_LV3);
	return TRUE;
}

/*************************************************************
 �� �� ����ClearDataCol()
 ���ܸ�Ҫ��ɾ���¼ӵ�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0261
void CPTSetting::ClearDataCol()
{
	//���������Ч��
	if(m_List.GetSafeHwnd() == NULL)
		return;

	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ:	ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0262
void CPTSetting::CancelCall()
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
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20015, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ���ֵ����
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20106, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20051, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20053, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����л���ֵ��
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20047, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20055, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����SetSettingValue()
 ���ܸ�Ҫ������ָ���Ķ�ֵ
 �� �� ֵ: 
 ��    ����param1	��ֵID
		   Param2	��ֵֵ
		   Parme3	������
**************************************************************/
//##ModelId=49B87B8D0271
void CPTSetting::SetSettingValue( CString ID, CString VALUE, int iCol )
{
	//���ҵ�Setting_ID==ID������
	CString strID = ID;
	strID.TrimLeft();
	strID.TrimRight();
	
	PT_SETTING* pst = (PT_SETTING*)FindSetting(strID);
	if(pst == NULL)
		return;
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//���Ҵ�ֵ�ڵڼ���
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return;
	
	//����ListCtrl��Value������
	m_List.SetItemText(nItem, m_nNewValueIndex/*����ֵ*/, GetDisplayValue(pst, pst->Value));

	//ˢ��
	m_List.Update(nItem);
}

/*************************************************************
 �� �� ����RevertModifyValue()
 ���ܸ�Ҫ���ظ��޸Ķ�ֵʱ�޸ĵ�ֵ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0280
void CPTSetting::RevertModifyValue(int nType, int nFlag)
{
	if(nType == 1)
	{
		if(m_arrModifyList.GetSize() < 0)
			return;
		for(int i = 0; i < m_arrModifyList.GetSize(); i++)
		{
			STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
			CString strID;
			strID.Format("%d", sttpData->id);
			SetSettingValue(strID, "", 0);
		}
		//����޸ı��
		m_List.ClearEdited();

		// ��ԭ���ݿ�λ��
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		if (0 == nFlag)
			pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
	}
	else if(nType == 2)
	{
		if (m_ListZone.GetItemCount() != 0)
		{
			if(!m_pObj->m_bNewDevice)
			{
				m_ListZone.SetItemText(0, 6,m_sOldZoneValue);
				m_ListZone.Update(0);
			}
			else
			{
				m_ListZone.SetItemText(m_nModifyItem, 6,m_sOldZoneValue);
				m_ListZone.Update(m_nModifyItem);
			}
		}
		m_ListZone.ClearEdited();
	}

}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02B0
void CPTSetting::EndOwnerThread()
{
	WriteLog("��ʼ�˳�������ֵҳ���ȡ�����߳�", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("�˳�������ֵҳ���ȡ�����߳����", XJ_LOG_LV3);
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B8D0281
BOOL CPTSetting::IsVaild20069( int nMsgID )
{
	if(20015 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ���ֵ
		return TRUE;
	}
	if((20106 == nMsgID || 20173 == nMsgID)  && 2 == m_nCurrentStatus)
	{
		//�ٻ���ֵ����
		return TRUE;
	}
	if(20051 == nMsgID && 3 == m_nCurrentStatus)
	{
		//�޸Ķ�ֵ
		return TRUE;
	}
	if(20053 == nMsgID && 3 == m_nCurrentStatus)
	{
		//�޸Ķ�ֵ
		return TRUE;
	}
	if((20047 == nMsgID || 20169 == nMsgID) && 4 == m_nCurrentStatus)
	{
		//��ֵ���л�
		return TRUE;
	}
	if((20055 == nMsgID || 20171 == nMsgID) && 4 == m_nCurrentStatus)
	{
		//��ֵ���л�
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B8E031C
BOOL CPTSetting::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTSET);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTZONE);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 �� �� ����FindSetting()
 ���ܸ�Ҫ�����ݶ�ֵID�ҵ���ֵ����
 �� �� ֵ: ��ֵ����
 ��    ����param1	��ֵID
		   Param2
**************************************************************/
//##ModelId=49B87B8D0291
PT_SETTING* CPTSetting::FindSetting( CString sID )
{
	PT_SETTING* pst = NULL;
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(pts->ID == sID)
		{
			pst = pts;
			break;
		}
	}
	return pst;
}

/*************************************************************
 �� �� ����LoadSettingGroup()
 ���ܸ�Ҫ�����Ҷ�ֵ��
 �� �� ֵ: ���ҳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0020
BOOL CPTSetting::LoadSettingGroup(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	WriteLogEx("CPTSetting::LoadSettingGroup, ��ѯ��ʼ");

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
		
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadSettingGroup, ��ѯʧ��");
		return FALSE;
	}

	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,��ѯ��%d������", nCount);
		WriteLogEx(str);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			//������ֵ�����
			PT_GROUP * group = new PT_GROUP;
			if(group == NULL)
			{
				pMemset.MoveNext();
				continue;
			}
			CString str = pMemset.GetValue((UINT)0); //103group
			
			group->Group = atoi(str);
			group->Name = ""; //name
			
			m_arrGroup.Add(group);

			if(i == 0)
				m_nGroup = group->Group;
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	if(m_arrGroup.GetSize() > 0)
		LoadPTGroupConfig(nQueryNo);

	WriteLogEx("CPTSetting::LoadSettingGroup, ��ѯ��������");

	return bReturn;
}

/*************************************************************
 �� �� ����ClearSettingGroup()
 ���ܸ�Ҫ�������ֵ������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8C031C
BOOL CPTSetting::ClearSettingGroup()
{
	EnterCriticalSection(&m_CriticalOper);
	//�����ֵ������
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		delete pg;
		pg = NULL;
	}
	m_arrGroup.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 �� �� ����SetGroupName()
 ���ܸ�Ҫ�����ö�ֵ�������
 �� �� ֵ: void
 ��    ����param1	��ֵ��ID
		   Param2	��ֵ������
**************************************************************/
//##ModelId=49B87B8D029F
void CPTSetting::SetGroupName( int nGroupID, CString sName )
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			pg->Name = sName;
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8F0109
void CPTSetting::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	//�ж�����/��ʾ ����TAB��
	if(g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//��ʾ
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_SHOW);
	}
	else
	{
		//����
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_HIDE);
	}
	OnSize(0, 0, 0);
	m_nLoadType = 0;
	ReFillAll();	
}

//##ModelId=49B87B8E032C
void CPTSetting::OnBtnPtsetStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

/*************************************************************
 �� �� ����LoadNewOrderTime()
 ���ܸ�Ҫ����ѯ���µĻ�׼ֵʱ��
 �� �� ֵ: ��׼ֵʱ��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D001F
CString CPTSetting::LoadNewOrderTime()
{
	CString strReturn = "";
	if(m_pObj == NULL)
		return "";
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//����PT_ID, CPU, Zone, ����Time�Ӵ�С����,ȡ����һ��
	
	//top
	CString str;	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "TIME", EX_STTP_DATA_TYPE_TIME);
	
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	//zone
	Condition condition3;
	int nzone = atoi(m_sZone);
	if(nzone == CURRENT_ZONE)
		nzone = atoi(m_sCurrentZone);
	str.Format("ZONE = %s", m_sZone);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
	
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
		WriteLogEx("CPTSetting::LoadNewOrderTime, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str = "";
		if(nCount == 0)
		{
			CString str;
			str.Format("����%s, CPU��%s, ZONE��%s, û�ж�ֵ��׼ֵ", m_pObj->m_sID, m_sCPU, m_sZone);
			WriteLog(str, XJ_LOG_LV3);
		}
		if(nCount == 1)
		{
			strReturn = pMemset->GetValue((UINT)0); //ֻ����һ��
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadNewOrderTime,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return strReturn;
}

//##ModelId=49B87B8D03B9
void CPTSetting::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_SETTING));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_SETTING));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ����TranslateSingleKey()
 ���ܸ�Ҫ�����͵����ؼ��ֵ�����
 �� �� ֵ: �ؼ��ֱ�ʾ��ֵ
 ��    ����param1	�ؼ���
		   Param2
**************************************************************/
//##ModelId=49B87B8D02BF
CString CPTSetting::TranslateSingleKey( CString sKey )
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
		if(strTail == "PT_ROUTINE_ORDER$")
		{
			return m_List.GetItemText(nIndex, 4); //��ֵ��׼ֵ
		}
		if(strTail == "PT_ROUTINE_DATATYPE$")
		{
			return m_List.GetItemText(nIndex, 10);//��������
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
			if(sKey == "$PT_SEL_ZONE$") //��ǰ��ѡZONE
				return m_sZone;
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
 �� �� ����OnBtnPtsetPrint()
 ���ܸ�Ҫ����ӡ��ֵ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E032E
void CPTSetting::OnBtnPtsetPrint() 
{
	// TODO: Add your control notification handler code here
}

/*************************************************************
 �� �� ����InitCOM()
 ���ܸ�Ҫ����ʼ����ӡģ��COM���
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02C1
BOOL CPTSetting::InitCOM()
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
//##ModelId=49B87B8D02C2
void CPTSetting::ReleaseCOM()
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
 �� �� ����DoPrintSetting()
 ���ܸ�Ҫ����ӡ��ֵ
 �� �� ֵ: void
 ��    ����param1	ģ���ļ�ȫ·��
		   Param2
**************************************************************/
//##ModelId=49B87B8D02CE
void CPTSetting::DoPrintSetting( CString strPath )
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
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//���ûص�����
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRSetting);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTSetting);
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

//##ModelId=49B87B8E033C
void CPTSetting::OnSettingprintNew1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_NEW1_TP);
}

//##ModelId=49B87B8E033E
void CPTSetting::OnSettingprintNew2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_NEW2_TP);
}

//##ModelId=49B87B8E0340
void CPTSetting::OnSettingprintNew3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_NEW3_TP);
}

//##ModelId=49B87B8E0342
void CPTSetting::OnSettingprintOrder1() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_ORDER1_TP);
}

//##ModelId=49B87B8E0344
void CPTSetting::OnSettingprintOrder2() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_ORDER2_TP);
}

//##ModelId=49B87B8E0346
void CPTSetting::OnSettingprintOrder3() 
{
	// TODO: Add your command handler code here
	//��ӡ
	DoPrintSetting(SETTING_ORDER3_TP);
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ20157, ��ʱ����֪ͨ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F00CB
void CPTSetting::OnSTTP20157( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4)
	{
		//û�д����ٻ���ֵ״̬,Ҳû�д����ٻ���ֵ����״̬,
		//Ҳû�д����޸Ķ�ֵ״̬, Ҳû�д��ڶ�ֵ���л�״̬,��������
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
		int nTimeOut = pData->m_nPTSettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ���ֵ����				
		int nTimeOut = pData->m_nPTZoneTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ
		
		int nTimeOut = pData->m_nModifySettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����л���ֵ��				
		int nTimeOut = pData->m_nChangeZoneTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

/*************************************************************
 �� �� ��: LoadCurrentZone()
 ���ܸ�Ҫ: �����ݿ��ѯ��ǰ��ֵ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CPTSetting::LoadCurrentZone()
{
	if(g_role != MODE_SUB && m_bExitThread)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", 1);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("pt_id = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition3;
	str.Format("CPU_CODE=%d", atoi(m_sCPU));
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
		
	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by CURTIME DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
			
	CMemSet pMemset;

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ZONE_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CPTSetting::LoadCurrentZone, ��ѯʧ��", XJ_LOG_LV3);
		return;
	}
	if(g_role != MODE_SUB && m_bExitThread)
		return;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount == 1)
		{
			CString str;
			str.Format("CPTSetting::LoadCurrentZone, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);

			m_sCurrentZone = pMemset.GetValue((UINT)3);
			//m_sZone = m_sCurrentZone;
		}	
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadCurrentZone,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	WriteLog("CPTSetting::LoadCurrentZone, ��ѯ��������", XJ_LOG_LV3);
}

void CPTSetting::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTSetting::OnDoubleclickedBtnPtsetCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSetting::OnDoubleclickedBtnPtsetCallzone() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSetting::OnBtnPtsetVerify0() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	int nDZModState = pApp->GetPTSetModState(PT_ZONE(), CString());

	//������Աȷ��
	CDlgCheckPro dlg(this, 0);
	dlg.m_strModify = m_strOutPut;
	dlg.m_sZone = m_sZone;
	dlg.m_sCPU = m_sCPU;
	if(dlg.DoModal() == IDOK)
	{
		if(g_PTControlModel == 1)
		{
			//��ͨģʽ,Ҫ��������Ա��֤
			CDlgValidateID dlgID(0);
			dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
			if(dlgID.DoModal() == IDOK)
			{
				m_sRunUser = dlgID.m_strUser;
				
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CString str;
				str.Format("�û�%s������Ա�����֤�ɹ�:�޸Ķ�ֵ", m_sRunUser);
				WriteLog(str);
				pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sRunUser, -1, OPER_SUCCESS,m_nOperationNum);
				
				// ������Ա�˶Ժ��޸�״̬��
				//pApp->ModifyDZModState(4, m_sRunUser, m_pObj);
				PT_ZONE zone;
				pApp->GetPTSetModState(zone);
				pApp->NextPTSetModState(4, zone, m_sRunUser);
			}
			else
			{
				//�޲���Ȩ��
				//�ظ��޸�ǰ��ֵ
				RevertModifyValue();
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CString str;
				str.Format("�û�%s������Ա�����֤ʧ��:�޸Ķ�ֵ", m_sRunUser);
				WriteLog(str, XJ_LOG_LV2);
				pApp->AddNewManOperator("�û���֤", m_pObj->m_sID, str, m_sRunUser, -1, OPER_FAILD,m_nOperationNum);

				return;
			}
		}
	}
	else
	{
		//��ͬ���޸�
		//�ظ��޸�ǰ��ֵ
		RevertModifyValue();
		UpdateControlsEnable();
		
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;

		return;
	}
}

void CPTSetting::OnBtnPtsetVerify1() 
{
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
	
	return;
}

void CPTSetting::OnBtnPtsetVerify2() 
{
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
			m_btnModifySetting.EnableWindow(TRUE);
			m_btnModifyZone.EnableWindow(TRUE);
		}
	}
}

void CPTSetting::OnDblclkListPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

 	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	CString sCurZone;
	CString sSelZone;
	
	GetDlgItem(IDC_EDIT_PTSET_CURZONE)->GetWindowText(sCurZone);
	GetDlgItem(IDC_CMB_PTSET_SELZONE)->GetWindowText(sSelZone);

	
	if(nItem != -1)
	{
		PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
		MYASSERT(pts);
		
		if(g_PTControl && nSubItem == m_nNewValueIndex /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
		{
			
			if( atoi(m_sZone) == CURRENT_ZONE )
			{
				AfxMessageBox(StringFromID(IDS_MODIFYSETTING_SELZONE));
				return;
			}
			if(g_PTNotModifyCurrentZone)
			{
				if (sCurZone == sSelZone)
				{
					AfxMessageBox(StringFromID(IDS_MODIFYSETTING_NOTCURRENTZONE));
					return;
				}
			}
			if(pts->nRefType == 0)
			{
				AfxMessageBox(StringFromID(IDS_MODIFYSETTING_NOTPARAM));
				return;
			}
			SetDlgItemText(IDC_REMIND, StringFromID(IDS_TIP_QUIT_EDITMODE));
			if (CheckVPicklist(pts->VPickList))
			{
				//����ֵ��
				CString str;
				str.Format("%s",pts->VPickList);
				int nMax =GetTranslatedNumber(str);
				str = "";
				for (int i = -1 ;i <= nMax; i++)
				{
					CString sID;
					CString sReturn;
					sID.Format("%d",i);
					sReturn = GetTranslatedValue(pts,sID);
					if (sID == sReturn )
						continue;
					str += sReturn;
					if (i != nMax)
					{
						str += ";";
					}	
				}
				m_List.ListSubItem(nItem, nSubItem, str);
			}
			else
			{
				//����ֵ��
				int nTextLength = -1;
				if(pts->DataType == 2 || pts->DataType == 4)
				{
					int nValueCount = 0;
					//������
					CString sName = pts->Name;
					if(!sName.IsEmpty())
					{
						int nFind = sName.Find(",", 0);
						if(nFind != -1)
						{
							sName.Delete(0, nFind+1);
							//��������
							nFind = sName.Find(",", 0);
							if(nFind != -1)
								nValueCount = atoi(sName.Left(nFind));
						}
					}
					if(nValueCount > 0)
					{
						if(pts->DataType == 2)
						{
							//16����
							int nTemp = nValueCount%4;
							nTextLength = (nValueCount+4-nTemp)/4;
						}
						if(pts->DataType == 4)
						{
							//2����
							nTextLength = nValueCount;
						}
					}
				}
				m_List.EditSubItem(nItem, nSubItem, atof(pts->minValue), atof(pts->maxValue),3,pts->DataType, atof(pts->stepValue), nTextLength,g_PTSettingJudge, 1==pts->ntimeMStoS);
			}
		}
		else
		{
			pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
			if ((pts->DataType == 2 || pts->DataType == 4)&& nSubItem < m_nNewValueIndex)
			{
				pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
				//��ʼ���Ի���
				CDlgSettingDetail dlg;
				dlg.m_pts = pts;
				dlg.DoModal();
 			}
		}	
	}
	*pResult = 0;
}

void CPTSetting::OnClickListPtzone(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	m_nModifyItem = 0;

	//�Ƿ��ʹ�ÿ��ƹ���
	if(g_PTControl != 1 /*|| m_pObj->m_nType == TYPE_WAVEREC*/)
		return;
	if(g_PTModifyZone != 1)
		return;
	if(nItem != -1)
	{
		if(nSubItem == m_nNewZoneIndex)
		{
			CString str = m_ListZone.GetItemText(nItem, 0);
			//����ֵ��
			if(m_pObj->m_bNewDevice == 1)
			{
				if (m_pObj->m_bNewDevice /*&& str.Find("�༭") != -1*/)
				{
					for (int j = 0; j < m_ListZone.GetItemCount(); j++)
					{
						CString str = m_ListZone.GetItemText(j, 6);
						if (str.Find("*") != -1)
						{
							if (j != nItem)
							{
								AfxMessageBox(StringFromID(IDS_SWITCHZONE_ONLYONE));
								return;
							}
						}
					}

					if(str.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
					{
						AfxMessageBox(StringFromID(IDS_MODIFYSETTING_EDITZONE));
						return;
					}
					PT_ZONE* ptz = (PT_ZONE*)m_ListZone.GetItemData(nItem);
					MYASSERT(ptz);
					
					m_nModifyItem = nItem;

					m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);

					m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2, 1, 1);
					
					/*if (str.Find("����") != -1)
					{
						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2);
					}
					else
					{
						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue);	
					}*/
				}
			}
			else
			{
				for (int j = 0; j < m_ListZone.GetItemCount(); j++)
				{
					CString str = m_ListZone.GetItemText(j, 6);
					if (str.Find("*") != -1)
					{
						if (j != nItem)
						{
							AfxMessageBox(StringFromID(IDS_SWITCHZONE_ONLYONE));
							return;
						}
					}
				}

				m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);

				m_ListZone.EditSubItem(nItem, nSubItem, atoi(m_ListZone.GetItemText(nItem, 3)), atoi(m_ListZone.GetItemText(nItem, 4)), 3, 1,1);
			}
		}
	}
	*pResult = 0;
}



/*************************************************************
 �� �� ����GetZone()
 ���ܸ�Ҫ�������ݿ���Ҷ�Ӧ�Ķ�ֵ����
 �� �� ֵ: ���ҳɹ�����TRUE, ʧ�ܻ������ݷ���FALSE
**************************************************************/
//##ModelId=49B87BA8023A
BOOL CPTSetting::GetZone(CString pt_id, int nCpu, int zone_id, PT_ZONE* pt_zone)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "zone_value";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);
		
	//����
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", pt_id);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_Code = %d", nCpu);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("zone_id = %d", zone_id);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CPTSetting::GetZone, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			bReturn = FALSE;
		}
		else
		{
			CString str = pMemset->GetValue((UINT)0);
			pt_zone->zoneValue = atoi(str);
		}
	
	}
	else
	{
				
		CString str;
		str.Format("CPTSetting::GetZone,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

BOOL CPTSetting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CViewBase::PreTranslateMessage(pMsg);
}

void CPTSetting::StartThread()
{
	//�����߳�
	
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTSettingLoad,this,THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("������ֵ��ѯ�߳������ɹ�,��ѯ����:%d", m_nQueryNo);
	WriteLog(strLog);
}

/****************************************************
Date:2012/3/13  Author:LYH
������:   FindNewValue	
����ֵ:   CString	
���ܸ�Ҫ: 
����: int nID	
*****************************************************/
CString CPTSetting::FindNewValue( int nID )
{
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(pts == NULL)
			continue;
		if(atoi(pts->ID) == nID)
			return pts->Value;
	}
	return "";
}

/*
void CPTSetting::ShowTip()
{
	m_tool.UpdateTipText("˫���˳��༭״̬", this);
}
*/


void CPTSetting::OnDblclkListPtzone(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	m_nModifyItem = 0;

	//�Ƿ��ʹ�ÿ��ƹ���
	if(g_PTControl != 1 /*|| m_pObj->m_nType == TYPE_WAVEREC*/)
		return;
	if(g_PTModifyZone != 1)
		return;
	if(nItem != -1)
	{
		if(nSubItem == m_nNewZoneIndex)
		{
			PT_ZONE *pZone = (PT_ZONE *)m_ListZone.GetItemData(nItem);
			if (pZone!=NULL)
			{
				CString str;
				//����ֵ��
				if (pZone->zoneValue != NULL)
				{
					
					str.Format("%d", pZone->zoneValue);
				}
				else
				{
					//str.Format("%d;%d;%d", 3, 4, 5);
					for(int i = pZone->minValue; i <= pZone->maxValue ; i++)
					{
						CString str2;
						if (i == pZone->maxValue)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
						
					}
					
				}
				CString sOld = m_ListZone.GetItemText(nItem, m_nNewZoneIndex);
				if( sOld.Find('*') == -1 )
					m_sOldZoneValue = sOld;
				m_ListZone.ListSubItem(nItem, nSubItem, str);
//				break;
			}
			else
			{
				CSecCPU* pCpu = NULL;
				for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
				{
					pCpu = m_pObj->m_arrCPU[k];
					if (pCpu->code == atoi(m_sCPU))
					{
						break;
					}
				}
				CString str;
				str = "";
				if(pCpu->from_zone <= pCpu->to_zone)
				{
					for(int i = pCpu->from_zone; i <= pCpu->to_zone; i++)
					{
						CString str2;
						if (i == pCpu->to_zone)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
					}
				}
				else
				{
					for(int i = pCpu->to_zone; i <= pCpu->from_zone; i++)
					{
						CString str2;
						if (i == pCpu->from_zone)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
					}
				}
				CString sOld = m_ListZone.GetItemText(nItem, m_nNewZoneIndex);
				if( sOld.Find('*') == -1 )
					m_sOldZoneValue = sOld;
				m_ListZone.ListSubItem(nItem, nSubItem, str);
			}
		}
	}

// 	if(nItem != -1)
// 	{
// 		if(nSubItem == m_nNewZoneIndex)
// 		{
// 			CString str = m_ListZone.GetItemText(nItem, 0);
// 			//����ֵ��
// 			if(m_pObj->m_bNewDevice == 1)
// 			{
// 				if (m_pObj->m_bNewDevice /*&& str.Find("�༭") != -1*/)
// 				{
// 					for (int j = 0; j < m_ListZone.GetItemCount(); j++)
// 					{
// 						CString str = m_ListZone.GetItemText(j, 6);
// 						if (str.Find("*") != -1)
// 						{
// 							if (j != nItem)
// 							{
// 								AfxMessageBox("��֧�ֶ�һ�ֵ�������޸�!");
// 								return;
// 							}
// 						}
// 					}
// 
// 					if(str.Find("�༭") != -1)
// 					{
// 						AfxMessageBox("�༭����ֵ�����޸�!");
// 						return;
// 					}
// 					PT_ZONE* ptz = (PT_ZONE*)m_ListZone.GetItemData(nItem);
// 					MYASSERT(ptz);
// 					
// 					m_nModifyItem = nItem;
// 
// 					m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);
// 
// 					m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2, 1, 1);
// 					
// 					/*if (str.Find("����") != -1)
// 					{
// 						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2);
// 					}
// 					else
// 					{
// 						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue);	
// 					}*/
// 				}
// 			}
// 			else
// 			{
// 				for (int j = 0; j < m_ListZone.GetItemCount(); j++)
// 				{
// 					CString str = m_ListZone.GetItemText(j, 6);
// 					if (str.Find("*") != -1)
// 					{
// 						if (j != nItem)
// 						{
// 							AfxMessageBox("��֧�ֶ�һ�ֵ�������޸�!");
// 							return;
// 						}
// 					}
// 				}
// 
// 				m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);
// 
// 				m_ListZone.EditSubItem(nItem, nSubItem, atoi(m_ListZone.GetItemText(nItem, 3)), atoi(m_ListZone.GetItemText(nItem, 4)), 3, 1,1);
// 			}
// 		}
// 	}
	*pResult = 0;
}
/****************************************************
Date:2012/10/22  Author:LY
������:   GetTranslatedNumber	
����ֵ:   int	
���ܸ�Ҫ: 
����: CString str	
*****************************************************/
int CPTSetting::GetTranslatedNumber(CString str)
{
	int nID = -1;
	CString sID = "";
	//����VPickList���Value����,VPickList����ʱ��Valueֵ�Ľ��͸���VPivkListȷ�� 
	//��VPickList="0:on;1:off" �� Value=1 ʱ��strValueΪ"off"
	//��VPickListΪ�գ���Value=1��Unit="s" ��strValue = "1s"  
	CString strTemp = "";

	CString strVPickList = str;

	
	//ȥ������Ŀո�

	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (!strVPickList.IsEmpty())
	{
		int nFenhao =0;	//�ֺŵ�λ��
		int nMaohao =0; //ð�ŵ�λ��
		BOOL bFound = FALSE;
		
		//ֻҪstrVPickList���գ��������ֺ�֮����ַ������������ð����ߵ�ֵ�Ƿ���strReturn��ͬ
		//�����ͬ��stValue��ð���ұߵ��ִ�ȡ��
		while (!strVPickList.IsEmpty()) 	 
		{
			nFenhao = strVPickList.Find(';', 0);
			if(nFenhao>0)
				strTemp = strVPickList.Left(nFenhao);
			else
				strTemp = strVPickList;
			
			nMaohao = strTemp.Find(':', 0);			 
			if (nMaohao>0)
			{
				sID = strTemp.Left(nMaohao);
			}
			
			if (nFenhao>0)
				strVPickList.Delete(0, nFenhao+1);
			else
				break;
		}
	}
	nID = atoi(sID);
	return nID;
}

/****************************************************
Date:2012/11/27  Author:LYH
������:   GetGroupName	
����ֵ:   CString	
���ܸ�Ҫ: 
����: int nGroupID	
*****************************************************/
CString CPTSetting::GetGroupName( int nGroupID )
{
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			return pg->Name;
		}
	}
	return "";
}
/****************************************************
Date:2012/11/29  Author:LY
������:   CheckVPicklist	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: CString strVPickList
*****************************************************/
BOOL CPTSetting::CheckVPicklist(CString strVPickList)
{
	BOOL bRutern = FALSE;
	if (strVPickList == "")
	{
		return bRutern;
	}
	CString sCopy = strVPickList;
	while (!sCopy.IsEmpty())
	{
		int nMaohao = sCopy.Find(':',0);
		int nFenhao = sCopy.Find(';', 0);
		if (nMaohao<0 || nFenhao<0 || nMaohao>nFenhao || nMaohao == nFenhao-1 || nMaohao == 0) //��1.�Ҳ����ֺŻ���ð��2.ð���ڷֺź���3.ð���ڵ�һ��4.����ð�źͷֺ��ǰ��ŵ� ���ش���
		{	
			return bRutern;
		}
		sCopy.Delete(0, nMaohao+1);
		nMaohao = sCopy.Find(':',0);
		nFenhao = sCopy.Find(';', 0);
		if (nMaohao<nFenhao && nMaohao != -1)   //��һ���ֺ�ǰ������ð�� ���ش���
		{
			return bRutern; 
		}
		sCopy.Delete(0,nFenhao+1); //ɾ����ǰ��һ���ֺ�ǰ���ַ�
	}
	return TRUE;

}
/****************************************************
Date:2012/12/3  Author:LY
������:   SetFloatValue	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString strVPickList
*****************************************************/
CString CPTSetting::SetFloatValue(PT_SETTING *pst, CString strValue)
{
	if (pst==NULL)
	{
		return strValue;
	}
	if (pst->DataType == 0)
	{
		CString sValue = strValue;
		if (sValue != "")
		{
			CString sPrecision = pst->Precision;
			int nDouhao = sPrecision.Find(",");
			sPrecision.Delete(0,nDouhao+1);
			int nAdd = 3;
			sPrecision.TrimLeft();
			sPrecision.TrimRight();
			if(!sPrecision.IsEmpty())
			{
				nAdd = atoi(sPrecision);			//����
			}
			
			float fValue = atof(sValue);
			CString sFormat;
			sFormat.Format("%%.0%df", nAdd);
			sValue.Format(sFormat, fValue);
			return sValue;
		}
	}
	return strValue;
}


int CPTSetting::GetNextQueryNo()
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


void CPTSetting::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("�˳�������ֵ��ѯ�߳�,��ѯ��:%d", nNo);
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

	str.Format("������ֵ��ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTSetting::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("������ֵ��ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTSetting::GetFirstNode()
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

/****************************************************
Date:2013/12/23  Author:LYH
������:   SetZoneCmbToCurrentZone	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTSetting::SetZoneCmbToCurrentZone()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb != NULL)
	{
		pCmb->SelectString(-1, m_sCurrentZone);
		SaveSelZone(pCmb);
	}
}

void CPTSetting::AddValueCol( int nQueryNo )
{
	if(g_PTSettingValues <= 0)
		return;
	if(m_arrSetting.GetSize() == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//����ֵ��
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTSetting::AddValueCol, ��ʼ����ֵ��", XJ_LOG_LV3);
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_HisValueMgr.GetCount(); i++)
	{
		strTime = m_HisValueMgr.GetTime(i);
		if(strTime.ReverseFind('.') != -1)
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		else
			strTimeDisplay = strTime;
		if(IsCurrentQueryNo(nQueryNo) || MODE_SUB == g_role)
		{
			m_List.InsertColumn(m_nNewValueIndex+i, strTimeDisplay, LVCFMT_LEFT, 150, 0);
			CString strLog;
			strLog.Format("��ӵ�%d�ж�ֵ��ʷֵ��,ʱ��:%s", m_nNewValueIndex+i+1, strTimeDisplay);
			WriteLog(strLog);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	
	WriteLog("CPTSetting::AddValueCol, ����ֵ�����", XJ_LOG_LV3);
}

void CPTSetting::FillHisData( int nQueryNo )
{
	if(g_PTSettingValues <= 0)
		return;
	if(m_arrSetting.GetSize() == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	WriteLog("CPTSetting::FillHisData, ��ʼ�����ʷֵ", XJ_LOG_LV3);
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	if(m_HisValueMgr.GetCount() > 0)
	{
		for(int i = 0; i < m_List.GetItemCount(); i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
				break;
			PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(i);
			if(!pts)
				continue;
			for(int k = 0; k < m_HisValueMgr.GetCount(); k++)
			{				
				CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
				if( 0 == k )
				{					
					pts->Value = sHisValue;
				}
				m_List.SetItemText(i, m_nNewValueIndex+k, GetDisplayValue(pts, sHisValue));
			}
		}
	}
	//�ָ�ˢ��
	LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	WriteLog("CPTSetting::FillHisData, ��ʷֵ������", XJ_LOG_LV3);
}

CString CPTSetting::GetDisplayValue( PT_SETTING* pst, CString strValue )
{
	CString strReturn = SetFloatValue(pst, strValue);
	strReturn = GetTranslatedValue(pst, strReturn);
	if(pst->ntimeMStoS == 1)
	{
		float fValue = atof(strReturn);
		fValue /= 1000;
		strReturn.Format("%.3f", fValue);
	}

	if( pst->DataType == 2 && !strReturn.IsEmpty() ) //16���ƿ�����
	{
		//��16���ƿ�����תΪ0x0000 �ĸ�ʽ������4λ�Ľ���ȫ
		CString str = strReturn;
		str.MakeLower();
		int nfind = str.Find( "0x", 0 );
		if( nfind == 0 )
		{
			//��0x��ͷ
			str.Delete( 0, 2 );
			strReturn.Delete( 0, 2 );
		}
		int noffset = 0;
		if( str.GetLength()%4 != 0 )
			noffset = 4 - str.GetLength() % 4;
		CString strAdd = "0x";
		if( nfind < 0 && g_PTSetting160x == 0 ) //����û��0x�����Ҳ�ǿ�����
			strAdd = "";
		for ( int i = 0; i < noffset; i++ )
		{
			strAdd += "0";
		}
		strReturn = strAdd + strReturn;		
	}
	return strReturn;
}

void CPTSetting::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//���ڵ�����
		return;
	}
	//�������
	FillData(wParam, lParam);
}

/****************************************************
Date:2014/1/9  Author:LYH
������:   FillData	
����ֵ:   void	
���ܸ�Ҫ: 
����: int nQueryNo	
����: int nLoadType	
*****************************************************/
void CPTSetting::FillData( int nQueryNo, int nLoadType )
{
	if(0 == nLoadType)
	{
		ResetGroupTab();
		//SetZoneCmbToCurrentZone();
		UpdateData(FALSE);
	}
	if(0 == nLoadType || 2 == nLoadType)
	{
		FillListData(nQueryNo);
	}
	
	AddValueCol(nQueryNo);
	FillHisData(nQueryNo);
	GetDlgItem(IDC_STATIC_PTSETTING_LOADING)->ShowWindow(SW_HIDE);
}

BOOL CPTSetting::ReLoadData( int nQueryNo, int nLoadType )
{
	BOOL bLoad = TRUE;
	//ֱ������
	if(0 == nLoadType)
	{
		LoadCurrentZone();
		LoadSettingGroup(nQueryNo);
		bLoad = LoadPTSettingConfig(nQueryNo);
		if(bLoad)
		{
			//��ѯ��׼ֵ
			LoadOrderValue(0);
		}
	}
	if(bLoad && 2 != nLoadType)
	{
		LoadPTHistorySettingData(nQueryNo);
	}
	return bLoad;
}

/****************************************************
Date:2014/1/11  Author:LYH
������:   GetThreadCount	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CPTSetting::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTSetting::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLog("CPTSetting::LoadDefaultCpu, ��ѯ��ʼ", XJ_LOG_LV3);
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadDefaultCpu, ��ѯʧ��");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadDefaultCpu,��ѯ��%d������", nCount);
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
		str.Format("CPTSetting::LoadDefaultCpu,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTSetting::LoadDefaultCpu, ��ѯ����", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTSetting::OnBtnPtsetExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString sZone = m_sZone;
	if( atoi(m_sZone) == 99 )
	{
		sZone = m_sCurrentZone;
	}
	CString strName;
	strName.Format("%s(%s-%s[%s]-%s[%s])-%s.xls", StringFromID(IDS_SECPROP_SETTING),m_pObj->m_sName, 
		StringFromID(IDS_MODEL_CPU),m_sCPU, StringFromID(IDS_SECPROP_ZONE),sZone, t.Format("%Y%m%d%H%M%S"));
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
