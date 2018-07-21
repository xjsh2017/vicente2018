#if !defined(AFX_SVGVIEWERINTERFACE_H)
#define AFX_SVGVIEWERINTERFACE_H

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <AFXTEMPL.H>

struct ISvgViewer:public IUnknown
{
/*
*  @brief   	LoadFromFile	 ����SVG�ļ� 
*  @param 		[In]a param of Type  CString  �ļ�·��
*  @param 		[Out]a param of Type  CRect&  �ļ���ʾ��ͼ�δ�С
*  @param 		[Out]a param of Type  CString&  �������
*  @return 	int <0��ʾʧ�ܡ�>=0��ʾ�ɹ�
 */
	STDMETHOD_(int, LoadFromFile)(CString, CRect&, CString&) = 0;

/*
*  @brief   	XJGetDocContent	 ȡ��ͼ���ĵ�ת��Ϊͼ���ļ���ʽ��ʽ��ʵʱ���� 
*  @return 	CString ͼ���ĵ�ת��Ϊͼ���ļ���ʽ��ʵʱ����
 */
	STDMETHOD_(CString, GetDocContent)() = 0;

/*
 *  @brief   	SaveToFile	 �����ĵ����ļ� 
 *  @param 		[In]a param of Type  CString  ���ļ�·��
 *	@param		[In]a param of Type	 CString  ���ļ�·��
 *  @return 	BOOL ����ɹ�����TRUE,ʧ�ܷ���FALSE 
 */
	STDMETHOD_(BOOL, SaveToFile)(CString, CString sOldFileName="") = 0;

/*
*  @brief   	DrawShapes	 ����ͼ�� 
*  @param 		[In]a param of Type  CDC*  �����豸ָ��
*  @return 	void
 */
	STDMETHOD_(void, DrawShapes)(CDC* pDC) = 0;

/*
*  @brief   	DrawShapesEx	 ��Ӧ���������¶��� 
*  @param 		[In]a param of Type  CDC*  �����豸ָ��
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @return 	void
 */
	STDMETHOD_(void, DrawShapesEx)(CDC* pDC, CScrollView* pView) = 0;

/*
*  @brief   	DoLButtonDown	 ��Ӧ���������¶��� 
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @param		[In]a param of Type	 const CPoint& ���λ��
*  @param		[In]a param of Type	 UINT ������־
*  @param		[In]a param of Type	 UINT ������������
*  @return 	void
 */
	STDMETHOD_(void, DoLButtonDown)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoLButtonUp	 ��Ӧ������������ 
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @param		[In]a param of Type	 const CPoint& ���λ��
*  @param		[In]a param of Type	 UINT ������־
*  @param		[In]a param of Type	 UINT ������������
*  @return 	void
 */
	STDMETHOD_(void, DoLButtonUp)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoMouseMove	 ��Ӧ����ƶ����� 
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @param		[In]a param of Type	 const CPoint& ���λ��
*  @param		[In]a param of Type	 UINT ������־
*  @param		[In]a param of Type	 UINT ������������
*  @return 	void
 */
	STDMETHOD_(void, DoMouseMove)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoLButtonDown	 ��Ӧ����Ҽ����¶��� 
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @param		[In]a param of Type	 const CPoint& ���λ��
*  @param		[In]a param of Type	 UINT ������־
*  @param		[In]a param of Type	 UINT ������������
*  @return 	void
 */
	STDMETHOD_(void, DoRButtonDown)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoRButtonUp	 ��Ӧ����Ҽ������� 
*  @param 		[In]a param of Type  CScrollView*  ��ͼ����ָ��
*  @param		[In]a param of Type	 const CPoint& ���λ��
*  @param		[In]a param of Type	 UINT ������־
*  @param		[In]a param of Type	 UINT ������������
*  @return 	void
 */
	STDMETHOD_(void, DoRButtonUp)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
 *  @brief   	GetDevID	 ȡ��ָ��ͼ�ε��豸��ʶ 
 *  @param 		[In]a param of Type  int  ָ��ͼ������
 *  @return 	CString		�豸��ʶ
 */
	STDMETHOD_(CString, GetDevID)(int nEleIndex) = 0;

/*
 *  @brief   	GetDevType	 ȡ��ָ��ͼ�ε��豸���� 
 *  @param 		[In]a param of Type  int  ָ��ͼ������
 *  @return 	CString		�豸����
 */
	STDMETHOD_(CString, GetDevType)(int nEleIndex) = 0;

/*
*  @brief   	GetCurSelected	 ȡ�����е�ǰ��ѡԪ�ص�ID 
*  @param 		[Out]a param of Type  CUIntArray&  ��ǰ��ѡԪ�ص�ID
*  @return 		void
 */
	STDMETHOD_(void, GetCurSelected)(CUIntArray& arrSelected) = 0;

/*
 *  @brief   	CartoonControl	 �������ж������ܵĿ�����ر�
 *  @param 		[In]a param of Type  BOOL  TRUE��ʾ������FALSE��ʾ�ر�
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	void
 */
	STDMETHOD_(void, CartoonControl)(BOOL bStart, CScrollView* pView) = 0;

/*
 *  @brief   	BlinkShape	 ����ͼ����˸ 
 *  @param 		[In]a param of Type  int  ͼ��Ԫ��ID
 *  @param 		[In]a param of Type  BOOL  TRUE��ʾ������˸��FALSE��ʾֹͣ��˸
 *	@param		[In]a param of Type  int  ��˸���ʱ��
 *  @return 	void 
 */
	STDMETHOD_(void, BlinkShape)(int nEleIndex, BOOL bBlink, int nTime = 1000) = 0;

/*
 *  @brief   	ChangedFillColor	 �޸�ָ��ͼ�ε����ɫ 
 *  @param 		[In]a param of Type  int  ָ��ͼ��Ԫ��
 *  @param 		[In]a param of Type  COLORREF  ָ�����ɫ
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	void
 */
	STDMETHOD_(void, ChangedFillColor)(int nEleIndex, COLORREF colFill, CScrollView* pView) = 0;

/*
 *  @brief   	ZoomView	 ������ͼ 
 *  @param 		[In]a param of Type  int  ���ŷ�ʽ,0-�Ŵ�/��С.1-���ŵ�����.2-ȡ����������.
 *  @param 		[In]a param of Type  CSize  ���Ŵ�С,����
 *  @param 		[In]a param of Type  CSize	��ͼ���ڴ�С
 *  @return 	CSize ���ź��ĵ��߼���С 
 */
	STDMETHOD_(CSize, ZoomView)(int nType, CSize szAdd, CSize szClient) = 0;
	
/*
 *  @brief   	GetIndexByDevID	 ���豸ID���Ҷ�Ӧ������ͼ������
 *  @param 		[In]a param of Type  CString  ָ���豸ID
 *  @return 	int ͼ������
 */
	STDMETHOD_(int, GetIndexByDevID)(CString sDevID) = 0;

/*
 *  @brief   	SetSelected	 ��ָ��ͼ�μ��뵽ѡ�� 
 *  @param 		[In]a param of Type  int  ָ��ͼ������
 *  @param 		[In]a param of Type  BOOL  �Ƿ����֮ǰ��ѡ��
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	 
 */
	STDMETHOD_(void,SetSelected)(int nEleIndex, BOOL bClearOld, CScrollView* pView) = 0;

/*
 *  @brief   	GetShapeSize  ȡ��ָ��ͼ�εķ�Χ 
 *  @param 		[In]a param of Type  int  ָ��ͼ��
 *  @return 	 ͼ�η�Χ
 */
	STDMETHOD_(CRect,GetShapeSize)(int nEleIndex) = 0;
	
/*
	 *  @brief   	AddMark	 ��ָ��ͼ�ε����Ͻ�����һ��ͼ�����ڱ��
	 *	@param 		[In]a param of Type  CSvgElement * Parent	Ҫ���ӱ�ǵ�ͼ�Σ������ı��ͼ�꽫Ϊ����Ԫ�ء�
	 *	@param 		[In]a param of Type  CString imagePath	���ͼ���·��,Ϊ����ֻ��ɾ����ǲ���
	 *	@param 		[In]a param of Type  int imageSize	���ͼ���С
	 *  @return 	BOOL	 ��ӳɹ�����true,ʧ�ܷ���false
	 */
	STDMETHOD_(int,AddMark)(int nEleIndex,CString imagePath, int imageSize, CScrollView*) = 0;
};
// {7EB4C3A6-1040-480d-8A6A-270C481B941E}
static const IID IID_ISvgViewer = 
{ 0x7eb4c3a6, 0x1040, 0x480d, { 0x8a, 0x6a, 0x27, 0xc, 0x48, 0x1b, 0x94, 0x1e } };

#endif
