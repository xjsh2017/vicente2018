#if !defined(XJREPORTINTERFACEY_H_)
#define XJREPORTINTERFACEY_H_

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <AFXTEMPL.H>

typedef int (*RptPFUNCALLBACK)(CString sKey, CString & sValue);
typedef int (*RptPFUNCALLBACKEX)(CString sKey, CString & sValue, void* pObject);

struct IXJReport:public IUnknown
{
/*
 *  @brief   	OpenReportFile	 �򿪱����ӡģ���ļ� 
 *  @param 		[In]a param of Type  CString  �ļ���(ȫ·��)
 *  @return 	CSize	����򿪳ɹ�, �����ĵ���С, ��ʧ��,����(0, 0)
 */
	STDMETHOD_(CSize, OpenReportFile)(CString) = 0;

/*
 *  @brief   	OpenReportFileEx	 �򿪱����ӡģ���ļ� 
 *  @param 		[In]a param of Type  CString  �ļ���(ȫ·��)
 *  @param 		[In]a param of Type  void*  
 *  @return 	CSize	����򿪳ɹ�, �����ĵ���С, ��ʧ��,����(0, 0)
 */
	STDMETHOD_(CSize, OpenReportFileEx)(CString, void*) = 0;

/*
 *  @brief   	SetQueryFunction	 ���ò�ѯ�ؼ��ֵĻص����� 
 *  @param 		[In]a param of Type  RptPFUNCALLBACK �ص�����ָ��
 *  @return 	int	���óɹ�����0, ʧ�ܷ��ط�0
 */
	STDMETHOD_(int, SetQueryFunction)(RptPFUNCALLBACK) = 0;

/*
 *  @brief   	SetQueryFunctionEx	 ���ò�ѯ�ؼ��ֵĻص����� 
 *  @param 		[In]a param of Type  RptPFUNCALLBACKEX �ص�����ָ��
 *  @return 	int	���óɹ�����0, ʧ�ܷ��ط�0
 */
	STDMETHOD_(int, SetQueryFunctionEx)(RptPFUNCALLBACKEX) = 0;

/*
 *  @brief   	Draw	 ���Ʊ���ͼ�� 
 *  @param 		[In]a param of Type  CDC* �����豸���豸����ָ��
 *  @return 	void
 */
	STDMETHOD_(void, Draw)(CDC*) = 0;

	/*
	* @brief	DrawEx	
	* @param	[In]a param of Type CDC* �����豸���豸����ָ��		
	* @param	[In]a param of Type int	���λ��X������	
	* @param	[In]a param of Type int	���λ��Y������
	* @return   STDMETHOD_(void,	
	*/
	STDMETHOD_(void, DrawEx)(CDC*, int, int) = 0;

/*
 *  @brief   	FillValueData	 �������ñ����и�Ԫ�ص���ʵֵ 
 *  @return 	CSize �����ĵ���С
 */
	STDMETHOD_(CSize, FillValueData)() = 0;

/*
 *  @brief   	FillValueData	 ��ӡ��ǰ�ı��� 
 *  @return 	void
 */
	STDMETHOD_(void, Print)() = 0;

/*
 *  @brief   	PrintDialogModel	 ��ӡ�Ի�����ʾ��ʽ(���÷�Χ: ���ε���) 
 *  @param 		[In]a param of Type  int 0-��Ĭ�ϴ�ӡ�������ӵ������ֱ��ʹ��Ĭ�ϴ�ӡ��,��������ӡ���öԻ���
										 1-������ζ�������ӡ���öԻ���
										 ��ʼֵΪ0
 *  @return 	void
 */
	STDMETHOD_(void, PrintDialogModel)(int) = 0;

/*
 *  @brief   	PrintPageSet	 ��ӡֽ����(���÷�Χ: ���ε���) 
 *  @param 		[In]a param of Type  int ��ӡֽ��С,��ʼֵΪA4,�������ֵ�μ�MSDN -- DEVMODE
 *	@param		[In]a param of Type	 int �Ű淽ʽ. 1��ʾ����. 2Ϊ���. ��ʼֵΪ���
 *	@param		[In]a param of Type	 int ��ӡ�ֱ���(dpi). ������ֵ. ��ʼֵΪ600
 *  @return 	void
 */
	STDMETHOD_(void, PrintPageSet)(int, int, int) = 0;

/*
 *  @brief   	PrintCopies	 ���ô�ӡ���� (���÷�Χ: ���δ�ӡ)
 *  @param 		[In]a param of Type  int ���ô�ӡ����, ������ֵ, ��ʼֵΪ1
 *  @return 	void
 */
	STDMETHOD_(void, PrintCopies)(int) = 0;

/*
 *  @brief   	PrintRange	 ��ӡ��Χ���� (���÷�Χ: ���δ�ӡ)
 *  @param 		[In]a param of Type  int ��ӡ��Χ, 0��ʾ��ӡȫ��, 1��ʾ������ѡҳ�뷶Χ��ӡ. ��ʼֵΪ0
 *	@param		[In]a param of Type	 int ��ʼ��ӡ��ҳ��. ��������ѡҳ�뷶Χ��ӡʱ��Ч.��ʼֵΪ1
 *	@param		[In]a param of Type	 int ��ӡ����ҳ��, ������ѡҳ�뷶Χ��ӡ��Ч.��ʼֵΪ32765
 *  @return 	void
 */
	STDMETHOD_(void, PrintRange)(int, int, int) = 0;

/*
 *  @brief   	GetHitElement	 �õ��������Ԫ�ص�ID
 *  @param 		[In]a param of Type  CPoint	���λ��(�߼�����)
 *  @return 	int	����б������Ԫ��,����>=0 ��Ԫ��ID, ���򷵻�-1
 */
	STDMETHOD_(int, GetHitElement)(CPoint ptHit) = 0;

/*
 *  @brief   	GetValueCode	 �õ�ָ��Ԫ�ص�ֵ����
 *  @param 		[In]a param of Type  int	Ԫ��ID
 *  @return 	CString		Ԫ��ֵ����
 */
	STDMETHOD_(CString, GetValueCode)(int nID) = 0;

/*
 *  @brief   	SetElementTextColor	 ����ָ��Ԫ�ص�������ɫ
 *  @param 		[In]a param of Type  int	Ԫ��ID
 *	@param		[In]a param of Type	 COLORREF ָ����ɫ
 *  @return 	CRect	�������
 */
	STDMETHOD_(CRect, SetElementTextColor)(int nID, COLORREF colText) = 0;

/*
 *  @brief   	RevertTextColor	 ��ԭָ��Ԫ�ص�������ɫ 
 *  @param 		[In]a param of Type  int  ָ��Ԫ��ID
 *  @return 	CRect ���Ҫˢ�µ�����
 */
	STDMETHOD_(CRect, RevertTextColor)(int nID) = 0;
	
/*
 *  @brief   	GetStartPosition	 ȡ��ͼ����ʼ��λ�� 
 *  @return 	CSize ͼ����ʼ��λ��
 */
	STDMETHOD_(CSize, GetStartPosition)() = 0;

	STDMETHOD_(int, ExportToExcel)(CString sFilePath) = 0;

	STDMETHOD_(int, ExportToWord)(CString sFilePath) = 0;
};

// {EFC3F912-227B-4232-91B8-F1A77B7DE8C6}
// IMPLEMENT_OLECREATE(XJReportDoc, "XJReportDoc", 
// 					0xefc3f912, 0x227b, 0x4232, 0x91, 0xb8, 0xf1, 0xa7, 0x7b, 0x7d, 0xe8, 0xc6);
// {EFC3F912-227B-4232-91B8-F1A77B7DE8C6}
static const IID IID_IXJReport = 
{ 0xefc3f912, 0x227b, 0x4232, { 0x91, 0xb8, 0xf1, 0xa7, 0x7b, 0x7d, 0xe8, 0xc6 } };
#endif