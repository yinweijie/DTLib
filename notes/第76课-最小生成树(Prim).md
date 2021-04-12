在n个顶点直接选择n-1条边，使两两顶点可达，且n-1条边权值和最小：
![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200716232831.png)

prim算法用于解决这个问题，过程如下：
![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200716233005.png)

$$
T: 0 \qquad H: 1 \quad 2 \quad 3 \\
T: 0 \quad 1 \qquad H: 2 \quad 3 \\
T: 0 \quad 1 \quad 2 \qquad H: 3 \\
T: 0 \quad 1 \quad 3 \qquad H: \phi
$$

`Array<bool> mark`：标记元素所属集合，`true`表示在$T$集合，`false`表示在$H$集合

`Array<E> cost`：$H$集合中元素到$T$集合中的权值，例如在第一行中，`cost`数组会分别记录`cost[1]`、`cost[2]`、`cost[3]`到集合$T$中的值（如果有的话），并且如果下一次这些值有更新，会和当前值对比，选择较小的保存

`Array<int> adjVex`：记录`cost`中权值对应的顶点，例如在第一行中，`adjVex[1]`、`adjVex[2]`、`adjVex[3]`会保存$H$中元素$1,2,3$分别和谁相连

`Queue<Edge> ret`：每次选择最短的那条边，保存到`ret`中

算法流程如下：

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200717002324.png)

代码实现：

首先在`Graph`中添加两个函数：
```cpp
public:
    virtual bool isAdjacent(int i, int j) = 0;

...

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
```

`asUndirected`用来判断该图是否为无向图，`isAdjacent`用来判断两个点是否相连，只有相连才进一步判断是否双向且权值相等。

`isAdjacent`函数在`MatrixGraph`中的实现为：
```cpp
public:
    bool isAdjacent(int i, int j) override
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_edges[i][j] != nullptr);
    }
```
在`ListGraph`中的实现为：
```cpp
public:
    bool isAdjacent(int i, int j) override
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_list.get(i)->edge.find(Edge<E>(i, j)) >= 0);
    }
```

`Prim`在`Graph`中的实现为：
```cpp
public:
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
```