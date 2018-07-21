// WaveRecGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecGeneral.h"
#include "DlgStatusHis.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

UINT WRGeneralLoad(LPVOID pParam)
{
	CWaveRecGeneral* wrGeneral = (CWaveRecGeneral*)pParam;
	if(wrGeneral == NULL)
		return 0;
	//��ȡ����
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//�����˳��߳�,�˳�
		if(wrGeneral->m_bExitThread)
			return 0;
		//�������Դ���
		if(nCount < 0)
			return 0;
		//��ȡ���, �˳�
		if(bLoad)
		{
			//log
			WriteLog("¼��������--��������ҳ��, ��ȡ���ݽ���, �����˳��߳�!", XJ_LOG_LV3);
			return 0;
		}
		if(wrGeneral->ReFillAll())
		{
			wrGeneral->PostMessage(WM_WINDOW_REFRESH, 0, 0);
			bLoad = TRUE;
		}
		else
		{
			//��ȡʧ��
			CString str;
			str.Format("WRGeneral, ʧ��%d��", (4 - nCount));
			WriteLog(str, XJ_LOG_LV3);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral

IMPLEMENT_DYNCREATE(CWaveRecGeneral, CViewBase)

//##ModelId=49B87B800291
CWaveRecGeneral::CWaveRecGeneral()
	: CViewBase(CWaveRecGeneral::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecGeneral)
	m_strCommStatus = _T("");
	m_strRunStatus = _T("");
	m_strType = _T("");
	m_strVer = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_bMoreInfo = FALSE;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_bGetStatus = FALSE;
	m_hIcon_open= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_GREEN_LIGHT));
	m_hIcon_close= LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_RED_LIGHT));
}

//##ModelId=49B87B80034E
CWaveRecGeneral::~CWaveRecGeneral()
{
}

//##ModelId=49B87B80034B
void CWaveRecGeneral::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecGeneral)
	DDX_Control(pDX, IDC_STAT_LIGHT, m_iconStatLight);
	DDX_Control(pDX, IDC_COMM_LIGHT, m_iconCommLight);
	DDX_Control(pDX, IDC_BTN_WRGEN_CALL, m_btnCall);
	DDX_Control(pDX, IDC_BTN_WRGEN_COMMHIS, m_btnCommHis);
	DDX_Control(pDX, IDC_BTN_WRGEN_RUNHIS, m_btnRunHis);
	DDX_Control(pDX, IDC_GIF_WRGEN, m_gif);
	DDX_Control(pDX, IDC_BTN_WRGEN_TIME, m_btnTime);
	DDX_Control(pDX, IDC_BTN_WRGEN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTN_WRGEN_REL, m_btnRel);
	DDX_Control(pDX, IDC_BTN_WRGEN_MORE, m_btnMore);
	DDX_Text(pDX, IDC_EDIT_WRGEN_COMM, m_strCommStatus);
	DDX_Text(pDX, IDC_EDIT_WRGEN_RUN, m_strRunStatus);
	DDX_Text(pDX, IDC_EDIT_WRGEN_TYPE, m_strType);
	DDX_Text(pDX, IDC_EDIT_WRGEN_VER, m_strVer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecGeneral, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecGeneral)
	ON_BN_CLICKED(IDC_BTN_WRGEN_MORE, OnBtnWrgenMore)
	ON_BN_CLICKED(IDC_BTN_WRGEN_TIME, OnBtnWrgenTime)
	ON_BN_CLICKED(IDC_BTN_WRGEN_START, OnBtnWrgenStart)
	ON_CBN_SELCHANGE(IDC_CMB_WRGEN_CPU, OnSelchangeCmbWrgenCpu)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_WRGEN_COMMHIS, OnBtnWrgenCommhis)
	ON_BN_CLICKED(IDC_BTN_WRGEN_RUNHIS, OnBtnWrgenRunhis)
	ON_BN_CLICKED(IDC_BTN_WRGEN_CALL, OnBtnWrgenCall)
	ON_WM_CTLCOLOR()
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_CALL, OnDoubleclickedBtnWrgenCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_START, OnDoubleclickedBtnWrgenStart)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRGEN_TIME, OnDoubleclickedBtnWrgenTime)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20124, OnSTTP20124)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20086, OnSTTP20086)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
	ON_MESSAGE(WM_WINDOW_REFRESH, OnWindowRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral diagnostics

#ifdef _DEBUG
//##ModelId=49B87B80035B
void CWaveRecGeneral::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B80035D
void CWaveRecGeneral::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecGeneral message handlers

//##ModelId=49B87B800340
void CWaveRecGeneral::OnInitialUpdate() 
{
	//��¼�ؼ�λ��
	RecordRate(IDC_WRGEN_SP, 1, left_client, top_client);
	RecordRate(IDC_WRGEN_SP2, 1, left_client, top_client);
	
	//���
	RecordRate(IDC_COMM_LIGHT, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRGEN_COMM, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_COMM, 1, left_client, top_client);

	RecordRate(IDC_BTN_WRGEN_CALL, 0, IDC_EDIT_WRGEN_COMM, top_client);
	RecordRate(IDC_BTN_WRGEN_COMMHIS, 0, IDC_BTN_WRGEN_CALL, top_client);
	
	RecordRate(IDC_STATIC_WRGEN_VER, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_VER, 1, left_client, top_client);
	
	RecordRate(IDC_STATIC_WRGEN_TYPE, 0, left_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_TYPE, 1, left_client, top_client);
	
	
	//�ұ�
	RecordRate(IDC_STAT_LIGHT, 0, mid_client, top_client);
	RecordRate(IDC_STATIC_WRGEN_RUN, 0, mid_client, top_client);
	RecordRate(IDC_EDIT_WRGEN_RUN, 1, mid_client, top_client);

	RecordRate(IDC_BTN_WRGEN_RUNHIS, 0, IDC_EDIT_WRGEN_RUN, top_client);
		
	//��ť
	RecordRate(IDC_BTN_WRGEN_MORE, 0, null_effect, null_effect, right_client, bottom_client);
	RecordRate(IDC_BTN_WRGEN_REL, 0, left_client, top_client);

	//Զ��������
	RecordRate(IDC_CMB_WRGEN_CPU, 0, left_client,top_client);
	RecordRate(IDC_BTN_WRGEN_START, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRGEN_TIME, 0, left_client, top_client);
	RecordRate(IDC_GIF_WRGEN, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_WRGEN, 0, left_client, top_client);
	
	CViewBase::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();

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
	RefreshControls();
}

/*************************************************************
 �� �� ����InitControls()
 ���ܸ�Ҫ����ʼ�������ؼ�, ���ó�ʼֵ, ��ʾ���
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
//##ModelId=49B87B80030E
int CWaveRecGeneral::InitControls()
{
	/*
	//�鿴���ఴť
	m_btnMore.SetThemeHelper(&m_ThemeHelper);
//	m_btnMore.SetIcon(IDI_SEARCH);
	m_btnMore.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnMore.SetTooltipText(_T("�鿴���������Ϣ"));

	//Զ������
	m_btnStart.SetThemeHelper(&m_ThemeHelper);
//	m_btnStart.SetIcon(IDI_CONTROL);
	m_btnStart.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStart.SetTooltipText(_T("����Զ��װ������"));

	//�鿴�����豸��ť
	m_btnRel.SetThemeHelper(&m_ThemeHelper);
//	m_btnRel.SetIcon(IDI_SEARCH);
	m_btnRel.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRel.SetTooltipText(_T("�鿴�����豸����"));
	
	//ǿ�ƶ�ʱ
	m_btnTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnTime.SetIcon(IDI_SEARCH);
	m_btnTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnTime.SetTooltipText(_T("ǿ��װ�ö�ʱ"));

	//ͨѶ״̬��ʷ
	m_btnCommHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommHis.SetTooltipText(_T("�鿴��ʷ"));

	//����״̬��ʷ
	m_btnRunHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnRunHis.SetIcon(IDI_HISTORY);
	m_btnRunHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRunHis.SetTooltipText(_T("�鿴��ʷ"));

	//�ٻ�ͨѶ״̬
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
	//	m_btnCall.SetIcon(IDI_HISTORY);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("�ٻ�"));
	*/

	/*
	LONG style = GetWindowLong(m_iconCommLight.GetSafeHwnd(), GWL_STYLE);
	style |= SS_ICON;   
	style |= SS_CENTERIMAGE;    
	SetWindowLong(m_iconCommLight.GetSafeHwnd(),GWL_STYLE,style); 
    SetWindowLong(m_iconStatLight.GetSafeHwnd(),GWL_STYLE,style); 
*/

	return 0;
}

/*************************************************************
 �� �� ����OnWRFrameOpen()
 ���ܸ�Ҫ����Ӧ¼��������ҳ�����Ϣ, ˢ�½�����ʾ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B80038D
void CWaveRecGeneral::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//���¶�Ӧ�Ķ����豸
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = pObj;

	int nIndex = (int)wParam;

	//�˳����߳�
// 	EndOwnerThread();
// 
// 	//������ȡ�߳�
// 	if(m_pLoadThread == NULL)
// 	{
// 		m_bExitThread = FALSE;
// 		m_pLoadThread = AfxBeginThread(WRGeneralLoad, this, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
// 		if(m_pLoadThread != NULL)
// 		{
// 			m_pLoadThread->m_bAutoDelete = FALSE;
// 			m_pLoadThread->ResumeThread();
// 		}
// 	}

// 	else
// 	{
// 		//ˢ��
 		RefreshControls();
		//��һ�δ�ʱ��һҳ�Ŀؼ�ˢ��������, ԭ��δ��, ��ǿ��ˢ��һ��
		RegulateControlSize();
// 	}
	//ҳ�滹ԭ
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_WRGEN, "");
}

/*************************************************************
 �� �� ����RefreshControls()
 ���ܸ�Ҫ��ˢ�¿ؼ�ֵ
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B80031C
void CWaveRecGeneral::RefreshControls()
{
	//���������Ч��
	if(m_pObj == NULL)
		return;
	m_strType = m_pObj->m_sModel; //�ͺ�
	m_strVer = m_pObj->m_sSysVer; //�汾��
	
	//ͨѶ״̬
	m_strCommStatus = GetCommStatusString(m_pObj->m_nCommStatus);
	
	//����״̬
	m_strRunStatus = GetRunStatusString(m_pObj->m_nRunStatu);

/*
	if (m_pObj->m_nCommStatus == 0)
	{
		m_iconCommLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconCommLight.SetIcon(m_hIcon_close);
	}
	
	//����״̬
	if (m_pObj->m_nRunStatu == 2)
	{
		m_iconStatLight.SetIcon(m_hIcon_open);
	}
	else
	{
		m_iconStatLight.SetIcon(m_hIcon_close);
	}
	*/

	FillCPU();

	UpdateData(FALSE);
}

//##ModelId=49B87B80036C
void CWaveRecGeneral::OnBtnWrgenMore() 
{
	// TODO: Add your control notification handler code here
	if(m_bMoreInfo)
	{
		//Ŀǰ������ʾ������Ϣ,������Щ��Ϣ
		SetDlgItemText(IDC_BTN_WRGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	else
	{
		//Ŀǰû����ʾ������Ϣ, ��ʾ��Щ��Ϣ
		SetDlgItemText(IDC_BTN_WRGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	HideSomeInfo(m_bMoreInfo);
	m_bMoreInfo = !m_bMoreInfo;
}

/*************************************************************
 �� �� ����HideSomeInfo()
 ���ܸ�Ҫ������/��ʾĳЩ��Ϣ
 �� �� ֵ: void
 ��    ����param1	ָ���Ƿ�����
**************************************************************/
//##ModelId=49B87B80031D
void CWaveRecGeneral::HideSomeInfo( BOOL bHide )
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
	//�豸�汾��
	pWnd = GetDlgItem(IDC_STATIC_WRGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_WRGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	if(g_PTControl == 1 && nCode == SW_SHOW)
		nCode = SW_SHOW;
	else
		nCode = SW_HIDE;
	//Զ��������
	pWnd = GetDlgItem(IDC_WRGEN_SP2);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_STATIC_WRGEN_CPU);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	m_btnStart.ShowWindow(nCode);
	m_btnTime.ShowWindow(nCode);
	m_gif.ShowWindow(nCode);
}

//##ModelId=49B87B80037A
void CWaveRecGeneral::OnBtnWrgenTime() 
{
	// TODO: Add your control notification handler code here
	//����ǿ�ƶ�ʱ��ť, ǿ�ƶ�ʱ��ȡ��
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
		
		//��֯20059����
		if(!pApp->m_SttpEngine.BuildDataFor20059PTSetTime(sttpData, m_pObj->m_sID, m_nCPU))
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
				str.Format("¼����%s����ǿ�ƶ�ʱ����ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("¼����%s����ǿ�ƶ�ʱ����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 1;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nVerifyTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 �� �� ����UpdateControlsEnable()
 ���ܸ�Ҫ������ҳ�浱ǰ״̬, �ı�ؼ����ö�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B80032C
void CWaveRecGeneral::UpdateControlsEnable()
{
	//��״̬�����
	if(m_nCurrentStatus == 0)
	{	
		SetDlgItemText(IDC_BTN_WRGEN_TIME, StringFromID(IDS_BTN_FORCE_SYNCTIME));
		SetDlgItemText(IDC_BTN_WRGEN_START, StringFromID(IDS_BTN_REMOTESTART));
		SetDlgItemText(IDC_BTN_WRGEN_CALL, StringFromID(IDS_BTN_CALL));
		//����״̬,������
		GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_MORE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_WRGEN_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(TRUE);
		//ֹͣ����
		m_gif.Stop();
		//ֹͣ��ʱ
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 6)
	{
		//�ٻ������״̬,�����Լ������Ķ�������
		
		GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_MORE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_WRGEN_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(FALSE);
		//��ʼ����
		m_gif.Draw();
		//��ʼ��ʱ
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_WRGEN, str);
		
		//���Լ�����
		switch(m_nCurrentStatus)
		{
		case 1: //ǿ�ƶ�ʱ
			SetDlgItemText(IDC_BTN_WRGEN_TIME, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_TIME) ->EnableWindow(TRUE);
			break;
		case 2: //Զ������
			SetDlgItemText(IDC_BTN_WRGEN_START, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_START) ->EnableWindow(TRUE);
			break;
		case 5: //ͨѶ״̬�ٻ�
			SetDlgItemText(IDC_BTN_WRGEN_CALL, CANCEL_NULL);
			GetDlgItem(IDC_BTN_WRGEN_CALL) ->EnableWindow(TRUE);
		default:
			break;
		}
	}
}

//##ModelId=49B87B80037C
void CWaveRecGeneral::OnBtnWrgenStart() 
{
	// TODO: Add your control notification handler code here
	//����Զ��������ť, ִ��Զ��������ȡ��
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
		
		//��֯20123����
		if(!pApp->m_SttpEngine.BuildDataFor20123WaveRecRun(sttpData, m_pObj->m_sID, m_nCPU))
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
				str.Format("¼����%s����Զ����������ʧ��,ԭ��Ϊ�����ж�", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("¼����%s����Զ����������ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//�ɹ�,�ı�״̬�Ͱ�ť����
			m_nCurrentStatus = 2;

			//������ʱ��
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nRemoteStartTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 �� �� ����FillCPU()
 ���ܸ�Ҫ�����CPU������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B80032D
void CWaveRecGeneral::FillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pCmb == NULL)
		return;
	//���
	pCmb->ResetContent();
	
	if(m_pObj == NULL)
		return;
	
	//CPU������, �������Ϊ"CPU��"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//ȡ��CPU����
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d", pCpu->code); //cpu��
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//����Ĭ��ѡ��
	if(pCmb->GetCount() > 0)
	{
		pCmb->SetCurSel(0);
		//���浱ǰѡ��
		int nIndex = pCmb->GetCurSel();
		CString sCPU;
		pCmb->GetLBText(nIndex, sCPU);
		m_nCPU = atoi(sCPU);
		return;
	}
}

//##ModelId=49B87B80037E
void CWaveRecGeneral::OnSelchangeCmbWrgenCpu() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRGEN_CPU);
	if(pCmb == NULL)
		return;
	//���浱ǰѡ��
	int nIndex = pCmb->GetCurSel();
	CString sCPU;
	pCmb->GetLBText(nIndex, sCPU);
		m_nCPU = atoi(sCPU);
}

/*************************************************************
 �� �� ����OnSTTP20060()
 ���ܸ�Ҫ����Ӧװ�ö�ʱ֪ͨ(20060)
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B800399
void CWaveRecGeneral::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//û�д��ڶ�ʱ��״̬,��������
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
		str.Format("CWaveRecGeneral::OnSTTP20060 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20060 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU���Ƿ��Ӧ
	int nCPU = pSttpData->sttp_body.nCpu;
	if(nCPU != m_nCPU)
	{
		//���Ĵ���
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20060 �յ������忨�ı���, �忨��Ϊ%d", nCPU);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	int nReturn = pSttpData->sttp_body.nStatus;

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;	
	//���Ŀؼ�������
	UpdateControlsEnable();
	if(nReturn == 0)
	{
		//�ɹ�
		AfxMessageBox(StringFromID(IDS_SYNCTIME_SUCCESS));
	}
	else
	{
		//ʧ�� 
		AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
	}
	
}

/*************************************************************
 �� �� ����OnSTTP20124()
 ���ܸ�Ҫ��¼����Զ������֪ͨ
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	��������
**************************************************************/
//##ModelId=49B87B80039C
void CWaveRecGeneral::OnSTTP20124( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//û�д���Զ��������״̬,��������
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
	if(pSttpData->sttp_head.uMsgID != 20124)
	{
		//������
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 �յ�����װ�ñ���, װ��IDΪ%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//���CPU���Ƿ��Ӧ
	int nCPU = pSttpData->sttp_body.nCpu;
	if(nCPU != m_nCPU)
	{
		//���Ĵ���
		CString str;
		str.Format("CWaveRecGeneral::OnSTTP20124 �յ������忨�ı���, �忨��Ϊ%d", nCPU);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,XJ_OPER_RemoteStart,%d", pSttpData->sttp_body.nStatus, pSttpData->sttp_body.nCpu);

	pApp->AddNewManOperator(FUNC_OPT_RUNWAVEREC, m_pObj->m_sID, strMsg, XJ_OPER_RemoteStart, pSttpData->sttp_body.nStatus);

	//�رն�ʱ��
	KillTimer(m_nTimer);
	
	//������
	int nReturn = pSttpData->sttp_body.nStatus;

	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;	
	//���Ŀؼ�������
	UpdateControlsEnable();
	if(nReturn == 1)
	{
		//�ɹ�
		AfxMessageBox(StringFromID(IDS_REMOTESTART_SUCCESS));
	}
	else
	{
		//ʧ�� 
		AfxMessageBox(StringFromID(IDS_REMOTESTART_FAIL));
	}
}

//##ModelId=49B87B8003AB
void CWaveRecGeneral::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 5)
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
		//���Ǳ����ܵı���
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
			//����ǿ�ƶ�ʱ

			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
				
			CString str;
			str.Format("¼����[%s]ǿ�ƶ�ʱʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//����Զ������

			m_nCurrentStatus = 0;
			
			//���Ŀؼ����ö�
			UpdateControlsEnable();
				
			CString str;
			str.Format("¼����[%s]Զ������ʧ��!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			AfxMessageBox(StringFromID(IDS_REMOTESTART_FAIL));
		}	
	}
}

//##ModelId=49B87B8003B9
void CWaveRecGeneral::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 5)
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
	//�������,�ı�״̬�Ͱ�ť����
	if(m_nCurrentStatus == 1)
	{
		//����ǿ�ƶ�ʱ
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//�����ٻ�ʱ��
		m_nCurrentStatus = 0;
		
		//���Ŀؼ����ö�
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}

	
}

//##ModelId=49B87B80038A
void CWaveRecGeneral::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		KillTimer(m_nTimer);
		//��ʾ
		CString str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		
		//�ı�״̬
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(str);
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//�ı����ʱ����ʾ
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_WRGEN, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 �� �� ����OnWRFrameClose()
 ���ܸ�Ҫ����Ӧ���ڹر���Ϣ, �˳��߳�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8003C8
void CWaveRecGeneral::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 �� �� ����OnWindowRefresh()
 ���ܸ�Ҫ����Ӧ����ˢ����Ϣ, ���¿ؼ�ֵ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8003CB
void CWaveRecGeneral::OnWindowRefresh( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
}

/*************************************************************
 �� �� ����ReFillAll()
 ���ܸ�Ҫ�����¶�ȡ�������������
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B80033F
BOOL CWaveRecGeneral::ReFillAll()
{
	//���������Ч��
	if(m_pObj == NULL)
		return FALSE;
	m_strType = m_pObj->m_sModel; //�ͺ�
	m_strVer = m_pObj->m_sSysVer; //�汾��
	
	//ͨѶ״̬
	m_strCommStatus = GetCommStatusString(m_pObj->m_nCommStatus);
	
	//����״̬
	m_strRunStatus = GetRunStatusString(m_pObj->m_nRunStatu);
	

	FillCPU();

	return TRUE;
}

/*************************************************************
 �� �� ����CancelCall()
 ���ܸ�Ҫ��ȡ���ٻ�����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B80032E
void CWaveRecGeneral::CancelCall()
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
		//����ǿ�ƶ�ʱ
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20069, m_pObj->m_sID, m_nCPU);
	}
	else if(m_nCurrentStatus == 2)
	{
		//����Զ������
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20123, m_pObj->m_sID, m_nCPU);
	}
	else if(m_nCurrentStatus == 5)
	{
		//����Զ������
		//�齨ȡ������
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20003, m_pObj->m_sID, m_nCPU);
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
//##ModelId=49B87B80033C
BOOL CWaveRecGeneral::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20059 && m_nCurrentStatus == 1)
	{
		//ǿ�ƶ�ʱ
		return TRUE;
	}
	if(nMsgID == 20123 && m_nCurrentStatus == 2)
	{
		//Զ������
		return TRUE;
	}
	if (nMsgID == 20085 && m_nCurrentStatus == 5)
	{
		//¼����ͨѶ״̬����
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ���˳��Լ�����������߳�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B80033E
void CWaveRecGeneral::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B8003BC
void CWaveRecGeneral::OnSTTP20157( WPARAM wParam, LPARAM lParam )
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
		//����ǿ�ƶ�ʱ				
		int nTimeOut = pData->m_nVerifyTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		////����Զ������
		int nTimeOut = pData->m_nRemoteStartTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}


void CWaveRecGeneral::OnSTTP20086( WPARAM wparam, LPARAM lParam )
{
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
	CString strID = pSttpData->sttp_body.ch_pt_id;
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
	
	int nOSCStatus = pSttpData->sttp_body.nStatus;

    if (nOSCStatus  == 0)
	{
		m_strCommStatus = StringFromID(IDS_COMMUSTATUS_ON);
	}
	else if (nOSCStatus == 1)
	{
		m_strCommStatus = StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else if (nOSCStatus == 2)
	{
		m_strCommStatus = StringFromID(IDS_COMMON_UNKNOWN);
	}

	WriteLog("CWaveRecGeneral::OnSTTP20086, �յ�����״̬Ϊ" +  m_strCommStatus, XJ_LOG_LV3);
	//ˢ�½���
	UpdateData(FALSE);
	
	//�������,�ı�״̬�Ͱ�ť����
	m_nCurrentStatus = 0;
	
	//���Ŀؼ�������
	UpdateControlsEnable();
}

void CWaveRecGeneral::OnBtnWrgenCommhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 1;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CWaveRecGeneral::OnBtnWrgenRunhis() 
{
	// TODO: Add your control notification handler code here
	CDlgStatusHis dlg;
	dlg.m_nType = 2;
	dlg.m_pSec = m_pObj;
	dlg.DoModal();
}

void CWaveRecGeneral::OnBtnWrgenCall() 
{
	// TODO: Add your control notification handler code here
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
		
		//��֯20085����,����վ����
		if(!pApp->m_SttpEngine.BuildDataFor20085CommStatus(sttpData, m_pObj->m_sID))
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
				str.Format("%s����20086����ʧ��, ԭ��Ϊ�����ж�", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("%s����20086����ʧ��,ԭ��Ϊ��������", m_pObj->m_sID);
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
		}
	}
	else if(m_nCurrentStatus == 5)
	{
		//�����ٻ�״̬, ȡ���ٻ�
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

void CWaveRecGeneral::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

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

	/*
	if (!m_bGetStatus)
	{
		m_strCommStatus = GetCommStatusString(LoadOSCStatus());
		m_bGetStatus = true;
		UpdateData(FALSE);
	}
	*/
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



int CWaveRecGeneral::LoadOSCStatus()
{
	if(m_pObj == NULL)
		return FALSE;
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

	//�ֶ�
	//value
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
		WriteLog("CWaveRecGeneral::LoadOSCStatus(), ��ѯʧ��", XJ_LOG_LV3);
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
			str = pMemset->GetValue((UINT)1);
			WriteLog("����¼����ͨѶ״̬Ϊ" + GetCommStatusString(m_pObj->m_nCommStatus) + " ʱ��:" + str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecGeneral::LoadOSCStatus(),��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		m_pObj->m_nCommStatus = 2;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return m_pObj->m_nCommStatus;
}


HBRUSH CWaveRecGeneral::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_WRGEN_COMM)
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

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_WRGEN_RUN)
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
	
	return hbr;
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenStart() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecGeneral::OnDoubleclickedBtnWrgenTime() 
{
	// TODO: Add your control notification handler code here
	
}
