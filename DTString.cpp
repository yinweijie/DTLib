#include <cstring>
#include <cstdlib>
#include "DTString.h"
#include "Exception.h"

namespace DTLib
{

void String::init(const char* s)
{
    m_str = strdup(s); // 这个函数用到了malloc，因此需要free去释放

    if(m_str)
    {
        m_length = strlen(s);
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create String object ...");
    }
}

bool String::equal(const char* l, const char* r, int len) const
{
    bool ret = true;

    for(int i = 0; i < len && ret; i++)
    {
        ret = ret && (l[i] == r[i]);
    }

    return ret;
}

int* String::make_pmt(const char* p)
{
    int len = strlen(p);
    int* ret = static_cast<int*>(malloc(sizeof(int) * len));

    if(ret != nullptr)
    {
        int ll = 0;
        ret[0] = 0;

        for(int i = 1; i < len; i++)
        {
            while((ll > 0) && (p[ll] != p[i]))
            {
                ll = ret[ll - 1];
            }

            if(p[ll] == p[i])
            {
                ll++;
            }

            ret[i] = ll;
        }
    }

    return ret;
}

int String::kmp(const char* s, const char* p) // s是较长的字符串，p是较短的字符串
{
    int ret = -1;
    int slen = strlen(s);
    int plen = strlen(p);
    int* pmt = make_pmt(p);

    if((pmt != nullptr) && (0 < plen) && (plen <= slen)) // pmt表生成成功 且 p串比s串短
    {
        for(int i = 0, j = 0; i < slen; i++)
        {
            while((j > 0) && (s[i] != p[j]))
            {
                j = pmt[j-1];
            }

            if(s[i] == p[j])
            {
                j++;
            }

            if(j == plen) // 最后一次s[i] == p[j]对比操作以后，j++又多进行了一次，所以j此时为len而不是len-1
            {
                ret = i + 1 - plen;
                break;
            }
        }
    }

    free(pmt);

    return ret;
}

String::String()
{
    init("");
}

String::String(char c)
{
    char s[] = {c, '\0'};

    init(s);
}

String::String(const char* s)
{
    init(s ? s : ""); // 有可能传过来一个nullptr，需要把nullptr转换成空字符串
}

String::String(const String& s)
{
    init(s.m_str);
}

int String::length() const
{
    return m_length;
}

const char* String::str() const
{
    return m_str;
}

bool String::startWith(const char* s) const
{
    bool ret = (s != nullptr);

    if(ret)
    {
        int len = strlen(s);

        ret = (len < m_length) && equal(m_str, s, len);
    }

    return ret;
}

bool String::startWith(const String& s) const
{
    return startWith(s.m_str);
}

bool String::endOf(const char* s) const
{
    bool ret = (s != nullptr);

    if(ret)
    {
        int len = strlen(s);
        char* str = m_str + (m_length - len);

        ret = (len < m_length) && equal(str, s, len);
    }

    return ret;
}

bool String::endOf(const String& s) const
{
    return endOf(s.m_str);
}

String& String::insert(int i, const char* s)
{
    if((0 <= i) && (i <= m_length))
    {
        if((s != nullptr) && (s[0] != '\0'))
        {
            int len = strlen(s);
            char* str = reinterpret_cast<char*>(malloc(m_length + len + 1));

            if(str)
            {
                strncpy(str, m_str, i);
                strncpy(str + i, s, len);
                strncpy(str + i + len, m_str + i, m_length - i);

                str[m_length + len] = '\0';

                free(m_str);
                m_str = str;
                m_length = m_length + len;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert string value ...");
            }
        }
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }

    return *this;
}

String& String::insert(int i, const String& s)
{
    return insert(i, s.m_str);
}

String& String::trim()
{
    int b = 0;
    int e = m_length - 1;

    while(m_str[b] == ' ') b++;
    while(m_str[e] == ' ') e--;

    if(b == 0)
    {
        m_str[e + 1] = '\0';
        m_length = e + 1;
    }
    else
    {
        for(int i = 0, j = b; j <= e; i++, j++)
        {
            m_str[i] = m_str[j];
        }

        m_str[e - b + 1] = '\0';
        m_length = e - b + 1;
    }

    return *this;
}

int String::indexOf(const char* s) const
{
    return kmp(m_str, s ? s : "");
}

int String::indexOf(const String& s) const
{
    return kmp(m_str, s.m_str);
}

String& String::remove(int i, int len)
{
    if((0 <= i) && (i < m_length))
    {
        int n = i;
        int m = i + len;

        while((n < m) && (m < m_length))
        {
            m_str[n++] = m_str[m++];
        }

        m_str[n] = '\0';
        m_length = n;
    }
    return *this;
}

String& String::remove(const char* s)
{
    return remove(indexOf(s), s ? strlen(s) : 0);
}

String& String::remove(const String& s)
{
    return remove(indexOf(s), s.length());
}

char& String::operator[](int i)
{
    if(0 <= i && i < m_length)
    {
        return m_str[i];
    }
    else
    {
        THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
    }
}

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

char String::operator[](int i) const
{
    return (const_cast<String&>(*this))[i];
}

bool String::operator==(const String& s) const
{
    return (strcmp(m_str, s.m_str) == 0);
}

bool String::operator==(const char* s) const
{
    return (strcmp(m_str, s ? s : "") == 0);
}

bool String::operator!=(const String& s) const
{
    return !(*this == s);
}

bool String::operator!=(const char* s) const
{
    return !(*this == s);
}

bool String::operator>(const String& s) const
{
    return (strcmp(m_str, s.m_str) > 0);
}

bool String::operator>(const char* s) const
{
    return (strcmp(m_str, s ? s : "") > 0);
}

bool String::operator<(const String& s) const
{
    return (strcmp(m_str, s.m_str) < 0);
}

bool String::operator<(const char* s) const
{
    return (strcmp(m_str, s ? s : "") < 0);
}

bool String::operator>=(const String& s) const
{
    return (strcmp(m_str, s.m_str) >= 0);
}

bool String::operator>=(const char* s) const
{
    return (strcmp(m_str, s ? s : "") >= 0);
}

bool String::operator<=(const String& s) const
{
    return (strcmp(m_str, s.m_str) <= 0);
}

bool String::operator<=(const char* s) const
{
    return (strcmp(m_str, s ? s : "") <= 0);
}

String String::operator+(const String& s) const
{
    return (*this + s.m_str); // 复用operator+(const char* s)
}

String String::operator+(const char* s) const
{
    String ret;
    int len = m_length + strlen(s ? s : "");
    char* str = reinterpret_cast<char*>(malloc(len + 1));

    if(str)
    {
        strcpy(str, m_str);
        strcat(str, s ? s : "");

        free(ret.m_str); // 新建立的String对象，指向空字符串，用到了strdup函数，是用malloc去创建的字符串

        ret.m_str = str;
        ret.m_length = len;
    }
    else
    {
        THROW_EXCEPTION(NoEnoughMemoryException, "No memory to add string ...");
    }
    return ret;
}

String& String::operator+=(const String& s)
{
    return (*this = *this + s.m_str); // 复用operator+(const char* s)和operator=(const char* s)
}

String& String::operator+=(const char* s)
{
    return (*this = *this + s); // 复用operator+(const char* s)和operator=(const char* s)
}

String String::operator-(const String& s) const
{
    return String(*this).remove(s); // 创建了一个与原对象一模一样的临时对象String，并删除子串s，最后返回这个临时对象
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
    return (*this = s.m_str); // 复用operator=(const char* s)
}

String& String::operator=(const char* s)
{
    if(m_str != s)
    {
        char* str = strdup(s ? s : "");

        if(str)
        {
            free(m_str);

            m_str = str;
            m_length = strlen(m_str);
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to assign new string ...");
        }
    }

    return (*this);
}

String& String::operator=(char c)
{
    char s[] = {c, '\0'};

    return (*this = s); // 复用operator=(const char* s)
}

String::~String()
{
    free(m_str);
}

}
