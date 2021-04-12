#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Array.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class DynamicArray : public Array<T>
{
protected:
    int m_length;

    void init(T* arr, int length)
    {
        if(arr != nullptr)
        {
            this->m_array = arr;
            m_length = length;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create DynamicArray ...");
        }
    }

    T* copy(T* arr, int len, int newLen)
    {
        T* ret = new T[newLen];

        if(ret != nullptr)
        {
            int size = len < newLen ? len : newLen;

            for(int i = 0; i < size; i++)
            {
                ret[i] = arr[i];
            }
        }

        return ret;
    }

    void update(T* arr, int length)
    {
        if(arr != nullptr)
        {
            T* tmp = this->m_array;

            this->m_array = arr;
            m_length = length;

            delete[] tmp;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to DynamicArray ...");
        }
    }
public:
    DynamicArray(int length = 0)
    {
        init(new T[length], length);
    }

    DynamicArray(const DynamicArray<T>& rhs)
    {
        init(copy(rhs.m_array, rhs.m_length, rhs.m_length), rhs.m_length);
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& rhs)
    {
        if(this != &rhs)
        {
            update(copy(rhs.m_array, rhs.m_length, rhs.m_length), rhs.m_length);
        }

        return *this;
    }

    int length() const
    {
        return m_length;
    }

    void resize(int length)
    {
        if(length != m_length)
        {
            update(copy(this->m_array, m_length, length), length);
        }
    }

    ~DynamicArray()
    {
        delete[] this->m_array;
    }
};

}

#endif // DYNAMICARRAY_H
