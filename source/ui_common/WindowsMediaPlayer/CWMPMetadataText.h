// CWMPMetadataText.h : �� Microsoft Visual C++ ������ ActiveX �ؼ���װ�������

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPMetadataText

class CWMPMetadataText : public COleDispatchDriver
{
public:
	CWMPMetadataText() {}		// ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CWMPMetadataText(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPMetadataText(const CWMPMetadataText& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// ����
public:

// ����
public:

	CString get_Description()
	{
		CString result;
		InvokeHelper(0x420, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_text()
	{
		CString result;
		InvokeHelper(0x41f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}


};
