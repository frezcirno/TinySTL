#pragma once
#include "deque.hpp"

namespace tinySTL
{

/* Forward Declaraion */
template <typename T, class Sequence = deque<T>>
class queue;

template <typename T, class Sequence>
inline bool operator==(const queue<T, Sequence> &x,
                       const queue<T, Sequence> &y)
{
    return x.c == y.c;
}

template <typename T, class Sequence>
inline bool operator<(const queue<T, Sequence> &x,
                      const queue<T, Sequence> &y)
{
    return x.c < y.c;
}

template <typename T, class Sequence>
class queue
{
    friend bool operator==<>(const queue &, const queue &);
    friend bool operator</**/<>(const queue &, const queue &);

public:
    queue() {}
    queue(unsigned int n, const T &value) : c(n, value) {}
    ~queue() {}

    bool empty() const { return c.empty(); }
    unsigned int size() const { return c.size(); }

    void push(const T &value) { c.push_back(value); }
    void pop() { c.pop_front(); }

    T &front() { return c.front(); }
    const T &front() const { return c.front(); }
    T &back() { return c.back(); }
    const T &back() const { return c.back(); }

private:
    Sequence c;
};

} // namespace tinySTL