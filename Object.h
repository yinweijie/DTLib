#ifndef OBJECT_H
#define OBJECT_H

#include <cstdlib>

namespace DTLib
{

class Object
{
public:
    void* operator new(size_t size) noexcept;
    void operator delete(void* p);
    void* operator new[](size_t size) noexcept;
    void operator delete[](void* p);
    bool operator==(const Object& rhs);
    bool operator!=(const Object& rhs);
    virtual ~Object() = 0;
};

}

#endif // OBJECT_H
