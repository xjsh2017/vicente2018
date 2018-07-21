// HisValueMgr.h: interface for the CHisValueMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HISVALUEMGR_H__ECF02419_54CB_4C9D_8BB4_A1E9FC783143__INCLUDED_)
#define AFX_HISVALUEMGR_H__ECF02419_54CB_4C9D_8BB4_A1E9FC783143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct HISVALUENODE_TAG{
	int		nID;
	CString	sValue;
}HISVALUENODE;

class CHisValueNode
{
public:
	CHisValueNode(){m_nID = -1; m_sValue = ""; };
	CHisValueNode(int nID, CString sValue){ m_nID = nID; m_sValue = sValue; };
	~CHisValueNode(){};
private:
	int		m_nID;
	CString	m_sValue;

public:
	int GetID() const { return m_nID; };	
	void SetID(int ID) { m_nID = ID; };
	CString GetValue() const { return m_sValue; };	
	void SetValue(CString Value) { m_sValue = Value; };
};

class CHisValue
{
public:
	CHisValue(){m_sTime = "";};
	CHisValue(CString sTime) {m_sTime = sTime; };
	~CHisValue(){};
private:
	/** @brief			此批历史值的时间*/
	CString m_sTime;
	CMap<int, int, CHisValueNode, CHisValueNode> m_Nodes;
public:
	void SetTime(CString sTime){ m_sTime = sTime; };
	CString GetTime() const { return m_sTime; };
	void AddValue(int nID, CString sValue){ m_Nodes.SetAt(nID, CHisValueNode(nID, sValue)); };
	CString GetValue(int nID) const { CHisValueNode node; m_Nodes.Lookup(nID, node); return node.GetValue(); };
};

class CHisValueMgr  
{
public:
	CHisValueMgr();
	~CHisValueMgr();
private:
	CPtrArray	m_Values;
private:
	void Clear();
public:
	/*
	* @brief	GetCount 取得值的批次	
	* @return   int	一共有几批值
	*/
	int	GetCount() const { return m_Values.GetSize(); };
	void ResetContent(){ Clear(); };
	CString GetTime(int nValueNo);
	void SetTime(int nValueNo, CString sTime);
	CString	GetValue(int nValueNo, int nID);
	/*
	* @brief	CreateValue	创建一个新的值批次
	* @param	[In]a param of Type CString sTime 新创建的批次对应的时间		
	* @return   int	返回批次号
	*/
	int	CreateValue(CString sTime);
	void AddValue(int nValueNo, int nID, CString sValue);
};

#endif // !defined(AFX_HISVALUEMGR_H__ECF02419_54CB_4C9D_8BB4_A1E9FC783143__INCLUDED_)
