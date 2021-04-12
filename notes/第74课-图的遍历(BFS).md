1. 广度优先搜索

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200703212224.png)

实现步骤：

借助队列，先将起始顶点压入，然后每次弹出队首元素到另一个队列中时，将弹出顶点的邻接顶点压入队列中，如此反复；需要注意的是，对于已经压入返回队列中的元素，需要进行标记，防止重复压入返回队列。这个过程类似二叉树的层次遍历。

在`Graph`中增加函数：

```cpp
public:
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
```

最后需要将队列数据保存到`Array`中，并返回，定义函数：

```cpp
private:
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
```