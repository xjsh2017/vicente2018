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
	//读取内容
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//碰到退出线程,退出
		if(ptGeneral->m_bExitThread)
			break;
		//超过重试次数
		if(nCount < 0)
			break;
		//读取完成, 退出
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
			//读取失败
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
	//记录控件位置
	RecordRate(IDC_PTGEN_SP, 1, left_client, top_client);
	RecordRate(IDC_PTGEN_SP2, 1, left_client, top_client);
	RecordRate(IDC_PTGEN_SP3, 1, left_client, top_client);
	
	//左边
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

	//右边
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

	//按钮
	RecordRate(IDC_BTN_PTGEN_MORE, 0, null_effect, null_effect, right_client, bottom_client);
	RecordRate(IDC_BTN_PTGEN_CPU, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTGEN_ONE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTGEN_GEN, 0, left_client, top_client);

	//装置内时间
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
		//不显示“更多信息”按钮
		m_btnMore.ShowWindow(SW_HIDE);
	}
	//初始化控件
	InitControls();
	//隐藏某些信息
	HideSomeInfo(TRUE);

	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}

	//初始化值
	RefreshControls();
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件, 设置初始值, 显示风格
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B910149
int CPTGeneral::InitControls()
{
	/*
	//查看更多按钮
	m_btnMore.SetThemeHelper(&m_ThemeHelper);
//	m_btnMore.SetIcon(IDI_SEARCH);
	m_btnMore.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnMore.SetTooltipText(_T("查看更多相关信息"));

	//查看CPU按钮
	m_btnCPU.SetThemeHelper(&m_ThemeHelper);
//	m_btnCPU.SetIcon(IDI_SEARCH);
	m_btnCPU.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCPU.SetTooltipText(_T("查看装置CPU列表"));

	//查看一次设备按钮
	m_btnOne.SetThemeHelper(&m_ThemeHelper);
//	m_btnOne.SetIcon(IDI_SEARCH);
	m_btnOne.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOne.SetTooltipText(_T("查看关联一次设备属性"));

	//召唤基本属性按钮
	m_btnCallGen.SetThemeHelper(&m_ThemeHelper);
//	m_btnCallGen.SetIcon(IDI_SEARCH);
	m_btnCallGen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallGen.SetTooltipText(_T("召唤设备基本属性"));

	//召唤时间按钮
	m_btnCallTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnCallTime.SetIcon(IDI_SEARCH);
	m_btnCallTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallTime.SetTooltipText(_T("召唤设备时间"));

	//强制对时按钮
	m_btnSetTime.SetThemeHelper(&m_ThemeHelper);
//	m_btnSetTime.SetIcon(IDI_CONTROL);
	m_btnSetTime.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSetTime.SetTooltipText(_T("对设备进行对时"));

	//信息复归按钮
	m_btnReSet.SetThemeHelper(&m_ThemeHelper);
//	m_btnReSet.SetIcon(IDI_CONTROL);
	m_btnReSet.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnReSet.SetTooltipText(_T("信息复归"));

	//历史通讯状态按钮
	m_btnCommHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommHis.SetTooltipText(_T("查看历史"));

	//历史运行状态按钮
	m_btnRunHis.SetThemeHelper(&m_ThemeHelper);
//	m_btnRunHis.SetIcon(IDI_HISTORY);
	m_btnRunHis.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRunHis.SetTooltipText(_T("查看历史"));

	//召唤通讯状态
	m_btnCommCall.SetThemeHelper(&m_ThemeHelper);
	//	m_btnCommHis.SetIcon(IDI_HISTORY);
	m_btnCommCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCommCall.SetTooltipText(_T("召唤"));
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
 函 数 名：RefreshControls()
 功能概要：刷新控件值
 返 回 值: void
**************************************************************/
//##ModelId=49B87B910157
void CPTGeneral::RefreshControls()
{
	if(m_pObj == NULL)
		return;

	if(m_bLoadData)
	{
		//通讯状态
		m_strComm = GetCommStatusString(m_pObj->m_nCommStatus);
		m_strRun = GetRunStatusString(m_pObj->m_nRunStatu);
		//挂牌状态
		m_strSuspend = m_pObj->m_nSuspendstatus == 0?StringFromID(IDS_SUSPEND_NO):StringFromID(IDS_SUSPEND_YES);
		UpdateData(FALSE);
		return;
	}

	m_bLoadData = TRUE;

	CString str;
	str.Format("%d", m_pObj->m_n103Addr);
	m_str103Addr = str; //103地址
	m_strAddr = m_pObj->m_sAddr; //装置地址
	if(m_pObj->m_pOwner != NULL)
		m_strOne = m_pObj->m_pOwner->m_sName; //一次设备名称
	m_strTime = m_pObj->m_sDevTime; //程序生成时间
	m_strVer = m_pObj->m_sSysVer; //设备版本号
	m_strCOMCheck = m_pObj->m_sCrc; //通信校检码
	m_strType = m_pObj->m_sModel; //设备类型

	m_strIPA = m_pObj->m_sIPA;
	m_strIPB = m_pObj->m_sIPB;
	m_strPortA.Format("%d", m_pObj->m_nPortA);
	m_strPortB.Format("%d", m_pObj->m_nPortB);

	//通讯状态
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

	//运行状态
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

	//设备类型
//	GetPTType(m_pObj->m_nPTTypeID);
	//通信信息
	//GetCOMInfo(m_pObj->m_nComID, m_pObj->m_sStationID);

	//挂牌状态
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
 函 数 名：OnPTFrameOpen()
 功能概要：响应打开保护属性页消息, 更新内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B910292
void CPTGeneral::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = pObj;

	int nIndex = (int)wParam;


	m_bLoadData = FALSE;
	StartThread();
	RefreshControls();
	//第一次打开时第一页的控件刷新有问题, 原因未明, 现强制刷新一次
	RegulateControlSize();
// 	}
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//清除召唤计时
	SetDlgItemText(IDC_STATIC_TIME_PTGEN, "");
}

//##ModelId=49B87B910232
void CPTGeneral::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
}

/*************************************************************
 函 数 名：HideSomeInfo()
 功能概要：隐藏某些信息
 返 回 值: void
 参    数：param1	是否隐藏
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

	//软件版本号
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_VER);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//装置地址
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//通信口
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//通信参数
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COMPARAM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COMPARAM);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//程序生成时间
// 	pWnd= GetDlgItem(IDC_STATIC_PTGEN_TIME);
// 	if(pWnd != NULL)
// 		pWnd->ShowWindow(nCode);
// 	pWnd = GetDlgItem(IDC_EDIT_PTGEN_TIME);
// 	if(pWnd != NULL)
// 		pWnd->ShowWindow(nCode);

	//103地址
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_103ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_103ADDR);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//通信协议
	pWnd= GetDlgItem(IDC_STATIC_PTGEN_COMPRO);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);
	pWnd = GetDlgItem(IDC_EDIT_PTGEN_COMPRO);
	if(pWnd != NULL)
		pWnd->ShowWindow(nCode);

	//通信校检码
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

	//按钮
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
		//目前正在显示更多信息,隐藏这些信息
		HideSomeInfo(TRUE);
		m_bMoreInfo = FALSE;
		SetDlgItemText(IDC_BTN_PTGEN_MORE, StringFromID(IDS_BTN_MOREINFO));
	}
	else
	{
		//目前没有显示更多信息, 显示这些信息
		HideSomeInfo(FALSE);
		m_bMoreInfo = TRUE;
		SetDlgItemText(IDC_BTN_PTGEN_MORE, StringFromID(IDS_BTN_HIDEINFO));
	}
}

/*************************************************************
 函 数 名：GetPTType()
 功能概要：从数据库表(tb_pt_type)中查找保护类型,保存在m_strType中
 返 回 值: void
 参    数：param1	设备类型ID
**************************************************************/
//##ModelId=49B87B910177
BOOL CPTGeneral::GetPTType( int nID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//字段
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);
		
	//条件
	//只查询未确认事件
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
		WriteLogEx("CPTGeneral::GetPTType, 查询失败");
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
			//应该只有一条记录
			m_strType = pMemset->GetValue((UINT)0);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetPTType,查询失败,原因为%s", sError);
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
 函 数 名：GetCOMInfo()
 功能概要：从数据库表(tb_port_config)中查找通信口名称,波特率,通信协议ID
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数：param1	通信口ID
		   Param2	厂站ID
**************************************************************/
//##ModelId=49B87B910196
BOOL CPTGeneral::GetCOMInfo( int nCOM_ID, CString sStationID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//字段
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);

	//波特率
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "baudrate", EX_STTP_DATA_TYPE_INT);

	//通信协议ID
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "protocol_id", EX_STTP_DATA_TYPE_INT);
	
	//条件
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
		WriteLogEx("CPTGeneral::GetCOMInfo, 查询失败");
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
			//应该只有一条记录
			EnterCriticalSection(&m_CriticalOper);  
			m_strCOM = pMemset->GetValue((UINT)0); //通信口名称
			m_strCOMParam = pMemset->GetValue(1); //通信参数
			LeaveCriticalSection(&m_CriticalOper);
			//规约ID,查找规约
			CString str = pMemset->GetValue(2);
			GetProtocolInfo(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetCOMInfo,查询失败,原因为%s", sError);
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
 函 数 名：GetProtocolInfo()
 功能概要：从数据库表(tb_front_protocol_config)中查找规约名称
 返 回 值: 查找成功返回TRUE,失败返回FALSE
 参    数：param1	规约ID
**************************************************************/
//##ModelId=49B87B9101A5
BOOL CPTGeneral::GetProtocolInfo( CString sID )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//SELECT Name FROM tb_pt_type WHERE ID= 	
	//字段
	//TOP num
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "name", EX_STTP_DATA_TYPE_STRING);
	
	//条件,protocol_id
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
		WriteLogEx("CPTGeneral::GetProtocolInfo, 查询失败");
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
			//应该只有一条记录
			EnterCriticalSection(&m_CriticalOper);
			m_strCOMPro = pMemset->GetValue((UINT)0);
			LeaveCriticalSection(&m_CriticalOper);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::GetProtocolInfo,查询失败,原因为%s", sError);
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
 函 数 名：UpdateControlsEnable()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9101B5
void CPTGeneral::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{
		//召唤基本属性
		SetDlgItemText(IDC_BTN_PTGEN_GEN, StringFromID(IDS_BTN_CALLPROPERTY));
		//召唤时间
		SetDlgItemText(IDC_BTN_PTGEN_TIME, StringFromID(IDS_BTN_CALLTIME));
		//强制对时
		SetDlgItemText(IDC_BTN_PTGEN_SETTIME, StringFromID(IDS_BTN_FORCE_SYNCTIME));
		//信号复归
		SetDlgItemText(IDC_BTN_PTGEN_RESET, StringFromID(IDS_BTN_SIGNALRETURN));
		//召唤通讯状态
		SetDlgItemText(IDC_BTN_PTGEN_CALL, StringFromID(IDS_CALLOPER_CALL));
		//空闲状态,都可用
		GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_MORE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTGEN_CALL) ->EnableWindow(TRUE);
		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 6)
	{
		//召唤或操作状态,除了自己其它的都不可用
		
		GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_MORE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTGEN_CALL) ->EnableWindow(FALSE);
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTGEN, str);
		
		//让自己可用
		switch(m_nCurrentStatus)
		{
		case 1: //召唤基本属性
			SetDlgItemText(IDC_BTN_PTGEN_GEN, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTGEN_GEN) ->EnableWindow(TRUE);
			break;
		case 2: //召唤装置时间
			SetDlgItemText(IDC_BTN_PTGEN_TIME, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTGEN_TIME) ->EnableWindow(TRUE);
			break;
		case 3: //强制对时
			SetDlgItemText(IDC_BTN_PTGEN_SETTIME, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTGEN_SETTIME) ->EnableWindow(TRUE);
			break;
		case 4: //信息复归
			SetDlgItemText(IDC_BTN_PTGEN_RESET, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTGEN_RESET) ->EnableWindow(TRUE);
			break;
		case 5: //召唤运行状态
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
	//按下召唤基本属性按钮, 召唤基本属性或取消召唤
	MYASSERT(m_pObj);
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20110报文(目前页面不支持按CPU召唤基本信息)
		if(!pApp->m_SttpEngine.BuildDataFor20110PTGeneral(sttpData, m_pObj->m_sID, 0,1))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送召唤基本信息报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤基本信息报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变按钮文字,修改当前状态
			m_nCurrentStatus = 1;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nDevGeneralTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//更改控件可用度
			UpdateControlsEnable();
			CString str;
			str.Format("保护[%s]发送召唤基本信息报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//正在召唤状态, 取消召唤
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
	//按下信号复归按钮, 信号复归或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
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

		//组织20057报文(暂不支持CPU)
		if(!pApp->m_SttpEngine.BuildDataFor20057ResetPT(sttpData, m_pObj->m_sID, nCpu))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送信号复归报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送信号复归报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变状态和按钮文字
			m_nCurrentStatus = 4;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nSignalResetTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送信号复归报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在召唤状态, 取消召唤
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
	//按下强制对时按钮, 强制对时或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20059报文(暂不支持CPU)
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
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送强制对时报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送强制对时报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变状态和按钮文字
			m_nCurrentStatus = 3;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nVerifyTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送强制对时报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在召唤状态, 取消召唤
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
	//按下召唤时间按钮, 召唤时间或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//正处于空闲状态,开始召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20088报文
		if(!pApp->m_SttpEngine.BuildDataFor20088PTTime(sttpData, m_pObj->m_sID))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送召唤装置时间报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤装置时间报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,改变状态和按钮文字
			m_nCurrentStatus = 2;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nDevTimeTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送召唤装置时间报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤状态, 取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox( StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 函 数 名：OnSTTP20111()
 功能概要：响应20111(保护基本信息下载通知)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B91029F
void CPTGeneral::OnSTTP20111( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤基本属性的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20111报文
	if(pSttpData->sttp_head.uMsgID != 20111)
	{
		//错误报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20111 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20111 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//装置版本号
	m_strVer = pSttpData->sttp_body.ch_version;
	m_pObj->m_sSysVer = m_strVer;
	//设备时间
	m_strTime = pSttpData->sttp_body.ch_time_12_BIT1;
	CTime tmTime;
	tmTime = StringToTimeSttp12(m_strTime);
	m_strTime = tmTime.Format("%Y-%m-%d");
	m_pObj->m_sDevTime = m_strTime;
	//通讯校检码
	m_strCOMCheck = pSttpData->sttp_body.ch_check_code;
	m_pObj->m_sCrc = m_strCOMCheck;

	//刷新界面
	UpdateData(FALSE);
	
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤基本信息成功", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_DEVINFO);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_SUCCESS);
}

/*************************************************************
 函 数 名：OnSTTP20089()
 功能概要：响应召唤装置时间通知(20089)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9102B1
void CPTGeneral::OnSTTP20089( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//没有处于召唤时间的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20089报文
	if(pSttpData->sttp_head.uMsgID != 20089)
	{
		//错误报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20089 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20089 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	m_strInsideTime = pSttpData->sttp_body.ch_time_15_BIT1;
	int nms = 0;
	CTime tmTime = StringToTimeSttp15(m_strInsideTime, nms);
	m_strInsideTime.Format("%s.%03d", tmTime.Format("%Y-%m-%d %H:%M:%S"), nms);
	
	//刷新界面
	UpdateData(FALSE);
	
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤装置时间成功", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_DEVTIME);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_SUCCESS);
	
}

/*************************************************************
 函 数 名：OnSTTP20060()
 功能概要：响应20060(保护对时通知)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9102C0
void CPTGeneral::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 3)
	{
		//没有处于保护对时的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20060报文
	if(pSttpData->sttp_head.uMsgID != 20060)
	{
		//错误报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20060 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20060 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CALL_PROTECT_SETTIME);
	
	pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, pSttpData->sttp_body.nStatus);
	
	//处理报文
	int nReturn = pSttpData->sttp_body.nStatus;
	if(nReturn == 0)
	{
		CString str;
		str.Format("保护[%s]强制对时成功", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//成功
		AfxMessageBox( StringFromID(IDS_SYNCTIME_SUCCESS));
	}
	else
	{
		CString str;
		str.Format("保护[%s]强制对时失败", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//失败 
		AfxMessageBox( StringFromID(IDS_SYNCTIME_FAIL));
	}
}

/*************************************************************
 函 数 名：OnSTTP20058()
 功能概要：响应20058(保护信号复归通知)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9102CE
void CPTGeneral::OnSTTP20058( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		//没有处于信号复归的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20058报文
	if(pSttpData->sttp_head.uMsgID != 20058)
	{
		//错误报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20058 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTGeneral::OnSTTP20058 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_SIGNRESET, pSttpData->sttp_body.nCpu);
	
	pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, pSttpData->sttp_body.nStatus);

	//关闭定时器
	KillTimer(m_nTimer);

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();
	
	//处理报文
	int nReturn = pSttpData->sttp_body.nStatus;
	if(nReturn == 0)
	{
		CString str;
		str.Format("保护[%s]信号复归成功", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//成功
		AfxMessageBox( StringFromID(IDS_SIGNALRETURN_SUCCESS));
	}
	else
	{
		CString str;
		str.Format("保护[%s]信号复归失败", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
		//失败 
		AfxMessageBox( StringFromID(IDS_SIGNALRETURN_FAIL));
	}
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069(命令失败报文)
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9102D1
void CPTGeneral::OnSTTP20069( WPARAM wparam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
	{
		//没有处于操作状态, 不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本装置的报文
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);

	//处理报文
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//操作成功
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
	}
	else
	{
		//操作失败
		//处理完毕,改变状态和按钮文字
		if(m_nCurrentStatus == 1)
		{
			//正在召唤基本属性
				
			CString str;
			str.Format("保护[%s]召唤基本属性失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVINFO);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_FAILD);

			AfxMessageBox( StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//正在召唤时间
				
			CString str;
			str.Format("保护[%s]召唤装置时间失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVTIME);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 3)
		{
			//正在强制对时
				
			CString str;
			str.Format("保护[%s]强制对时失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_PROTECT_SETTIME);
			
			pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_SYNCTIME_FAIL));
		}
		else if(m_nCurrentStatus == 4)
		{
			//正在信号复归
				
			CString str;
			str.Format("保护[%s]信号复归失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_SIGNRESET);
			
			pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, OPER_FAILD);

			AfxMessageBox(StringFromID(IDS_SIGNALRETURN_FAIL));
		}
		else if (m_nCurrentStatus == 5)
		{
			//正在信号复归
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
			
			CString str;
			str.Format("保护[%s]召唤通信状态失败", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->AddNewManOperator("召唤装置通信状态", m_pObj->m_sID, "召唤装置通信状态失败", -1, OPER_FAILD);
			
			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
		}
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持报文)
 返 回 值: 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9102E0
void CPTGeneral::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTGeneral::OnSTTP20125", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
	{
		//没有处于召唤开关量的状态,不处理报文
		WriteLog("CPTGeneral::OnSTTP20125 没有处于召唤开关量的状态,不处理报文", XJ_LOG_LV3);
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		WriteLog("CPTGeneral::OnSTTP20125 没有处于召唤开关量的状态,不处理报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20125报文
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//错误报文
		WriteLog("CPTGeneral::OnSTTP20125 错误报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		WriteLog("CPTGeneral::OnSTTP20125 不是本装置的报文", XJ_LOG_LV3);
		return;
	}

	//检查是否本功能的报文
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		WriteLog("CPTGeneral::OnSTTP20125 不是本功能的报文", XJ_LOG_LV3);
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);
	//处理报文
	//处理完毕,改变状态和按钮文字
	if(m_nCurrentStatus == 1)
	{
		//正在召唤基本属性
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CString str;
		str.Format("保护[%s]召唤基本信息失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVINFO);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLGENERAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVINFO, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤时间
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
		CString str;
		str.Format("保护[%s]召唤装置时间失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_DEVTIME);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DEVTIME, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在强制对时
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CString str;
		str.Format("保护[%s]强制对时失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_PROTECT_SETTIME);
		
		pApp->AddNewManOperator(FUNC_OPT_SETPTTIME, m_pObj->m_sID, strMsg, XJ_OPER_CALL_PROTECT_SETTIME, OPER_FAILD);
	

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在信号复归
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CString str;
		str.Format("保护[%s]信号复归失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_SIGNRESET);
		
		pApp->AddNewManOperator(FUNC_OPT_SIGNRESET, m_pObj->m_sID, strMsg, XJ_OPER_SIGNRESET, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if (m_nCurrentStatus == 5)
	{
		//正在信号复归
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CString str;
		str.Format("保护[%s]召唤通信状态失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->AddNewManOperator("召唤装置通信状态", m_pObj->m_sID, "召唤装置通信状态失败", -1, OPER_FAILD);

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
		//关闭定时器
		KillTimer(m_nTimer);
		//提示
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
			pApp->AddNewManOperator("召唤装置通信状态", m_pObj->m_sID, "召唤装置通信状态失败", -1, OPER_FAILD);
			break;
		default:
			str = "";
		}
		//改变状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		CString sLog;
		sLog.Format("[%s]%s", m_pObj->m_sName, str);
		WriteLog(sLog, XJ_LOG_LV1);
		AfxMessageBox(str);
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTGEN, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：响应关闭窗口消息,退出线程 
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102F1
void CPTGeneral::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TURE, 失败返回FALSE
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
 函 数 名：OnWindowRefresh()
 功能概要：响应窗口刷新消息, 刷新窗口控件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102FE
void CPTGeneral::OnWindowRefresh( WPARAM wParam, LPARAM lParam )
{
	UpdateData(FALSE);
}

/*************************************************************
 函 数 名：OnSTTP20004()
 功能概要：响应保护通讯状态下载通知
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102EE
void CPTGeneral::OnSTTP20004( WPARAM wparam, LPARAM lParam )
{
	WriteLog("收到20004报文", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 5)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	

	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.variant_member.value_data[0].str_value.c_str();
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	
	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	int nPTStatus = pSttpData->sttp_body.variant_member.value_data[0].id;
	if(nPTStatus != m_pObj->m_nCommStatus)
	{
		m_pObj->SetCommStatus(nPTStatus);
		m_pObj->m_tmNewCommStatus = CTime::GetCurrentTime();
		m_pObj->RefreshCommDurativeTime();
	}
	m_strComm = GetCommStatusString(nPTStatus);

	//刷新界面
	UpdateData(FALSE);
	
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//更改控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]发送召唤装置通信状态成功", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	pApp->AddNewManOperator("召唤装置通信状态", m_pObj->m_sID, "召唤装置通信状态成功", -1);
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
		//未载入, 现在载入
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
	//载入数据
	RefreshControls();
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9101B6
void CPTGeneral::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20110, m_pObj->m_sID, 0, 1);
	}
	else if(m_nCurrentStatus == 2)
	{
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20088, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20059, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在切换定值区
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20057, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 5)
	{
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20003, m_pObj->m_sID, 0);
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己起的所有线程
 返 回 值: void
 参    数：param1
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
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B9101B7
BOOL CPTGeneral::IsVaild20069( int nMsgID )
{
	if(20110 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤基本属性
		return TRUE;
	}
	if(20057 == nMsgID && 4 == m_nCurrentStatus)
	{
		//信号复归
		return TRUE;
	}
	if(20088 == nMsgID && 2 == m_nCurrentStatus)
	{
		//召唤时间
		return TRUE;
	}
	if(20059 == nMsgID && 3 == m_nCurrentStatus)
	{
		//强制对时
		return TRUE;
	}
	if (20003 == nMsgID && 5 == m_nCurrentStatus)
	{
		//召唤通讯状态状态
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
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9102EE
void CPTGeneral::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4 && m_nCurrentStatus != 5)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}
	
	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	if(m_nCurrentStatus == 1)
	{
		//正在召唤基本属性
		
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤时间
		
		int nTimeOut = pData->m_nDevTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在强制对时
		
		int nTimeOut = pData->m_nVerifyTimeTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在信号复归
		
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
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20003报文,从子站下载
		if(!pApp->m_SttpEngine.BuildDataFor20003CommStatus(sttpData, m_pObj->m_sID))
		{
			//组织报文失败
			//发送召唤完成消息,结果为发送失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("[%s]发送召唤通信状态报文失败, 原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("[%s]发送召唤通信状态报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
				return;
			}
		}
		else
		{
			//成功,改变按钮文字,修改当前状态
			m_nCurrentStatus = 5;
			
			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nCommStatusTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("[%s]发送召唤通信状态报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 5)
	{
		//正在召唤状态, 取消召唤
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
	//与数据平台访问DLL的连接可用
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
		
	//字段
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "curtime", EX_STTP_DATA_TYPE_TIME);
	
	CString str;
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	
	//按时间排序
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
		WriteLog("CPTGeneral::LoadPTStatus(), 查询失败", XJ_LOG_LV3);
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
			//取第一个
			str = pMemset->GetValue((UINT)0);
			m_pObj->m_nCommStatus = atoi(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTGeneral::LoadPTStatus(),查询失败,原因为%s", sError);
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
       // pDC->SetTextColor(RGB(63, 203, 70));//字体背景色******
	   pDC->SetBkColor(RGB(98, 213, 103));//字体背景色******
		HBRUSH B = CreateSolidBrush(RGB(98, 213, 103)); //背景色
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
			pDC->SetBkColor(RGB(131, 222, 135));//字体背景色******
			B = CreateSolidBrush(RGB(131, 222, 135)); //背景色
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//字体背景色******
			B = CreateSolidBrush(RGB(255, 104, 104)); //背景色
		}
		
		return B;
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_PTGEN_RUN)
	{
		HBRUSH B;
		if (m_pObj->m_nRunStatu == 2)
		{
			pDC->SetBkColor(RGB(131, 222, 135));//字体背景色******
			B = CreateSolidBrush(RGB(131, 222, 135)); //背景色
		}
		else
		{
			pDC->SetBkColor(RGB(255, 104, 104));//字体背景色******
			B = CreateSolidBrush(RGB(255, 104, 104)); //背景色
		}
		
		return B;
	}

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_PTGEN_SUSPEND)
	{
		HBRUSH B;
		if(m_pObj->m_nSuspendstatus == 0)
		{
			//未挂牌
			pDC->SetBkColor(RGB(131, 222, 135));//字体背景色******
			B = CreateSolidBrush(RGB(131, 222, 135)); //背景色
		}
		else
		{
			pDC->SetBkColor(RGB(250, 120, 0));//字体背景色******
			B = CreateSolidBrush(RGB(250, 120, 0)); //背景色
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
函数名:   StartLoadThread	
返回值:   void	
功能概要: 
*****************************************************/
void CPTGeneral::StartThread()
{
	m_nQueryNo = GetNextQueryNo();
	//启动读取线程
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
	str.Format("保护基本信息查询线程成功退出,查询号:%d", nNo);
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
	str.Format("保护基本信息查询线程成功启动,查询号:%d", nNo);
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

