#pragma once

namespace tinySTL
{

template <typename T>
struct less
{
    bool operator()(const T &x, const T &y) { return x < y; }
};

template <typename T>
struct greater
{
    bool operator()(const T &x, const T &y) { return x > y; }
};

} // namespace tinySTL
