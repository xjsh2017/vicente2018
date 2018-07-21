#include "XJReportDisplay.h"


/****************************************************
Date:2011/11/4  Author:LYH
函数名:   CXJReportDisplay	
返回值:   	
功能概要: 构造函数，初始化COM组件
*****************************************************/
CXJReportDisplay::CXJReportDisplay()
{
	pClf = NULL;
	pUnk = NULL;
	pReportDoc = NULL;
	hdllInst = NULL;
	//InitCOM();
	InitCOMnoReg();
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   ~CXJReportDisplay	
返回值:   	
功能概要: 析构函数，释放COM组件
*****************************************************/
CXJReportDisplay::~CXJReportDisplay()
{
	ReleaseCOM();
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   InitCOM	
返回值:   BOOL	成功返回TRUE,失败返回FALSE
功能概要: 初始化COM组件
*****************************************************/
BOOL CXJReportDisplay::InitCOM()
{
	HRESULT hr;
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

	if(pReportDoc != NULL)
		pReportDoc->SetQueryFunctionEx(CXJReportDisplay::TranslateKey);

	return TRUE;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   ReleaseCOM	
返回值:   void	
功能概要: 释放COM组件
*****************************************************/
void CXJReportDisplay::ReleaseCOM()
{
	if(pReportDoc != NULL)
		pReportDoc->Release();
	if(pUnk != NULL)
		pUnk->Release();
	if(pClf != NULL)
		pClf->Release();
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   TranslateKey	
返回值:   int	
功能概要: 
参数: CString sKey	
参数: CString & sValue	
参数: void * pContent	
*****************************************************/
int CXJReportDisplay::TranslateKey( CString sKey, CString &sValue, void* pContent )
{
	if(pContent == NULL)
		return 0;
	CXJReportDisplay* pDis = (CXJReportDisplay*)pContent;

	//判断数据有效性
	if(!pDis->IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//找到所有关键字
	sFormat = pDis->FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//解释每个关键字
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pDis->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//填充字符串
	int nFind = 0;
	while(nFind != -1)
	{
		//查找%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//删除%s
		sFormat.Delete(nFind, 2);
		//写入值
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   IsValidKeyString	
返回值:   BOOL	有效返回TRUE, 无效返回FALSE
功能概要: 判断一条带有关键字的语句是否有效语句
参数: CString sKey	关键字语句
*****************************************************/
BOOL CXJReportDisplay::IsValidKeyString( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return FALSE;
	//判断是否有关键字
	int nFind1 = sKey.Find('$', 0);
	int nFind2 = -1;
	//没有关键字, 返回sKey
	if(nFind1 == -1)
		return FALSE;
	//检查$的个数是否双数
	int nCount = 1;
	while(nFind1 != -1)
	{
		nFind1 = sKey.Find('$', nFind1 + 1);
		if(nFind1 == -1)
			break;
		nCount++;
	}
	if(nCount < 2)
		return FALSE;
	int Residue = nCount%2;
	if(Residue != 0)
		return FALSE;
	return TRUE;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   FindOutAllKey	
返回值:   CString	
功能概要: 找出一个语句中所有的关键字
参数: CString sKey	关键字语句
参数: CStringArray & arrString	各关键字
*****************************************************/
CString CXJReportDisplay::FindOutAllKey( CString sKey, CStringArray& arrString )
{
	//清空数组
	arrString.RemoveAll();
	CString strReturn = "";
	int nFind1 = -1;
	int nFind2 = -1;
	CString sTempKey = sKey;
	nFind1 = sTempKey.Find('$', 0);
	while(nFind1 != -1)
	{
		//查找第一个$位置
		nFind1 = sTempKey.Find('$', 0);
		if(nFind1 == -1)
			break;
		//查找与之对应的$的位置
		nFind2 = sTempKey.Find('$', nFind1 + 1);
		if(nFind2 == -1)
			break;
		//保存关键字到数组
		CString sTemp = sTempKey.Mid(nFind1, nFind2 - nFind1 + 1);
		arrString.Add(sTemp);
		//把关键字替换成%s
		sTempKey.Delete(nFind1, nFind2 - nFind1 + 1);
		sTempKey.Insert(nFind1, "%s");
	}
	strReturn = sTempKey;
	return strReturn;
}

/****************************************************
Date:2011/11/4  Author:LYH
函数名:   TranslateSingleKey	
返回值:   CString	关键字代表的值
功能概要: 解释单个关键字代表的意义
参数: CString sKey	关键字内容
*****************************************************/
CString CXJReportDisplay::TranslateSingleKey( CString sKey )
{
	return "";
}

typedef HRESULT (__stdcall * pfnHello)(REFCLSID,REFIID,void**);
/****************************************************
Date:2011/12/9  Author:LYH
函数名:   InitCOMnoReg	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CXJReportDisplay::InitCOMnoReg()
{
	pfnHello fnHello= NULL;
	hdllInst = LoadLibrary("XJReportMgr.dll");
	fnHello=(pfnHello)GetProcAddress(hdllInst,"DllGetClassObject");
	if (fnHello != 0)
	{
		pClf = NULL;
		HRESULT hr=(fnHello)(IID_IXJReport,IID_IClassFactory,(void**)&pClf);
		if (SUCCEEDED(hr) && (pClf != NULL))
		{
			hr = pClf->CreateInstance(NULL, IID_IXJReport, (void**)&pReportDoc);
			if (SUCCEEDED(hr) && (pReportDoc != NULL))
			{
				pReportDoc->SetQueryFunctionEx(CXJReportDisplay::TranslateKey);
				return TRUE;
			}
		}
	}
	return FALSE;
}

/****************************************************
Date:2011/12/9  Author:LYH
函数名:   ReleaseCOMnoReg	
返回值:   void	
功能概要: 
*****************************************************/
void CXJReportDisplay::ReleaseCOMnoReg()
{
	if(pReportDoc != NULL)
		pReportDoc->Release();
	if(pUnk != NULL)
		pUnk->Release();
	if(pClf != NULL)
		pClf->Release();
	if(hdllInst != NULL)
		FreeLibrary(hdllInst);
}
