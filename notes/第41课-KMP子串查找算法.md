1. KMP算法实现过程
对于如下的字符串：

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200602162911.png)

`i`会依次往后移动，而`j`只有在匹配上第一个字符以后，才会往后移动，且当`j`碰到和`i`位置不同的时候，会查表，更新`j`的位置，再和`i`位置的值对比，该过程的代码为：

```cpp
int kmp(const char* s, const char* p) // s是较长的字符串，p是较短的字符串
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
```

关键的地方在于这个`pmt`表的生成以及`pmt`表的含义。

2. PMT表的生成

- PMT表的含义

PMT是部分匹配表的缩写，每个字母的位置对应着，到这个字母为止，组成的字符串，最长的首尾匹配子串长度为多少：

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200602164039.png)

如下给出了MPT值的具体计算方法：

![](https://md-pic-1259272405.cos.ap-guangzhou.myqcloud.com/img/20200602164158.png)

重新审视下面这段代码：

```cpp
while((j > 0) && (s[i] != p[j]))
{
    j = pmt[j-1];
}
```

可以看到，当前面的字符都匹配上了，突然在某个位置匹配不上了，`j`就要往前挪，正好挪到首尾匹配子串中，前面那段子串的后面一位。

例如`j`当前为`D`这个位置，匹配不上，则往前挪到`C`这个位置，去继续与`i`对比，此时还是对比不上，则继续查PMT表`pmt[j-1]`，即`B`位置的值为0，则终止循环，用首位去和`i`对比。

- PMT表的生成

生成匹配表的过程，和查找子串的代码类似：

```cpp
int* make_pmt(const char* p)
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
```

其中`ll`代表和`i`位置字母相等前缀的**下标值加一**，这个值正好是位置`i`的PMT值，也即到`i`为止组成的字符串，首尾最长的匹配**字符串长度**。

首先第0位的PMT值为0，然后`i`往后循环，依次和`ll`去比对，一旦比对上了，`ll`做加一操作，这一动作既代表匹配长度增加了一，又代表`ll`和`i`匹配的位置往后挪了一位

当突然在某个位置匹配不上了，说明前缀`ll`和后缀`i`匹配不上了，`p[ll] != p[i]`，此时去查表，找到`ll-1`位置的PMT值，即往前找是否有匹配的位置，更新`ll = ret[ll - 1]`，然后再来和`i`位置的字符对比，如果匹配上了，`ll`更新后的值就是`i`这个位置的PMT值，如果一直没匹配上，最终`ll`会等于0.