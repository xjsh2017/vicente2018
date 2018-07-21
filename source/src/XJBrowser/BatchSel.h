// BatchSel.h: interface for the CBatchSel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_)
#define AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSel : public CObject
{
public:
	/** @brief           是否召唤定值区号.0-不召唤,1-召唤,2-不支持, 3-装置已挂牌*/
	int	m_nCallZone;
	/** @brief           是否召唤定值.0-不召唤,1-召唤,2-不支持, 3-装置已挂牌*/
	//##ModelId=49B87BB1029F
	int	m_nCallSetting;
	/** @brief           是否召唤开关量.0-不召唤,1-召唤,2-不支持, 3-装置已挂牌*/
	//##ModelId=49B87BB102A0
	int	m_nCallDigital;
	/** @brief           是否召唤软压板.0-不召唤,1-召唤,2-不支持, 3-装置已挂牌*/
	//##ModelId=49B87BB102AF
	int	m_nCallSoftBoard;
	/** @brief           是否召唤模拟量.0-不召唤,1-召唤,2-不支持, 3-装置已挂牌*/
	//##ModelId=49B87BB102B0
	int	m_nCallAnalog;
	int	m_nCallOsc;
	int	m_nCallHistory;

	BOOL m_bChecked;

    CObject* pMap;

	enum CallStatus{
		DONT_CALL = 0,//不召唤
		CALL = 1, //召唤
		UNSUPPORT = 2,//不支持
		SUSPEND = 3,//挂牌
	};

/*
 *  @brief   	GetCallString	 取得是否召唤描述 
 *  @param 		[In]a param of Type  int  是否召唤
 *  @return 	CString 是否召唤描述
 */
	CString GetCallString(int nCall);
};

class CBatchSel : public CSel  
{
public:
	//##ModelId=49B87BB101F6
	CBatchSel();
	//##ModelId=49B87BB10213
	CBatchSel(CSecObj* pObj, CSecCPU* pCpu, BOOL bZone, BOOL bSetting, BOOL bDigital, BOOL bSoftBoard, BOOL bAnalog);
	//##ModelId=49B87BB10232
	~CBatchSel();

/*
 *  @brief   	SaveData	 保存数据到数据库 
 *  @param 		[In]a param of Type  int 是自动总召配置还是批量召唤配置.0-自动总召. 1-批量召唤
 *  @return 	BOOL 保存数据成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BB10251
	BOOL SaveData(int nIS_OC);

	//##ModelId=49B87BB10271
	BOOL MultiSaveData(STTP_DBDATA_RECORD& sttpDB, int nIS_OC);

/*
 *  @brief   	LoadFromDB	 从数据库载入信息 
 *  @param 		[In]a param of Type  CMemSet*  数据库指针
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);


public:
	/** @brief           对应的二次设备*/
	//##ModelId=49B87BB10291
	CSecObj* m_pObj;
	/** @brief           对应的二次设备的CPU*/
	//##ModelId=49B87BB10296
	CSecCPU* m_pCpu;

	/** @brief           当前定值区号召唤结果,  0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤*/
	//##ModelId=49B87BB102BF
	int		m_nZone;
	/** @brief           定值召唤结果, 0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤*/
	//##ModelId=49B87BB102C0
	int		m_nSetting;
	/** @brief           开关量召唤结果,  0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤*/
	//##ModelId=49B87BB102C1
	int		m_nDigital;
	/** @brief           软压板召唤结果, 0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤*/
	//##ModelId=49B87BB102CE
	int		m_nSoftBoard;
	/** @brief           模拟量召唤结果, 0:此项不召唤 1：成功 2：失败 3:装置不支持 9:尚未召唤*/
	//##ModelId=49B87BB102CF
	int		m_nAnalog;

	int		m_nOsc;
	int		m_nHistory;

	/** @brief           定值区号结果说明*/
	//##ModelId=49B87BB102DE
	int	    m_nZoneNote;
	/** @brief           定值结果说明*/
	//##ModelId=49B87BB102DF
	int		m_nSettingNote;
	/** @brief           开关量结果说明*/
	//##ModelId=49B87BB102E0
	int		m_nDigitalNote;
	/** @brief           软压板结果说明*/
	//##ModelId=49B87BB102EE
	int		m_nSoftBoardNote;
	/** @brief           模拟量结果说明*/
	//##ModelId=49B87BB102EF
	int		m_nAnalogNote;
	int		m_nOscNote;
	int		m_nHistoryNote;
};

class BatchSel : public CSel
{
public:
	
	BatchSel();
};


/** @brief           选择项链表*/
//##ModelId=49B87BB102FD
typedef CPtrList  BATCH_LIST;
typedef CTypedPtrList<CObList , CBatchSel*>	BATCHSEL_LIST;
typedef CMapStringToPtr PRIDEVICE_MAP;
typedef CMapStringToPtr DEVICETYPE_MAP;
typedef CMapWordToPtr   STATION_MAP;
typedef CMapStringToPtr NET_MAP;
typedef CMapStringToPtr BATCHSEL_MAP;
typedef CMapStringToPtr STRING_MAP;
typedef CMapWordToPtr   WORD_MAP;

#endif // !defined(AFX_BATCHSEL_H__CCF0D6B2_5385_4CA5_BC7F_5E8861804409__INCLUDED_)
