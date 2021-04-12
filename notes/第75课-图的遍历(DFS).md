1. 非递归版深度优先搜索

对于非递归版的深度优先搜索，其核心过程与广度优先搜索一模一样，唯一的不同是，深度优先借助的是堆数据结构，而广度优先借助的是队列数据结构

代码实现：

```cpp
public:
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
```

2. 递归版深度优先搜索

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200704161140.png)

递归版的深度优先不需要借助额外的数据结构，而是用类似二叉树先序遍历的方式进行搜索，即先搜索当前节点，然后递归遍历当前顶点的邻接顶点

代码实现：

```cpp
template <typename V, typename E>
void DFS(Graph<V, E>& g, int v, Array<bool>& visited)
{
    if((0 <= v) && (v < g.vCount()))
    {
        cout << v << endl;

        visited[v] = true;

        SharedPointer<Array<int>> aj = g.getAdjacent(v);

        for(int i = 0; i < aj->length(); i++)
        {
            if(!visited[(*aj)[i]])
            {
                DFS(g, (*aj)[i], visited);
            }
        }
    }
    else
    {
        THROW_EXCEPTION(InvalidParameterException, "Index v is invalid ...");
    }
}

template <typename V, typename E>
void DFS(Graph<V, E>& g, int v)
{
    DynamicArray<bool> visited(g.vCount());

    for(int i = 0; i < visited.length(); i++)
    {
        visited[i] = false;
    }

    DFS(g, v, visited);
}
```