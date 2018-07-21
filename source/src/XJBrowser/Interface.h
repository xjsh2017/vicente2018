//##ModelId=49B87B990375
typedef int (*STSVGPFUNCALLBACK)(CRect rcBlink);

//##ModelId=49B87B99038A
struct ISvgDoc:public IUnknown
{

/**	\brief	The OpenSvgFile function
			打开SVG文件
	\param	&	a parameter of type CString
			文件绝对路径, 包括文件名
	\return	STDMETHOD_(CSize)
			返回图形大小
	
*/
	//##ModelId=49B87B99038D
	STDMETHOD_(CSize, OpenSvgFile)(CString &) = 0; 

/**	\brief	The SaveSvgFile function
			保存SVG文件
	\param	&	a parameter of type CString
			保存文件的目标地址
	\return	
    STDMETHOD_(BOOL)
			成功返回TRUE, 失败返回FALSE
*/
	//##ModelId=49B87B990390
	STDMETHOD_(BOOL, SaveSvgFile)(CString &) = 0;
	
/**	\brief	The DrawSvg function
			显现SVG图形
	\param	*	a parameter of type CDC
			
	\return	STDMETHOD_(void)	
*/
	//##ModelId=49B87B99039A
	STDMETHOD_(void, DrawSvg)(CDC * ) = 0;

/**	\brief	The LButtonDown) function
			左键按下功能
	\param	&	a parameter of type UINT
			鼠标标志位
	\param  a Parameter of type CPoint
			鼠标位置
	\return	STDMETHOD_(CRect)
			相关的刷新范围	
*/
	//##ModelId=49B87B99039D
	STDMETHOD_(CRect, LButtonDown)(UINT &, CPoint) = 0;


/**	\brief	The MouseMove function
			鼠标移动功能
	\param	&	a parameter of type UINT
			鼠标标志位
	\param  a Parameter of type CPoint
			鼠标位置
	\return	STDMETHOD_(CRect)
			相关的刷新范围	
*/
	//##ModelId=49B87B9903A1
	STDMETHOD_(CRect, MouseMove)(UINT &, CPoint) = 0;

/**	\brief	The LButtonUp) function
			左键弹起功能
	\param	&	a parameter of type UINT
			鼠标标志位
	\param  a Parameter of type CPoint
			鼠标位置
	\return	STDMETHOD_(CRect)
			相关的刷新范围	
*/
	//##ModelId=49B87B9903A5
	STDMETHOD_(CRect, LButtonUp)(UINT &, CPoint) = 0;

/**	\brief	The GetSelectedDevice) function
			得到所有选中设备的ID
	\return	STDMETHOD_(CString
			所有选中设备的ID, 写在一个字符串中, 以","号分隔	
*/
	//##ModelId=49B87B9903A9
	STDMETHOD_(CString, GetSelectedDevice)() = 0;

/**	\brief	The GetDeviceType) function
			得到指定设备类型
	\param	strID	a parameter of type LPTSTR
					设备ID
	\return	STDMETHOD_(CString,
				类型值	
*/
	//##ModelId=49B87B9903AB
	STDMETHOD_(CString, GetDeviceType)(CString strID) = 0;

/**	\brief	The DeleteDeviceShape) function
			删除目前选中的图形
	\return	STDMETHOD_(CRect,
			返回相关的刷新范围	
*/
	//##ModelId=49B87B9903AE
	STDMETHOD_(CRect, DeleteDeviceShape)() = 0;

/**	\brief	The DeleteDeviceShapeEx) function
			删除图形加强版(可随意指定图形)
	\param	&	a parameter of type CString
			指定图形的ID(可多项)
	\return	STDMETHOD_(CRect)
			相关刷新范围
*/
	//##ModelId=49B87B9903B0
	STDMETHOD_(CRect, DeleteDeviceShapeEx)(CString &) = 0;

/**	\brief	The LinearMove) function
			平移图形加强版(可随意指定图形)
	\param	&	a parameter of type CString 指定图形的ID(可多项)
	\param	&	a parameter of type int	X轴方向平移距离
	\param	&	a parameter of type int Y轴方向平移距离

	\return	STDMETHOD_(CRect)
			相关的刷新范围
*/
	//##ModelId=49B87B9903B3
	STDMETHOD_(CRect, LinearMoveEx)(CString &, int &, int &) = 0;

/**	\brief	The LinearMoveLimit) function
			平移图形, 只能移动选中的图形
	\param	&	a parameter of type int X轴方向平移距离
	\param	&	a parameter of type int Y轴方向平移距离

	\return	STDMETHOD_(CRect,
			相关的刷新范围
*/
	//##ModelId=49B87B9903B8
	STDMETHOD_(CRect, LinearMove)(int &, int &) = 0;

/**	\brief	The UndoSelect) function
			取消所有选择
	\return	CRect
			相关的刷新范围	
*/
	//##ModelId=49B87B9903BC
	STDMETHOD_(CRect, UndoSelect)() = 0;
	
/**	\brief	The FillTemplate) function
			填充模板图元
	\param	path	a parameter of type CString
			模板图元存放文件夹路径
	\return	void	
*/
//	STDMETHOD_(void, FillTemplate)(CString path) = 0;

/**	\brief	The AddTemplateShape) function
			加入定义好的模板图形到SVG图形中
	\param	type	a parameter of type CString 模板图形所表示的设备类型
	\param	pt	a parameter of type CPoint  模板图形初始点
	\param	msg	a parameter of type CString 组号条目号
	\param  msg a parameter of type COLORREF 电压等级颜色
	\return	CRect
			 相关的刷新范围	
*/
//	STDMETHOD_(CRect, AddTemplateShape)(CString type, CPoint pt, CString id, CString Voltage) = 0;

	/**	\brief	The AddBlink) function
			将指定ID的图形加入到闪烁链表中
	\param	id	a parameter of type CString
			指定ID
	\return	CRect
			相差的刷新范围
*/
	//##ModelId=49B87B9903BE
	STDMETHOD_(CRect, AddBlink)(CString id) = 0;

/**	\brief	The RemoveAtBlink) function
			将指定ID的图形移出闪烁链表
	\param	id	a parameter of type CString
			指定ID
	\return	CRect
			相关的刷新范围
*/
	//##ModelId=49B87B9903C1
	STDMETHOD_(CRect, RemoveAtBlink)(CString id) = 0;

/**	\brief	The RemoveAllBlink) function
			移除闪烁链表中所有图形
	\return	CRect
			相关的刷新范围	
*/
	//##ModelId=49B87B9903C4
	STDMETHOD_(CRect, RemoveAllBlink)() = 0;

/**	\brief	The GetDeviceShapeSize) function
			得到指定图形的范围,返回的值为逻辑坐标值
	\param	id	a parameter of type CString
			图形ID
	\return	STDMETHOD_(CRect,
			图形范围, 如果找不到此图形返回(32766, 32766, -32766, -32766)	
*/
	//##ModelId=49B87B9903C6
	STDMETHOD_(CRect, GetDeviceShapeSize)(CString id) = 0;
/**	\brief	The FindShape) function
			查找是否有指定图形
	\param	id	a parameter of type CString
			指定图形ID
	\return	STDMETHOD_(BOOL,
			如果有返回true,没有返回false	
*/
	//##ModelId=49B87B9903C9
	STDMETHOD_(BOOL, FindShape)(CString id) = 0;
/**	\brief	The SetShapeSelected) function
			设置指定图形为选中状态. 之前的选择将会消失, 只有当前图形被选中
	\param	id	a parameter of type CString
			指定图形ID
	\return	STDMETHOD_(void	
*/
	//##ModelId=49B87B9903CC
	STDMETHOD_(void, SetShapeSelected)(CString id) = 0;

/**	\brief	The SetDeviceStatus) function
			设置指定图形为选中状态. 之前的选择将会消失, 只有当前图形被选中
	\param	id	a parameter of type CString
			指定图形ID
	\param	nStatus	a parameter of type COLORREF
			指定设置状态.
	\return	STDMETHOD_(void	
*/
	//##ModelId=49B87B9903CF
	STDMETHOD_(void, SetDeviceStatus)(CString id, COLORREF col) = 0;

	/**	\brief	The ZoomView) function
			改变视图大小
	\param	flag	a parameter of type int
			指定缩放, 0为缩小,1为放大
	\param	width_zoom	a parameter of type int
			视图横向缩放大小.
	\param  height_zoom a parameter of type int
			视图纵向缩放大小
	\return	STDMETHOD_(void	
			视图大小
*/
	//##ModelId=49B87B9903D3
	STDMETHOD_(CSize, ZoomView)(int flag, int width_zoom, int height_zoom) = 0;

	/**	\brief	The SetBlinkTime function
			设置图形闪烁的时间间隔
	\param	flag	a parameter of type int
			时间(毫秒)
	\return	STDMETHOD_(void	
			
*/
	//##ModelId=49B87B9903D8
	STDMETHOD_(void, SetBlinkTime)(int nTime) = 0;

	/*
	*  @brief   	SetBlinkUpdateFunction	 设置查询关键字的回调函数 
	*  @param 		[In]a param of Type  STSVGPFUNCALLBACK 回调函数指针
	*  @return 		int	设置成功返回0, 失败返回非0
	*/
	//##ModelId=49B87B9903DB
	STDMETHOD_(int, SetBlinkUpdateFunction)(STSVGPFUNCALLBACK) = 0;
};

// {7A52DB73-DDC0-4ed3-8F8F-50C41B6D2608}
static const IID IID_ISvgDoc = 
{ 0x7a52db73, 0xddc0, 0x4ed3, { 0x8f, 0x8f, 0x50, 0xc4, 0x1b, 0x6d, 0x26, 0x8 } };
