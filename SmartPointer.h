#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Pointer.h"

namespace DTLib {

template <typename T>
class SmartPointer : Pointer<T>
{
public:
    SmartPointer(T* p = nullptr) : Pointer<T>(p)
    {

    }

    SmartPointer(const SmartPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer; // 模版中父类继承过来的成员需要this获取
        const_cast<SmartPointer<T>&>(obj).m_pointer = nullptr;
    }

    SmartPointer& operator=(const SmartPointer<T>& obj)
    {
        if(this != &obj)
        {
            T* tmp = this->m_pointer;

            this->m_pointer = obj.m_pointer;
            const_cast<SmartPointer<T>&>(obj).m_pointer = nullptr;

            delete tmp; // 始终最后释放空间，保证异常安全
        }

        return *this;
    }

    ~SmartPointer()
    {
        delete this->m_pointer;
    }
};

}

#endif // SMARTPOINTER_H
