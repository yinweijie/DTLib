1. `StaticList`的定义为：

接口：
```cpp
template <typename T, int N>
class StaticList : public SeqList<T>
{
protected:
    T m_space[N];
public:
    StaticList();
    int capacity() const override;
};
```

定义：
```cpp
template <typename T, int N>
class StaticList : public SeqList<T>
{
protected:
    T m_space[N];
public:
    StaticList()
    {
        this->m_array = m_space;
        this->m_length = 0;
    }
    int capacity() const override
    {
        return N;
    }
};
```

需要注意，对于模版类的继承，子类要获取父类的成员，必须要用`this`

2. `DynamicList`的接口和定义为：

接口：
```cpp
template <typename T>
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;
public:
    DynamicList(int capacity);
    int capacity() const override;
    void resize(int capacity);
    ~DynamicList();
};
```

定义：
```cpp
template <typename T>
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;
public:
    DynamicList(int capacity)
    {
        this->m_array = new T[capacity];

        if(this->m_array != nullptr)
        {
            this->m_length = 0;
            m_capacity = capacity;
        }
        else // 注意添加异常处理
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create DynamicList ...");
        }
    }
    int capacity() const override
    {
        return m_capacity;
    }
    void resize(int capacity)
    {
        if(capacity != m_capacity)
        {
            T* arr = new T[capacity]; // 先创建临时空间的好处是，如果没创建成功不会影响原来内存区的内容，如果先释放原来内存，再创建就可能出问题

            if(arr != nullptr)
            {
                int length = this->m_length < m_capacity ? this->m_length : m_capacity; // 有可能原来capacity是5，我resize为3

                for(int i = 0; i < length; i++)
                {
                    arr[i] = this->m_array[i];
                }

//                delete[] this->m_array;
                T* tmp = this->m_array;

                this->m_array = arr;
                m_capacity = capacity;
                this->m_length = length;

                delete[] tmp; // 如果T是类类型，delete触发析构函数就可能引发异常，为了保证上面3条语句都能被顺利执行，因此把delete放在最后
            }
            else // 注意这里保证了异常安全
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to resize");
            }
        }
    }
    ~DynamicList()
    {
        delete[] this->m_array;
    }
};
```

对于动态分配内存的地方，都要注意由于空间不足导致的异常处理。