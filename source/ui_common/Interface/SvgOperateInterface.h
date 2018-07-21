#if !defined(AFX_SVGOPERATEINTERFACE_H)
#define AFX_SVGOPERATEINTERFACE_H

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <AFXTEMPL.H>

//#include "SvgAttribute.h"

/** @brief           选择改变回调函数*/
typedef void (*XJSVGSELECTCALLBACK)(CUIntArray& arrSelected);
/** @brief           属性改变回调函数*/
typedef void (*XJSVGATTRCALLBACK)();
/** @brief           当前文档改变回调函数*/
typedef void (*XJSVGDOCChANGED)();
/** @brief           文件改变*/
typedef void (*XJSVGFILECHANGED)();

//0：选择。1：绘制图形。2：绘制文字。3：绘制多边形。4：绘制路径。
const int DRAWMODE_SELECTION	= 0;
const int DRAWMODE_DRAW			= 1;
const int DRAWMODE_TEXT			= 2;
const int DRAWMODE_POLY			= 3;
const int DRAWMODE_PATH			= 4;

/** @brief           属性更改记录*/
struct AttrChange{
	/** @brief           元素ID*/
	int		nEleIndex;
	/** @brief           属性ID*/
	int		nAttrIndex;
};

/** @brief           新建图形时的默认属性*/
struct DefaultAttr
{
	/** @brief           填充色*/
	COLORREF	colFill;
	/** @brief           画笔色*/
	COLORREF	colStroke;
	/** @brief           画笔宽度*/
	int			nStrokeWidth;
	/** @brief           引用风格名*/
	CString		sClassName;
	/** @brief           字体名*/
	CString		sFontFamily;
	/** @brief           字体大小*/
	int			nFontSize;
	/** @brief           文字颜色*/
	COLORREF	colText;
	
	DefaultAttr::DefaultAttr()
	{
		colFill = -1;
		colStroke = RGB(0, 0, 0);
		nStrokeWidth = 2;
		sClassName = "";
		sFontFamily = "黑体";
		nFontSize = 12;
		colText = RGB(0, 0, 0);
	}

	const DefaultAttr& operator = (const DefaultAttr& character)
	{
		/** @brief           填充色*/
		colFill = character.colFill;
		/** @brief           画笔色*/
		colStroke = character.colStroke;
		/** @brief           画笔宽度*/
		nStrokeWidth = character.nStrokeWidth;
		/** @brief           引用风格名*/
		sClassName = character.sClassName;
		/** @brief           字体名*/
		sFontFamily = character.sFontFamily;
		/** @brief           字体大小*/
		nFontSize = character.nFontSize;
		/** @brief           文字颜色*/
		colText = character.colText;
		return *this;
	}
};

struct ISvgOperate:public IUnknown
{
/*
*  @brief   	GetCurSelected	 取得所有当前所选元素的ID 
*  @param 		[Out]a param of Type  CUIntArray&  当前所选元素的ID
*  @return 		void
 */
	STDMETHOD_(void, GetCurSelected)(CUIntArray& arrSelected) = 0;

/*
*  @brief   	SetSelectChangedFunction	 设置选择改变回调函数 
*  @param 		[In]a param of Type  XJSVGSELECTCALLBACK 回调函数指针
*  @return 		int	设置成功返回0, 失败返回非0
*/
	STDMETHOD_(int, SetSelectChangedFunction)(XJSVGSELECTCALLBACK) = 0;

/*
*  @brief   	SetAttrChangedFunction	 设置属性改变回调函数 
*  @param 		[In]a param of Type  XJSVGATTRCALLBACK 回调函数指针
*  @return 		int	设置成功返回0, 失败返回非0
*/
	STDMETHOD_(int, SetAttrChangedFunction)(XJSVGATTRCALLBACK) = 0;

/*
*  @brief   	SetDocChangedFunction	 设置文档改变回调函数 
*  @param 		[In]a param of Type  XJSVGATTRCALLBACK 回调函数指针
*  @return 		int	设置成功返回0, 失败返回非0
*/
	STDMETHOD_(int, SetDocChangedFunction)(XJSVGDOCChANGED) = 0;

/*
*  @brief   	SetFileChangedFunction	 设置文件改变回调函数 
*  @param 		[In]a param of Type  XJSVGFILECHANGED 回调函数指针
*  @return 		int	设置成功返回0, 失败返回非0
*/
	STDMETHOD_(int, SetFileChangedFunction)(XJSVGFILECHANGED) = 0;
	
/*
*  @brief   	GetAttributeValue	 取得指定元素指定属性的值 
*  @param 		[In]a param of Type  int  指定元素
*  @param		[In]a param of Type	 int  指定属性
*  @return 		CString	属性值
 */	
	STDMETHOD_(CString, GetAttributeValue)(int nElementIndex, int nAttrIndex) = 0;

/*
*  @brief   	SetAttributeValue	 设置指定元素指定属性的值 
*  @param 		[In]a param of Type  int  指定元素
*  @param		[In]a param of Type	 int  指定属性
*  @param		[In]a param of Type  CString 指定属性值
*  @return 		void
 */	
	STDMETHOD_(void, SetAttributeValue)(int nElementIndex, int nAttrIndex, CString sValue, CScrollView* pView) = 0;

/*
*  @brief   	GetElementType		取得指定元素的类型索引 
*  @param 		[In]a param of Type  int  指定元素ID
*  @return 		int	元素类型
 */	
	STDMETHOD_(int, GetElementType)(int nElementIndex) = 0;

/*
*  @brief   	GetLastAttrChange	 取得最后一次属性改变信息 
*  @param 		[In]a param of Type  int  
*  @return 		int	
 */
	STDMETHOD_(void, GetLastAttrChange)(CTypedPtrArray<CPtrArray, AttrChange*>&) = 0;

/*
 *  @brief   	GetAttrChNameInfo	 取得指定属性的中文名和属性说明信息 
 *  @param 		[In]a param of Type  int  指定属性ID
 *  @param 		[Out]a param of Type  CString&  中文名
 *  @param 		[Out]a param of Type  CString&  属性说明
 *  @param 		[In]a param of Type  CString  对应的元素名
 *  @return 	void 
 */
	STDMETHOD_(void, GetAttrChInfo)(int nAttrIndex, CString& sChName, CString& sNote, CString sEleName = "") = 0;

/*
 *  @brief   	GetEnumAttrDispalyValues	 取得指定枚举属性的所有显示值(显示值不是实际赋给属性的值)列表 
 *  @param 		[In]a param of Type  int  指定属性ID
 *  @param 		[Out]a param of Type  CStringArray&  保存所有显示值
 *  @return 	void 
 */
	STDMETHOD_(void, GetEnumAttrDisplayValues)(int nAttrIndex, CStringArray& arrDisplayValues) = 0;

/*
 *  @brief   	GetEnumAttrValueIndex	 取得枚举属性的值对应其值列表中的索引号 
 *  @param 		[In]a param of Type  int  指定元素
 *  @param 		[In]a param of Type  int  指定属性
 *  @return 	int 值的索引号
 */
	STDMETHOD_(int, GetEnumAttrValueIndex)(int nEleIndex, int nAttrIndex) = 0;

/*
 *  @brief   	SetEnumAttrValueIndex	 使用枚举属性值列表的索引号设置属性的值 
 *  @param 		[In]a param of Type  int  指定元素
 *  @param 		[In]a param of Type  int  指定属性
 *  @param 		[In]a param of Type  int  指定值的索引号
 *  @return 	void 
 */
	STDMETHOD_(void, SetEnumAttrValueIndex)(int nEleIndex, int nAttrIndex, int nValueIndex, CScrollView* pView) = 0;

/*
 *  @brief   	GetCssStyleNames	 取得所有CSS样式名 
 *  @param 		[Out]a param of Type  CStringArray&  保存CSS样式名
 *  @return 	void 
 */
	STDMETHOD_(void, GetCssStyleNames)(CStringArray& arrCssStyle) = 0;

/*
 *  @brief   	GetCssStyleIndex	 取得指定元素的CSS样式索引 
 *  @param 		[In]a param of Type  int  指定元素索引
 *  @return 	int 样式索引
 */
	STDMETHOD_(int, GetCssStyleIndex)(int nEleIndex) = 0;

/*
 *  @brief   	SetCssStyleValueIndex	 使用风格索引设置指定元素的引用风格的值 
 *  @param 		[In]a param of Type  int  指定元素
 *  @param 		[In]a param of Type  int  指定风格索引
 *  @return 	void 
 */
	STDMETHOD_(void, SetCssStyleValueIndex)(int nEleIndex, int nValueIndex, CScrollView* pView) = 0;

/*
 *  @brief   	DoScaleShape	缩放指定图形	  
 *  @param 		[In]a param of Type  int  指定要进行缩放操作的图形,<0时视为对当前所有选中图形进行操作
 *  @param 		[In]a param of Type  double  X轴方向缩放倍数
 *  @param 		[In]a param of Type  double  Y轴方向缩放倍数
 *  @return 	void 
 */
	STDMETHOD_(void, DoScaleShape)(int nEleIndex, double dScaleX, double dScaleY, CScrollView* pView) = 0;

/*
 *  @brief   	DoScaleShape	旋转指定图形	  
 *  @param 		[In]a param of Type  int  指定要进行旋转操作的图形,<0时视为对当前所有选中图形进行操作
 *  @param 		[In]a param of Type  double  旋转角度。正数为顺时针，负数为逆时针
 *  @return 	void 
 */
	STDMETHOD_(void, DoRotateShape)(int nEleIndex, double dAngle, CScrollView* pView) = 0;

/*
 *  @brief   	DoTranslateShapes	移动指定图形	  
 *  @param 		[In]a param of Type  int  指定要进行移动操作的图形,<0时视为对当前所有选中图形进行操作
 *  @param 		[In]a param of Type  int  X轴方向移动距离
 *  @param 		[In]a param of Type  int  Y轴方向移动距离
 *  @return 	void 
 */
	STDMETHOD_(void, DoTranslateShapes)(int nEleIndex, int nX, int nY, CScrollView* pView) = 0;

/*
 *  @brief   	SetActiveMouseDraw	设置当前鼠标绘制图形的类型	  
 *  @param 		[In]a param of Type  int  鼠标绘制图形的类型
 *  @return 	void 
 */
	STDMETHOD_(void, SetActiveMouseDraw)(int nMouseDrawMode) = 0;

/*
 *  @brief   	CreateNewDoc	创建SVG文档	  
 *  @return 	void 
 */
	STDMETHOD_(BOOL, CreateNewDoc)() = 0;

/*
 *  @brief   	GetElementValue	取得指定元素的值，当前只有text元素有值	  
 *  @param 		[In]a param of Type  int  指定元素ID
 *  @return 	CString 元素值
 */
	STDMETHOD_(CString, GetElementValue)(int nEleIndex) = 0;

/*
 *  @brief   	SetElementValue	设置指定元素的值，当前只有text元素有值	  
 *  @param 		[In]a param of Type  int  指定元素ID
 *  @param 		[In]a param of Type  CString  指定元素值
 *  @return 	void
 */
	STDMETHOD_(void, SetElementValue)(int nEleIndex, CString sValue, CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	执行一次撤销操作	  
 *  @return 	void
 */
	STDMETHOD_(void, DoUndo)(CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	执行一次还原操作	  
 *  @return 	void
 */
	STDMETHOD_(void, DoRedo)(CScrollView* pView) = 0;

/*
 *  @brief   	SetElementValue	删除选中元素	  
 *  @return 	int
 */
	STDMETHOD_(int, DeleteElement)(CScrollView* pView) = 0;

/*
 *  @brief   	ComposeSelectedEles	将选中元素进行组合	  
 *  @return 	int
 */
	STDMETHOD_(int, ComposeSelectedEles)(CScrollView* pView) = 0;

/*
 *  @brief   	DeComposeElement	 将指定元素解组 
 *  @param 		[In]a param of Type  int  指定要解组的元素
 *  @return 	int
 */
	STDMETHOD_(int, DeComposeElement)(int nEleIndex, CScrollView* pView) = 0;

/*
 *  @brief   	CreateShapeByTemplate	 使用模板添加新图形 
 *  @param 		[In]a param of Type  CString  模板名
 *  @param 		[In]a param of Type  CString  模板文件全路径
 *  @param 		[In]a param of Type  CPoint  图形初始位置
 *  @param 		[In]a param of Type  CScrollView*  
 *  @return 	int 成功返回新图形索引.失败返回<0
 */
	STDMETHOD_(int, CreateShapeByTemplate)(CString sLinkName, CString sFilePath, CPoint pt, CScrollView* pView) = 0;

/*
 *  @brief   	GetFirstSymbolName)	 取得第一个预定义图元的名称 
 *  @return 	CString 第一个预定义图元的名称 
 */
	STDMETHOD_(CString, GetFirstSymbolName)() = 0;

/*
 *  @brief   	AlignmentSelectedEles	 简要的函数功能说明文字 
 *  @param 		[In]a param of Type  int  对齐风格，参见constant.h
 *  @param 		[In]a param of Type  CScrollView*  输入参数说明
 *  @return 	
 */
	STDMETHOD_(void, AlignmentSelectedEles)(int nAlignmentType, CScrollView* pView) = 0;

/*
 *  @brief   	OnDragOver	 响应鼠标拖放时的鼠标移动消息 
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @param 		[In]a param of Type  CPoint  鼠标位置
 *  @return 	 
 */
	STDMETHOD_(void, OnDragOver)(CScrollView* pView, CPoint point) = 0;

/*
 *  @brief   	SetDevTypeLayer	 设置指定图形所属设备层
 *  @param 		[In]a param of Type  int  指定图形元素
 *  @param 		[In]a param of Type  CString  指定设备层
 *  @return 	
 */
	STDMETHOD_(void, SetDevTypeLayer)(int nEleIndex, CString sLayerName) = 0;

/*
 *  @brief   	GetDevTypeLayer	 取得指定图形的设备层名称 
 *  @param 		[In]a param of Type  int  指定图形
 *  @return 	CString 设备成名称
 */
	STDMETHOD_(CString, GetDevTypeLayer)(int nEleIndex) = 0;

/*
 *  @brief   	LoadCssStyle	 载入CSS风格内容 
 *  @param 		[In]a param of Type  CString  CSS风格内容
 *  @return 	void 
 */
	STDMETHOD_(void, LoadCssStyle)(CString sContent) = 0;

/*
 *  @brief   	PolylinePort	 对折线拐点进行操作 
 *  @param 		[In]a param of Type  int  指定图形元素
 *  @param 		[In]a param of Type  int  操作类型。0-增加拐点。1-删除拐点
 *  @return 	void
 */
	STDMETHOD_(void, PolylinePort)(int nEleIndex, int nType, CScrollView* pView) = 0;

/*
 *  @brief   	AddNote	 给指定图形元素增加注释文字 
 *  @param 		[In]a param of Type  int  指定元素
 *  @param 		[In]a param of Type  CString  指定注释
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	int 新增加的注释文字元素
 */
	STDMETHOD_(int, AddNote)(int nEleIndex, CString sNote, CScrollView* pView) = 0;

/*
 *  @brief   	CopySelectedShapes	 复制所选图形 
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	void 
 */
	STDMETHOD_(void, CopySelectedShapes)(CScrollView* pView) = 0;

/*
 *  @brief   	CutSelectedShapes	 剪切所选图形 
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	void
 */
	STDMETHOD_(void, CutSelectedShapes)(CScrollView* pView)= 0;

/*
 *  @brief   	PasteShapes	 粘贴图形 
 *  @param 		[In]a param of Type  CPoint  图形位置
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	void 
 */
	STDMETHOD_(void, PasteShapes)(CPoint point, CScrollView* pView) = 0;

/*
 *  @brief   	GetDefaultAttr	 取得文档默认属性 
 *  @param 		[Out]a param of Type  DefaultAttr&  文档默认属性
 *  @return 	void 
 */
	STDMETHOD_(void, GetDefaultAttr)(DefaultAttr& defaultAttr) = 0;

/*
 *  @brief   	SetDefaultAttr	 设置文档默认属性 
 *  @param 		[In]a param of Type  DefaultAttr  文档默认属性
 *  @return 	void 
 */
	STDMETHOD_(void, SetDefaultAttr)(DefaultAttr defaultAttr) = 0;

/*
 *  @brief   	SetCanvasSize	 设置图形画布大小 
 *  @param 		[In]a param of Type  Size  图形画布大小
 *  @return 	CSize 当前文档逻辑大小
 */
	STDMETHOD_(CSize, SetCanvasSize)(CSize szDoc) = 0;

/*
 *  @brief   	GetCanvasSize	 取得画布大小 
 *  @return 	CSize 画布大小
 */
	STDMETHOD_(CSize, GetCanvasSize)() = 0;

/*
 *  @brief   	GetViewboxSize	 取得viewbox大小 
 *  @return 	CSize viewbox大小
 */
	STDMETHOD_(CSize, GetViewboxSize)() = 0;

/*
 *  @brief   	GetBKColor	 取得图形背景色 
 *  @return 	COLORREF 图形背景色
 */
	STDMETHOD_(COLORREF, GetBKColor)() = 0;

/*
 *  @brief   	SetBkColor	 设置图形背景色 
 *  @param 		[In]a param of Type  COLORREF  指定背景色
 *  @return 	void 
 */
	STDMETHOD_(void, SetBkColor)(COLORREF colBK) = 0;

/*
 *  @brief   	SetEditStatus	 设置图形状态.0-编辑。1-运行 
 *  @param 		[In]a param of Type  int  指定图形状态
 *  @return 	void 
 */
	STDMETHOD_(void, SetEditStatus)(int nStatus) = 0;

/*
 *  @brief   	GetEditStatus	 取得图形状态。
 *  @return 	int 0-编辑。1-运行
 */
	STDMETHOD_(int, GetEditStatus)() = 0;

/*
 *  @brief   	CanUndo	 判断文档当前是否可执行UNDO操作 
 *  @return 	BOOL	可执行返回TRUE,不可执行返回FALSE 
 */
	STDMETHOD_(BOOL, CanUndo)() = 0;

/*
 *  @brief   	CanRedo	 判断文档当前是否可执行UNDO操作 
 *  @return 	BOOL	可执行返回TRUE,不可执行返回FALSE 
 */
	STDMETHOD_(BOOL, CanRedo)() = 0;

/*
 *  @brief   	CanPaste	 判断文档当前是否可执行粘贴操作 
 *  @return 	BOOL		可执行返回TRUE,不可执行返回FALSE	 
 */
	STDMETHOD_(BOOL, CanPaste)() = 0;

/*
 *  @brief   	LinkSelectedEles	 将选中图形设置为链接对象，设置为链接对象后点击此图形将转到链接地址 
 *  @return 	int			新生成的链接对象编号 
 */
	STDMETHOD_(int, LinkSelectedEles)(CScrollView* pView) = 0;

/*
 *  @brief   	GetFilePath	 取得SVG文件路径 
 *  @return 	CString SVG文件路径
 */
	STDMETHOD_(CString, GetFilePath)() = 0;

/*
 *  @brief   	SetGridSpace	 设置辅助网格间距 
 *  @param 		[In]a param of Type  int  辅助网格间距。-1表示不使用辅助网格
 *  @return 	void 
 */
	STDMETHOD_(void, SetGridSpace)(int nGridSpace) = 0;

/*
 *  @brief   	AddImage	 设置辅助网格间距 
 *  @param 		[In]a param of Type  CPoint  图片起始点
 *  @param 		[In]a param of Type  CSize  图片大小
 *  @param 		[In]a param of Type  CString  图片路径
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	int 新插入的图片元素索引 
 */
	STDMETHOD_(int, AddImage)(CPoint ptTopLeft, CSize szSize, CString sFilePath, CScrollView* pView) = 0;

/*
 *  @brief   	GetCssStroke	 取得指定CSS定义中的指定属性的值 
 *  @param 		[In]a param of Type  CString  指定CSS定义名称
 *  @param 		[In]a param of Type  CString  指定指定属性名
 *  @return 	CString CSS定义中指定属性的值 
 */
	STDMETHOD_(CString, GetCssStyleValue)(CString sCssName, CString sStyleName) = 0;

/*
 *  @brief   	SetCssStroke	 设置指定CSS定义中的指定属性的值
 *  @param 		[In]a param of Type  CString  指定CSS定义名称
 *  @param 		[In]a param of Type  CString  指定指定属性名
 *  @param 		[In]a param of Type  CString  指定指定属性值
 *  @return 	void 
 */
	STDMETHOD_(void, SetCssStyleValue)(CString sCssName, CString sStyleName, CString sStyleValue) = 0;

/*
 *  @brief   	SetCssStroke	 取得CSS描述内容 
 *  @return 	CString CSS描述内容 
 */
	STDMETHOD_(CString, GetCssContent)() = 0;

	/*
 *  @brief   	AddCssStyleItem	 添加CSS风格属性 
 *  @param 		[In]a param of Type  CString  指定CSS名
 *  @param 		[In]a param of Type  CString  指定属性名
 *  @param 		[In]a param of Type  CString  指定属性值
 *  @return 	int 0:添加成功.-1:已存在此属性,不再添加.-2:不支持的属性.-3:创建属性失败.-4:创建CSS对象失败.-5:添加到CSS链表失败
 */
	STDMETHOD_ (int, AddCssStyleItem)(CString sCSSName, CString sStyleName, CString sStyleValue) = 0;
/*
 *  @brief   	DeleteCssStyleItem	 删除CSS风格属性 
 *  @param 		[In]a param of Type  CString  指定CSS名
 *  @param 		[In]a param of Type  CString  指定属性名
 *  @return 	void 
 */
	STDMETHOD_ (void, DeleteCssStyleItem)(CString sCSSName, CString sStyleName) = 0;

/*
 *  @brief   	SelectAll	 选中所有图形 
 *  @return 	void 
 */
	STDMETHOD_ (void, SelectAll)() = 0;

/*
 *  @brief   	DeComposeLink	 将指定元素链接解除 
 *  @param 		[In]a param of Type  int  指定要解组链接的元素
 *  @return 	int
 */
	STDMETHOD_(int, DeComposeLink)(int nEleIndex, CScrollView* pView) = 0;
	
/*
 *  @brief   	AddElement	 添加指定类型的SVG元素到文档中 
 *  @param 		[In]a param of Type  int  父节点索引，-1表示根节点
 *  @param 		[In]a param of Type  CString  元素类型名
 *  @return 	int 添加成功将返回新元素的索引，失败返回-1
 */
	STDMETHOD_(int, AddElement)(int parentIndex, CString elementName) = 0;
};
// {06CC779C-FC34-4995-96AD-41579B8D2486}
static const IID IID_ISvgOperate = 
{ 0x6cc779c, 0xfc34, 0x4995, { 0x96, 0xad, 0x41, 0x57, 0x9b, 0x8d, 0x24, 0x86 } };

#endif
