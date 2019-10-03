#pragma once

#include "algorithm.hpp"
#include "allocator.hpp"
namespace tinySTL {

template <typename T>
class Vector {
   public:
    typedef T* Iterator;
    typedef allocator<T> Alloc;

    //构造、析构、赋值
    Vector();
    explicit Vector(unsigned int count, const T& value);
    Vector(const Vector& other);
    ~Vector();

    Vector<T>& operator=(const Vector<T>& other) {
        return assign(other.begin(), other.end());
    }
    void assign(const Iterator& begin, const Iterator& end);
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
    const Iterator begin() const { return m_element; }
    const Iterator end() const { return m_element + m_size; }

    //容量
    bool empty() const { return m_size == 0; }
    int size() const { return m_size; }
    int capacity() const { return m_capacity; }

    //修改器
    void clear() { erase(begin(), end()); }
    void insert(const Iterator& pos, unsigned int count, const T& value);
    Iterator insert(const Iterator& pos, const T& value);
    Iterator erase(const Iterator& begin, const Iterator& end);
    Iterator erase(const Iterator& pos);
    void push_back(const T& value) { insert(end(), value); }
    void pop_back() { destroy(m_element + (--m_size) - 1); }

   private:
    T* m_element;
    unsigned int m_size, m_capacity;
};

template <typename T>
inline Vector<T>::Vector() : m_size(0), m_capacity(10) {
    m_element = Alloc::allocate(m_capacity);
}

template <typename T>
inline Vector<T>::Vector(unsigned int count, const T& value)
    : m_size(count), m_capacity(count) {
    m_element = Alloc::allocate(count);
    construct_n(m_element, count, value);
}

template <typename T>
inline Vector<T>::Vector(const Vector& other)
    : m_size(other.m_size), m_capacity(other.m_size) {
    m_element = Alloc::allocate(other.m_capacity);
    construct_copy(other.begin, other.end, m_element);
}

template <typename T>
inline Vector<T>::~Vector() {
    clear();
    Alloc::deallocate(m_element);
}

template <typename T>
inline void Vector<T>::assign(const Iterator& begin, const Iterator& end) {
    unsigned int newSize = static_cast<unsigned int>(end - begin);
    clear();
    if (newSize > m_capacity) {
        Alloc::deallocate(m_element);
        m_element = Alloc::allocate(newSize);
        m_capacity = newSize;
    }
    construct_copy(begin, end, m_element);
    m_size = newSize;
}

template <typename T>
inline void Vector<T>::assign(unsigned int count, const T& value) {
    clear();
    if (count > m_capacity) {
        Alloc::deallocate(m_element);
        m_element = Alloc::allocate(count);
        m_capacity = count;
    }
    construct_n(m_element, count, value);
    m_size = count;
}

template <typename T>
void Vector<T>::insert(const Iterator& pos, unsigned int count,
                       const T& value) {
    if (count == 0) return;
    if (m_capacity > m_size + count) {
        /*空间尚且足够*/
        unsigned int num_elem_after = end() - pos;
        Iterator old_end = end();
        if (num_elem_after > count) {
            /*新插入元素个数少于后面的元素个数*/
            //构造在vector末尾新增的count个元素(以原来的结尾为蓝本)
            construct_copy(old_end - count, old_end, old_end);
            m_size += count;
            //拷贝[pos+count,end-count)的元素
            copy_backward(pos, old_end - count, old_end);
            fill(pos, pos + count, value);
        } else {
            /*新插入元素个数较多*/
            construct_n(old_end, count - num_elem_after, value);
            construct_copy(pos, old_end, pos + count);
            m_size += count;
            fill(pos, old_end, value);
        }
    } else {
        /*空间不足*/
        unsigned int newCapacity = m_capacity * 3 / 2;
        Iterator newSpace = Alloc::allocate(newCapacity);
        Iterator finish;
        finish = construct_copy(begin(), pos, newSpace);
        finish = construct_n(finish, count, value);  //构造count个value
        finish = construct_copy(pos, end(), finish);
        destroy(begin(), end());
        Alloc::deallocate(m_element);
        m_element = newSpace;
        m_size += count;
        m_capacity = newCapacity;
    }
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::insert(const Iterator& pos,
                                               const T& value) {
    if (m_size < m_capacity) {
        /*空间还有*/
        if (pos == end()) {
            /*空表*/
            construct(pos, value);
            ++m_size;
            return pos;
        } else {
            Iterator old_end = end();
            construct(old_end, *(old_end - 1));  //在end()处构造一个工具元素
            ++m_size;
            copy_backward(pos, old_end, old_end + 1);
            *pos = value;
            return pos;
        }
    } else {
        /*空间不足*/
        unsigned int newCapacity = m_capacity * 3 / 2;
        Iterator newSpace = Alloc::allocate(newCapacity);
        Iterator finish;
        finish = construct_copy(begin(), pos, newSpace);
        construct(finish, value);
        construct_copy(pos, end(), finish + 1);
        destroy(begin(), end());
        Alloc::deallocate(m_element);
        m_element = newSpace;
        ++m_size;
        m_capacity = newCapacity;
        return finish;
    }
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(const Iterator& start,
                                                     const Iterator& finish) {
    // copy
    Iterator new_end = copy(finish, end(), start);
    // delete
    destroy(new_end, finish);
    m_size -= finish - start;
    return start;
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::erase(const Iterator& pos) {
    // copy
    copy(pos + 1, end(), pos);
    // delete the last element
    --m_size;
    destroy(end());
    return pos;
}

}  // namespace tinySTL