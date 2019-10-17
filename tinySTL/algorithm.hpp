#pragma once
#include <algorithm>

namespace tinySTL
{
template <class Iter>
Iter copy(Iter start, Iter end, Iter dst)
{
    while (start != end)
    {
        *dst = *start;
        ++start;
        ++dst;
    }
    return dst;
}

template <class Iter>
Iter copy_backward(Iter start, Iter end, Iter dst_end)
{
    do
    {
        --dst_end;
        --end;
        *dst_end = *end;
    } while (end != start);
    return dst_end;
}

template <typename T, class Iter>
void fill(Iter start, Iter end, const T &value)
{
    while (start != end)
    {
        *start = value;
        ++start;
    }
}

} // namespace tinySTL