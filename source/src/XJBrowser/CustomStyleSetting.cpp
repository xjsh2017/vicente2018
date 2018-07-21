// CustomStyleSetting.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "CustomStyleSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int XMLParser::CheckFileExist(CString fileName)
{
	//检查UI配置文件是否存在
	CFileStatus   status;	   
	if(CFile::GetStatus(fileName ,status) != TRUE)
	{
		//Style配置文件不存在,创建一个新文件
		CFile File;
		if(File.Open(fileName, CFile::modeCreate | CFile::modeReadWrite))
		{
			CString str;
			str.Format("<?xml version=\"1.0\"?>\r\n<StyleConfig>\r\n</StyleConfig>");
			char * pBuffer = str.GetBuffer(0);
			File.Write(pBuffer, str.GetLength());
			str.ReleaseBuffer();
			return 0;
		}
		else
		{
			TRACE("open Styleconfig faild or wirte faild\n");
			return -1;
		}		
	}
	else
	{
		return 1;
	}
}

int XMLParser::SaveNewStyleSetting(CString fileName, CString& sNodeName)
{
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog(" XMLParser::SaveNewStyleSetting失败", XJ_LOG_LV3);
			return -1;
		} 	
		
		//加载fileName文件 
		if(!pDoc->load((_variant_t)fileName))
		{
			//加载失败
			MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
			CString str;
			str.Format("when load %s, error in line %d, char %d\n%s", fileName, errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
			WriteLog(str);
			WriteLog("XMLParser::SaveNewStyleSetting", XJ_LOG_LV3);
			return -1;
		}
		else
		{
			//加载文件成功
			MSXML2::IXMLDOMNodePtr pRoot;
			pRoot = pDoc ->selectSingleNode("//StyleConfig");
			//	MSXML2::IXMLDOMNodePtr pSelected;
			//	pSelected = pDoc->selectSingleNode("//ViewActionConfig");
			
			//新建节点
			MSXML2::IXMLDOMElementPtr pNew = NULL;
			CString str;
			str.Format("自定义风格设置%d", ++m_nSettingCount);
			StyleSetting* pStyle = new StyleSetting;
			pStyle->m_sName = str;
			sNodeName = str;
			pNew = pDoc ->createElement((_bstr_t)str);
			if(pNew != NULL)
			{
				//加入新节点
				pRoot ->appendChild(pNew);
				//新建所有子节点
				MSXML2::IXMLDOMElementPtr pNewChild = NULL;
				pNewChild = pDoc ->createElement("Item");
				CString sName = "ToolBarBK";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				CString sValue = "";
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sToolBarBK = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "StartBmp";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue = "";
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sStartBmp = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "ListSpaceColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(118,226,172));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sListSpaceColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "ListHeadColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(200,154,85));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sListHeadColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "DeviceTreeBk";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(255,255,255));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sDeviceTreeBk = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "DeviceTreeTextColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(0,0,0));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sDeviceTreeTextColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "TitleLightColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(197,206,220));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sTitleLightColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "TitleDeepColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(87,111,145));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sTitleDeepColor = sValue;
			}
			
			m_list.AddTail(pStyle);
			//保存配置文件
			pDoc ->save(_variant_t(fileName));
		}
	}
	//关闭打开的COM
	CoUninitialize();
	return 1;
}

int XMLParser::SaveNewStyleSetting(CString fileName)
{
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog(" XMLParser::SaveNewStyleSetting失败", XJ_LOG_LV3);
			return -1;
		} 	
		
		//加载fileName文件 
		if(!pDoc->load((_variant_t)fileName))
		{
			//加载失败
			MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
			CString str;
			str.Format("when load %s, error in line %d, char %d\n%s", fileName, errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
			WriteLog(str);
			WriteLog("XMLParser::SaveNewStyleSetting", XJ_LOG_LV3);
			return -1;
		}
		else
		{
			//加载文件成功
			MSXML2::IXMLDOMNodePtr pRoot;
			pRoot = pDoc ->selectSingleNode("//StyleConfig");
			//	MSXML2::IXMLDOMNodePtr pSelected;
			//	pSelected = pDoc->selectSingleNode("//ViewActionConfig");
			
			//新建节点
			MSXML2::IXMLDOMElementPtr pNew = NULL;
			CString str;
			str.Format("自定义风格设置%d", ++m_nSettingCount);
			StyleSetting* pStyle = new StyleSetting;
			pStyle->m_sName = str;
			pNew = pDoc ->createElement((_bstr_t)str);
			if(pNew != NULL)
			{
				//加入新节点
				pRoot ->appendChild(pNew);
				//新建所有子节点
				MSXML2::IXMLDOMElementPtr pNewChild = NULL;
				pNewChild = pDoc ->createElement("Item");
				CString sName = "ToolBarBK";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				CString sValue = "";
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sToolBarBK = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "StartBmp";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue = "";
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sStartBmp = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "ListSpaceColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(118,226,172));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sListSpaceColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "ListHeadColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(200,154,85));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sListHeadColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "DeviceTreeBk";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(255,255,255));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sDeviceTreeBk = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "DeviceTreeTextColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(0,0,0));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sDeviceTreeTextColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "TitleLightColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(197,206,220));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sTitleLightColor = sValue;
				
				pNewChild = pDoc ->createElement("Item");
				sName = "TitleDeepColor";
				pNewChild ->setAttribute("Name", _variant_t(sName));
				sValue.Format("%d", RGB(87,111,145));
				pNewChild ->setAttribute("Value", _variant_t(sValue));
				pNew ->appendChild(pNewChild);
				pStyle->m_sTitleDeepColor = sValue;
			}
			
			m_list.AddTail(pStyle);
			//保存配置文件
			pDoc ->save(_variant_t(fileName));
		}
	}
	//关闭打开的COM
	CoUninitialize();
	return 1;
}

int XMLParser::RefreshItemSetting(CString fileName, CString sNodeName, CString sItemName, CString sValue)
{
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("XMLParser::RefreshItemSetting", XJ_LOG_LV3);
			return -1;
		} 	
		
		//加载fileName文件 
		if(!pDoc->load((_variant_t)fileName))
		{
			//加载失败
			MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
			CString str;
			str.Format("when load %s, error in line %d, char %d\n%s", fileName, errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
			WriteLog(str);
			WriteLog("XMLParser::RefreshItemSetting", XJ_LOG_LV3);
			return -1;
		}
		else
		{
			//加载文件成功
			MSXML2::IXMLDOMNodePtr pRoot;
			pRoot = pDoc ->selectSingleNode("//StyleConfig");
			MSXML2::IXMLDOMNodePtr pSelected;
			sNodeName = "//" + sNodeName;
			pSelected = pDoc->selectSingleNode((_bstr_t)sNodeName);
			
			//查找
			if(pSelected == NULL)
			{	
				//未找到,新建节点
				WriteLog("未找到" + sNodeName + "节点");	
				
			}
			else 
			{
				MSXML2::IXMLDOMNodeListPtr pChild;
				pChild = pSelected ->GetchildNodes();
				for (int i = 0; i < pChild->Getlength(); i++)
				{
					MSXML2::IXMLDOMNodePtr pNode;
					pNode = pChild->Getitem(i);
					MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
					pAttrMap = pNode ->Getattributes();
					MSXML2::IXMLDOMNodePtr	pItem;
					MSXML2::IXMLDOMNodePtr	pRefreshItem;
					pItem = pAttrMap ->Getitem(0);
					pRefreshItem = pAttrMap->Getitem(1);
					CString strNodeName = (char*)(_bstr_t)pItem->GetnodeValue();
					if (!strNodeName.CompareNoCase(sItemName))
					{
						//CString str = (char*)(_bstr_t)pRefreshItem->GetnodeValue();
						pRefreshItem->nodeValue = (_variant_t)sValue;
						//RefreshList
						break;
					}
				}
			}
			//保存配置文件
			pDoc ->save(_variant_t(fileName));
		}
	}	
	//关闭打开的COM
	CoUninitialize();
	return 1;
}


int XMLParser::DeleteStyleSetting(CString fileName, CString sNodeName)
{
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("XMLParser::DeleteStyleSetting", XJ_LOG_LV3);
			return -1;
		} 	
		
		//加载fileName文件 
		if(!pDoc->load((_variant_t)fileName))
		{
			//加载失败
			MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
			CString str;
			str.Format("when load %s, error in line %d, char %d\n%s", fileName, errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
			WriteLog(str);
			WriteLog("XMLParser::DeleteStyleSetting", XJ_LOG_LV3);
			return -1;
		}
		else
		{
			//加载文件成功
			MSXML2::IXMLDOMNodePtr pRoot;
			pRoot = pDoc ->selectSingleNode("//StyleConfig");
			MSXML2::IXMLDOMNodePtr pSelected;
			sNodeName = "//" + sNodeName;
			pSelected = pDoc->selectSingleNode((_bstr_t)sNodeName);
			
			//查找
			if(pSelected == NULL)
			{	
				//未找到,新建节点
				WriteLog("未找到" + sNodeName + "节点");	
			}
			else 
			{
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
				pRoot->removeChild(pSelected);	
			}
			//保存配置文件
			pDoc ->save(_variant_t(fileName));
		}
	}	
	//关闭打开的COM
	CoUninitialize();
	return 1;
}

int XMLParser::ParserXmlFile(CString fileName)
{
	this->~XMLParser();
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("XMLParser::ParserXmlFile", XJ_LOG_LV3);
			return -1;
		} 	
		
		//加载fileName文件 
		if(!pDoc->load((_variant_t)fileName))
		{
			//加载失败
			MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
			CString str;
			str.Format("when load %s, error in line %d, char %d\n%s", fileName, errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
			WriteLog(str);
			WriteLog("XMLParser::ParserXmlFile", XJ_LOG_LV3);
			return -1;
		}
		else
		{
			//加载文件成功
			MSXML2::IXMLDOMNodePtr pRoot;
			pRoot = pDoc ->selectSingleNode("//StyleConfig");
			MSXML2::IXMLDOMNodeListPtr pListSelected;
			pListSelected = pRoot->GetchildNodes();

			for (int i = 0; i < pListSelected->Getlength(); i++)
			{
				MSXML2::IXMLDOMNodePtr pNode;
				pNode = pListSelected->Getitem(i);
				StyleSetting* pStyle = new StyleSetting;
				pStyle->m_sName = (char*)pNode->nodeName;

				MSXML2::IXMLDOMNodeListPtr pChildListNode;
				pChildListNode = pNode->GetchildNodes();

				for (int j = 0; j < pChildListNode->Getlength(); j++)
				{
					MSXML2::IXMLDOMNodePtr pChildNode;
					pChildNode = pChildListNode->Getitem(j);

					MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
					pAttrMap = pChildNode ->Getattributes();
					MSXML2::IXMLDOMNodePtr	pItem;
					pItem = pAttrMap ->Getitem(1);
					switch (j)
					{
					case 0:
						pStyle->m_sToolBarBK = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 1:
						pStyle->m_sStartBmp = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 2:
						pStyle->m_sListSpaceColor = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 3:
						pStyle->m_sListHeadColor = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 4:
						pStyle->m_sDeviceTreeBk = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 5:
						pStyle->m_sDeviceTreeTextColor = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 6:
						pStyle->m_sTitleLightColor = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					case 7:
						pStyle->m_sTitleDeepColor = (char *)(_bstr_t)pItem->GetnodeValue();
						break;
					}
				}
				m_list.AddTail(pStyle);
			}
			//保存配置文件
			m_nSettingCount = m_list.GetCount();
			pDoc ->save(_variant_t(fileName));
		}
	}	
	//关闭打开的COM
	CoUninitialize();
	return 1;
}


XMLParser::XMLParser()
{
	m_list.RemoveAll();
	m_nSettingCount = 0;
}

XMLParser::~XMLParser()
{
	StyleSetting* pStyle = NULL;
	for(POSITION pos = m_list.GetHeadPosition(); pos != NULL;)
	{
		pStyle = (StyleSetting*)m_list.GetNext(pos);
		if(pStyle != NULL)
		{
			delete 	pStyle;
			pStyle = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCustomStyleSetting property page

IMPLEMENT_DYNCREATE(CCustomStyleSetting, CPropertyPage)

CCustomStyleSetting::CCustomStyleSetting() : CPropertyPage(CCustomStyleSetting::IDD)
{
	//{{AFX_DATA_INIT(CCustomStyleSetting)
	m_sStartPic = _T("");
	m_sToolBarBK = _T("");
	m_bOper = FALSE;
	//}}AFX_DATA_INIT
}

CCustomStyleSetting::~CCustomStyleSetting()
{
}

void CCustomStyleSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomStyleSetting)
	DDX_Control(pDX, IDC_BTN_TREE_TEXT_COLOR, m_btnTreeTextColor);
	DDX_Control(pDX, IDC_BTN_TREE_BK_COLOR, m_btnTreeBKColor);
	DDX_Control(pDX, IDC_BTN_TITLE_LIGHT_COLOR, m_btnTitleLightColor);
	DDX_Control(pDX, IDC_BTN_TITLE_DEEP_COLOR, m_btnTitleDeepColor);
	DDX_Control(pDX, IDC_BTN_LIST_SPACE_COLOR, m_btnListSpaceColor);
	DDX_Control(pDX, IDC_BTN_LIST_HEAD_COLOR, m_btnListHeadColor);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_ComboStyle);
	DDX_Text(pDX, IDC_EDIT_START_PIC, m_sStartPic);
	DDX_Text(pDX, IDC_EDIT_TOOLBAR_BK, m_sToolBarBK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomStyleSetting, CPropertyPage)
	//{{AFX_MSG_MAP(CCustomStyleSetting)
	ON_BN_CLICKED(IDC_BTN_LIST_HEAD_COLOR, OnBtnListHeadColor)
	ON_BN_CLICKED(IDC_BTN_LIST_SPACE_COLOR, OnBtnListSpaceColor)
	ON_BN_CLICKED(IDC_BTN_TITLE_DEEP_COLOR, OnBtnTitleDeepColor)
	ON_BN_CLICKED(IDC_BTN_TITLE_LIGHT_COLOR, OnBtnTitleLightColor)
	ON_BN_CLICKED(IDC_BTN_TREE_BK_COLOR, OnBtnTreeBkColor)
	ON_BN_CLICKED(IDC_BTN_TREE_TEXT_COLOR, OnBtnTreeTextColor)
	ON_BN_CLICKED(IDC_BTN_TOOLBAR_BK, OnBtnToolbarBk)
	ON_BN_CLICKED(IDC_BTN_START_PIC, OnBtnStartPic)
	ON_BN_CLICKED(IDC_BUTTON_ADD_STYLE, OnButtonAddStyle)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_STYLE, OnButtonDeleteStyle)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DELETE_STYLE, OnButtonDeleteStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomStyleSetting message handlers

BOOL CCustomStyleSetting::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_xmlParser.CheckFileExist(g_StyleConfig);
	m_xmlParser.ParserXmlFile(g_StyleConfig);

	if (m_xmlParser.m_list.GetCount() == 0)
	{
		m_xmlParser.SaveNewStyleSetting(g_StyleConfig);
	}

	StyleSetting* pStyle = NULL;
	for(POSITION pos = m_xmlParser.m_list.GetHeadPosition(); pos != NULL;)
	{
		pStyle = (StyleSetting*)m_xmlParser.m_list.GetNext(pos);
		if(pStyle != NULL)
		{
			m_ComboStyle.AddString(pStyle->m_sName);
		}
	}
	
	m_ComboStyle.SetCurSel(0);
	pStyle = (StyleSetting*)m_xmlParser.m_list.GetHead();
	if (pStyle != NULL)
	{
		m_sStartPic = pStyle->m_sStartBmp;
		m_sToolBarBK = pStyle->m_sToolBarBK;
		m_btnTreeTextColor.m_Color = atoi(pStyle->m_sDeviceTreeTextColor);
		m_btnTreeBKColor.m_Color = atoi(pStyle->m_sDeviceTreeBk);
		m_btnTitleLightColor.m_Color = atoi(pStyle->m_sTitleLightColor);
		m_btnTitleDeepColor.m_Color = atoi(pStyle->m_sTitleDeepColor);
		m_btnListSpaceColor.m_Color = atoi(pStyle->m_sListSpaceColor);
		m_btnListHeadColor.m_Color = atoi(pStyle->m_sListHeadColor);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomStyleSetting::OnBtnListHeadColor() 
{
	// TODO: Add your control notification handler code here
	m_btnListHeadColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnListHeadColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "ListHeadColor", strColor);
//	g_ListHeadColor = m_btnListHeadColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnListSpaceColor() 
{
	// TODO: Add your control notification handler code here
	m_btnListSpaceColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnListSpaceColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "ListSpaceColor", strColor);
//	g_ListSpaceColor = m_btnListSpaceColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnTitleDeepColor() 
{
	// TODO: Add your control notification handler code here
	m_btnTitleDeepColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnTitleDeepColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "TitleDeepColor", strColor);
//	g_TitleDeepColor = m_btnTitleDeepColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnTitleLightColor() 
{
	// TODO: Add your control notification handler code here
	m_btnTitleLightColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnTitleLightColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "TitleLightColor", strColor);
//	g_TitleLightColor = m_btnTitleLightColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnTreeBkColor() 
{
	// TODO: Add your control notification handler code here
	m_btnTreeBKColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnTreeBKColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "DeviceTreeBk", strColor);
//	g_TreeBkColor = m_btnTreeBKColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnTreeTextColor() 
{
	// TODO: Add your control notification handler code here
	m_btnTreeTextColor.ShowColorDialog();
	CString str;
	m_ComboStyle.GetWindowText(str);
	CString strColor;
	strColor.Format("%d", m_btnTreeTextColor.m_Color);
	m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "DeviceTreeTextColor", strColor);
//	g_TreeTextColor = m_btnTreeTextColor.m_Color;
	m_bOper = TRUE;
}

void CCustomStyleSetting::OnBtnToolbarBk() 
{
	// TODO: Add your control notification handler code here
	//取得当前系统路径
	DWORD cchCurDir = MAX_PATH; 
    LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH];
	for(int i = 0; i < MAX_PATH; i++)
	{
		tchBuffer[i] = '\0';
	}
	lpszCurDir = tchBuffer; 
    GetCurrentDirectory(cchCurDir, lpszCurDir);
	
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString szFilters = StringFromID(IDS_STYLE_TOOLBAR_BKIMAGE)+"(*.bmp)|*.bmp|All Files (*.*)|*.*||";

	//打开选择文件对话框,让用户选择文件
	CFileDialog dlgFile(TRUE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilters, NULL);
	dlgFile.m_ofn.lpstrTitle = StringFromID(IDS_STYLE_TOOLBAR_BKIMAGE); //对话框标题
//	dlgFile.m_ofn.lpstrFilter =_T("*.bmp"); 
	dlgFile.m_ofn.lpstrInitialDir = ".";
	if(dlgFile.DoModal() == IDOK)
	{
		/*
		//得到全路径
		CString strFullPath = dlgFile.GetPathName();
		//把两个路径都转换成小写再判断
		CString tempFullPath = strFullPath;
		CString tempSTDown = strSTDown;
		tempFullPath.MakeLower();
		tempSTDown.MakeLower();
		*/
		m_sToolBarBK = dlgFile.GetPathName();
		CString str;
		m_ComboStyle.GetWindowText(str);
		m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "ToolBarBK", m_sToolBarBK);
	//	g_strToolBarBK = m_sToolBarBK;
		m_bOper = TRUE;
	}
	
	//设置当前目录
	SetCurrentDirectory(lpszCurDir);
	UpdateData(FALSE);
}

void CCustomStyleSetting::OnBtnStartPic() 
{
	// TODO: Add your control notification handler code here
		//取得当前系统路径
	DWORD cchCurDir = MAX_PATH; 
    LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH];
	for(int i = 0; i < MAX_PATH; i++)
	{
		tchBuffer[i] = '\0';
	}
	lpszCurDir = tchBuffer; 
    GetCurrentDirectory(cchCurDir, lpszCurDir);
	
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString szFilters = StringFromID(IDS_STYLE_START_IMAGE)+"选择程序启动图片(*.bmp)|*.bmp|All Files (*.*)|*.*||";

	//打开选择文件对话框,让用户选择文件
	CFileDialog dlgFile(TRUE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilters, NULL);
	dlgFile.m_ofn.lpstrTitle = StringFromID(IDS_STYLE_START_IMAGE); //对话框标题
//	dlgFile.m_ofn.lpstrFilter =_T("*.bmp"); 
	dlgFile.m_ofn.lpstrInitialDir = ".";
	if(dlgFile.DoModal() == IDOK)
	{
		/*
		//得到全路径
		CString strFullPath = dlgFile.GetPathName();
		//把两个路径都转换成小写再判断
		CString tempFullPath = strFullPath;
		CString tempSTDown = strSTDown;
		tempFullPath.MakeLower();
		tempSTDown.MakeLower();
		*/
		m_sStartPic = dlgFile.GetPathName();
		CString str;
		m_ComboStyle.GetWindowText(str);
		m_xmlParser.RefreshItemSetting(g_StyleConfig, str, "StartBmp", m_sStartPic);
	//	g_strStartBmp = m_sStartPic;
		m_bOper = TRUE;
	}
	
	//设置当前目录
	SetCurrentDirectory(lpszCurDir);
	UpdateData(FALSE);
}

void CCustomStyleSetting::OnButtonAddStyle() 
{
	// TODO: Add your control notification handler code here
	 CString strNodeName;
	m_xmlParser.SaveNewStyleSetting(g_StyleConfig, strNodeName);
	m_ComboStyle.AddString(strNodeName);
}

void CCustomStyleSetting::OnButtonDeleteStyle() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_ComboStyle.GetWindowText(str);
	if(str == "")
	{
		return;
	}

	int nIndex = m_ComboStyle.GetCurSel();
	if (m_xmlParser.DeleteStyleSetting(g_StyleConfig, str))
	{
		m_ComboStyle.DeleteString(nIndex);
		m_ComboStyle.SetCurSel(0);
	}
}

void CCustomStyleSetting::OnSelchangeComboStyle() 
{
	// TODO: Add your control notification handler code here
	m_xmlParser.ParserXmlFile(g_StyleConfig);
	CString str;
	m_ComboStyle.GetWindowText(str);
	StyleSetting* pStyle = NULL;
	for(POSITION pos = m_xmlParser.m_list.GetHeadPosition(); pos != NULL;)
	{
		pStyle = (StyleSetting*)m_xmlParser.m_list.GetNext(pos);
		if (pStyle != NULL && !pStyle->m_sName.CompareNoCase(str))
		{
			m_sStartPic = pStyle->m_sStartBmp;
			m_sToolBarBK = pStyle->m_sToolBarBK;
			m_btnListSpaceColor.m_Color = atoi(pStyle->m_sListSpaceColor);
			m_btnListHeadColor.m_Color = atoi(pStyle->m_sListHeadColor);
			m_btnTitleDeepColor.m_Color = atoi(pStyle->m_sTitleDeepColor);
			m_btnTitleLightColor.m_Color = atoi(pStyle->m_sTitleLightColor);
			m_btnTreeBKColor.m_Color = atoi(pStyle->m_sDeviceTreeBk);
			m_btnTreeTextColor.m_Color = atoi(pStyle->m_sDeviceTreeTextColor);
			Invalidate(TRUE);
			UpdateData(FALSE);
			return;
		}
	}
}
