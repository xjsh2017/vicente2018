// PTOSC.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTOSC.h"
#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           升降序作用位*/
int g_iPTOSCAsc;

UINT PTOSCLoad(LPVOID pParam)
{
	CPTOSC* ptOSC = (CPTOSC*)pParam;
	if(ptOSC == NULL)
		return 0;
	int nQueryNo = ptOSC->GetCurrentQueryNo();
	//读取内容
	if(ptOSC->IsCurrentQueryNo(nQueryNo) && !ptOSC->m_bExitThread)
	{
		ptOSC->ReLoadData(nQueryNo);
		ptOSC->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
	}
	ptOSC->RemoveThreadNode(nQueryNo);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPTOSC

IMPLEMENT_DYNCREATE(CPTOSC, CViewBase)

//##ModelId=49B87B900002
CPTOSC::CPTOSC()
	: CViewBase(CPTOSC::IDD)
{
	//{{AFX_DATA_INIT(CPTOSC)
	m_sDirPath = _T("");
	m_bOnlyCFG = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B9000D0
CPTOSC::~CPTOSC()
{
	m_arrColum.RemoveAll();
	ClearOSCMsg();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B9000BB
void CPTOSC::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTOSC)
	DDX_Control(pDX, IDC_BTN_PTOSC_RF, m_btnRefur);
	DDX_Control(pDX, IDC_BTN_PTOSC_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_LIST_PTOSC, m_List);
	DDX_Text(pDX, IDC_STATIC_PTOSC_DIR, m_sDirPath);
	DDX_Check(pDX, IDC_CHECK_PTOSC_SHOW, m_bOnlyCFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTOSC, CViewBase)
	//{{AFX_MSG_MAP(CPTOSC)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTOSC, OnClickListPtosc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PTOSC, OnItemchangedListPtosc)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTOSC, OnColumnclickListPtosc)
	ON_BN_CLICKED(IDC_CHECK_PTOSC_SHOW, OnCheckPtoscShow)
	ON_BN_CLICKED(IDC_BTN_PTOSC_RF, OnBtnPtoscRf)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTOSC_OPEN, OnBtnPtoscOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTOSC, OnDblclkListPtosc)
	ON_BN_CLICKED(IDC_BTN_PTOSC_COPY, OnBtnPtoscCopy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTOSC, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9000DA
void CPTOSC::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B9000DC
void CPTOSC::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTOSC message handlers

//##ModelId=49B87B9000B0
void CPTOSC::OnInitialUpdate() 
{
	//记录控件位置
	//文件路径文本
	RecordRate(IDC_STATIC_PTOSC_DIR, 1, left_client, top_client);
	//刷新按钮
	RecordRate(IDC_BTN_PTOSC_RF, 0, left_client, top_client);
	//显示所有单选框
	RecordRate(IDC_CHECK_PTOSC_SHOW, 0, left_client, top_client);
	//查看波形按钮
	RecordRate(IDC_BTN_PTOSC_OPEN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSC_COPY, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTOSC_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSC_P2, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTOSC_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//初始化列表
	InitListCtrl();
	//初始化其它控件
	InitControls();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B90006F
int CPTOSC::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B900070
int CPTOSC::InitListStyle()
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
		//char* arColumn[4]={"序号","名称","大小", "修改时间"};
	
		//分配列宽,最小需求
		for (int nCol=0; nCol < 4; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 50; //序号
				colName = StringFromID( IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 200; // 名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 80; // 大小
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 3://
				lvCol.cx = 200; // 修改时间
				colName = StringFromID(IDS_COMMON_MODIFYTIME);
				break;
			default:
				lvCol.cx = 50;
				break;
			}
			lvCol.pszText = colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B90007E
BOOL CPTOSC::LoadListConfig()
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
			WriteLog("PTOSC::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("PTOSC::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTOSCConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTOSCConfig");
				WriteLog("PTOSC::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTOSCConfig下的配置");
					WriteLog("PTOSC::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B90008C
BOOL CPTOSC::SaveListConfig()
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
			WriteLog("PTOSC::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTOSC::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTOSCConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTOSCConfig");
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
						WriteLog("未找到PTOSCConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了PTOSCConfig下的配置, 先删除新建所有子节点");
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
 函 数 名：FillListData()
 功能概要：填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B90007D
int CPTOSC::FillListData(int nQueryNo)
{
	WriteLog("CPTOSC::FillListData, 开始填充数据");
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	//填充ListCtrl内容
	PT_OSC *pts = NULL;
	for(int i = 0; i < m_arrOSC.GetSize(); i ++)
	{
		if(m_bExitThread)
			break;
		if(!IsCurrentQueryNo(nQueryNo))
			break;

		CString str;
		pts = m_arrOSC.GetAt(i);
		pts->Code = i;
		str.Format("%d", pts->Code);
		//0.序号
		if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
			continue;
		//文件名
		m_List.SetItemText(i, 1, pts->FileName);
		//2.大小
		str.Format("%.0f Byte", pts->FileSize);
		m_List.SetItemText(i, 2, str);
		//3.时间
		str = pts->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		m_List.SetItemText(i, 3, str);
		m_List.SetItemData(i, (DWORD)pts);
	}

	LeaveCriticalSection(&m_CriticalOper);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	WriteLog("CPTOSC::FillListData, 填充数据完毕");
	
	return 0;
}

//##ModelId=49B87B90008D
BOOL CPTOSC::NeedSave()
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
 函 数 名：InitControls()
 功能概要：初始化其它控件, 设置初始值, 显示风格
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B90008E
int CPTOSC::InitControls()
{
	/*
	//打开按钮
	m_btnOpen.SetThemeHelper(&m_ThemeHelper);
//	m_btnOpen.SetIcon(IDI_WAVE);
	m_btnOpen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOpen.SetTooltipText(_T("察看选中文件的波形"));

	//刷新按钮
	m_btnRefur.SetThemeHelper(&m_ThemeHelper);
//	m_btnRefur.SetIcon(IDI_REFRESH);
	m_btnRefur.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnRefur.SetTooltipText(_T("刷新查找新文件"));
*/
	return 0;
}

/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置
 返 回 值: 返回值说明
**************************************************************/
/*void CPTOSC::RegulateControlSize()
{	

}*/


//##ModelId=49B87B9000EB
void CPTOSC::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
//	RegulateControlSize();
}

//##ModelId=49B87B9000FA
void CPTOSC::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTOSC, left_client, IDC_BTN_PTOSC_RF, right_client, bottom_client);
}

//##ModelId=49B87B9000FF
void CPTOSC::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：GetOSCFileDir()
 功能概要：得到录波文件录放目录
 返 回 值: 录波文件存放目录
**************************************************************/
//##ModelId=49B87B90008F
CString CPTOSC::GetOSCFileDir(BOOL bNewPath)
{
	MYASSERT_STRING(m_pObj);
	//得到基址
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath = strDownDir + m_pObj->m_sStationID;

	if (bNewPath)
	{
		strFullPath += "\\Collection";
	}

	if (m_pObj->m_nType == TYPE_WAVEREC)
	{
		strFullPath += "\\录波器\\";
	}
	else
	{
		strFullPath += "\\保护\\";
	}

	strFullPath += m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";

	return strFullPath;
}

/*************************************************************
 函 数 名：SearchOSCFiles()
 功能概要：搜索目录下所有录波文件
 返 回 值: 搜索成功返回TRUE, 失败返回FALSE
 参    数：param1   目录路径
**************************************************************/
//##ModelId=49B87B90009C
BOOL CPTOSC::SearchOSCFiles(CString& path, int nQueryNo)
{
	if(!IsCurrentQueryNo(nQueryNo) || m_bExitThread)
		return FALSE;
	CFileFind finder; //文件搜索类
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //所有文件
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		if(!IsCurrentQueryNo(nQueryNo) || m_bExitThread)
			break;
		bWorking = finder.FindNextFile();

		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // 是目录,进入遍历
		{
			CString str = finder.GetFilePath();
			SearchOSCFiles(str, nQueryNo);
		}
		else //是文件
		{
			PT_OSC * osc = new PT_OSC;
			osc->FileName = finder.GetFileName(); //文件名,带后缀
			//得到文件后缀
			CString strFileName = osc->FileName;
			int pos = strFileName.ReverseFind('.');
			if(pos != -1)
			{
				CString str = strFileName.Right(strFileName.GetLength() - pos -1);
				osc->FileType = str;
				if(!IsValidOSC(str))
				{
					delete osc;
					osc = NULL;
					continue;
				}
			}
			//得到文件大小
			CString strSize;
			DWORD dSize = finder.GetLength();
			float fSize = float(dSize);
			osc->FileSize = fSize;
			//最后修改时间
			CTime	tm;
			finder.GetLastWriteTime(tm);
			osc->ModifyTime = tm;
			//目录
			CString sDirPath = finder.GetFilePath();
			osc->DirPath = sDirPath.Left(sDirPath.GetLength() - osc->FileName.GetLength());

			EnterCriticalSection(&m_CriticalOper);
			m_arrOSC.Add(osc);
			LeaveCriticalSection(&m_CriticalOper);
		}
	}
	finder.Close();

	return TRUE;
}

/*************************************************************
 函 数 名：IsValidOSC()
 功能概要：判断某后缀名是否属于录波文件的后缀名
 返 回 值: 返回TRUE说明是录波文件,FALSE说明不是
 参    数：param1   指定后缀名
**************************************************************/
//##ModelId=49B87B90009E
BOOL CPTOSC::IsValidOSC(CString strName)
{
	if(m_bOnlyCFG)
	{
		if(strName != "cfg" && strName != "CFG")
			return FALSE;
	}
	else 
	{
		if(strName != "cfg" && strName != "dat" && strName != "hdr" && strName != "rev" && strName != "reh")
		{
			if(strName != "CFG" && strName != "DAT" && strName != "HDR" && strName != "REV" && strName != "REH")
				return FALSE;
		}
	}
	return TRUE;
}

//##ModelId=49B87B9000AB
BOOL CPTOSC::ClearOSCMsg()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrOSC.GetSize(); i++)
	{
		PT_OSC * osc = (PT_OSC*)m_arrOSC.GetAt(i);
		delete osc;
		osc = NULL;
	}
	m_arrOSC.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);

	return TRUE;
}

//##ModelId=49B87B900109
void CPTOSC::OnClickListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//##ModelId=49B87B90010D
void CPTOSC::OnItemchangedListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
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
		PT_OSC * pts = (PT_OSC*)m_List.GetItemData(nSelected);
		if(pts == NULL)
			return;
		//设置显示值
		m_sDirPath = pts->DirPath;
		UpdateData(FALSE);
	}

	*pResult = 0;
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页面打开消息, 刷新页面内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B90013A
void CPTOSC::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	MYASSERT(pObj);
	if(m_pObj == pObj)
	{
		return;
	}
	m_pObj = pObj;

	int nIndex = (int)wParam;

	//退出刚才的线程
// 	EndOwnerThread();
// 	m_bLoadData = FALSE;
// 	if(nIndex == PT_PROP_OSC)
// 	{
// 		ReFillAll();
// 	}

	if(m_pObj != NULL)
		m_sDirPath = GetOSCFileDir(FALSE);
	else
		m_sDirPath = "";
	UpdateData(FALSE);
	
	ReFillAll();
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87B90005E
int CALLBACK CPTOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	PT_OSC* pOSC1 = (PT_OSC*)lParam1;
	PT_OSC* pOSC2 = (PT_OSC*)lParam2;
	if(pOSC1 == NULL || pOSC2 == NULL)
		return 0;
	int nCol = (int)lParamData;

	CString str1, str2;
	int iResult = 0;
	//"名称","大小", "修改时间"
	switch(nCol)
	{
	case 0://序号
		iResult = pOSC1->Code - pOSC2->Code;
		break;
	case 1: //名称
		str1 = pOSC1->FileName;
		str2 = pOSC2->FileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //大小
		if(pOSC1->FileSize - pOSC2->FileSize < -0.00001)
			iResult = -1;
		else if(pOSC1->FileSize - pOSC2->FileSize > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 3: //修改时间
		str1 = pOSC1->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pOSC2->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTOSCAsc;
	return iResult;
}

//##ModelId=49B87B90011A
void CPTOSC::OnColumnclickListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTOSCAsc = - g_iPTOSCAsc;
	}
	else
	{
		g_iPTOSCAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B90011E
void CPTOSC::OnCheckPtoscShow() 
{
	// TODO: Add your control notification handler code here
	//更新数据
	UpdateData(TRUE);
	//删除数据,读取数据,填充数据
	ReFillAll();
}

//##ModelId=49B87B900128
void CPTOSC::OnBtnPtoscRf() 
{
	// TODO: Add your control notification handler code here
	//更新数据
	UpdateData(TRUE);
	//删除数据,读取数据,填充数据
	ReFillAll();
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：消息窗口关闭消息, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CPTOSC::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL CPTOSC::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTOSC_LOADING)->ShowWindow(SW_SHOW);


	m_List.DeleteAllItems();
	ClearOSCMsg();

	StartThread();
	
	return TRUE;
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CPTOSC::EndOwnerThread()
{
	WriteLog("开始退出保护录波页面读取数据线程", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护录波页面读取数据线程完毕", XJ_LOG_LV3);

}

//##ModelId=49B87B90012A
BOOL CPTOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTOSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B90012D
void CPTOSC::OnBtnPtoscOpen() 
{
	// TODO: Add your control notification handler code here

	WriteLog("点击了查看波形按钮, 检查所选录波", XJ_LOG_LV3);
	//检查数据有效性
	if(m_List.GetSafeHwnd() == NULL)
	{
		WriteLog("CPTOSC::OnBtnPtoscOpen 列表框对象无效", XJ_LOG_LV3);
		return;
	}
	//判断选择
	if(m_List.GetSelectedCount() < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
		return;
	}
	//只打开第一个选择的文件
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
	{
		WriteLog("CPTOSC::OnBtnPtoscOpen 未得到选中记录的信息", XJ_LOG_LV3);
		return;
	}
	//取得对象
	PT_OSC * pts = (PT_OSC*)m_List.GetItemData(nIndex);
	//打开文件
	WriteLog("开始打开录波文件", XJ_LOG_LV3);
	OpenOSCWaveFile(pts);
}

/*************************************************************
 函 数 名：OpenWaveFile()
 功能概要：打开波形文件
 返 回 值: 波形文件结构
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9000AC
void CPTOSC::OpenOSCWaveFile( PT_OSC* pts )
{
	//判断有效性
	if(pts == NULL)
	{
		return;
	}
	if(pts->FileName == "")
	{
		return;
	}
	//合并文件名
	CString sFullPath = GetOSCFileDir(FALSE) + pts->FileName;
	//打开文件
	OpenWaveFile(sFullPath, GetSafeHwnd());
}

//##ModelId=49B87B90012F
void CPTOSC::OnDblclkListPtosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//得到该行对应的对象
		PT_OSC* pts = (PT_OSC*)m_List.GetItemData(nSelected);
		OpenOSCWaveFile(pts);
	}
	*pResult = 0;
}

//##ModelId=49B87B9000CB
void CPTOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_LOCALFILE));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_LOCALFILE));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPTOSC::OnBtnPtoscCopy() 
{
	//取得所选文件
	CStringArray arrSelectFile;
	arrSelectFile.RemoveAll();
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			PT_OSC* pOSC = (PT_OSC*)m_List.GetItemData(nIndex);
			MYASSERT_CONTINUE(pOSC);
			CString strName = pOSC->FileName;
			//去后缀名
			int nFind = strName.ReverseFind('.');
			if(nFind >= 0)
			{
				strName = strName.Left(nFind);
			}
			//将文件名加入到数组
			BOOL bExist = FALSE;
			for(int k = 0; k < arrSelectFile.GetSize(); k++)
			{
				CString strValue = arrSelectFile.GetAt(k);
				if(strValue == strName)
				{
					bExist = TRUE;
					break;
				}
			}
			if(!bExist)
			{
				//添加
				arrSelectFile.Add(strName);
			}
		}
	}

	//是否有文件
	if(arrSelectFile.GetSize() <= 0)
		return;

	//取得用户选择文件夹
	CString strNewDir = GetSelectDir();
	if(strNewDir.IsEmpty())
	{
		return;
	}

	int total = 0;
	int fail = 0;
	//复制文件
	for(i = 0; i < arrSelectFile.GetSize(); i++)
	{
		CFileFind finder; //文件搜索类
		CString strPath;
		strPath = GetOSCFileDir(FALSE);
		if(strPath.Right(1) != "\\")
		{
			strPath += "\\";
		}
		CString strFilter;
		strFilter.Format("%s.*", arrSelectFile.GetAt(i));
		strPath += strFilter; //指定文件名
		BOOL bWorking = finder.FindFile(strPath);

		if (!bWorking)
		{
			strPath = GetOSCFileDir(TRUE);
			if(strPath.Right(1) != "\\")
			{
				strPath += "\\";
			}
			strFilter.Format("%s.*", arrSelectFile.GetAt(i));
			strPath += strFilter; //指定文件名
		    bWorking = finder.FindFile(strPath);			
		}

		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			
			// skip . and .. files;
			if (finder.IsDots())
			{
				continue;
			}
			else if(finder.IsDirectory()) // 是目录
			{
				continue;
			}
			else //是文件
			{
				CString strLog;
				total++;
				CString strName = finder.GetFileName(); //文件名,带后缀
				
				//目标路径
				CString strNewPath = strNewDir+strName;
				//目录
				CString strOldPath = finder.GetFilePath();
				
				//复制				
				
				if(CopyFile(strOldPath, strNewPath, TRUE))
				{
					strLog.Format("导出文件成功,源路径:%s, 目标路径:%s", strOldPath, strNewPath);
					WriteLog(strLog);
				}
				else
				{
					DWORD errcode = GetLastError();
					strLog.Format("导出文件失败,源路径:%s, 目标路径%s, errorcode:%d", strOldPath, strNewPath, errcode);
					WriteLog(strLog);
					fail++;
				}
			}
		}
		finder.Close();
	}

	CString strTip;
	strTip.Format("%s, %s:%d, %s:%d, %s:%d.\r\n%s", StringFromID(IDS_EXPORT_FINISHED), StringFromID(IDS_EVENTCOUNT_TOTAL),
		total, StringFromID(IDS_COMMON_SUCCESS),total-fail, StringFromID(IDS_COMMON_FAILED),fail, StringFromID(IDS_TIP_OPENDIR));
	//导出
	if(AfxMessageBox(strTip, MB_OKCANCEL) == IDOK)
	{
		ShellExecute(GetSafeHwnd(), "open", "Explorer.exe", strNewDir, NULL, SW_SHOW); 
	}
}

void CPTOSC::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				if(nItem  != -1)
				{
					//间隔
					if(nItem%2 != 0)
					{
						if (g_style == 1)
						{
							lplvcd->clrTextBk = g_ListSpaceColor2;
						}
						else if(g_style == 2)
						{
							lplvcd->clrTextBk = g_ListSpaceColor3;
						}
						else if(g_style == 3)
						{
							lplvcd->clrTextBk = g_ListSpaceColor4;
						}
						else
						{
							lplvcd->clrTextBk = g_ListSpaceColor;
						}
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

void CPTOSC::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTOSCLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("本次录波查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTOSC::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 1000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}

void CPTOSC::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("开始退出保护录波查询线程,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
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
	LeaveCriticalSection(&m_CriticalSection);
	
	str.Format("保护录波查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTOSC::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护录波查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTOSC::GetFirstNode()
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

BOOL CPTOSC::ReLoadData( int nQueryNo )
{
	CString sPath = GetOSCFileDir(FALSE);
	CString sNewPath = GetOSCFileDir(TRUE);

	CString strLog;
	strLog.Format("CPTOSC::ReLoadData, 开始搜索, 目录:%s", sPath);
	WriteLog(strLog);
	SearchOSCFiles(sPath, nQueryNo);
	strLog.Format("CPTOSC::ReLoadData, 搜索完毕, 目录:%s", sPath);
	WriteLog(strLog);
	strLog.Format("CPTOSC::ReLoadData, 开始搜索, 目录:%s", sNewPath);
	WriteLog(strLog);
	SearchOSCFiles(sNewPath, nQueryNo);
	strLog.Format("CPTOSC::ReLoadData, 搜索完毕, 目录:%s", sNewPath);
	WriteLog(strLog);

	return TRUE;
}

void CPTOSC::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
		return;
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTOSC_LOADING)->ShowWindow(SW_HIDE);
}

/****************************************************
Date:2014/1/11  Author:LYH
函数名:   GetThreadCount	
返回值:   int	
功能概要: 
*****************************************************/
int CPTOSC::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}
