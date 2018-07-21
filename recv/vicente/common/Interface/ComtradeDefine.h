#ifndef COMTRADEDEFINE_H_
#define COMTRADEDEFINE_H_

#include <iostream>
#include <string>
#include <list>
#include <map>
using namespace std;

struct ComtradeCFG
{
	/** @brief           厂站名*/
	string sStation;
	/** @brief           装置名*/
	string sDevice;
	/** @brief           文件标准年份*/
	int		nYear;
	/** @brief           模拟量通道个数*/
	int		nAChannels;
	/** @brief           开关量通道个数*/
	int		nDChannels;
	/** @brief           线路频率*/
	float	fLineFreq;
	/** @brief           开始采样时间*/
	string	sStartTime;
	/** @brief           故障触发事件*/
	string	sTriggerTime;
	/** @brief           数据文件格式。0-ascii, 1-binary*/
	int		nDATType;
	/** @brief           时标倍乘系数*/
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
	/** @brief           通道索引号*/
	int		nChannelNo;
	/** @brief           通道名称*/
	string	sChannelName;
	/** @brief           相位标识符*/
	string	sPhase;
	/** @brief           被监视的回路元件*/
	string	sCcbm;
	/** @brief           单位*/
	string	sUnit;
	/** @brief           通道增益系数a*/
	float	fScale;
	/** @brief           通道偏移量b*/
	float	fOffset;
	/** @brief           采样时滞*/
	float	fSkew;
	/** @brief           最小值范围*/
	int		nMinValue;
	/** @brief           最大值范围*/
	int		nMaxValue;
	/** @brief           一次比例因子*/
	float	fPrimary;
	/** @brief           二次比例因子*/
	float	fSecondary;
	/** @brief           一次或二次标识符.P,p,S,s*/
	string	sPSType;
	/** @brief           通道正常状态.0/1*/
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
	//记录故障过程中的故障电流、故障电压、故障相、故障距离等信息
	list< map<string, string> > FaultInfo;
	//记录故障前装置开入自检等信号状态
	list< map<string, string> > DigitalStatus;
	//记录保护故障过程中装置开入自检等信号的变化事件
	list< map<string, string> > DigitalEvent;
	//记录故障前装置定值的值
	list< map<string, string> > SettingValue;
};
#endif