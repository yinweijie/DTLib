#ifndef CIRCLELIST_H
#define CIRCLELIST_H

#include "LinkList.h"

namespace DTLib
{

template <typename T>
class CircleList : public LinkList<T>
{
protected:
    using Node = typename LinkList<T>::Node;

    int mod(int i) const
    {
        return (this->m_length == 0) ? 0 : (i % this->m_length);
    }
    Node* last() const
    {
        return this->position(this->m_length - 1)->next;
    }

    void last_to_first() const
    {
        last()->next = this->m_header.next;
    }

public:
    bool insert(const T& e) override
    {
        return insert(this->m_length, e);
    }

    bool insert(int i, const T& e) override
    {
        bool ret = true;

        i = i % (this->m_length + 1);

        ret = LinkList<T>::insert(i, e);

        if(ret && i == 0)
        {
            last_to_first();
        }

        return ret;
    }

    bool remove(int i) override
    {
        bool ret = true;

        i = mod(i);

        if(i == 0) // 删除0号节点需要特殊处理
        {
            Node* toDel = this->m_header.next;

            if(toDel != nullptr)
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if(this->m_length != 0)
                {
                    last_to_first();

                    if(this->m_current == toDel)
                    {
                        this->m_current = toDel->next;
                    }
                }
                else // 当链表节点都被删除以后，需要把指针置空
                {
                    this->m_header.next = nullptr;
                    this->m_current = nullptr;
                }

                this->destroy(toDel);
            }
            else // 空链表不能删除节点
            {
                ret = false;
            }
        }
        else
        {
            ret = LinkList<T>::remove(i);
        }

        return ret;
    }

    bool set(int i, const T& e) override
    {
        return LinkList<T>::set(mod(i), e);
    }

    T get(int i) const override
    {
        return LinkList<T>::get(mod(i));
    }

    bool get(int i, T& e) const override
    {
        return LinkList<T>::get(mod(i), e);
    }

    int find(const T& e) const override
    {
        int ret = -1;
        Node* slider = this->m_header.next;

        for(int i = 0; i < this->m_length; i++)
        {
            if(slider->value == e)
            {
                ret = i;
                break;
            }

            slider = slider->next;
        }

        return ret;
    }

    void clear() override
    {
        while(this->m_length > 1)
        {
            LinkList<T>::remove(1);
        }

        if(this->m_length == 1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = nullptr;
            this->m_current = nullptr;
            this->m_length = 0;

            this->destroy(toDel);
        }
    }

    bool move(int i, int step) override
    {
        return LinkList<T>::move(mod(i), step);
    }

    bool end() override // 循环链表没有末尾，因此唯一让链表在current()和next()函数中能够停下来的条件就是，链表中没有节点了
    {
        return (this->m_length == 0) || (this->m_header.next == nullptr);
    }

    ~CircleList()
    {
        clear();
    }
};

}
#endif // CIRCLELIST_H
