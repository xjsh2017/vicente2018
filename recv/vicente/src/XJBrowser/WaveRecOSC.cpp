// WaveRecOSC.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecOSC.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           升降序作用位*/
int g_iWROSCAsc;

UINT WROSCLoad(LPVOID pParam)
{
	CWaveRecOSC* wrOSC = (CWaveRecOSC*)pParam;
	if(wrOSC == NULL)
		return 0;
	//读取内容
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//碰到退出线程,退出
		if(wrOSC->m_bExitThread)
			return 0;
		//超过重试次数
		if(nCount < 0)
			return 0;
		//读取完成, 退出
		if(bLoad)
		{
			//log
			WriteLog("录波器属性--录波页面, 读取内容结束, 正常退出线程!");
			return 0;
		}
		if(wrOSC->ReFillAll())
		{
			bLoad = TRUE;
		}
		else
		{
			//读取失败
			CString str;
			str.Format("wrOSC, 失败%d次", (4 - nCount));
			WriteLog(str);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC

IMPLEMENT_DYNCREATE(CWaveRecOSC, CViewBase)

//##ModelId=49B87B800169
CWaveRecOSC::CWaveRecOSC()
	: CViewBase(CWaveRecOSC::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecOSC)
	m_sDirPath = _T("");
	m_bOnlyCFG = FALSE;
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_nMaxRow = 100;
	m_arrOSC.RemoveAll();
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
}

//##ModelId=49B87B800207
CWaveRecOSC::~CWaveRecOSC()
{
	ClearOSCMsg();
}

//##ModelId=49B87B8001F4
void CWaveRecOSC::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecOSC)
	DDX_Control(pDX, IDC_LIST_WROSC, m_List);
	DDX_Control(pDX, IDC_BTN_WROSC_RF, m_btnRefur);
	DDX_Control(pDX, IDC_BTN_WROSC_OPEN, m_btnOpen);
	DDX_Text(pDX, IDC_STATIC_WROSC_DIR, m_sDirPath);
	DDX_Check(pDX, IDC_CHECK_WROSC_SHOW, m_bOnlyCFG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecOSC, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecOSC)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_WROSC, OnClickListWrosc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WROSC, OnItemchangedListWrosc)
	ON_BN_CLICKED(IDC_CHECK_WROSC_SHOW, OnCheckWroscShow)
	ON_BN_CLICKED(IDC_BTN_WROSC_RF, OnBtnWroscRf)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_WROSC, OnColumnclickListWrosc)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_WROSC_OPEN, OnBtnWroscOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WROSC, OnDblclkListWrosc)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B800213
void CWaveRecOSC::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B800215
void CWaveRecOSC::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecOSC message handlers

//##ModelId=49B87B8001E8
void CWaveRecOSC::OnInitialUpdate() 
{
	//记录控件位置
	//文件路径文本
	RecordRate(IDC_STATIC_WROSC_DIR, 1, left_client, null_effect, null_effect, bottom_client);
	//刷新按钮
	RecordRate(IDC_BTN_WROSC_RF, 0, null_effect, top_client, right_client);
	//显示所有单选框
	RecordRate(IDC_CHECK_WROSC_SHOW, 0, null_effect, top_client, right_client);
	//查看波形按钮
	RecordRate(IDC_BTN_WROSC_OPEN, 0, null_effect, null_effect, right_client, bottom_client);
	CViewBase::OnInitialUpdate();

	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();
	
	// TODO: Add your specialized code here and/or call the base class
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
//##ModelId=49B87B8001AB
int CWaveRecOSC::InitListCtrl()
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
//##ModelId=49B87B8001B5
int CWaveRecOSC::InitListStyle()
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
		char* arColumn[3]={"名称","大小", "修改时间"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 250; // 名称
				break;
			case 1://
				lvCol.cx = 160; // 大小
				break;
			case 2://
				lvCol.cx = 200; // 修改时间
				break;
			}
			lvCol.pszText=arColumn[nCol];
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
//##ModelId=49B87B8001C5
BOOL CWaveRecOSC::LoadListConfig()
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
			WriteLog("WaveRecOSC::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("WaveRecOSC::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//WaveRecOSCConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到WaveRecOSCConfig");
				WriteLog("WaveRecOSC::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到WaveRecOSCConfig下的配置");
					WriteLog("WaveRecOSC::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B8001C6
BOOL CWaveRecOSC::SaveListConfig()
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
			WriteLog("WaveRecOSC::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("WaveRecOSC::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//WaveRecOSCConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到WaveRecOSCConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("WaveRecOSCConfig");
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
						WriteLog("未找到WaveRecOSCConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了WaveRecOSCConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B8001B6
int CWaveRecOSC::FillListData()
{
	CString sPath = GetOSCFileDir();
	//先清空
	m_List.DeleteAllItems();
	ClearOSCMsg();
	SearchOSCFiles(sPath);
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//填充ListCtrl内容
	PT_OSC *pts = NULL;
	for(int i = 0; i < m_arrOSC.GetSize(); i ++)
	{
		pts = m_arrOSC.GetAt(i);
		if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, pts->FileName, 0, 0, 0, i) == -1)
			continue;
		CString str;
		str.Format("%.0f Byte", pts->FileSize);
		m_List.SetItemText(i, 1, str);
		str = pts->ModifyTime.Format("%Y-%m-%d %H:%M:%S");
		m_List.SetItemText(i, 2, str);
		m_List.SetItemData(i, (DWORD)pts);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8001C7
BOOL CWaveRecOSC::NeedSave()
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
//##ModelId=49B87B8001D4
int CWaveRecOSC::InitControls()
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

//##ModelId=49B87B800222
void CWaveRecOSC::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_WROSC, left_client, IDC_BTN_WROSC_RF, right_client, IDC_BTN_WROSC_OPEN);
}

//##ModelId=49B87B800232
void CWaveRecOSC::OnDestroy() 
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
//##ModelId=49B87B8001D5
CString CWaveRecOSC::GetOSCFileDir()
{
	if(m_pObj == NULL)
		return "";
	//得到基址
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath;
	strFullPath = strDownDir + m_pObj->m_sStationID + "\\录波器\\" + m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";
	return strFullPath;
}

/*************************************************************
 函 数 名：SearchOSCFiles()
 功能概要：搜索目录下所有录波文件
 返 回 值: 搜索成功返回TRUE, 失败返回FALSE
 参    数：param1   目录路径
**************************************************************/
//##ModelId=49B87B8001D6
BOOL CWaveRecOSC::SearchOSCFiles(CString& path)
{
	CFileFind finder; //文件搜索类
	CString strPath;
	strPath = path;
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*"; //所有文件
	BOOL bWorking = finder.FindFile(strPath);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files;
		if (finder.IsDots())
		{
			continue;
		}
		else if(finder.IsDirectory()) // 是目录,进入遍历
		{
			CString str = finder.GetFilePath();
			SearchOSCFiles(str);
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
		//	fSize /= 1024;
			osc->FileSize = fSize;
			//最后修改时间
			CTime	tm;
			finder.GetLastWriteTime(tm);
			osc->ModifyTime = tm;
			//目录
			CString sDirPath = finder.GetFilePath();
			osc->DirPath = sDirPath.Left(sDirPath.GetLength() - osc->FileName.GetLength());

			m_arrOSC.Add(osc);
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
//##ModelId=49B87B8001D8
BOOL CWaveRecOSC::IsValidOSC(CString strName)
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

/*************************************************************
 函 数 名：ClearOSCMsg()
 功能概要：清空录波文件信息
 返 回 值: 清空成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8001E5
BOOL CWaveRecOSC::ClearOSCMsg()
{
	for(int i = 0; i < m_arrOSC.GetSize(); i++)
	{
		PT_OSC * osc = (PT_OSC*)m_arrOSC.GetAt(i);
		delete osc;
		osc = NULL;
	}
	m_arrOSC.RemoveAll();

	return TRUE;
}

//##ModelId=49B87B800234
void CWaveRecOSC::OnClickListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//##ModelId=49B87B800238
void CWaveRecOSC::OnItemchangedListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
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
 函 数 名：OnWRFrameOpen()
 功能概要：响应录波器属性页面打开消息, 刷新界面
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B800255
void CWaveRecOSC::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	int nIndex = (int)wParam;

	//退出旧线程
// 	EndOwnerThread();
// 	//启动读取线程
// 	if(m_pLoadThread == NULL)
// 	{
// 		m_bExitThread = FALSE;
// 		m_pLoadThread = AfxBeginThread(WROSCLoad, this, THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
// 		if(m_pLoadThread != NULL)
// 		{
// 			m_pLoadThread->m_bAutoDelete = FALSE;
// 			m_pLoadThread->ResumeThread();
// 		}
// 	}
// 	else
// 	{
	m_bLoadData = FALSE;
	if(nIndex == WR_PROP_OSC)
	{
		ReFillAll();
	}
// 	}
	
}

//##ModelId=49B87B800245
void CWaveRecOSC::OnCheckWroscShow() 
{
	// TODO: Add your control notification handler code here
	//更新数据
	UpdateData(TRUE);
	//删除数据,读取数据,填充数据
	FillListData();
}

//##ModelId=49B87B800247
void CWaveRecOSC::OnBtnWroscRf() 
{
	// TODO: Add your control notification handler code here
	//更新数据
	UpdateData(TRUE);
	//删除数据,读取数据,填充数据
	FillListData();
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87B8001A6
int CALLBACK CWaveRecOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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
	case 0: //名称
		str1 = pOSC1->FileName;
		str2 = pOSC2->FileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //大小
		if(pOSC1->FileSize - pOSC2->FileSize < -0.00001)
			iResult = -1;
		else if(pOSC1->FileSize - pOSC2->FileSize > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 2: //修改时间
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
	iResult *= g_iWROSCAsc;
	return iResult;
}

//##ModelId=49B87B800249
void CWaveRecOSC::OnColumnclickListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iWROSCAsc = - g_iWROSCAsc;
	}
	else
	{
		g_iWROSCAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnWRFrameClose()
 功能概要：响应窗口关闭消息, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B800258
void CWaveRecOSC::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8001E7
BOOL CWaveRecOSC::ReFillAll()
{
	m_bLoadData = TRUE;
	FillListData();
	return TRUE;
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8001E6
void CWaveRecOSC::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B800252
BOOL CWaveRecOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_WROSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8001F7
void CWaveRecOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = "未知厂站";
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, "本地录波");
	}
	else
 		pApp->SetAppTile("未知装置", "本地录波");

	if(!m_bLoadData)
	{
		m_bLoadData = TRUE;
		//在此添加数据读入中等待窗口
		COXSplashWnd wndWait;
		wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
		wndWait.Show(SPLASH_NOTIMER);
		//未载入, 现在载入
		ReFillAll();
		wndWait.Hide();
	}
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CWaveRecOSC::OnBtnWroscOpen() 
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
		AfxMessageBox("你必须选中一条记录才能进行此操作！");
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
 函 数 名：OpenOSCWaveFile()
 功能概要：打开波形文件
 返 回 值: 
 参    数：param1	波形文件结构
		   Param2
**************************************************************/
void CWaveRecOSC::OpenOSCWaveFile( PT_OSC* pts )
{
	//判断有效性
	if(pts == NULL)
	{
		AfxMessageBox("此条记录中未包含相关文件信息，操作无法完成！");
		return;
	}
	if(pts->FileName == "")
	{
		AfxMessageBox("此条记录中未包含相关文件信息，操作无法完成！");
		return;
	}
	//合并文件名
	CString sFullPath = GetOSCFileDir() + pts->FileName;
	//打开文件
	OpenWaveFile(sFullPath, GetSafeHwnd());
}


void CWaveRecOSC::OnDblclkListWrosc(NMHDR* pNMHDR, LRESULT* pResult) 
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
