// DLGMarked.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DLGMarked.h"
#include "MainFrm.h"

#include "DlgValidateID.h"

#include "XJTagOutStore.h"
#include "qptsetstatetable.h"
#include "XJUserStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// DLGMarked dialog

DLGMarked::DLGMarked(CWnd* pParent /*=NULL*/)
	: CDialog(DLGMarked::IDD, pParent)
{
	m_bMark = FALSE;
	//{{AFX_DATA_INIT(DLGMarked)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLGMarked::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLGMarked)
	DDX_Control(pDX, IDC_CMB_MARKREASON, m_cmbMarkReason);
	DDX_Control(pDX, IDC_LIST_CTRL1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGMarked, CDialog)
	//{{AFX_MSG_MAP(DLGMarked)
	ON_BN_CLICKED(IDC_BTN_MARK, OnBtnMark)
	ON_BN_CLICKED(IDC_BTN_UNMARK, OnBtnUnmark)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnApplySetting)
	ON_CBN_SELCHANGE(IDC_CMB_MARKREASON, OnSelchangeCmbMarkreason)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLGMarked message handlers

void DLGMarked::OnBtnMark() 
{
	// TODO: Add your control notification handler code here
	//更新挂牌状态
	m_bMark = TRUE;

	//判断是否存在已挂牌装置（一次仅一个保护）
	if (!CanMark())
		return;

	//运行人员确认
	CString sRunUser;
	CDlgValidateID dlgID(0, 1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{	
		sRunUser = dlgID.m_strUser;

		//更新数据库的挂牌表
		if (InsertDBMark())
		{
			CString sInsert = "";
			((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);

			CXJTagOutStore::GetInstance()->GetState()->Next_1(sRunUser.GetBuffer(0)
				, m_pObj->m_sID.GetBuffer(0), sInsert.GetBuffer(0));
			
			AfxMessageBox( StringFromID(IDS_HANGOUT_SUCCESS), MB_OK | MB_ICONINFORMATION);
			SetDeviceState();
		}
	}
}

void DLGMarked::OnBtnUnmark() 
{
	// TODO: Add your control notification handler code here
	//更新挂牌状态
	m_bMark = FALSE;

	// 检查是否可以取消挂牌
	if (!CanUnMark())
		return;
	
	//运行人员确认
	CString sRunUser;
	CDlgValidateID dlgID(0, 1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{	
		sRunUser = dlgID.m_strUser;
		
		//更新数据库的挂牌表
		if (InsertDBMark())
		{
			CXJTagOutStore::GetInstance()->GetState()->Next_0(sRunUser.GetBuffer(0));
			
			AfxMessageBox( StringFromID(IDS_UNHANGOUT_SUCCESS), MB_OK | MB_ICONINFORMATION);
			SetDeviceState();
		}
	}
}

void DLGMarked::OnApplySetting()
{
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();
	
	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);
	int nTagOutRowIdx = pTagOutStore->GetTagOutRowIdx(nReasonType);

	CPoint pt;
	CString strText;
	nIndex = 0;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		pt = m_List.m_arrEdited.GetAt(i);
		CString strText = m_List.GetItemText(pt.x, pt.y);
		
		nIndex = strText.Find("*", 0);
		if (nIndex != -1)
		{
			strText = strText.Left(nIndex);
			m_List.SetItemText(pt.x, pt.y, strText);

			int iRowFlow = m_List.GetItemData(pt.x);
			flow.SetFieldValue(iRowFlow, 5, QByteArray(strText.GetBuffer(0)));
			pTagOutState->SetFieldValue(nTagOutRowIdx, "reverse1", flow);
		}
	}

	pTagOutStore->SaveState("c:/tb_sys_config.txt");
	
	m_List.ClearEdited();
	((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(FALSE);
}

/*************************************************************
 函 数 OnClickList1()
 功能概要：响应点击事件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A03AB
void DLGMarked::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();

	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);

	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	int nRowIdx = static_cast<int> (pLVCD->nmcd.dwItemSpec); //行索引
	int nColIdx = pLVCD->iSubItem; //列索引

	if(nItem != -1)
	{
		if(nSubItem == 2)
		{
			//AfxMessageBox("hello");
			//最新值列

			int iRowFlow = m_List.GetItemData(nItem);
			int nGroupType = flow.GetFieldValue(iRowFlow, 4).toInt();
			int nStateID = flow.GetFieldValue(iRowFlow, 2).toInt();

			if (pTagOutState->GetType() == XJ_TAGOUT_UNDEFINE ||
				!pTagOutState->GetLogs().contains(nStateID)){
				m_List.ListSubItem(nItem, nSubItem, pUserStore->BuildComboxUserList(nGroupType).constData());
			}
			//AfxMessageBox(QByteArray::number(nGroupType).constData());
			//AfxMessageBox(QByteArray::number(nStateID).constData());
		}

		if (m_List.m_arrEdited.GetSize() > 0){
			((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(TRUE);
		}
	}

	*pResult = 0;
}

BOOL DLGMarked::CanMark()
{
	BOOL bReturn = TRUE;
	
	CString str;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	CXJTagOutStore *pPTSetStore = CXJTagOutStore::GetInstance();
	pPTSetStore->ReLoadState();	
	QPTSetStateTable *pPTSetState = pPTSetStore->GetState();

	QByteArrayMatrix &log = pPTSetState->GetLogs();

	int nPTSetState = pPTSetState->GetStateID();
	
	// 状态不是未挂牌状态
	if (XJ_OPER_UNHANGOUT != nPTSetState){
		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pPTSetState->GetPTID().constData(), TYPE_SEC);
		if (NULL != pObj){
			if (pObj == m_pObj){
				str.Format("装置已挂牌！");
			}else{
				str.Format("当前已存在挂牌的保护装置： \n\n-----------------------\n"
					"挂牌装置：[ %s ] [ %s ] \n\n"
					"挂牌原因：%s \n\n"
					"挂牌时间：%s \n\n"
					"执行用户：%s"
					, pObj->m_pStation ? pObj->m_pStation->m_sName : ""
					, pObj->m_sName
					, pPTSetState->GetTypeName().constData()
					, pPTSetState->GetLog(XJ_OPER_HANGOUT).GetFieldValue(1, 1).constData()
					, pPTSetState->GetRunnerUserID().constData());
			}
			AfxMessageBox(str, MB_ICONWARNING);
		}
		
		return FALSE;
	}

	CString sInsert = "";
	((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);
	if (sInsert == "")
	{
		AfxMessageBox("请选择或输入挂牌原因.");

		return FALSE;
	}
	
	return bReturn;
}

BOOL DLGMarked::CanUnMark()
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp *pApp = (CXJBrowserApp *)AfxGetApp();
	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	pStore->ReLoadState();	
	QPTSetStateTable *pState = pStore->GetState();
	
	QByteArrayMatrix &log = pState->GetLogs();
	
	int nPTSetState = pState->GetStateID();
	int nFlag = pState->GetFlags();
	int nType = pState->GetType();
	CString sRunnerUserID = pState->GetRunnerUserID().constData();

	if (XJ_OPER_UNHANGOUT == nPTSetState || CString(pState->GetPTID().constData()) != m_pObj->m_sID){
		CString str;
		str.Format("该装置未挂牌！");
		AfxMessageBox(str);
		
		return FALSE;
	}
	
	if (sRunnerUserID != pApp->m_User.m_strUSER_ID){
		CString str;
		str.Format("该装置已授权给用户 [%s]，您无法进行取消操作！", sRunnerUserID);
		AfxMessageBox(str);
		
		return FALSE;
	}

	if (XJ_OPER_PTVALVSET_STATE_4 == nPTSetState){
		CString str;

		if (nFlag == 2){
			if (XJ_TAGOUT_PTVALVSET == nType)
				str.Format("该装置：操作员正在下发定值修改执行请求，您暂时无法取消挂牌，请稍后再试！", sRunnerUserID);
			else if (XJ_TAGOUT_PTZONESET == nType)
				str.Format("该装置：操作员正在下发定值区切换执行请求，您暂时无法取消挂牌，请稍后再试！", sRunnerUserID);
			else if (XJ_TAGOUT_PTSOFTSET == nType)
				str.Format("该装置：操作员正在下发软压板投退执行请求，您暂时无法取消挂牌，请稍后再试！", sRunnerUserID);

			AfxMessageBox(str, MB_OK | MB_ICONWARNING);

			return FALSE;
		}else{
			/*str.Format("该装置：操作人员还未下发定值修改，您确定要取消挂牌吗？", sRunnerUserID);
			int n = AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION);
			if (n == IDYES){
				//AfxMessageBox("yes");
			}else{
				
				return;
			}*/
		}
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：InsertDBMark()
 功能概要：更新数据库的装置挂牌信息
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
BOOL DLGMarked::InsertDBMark()
{
	ASSERT(NULL != m_pObj);

	CString strTemp;
	strTemp.Format("PT_ID: %s, NAME: %s",m_pObj->m_sID, m_pObj->m_sName);
	//AfxMessageBox(strTemp);

	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if (m_bMark)
	{
		//获取当前选择项
		CString sInsert = "";
		((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);
		if (sInsert == "")
		{
			return FALSE;
		}
		m_pObj->m_sSuspendreason = sInsert;

		//判断原因是否已经存在
		BOOL bInsert = Comparison(sInsert);
		
		//更新挂牌原因表
		if (bInsert)
		{
			//获得新原因的ID
			m_pObj->m_nSuspendreason = pApp->GetDataEngine()->m_arrMark.GetCount(); 
			
			//组建查询条件
			SQL_DATA sql;
			sql.Conditionlist.clear();
			sql.Fieldlist.clear();
			
			//设置字段
			
			//挂牌原因编号
			CString str = "";
			str.Format("%d", m_pObj->m_nSuspendreason);
			Field field1;
			pApp->m_DBEngine.SetField(sql, field1, "id", EX_STTP_DATA_TYPE_INT, str);
			
			//挂牌原因描述
			Field field2;
			pApp->m_DBEngine.SetField(sql, field2, "note", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sSuspendreason);
			
			char * sError = new char[MAXMSGLEN];
			memset(sError, '\0', MAXMSGLEN);
			
			int nResult;
			try
			{
				nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_TB_SUSPEND_REASON_DEF, sql, sError);
			}
			catch (...)
			{
				WriteLog("DLGMarked::InsertDBMark, 保存失败");
				delete[] sError;
				return FALSE;
			}
			
			if(nResult == 1)
			{
				//成功
				CMark* pObj = new CMark;
				pObj->m_sMarkInfo = sInsert;
				pObj->m_nMarkID = m_pObj->m_nSuspendreason;
				//读取数据成功,加入到链表
				pApp->GetDataEngine()->m_arrMark.AddTail(pObj);
				bReturn = TRUE;
			}
			else
				bReturn = FALSE;
			
			delete[] sError;
		}
	}
	
	//更新装置设备表
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;	
	//字段
	//SVG_Path
	if (m_pObj->m_nSuspendreason != -1)
	{
		str.Format("%d", m_pObj->m_nSuspendreason);
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "suspendreason", EX_STTP_DATA_TYPE_INT, str);
	}
	m_pObj->m_nSuspendstatus = m_bMark;
	str.Format("%d",m_bMark);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "suspendstatus", EX_STTP_DATA_TYPE_INT, str);
	Field Field3;
	str.Format("%d", m_bMark ? 5 : 2);
	m_pObj->m_nRunStatu = m_bMark ? 5 : 2;
	pApp->m_DBEngine.SetField(sql, Field3, "run_status", EX_STTP_DATA_TYPE_INT, str);

	//条件
	//station_id
	Condition condition2;
	str.Format("name = '%s'", m_pObj->m_sName);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_SECDEV_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("DLGMarked::InsertDBMark, 更新失败");
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
		str.Format("DLGMarked::InsertDBMark,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	


	//store->Next(1, pApp->m_User.m_strUSER_ID);

	//pApp->NextPTSetModState(m_bMark ? 1 : 0, zone, pApp->m_User.m_strUSER_ID);
	
 	return bReturn;
}

/*************************************************************
 函 数 名：SetDeviceState()
 功能概要：设置挂牌状态
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
void DLGMarked::SetDeviceState()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->m_pDeviceView->FillTree();
	CXJBrowserView * pView = pApp->GetSVGView();
	pView->SetDeviceCol(m_pObj);//设置颜色
}


void DLGMarked::OnSelchangeCmbMarkreason() 
{
	// TODO: Add your control notification handler code here
	
// 	int nIndex=((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetCurSel();
// 	if (nIndex!=0)
// 	{
// 		CStationObj* pStationid=(CStationObj*)m_cmbMarkReason.GetItemData(nIndex);
// 		
// 		CString sStationID=	pStationid->m_sID;
// 		m_sChooseStationID.Format("%s",sStationID);
// 	}

	if (m_List.m_arrEdited.GetSize() > 0){
		int nID = AfxMessageBox("更改未保存， 您确定要放弃吗？", MB_YESNO);
		if (nID == IDNO){
			return;
		}
		
		m_List.ClearEdited();
		((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(FALSE);
	}

	FillData();
}

/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void DLGMarked::InitComBox()
{
	//用厂站中的数据填充列表
	m_cmbMarkReason.ResetContent();
	
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	//m_cmbMarkReason.InsertString(0,"所有");
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if(pObj == NULL)
			continue;
		//加入到下拉框
		int nIndex = pObj->m_nMarkID;
			
		m_cmbMarkReason.InsertString(nIndex,pObj->m_sMarkInfo);
		m_cmbMarkReason.SetItemData(nIndex,(DWORD)pObj);
		nIndex++;
	}
	//m_cmbMarkReason.SetCurSel(0);
}

BOOL DLGMarked::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	InitListStyle();
	
	// TODO: Add extra initialization here
	InitComBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int DLGMarked::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList  m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	{
		//载入配置失败
		//char* arColumn[11]={"序号","定值名称", "定值代码", "单位", "基准值", "组号", "条目号","步长", "最小值/最大值", "精度", "数据类型"};
		//分配列宽,最小需求
		for (int nCol=0; nCol < 4 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 220; // 操作类型
				colName = "操作类型";
				break;
			case 2://
				lvCol.cx = 100; // 执行用户
				colName = "执行用户";
				break;
			case 3:
				lvCol.cx = 60; // 是否启用
				colName = "启用";
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		//m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(3, TRUE);
	}
	
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名: Comparison()
 功能概要: 比较描述原因是否是原有的
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BOOL DLGMarked::Comparison(CString sMarkInfo)
{
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return TRUE;
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (pObj->m_sMarkInfo == sMarkInfo)
		{
			return FALSE;
		}
	}
	return TRUE;
}

void DLGMarked::FillData()
{
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();
	
	m_List.SetRedraw(FALSE);
	m_List.DeleteAllItems();
	m_List.SetRedraw(TRUE);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper); 

	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	
	//AfxMessageBox(pTagOutState->GetTagOutReasonName(nReasonType).constData());

	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);
	//AfxMessageBox(flow.constData());

	nIndex = 0;
	int nRowCount = flow.GetRowCount();
	for(int i = 1; i <= nRowCount; i ++)
	{
		CString str;
		
		int nTagOutType = flow.GetFieldValue(i, 1).toInt();
		int nStateID = flow.GetFieldValue(i, 2).toInt();
		int nEnable = flow.GetFieldValue(i, 3).toInt();
		QByteArray &usrGroupID = flow.GetFieldValue(i, 4);
		QByteArray &usrID = flow.GetFieldValue(i, 5);
		QByteArray typeName;// = pTagOutStore->GetSubFuncID(nStateID);
		typeName << baReasonName << " : " << pTagOutStore->GetSubFuncID(nStateID);

		if (nStateID == XJ_OPER_HANGOUT || nStateID == XJ_OPER_UNHANGOUT)
			continue;

		if (nEnable == 0)
			continue;

		str.Format("%d", nIndex+1);
		m_List.InsertItem(nIndex, str); //ID

		m_List.SetItemText(nIndex, 1, typeName.constData()); // 操作类型

		m_List.SetItemText(nIndex, 2, usrID); // 执行用户
		
		m_List.SetItemText(nIndex, 3, QByteArray::number(nEnable).constData()); // 是否启用

		m_List.SetItemData(nIndex, (DWORD)i);
		
		nIndex++;
	}
	//恢复刷新
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	//WriteLog("CDlgCheckPro::FillListData, 结束", XJ_LOG_LV3);
}