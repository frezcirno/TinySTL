#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstddef>
namespace tinySTL
{
template <class Iter>
ptrdiff_t distance(Iter start, Iter end)
{
    ptrdiff_t dist = 0;
    while (start != end) {
        dist++;
        ++start;
    }
    return dist;
}

template <class Iter>
Iter advance(Iter start, size_t dist)
{
    while (dist) {
        --dist;
        ++start;
    }
    return start;
}

}  // namespace tinySTL
