#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "LinkList.h"

namespace DTLib
{

template <typename T, int N>
class StaticLinkList : public LinkList<T>
{
protected:
    using Node = typename LinkList<T>::Node;

    struct SNode : public Node // 定义静态链表自己的节点类，该类重载new操作符
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size; // 消除unused警告
            return loc;
        }
    };

    unsigned char m_space[sizeof(SNode) * N];
    int m_used[N];

    Node* create()
    {
        SNode* ret = nullptr;

        for(int i = 0; i < N; i++)
        {
            if(!m_used[i])
            {
                ret = reinterpret_cast<SNode*>(m_space) + i; // 这里仅仅分配了内存，没有构造对象
                ret = new(ret) SNode(); // Snode重载了new操作符，会将对象创建在ret这块内存，并返回这块内存
                m_used[i] = 1;
                break;
            }
        }

        return ret;
    }

    void destroy(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn);

        for(int i = 0; i < N; i++)
        {
            if(pn == space + i)
            {
                m_used[i] = 0; // 循环每块内存，标记为未使用
                psn->~SNode(); // 析构节点
                break; // 找到了对应需要删除的内存块，没必要继续循环了
            }
        }
    }
public:
    StaticLinkList()
    {
        for(int i = 0; i < N; i++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }

    ~StaticLinkList()
    {
        this->clear(); // 构造函数和析构函数中不发生多态，只会调用自身版本的虚函数，这里调用继承过来的clear()，并在clear()中调用自身的destroy()
    }
};

}
#endif // STATICLINKLIST_H
