// WaveRecSetting.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecSetting.h"
#include "DlgSettingDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WRSettingLoad(LPVOID pParam)
{
	CWaveRecSetting* wrSetting = (CWaveRecSetting*)pParam;
	if(wrSetting == NULL)
		return 0;
	//读取内容
	BOOL bLoad = FALSE;
	int  nCount = PT_LOAD_COUNT;
	while(1)
	{
		//碰到退出线程,退出
		if(wrSetting->m_bExitThread)
			return 0;
		//超过重试次数
		if(nCount < 0)
			return 0;
		//读取完成, 退出
		if(bLoad)
		{
			//log
			WriteLog("录波器属性--定值页面, 读取内容结束, 正常退出线程!");
			return 0;
		}
		if(wrSetting->ReFillAll())
		{
			bLoad = TRUE;
		}
		else
		{
			//读取失败
			CString str;
			str.Format("wrSetting, 失败%d次", (4 - nCount));
			WriteLog(str);
			int nSec = PT_LOAD_SLEEP;
			Sleep(nSec * 1000);
			nCount--;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting

IMPLEMENT_DYNCREATE(CWaveRecSetting, CViewBase)

//##ModelId=49B87B7F0020
CWaveRecSetting::CWaveRecSetting()
	: CViewBase(CWaveRecSetting::IDD)
{
	//{{AFX_DATA_INIT(CWaveRecSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_arrSetting.RemoveAll();
	m_arrGroup.RemoveAll();
	m_nNewValueIndex = 4;
	m_nSrc = 0;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bCanGroup = FALSE;

	m_arrNewSetting.RemoveAll();
	m_arrFrame.RemoveAll();
	m_nFrameCount = 0;

	m_bExitThread = FALSE;
	m_pLoadThread = NULL;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
}

//##ModelId=49B87B7F038A
CWaveRecSetting::~CWaveRecSetting()
{
	ClearSettingConfig();
	ClearSettingGroup();
}

//##ModelId=49B87B7F033C
void CWaveRecSetting::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveRecSetting)
	DDX_Control(pDX, IDC_GIF_WRSET, m_gif);
	DDX_Control(pDX, IDC_TAB_WRSET, m_TabCtrl);
	DDX_Control(pDX, IDC_BTN_WRSET_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_WRSET_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_WRSET_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_WRSET, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveRecSetting, CViewBase)
	//{{AFX_MSG_MAP(CWaveRecSetting)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_WRSET_CALL, OnBtnWrsetCall)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_WRSET, OnSelchangeTabWrset)
	ON_CBN_SELCHANGE(IDC_CMB_WRSET_CPU, OnSelchangeCmbWrsetCpu)
	ON_CBN_SELCHANGE(IDC_CMB_WRSET_SRC, OnSelchangeCmbWrsetSrc)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_WRSET, OnCustomDraw)
	ON_BN_DOUBLECLICKED(IDC_BTN_WRSET_CALL, OnDoubleclickedBtnWrsetCall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WRSET, OnDblclkListWrset)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting diagnostics

#ifdef _DEBUG
//##ModelId=49B87B7F03A9
void CWaveRecSetting::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B7F03D8
void CWaveRecSetting::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaveRecSetting message handlers

//##ModelId=49B87B7F031C
void CWaveRecSetting::OnInitialUpdate() 
{
	//记录控件位置
	//上面一排
	RecordRate(IDC_STATIC_WRSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_WRSET_SRC, 0, left_client, top_client);
	
	RecordRate(IDC_STATIC_WRSET_CPU, 0, left_client, top_client);
	RecordRate(IDC_CMB_WRSET_CPU, 0, left_client, top_client);
	//TAB栏
	RecordRate(IDC_TAB_WRSET, 1, left_client, top_client);
	//下面一排按钮
	RecordRate(IDC_BTN_WRSET_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRSET_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_WRSET_STYLE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRSETTING_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_WRSETTING_P2, 0, left_client, top_client);
	RecordRate(IDC_GIF_WRSET, 0, null_effect, top_client, right_client, null_effect);
	RecordRate(IDC_STATIC_TIME_WRSETTING, 0, null_effect, top_client, right_client, null_effect);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
//	ResizeParentToFit();

	//初始化列表
	InitListCtrl();
	//初始化其它控件
	InitControls();
	//判断隐藏/显示 分组TAB栏
	if(g_PTSettingGroup != 1)
	{
		//隐藏
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_HIDE);
	}
	else
	{
		//显示
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_SHOW);
	}

	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

//##ModelId=49B87B80001F
void CWaveRecSetting::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(g_PTSettingGroup != 1) //定值不分组
		MoveCtrl(IDC_LIST_WRSET, left_client, IDC_BTN_WRSET_CALL, right_client, bottom_client);
	else //分组
	{
		MoveCtrl(IDC_LIST_WRSET, left_client, IDC_TAB_WRSET, right_client, bottom_client);
	
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.top -= 10;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}
	}
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B7F0109
int CWaveRecSetting::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
// 	if(g_PTSettingGroup == 1)
// 	{
// 		//定值组
// 		LoadPTGroupConfig();
// 	}
// 	//定值配置
// 	LoadPTSettingConfig();
// 	//定值值
// 	LoadPTHistorySettingData();

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B7F0128
int CWaveRecSetting::InitListStyle()
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
	lvCol.fmt=LVCFMT_LEFT;
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
		char* arColumn[4]={"序号","定值名称", "定值代码", "单位"};
		LV_COLUMN lvCol;
		lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM| LVS_SHOWSELALWAYS;
		lvCol.fmt=LVCFMT_LEFT;
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
		{
			lvCol.iSubItem=nCol;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // 序号
				break;
			case 1://
				lvCol.cx = 140; // 定值名称
				break;
			case 2://
				lvCol.cx = 140; // 定值代码
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=arColumn[nCol];
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(定值代码)
		m_List.SetColumnHide(2, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7F0167
BOOL CWaveRecSetting::LoadListConfig()
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
			WriteLog("WaveRecSetting::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("WaveRecSetting::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//WaveRecSettingConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到WaveRecSettingConfig");
				WriteLog("WaveRecSetting::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到WaveRecSettingConfig下的配置");
					WriteLog("WaveRecSetting::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B7F0196
BOOL CWaveRecSetting::SaveListConfig()
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
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", XJ_LOG_LV3);
			WriteLog("WaveRecSetting::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("WaveRecSetting::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//WaveRecSettingConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("WaveRecSettingConfig");
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
						WriteLog("未找到WaveRecSettingConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了WaveRecSettingConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B7F0148
int CWaveRecSetting::FillListData()
{
	//填充ListCtrl内容
	m_List.DeleteAllItems();
	PT_SETTING *pts = NULL;
	for(int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(g_PTSettingGroup == 1 && m_bCanGroup)
		{
			//按组显示
			if(pts->Group != m_nGroup)
			{
				//如果不等于当前所选组号
				continue;
			}
		}
		m_List.InsertItem(i, pts->ID); //ID
		m_List.SetItemText(i, 1, pts->Name); //名称
		m_List.SetItemText(i, 2, pts->CodeName); //代码
		m_List.SetItemText(i, 3, pts->Unit); //单位
	//	m_List.SetItemText(i, 3, pts->OrderValue); //基准值
	}

	return 0;
}

//##ModelId=49B87B7F01B5
BOOL CWaveRecSetting::NeedSave()
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
		for(int i = 0; i < m_nNewValueIndex; i++)
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
//##ModelId=49B87B7F01D4
int CWaveRecSetting::InitControls()
{
	/*
	//召唤定值按钮
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤定值"));

	//显示设置按钮
	m_btnStyle.SetThemeHelper(&m_ThemeHelper);
//	m_btnStyle.SetIcon(IDI_SEARCH);
	m_btnStyle.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStyle.SetTooltipText(_T("设置显示风格"));
*/
	//打印按钮
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
//	m_btnPrint.SetIcon(IDI_PRINT);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText(_T("打印"));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());

	return 0;
}


//##ModelId=49B87B80004E
void CWaveRecSetting::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：ClearSettingConfig()
 功能概要：清除定值配置信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7F01E4
BOOL CWaveRecSetting::ClearSettingConfig()
{
	//清除定值配置
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING * pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		delete pts;
		pts = NULL;
	}
	m_arrSetting.RemoveAll();

	return TRUE;
}

/*************************************************************
 函 数 名：LoadSettingConfig()
 功能概要：从数据库载入保护定值定义, 保存在定值链表中
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7F0203
BOOL CWaveRecSetting::LoadPTSettingConfig()
{
	WriteLogEx("CWaveRecSetting::LoadPTSettingConfig,查询开始");
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	//清除之前的配置
	if(!ClearSettingConfig())
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//与数据平台访问DLL的连接可用
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code, zone,103group查找
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//按setting_ID从小到大排列
		
	CString str;
		
	//字段
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Setting_ID", EX_STTP_DATA_TYPE_INT);

	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);

	//code_name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);

	//vpickList
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "vpicklist", EX_STTP_DATA_TYPE_STRING);

	//unit
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "unit", EX_STTP_DATA_TYPE_STRING);

	//s_precision
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "s_precision", EX_STTP_DATA_TYPE_STRING);

	//datatype
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "datatype", EX_STTP_DATA_TYPE_INT);
		
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	if(g_PTSettingGroup == 1 && m_bCanGroup)
	{
		//分组显示
		Condition condition5;
		str.Format("103GROUP = %d", m_nGroup);
		pApp->m_DBEngine.SetCondition(sql, condition5, str);
	}

	//按Setting_ID大小排序
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //非where条件	
		
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadPTSettingConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();

		CString str;
		str.Format("CWaveRecSetting::LoadPTSettingConfig,查询到%d条数据", nCount);
		WriteLog(str);
		for(int i = 0; i < nCount; i++)
		{
			//创建定值对象
			PT_SETTING * pts = new PT_SETTING;
			//setting_id,NAME,CODE_NAME,103group,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = pMemset->GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset->GetValue(1); //NAME
			pts->CodeName = pMemset->GetValue(2); //Code_Name
			pts->VPickList = pMemset->GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = pMemset->GetValue(4); //unit
			pts->Precision = pMemset->GetValue(5); //s_precision
			str = pMemset->GetValue(6); //datatype
			pts->DataType = atoi(str);

			//定值组
			if(g_PTSettingGroup == 1 && m_bCanGroup)
			{
				pts->Group = m_nGroup;
			}

			m_arrSetting.Add(pts);

			//查找基准值,录波器暂不支持
			pts->OrderValue = "";
		//	LoadOrderValue(pts);
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadPTSettingConfig,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
		
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//填充非Value数据
	FillListData();

	WriteLogEx("CWaveRecSetting::LoadPTSettingConfig,查询开始");

	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTGroupConfig()
 功能概要：载入保护定值组描述信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：要载入的定值组
**************************************************************/
//##ModelId=49B87B7F0213
BOOL CWaveRecSetting::LoadPTGroupConfig()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLogEx("CWaveRecSetting::LoadPTGroupConfig,查询开始");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//根据station_id, 103addr, cpu_code, 查找103group,name
	
	CString str;	
	//字段
	//id
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);
	
	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
	
	//条件
	//station_id
	Condition condition2;
	str.Format("STATION_ID = '%s'", m_pObj->m_sStationID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	//103addr
	Condition condition3;
	str.Format("103ADDR = %d", m_pObj->m_n103Addr);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
	
	//cpu
	Condition condition4;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_103GROUP_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CWaveRecSetting::LoadPTGroupConfig, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();

		CString str;
		str.Format("CWaveRecSetting::LoadPTGroupConfig,查询到%d条数据", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			CString str = pMemset->GetValue((UINT)0); //103group
			int nGroup = atoi(str);
			str = pMemset->GetValue(1); //name
			SetGroupName(nGroup, str);
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	WriteLogEx("CWaveRecSetting::LoadPTGroupConfig,查询正常结束");
	
	return bReturn;
}

/*************************************************************
 函 数 名：LoadOrderValue()
 功能概要：载入定值基准值信息
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	要载入的定值
**************************************************************/
//##ModelId=49B87B7F0232
BOOL CWaveRecSetting::LoadOrderValue(PT_SETTING* pts)
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//与数据平台访问DLL的连接可用
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	//根据PT_ID, CPU, Zone, ID(定值ID)查找Value(基准值),按Time从大到小排序,取最新一个
		
	CString str;	
	//字段
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "value", EX_STTP_DATA_TYPE_STRING);
		
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
		
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	//id
	Condition condition4;
	str.Format("ID = %s", pts->ID);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);

	//按时间排序
	Condition condition5;
	str = "order by TIME DESC";
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ST_ORDERSETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadOrderValue, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str = "";
		if(nCount > 0)
		{
			//取第一个
			str = pMemset->GetValue((UINT)0);
			pts->OrderValue = str;
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadOrderValue,查询失败,原因为%s", sError);
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
 函 数 名：AddSettingValue()
 功能概要：插入某项定值到列表
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1   CString		定值ID
		   param2   CString		定值值
		   param3   int			要插入的列
**************************************************************/
//##ModelId=49B87B7F0251
BOOL CWaveRecSetting::AddSettingValue( CString ID, CString VALUE, int iCol )
{
	//先找到Setting_ID==ID的索引
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	LVFINDINFO  fi;
	fi.flags = LVFI_STRING;
	fi.psz = strID;
	
	//查找此值在第几行
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return FALSE;
	
	//设置值
	PT_SETTING *pst = m_arrSetting.GetAt(nItem);
	pst->Value = VALUE;
	
	//设置ListCtrl的Value列内容
	bReturn = m_List.SetItemText(nItem, iCol/*第三列为最新值*/, pst->Value);
	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTHistorySettingData()
 功能概要：载入保护历史值
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7F0261
BOOL CWaveRecSetting::LoadPTHistorySettingData()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;
	//清除先前添加的列
	CHeaderCtrl *pHC = m_List.GetHeaderCtrl();
	int iColCount = pHC->GetItemCount();
	for (int z = 0; z < iColCount - m_nNewValueIndex; z ++)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CStringArray arrTime;
	arrTime.RemoveAll();
	//与数据平台访问DLL的连接可用
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//取得数据库内有历史数据的Time列表
	CString str;
		
	//字段
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);

	//CURTIME
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "CURTIME";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field1);
		
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	//按时间大小排序
	Condition condition3;
	str.Format("order by CURTIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//非where
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadPTHistorySettingData, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		int iItem = 0;
		for(int i = 0; i < nCount; i++)
		{
			if(iItem >= g_PTSettingValues)
				break;

			CString str;
			str = pMemset->GetValue((UINT)0);
			arrTime.InsertAt(0, str);
			iItem++;
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CWaveRecSetting::LoadPTHistorySettingData,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//添加每一列
	//从数据库读出每个Time对应的定值组
	for (int i = 0; i < arrTime.GetSize(); i ++)
	{
		CString strTimeDisplay;
		CString strTime = arrTime.GetAt(i);
		if(strTime.ReverseFind('.') != -1)
		{
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		}
		else
		{
			strTimeDisplay = strTime;
		}
		
		//给每个Time添加一列
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
		
		if(-1 == Index)
			continue;
		//从数据库读出每个Time对应的开关量组
		//与数据平台访问DLL的连接可用
		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//取得数据库内有历史数据的Time列表
		CString str;
			
		//字段
		//setting_id
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "setting_id", EX_STTP_DATA_TYPE_INT);

		//VALUE
		Field Field2;
		pApp->m_DBEngine.SetField(sql, Field2, "Value", EX_STTP_DATA_TYPE_STRING);
			
		//条件
		//PT_ID
		Condition condition1;
		str.Format("PT_ID = '%s'", m_pObj->m_sID);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);

		//cpu_code
		Condition condition2;
		str.Format("CPU_CODE = %s", m_sCPU);
		pApp->m_DBEngine.SetCondition(sql, condition2, str);

		//CURTIME
		Condition condition4;
		str.Format("CURTIME = '%s'", strTime);
		pApp->m_DBEngine.SetCondition(sql, condition4, str);
				
		CMemSet* pMemset;
		pMemset = new CMemSet;
			
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, pMemset);
		}
		catch (...)
		{
			WriteLog("CWaveRecSetting::LoadPTHistorySettingData, 查询失败");
			delete[] sError;
			delete pMemset;
			return FALSE;
		}
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			int nItem = 0;
			for(int i = 0; i < nCount; i++)
			{
				CString strID, strValue;
				strID = pMemset->GetValue((UINT)0);
				strValue = pMemset->GetValue(1);

				BOOL bAddReturn = AddSettingValue(strID, strValue, Index);
				if(bAddReturn)
					nItem++;
				pMemset->MoveNext();
			}
			if(0 == nItem)
				m_List.DeleteColumn(Index);
		}
		else
		{
			CString str;
			str.Format("CWaveRecSetting::LoadPTHistorySettingData,查询失败,原因为%s", sError);
			WriteLog(str);
			bReturn = FALSE;
		}
			
		delete[] sError;
		delete pMemset;
		sError = NULL;
		pMemset = NULL;
	}
	return TRUE;
}

//##ModelId=49B87B80006D
void CWaveRecSetting::OnBtnWrsetCall() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤定值或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤定值
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20015报文
		//定值区号填0
		if(!pApp->m_SttpEngine.BuildDataFor20015PTSetting(sttpData, m_pObj->m_sID, atoi(m_sCPU), 0, m_nSrc))
		{
			//组织报文失败
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
				str.Format("保护%s发送录波器定值报文失败,原因为连接中断", m_pObj->m_sID);
				WriteLog(str);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护%s发送录波器定值报文失败,原因为缓存已满", m_pObj->m_sID);
				WriteLog(str);
			}
			AfxMessageBox("发送报文失败");
		}
		else
		{
			//成功,状态改为正在召唤定值
			m_nCurrentStatus = 1;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nWRSettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			CString str;
			str.Format("%s开始召唤定值，设置超时时间为%d", m_pObj->m_sName, nTimeOut);
			WriteLog(str);

			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//正在召唤定值
		//取消召唤
		CancelCall();
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
**************************************************************/
//##ModelId=49B87B7F0282
void CWaveRecSetting::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		SetDlgItemText(IDC_BTN_WRSET_CALL, "召唤定值");
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_WRSET_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_WRSET_STYLE) ->EnableWindow(TRUE);
		
		//下拉框
		GetDlgItem(IDC_CMB_WRSET_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_WRSET_SRC) ->EnableWindow(TRUE);
		//停止动画
		m_gif.Stop();
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		SetDlgItemText(IDC_BTN_WRSET_CALL, CANCEL_CALL);
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_WRSET_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_WRSET_STYLE) ->EnableWindow(FALSE);
		
		//下拉框
		GetDlgItem(IDC_CMB_WRSET_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_WRSET_SRC) ->EnableWindow(FALSE);
		//开始动画
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("用时%d秒", m_nOPTime);
		SetDlgItemText(IDC_STATIC_TIME_WRSETTING, str);
	}
}

/*************************************************************
 函 数 名：OnSTTP20016()
 功能概要：响应定值下载通知(20016)
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8000CC
void CWaveRecSetting::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤定值的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20016报文
	if(pSttpData->sttp_head.uMsgID != 20016)
	{
		//错误报文
		CString str;
		str.Format("CWaveRecSetting::OnSTTP20016 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CWaveRecSetting::OnSTTP20016 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}

	//关闭定时器
//	KillTimer(m_nTimer);
	
	//判断是单帧还是多帧
	if(pSttpData->sttp_head.uMsgType < 2)
	{
		//检查CPU
		int nCpu = pSttpData->sttp_body.nCpu;
		if(nCpu != atoi(m_sCPU))
		{
			//不是本CPU的报文
			CString str;
			str.Format("CWaveRecSetting::OnSTTP20016 收到错误CPU报文, CPU为%d", nCpu);
			WriteLog(str);
			return;
		}

		//单帧,直接处理
		//关闭定时器
		KillTimer(m_nTimer);
		
		//处理报文
		if(!LoadDataFromSTTP(pSttpData))
		{
			WriteLog("CPTSetting::OnSTTP20016 处理报文失败");
		}
		
		//处理完毕,改变状态和按钮文字
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
	}
	else if(pSttpData->sttp_head.uMsgType >= 2)
	{
		//多帧
		m_nFrameCount = pSttpData->sttp_head.uMsgType; //总帧数
		//当前帧序号
		int nFrame = pSttpData->sttp_head.uMsgLengthType;
		//判断合法性
		if(nFrame > m_nFrameCount)
		{
			//处理完毕,改变状态和按钮文字
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
			return;
		}
		//判断当前帧是否已收到过
		BOOL bFound = FALSE;
		for(int i = 0; i < m_arrFrame.GetSize(); i++)
		{
			int nTempFrame = m_arrFrame.GetAt(i);
			if(nTempFrame == nFrame)
			{
				//已存在
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			//已存在, 不处理此报文
			CString str;
			str.Format("收到定值多帧报文, 总帧数%d, 当前是第%d帧, 之前已收到过,不处理", m_nFrameCount, nFrame);
			WriteLog(str);
			return;
		}
		else
		{
			//之前不存在,处理
			CString str;
			str.Format("收到定值多帧报文, 总帧数%d, 当前是第%d帧", m_nFrameCount, nFrame);
			WriteLog(str);
			//保存序号
			m_arrFrame.Add(nFrame);
			//解析报文
			TranslateMultiFrame(pSttpData);
		}
	}
}

/*************************************************************
 函 数 名：LoadDataFromSTTP()
 功能概要: 从STTP报文中读取数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
**************************************************************/
//##ModelId=49B87B7F0280
BOOL CWaveRecSetting::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//解析定值数据
	try
	{
		//时间,取当前系统时间
		CTime tmTime = CTime::GetCurrentTime();
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//新加一列
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
		//读取每项值,加入到列表
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddSettingValue(id, Value, Index);
		}
	}
	catch (...)
	{
		WriteLog("CWaveRecSetting::LoadDataFromSTTP, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：ReSetGroupTab()
 功能概要：重新设置分组TAB栏
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F0290
void CWaveRecSetting::ReSetGroupTab()
{
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_WRSET);
	if(pTab == NULL)
		return;
	//清空原来的数据
	pTab->DeleteAllItems();
	
	if(g_PTSettingGroup != 1)
		return; //不按组显示, 不处理
	
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * ptg = (PT_GROUP*)m_arrGroup.GetAt(i);
		CString str; //格式: 组号-组名
		str.Format("%d-%s", ptg->Group, ptg->Name);
		pTab->InsertItem(i, str);
	}
	
	//设置默认
	if(pTab->GetItemCount() > 0)
	{
		pTab->SetCurSel(0);
		
		//保存当前选择
		SaveSelGroup(pTab);
	}
}

/*************************************************************
 函 数 名：SaveSelGroup()
 功能概要：保存TAB栏当前选择的信息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F029F
void CWaveRecSetting::SaveSelGroup( CTabCtrl* pTab )
{
	int nIndex = pTab->GetCurSel();
	TCITEM tc;
	char temp[256];
	for(int j = 0; j < 256; j++)
	{
		temp[j] = '\0';
	}
	tc.mask = TCIF_TEXT;
	tc.pszText = temp;
	tc.cchTextMax = 256;
	pTab->GetItem(nIndex, &tc);
	
	CString strGroup = tc.pszText;
	int pos = strGroup.Find('-', 0);
	if(pos != -1)
	{
		strGroup = strGroup.Left(pos);
	}
	m_nGroup = atoi(strGroup);
}

/*************************************************************
 函 数 名：ReFillCPU()
 功能概要：重新填充CPU下拉框
 返 回 值: void
 参    数：param1	分组TAB栏
		   Param2
**************************************************************/
//##ModelId=49B87B7F02AF
void CWaveRecSetting::ReFillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_CPU);
	if(pCmb == NULL)
		return;
	//清空
	pCmb->ResetContent();
	
	if(m_pObj == NULL)
		return;
	
	//CPU下拉框, 填充内容为"CPU号-描述"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU对象
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d-%s", pCpu->code, pCpu->des); //cpu号-描述
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//设置默认选择
	if(pCmb->GetCount() > 0)
	{
		pCmb->SetCurSel(0);
		//保存当前选择
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 函 数 名：SaveSelCPU()
 功能概要：保存当前选择的CPU号信息
 返 回 值: void
 参    数：param1	CPU下拉框
		   Param2
**************************************************************/
//##ModelId=49B87B7F02BF
void CWaveRecSetting::SaveSelCPU( CComboBox* pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return;
	CString strCPU;
	pCmb->GetLBText(nIndex, strCPU);
	//CPU显示的格式为: CPU号-CPU描述
	int pos = strCPU.Find('-', 0);
	if(pos != -1)
	{
		strCPU = strCPU.Left(pos);
	}
	m_sCPU = strCPU;
}

//##ModelId=49B87B80007D
void CWaveRecSetting::OnSelchangeTabWrset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//保存选择值
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_WRSET);
	if(pTab == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	SaveSelGroup(pTab);
	
	//重新载入数据
	LoadPTSettingConfig();
	LoadPTHistorySettingData();
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();

	*pResult = 0;
}

//##ModelId=49B87B80008C
void CWaveRecSetting::OnSelchangeCmbWrsetCpu() 
{
	// TODO: Add your control notification handler code here
	//取得当前值
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_CPU);
	if(pCmb == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	SaveSelCPU(pCmb);
	
	//载入配置和历史值
	if(g_PTSettingGroup == 1)
	{
		//按组显示,分组与CPU有关
		LoadSettingGroup();
		ReSetGroupTab();
	}
	LoadPTSettingConfig();
	LoadPTHistorySettingData();
// 	if(g_wndWait.GetSafeHwnd() != NULL)
// 		g_wndWait.Hide();
}

//##ModelId=49B87B80009C
void CWaveRecSetting::OnSelchangeCmbWrsetSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_SRC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	if(nIndex == 0 || nIndex == 1)
	{
		m_nSrc = nIndex;
	}
}

/*************************************************************
 函 数 名：OnWRFrameOpen()
 功能概要：响应录波器属性页面打开消息, 刷新界面值
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B8000DB
void CWaveRecSetting::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	
	int nIndex = (int)wParam;

	m_bLoadData = FALSE;
	if(nIndex == WR_PROP_SETTING)
	{
		ReFillAll();
	}

	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_WRSETTING, "");
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应操作失败报文(20069)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8000FA
void CWaveRecSetting::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤定值状态,不处理报文
		return;
	}
	
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
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//操作成功
		if(m_nCurrentStatus == 1)
		{
			//正在召唤定值
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	else
	{
		//操作失败
		//处理完毕,改变状态和按钮文字
		if(m_nCurrentStatus == 1)
		{
			//正在召唤定值
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
				
			CString str;
			str.Format("录波器 %s 召唤定值失败!", m_pObj->m_sID);
			WriteLog(str);
			AfxMessageBox("召唤定值失败!");
		}
	}
	
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应装置不支持功能报文(20125)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B800109
void CWaveRecSetting::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤开关量的状态,不处理报文
		return;
	}
	
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
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//装置不支持
	//处理完毕,改变状态和按钮文字
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
		AfxMessageBox("该装置不支持召唤定值!");
	}
}

//##ModelId=49B87B80009E
void CWaveRecSetting::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		//改变状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//提示
		AfxMessageBox("录波器召唤定值超时!");
	}
	
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("用时%d秒", m_nOPTime);
		SetDlgItemText(IDC_STATIC_TIME_WRSETTING, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：ClearMultiFrame()
 功能概要：清理处理多帧报文时的缓存数据
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02CE
void CWaveRecSetting::ClearMultiFrame()
{
	//总帧数,超时是要用总帧数来判断是否正在接收多帧报文
	m_nFrameCount = 0;
	//帧
	m_arrFrame.RemoveAll();
	//定值数据
	for(int i = 0; i < m_arrNewSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(i);
		delete pts;
	}
	m_arrNewSetting.RemoveAll();
}

/*************************************************************
 函 数 名：TranslateMultiFrame()
 功能概要：解析多帧的定值报文, 把数据保存在m_arrNewSetting中, 收全后统一处理
 返 回 值: void
 参    数：param1	当前帧报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B7F02CF
void CWaveRecSetting::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	
	if(pSttpData == NULL)
		return;
	
	//解析定值数据
	try
	{
		//检查CPU
		int nCpu = pSttpData->sttp_body.nCpu;
		if(nCpu != atoi(m_sCPU))
		{
			//不是本CPU的报文
			CString str;
			str.Format("CWaveRecSetting::OnSTTP20016 收到错误CPU报文, CPU为%d", nCpu);
			WriteLog(str);
		}
		else
		{
			//读取每项值,加入到新定值链表
			int nCount = pSttpData->sttp_body.variant_member.value_data.size();
			for(int i = 0; i < nCount; i++)
			{
				CString id;
				id.Format("%d", pSttpData->sttp_body.variant_member.value_data[i].id);
				CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
				Value = ComplexExchange(Value);
				//创建对象
				PT_SETTING* pts = new PT_SETTING;
				pts->ID = id;
				pts->Value = Value;
				m_arrNewSetting.Add(pts);
			}
		}
				
		//判断是否已收取完,序号数组里的元素数=总帧数就认为已收取完
		if(m_arrFrame.GetSize() == m_nFrameCount)
		{
			//收取完成,关闭定时器
			KillTimer(m_nTimer);
			//时间,取当前系统时间
			CTime tmTime = CTime::GetCurrentTime();
			CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
			//新加一列
			int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
			//加入到list显示
			for(int j = 0; j < m_arrNewSetting.GetSize(); j++)
			{
				PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(j);
				AddSettingValue(pts->ID, pts->Value, Index);
			}
			//清除缓存数据
			ClearMultiFrame();
			//处理完毕,改变状态和按钮文字
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, 错误", 3);
		return;
	}
}

/*************************************************************
 函 数 名：OnWRFrameClose()
 功能概要：响应窗口关闭消息,退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B80011C
void CWaveRecSetting::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B7F02FF
BOOL CWaveRecSetting::ReFillAll()
{
	m_bLoadData = TRUE;
	//载入配置和历史值
	//1.填充定值来源下拉框
	CComboBox * pCMB = (CComboBox*)GetDlgItem(IDC_CMB_WRSET_SRC);
	if(pCMB == NULL)
		return FALSE;
	pCMB->ResetContent();
	pCMB->AddString("录波器装置");
	pCMB->AddString("子站数据库");
	pCMB->SetCurSel(0);
	m_nSrc = 0;
	//2.填充CPU下拉框
	ReFillCPU();
	
	if(g_PTSettingGroup == 1)
	{
		//按组显示
		if(!LoadSettingGroup())
			return FALSE;
		ReSetGroupTab();
	}
	else 
	{
		m_bCanGroup = FALSE;
		ClearSettingGroup();
		ReSetGroupTab();
	}
	if(!LoadPTSettingConfig())
		return FALSE;
	/*
	if(!LoadPTHistorySettingData())
		return FALSE;
	*/
	return TRUE;
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消操作
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02DE
void CWaveRecSetting::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值

		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20015, m_pObj->m_sID, atoi(m_sCPU));
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B7F02DF
BOOL CWaveRecSetting::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20015 && m_nCurrentStatus == 1)
	{
		//召唤定值失败
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02EE
void CWaveRecSetting::EndOwnerThread()
{
	if(m_pLoadThread != NULL)
	{
		m_bExitThread = TRUE;
		WaitForSingleObject(m_pLoadThread->m_hThread, INFINITE);
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}

//##ModelId=49B87B8000AB
BOOL CWaveRecSetting::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_WRSET);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：ClearSettingGroup()
 功能概要：清除定值组配置
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02EF
BOOL CWaveRecSetting::ClearSettingGroup()
{
	//清除定值组配置
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		delete pg;
		pg = NULL;
	}
	m_arrGroup.RemoveAll();
	return TRUE;
}

/*************************************************************
 函 数 名：LoadSettingGroup()
 功能概要：查找定值组
 返 回 值: 查找成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F02F0
BOOL CWaveRecSetting::LoadSettingGroup()
{
	BOOL bReturn = TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLogEx("CWaveRecSetting::LoadSettingGroup, 开始查询");
	
	ClearSettingGroup();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//根据PT_ID, cpu_code,查找103group
	
	CString str;

	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
	
	//字段
	//103group
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);
	
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CWaveRecSetting::LoadSettingGroup, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount <= 0)
		{
			//查找不到此装置的分组
			m_bCanGroup = FALSE;
		}
		else
		{
			m_bCanGroup = TRUE;
		}
		CString strLog;
		strLog.Format("CWaveRecSetting::LoadSettingGroup, 查询到%d条数据", nCount);
		WriteLogEx(strLog);
		for(int i = 0; i < nCount; i++)
		{
			//创建定值组对象
			PT_GROUP * group = new PT_GROUP;
			CString str = pMemset->GetValue((UINT)0); //103group
			
			group->Group = atoi(str);
			group->Name = ""; //name
			
			m_arrGroup.Add(group);
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	if(m_bCanGroup)
		LoadPTGroupConfig();
	WriteLogEx("CWaveRecSetting::LoadSettingGroup, 查询正常结束");
	return bReturn;
}

/*************************************************************
 函 数 名：SetGroupName()
 功能概要：设置定值组的名称
 返 回 值: 
 参    数：param1	定值组ID
		   Param2	定值组名称
**************************************************************/
//##ModelId=49B87B7F02F1
void CWaveRecSetting::SetGroupName( int nGroupID, CString sName )
{
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			pg->Name = sName;
			break;
		}
	}
}

//##ModelId=49B87B800128
void CWaveRecSetting::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	//判断隐藏/显示 分组TAB栏
	if(g_PTSettingGroup != 1)
	{
		//隐藏
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_HIDE);
	}
	else
	{
		//显示
		GetDlgItem(IDC_TAB_WRSET)->ShowWindow(SW_SHOW);
	}
	OnSize(0, 0, 0);
	ReFillAll();
}

//##ModelId=49B87B7F037A
void CWaveRecSetting::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!bActivate)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = "未知厂站";
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, "定值");
	}
	else
 		pApp->SetAppTile("未知装置", "定值");

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

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B7F030D
CString CWaveRecSetting::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pObj == NULL)
		return "";
	if(m_List.GetSafeHwnd() == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//判断是否带有[ ]运算符
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//带有[]运算符
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//没找到结束括号
			return "";
		}
		//取得索引号
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		if(nIndex < 0)
			return "";
		if(nIndex >= m_List.GetItemCount())
			return "";
		//取得头部
		CString strHead = sKey.Left(nFind);
		if(strHead != "$PT_ROUTINE_LIST")
		{
			//如果不是保护常规值, 返回空
			return "";
		}
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_ROUTINE_ID$")
		{
			return m_List.GetItemText(nIndex, 0); //常规值ID
		}
		if(strTail == "PT_ROUTINE_NAME$")
		{
			return m_List.GetItemText(nIndex, 1); //常规值名称
		}
		if(strTail == "PT_ROUTINE_CODE$")
		{
			return m_List.GetItemText(nIndex, 2); //常规值代码
		}
		if(strTail == "PT_ROUTINE_UNIT$")
		{
			return m_List.GetItemText(nIndex, 3); //单位
		}
		//以上都不是,可能是$PT_ROUTINE_ VALUE#N$的形式
		int nValueItem = -1;
		nValueItem = strTail.Find('#', 0);
		if(nValueItem != -1)
		{
			//是有#的形式, 判断是不是PT_ROUTINE_ VALUE#N
			CString strTemp = strTail;
			strTemp = strTemp.Left(nValueItem);
			if(strTemp != "PT_ROUTINE_VALUE")
			{
				return "";
			}
			//得到值索引
			strTemp = strTail;
			strTemp.Delete(0, nValueItem+1);
			nValueItem = atoi(strTemp);
			if(nValueItem < 1)
				nValueItem = 1;
			int nSubItem = m_nNewValueIndex + nValueItem - 1;
			return m_List.GetItemText(nIndex, nSubItem);
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$PT_ROUTINE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//普通关键字
			if(sKey == "$PT_SEL_CPU$") //当前所选CPU
				return m_sCPU;
			if(sKey == "$PT_NAME$") //保护名称
			{
				return m_pObj->m_sName;
			}
			if(sKey == "$PT_MODEL$") //保护型号
			{
				return m_pObj->m_sModel;
			}
			if(sKey == "$DEVICE_NAME$") //一次设备
			{
				if(m_pObj->m_pOwner != NULL)
					return m_pObj->m_pOwner->m_sName;
				return "";
			}
			if(sKey == "$STATION_NAME$") //厂站名
			{
				
				//查找厂站
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CDataEngine* pData = pApp->GetDataEngine();
				CStationObj* pStation = (CStationObj*)pData->FindDevice(m_pObj->m_sStationID, TYPE_STATION);
				if(pStation != NULL)
				{
					return pStation->m_sName;
				}
				else
				{
					return "";
				}
				
			}
			//以上都不是, 有可能是$PT_ROUTINE_TIME#N$的情况
			int nValueItem = -1;
			nValueItem = sKey.Find('#', 0);
			if(nValueItem != -1)
			{
				//是有#的形式, 判断是不是PT_ROUTINE_TIME#N
				CString strTemp = sKey;
				strTemp = strTemp.Left(nValueItem);
				if(strTemp != "$PT_ROUTINE_TIME")
				{
					return "";
				}
				//得到值索引
				strTemp = sKey;
				strTemp.Delete(0, nValueItem+1);
				nValueItem = atoi(strTemp);
				if(nValueItem < 1)
					nValueItem = 1;
				int nSubItem = m_nNewValueIndex + nValueItem - 1;
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				char temp[256];
				for(int j = 0; j < 256; j++)
				{
					temp[j] = '\0';
				}
				col.cchTextMax = 256;
				col.pszText = temp;
				m_List.GetColumn(nSubItem, &col);
				CString strReturn = temp;
				return strReturn;
			}
		}
	}
	return "";
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F030F
BOOL CWaveRecSetting::InitCOM()
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
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);
	
	return TRUE;*/
	return InitReportComNoReg(pClf, pReportDoc);
}

/*************************************************************
 函 数 名：ReleaseCOM()
 功能概要：删除COM组件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F0310
void CWaveRecSetting::ReleaseCOM()
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

/*************************************************************
 函 数 名：DoPrintWRSetting()
 功能概要：打印录波器定值
 返 回 值: 打印模板文件
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B7F0311
void CWaveRecSetting::DoPrintWRSetting( CString strPath )
{
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strPath);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("打开打印模板文件失败.\r\n%s", strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInWRSetting);
	if(pReportDoc != NULL && bOpened)
		pReportDoc->FillValueData();
	//打印
	if(pReportDoc != NULL && bOpened)
	{
		g_hook = 1;
		pReportDoc->Print();
		g_hook = 0;
	}
	//删除COM
	ReleaseCOM();
}

//##ModelId=49B87B8000BB
void CWaveRecSetting::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintWRSetting(WRSETTING_NEW1_TP);
}

//##ModelId=49B87B8000BD
void CWaveRecSetting::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintWRSetting(WRSETTING_NEW2_TP);
}

//##ModelId=49B87B8000BF
void CWaveRecSetting::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintWRSetting(WRSETTING_NEW3_TP);
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置LIST文字显示风格
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8000C1
void CWaveRecSetting::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

				//控制字类型
				if(m_arrSetting.GetSize() > nItem)
				{
					PT_SETTING *pts = m_arrSetting[nItem];
					if(pts != NULL)
					{
						if (pts->DataType == 2)
						{
							lplvcd->clrTextBk = RGB(220, 220, 220);
						}
					}
				}
				
				//值的第一列与第二列差异
				if(nSubItem == m_nNewValueIndex)
				{
					//值
					CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					
					//去除两边空格
					strValue.TrimLeft();
					strValue.TrimRight();
					strOldValue.TrimLeft();
					strOldValue.TrimRight();
					
					//设置文本颜色
					if(strValue != strOldValue)
						lplvcd->clrText = XJ_LIST_ONE_TWO;
				}
				//第二列与第三列差异
				if(nSubItem == m_nNewValueIndex + 1)
				{
					//值
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex + 2);
					
					//去除空格
					strOldValue.TrimLeft();
					strOldValue.TrimRight();
					strOlderValue.TrimLeft();
					strOlderValue.TrimRight();
					
					//设置文本颜色
					if(strOldValue != strOlderValue)
						lplvcd->clrText = XJ_LIST_TWO_THREE;
				}
				
				*pResult = CDRF_NEWFONT;
				break;
				/*

				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//按事件类型(I, II, III)给定背景色
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
				*/

			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B800119
void CWaveRecSetting::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
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
	
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值				
		int nTimeOut = pData->m_nWRSettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		CString str;
		str.Format("%s召唤定值，收到20157报文，重新设置超时时间为%d", m_pObj->m_sName, nTimeOut);
		WriteLog(str);
	}
}

void CWaveRecSetting::OnDoubleclickedBtnWrsetCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CWaveRecSetting::OnDblclkListWrset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	//取得被点击的行号
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if (nItem == -1)
		return;
	
	
	PT_SETTING *pts = NULL;
	pts = (PT_SETTING *)m_List.GetItemData(nItem);
	pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
	if (pts->DataType == 2 || pts->DataType == 4)
	{
		//初始化对话框
		CDlgSettingDetail dlg;
		dlg.m_pts = pts;
		dlg.DoModal();
	}
	
	*pResult = 0;
}
