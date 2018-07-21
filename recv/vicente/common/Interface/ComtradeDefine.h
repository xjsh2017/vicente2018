#ifndef COMTRADEDEFINE_H_
#define COMTRADEDEFINE_H_

#include <iostream>
#include <string>
#include <list>
#include <map>
using namespace std;

struct ComtradeCFG
{
	/** @brief           ��վ��*/
	string sStation;
	/** @brief           װ����*/
	string sDevice;
	/** @brief           �ļ���׼���*/
	int		nYear;
	/** @brief           ģ����ͨ������*/
	int		nAChannels;
	/** @brief           ������ͨ������*/
	int		nDChannels;
	/** @brief           ��·Ƶ��*/
	float	fLineFreq;
	/** @brief           ��ʼ����ʱ��*/
	string	sStartTime;
	/** @brief           ���ϴ����¼�*/
	string	sTriggerTime;
	/** @brief           �����ļ���ʽ��0-ascii, 1-binary*/
	int		nDATType;
	/** @brief           ʱ�걶��ϵ��*/
	float	fTimeMult;
	ComtradeCFG()
	{
		sStation = "";
		sDevice = "";
		nYear = 0;
		nAChannels = 0;
		nDChannels = 0;
		fLineFreq = 0.0;
		sStartTime = "";
		sTriggerTime = "";
		nDATType = 0;
		fTimeMult = 0.0;
	}

// 	const ComtradeCFG& operate= (ComtradeCFG cc)
// 	{
// 		sStation = cc.sStation;
// 		sDevide = cc.sDevide;
// 		nYear = cc.nYear;
// 		nAChannels = cc.nAChannels;
// 		nDChannels = cc.nDChannels;
// 		fLineFreq = cc.fLineFreq;
// 		sStartTime = cc.sStartTime;
// 		sTriggerTime = cc.sTriggerTime;
// 		nDATType = cc.nDATType;
// 		fTimeMult = cc.fTimeMult;
// 	}
};

struct ComtradeChannel 
{
	/** @brief           ͨ��������*/
	int		nChannelNo;
	/** @brief           ͨ������*/
	string	sChannelName;
	/** @brief           ��λ��ʶ��*/
	string	sPhase;
	/** @brief           �����ӵĻ�·Ԫ��*/
	string	sCcbm;
	/** @brief           ��λ*/
	string	sUnit;
	/** @brief           ͨ������ϵ��a*/
	float	fScale;
	/** @brief           ͨ��ƫ����b*/
	float	fOffset;
	/** @brief           ����ʱ��*/
	float	fSkew;
	/** @brief           ��Сֵ��Χ*/
	int		nMinValue;
	/** @brief           ���ֵ��Χ*/
	int		nMaxValue;
	/** @brief           һ�α�������*/
	float	fPrimary;
	/** @brief           ���α�������*/
	float	fSecondary;
	/** @brief           һ�λ���α�ʶ��.P,p,S,s*/
	string	sPSType;
	/** @brief           ͨ������״̬.0/1*/
	int		nStatus;
	ComtradeChannel()
	{
		nChannelNo = 0;
		sChannelName = "";
		sPhase = "";
		sCcbm = "";
		sUnit = "";
		fScale = 0.0;
		fOffset = 0.0;
		fSkew = 0.0;
		nMinValue = 0;
		nMaxValue = 0;
		fPrimary = 0.0;
		fSecondary = 0.0;
		sPSType = "";
		nStatus = -1;
	}
};

class TripInfoObj
{
public:
	TripInfoObj(){time="";name="";phase="";value="";};
	~TripInfoObj(){};

	string time;
	string name;
	string phase;
	string value;
	list< map<string, string> > FaultInfo;
};

class HdrObj
{
public:
	HdrObj(){FaultStartTime="";DataFileSize="";FaultKeepingTime="";result=0;OriginalContent="";};
	~HdrObj(){};

	int result;
	string OriginalContent;
	string FaultStartTime;
	string DataFileSize;
	string FaultKeepingTime;
	list<TripInfoObj> TripInfo;
	//��¼���Ϲ����еĹ��ϵ��������ϵ�ѹ�������ࡢ���Ͼ������Ϣ
	list< map<string, string> > FaultInfo;
	//��¼����ǰװ�ÿ����Լ���ź�״̬
	list< map<string, string> > DigitalStatus;
	//��¼�������Ϲ�����װ�ÿ����Լ���źŵı仯�¼�
	list< map<string, string> > DigitalEvent;
	//��¼����ǰװ�ö�ֵ��ֵ
	list< map<string, string> > SettingValue;
};
#endif