//##ModelId=49B87B990375
typedef int (*STSVGPFUNCALLBACK)(CRect rcBlink);

//##ModelId=49B87B99038A
struct ISvgDoc:public IUnknown
{

/**	\brief	The OpenSvgFile function
			��SVG�ļ�
	\param	&	a parameter of type CString
			�ļ�����·��, �����ļ���
	\return	STDMETHOD_(CSize)
			����ͼ�δ�С
	
*/
	//##ModelId=49B87B99038D
	STDMETHOD_(CSize, OpenSvgFile)(CString &) = 0; 

/**	\brief	The SaveSvgFile function
			����SVG�ļ�
	\param	&	a parameter of type CString
			�����ļ���Ŀ���ַ
	\return	
    STDMETHOD_(BOOL)
			�ɹ�����TRUE, ʧ�ܷ���FALSE
*/
	//##ModelId=49B87B990390
	STDMETHOD_(BOOL, SaveSvgFile)(CString &) = 0;
	
/**	\brief	The DrawSvg function
			����SVGͼ��
	\param	*	a parameter of type CDC
			
	\return	STDMETHOD_(void)	
*/
	//##ModelId=49B87B99039A
	STDMETHOD_(void, DrawSvg)(CDC * ) = 0;

/**	\brief	The LButtonDown) function
			������¹���
	\param	&	a parameter of type UINT
			����־λ
	\param  a Parameter of type CPoint
			���λ��
	\return	STDMETHOD_(CRect)
			��ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B99039D
	STDMETHOD_(CRect, LButtonDown)(UINT &, CPoint) = 0;


/**	\brief	The MouseMove function
			����ƶ�����
	\param	&	a parameter of type UINT
			����־λ
	\param  a Parameter of type CPoint
			���λ��
	\return	STDMETHOD_(CRect)
			��ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B9903A1
	STDMETHOD_(CRect, MouseMove)(UINT &, CPoint) = 0;

/**	\brief	The LButtonUp) function
			���������
	\param	&	a parameter of type UINT
			����־λ
	\param  a Parameter of type CPoint
			���λ��
	\return	STDMETHOD_(CRect)
			��ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B9903A5
	STDMETHOD_(CRect, LButtonUp)(UINT &, CPoint) = 0;

/**	\brief	The GetSelectedDevice) function
			�õ�����ѡ���豸��ID
	\return	STDMETHOD_(CString
			����ѡ���豸��ID, д��һ���ַ�����, ��","�ŷָ�	
*/
	//##ModelId=49B87B9903A9
	STDMETHOD_(CString, GetSelectedDevice)() = 0;

/**	\brief	The GetDeviceType) function
			�õ�ָ���豸����
	\param	strID	a parameter of type LPTSTR
					�豸ID
	\return	STDMETHOD_(CString,
				����ֵ	
*/
	//##ModelId=49B87B9903AB
	STDMETHOD_(CString, GetDeviceType)(CString strID) = 0;

/**	\brief	The DeleteDeviceShape) function
			ɾ��Ŀǰѡ�е�ͼ��
	\return	STDMETHOD_(CRect,
			������ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B9903AE
	STDMETHOD_(CRect, DeleteDeviceShape)() = 0;

/**	\brief	The DeleteDeviceShapeEx) function
			ɾ��ͼ�μ�ǿ��(������ָ��ͼ��)
	\param	&	a parameter of type CString
			ָ��ͼ�ε�ID(�ɶ���)
	\return	STDMETHOD_(CRect)
			���ˢ�·�Χ
*/
	//##ModelId=49B87B9903B0
	STDMETHOD_(CRect, DeleteDeviceShapeEx)(CString &) = 0;

/**	\brief	The LinearMove) function
			ƽ��ͼ�μ�ǿ��(������ָ��ͼ��)
	\param	&	a parameter of type CString ָ��ͼ�ε�ID(�ɶ���)
	\param	&	a parameter of type int	X�᷽��ƽ�ƾ���
	\param	&	a parameter of type int Y�᷽��ƽ�ƾ���

	\return	STDMETHOD_(CRect)
			��ص�ˢ�·�Χ
*/
	//##ModelId=49B87B9903B3
	STDMETHOD_(CRect, LinearMoveEx)(CString &, int &, int &) = 0;

/**	\brief	The LinearMoveLimit) function
			ƽ��ͼ��, ֻ���ƶ�ѡ�е�ͼ��
	\param	&	a parameter of type int X�᷽��ƽ�ƾ���
	\param	&	a parameter of type int Y�᷽��ƽ�ƾ���

	\return	STDMETHOD_(CRect,
			��ص�ˢ�·�Χ
*/
	//##ModelId=49B87B9903B8
	STDMETHOD_(CRect, LinearMove)(int &, int &) = 0;

/**	\brief	The UndoSelect) function
			ȡ������ѡ��
	\return	CRect
			��ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B9903BC
	STDMETHOD_(CRect, UndoSelect)() = 0;
	
/**	\brief	The FillTemplate) function
			���ģ��ͼԪ
	\param	path	a parameter of type CString
			ģ��ͼԪ����ļ���·��
	\return	void	
*/
//	STDMETHOD_(void, FillTemplate)(CString path) = 0;

/**	\brief	The AddTemplateShape) function
			���붨��õ�ģ��ͼ�ε�SVGͼ����
	\param	type	a parameter of type CString ģ��ͼ������ʾ���豸����
	\param	pt	a parameter of type CPoint  ģ��ͼ�γ�ʼ��
	\param	msg	a parameter of type CString �����Ŀ��
	\param  msg a parameter of type COLORREF ��ѹ�ȼ���ɫ
	\return	CRect
			 ��ص�ˢ�·�Χ	
*/
//	STDMETHOD_(CRect, AddTemplateShape)(CString type, CPoint pt, CString id, CString Voltage) = 0;

	/**	\brief	The AddBlink) function
			��ָ��ID��ͼ�μ��뵽��˸������
	\param	id	a parameter of type CString
			ָ��ID
	\return	CRect
			����ˢ�·�Χ
*/
	//##ModelId=49B87B9903BE
	STDMETHOD_(CRect, AddBlink)(CString id) = 0;

/**	\brief	The RemoveAtBlink) function
			��ָ��ID��ͼ���Ƴ���˸����
	\param	id	a parameter of type CString
			ָ��ID
	\return	CRect
			��ص�ˢ�·�Χ
*/
	//##ModelId=49B87B9903C1
	STDMETHOD_(CRect, RemoveAtBlink)(CString id) = 0;

/**	\brief	The RemoveAllBlink) function
			�Ƴ���˸����������ͼ��
	\return	CRect
			��ص�ˢ�·�Χ	
*/
	//##ModelId=49B87B9903C4
	STDMETHOD_(CRect, RemoveAllBlink)() = 0;

/**	\brief	The GetDeviceShapeSize) function
			�õ�ָ��ͼ�εķ�Χ,���ص�ֵΪ�߼�����ֵ
	\param	id	a parameter of type CString
			ͼ��ID
	\return	STDMETHOD_(CRect,
			ͼ�η�Χ, ����Ҳ�����ͼ�η���(32766, 32766, -32766, -32766)	
*/
	//##ModelId=49B87B9903C6
	STDMETHOD_(CRect, GetDeviceShapeSize)(CString id) = 0;
/**	\brief	The FindShape) function
			�����Ƿ���ָ��ͼ��
	\param	id	a parameter of type CString
			ָ��ͼ��ID
	\return	STDMETHOD_(BOOL,
			����з���true,û�з���false	
*/
	//##ModelId=49B87B9903C9
	STDMETHOD_(BOOL, FindShape)(CString id) = 0;
/**	\brief	The SetShapeSelected) function
			����ָ��ͼ��Ϊѡ��״̬. ֮ǰ��ѡ�񽫻���ʧ, ֻ�е�ǰͼ�α�ѡ��
	\param	id	a parameter of type CString
			ָ��ͼ��ID
	\return	STDMETHOD_(void	
*/
	//##ModelId=49B87B9903CC
	STDMETHOD_(void, SetShapeSelected)(CString id) = 0;

/**	\brief	The SetDeviceStatus) function
			����ָ��ͼ��Ϊѡ��״̬. ֮ǰ��ѡ�񽫻���ʧ, ֻ�е�ǰͼ�α�ѡ��
	\param	id	a parameter of type CString
			ָ��ͼ��ID
	\param	nStatus	a parameter of type COLORREF
			ָ������״̬.
	\return	STDMETHOD_(void	
*/
	//##ModelId=49B87B9903CF
	STDMETHOD_(void, SetDeviceStatus)(CString id, COLORREF col) = 0;

	/**	\brief	The ZoomView) function
			�ı���ͼ��С
	\param	flag	a parameter of type int
			ָ������, 0Ϊ��С,1Ϊ�Ŵ�
	\param	width_zoom	a parameter of type int
			��ͼ�������Ŵ�С.
	\param  height_zoom a parameter of type int
			��ͼ�������Ŵ�С
	\return	STDMETHOD_(void	
			��ͼ��С
*/
	//##ModelId=49B87B9903D3
	STDMETHOD_(CSize, ZoomView)(int flag, int width_zoom, int height_zoom) = 0;

	/**	\brief	The SetBlinkTime function
			����ͼ����˸��ʱ����
	\param	flag	a parameter of type int
			ʱ��(����)
	\return	STDMETHOD_(void	
			
*/
	//##ModelId=49B87B9903D8
	STDMETHOD_(void, SetBlinkTime)(int nTime) = 0;

	/*
	*  @brief   	SetBlinkUpdateFunction	 ���ò�ѯ�ؼ��ֵĻص����� 
	*  @param 		[In]a param of Type  STSVGPFUNCALLBACK �ص�����ָ��
	*  @return 		int	���óɹ�����0, ʧ�ܷ��ط�0
	*/
	//##ModelId=49B87B9903DB
	STDMETHOD_(int, SetBlinkUpdateFunction)(STSVGPFUNCALLBACK) = 0;
};

// {7A52DB73-DDC0-4ed3-8F8F-50C41B6D2608}
static const IID IID_ISvgDoc = 
{ 0x7a52db73, 0xddc0, 0x4ed3, { 0x8f, 0x8f, 0x50, 0xc4, 0x1b, 0x6d, 0x26, 0x8 } };
