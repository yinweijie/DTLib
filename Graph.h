#ifndef GRAPH_H
#define GRAPH_H

#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"

namespace DTLib
{

template <typename E>
struct Edge : public Object
{
    int b;
    int e;
    E data;

    Edge(int i = -1, int j = -1)
    {
        b = i;
        e = j;
    }

    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data = value;
    }

    bool operator==(const Edge<E>& rhs)
    {
        return ((b == rhs.b) && (e == rhs.e));
    }

    bool operator!=(const Edge<E>& rhs)
    {
        return !(*this == rhs);
    }

    bool operator<(const Edge<E>& rhs)
    {
        return (data < rhs.data);
    }

    bool operator>(const Edge<E>& rhs)
    {
        return (data > rhs.data);
    }
};

template <typename V, typename E>
class Graph : public Object
{
protected:
    template <typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if(ret != nullptr)
        {
            for(int i = 0; i < ret->length(); i++)
            {
                (*ret)[i] = queue.front();
                queue.remove();
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create new array ...");
        }

        return ret;
    }

    SharedPointer<Array<Edge<E>>> getUndirectedEdges()
    {
        DynamicArray<Edge<E>>* ret = nullptr;

        if(asUndirected())
        {
            LinkQueue<Edge<E>> queue;

            for(int i = 0; i < vCount(); i++)
            {
                for(int j = 0; j < vCount(); j++)
                {
                    if(isAdjacent(i, j))
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }

            ret = toArray(queue);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "This function is for undirected graph only ...");
        }

        return ret;
    }

    int find(Array<int>& p, int v)
    {
        while(p[v] != -1)
        {
            v = p[v];
        }

        return v;
    }
public:
    virtual V getVertex(int i) = 0;
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;
    virtual SharedPointer<Array<int>> getAdjacent(int i) = 0;
    virtual bool isAdjacent(int i, int j) = 0;
    virtual E getEdge(int i, int j) = 0;
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;
    virtual bool removeEdge(int i, int j) = 0;
    virtual int vCount() = 0;
    virtual int eCount() = 0;
    virtual int OD(int i) = 0;
    virtual int ID(int i) = 0;

    virtual int TD(int i)
    {
        return OD(i) + ID(i);
    }

    bool asUndirected()
    {
        bool ret = true;

        for(int i = 0; i < vCount(); i++)
        {
            for(int j = 0; j < vCount(); j++)
            {
                if(isAdjacent(i, j))
                {
                    ret = ret && (isAdjacent(j, i)) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }

        return ret;
    }

    SharedPointer<Array<Edge<E>>> Prim(const E LIMIT, const bool MINIMUM = true)
    {
        LinkQueue<Edge<E>> ret;

        if(asUndirected())
        {
            // ???????????????
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());
            DynamicArray<int> adjVex(vCount());
            SharedPointer<Array<int>> aj = nullptr;

            // ????????????????????????
            for(int i = 0; i < vCount(); i++)
            {
                mark[i] = false;
                cost[i] = LIMIT;
                adjVex[i] = -1;
            }

            // ??????0???????????????T?????????????????????
            int v = 0;

            mark[v] = true;
            aj = getAdjacent(v);

            // ???????????????v??????????????????(*aj)[i]?????????v?????????????????????
            for(int i = 0; i < aj->length(); i++)
            {
                cost[(*aj)[i]] = getEdge(v, (*aj)[i]);
                adjVex[(*aj)[i]] = v;
            }

            bool end =false;
            // ?????????F??????????????????????????????T????????????????????????vCount()???
            for(int i = 0; (i < vCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1;

                // ????????????T?????????H???????????????
                for(int j = 0; j < cost.length(); j++)
                {
                    // ??????H???????????????
                    if(!mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1); // H????????????????????????

                if(!end) // ??????????????????????????????????????????????????????????????????
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));
                    mark[k] = true;

                    // ???k?????????T??????????????????????????????T???H????????????????????????cost????????????????????????????????????????????????????????????
                    aj = getAdjacent(k);

                    for(int j = 0; j < aj->length(); j++)
                    {
                        // ?????????H??????????????????
                        if(!mark[(*aj)[j]] && (MINIMUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : (getEdge(k, (*aj)[j]) > cost[(*aj)[j]])))
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "Prim operation is for undirected graph only ...");
        }

        if(ret.length() != (vCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for prim operation ...");
        }

        return toArray(ret);
    }
    
    SharedPointer<Array<Edge<E>>> Kruskal(const bool MINIMUM = true)
    {
        LinkQueue<Edge<E>> ret;
        SharedPointer<Array<Edge<E>>> edges = getUndirectedEdges();
        DynamicArray<int> p(vCount());

        for(int i = 0; i < vCount(); i++)
        {
            p[i] = -1;
        }

        Sort::Shell(*edges, MINIMUM);

        for(int i = 0; (i < edges->length()) && (ret.length() < vCount() - 1); i++)
        {
            int b = find(p, (*edges)[i].b);
            int e = find(p, (*edges)[i].e);

            if(b != e)
            {
                p[e] = b;
                ret.add((*edges)[i]);
            }
        }

        if(ret.length() != vCount() - 1)
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for Kruskal operation ...");
        }

        return toArray(ret);
    }

    SharedPointer<Array<int>> BFS(int i)
    {
        DynamicArray<int>* ret = nullptr;

        if((0 <= i) && (i < vCount()))
        {
            LinkQueue<int> q;
            LinkQueue<int> r;

            DynamicArray<bool> visited(vCount());

            for(int i = 0; i < visited.length(); i++)
            {
                visited[i] = false;
            }

            q.add(i);

            while(q.length() > 0)
            {
                int v = q.front();
                q.remove();

                if(!visited[v])
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v);

                    for(int i = 0; i < aj->length(); i++)
                    {
                        q.add((*aj)[i]);
                    }

                    r.add(v);
                    visited[v] = true;
                }
            }

            ret = this->toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    SharedPointer<Array<int>> DFS(int i)
    {
        DynamicArray<int>* ret = nullptr;

        if((0 <= i) && (i < vCount()))
        {
            LinkStack<int> s;
            LinkQueue<int> r;

            DynamicArray<bool> visited(vCount());

            for(int i = 0; i < visited.length(); i++)
            {
                visited[i] = false;
            }

            s.push(i);

            while(s.size() > 0)
            {
                int v = s.top();
                s.pop();

                if(!visited[v])
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v); // ????????????????????????????????????????????????

                    for(int i = aj->length() - 1; i >= 0; i--) // ???????????????????????????????????????????????????????????????????????????
                    {
                        s.push((*aj)[i]);
                    }

                    r.add(v);
                    visited[v] = true;
                }
            }

            ret = this->toArray(r);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }

        return ret;
    }

    SharedPointer<Array<int>> Dijkstra(int i, int j, const E LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()))
        {
            DynamicArray<E> dist(vCount()); // ????????????????????????
            DynamicArray<int> path(vCount()); // ???????????????????????????????????????
            DynamicArray<bool> mark(vCount()); // ???????????????U??????

            for(int k = 0; k < vCount(); k++) // ?????????
            {
                mark[k] = false;
                path[k] = -1;

                dist[k] = isAdjacent(i, k) ? (path[k] = i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true; // ???????????????U??????

            for(int k = 0; k < vCount(); k++) // ?????????????????????U???
            {
                E m = LIMIT;
                int u = -1;

                // ??????A?????????dist[w]?????????
                for(int w = 0; w < vCount(); w++)
                {
                    if(!mark[w] && (dist[w] < m))
                    {
                        m = dist[w];
                        u = w;
                    }
                }

                if(u == -1) // ??????A????????????????????????U??????
                {
                    break;
                }

                mark[u] = true; // ?????????dist[w]?????????U??????

                // ???????????????dist[w]????????????u??????U????????????dist
                for(int w = 0; w < vCount(); w++)
                {
                    if(!mark[w] && isAdjacent(u, w) && (dist[u] + getEdge(u, w) < dist[w]))
                    {
                        dist[w] = dist[u] + getEdge(u, w);
                        path[w] = u;
                    }
                }
            }

            LinkStack<int> s;

            s.push(j); // ?????????j???????????????????????????

            for(int k = path[j]; k != -1; k = path[k])
            {
                s.push(k);
            }

            while(s.size() > 0)
            {
                ret.add(s.top());
                s.pop();
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <i, j> is invalid ...");
        }

        if(ret.length() < 2)
        {
            THROW_EXCEPTION(ArithmeticException, "There is not path from i to j...");
        }

        return toArray(ret);
    }

    SharedPointer<Array<int>> Floyd(int x, int y, const E LIMIT)
    {
        LinkQueue<int> ret;

        if((0 <= x) && (x < vCount()) && (0 <= y) && (y < vCount()))
        {
            DynamicArray<DynamicArray<int>> dist(vCount());
            DynamicArray<DynamicArray<int>> path(vCount());

            for(int i = 0; i < vCount(); i++)
            {
                dist[i].resize(vCount());
                path[i].resize(vCount());
            }

            for(int i = 0; i < vCount(); i++)
            {
                for(int j = 0; j < vCount(); j++)
                {
                    path[i][j] = -1;
                    dist[i][j] = isAdjacent(i, j) ? (path[i][j] = j, getEdge(i, j)) : LIMIT;
                }
            }

            for(int k = 0; k < vCount(); k++)
            {
                for(int i = 0; i < vCount(); i++)
                {
                    for(int j = 0; j < vCount(); j++)
                    {
                        // ????????????????????????k????????????????????????i->k->j
                        if(dist[i][k] + dist[k][j] < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k]; // ??????i->j???????????????i??????????????????
                        }
                    }
                }
            }

            while((x != -1) && (x != y))
            {
                ret.add(x);
                x = path[x][y];
            }

            if(x != -1) // x == y
            {
                ret.add(y);
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <x, y> is invalid ...");
        }

        if(ret.length() < 2)
        {
            THROW_EXCEPTION(ArithmeticException, "There is not path from x to y...");
        }

        return toArray(ret);
    }
};

}
#endif // GRAPH_H
