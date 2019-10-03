#pragma once
#include "Deque.hpp"

namespace tinySTL {

template <typename T, class Sequence = Deque<T> >
class Queue;

template <typename T, class Sequence>
inline bool operator==(const Queue<T, Sequence>& x,
                       const Queue<T, Sequence>& y) {
    return x.c == y.c;
}

template <typename T, class Sequence>
inline bool operator<(const Queue<T, Sequence>& x,
                      const Queue<T, Sequence>& y) {
    return x.c < y.c;
}

template <typename T, class Sequence>
class Queue {
    friend bool operator== <>(const Queue&, const Queue&);
    friend bool operator< <>(const Queue&, const Queue&);

   public:
    Queue() {}
    Queue(unsigned int n, const T& value) : c(n, value) {}
    ~Queue() {}

    bool empty() const { return c.empty(); }
    unsigned int size() const { return c.size(); }

    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_front(); }

    T& front() { return c.front(); }
    const T& front() const { return c.front(); }
    T& back() { return c.back(); }
    const T& back() const { return c.back(); }

   private:
    Sequence c;
};

}  // namespace tinySTL