#ifndef TREENODE_H
#define TREENODE_H

#include "Object.h"

namespace DTLib
{

template <typename T>
class TreeNode : public Object
{
protected:
    bool m_flag;

    TreeNode(const TreeNode<T>& rhs) = delete;

    TreeNode& operator=(const TreeNode<T>& rhs) = delete;

    void* operator new(unsigned int size) noexcept
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T>* parent;

    TreeNode()
    {
        m_flag = false;
        parent = nullptr;
    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template <typename T>
TreeNode<T>::~TreeNode()
{

}

}
#endif // TREENODE_H
