#if !defined(XJREPORTINTERFACEY_H_)
#define XJREPORTINTERFACEY_H_

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <AFXTEMPL.H>

typedef int (*RptPFUNCALLBACK)(CString sKey, CString & sValue);
typedef int (*RptPFUNCALLBACKEX)(CString sKey, CString & sValue, void* pObject);

struct IXJReport:public IUnknown
{
/*
 *  @brief   	OpenReportFile	 打开报表打印模板文件 
 *  @param 		[In]a param of Type  CString  文件名(全路径)
 *  @return 	CSize	如果打开成功, 返回文档大小, 打开失败,返回(0, 0)
 */
	STDMETHOD_(CSize, OpenReportFile)(CString) = 0;

/*
 *  @brief   	OpenReportFileEx	 打开报表打印模板文件 
 *  @param 		[In]a param of Type  CString  文件名(全路径)
 *  @param 		[In]a param of Type  void*  
 *  @return 	CSize	如果打开成功, 返回文档大小, 打开失败,返回(0, 0)
 */
	STDMETHOD_(CSize, OpenReportFileEx)(CString, void*) = 0;

/*
 *  @brief   	SetQueryFunction	 设置查询关键字的回调函数 
 *  @param 		[In]a param of Type  RptPFUNCALLBACK 回调函数指针
 *  @return 	int	设置成功返回0, 失败返回非0
 */
	STDMETHOD_(int, SetQueryFunction)(RptPFUNCALLBACK) = 0;

/*
 *  @brief   	SetQueryFunctionEx	 设置查询关键字的回调函数 
 *  @param 		[In]a param of Type  RptPFUNCALLBACKEX 回调函数指针
 *  @return 	int	设置成功返回0, 失败返回非0
 */
	STDMETHOD_(int, SetQueryFunctionEx)(RptPFUNCALLBACKEX) = 0;

/*
 *  @brief   	Draw	 绘制报表图形 
 *  @param 		[In]a param of Type  CDC* 绘制设备的设备内容指针
 *  @return 	void
 */
	STDMETHOD_(void, Draw)(CDC*) = 0;

	/*
	* @brief	DrawEx	
	* @param	[In]a param of Type CDC* 绘制设备的设备内容指针		
	* @param	[In]a param of Type int	起点位置X轴坐标	
	* @param	[In]a param of Type int	起点位置Y轴坐标
	* @return   STDMETHOD_(void,	
	*/
	STDMETHOD_(void, DrawEx)(CDC*, int, int) = 0;

/*
 *  @brief   	FillValueData	 重新设置报表中各元素的真实值 
 *  @return 	CSize 返回文档大小
 */
	STDMETHOD_(CSize, FillValueData)() = 0;

/*
 *  @brief   	FillValueData	 打印当前的报表 
 *  @return 	void
 */
	STDMETHOD_(void, Print)() = 0;

/*
 *  @brief   	PrintDialogModel	 打印对话框显示方式(作用范围: 本次调用) 
 *  @param 		[In]a param of Type  int 0-在默认打印机能连接的情况下直接使用默认打印机,不弹出打印设置对话框
										 1-无论如何都弹出打印设置对话框
										 初始值为0
 *  @return 	void
 */
	STDMETHOD_(void, PrintDialogModel)(int) = 0;

/*
 *  @brief   	PrintPageSet	 打印纸设置(作用范围: 本次调用) 
 *  @param 		[In]a param of Type  int 打印纸大小,初始值为A4,可填入的值参见MSDN -- DEVMODE
 *	@param		[In]a param of Type	 int 排版方式. 1表示竖版. 2为横版. 初始值为横版
 *	@param		[In]a param of Type	 int 打印分辨率(dpi). 正整数值. 初始值为600
 *  @return 	void
 */
	STDMETHOD_(void, PrintPageSet)(int, int, int) = 0;

/*
 *  @brief   	PrintCopies	 设置打印份数 (作用范围: 本次打印)
 *  @param 		[In]a param of Type  int 设置打印份数, 正整数值, 初始值为1
 *  @return 	void
 */
	STDMETHOD_(void, PrintCopies)(int) = 0;

/*
 *  @brief   	PrintRange	 打印范围设置 (作用范围: 本次打印)
 *  @param 		[In]a param of Type  int 打印范围, 0表示打印全部, 1表示根据所选页码范围打印. 初始值为0
 *	@param		[In]a param of Type	 int 开始打印的页码. 当根据所选页码范围打印时有效.初始值为1
 *	@param		[In]a param of Type	 int 打印结束页码, 根据所选页码范围打印有效.初始值为32765
 *  @return 	void
 */
	STDMETHOD_(void, PrintRange)(int, int, int) = 0;

/*
 *  @brief   	GetHitElement	 得到被点击的元素的ID
 *  @param 		[In]a param of Type  CPoint	点击位置(逻辑坐标)
 *  @return 	int	如果有被点击的元素,返回>=0 的元素ID, 否则返回-1
 */
	STDMETHOD_(int, GetHitElement)(CPoint ptHit) = 0;

/*
 *  @brief   	GetValueCode	 得到指定元素的值代码
 *  @param 		[In]a param of Type  int	元素ID
 *  @return 	CString		元素值代码
 */
	STDMETHOD_(CString, GetValueCode)(int nID) = 0;

/*
 *  @brief   	SetElementTextColor	 设置指定元素的文字颜色
 *  @param 		[In]a param of Type  int	元素ID
 *	@param		[In]a param of Type	 COLORREF 指定颜色
 *  @return 	CRect	相关区域
 */
	STDMETHOD_(CRect, SetElementTextColor)(int nID, COLORREF colText) = 0;

/*
 *  @brief   	RevertTextColor	 还原指定元素的文字颜色 
 *  @param 		[In]a param of Type  int  指定元素ID
 *  @return 	CRect 相关要刷新的区域
 */
	STDMETHOD_(CRect, RevertTextColor)(int nID) = 0;
	
/*
 *  @brief   	GetStartPosition	 取得图形起始点位置 
 *  @return 	CSize 图形起始点位置
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