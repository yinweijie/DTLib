#ifndef LINKLIST_H
#define LINKLIST_H

#include "List.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class LinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
    };

    mutable struct : public Object// 保证了内存结构与Node类相同，但是m_header的构造不会调用T类型的构造函数，并且要和Node一样，也继承Object
    {
        char reserved[sizeof(T)]; // 内存大小与T相同
        Node* next;
    } m_header;

    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const // O(n)
    {
        Node* ret = reinterpret_cast<Node*>(&m_header); // 虽然m_length内存布局与Node相同，但是类型不同，因此需要强制类型转换

        for(int p = 0; p < i; p++)
        {
            ret = ret->next;
        }

        return ret;
    }

    virtual Node* create()
    {
        return new Node();
    }

    virtual void destroy(Node* pn)
    {
        delete pn;
    }
public:
    LinkList()
    {
        m_header.next = nullptr;
        m_length = 0;
        m_step = 1;
        m_current = nullptr;
    }

    bool insert(const T& e) override
    {
        return insert(m_length, e);
    }

    bool insert(int i, const T& e) override // O(n)
    {
        bool ret = (0 <= i) && (i <= m_length);

        if(ret)
        {
            Node* node = create();

            if(node != nullptr)
            {
                Node* current = position(i);

                node->value = e;
                node->next = current->next;
                current->next = node;

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to insert element ...");
            }
        }

        return  ret;
    }

    bool remove(int i) override // O(n)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            Node* current = position(i);

            Node* toDel = current->next;
            current->next = toDel->next;

            if(m_current == toDel) // 如果删除的元素正好为游标m_current所在位置，就把m_current往后移一位
            {
                m_current = toDel->next;
            }

            m_length--; // 先更新长度信息，再销毁对象，保证异常安全

            destroy(toDel);
        }

        return ret;
    }

    int find(const T& e) const override // O(n)
    {
        int i = 0;
        int ret = -1;
        Node* node = m_header.next;

        while(node != nullptr)
        {
            if(node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                i++;
                node = node->next;
            }
        }

        return ret;
    }

    bool set(int i, const T& e) override // O(n)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            position(i)->next->value = e;
        }

        return ret;
    }

    virtual T get(int i) const // O(n)
    {
        T ret;

        if(get(i, ret))
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "i is out of bounds ...");
        }

        return ret;
    }

    bool get(int i, T& e) const override // O(n)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            e = position(i)->next->value;
        }

        return ret;
    }

    int length() const override // O(1)
    {
        return m_length;
    }

    void clear() override // O(n)
    {
        while (m_header.next != nullptr)
        {
            Node* toDel = m_header.next;
            m_header.next = toDel->next;

            m_length--; // 先更新长度信息，再销毁对象，保证异常安全

            destroy(toDel);
        }
    }

    virtual bool move(int i, int step = 1)
    {
        bool ret = (0 <= i) && (i < m_length) && (step > 0);

        if(ret)
        {
            m_current = position(i)->next;
            m_step = step;
        }

        return ret;
    }

    virtual bool end()
    {
        return (m_current == nullptr);
    }

    virtual T current()
    {
        if(!end())
        {
            return m_current->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "No value at current position ...");
        }
    }

    virtual bool next()
    {
        int i = 0;

        while((i < m_step) && !end())
        {
            m_current = m_current->next;
            i++;
        }

        return (i == m_step);
    }

    ~LinkList() // O(n)
    {
        clear(); // 构造函数和析构函数中不发生多态，只会调用自身版本的虚函数
    }
};

}
#endif // LINKLIST_H
