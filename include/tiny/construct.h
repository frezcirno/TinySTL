#pragma once
#include "type_traits.h"

namespace tiny
{
//构造元素
template <typename T1, typename T2>
inline void construct(T1 *p, const T2 &value)
{
    new (p) T1(value);
}

template <typename T1>
inline void construct(T1 *p)
{
    new (p) T1;
}

//析构元素
template <typename T>
inline void destroy(T *pointer)
{
    pointer->~T();
}

//析构元素(范围)
template <class Iter>
inline void destroy(Iter begin, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    typedef typename is_pod<value_type>::type is_POD;
    return __destroy(begin, end, is_POD());
}
template <class Iter>
inline void __destroy(Iter begin, Iter end, __true_type) {}
template <class Iter>
void __destroy(Iter begin, Iter end, __false_type)
{
    while (begin != end)
    {
        destroy(&(*begin));
        ++begin;
    }
}
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

} // namespace tiny
