#if !defined(AFX_SVGVIEWERINTERFACE_H)
#define AFX_SVGVIEWERINTERFACE_H

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <AFXTEMPL.H>

struct ISvgViewer:public IUnknown
{
/*
*  @brief   	LoadFromFile	 载入SVG文件 
*  @param 		[In]a param of Type  CString  文件路径
*  @param 		[Out]a param of Type  CRect&  文件表示的图形大小
*  @param 		[Out]a param of Type  CString&  错误输出
*  @return 	int <0表示失败。>=0表示成功
 */
	STDMETHOD_(int, LoadFromFile)(CString, CRect&, CString&) = 0;

/*
*  @brief   	XJGetDocContent	 取得图形文档转换为图形文件格式形式的实时内容 
*  @return 	CString 图形文档转换为图形文件格式的实时内容
 */
	STDMETHOD_(CString, GetDocContent)() = 0;

/*
 *  @brief   	SaveToFile	 保存文档到文件 
 *  @param 		[In]a param of Type  CString  新文件路径
 *	@param		[In]a param of Type	 CString  旧文件路径
 *  @return 	BOOL 保存成功返回TRUE,失败返回FALSE 
 */
	STDMETHOD_(BOOL, SaveToFile)(CString, CString sOldFileName="") = 0;

/*
*  @brief   	DrawShapes	 绘制图形 
*  @param 		[In]a param of Type  CDC*  绘制设备指针
*  @return 	void
 */
	STDMETHOD_(void, DrawShapes)(CDC* pDC) = 0;

/*
*  @brief   	DrawShapesEx	 响应鼠标左键按下动作 
*  @param 		[In]a param of Type  CDC*  绘制设备指针
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @return 	void
 */
	STDMETHOD_(void, DrawShapesEx)(CDC* pDC, CScrollView* pView) = 0;

/*
*  @brief   	DoLButtonDown	 响应鼠标左键按下动作 
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @param		[In]a param of Type	 const CPoint& 鼠标位置
*  @param		[In]a param of Type	 UINT 动作标志
*  @param		[In]a param of Type	 UINT 交互工具类型
*  @return 	void
 */
	STDMETHOD_(void, DoLButtonDown)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoLButtonUp	 响应鼠标左键弹起动作 
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @param		[In]a param of Type	 const CPoint& 鼠标位置
*  @param		[In]a param of Type	 UINT 动作标志
*  @param		[In]a param of Type	 UINT 交互工具类型
*  @return 	void
 */
	STDMETHOD_(void, DoLButtonUp)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoMouseMove	 响应鼠标移动动作 
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @param		[In]a param of Type	 const CPoint& 鼠标位置
*  @param		[In]a param of Type	 UINT 动作标志
*  @param		[In]a param of Type	 UINT 交互工具类型
*  @return 	void
 */
	STDMETHOD_(void, DoMouseMove)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoLButtonDown	 响应鼠标右键按下动作 
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @param		[In]a param of Type	 const CPoint& 鼠标位置
*  @param		[In]a param of Type	 UINT 动作标志
*  @param		[In]a param of Type	 UINT 交互工具类型
*  @return 	void
 */
	STDMETHOD_(void, DoRButtonDown)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
*  @brief   	DoRButtonUp	 响应鼠标右键弹起动作 
*  @param 		[In]a param of Type  CScrollView*  视图对象指针
*  @param		[In]a param of Type	 const CPoint& 鼠标位置
*  @param		[In]a param of Type	 UINT 动作标志
*  @param		[In]a param of Type	 UINT 交互工具类型
*  @return 	void
 */
	STDMETHOD_(void, DoRButtonUp)(CScrollView* pView, const CPoint& point, UINT nFlags, UINT drawMode) = 0;

/*
 *  @brief   	GetDevID	 取得指定图形的设备标识 
 *  @param 		[In]a param of Type  int  指定图形索引
 *  @return 	CString		设备标识
 */
	STDMETHOD_(CString, GetDevID)(int nEleIndex) = 0;

/*
 *  @brief   	GetDevType	 取得指定图形的设备类型 
 *  @param 		[In]a param of Type  int  指定图形索引
 *  @return 	CString		设备类型
 */
	STDMETHOD_(CString, GetDevType)(int nEleIndex) = 0;

/*
*  @brief   	GetCurSelected	 取得所有当前所选元素的ID 
*  @param 		[Out]a param of Type  CUIntArray&  当前所选元素的ID
*  @return 		void
 */
	STDMETHOD_(void, GetCurSelected)(CUIntArray& arrSelected) = 0;

/*
 *  @brief   	CartoonControl	 控制所有动画功能的开启或关闭
 *  @param 		[In]a param of Type  BOOL  TRUE表示开启，FALSE表示关闭
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	void
 */
	STDMETHOD_(void, CartoonControl)(BOOL bStart, CScrollView* pView) = 0;

/*
 *  @brief   	BlinkShape	 控制图形闪烁 
 *  @param 		[In]a param of Type  int  图形元素ID
 *  @param 		[In]a param of Type  BOOL  TRUE表示开启闪烁，FALSE表示停止闪烁
 *	@param		[In]a param of Type  int  闪烁间隔时间
 *  @return 	void 
 */
	STDMETHOD_(void, BlinkShape)(int nEleIndex, BOOL bBlink, int nTime = 1000) = 0;

/*
 *  @brief   	ChangedFillColor	 修改指定图形的填充色 
 *  @param 		[In]a param of Type  int  指定图形元素
 *  @param 		[In]a param of Type  COLORREF  指定填充色
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	void
 */
	STDMETHOD_(void, ChangedFillColor)(int nEleIndex, COLORREF colFill, CScrollView* pView) = 0;

/*
 *  @brief   	ZoomView	 缩放视图 
 *  @param 		[In]a param of Type  int  缩放方式,0-放大/缩小.1-缩放到窗口.2-取消所有缩放.
 *  @param 		[In]a param of Type  CSize  缩放大小,增量
 *  @param 		[In]a param of Type  CSize	视图窗口大小
 *  @return 	CSize 缩放后文档逻辑大小 
 */
	STDMETHOD_(CSize, ZoomView)(int nType, CSize szAdd, CSize szClient) = 0;
	
/*
 *  @brief   	GetIndexByDevID	 由设备ID查找对应关联的图形索引
 *  @param 		[In]a param of Type  CString  指定设备ID
 *  @return 	int 图形索引
 */
	STDMETHOD_(int, GetIndexByDevID)(CString sDevID) = 0;

/*
 *  @brief   	SetSelected	 将指定图形加入到选择 
 *  @param 		[In]a param of Type  int  指定图形索引
 *  @param 		[In]a param of Type  BOOL  是否清除之前的选择
 *  @param 		[In]a param of Type  CScrollView*  视图对象
 *  @return 	 
 */
	STDMETHOD_(void,SetSelected)(int nEleIndex, BOOL bClearOld, CScrollView* pView) = 0;

/*
 *  @brief   	GetShapeSize  取得指定图形的范围 
 *  @param 		[In]a param of Type  int  指定图形
 *  @return 	 图形范围
 */
	STDMETHOD_(CRect,GetShapeSize)(int nEleIndex) = 0;
	
/*
	 *  @brief   	AddMark	 在指定图形的右上角增加一个图标用于标记
	 *	@param 		[In]a param of Type  CSvgElement * Parent	要增加标记的图形，新增的标记图标将为其子元素。
	 *	@param 		[In]a param of Type  CString imagePath	标记图标的路径,为空是只做删除标记操作
	 *	@param 		[In]a param of Type  int imageSize	标记图标大小
	 *  @return 	BOOL	 添加成功返回true,失败返回false
	 */
	STDMETHOD_(int,AddMark)(int nEleIndex,CString imagePath, int imageSize, CScrollView*) = 0;
};
// {7EB4C3A6-1040-480d-8A6A-270C481B941E}
static const IID IID_ISvgViewer = 
{ 0x7eb4c3a6, 0x1040, 0x480d, { 0x8a, 0x6a, 0x27, 0xc, 0x48, 0x1b, 0x94, 0x1e } };

#endif
