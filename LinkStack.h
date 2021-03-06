#ifndef LINKSTACK_H
#define LINKSTACK_H

#include "Stack.h"
#include "LinkList.h"

namespace DTLib
{

template <typename T>
class LinkStack : public Stack<T>
{
protected:
    LinkList<T> m_list;
public:
    void push(const T& e) override // O(1)
    {
        m_list.insert(0, e);
    }

    void pop() override // O(1)
    {
        if(m_list.length() > 0)
        {
            m_list.remove(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "No element in current stack ...");
        }
    }

    T top() const override // O(1)
    {
        if(m_list.length() > 0)
        {
            return m_list.get(0);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "No element in current stack ...");
        }
    }
    void clear() override // O(n)
    {
        m_list.clear();
    }

    int size() const override // O(1)
    {
        return  m_list.length();
    }
};

}
#endif // LINKSTACK_H
