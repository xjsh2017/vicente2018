// PTGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTGeneral.h"
#include "DlgStatusHis.h"
#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/
UINT PTGeneralLoad(LPVOID pParam)
{
	CPTGeneral* ptGeneral = (CPTGeneral*)pParam;
	if(ptGeneral == NULL)
		return 0;
	int nQueryNo = ptGeneral->GetCurrentQueryNo();
	//��ȡ����
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//�����˳��߳�,�˳�
		if(ptGeneral->m_bExitThread)
			break;
		//�������Դ���
		if(nCount < 0)
			break;
		//��ȡ���, �˳�
		if(bLoad)
		{
			break;
		}
		if(!ptGeneral->IsCurrentQueryNo(nQueryNo))
		{
			break;
		}
		if(ptGeneral->ReFillAll(nQueryNo))
		{
			ptGeneral->PostMessage(WM_WINDOW_REFRESH, 0, 0);
			bLoad = TRUE;
		}
		else
		{
			//��ȡʧ��
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	ptGeneral->RemoveThreadNode(nQueryNo);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTGeneral

IMPLEMENT_DYNCREATE(CPTGeneral, CViewBase)

//##ModelId=49B87B91006F
CPTGeneral::CPTGeneral()
	: CViewBase(CPTGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPTGeneral)
	m_str103Addr = _T("");
	m_strAddr = _T("");
	m_strCOM = _T("");
	m_strCOMCheck = _T("");
	m_strComm = _T("");
	m_strCOMParam = _T("");
	m_strCOMPro = _T("");
	m_strOne = _T("");
	m_strRun = _T("");
	m_strTime = _T("");
	m_strType = _T("");
	m_strVer = _T("");
	m_strInsideTime = _T("");
	m_strIPA = _T("");
	m_strIPB = _T("");
	m_strPortA = _T("");
	m_strPortB = _T("");
	m_strSuspend = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_bMoreInfo = FALSE;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_bQuery = false;
	//m_bGetStatus = false;
	m_hIcon_open= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_GREEN_LIGHT));
	m_hIcon_close= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_RED_LIGHT));

	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);

}

//##ModelId=49B87B910203
CPTGeneral::~CPTGeneral()
{

}

//##ModelId=49B87B9101D4
void CPTGeneral::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTGeneral)
	DDX_Control(pDX, IDC_STAT_LIGHT, m_iconStatLight);
	DDX_Control(pDX, IDC_COMM_LIGHT, m_iconCommLight);
	DDX_Control(pDX, IDC_BTN_PTGEN_CALL, m_btnCommCall);
	DDX_Control(pDX, IDC_BTN_PTGEN_RUNHIS, m_btnRunHis);
	DDX_Control(pDX, IDC_BTN_PTGEN_COMMHIS, m_btnCommHis);
	DDX_Control(pDX, IDC_GIF_PTGENERAL, m_gif);
	DDX_Control(pDX, IDC_BTN_PTGEN_TIME, m_btnCallTime);
	DDX_Control(pDX, IDC_BTN_PTGEN_SETTIME, m_btnSetTime);
	DDX_Control(pDX, IDC_BTN_PTGEN_RESET, m_btnReSet);
	DDX_Control(pDX, IDC_BTN_PTGEN_GEN, m_btnCallGen);
	DDX_Control(pDX, IDC_BTN_PTGEN_ONE, m_btnOne);
	DDX_Control(pDX, IDC_BTN_PTGEN_MORE, m_btnMore);
	DDX_Control(pDX, IDC_BTN_PTGEN_CPU, m_btnCPU);
	DDX_Text(pDX, IDC_EDIT_PTGEN_103ADDR, m_str103Addr);
	DDX_Text(pDX, IDC_EDIT_PTGEN_ADDR, m_strAddr);
	DDX_Text(pDX, IDC_EDIT_PTGEN_COM, m_strCOM);
	DDX_Text(pDX, IDC_EDIT_PTGEN_COMCHECK, m_strCOMCheck);
	DDX_Text(pDX, IDC_EDIT_PTGEN_COMM, m_strComm);
	DDX_Text(pDX, IDC_EDIT_PTGEN_COMPARAM, m_strCOMParam);
	DDX_Text(pDX, IDC_EDIT_PTGEN_COMPRO, m_strCOMPro);
	DDX_Text(pDX, IDC_EDIT_PTGEN_ONE, m_strOne);
	DDX_Text(pDX, IDC_EDIT_PTGEN_RUN, m_strRun);
	DDX_Text(pDX, IDC_EDIT_PTGEN_TIME, m_strTime);
	DDX_Text(pDX, IDC_EDIT_PTGEN_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_PTGEN_VER, m_strVer);
	DDX_Text(pDX, IDC_EDIT_PTGEN_INSIDETIME, m_strInsideTime);
	DDX_Text(pDX, IDC_EDIT_PTGEN_IPA, m_strIPA);
	DDX_Text(pDX, IDC_EDIT_PTGEN_IPB, m_strIPB);
	DDX_Text(pDX, IDC_EDIT_PTGEN_PORTA, m_strPortA);
	DDX_Text(pDX, IDC_EDIT_PTGEN_PORTB, m_strPortB);
	DDX_Text(pDX, IDC_EDIT_PTGEN_SUSPEND, m_strSuspend);
	DDX_Control(pDX, IDC_BTN_PTGEN_SUSPEND, m_btnSuspend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTGeneral, CViewBase)
	//{{AFX_MSG_MAP(CPTGeneral)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_PTGEN_MORE, OnBtnPtgenMore)
	ON_BN_CLICKED(IDC_BTN_PTGEN_GEN, OnBtnPtgenGen)
	ON_BN_CLICKED(IDC_BTN_PTGEN_RESET, OnBtnPtgenReset)
	ON_BN_CLICKED(IDC_BTN_PTGEN_SETTIME, OnBtnPtgenSettime)
	ON_BN_CLICKED(IDC_BTN_PTGEN_TIME, OnBtnPtgenTime)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BTN_PTGEN_COMMHIS, OnBtnPtgenCommhis)
	ON_BN_CLICKED(IDC_BTN_PTGEN_RUNHIS, OnBtnPtgenRunhis)
	ON_BN_CLICKED(IDC_BTN_PTGEN_CALL, OnBtnPtgenCall)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTGEN_CALL, OnDoubleclickedBtnPtgenCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTGEN_GEN, OnDoubleclickedBtnPtgenGen)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTGEN_TIME, OnDoubleclickedBtnPtgenTime)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTGEN_SETTIME, OnDoubleclickedBtnPtgenSettime)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTGEN_RESET, OnDoubleclickedBtnPtgenReset)
	ON_BN_CLICKED(IDC_BTN_PTGEN_SUSPEND, OnBtnPtgenSuspend)
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_STTP_20111, OnSTTP20111)
	ON_MESSAGE(WM_STTP_20089, OnSTTP20089)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20058, OnSTTP20058)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(WM_WINDOW_REFRESH, OnWindowRefresh)
	ON_MESSAGE(WM_STTP_20004, OnSTTP20004)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTGeneral diagnostics

#ifdef _DEBUG
//##ModelId=49B87B910213
void CPTGeneral::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B910222
void CPTGeneral::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTGeneral message handlers

//##ModelId=49B87B9101C7
void CPTGeneral::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	RecordRate(IDC_PTGEN_SP, 1, left_client, top_client);
	RecordRate(IDC_PTGEN_SP2, 1, left_client, top_client);
	RecordRate(IDC_PTGEN_SP3, 1, left_client, top_client);
	
	//���
	RecordRate(IDC_STATIC_PTGEN_ONE, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_ONE, 1, left_client, top_client);

    RecordRate(IDC_COMM_LIGHT, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTGEN_COMM, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_COMM, 1, left_client, top_client);

	RecordRate(IDC_BTN_PTGEN_CALL, 0, IDC_EDIT_PTGEN_COMM, top_client);
	RecordRate(IDC_BTN_PTGEN_COMMHIS, 0, IDC_BTN_PTGEN_CALL, top_client);
	RecordRate(IDC_STATIC_TIME_PTGEN, 0, IDC_BTN_PTGEN_COMMHIS, top_client);

	RecordRate(IDC_STATIC_PTGEN_VER, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_VER, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_ADDR, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_ADDR, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_COM, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_COM, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_COMPARAM, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_COMPARAM, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_IPA, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_IPA, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_IPB, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_IPB, 1, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_SUSPEND, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_SUSPEND, 1, left_client, top_client);
	RecordRate(IDC_BTN_PTGEN_SUSPEND, 0, IDC_EDIT_PTGEN_SUSPEND, top_client);

	//�ұ�
	RecordRate(IDC_STATIC_PTGEN_TYPE, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_TYPE, 1, mid_client, top_client);

	RecordRate(IDC_STAT_LIGHT, 0, mid_client, top_client);
	RecordRate(IDC_STATIC_PTGEN_RUN, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_RUN, 1, mid_client, top_client);

	RecordRate(IDC_BTN_PTGEN_RUNHIS, 0, IDC_EDIT_PTGEN_RUN, top_client);

	RecordRate(IDC_STATIC_PTGEN_TIME, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_TIME, 1, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_103ADDR, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_103ADDR, 1, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_COMPRO, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_COMPRO, 1, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_COMCHECK, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_COMCHECK, 1, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_PORTA, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_PORTA, 1, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_PORTB, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_PTGEN_PORTB, 1, mid_client, top_client);

	//��ť
	RecordRate(IDC_BTN_PTGEN_MORE, 0, null_effect, null_effect, right_client, bottom_client);
	RecordRate(IDC_BTN_PTGEN_CPU, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTGEN_ONE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTGEN_GEN, 0, left_client, top_client);

	//װ����ʱ��
	RecordRate(IDC_EDIT_PTGEN_INSIDETIME, 1, mid_client, top_client);
	RecordRate(IDC_STATIC_PTGEN_DEVTIME, 0, mid_client, top_client);

	RecordRate(IDC_BTN_PTGEN_TIME, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTGEN_SETTIME, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTGEN_RESET, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_STATIC_PTGEN_SP1, 0, left_client, top_client, null_effect, null_effect);

	RecordRate(IDC_GIF_PTGENERAL, 0, IDC_STATIC_TIME_PTGEN, top_client);

	RecordRate(IDC_STATIC_PTGEN_CPU_VER, 0, left_client, top_client);
	RecordRate(IDC_COMBO_PTGEN_CPU_VER, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTGEN_CPU_DES, 0, mid_client, top_client);
	RecordRate(IDC_COMBO_PTGEN_CPU_DES, 0, mid_client, top_client);

	RecordRate(IDC_STATIC_PTGENERAL_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	if(g_PTGeneralMore == 0)
	{
		//����ʾ��������Ϣ����ť
		m_btnMore.ShowWindow(SW_HIDE);
	}
	//��ʼ���ؼ�
	InitControls();
	//����ĳЩ��Ϣ
	HideSomeInfo(TRUE);

	//���ض���
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}

	//��ʼ��ֵ
	RefreshControls();
}

/*************************************************************
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ�������ؼ�, ���ó�ʼֵ, ��ʾ���
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B910149
int CPTGeneral::InitControls()
{
	/*
	//�鿴���ఴť
	m_btnMore.SetThemeHelper(&m_ThemeHelper);
//	m_btnMore.SetIcon(IDI_SEARCH);
	m_btnMore.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnMore.SetTooltipText(_T("�鿴���������Ϣ"));

	//�鿴CPU��ť
	m_btnCPU.SetThemeHelper(&m_ThemeHelper);
//	m_btnCPU.SetIcon(IDI_SEARCH);
	m_btnCPU.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCPU.SetTooltipText(_T("�鿴װ��CPU�б�"));

	//�鿴һ���豸��ť
	m_btnOne.SetThemeHelper(&m_ThemeHelper);
//	m_btnOne.SetIcon(IDI_SEARCH);
	m_btnOne.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOne.SetTooltipText(_T("�鿴����һ���豸����"));

	//�ٻ��������԰�ť
	m_btnCallGen.SetThemeHelper(&m_ThemeHelper);
//	m_btnCallGen.SetIcon(IDI_SEARCH);
	m_btnCallGen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallGen.SetTooltipText(_T("�ٻ��豸��������"));

	//�ٻ�ʱ�䰴ť
	m_btnCallTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnCallTime.SetIcon(IDI_SEARCH);
	m_btnCallTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallTime.SetTooltipText(_T("�ٻ��豸ʱ��"));

	//ǿ�ƶ�ʱ��ť
	m_btnSetTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnSetTime.SetIcon(IDI_CONTROL);
	m_btnSetTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSetTime.SetTooltipText(_T("���豸���ж�ʱ"));

	//��Ϣ���鰴ť
	m_btnReSet.SetThemeHelper(&m_ThemeHelper);
//	m_btnReSet.SetIcon(IDI_CONTROL);
	m_btnReSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnReSet.SetTooltipText(_T("��Ϣ����"));

	//��ʷͨѶ״̬��ť
	m_btnCommHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommHis.SetTooltipText(_T("�鿴��ʷ"));

	//��ʷ����״̬��ť
	m_btnRunHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnRunHis.SetIcon(IDI_HISTORY);
	m_btnRunHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRunHis.SetTooltipText(_T("�鿴��ʷ"));

	//�ٻ�ͨѶ״̬
	m_btnCommCall.SetThemeHelper(&m_ThemeHelper);
	//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommCall.SetTooltipText(_T("�ٻ�"));
*/

/*
	LONG style = GetWindowLong(m_iconCommLight.GetSafeHwnd(), GWL_STYLE);
	style |= SS_ICON;   
	style |= SS_CENTERIMAGE;    
	SetWindowLong(m_iconCommLight.GetSafeHwnd(), GWL_STYLE, style); 
    SetWindowLong(m_iconStatLight.GetSafeHwnd(), GWL_STYLE, style); 
	*/

	return 0;
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B910157
void CPTGeneral::RefreshControls()
{
	if(m_pObj == NULL)
		return;

	if(m_bLoadData)
	{
		//ͨѶ״̬
		m_strComm = GetCommStatusString(m_pObj->m_nCommStatus);
		m_strRun = GetRunStatusString(m_pObj->m_nRunStatu);
		//����״̬
		m_strSuspend = m_pObj->m_nSuspendstatus == 0?StringFromID(IDS_SUSPEND_NO):StringFromID(IDS_SUSPEND_YES);
		UpdateData(FALSE);
		return;
	}

	m_bLoadData = TRUE;

	CString str;
	str.Format("%d", m_pObj->m_n103Addr);
	m_str103Addr = str; //103��ַ
	m_strAddr = m_pObj->m_sAddr; //װ�õ�ַ
	if(m_pObj->m_pOwner != NULL)
		m_strOne = m_pObj->m_pOwner->m_sName; //һ���豸����
	m_strTime = m_pObj->m_sDevTime; //��������ʱ��
	m_strVer = m_pObj->m_sSysVer; //�豸�汾��
	m_strCOMCheck = m_pObj->m_sCrc; //ͨ��У����
	m_strType = m_pObj->m_sModel; //�豸����

	m_strIPA = m_pObj->m_sIPA;
	m_strIPB = m_pObj->m_sIPB;
	m_strPortA.Format("%d", m_pObj->m_nPortA);
	m_strPortB.Format("%d", m_pObj->m_nPortB);

	//ͨѶ״̬
	m_strComm = GetCommStatusString(m_pObj->m_nCommStatus);

	/*
	if (m_pObj->m_nCommStatus == 0)
	{
		m_iconCommLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconCommLight.SetIcon(m_hIcon_close);
	}
	*/

	//����״̬
	m_strRun = GetRunStatusString(m_pObj->m_nRunStatu);

	/*
	if (m_pObj->m_nRunStatu == 2)
	{
		m_iconStatLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconStatLight.SetIcon(m_hIcon_close);
	}
	*/

	//�豸����
//	GetPTType(m_pObj->m_nPTTypeID);
	//ͨ����Ϣ
	//GetCOMInfo(m_pObj->m_nComID, m_pObj->m_sStationID);

	//����״̬
	m_strSuspend = m_pObj->m_nSuspendstatus == 0?StringFromID(IDS_SUSPEND_NO):StringFromID(IDS_SUSPEND_YES);


	CComboBox* pCpuVerComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PTGEN_CPU_VER);
	CComboBox* pCpuDesComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PTGEN_CPU_DES);

	pCpuVerComboBox->ResetContent();
	pCpuDesComboBox->ResetContent();
	for (int i = 0; i < m_pObj->m_arrCPU.GetSize(); i ++)
	{
		CSecCPU * cpu = m_pObj->m_arrCPU[i];
		MYASSERT_CONTINUE(cpu);
		str.Format("CPU%d %s", cpu->code, cpu->ver);
		int nIndex = pCpuVerComboBox->AddString(str);
		if(nIndex >= 0)
			pCpuVerComboBox->SetItemData(nIndex, (DWORD)cpu);
		str.Format("CPU%d %s", cpu->code, cpu->des);
		pCpuDesComboBox->AddString(str);
	}
	
	int nLast = pCpuDesComboBox->GetCount() - 1;
	if (pCpuDesComboBox->GetCount() > 0)
	{
		pCpuDesComboBox->SetCurSel(nLast);
	}

	nLast = pCpuVerComboBox->GetCount() - 1;
	if (pCpuVerComboBox->GetCount() > 0)
	{
		pCpuVerComboBox->SetCurSel(nLast);
	}

	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ�򿪱�������ҳ��Ϣ, ��������
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B910292
void CPTGeneral::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = pObj;

	int nIndex = (int)wParam;


	m_bLoadData = FALSE;
	StartThread();
	RefreshControls();
	//��һ�δ�ʱ��һҳ�Ŀؼ�ˢ��������, ԭ��δ��, ��ǿ��ˢ��һ��
	RegulateControlSize();
// 	}
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//����ٻ���ʱ
	SetDlgItemText(IDC_STATIC_TIME_PTGEN, "");
}

//##ModelId=49B87B910232
void CPTGeneral::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
}

/*************************************************************
 �� �� ����HideSomeInfo()
 ���ܸ�Ҫ������ĳЩ��Ϣ
 �� �� ֵ: void
 ��    ����param1	�Ƿ�����
**************************************************************/
//##ModelId=49B87B910167
void CPTGeneral::HideSomeInfo( BOOL bHide )
{
	UINT nCode;
	if(bHide)
	{
		nCode= SW_HIDE;
	}
	else
	{
		nCode = SW_SHOW;
	}
	CWnd * pWnd = NULL;

	//����汾��
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//װ�õ�ַ
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//ͨ�ſ�
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//ͨ�Ų���
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COMPARAM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COMPARAM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//��������ʱ��
// 	pWnd= GetDlgItem(IDC_STATIC_PTGEN_TIME);
// 	if(pWnd != NULL)
// 		pWnd->ShowWindow(nCode);
// 	pWnd = GetDlgItem(IDC_EDIT_PTGEN_TIME);
// 	if(pWnd != NULL)
// 		pWnd->ShowWindow(nCode);

	//103��ַ
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_103ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_103ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//ͨ��Э��
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COMPRO);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COMPRO);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//ͨ��У����
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COMCHECK);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COMCHECK);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_IPA);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_IPA);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_IPB);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_IPB);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_PORTA);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_PORTA);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	
	pWnd = GetDlgItem(IDC_STATIC_PTGEN_PORTB);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_PORTB);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//��ť
	m_btnCallTime.ShowWindow(nCode);
	m_btnCallGen.ShowWindow(nCode);

	//inside time
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_INSIDETIME);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_STATIC_PTGEN_DEVTIME);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_PTGEN_SP2);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_PTGEN_SP3);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	
	/*
	//gif
	m_gif.ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_STATIC_TIME_PTGEN);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
		*/

	if(g_PTControl == 1 && nCode == SW_SHOW)
		nCode = SW_SHOW;
	else
		nCode = SW_HIDE;

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_SP1);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	
	m_btnReSet.ShowWindow(nCode);
	m_btnSetTime.ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_CPU_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_COMBO_PTGEN_CPU_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	pWnd = GetDlgItem(IDC_STATIC_PTGEN_CPU_DES);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	
	pWnd = GetDlgItem(IDC_COMBO_PTGEN_CPU_DES);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
}

//##ModelId=49B87B910251
void CPTGeneral::OnBtnPtgenMore() 
{
	// TODO: Add your control notification handler code here
	if(m_bMoreInfo)
	{
		//Ŀǰ������ʾ������Ϣ,������Щ��Ϣ
		HideSomeInfo(TRUE);
		m_bMoreInfo = FALSE;
		SetDlgItemText(IDC_BTN_PTGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	else
	{
		//Ŀǰû����ʾ������Ϣ, ��ʾ��Щ��Ϣ
		HideSomeInfo(FALSE);
		m_bMoreInfo = TRUE;
		SetDlgItemText(IDC_BTN_PTGEN_MORE, StringFromID(IDS_BTN_HIDEINFO));
	}
}

/*************************************************************
 �� �� ����GetPTType()
 ���ܸ�Ҫ�������ݿ��(tb_pt_type)�в��ұ�������,������m_strType��
 �� �� ֵ: void
 ��    ����param1	�豸����ID
**************************************************************/
//##ModelId=49B87B910177
BOOL CPTGeneral::GetPTType( int nID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//�ֶ�
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);
		
	//����
	//ֻ��ѯδȷ���¼�
	Condition condition1;
	str.Format("TYPE_ID = %d", nID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_TYPE_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTGeneral::GetPTType, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//Ӧ��ֻ��һ����¼
			m_strType = pMemset->GetValue((UINT)0);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetPTType,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ����GetCOMInfo()
 ���ܸ�Ҫ�������ݿ��(tb_port_config)�в���ͨ�ſ�����,������,ͨ��Э��ID
 �� �� ֵ: ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ͨ�ſ�ID
		   Param2	��վID
**************************************************************/
//##ModelId=49B87B910196
BOOL CPTGeneral::GetCOMInfo( int nCOM_ID, CString sStationID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//�ֶ�
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);

	//������
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "baudrate", EX_STTP_DATA_TYPE_INT);

	//ͨ��Э��ID
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "protocol_id", EX_STTP_DATA_TYPE_INT);
	
	//����
	Condition condition1;
	str.Format("PORT_ID = %d", nCOM_ID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	Condition condition2;
	str.Format("STATION_ID = '%s'", sStationID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PORT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTGeneral::GetCOMInfo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//Ӧ��ֻ��һ����¼
			EnterCriticalSection(&m_CriticalOper);  
			m_strCOM = pMemset->GetValue((UINT)0); //ͨ�ſ�����
			m_strCOMParam = pMemset->GetValue(1); //ͨ�Ų���
			LeaveCriticalSection(&m_CriticalOper);
			//��ԼID,���ҹ�Լ
			CString str = pMemset->GetValue(2);
			GetProtocolInfo(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetCOMInfo,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ����GetProtocolInfo()
 ���ܸ�Ҫ�������ݿ��(tb_front_protocol_config)�в��ҹ�Լ����
 �� �� ֵ: ���ҳɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ����param1	��ԼID
**************************************************************/
//##ModelId=49B87B9101A5
BOOL CPTGeneral::GetProtocolInfo( CString sID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//�ֶ�
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);
	
	//����,protocol_id
	Condition condition1;
	str.Format("PROTOCOL_ID = %s", sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FRONT_PROTOCOL_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTGeneral::GetProtocolInfo, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//Ӧ��ֻ��һ����¼
			EnterCriticalSection(&m_CriticalOper);
			m_strCOMPro = pMemset->GetValue((UINT)0);
			LeaveCriticalSection(&m_CriticalOper);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetProtocolInfo,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9101B5
void CPTGeneral::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{
		//�ٻ���������
		SetDlgItemText(IDC_BTN_PTGEN_GEN, StringFromID(IDS_BTN_CALLPROPERTY));
		//�ٻ�ʱ��
		SetDlgItemText(IDC_BTN_PTGEN_TIME, StringFromID(IDS_BTN_CALLTIME));
		//ǿ�ƶ�ʱ
		SetDlgItemText(IDC_BTN_PTGEN_SETTIME, StringFromID(IDS_BTN_FORCE_SYNCTIME));
		//�źŸ���
		SetDlgItemText(IDC_BTN_PTGEN_RESET, StringFromID(IDS_BTN_SIGNALRETURN));
		//�ٻ�ͨѶ״̬
		SetDlgItemText(IDC_BTN_PTGEN_CALL, StringFromID(IDS_CALLOPER_CALL));
		//����״̬,������
		GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_MORE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_CALL) ->EnableWindow(TRUE);
		//ֹͣ����
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 6)
	{
		//�ٻ������״̬,�����Լ������Ķ�������
		
		GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_MORE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_CALL) ->EnableWindow(FALSE);
		//��ʼ����
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTGEN, str);
		
		//���Լ�����
		switch(m_nCurrentStatus)
		{
		case 1: //�ٻ���������
			SetDlgItemText(IDC_BTN_PTGEN_GEN, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(TRUE);
			break;
		case 2: //�ٻ�װ��ʱ��
			SetDlgItemText(IDC_BTN_PTGEN_TIME, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(TRUE);
			break;
		case 3: //ǿ�ƶ�ʱ
			SetDlgItemText(IDC_BTN_PTGEN_SETTIME, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(TRUE);
			break;
		case 4: //��Ϣ����
			SetDlgItemText(IDC_BTN_PTGEN_RESET, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(TRUE);
			break;
		case 5: //�ٻ�����״̬
			SetDlgItemText(IDC_BTN_PTGEN_CALL, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTGEN_CALL) ->EnableWindow(TRUE);
		default:
			break;
		}
	}
}


void CPTGeneral::OnBtnPtgenGen() 
{
	// TODO: Add your control notification handler code here
	//�����ٻ��������԰�ť, �ٻ��������Ի�ȡ���ٻ�
	MYASSERT(m_pObj);
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
		//�����ڿ���״̬,��ʼ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20110����(Ŀǰҳ�治֧�ְ�CPU�ٻ�������Ϣ)
		if(!pApp->m_SttpEngine.BuildDataFor20110PTGeneral(sttpData, m_pObj->m_sID, 0,1))
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
				str.Format("����[%s]�����ٻ�������Ϣ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ�������Ϣ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı䰴ť����,�޸ĵ�ǰ״̬
			m_nCurrentStatus = 1;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nDevGeneralTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//���Ŀؼ����ö�
			UpdateControlsEnable();
			CString str;
			str.Format("����[%s]�����ٻ�������Ϣ���ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}


void CPTGeneral::OnBtnPtgenReset() 
{
	// TODO: Add your control notification handler code here
	//�����źŸ��鰴ť, �źŸ����ȡ���ٻ�
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
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
		//�����ڿ���״̬,��ʼ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		int nCpu = 0;
		CComboBox* pCpuVerComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PTGEN_CPU_VER);
		if(pCpuVerComboBox != NULL)
		{
			int nCurSel = pCpuVerComboBox->GetCurSel();
			if(nCurSel >= 0)
			{
				CSecCPU* cpu = (CSecCPU*)pCpuVerComboBox->GetItemData(nCurSel);
				nCpu = cpu->code;
			}
		}
		/*if (m_pObj->m_arrCPU.GetSize() > 0)
		{
			nCpu= m_pObj->m_arrCPU[0]->code;
		}
		else
		{
			nCpu = 0;
		}*/

		//��֯20057����(�ݲ�֧��CPU)
		if(!pApp->m_SttpEngine.BuildDataFor20057ResetPT(sttpData, m_pObj->m_sID, nCpu))
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
				str.Format("����[%s]�����źŸ��鱨��ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����źŸ��鱨��ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 4;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nSignalResetTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����źŸ��鱨�ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}

//##ModelId=49B87B910272
void CPTGeneral::OnBtnPtgenSettime() 
{
	// TODO: Add your control notification handler code here
	//����ǿ�ƶ�ʱ��ť, ǿ�ƶ�ʱ��ȡ���ٻ�
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
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
		//�����ڿ���״̬,��ʼ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20059����(�ݲ�֧��CPU)
		int nCpu;
		if (m_pObj->m_arrCPU.GetSize() > 0)
		{
			nCpu= m_pObj->m_arrCPU[0]->code;
		}
		else
		{
			nCpu = 0;
		}

		if(!pApp->m_SttpEngine.BuildDataFor20059PTSetTime(sttpData, m_pObj->m_sID, nCpu))
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
				str.Format("����[%s]����ǿ�ƶ�ʱ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]����ǿ�ƶ�ʱ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 3;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nVerifyTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]����ǿ�ƶ�ʱ���ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}

//##ModelId=49B87B910274
void CPTGeneral::OnBtnPtgenTime() 
{
	// TODO: Add your control notification handler code here
	//�����ٻ�ʱ�䰴ť, �ٻ�ʱ���ȡ���ٻ�
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
		//�����ڿ���״̬,��ʼ�ٻ�
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20088����
		if(!pApp->m_SttpEngine.BuildDataFor20088PTTime(sttpData, m_pObj->m_sID))
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
				str.Format("����[%s]�����ٻ�װ��ʱ�䱨��ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("����[%s]�����ٻ�װ��ʱ�䱨��ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 2;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nDevTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("����[%s]�����ٻ�װ��ʱ�䱨�ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 �� �� ����OnSTTP20111()
 ���ܸ�Ҫ����Ӧ20111(����������Ϣ����֪ͨ)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B91029F
void CPTGeneral::OnSTTP20111( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д����ٻ��������Ե�״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20111����
	if(pSttpData->sttp_head.uMsgID != 20111)
	{
		//������
		CString str;
		str.Format("CPTGeneral::OnSTTP20111 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTGeneral::OnSTTP20111 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	//װ�ð汾��
	m_strVer = pSttpData->sttp_body.ch_version;
	m_pObj->m_sSysVer = m_strVer;
	//�豸ʱ��
	m_strTime = pSttpData->sttp_body.ch_time_12_BIT1;
	CTime tmTime;
	tmTime = StringToTimeSttp12(m_strTime);
	m_strTime = tmTime.Format("%Y-%m-%d");
	m_pObj->m_sDevTime = m_strTime;
	//ͨѶУ����
	m_strCOMCheck = pSttpData->sttp_body.ch_check_code;
	m_pObj->m_sCrc = m_strCOMCheck;

	//ˢ�½���
	UpdateData(FALSE);
	
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ�������Ϣ�ɹ�", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_DEVINFO);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_SUCCESS);
}

/*************************************************************
 �� �� ����OnSTTP20089()
 ���ܸ�Ҫ����Ӧ�ٻ�װ��ʱ��֪ͨ(20089)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9102B1
void CPTGeneral::OnSTTP20089( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//û�д����ٻ�ʱ���״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20089����
	if(pSttpData->sttp_head.uMsgID != 20089)
	{
		//������
		CString str;
		str.Format("CPTGeneral::OnSTTP20089 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTGeneral::OnSTTP20089 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	m_strInsideTime = pSttpData->sttp_body.ch_time_15_BIT1;
	int nms = 0;
	CTime tmTime = StringToTimeSttp15(m_strInsideTime, nms);
	m_strInsideTime.Format("%s.%03d", tmTime.Format("%Y-%m-%d %H:%M:%S"), nms);
	
	//ˢ�½���
	UpdateData(FALSE);
	
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�ٻ�װ��ʱ��ɹ�", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_DEVTIME);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_SUCCESS);
	
}

/*************************************************************
 �� �� ����OnSTTP20060()
 ���ܸ�Ҫ����Ӧ20060(������ʱ֪ͨ)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9102C0
void CPTGeneral::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 3)
	{
		//û�д��ڱ�����ʱ��״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20060����
	if(pSttpData->sttp_head.uMsgID != 20060)
	{
		//������
		CString str;
		str.Format("CPTGeneral::OnSTTP20060 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTGeneral::OnSTTP20060 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CALL_PROTECT_SETTIME);
	
	pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, pSttpData->sttp_body.nStatus);
	
	//������
	int nReturn = pSttpData->sttp_body.nStatus;
	if(nReturn == 0)
	{
		CString str;
		str.Format("����[%s]ǿ�ƶ�ʱ�ɹ�", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//�ɹ�
		AfxMessageBox( StringFromID(IDS_SYNCTIME_SUCCESS));
	}
	else
	{
		CString str;
		str.Format("����[%s]ǿ�ƶ�ʱʧ��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//ʧ�� 
		AfxMessageBox( StringFromID(IDS_SYNCTIME_FAIL));
	}
}

/*************************************************************
 �� �� ����OnSTTP20058()
 ���ܸ�Ҫ����Ӧ20058(�����źŸ���֪ͨ)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9102CE
void CPTGeneral::OnSTTP20058( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		//û�д����źŸ����״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20058����
	if(pSttpData->sttp_head.uMsgID != 20058)
	{
		//������
		CString str;
		str.Format("CPTGeneral::OnSTTP20058 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CPTGeneral::OnSTTP20058 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_SIGNRESET, pSttpData->sttp_body.nCpu);
	
	pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, pSttpData->sttp_body.nStatus);

	//�رն�ʱ��
	KillTimer(m_nTimer);

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();
	
	//������
	int nReturn = pSttpData->sttp_body.nStatus;
	if(nReturn == 0)
	{
		CString str;
		str.Format("����[%s]�źŸ���ɹ�", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//�ɹ�
		AfxMessageBox( StringFromID(IDS_SIGNALRETURN_SUCCESS));
	}
	else
	{
		CString str;
		str.Format("����[%s]�źŸ���ʧ��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//ʧ�� 
		AfxMessageBox( StringFromID(IDS_SIGNALRETURN_FAIL));
	}
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ����Ӧ20069(����ʧ�ܱ���)
 �� �� ֵ: void 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9102D1
void CPTGeneral::OnSTTP20069( WPARAM wparam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
	{
		//û�д��ڲ���״̬, ��������
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
		//���Ǳ�װ�õı���
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
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
	}
	else
	{
		//����ʧ��
		//�������,�ı�״̬�Ͱ�ť����
		if(m_nCurrentStatus == 1)
		{
			//�����ٻ���������
				
			CString str;
			str.Format("����[%s]�ٻ���������ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVINFO);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_FAILD);

			AfxMessageBox( StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//�����ٻ�ʱ��
				
			CString str;
			str.Format("����[%s]�ٻ�װ��ʱ��ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVTIME);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 3)
		{
			//����ǿ�ƶ�ʱ
				
			CString str;
			str.Format("����[%s]ǿ�ƶ�ʱʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_PROTECT_SETTIME);
			
			pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
		}
		else if(m_nCurrentStatus == 4)
		{
			//�����źŸ���
				
			CString str;
			str.Format("����[%s]�źŸ���ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_SIGNRESET);
			
			pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_SIGNALRETURN_FAIL));
		}
		else if (m_nCurrentStatus == 5)
		{
			//�����źŸ���
			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
			
			CString str;
			str.Format("����[%s]�ٻ�ͨ��״̬ʧ��", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->AddNewManOperator("�ٻ�װ��ͨ��״̬", m_pObj->m_sID, "�ٻ�װ��ͨ��״̬ʧ��", -1, OPER_FAILD);
			
			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
		}
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ����Ӧ20125(װ�ò�֧�ֱ���)
 �� �� ֵ: 
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B9102E0
void CPTGeneral::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTGeneral::OnSTTP20125", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
	{
		//û�д����ٻ���������״̬,��������
		WriteLog("CPTGeneral::OnSTTP20125 û�д����ٻ���������״̬,��������", XJ_LOG_LV3);
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		WriteLog("CPTGeneral::OnSTTP20125 û�д����ٻ���������״̬,��������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ�20125����
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//������
		WriteLog("CPTGeneral::OnSTTP20125 ������", XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		WriteLog("CPTGeneral::OnSTTP20125 ���Ǳ�װ�õı���", XJ_LOG_LV3);
		return;
	}

	//����Ƿ񱾹��ܵı���
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//���Ǳ����ܵı���
		WriteLog("CPTGeneral::OnSTTP20125 ���Ǳ����ܵı���", XJ_LOG_LV3);
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nTimer);
	//������
	//�������,�ı�״̬�Ͱ�ť����
	if(m_nCurrentStatus == 1)
	{
		//�����ٻ���������
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CString str;
		str.Format("����[%s]�ٻ�������Ϣʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVINFO);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�ʱ��
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
		CString str;
		str.Format("����[%s]�ٻ�װ��ʱ��ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVTIME);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 3)
	{
		//����ǿ�ƶ�ʱ
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CString str;
		str.Format("����[%s]ǿ�ƶ�ʱʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_PROTECT_SETTIME);
		
		pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, OPER_FAILD);
	

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����źŸ���
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CString str;
		str.Format("����[%s]�źŸ���ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_SIGNRESET);
		
		pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if (m_nCurrentStatus == 5)
	{
		//�����źŸ���
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();

		CString str;
		str.Format("����[%s]�ٻ�ͨ��״̬ʧ��,ԭ��Ϊ:װ�ò�֧��", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->AddNewManOperator("�ٻ�װ��ͨ��״̬", m_pObj->m_sID, "�ٻ�װ��ͨ��״̬ʧ��", -1, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
}

//##ModelId=49B87B910280
void CPTGeneral::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//��ʾ
		CString str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		switch(m_nCurrentStatus)
		{
		case 1:
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVINFO);
			pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_FAILD);
			break;
		case 2:
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVTIME);
			pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_FAILD);
			break;
		case 3:
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_PROTECT_SETTIME);
			pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, OPER_FAILD);
			break;
		case 4:
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_SIGNRESET);
			pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, OPER_FAILD);
			break;
		case 5:
			pApp->AddNewManOperator("�ٻ�װ��ͨ��״̬", m_pObj->m_sID, "�ٻ�װ��ͨ��״̬ʧ��", -1, OPER_FAILD);
			break;
		default:
			str = "";
		}
		//�ı�״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		CString sLog;
		sLog.Format("[%s]%s", m_pObj->m_sName, str);
		WriteLog(sLog, XJ_LOG_LV1);
		AfxMessageBox(str);
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTGEN, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����OnPTFrameClose()
 ���ܸ�Ҫ����Ӧ�رմ�����Ϣ,�˳��߳� 
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102F1
void CPTGeneral::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TURE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B9101C5
BOOL CPTGeneral::ReFillAll(int nQueryNo)
{
	if(!IsCurrentQueryNo(nQueryNo))
		return FALSE;
	if(m_pObj == NULL)
		return FALSE;
	GetDlgItem(IDC_STATIC_PTGENERAL_LOADING)->ShowWindow(SW_SHOW);
	if(g_PTGeneralMore)
	{
		if(!GetCOMInfo(m_pObj->m_nComID, m_pObj->m_sStationID))
		{
			if(IsCurrentQueryNo(nQueryNo))
				GetDlgItem(IDC_STATIC_PTGENERAL_LOADING)->ShowWindow(SW_HIDE);
			return FALSE;
		}
	}
	if(IsCurrentQueryNo(nQueryNo))
		GetDlgItem(IDC_STATIC_PTGENERAL_LOADING)->ShowWindow(SW_HIDE);

	return TRUE;
}

/*************************************************************
 �� �� ����OnWindowRefresh()
 ���ܸ�Ҫ����Ӧ����ˢ����Ϣ, ˢ�´��ڿؼ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102FE
void CPTGeneral::OnWindowRefresh( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����OnSTTP20004()
 ���ܸ�Ҫ����Ӧ����ͨѶ״̬����֪ͨ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102EE
void CPTGeneral::OnSTTP20004( WPARAM wparam, LPARAM lParam )
{
	WriteLog("�յ�20004����", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 5)
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
	

	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.variant_member.value_data[0].str_value.c_str();
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		return;
	}

	
	//��ʱ��Ϊ
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: ��ʼ��׼ʱ������趨���
	//�رն�ʱ��
	KillTimer(m_nTimer);
	//������ʱ��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	int nPTStatus = pSttpData->sttp_body.variant_member.value_data[0].id;
	if(nPTStatus != m_pObj->m_nCommStatus)
	{
		m_pObj->SetCommStatus(nPTStatus);
		m_pObj->m_tmNewCommStatus = CTime::GetCurrentTime();
		m_pObj->RefreshCommDurativeTime();
	}
	m_strComm = GetCommStatusString(nPTStatus);

	//ˢ�½���
	UpdateData(FALSE);
	
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();

	CString str;
	str.Format("����[%s]�����ٻ�װ��ͨ��״̬�ɹ�", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	pApp->AddNewManOperator("�ٻ�װ��ͨ��״̬", m_pObj->m_sID, "�ٻ�װ��ͨ��״̬�ɹ�", -1);
}


//##ModelId=49B87B9101F4
void CPTGeneral::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_GENERAL));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_GENERAL));

	if(!m_bLoadData)
	{
		//δ����, ��������
	//	g_wndWait.Show(SPLASH_NOTIMER);
	//	RefreshControls();
	//	g_wndWait.Hide();
	}
	/*
	if (!m_bQuery && m_pObj != NULL)
	{
		m_strComm = GetCommStatusString(LoadPTStatus());
		UpdateData(FALSE);
		m_bQuery = true;
	}
	*/
	//��������
	RefreshControls();
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9101B6
void CPTGeneral::CancelCall()
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
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20110, m_pObj->m_sID, 0, 1);
	}
	else if(m_nCurrentStatus == 2)
	{
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20088, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//�����޸Ķ�ֵ
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20059, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����л���ֵ��
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20057, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 5)
	{
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20003, m_pObj->m_sID, 0);
	}
	//���ͱ���
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ı�״̬
	m_nCurrentStatus = 0;
	//���Ŀؼ�������
	UpdateControlsEnable();
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ���������߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9101C6
void CPTGeneral::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

/*************************************************************
 �� �� ����IsVaild20069()
 ���ܸ�Ҫ���ж��յ���20069�Ƿ��Լ���
 �� �� ֵ: ���Լ���ʧ�ܱ��ķ���TRUE, ���򷵻�FALSE
 ��    ����param1	ʧ�ܵĲ�������ID
		   Param2
**************************************************************/
//##ModelId=49B87B9101B7
BOOL CPTGeneral::IsVaild20069( int nMsgID )
{
	if(20110 == nMsgID && 1 == m_nCurrentStatus)
	{
		//�ٻ���������
		return TRUE;
	}
	if(20057 == nMsgID && 4 == m_nCurrentStatus)
	{
		//�źŸ���
		return TRUE;
	}
	if(20088 == nMsgID && 2 == m_nCurrentStatus)
	{
		//�ٻ�ʱ��
		return TRUE;
	}
	if(20059 == nMsgID && 3 == m_nCurrentStatus)
	{
		//ǿ�ƶ�ʱ
		return TRUE;
	}
	if (20003 == nMsgID && 5 == m_nCurrentStatus)
	{
		//�ٻ�ͨѶ״̬״̬
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B910283
void CPTGeneral::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CViewBase::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102EE
void CPTGeneral::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
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
		//�����ٻ���������
		
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�ʱ��
		
		int nTimeOut = pData->m_nDevTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//����ǿ�ƶ�ʱ
		
		int nTimeOut = pData->m_nVerifyTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//�����źŸ���
		
		int nTimeOut = pData->m_nSignalResetTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}	
}

void CPTGeneral::OnBtnPtgenCommhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 1;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CPTGeneral::OnBtnPtgenRunhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 2;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CPTGeneral::OnBtnPtgenCall() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp *)AfxGetApp();
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
		
		//���뱨����������
		STTP_FULL_DATA sttpData;
		
		//��֯20003����,����վ����
		if(!pApp->m_SttpEngine.BuildDataFor20003CommStatus(sttpData, m_pObj->m_sID))
		{
			//��֯����ʧ��
			//�����ٻ������Ϣ,���Ϊ����ʧ��
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
				str.Format("[%s]�����ٻ�ͨ��״̬����ʧ��, ԭ��Ϊ�����ж�", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("[%s]�����ٻ�ͨ��״̬����ʧ��,ԭ��Ϊ��������", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
		}
		else
		{
			//�ɹ�,�ı䰴ť����,�޸ĵ�ǰ״̬
			m_nCurrentStatus = 5;
			
			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nCommStatusTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();

			CString str;
			str.Format("[%s]�����ٻ�ͨ��״̬���ĳɹ�", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 5)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}

int CPTGeneral::LoadPTStatus()
{
	if(m_pObj == NULL)
		return -1;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//������ƽ̨����DLL�����ӿ���
	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
		
	//�ֶ�
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "curtime", EX_STTP_DATA_TYPE_TIME);
	
	CString str;
	//����
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	
	//��ʱ������
	Condition condition5;
	str = "order by curtime DESC";
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CPTGeneral::LoadPTStatus(), ��ѯʧ��", XJ_LOG_LV3);
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
			m_pObj->m_nCommStatus = atoi(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::LoadPTStatus(),��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return m_pObj->m_nCommStatus;
}


HBRUSH CPTGeneral::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	/*
	if(nCtlColor == CTLCOLOR_EDIT)
	{
       // pDC->SetTextColor(RGB(63, 203, 70));//���屳��ɫ******
	   pDC->SetBkColor(RGB(98, 213, 103));//���屳��ɫ******
		HBRUSH B = CreateSolidBrush(RGB(98, 213, 103)); //����ɫ
		return B;
	}
	*/

	if(m_pObj == NULL)
		return hbr;

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PTGEN_COMM)
	{
        HBRUSH B;
		if (m_pObj->m_nCommStatus == 0)
		{
			pDC->SetBkColor(RGB(131, 222, 135));//���屳��ɫ******
			B = CreateSolidBrush(RGB(131, 222, 135)); //����ɫ
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//���屳��ɫ******
			B = CreateSolidBrush(RGB(255, 104, 104)); //����ɫ
		}
		
		return B;
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PTGEN_RUN)
	{
		HBRUSH B;
		if (m_pObj->m_nRunStatu == 2)
		{
			pDC->SetBkColor(RGB(131, 222, 135));//���屳��ɫ******
			B = CreateSolidBrush(RGB(131, 222, 135)); //����ɫ
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//���屳��ɫ******
			B = CreateSolidBrush(RGB(255, 104, 104)); //����ɫ
		}
		
		return B;
	}

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_PTGEN_SUSPEND)
	{
		HBRUSH B;
		if(m_pObj->m_nSuspendstatus == 0)
		{
			//δ����
			pDC->SetBkColor(RGB(131, 222, 135));//���屳��ɫ******
			B = CreateSolidBrush(RGB(131, 222, 135)); //����ɫ
		}
		else
		{
			pDC->SetBkColor(RGB(250, 120, 0));//���屳��ɫ******
			B = CreateSolidBrush(RGB(250, 120, 0)); //����ɫ
		}
		return B;
	}
	
	return hbr;
}


void CPTGeneral::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTGeneral::OnDoubleclickedBtnPtgenCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTGeneral::OnDoubleclickedBtnPtgenGen() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTGeneral::OnDoubleclickedBtnPtgenTime() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTGeneral::OnDoubleclickedBtnPtgenSettime() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTGeneral::OnDoubleclickedBtnPtgenReset() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTGeneral::OnBtnPtgenSuspend() 
{
	// TODO: Add your control notification handler code here
	if(m_pObj->m_nSuspendstatus == 0)
		AfxMessageBox( StringFromID(IDS_SUSPEND_NO));
	else
		AfxMessageBox(m_pObj->m_sSuspendreason);
}


/****************************************************
Date:2013/7/26  Author:LYH
������:   StartLoadThread	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CPTGeneral::StartThread()
{
	m_nQueryNo = GetNextQueryNo();
	//������ȡ�߳�
	if(1)
	{
		m_bExitThread = FALSE;
		CWinThread* pThread = AfxBeginThread(PTGeneralLoad,this,THREAD_PRIORITY_BELOW_NORMAL);
		if(pThread)
			AddThreadNode(m_nQueryNo, pThread->m_hThread);
	}
}

int CPTGeneral::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 100000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}

void CPTGeneral::RemoveThreadNode( int nNo )
{
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
	CString str;
	str.Format("����������Ϣ��ѯ�̳߳ɹ��˳�,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

void CPTGeneral::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("����������Ϣ��ѯ�̳߳ɹ�����,��ѯ��:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTGeneral::GetFirstNode()
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

