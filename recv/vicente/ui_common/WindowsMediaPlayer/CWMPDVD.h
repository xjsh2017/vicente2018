// CWMPDVD.h : �� Microsoft Visual C++ ������ ActiveX �ؼ���װ�������

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPDVD

class CWMPDVD : public COleDispatchDriver
{
public:
	CWMPDVD() {}		// ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CWMPDVD(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPDVD(const CWMPDVD& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// ����
public:

// ����
public:

	BOOL get_isAvailable(LPCTSTR bstrItem)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, bstrItem);
		return result;
	}
	CString get_domain()
	{
		CString result;
		InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void topMenu()
	{
		InvokeHelper(0x3eb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void titleMenu()
	{
		InvokeHelper(0x3ec, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void back()
	{
		InvokeHelper(0x3ed, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void resume()
	{
		InvokeHelper(0x3ee, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
