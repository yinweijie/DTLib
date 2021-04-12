#ifndef STATICARRAY_H
#define STATICARRAY_H

#include "Array.h"

namespace DTLib
{

template <typename T, int N>
class StaticArray : public Array<T>
{
protected:
    T m_space[N];
public:
    StaticArray()
    {
        this->m_array = m_space;
    }

    StaticArray(const StaticArray<T, N>& rhs)
    {
        this->m_array = m_space; // 这一步不能漏了，拷贝构造也是构造函数的一种，说明此时还没有创建对象，此时this->m_array还未被赋值。

        for(int i = 0; i < N; i++)
        {
            this->m_array[i] = rhs.m_space[i];
        }
    }
    StaticArray<T, N>& operator=(const StaticArray<T, N>& rhs)
    {
        if(this != &rhs)
        {
            for(int i = 0; i < N; i++)
            {
                this->m_array[i] = rhs.m_space[i];
            }
        }

        return *this;
    }

    int length() const override
    {
        return N;
    }
};

}

#endif // STATICARRAY_H
