#pragma once
#include <list>
#include <vector>

#include "VectorIterator.h"
template <typename T>
class Vector {
   public:
    Vector() : m_element(new T[10]), m_size(0), m_capacity(10) {}
    ~Vector() {}

    void push_back(const T& elem) { m_element[m_size++] = T; }
    void pop_back() { --m_size; }

    typedef T* Iterator;
    Iterator begin() { return Iterator(m_element); }
    Iterator end() { return Iterator(m_element + m_size); }
    Iterator insert(Iterator iter, T& elem) {
        if (m_size == m_capacity) expand();
        for (Iterator it = end(); it > iter + 1; it--) {
            *it = *(it - 1);
        }
        *iter = elem;
        return iter;
    }
    Iterator erase(Iterator iter) {
        while (iter < end()) {
            *iter = *(iter + 1);
            iter++;
        }
        delete iter;
        *it = elem;
    }

    T& at(int pos) {
        if (pos < 0 || pos >= m_size) throw pos;
        return m_element[pos];
    }
    T& operator[](int pos) { return m_element[pos]; }

    bool empty() const { return m_size == 0; }
    int size() const { return m_size; }

   private:
    T* m_element;
    int m_size;
    int m_capacity;
};

int main(int argc, char const* argv[]) {
    std::vector<int> v;
    v.pop_back();
    std::list<int> l;

    return 0;
}
