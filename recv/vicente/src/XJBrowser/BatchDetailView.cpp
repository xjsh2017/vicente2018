// BatchDetailView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchDetailView.h"
#include "DlgOCSet.h"
#include "GlobalFunc.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

extern int g_iBatchViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView

IMPLEMENT_DYNCREATE(CBatchDetailView, CViewBase)

//##ModelId=49B87BB103DA
CBatchDetailView::CBatchDetailView()
	: CViewBase(CBatchDetailView::IDD)
{
	//{{AFX_DATA_INIT(CBatchDetailView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();

	g_iBatchViewAsc = -1;
	
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	
	m_nOldSortCol = -1;

}

//##ModelId=49B87BB3009C
CBatchDetailView::~CBatchDetailView()
{
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	TRACE("CBatchDetailView::~CBatchDetailView \n");
}

//##ModelId=49B87BB3005D
void CBatchDetailView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchDetailView)

	DDX_Control(pDX, IDC_LIST_BATCH_DETAIL, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchDetailView, CViewBase)
	//{{AFX_MSG_MAP(CBatchDetailView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BATCH_DETAIL, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BATCH_DETAIL, OnColumnclickListBatchDetail)
	
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	
	ON_MESSAGE(WM_STTP_20154, OnSTTP20154)
	
	ON_MESSAGE(SHOW_RESULT, OnShowResult)
	ON_COMMAND(ID_BATCH_CLOSE, OnBatchClose)
	
	ON_COMMAND(ID_BATCH_EXPORT, OnBatchExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB300DA
void CBatchDetailView::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87BB30119
void CBatchDetailView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchDetailView message handlers

//##ModelId=49B87BB3002E
void CBatchDetailView::OnInitialUpdate() 
{
	//记录控件位置

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
	//初始化列表
	InitListCtrl();
	//
}

//##ModelId=49B87BB30157
void CBatchDetailView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_BATCH_DETAIL, left_client, top_client, right_client, bottom_client);
	RegulateControlSize();
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BB2013B
BOOL CBatchDetailView::LoadListConfig()
{
	//读取UI配置文件
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //初始化COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", 3);
			WriteLog("BatchDetailView::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("BatchDetailView::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			
			pSelected = pDoc->selectSingleNode("//BatchDetailViewConfig");
			
			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到BatchDetailViewConfig");
				WriteLog("BatchDetailView::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到BatchDetailViewConfig下的配置");
					WriteLog("BatchDetailView::读取UIConfig失败,将使用默认列风格");
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() == 25 )
					{
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
					else
						bResult = FALSE;
				}
			}
		}		
// 	}	
// 	//关闭打开的COM
// 	CoUninitialize();
	return bResult;
}

//##ModelId=49B87BB20177
BOOL CBatchDetailView::SaveListConfig()
{
	//保存UIConfig失败", 3
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //初始化COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", 3);
			WriteLog("BatchDetailView::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("BatchDetailView::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				
				pSelected = pDoc->selectSingleNode("//BatchDetailViewConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到BatchDetailViewConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					
					pNew = pDoc ->createElement("BatchDetailViewConfig");
					
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
				else //找到了ViewActionConfig
				{
					//查找是否有子节点
					if(!pSelected ->hasChildNodes())
					{
						//找不到子节点
						WriteLog("未找到BatchDetailViewConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了BatchDetailViewConfig下的配置, 先删除新建所有子节点");
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
// 	}
// 	//关闭打开的COM
// 	CoUninitialize();
	return bResult;
}

/*************************************************************
 函 数 名：NeedSave()
 功能概要：判断是否列信息有改变需要保存
 返 回 值: 需要保存返回TRUE, 不需要保存返回FALSE
**************************************************************/
//##ModelId=49B87BB201A5
BOOL CBatchDetailView::NeedSave()
{
	BOOL bReturn = FALSE;

	//先读取最新列信息
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

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB201D4
void CBatchDetailView::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
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
int CBatchDetailView::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	if(LoadListConfig())
	{
		//载入配置成功
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
		//载入配置失败
		char* arColumn[25]={"二次设备", "CPU号", "定值区号", "定值区号结果", "定值区号结果说明", 
			"定值", "定值结果", "定值结果说明", "开关量","开关量结果", "开关量结果说明", 
			"软压板","软压板结果", "软压板结果说明", "模拟量","模拟量结果", "模拟量结果说明",
			"录波文件","录波文件结果","录波文件结果说明","历史事件","历史事件结果","历史事件结果说明",
			"厂站名", "一次设备"};
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 100; //列宽50象素
				break;
			case 1://
				lvCol.cx = 100; //列宽60象素
				break;
			case 2://
				lvCol.cx = 100; //列宽60象素
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText=arColumn[nCol];
			m_List.InsertColumn(nCol,&lvCol);
		}
	}

	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);

	return 0;
}

//##ModelId=49B87BB30196
void CBatchDetailView::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：响应窗口打开消息, 刷新视图
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB302E4
void CBatchDetailView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//LPARAM 表示视图类型
	
	//刷新一次
	RegulateControlSize();
	MoveCtrl(IDC_LIST_BATCH_DETAIL, left_client, top_client, right_client, bottom_client);
}

/*************************************************************
 函 数 名：AddSelToList()
 功能概要：把选择项加入到列表框中显示
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	指定选择项对象
		   Param2	指定在列表中的位置索引
**************************************************************/
//##ModelId=49B87BB20280
BOOL CBatchDetailView::AddSelToList( CBatchSel* pSel, int nIndex )
{
	//检查数据有效性
	if(pSel == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	if(pSel->m_pObj == NULL)
		return FALSE;
	if(pSel->m_pCpu == NULL)
		return FALSE;

	CString str;
	//0: 二次设备名
	str = pSel->m_pObj->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1: CPU号
	str.Format("%d(%s)", pSel->m_pCpu->code, pSel->m_pCpu->des);
	m_List.SetItemText(nIndex, 1, str);
	//2: 定值区号
	m_List.SetItemText(nIndex, 2, pSel->GetCallString(pSel->m_nCallZone));
	//3: 定值区号结果
	str = GetResult(pSel->m_nZone);
	m_List.SetItemText(nIndex, 3, str);
	//4:定值区号结果说明
	if(pSel->m_nZone == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 4, str);
	//5: 定值
	m_List.SetItemText(nIndex, 5, pSel->GetCallString(pSel->m_nCallSetting));
	//6: 定值结果
	str = GetResult(pSel->m_nSetting);
	m_List.SetItemText(nIndex, 6, str);
	//7:定值结果说明
	if(pSel->m_nSetting == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 7, str);
	//8: 开关量
	m_List.SetItemText(nIndex, 8, pSel->GetCallString(pSel->m_nCallDigital));
	//9: 开关量结果
	str = GetResult(pSel->m_nDigital);
	m_List.SetItemText(nIndex, 9, str);
	//10:开关量结果说明
	if(pSel->m_nDigital == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 10, str);
	//11: 软压板
	m_List.SetItemText(nIndex, 11, pSel->GetCallString(pSel->m_nCallSoftBoard));
	//12: 软压板结果
	str = GetResult(pSel->m_nSoftBoard);
	m_List.SetItemText(nIndex, 12, str);
	//13:软压板结果说明
	if(pSel->m_nSoftBoard == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 13, str);
	//14: 模拟量
	m_List.SetItemText(nIndex, 14, pSel->GetCallString(pSel->m_nCallAnalog));
	//15: 模拟量结果
	str = GetResult(pSel->m_nAnalog);
	m_List.SetItemText(nIndex, 15, str);
	//16:模拟量结果说明
	if(pSel->m_nAnalog == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 16, str);
	//17: 录波
	m_List.SetItemText(nIndex, 17, pSel->GetCallString(pSel->m_nCallOsc));
	//18: 录波结果
	str = GetResult(pSel->m_nOsc);
	m_List.SetItemText(nIndex, 18, str);
	//19:录波结果说明
	if(pSel->m_nOsc == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 19, str);

	//20: 历史事件
	m_List.SetItemText(nIndex, 20, pSel->GetCallString(pSel->m_nCallHistory));
	//21: 历史事件结果
	str = GetResult(pSel->m_nHistory);
	m_List.SetItemText(nIndex, 21, str);
	//22:历史事件结果说明
	if(pSel->m_nHistory == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 22, str);

	//23: 厂站名
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pSel->m_pObj->m_sStationID, TYPE_STATION);
	str = "";
	if(pStation != NULL)
		str = pStation->m_sName;
	m_List.SetItemText(nIndex, 23, str);
	//24: 一次设备名
	str = "";
	if(pSel->m_pObj->m_pOwner  != NULL)
		str = pSel->m_pObj->m_pOwner->m_sName;
	m_List.SetItemText(nIndex, 24, str);
	//关联数据
	m_List.SetItemData(nIndex, (DWORD)pSel);
	return TRUE;
}

/*************************************************************
 函 数 名：GetResult()
 功能概要：得到召唤结果文字描述
 返 回 值: 召唤结果文字描述
 参    数：param1	召唤结果
		   Param2
**************************************************************/
//##ModelId=49B87BB202AF
CString CBatchDetailView::GetResult( int nResult )
{
	//检查数据有效性
	if(nResult < 0 || nResult >9)
		return "";
	CString sResult = "";
	//0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤
	if(nResult == 0)
	{
		//此项不召唤
		sResult = "此项不召唤";
	}
	else if(nResult == 1)
	{
		//成功
		sResult = "召唤成功";
	}
	else if(nResult == 2)
	{
		//召唤失败
		sResult = "召唤失败";
	}
	else if(nResult == 3)
	{
		//装置不支持
		sResult = "装置不支持";
	}
	else if(nResult == 4)
	{
		//指定时间内无事件
		sResult = "召唤成功但指定时间内无数据";
	}
	else
	{
		sResult = "尚未召唤";
	}
	return sResult;
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：响应列表个性化显示消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB301C5
void CBatchDetailView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				if(nSubItem == 2 || nSubItem == 5 || nSubItem == 8 || nSubItem == 11 || nSubItem == 14 || nSubItem == 17 || nSubItem == 20)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					
					if(str == "不召唤")
					{
						lplvcd->clrText = g_BatchNotCall;
					}
					if(str == "召唤")
					{
						lplvcd->clrText = g_BatchCall;
					}
					if(str == "不支持")
					{
						lplvcd->clrText = g_BatchNotSupport;
					}
				}
				//结果列
				if(nSubItem == 3 || nSubItem == 6 || nSubItem == 9 || nSubItem == 12 || nSubItem == 15 || nSubItem == 18 || nSubItem == 21)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);

					if(str == "召唤成功")
					{
						lplvcd->clrText = g_BatchSuccess;
					}
					else if(str == "召唤失败")
					{
						lplvcd->clrText = g_BatchFail;
					}
					else if(str == "此项不召唤")
					{
						lplvcd->clrText = g_BatchNULL;
					}
					else if(str == "尚未召唤")
					{
						lplvcd->clrText = g_BatchLater;
					}
					else if(str == "装置不支持")
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

//##ModelId=49B87BB301F4
void CBatchDetailView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

	//CListCtrlEx先截取了WM_CONTEXTMENU消息, 处理完后再转完给所有者窗口
	//所以这里先得到消息参数再处理消息
	LPMSG  msg;
	msg = (LPMSG)GetCurrentMessage();
	UINT wParam = (UINT)msg ->wParam;
	if(wParam == LISTCTRLEX_MARK)
	{
		CPoint * ptTemp = (CPoint*)msg->lParam;
		point = *ptTemp;
		
		//判断是右击在列头还是列表,只处理列表,列头在CListCtrlEx中处理
		CPoint pt = point;
		m_List.ScreenToClient(&pt);
		
		CWnd* pChildWnd = m_List.ChildWindowFromPoint(pt);
		if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() != m_List.GetSafeHwnd())
		{
			char szClass [50];
			
			// Verify that this window handle is indeed the header
			// control's by checking its classname.
			GetClassName(pChildWnd->GetSafeHwnd(), szClass, 50);
			if (!lstrcmp (szClass, "SysHeader32"))
			{
			}
		}
		else if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() == m_List.GetSafeHwnd())
		{//点击的是ListCtrl的非列头部分
			//生成菜单
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_BATCH))
				return;
			pMenu = Menu.GetSubMenu(0);
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
		}
	}
}

/*************************************************************
 函 数 名：OnSTTP20154()
 功能概要：收到20154, 批量召唤回应报文
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB3036B
void CBatchDetailView::OnSTTP20154( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CBatchDetailView::OnSTTP20154 start");

	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//判断报文头
	if(pSttpData->sttp_head.uMsgID != 20154)
		return;

	//PT_ID
	CString PT_ID = pSttpData->sttp_body.ch_pt_id;
	//CPU
	int nCPU = pSttpData->sttp_body.nCpu;

	//查找对应的选择项
	CBatchSel* pSel = FindMapBatchSel(PT_ID, nCPU);
	if(pSel == NULL)
	{
		CString str;
		str.Format("批量召唤--查找不到%s装置%dCPU对应的项", PT_ID, nCPU);
		WriteLog(str);
		return;
	}
	CString str;
	str = pSel->m_pObj->m_sName;
	CString strOut;
	if(!AddToComplete(pSel))
	{
		strOut.Format("批量召唤---装置%s的%d号CPU重复报文, 不处理", str, nCPU);
		return;
	}
	WriteLog("20154报文内容");
	//更新值
	pSel->m_nZone = pSttpData->sttp_body.nZone;
	pSel->m_nSetting = pSttpData->sttp_body.nMsgId;
	pSel->m_nDigital = pSttpData->sttp_body.nStatus;
	pSel->m_nSoftBoard = pSttpData->sttp_body.nChangeConfigType;
	pSel->m_nAnalog = pSttpData->sttp_body.nChangeInfoType;
	pSel->m_nZoneNote = pSttpData->sttp_body.nEventType;
	pSel->m_nSettingNote = pSttpData->sttp_body.nCmdSource;
	pSel->m_nDigitalNote = pSttpData->sttp_body.nFlag;
	pSel->m_nAnalogNote = pSttpData->sttp_body.nSource;
	pSel->m_nOsc = atoi( pSttpData->sttp_body.ch_station_id );
	WriteLog(pSttpData->sttp_body.ch_station_id);
	pSel->m_nOscNote = atoi( pSttpData->sttp_body.ch_HandlerName );
	WriteLog(pSttpData->sttp_body.ch_HandlerName);
	pSel->m_nHistory = atoi( pSttpData->sttp_body.ch_version );
	WriteLog(pSttpData->sttp_body.ch_version);
	pSel->m_nHistoryNote = atoi( pSttpData->sttp_body.ch_time_20_BIT1 );
	WriteLog(pSttpData->sttp_body.ch_time_20_BIT1);

	str = pSttpData->sttp_body.ch_check_code;
	pSel->m_nSoftBoardNote = atoi(str);
	//改变值显示
	ChangeValueCalling(pSel);
	
	str = pSel->m_pObj->m_sName;
	strOut.Format("批量召唤---装置%s的%d号CPU召唤完成", str, nCPU);
	WriteLog(strOut);

	if (m_mapComplete.GetCount() == m_mapSave.GetCount())
	{
		WriteLog("CBatchDetailView::OnSTTP20154 批量召唤完成");
		//通知设置页面
		GetParentFrame()->PostMessage(BATCHCALL_END);
		AfxMessageBox("批量召唤完毕");
	}
}

/*************************************************************
 函 数 名：ChangeValueCalling()
 功能概要：在召唤时改变选择项的显示
 返 回 值: 
 参    数：param1	指定选择项
		   Param2	指定在列表框中的位置,可为-1,表示未知位置,函数自己会查找
**************************************************************/
//##ModelId=49B87BB2035B
int CBatchDetailView::ChangeValueCalling( CBatchSel* pSel, int nIndex /*= -1*/ )
{
	//检查数据有效性
	if(pSel == NULL)
		return -1;
	if(nIndex < 0)
	{
		//查找在列表框中的位置
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pSel;
		nIndex = m_List.FindItem(&lvFind);
		if(nIndex < 0)
			return nIndex;
	}
	//设置值
	//0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤
	//3.定值区号
	m_List.SetItemText(nIndex, 3, GetResult(pSel->m_nZone));
	//4.定值区号结果说明
	CString str;
	if(pSel->m_nZone == 2) //失败
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote);
		m_List.SetItemText(nIndex, 4, str);

	}
	else
	{
		m_List.SetItemText(nIndex, 4, "-");
	}
	//6.定值
	m_List.SetItemText(nIndex, 6, GetResult(pSel->m_nSetting));
	//7:定值结果说明
	if(pSel->m_nSetting == 2) //失败
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote);
		m_List.SetItemText(nIndex, 7, str);
	}
	else
	{
		m_List.SetItemText(nIndex, 7, "-");
	}
	//9.开关量
	m_List.SetItemText(nIndex, 9, GetResult(pSel->m_nDigital));
	//10: 开关量结果说明
	if(pSel->m_nDigital == 2) //失败
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote);
		m_List.SetItemText(nIndex, 10, str);
	}
	else
		m_List.SetItemText(nIndex, 10, "-");
	//12.软压板
	m_List.SetItemText(nIndex, 12, GetResult(pSel->m_nSoftBoard));
	//13:软压板结果说明
	if(pSel->m_nSoftBoard == 2)//失败
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote);
		m_List.SetItemText(nIndex, 13, str);
	}
	else
		m_List.SetItemText(nIndex, 13, "-");
	
	//15.模拟量
	m_List.SetItemText(nIndex, 15, GetResult(pSel->m_nAnalog));
	//16:模拟量结果说明
	if(pSel->m_nAnalog == 2) //失败
	{
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote);
		m_List.SetItemText(nIndex, 16, str);
	}
	else
		m_List.SetItemText(nIndex, 16, "-");
	
	//18: 录波结果
	str = GetResult(pSel->m_nOsc);
	m_List.SetItemText(nIndex, 18, str);
	//19:录波结果说明
	if(pSel->m_nOsc == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 19, str);
	
	
	//21: 历史事件结果
	str = GetResult(pSel->m_nHistory);
	m_List.SetItemText(nIndex, 21, str);
	//22:历史事件结果说明
	if(pSel->m_nHistory == 2)
		str = getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote);
	else
		str = "-";
	m_List.SetItemText(nIndex, 22, str);

	
	m_List.Update(nIndex);

	return nIndex;
}

/*************************************************************
 函 数 名：AddToComplete()
 功能概要：把指定选择项加入到已召唤完成链表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1	指定选择项
		   Param2
**************************************************************/
//##ModelId=49B87BB30003
BOOL CBatchDetailView::AddToComplete( CBatchSel* pSel )
{
	//检查数据有效性
	if(pSel == NULL)
		return FALSE;
	//查找是否已在链表中

	CBatchSel* pFind = NULL;
	CString strKey;
	strKey.Format(pSel->m_pObj->m_sID + "cpu:%d", pSel->m_pCpu->code);
	m_mapComplete.Lookup(strKey, (void*&)pFind);
	if( pFind)
	{
		return FALSE;
	}
	m_mapComplete.SetAt(strKey, pSel);
	
	return TRUE;
}


/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果.1, 0, -1
 参    数：param1 比较对象1
		   Param2 比较对象2
		   param3 比较参数
**************************************************************/
//##ModelId=49B87BB20128
int CALLBACK CBatchDetailView::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CBatchSel* pResult1 = (CBatchSel*)lParam1;
	CBatchSel* pResult2 = (CBatchSel*)lParam2;
	if(pResult2 == NULL || pResult1 == NULL)
		return 0;
	if(pResult1->m_pObj == NULL || pResult2->m_pObj == NULL)
		return 0;
	int nCol = (int)lParamData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation1 = NULL;
	
	CStationObj* pStation2 = NULL;
	
	
	CString str1, str2;
	int iResult = 0;
	//"二次设备", "CPU号", "定值区号结果", "定值区号结果说明", "定值", "定值结果", "定值结果说明", 
	//"开关量","开关量结果", "开关量结果说明", "软压板","软压板结果", "软压板结果说明", 
	//"模拟量","模拟量结果", "模拟量结果说明", "厂站名", "一次设备"
	switch(nCol)
	{
	case 0: //二次设备
		str1 = pResult1->m_pObj->m_sName;
		str2 = pResult2->m_pObj->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //CPU
		if(pResult1->m_pCpu != NULL && pResult2->m_pCpu != NULL)
			iResult = pResult1->m_pCpu->code - pResult2->m_pCpu->code;
		else
			iResult = 0;
		break;
	case 2: //定值区号是否召唤
		iResult = (pResult1->m_nCallZone)-(pResult2->m_nCallZone);
		break;
	case 3: //定值区号结果
		iResult = pResult1->m_nZone - pResult2->m_nZone;
		break;
	case 4: //定值区号结果说明
		iResult = pResult1->m_nZoneNote - pResult2->m_nZoneNote;
		break;
	case 5: //定值是否召唤
		iResult = (pResult1->m_nCallSetting)-(pResult2->m_nCallSetting);
		break;
	case 6:	//定值结果
		iResult = pResult1->m_nSetting - pResult2->m_nSetting;
		break;
	case 7: //定值结果说明
		iResult = pResult1->m_nSettingNote - pResult2->m_nSettingNote;
		break;
	case 8: //开关量是否召唤
		iResult = (pResult1->m_nCallDigital)-(pResult2->m_nCallDigital);
		break;
	case 9: //开关量结果
		iResult = pResult1->m_nDigital - pResult2->m_nDigital;
		break;
	case 10: //开关量结果说明
		iResult = pResult1->m_nDigitalNote - pResult2->m_nDigitalNote;
		break;
	case 11: //软压板是否召唤
		iResult = (pResult1->m_nCallSoftBoard)-(pResult2->m_nCallSoftBoard);
		break;
	case 12: //软压板结果
		iResult = pResult1->m_nSoftBoard - pResult2->m_nSoftBoard;
		break;
	case 13://软压板结果说明
		iResult = pResult1->m_nSoftBoardNote - pResult2->m_nSoftBoardNote;
		break;
	case 14: //模拟量是否召唤
		iResult = (pResult1->m_nCallAnalog)-(pResult2->m_nCallAnalog);
		break;
	case 15: //模拟量结果
		iResult = pResult1->m_nAnalog - pResult2->m_nAnalog;
		break;
	case 16: //模拟量结果说明
		iResult = pResult1->m_nAnalogNote - pResult2->m_nAnalogNote;
		break;
	case 17: //录波是否召唤
		iResult = (pResult1->m_nCallOsc)-(pResult2->m_nCallOsc);
		break;
	case 18: //录波结果
		iResult = pResult1->m_nOsc - pResult2->m_nOsc;
		break;
	case 19: //录波结果说明
		iResult = pResult1->m_nOscNote - pResult2->m_nOscNote;
		break;
	case 20: //历史事件是否召唤
		iResult = (pResult1->m_nCallHistory)-(pResult2->m_nCallHistory);
		break;
	case 21: //历史事件结果
		iResult = pResult1->m_nHistory - pResult2->m_nHistory;
		break;
	case 22: //历史事件结果说明
		iResult = pResult1->m_nHistoryNote - pResult2->m_nHistoryNote;
		break;
	case 23: //厂站名
		pStation1 = (CStationObj*)pApp->GetDataEngine()->FindDevice(pResult1->m_pObj->m_sStationID, TYPE_STATION);
		if(pStation1 != NULL)
		{
			str1 = pStation1->m_sName;
		}
		else
			str1 = "";
		pStation2 = (CStationObj*)pApp->GetDataEngine()->FindDevice(pResult2->m_pObj->m_sStationID, TYPE_STATION);
		if(pStation2 != NULL)
		{
			str2 = pStation2->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 24: //一次设备
		if(pResult1->m_pObj->m_pOwner != NULL)
		{
			str1 = pResult1->m_pObj->m_pOwner->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pObj->m_pOwner != NULL)
		{
			str2 = pResult2->m_pObj->m_pOwner->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iBatchViewAsc;
	return iResult;
}

//##ModelId=49B87BB302E0
void CBatchDetailView::OnColumnclickListBatchDetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iBatchViewAsc = - g_iBatchViewAsc;
	}
	else
	{
		g_iBatchViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);

	*pResult = 0;
}

void CBatchDetailView::OnShowResult( WPARAM wParam, LPARAM lParam )
{
	if (m_mapComplete.GetCount() != m_mapSave.GetCount())
	{
		if (AfxMessageBox("是否刷新批量召唤条目?", MB_YESNO) == IDNO)
		{
			return;
		}
	}
	m_mapComplete.RemoveAll();
	m_mapSave.RemoveAll();
	BATCHSEL_MAP* pMap = (BATCHSEL_MAP*)lParam;
	POSITION pos = pMap->GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = "";
	while (pos != NULL)
	{
		 pMap->GetNextAssoc(pos, strTemp, (void*&)pSel);
		 if (pSel == NULL)
		 {
			 continue;
		 }
		 if(pSel->m_bChecked)
		 {
			 ChangeSetting(pSel);
			 m_mapSave.SetAt(strTemp, pSel);
		 }
	}
	FillMap();
}


/*************************************************************
 函 数 名: FillList()
 功能概要: 填充列表
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchDetailView::FillMap()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	
	POSITION pos = m_mapSave.GetStartPosition();
	CBatchSel* pSel = NULL;
	CString strTemp = "";
	while (pos != NULL)
	{
		m_mapSave.GetNextAssoc(pos, strTemp, (void*&)pSel);
		if(pSel == NULL)
		{
			continue;
		}
		AddSelToList(pSel, 0);
	}
	m_List.SetRedraw(TRUE);
}


CBatchSel* CBatchDetailView::FindMapBatchSel( CString PT_ID, int nCpu )
{
	//没有选择项,返回NULL
	if(m_mapSave.GetCount() < 1)
		return NULL;

	CBatchSel* pSel = NULL;
	CString strKey;
	strKey.Format(PT_ID + "cpu:%d",  nCpu);
	m_mapSave.Lookup(strKey, (void*&)pSel);
	MYASSERT_NULL(pSel);
	//查找到
	return pSel;
}

void CBatchDetailView::ChangeSetting(CBatchSel* pSel)
{
	if (pSel->m_nZone != 3)
	{
		pSel->m_nZone = 9;
	}
	if (pSel->m_nSetting != 3)
	{
		pSel->m_nSetting = 9;
	}
	if (pSel->m_nDigital != 3)
	{
		pSel->m_nDigital = 9;
	}
	if (pSel->m_nSoftBoard != 3)
	{
		pSel->m_nSoftBoard = 9;
	}
	if (pSel->m_nAnalog != 3)
	{
		pSel->m_nAnalog = 9;
	}
	if (pSel->m_nOsc != 3 )
	{
		pSel->m_nOsc = 9;
	}
	if (pSel->m_nHistory != 3 )
	{
		pSel->m_nHistory = 9;
	}
}

void CBatchDetailView::OnBatchClose() 
{
	// TODO: Add your command handler code here
	//OnBtnClose();
	GetParentFrame()->SendMessage(BATCHCALL_END);
	GetParentFrame()->SendMessage(WM_CLOSE);
}


void CBatchDetailView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile("批量召唤结果");
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CBatchDetailView::OnBatchExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName = "批量召唤";
	strName.Format("批量召唤%s.xls", t.Format("%Y%m%d%H%M%S"));
	
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//导出成功
		if(AfxMessageBox("导出文件成功, 按[确定]打开文件目录", MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//导出失败
		AfxMessageBox("导出到文件失败");
	}
}

/****************************************************
Date:2014/1/2  Author:LYH
函数名:   WriteResultToExcel	
返回值:   BOOL	
功能概要: 将结果导出到Excel
参数: CString strFullPath	
*****************************************************/
BOOL CBatchDetailView::WriteResultToExcel( CString strFullPath )
{
	CDatabase database;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString strSQL,str;
	
	strSQL.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		strDriver, strFullPath, strFullPath);
	
	CFileFind find;
	BOOL IsFind = FALSE;
	IsFind = find.FindFile(strFullPath, 0);
	//"二次设备", "CPU号", "定值区号结果", "定值区号结果说明", "定值结果", "定值结果说明", "开关量结果", "开关量结果说明", "软压板结果", "软压板结果说明", "模拟量结果", "模拟量结果说明", "厂站名", "一次设备"
	try
	{
		if( database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{
			if(!IsFind)
			{
				//新建
				strSQL = "CREATE TABLE Log(二次设备 TEXT, CPU号 TEXT, 定值区号结果 TEXT, 定值区号结果说明 TEXT, 定值结果 TEXT, 定值结果说明 TEXT, 开关量结果 TEXT, 开关量结果说明 TEXT, 软压板结果 TEXT, 软压板结果说明 TEXT, 模拟量结果 TEXT, 模拟量结果说明 TEXT, 录波文件结果 TEXT,录波文件结果说明 TEXT,历史事件结果 TEXT,历史事件结果说明 TEXT,厂站名 TEXT, 一次设备 TEXT)";
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CBatchSel* pSel = (CBatchSel*)m_List.GetItemData(i);
				if(pSel)
				{
					AddSelToExcel(&database, pSel);
				}
			}
		}
		else
			return FALSE;
		database.Close();
	}
	catch(CDBException * e)
	{
		TRACE("数据库错误: " + e->m_strError);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2014/1/2  Author:LYH
函数名:   AddSelToExcel	
返回值:   BOOL	
功能概要: 
参数: CDatabase * pDatabase	
参数: CBatchSel * pSel	
*****************************************************/
BOOL CBatchDetailView::AddSelToExcel( CDatabase* pDatabase, CBatchSel* pSel )
{
	if(!pDatabase || !pSel)
		return FALSE;
	if(!pSel->m_pObj || !pSel->m_pCpu)
		return FALSE;
	CString strCPU = "";
	strCPU.Format("%d(%s)", pSel->m_pCpu->code, pSel->m_pCpu->des);
	
	//17: 厂站名
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CStationObj* pStation = (CStationObj*)pApp->GetDataEngine()->FindDevice(pSel->m_pObj->m_sStationID, TYPE_STATION);
	CString strStation = "";
	if(pStation != NULL)
		strStation = pStation->m_sName;
	//18: 一次设备名
	CString strDevice = "";
	if(pSel->m_pObj->m_pOwner  != NULL)
		strDevice = pSel->m_pObj->m_pOwner->m_sName;

	CString strSQL;
	strSQL.Format("INSERT INTO Log (二次设备, CPU号, 定值区号结果, 定值区号结果说明, 定值结果, 定值结果说明, 开关量结果, 开关量结果说明, 软压板结果, 软压板结果说明, 模拟量结果, 模拟量结果说明,录波文件结果,录波文件结果说明,历史事件结果,历史事件结果说明,厂站名, 一次设备) VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		pSel->m_pObj->m_sName, strCPU, 
		GetResult(pSel->m_nZone), pSel->m_nZone == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nZoneNote):"-", 
		GetResult(pSel->m_nSetting), pSel->m_nSetting == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSettingNote):"-", 
		GetResult(pSel->m_nDigital), pSel->m_nDigital == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nDigitalNote):"-", 
		GetResult(pSel->m_nSoftBoard), pSel->m_nSoftBoard == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nSoftBoardNote):"-", 
		GetResult(pSel->m_nAnalog), pSel->m_nAnalog == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nAnalogNote):"-", 
		GetResult(pSel->m_nOsc), pSel->m_nOsc == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nOscNote):"-", 
		GetResult(pSel->m_nHistory), pSel->m_nHistory == 2?getErrorMsg(BUSINESS_ERROR_START_NO + pSel->m_nHistoryNote):"-", 
		strStation, strDevice);
	pDatabase->ExecuteSQL(strSQL);
	return TRUE;
}
