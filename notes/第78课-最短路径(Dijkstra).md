![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200725010512.png)

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200725011534.png)

这里借鉴计蒜客中的算法描述：

> 我们定义带权图 $G$ 所有顶点的集合为 $V$，接着我们再定义已确定最短路径的顶点集合为 $U$，初始集合 $U$ 为空。接着执行以下操作：
> 1. 首先我们将起点 $x$ 加入集合 $U$，并在数组 $A$ 中记录起点 $x$ 到各个点的最短路径（如果顶点到起点 $x$ 有直接相连的边，则最短路径为边权值，否则为一个极大值）。
> 2. 从数组 $A$ 中选择一个距离起点 $x$ 最近的，且不属于集合 $U$ 的顶点 $v$（如果有多个顶点 $v$，任选其一即可），将顶点 $v$ 加入集合 $U$，并更新所有与顶点 $v$ 相连的顶点到起点 $x$ 的最短路径。
> 3. 重复第二步操作，直至集合 $U$ 等于集合 $V$。算法结束，数组 $A$ 记录了起点 $x$ 到其余 $n - 1$ 个点的最短路径。

代码：
`Graph.h`
```cpp
public:
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
```