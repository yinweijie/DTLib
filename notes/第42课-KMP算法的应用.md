1. 将KMP算法封装到`String`类中：

```cpp
static int* make_pmt(const char* p);
static int kmp(const char* s, const char* p);
```

2. 查找字符串的index：

```cpp
int indexOf(const char* s) const;
int indexOf(const String& s) const;
```
```cpp
int String::indexOf(const char* s) const
{
    return kmp(m_str, s ? s : "");
}

int String::indexOf(const String& s) const
{
    return kmp(m_str, s.m_str);
}
```

3. 删除子串：

```cpp
String& remove(int i, int len);
String& remove(const char* s);
String& remove(const String& s);
```
```cpp
String& String::remove(int i, int len)
{
    if((0 <= i) && (i < m_length))
    {
        int n = i;
        int m = i + len;

        while((n < m) && (m < m_length))
        {                            //  n   m             n   m             n   m                     n   m
            m_str[n++] = m_str[m++]; // |0|1|2|3|4| --> |2|1|2|3|4| --> |2|3|2|3|4| --> 跳出循环时|2|3|4|3|4|
        }

        m_str[n] = '\0'; // 循环结束的时候，m == m_length，n在移动后字符串最后一位的后面一位
        m_length = n;
    }

    return *this;
}

String& String::remove(const char* s)
{
    return remove(indexOf(s), strlen(s ? s : ""));
}

String& String::remove(const String& s)
{
    return remove(indexOf(s), strlen(s.m_str));
}
```

4. 替换子串，替换`t`为`s`：

```cpp
String& replace(const char* t, const char* s);
String& replace(const String& t, const char* s);
String& replace(const char* t, const String& s);
String& replace(const String& t, const String& s);
```
```cpp
String& String::replace(const char* t, const char* s)
{
    int index = indexOf(t);

    if(index >= 0)
    {
        remove(t);
        insert(index, s);
    }

    return *this;
}

String& String::replace(const String& t, const char* s)
{
    return replace(t.m_str, s);
}

String& String::replace(const char* t, const String& s)
{
    return replace(t, s.m_str);
}

String& String::replace(const String& t, const String& s)
{
    return replace(t.m_str, s.m_str);
}
```

5. 返回位置`i`处，长度为`len`的子串：

```cpp
String sub(int i, int len) const;
```
```cpp
String String::sub(int i, int len) const
{
    String ret;

    if((0 <= i) && (i < m_length))
    {
        if(len < 0) len = 0;
        if(i + len > m_length) len = m_length - i;

        char* str = reinterpret_cast<char*>(malloc(len + 1));
        strncpy(str, m_str + i, len);
        str[len] = '\0';

        ret = str;
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter is invalid ...");
    }

    return ret;
}
```

6. 删除子串，作用和`remove`相同：
```cpp
String operator-(const String& s) const;
String operator-(const char* s) const;
String& operator-=(const String& s);
String& operator-=(const char* s);
```
```cpp
String String::operator-(const String& s) const
{
    return String(*this).remove(s);
}

String String::operator-(const char* s) const
{
    return String(*this).remove(s);
}

String& String::operator-=(const String& s)
{
    return (*this = *this - s);
}

String& String::operator-=(const char* s)
{
    return (*this = *this - s);
}

String& String::operator=(const String& s)
{
    return (*this = s.m_str);
}
```