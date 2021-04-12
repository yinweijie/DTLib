#ifndef SORT_H
#define SORT_H

#include "Object.h"
#include "Array.h"

namespace DTLib
{

class Sort : public Object
{
private:
    Sort();
    Sort(const Sort& rhs);
    Sort& operator=(const Sort& rhs);

    template <typename T>
    static void Swap(T& a, T& b)
    {
        T c(a);
        a = b;
        b = c;
    }

    template <typename T>
    static void Merge(T src[], T helper[], int begin, int mid, int end, bool min2max)
    {
        int i = begin;
        int j = mid + 1;
        int k = begin;

        while((i <= mid) && (j <= end)) // 只要前后两部分，有一个到头了，就跳出循环，剩下的部分在后面的while循环中去操作
        {
            if(min2max ? (src[i] < src[j]) : (src[i] > src[j])) // 比较前后两个部分的值
            {
                helper[k++] = src[i++]; // 选出较小的，保存到辅助空间中，对于被选中的那个数，下标往后前进一位
            }
            else
            {
                helper[k++] = src[j++];
            }
        }

        while(i <= mid) // 循环i，j的时候，有可能其中一个先到达终点，另一个则全部紧接着放到辅助空间中
        {
            helper[k++] = src[i++];
        }

        while(j <= end)
        {
            helper[k++] = src[j++];
        }

        for(int i = begin; i <= end; i++) // 把排序好的部分重新拷贝到原始的数组中
        {
            src[i] = helper[i];
        }
    }

    template <typename T>
    static void Merge(T src[], T helper[], int begin, int end, bool min2max)
    {
        if(begin == end)
        {
            return;
        }

        int mid = (begin + end) / 2;

        Merge(src, helper, begin, mid, min2max); // 对前面一半进行归并排序
        Merge(src, helper, mid + 1, end, min2max); // 对后面一半进行归并排序
        Merge(src, helper, begin, mid, end, min2max); // 在前后两部分分各自排序好的基础上，做进一步的排序
    }

    template <typename T>
    static int Partition(T array[], int begin, int end, bool min2max)
    {
        T pv = array[begin]; // 将第一个元素的值作为pivot

        while(begin < end) // 每次循环都做两次Swap操作
        {
            while((begin < end) && (min2max ? (array[end] > pv) : (array[end] < pv))) // 一次是将数组靠近尾部小于pivot的值与靠近头部的值交换
            {
                end--;
            }

            Swap(array[begin], array[end]);

            while((begin < end) && (min2max ? (array[begin] <= pv) : (array[begin] >= pv))) // 另一次是将数组靠近头部大于等于pivot的值与靠近尾部的值交换，上面和下面的while循环，有一个取等就可以了，如果都不取等，则只有在不等的时候才会往后或往前挪，如果前后都移到与pivot的值相等，就会出现死循环
            {
                begin++;
            }

            Swap(array[begin], array[end]);
        }

        array[begin] = pv; // 最终begin和end重合

        return begin;
    }

    template <typename T>
    static void Quick(T array[], int begin, int end, bool min2max)
    {
        if(begin < end)
        {
            int pivot = Partition(array, begin, end, min2max); // 先找到pivot的位置

            Quick(array, begin, pivot - 1, min2max); // 将pivot前面的数进一步进行Quick排序
            Quick(array, pivot + 1, end, min2max); // 将pivot后面的数进一步进行Quick排序
        }
    }

public:
    template <typename T>
    static void Selection(T array[], int n, bool min2max = true)
    {
        for(int i = 0; i < n; i++)
        {
            int min = i; // 从i开始，往后找最小的元素，找到了最小的就和i交换，没有比i更小的就不交换
            for(int j = i + 1; j < n; j++)
            {
                if(min2max ? (array[min] > array[j]) : (array[min] < array[j]))
                {
                    min = j; // 标记最小元素的下标
                }
            }

            if(min != i) // 如果min == i，说明i后面没有更小的元素，此时不用交换
            {
                Swap(array[i], array[min]);
            }
        }
    }

    template <typename T>
    static void Insert(T array[], int n, bool min2max = true)
    {
        for(int i = 1; i < n; i++) // 第0个元素已经是排序好了的状态，所以从第1个元素开始
        {
            int k = i;
            T e = array[i]; // 把待插入元素拷贝一份

            for(int j = i - 1; (j >= 0) && (min2max ? (array[j] > e) : (array[j] < e)); j--)
            {
                array[j + 1] = array[j]; // 依次往前找是否有比待插入元素大的元素，如果有，就把这个元素往后挪一位，空出这个可能插入的位置
                k = j; // 标记可能插入的位置
            }

            if(k != i) // 如果k == i，说明前面没有比i大的元素，保持不动即可
            {
                array[k] = e; // 把元素i插入到前面合适的位置
            }
        }
    }

    template <typename T>
    static void Bubble(T array[], int n, bool min2max = true)
    {
        bool exchange = true;

        for(int i = 0; (i < n) && exchange; i++)
        {
            exchange = false;

            for(int j = n - 1; j > i; j--) // 从末尾到i + 1
            {
                if(min2max ? (array[j] < array[j - 1]) : (array[j] > array[j - 1])) // 相邻两两元素对比
                {
                    Swap(array[j], array[j - 1]);
                    exchange = true; // 如果交换，则做标记，如果都没有交换过，说明后面的元素已经有序
                }
            }
        }
    }

    template <typename T>
    static void Shell(T array[], int n, bool min2max = true)
    {
        int d = n;

        do
        {
            d = d / 3 + 1;

            for(int i = d; i < n; i+=d) // 第0个元素已经是排序好了的状态，所以从第1个元素开始
            {
                int k = i;
                T e = array[i]; // 把待插入元素拷贝一份

                for(int j = i - d; (j >= 0) && (min2max ? (array[j] > e) : (array[j] < e)); j-=d)
                {
                    array[j + d] = array[j]; // 依次往前找是否有比待插入元素大的元素，如果有，就把这个元素往后挪一位，空出这个可能插入的位置
                    k = j; // 标记可能插入的位置
                }

                if(k != i) // 如果k == i，说明前面没有比i大的元素，保持不动即可
                {
                    array[k] = e; // 把元素i插入到前面合适的位置
                }
            }
        }
        while(d > 1);
    }

    template <typename T>
    static void Merge(T array[], int n, bool min2max = true)
    {
        T* helper = new T[n]; // 辅助空间

        if(helper != nullptr)
        {
            Merge(array, helper, 0, n - 1, min2max);
        }

        delete[] helper;
    }

    template <typename T>
    static void Quick(T array[], int n, bool min2max = true)
    {
        Quick(array, 0, n - 1, min2max);
    }

    template <typename T>
    static void Selection(Array<T>& array, bool min2max = true)
    {
        Selection(array.array(), array.length(), min2max);
    }

    template <typename T>
    static void Insert(Array<T>& array, bool min2max = true)
    {
        Insert(array.array(), array.length(), min2max);
    }

    template <typename T>
    static void Bubble(Array<T>& array, bool min2max = true)
    {
        Bubble(array.array(), array.length(), min2max);
    }

    template <typename T>
    static void Shell(Array<T>& array, bool min2max = true)
    {
        Shell(array.array(), array.length(), min2max);
    }

    template <typename T>
    static void Merge(Array<T>& array, bool min2max = true)
    {
        Merge(array.array(), array.length(), min2max);
    }

    template <typename T>
    static void Quick(Array<T>& array, bool min2max = true)
    {
        Quick(array.array(), array.length(), min2max);
    }
};

}
#endif // SORT_H
