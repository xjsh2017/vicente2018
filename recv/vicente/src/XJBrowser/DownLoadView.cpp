// DownLoadView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DownLoadView.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/** @brief           升降序作用位*/
int g_iDownloadViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView

IMPLEMENT_DYNCREATE(CDownLoadView, CViewBase)

//##ModelId=49B87BA3010B
CDownLoadView::CDownLoadView()
	: CViewBase(CDownLoadView::IDD)
{
	//{{AFX_DATA_INIT(CDownLoadView)
	m_strCondition = _T("");
	//}}AFX_DATA_INIT
	m_nConditionType = 1;
	m_nCurrentStatus = 0;
	m_sStationID = "";
	m_nTimer = -1;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bUseBreakPoint = FALSE;
	m_nNum = 1;
	m_nOldSortCol = -1;
}

//##ModelId=49B87BA30251
CDownLoadView::~CDownLoadView()
{
	ClearFileList();
}

//##ModelId=49B87BA30232
void CDownLoadView::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownLoadView)
	DDX_Control(pDX, IDC_CMB_DOWN_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_BTN_FILE_CLOSE, m_btnFileClose);
	DDX_Control(pDX, IDC_GIF_DOWNLOAD, m_gif);
	DDX_Control(pDX, IDC_BTN_DOWNLOAD_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_DOWNLOAD_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_DOWN, m_List);
	DDX_Text(pDX, IDC_EDIT_DOWN_CONDITION, m_strCondition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownLoadView, CViewBase)
	//{{AFX_MSG_MAP(CDownLoadView)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CMB_DOWN_CONDITION, OnSelchangeCmbDownCondition)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_CALL, OnBtnDownloadCall)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_DOWN, OnBtnDownloadDown)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DOWN, OnColumnclickListDown)
	ON_BN_CLICKED(IDC_BTN_FILE_CLOSE, OnBtnFileClose)
	ON_BN_DOUBLECLICKED(IDC_BTN_DOWNLOAD_CALL, OnDoubleclickedBtnDownloadCall)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_OPEN, OnBtnDownloadOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOWN, OnDblclkListDown)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DOWNLOAD_OPEN, OnDownloadOpen)
	ON_MESSAGE(WM_STTP_00204, OnSTTP00204)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_00212, OnSTTP00212)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(COMMONFILE_DOWNLOAD_END, DownloadFileEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BA30261
void CDownLoadView::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87BA30271
void CDownLoadView::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownLoadView message handlers

//##ModelId=49B87BA30223
void CDownLoadView::OnInitialUpdate() 
{
	//文字
	RecordRate(IDC_STATIC_DOWNLOAD_1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_4, 0, left_client, top_client);
	RecordRate(IDC_STATIC_DOWNLOAD_5, 0, left_client, top_client);
	//
	RecordRate(IDC_CMB_DOWN_STATION, 0, left_client, top_client);
	RecordRate(IDC_CMB_DOWN_CONDITION, 0, left_client, top_client);
	RecordRate(IDC_EDIT_DOWN_CONDITION, 0, left_client, top_client);
	RecordRate(IDC_DP_DOWN_TIME1, 0, left_client, top_client);
	RecordRate(IDC_DP_DOWN_TIME2, 0, left_client, top_client);
	//下排
	RecordRate(IDC_BTN_FILE_CLOSE, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_DOWN, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_DOWNLOAD_OPEN, 0, left_client, top_client);
	RecordRate(IDC_GIF_DOWNLOAD, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_DOWN, 0, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//初始化列表框
	InitListStyle();
	//初始化按钮
	InitControls();
	//初始化厂站下拉框
	FillStation();
	//初始化条件下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_CONDITION);
	if(pCmb == NULL)
		return;
	pCmb->AddString( StringFromID(IDS_COMMONFILE_CONDITION_FILENAME));
	pCmb->AddString( StringFromID(IDS_COMMONFILE_CONDITION_TIME));
	pCmb->SetCurSel(0);
	//初始化条件编辑框
	m_strCondition = "*.*";
	//设置时间框
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME1);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME2);
	if(pTime2 == NULL)
		return;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1972, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);

	//设置时间
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));

	UpdateData(FALSE);
	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
	m_gif.ShowWindow(SW_HIDE);
}

//##ModelId=49B87BA30280
void CDownLoadView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_DOWN, left_client, IDC_DP_DOWN_TIME2, right_client, bottom_client);
}

/*************************************************************
 函 数 名：OnDownloadOpen()
 功能概要：响应窗口打开消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA302CF
void CDownLoadView::OnDownloadOpen( WPARAM wParam, LPARAM lParam )
{
	//强制刷新一次
	RegulateControlSize();
	MoveCtrl(IDC_LIST_DOWN, left_client, IDC_DP_DOWN_TIME2, right_client, bottom_client);
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BA301A5
BOOL CDownLoadView::LoadListConfig()
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
				WriteLog("DownloadViewConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//DownloadViewConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到DownloadViewConfig");
				WriteLog("DownloadViewConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到DownloadViewConfig下的配置");
					WriteLog("DownloadViewConfig::读取UIConfig失败,将使用默认列风格");
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
BOOL CDownLoadView::SaveListConfig()
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
			WriteLog("DownloadViewConfig::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("DownloadViewConfig::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//DownloadViewConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到DownloadViewConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("DownloadViewConfig");
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
						WriteLog("未找到DownloadViewConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了DownloadViewConfig下的配置, 先删除新建所有子节点");
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
BOOL CDownLoadView::NeedSave()
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
 函 数 名：InitListStyle()
 功能概要：初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301D4
int CDownLoadView::InitListStyle()
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
		//char* arColumn[6]={"序号", "厂站名称", "文件名","文件大小","文件时间","下载情况"};
		for (int nCol=0; nCol < 6 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; //
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1: //厂站名
				lvCol.cx = 160;
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 2: //文件名
				lvCol.cx = 200;
				colName = StringFromID(IDS_COMMON_FILENAME);
				break;
			case 3: //文件大小
				lvCol.cx = 100;
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 4: //文件时间
				lvCol.cx = 180;
				colName = StringFromID(IDS_COMMON_FILETIME);
				break;
			case 5: //已下载
				lvCol.cx = 100;
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			default:
				lvCol.cx=100;//
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列, 序号
		m_List.SetColumnHide(0, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件(除List外的)
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301D5
int CDownLoadView::InitControls()
{

	return 0;
}

/*************************************************************
 函 数 名：FillStation()
 功能概要：填充厂站下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301E4
void CDownLoadView::FillStation()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_STATION);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//加入到下拉框
		int nIndex = pCmb->AddString(pStation->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pStation);
		}
	}
	pCmb->SetCurSel(0);

	m_cmbStation.OnDataFilled(TRUE, 0);
}

/*************************************************************
 函 数 名：ClearFileList()
 功能概要：清除所有通用文件记录
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301E5
void CDownLoadView::ClearFileList()
{
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = (CCommonFile*)m_FileList.GetNext(pos);
		if(pFile == NULL)
			continue;
		delete pFile;
	}
	m_FileList.RemoveAll();
}

//##ModelId=49B87BA3029F
void CDownLoadView::OnSelchangeCmbDownCondition() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_CONDITION);
	if(pCmb == NULL)
		return;
	m_nConditionType = pCmb->GetCurSel() + 1;
}

/*************************************************************
 函 数 名：OnBtnDownloadCall()
 功能概要：下载通用文件列表
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA302A1
void CDownLoadView::OnBtnDownloadCall() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;

		//刷新控件值
		UpdateData(TRUE);

		//取得厂站
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DOWN_STATION);
		if(pCmb == NULL)
			return;
		int nSel = pCmb->GetCurSel();
		if(nSel < 0)
			return;
		CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
		if(pStation == NULL)
			m_sStationID = "";
		else
			m_sStationID = pStation->m_sID;
		//取得时间
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME1);
		if(pTime == NULL)
			return;
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_DOWN_TIME2);
		if(pTime2 == NULL)
			return;
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//组织00203报文
		if(!pApp->m_SttpEngine.BuildDataFor00203CommonFileList(sttpData, m_sStationID, m_nConditionType, tmStart, tmEnd, m_strCondition))
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
				str = "发送召唤通用文件列表报文失败,原因为连接中断";
				WriteLog(str);
			}
			else if(nReturn == -2)
			{
				CString str;
				str = "发送召唤录波列表报文失败,原因为缓存已满";
				WriteLog(str);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤列表
			m_nCurrentStatus = 1;
			
			//启动定时器
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nCommFileListTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//根据当前状态更新控件可用性
			UpdateControlsEnable();
			
			//删除之前的列表
			m_List.DeleteAllItems();
			ClearFileList();
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//取消召唤
		CancelCall();
	}
}

//##ModelId=49B87BA302AF
void CDownLoadView::OnBtnDownloadDown() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 下载文件或取消下载
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//取得当前选择的录波列表
		int nCount = m_List.GetSelectedCount();
		if(nCount < 1)
		{
			AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
			return;
		}
		//把选择的文件列表加入到待召唤链表
		m_FileDownload.RemoveAll();
		m_CallFileResult = "";
		
		int nIndex = -1;
		for(int i = 0; i < nCount; i++)
		{
			nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
			if(nIndex != -1)
			{
				CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nIndex);
				if(pFile != NULL)
				{
					//加入到待召唤链表
					m_FileDownload.AddTail(pFile);
				}
			}
		}
		//开始下载
		//改变状态
		m_nCurrentStatus = 2;
		//更新显示
		UpdateControlsEnable();
		
		//检查文件
		CheckDownload();

		if(m_FileDownload.GetCount() > 0)
		{
			//开始第一个下载
			CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
			if(pFile != NULL)
				DownloadCommonFile(pFile);
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_DOWNLOAD_FINISHED));
			//改变状态
			m_nCurrentStatus = 0;
			//更新显示
			UpdateControlsEnable();
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//取消召唤
		CancelCall();
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301F4
void CDownLoadView::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//按钮
		m_btnCall.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_DOWNLOAD_CALL, StringFromID(IDS_FUNC_CALL_LIST));
		m_btnDown.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_DOWNLOAD_DOWN, StringFromID(IDS_FUNC_CALL_FILE));
		
		//下拉框
		GetDlgItem(IDC_CMB_DOWN_CONDITION) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_DOWN_STATION) ->EnableWindow(TRUE);
		//时间
		GetDlgItem(IDC_DP_DOWN_TIME1) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_DOWN_TIME2) ->EnableWindow(TRUE);
		//编辑框
		GetDlgItem(IDC_EDIT_DOWN_CONDITION) ->EnableWindow(TRUE);
		
		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  > 0)
	{
		//按钮
		m_btnCall.EnableWindow(FALSE);
		m_btnDown.EnableWindow(FALSE);
		
		//下拉框
		GetDlgItem(IDC_CMB_DOWN_CONDITION) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_DOWN_STATION) ->EnableWindow(FALSE);
		//时间
		GetDlgItem(IDC_DP_DOWN_TIME1) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_DOWN_TIME2) ->EnableWindow(FALSE);
		//编辑框
		GetDlgItem(IDC_EDIT_DOWN_CONDITION) ->EnableWindow(FALSE);
		
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_DOWN, str);
		
		if(m_nCurrentStatus == 1)
		{
			//召唤文件列表
			m_btnCall.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_DOWNLOAD_CALL, CANCEL_CALL);
		}
		else if(m_nCurrentStatus == 2)
		{
			//下载文件
			m_btnDown.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_DOWNLOAD_DOWN, CANCEL_CALL);
		}
	}
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA301F5
void CDownLoadView::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//正在召唤列表
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 203, "", 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在下载文件
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 210, "", 0);
		//清空待下载列表
		m_FileDownload.RemoveAll();
		m_CallFileResult = "";
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：OnSTTP00204()
 功能概要：响应00204, 通用文件信息一览通知
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BA302DE
void CDownLoadView::OnSTTP00204( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤列表的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否00204报文
	if(pSttpData->sttp_head.uMsgID != 204)
	{
		//错误报文
		CString str;
		str.Format("CDownLoadView::OnSTTP00204 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(m_sStationID != "")
	{
		if(strID != m_sStationID)
		{
			//不是所选厂站的报文
			CString str;
			str.Format("CDownLoadView::OnSTTP00204 收到其它厂站报文, 厂站ID为%s", strID);
			WriteLog(str);
			return;
		}
	}
	
	//处理报文
	//本帧包含的文件个数
	int nFileNum = pSttpData->sttp_body.variant_member.file_list.size();
	for(int i = 0; i < nFileNum; i++)
	{
		//新建对象
		CCommonFile* pFile = new CCommonFile;
		if(!pFile->LoadFromSTTP00204(pSttpData, i))
		{
			delete pFile;
			pFile = NULL;
			continue;
		}
		//增加ID
		pFile->m_nID = m_nNum++;
		//加入到链表
		m_FileList.AddTail(pFile);
	}

	//判断是否还有后续帧
	if(pSttpData->sttp_head.uMsgEndFlag == 0)
	{
		//停止计时器
		KillTimer(m_nTimer);
		//刷新状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();

		//最后一帧, 填充List
		FillListData();

		//增加人工操作日志
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString str;
		str.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_COMMFILELIST);
		pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_SUCCESS);
	}
}

/*************************************************************
 函 数 名：FillListData()
 功能概要：填充列表框数据
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30203
void CDownLoadView::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	POSITION pos = m_FileList.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = (CCommonFile*)m_FileList.GetNext(pos);
		if(pFile == NULL)
			continue;
		//加入到显示
		AddFileToList(pFile);
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名：AddFileToList()
 功能概要：增加文件对象到列表框中显示
 返 回 值: void
 参    数：param1	文件对象
		   Param2	列表框索引
**************************************************************/
//##ModelId=49B87BA30204
void CDownLoadView::AddFileToList( CCommonFile* pFile, int nIndex /*= 0*/ )
{
	//判断数据有效性
	if(pFile == NULL)
		return;
	if(nIndex < 0)
		return;
	//"序号", "厂站名称", "文件名","文件大小","文件时间","已下载"
	//0:序号
	CString str;
	str.Format("%d", pFile->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:厂站名
	str = "";
	if(pFile->m_pStation != NULL)
		str = pFile->m_pStation->m_sName;
	m_List.SetItemText(nIndex, 1, str);
	//2:文件名
	m_List.SetItemText(nIndex, 2, pFile->m_sFilePath);
	//3:文件大小
	str.Format("%d Byte", pFile->m_nFileSize);
	m_List.SetItemText(nIndex, 3, str);
	//4:文件时间
	m_List.SetItemText(nIndex, 4, pFile->m_sFileTime);
	//5:已下载
	m_List.SetItemText(nIndex, 5, pFile->GetDownloadString());
	//关联数据
	m_List.SetItemData(nIndex, (DWORD)pFile);
}

//##ModelId=49B87BA302B1
void CDownLoadView::OnDestroy() 
{
	//保存UI设置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069(操作失败)报文
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87BA302EF
void CDownLoadView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于下载文件或召唤列表的状态,不处理报文
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
		CString str;
		str.Format("CDownLoadView::OnSTTP20069 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDownLoadView::OnSTTP20069 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//召唤录波列表
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
		//删除数据
		ClearFileList();
		//是本次召唤的失败报文
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//关闭计时器
		KillTimer(m_nTimer);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString str;
		str.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_COMMFILELIST);
		pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_FAILD);

		//提示失败
		AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_CALL_FAIL));
	}
	else if(m_nCurrentStatus == 2)
	{
		//召唤通用文件失败
		if(pSttpData->sttp_body.nMsgId != 210)
			return;
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		pFile->m_nIsDownload = 0;
		pFile->m_nOffset = 0;
		
		//关闭定时器
		KillTimer(m_nTimer);
		//发送消息
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
	}
}

//##ModelId=49B87BA302BF
void CDownLoadView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//删除数据
			ClearFileList();
			//改变状态
			m_nCurrentStatus = 0;
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString str;
			str.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_COMMFILELIST);
			pApp->AddNewManOperator(FUNC_QUY_CALLFILELIST, m_sStationID, str, XJ_OPER_CALL_COMMFILELIST, OPER_FAILD);
			//提示
			AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_CALL_TIMEOUT));
		}
		else if(m_nCurrentStatus == 2)
		{
			//发送完成消息
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 3);
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_DOWN, str);
	}

	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：DownloadFileEnd()
 功能概要：下载文件回应
 返 回 值: void
 参    数：param1
		   Param2 召唤结果. 1-发送失败. 2-返回失败. 3-超时. 4.-装置不支持. 5-成功. 6-部分下载
**************************************************************/
//##ModelId=49B87BA302FD
void CDownLoadView::DownloadFileEnd( WPARAM wParam, LPARAM lParam )
{
	//没有处于下载录波文件的状态, 不处理消息
	if(m_nCurrentStatus != 2)
		return;
	if(m_FileDownload.GetCount() < 1)
	{
		//没有待下载文件, 当作已完成
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	}
	CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
	//记录完成结果
	m_CallFileResult += StringFromID(IDS_FUNC_CALL);
	m_CallFileResult += "[";
	m_CallFileResult += pFile->m_sFilePath;
	m_CallFileResult += "]";
	m_CallFileResult += "\t";
	int nResult = (int)lParam;

	int nOperResult = OPER_SUCCESS;
	switch(pFile->m_nIsDownload)
	{
	case 0:
		m_CallFileResult += StringFromID(IDS_COMMON_FAILED);
		nOperResult = OPER_FAILD;
		break;
	case 1:
		m_CallFileResult += StringFromID(IDS_COMMON_SUCCESS);
		break;
	case 2:
		m_CallFileResult += StringFromID(IDS_DOWNLOAD_PARTIAL);
		nOperResult = OPER_FAILD;
		break;
	default:
		break;
	}
	ChangeValue(pFile);
	m_CallFileResult += "\r\n";
	//移除第一个待下载列表
	m_FileDownload.RemoveHead();


	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	str.Format("%d,%d", nOperResult, XJ_OPER_CALL_COMMFILE);
	pApp->AddNewManOperator(FUNC_QUY_DOWNLOADFILE, m_sStationID, str, XJ_OPER_CALL_COMMFILE, nOperResult);

	//检查是否还有待下载列表
	if(m_FileDownload.GetCount() < 1)
	{
		//已没有等下载列表,表示全部下载完成
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(m_CallFileResult);
	}
	else
	{
		CCommonFile* pNew = (CCommonFile*)m_FileDownload.GetHead();
		DownloadCommonFile(pNew);
	}
}

/*************************************************************
 函 数 名：ChangeValue()
 功能概要：改变文件列表是否已下载的显示
 返 回 值: 
 参    数：param1	指定列表
		   Param2
**************************************************************/
//##ModelId=49B87BA30213
void CDownLoadView::ChangeValue( CCommonFile* pFile )
{
	//检查数据有效性
	if(pFile == NULL)
		return;
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pFile;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	//改变值
	m_List.SetItemText(nIndex, 5, pFile->GetDownloadString());
	//刷新
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：DownloadCommonFile()
 功能概要：下载通用文件
 返 回 值: 
 参    数：param1	指定列表
		   Param2
**************************************************************/
//##ModelId=49B87BA30215
void CDownLoadView::DownloadCommonFile( CCommonFile* pFile )
{
	//响应按钮按下操作, 下载录波文件或取消召唤
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	if(pFile == NULL)
	{
		//发送召唤完成消息,结果为发送失败
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
		return;
	}
	if(pFile->m_pStation == NULL)
	{
		//发送召唤完成消息,结果为发送失败
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
		return;
	}
	
	//申请报文数据容器
	STTP_FULL_DATA sttpData;

	int nOffset = 0;
	if(m_bUseBreakPoint)
	{
		nOffset = pFile->m_nOffset;
	}
	
	//组织00210报文
	if(!pApp->m_SttpEngine.BuildDataFor00210CommonFileDown(sttpData, pFile->m_sStationID, pFile->m_sFilePath, nOffset, pFile->m_pStation->m_sIP))
	{
		//组织报文失败
		//发送召唤完成消息,结果为发送失败
		pFile->m_nIsDownload = 0;
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
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
			str.Format("发送召唤通用文件报文失败,原因为连接中断", pFile->m_sFilePath);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pFile->m_nIsDownload = 0;
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("发送召唤通用文件报文失败,原因为缓存已满", pFile->m_sFilePath);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pFile->m_nIsDownload = 0;
			this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 1);
			return;
		}
	}
	else
	{
		pFile->m_nIsDownload = 0;
		//启动定时器
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nCommFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//改变下载内容大小
		ChangeDownloadedValue(pFile, "0");
	}
}

/*************************************************************
 函 数 名：OnSTTP00212()
 功能概要：响应00212,文件下载完毕通知
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30301
void CDownLoadView::OnSTTP00212( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2)
	{
		//没有处于召唤文件的状态,不处理报文
		return;
	}

	if(m_FileDownload.GetCount() < 1)
	{
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否00212报文
	if(pSttpData->sttp_head.uMsgID != 212)
	{
		//错误报文
		CString str;
		str.Format("CDownLoadView::OnSTTP00212 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}

	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	
	CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
	if(pFile == NULL)
		return;
	if(pFile->m_sStationID != strID)
		return;
	CString strName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	strName.Replace("/", "\\");
	if(pFile->GetFileRelatePath() != strName)
	{
		WriteLog("CDownLoadView::OnSTTP00212 上送的文件名与本列表文件不匹配");
		CString str;
		str.Format("上送路径为%s, 本地路径为%s", strName, pFile->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	pFile->m_nIsDownload = pSttpData->sttp_body.nStatus;
	pFile->m_nOffset = 0;
	if(pFile->m_nIsDownload == 0 && pSttpData->sttp_body.nFlag > 0)
	{
		pFile->m_nIsDownload = 2;
		pFile->m_nOffset = pSttpData->sttp_body.nFlag;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	//发送消息
	this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
}

/*************************************************************
 函 数 名：CheckDownload()
 功能概要：核对要下载的文件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30222
void CDownLoadView::CheckDownload()
{
	if(m_FileDownload.GetCount() < 1)
		return;
	BOOL bHasSuccess = FALSE; //是否包含已下载的文件
	BOOL bHasHalf = FALSE; //是否包含部分下载的文件

	POSITION pos = m_FileDownload.GetHeadPosition();
	while(pos != NULL)
	{
		CCommonFile* pFile = m_FileDownload.GetNext(pos);
		if(pFile == NULL)
			continue;
		if(pFile->m_nIsDownload == 1)
		{
			bHasSuccess = TRUE;
		}
		if(pFile->m_nIsDownload == 2)
		{
			bHasHalf = TRUE;
		}
	}

	if(bHasSuccess)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_AGAIN), MB_YESNO) == IDYES)
		{

		}
		else
		{
			//不重新下载, 去除已下载的文件
			POSITION pos = m_FileDownload.GetHeadPosition();
			while(pos != NULL)
			{
				POSITION oldPos = pos;
				CCommonFile* pFile = m_FileDownload.GetNext(pos);
				if(pFile == NULL)
					continue;
				if(pFile->m_nIsDownload == 1)
				{
					m_FileDownload.RemoveAt(oldPos);
				}
			}
		}
	}
	if(bHasHalf)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_RESUME), MB_YESNO) == IDYES)
		{
			m_bUseBreakPoint = TRUE;
		}
		else
		{
			m_bUseBreakPoint = FALSE;
		}
	}
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87BA3013A
int CALLBACK CDownLoadView::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CCommonFile* pFile1 = (CCommonFile*)lParam1;
	CCommonFile* pFile2 = (CCommonFile*)lParam2;
	if(pFile1 == NULL || pFile2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号", "厂站名称", "文件名","文件大小","文件时间","已下载"
	switch(nCol)
	{
	case 0: //序号
		iResult = pFile1->m_nID - pFile2->m_nID;
		break;
	case 1: //厂站名称
		str1 = "";
		if(pFile1->m_pStation != NULL)
			str1 = pFile1->m_pStation->m_sName;
		str2 = "";
		if(pFile2->m_pStation != NULL)
			str2 = pFile2->m_pStation->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //文件名
		str1 = pFile1->m_sFileName;
		str2 = pFile2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //文件大小
		iResult = pFile1->m_nFileSize - pFile2->m_nFileSize;
		break;
	case 4: //文件时间
		str1 = pFile1->m_sFileTime;
		str2 = pFile2->m_sFileTime;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //已下载
		iResult = pFile1->m_nIsDownload - pFile2->m_nIsDownload;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDownloadViewAsc;
	return iResult;
}

//##ModelId=49B87BA302C2
void CDownLoadView::OnColumnclickListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iDownloadViewAsc = - g_iDownloadViewAsc;
	}
	else
	{
		g_iDownloadViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87BA3030E
void CDownLoadView::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于下载文件或召唤列表的状态,不处理报文
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
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//错误报文
		CString str;
		str.Format("CDownLoadView::OnSTTP20125 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDownLoadView::OnSTTP20125 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//召唤录波列表
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
		//删除数据
		ClearFileList();
		//是本次召唤的失败报文
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//关闭计时器
		KillTimer(m_nTimer);
		//提示失败
		AfxMessageBox( StringFromID(IDS_COMMONFILE_LIST_NOTSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//召唤通用文件失败
		if(pSttpData->sttp_body.nMsgId != 210)
			return;
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		pFile->m_nIsDownload = 0;
		pFile->m_nOffset = 0;
		
		//关闭定时器
		KillTimer(m_nTimer);
		//发送消息
		this->SendMessage(COMMONFILE_DOWNLOAD_END, 0, 0);
	}
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA30312
void CDownLoadView::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		WriteLog("CDownLoadView::OnSTTP20157 报文为空", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		WriteLog("CDownLoadView::OnSTTP20157 报文!=20157", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_sStationID)
	{
		//不是本装置的报文
		WriteLog("CDownLoadView::OnSTTP20157 非本装置报文", XJ_LOG_LV3);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//召唤通用文件列表
		if(pSttpData->sttp_body.nMsgId != 203)
			return;
	}
	else if(m_nCurrentStatus == 2)
	{
		//召唤通用文件
		if(pSttpData->sttp_body.nMsgId != 210)
		{
			WriteLog("CDownLoadView::OnSTTP20157 非召唤通用文件回应", XJ_LOG_LV3);
			return;
		}
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
		//正在召唤通用文件列表				
		int nTimeOut = pData->m_nCommFileListTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤通用文件列表
		WriteLog("CDownLoadView::OnSTTP20157 重新设置超时时间", XJ_LOG_LV3);
		int nTimeOut = pData->m_nCommFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//更改已下载文件大小
		CCommonFile* pFile = (CCommonFile*)m_FileDownload.GetHead();
		if(pFile == NULL)
			return;
		CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
		ChangeDownloadedValue(pFile, sValue);
	}
}

/*************************************************************
 函 数 名：ChangeDownloadedValue()
 功能概要：更新正在下载的文件的已下载内容大小
 返 回 值: 
 参    数：param1	正在下载的文件
		   Param2	已下载文件大小
**************************************************************/
void CDownLoadView::ChangeDownloadedValue( CCommonFile* pFile, CString sValue )
{
	//检查数据有效性
	if(pFile == NULL)
		return;
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pFile;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	CString str;
	str.Format("%s K", sValue);
	//改变值
	m_List.SetItemText(nIndex, 5, str);
	//刷新
	m_List.Update(nIndex);
}

void CDownLoadView::OnBtnFileClose() 
{
	// TODO: Add your control notification handler code here
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CDownLoadView::OnDoubleclickedBtnDownloadCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CDownLoadView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile( StringFromID(IDS_FUNC_COMMONFILE));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDownLoadView::OnBtnDownloadOpen() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox( StringFromID(IDS_COMMON_SELECTFORWATCH));
		return;
	}
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int nIndex = m_List.GetNextSelectedItem(pos);
	CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nIndex);
	if(!pFile || pFile->m_nIsDownload != 1)
	{
		AfxMessageBox( StringFromID(IDS_COMMON_FILE_NOTDOWNLOADED));
		return;
	}
	OpenFileDir(pFile);
}

/****************************************************
Date:2013/12/23  Author:LYH
函数名:   OpenFileDir	
返回值:   void	
功能概要: 
参数: CCommonFile * pFile	
*****************************************************/
void CDownLoadView::OpenFileDir( CCommonFile* pFile )
{
	if(!pFile)
		return;
	CString str = "/n,/select,";
	str += pFile->GetFilePath();
	ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
}

void CDownLoadView::OnDblclkListDown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CCommonFile* pFile = (CCommonFile*)m_List.GetItemData(nSelected);
		if(!pFile || pFile->m_nIsDownload != 1)
		{
			AfxMessageBox( StringFromID(IDS_COMMON_FILE_NOTDOWNLOADED));
			return;
		}
		OpenFileDir(pFile);
	}
	*pResult = 0;
}
