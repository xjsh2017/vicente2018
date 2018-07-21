// DeviceList.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ReportCtrl.h"
#include "DeviceList.h"
#include "GlobalFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceList dialog


CDeviceList::CDeviceList(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStationObj = NULL;
    m_pBatchMap = NULL;
}


CDeviceList::CDeviceList(CStationObj* pStationObj, BATCHSEL_MAP* pBatchMap, CWnd* pParent /*=NULL*/)
: CDialog(CDeviceList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceList)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStationObj = pStationObj;
    m_pBatchMap = pBatchMap;
}


void CDeviceList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceList)
	DDX_Control(pDX, IDC_LIST_BATCH_DETAIL_SETTING, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceList, CDialog)
	//{{AFX_MSG_MAP(CDeviceList)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BATCH_DETAIL_SETTING, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceList message handlers

BOOL CDeviceList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//空图形列表, 用来调整行高
	CString shead="";
	shead.Format("%s, 220; %s, 95; %s, 95; %s, 95; %s, 95; %s, 95; %s, 85", StringFromID(IDS_MODEL_SECONDARY),
		StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_SECPROP_SETTING), StringFromID(IDS_SECPROP_DIGITAL), 
		StringFromID(IDS_SECPROP_SOFT), StringFromID(IDS_SECPROP_ANALOG), StringFromID(IDS_MODEL_CPU));
	m_List.SetColumnHeader(shead);
	m_List.SetGridLines(TRUE); // SHow grid lines
	//m_List.SetCheckboxeStyle(RC_CHKBOX_NORMAL); // Enable checkboxes
	m_List.SetEditable(FALSE); //
	FillDataToList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeviceList::FillDataToList()
{
	MYASSERT(m_pBatchMap);
	POSITION pos = m_pBatchMap->GetStartPosition();
	CBatchSel* pBatchSel = NULL;
	CString strTemp = _T("");
	while (pos != NULL)
	{
		m_pBatchMap->GetNextAssoc(pos, strTemp, (void*&)pBatchSel);
		MYASSERT_CONTINUE(pBatchSel);
		MYASSERT_CONTINUE(pBatchSel->m_pObj);
		MYASSERT_CONTINUE(pBatchSel->m_pCpu);
		const int IDX = m_List.InsertItem(0, _T(""));
		m_List.SetItemText(IDX, 0, pBatchSel->m_pObj->m_sName);
		//1: 定值区号
		m_List.SetItemText(IDX, 1, pBatchSel->GetCallString(pBatchSel->m_nCallZone));
		//2: 定值
		m_List.SetItemText(IDX, 2, pBatchSel->GetCallString(pBatchSel->m_nCallSetting));
		//3: 开关量
		m_List.SetItemText(IDX, 3, pBatchSel->GetCallString(pBatchSel->m_nCallDigital));
		//4: 软压板
		m_List.SetItemText(IDX, 4, pBatchSel->GetCallString(pBatchSel->m_nCallSoftBoard));
		//5: 模拟量
		m_List.SetItemText(IDX, 5, pBatchSel->GetCallString(pBatchSel->m_nCallAnalog));
		//6: cpu
		CString strTemp;
		strTemp.Format("%d", pBatchSel->m_pCpu->code);
		m_List.SetItemText(IDX, 6, strTemp);
	}
}


/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：响应列表个性化显示消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void CDeviceList::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				if(nSubItem == 1 || nSubItem == 2 || nSubItem == 3 || nSubItem == 4 || nSubItem == 5)
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