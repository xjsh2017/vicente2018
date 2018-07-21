#if !defined(COMTRADEPARSEENGINE_H_)
#define COMTRADEPARSEENGINE_H_

#include "../../common/define.h"
#include "../../common/Interface/ComtradeDefine.h"
#include "../../common/XJString.h"
#include "../../common/xjlib.h"

#ifdef OS_WINDOWS
#define LIBNAME_COMTRADEPARSE "XJComtrade.dll"
#else 
#define LIBNAME_COMTRADEPARSE "libXJComtrade.so"
#endif

//��������ComtradeParse����дcp��ͷ
typedef int		(*cp_LoadFile)(char*, char*, XJHANDLE&); //���ļ�
typedef void	(*cp_CloseFile)(XJHANDLE); //�ر��ļ�
typedef void	(*cp_GetCFGAttr)(XJHANDLE, ComtradeCFG&); //ȡ��CFG�ļ���¼�Ĳ���������������
typedef int		(*cp_GetRatesCount)(XJHANDLE); //ȡ�ò���Ƶ������
typedef void	(*cp_GetRateSample)(XJHANDLE, int, float&, int&); //ȡ��ָ������Ƶ�������Ĳ���Ƶ�ʺͲ�������
typedef float	(*cp_GetRate)(XJHANDLE, int); //ȡ��ָ��������Ĳ���Ƶ��
typedef int		(*cp_GetChannelCFG)(XJHANDLE, int, ComtradeChannel&);//ȡ��ָ��ͨ����������Ϣ
typedef int		(*cp_GetChannelType)(XJHANDLE, int);//ȡ��ָ��ͨ��������
typedef int		(*cp_GetTimeRelativeTrigger)(XJHANDLE, int);//ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ��
typedef int		(*cp_GetTimeRelativeStart)(XJHANDLE, int);//ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ��
typedef float	(*cp_GetRealValue)(XJHANDLE, int, int, int);//ȡ��ָ��ͨ��ָ�����������ʵֵ
typedef int		(*cp_GetRecordValue)(XJHANDLE, int, int);//ȡ��ָ��ͨ��ָ��������ļ�¼ֵ
typedef int		(*cp_GetTotalSamples)(XJHANDLE);//ȡ�ò���������
typedef int		(*cp_GetTriggerTime)(XJHANDLE);//ȡ�ù��ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ��
typedef int		(*cp_GetTotalTime)(XJHANDLE);//ȡ����ʱ��
typedef float	(*cp_GetMaxValue)(XJHANDLE,int,int,int);//ȡ��ָ��ͨ�����ֵ
typedef void	(*cp_GetCTPT)(XJHANDLE, float&, float&);//ȡ��CTPT
typedef void	(*cp_SetCTPT)(XJHANDLE, float, float);//����CTPT
typedef double	(*cp_GetTimeRelativeTriggerMSEC)(XJHANDLE, int);//ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ�䣬����΢��ֵ 
typedef double	(*cp_GetTimeRelativeStartMSEC)(XJHANDLE, int);//ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ�䣬����΢��ֵ
typedef HdrObj*	(*cp_GetHdrInfoObject)(XJHANDLE);
typedef std::string	(*cp_GetHdrInfoJSON)(XJHANDLE);

class ComtradeParseEngine
{
public:
	ComtradeParseEngine();
	virtual ~ComtradeParseEngine();
//Attribute
public:
	/** @brief           ��̬����*/
	XJHANDLE	m_hModule;
private:
	/** @brief           ���ļ��ӿ�*/
	cp_LoadFile m_pLoadFile;
	/** @brief           �ر��ļ��ӿ�*/
	cp_CloseFile m_pCloseFile;
	/** @brief           ȡ��CFG�ļ���¼�Ĳ��������������Խӿ�*/
	cp_GetCFGAttr m_pGetCFGAttr;
	/** @brief           ȡ�ò���Ƶ�������ӿ�*/
	cp_GetRatesCount m_pGetRatesCount;
	/** @brief           ȡ��ָ������Ƶ�������Ĳ���Ƶ�ʺͲ��������ӿ�*/
	cp_GetRateSample m_pGetRateSample;
	/** @brief           ȡ��ָ��������Ĳ���Ƶ��*/
	cp_GetRate m_pGetRate;
	/** @brief           ȡ��ָ��ͨ����������Ϣ*/
	cp_GetChannelCFG m_pGetChannelCFG;
	/** @brief           ȡ��ָ��ͨ��������*/
	cp_GetChannelType m_pGetChannelType;
	/** @brief           ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ��*/
	cp_GetTimeRelativeTrigger m_pTimeRelativeTrigger;
	/** @brief           ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ��*/
	cp_GetTimeRelativeStart m_pTimeRelativeStart;
	/** @brief           ȡ��ָ��ͨ��ָ�����������ʵֵ*/
	cp_GetRealValue m_pGetRealValue;
	/** @brief           ȡ�ò���������*/
	cp_GetTotalSamples m_pGetTotalSamples;
	/** @brief           ȡ�ù��ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ��*/
	cp_GetTriggerTime m_pGetTriggerTime;
	/** @brief           ȡ����ʱ��*/
	cp_GetTotalTime m_pGetTotalTime;
	/** @brief           ȡ��ָ��ͨ�����ֵ*/
	cp_GetMaxValue m_pGetMaxValue;
	/** @brief           ȡ��ָ��ͨ��ָ��������ļ�¼ֵ*/
	cp_GetRecordValue m_pGetRecordValue;
	/** @brief           ����CTPT���*/
	cp_SetCTPT m_pSetCTPT;
	/** @brief           ȡ��CTPT���*/
	cp_GetCTPT m_pGetCTPT;
	cp_GetTimeRelativeTriggerMSEC m_pGetTimeRelativeTriggerMSEC;
	cp_GetTimeRelativeStartMSEC m_pGetTimeRelativeStartMSEC;
	cp_GetHdrInfoObject m_pGetHdrInfoObject;
	cp_GetHdrInfoJSON m_pGetHdrInfoJSON;

//Operations
public:
/*
 *  @brief   	LoadCPLib	 װ��XJComtrade.dll 
 *  @return 	bool װ�سɹ�����TRUE,ʧ�ܷ���FALSE
 */
	bool xjcp_LoadCPLib();
public:
/*
 *  @brief   	XJLoadFile	 ���벨���ļ� 
 *  @param 		[In]a param of Type  char*  �����ļ�·��
 *  @param 		[Out]a param of Type  char*&  ������Ϣ
 *  @param 		[Out]a param of Type  int&  ���ɵĲ����ļ����ݶ���ָ��
 *  @return 	int ����ɹ�����>=0, ʧ�ܷ���<0
 */
	int xjcp_LoadFile(char* sFilePath, char* sError, XJHANDLE& hComtrade);

/*
 *  @brief   	XJCloseFile	 �رղ����ļ� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @return 	void
 */
	void xjcp_CloseFile(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetCFGAttr	 ȡ��CFG�ļ���¼�Ĳ��������������� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[Out]a param of Type  ComtradeCFG&  ����������������
 *  @return 	void 
 */
	void xjcp_GetCFGAttr(XJHANDLE hComtrade, ComtradeCFG& cfgAttr);

/*
 *  @brief   	XJGetRatesCount	 ȡ�ò���Ƶ����
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @return 	int ����Ƶ���� 
 */
	int	xjcp_GetRatesCount(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetRateSample	 ȡ��ָ������Ƶ�������Ĳ���Ƶ�ʺͲ������� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ������Ƶ������.������0��ʼ
 *	@param		[Out]a param of Type float& ����Ƶ��
 *	@param		[Out]a param of Type int& ����Ƶ���µĲ��������
 *  @return 	void 
 */
	void xjcp_GetRateSample(XJHANDLE hComtrade, int nRateIndex, float& fSampleRate, int& nSampelCount);

/*
*  @brief   	XJGetRate	 ȡ��ָ��������Ĳ���Ƶ�� 
*  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
*  @param 		[In]a param of Type  int  ָ��������
 *  @return 	float ����Ƶ�� 
 */
	float xjcp_GetRate(XJHANDLE hComtrade, int nSamplePos);

/*
 *  @brief   	XJGetChannelCFG	 ȡ��ָ��ͨ����������Ϣ 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ��ͨ������.������0��ʼ
 *	@param		[Out]a param of Type ComtradeAChannel& ͨ��������Ϣ�ṹ��
 *  @return 	int  ͨ������.1��ʾģ����ͨ����0��ʾ������ͨ��
 */
	int	xjcp_GetChannelCFG(XJHANDLE hComtrade, int nCn, ComtradeChannel& chl);

/*
 *  @brief   	XJGetChannelType	 ȡ��ָ��ͨ�������� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ��ͨ������.������0��ʼ
 *  @return 	int  ͨ������.1��ʾģ����ͨ����0��ʾ������ͨ��
 */
	int	xjcp_GetChannelType(XJHANDLE hComtrade, int nCn);

/*
 *  @brief   	XJGetTimeRelativeTrigger	 ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ�� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *  @return 	int  ����ڹ��ϴ���ʱ�̵�ʱ�䣨���룩
 */
	int	xjcp_GetTimeRelativeTrigger(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetTimeRelativeStart	 ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ�� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *  @return 	int  ����ڲ�����ʼʱ�̵�ʱ�䣨���룩
 */
	int	xjcp_GetTimeRelativeStart(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetRealValue	 ȡ��ָ��ͨ��ָ�����������ʵֵ
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ��ͨ������.������0��ʼ
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *	@param		[In]a param of Type  int  ָ����Ҫ��ֵ�����ͣ�Ĭ��Ϊ����ֵ.0Ϊ����ֵ��1Ϊһ��ֵ
 *  @return 	float  ָ��ͨ��ָ�����������ʵֵ
 */
	float xjcp_GetRealValue(XJHANDLE hComtrade, int nCn, int nSampleNo, int nDataType = 0);

/*
 *  @brief   	XJGetRecordValue	 ȡ��ָ��ͨ��ָ��������ļ�¼ֵ
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ��ͨ������.������0��ʼ
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *  @return		int  ָ��ͨ��ָ��������ļ�¼ֵ
 */
	int xjcp_GetRecordValue(XJHANDLE hComtrade, int nCn, int nSampleNo);

/*
 *  @brief   	XJGetTotalSamples	 ȡ�ò��������� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @return 	int  ����������
 */
	int	xjcp_GetTotalSamples(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetTriggerTime	 ȡ�ù��ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ��
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @return 	int  ���ϴ���ʱ������ڿ�ʼ����ʱ�̵����ʱ�䣨���룩
 */
	int	xjcp_GetTriggerTime(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetTotalTime	 ȡ����ʱ�� 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @return 	int  ��ʱ�䣨���룩
 */
	int	xjcp_GetTotalTime(XJHANDLE hComtrade);

/*
 *  @brief   	XJGetMaxValue	 ȡ��ָ��ͨ�����ֵ
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ��ͨ������.������0��ʼ
 *	@param		[In]a param of Type  int  ָ����Ҫ��ֵ�����ͣ�0Ϊ����ֵ��1Ϊһ��ֵ
 *	@param		[In]a param of Type  int  ָ���Ƿ�ȡ�������ֵ. 0��ʾȡ�������ֵ,1��ʾ��ͨ���Լ������ֵ
 *  @return 	float  ���ֵ
 */
	float xjcp_GetMaxValue(XJHANDLE hComtrade, int nCn, int nDataType, int nGroup = 0);

/*
 *  @brief   	XJGetCTPT	 ȡ��CTPT���
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[Out]a param of Type  float CT���
 *	@param		[Out]a param of Type  float PT���
 *  @return 	void
 */
	void xjcp_GetCTPT(XJHANDLE hComtrade, float& fCt, float& fPT);

/*
 *  @brief   	XJSetCTPT	 ����CTPT���
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  float CT���
 *	@param		[In]a param of Type  float PT���
 *  @return 	void
 */
	void xjcp_SetCTPT(XJHANDLE hComtrade, float fCT, float fPT);

/*
 *  @brief   	XJGetTimeRelativeTrigger	 ȡ��ָ������������ڹ��ϴ���ʱ�̵�ʱ�䣬����΢��ֵ 
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *  @return 	double  ����ڹ��ϴ���ʱ�̵�ʱ�䣨΢�룩
 */
	double xjcp_GetTimeRelativeTriggerMSEC(XJHANDLE hComtrade, int nSampleNo);

/*
 *  @brief   	XJGetTimeRelativeStart	 ȡ��ָ������������ڲ�����ʼʱ�̵�ʱ�䣬����΢��ֵ
 *  @param 		[In]a param of Type  int  ָ�������ļ����ݶ���ָ��
 *  @param 		[In]a param of Type  int  ָ������������.������0��ʼ
 *  @return 	double  ����ڲ�����ʼʱ�̵�ʱ�䣨΢�룩
 */
	double xjcp_GetTimeRelativeStartMSEC(XJHANDLE hComtrade, int nSampleNo);

	HdrObj* xjcp_GetHdrInfoObject(XJHANDLE hComtrade);

	std::string xjcp_GetHdrInfoJSON(XJHANDLE hComtrade);
};
#endif
