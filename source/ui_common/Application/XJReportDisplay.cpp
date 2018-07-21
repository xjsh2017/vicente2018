#include "XJReportDisplay.h"


/****************************************************
Date:2011/11/4  Author:LYH
������:   CXJReportDisplay	
����ֵ:   	
���ܸ�Ҫ: ���캯������ʼ��COM���
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
������:   ~CXJReportDisplay	
����ֵ:   	
���ܸ�Ҫ: �����������ͷ�COM���
*****************************************************/
CXJReportDisplay::~CXJReportDisplay()
{
	ReleaseCOM();
}

/****************************************************
Date:2011/11/4  Author:LYH
������:   InitCOM	
����ֵ:   BOOL	�ɹ�����TRUE,ʧ�ܷ���FALSE
���ܸ�Ҫ: ��ʼ��COM���
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
������:   ReleaseCOM	
����ֵ:   void	
���ܸ�Ҫ: �ͷ�COM���
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
������:   TranslateKey	
����ֵ:   int	
���ܸ�Ҫ: 
����: CString sKey	
����: CString & sValue	
����: void * pContent	
*****************************************************/
int CXJReportDisplay::TranslateKey( CString sKey, CString &sValue, void* pContent )
{
	if(pContent == NULL)
		return 0;
	CXJReportDisplay* pDis = (CXJReportDisplay*)pContent;

	//�ж�������Ч��
	if(!pDis->IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = pDis->FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pDis->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
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
������:   IsValidKeyString	
����ֵ:   BOOL	��Ч����TRUE, ��Ч����FALSE
���ܸ�Ҫ: �ж�һ�����йؼ��ֵ�����Ƿ���Ч���
����: CString sKey	�ؼ������
*****************************************************/
BOOL CXJReportDisplay::IsValidKeyString( CString sKey )
{
	//�ж�������Ч��
	if(sKey == "")
		return FALSE;
	//�ж��Ƿ��йؼ���
	int nFind1 = sKey.Find('$', 0);
	int nFind2 = -1;
	//û�йؼ���, ����sKey
	if(nFind1 == -1)
		return FALSE;
	//���$�ĸ����Ƿ�˫��
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
������:   FindOutAllKey	
����ֵ:   CString	
���ܸ�Ҫ: �ҳ�һ����������еĹؼ���
����: CString sKey	�ؼ������
����: CStringArray & arrString	���ؼ���
*****************************************************/
CString CXJReportDisplay::FindOutAllKey( CString sKey, CStringArray& arrString )
{
	//�������
	arrString.RemoveAll();
	CString strReturn = "";
	int nFind1 = -1;
	int nFind2 = -1;
	CString sTempKey = sKey;
	nFind1 = sTempKey.Find('$', 0);
	while(nFind1 != -1)
	{
		//���ҵ�һ��$λ��
		nFind1 = sTempKey.Find('$', 0);
		if(nFind1 == -1)
			break;
		//������֮��Ӧ��$��λ��
		nFind2 = sTempKey.Find('$', nFind1 + 1);
		if(nFind2 == -1)
			break;
		//����ؼ��ֵ�����
		CString sTemp = sTempKey.Mid(nFind1, nFind2 - nFind1 + 1);
		arrString.Add(sTemp);
		//�ѹؼ����滻��%s
		sTempKey.Delete(nFind1, nFind2 - nFind1 + 1);
		sTempKey.Insert(nFind1, "%s");
	}
	strReturn = sTempKey;
	return strReturn;
}

/****************************************************
Date:2011/11/4  Author:LYH
������:   TranslateSingleKey	
����ֵ:   CString	�ؼ��ִ����ֵ
���ܸ�Ҫ: ���͵����ؼ��ִ��������
����: CString sKey	�ؼ�������
*****************************************************/
CString CXJReportDisplay::TranslateSingleKey( CString sKey )
{
	return "";
}

typedef HRESULT (__stdcall * pfnHello)(REFCLSID,REFIID,void**);
/****************************************************
Date:2011/12/9  Author:LYH
������:   InitCOMnoReg	
����ֵ:   BOOL	
���ܸ�Ҫ: 
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
������:   ReleaseCOMnoReg	
����ֵ:   void	
���ܸ�Ҫ: 
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
