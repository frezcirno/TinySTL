#pragma once
#include "Deque.hpp"

namespace tinySTL {

template <typename T, class Sequence = Deque<T> >
class Stack {
    friend bool operator==<>(const Stack&, const Stack&);
    friend bool operator<<>(const Stack&, const Stack&);

   public:
    Stack() {}
    Stack(unsigned int n, const T& value) : c(n, value) {}
    ~Stack() {}

    bool empty() const { return c.empty(); }
    unsigned int size() const { return c.size(); }

    T& top() { return c.back(); }
    const T& top() const { return c.back(); }

    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_back(); }

   private:
    Sequence c;
};

template <typename T, class Sequence>
inline bool operator==(const Stack<T, Sequence>& x,
                       const Stack<T, Sequence>& y) {
    return x.c == y.c;
}

template <typename T, class Sequence>
inline bool operator<(const Stack<T, Sequence>& x,
                      const Stack<T, Sequence>& y) {
    return x.c < y.c;
}

}  // namespace tinySTL