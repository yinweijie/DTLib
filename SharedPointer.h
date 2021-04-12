#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H

#include <cstdlib>
#include "Pointer.h"
#include "Exception.h"

namespace DTLib
{

template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int* m_ref;

    void assign(const SharedPointer<T>& rhs)
    {
        this->m_pointer = rhs.m_pointer;
        m_ref = rhs.m_ref; // 要保证所有智能指针的计数变量指针都指向同一块计数空间

        if(m_ref)
        {
            (*m_ref)++;
        }
    }
public:
    SharedPointer(T* p = nullptr) : m_ref(nullptr) // 不要忘记初始化成员变量
    {
        if(p)
        {
            m_ref = static_cast<int*>(std::malloc(sizeof(int)));

            if(m_ref)
            {
                *m_ref = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create SharedPointer object ...");
            }
        }
    }
    SharedPointer(const SharedPointer<T>& rhs)
    {
        assign(rhs);
    }

    SharedPointer<T>& operator=(const SharedPointer<T>& rhs)
    {
        if(this != &rhs)
        {
            clear();

            assign(rhs);
        }

        return *this;
    }

    void clear() // 置空指针，并检查引用计数是否为0，为0则清空堆内存
    {
        T* toDel = this->m_pointer;
        int* ref = m_ref;

        this->m_pointer = nullptr;
        m_ref = nullptr;

        if(ref)
        {
            (*ref)--;

            if(*ref == 0)
            {
                free(ref);

                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

}
#endif // SHAREDPOINTER_H
