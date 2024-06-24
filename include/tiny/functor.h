/***
	仿函数
***/
#pragma once

namespace tiny
{

template <typename T>
struct less
{
    bool operator()(const T &x, const T &y) const { return x < y; }
};

template <typename T>
struct greater
{
    bool operator()(const T &x, const T &y) const { return x > y; }
};

template <typename T>
struct identity
{
    T &operator()(T &x) const { return x; }
    const T &operator()(const T &x) const { return x; }
};



} // namespace tiny
