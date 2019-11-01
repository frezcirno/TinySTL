#pragma once
#include "type_traits.hpp"
namespace tinySTL
{

//以x为初始值在[begin,end)范围内构造元素
template <class Iter, typename T>
inline void uninitialized_fill(Iter begin, Iter end, const T &x)
{
    typedef typename type_traits<T>::is_POD is_POD;
    return __uninitialized_fill(begin, end, x, is_POD());
}
template <class Iter, typename T>
void __uninitialized_fill(Iter begin, Iter end, const T &x, __true)
{
    while (begin != end)
    {
        *begin = x;
        ++begin;
    }
}
template <class Iter, typename T>
void __uninitialized_fill(Iter begin, Iter end, const T &x, __false)
{
    while (begin != end)
    {
        construct(begin, x);
        ++begin;
    }
}

//以x为初始值构造n个元素
template <class Iter, typename T>
inline Iter uninitialized_fill_n(Iter pos, size_t n, const T &x)
{
    typedef typename type_traits<T>::is_POD is_POD;
    return __uninitialized_fill_n(pos, n, x, is_POD());
}
template <class Iter, typename T>
Iter __uninitialized_fill_n(Iter pos, size_t n, const T &x, __true)
{
    while (n > 0)
    {
        *pos = x;
        --n;
        ++pos;
    }
    return pos;
}
template <class Iter, typename T>
Iter __uninitialized_fill_n(Iter pos, size_t n, const T &x, __false)
{
    while (n > 0)
    {
        construct(pos, x);
        --n;
        ++pos;
    }
    return pos;
}

//以[begin,end)处元素为初始值在dest处移动构造
template <class Iter, class Iter2>
inline Iter2 uninitialized_copy(Iter begin, Iter end, Iter2 dst)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    typedef typename type_traits<value_type>::is_POD is_POD;
    return __uninitialized_copy(begin, end, dst, is_POD());
}
template <class Iter, class Iter2>
Iter2 __uninitialized_copy(Iter begin, Iter end, Iter2 dst, __true)
{
    while (begin != end)
    {
        *dst = *begin;
        ++begin;
        ++dst;
    }
    return dst;
}
template <class Iter, class Iter2>
Iter2 __uninitialized_copy(Iter begin, Iter end, Iter2 dst, __false)
{
    while (begin != end)
    {
        construct(dst, *begin);
        ++begin;
        ++dst;
    }
    return dst;
}
} // namespace tinySTL