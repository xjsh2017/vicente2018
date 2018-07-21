// SttpMsgParser.h: interface for the CSttpMsgParser class.
//
//////////////////////////////////////////////////////////////////////
#pragma  warning	(disable : 4275) 
#if !defined(AFX_STTPMSGPARSER_H__D0F3545B_1380_4756_AE3B_3B009138B990__INCLUDED_)
#define AFX_STTPMSGPARSER_H__D0F3545B_1380_4756_AE3B_3B009138B990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SttpDefine.h"       //sttp通用的结构定义
//
/**
 * @defgroup 
 * @{
 */
/**
 * @brief    CSttpMsgParser  sttp报文的管理操作以及数据操作的封装        
 * @author    yinzhehong
 * @version   1.0
 * @date      2007/12/24
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
class CSttpMsgParser
{
private:
	STTPMSG    * m_pMsg ;
	STTPMSGHDR * m_pMsgHdr ;

public:
	CSttpMsgParser() ;
	CSttpMsgParser(STTPMSG* pMsg);
	virtual ~CSttpMsgParser();
public:
	char* GetMsgHdr() ;
	void SetMsgHdr(char* strHdr);
	char* GetMsgBody() ;
	void SetMsgBody(char* strBody);
	void Attach(STTPMSG* pMsg);
public:	
	UINT GetMsgRII();
	void SetMsgRII(UINT nRiiNum);
	UINT GetMsgCurNum();
	void SetMsgCurNum(UINT nNum);
	UINT GetMsgNums();
	void SetMsgNums(UINT nNums);
	void SetMsgEndFlag(bool bEndFlag);
	bool GetMsgEndFlag();
	UINT GetMsgID() ;	
	void GetMsgID(char* strID) ;
	void SetMsgID(char* strID) ;
	void SetMsgID(UINT msgId) ;
	char GetMsgType() ;
	void SetMsgType(char cType) ;
	UINT GetMsgLength() ;
	void SetMsgLength(UINT nLeng) ;
	char GetMsgLengthType() ;
	void SetMsgLengthType(char cType) ;
public:
	char*  Strrev(char* szT);
	char*  Itoa(int value, char* str, int radix);
	void   DoubleToArray(double fDouble,char* szByte,int iLength);
	void   IntToArray(UINT l,char* ch,int length);
	int    ArrayToInt(char* byte, int nCount);
	UINT   ArrayToHex(char* byte, int nCount);
	double ArrayToDouble(char* szByte,int iLength);

};

#endif // !defined(AFX_STTPMSGPARSER_H__D0F3545B_1380_4756_AE3B_3B009138B990__INCLUDED_)
