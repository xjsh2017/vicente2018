#if !defined(AFX_SVGOPERATEINTERFACE_H)
#define AFX_SVGOPERATEINTERFACE_H

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <AFXTEMPL.H>

//#include "SvgAttribute.h"

/** @brief           ѡ��ı�ص�����*/
typedef void (*XJSVGSELECTCALLBACK)(CUIntArray& arrSelected);
/** @brief           ���Ըı�ص�����*/
typedef void (*XJSVGATTRCALLBACK)();
/** @brief           ��ǰ�ĵ��ı�ص�����*/
typedef void (*XJSVGDOCChANGED)();
/** @brief           �ļ��ı�*/
typedef void (*XJSVGFILECHANGED)();

//0��ѡ��1������ͼ�Ρ�2���������֡�3�����ƶ���Ρ�4������·����
const int DRAWMODE_SELECTION	= 0;
const int DRAWMODE_DRAW			= 1;
const int DRAWMODE_TEXT			= 2;
const int DRAWMODE_POLY			= 3;
const int DRAWMODE_PATH			= 4;

/** @brief           ���Ը��ļ�¼*/
struct AttrChange{
	/** @brief           Ԫ��ID*/
	int		nEleIndex;
	/** @brief           ����ID*/
	int		nAttrIndex;
};

/** @brief           �½�ͼ��ʱ��Ĭ������*/
struct DefaultAttr
{
	/** @brief           ���ɫ*/
	COLORREF	colFill;
	/** @brief           ����ɫ*/
	COLORREF	colStroke;
	/** @brief           ���ʿ��*/
	int			nStrokeWidth;
	/** @brief           ���÷����*/
	CString		sClassName;
	/** @brief           ������*/
	CString		sFontFamily;
	/** @brief           �����С*/
	int			nFontSize;
	/** @brief           ������ɫ*/
	COLORREF	colText;
	
	DefaultAttr::DefaultAttr()
	{
		colFill = -1;
		colStroke = RGB(0, 0, 0);
		nStrokeWidth = 2;
		sClassName = "";
		sFontFamily = "����";
		nFontSize = 12;
		colText = RGB(0, 0, 0);
	}

	const DefaultAttr& operator = (const DefaultAttr& character)
	{
		/** @brief           ���ɫ*/
		colFill = character.colFill;
		/** @brief           ����ɫ*/
		colStroke = character.colStroke;
		/** @brief           ���ʿ��*/
		nStrokeWidth = character.nStrokeWidth;
		/** @brief           ���÷����*/
		sClassName = character.sClassName;
		/** @brief           ������*/
		sFontFamily = character.sFontFamily;
		/** @brief           �����С*/
		nFontSize = character.nFontSize;
		/** @brief           ������ɫ*/
		colText = character.colText;
		return *this;
	}
};

struct ISvgOperate:public IUnknown
{
/*
*  @brief   	GetCurSelected	 ȡ�����е�ǰ��ѡԪ�ص�ID 
*  @param 		[Out]a param of Type  CUIntArray&  ��ǰ��ѡԪ�ص�ID
*  @return 		void
 */
	STDMETHOD_(void, GetCurSelected)(CUIntArray& arrSelected) = 0;

/*
*  @brief   	SetSelectChangedFunction	 ����ѡ��ı�ص����� 
*  @param 		[In]a param of Type  XJSVGSELECTCALLBACK �ص�����ָ��
*  @return 		int	���óɹ�����0, ʧ�ܷ��ط�0
*/
	STDMETHOD_(int, SetSelectChangedFunction)(XJSVGSELECTCALLBACK) = 0;

/*
*  @brief   	SetAttrChangedFunction	 �������Ըı�ص����� 
*  @param 		[In]a param of Type  XJSVGATTRCALLBACK �ص�����ָ��
*  @return 		int	���óɹ�����0, ʧ�ܷ��ط�0
*/
	STDMETHOD_(int, SetAttrChangedFunction)(XJSVGATTRCALLBACK) = 0;

/*
*  @brief   	SetDocChangedFunction	 �����ĵ��ı�ص����� 
*  @param 		[In]a param of Type  XJSVGATTRCALLBACK �ص�����ָ��
*  @return 		int	���óɹ�����0, ʧ�ܷ��ط�0
*/
	STDMETHOD_(int, SetDocChangedFunction)(XJSVGDOCChANGED) = 0;

/*
*  @brief   	SetFileChangedFunction	 �����ļ��ı�ص����� 
*  @param 		[In]a param of Type  XJSVGFILECHANGED �ص�����ָ��
*  @return 		int	���óɹ�����0, ʧ�ܷ��ط�0
*/
	STDMETHOD_(int, SetFileChangedFunction)(XJSVGFILECHANGED) = 0;
	
/*
*  @brief   	GetAttributeValue	 ȡ��ָ��Ԫ��ָ�����Ե�ֵ 
*  @param 		[In]a param of Type  int  ָ��Ԫ��
*  @param		[In]a param of Type	 int  ָ������
*  @return 		CString	����ֵ
 */	
	STDMETHOD_(CString, GetAttributeValue)(int nElementIndex, int nAttrIndex) = 0;

/*
*  @brief   	SetAttributeValue	 ����ָ��Ԫ��ָ�����Ե�ֵ 
*  @param 		[In]a param of Type  int  ָ��Ԫ��
*  @param		[In]a param of Type	 int  ָ������
*  @param		[In]a param of Type  CString ָ������ֵ
*  @return 		void
 */	
	STDMETHOD_(void, SetAttributeValue)(int nElementIndex, int nAttrIndex, CString sValue, CScrollView* pView) = 0;

/*
*  @brief   	GetElementType		ȡ��ָ��Ԫ�ص��������� 
*  @param 		[In]a param of Type  int  ָ��Ԫ��ID
*  @return 		int	Ԫ������
 */	
	STDMETHOD_(int, GetElementType)(int nElementIndex) = 0;

/*
*  @brief   	GetLastAttrChange	 ȡ�����һ�����Ըı���Ϣ 
*  @param 		[In]a param of Type  int  
*  @return 		int	
 */
	STDMETHOD_(void, GetLastAttrChange)(CTypedPtrArray<CPtrArray, AttrChange*>&) = 0;

/*
 *  @brief   	GetAttrChNameInfo	 ȡ��ָ�����Ե�������������˵����Ϣ 
 *  @param 		[In]a param of Type  int  ָ������ID
 *  @param 		[Out]a param of Type  CString&  ������
 *  @param 		[Out]a param of Type  CString&  ����˵��
 *  @param 		[In]a param of Type  CString  ��Ӧ��Ԫ����
 *  @return 	void 
 */
	STDMETHOD_(void, GetAttrChInfo)(int nAttrIndex, CString& sChName, CString& sNote, CString sEleName = "") = 0;

/*
 *  @brief   	GetEnumAttrDispalyValues	 ȡ��ָ��ö�����Ե�������ʾֵ(��ʾֵ����ʵ�ʸ������Ե�ֵ)�б� 
 *  @param 		[In]a param of Type  int  ָ������ID
 *  @param 		[Out]a param of Type  CStringArray&  ����������ʾֵ
 *  @return 	void 
 */
	STDMETHOD_(void, GetEnumAttrDisplayValues)(int nAttrIndex, CStringArray& arrDisplayValues) = 0;

/*
 *  @brief   	GetEnumAttrValueIndex	 ȡ��ö�����Ե�ֵ��Ӧ��ֵ�б��е������� 
 *  @param 		[In]a param of Type  int  ָ��Ԫ��
 *  @param 		[In]a param of Type  int  ָ������
 *  @return 	int ֵ��������
 */
	STDMETHOD_(int, GetEnumAttrValueIndex)(int nEleIndex, int nAttrIndex) = 0;

/*
 *  @brief   	SetEnumAttrValueIndex	 ʹ��ö������ֵ�б���������������Ե�ֵ 
 *  @param 		[In]a param of Type  int  ָ��Ԫ��
 *  @param 		[In]a param of Type  int  ָ������
 *  @param 		[In]a param of Type  int  ָ��ֵ��������
 *  @return 	void 
 */
	STDMETHOD_(void, SetEnumAttrValueIndex)(int nEleIndex, int nAttrIndex, int nValueIndex, CScrollView* pView) = 0;

/*
 *  @brief   	GetCssStyleNames	 ȡ������CSS��ʽ�� 
 *  @param 		[Out]a param of Type  CStringArray&  ����CSS��ʽ��
 *  @return 	void 
 */
	STDMETHOD_(void, GetCssStyleNames)(CStringArray& arrCssStyle) = 0;

/*
 *  @brief   	GetCssStyleIndex	 ȡ��ָ��Ԫ�ص�CSS��ʽ���� 
 *  @param 		[In]a param of Type  int  ָ��Ԫ������
 *  @return 	int ��ʽ����
 */
	STDMETHOD_(int, GetCssStyleIndex)(int nEleIndex) = 0;

/*
 *  @brief   	SetCssStyleValueIndex	 ʹ�÷����������ָ��Ԫ�ص����÷���ֵ 
 *  @param 		[In]a param of Type  int  ָ��Ԫ��
 *  @param 		[In]a param of Type  int  ָ���������
 *  @return 	void 
 */
	STDMETHOD_(void, SetCssStyleValueIndex)(int nEleIndex, int nValueIndex, CScrollView* pView) = 0;

/*
 *  @brief   	DoScaleShape	����ָ��ͼ��	  
 *  @param 		[In]a param of Type  int  ָ��Ҫ�������Ų�����ͼ��,<0ʱ��Ϊ�Ե�ǰ����ѡ��ͼ�ν��в���
 *  @param 		[In]a param of Type  double  X�᷽�����ű���
 *  @param 		[In]a param of Type  double  Y�᷽�����ű���
 *  @return 	void 
 */
	STDMETHOD_(void, DoScaleShape)(int nEleIndex, double dScaleX, double dScaleY, CScrollView* pView) = 0;

/*
 *  @brief   	DoScaleShape	��תָ��ͼ��	  
 *  @param 		[In]a param of Type  int  ָ��Ҫ������ת������ͼ��,<0ʱ��Ϊ�Ե�ǰ����ѡ��ͼ�ν��в���
 *  @param 		[In]a param of Type  double  ��ת�Ƕȡ�����Ϊ˳ʱ�룬����Ϊ��ʱ��
 *  @return 	void 
 */
	STDMETHOD_(void, DoRotateShape)(int nEleIndex, double dAngle, CScrollView* pView) = 0;

/*
 *  @brief   	DoTranslateShapes	�ƶ�ָ��ͼ��	  
 *  @param 		[In]a param of Type  int  ָ��Ҫ�����ƶ�������ͼ��,<0ʱ��Ϊ�Ե�ǰ����ѡ��ͼ�ν��в���
 *  @param 		[In]a param of Type  int  X�᷽���ƶ�����
 *  @param 		[In]a param of Type  int  Y�᷽���ƶ�����
 *  @return 	void 
 */
	STDMETHOD_(void, DoTranslateShapes)(int nEleIndex, int nX, int nY, CScrollView* pView) = 0;

/*
 *  @brief   	SetActiveMouseDraw	���õ�ǰ������ͼ�ε�����	  
 *  @param 		[In]a param of Type  int  ������ͼ�ε�����
 *  @return 	void 
 */
	STDMETHOD_(void, SetActiveMouseDraw)(int nMouseDrawMode) = 0;

/*
 *  @brief   	CreateNewDoc	����SVG�ĵ�	  
 *  @return 	void 
 */
	STDMETHOD_(BOOL, CreateNewDoc)() = 0;

/*
 *  @brief   	GetElementValue	ȡ��ָ��Ԫ�ص�ֵ����ǰֻ��textԪ����ֵ	  
 *  @param 		[In]a param of Type  int  ָ��Ԫ��ID
 *  @return 	CString Ԫ��ֵ
 */
	STDMETHOD_(CString, GetElementValue)(int nEleIndex) = 0;

/*
 *  @brief   	SetElementValue	����ָ��Ԫ�ص�ֵ����ǰֻ��textԪ����ֵ	  
 *  @param 		[In]a param of Type  int  ָ��Ԫ��ID
 *  @param 		[In]a param of Type  CString  ָ��Ԫ��ֵ
 *  @return 	void
 */
	STDMETHOD_(void, SetElementValue)(int nEleIndex, CString sValue, CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	ִ��һ�γ�������	  
 *  @return 	void
 */
	STDMETHOD_(void, DoUndo)(CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	ִ��һ�λ�ԭ����	  
 *  @return 	void
 */
	STDMETHOD_(void, DoRedo)(CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	ɾ��ѡ��Ԫ��	  
 *  @return 	int
 */
	STDMETHOD_(int, DeleteElement)(CScrollView* pView) = 0;

/*
 *  @brief   	ComposeSelectedEles	��ѡ��Ԫ�ؽ������	  
 *  @return 	int
 */
	STDMETHOD_(int, ComposeSelectedEles)(CScrollView* pView) = 0;

/*
 *  @brief   	DeComposeElement	 ��ָ��Ԫ�ؽ��� 
 *  @param 		[In]a param of Type  int  ָ��Ҫ�����Ԫ��
 *  @return 	int
 */
	STDMETHOD_(int, DeComposeElement)(int nEleIndex, CScrollView* pView) = 0;

/*
 *  @brief   	CreateShapeByTemplate	 ʹ��ģ�������ͼ�� 
 *  @param 		[In]a param of Type  CString  ģ����
 *  @param 		[In]a param of Type  CString  ģ���ļ�ȫ·��
 *  @param 		[In]a param of Type  CPoint  ͼ�γ�ʼλ��
 *  @param 		[In]a param of Type  CScrollView*  
 *  @return 	int �ɹ�������ͼ������.ʧ�ܷ���<0
 */
	STDMETHOD_(int, CreateShapeByTemplate)(CString sLinkName, CString sFilePath, CPoint pt, CScrollView* pView) = 0;

/*
 *  @brief   	GetFirstSymbolName)	 ȡ�õ�һ��Ԥ����ͼԪ������ 
 *  @return 	CString ��һ��Ԥ����ͼԪ������ 
 */
	STDMETHOD_(CString, GetFirstSymbolName)() = 0;

/*
 *  @brief   	AlignmentSelectedEles	 ��Ҫ�ĺ�������˵������ 
 *  @param 		[In]a param of Type  int  �����񣬲μ�constant.h
 *  @param 		[In]a param of Type  CScrollView*  �������˵��
 *  @return 	
 */
	STDMETHOD_(void, AlignmentSelectedEles)(int nAlignmentType, CScrollView* pView) = 0;

/*
 *  @brief   	OnDragOver	 ��Ӧ����Ϸ�ʱ������ƶ���Ϣ 
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @param 		[In]a param of Type  CPoint  ���λ��
 *  @return 	 
 */
	STDMETHOD_(void, OnDragOver)(CScrollView* pView, CPoint point) = 0;

/*
 *  @brief   	SetDevTypeLayer	 ����ָ��ͼ�������豸��
 *  @param 		[In]a param of Type  int  ָ��ͼ��Ԫ��
 *  @param 		[In]a param of Type  CString  ָ���豸��
 *  @return 	
 */
	STDMETHOD_(void, SetDevTypeLayer)(int nEleIndex, CString sLayerName) = 0;

/*
 *  @brief   	GetDevTypeLayer	 ȡ��ָ��ͼ�ε��豸������ 
 *  @param 		[In]a param of Type  int  ָ��ͼ��
 *  @return 	CString �豸������
 */
	STDMETHOD_(CString, GetDevTypeLayer)(int nEleIndex) = 0;

/*
 *  @brief   	LoadCssStyle	 ����CSS������� 
 *  @param 		[In]a param of Type  CString  CSS�������
 *  @return 	void 
 */
	STDMETHOD_(void, LoadCssStyle)(CString sContent) = 0;

/*
 *  @brief   	PolylinePort	 �����߹յ���в��� 
 *  @param 		[In]a param of Type  int  ָ��ͼ��Ԫ��
 *  @param 		[In]a param of Type  int  �������͡�0-���ӹյ㡣1-ɾ���յ�
 *  @return 	void
 */
	STDMETHOD_(void, PolylinePort)(int nEleIndex, int nType, CScrollView* pView) = 0;

/*
 *  @brief   	AddNote	 ��ָ��ͼ��Ԫ������ע������ 
 *  @param 		[In]a param of Type  int  ָ��Ԫ��
 *  @param 		[In]a param of Type  CString  ָ��ע��
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	int �����ӵ�ע������Ԫ��
 */
	STDMETHOD_(int, AddNote)(int nEleIndex, CString sNote, CScrollView* pView) = 0;

/*
 *  @brief   	CopySelectedShapes	 ������ѡͼ�� 
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	void 
 */
	STDMETHOD_(void, CopySelectedShapes)(CScrollView* pView) = 0;

/*
 *  @brief   	CutSelectedShapes	 ������ѡͼ�� 
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	void
 */
	STDMETHOD_(void, CutSelectedShapes)(CScrollView* pView)= 0;

/*
 *  @brief   	PasteShapes	 ճ��ͼ�� 
 *  @param 		[In]a param of Type  CPoint  ͼ��λ��
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	void 
 */
	STDMETHOD_(void, PasteShapes)(CPoint point, CScrollView* pView) = 0;

/*
 *  @brief   	GetDefaultAttr	 ȡ���ĵ�Ĭ������ 
 *  @param 		[Out]a param of Type  DefaultAttr&  �ĵ�Ĭ������
 *  @return 	void 
 */
	STDMETHOD_(void, GetDefaultAttr)(DefaultAttr& defaultAttr) = 0;

/*
 *  @brief   	SetDefaultAttr	 �����ĵ�Ĭ������ 
 *  @param 		[In]a param of Type  DefaultAttr  �ĵ�Ĭ������
 *  @return 	void 
 */
	STDMETHOD_(void, SetDefaultAttr)(DefaultAttr defaultAttr) = 0;

/*
 *  @brief   	SetCanvasSize	 ����ͼ�λ�����С 
 *  @param 		[In]a param of Type  Size  ͼ�λ�����С
 *  @return 	CSize ��ǰ�ĵ��߼���С
 */
	STDMETHOD_(CSize, SetCanvasSize)(CSize szDoc) = 0;

/*
 *  @brief   	GetCanvasSize	 ȡ�û�����С 
 *  @return 	CSize ������С
 */
	STDMETHOD_(CSize, GetCanvasSize)() = 0;

/*
 *  @brief   	GetViewboxSize	 ȡ��viewbox��С 
 *  @return 	CSize viewbox��С
 */
	STDMETHOD_(CSize, GetViewboxSize)() = 0;

/*
 *  @brief   	GetBKColor	 ȡ��ͼ�α���ɫ 
 *  @return 	COLORREF ͼ�α���ɫ
 */
	STDMETHOD_(COLORREF, GetBKColor)() = 0;

/*
 *  @brief   	SetBkColor	 ����ͼ�α���ɫ 
 *  @param 		[In]a param of Type  COLORREF  ָ������ɫ
 *  @return 	void 
 */
	STDMETHOD_(void, SetBkColor)(COLORREF colBK) = 0;

/*
 *  @brief   	SetEditStatus	 ����ͼ��״̬.0-�༭��1-���� 
 *  @param 		[In]a param of Type  int  ָ��ͼ��״̬
 *  @return 	void 
 */
	STDMETHOD_(void, SetEditStatus)(int nStatus) = 0;

/*
 *  @brief   	GetEditStatus	 ȡ��ͼ��״̬��
 *  @return 	int 0-�༭��1-����
 */
	STDMETHOD_(int, GetEditStatus)() = 0;

/*
 *  @brief   	CanUndo	 �ж��ĵ���ǰ�Ƿ��ִ��UNDO���� 
 *  @return 	BOOL	��ִ�з���TRUE,����ִ�з���FALSE 
 */
	STDMETHOD_(BOOL, CanUndo)() = 0;

/*
 *  @brief   	CanRedo	 �ж��ĵ���ǰ�Ƿ��ִ��UNDO���� 
 *  @return 	BOOL	��ִ�з���TRUE,����ִ�з���FALSE 
 */
	STDMETHOD_(BOOL, CanRedo)() = 0;

/*
 *  @brief   	CanPaste	 �ж��ĵ���ǰ�Ƿ��ִ��ճ������ 
 *  @return 	BOOL		��ִ�з���TRUE,����ִ�з���FALSE	 
 */
	STDMETHOD_(BOOL, CanPaste)() = 0;

/*
 *  @brief   	LinkSelectedEles	 ��ѡ��ͼ������Ϊ���Ӷ�������Ϊ���Ӷ��������ͼ�ν�ת�����ӵ�ַ 
 *  @return 	int			�����ɵ����Ӷ����� 
 */
	STDMETHOD_(int, LinkSelectedEles)(CScrollView* pView) = 0;

/*
 *  @brief   	GetFilePath	 ȡ��SVG�ļ�·�� 
 *  @return 	CString SVG�ļ�·��
 */
	STDMETHOD_(CString, GetFilePath)() = 0;

/*
 *  @brief   	SetGridSpace	 ���ø��������� 
 *  @param 		[In]a param of Type  int  ���������ࡣ-1��ʾ��ʹ�ø�������
 *  @return 	void 
 */
	STDMETHOD_(void, SetGridSpace)(int nGridSpace) = 0;

/*
 *  @brief   	AddImage	 ���ø��������� 
 *  @param 		[In]a param of Type  CPoint  ͼƬ��ʼ��
 *  @param 		[In]a param of Type  CSize  ͼƬ��С
 *  @param 		[In]a param of Type  CString  ͼƬ·��
 *  @param 		[In]a param of Type  CScrollView*  ��ͼ����
 *  @return 	int �²����ͼƬԪ������ 
 */
	STDMETHOD_(int, AddImage)(CPoint ptTopLeft, CSize szSize, CString sFilePath, CScrollView* pView) = 0;

/*
 *  @brief   	GetCssStroke	 ȡ��ָ��CSS�����е�ָ�����Ե�ֵ 
 *  @param 		[In]a param of Type  CString  ָ��CSS��������
 *  @param 		[In]a param of Type  CString  ָ��ָ��������
 *  @return 	CString CSS������ָ�����Ե�ֵ 
 */
	STDMETHOD_(CString, GetCssStyleValue)(CString sCssName, CString sStyleName) = 0;

/*
 *  @brief   	SetCssStroke	 ����ָ��CSS�����е�ָ�����Ե�ֵ
 *  @param 		[In]a param of Type  CString  ָ��CSS��������
 *  @param 		[In]a param of Type  CString  ָ��ָ��������
 *  @param 		[In]a param of Type  CString  ָ��ָ������ֵ
 *  @return 	void 
 */
	STDMETHOD_(void, SetCssStyleValue)(CString sCssName, CString sStyleName, CString sStyleValue) = 0;

/*
 *  @brief   	SetCssStroke	 ȡ��CSS�������� 
 *  @return 	CString CSS�������� 
 */
	STDMETHOD_(CString, GetCssContent)() = 0;

	/*
 *  @brief   	AddCssStyleItem	 ���CSS������� 
 *  @param 		[In]a param of Type  CString  ָ��CSS��
 *  @param 		[In]a param of Type  CString  ָ��������
 *  @param 		[In]a param of Type  CString  ָ������ֵ
 *  @return 	int 0:��ӳɹ�.-1:�Ѵ��ڴ�����,�������.-2:��֧�ֵ�����.-3:��������ʧ��.-4:����CSS����ʧ��.-5:��ӵ�CSS����ʧ��
 */
	STDMETHOD_ (int, AddCssStyleItem)(CString sCSSName, CString sStyleName, CString sStyleValue) = 0;
/*
 *  @brief   	DeleteCssStyleItem	 ɾ��CSS������� 
 *  @param 		[In]a param of Type  CString  ָ��CSS��
 *  @param 		[In]a param of Type  CString  ָ��������
 *  @return 	void 
 */
	STDMETHOD_ (void, DeleteCssStyleItem)(CString sCSSName, CString sStyleName) = 0;

/*
 *  @brief   	SelectAll	 ѡ������ͼ�� 
 *  @return 	void 
 */
	STDMETHOD_ (void, SelectAll)() = 0;

/*
 *  @brief   	DeComposeLink	 ��ָ��Ԫ�����ӽ�� 
 *  @param 		[In]a param of Type  int  ָ��Ҫ�������ӵ�Ԫ��
 *  @return 	int
 */
	STDMETHOD_(int, DeComposeLink)(int nEleIndex, CScrollView* pView) = 0;
	
/*
 *  @brief   	AddElement	 ���ָ�����͵�SVGԪ�ص��ĵ��� 
 *  @param 		[In]a param of Type  int  ���ڵ�������-1��ʾ���ڵ�
 *  @param 		[In]a param of Type  CString  Ԫ��������
 *  @return 	int ��ӳɹ���������Ԫ�ص�������ʧ�ܷ���-1
 */
	STDMETHOD_(int, AddElement)(int parentIndex, CString elementName) = 0;
};
// {06CC779C-FC34-4995-96AD-41579B8D2486}
static const IID IID_ISvgOperate = 
{ 0x6cc779c, 0xfc34, 0x4995, { 0x96, 0xad, 0x41, 0x57, 0x9b, 0x8d, 0x24, 0x86 } };

#endif
