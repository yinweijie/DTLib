#include <iostream>
#include <cstring>
#include "ListGraph.h"
#include "LinkList.h"

using namespace std;
using namespace DTLib;

// 局部最多顶点路径
int search_max_path(Graph<int, int>& g, int v, Array<int>& count, Array<LinkList<int>*>& path, Array<bool>& mark)
{
    int ret = 0;
//    int k = -1;

    SharedPointer<Array<int>> aj = g.getAdjacent(v);

    // 遍历v周围的邻接顶点
    for(int i = 0; i < aj->length(); i++)
    {
        int num = 0;

        // 找到邻接顶点的最大顶点数
        if(!mark[(*aj)[i]])
        {
            num = search_max_path(g, (*aj)[i], count, path, mark);
        }
        else
        {
            num = count[(*aj)[i]];
        }

        // 挑选出最大的那个
        if(ret < num)
        {
            ret = num;
//            k = (*aj)[i]; // 有可能周围有多个点符合要求
        }
    }

    for(int i = 0; i < aj->length(); i++)
    {
        if(ret == count[(*aj)[i]])
        {
            // 如果v周围有多个点符合要求，v的下一步就有多种走法
            path[v]->insert((*aj)[i]);
        }
    }

    ret++;

    count[v] = ret;
    mark[v] = true;
//    path[v] = k;

    return ret;
}

SharedPointer<Graph<int, int>> create_graph(int* a, int len)
{
    ListGraph<int, int>* g = new ListGraph<int, int>(len);

    for(int i = 0; i < len; i++)
    {
        g->setVertex(i, a[i]);
    }

    for(int i = 0; i < len; i++)
    {
        for(int j = i + 1; j < len; j++)
        {
            if(a[i] <= a[j])
            {
                g->setEdge(i, j, 1);
            }
        }
    }

    return g;
}

void init_array(Array<int>& count, Array<LinkList<int>*>& path, Array<bool>& mark)
{
    for(int i = 0; i < count.length(); i++)
    {
        count[i] = 0;
    }

    for(int i = 0; i < path.length(); i++)
    {
        path[i] = new LinkList<int>();
    }

    for(int i = 0; i < mark.length(); i++)
    {
        mark[i] = false;
    }
}

void print_path(Graph<int, int>& g, int v, Array<LinkList<int>*>& path, LinkList<int>& cp)
{
    cp.insert(v);

    if(path[v]->length() > 0)
    {
        // 拿出v下一步的点，可能有多个
        for(path[v]->move(0); !path[v]->end(); path[v]->next())
        {
            // 对v下一步的每个分支，进行递归
            print_path(g, path[v]->current(), path, cp);
        }
    }
    else // 到尽头了，v下一步没有顶点了
    {
        cout << "Element:";

        for(cp.move(0); !cp.end(); cp.next())
        {
            cout << g.getVertex(cp.current()) << " ";
        }

        cout << endl;
    }

    cp.remove(cp.length() - 1);
}

void print_max_path(Graph<int, int>& g, Array<int>& count, Array<LinkList<int>*>& path)
{
    int max = 0;
    LinkList<int> cp;

    for(int i = 0; i < count.length(); i++)
    {
        if(max < count[i])
        {
            max = count[i];
        }
    }

    cout << "Len: " << max << endl;

    for(int i = 0; i < count.length(); i++)
    {
        if(max == count[i])
        {
            print_path(g, i, path, cp);
//            cout << "Element: " << g.getVertex(i) << " ";

//            for(int j = path[i]; j != -1; j = path[j])
//            {
//                cout << g.getVertex(j) << " ";
//            }

//            cout << endl;
        }
    }
}

void search_max_path(Graph<int, int>& g, Array<int>& count, Array<LinkList<int>*>& path, Array<bool>& mark)
{
    for(int i = 0; i < g.vCount(); i++)
    {
        if(!mark[i])
        {
            search_max_path(g, i, count, path, mark);
        }
    }
}

void solution(int* a, int len)
{
    DynamicArray<int> count(len);
    DynamicArray<LinkList<int>*> path(len);
    DynamicArray<bool> mark(len);
    SharedPointer<Graph<int, int>> g;

    g = create_graph(a, len);

    init_array(count, path, mark);

    search_max_path(*g, count, path, mark);

    print_max_path(*g, count, path);
}

int main()
{
    int a[] = {1, 2, 3, 4, 5};

    solution(a, sizeof(a) / sizeof(int));

    return 0;
}
