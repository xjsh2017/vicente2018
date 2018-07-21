// ViewCross.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewCross.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iCrossViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewCross

IMPLEMENT_DYNCREATE(CViewCross, CFormView)

CViewCross::CViewCross()
	: CFormView(CViewCross::IDD)
{
	//{{AFX_DATA_INIT(CViewCross)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_listCross.RemoveAll();
	m_nOldSortCol = -1;
	g_iCrossViewAsc = -1;
}

CViewCross::~CViewCross()
{
	Clear();
}

void CViewCross::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCross)
	DDX_Control(pDX, IDC_LIST_CROSS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCross, CFormView)
	//{{AFX_MSG_MAP(CViewCross)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_CROSS, OnClickListCross)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CROSS, OnItemchangedListCross)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CROSS, OnColumnclickListCross)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20159, OnSTTP20159)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCross diagnostics

#ifdef _DEBUG
void CViewCross::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewCross::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

/*************************************************************
 函 数 名: InitListCtrl()
 功能概要: 初始化列表
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCross::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	if(!LoadDataFromDB())
	{
		WriteLog("预警视图读取数据失败", 1);
	}
	else
	{
		FillListData();
	}
	return 0;
}

/*************************************************************
 函 数 名: InitListStyle()
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCross::InitListStyle()
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
		//char* arColumn[6]={"序号","预警点名称","当前状态","状态改变时间", "数据类型","厂站名"};
		for (int nCol=0; nCol < 6 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; //序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 200; //名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 80; //当前状态
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3://
				lvCol.cx = 180; //时间
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 4:
				lvCol.cx = 100; //数据类型
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			case 5:
				lvCol.cx = 120; //厂站名
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			default:
				lvCol.cx=100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCross::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CCrossDef * pEvent = NULL;
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pEvent = (CCrossDef*)m_listCross.GetNext(pos);
		AddEventToList(pEvent, 0);
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 函 数 名: LoadListConfig()
 功能概要: 载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewCross::LoadListConfig()
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
			WriteLog("ViewCross::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewCross::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewCrossConfig");

			//查找ViewCrossConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewCrossConfig");
				WriteLog("ViewCross::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewCrossConfig下的配置");
					WriteLog("ViewCross::读取UIConfig失败,将使用默认列风格");
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

/*************************************************************
 函 数 名: SaveListConfig()
 功能概要: 保存列信息配置
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewCross::SaveListConfig()
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
			WriteLog("ViewCross::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewCross::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCrossConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewAlarmConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCrossConfig");
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
						WriteLog("未找到ViewCrossConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewCrossConfig下的配置, 先删除新建所有子节点");
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

/*************************************************************
 函 数 名: NeedSave()
 功能概要: 判断是否需要保存列表的列信息
 返 回 值: 需要保存返回TRUE,不需要保存返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewCross::NeedSave()
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

/*************************************************************
 函 数 名: AddEventToList()
 功能概要: 将指定预警填充到列表
 返 回 值: 填充成功返回TRUE, 失败返回FALSE
 参    数: param1 指定预警
		   Param2 指定在列表中的位置
**************************************************************/
BOOL CViewCross::AddEventToList( CCrossDef * pEvent, int i /*= 0*/ )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	//"序号","预警点名称","当前状态","状态改变时间", "数据类型","厂站名"
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:预警点名称
	m_List.SetItemText(i, 1, pEvent->m_sName);
	
	//2:当前状态
	m_List.SetItemText(i, 2, pEvent->GetStatusDes(pEvent->m_nCurStatus));
	//3:发生时间
	m_List.SetItemText(i, 3, pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	//4:数据类型
	m_List.SetItemText(i, 4, pEvent->GetDataTypeDes(pEvent->m_nDataType));
	//5:厂站名
	CString str = "";
	if(pEvent->m_pStation != NULL)
		str = pEvent->m_pStation->m_sName;
	m_List.SetItemText(i, 5, str);
	//设置数据
	m_List.SetItemData(i, (DWORD)(pEvent));
	return TRUE;
}

void CViewCross::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//设置可滚动视图的必要属性
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//初始化列表控件
	InitListCtrl();
}

void CViewCross::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//当窗口位置改变后改变控件位置
	RegulateControlSize();
}

/*************************************************************
 函 数 名: RegulateControlSize()
 功能概要: 当窗口位置改变后改变控件位置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewCross::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//调整列表框大小,占整个高度
	m_List.MoveWindow(&rc, TRUE);
}

void CViewCross::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CViewCross::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_CROSSSTATUS_ALERT));
	//GetParentFrame()->SetWindowText(" 预警 ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名: LoadDataFromDB()
 功能概要: 从数据库读取数据
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewCross::LoadDataFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//清除旧数据
	Clear();
	m_List.DeleteAllItems();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SECTION_COMB, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewCross::LoadDataFromDB, 查询失败");
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
			str.Format("CViewCross::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CCrossDef * pEvent = new CCrossDef;
			if(pEvent->LoadFromDB(pMemset))
			{
				//读取数据成功,加入到链表
				m_listCross.AddTail(pEvent);
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewCross::LoadDataFromDB,查询失败,原因为%s", sError);
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
 函 数 名: Clear()
 功能概要: 清除旧数据
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewCross::Clear()
{
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pDef = (CCrossDef*)m_listCross.GetNext(pos);
		if(pDef != NULL)
			delete pDef;
	}
	m_listCross.RemoveAll();
}

void CViewCross::OnClickListCross(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;   
    
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；  
	if(nSelected < 0)
	{	
		//父窗口是分割窗口,父窗口的父窗口是ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息,选择了空行,发送一个空值
		pWnd ->PostMessage(VIEWCROSS_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	*pResult = 0;
}

void CViewCross::OnItemchangedListCross(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//事实上鼠标点击的选择动作, 会产生三次ITEMCHANGED消息, 
		//1是A的文字背景消失触发，2是A取消选中触发，3是B被选中触发
		//为避免刷新太频繁, 这里只处理被选中消息, 这样处理会漏掉选择了空行的消息
		//所以把选择空行放在NM_CLICK中处理, 对于移动键盘上下键导致的选择到空行不做处理
		
		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
		if(nSelected < 0)
			return;
		
		//得到该行对应的事件
		CCrossDef * pEvent = (CCrossDef*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//父窗口是分割窗口,父窗口的父窗口是ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息
		pWnd ->PostMessage(VIEWCROSS_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用 
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CViewCross::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CCrossDef* pEvent1 = (CCrossDef*)lParam1;
	CCrossDef* pEvent2 = (CCrossDef*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","预警点名称","当前状态","状态改变时间", "数据类型","厂站名"
	switch(nCol)
	{
	case 0: //序号
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //预警点名称
		str1 = pEvent1->m_sName;
		str2 = pEvent2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //当前状态
		iResult = pEvent1->m_nCurStatus - pEvent2->m_nCurStatus;
		break;
	case 3: //状态改变时间
		str1 = pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //数据类型
		iResult = pEvent1->m_nDataType - pEvent2->m_nDataType;
		break;
	case 5: //厂站名
		str1 = "";
		if(pEvent1->m_pStation != NULL)
			str1 = pEvent1->m_pStation->m_sName;
		str2 = "";
		if(pEvent2->m_pStation != NULL)
			str2 = pEvent2->m_pStation->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iCrossViewAsc;
	return iResult;
}

void CViewCross::OnColumnclickListCross(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iCrossViewAsc = - g_iCrossViewAsc;
	}
	else
	{
		g_iCrossViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名: OnSTTP20159()
 功能概要: 响应预警结构通知报文
 返 回 值: 
 参    数: param1 
		   Param2 报文数据
**************************************************************/
void CViewCross::OnSTTP20159( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewCross::OnSTTP20159 start");
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//判断报文头
	if(pSttpData->sttp_head.uMsgID != 20159)
		return;

	//取得ID
	int nID = pSttpData->sttp_body.nFlag;
	//查找预警点
	CCrossDef* pDef = FindCross(nID);

	if(pDef != NULL)
	{
		//读取报文
		pDef->LoadFromSTTP(*pSttpData);
		//更新值
		pDef->ClearDetail();
		pDef->m_bLoadDetail = FALSE;
		UpdateCrossList(pDef);
	}
	else
	{
		CString str;
		str.Format("CViewCross::OnSTTP20159, 找不到ID为%d的预警点", nID);
		WriteLog(str, XJ_LOG_LV3);
	}

	WriteLog("CViewCross::OnSTTP20159 end");
}

/*************************************************************
 函 数 名: FindCross()
 功能概要: 查找指定ID的预警点
 返 回 值: 预警点
 参    数: param1 指定ID
		   Param2 
**************************************************************/
CCrossDef* CViewCross::FindCross( int nID )
{
	CCrossDef* pReturn = NULL;
	POSITION pos = m_listCross.GetHeadPosition();
	while(pos != NULL)
	{
		CCrossDef* pDef = (CCrossDef*)m_listCross.GetNext(pos);
		if(pDef == NULL)
			continue;
		if(pDef->m_nID == nID)
		{
			pReturn = pDef;
			break;
		}
	}
	return pReturn;
}

/*************************************************************
 函 数 名: UpdateCrossList()
 功能概要: 更新预警在列表中的显示
 返 回 值: 
 参    数: param1 指定预警
		   Param2 
**************************************************************/
void CViewCross::UpdateCrossList( CCrossDef* pDef )
{
	if(pDef == NULL)
		return;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pDef;
	int nIndex = m_List.FindItem(&lvFind);
	if(nIndex < 0)
		return;
	//改变值
	//2:当前状态
	m_List.SetItemText(nIndex, 2, pDef->GetStatusDes(pDef->m_nCurStatus));
	//3:发生时间
	m_List.SetItemText(nIndex, 3, pDef->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.Update(nIndex);
}
