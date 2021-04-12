Kruskal思想比较直观，步骤如下：
![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200720000119.png)

关键点在于数组`p`的理解，`p[n]`记录的是连通路的另一端端点，例如：
![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200720002200.png)

现在如果我们想连通`1-4`，则`p[1]`首先返回`4`，然后作为下标`p[4]`进一步返回`-1`，此时`4`作为该次查找的返回值；再看`p[4]`返回`-1`，最终返回值为`4`。可以发现两端点的最终返回值相等，说明此时有连通，这条边不能选。这个过程实际上就是下图中的`find`函数过程：

算法描述为：
![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200720002646.png)

`find`函数实现为：
`Graph.h`
```cpp
private:
    int find(Array<int>& p, int v)
    {
        while(p[v] != -1)
        {
            v = p[v];
        }

        return v;
    }
```

获取所有边的实现：
```cpp
private:
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
```

`Kruskal`算法实现：
```cpp
public:
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
                p[e] = b; // 更新前驱标记数组p
                ret.add((*edges)[i]);
            }
        }

        if(ret.length() != vCount() - 1)
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for Kruskal operation ...");
        }

        return toArray(ret);
    }
```