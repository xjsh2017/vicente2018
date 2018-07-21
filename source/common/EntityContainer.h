/**
 * @defgroup 
 * @{
 */
/**
 * @brief     EntityContainer  通用链表类
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/25
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
#ifndef AFX_ENTITY_H__72A84412_56C5_4A57_A8C7_0AC65895E2B2__INCLUDED_
#define AFX_ENTITY_H__72A84412_56C5_4A57_A8C7_0AC65895E2B2__INCLUDED_
/**	\brief	The LNode struct 链表的节点
*/
#pragma warning(disable : 4786)//屏蔽4786错误
#include "define.h"
/**	\brief	The LNode struct 指针链表中的节点

*/
typedef struct LNode 
{
	void* m_pObj;           //结点中包含的数据指针
	struct LNode *next;     //下一个节点的指针
	struct LNode *forward;  //上一个节点的指针
	UINT m_nTLife;          //该节点的存在时间，初始值为0 单位为秒 add by yinzhehong 2006
}LNode, *pLinkList;
/**
 * @defgroup 
 * @{
 */
/**
 * @brief     CEntityContainer     双向指针链表                    
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/18
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
class CEntityContainer 
{
private:
	pLinkList    m_pList;      //第一个链表节点
	int          m_listLength;
	int          m_nMaxLength; //链表的最大节点数目	
public:
	CEntityContainer();
	~CEntityContainer();
public:	
	int    SetNodeData(int position,  void * pNewObj);
	void*  GetNodeData(LNode *pNode);
	int    DeleteNode(LNode* pnode);
	int    InsertNode(int beforeWhich,void * pObj);
	int    DeleteNode(int position);
	int    AddTail(void* pObj);
	int    DeleteTailNode();
	int    AddHead(void * pObj);
	int    DeleteHeadNode();
	void*  GetNodeData(int position);
	void*  GetNodeDataTest(int position);
	
	void*  GetTailNodeData();
	void*  GetHeadNodeData();
	int    ClearList();
	bool   IsEmpty  ();
	int    GetLength();
	LNode* GetNode  (int position);
	LNode* GetForwardNode(LNode *pNode);
	LNode* GetNextNode(LNode* pNode);
	LNode* GetTailNode();
    LNode* FindNode(int position);
	LNode* GetNodeTest (int position);
};

#endif