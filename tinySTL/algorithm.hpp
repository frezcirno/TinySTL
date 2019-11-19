#pragma once
#define _CRT_SECURE_NO_WARNINGS
namespace tinySTL
{
template <class Iter, class Iter2>
Iter2 copy(Iter start, Iter end, Iter2 dst)
{
    while (start != end)
    {
        *dst = *start;
        ++start;
        ++dst;
    }
    return dst;
}

//  [start, end) -> [ret, dst_end)
template <class Iter, class Iter2>
Iter2 copy_backward(Iter start, Iter end, Iter2 dst_end)
{
    while (end != start) {
        --dst_end;
        --end;
        *dst_end = *end;
    }
    return dst_end;
}

template <typename T, class Iter>
Iter fill(Iter start, Iter end, const T &value)
{
    while (start != end)
    {
        *start = value;
        ++start;
    }
    return start;
}

template <typename T, class Iter>
Iter fill_n(Iter start, unsigned int n, const T &value)
{
    while (n != 0)
    {
        *start = value;
        ++start;
        --n;
    }
    return start;
}

template<typename T>
void swap(T& t1, T& t2) {
	T tmp = t1;
	t1 = t2;
	t2 = tmp;
}

template <class Iter, typename T>
Iter find(Iter begin, Iter end, const T &value)
{
    while (begin != end) {
        if (*begin == value) return begin;
        ++begin;
    }
    return end;
}

} // namespace tinySTL