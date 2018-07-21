// BatchDetailSettingView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchDetailSettingView.h"
#include "DlgOCSet.h"
#include "GlobalFunc.h"
#include "DeviceList.h"
#include "DlgAutoCallStop.h"
#include "MainFrm.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

extern int g_iBatchViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailSettingView

IMPLEMENT_DYNCREATE(CBatchDetailSettingView, CViewBase)

//##ModelId=49B87BB103DA
CBatchDetailSettingView::CBatchDetailSettingView()
	: CViewBase(CBatchDetailSettingView::IDD)
{
	//{{AFX_DATA_INIT(CBatchDetailSettingView)
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_mapSel.RemoveAll();
	m_mapSave.RemoveAll();
	g_iBatchViewAsc = -1;
	m_bAnalog = TRUE;
	m_bDigital = TRUE;
	m_bSetting = TRUE;
	m_bZone = TRUE;
	m_bSoftBoard = TRUE;

	m_bMenuZone = FALSE;
	/** @brief           菜单项弹出前记录是否召唤定值*/
	m_bMenuSetting = FALSE;
	/** @brief           菜单项弹出前记录是否召唤开关量*/
	m_bMenuDigital = FALSE;
	/** @brief           菜单项弹出前记录是否召唤软压板*/
	m_bMenuSoftBoard = FALSE;
	/** @brief           菜单项弹出前记录是否召唤模拟量*/
	m_bMenuAnalog = FALSE;

	m_nCuruentStatus = 0;

	m_pAutoCall = NULL;

	m_nViewType = 0;

	m_sCyc = "";
	m_sStartTime = "";

	m_nOldSortCol = -1;
	m_pActiveStation = NULL;

	m_nTimer = 1;
}

//##ModelId=49B87BB3009C
CBatchDetailSettingView::~CBatchDetailSettingView()
{
	m_mapSave.RemoveAll();
	TRACE("CBatchDetailSettingView::~CBatchDetailSettingView \n");
}


void CBatchDetailSettingView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchDetailSettingView)
	DDX_Control(pDX, IDC_BTN_AUTO_CALL_RESULT, m_btnCallResult);
	DDX_Control(pDX, IDC_BTN_BATCH_START_ALL_CALL, m_btnStartCall);
	DDX_Control(pDX, IDC_BTN_CALL_STOP, m_btnCallStop);
	DDX_Control(pDX, IDC_BTN_AUTO_CLOSE, m_btnAutoClose);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_DEVICE_LIST, m_btnDeviceList);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_SET, m_btnSet);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_SEL_WR, m_btnSelWR);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_SEL_PT, m_btnSelPT);
	DDX_Control(pDX, IDC_BTN_BATCH_SETTING_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_GIF_BATCH_SETTING, m_gif);
	DDX_Control(pDX, IDC_LIST_BATCH_DETAIL_SETTING, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBatchDetailSettingView, CViewBase)
	//{{AFX_MSG_MAP(CBatchDetailSettingView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BATCH_DETAIL_SETTING, OnCustomDraw)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_SEL_NULL, OnBtnBatchSelNull)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_SEL_PT, OnBtnBatchSelPt)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_SEL_WR, OnBtnBatchSelWr)

	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_SAVE, OnBtnBatchSave)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_SET, OnBtnBatchSet)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BATCH_DETAIL_SETTING, OnClickListBatchDetailSetting)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_RESET, OnBtnBatchSettingReset)
	ON_BN_CLICKED(IDC_BTN_BATCH_SETTING_DEVICE_LIST, OnBtnBatchSettingDeviceList)
	ON_BN_CLICKED(IDC_BTN_AUTO_CLOSE, OnBtnAutoClose)
	ON_BN_CLICKED(IDC_BTN_CALL_STOP, OnBtnCallStop)
	ON_BN_CLICKED(IDC_BTN_BATCH_START_ALL_CALL, OnBtnBatchStartAllCall)
	ON_BN_CLICKED(IDC_BTN_AUTO_CALL_RESULT, OnBtnAutoCallResult)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_BATCH_START_ALL_CALL, OnDoubleclickedBtnBatchStartAllCall)
	ON_BN_CLICKED(IDC_BTN_BATCH_PROCESS, OnBtnBatchProcess)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_CHECK, OnBatchCheck)

	ON_MESSAGE(AUTOCALL_SET_CHANGE, OnAutoCallSetChange)
	ON_MESSAGE(AUTOCALL_STATION_CHANGE, OnAutoCallStationChange)
	ON_MESSAGE(SHOW_BATCH, OnShowList)
	ON_MESSAGE(CHECK_CHANGE, OnCheckChange)
	ON_MESSAGE(BATCHCALL_END, OnBatchCallEnd)
	ON_MESSAGE(WM_STTP_00901, OnSTTP901)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailSettingView diagnostics

#ifdef _DEBUG
void CBatchDetailSettingView::AssertValid() const
{
	CViewBase::AssertValid();
}


void CBatchDetailSettingView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailSettingView message handlers


void CBatchDetailSettingView::OnInitialUpdate() 
{
	//记录控件位置
	RecordRate(IDC_BTN_BATCH_START_ALL_CALL, 0, left_client, top_client, null_effect, null_effect);

	RecordRate(IDC_BTN_BATCH_SETTING_SET, 0, IDC_BTN_BATCH_START_ALL_CALL, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_AUTO_CALL_RESULT, 0, IDC_BTN_BATCH_SETTING_SET, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_CALL_STOP, 0, IDC_BTN_AUTO_CALL_RESULT, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_BATCH_SETTING_SAVE, 0, IDC_BTN_CALL_STOP, top_client, null_effect, null_effect);
	
	RecordRate(IDC_BTN_BATCH_SETTING_SEL_WR, 0, IDC_BTN_AUTO_CALL_RESULT, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_BATCH_SETTING_SEL_PT, 0, IDC_BTN_BATCH_SETTING_SEL_WR, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_BATCH_SETTING_RESET, 0, IDC_BTN_BATCH_SETTING_SEL_PT, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_BATCH_SETTING_DEVICE_LIST, 0, IDC_BTN_BATCH_SETTING_RESET, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_AUTO_CLOSE, 0, IDC_BTN_BATCH_SETTING_DEVICE_LIST, top_client, null_effect, null_effect);

    CViewBase::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
	//初始化列表
	InitListCtrl();
}

void CBatchDetailSettingView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_BATCH_DETAIL_SETTING, left_client, IDC_BTN_BATCH_SETTING_SAVE, right_client, bottom_client);
	RegulateControlSize();
	Invalidate(TRUE);
}



/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB201D4
void CBatchDetailSettingView::InitListCtrl()
{
	MYASSERT(m_List.GetSafeHwnd()); 
	InitListStyle();
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB20203
int CBatchDetailSettingView::InitListStyle()
{
	//空图形列表, 用来调整行高
	CString headerformat="";
	headerformat.Format("%s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
		StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), StringFromID(IDS_SECPROP_SOFT),
		StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
	m_List.SetColumnHeader(headerformat);
	m_List.SetGridLines(TRUE); // SHow grid lines
	m_List.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
	m_List.SetEditable(FALSE); // Allow sub-text edit
	return 0;
}

//##ModelId=49B87BB30196
void CBatchDetailSettingView::OnDestroy() 
{
	CViewBase::OnDestroy();
	// TODO: Add your message handler code here
	ClearBatchSel();
}

/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：响应窗口打开消息, 刷新视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB302E4
void CBatchDetailSettingView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//设置控件
	SetControls(m_nViewType);
	LoadDataFromDB();
	//刷新一次
	RegulateControlSize();
	MoveCtrl(IDC_LIST_BATCH_DETAIL_SETTING, left_client, IDC_BTN_BATCH_SETTING_SAVE, right_client, bottom_client);
}

/*************************************************************
 函 数 名：OnBatchCheck()
 功能概要：响应设备树发生选择/取消选择动作, 进行增加或删除选择
 返 回 值: void
 参    数：param1	1表示选中, 0表示取消选择
		   Param2	设备指针
**************************************************************/
//##ModelId=49B87BB3031C
void CBatchDetailSettingView::OnBatchCheck( WPARAM wParam, LPARAM lParam )
{
	if (lParam == -1)
	{
		return;
	}

	if (lParam == 0)
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_AREA), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
		InsertNet();
		return;
	}

	if (0 < lParam && lParam < 11)
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
		InsertPriDevice(wParam, lParam);
	    return;
	}

	if (lParam == TYPE_WAVEREC)
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_RECORDER), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
        InsertWaverc(wParam, lParam);
		return;
	}

	CDeviceObj* pObj = (CDeviceObj*)lParam;

	if (pObj == NULL)
	{
		return;
	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CNetObj)))
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
	    InsertStation(wParam, lParam);
		return;
	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CStationObj)))
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_COMMON_DEVICETYPE), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
		InsertDeviceType(wParam, lParam);
		return;
	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CSecCPU)))
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_CPU), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
		
		InsertSingleCPU(wParam, lParam);
		return;	
	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CSecObj)) || pObj->IsKindOf(RUNTIME_CLASS(CSecCPU)))
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120", 
			StringFromID(IDS_MODEL_CPU), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT));
		m_List.SetColumnHeader(headerformat);
		
		InsertCPU(wParam, lParam);
		return;
		
	}

	if (pObj->IsKindOf(RUNTIME_CLASS(CDeviceObj)))
	{
		CString headerformat;
		headerformat.Format("%s, 150; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s, 120; %s 120", 
			StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
			StringFromID(IDS_SECPROP_SOFT),StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_SECPROP_OSC), StringFromID(IDS_SECPROP_HISTORYEVENT),StringFromID(IDS_MODEL_CPU));
		m_List.SetColumnHeader(headerformat);
		m_List.SetColumnHeader(_T("二次设备, 250; 定值区号, 100; 定值, 100; 开关量, 100; 软压板, 100; 模拟量, 100; 录波文件, 100; 历史事件, 100; CPU, 150"));
		InsertSecDevice(wParam, lParam);
		return;
	}
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：响应列表个性化显示消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB301C5
void CBatchDetailSettingView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//选项列
				if(nSubItem >= 1 && nSubItem <=7)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
				
					if(str == StringFromID(IDS_CALLOPER_NOTCALL))
					{
						lplvcd->clrText = g_BatchNotCall;
					}
					if(str == StringFromID(IDS_CALLOPER_CALL))
					{
						//lplvcd->clrText = g_BatchCall;
						lplvcd->clrText = RGB(0, 0, 255);
					}
					if(str == StringFromID(IDS_CALLOPER_NOTSUPPORT))
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

/*************************************************************
 函 数 名：OnBtnBatchSelNull()
 功能概要：点击"清除选择"按钮, 取消所有选择
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB30271
void CBatchDetailSettingView::OnBtnBatchSelNull() 
{
	// TODO: Add your control notification handler code here
	
		//wParam参数不写, lParam参数不写
		m_List.SetRedraw(FALSE);
		for(int i = 0; i < m_List.GetItemCount(); i++)
		{
			SendMessage(CHECK_CHANGE, (WPARAM)i, (LPARAM)0);
			m_List.SetCheck(i, FALSE);
		}

		m_List.SetRedraw(TRUE);

}

/*************************************************************
 函 数 名：OnBtnBatchSelPt()
 功能概要：点击"保护全选"按钮, 选择所有保护
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB30280
void CBatchDetailSettingView::OnBtnBatchSelPt() 
{
	// TODO: Add your control notification handler code here
	//发送消息给父窗口父窗口(因为父窗口是分隔窗口),让其转发给设备树视图
	POSITION pos = m_mapSave.GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = _T("");
	CString str = "";
	int iSetting = 1;
	m_btnSelPT.GetWindowText(str);
	if (!str.CompareNoCase(StringFromID(IDS_BTN_SELECTALL_RELAY)))
	{
		iSetting = 1;
		m_btnSelPT.SetWindowText(StringFromID(IDS_BTN_UNSELECTALL_RELAY));
	}
	else
	{
		iSetting = 0;
		m_btnSelPT.SetWindowText(StringFromID(IDS_BTN_SELECTALL_RELAY));
	}
	
	while(pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		MYASSERT_CONTINUE(pSel);
		MYASSERT_CONTINUE(pSel->m_pObj);
		if(pSel->m_pObj->m_nSecType != 9)
		{
			pSel->m_bChecked = iSetting;
		}
	}
}

/*************************************************************
 函 数 名：OnBtnBatchSelWr()
 功能概要：点击"录波器全选"按钮, 选择所有录波器
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CBatchDetailSettingView::OnBtnBatchSelWr() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_mapSave.GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = _T("");
	CString str = "";
	int iSetting = 1;
	m_btnSelWR.GetWindowText(str);
	if (!str.CompareNoCase(StringFromID(IDS_BTN_SELECTALL_RECORDER)))
	{
		iSetting = 1;
		m_btnSelWR.SetWindowText(StringFromID(IDS_BTN_UNSELECTALL_RECORDER));
	}
	else
	{
		iSetting = 0;
		m_btnSelWR.SetWindowText(StringFromID(IDS_BTN_SELECTALL_RECORDER));
	}

	while(pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		MYASSERT_CONTINUE(pSel);
		MYASSERT_CONTINUE(pSel->m_pObj);
		if(pSel->m_pObj->m_nSecType == 9)
		{
			pSel->m_bChecked = iSetting;
		}
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：改变控件可用性
 返 回 值: 
 参    数：param1	TRUE-控件可用.FALSE-控件不可用
		   Param2
**************************************************************/
//##ModelId=49B87BB2035E
void CBatchDetailSettingView::UpdateControlsEnable( BOOL bEnable )
{
	CWnd* pWnd = NULL;
	//取消选择按钮
	pWnd = GetDlgItem(IDC_BTN_BATCH_START_ALL_CALL);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//选择所有保护按钮
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_SAVE);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//选择所有录波器按钮
	pWnd = GetDlgItem(IDC_BTN_CALL_STOP);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//保存按钮
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_SET);
	if(pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//数据重置
	pWnd = GetDlgItem(IDC_BTN_AUTO_CALL_RESULT);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//设置周期
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_SET);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//设备列表
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_SEL_WR);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//设备列表
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_SEL_PT);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//设备列表
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_RESET);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_DEVICE_LIST);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	pWnd = GetDlgItem(IDC_BTN_AUTO_CLOSE);
	if (pWnd != NULL)
	{
		pWnd->EnableWindow(bEnable);
	}
	//gif
	if(bEnable)
	{	
		m_gif.Stop();
	}
	else
		m_gif.Draw();
}

/*************************************************************
 函 数 名：NotifySelectView()
 功能概要：通知设备树选择视图让控件禁用/可用
 返 回 值: void
 参    数：param1	TRUE-控件可用. FALSE-控件不可用
		   Param2
**************************************************************/
//##ModelId=49B87BB2036B
void CBatchDetailSettingView::NotifySelectView( BOOL bEnable )
{
	//发送消息给父窗口父窗口(因为父窗口是分隔窗口),让其转发给设备树视图
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam参数不写, lParam参数写可用/不可用
		pWnd->PostMessage(BATCH_ENABLE_CONTROLS, (WPARAM)0, (LPARAM)bEnable);
	}
}

/*************************************************************
 函 数 名：DeleteSelData()
 功能概要：清除自动总召配置中的数据
 返 回 值: 清除成功返回TRUE, 失败返回FALSE
 参    数：param1	0-删除自动总召的数据. 1-删除批量召唤的数据
		   Param2
**************************************************************/
//##ModelId=49B87BB2038A
BOOL CBatchDetailSettingView::DeleteSelData( int nIs_OC )
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置条件
	
	CString str;
	Condition con2;
	if(m_pActiveStation != NULL)
	{
		str.Format("STATION_ID='%s' AND IS_AUTO=%d", m_pActiveStation->m_sID, nIs_OC);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}
	else
	{
		str.Format("IS_AUTO=%d", nIs_OC);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_OC_CONFIG_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBatchDetailSettingView::DeleteSelData, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/*************************************************************
 函 数 名：NotifyOC()
 功能概要：通知自动总召模块进行操作
 返 回 值: 通知成功返回TRUE, 失败返回FALSE
 参    数：param1	0-召唤. 1-取消召唤
		   Param2
**************************************************************/
//##ModelId=49B87BB20399
BOOL CBatchDetailSettingView::NotifyOC( int nAction )
{

	//发送报文
	if(nAction == 0)
	{
		//召唤
		if(!NotifyOCCall())
		{
			//发送召唤通知失败
			WriteLog("CBatchDetailSettingView::NotifyOC, 发送召唤通知失败");
			return FALSE;
		}
		//等待回应
	}
	else
	{
		//取消召唤
		if(!NotifyOCCancel())
		{
			//发送取消召唤报文失败
			WriteLog("CBatchDetailSettingView::NotifyOC, 发送取消召唤报文失败");
		}
	}
	return TRUE;
}


BOOL CBatchDetailSettingView::MultiSaveData(int nIS_OC, POSITION pos)
{

	//循环所有选择项
	STTP_FULL_DATA sttpData;
	//清空结构
	zero_sttp_full_data(sttpData);
	
	CString strTemp = _T("");
	CBatchSel* pSel = NULL;
	int iCount = 0;
	while(pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		MYASSERT_CONTINUE(pSel);
		if (!pSel->m_bChecked)
		{
			continue;
		}
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pSel->MultiSaveData(sttpDB, nIS_OC))
		{
			return FALSE;
		}
		else
		{
			++iCount;
		}
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
		if (iCount == 200)
		{
			MultiSaveData(nIS_OC, pos);
			break;
		}
	}

	STTP_DBDATA_RECORD sttpDB;
	sttpDB.condition_list.clear();
	sttpDB.field_list.clear();

	//厂站ID
	STTP_DBDATA_UNIT field;
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	field.strValue = "Station_ID";
	sttpDB.field_list.push_back(field);
	
	//保护ID
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	field.strValue = "PT_ID";
	sttpDB.field_list.push_back(field);
	
	//IsCallSetting
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "IsCallSetting";
	sttpDB.field_list.push_back(field);
	
	//IsCallDi
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "IsCallDi";
	sttpDB.field_list.push_back(field);
	
	//IsCallSoftboard
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "IsCallSoftboard";
	sttpDB.field_list.push_back(field);

	//IsCallAi
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "IsCallAi";
	sttpDB.field_list.push_back(field);

	//Is_Auto
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "Is_Auto";
	sttpDB.field_list.push_back(field);

	//cpu_code
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "cpu_code";
	sttpDB.field_list.push_back(field);

	//Reverse1 //是否召唤定值区号
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	field.strValue = "Reverse1";
	sttpDB.field_list.push_back(field);

	//Reverse2 //是否召唤历史事件
	field.nDataType = EX_STTP_DATA_TYPE_FLOAT;
	field.strValue = "Reverse2";
	sttpDB.field_list.push_back(field);

	//Reverse3 //是否召唤录波
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	field.strValue = "Reverse3";
	sttpDB.field_list.push_back(field);

	sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	BOOL bReturn = TRUE;
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertMutiData(EX_STTP_INFO_OC_CONFIG_CFG, sttpData, sError);
	}
	catch (...)
	{
		WriteLog("CBatchSel::test, 保存失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
	{
		bReturn = FALSE;
	}

	zero_sttp_full_data(sttpData);
	
	delete[] sError;

	CString strLog;
	if(bReturn)
	{
		strLog.Format("CFaultReport::MultiSaveEvent, 保存%d条选项记录成功", iCount);
		WriteLog(strLog);
	}
	else
	{
		WriteLog("CFaultReport::MultiSaveEvent, 保存失败");
	}
	return bReturn;
}

//##ModelId=49B87BB302AF
BOOL CBatchDetailSettingView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CViewBase::OnEraseBkgnd(pDC);
}


//##ModelId=49B87BB302BF
void CBatchDetailSettingView::OnBtnBatchSave() 
{
	// TODO: Add your control notification handler code here
	if(!HasSelected())
	{
		AfxMessageBox(StringFromID(IDS_AUTOCALL_NOSELECTION));
		WriteLog(_T("存库记录数为0条")); 
		return;
	}
	//选删除, 再保存
	if(DeleteSelData(0))
	{
		POSITION pos = m_mapSave.GetStartPosition();
		if(!MultiSaveData(0, pos))
		{
			AfxMessageBox(StringFromID(IDS_SAVECONFIG_FAILED));
			WriteLog("CBatchDetailSettingView::OnBtnBatchSave, 保存失败\n");
			return;
		}
		AfxMessageBox(StringFromID(IDS_SAVECONFIG_SUCCESS));
		WriteLog("CBatchDetailSettingView::OnBtnBatchSave, 保存成功\n");
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_SAVECONFIG_FAILED));
		WriteLog("CBatchDetailSettingView::OnBtnBatchSave, 删除旧数据失败\n");
		return;
	}
}

//##ModelId=49B87BB203A9
BOOL CBatchDetailSettingView::NotifyOCCall()
{
	//组建报文
	STTP_FULL_DATA sttpData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 1))
		return FALSE; //组建报文失败
	//发送报文
	int nResult = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	if(nResult != 1)
		return FALSE; //发送失败
	m_nTimer = SetTimer(m_nTimer, 1000*60, NULL);
	return TRUE;
}

/*************************************************************
 函 数 名：NotifyOCCancel()
 功能概要：通知自动总召模块进行取消召唤操作
 返 回 值: 通知成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB203B9
BOOL CBatchDetailSettingView::NotifyOCCancel()
{
	//组建报文
	STTP_FULL_DATA sttpData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(!pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20153, "", 0))
		return FALSE; //组建报文失败
	//发送报文
	int nResult = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	if(nResult != 1)
		return FALSE; //发送失败
	return TRUE;
}

/*************************************************************
 函 数 名：ReceiveAutoCall()
 功能概要：等待自动总召的回应
 返 回 值: 收到回应且可召唤返回TRUE, 未收到回应或现在不可召唤返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB203D9
BOOL CBatchDetailSettingView::ReceiveAutoCall()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	STTP_FULL_DATA sttpData;
	zero_sttp_full_data(sttpData);
	pApp->m_SttpEngine.XJSTTPSetOption(m_pAutoCall, 1, 30000, 0);
	int nResult = pApp->m_SttpEngine.XJSTTPRead(m_pAutoCall, sttpData);
	if(nResult != 1)
	{
		return FALSE;
	}
	//判断报文头
	int nHead = sttpData.sttp_head.uMsgID;
	if(nHead == 901)
		return TRUE;
	else 
		return FALSE;
	return FALSE;
}

//##ModelId=49B87BB302DE
void CBatchDetailSettingView::OnBtnBatchSet() 
{
	// TODO: Add your control notification handler code here

	//查库, 得到之前的设置
	CString str= "";
	GetAutoCallConfig("OC_STARTTIME", m_sStartTime); //总召启动时间
	GetAutoCallConfig("OC_CYC", m_sCyc); //总召周期
	CDlgOCSet dlg;
	dlg.pView = this;
	dlg.DoModal();
}

/*************************************************************
 函 数 名：OnAutoCallSetChange()
 功能概要：收到自动总召周期设置变更消息, 写入数据库, 并通知自动总召模块
 返 回 值: void
 参    数：param1	第一次启动时间
		   Param2	周期
**************************************************************/
//##ModelId=49B87BB3037B
void CBatchDetailSettingView::OnAutoCallSetChange( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format("自动总召周期变更: 第一次启动时间:%s, 周期:%s小时", m_sStartTime, m_sCyc);
	WriteLog(str);

	//存库
	//启动时间
	if(!SaveAutoCallSet("OC_STARTTIME", m_sStartTime))
	{
		//保存失败
		WriteLog("CBatchDetailSettingView::OnAutoCallSetChange, 保存到数据库失败");
		AfxMessageBox(StringFromID(IDS_SAVECONFIG_FAILED));
		return;
	}
	WriteLog("保存启动时间设置到数据库成功");
	if(!SaveAutoCallSet("OC_CYC", m_sCyc))
	{
		//保存失败
		WriteLog("CBatchDetailSettingView::OnAutoCallSetChange, 保存到数据库失败");
		AfxMessageBox(StringFromID(IDS_SAVECONFIG_FAILED));
		return;
	}
	WriteLog("保存总召周期设置到数据库成功");


	//组建报文
	STTP_FULL_DATA sttpData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 2))
		return; //组建报文失败
	//发送报文
	int nResult = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	if(nResult != 1)
	{
		WriteLog("通知自动总召模块失败, 此次设置将在自动总召模块下一次启动时生效", XJ_LOG_LV1);
		AfxMessageBox(StringFromID(IDS_AUTOCALL_NOTIFY_FAILED));
		return; //发送失败
	}

	//保存设置成功
	WriteLog("保存设置成功, 通知自动总召模块成功", XJ_LOG_LV2);
	AfxMessageBox(StringFromID(IDS_SAVECONFIG_SUCCESS));
}

/*************************************************************
 函 数 名：SetCyc()
 功能概要：设置自动总召周期
 返 回 值: void
 参    数：param1	第一次启动时间
		   Param2	周期
**************************************************************/
//##ModelId=49B87BB20138
void CBatchDetailSettingView::SetCyc( CString sStartTime, CString sCyc )
{
	m_sStartTime = sStartTime;
	m_sCyc = sCyc;
}

/*************************************************************
 函 数 名：SaveAutoCallSet()
 功能概要：保存自动总召设置到系统配置表
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数：param1	关键字
		   Param2	值
**************************************************************/
//##ModelId=49B87BB30010
BOOL CBatchDetailSettingView::SaveAutoCallSet( CString sKey, CString sValue )
{
	BOOL bReturn;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//指定KeyName,更新Value
	
	CString str;	
	//字段
	//Value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING, sValue);
	
	//条件
	//KeyName
	Condition condition2;
	str.Format("KeyName = '%s'", sKey);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CBatchDetailSettingView::SaveAutoCallSet, 更新失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("CBatchDetailSettingView::SaveAutoCallSet,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：GetAutoCallConfig()
 功能概要：在系统配置表中查询自动总召配置
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数：param1	关键字
		   Param2	值
**************************************************************/
//##ModelId=49B87BB3001F
BOOL CBatchDetailSettingView::GetAutoCallConfig( CString sKey, CString& sValue )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件:KeyName,查询Value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);
	
	Condition condition1;
	CString str;
	str.Format("KEYNAME='%s'", sKey);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CAutoCallResult::GetOCTime, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			sValue = pMemset->GetValue(UINT(0));
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::GetOCTime, 查询失败,原因为%s", sError);
		WriteLog(str);
		return FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return TRUE;
}

/*************************************************************
 函 数 名: LoadDataFromDB()
 功能概要: 从数据库读取数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CBatchDetailSettingView::LoadDataFromDB()
{

	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
		
	//条件
	//只查询总召唤配置
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("Is_Auto = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_CONFIG_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CBatchDetailSettingView::LoadDataFromDB, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CBatchDetailSettingView::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CBatchSel * pEvent = new CBatchSel;
			if(pEvent->LoadFromDB(pMemset))
			{
				CheckCallSetting(pEvent);
				AddBatchSel(pEvent);
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
		
		OnBtnBatchSettingReset();
	}
	else
	{
		CString str;
		str.Format("CBatchDetailSettingView::LoadDataFromDB,查询失败,原因为%s", sError);
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
 函 数 名: OnAutoCallStationChange()
 功能概要: 响应可操作厂站改变消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::OnAutoCallStationChange( WPARAM wParam, LPARAM lParam )
{
	m_pActiveStation = (CStationObj*)lParam;
	//重新载入数据
	LoadDataFromDB();
}

void CBatchDetailSettingView::InsertNet()
{
	//m_List.DeleteAllItems();
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有地区
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		//取得地区
		CNetObj* pObj = (CNetObj*)pData->m_NetList.GetNext(pos);
		BatchSel* pBatchSel = FindNetBatchSel(pObj->m_sID);
		InsertItem(pObj->m_sID, pBatchSel);
	}
}

void CBatchDetailSettingView::InsertPriDevice(WPARAM wParam, LPARAM lParam)
{
	//m_List.DeleteAllItems();
	CStationObj* pStation = (CStationObj*)wParam;
	if ((DWORD)pStation < 100)
	{
		return;
	}
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;

	DEVICE_LIST listDevice;

	POSITION pos = NULL;
	BatchSel* pBatchSel = NULL;
	switch(lParam)
	{
	case TYPE_BUS:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
            pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
		break;
	case TYPE_LINE:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_LINE, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
		break;
	case TYPE_BREAK:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_BREAK, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	    break;
	case TYPE_GEN:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_GEN, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	    break;
	case TYPE_REACTOR:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_REACTOR, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
		break;
	case TYPE_TRANS:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_TRANS, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
		break;
	case TYPE_SWITCH:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_SWITCH, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	    break;
	case TYPE_CAP:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_CAP, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	    break;
	case TYPE_PMS:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_PMS, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	    break;
	case TYPE_BAY:
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_BAY, pStation->m_sID);
	    pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
			pBatchSel = FindPriDeviceBatchSel(pStation->m_sNetID, pObj->m_sStationID, pObj->m_nType, pObj->m_sName);
			if (pObj->m_bHasSecDevice)
			{
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
		break;
	}
}

void CBatchDetailSettingView::InsertDeviceType(WPARAM wParam, LPARAM lParam)
{
	CStationObj* pStationObj = (CStationObj*)lParam;
	BatchSel* pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_BUS);
	if (pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_BUS), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_LINE);
	if (pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_LINE), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_BREAK);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_BREAKER), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_GEN);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_GEN), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_REACTOR);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_REACTOR), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_TRANS);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_TRANS), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_SWITCH);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_SWITCH), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_CAP);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_CAP), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_PMS);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_PMS), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_BAY);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_BAY), pBatchSel);
	}
	pBatchSel = FindDeviceTypeBatchSel(pStationObj->m_sNetID, pStationObj->m_sID, TYPE_WAVEREC);
	if(pBatchSel != NULL)
	{
		InsertItem(StringFromID(IDS_MODEL_RECORDER), pBatchSel);
	}
}

void CBatchDetailSettingView::InsertStation(WPARAM wParam, LPARAM lParam)
{
	CNetObj* pNet = (CNetObj*)lParam;
	if ((DWORD)pNet < 100)
	{
		return;
	}
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listStation;
	listStation.RemoveAll();
	pData->GetStationList(listStation, pNet->m_sID);
	POSITION pos = listStation.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj*  pObj = (CStationObj*)listStation.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
		{
			if (pObj->m_sNetID == pNet->m_sID)
			{
				BatchSel* pBatchSel = FindStationBatchSel(pObj->m_sNetID, pObj->m_sID);
				InsertItem(pObj->m_sName, pBatchSel);
			}
		}
	}
}

void CBatchDetailSettingView::InsertWaverc(WPARAM wParam, LPARAM lParam)
{
	CStationObj* pStation = (CStationObj*)wParam;
	if ((DWORD)pStation < 100)
	{
		return;
	}
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj*  pSecObj = (CSecObj*)listDevice.GetNext(pos);
		CBatchSel* pBatchSel = NULL;
		BatchSel* pSel = FindSecDeviceBatchSel(pSecObj->m_pStation->m_sNetID, pSecObj->m_sStationID, TYPE_WAVEREC, "", pSecObj->m_sID);
		for (int i = 0; i < pSecObj->m_arrCPU.GetSize(); i++)
		{
			pBatchSel = FindSecCPUBatchSel(pSecObj->m_arrCPU.GetAt(i)->code, pSel);
			InsertItem(pSecObj->m_sName, pBatchSel, pSecObj->m_arrCPU.GetAt(i)->code);
		}
	}
}

void CBatchDetailSettingView::InsertCPU(WPARAM wParam, LPARAM lParam)
{
	CSecObj* pSecObj = (CSecObj*)lParam;
	if ((DWORD)pSecObj < 100)
	{
		return;
	}

	CString str = _T("");
	int nType = 0;
	CString strPriDeviceName = _T("");
	if (pSecObj->m_nType == TYPE_WAVEREC)
	{
		nType = TYPE_WAVEREC;
	}
	else
	{
		nType = pSecObj->m_pOwner->m_nType;
		strPriDeviceName = pSecObj->m_pOwner->m_sName;
	}

	for(int i = 0; i < pSecObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU对象
		CSecCPU * pCpu = (CSecCPU*)pSecObj->m_arrCPU.GetAt(i);
        str.Format("%d(%s)", pCpu->code, pCpu->des);
		CBatchSel* pBatchSel = FindSecCPUBatchSel(pSecObj->m_pStation->m_sNetID, pSecObj->m_sStationID, nType, strPriDeviceName, pSecObj->m_sID, pCpu->code);
		InsertItem(str, pBatchSel, pCpu->code);
	}
}

void CBatchDetailSettingView::InsertSecDevice(WPARAM wParam, LPARAM lParam)
{
	CDeviceObj* pDevice = (CDeviceObj*)lParam;
	if ((DWORD)pDevice < 100)
	{
		return;
	}

	CStationObj* pStation = (CStationObj*)wParam;
	if ((DWORD)pStation < 100)
	{
		return;
	}

	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, pDevice->m_sID, TYPE_PROTECT, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while (pos != NULL)
	{	
		CSecObj* pSecObj = (CSecObj*)listDevice.GetNext(pos);
		if (pSecObj->m_pOwner != NULL && pSecObj->m_pOwner == pDevice)
		{
			int nType = 0;
			CString strPriDeviceName = _T("");
			if (pSecObj->m_nType == TYPE_WAVEREC)
			{
				nType = TYPE_WAVEREC;
			}
			else
			{
				nType = pSecObj->m_pOwner->m_nType;
				strPriDeviceName = pSecObj->m_pOwner->m_sName;
			}

			CBatchSel* pBatchSel = NULL;
			BatchSel* pSel = FindSecDeviceBatchSel(pStation->m_sNetID, pStation->m_sID, nType, strPriDeviceName, pSecObj->m_sID);
			for (int i = 0; i < pSecObj->m_arrCPU.GetSize(); i++)
			{
			    pBatchSel = FindSecCPUBatchSel(pSecObj->m_arrCPU.GetAt(i)->code, pSel);
				InsertItem(pSecObj->m_sName, pBatchSel, pSecObj->m_arrCPU.GetAt(i)->code);
			}
		}
	}
}


void CBatchDetailSettingView::InsertSingleCPU(WPARAM wParam, LPARAM lParam)
{
	CSecObj* pSecObj = (CSecObj*)wParam;
	if ((DWORD)pSecObj < 100)
	{
		return;
	}
	CSecCPU* pCpu = (CSecCPU*)lParam;
	if ((DWORD)pCpu < 100)
	{
		return;
	}
	CString str = _T("");
    str.Format("%d(%s)", pCpu->code, pCpu->des);

	int nType = 0;
	CString strPriDeviceName = _T("");
	if (pSecObj->m_nType == TYPE_WAVEREC)
	{
		nType = TYPE_WAVEREC;
	}
	else
	{
		nType = pSecObj->m_pOwner->m_nType;
		strPriDeviceName = pSecObj->m_pOwner->m_sName;
	}
	CBatchSel* pBatchSel = FindSecCPUBatchSel(pSecObj->m_pStation->m_sNetID, pSecObj->m_sStationID, nType, strPriDeviceName, pSecObj->m_sID, pCpu->code);
	InsertItem(str, pBatchSel, pCpu->code);
}

void CBatchDetailSettingView::InsertItem(CString strName, CSel* pBatchSel, int iCPU)
{

	if(!pBatchSel)
		return;
	const int IDX = m_List.InsertItem(0, _T(""));
	m_List.SetItemText(IDX, 0, strName);
	//1: 定值区号
	m_List.SetItemText(IDX, 1, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallZone));
    //2: 定值
	m_List.SetItemText(IDX, 2, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallSetting));
	//3: 开关量
	m_List.SetItemText(IDX, 3, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallDigital));
	//4: 软压板
	m_List.SetItemText(IDX, 4, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallSoftBoard));
	//5: 模拟量
	m_List.SetItemText(IDX, 5, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallAnalog));
	//6:录波
	m_List.SetItemText(IDX, 6, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallOsc));
	//7:历史事件
	m_List.SetItemText(IDX, 7, pBatchSel == NULL?StringFromID(IDS_CALLOPER_CALL):pBatchSel->GetCallString(pBatchSel->m_nCallHistory));

	if (iCPU != -1 && m_List.GetColumnCount() == 9)
	{
		CString strTemp;
		strTemp.Format("%d", iCPU);
		m_List.SetItemText(IDX, 8, strTemp);
	}
	
	if (pBatchSel != NULL && pBatchSel->m_bChecked)
	{
		m_List.SetCheck(IDX, TRUE);
	}
	else
	{
		m_List.SetCheck(IDX, FALSE);
	}
	m_List.SetItemData(IDX, (DWORD)pBatchSel);

}

void CBatchDetailSettingView::OnClickListBatchDetailSetting(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	if (nItem > -1)
	{
		UpdateBatchSetting(nItem, nSubItem);
	}

	*pResult = 0;
}

void CBatchDetailSettingView::UpdateBatchSetting(int nItem, int nSubItem)
{
	if( nSubItem >= 1 && nSubItem <=7 )
	{
		CString str = m_List.GetItemText(nItem, nSubItem);
		CSel* pSel = (CSel*)m_List.GetItemData(nItem);

		//------------------------------------------------------------------------------------------
		if(str == StringFromID(IDS_CALLOPER_NOTCALL))
		{
			str = StringFromID(IDS_CALLOPER_CALL);
			SetBatchSels(pSel, nSubItem, 1);
		}
		else if(str == StringFromID(IDS_CALLOPER_CALL))
		{
			str = StringFromID(IDS_CALLOPER_NOTCALL);
			SetBatchSels(pSel, nSubItem, 0);
		}
		m_List.SetItemText(nItem, nSubItem, str);
	}
}

void CBatchDetailSettingView::SetBatchSel(int nSubItem, CSel* pSel, int nValue)
{
	MYASSERT(pSel);
	switch (nSubItem)
	{
	case 1:
		if (pSel->m_nCallZone != 2 && pSel->m_nCallZone != CSel::SUSPEND)
		{
			pSel->m_nCallZone = nValue;
		}
		break;
	case 2:
		if (pSel->m_nCallSetting != 2 && pSel->m_nCallSetting != CSel::SUSPEND)
		{
			pSel->m_nCallSetting = nValue;
		}
		break;
	case 3:
		if (pSel->m_nCallDigital != 2 && pSel->m_nCallDigital != CSel::SUSPEND)
		{
			pSel->m_nCallDigital = nValue;
		}
		break;
	case 4:
		if (pSel->m_nCallSoftBoard != 2 && pSel->m_nCallSoftBoard != CSel::SUSPEND)
		{
			pSel->m_nCallSoftBoard = nValue;
		}
		break;
	case 5:
		if (pSel->m_nCallAnalog != 2 && pSel->m_nCallAnalog != CSel::SUSPEND)
		{
			pSel->m_nCallAnalog = nValue;
		}
		break;
	case 6:
		if( pSel->m_nCallOsc != CSel::UNSUPPORT && pSel->m_nCallOsc != CSel::SUSPEND )
		{
			pSel->m_nCallOsc = nValue;
		}
		break;
	case 7:
		if( pSel->m_nCallHistory != CSel::UNSUPPORT && pSel->m_nCallHistory != CSel::SUSPEND )
		{
			pSel->m_nCallHistory = nValue;
		}
		break;
	}
}

void CBatchDetailSettingView::SetBatchSels(CSel* pSel, int nSubItem, int nValue)
{
	MYASSERT(pSel);
    CString strTemp = _T("");
	WORD wTemp = 0;
	if (pSel->pMap == NULL)
	{
		SetBatchSel(nSubItem, pSel, nValue);
		CBatchSel* pBatch = (CBatchSel*)pSel;
	}
	else if (pSel->pMap->IsKindOf(RUNTIME_CLASS(CMapStringToPtr)))
	{
		SetBatchSel(nSubItem, pSel, nValue);
		STRING_MAP* pMap = (STRING_MAP*)pSel->pMap;
		POSITION pos = pMap->GetStartPosition();
		while(pos != NULL)
		{
			pMap->GetNextAssoc(pos, strTemp, (void*&)pSel);
			SetBatchSels(pSel, nSubItem, nValue);
		}
	}
	else if (pSel->pMap->IsKindOf(RUNTIME_CLASS(CMapWordToPtr)))
	{
		SetBatchSel(nSubItem, pSel, nValue);
		WORD_MAP* pMap = (WORD_MAP*)pSel->pMap;
		POSITION pos = pMap->GetStartPosition();
		while(pos != NULL)
		{
			pMap->GetNextAssoc(pos, wTemp, (void*&)pSel);
			SetBatchSels(pSel, nSubItem, nValue);
		}
	}
	else if (pSel->pMap->IsKindOf(RUNTIME_CLASS(CPtrList)))
	{	
		SetBatchSel(nSubItem, pSel, nValue);
		BATCH_LIST* pList = (BATCH_LIST*)pSel->pMap;
		POSITION pos = pList->GetHeadPosition();
		CBatchSel* pBatch = NULL;
		while(pos != NULL)
		{
			pBatch = (CBatchSel*)pList->GetNext(pos);
			SetBatchSel(nSubItem, pBatch, nValue);
		}
	}
}

/*************************************************************
 函 数 名: AddBatchSel()
 功能概要: 插入选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::AddBatchSel(CBatchSel *pSel)
{
	InsertNetBatchSel(pSel);
}

/*************************************************************
 函 数 名: InsertNetBatchSel()
 功能概要: 查找地区选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertNetBatchSel(CBatchSel *pSel)
{
	MYASSERT(pSel);
	MYASSERT(pSel->m_pObj);
	MYASSERT(pSel->m_pObj->m_pStation);
    BatchSel* pBatch = FindNetBatchSel(pSel->m_pObj->m_pStation->m_sNetID);
	if ( pBatch  == NULL)
	{
		pBatch = new BatchSel();
		NET_MAP* pNetMap = new NET_MAP;
		pBatch->pMap = pNetMap;
		m_mapSel.SetAt(pSel->m_pObj->m_pStation->m_sNetID, pBatch);
	}
	
	InsertStationBatchSel(pSel, (NET_MAP*)pBatch->pMap);
}

/*************************************************************
 函 数 名: InsertStationBatchSel()
 功能概要: 插入厂站选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertStationBatchSel(CBatchSel *pSel, NET_MAP* pNetMap)
{
	MYASSERT(pSel);
    MYASSERT(pNetMap);
    MYASSERT(pSel->m_pObj);
	BatchSel* pBatch = FindStationBatchSel(pSel->m_pObj->m_sStationID,pNetMap);
	if (pBatch == NULL)
	{
		pBatch = new BatchSel();
		STATION_MAP* pStationMap = new STATION_MAP;
		pBatch->pMap = pStationMap;
		pNetMap->SetAt(pSel->m_pObj->m_sStationID, pBatch);
	}
	
	InsertDeviceTypeBatchSel(pSel, (STATION_MAP*)pBatch->pMap);
}

/*************************************************************
 函 数 名: InsertDeviceTypeBatchSel()
 功能概要: 插入设备类型选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertDeviceTypeBatchSel(CBatchSel *pSel, STATION_MAP* pStationMap)
{
	MYASSERT(pSel);
    MYASSERT(pStationMap);
    MYASSERT(pSel->m_pObj);
	BatchSel* pBatch = NULL;
	if (pSel->m_pObj->m_nType == TYPE_WAVEREC)
	{
		pBatch = FindDeviceTypeBatchSel(pSel->m_pObj->m_nType, pStationMap);
	}
	else
	{
		if(!pSel->m_pObj->m_pOwner)
		{
			delete pSel;
			pSel = NULL;
			return;
		}
        MYASSERT(pSel->m_pObj->m_pOwner);
		pBatch = FindDeviceTypeBatchSel(pSel->m_pObj->m_pOwner->m_nType, pStationMap);
	}
	
	if (pBatch == NULL)
	{
		pBatch = new BatchSel();
		
		if (pSel->m_pObj->m_nType == TYPE_WAVEREC)
		{
			PRIDEVICE_MAP* pPriDeviceMap = new PRIDEVICE_MAP;
			pBatch->pMap = pPriDeviceMap;
			pStationMap->SetAt(pSel->m_pObj->m_nType, pBatch);
		}
		else
		{
			DEVICETYPE_MAP* pDeviceTypeMap = new DEVICETYPE_MAP;
			pBatch->pMap = pDeviceTypeMap;
            pStationMap->SetAt(pSel->m_pObj->m_pOwner->m_nType, pBatch);
		}
	}

	if (pSel->m_pObj->m_nType == TYPE_WAVEREC)
	{
		InsertSecDeviceBatchSel(pSel, (PRIDEVICE_MAP*)pBatch->pMap);
	}
	else
	{
		InsertPriDeviceBatchSel(pSel, (DEVICETYPE_MAP*)pBatch->pMap);
	}
}

/*************************************************************
 函 数 名: InsertPriDeviceBatchSel()
 功能概要: 插入一次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertPriDeviceBatchSel(CBatchSel *pSel, DEVICETYPE_MAP* pDeviceTypeMap)
{
	MYASSERT(pSel);
	MYASSERT(pDeviceTypeMap);
	MYASSERT(pSel->m_pObj);
	MYASSERT(pSel->m_pObj->m_pOwner);
	BatchSel* pBatch = FindPriDeviceBatchSel(pSel->m_pObj->m_pOwner->m_sName, pDeviceTypeMap);
	if (pBatch == NULL)
	{
		pBatch = new BatchSel();
		PRIDEVICE_MAP* pPriDeviceMap = new PRIDEVICE_MAP;
		pBatch->pMap = pPriDeviceMap;
		pDeviceTypeMap->SetAt(pSel->m_pObj->m_pOwner->m_sName, pBatch);
	}
	InsertSecDeviceBatchSel(pSel, (PRIDEVICE_MAP*)pBatch->pMap);
}

/*************************************************************
 函 数 名: InsertSecDeviceBatchSel()
 功能概要: 插入二次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertSecDeviceBatchSel(CBatchSel *pSel, PRIDEVICE_MAP* pPriDeviceMap)
{
	MYASSERT(pSel);
    MYASSERT(pSel->m_pObj);
	BatchSel* pBatchSel = FindSecDeviceBatchSel(pSel->m_pObj->m_sID, pPriDeviceMap);
	if (pBatchSel == NULL)
	{
		pBatchSel = new BatchSel();
        BATCH_LIST* pList = new BATCH_LIST;
		pBatchSel->pMap = pList;
		pPriDeviceMap->SetAt(pSel->m_pObj->m_sID, pBatchSel);
	}
	InsertSecCPUBatchSel(pSel, (BATCH_LIST*)pBatchSel->pMap);
}

/*************************************************************
 函 数 名: InsertSecCPUBatchSel()
 功能概要: 插入CPU 选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailSettingView::InsertSecCPUBatchSel(CBatchSel* pSel, BATCH_LIST* pList)
{
	MYASSERT(pSel);
    MYASSERT(pList);
	MYASSERT(pSel->m_pObj);
	MYASSERT(pSel->m_pCpu);
    pList->AddHead(pSel);
	CString strKey;
	strKey.Format(pSel->m_pObj->m_sID + "cpu:%d", pSel->m_pCpu->code);
    m_mapSave.SetAt(strKey, pSel);
}

/*************************************************************
 函 数 名: FindNetBatchSel()
 功能概要: 查找地区选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindNetBatchSel(CString strNetName)
{
	BatchSel* pBatchSel = NULL;
	m_mapSel.Lookup(strNetName, (void*&)pBatchSel);
	return pBatchSel;
}

/*************************************************************
 函 数 名: FindStationBatchSel（)
 功能概要: 查找厂站选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindStationBatchSel(CString strStationID, NET_MAP* pNetMap)
{
	BatchSel* pBatchSel = NULL;
	pNetMap->Lookup(strStationID, (void*&)pBatchSel);
	return pBatchSel;
}

/*************************************************************
 函 数 名: FindStationBatchSel（)
 功能概要: 查找厂站选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindStationBatchSel(CString strNetName, CString strStationID)
{
	BatchSel* pBatchSel = FindNetBatchSel(strNetName);
	if (pBatchSel == NULL)
	{
		return NULL;
	}
	else
	{
		pBatchSel = FindStationBatchSel(strStationID, (NET_MAP*)pBatchSel->pMap);
		return pBatchSel;
	}
}

/*************************************************************
 函 数 名: FindDeviceTypeBatchSel()
 功能概要: 查找设备类型选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindDeviceTypeBatchSel(int iDeviceType, STATION_MAP* pStationMap)
{
	BatchSel* pBatchSel = NULL;
	pStationMap->Lookup(iDeviceType, (void*&)pBatchSel);
	return pBatchSel;
}

/*************************************************************
 函 数 名: FindDeviceTypeBatchSel()
 功能概要: 查找设备类型选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindDeviceTypeBatchSel(CString strNetName, CString strStationID, int iDeviceType)
{
	BatchSel* pBatchSel = FindStationBatchSel(strNetName, strStationID);
	if (pBatchSel == NULL)
	{
		return NULL;
	}
	else
	{
		pBatchSel = FindDeviceTypeBatchSel(iDeviceType, (STATION_MAP*)pBatchSel->pMap);
		return pBatchSel;
	}
}

/*************************************************************
 函 数 名: FindPriDeviceBatchSel()
 功能概要: 查找一次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindPriDeviceBatchSel(CString strPriDeviceName, DEVICETYPE_MAP* pDeviceTypeMap)
{
	
	BatchSel* pBatchSel = NULL;
	pDeviceTypeMap->Lookup(strPriDeviceName, (void*&)pBatchSel);
	return pBatchSel;
}

/*************************************************************
 函 数 名: FindPriDeviceBatchSel()
 功能概要: 查找一次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindPriDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName)
{
	BatchSel* pBatchSel = FindDeviceTypeBatchSel(strNetName, strStationID, iDeviceType);
	if (pBatchSel == NULL)
	{
		return NULL;
	}
	else
	{
		pBatchSel = FindPriDeviceBatchSel(strPriDeviceName, (DEVICETYPE_MAP*)pBatchSel->pMap);
		return pBatchSel;
	}
}

/*************************************************************
 函 数 名: FindSecDeviceBatchSel()
 功能概要: 查找二次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindSecDeviceBatchSel(CString strSecDeviceName, PRIDEVICE_MAP* pPriDeviceMap)
{
	BatchSel* pBatchSel = NULL;
	pPriDeviceMap->Lookup(strSecDeviceName, (void*&)pBatchSel);
	return pBatchSel;
}

/*************************************************************
 函 数 名: FindSecDeviceBatchSel()
 功能概要: 查找二次设备选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BatchSel* CBatchDetailSettingView::FindSecDeviceBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName)
{
	BatchSel* pBatchSel = NULL;
	if (iDeviceType == TYPE_WAVEREC)
	{
		pBatchSel = FindDeviceTypeBatchSel(strNetName, strStationID, iDeviceType);
	}
	else
	{
		pBatchSel = FindPriDeviceBatchSel(strNetName, strStationID, iDeviceType, strPriDeviceName);
	}

	if (pBatchSel == NULL)
	{
		return NULL;
	}
	else
	{
	    pBatchSel = FindSecDeviceBatchSel(strSecDeviceName, (PRIDEVICE_MAP*)pBatchSel->pMap);
		return pBatchSel;
	}
}


/*************************************************************
 函 数 名: FindSecCPUBatchSel()
 功能概要: 查找CPU 选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
CBatchSel* CBatchDetailSettingView::FindSecCPUBatchSel(int iCPU, BatchSel* pBatchSel)
{
	 MYASSERT_NULL(pBatchSel);
	 BATCH_LIST* pList = (BATCH_LIST*)pBatchSel->pMap;
	 CBatchSel* pSel = NULL;
	 POSITION pos = pList->GetHeadPosition();
	 while (pos != NULL)
	 {
		 pSel = (CBatchSel*)pList->GetNext(pos);
		 if (pSel->m_pCpu->code == iCPU)
		 {
			 break;
		 }
	 }
	 
	 return pSel;
}

/*************************************************************
 函 数 名: FindSecCPUBatchSel()
 功能概要: 查找CPU 选择项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
CBatchSel* CBatchDetailSettingView::FindSecCPUBatchSel(CString strNetName, CString strStationID, int iDeviceType, CString strPriDeviceName, CString strSecDeviceName, int iCPU)
{
	BatchSel* pBatchSel = FindSecDeviceBatchSel(strNetName, strStationID, iDeviceType, strPriDeviceName, strSecDeviceName);
	if (pBatchSel == NULL)
	{
		return NULL;
	}
	else
	{
		CBatchSel* pSel = FindSecCPUBatchSel(iCPU, pBatchSel);
		return pSel;
	}
}

void CBatchDetailSettingView::CheckCallSetting(CBatchSel *pBatchSel)
{
	MYASSERT(pBatchSel);
	MYASSERT(pBatchSel->m_pObj);
	if(pBatchSel->m_pObj->m_nType == TYPE_WAVEREC)
	{
		pBatchSel->m_nCallZone = CSel::UNSUPPORT;//不支持
		pBatchSel->m_nCallSoftBoard = CSel::UNSUPPORT;
		pBatchSel->m_nCallHistory = CSel::UNSUPPORT;
	}
	
	if(pBatchSel->m_pObj->m_nSuspendstatus == 1)
	{
		pBatchSel->m_nCallZone = CSel::SUSPEND;
		pBatchSel->m_nCallSetting = CSel::SUSPEND;
		pBatchSel->m_nCallDigital = CSel::SUSPEND;
		pBatchSel->m_nCallSoftBoard = CSel::SUSPEND;
		pBatchSel->m_nCallAnalog = CSel::SUSPEND;
		pBatchSel->m_nCallOsc = CSel::SUSPEND;
		pBatchSel->m_nCallHistory = CSel::SUSPEND;
		return;
	}

	MYASSERT(pBatchSel->m_pCpu);
	if(pBatchSel->m_pCpu->m_nSettingDefs <= 0)
	{
		//没有点表
		pBatchSel->m_nCallSetting = 2;//不支持
	}

	if(pBatchSel->m_pCpu->m_nDigitalDefs <= 0)
	{
		//没有点表
		pBatchSel->m_nCallDigital = 2;//不支持
	}
	
	if(pBatchSel->m_pCpu->m_nSoftBoardDefs <= 0)
	{
		//没有点表
		pBatchSel->m_nCallSoftBoard = 2;//不支持
	}

	if(pBatchSel->m_pCpu->m_nAnalogDefs <= 0)
	{
		//没有点表
		pBatchSel->m_nCallAnalog = 2;//不支持
	}
}

void CBatchDetailSettingView::ClearBatchSelArray(BATCH_LIST* pList)
{
	MYASSERT(pList);
	POSITION pos = pList->GetHeadPosition();
	CBatchSel* pBatchSel = NULL;
	while(pos != NULL)
	{
		pBatchSel = (CBatchSel*)pList->GetNext(pos);
		if (pBatchSel != NULL)
		{
			delete pBatchSel;
			pBatchSel = NULL;
		}
	}
	pList->RemoveAll();
}

void CBatchDetailSettingView::ClearBatchSel()
{
	POSITION pos = m_mapSel.GetStartPosition();
	CSel* pObj = NULL;
	CString strTemp = _T("");

	while (pos != NULL)
	{
		m_mapSel.GetNextAssoc(pos, strTemp, (void*&)pObj);
		if (pObj != NULL)
		{
			ClearBatchSelMap(pObj->pMap);
			MYDELETE(pObj);
		}
	}
	m_mapSel.RemoveAll();
}

void CBatchDetailSettingView::ClearBatchSelMap(CObject* pObj)
{
	MYASSERT(pObj);
	CSel* pTempObj = NULL;
	if (pObj->IsKindOf(RUNTIME_CLASS(CMapStringToPtr)))
	{
		STRING_MAP* pMap = (STRING_MAP*)pObj;
        CString strTemp = _T("");
		POSITION pos = pMap->GetStartPosition();
		while(pos != NULL)
		{
			pMap->GetNextAssoc(pos, strTemp, (void*&)pTempObj);
			if (pTempObj != NULL)
			{
				ClearBatchSelMap(pTempObj->pMap);
				MYDELETE(pTempObj);
			}
		}
		pMap->RemoveAll();
		MYDELETE(pMap);
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CMapWordToPtr)))
	{
		WORD_MAP* pMap = (WORD_MAP*)pObj;
		POSITION pos = pMap->GetStartPosition();
		WORD wTemp = 0;
		while(pos != NULL)
		{
			pMap->GetNextAssoc(pos, wTemp, (void*&)pTempObj);
			if (pTempObj != NULL)
			{
				ClearBatchSelMap(pTempObj->pMap);
				MYDELETE(pTempObj);
			}
		}
		pMap->RemoveAll();
		MYDELETE(pMap);
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CPtrList)))
	{
		ClearBatchSelArray((BATCH_LIST*)pObj);
		MYDELETE(pObj);
	}
}

void CBatchDetailSettingView::AddBatchSel(CSecObj* pSecObj)
{
	MYASSERT(pSecObj);
	CSecCPU* pCpu = NULL;
	CString strTemp = _T("");
	for (int i = 0; i < pSecObj->m_arrCPU.GetSize(); i++)
	{
		CBatchSel* pBatchSel = NULL;
		pCpu = pSecObj->m_arrCPU.GetAt(i);
		MYASSERT_CONTINUE(pCpu);
		strTemp.Format(pSecObj->m_sID + "cpu:%d", pCpu->code);
		m_mapSave.Lookup(strTemp, (void*&)pBatchSel);
		if (pBatchSel != NULL)
		{
			continue;
		}
		CBatchSel* pSel = new CBatchSel;
		pSel->m_pObj = pSecObj;
		pSel->m_pCpu = pCpu;
		CheckCallSetting(pSel);
		AddBatchSel(pSel);
	}
}	

void CBatchDetailSettingView::AddBatchSel(CStationObj* pStationObj)
{
	MYASSERT(pStationObj);
	
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pStationObj->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
    CSecObj* pSecObj = NULL;
	CString strTemp = _T("");
	while (pos != NULL)
	{
		CSecObj* pSecObj = (CSecObj*)listDevice.GetNext(pos);
		MYASSERT_CONTINUE(pSecObj);
		AddBatchSel(pSecObj);
	}
}


void CBatchDetailSettingView::OnBtnBatchSettingReset() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	MYASSERT(pData);
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	CStationObj* pStationObj = NULL;
	while (pos != NULL)
	{
		CStationObj* pStationObj = (CStationObj*)listDevice.GetNext(pos);
		if(pStationObj != NULL)
			AddBatchSel(pStationObj);
	}
	WriteLog(_T("数据重置成功"));
	CString strLog;
	strLog.Format(_T("存库选项数量为%d"), m_mapSave.GetCount());
	WriteLog(strLog, 3);
}


void CBatchDetailSettingView::OnShowList( WPARAM wParam, LPARAM lParam )
{
	CStationObj* pObj = (CStationObj*)lParam;
	CDeviceList deviceList(pObj, &m_mapSave);
	deviceList.DoModal();
}

void CBatchDetailSettingView::OnCheckChange( WPARAM wParam, LPARAM lParam )
{
	int nItem = (int)wParam;
	BOOL bChecked = (BOOL)lParam;
	CSel* pSel = (CSel*)m_List.GetItemData(nItem);
	MYASSERT(pSel);
	if (bChecked)
	{
		pSel->m_bChecked = TRUE;
	}
	else
	{
		pSel->m_bChecked = FALSE;
	}
	UpdateCheckState(pSel->pMap, pSel->m_bChecked);
}

void CBatchDetailSettingView::UpdateCheckState(CObject* pObj, BOOL bCheck)
{
	MYASSERT(pObj);
	if (pObj->IsKindOf(RUNTIME_CLASS(CMapStringToPtr)))
	{
		STRING_MAP* pMap = (STRING_MAP*)pObj;
		POSITION pos = pMap->GetStartPosition();
		CSel* pSel = NULL;
		CString strTemp = _T("");
		while (pos != NULL)
		{
			pMap->GetNextAssoc(pos, strTemp, (void*&)pSel);
			MYASSERT(pSel);
			pSel->m_bChecked = bCheck;
			UpdateCheckState(pSel->pMap, bCheck);
		}
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CMapWordToPtr)))
	{
		WORD_MAP* pMap = (WORD_MAP*)pObj;
		POSITION pos = pMap->GetStartPosition();
		CSel* pSel = NULL;
	    WORD wTemp = 0;
		while (pos != NULL)
		{
			pMap->GetNextAssoc(pos, wTemp, (void*&)pSel);
			MYASSERT(pSel);
			pSel->m_bChecked = bCheck;
			UpdateCheckState(pSel->pMap, bCheck);
		}
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CPtrList)))
	{
		BATCH_LIST* pList = (BATCH_LIST*)pObj;
		POSITION pos = pList->GetHeadPosition();
		while(pos != NULL)
		{
			CBatchSel* pBatchSel = (CBatchSel*)pList->GetNext(pos);
			pBatchSel->m_bChecked = bCheck;
		}
	}
}

void CBatchDetailSettingView::OnBtnBatchSettingDeviceList() 
{
	// TODO: Add your control notification handler code here
	CDeviceList deviceList(NULL, &m_mapSave);
	deviceList.DoModal();
}


/*************************************************************
 函 数 名：SetControls()
 功能概要：设置控件显示还是隐藏
 返 回 值: void
 参    数：param1	视图属性.0表示是自动总召的设置视图. 1表示是批量召唤视图
		   Param2
**************************************************************/
//##ModelId=49B87BB203C9
void CBatchDetailSettingView::SetControls( int nViewType )
{
	CWnd * pWnd = GetDlgItem(IDC_BTN_BATCH_SETTING_CALL);
	if(pWnd != NULL)
	{
		pWnd-> ShowWindow(SW_HIDE); 
	}
}


void CBatchDetailSettingView::OnBtnAutoClose() 
{
	// TODO: Add your control notification handler code here
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CBatchDetailSettingView::OnBtnCallStop() 
{
	// TODO: Add your control notification handler code here
	CDlgAutoCallStop dlg;
	dlg.DoModal();
}

void CBatchDetailSettingView::OnBtnBatchStartAllCall() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if(!HasSelected())
	{
		AfxMessageBox(StringFromID(IDS_AUTOCALL_NOSELECTION));
		return;
	}
	if(m_nCuruentStatus == 0)
	{
		//要开始召唤了
		//改变状态
		m_nCuruentStatus = 1;
		//使其它控件不可用
		UpdateControlsEnable(FALSE);
		//改变按钮文字
		SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_CANCEL_CALL));
		//发送消息给设备树视图, 让其禁用控件
		NotifySelectView(FALSE);
		//删除数据库中原来的数据
		if(!DeleteSelData(1))
		{
			//改变状态
			m_nCuruentStatus = 0;
			//使其它控件可用
			UpdateControlsEnable(TRUE);
			//发送消息给设备树视图, 让其可用控件
			NotifySelectView(TRUE);
			//改变按钮文字
			SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
			//log
			WriteLog("CBatchDetailSettingView::OnBtnBatchCall 删除数据失败\n");

			return;
		}
		//保存数据到数据库
		POSITION pos = m_mapSave.GetStartPosition();
		if(!MultiSaveData(1, pos))
		{
			//改变状态
			m_nCuruentStatus = 0;
			//使其它控件可用
			UpdateControlsEnable(TRUE);
			//发送消息给设备树视图, 让其可用控件
			NotifySelectView(TRUE);
			//改变按钮文字
			SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
			//log
			WriteLog("CBatchDetailSettingView::OnBtnBatchCall 插入数据失败\n");

			return;
		}

		//通知自动总召模块进行召唤
		//log
		WriteLog("CBatchDetailSettingView::OnBtnBatchCall 正在通知自动总召模块.....\n");
		if(!NotifyOC(0))
		{
			//改变状态
			m_nCuruentStatus = 0;
			//使其它控件可用
			UpdateControlsEnable(TRUE);
			//发送消息给设备树视图, 让其可用控件
			NotifySelectView(TRUE);
			//改变按钮文字
			SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
			//log
			WriteLog("CBatchDetailSettingView::OnBtnBatchCall 通知自动总召模块失败\n");
			//提示
			AfxMessageBox(StringFromID(IDS_AUTOCALL_DISCONNECT));
			return;
		}
	}
	else
	{
		//要取消召唤了
		//通知自动总召模块取消召唤
		NotifyOC(1);
		//改变状态
		m_nCuruentStatus = 0;
		//使其它控件可用
		UpdateControlsEnable(TRUE);
		//发送消息给设备树视图, 让其可用控件
		NotifySelectView(TRUE);
		//改变按钮文字
		SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
		//log
		WriteLog("CBatchDetailSettingView::OnBtnBatchCall 取消召唤成功.....\n");
	}
}

void CBatchDetailSettingView::OnBtnAutoCallResult() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->OnToolAutocallResult(); 
}

void CBatchDetailSettingView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CBatchDetailSettingView::OnDoubleclickedBtnBatchStartAllCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CBatchDetailSettingView::OnBtnBatchProcess() 
{
	// TODO: Add your control notification handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	MYASSERT(pFrame);
	pFrame->OnTestToolBatch();
}

void CBatchDetailSettingView::OnBatchCallEnd( WPARAM wParam, LPARAM lParam )
{
	if(m_nCuruentStatus == 1)
	{
		//通知自动总召模块取消召唤
		NotifyOC(1);

	}
	//改变状态
	m_nCuruentStatus = 0;
	//使其它控件可用
	UpdateControlsEnable(TRUE);
	//改变按钮文字
	SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
	//log
	WriteLog("CBatchDetailSettingView::OnBtnBatchCall 召唤完成或过程窗口关闭.....\n");
}

/****************************************************
Date:2012/4/24  Author:LYH
函数名:   HasSelected	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CBatchDetailSettingView::HasSelected()
{
	CString strTemp = _T("");
	CBatchSel* pSel = NULL;
	POSITION pos = m_mapSave.GetStartPosition();
	while(pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		MYASSERT_CONTINUE(pSel);
		if (!pSel->m_bChecked)
		{
			continue;
		}
		else
			return TRUE;
	}
	return FALSE;
}

void CBatchDetailSettingView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_FUNC_AUTOCALL_CONFIG));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   OnSTTP901	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CBatchDetailSettingView::OnSTTP901( WPARAM wParam, LPARAM lParam )
{
	//检查是否处于召唤状态
	if(m_nCuruentStatus != 1)
		return;
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//判断报文头
	if(pSttpData->sttp_body.nMsgId != 20153)
		return;

	KillTimer(m_nTimer);
	StartBatchCall();
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   StartBatchCall	
返回值:   void	
功能概要: 
*****************************************************/
void CBatchDetailSettingView::StartBatchCall()
{
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	MYASSERT(pFrame);
	pFrame->OnTestToolBatch();
	MYASSERT(pFrame->pBatchFrame);
	CWnd* pWnd = pFrame->pBatchFrame->m_wndSplitter.GetPane(0,1);
	MYASSERT(pWnd);
	pWnd->SendMessage(SHOW_RESULT, 0, (LPARAM)&m_mapSave);
	
	//使其它控件可用
	UpdateControlsEnable(TRUE);
	//发送消息给设备树视图, 让其可用控件
	NotifySelectView(TRUE);
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   EndBatchCall	
返回值:   void	
功能概要: 
*****************************************************/
void CBatchDetailSettingView::EndBatchCall()
{
	KillTimer(m_nTimer);
	//改变状态
	m_nCuruentStatus = 0;
	//使其它控件可用
	UpdateControlsEnable(TRUE);
	//发送消息给设备树视图, 让其可用控件
	NotifySelectView(TRUE);
	//改变按钮文字
	SetDlgItemText(IDC_BTN_BATCH_START_ALL_CALL, StringFromID(IDS_BTN_START_CALL));
	//提示
	AfxMessageBox(StringFromID(IDS_AUTOCALL_DISCONNECT));
}

/****************************************************
Date:2013/8/7  Author:LYH
函数名:   OnSTTP20069	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CBatchDetailSettingView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//检查是否处于召唤状态
	if(m_nCuruentStatus != 1)
		return;
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//判断报文头
	if(pSttpData->sttp_body.nMsgId != 20153)
		return;
	
	EndBatchCall();
}

void CBatchDetailSettingView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		EndBatchCall();
	}
	CViewBase::OnTimer(nIDEvent);
}
