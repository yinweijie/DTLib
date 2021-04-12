#ifndef BTREE_H
#define BTREE_H

#include "Tree.h"
#include "BTreeNode.h"
#include "LinkQueue.h"
#include "DynamicArray.h"

namespace DTLib
{

enum BTTraversal
{
    PreOder,
    InOrder,
    PostOrder,
    LevelOrder
};

template <typename T>
class BTree : public Tree<T>
{
protected:
    LinkQueue<BTreeNode<T>*> m_queue;

    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value) const
    {
        BTreeNode<T>* ret = nullptr;

        if(node != nullptr)
        {
            if(node->value == value)
            {
                ret = node;
            }
            else
            {
                if(ret == nullptr) // 左子树一定会查找
                {
                    ret = find(node->left, value);
                }

                if(ret == nullptr) // 左子树找不到才会在右子树查找
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    virtual BTreeNode<T>* find(BTreeNode<T>* node, BTreeNode<T>* obj) const
    {
        BTreeNode<T>* ret = nullptr;

        if(node == obj)
        {
            ret = node;
        }
        else
        {
            if(node != nullptr)
            {
                if(ret == nullptr)
                {
                    ret = find(node->left, obj);
                }

                if(ret == nullptr)
                {
                    ret = find(node->right, obj);
                }
            }
        }

        return ret;
    }

    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;

        if(pos == ANY)
        {
            if(np->left == nullptr)
            {
                np->left = n;
            }
            else if(np->right == nullptr)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else if(pos == LEFT)
        {
            if(np->left == nullptr)
            {
                np->left = n;
            }
            else
            {
                ret = false;
            }
        }
        else if(pos == RIGHT)
        {
            if(np->right == nullptr)
            {
                np->right = n;
            }
            else
            {
                ret = false;
            }
        }
        else
        {
            ret = false;
        }

        return ret;
    }

    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();

        if(ret != nullptr)
        {
            if(root() == node)
            {
                this->m_root = nullptr;
            }
            else
            {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);

                if(parent->left == node)
                {
                    parent->left = nullptr;
                }
                if(parent->right == node)
                {
                    parent->right = nullptr;
                }

                node->parent = nullptr;
            }

            ret->m_root = node;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create node");
        }
    }

    virtual void free(BTreeNode<T>* node)
    {
        if(node != nullptr)
        {
            free(node->left);
            free(node->right);

            if(node->flag())
            {
                delete node;
            }
        }

    }

    virtual int degree(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != nullptr)
        {
            ret = !!node->left + !!node->right; // 非0值两次取反得到1，NULL两次取反得到0

            if(ret < 2) // node子节点小于2才需要继续递归寻找
            {
                int deg = degree(node->left); // 先从左子树寻找
                ret = ret > deg ? ret : deg;
            }

            if(ret < 2) // 左子树依然没有找到节点数等于2的，才继续寻找右子树
            {
                int deg = degree(node->right);
                ret = ret > deg ? ret : deg;
            }
        }

        return ret;
    }

    virtual int count(BTreeNode<T>* node) const
    {
        return (node != nullptr) ? (count(node->left) + count(node->right) + 1) : 0;
    }

    virtual int height(BTreeNode<T>* node) const
    {
        int ret = 0;

        if(node != nullptr)
        {
            int h1 = height(node->left);
            int h2 = height(node->right);
            ret = h1 > h2 ? h1 : h2;

            ret += 1;
        }

        return ret;
    }

    void preOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            queue.add(node);
            preOrderTraversal(node->left, queue);
            preOrderTraversal(node->right, queue);
        }
    }

    void inOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            inOrderTraversal(node->left, queue);
            queue.add(node);
            inOrderTraversal(node->right, queue);
        }
    }

    void postOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            postOrderTraversal(node->left, queue);
            postOrderTraversal(node->right, queue);
            queue.add(node);
        }
    }

    void levelOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            LinkQueue<BTreeNode<T>*> tmp;

            tmp.add(node);

            while(tmp.length() > 0)
            {
                BTreeNode<T>* n = tmp.front();

                if(n->left != nullptr)
                {
                    tmp.add(n->left);
                }
                if(n->right != nullptr)
                {
                    tmp.add(n->right);
                }

                tmp.remove();
                queue.add(n);
            }
        }
    }

    BTreeNode<T>* clone(BTreeNode<T>* node) const
    {
        BTreeNode<T>* ret = nullptr;

        if(node != nullptr)
        {
            ret = BTreeNode<T>::NewNode();

            if(ret != nullptr)
            {
                ret->value = node->value;
                ret->left = clone(node->left); // 拷贝左孩子节点，就会递归的拷贝左孩子的整个子树
                ret->right = clone(node->right);

                if(ret->left != nullptr) // 将左孩子的父节点设置为当前节点
                {
                    ret->left->parent = node;
                }
                if(ret->right != nullptr)
                {
                    ret->right->parent = node;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new node ...");
            }
        }

        return ret;
    }

    bool equal(BTreeNode<T>* lh, BTreeNode<T>* rh)
    {
        if(lh == rh) // nullptr == nullptr的时候，说明某一侧递归到叶子节点了，中途的节点都相等，返回true
        {
            return true;
        }
        else if(lh == nullptr && rh != nullptr) // 只要出现其中一个节点为空，另一个不为空，说明两个树不一样，返回false
        {
            return false;
        }
        else if(lh != nullptr && rh == nullptr)
        {
            return false;
        }

        return (lh->value == rh->value) && (equal(lh->left, rh->left)) && (equal(lh->right, rh->right));
    }

    BTreeNode<T>* add(BTreeNode<T>* lh, BTreeNode<T>* rh) const
    {
        BTreeNode<T>* ret = nullptr;

        if(lh == nullptr && rh != nullptr)
        {
            ret = clone(rh);
        }
        else if(lh != nullptr && rh == nullptr)
        {
            ret = clone(lh);
        }
        else if(lh != nullptr && rh != nullptr)
        {
            ret = BTreeNode<T>::NewNode();

            if(ret != nullptr)
            {
                ret->value = lh->value + rh->value;
                ret->left = add(lh->left, rh->left);
                ret->right = add(lh->right, rh->right);

                if(ret->left != nullptr)
                {
                    ret->left->parent = ret;
                }
                if(ret->right != nullptr)
                {
                    ret->right->parent = ret;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new node ...");
            }
        }

        return ret;
    }

    void traversal(BTTraversal order, LinkQueue<BTreeNode<T>*>& queue)
    {
        switch(order)
        {
        case PreOder:
            preOrderTraversal(root(), queue);
            break;
        case InOrder:
            inOrderTraversal(root(), queue);
            break;
        case PostOrder:
            postOrderTraversal(root(), queue);
            break;
        case LevelOrder:
            levelOrderTraversal(root(), queue);
            break;
        default:
            THROW_EXCEPTION(InvalidParameterException, "Parameter order is invalid ...");
            break;
        }
    }

    BTreeNode<T>* connect(LinkQueue<BTreeNode<T>*>& queue)
    {
        BTreeNode<T>* ret = nullptr;

        if(queue.length() > 0)
        {
            ret = queue.front();

            BTreeNode<T>* slider = queue.front();
            queue.remove();
            slider->left = nullptr;

            while(queue.length() > 0)
            {
                slider->right = queue.front();
                queue.front()->left = slider;
                slider = queue.front();
                queue.remove();
            }

            slider->right = nullptr;
        }

        return ret;
    }
public:
    bool insert(TreeNode<T>* node) override
    {
        return insert(node, ANY);
    }

    virtual bool insert(TreeNode<T>* node, BTNodePos pos)
    {
        bool ret = true;

        if(node != nullptr)
        {
            if(this->m_root == nullptr)
            {
                this->m_root = node;
                node->parent = nullptr;
            }
            else
            {
                BTreeNode<T>* np = find(node->parent);

                if(np != nullptr)
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(node), np, pos);
                }
                else
                {
                    THROW_EXCEPTION(InvalidOperationException, "Invalid parent tree node ...");
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Parameter cannot be NULL ...");
        }

        return ret;
    }

    bool insert(const T& value, TreeNode<T>* parent) override
    {
        return insert(value, parent, ANY);
    }

    virtual bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
    {
        bool ret = true;
        BTreeNode<T>* node = BTreeNode<T>::NewNode();

        if(node != nullptr)
        {
            node->value = value;
            node->parent = parent;

            ret = insert(node, pos);

            if(ret != true)
            {
                delete node;
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree node..");
        }

        return ret;
    }

    SharedPointer<Tree<T>> remove(const T& value) override
    {
        BTree<T>* ret = nullptr;

        BTreeNode<T>* node = find(value);

        if(node != nullptr)
        {
            remove(node, ret);
            m_queue.clear();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Cannot find node via given value ...");
        }

        return ret;
    }

    SharedPointer<Tree<T>> remove(TreeNode<T>* node) override
    {
        BTree<T>* ret = nullptr;

        node = find(node);

        if(node != nullptr)
        {
            remove(dynamic_cast<BTreeNode<T>*>(node), ret);
            m_queue.clear();
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Paramter node is invalid ...");
        }

        return ret;
    }

    BTreeNode<T>* find(const T& value) const override
    {
        return find(root(), value);
    }

    BTreeNode<T>* find(TreeNode<T>* node) const override
    {
        return find(root(), dynamic_cast<BTreeNode<T>*>(node));
    }

    BTreeNode<T>* root() const override
    {
        return dynamic_cast<BTreeNode<T>*>(this->m_root);
    }

    int degree() const override
    {
        return degree(root());
    }

    int count() const override
    {
        return count(root());
    }

    int height() const override
    {
        return height(root());
    }

    void clear() override
    {
        free(root());
        this->m_root = nullptr;
        m_queue.clear();
    }

    bool begin() override
    {
        bool ret = (root() != nullptr);

        if(ret)
        {
            m_queue.clear();
            m_queue.add(root());
        }

        return ret;
    }

    T current() override
    {
        if(!end())
        {
            return (m_queue.front()->value);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }
    }

    bool next() override
    {
        bool ret = (m_queue.length() > 0);

        if(ret)
        {
            BTreeNode<T>* node = m_queue.front();
            m_queue.remove();

            if(node->left != nullptr) m_queue.add(node->left);
            if(node->right != nullptr) m_queue.add(node->right);
        }

        return ret;
    }

    bool end() override
    {
        return (m_queue.length() == 0);
    }

    SharedPointer<Array<T>> traversal(BTTraversal order)
    {
        DynamicArray<T>* ret = nullptr;
        LinkQueue<BTreeNode<T>*> queue;

        traversal(order, queue);

        ret = new DynamicArray<T>(queue.length());

        if(ret != nullptr)
        {
            for(int i = 0; i < ret->length(); i++)
            {
    //            (*ret)[i] = queue.front()->value;
                ret->set(i, queue.front()->value);
                queue.remove();
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create return array ...");
        }

        return ret;
    }

    BTreeNode<T>* thread(BTTraversal order)
    {
        BTreeNode<T>* ret = nullptr;

        LinkQueue<BTreeNode<T>*> queue;
        traversal(order, queue); // 节点指针被压入队列中
        ret = connect(queue); // 队列中的节点关系被改为双向链表

        this->m_root = nullptr; // 树结构不复存在，元素被以双向链表的形式保存下来
        this->m_queue.clear();

        return ret;
    }

    SharedPointer<BTree<T>> clone() const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret != nullptr)
        {
            ret->m_root = clone(root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create new tree ...");
        }

        return ret;
    }

    bool operator==(const BTree<T>& tree)
    {
        return equal(root(), tree.root());
    }

    bool operator!=(const BTree<T>& tree)
    {
        return !(*this == tree);
    }

    SharedPointer<BTree<T>> add(const BTree<T>& tree) const
    {
        BTree<T>* ret = new BTree<T>();

        if(ret != nullptr)
        {
            ret->m_root = add(this->root(), tree.root());
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new tree ...");
        }

        return ret;
    }

    ~BTree()
    {
        clear();
    }
};

}
#endif // BTREE_H
