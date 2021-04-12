#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "Graph.h"
#include "LinkList.h"
#include "Exception.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include <iostream>
using namespace std;

namespace DTLib
{

template <typename V, typename E>
class ListGraph : public Graph<V, E>
{
protected:
    struct Vertex : public Object
    {
        V* data;
        LinkList<Edge<E>> edge;

        Vertex()
        {
            data = nullptr;
        }
    };

    LinkList<Vertex*> m_list;

public:
    ListGraph(int n = 0)
    {
        for(int i = 0; i < n; i++)
        {
            addVertex();
        }
    }

    int addVertex() // O(n)
    {
        int ret = -1;

        Vertex* v = new Vertex();

        if(v != nullptr)
        {
            m_list.insert(v);
            ret = m_list.length() - 1;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new vertex object ...");
        }

        return ret;
    }

    int addVertex(const V& value) // O(n)
    {
        int ret = addVertex();

        if(ret >= 0)
        {
            setVertex(ret, value);
        }

        return ret;
    }

    bool setVertex(int i, const V& value) override // O(n)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            V* data = vertex->data;

            if(data == nullptr)
            {
                data = new V();
            }

            if(data != nullptr)
            {
                *data = value;
                vertex->data = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new data ...");
            }
        }

        return ret;
    }

    V getVertex(int i) override // O(n)
    {
        V ret;

        if(!getVertex(i, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Invalid index i ...");
        }

        return ret;
    }

    bool getVertex(int i, V& value) override // O(n)
    {
        bool ret = ((0 <= i) && (i < this->vCount()));

        if(ret)
        {
            Vertex* vertex = m_list.get(i);

            if(vertex->data != nullptr)
            {
                value = *(vertex->data);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
            }
        }

        return ret;
    }

    void removeVertex() // O(n^2)
    {
        if(m_list.length() > 0)
        {
            int index = m_list.length() - 1;
            Vertex* vertex = m_list.get(index); // O(n)
            V* data = vertex->data;

            if(m_list.remove(index))
            {
                for(int i = (m_list.move(0), 0); !m_list.end(); m_list.next(), i++) // O(n)
                {
                    int pos = m_list.current()->edge.find(Edge<E>(i, index)); // O(n)

                    if(pos >= 0)
                    {
                        m_list.current()->edge.remove(pos);
                    }
                }

                delete data;
                delete vertex;
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No vertex in current graph ...");
        }
    }

    SharedPointer<Array<int>> getAdjacent(int i) override // O(n)
    {
        DynamicArray<int>* ret = nullptr;

        if((0 <= i) && (i < vCount()))
        {
            Vertex* vertex = m_list.get(i);
            LinkList<Edge<E>>& edges = vertex->edge;

            ret = new DynamicArray<int>(edges.length());

            if(ret != nullptr)
            {
                for(int k = (edges.move(0), 0); !edges.end(); edges.next(), k++)
                {
                    (*ret)[k] = edges.current().e;
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new array ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    bool isAdjacent(int i, int j) override
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0);
    }

    E getEdge(int i, int j) override // O(n)
    {
        E ret;

        if(!getEdge(i, j, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Edge <i, j> is invalid ...");
        }

        return ret;
    }

    bool getEdge(int i, int j, E& value) override // O(n)
    {
        bool ret = (0 <= i) && (i < vCount()) &&
                   (0 <= j) && (j < vCount());

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                value = vertex->edge.get(pos).data;
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }

        return ret;
    }

    bool setEdge(int i, int j, const E& value) override // O(n)
    {
        bool ret = (0 <= i) && (i < vCount()) &&
                   (0 <= j) && (j < vCount());

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.set(pos, Edge<E>(i, j, value));
            }
            else
            {
                ret = vertex->edge.insert(0, Edge<E>(i, j, value));
            }
        }

        return ret;
    }

    bool removeEdge(int i, int j) override // O(n)
    {
        bool ret = (0 <= i) && (i < vCount()) &&
                   (0 <= j) && (j < vCount());

        if(ret)
        {
            Vertex* vertex = m_list.get(i);
            int pos = vertex->edge.find(Edge<E>(i, j));

            if(pos >= 0)
            {
                ret = vertex->edge.remove(pos);
            }
        }

        return ret;
    }

    int vCount() override // O(1)
    {
        return m_list.length();
    }

    int eCount() override // O(n)
    {
        int ret = 0;

        for(m_list.move(0); !m_list.end(); m_list.next())
        {
            ret += m_list.current()->edge.length();
        }

        return ret;
    }

    int OD(int i) override // O(n)
    {
        int ret = 0;

        if((0 <= i) && (i < vCount()))
        {
            Vertex* vertex = m_list.get(i);

            ret = vertex->edge.length();
        }

        return ret;
    }

    int ID(int i) override // O(n^2)
    {
        int ret = 0;

        if((0 <= i) && (i < vCount()))
        {
            for(m_list.move(0); !m_list.end(); m_list.next())
            {
                LinkList<Edge<E>>& edges = m_list.current()->edge;

                for(edges.move(0); !edges.end(); edges.next())
                {
                    if(edges.current().e == i)
                    {
                        ret++;
                        break;
                    }
                }
            }
        }

        return ret;
    }

    ~ListGraph()
    {
        while(m_list.length() > 0)
        {
            Vertex* vertex = m_list.get(0);

            m_list.remove(0);

            delete vertex->data;
            delete vertex;
        }
    }
};

}
#endif // LISTGRAPH_H
