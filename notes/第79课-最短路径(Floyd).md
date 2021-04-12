Floyd算法直接给出所有两点之间的最短距离。核心代码为：

```cpp
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
```
每次选取一个中转节点`k`，我们就看看所有两点是否可以通过该点中转得到更短的路径，如果可以，就认为这两点之间的距离变为这个更短的。如此就可以得到所有两点之间的最短距离。

为了得到路径，我们还需要保存中间节点，具体方法如下：

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200801233726.png)

初始的时候，如果两点`i, j`之间有直接连接就保存在`path[i][j]`中，一旦两点之间建立新的更短连接，就更新`path[i][j]`为新路径下一步指向的点。如此一来，我们可以通过`path[i][j]`找到下一个点`k1`，然通过`path[k1][j]`得到`k1->j`的下一个点`k2`，如此反复可以得到中间的所有节点。

完整代码：

```cpp
public:
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
```