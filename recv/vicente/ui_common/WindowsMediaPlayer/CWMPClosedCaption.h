// CWMPClosedCaption.h : �� Microsoft Visual C++ ������ ActiveX �ؼ���װ�������

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPClosedCaption

class CWMPClosedCaption : public COleDispatchDriver
{
public:
	CWMPClosedCaption() {}		// ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CWMPClosedCaption(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPClosedCaption(const CWMPClosedCaption& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// ����
public:

// ����
public:

	CString get_SAMIStyle()
	{
		CString result;
		InvokeHelper(0x3b7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SAMIStyle(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_SAMILang()
	{
		CString result;
		InvokeHelper(0x3b8, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SAMILang(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_SAMIFileName()
	{
		CString result;
		InvokeHelper(0x3b9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_SAMIFileName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_captioningId()
	{
		CString result;
		InvokeHelper(0x3ba, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_captioningId(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3ba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}


};
