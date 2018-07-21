// PTDistance.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTDistance.h"
#include "MainFrm.h"
#include "PrintListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iPTDistanceAsc;

UINT PTDistanceLoad(LPVOID pParam)
{
	CPTDistance* ptDistance = (CPTDistance*)pParam;
	if(ptDistance == NULL)
		return 0;
	int nQueryNo = ptDistance->GetCurrentQueryNo();
	//读取内容
	if(ptDistance->IsCurrentQueryNo(nQueryNo) && !ptDistance->m_bExitThread)
	{
		if(ptDistance->ReLoadData(nQueryNo))
			ptDistance->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptDistance->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}
	ptDistance->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTDistance

IMPLEMENT_DYNCREATE(CPTDistance, CViewBase)

CPTDistance::CPTDistance()
	: CViewBase(CPTDistance::IDD)
{
	//{{AFX_DATA_INIT(CPTDistance)
	m_bShowAcked = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_EventManager.ClearAllEvents();
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

CPTDistance::~CPTDistance()
{
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

void CPTDistance::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTDistance)
	DDX_Control(pDX, IDC_LIST_PTDISTANCE, m_List);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_DETAIL, m_btnDetail);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_ACKALL, m_btnAckAll);
	DDX_Control(pDX, IDC_BTN_PTDISTANCE_ACK, m_btnAck);
	DDX_Check(pDX, IDC_CHECK_PTDISTANCE_HIDE, m_bShowAcked);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPTDistance, CViewBase)
	//{{AFX_MSG_MAP(CPTDistance)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_ACK, OnBtnPtdistanceAck)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_ACKALL, OnBtnPtdistanceAckall)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_DETAIL, OnBtnPtdistanceDetail)
	ON_BN_CLICKED(IDC_BTN_PTDISTANCE_PRINT, OnBtnPtdistancePrint)
	ON_BN_CLICKED(IDC_CHECK_PTDISTANCE_HIDE, OnCheckPtdistanceHide)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTDISTANCE, OnDblclkListPtdistance)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTDISTANCE, OnColumnclickListPtdistance)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTDISTANCE, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPTDistance::AssertValid() const
{
	CViewBase::AssertValid();
}

void CPTDistance::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTDistance message handlers

void CPTDistance::OnInitialUpdate() 
{
	// TODO: Add your specialized code here and/or call the base class
	//记录控件
	RecordRate(IDC_CHECK_PTDISTANCE_HIDE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_ACK, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_ACKALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTDISTANCE_DETAIL, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	
	//初始化列表
	InitListCtrl();
	//初始化其它控件
	InitControls();
}

void CPTDistance::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTDISTANCE, left_client, IDC_BTN_PTDISTANCE_DETAIL, right_client, bottom_client);
}

//##ModelId=49B87B9402AF
void CPTDistance::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CPTDistance::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("开始退出保护测距查询线程,查询号:%d", nNo);
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
	
	str.Format("保护测距查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTDistance::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护测距查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTDistance::GetFirstNode()
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

int CPTDistance::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);  
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

int CALLBACK CPTDistance::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventDistance* pEvent1 = (CXJEventDistance*)lParam1;
	CXJEventDistance* pEvent2 = (CXJEventDistance*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //发生时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //线路名称
		str1 = pEvent1->m_sLineName;
		str2 = pEvent2->m_sLineName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3:	//电压等级
		iResult = pEvent1->m_nVoltage - pEvent2->m_nVoltage;
		break;
	case 4: //线路总长度
		if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 5: //故障点到M端距离
		if(pEvent1->m_fDistance - pEvent2->m_fDistance < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fDistance - pEvent2->m_fDistance > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 6://杆塔位置
		iResult = pEvent1->m_nTowerPos - pEvent2->m_nTowerPos;
		break;
	case 7: //维护局名称
		str1 = pEvent1->m_sManagerUnit;
		str2 = pEvent2->m_sManagerUnit;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTDistanceAsc;
	return iResult;
}

int CPTDistance::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();

	return 0;
}

int CPTDistance::InitListStyle()
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
		//char* arColumn[9]={"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 180; // 时间
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 2://
				lvCol.cx = 220; // 线路名称 
				colName = StringFromID(IDS_MODEL_LINE);
				break;
			case 3://
				lvCol.cx = 80; // 电压等级
				colName = StringFromID(IDS_MODEL_VOLTAGE);
				break;
			case 4:
				lvCol.cx = 100; // 线路总长度
				colName = StringFromID(IDS_LINE_TOTALLENGTH);
				break;
			case 5:
				lvCol.cx = 100; //故障点到M端距离
				colName = StringFromID(IDS_FAULTPOINT_M);
				break;
			case 6:
				lvCol.cx = 80; //杆塔位置
				colName = StringFromID(IDS_TOWER_LOCATION);
				break;
			case 7:
				lvCol.cx = 160; //维护局名称
				colName = StringFromID(IDS_MAINTAIN_DEPARTMENT);
				break;
			case 8:
				lvCol.cx = 80; //已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=80;//列宽100象素
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏"确认"
		m_List.SetColumnHide(8, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

int CPTDistance::FillListData( int nQueryNo )
{
	WriteLog("CPTDistance::FillListData 开始填充", XJ_LOG_LV3);
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	
	CXJEventDistance * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDistance*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTDistance::FillListData 开始填充", XJ_LOG_LV3);
	
	return 0;
}

BOOL CPTDistance::LoadListConfig()
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
			WriteLog("CPTDistance::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("CPTDistance::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTDistanceConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("CPTDistance::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTDistanceConfig下的配置");
					WriteLog("CPTDistance::读取UIConfig失败,将使用默认列风格");
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

BOOL CPTDistance::SaveListConfig()
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
			WriteLog("CPTDistance::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("CPTDistance::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTDistanceConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTDistanceConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTDistanceConfig");
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
						WriteLog("未找到PTDistanceConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了PTDistanceConfig下的配置, 先删除新建所有子节点");
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

BOOL CPTDistance::NeedSave()
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

int CPTDistance::InitControls()
{
	return 0;
}

BOOL CPTDistance::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/, int nQueryNo /*= 0*/ )
{
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;
	
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTDistance::LoadDataFromDB, 开始查询");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);

		
	//条件
	//只查询未确认事件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//要查询已确认事件, 则此条件不加
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//不查询已确认事件, 则加上只查询未确认事件的条件
		sql.Conditionlist.push_back(condition1);
	}

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);

	//排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by TIME DESC,MS DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
	
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_DISTANCE_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CPTDistance::LoadDataFromDB, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTDistance::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str);
		}

		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//创建事件
			CXJEventDistance * pEvent = new CXJEventDistance;
			if(pEvent->LoadFromDB(&pMemset))
			{
				//读取数据成功,加入到链表
				//加入到链表
				EnterCriticalSection(&m_CriticalOper);
				m_EventManager.AddEvent(pEvent);
				LeaveCriticalSection(&m_CriticalOper);
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTDistance::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	WriteLog("CPTDistance::LoadDataFromDB, 查询完毕");
	return bReturn;
}

void CPTDistance::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	
	ReFillAll();
}

void CPTDistance::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

void CPTDistance::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_HIDE);
}

BOOL CPTDistance::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_SHOW);
	//清空内容
	EnterCriticalSection(&m_CriticalOper);
	m_EventManager.ClearAllEvents();
	LeaveCriticalSection(&m_CriticalOper);
	m_List.DeleteAllItems();
	//重新载入内容
	if(MODE_SUB == g_role)
	{
		if(LoadDataFromDB(m_nMaxRow, m_bShowAcked, 0))
			FillListData(0);
		GetDlgItem(IDC_STATIC_PTDISTANCE_LOADING) ->ShowWindow(SW_HIDE);
	}
	else
		StartThread();
	return TRUE;
}

int CPTDistance::GetNextQueryNo()
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

void CPTDistance::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTDistanceLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("装置测距查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

BOOL CPTDistance::ReLoadData( int nQueryNo )
{
	return LoadDataFromDB(m_nMaxRow, m_bShowAcked, nQueryNo);
}

void CPTDistance::EndOwnerThread()
{
	WriteLog("开始退出保护测距页面读取数据线程", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护测距页面读取数据线程完毕", XJ_LOG_LV3);
}

BOOL CPTDistance::AddEventToList( CXJEventDistance * pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	//"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"
	//0:序号
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 1, sTime);
	//2:线路名称
	m_List.SetItemText(i, 2, pEvent->m_sLineName);
	//3:电压等级
	str.Format("%dkV", pEvent->m_nVoltage);
	m_List.SetItemText(i, 3, str);
	//4:线路总长度
	str.Format("%.2fkm", pEvent->m_fLineTotalLength);
	m_List.SetItemText(i, 4, str);
	//5:故障点到M端距离
	str.Format("%.2fkm", pEvent->m_fDistance);
	m_List.SetItemText(i, 5, str);
	//6:杆塔位置
	str.Format("%d", pEvent->m_nTowerPos);
	m_List.SetItemText(i, 6, str);
	//7:维护局名称
	m_List.SetItemText(i, 7, pEvent->m_sManagerUnit);
	//8:已确认
	m_List.SetItemText(i, 8, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

void CPTDistance::OnBtnPtdistanceAck() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					RefreshAck(pEvent, nIndex);
					//发送消息通知主框架
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
					if(pFrame != NULL)
					{
						pFrame->PostMessage(WM_EVENTPT_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
					}
				}
			}
		}
	}
}

void CPTDistance::OnBtnPtdistanceAckall() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj == NULL)
		return;
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckPTAllEvent(EX_STTP_INFO_DISTANCE_CFG, m_pObj->m_sID))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

void CPTDistance::OnBtnPtdistanceDetail() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	//多个选择时只处理第一个
	if(nIndex != -1)
	{
		CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
		if(pEvent != NULL)
		{
			m_EventManager.m_pCurrentEvent = pEvent;
			pApp->ShowEventProp(&m_EventManager, TRUE);
		}
	}
}

void CPTDistance::OnBtnPtdistancePrint() 
{
	// TODO: Add your control notification handler code here
	CPrintListCtrl print;
	print.SetListCtrl(m_List.GetSafeHwnd());
	
	// 调整每列的位置
	print.ResizeColumnsToFitContent(TRUE);
	
	// 是否裁减每行的长度 FALSE 否， TRUE 是
	print.TruncateColumn(TRUE);
	
	// 打印列之间的分割线
	print.PrintColumnSeparator(TRUE);
	
	// 打印行之间的分割线
	print.PrintLineSeparator(TRUE);
	
	// 打印表的名称
	print.PrintHead(TRUE);
	
	// 设置各种字体
	print.SetHeadFont(StringFromID(IDS_HEAD_FONT), 14);
	print.SetFootFont(StringFromID(IDS_DEFAULT_FONT), 12);
	print.SetListFont(StringFromID(IDS_DEFAULT_FONT), 12);
	
	// 设置纸的走向 TRUE,横向；FALSE 纵向
	print.SetPrinterOrientation(FALSE);
    
	BOOL bHide = m_List.GetColumnIsHide(0);
	BOOL bHide8 = m_List.GetColumnIsHide(8);
    CString strCa;
	strCa.Format("%s %s", m_pObj->m_sName, StringFromID(IDS_SECPROP_DISTANCE));
	print.Print(GetSafeHwnd(), strCa, FALSE);
	m_List.SetColumnHide(0, bHide);
	m_List.SetColumnHide(8, bHide8);
}

void CPTDistance::OnCheckPtdistanceHide() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTDistance::OnDblclkListPtdistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//得到该行对应的事件
		CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager, TRUE);
	}
	*pResult = 0;
}

void CPTDistance::OnColumnclickListPtdistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTDistanceAsc = - g_iPTDistanceAsc;
	}
	else
	{
		g_iPTDistanceAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CPTDistance::RefreshAck( CXJEventDistance* pEvent, int nIndex /*= -1*/ )
{
	//判断数据有效性
	if(pEvent == NULL)
		return;
	if(nIndex == -1)
	{
		//查找事件在列表中的位置
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pEvent;
		nIndex = m_List.FindItem(&lvFind);
	}
	//实在找不到,退出
	if(nIndex == -1)
		return;
	//更新ID,因为新事件事先不知道ID
	CString str;
	str.Format("%d", pEvent->m_lID);
	m_List.SetItemText(nIndex, 0, str);
	//更新是否确认显示
	m_List.SetItemText(nIndex, 8, StringFromID(IDS_ACK_YES));
	//刷新
	m_List.Update(nIndex);
}

void CPTDistance::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					
					if (g_role == MODE_SUB)
					{
						//得到事件对象
						CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}
						
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
						else if (g_style == 3)
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

void CPTDistance::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

void CPTDistance::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;

	//查找在本地的地址
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
	}
}

void CPTDistance::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	
	//查找在本地的地址
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
		//通知事件属性对话框这里发生了被动确认
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			pFrame->PostMessage(WM_EVENTPT_PASSIVE_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
		}
	}
}

CXJEventDistance* CPTDistance::FindLocalPoint( CXJEventDistance* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventDistance* pReturn = NULL;
	//因为现在有ID不知道的情况,只能比较二次设备ID, 发生时间等信息
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventDistance* pTemp = (CXJEventDistance*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//二次设备相同,比较发生时间,时间用字符串比
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//发生时间相同,比较发生时间毫秒值
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//相同,认为是同一事件
					pTemp->m_lID = pEvent->m_lID;
					pTemp->m_nIsAck = pEvent->m_nIsAck;
					pReturn = pTemp;
					break;
				}
				else
				{
					//发生时间毫秒值不同,下一个
					continue;
				}
			}
			else
			{
				//发生时间不同,下一个
				continue;
			}
		}
		else
		{
			//二次设备不同, 下一个
			continue;
		}
	}
	return pReturn;
}

void CPTDistance::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTDistance::OnSTTP20168 start");
	
	CXJEventDistance* pParam = (CXJEventDistance*)lParam;
	if(pParam == NULL)
		return;
	
	//复制
	CXJEventDistance* pEvent = (CXJEventDistance*)pParam->Clone();
	if(pEvent == NULL)
		return;

	if(pEvent->m_sSecID != m_pObj->m_sID)
		return;
	
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	WriteLog("CPTDistance::OnSTTP20168 neat end");
}

void CPTDistance::CheckLineCount()
{
	int nCount = m_List.GetItemCount();
	int nDel = nCount - m_nMaxRow;
	if(nDel <= 0)
	{
		//列表中显示数没超过最大数
		return;
	}
	//删除最后一项,直到数量符合
	for(int i = 0; i < nDel; i++)
	{
		int nNewCount = m_List.GetItemCount();
		//取得最后一项
		CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nNewCount - 1);
		//从列表中删除
		m_List.DeleteItem(nNewCount - 1);
		//从事件链表和内存中删除
		m_EventManager.RemoveAtEvent(pEvent);
	}
}
