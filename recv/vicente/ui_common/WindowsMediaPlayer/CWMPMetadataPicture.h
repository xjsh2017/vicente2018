// CWMPMetadataPicture.h : �� Microsoft Visual C++ ������ ActiveX �ؼ���װ�������

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWMPMetadataPicture

class CWMPMetadataPicture : public COleDispatchDriver
{
public:
	CWMPMetadataPicture() {}		// ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CWMPMetadataPicture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWMPMetadataPicture(const CWMPMetadataPicture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// ����
public:

// ����
public:

	CString get_mimeType()
	{
		CString result;
		InvokeHelper(0x41b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_pictureType()
	{
		CString result;
		InvokeHelper(0x41c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_Description()
	{
		CString result;
		InvokeHelper(0x41d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_URL()
	{
		CString result;
		InvokeHelper(0x41e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}


};
