#ifndef _CDATALIST_H_
#define _CDATALIST_H_

#include <list>
#include <errno.h>
#include "xjlib.h"

/*************************************************************
 ��		��: CDataList
 ��		��: ��
 ���ܸ�Ҫ : ͨ���������������жԻ������ķ�װ��������ɶ�����
		  : �Ĳ��롢ɾ���Ȳ�����
 ʹ�÷��� : 
 ע		��:
			��Ҫʹ�ÿ⣺xjlib��pthread��dl
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
	 �� �� �� : CDataList()
	 ���ܸ�Ҫ : ���캯������ʼ��������
	 �� �� ֵ : ��
	 ��    �� : 
				��
	*************************************************************/
	CDataList()
	{
		xj_mutex_init(&m_mutex);
	};

	/*************************************************************
	 �� �� �� : CDataList()
	 ���ܸ�Ҫ : �������������ٻ������������������
	 �� �� ֵ : ��
	 ��    �� : 
				��
	*************************************************************/
	~CDataList()
	{
		m_list.clear();
		xj_mutex_destroy(&m_mutex);
	};

	/*************************************************************
	 �� �� �� : empty()
	 ���ܸ�Ҫ : �ж������Ƿ�Ϊ��
	 �� �� ֵ : trueΪ�գ�falseΪ�ǿ�
	 ��    �� : 
				��
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
	 �� �� �� : size()
	 ���ܸ�Ҫ : �ж�����Ľڵ���
	 �� �� ֵ : >=0Ϊʵ�ʽڵ���������Ϊ������
	 ��    �� : 
				��
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
	 �� �� �� : insert()
	 ���ܸ�Ҫ : ����һ���ڵ�
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				ite		����λ�õĵ�������
				element ������Ľڵ�
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
	 �� �� �� : push_front()
	 ���ܸ�Ҫ : ��һ���ڵ����ͷ��
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				element ������Ľڵ�
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
	 �� �� �� : push_back()
	 ���ܸ�Ҫ : ��һ���ڵ����β��
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				element ������Ľڵ�
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
	 �� �� �� : pop_front()
	 ���ܸ�Ҫ : ɾ��ͷ�ڵ�
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				��
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
	 �� �� �� : pop_back()
	 ���ܸ�Ҫ : ɾ��β�ڵ�
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				��
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
	 �� �� �� : begin()
	 ���ܸ�Ҫ : ȡͷ�ڵ�ĵ�����
	 �� �� ֵ : ��NULLΪ�ɹ���NULLΪʧ��
	 ��    �� : 
				��
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
	 �� �� �� : end()
	 ���ܸ�Ҫ : ȡβ�ڵ�ĵ�����
	 �� �� ֵ : ��NULLΪ�ɹ���NULLΪʧ��
	 ��    �� : 
				��
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
	 �� �� �� : front()
	 ���ܸ�Ҫ : ȡͷ�ڵ�����
	 �� �� ֵ : trueΪ�ɹ���falseΪʧ��
	 ��    �� : 
				element	�����洢�ڵ����ݣ�
				flag	Ϊtrue��ʾͬʱɾ���ýڵ㣬false��ɾ��
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
	 �� �� �� : back()
	 ���ܸ�Ҫ : ȡβ�ڵ�����
	 �� �� ֵ : trueΪ�ɹ���falseΪʧ��
	 ��    �� : 
				element	�����洢�ڵ����ݣ�
				flag	Ϊtrue��ʾͬʱɾ���ýڵ㣬false��ɾ��
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
	 �� �� �� : clear()
	 ���ܸ�Ҫ : �������
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				��
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
	 �� �� �� : erase()
	 ���ܸ�Ҫ : ���ָ��λ�õĽڵ�
	 �� �� ֵ : 0Ϊ�ɹ�������Ϊ������
	 ��    �� : 
				ite	������ڵ�ĵ�����
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
	* @brief			ȡָ���ڵ�λ�õ����ݡ�
	* @param  intex		�ڵ���������0��ʼ������
	* @param  element	�����洢�ڵ����ݡ�
	* @param  type		Ϊtrue��ʾͬʱɾ���ýڵ㣬false��ɾ����
	* @return			trueΪ�ɹ���falseΪʧ�ܡ�
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
