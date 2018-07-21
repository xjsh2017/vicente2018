// DownOscView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownOscView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownOscView

IMPLEMENT_DYNCREATE(CDownOscView, CViewBase)

CDownOscView::CDownOscView()
	: CViewBase(CDownOscView::IDD)
{
	//{{AFX_DATA_INIT(CDownOscView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	m_nTimer = -1;
	m_nDownTimer = -1;
	m_pCurrentOsc = NULL;
}

CDownOscView::~CDownOscView()
{
}

void CDownOscView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownOscView)
	DDX_Control(pDX, IDC_LIST_DOWN_OSC, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownOscView, CViewBase)
	//{{AFX_MSG_MAP(CDownOscView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_DOWNOSC_DOWN, OnDownoscDown)
	ON_COMMAND(IDC_DOWNOSC_CANCEL, OnDownoscCancel)
	ON_COMMAND(IDC_DOWNOSC_OPEN, OnDownoscOpen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DOWN_OSC, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNOSC_FRAME_OPEN, OnDownOscOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownOscView diagnostics

#ifdef _DEBUG
void CDownOscView::AssertValid() const
{
	CViewBase::AssertValid();
}

void CDownOscView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownOscView message handlers
/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA301A5
BOOL CDownOscView::LoadListConfig()
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
			WriteLog("DownloadViewConfig::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("DownOscViewConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//DownOscViewConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到DownOscViewConfig");
				WriteLog("DownOscViewConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到DownOscViewConfig下的配置");
					WriteLog("DownOscViewConfig::读取UIConfig失败,将使用默认列风格");
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
// 	}	
// 	//关闭打开的COM
// 	CoUninitialize();
	return bResult;
}

//##ModelId=49B87BA301B5
BOOL CDownOscView::SaveListConfig()
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
			WriteLog("DownOscViewConfig::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("DownOscViewConfig::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//DownOscViewConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到DownOscViewConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("DownOscViewConfig");
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
						WriteLog("未找到DownOscViewConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了DownOscViewConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87BA301C5
BOOL CDownOscView::NeedSave()
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
 函 数 名: InitListStyle()
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CDownOscView::InitListStyle()
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
		//char* arColumn[5]={"序号", "名称","大小", "故障时间", "已下载"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 5 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80;
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 250; // 名称
				colName = StringFromID(IDS_COMMON_FILENAME);
				break;
			case 2://
				lvCol.cx = 100; // 大小
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 3://
				lvCol.cx = 200; // 故障时间
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4:
				lvCol.cx = 80;
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

void CDownOscView::OnInitialUpdate() 
{
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
	
	//初始化列表框
	InitListStyle();

	//启动定时器
	int nTimeOut = 5;
	m_nDownTimer = SetTimer(2, nTimeOut*1000, 0);
}

void CDownOscView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_DOWN_OSC, left_client, top_client, right_client, bottom_client);
}

void CDownOscView::OnDestroy() 
{
	//保存UI设置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名: AddEventToList()
 功能概要: 把一条事件的信息加入到列表
 返 回 值: 加入成功返回TRUE,失败返回FALSE
 参    数: param1 要加入的事件
		   Param2 加入的位置
**************************************************************/
BOOL CDownOscView::AddEventToList( CXJEventOSC * pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//"序号", "名称","大小", "故障时间", "已下载"
	
	CString str;
	str.Format("%d", pEvent->m_lID);
	//0:序号
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:名称
	m_List.SetItemText(i, 1, pEvent->m_sFileName);
	
	//2:大小
	str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(i, 2, str);
	
	//3:故障时间
	str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, str);
	
	//4:已下载
	m_List.SetItemText(i, 4, pEvent->GetDownloadString());
	
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名: AddEvent()
 功能概要: 添加指定事件到视图
 返 回 值: 
 参    数: param1 指定事件
		   Param2 
**************************************************************/
void CDownOscView::AddEvent( CXJEventOSC* pEvent )
{
	if(pEvent == NULL)
		return;
	//添加到链表
	m_EventManager.AddEvent(pEvent);
	//添加到列表框,最后一行
	int nCount = m_List.GetItemCount();
	AddEventToList(pEvent, nCount);
}

/*************************************************************
 函 数 名: OnDownOscOpen()
 功能概要: 响应页面打开消息，添加事件到本页面
 返 回 值: 
 参    数: param1 
		   Param2 录波事件
**************************************************************/
void CDownOscView::OnDownOscOpen( WPARAM wParam, LPARAM lParam )
{
	CXJEventOSC* pEvent = (CXJEventOSC*)lParam;
	if(pEvent == NULL)
		return;
	//复制事件
	CXJEventOSC* pOsc = (CXJEventOSC*)pEvent->Clone();
	CXJEventOSC* pExist = NULL;
	if(NeedAdd(pOsc, pExist))
	{
		//添加到本页面
		AddEvent(pOsc);
	}
	else
	{
		delete pOsc;
		pOsc = NULL;
		//重新下载已存在的事件
		if(pExist != NULL)
		{
			pExist->m_nDownloaded = 0;
			ChangeValue(pExist);
		}
	}
}

/*************************************************************
 函 数 名: DownloadOSCFile()
 功能概要: 下载指定录波列表的录波文件
 返 回 值: 
 参    数: param1 指定录波列表
		   Param2 
**************************************************************/
void CDownOscView::DownloadOSCFile( CXJEventOSC* pEvent )
{
	if(pEvent == NULL)
		return;

	//响应按钮按下操作, 下载录波文件或取消召唤
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	
	int nOffset = 0;
	
	CString strFilePath = pEvent->m_sFilePath;

	//加上/
	strFilePath.Replace("\\", "/");
	if(strFilePath.Right(1) != "/")
		strFilePath += "/";

	CString strFileName;
	if (g_role != MODE_SUB)
	{
		strFileName = strFilePath + pEvent->m_sFileName;
	}
	else
	{
		strFileName = pEvent->m_sFileName;
	}
	 
	//CString strFileName = pEvent->m_sFileName;
	//组织20042报文,从子站下载
	if(!pApp->m_SttpEngine.BuildDataFor20042DownloadWaveFile(sttpData, pEvent->m_sSecID, strFileName,  1, nOffset, pEvent->m_nFileNum))
	{
		//组织报文失败
		//发送召唤完成消息,结果为发送失败
		pEvent->m_nDownloaded = -1;
		ChangeValue(pEvent);
		m_pCurrentOsc = NULL;
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
			str.Format("%s发送召唤录波文件报文失败,原因为连接中断", pEvent->m_sSecID);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pEvent->m_nDownloaded = -1;
			ChangeValue(pEvent);
			m_pCurrentOsc = NULL;
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("%s发送召唤录波文件报文失败,原因为缓存已满", pEvent->m_sSecID);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pEvent->m_nDownloaded = -1;
			ChangeValue(pEvent);
			m_pCurrentOsc = NULL;
			return;
		}
	}
	else
	{
		pEvent->m_nDownloaded = 0;
		//启动定时器
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nWavFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//修改值
		ChangeDownloadedValue(pEvent, "0");
	}
}

/*************************************************************
 函 数 名：ChangeDownloadedValue()
 功能概要：修改正在下载的文件的已下载内容大小
 返 回 值: 
 参    数：param1	正在下载的文件
		   Param2	已下载内容大小
**************************************************************/
void CDownOscView::ChangeDownloadedValue( CXJEventOSC* pEvent, CString sValue )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	CString str;
	str.Format("%s K", sValue);
	//改变值
	m_List.SetItemText(nIndex, 4, str);
	//刷新
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：ChangeValue()
 功能概要：改变录波列表是否已下载的显示
 返 回 值: 
 参    数：param1	指定录波列表
		   Param2
**************************************************************/
//##ModelId=49B87B8F0292
void CDownOscView::ChangeValue( CXJEventOSC* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	//改变文件大小
	CString str;
	str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(nIndex, 2, str);
	//改变值
	m_List.SetItemText(nIndex, 4, pEvent->GetDownloadString());
	//刷新
	m_List.Update(nIndex);
}

void CDownOscView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		//召唤失败
		if(m_pCurrentOsc != NULL)
		{
			m_pCurrentOsc->m_nDownloaded = -1;
			ChangeValue(m_pCurrentOsc);
			m_pCurrentOsc = NULL;
		}
	}
	if(nIDEvent == m_nDownTimer)
	{
		if(m_pCurrentOsc == NULL) //当前空闲
		{
			//查看是否有在等待下载的事件
			for(int i = 0; i < m_EventManager.GetCount(); i++)
			{
				CXJEventOSC* pOsc = (CXJEventOSC*)m_EventManager.GetAtEvent(i);
				if(pOsc == NULL)
					continue;
				if(pOsc->m_nDownloaded == 0) //未下载
				{
					m_pCurrentOsc = pOsc;
					//下载
					DownloadOSCFile(m_pCurrentOsc);
					break;
				}
			}
		}
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名: OnSTTP20069()
 功能概要: 响应20069(召唤失败)通知
 返 回 值: 
 参    数: param1 
		   Param2 报文数据
**************************************************************/
void CDownOscView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;

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
		CString str;
		str.Format("CDownOscView::OnSTTP20069 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDownOscView::OnSTTP20069 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}

	//关闭计时器
	KillTimer(m_nTimer);
	//设置当前召唤失败
	m_pCurrentOsc->m_nDownloaded = -1;
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;

}

/*************************************************************
 函 数 名: OnSTTP20157()
 功能概要: 响应超时通知报文
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscView::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;

	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		WriteLog("CDownOscView::OnSTTP20157 报文为空", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20157报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		WriteLog("CDownOscView::OnSTTP20157 报文号错误, 非20157", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//不是本装置的报文
		WriteLog("CDownOscView::OnSTTP20157 不是本装置报文", XJ_LOG_LV3);
		return;
	}
	
	//召唤录波文件
	if(pSttpData->sttp_body.nMsgId != 20042)
	{
		WriteLog("CDownOscView::OnSTTP20157 报文号错误, 非20042", XJ_LOG_LV3);
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
	
	//正在召唤录波文件
	WriteLog("CDownOscView::OnSTTP20157 刷新超时时间", XJ_LOG_LV3);
	int nTimeOut = pData->m_nWavFileTimeOut;
	m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	//更新正在下载的文件的已下载内容值
	CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
	ChangeDownloadedValue(m_pCurrentOsc, sValue);
}

/*************************************************************
 函 数 名: OnSTTP20125()
 功能概要: 响应装置不支持通知
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscView::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20125报文
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//错误报文
		CString str;
		str.Format("CDownOscView::OnSTTP20125 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDownOscView::OnSTTP20125 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	
	//关闭计时器
	KillTimer(m_nTimer);
	//设置当前召唤失败
	m_pCurrentOsc->m_nDownloaded = -1;
	//更改显示
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;
}

void CDownOscView::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	if(m_pCurrentOsc == NULL)
		return;

	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20043报文
	if(pSttpData->sttp_head.uMsgID != 20043)
	{
		//错误报文
		CString str;
		str.Format("CDownOscView::OnSTTP20043 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pCurrentOsc->m_sSecID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDownOscView::OnSTTP20043 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	
	//检查是否当前要下载的文件
	CString strFileName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	//取得第一个待下载的录波列表
	strFileName.Replace("/", "\\");
	if(strFileName != m_pCurrentOsc->GetFileRelatePath())
	{
		//不是本录波列表的文件
		WriteLog("CDownOscView::OnSTTP20043 上送的录波文件名与本录波列表文件不匹配");
		CString str;
		str.Format("上送路径为%s, 本地路径为%s", strFileName, m_pCurrentOsc->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		//return;
	}
	
	m_pCurrentOsc->m_nDownloaded = pSttpData->sttp_body.nStatus;
	m_pCurrentOsc->m_nOffset = 0;
	if (g_role != MODE_SUB)
	{
		if(m_pCurrentOsc->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag > 0)
		{
			m_pCurrentOsc->m_nDownloaded = 2; //部分下载
			m_pCurrentOsc->m_nOffset = pSttpData->sttp_body.nFlag;
		}
		else if(m_pCurrentOsc->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag <= 0)
		{
			m_pCurrentOsc->m_nDownloaded = -1; //下在失败
		}
		if(m_pCurrentOsc->m_nDownloaded == 1 && m_pCurrentOsc->m_nFileSize == 0)
		{
			//下载成功，记录文件大小
			m_pCurrentOsc->m_nFileSize = pSttpData->sttp_body.nFlag;
		}
	}
	else
	{
		m_pCurrentOsc->m_nDownloaded = 1;
	}
	//关闭计时器
	KillTimer(m_nTimer);
	//更改显示
	ChangeValue(m_pCurrentOsc);
	m_pCurrentOsc = NULL;
	WriteLog("CDownOscView::OnSTTP20043 neat end", XJ_LOG_LV3);
}

void CDownOscView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_DOWNOSC))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{	
				pMenu->EnableMenuItem(IDC_DOWNOSC_DOWN, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_DOWNOSC_CANCEL, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_DOWNOSC_OPEN, MF_ENABLED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

void CDownOscView::OnDownoscDown() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
		return;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent == NULL)
				continue;
			if(pEvent == m_pCurrentOsc)
			{
				//是正在下载的项，不处理
			}
			else
			{
				//将结果标志改为未下载，让下次遍历时能找到
				pEvent->m_nDownloaded = 0;
				ChangeValue(pEvent);
			}
		}
	}
}

void CDownOscView::OnDownoscCancel() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
		return;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent == NULL)
				continue;
			if(pEvent == m_pCurrentOsc)
			{
				//是正在下载的项，发送取消报文
				CancelCall();
				m_pCurrentOsc = NULL;
			}
			else
			{
				//将结果标志改为取消下载，让下次遍历时找不到
				if(pEvent->m_nDownloaded == 0)
				{
					pEvent->m_nDownloaded = -2;
					ChangeValue(pEvent);
				}
			}
		}
	}
}

void CDownOscView::OnDownoscOpen() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_nDownloaded  != 1)
	{
		if(AfxMessageBox( StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_OKCANCEL)==IDOK)
		{
			//增加到下载页面
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->ShowDownOsc(pEvent);
		}
		return;
	}
	CString strFilePath = pEvent->GetFilePath();
	//打开文件
	OpenWaveFile(strFilePath, GetSafeHwnd());
}

/*************************************************************
 函 数 名: CancelCall()
 功能概要: 取消召唤操作
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDownOscView::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_pCurrentOsc == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	
	//正在下载录波文件
	//组建取消报文
	pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20042, m_pCurrentOsc->m_sSecID, 0);

	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_pCurrentOsc->m_nDownloaded = -2;
	ChangeValue(m_pCurrentOsc);
}

void CDownOscView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//按状态给定背景色
				if(nItem%2 != 0)
				{
					lplvcd->clrTextBk = RGB(204, 253, 251);
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
 CreateDate: 19:1:2010     Author:LYH
 函 数 名: NeedAdd()
 功能概要: 判断指定事件是否要加入到待下载列表
 返 回 值: 要加入返回TRUE, 不要加入返回FALSE
 参    数: param1 指定录波事件
		   Param2 
**************************************************************/
BOOL CDownOscView::NeedAdd( CXJEventOSC* pEvent, CXJEventOSC*& pExist )
{
	if(pEvent == NULL)
		return FALSE;
	pExist = NULL;
	BOOL bReturn = TRUE;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList == NULL)
		return FALSE;
	for(int i = 0; i < pList->GetSize(); i++)
	{
		CXJEventOSC* pOsc = (CXJEventOSC*)pList->GetAt(i);
		if(pOsc == NULL)
			continue;
		if(pEvent->m_lID >= 0) //有ID，比较ID
		{
			if(pOsc->m_lID == pEvent->m_lID)
			{
				pExist = pOsc;
				return FALSE;
			}
		}
		else
		{
			//新事件，没有ID，比较文件名
			if(pOsc->m_sFileName == pEvent->m_sFileName)
			{
				pExist = pOsc;
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CDownOscView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile( StringFromID(IDS_FUNC_OSC_DOWNLOAD));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
