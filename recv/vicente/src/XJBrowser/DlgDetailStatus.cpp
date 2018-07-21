// DlgDetailStatus.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgDetailStatus.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus dialog
/** @brief           升降序作用位*/
int g_iDetailStatusAsc;

DlgDetailStatus::DlgDetailStatus(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDetailStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDetailStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_arrColum.RemoveAll();
	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	pList = NULL;
	startPos = NULL;
	g_iDetailStatusAsc = -1;
    m_nOldSortCol = -1;
}

DlgDetailStatus::DlgDetailStatus(CommStatusList* pCommList, POSITION pos, CWnd* pParent /*=NULL*/)
: CDialog(DlgDetailStatus::IDD, pParent)
{
	m_arrColum.RemoveAll();
	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	pList = pCommList;
	startPos = pos;
	g_iDetailStatusAsc = -1;
	m_nOldSortCol = -1;
}


void DlgDetailStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDetailStatus)
	DDX_Control(pDX, IDC_LIST_DETAIL_STATUS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDetailStatus, CDialog)
	//{{AFX_MSG_MAP(DlgDetailStatus)
    ON_WM_WINDOWPOSCHANGED()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DETAIL_STATUS, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DETAIL_STATUS, OnColumnclickListDetailStatus)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDetailStatus message handlers


BOOL DlgDetailStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	FillDataList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void DlgDetailStatus::InitListCtrl()
{
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	if(LoadListConfig())
	{
		//载入配置成功
		int IDX = 0;
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			lvCol.iSubItem = i;
			lvCol.cx = m_arrColum[i].nItemWidth;
			CString sName = m_arrColum[i].sName;
			char * name = sName.GetBuffer(sName.GetLength());
			lvCol.pszText = name;
			m_List.InsertColumn(IDX++, &lvCol);
			if (m_arrColum[i].bHide)
			{
				m_List.SetColumnWidth(i, 0);
			}
		}
		
	}
	else
	{
		//载入配置失败
		char* arColumn[6]={
			"状态",
			"断开原因",
			"原因改变时间",
			"备通道状态",
			"备通道断开原因",
			"备通道原因改变时间"
			};		
		//分配列宽,最小需求
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 1://
				lvCol.cx = 80; // 状态
				colName = StringFromID( IDS_COMMU_STATUS);
				break; 
			case 2://
				lvCol.cx = 160; // 断开原因
				colName = StringFromID( IDS_DISCONNECT_REASON);
				break;
			case 3://
				lvCol.cx = 160; // 原因改变时间
				colName = StringFromID( IDS_DISCONNECT_REASON_TIME);
				break;
			case 4:
				lvCol.cx = 80;  //备通道状态
				colName = StringFromID( IDS_STANDBYCHL_COMMU_STATUS);
				break;
			case 5:
				lvCol.cx = 160;  //备通道断开原因
				colName = StringFromID( IDS_STANDBYCHL_DISCONNECT_REASON);
				break;
			case 6:
				lvCol.cx = 160; //备通道原因改变时间
				colName = StringFromID( IDS_STANDBYCHL_DISCONNECT_REASON_TIME);
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol, &lvCol);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
}



/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B86006D
BOOL DlgDetailStatus::LoadListConfig()
{
	//读取UI配置文件
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
			bResult = FALSE;
		} 	
		//加载UIConfig文件 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//加载失败
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str);
				WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewDetailStatusConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewCommConfig");
				WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewCommConfig下的配置");
					WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					//循环读取子节点信息
					for(int i = 0; i < pChild ->Getlength(); i++)
					{
						//得到子节点
						MSXML2::IXMLDOMNodePtr pNode;
						pNode = pChild ->Getitem(i);
						//得到子节点属性列表指针
						MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
						pAttrMap = pNode ->Getattributes();
						
						//列属性结构
						SColumn	col;
						
						//循环读取属性
						for(int j = 0; j < pAttrMap ->Getlength(); j++)
						{
							MSXML2::IXMLDOMNodePtr			pItem;
							pItem = pAttrMap ->Getitem(j);
							CString strValue = (char *)(_bstr_t)pItem ->GetnodeValue();
							int nHide = 0;
							int nWidth = 0;
							switch(j)
							{
							case 0: //列名
								col.sName = strValue;
								break;
							case 1: //列宽
								nWidth = atoi(strValue);
								col.nItemWidth = nWidth;
								break;
							case 2:	//是否隐藏
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
	//关闭打开的COM
	CoUninitialize();
	return bResult;
}

void DlgDetailStatus::RegulateControlSize()
{
	MYASSERT(m_List.GetSafeHwnd());
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	rc.top += 50;
	//调整列表框大小,占整个高度
	m_List.MoveWindow(&rc, TRUE);
}

void  DlgDetailStatus::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL DlgDetailStatus::InitCOM()
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
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);*/
	return InitReportComNoReg(pClf, pReportDoc);
	
	//return TRUE;
}

/*************************************************************
 函 数 名：ReleaseCOM()
 功能概要：删除COM组件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void DlgDetailStatus::ReleaseCOM()
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

BOOL DlgDetailStatus::GetStatus(Record* pRecord)
{
	MYASSERT_BOOL(pRecord);
	if ((pRecord->m_nIPFlag == 2 && pRecord->m_nCommStatus == 0 && pRecord->m_nCommStatus1 == 0) || (pRecord->m_nIPFlag != 2 && pRecord->m_nCommStatus == 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void DlgDetailStatus::SetRecordInfo(Record* pRecord, int nItem)
{
	MYASSERT(pRecord);
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	//1:状态
	result = m_List.SetItemText(nItem, IDX, pRecord->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pRecord->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)));
	
	if (pRecord->m_nCommStatus  != -1)
	{
		//2:断开原因
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReason);
		
		//3:原因改变时间
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReasonTime);
	}
	else
	{
		IDX += 2;
	}
	
	//4:备通道状态
	result = m_List.SetItemText(nItem, ++IDX, pRecord->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pRecord->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)));	
	
	if (pRecord->m_nCommStatus1 != -1)
	{
 		//5:备通道断开原因
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReason1);
		
		//6:备通道原因改变时间
		result = m_List.SetItemText(nItem, ++IDX, pRecord->m_sReasonTime1);
	}
		
	//设置数据s
	m_List.SetItemData(nItem, (DWORD)pRecord);
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if (g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}


void  DlgDetailStatus::FillDataList()
{
	MYASSERT(pList);
	MYASSERT(startPos);
	CWnd* pWnd = GetDlgItem(IDC_STATION_STATIC);
	MYASSERT(pWnd);
	MYASSERT(pList->pStationObj); 
	pWnd->SetWindowText(pList->pStationObj->m_sName);
	
	pWnd = GetDlgItem(IDC_IP_MODE_STATIC);
    MYASSERT(pWnd);
	switch(pList->pStationObj->m_nIPFlag)
	{
	case 0://单机
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_ALONE));
		break;
	case 1://双机
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_DOUBLE_MACHINES));
		break;
	case 2:
		pWnd->SetWindowText( StringFromID(IDS_COMMUMODE_DOUBLE_CHANNELS));
		break;
	}

	if(pList != NULL)
	{
		CString str;
		str.Format("厂站[%s]状态变为记录数为%d", pList->pStationObj->m_sID, pList->recordList.GetCount());
		WriteLog(str);
	}

	MYASSERT(startPos);
    Record* pRecord = pList->recordList.GetAt(startPos);
	pList->recordList.GetPrev(startPos);

	do
	{
		int nItem = m_List.InsertItem(0, "");
		SetRecordInfo(pRecord, nItem);
		MYASSERT(startPos);
	    pRecord = pList->recordList.GetPrev(startPos);
		MYASSERT(pRecord);
	}while(GetStatus(pRecord));
}


void  DlgDetailStatus::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
			
				if (nSubItem == 0 || nSubItem == 3)
				{
					CString strStatus = m_List.GetItemText(nItem, nSubItem);
					if (!strStatus.CompareNoCase( StringFromID(IDS_COMMUSTATUS_OFF)))
					{
						lplvcd->clrText = RGB(255, 0, 0);
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
 函 数 名：CompareFunction()
 功能概要：比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数：param1	比较对象1
		   Param2	比较对象2
		   param3	比较参数
**************************************************************/
int CALLBACK DlgDetailStatus::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//取得比较数据
	Record* pRecord1 = (Record*)lParam1;
	Record* pRecord2 = (Record*)lParam2;
	int nCol = (int)lParamData;
	
	int iResult = 0;
	switch(nCol)
	{
	case 2: //原因改变时间
		iResult = lstrcmpi( pRecord1->m_sReasonTime, pRecord2->m_sReasonTime );
		break;
	case 5: //备通道原因改变时间
		iResult = lstrcmpi( pRecord1->m_sReasonTime1, pRecord2->m_sReasonTime1 );
		break;	
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDetailStatusAsc;
	return iResult;
}

void DlgDetailStatus::OnColumnclickListDetailStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iDetailStatusAsc = -g_iDetailStatusAsc;
	}
	else
	{
		g_iDetailStatusAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

BOOL DlgDetailStatus::SaveListConfig()
{
	//保存UI配置文件
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("ViewDetailStatusConfig::保存UIConfig失败", XJ_LOG_LV3);
			bResult = FALSE;
		} 	

		//加载UIConfig文件 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//加载失败
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str);
				WriteLog("ViewDetailStatusConfig::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewDetailStatusConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewDetailStatusConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewDetailStatusConfig");
					if(pNew != NULL)
					{
						//加入新节点
						pRoot ->appendChild(pNew);
						//新建所有子节点
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
							if(bHide) //隐藏
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							//加入子节点
							pNew ->appendChild(pNewChild);
						}
					}
				}
				else //找到了ViewDetailStatusConfig
				{
					//查找是否有子节点
					if(!pSelected ->hasChildNodes())
					{
						//找不到子节点
						WriteLog("未找到ViewDetailStatusConfig下的配置, 新建所有子节点");
						//新建所有子节点
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
							if(bHide) //隐藏
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
						//找到有子节点, 删除所有子节点,再新建
						WriteLog("找到了ViewDetailStatusConfig下的配置, 先删除新建所有子节点");
						//找到子节点列表指针
						MSXML2::IXMLDOMNodeListPtr pChild;
						pChild = pSelected ->GetchildNodes();
						
						//删除子节点
						MSXML2::IXMLDOMNodePtr pNode = NULL;
						pNode = pChild ->Getitem(0);
						while(pNode != NULL)
						{
							pSelected ->removeChild(pNode);
							pNode = pChild ->Getitem(0);
						}
						
						//新建子节点
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
							if(bHide) //隐藏
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
				//保存配置文件
				pDoc ->save(_variant_t(g_UIConfig));
				bResult = TRUE;
			}
		}	
	}
	//关闭打开的COM
	CoUninitialize();
	return bResult;
}


BOOL DlgDetailStatus::NeedSave()
{
	BOOL bReturn = FALSE;
	
	//先保存列信息
	int nCount = m_List.m_hideHeader.m_aColum.GetSize();
	for(int j = 0; j < nCount; j++)
	{
		if(!m_List.m_hideHeader.m_aColum[j].bHide)
		{
			//只保存非隐藏列的列宽
			m_List.m_hideHeader.m_aColum[j].nItemWidth = m_List.GetColumnWidth(j);
		}
	}
	
	if(m_arrColum.GetSize() == 0)
	{
		//m_arrColum为空, 需要保存
		bReturn = TRUE;
	}
	else if(m_arrColum.GetSize() != nCount)
	{
		//如果数量不相等,需要保存
		bReturn = TRUE;
	}
	else
	{
		//对比各项,发现一项不等就需要保存
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
		//如果需要保存, 则重写m_arrColum
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

void DlgDetailStatus::OnDestroy() 
{	
	//保存UI设置
	if(NeedSave())
		SaveListConfig();
	CDialog::OnDestroy();
	
	//TODO: Add your message handler code here
	
}
