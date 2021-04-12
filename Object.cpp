#include "Object.h"
#include <cstdlib>
#include <iostream>

using namespace std;

namespace DTLib
{

void* Object::operator new(size_t size) noexcept
{
    return malloc(size);
}

void Object::operator delete(void* p)
{
    free(p);
}

void* Object::operator new[](size_t size) noexcept
{
    return malloc(size);
}

void Object::operator delete[](void* p)
{
    free(p);
}

bool Object::operator==(const Object& rhs)
{
    return (this == &rhs);
}

bool Object::operator!=(const Object& rhs)
{
    return (this != &rhs);
}

Object::~Object()
{

}

}
