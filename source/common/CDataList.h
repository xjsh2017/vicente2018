#ifndef _CDATALIST_H_
#define _CDATALIST_H_

#include <list>
#include <errno.h>
#include "xjlib.h"

/*************************************************************
 类		名: CDataList
 父		类: 无
 功能概要 : 通用数据链表，包含有对互斥锁的封装，可以完成对链表
		  : 的插入、删除等操作。
 使用范例 : 
 注		意:
			需要使用库：xjlib、pthread、dl
*************************************************************/

using namespace std;

template <class XJ_LIST_TYPE>

class CDataList
{
public:
		MUTEX				m_mutex;
		list<XJ_LIST_TYPE>	m_list;

public:
	typedef typename list<XJ_LIST_TYPE>::iterator iterator;

protected:

public:

	/*************************************************************
	 函 数 名 : CDataList()
	 功能概要 : 构造函数，初始化互斥锁
	 返 回 值 : 无
	 参    数 : 
				无
	*************************************************************/
	CDataList()
	{
		xj_mutex_init(&m_mutex);
	};

	/*************************************************************
	 函 数 名 : CDataList()
	 功能概要 : 析构函数，销毁互斥锁，清除链表内容
	 返 回 值 : 无
	 参    数 : 
				无
	*************************************************************/
	~CDataList()
	{
		m_list.clear();
		xj_mutex_destroy(&m_mutex);
	};

	/*************************************************************
	 函 数 名 : empty()
	 功能概要 : 判断链表是否为空
	 返 回 值 : true为空；false为非空
	 参    数 : 
				无
	*************************************************************/
	bool  empty()
	{
		bool empty=true;

		xj_mutex_lock(&m_mutex);
		empty = m_list.empty();
		xj_mutex_unlock(&m_mutex);
		
		return empty;
	};

	/*************************************************************
	 函 数 名 : size()
	 功能概要 : 判断链表的节点数
	 返 回 值 : >=0为实际节点数；其它为错误码
	 参    数 : 
				无
	*************************************************************/
	int	size()
	{
		try
		{
			int size=0;

			xj_mutex_lock(&m_mutex);
			size = m_list.size();
			xj_mutex_unlock(&m_mutex);

			return size;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : insert()
	 功能概要 : 插入一个节点
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				ite		插入位置的迭代器，
				element 待插入的节点
	*************************************************************/
	int insert(typename list<XJ_LIST_TYPE>::iterator ite, const XJ_LIST_TYPE &element)
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.insert(ite, element);
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : push_front()
	 功能概要 : 把一个节点插入头部
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				element 待插入的节点
	*************************************************************/
	int push_front(const XJ_LIST_TYPE &element)
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.push_front(element);
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : push_back()
	 功能概要 : 把一个节点插入尾部
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				element 待插入的节点
	*************************************************************/
	int push_back(const XJ_LIST_TYPE &element)
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.push_back(element);
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : pop_front()
	 功能概要 : 删除头节点
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				无
	*************************************************************/
	int pop_front()
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.pop_front();
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : pop_back()
	 功能概要 : 删除尾节点
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				无
	*************************************************************/
	int pop_back()
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.pop_back();
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : begin()
	 功能概要 : 取头节点的迭代器
	 返 回 值 : 非NULL为成功；NULL为失败
	 参    数 : 
				无
	*************************************************************/
	typename list<XJ_LIST_TYPE>::iterator begin()
	{
		try
		{
			typename list<XJ_LIST_TYPE>::iterator ite;

			xj_mutex_lock(&m_mutex);
			ite = m_list.begin();
			xj_mutex_unlock(&m_mutex);

			return ite;
		}
		catch (...) 
		{
			return (typename list<XJ_LIST_TYPE>::iterator)0;
		}
	};

	/*************************************************************
	 函 数 名 : end()
	 功能概要 : 取尾节点的迭代器
	 返 回 值 : 非NULL为成功；NULL为失败
	 参    数 : 
				无
	*************************************************************/
	typename list<XJ_LIST_TYPE>::iterator end()
	{
		try
		{
			typename list<XJ_LIST_TYPE>::iterator ite;

			xj_mutex_lock(&m_mutex);
			ite = m_list.end();
			xj_mutex_unlock(&m_mutex);

			return ite;
		}
		catch (...) 
		{
			return (typename list<XJ_LIST_TYPE>::iterator)0;
		}
	};

	/*************************************************************
	 函 数 名 : front()
	 功能概要 : 取头节点数据
	 返 回 值 : true为成功；false为失败
	 参    数 : 
				element	用来存储节点数据，
				flag	为true表示同时删除该节点，false不删除
	*************************************************************/
	bool front(XJ_LIST_TYPE &element, bool flag=true)
	{
		try
		{
			bool	ret=false;

			xj_mutex_lock(&m_mutex);

			if (m_list.size() > 0)
			{
				element = m_list.front();

				if (flag)
				{
					m_list.pop_front();
				}

				ret = true;
			}
			
			xj_mutex_unlock(&m_mutex);

			return ret;
		}
		catch (...) 
		{
			return false;
		}
	};

	/*************************************************************
	 函 数 名 : back()
	 功能概要 : 取尾节点数据
	 返 回 值 : true为成功；false为失败
	 参    数 : 
				element	用来存储节点数据，
				flag	为true表示同时删除该节点，false不删除
	*************************************************************/
	bool back(XJ_LIST_TYPE &element, bool flag=true)
	{
		try
		{
			bool	ret=false;

			xj_mutex_lock(&m_mutex);

			if (m_list.size() > 0)
			{
				element = m_list.back();

				if (flag)
				{
					m_list.pop_back();
				}

				ret = true;
			}
			
			xj_mutex_unlock(&m_mutex);

			return ret;
		}
		catch (...) 
		{
			return false;
		}
	};

	/*************************************************************
	 函 数 名 : clear()
	 功能概要 : 清空链表
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				无
	*************************************************************/
	int clear()
	{
		try
		{
			xj_mutex_lock(&m_mutex);
			m_list.clear();
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/*************************************************************
	 函 数 名 : erase()
	 功能概要 : 清除指定位置的节点
	 返 回 值 : 0为成功；其它为错误码
	 参    数 : 
				ite	待清除节点的迭代器
	*************************************************************/
	int erase(typename list<XJ_LIST_TYPE>::iterator ite)
	{
		try
		{
/*
			if (ite == NULL)
			{
				return -1;
			}
			*/
			xj_mutex_lock(&m_mutex);
			m_list.erase(ite);
			xj_mutex_unlock(&m_mutex);

			return 0;
		}
		catch (...) 
		{
			return errno;
		}
	};

	/**
	* @name		at
	* @brief			取指定节点位置的数据。
	* @param  intex		节点索引，从0开始计数。
	* @param  element	用来存储节点数据。
	* @param  type		为true表示同时删除该节点，false不删除。
	* @return			true为成功；false为失败。
	* @note 
	*/
	bool at(unsigned int intex, XJ_LIST_TYPE &element, bool type)
	{
		try
		{
			int		i=0;
			bool	ret=false;
			typename list<XJ_LIST_TYPE>::iterator ite;

			xj_mutex_lock(&m_mutex);

			if (intex >= m_list.size())
			{
				ret=false;
			}
			else
			{
				for (ite = m_list.begin(); ite!=m_list.end(); ite++)
				{
					if (i == intex)
					{
						break ;
					}

					i++;
				}

				element = (*ite);
				if (type == true)
				{
					m_list.erase(ite);
				}

				ret=true;
			}
	
			xj_mutex_unlock(&m_mutex);

			return ret;
		}
		catch(...)
		{
			return false;
		}
	};
};

#endif/*_CDATALIST_H_*/
