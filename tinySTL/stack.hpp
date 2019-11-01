#pragma once
#include "deque.hpp"

namespace tinySTL
{

template <typename T, class Sequence>
class stack
{
    // friend bool operator== /**/<>(const stack &, const stack &);
    // friend bool operator</**/<>(const stack &, const stack &);

public:
    stack(unsigned int n, const T &value) : c(n, value) {}

    bool empty() const { return c.empty(); }
    unsigned int size() const { return c.size(); }

    T &top() { return c.back(); }
    const T &top() const { return c.back(); }

    void push(const T &value) { c.push_back(value); }
    void pop() { c.pop_back(); }

private:
    Sequence c;
};

// template <typename T, class Sequence>
// inline bool operator==(const stack<T, Sequence> &x,
//                        const stack<T, Sequence> &y)
// {
//     return x.c == y.c;
// }

// template <typename T, class Sequence>
// inline bool operator<(const stack<T, Sequence> &x,
//                       const stack<T, Sequence> &y)
// {
//     return x.c < y.c;
// }
} // namespace tinySTL