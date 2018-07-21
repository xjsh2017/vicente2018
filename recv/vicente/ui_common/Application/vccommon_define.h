#ifndef vccommon_define_
#define vccommon_define_

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <AFXTEMPL.H>

#include "..\..\common\define.h"
#include "..\..\common\MessageLog.h"
#include "..\..\common\SttpDefine.h"
#include "..\..\common\MemSet.h"
#include "..\..\common\MemField.h"
#include "..\..\common\XJErrorID.h"

//��־�ȼ�����
int const XJ_LOG_LV1		= 1;	//1����־, ��Ҫ, �����
int const XJ_LOG_LV2		= 2;	//2����־, һ��
int const XJ_LOG_LV3		= 3;	//3����־, ����Ҫ, ������

//�¼����Ͷ���
int const XJ_EVENT_ACTION	= 0;	//��������
int const XJ_EVENT_ALARM	= 1;	//�����澯
int const XJ_EVENT_FAULT	= 2;	//�¹ʱ���
int const XJ_EVENT_OSC		= 3;	//¼��
int const XJ_EVENT_DI		= 4;	//��������λ��Ϣ
int const XJ_EVENT_PTCOMM	= 5;	//����ͨѶ״̬
int const XJ_EVENT_PTRUN	= 6;	//��������״̬
int const XJ_EVENT_SYSALARM = 7;	//ϵͳ�澯
int const XJ_EVENT_COMM		= 8;	//����վͨѶ״̬

//֪ͨ����
int const XJ_NOTIFY_EVENT	= 9;	//�¼�֪ͨ
int const XJ_NOTIFY_FAULT	= 10;	//���ϱ���֪ͨ
int const XJ_NOTIFY_DECISION = 11;	//��������֪ͨ

//�¼���Ϣ���Ͷ���
int const XJ_EVENTINFO_ALL		= 0;	//����
int const XJ_EVENTINFO_NORMAL	= 1;	//������Ϣ
int const XJ_EVENTINFO_DEBUG	= 2;	//������Ϣ

//���ϱ�����¼����Ͷ���
int const XJ_FAULT_EVENT_CHR		= 1; //�����¼�������
int const XJ_FAULT_EVENT_SING		= 2; //�����¼��ź���
int const XJ_FAULT_EVENT_ALARM		= 3; //�澯�¼�
int const XJ_FAULT_EVENT_OSCREPORT	= 4; //¼����
int const XJ_FAULT_EVENT_OSCLIST	= 5; //¼���б�

//�Զ���ṹ
//��������ֵ tb_pt_action_character
typedef struct PT_ActionCharacter
{
	int		nID;			//����ֵID
	CString strName;		//����ֵ��
	CString strCodeName;	//����ֵ������
	CString strValue;		//����ֵ
	CString strUnits;		//��λ
	int     nEventType;     //�ź�������0 һ���¼� 1 �����¼� 2 �����¼�

	CString	strPT_ID;		//����ID
	int		nCPU;			//����CPU
	const PT_ActionCharacter& operator = (const PT_ActionCharacter& character)
	{
		nID         = character.nID;
		strName     = character.strName;
		strValue    = character.strValue;
		strCodeName = character.strCodeName;
		strUnits	= character.strUnits;
		strPT_ID	= character.strPT_ID;
		nCPU		= character.nCPU;
		nEventType	= character.nEventType;
		return *this;
	}

	PT_ActionCharacter()
	{
		nID         = -1;
		strName     = "";
		strValue    = "";
		strCodeName = "";
		strUnits	= "";
		strPT_ID	= "";
		nCPU		= -1;
		nEventType	= 2;
	}
}PT_ActionCharacter;

//�ź���
//##ModelId=49B87B890263
typedef struct PT_Signal
{
	int	    nID;				//�ź����
	CString strName;		//�ź�����
	CString strCodeName;	//�źŴ�����
	int     nEventType;         //�ź�������0 һ���¼� 1 �����¼� 2 �����¼�  3,ͨѶ�澯�¼�
	CString strTime;			//�źŷ���ʱ��
	int	    nValue;				//�ź�ֵ��1-ON,0-OFF

	CString strPT_ID;		//����ID
	CString strPTName;		//��������
	CString strPT_Model;	//�����ͺ�
	int		nCPU;			//����CPU��

	CString strStationId;   //�������ڳ�վID
	CString strStation;		//�������ڵĳ�վ����

	const PT_Signal& operator =(const PT_Signal& sign)
	{
		nID          = sign.nID;     
		strName      = sign.strName;
		strCodeName  = sign.strCodeName;
		nEventType   = sign.nEventType;
		strTime      = sign.strTime;
		nValue       = sign.nValue;
		strPT_ID     = sign.strPT_ID;
		strPTName    = sign.strPTName;
		strPT_Model  = sign.strPT_Model;
		nCPU		 = sign.nCPU;
		strStationId = sign.strStationId;
		strStation   = sign.strStation;
		return *this;
	}
}PT_Signal;

//����CPU TB_SECDEV_CPU_BASE
//##ModelId=49B87B890272
typedef struct PT_CPU
{
	int   code;
	CString   des;
	CString   ver;
	int       from_zone;
	int		  to_zone;
	int       fun;

	const PT_CPU& operator =(const PT_CPU& sign)
	{
		code      = sign.code;     
		des       = sign.des;
		//	ver		  = sign.ver;
		from_zone    = sign.from_zone;
		to_zone      = sign.to_zone;
		//	fun			 = sign.fun;
		return *this;
	}
}PT_CPU;

//��ֵ��
typedef struct PT_ZONE
{

	int		state; //��ʶ�Ƿ�ǰ��ֵ����,0-��,1-��

	CString  PT_ID;//����ID
	int		cpu; //cpu��
	int		code; //��ֵ����
	CString zone_name;
	CString code_name;
	int     group;
	int     item;
	int     minValue;
	int     maxValue;
	int     stepValue;
	int     zoneValue;


	const PT_ZONE& operator =(const PT_ZONE& sign)
	{
		code = sign.code;
		state = sign.state;
		cpu = sign.cpu;
		PT_ID = sign.PT_ID;
		zone_name = sign.zone_name;
		code_name = sign.code_name;
		group = sign.group;
		item = sign.item;
		minValue = sign.minValue;
		maxValue = sign.maxValue;
		stepValue = sign.stepValue;
		zoneValue = sign.zoneValue;
		return *this;
	}

}PT_ZONE;

//��������������
typedef struct PT_DIGITAL
{
	CString	ID;			//������ID
	CString CodeName;	//����������
	CString	Name;		//����������
	int     CpuCode;
	CString	Value;		//ʵ��ֵ

	const PT_DIGITAL& operator =(const PT_DIGITAL& digital)
	{
		ID			= digital.ID;
		CodeName	= digital.CodeName;
		Name		= digital.Name;
		CpuCode     = digital.CpuCode;
		Value		= digital.Value;
		return *this;
	}
}PT_DIGITAL;

//������ѹ�嶨��
typedef struct PT_SOFTBOARD{
	CString ID;			//��ѹ��ID
	CString CodeName;	//��ѹ�����
	CString Name;		//��ѹ������
	CString Value;		//ʵ��ֵ

	const PT_SOFTBOARD& operator =(const PT_SOFTBOARD& softboard)
	{
		ID			= softboard.ID;
		CodeName	= softboard.CodeName;
		Name		= softboard.Name;
		Value		= softboard.Value;
		return *this;
	}
}PT_SOFTBOARD;

//����ģ��������
typedef struct PT_ANALOG{
	CString ID;			//ģ����ID
	CString CodeName;	//ģ��������
	CString Name;		//ģ��������
	CString Value;		//ʵ��ֵ
	CString Unit;		//ֵ��λ
	CString Precision;	//ֵ����

	const PT_ANALOG& operator =(const PT_ANALOG& analog)
	{
		ID			= analog.ID;
		CodeName	= analog.CodeName;
		Name		= analog.Name;
		Value		= analog.Value;
		Unit		= analog.Unit;
		Precision	= analog.Precision;
		return *this;
	}
}PT_ANALOG;

//¼���ļ�����
typedef struct PT_OSC{
	CString FileName;	//�ļ�����,����׺��
	CString	FileType;	//�ļ�����,����׺��
	float	FileSize;	//�ļ���С,��λbytes
	CTime	ModifyTime;	//����޸�ʱ��
	CString	DirPath;	//Ŀ¼·�� 

	const PT_OSC& operator =(const PT_OSC& osc)
	{
		FileName	= osc.FileName;
		FileType	= osc.FileType;
		FileSize	= osc.FileSize;
		ModifyTime	= osc.ModifyTime;
		DirPath		= osc.DirPath;
		return *this;
	}

}PT_OSC;

//��ֵ�޸Ķ���
//##ModelId=49B87B8902BF
typedef CTypedPtrArray<CPtrArray, STTP_DATA*> MODIFY_LIST; //�޸�ֵ����

//��ֵ����
//##ModelId=49B87B8902C1
typedef struct PT_SETTING{
	CString ID;			//��ֵID
	CString CodeName;	//��ֵ����
	CString Name;		//��ֵ����
	int		Group;		//���
	CString Value;		//ʵ��ֵ
	CString VPickList;	//ֵ˵��
	CString Unit;		//ֵ��λ
	CString	Precision;	//ֵ���� ��ʽ"2,2"
	int		DataType;   //ֵ���� 0-���� 1������ 2 ������3-�ַ���
	CString OrderValue; //��׼ֵ
	CString minValue;
	CString maxValue;
	CString stepValue;
	int     sValue;
	int		Item;

	const PT_SETTING& operator =(const PT_SETTING& setting)
	{
		ID			= setting.ID;
		CodeName	= setting.CodeName;
		Name		= setting.Name;
		Group		= setting.Group;
		Value		= setting.Value;
		VPickList	= setting.VPickList;
		Unit		= setting.Unit;
		Precision	= setting.Precision;
		DataType	= setting.DataType;
		OrderValue	= setting.OrderValue;
		minValue    = setting.minValue;
		maxValue    = setting.maxValue;
		stepValue   = setting.stepValue;
		sValue      = setting.sValue;
		Item		= setting.Item;
		return *this;
	}
}PT_SETTING;

//103������
//##ModelId=49B87B8902CF
typedef struct PT_GROUP{
	int		Group;	//���
	CString	Name;	//����
	const PT_GROUP& operator =(const PT_GROUP& gp)
	{
		Group = gp.Group;
		Name = gp.Name;
		return *this;
	}
}PT_GROUP;

//ϵͳ�澯��ϸ��Ϣ
typedef struct SYSALARM{
	CString		sName; //����
	CString		sOldValue;	//��ֵ
	CString		sNewValue;	//��ֵ
	const SYSALARM& operator =(const SYSALARM& sa)
	{
		sNewValue = sa.sName;
		sOldValue = sa.sOldValue;
		sNewValue = sa.sNewValue;
		return *this;
	}
}SYSALARM;

#define MYASSERT(p) \
if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return;\
}

#define MYASSERT_NULL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return NULL;\
}

#define MYASSERT_CONTINUE(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	continue;\
}

#define MYASSERT_BOOL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return FALSE;\
}

#define MYASSERT_STRING(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return "";\
}

#define MYASSERT_INT(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return -1;\
}

#define  MYDELETE(p) \
	if (p != NULL)\
{\
	delete p;\
	 p = NULL;\
}

void WriteLog(CString sLog, int nLogLevel = 3);

void WriteLogTemp(CString sLog, int nLogLevel = 3);

/*************************************************************
 �� �� ����StringToTimeSttp15()
 ���ܸ�Ҫ����15λ��STTPʱ���ַ���ת��ΪCTime
 �� �� ֵ: ת���ɵ�CTime����
 ��    ����param1 ʱ���ַ���
		   Parsm2 ���غ���
**************************************************************/
CTime StringToTimeSttp15(CString & sTime, int& nms);

/*************************************************************
 �� �� ����StringToTimeSttp12()
 ���ܸ�Ҫ����12λ��STTPʱ���ַ���ת��ΪCTime,12λ��ʱ�䲻������ֵ
 �� �� ֵ: ת���ɵ�CTime����
 ��    ����param1	ʱ���ַ���
**************************************************************/
CTime StringToTimeSttp12(CString & sTime);

/*************************************************************
 �� �� ����StringToTimeSttp26()
 ���ܸ�Ҫ����26λ��STTPʱ���ַ���ת��ΪCTime,����ֵռ6λ
 �� �� ֵ: CTime����ʱ��
 ��    ����param1	ʱ���ַ���
		   Param2	����ֵ
**************************************************************/
CTime StringToTimeSttp26(CString & sTime, int& nms);

/*************************************************************
 �� �� ����StringToTime14()
 ���ܸ�Ҫ����14����ʱ���ַ���ת����ʱ�����
 �� �� ֵ: CTime����ʱ��
 ��    ����param1 ʱ���ַ���
		   Param2
**************************************************************/
CTime StringToTime14(CString sTime);

/*************************************************************
 �� �� ����XJToMS()
 ���ܸ�Ҫ��CXJString�����ַ���ת��ΪCString�ַ���
 �� �� ֵ: ת����CString����
 ��    ����param1	Ҫת����CXJString����
**************************************************************/
CString XJToMS(string xjStr);

string WChar2Ansi(LPCWSTR pwszSrc);

/*************************************************************
 �� �� ����MSToXJ()
 ���ܸ�Ҫ��CString�����ַ���ת��ΪCXJString�ַ���
 �� �� ֵ: ת����CXJString����
 ��    ����param1	Ҫת����CString����
**************************************************************/
string MSToXJ(CString str);

CTime StringToTimeWithMillisecond(CString sTime);

CTime StringToTimeWithoutMillisecond(CString sTime);

/*************************************************************
 �� �� ����StringToTime()
 ���ܸ�Ҫ�����ݿ��е�ʱ���ַ���ת��Ϊʱ��.�ַ�����ʽΪYYYY-MM-DD HH:MM:SS.MMMM
 �� �� ֵ: ת���ɵ�ʱ��
 ��    ����param1	ʱ���ַ���
**************************************************************/
CTime StringToTime(CString& sTime);


/*************************************************************
 �� �� ����ComplexExchange()
 ���ܸ�Ҫ������ת��, �жϴ�����ַ����Ƿ��ʾ����,�����,ת���ɸ�����ʽ��ʾ
 �� �� ֵ: void
 ��    ����param1	��ת�����ַ���	
		   Param2
**************************************************************/
CString	ComplexExchange(CString sStr);

/*************************************************************
 �� �� ����CompareComplex()
 ���ܸ�Ҫ���Ƚ����������Ĵ�С
 �� �� ֵ: ���1��2��, ����1. 1=2,����0. 1<2,����-1
 ��    ����param1	����1
		   Param2	����2
**************************************************************/
int	CompareComplex(CString str1, CString str2);


inline BOOL XJASSERT(void* p, CString strFileName, int iLine)
{
	if (p == NULL)
	{
		CString strLog;
		strLog.Format(strFileName + _T(" ") + ":%d" + _T(" ") + _T("ָ��Ϊ��"), iLine);
		WriteLog(strLog, 3);
		return TRUE;
	}
	return FALSE;
}


inline LONGLONG StartCount()
{
//#ifdef _DEBUG	
	LARGE_INTEGER lv;

	// ��ȡCPU���е����ڵ�Tick��
	QueryPerformanceCounter( &lv );
	
	// ����CPU���е����ڵ�ʱ��
	// ��GetTickCount��timeGetTime���Ӿ�ȷ
	return lv.QuadPart;
//#endif
}

inline void EndCount(CString strLog, LONGLONG startTime)
{
//#ifdef _DEBUG	
	LARGE_INTEGER lv;
    
	QueryPerformanceFrequency( &lv );
    // 1���ж���tick
    LONGLONG Tick = lv.QuadPart;
	// ��ȡCPU���е����ڵ�Tick��
	QueryPerformanceCounter( &lv );
	
	// ����CPU���е����ڵ�ʱ��
	// ��GetTickCount��timeGetTime���Ӿ�ȷ
	LONGLONG endTime = lv.QuadPart - startTime;
	double interval = (double)endTime/(double)Tick;
	strLog.Format(strLog + ":%f", interval);
	WriteLog(strLog, 3);
//#endif
}
#endif
