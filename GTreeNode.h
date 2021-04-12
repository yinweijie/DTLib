#ifndef GTREENODE_H
#define GTREENODE_H

#include "LinkList.h"
#include "TreeNode.h"

namespace DTLib
{

template <typename T>
class GTreeNode : public TreeNode<T>
{
public:
    LinkList<GTreeNode<T>*> child;

    static GTreeNode<T>* NewNode()
    {
        GTreeNode<T>* ret = new GTreeNode();

        if(ret != nullptr)
        {
            ret->m_flag = true;
        }

        return ret;
    }
};

}

#endif // GTREENODE_H
