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
            // 创建原材料
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());
            DynamicArray<int> adjVex(vCount());
            SharedPointer<Array<int>> aj = nullptr;

            // 初始化原材料的值
            for(int i = 0; i < vCount(); i++)
            {
                mark[i] = false;
                cost[i] = LIMIT;
                adjVex[i] = -1;
            }

            // 从第0个点开始往T集合中加入元素
            int v = 0;

            mark[v] = true;
            aj = getAdjacent(v);

            // 依次记录与v邻接的元素，(*aj)[i]表示与v邻接的元素编号
            for(int i = 0; i < aj->length(); i++)
            {
                cost[(*aj)[i]] = getEdge(v, (*aj)[i]);
                adjVex[(*aj)[i]] = v;
            }

            bool end =false;
            // 每次将F集合中的一个元素放到T中，因此最多遍历vCount()次
            for(int i = 0; (i < vCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1;

                // 寻找集合T到集合H的最短路径
                for(int j = 0; j < cost.length(); j++)
                {
                    // 只在H集合中寻找
                    if(!mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }

                end = (k == -1); // H集合中没有元素了

                if(!end) // 如果还有元素，因此也找到了最短路径对应的元素
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));
                    mark[k] = true;

                    // 将k加入到T集合以后，会产生新的T到H的连接，需要更新cost数组的值，如果有更小的，需要覆盖原来的值
                    aj = getAdjacent(k);

                    for(int j = 0; j < aj->length(); j++)
                    {
                        // 只更新H中元素的权值
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
                    SharedPointer<Array<int>> aj = getAdjacent(v); // 数组中的邻接顶点编号是从小到大的

                    for(int i = aj->length() - 1; i >= 0; i--) // 但是我们希望最大的先压入栈中，每次弹出这一组最小的
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
            DynamicArray<E> dist(vCount()); // 到起点的最短距离
            DynamicArray<int> path(vCount()); // 最短路径中，前一个节点编号
            DynamicArray<bool> mark(vCount()); // 是否进入了U集合

            for(int k = 0; k < vCount(); k++) // 初始化
            {
                mark[k] = false;
                path[k] = -1;

                dist[k] = isAdjacent(i, k) ? (path[k] = i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true; // 将起点加入U集合

            for(int k = 0; k < vCount(); k++) // 逐一将节点加入U中
            {
                E m = LIMIT;
                int u = -1;

                // 选择A集合中dist[w]最小的
                for(int w = 0; w < vCount(); w++)
                {
                    if(!mark[w] && (dist[w] < m))
                    {
                        m = dist[w];
                        u = w;
                    }
                }

                if(u == -1) // 如果A中没有可以连接到U的点
                {
                    break;
                }

                mark[u] = true; // 最小的dist[w]被加入U集合

                // 前面选择完dist[w]最小的点u加入U后，更新dist
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

            s.push(j); // 从终点j开始往回找最短路径

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
                        // 看看能否通过中转k，找到更短路径从i->k->j
                        if(dist[i][k] + dist[k][j] < dist[i][j])
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k]; // 保存i->j的过程中，i的下一个顶点
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
