#pragma once

#include "allocator.hpp"
namespace tinySTL {

template <typename T>
class Vector {
   public:
    typedef T* Iterator;
    typedef const Iterator const_Iterator;
    typedef allocator<T> Alloc;

    //构造、析构、赋值
    Vector();
    explicit Vector(unsigned int count, const T& value);
    Vector(const Vector& other);
    ~Vector();

    Vector<T>& operator=(const Vector<T>& other);
    void assign(const_Iterator& begin, const_Iterator& end);
    void assign(unsigned int count, const T& value);

    //元素访问
    T& at(int pos) {
        if (pos < 0 || pos >= m_size) throw pos;
        return m_element[pos];
    }
    T& operator[](int pos) { return m_element[pos]; }
    T& front() { return *begin(); }
    T& back() { return m_element[m_size - 1]; }
    T* data() { return m_element; }

    //迭代器
    Iterator begin() { return m_element; }
    Iterator end() { return m_element + m_size; }
    const_Iterator begin() const { return m_element; }
    const_Iterator end() const { return m_element + m_size; }

    //容量
    bool empty() const { return m_size == 0; }
    int size() const { return m_size; }
    int capacity() const { return m_capacity; }

    //修改器
    void clear();
    Iterator insert(const Iterator& pos, const T& value);
    Iterator erase(const Iterator& pos);
    void push_back(const T& value);
    void pop_back();

   private:
    T* m_element;
    unsigned int m_size, m_capacity;
    void expand();
};



template <typename T>
inline Vector<T>::Vector() : m_size(0), m_capacity(10) {
    m_element = Alloc::allocate(m_capacity);
}

template <typename T>
inline Vector<T>::Vector(unsigned int count, const T& value)
    : m_size(count), m_capacity(count) {
    m_element = Alloc::allocate(count);
    Alloc::construct_n(m_element, count, value);
}

template <typename T>
inline Vector<T>::Vector(const Vector& other)
    : m_size(other.m_size), m_capacity(other.m_size) {
    m_element = Alloc::allocate(other.m_capacity);
    Alloc::construct_copy(other.begin, other.end, m_element);
}

template <typename T>
inline Vector<T>::~Vector() {
    clear();
    Alloc::deallocate(m_element);
}

template <typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    return assign(other.begin(), other.end());
}

template <typename T>
inline void Vector<T>::assign(const_Iterator& begin, const_Iterator& end) {
    unsigned int newSize = static_cast<unsigned int>(end - begin);
    clear();
    if (newSize > m_capacity) {
        Alloc::deallocate(m_element);
        m_element = Alloc::allocate(newSize);
    }
    Alloc::construct_copy(begin, end, m_element);
    m_size = newSize;
    m_capacity = newSize;
}

template <typename T>
inline void Vector<T>::assign(unsigned int count, const T& value) {
    clear();
    if (count > m_capacity) {
        Alloc::deallocate(m_element);
        m_element = Alloc::allocate(count);
    }
    Alloc::construct_n(m_element, count, value);
    m_size = count;
    m_capacity = count;
}

template <typename T>
inline void Vector<T>::expand() {
    unsigned int newCapacity = m_capacity * 3 / 2;
    Iterator newSpace = Alloc::allocate(newCapacity);
    Alloc::construct_copy(begin(), end(), newSpace);
    Alloc::destroy(begin(), end());
    Alloc::deallocate(m_element);
    m_capacity = newCapacity;
    m_element = newSpace;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(const_Iterator& pos, const T& value) {
    if (m_size == m_capacity) expand();
    for (Iterator cur = end(); cur > pos + 1; --cur) {
        Alloc::construct(cur, *(cur - 1));
        Alloc::destroy(cur + 1);
    }
    Alloc::construct(pos, value);
    ++m_size;
    return pos;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::erase(const_Iterator& pos) {
    if (pos < m_element || pos >= end()) throw pos;
    Alloc::destroy(pos);
    for (Iterator cur = pos + 1; cur < end(); ++cur) {
        Alloc::construct(cur - 1, *cur);
        Alloc::destroy(cur);
    }
    --m_size;
    return pos;
}

template <typename T>
void Vector<T>::clear() {
    for (Iterator cur = begin(); cur != end(); ++cur) Alloc::destroy(cur);
    m_size = 0;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (m_size == m_capacity) expand();
    Alloc::construct(m_element + m_size, value);
    ++m_size;
}
template <typename T>
void Vector<T>::pop_back() {
    Alloc::destroy(m_element + m_size - 1);
    --m_size;
}

}  // namespace tinySTL