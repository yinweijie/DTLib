#ifndef SEQLIST_H
#define SEQLIST_H

#include "List.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class SeqList : public List<T>
{
protected:
    T* m_array;
    int m_length;
public:
    bool insert(int i, const T& e)
    {
        bool ret = (0 <= i) && (i <= m_length);
        ret = ret && (m_length < capacity());

        if(ret)
        {
            for(int p = m_length-1; p >= i; p--)
            {
                m_array[p+1] = m_array[p];
            }

            m_array[i] = e;
            m_length++;
        }

        return ret;
    }

    bool insert(const T& e)
    {
        return insert(m_length, e);
    }

    bool remove(int i)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            for(int p = i; p < m_length-1; p++)
            {
                m_array[p] = m_array[p+1];
            }

            m_length--;
        }

        return ret;
    }

    int find(const T& e) const override
    {
        int ret = -1;

        for(int i = 0; i < m_length; i++)
        {
            if(m_array[i] == e)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            m_array[i] = e;
        }

        return ret;
    }

    bool get(int i , T& e) const
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            e = m_array[i];
        }

        return ret;
    }

    int length() const
    {
        return m_length;
    }

    void clear()
    {
        m_length = 0;
    }

    T& operator[](int i)
    {
        bool ret = (0 <= i) && (i < m_length);

        if(ret)
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "index out of bound...");
        }
    }

    const T& operator[](int i) const
    {
        return const_cast<const T&>((const_cast<SeqList<T>*>(this))->operator[](i));
    }

    virtual int capacity() const = 0;
};

}

#endif // SEQLIST_H
