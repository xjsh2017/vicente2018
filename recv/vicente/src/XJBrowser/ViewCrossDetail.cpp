// ViewCrossDetail.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewCrossDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/** @brief           升降序作用位*/
int g_iCrossDetailViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail

IMPLEMENT_DYNCREATE(CViewCrossDetail, CFormView)

CViewCrossDetail::CViewCrossDetail()
	: CFormView(CViewCrossDetail::IDD)
{
	//{{AFX_DATA_INIT(CViewCrossDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCrossDef = NULL;
	m_nOldSortCol = -1;
}

CViewCrossDetail::~CViewCrossDetail()
{
}

void CViewCrossDetail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCrossDetail)
	DDX_Control(pDX, IDC_LIST_CROSSDETAIL, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCrossDetail, CFormView)
	//{{AFX_MSG_MAP(CViewCrossDetail)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CROSSDETAIL, OnColumnclickListCrossdetail)
	//}}AFX_MSG_MAP
	ON_MESSAGE(VIEWCROSS_SELECTCHANGED, OnViewCrossSelChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail diagnostics

#ifdef _DEBUG
void CViewCrossDetail::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewCrossDetail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCrossDetail message handlers
/*************************************************************
 函 数 名: LoadListConfig()
 功能概要: 载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewCrossDetail::LoadListConfig()
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
			WriteLog("ViewCrossDetail::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewCrossDetail::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewCrossDetailConfig");

			//查找ViewCrossConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewCrossDetailConfig");
				WriteLog("ViewCrossDetail::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewCrossDetailConfig下的配置");
					WriteLog("ViewCrossDetail::读取UIConfig失败,将使用默认列风格");
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
BOOL CViewCrossDetail::SaveListConfig()
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
			WriteLog("ViewCrossDetail::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewCrossDetail::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCrossDetailConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCrossDetailConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCrossDetailConfig");
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
						WriteLog("未找到ViewCrossDetailConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewCrossDetailConfig下的配置, 先删除新建所有子节点");
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
BOOL CViewCrossDetail::NeedSave()
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
 函 数 名: InitListStyle()
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::InitListStyle()
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
		//char* arColumn[8]={"序号","数据点名称", "状态","数据点值","预警值", "二次设备","CPU","厂站名"};
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
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
				lvCol.cx = 100; //当前状态
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3://
				lvCol.cx = 100; //时间
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 4:
				lvCol.cx = 100; //数据点值
				colName = StringFromID(IDS_COMMON_VALUE);
				break;
			case 5:
				lvCol.cx = 100; //预警值
				colName = StringFromID(IDS_CROSSSTATUS_ALERT);
				break;
			case 6:
				lvCol.cx = 300; //二次设备
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 7:
				lvCol.cx = 80; //CPU
				colName = StringFromID(IDS_MODEL_CPU);
				break;
			case 8:
				lvCol.cx = 160; //厂站名
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
//	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名: RegulateControlSize()
 功能概要: 调整控件位置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewCrossDetail::RegulateControlSize()
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

void CViewCrossDetail::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//当窗口位置改变后改变控件位置
	RegulateControlSize();
}

/*************************************************************
 函 数 名: InitListCtrl()
 功能概要: 初始化列表
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	return 1;
}

void CViewCrossDetail::OnInitialUpdate() 
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

void CViewCrossDetail::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewCrossDetail::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	if(m_pCrossDef != NULL)
	{
		if(!m_pCrossDef->m_bLoadDetail)
		{
			//未载入详细信息,现在载入
			m_pCrossDef->LoadDetail();
		}
		POSITION pos = m_pCrossDef->m_listCross.GetHeadPosition();
		while(pos != NULL)
		{
			CCrossPoint* pEvent = (CCrossPoint*)m_pCrossDef->m_listCross.GetNext(pos);
			AddEventToList(pEvent, 0);
		}
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 函 数 名: AddEventToList()
 功能概要: 将指定预警数据点填充到列表
 返 回 值: 填充成功返回TRUE, 失败返回FALSE
 参    数: param1 指定预警数据点
		   Param2 指定在列表中的位置
**************************************************************/
BOOL CViewCrossDetail::AddEventToList( CCrossPoint * pEvent, int i /*= 0*/ )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	//"序号","数据点名称", "状态","数据点值","预警值", "二次设备","CPU","厂站名"
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:数据点名称
	m_List.SetItemText(i, 1, pEvent->m_sDataPointName);
	
	//2:当前状态
	m_List.SetItemText(i, 2, pEvent->GetStatusDes(pEvent->m_nCrossType));
	//3:数据点值
	m_List.SetItemText(i, 3, pEvent->m_sDataPointValue);
	//4:预警值
	m_List.SetItemText(i, 4, pEvent->m_sSectionValue);
	//5:二次设备
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 5, str);
	//6:CPU
	str = "";
	str.Format("%d", pEvent->m_nCPU);
	m_List.SetItemText(i, 6, str);
	//7:厂站
	str = "";
	if(pEvent->m_pStation != NULL)
		str = pEvent->m_pStation->m_sName;
	m_List.SetItemText(i, 7, str);
	//设置数据
	m_List.SetItemData(i, (DWORD)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名: OnViewCrossSelChanged()
 功能概要: 到预警视图选择改变的消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewCrossDetail::OnViewCrossSelChanged( WPARAM wParam, LPARAM lParam )
{
	CCrossDef* pDef = (CCrossDef*)lParam;
	if(pDef != m_pCrossDef)
	{
		m_pCrossDef = pDef;
		FillListData();
	}
}

/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用 
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CViewCrossDetail::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CCrossPoint* pEvent1 = (CCrossPoint*)lParam1;
	CCrossPoint* pEvent2 = (CCrossPoint*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","数据点名称", "状态","数据点值","预警值", "二次设备","CPU","厂站名"
	switch(nCol)
	{
	case 0: //序号
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //数据点名称
		str1 = pEvent1->m_sDataPointName;
		str2 = pEvent2->m_sDataPointName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //状态
		iResult = pEvent1->m_nCrossType - pEvent2->m_nCrossType;
		break;
	case 3: //数据点值
		str1 = pEvent1->m_sDataPointValue;
		str2 = pEvent2->m_sDataPointValue;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //预警值
		str1 = pEvent1->m_sSectionValue;
		str2 = pEvent2->m_sSectionValue;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //二次设备
		str1 = "";
		if(pEvent1->m_pSec != NULL)
			str1 = pEvent1->m_pSec->m_sName;
		str2 = "";
		if(pEvent2->m_pSec != NULL)
			str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //CPU
		iResult = pEvent1->m_nCPU - pEvent2->m_nCPU;
		break;
	case 7: //厂站名
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
	iResult *= g_iCrossDetailViewAsc;
	return iResult;
}

void CViewCrossDetail::OnColumnclickListCrossdetail(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iCrossDetailViewAsc = - g_iCrossDetailViewAsc;
	}
	else
	{
		g_iCrossDetailViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewCrossDetail::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_CROSSSTATUS_ALERT));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
