#pragma once
#include <cstdlib>
#include <new>
#include "type_traits.hpp"
/**
 * 元素构造器
 * 详见<<STL源码剖析>>
 */
namespace tinySTL
{

//构造元素
template <typename T1, typename T2>
inline void construct(T1 *p, const T2 &value)
{
    new (p) T1(value);
}

//析构元素
template <typename T>
inline void destroy(T *ptr)
{
    ptr->~T();
}

//析构元素(范围)
template <typename T>
inline void destroy(T *begin, T *end)
{
    typedef iterator_traits<begin>::value_type value_type;
    typedef type_traits<value_type>::is_POD is_POD;
    return __destroy(begin,end,is_POD());
}
template <typename T>
void __destroy(T *begin, T *end,__true) {}
template <typename T>
void __destroy(T *begin, T *end,__false)
{
    while (begin != end)
    {
        begin->~T();
        ++begin;
    }
}

//以x为初始值在[begin,end)范围内构造元素
template <typename T>
inline void construct_range(T *begin, T *end, const T &x)
{
    typedef type_traits<T>::is_POD is_POD;
    return __construct_range(begin, end, x, is_POD());
}
template <typename T>
void __construct_range(T *begin, T *end, const T &x, __true)
{
    while (begin != end)
    {
        *begin = x;
        ++begin;
    }
}
template <typename T>
void __construct_range(T *begin, T *end, const T &x, __false)
{
    while (begin != end)
    {
        construct(begin, x);
        ++begin;
    }
}

//以x为初始值构造n个元素
template <typename T>
inline T *construct_n(T *pos, unsigned int n, const T &x)
{
    typedef type_traits<T>::is_POD is_POD;
    return __construct_n(pos, n, x, is_POD());
}
template <typename T>
T *__construct_n(T *pos, unsigned int n, const T &x, __true)
{
    while (n > 0)
    {
        *pos = x;
        --n;
        ++pos;
    }
    return pos;
}
template <typename T>
T *__construct_n(T *pos, unsigned int n, const T &x, __false)
{
    while (n > 0)
    {
        construct(pos, x);
        --n;
        ++pos;
    }
    return pos;
}

//以[begin,end)处元素为初始值在dest处复制构造
template <typename T>
inline T *construct_copy(T *begin, T *end, T *dst)
{
    typedef type_traits<T>::is_POD is_POD;
    return __construct_copy(begin, end, dst, is_POD());
}
template <typename T>
T *__construct_copy(T *begin, T *end, T *dst, __true)
{
    while (begin != end)
    {
        *dst = *begin;
        ++begin;
        ++dst;
    }
    return dst;
}
template <typename T>
T *__construct_copy(T *begin, T *end, T *dst, __false)
{
    while (begin != end)
    {
        construct(dst, *begin);
        ++begin;
        ++dst;
    }
    return dst;
}

//分配器
template <typename T>
class allocator
{
public:
    typedef T *ptr;
    typedef T &ref;
    typedef const T &c_ref;

    //空间分配
    static ptr allocate(unsigned int n = 1)
    {
        ptr space = static_cast<ptr>(::operator new(n * sizeof(T)));
        if (!space)
            exit(1);
        return space;
    };
    static void deallocate(ptr p) { ::operator delete[](p); };

    //取地址
    static ptr address(ref x) { return (ptr)&x; }
};

template <>
class allocator<void>
{
public:
    typedef void *ptr;
};

} // namespace tinySTL