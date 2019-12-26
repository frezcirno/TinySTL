#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstddef>
#include "algorithm.h"
#include "allocator.h"
#include "construct.h"
// #include "pointer_iterator.hpp"
#include "uninitialized.h"
#include "utility.h"

namespace tinySTL
{
template <typename T>  // T 不能是const, volatile类型
class vector
{
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;

    typedef size_t size_type;
    typedef ptrdiff_t diff_type;

    typedef pointer iterator;
    typedef const_pointer const_iterator;  //只读迭代器,而非迭代器不可变

  private:
    typedef allocator<T> Alloc;

  private:
    pointer _element;
    pointer _finish;  // last element+1
    pointer _end;     // end of capacity

  public:
    //构造、析构、赋值
    vector() : _element(nullptr), _finish(nullptr), _end(nullptr) {}
    explicit vector(size_type count);
    explicit vector(size_type count, const_reference value);

    template <class Iter> vector(Iter begin, Iter end);

    vector(const vector &other);  //拷贝构造函数

    vector<T> &operator=(const vector<T> &other)  //拷贝赋值运算符
    {
        assign(other.begin(), other.end());
        return *this;
    }
    ~vector();

    void assign(diff_type count, const_reference value);

    template <class Iter>
    void assign(Iter begin, Iter end);

    void reserve(size_type new_capacity);
    void resize(size_type new_size);
    //元素访问
    reference at(size_type pos)
    {
        if (pos >= size()) throw pos;
        return _element[pos];
    }
    reference operator[](size_type pos) { return _element[pos]; }
    const_reference operator[](size_type pos) const { return _element[pos]; }
    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *(end() - 1); }
    pointer data() { return _element; }

    //迭代器
    iterator begin() { return _element; }
    iterator end() { return _finish; }
    const_iterator begin() const { return _element; }
    const_iterator end() const { return _finish; }

    //容量
    bool empty() const { return (_element == _finish); }
    size_type size() const { return (_finish - _element); }
    size_type capacity() const { return (_end - _element); }

    //修改器
    void clear();
    void insert(iterator pos, diff_type count, const_reference value);
    iterator insert(iterator pos, const_reference value);
    iterator erase(iterator begin, iterator end);
    iterator erase(iterator pos);
    void push_back(const_reference value);
    void pop_back() { destroy(--_finish); }
    iterator find(const_reference value)
    {
        return tinySTL::find(_element, _end, value);
    }
};

template <typename T>
inline vector<T>::vector(size_type capacity)
{
    _finish = _element = Alloc::allocate(capacity);
    _end = _element + capacity;
}

template <typename T>
inline vector<T>::vector(size_type count, const_reference value)
{
    _element = Alloc::allocate(count);
    _finish = _end = tinySTL::uninitialized_fill_n(_element, count, value);
}

template <typename T>
inline vector<T>::vector(const vector &other)
{
    _element = Alloc::allocate(other.size());
    _finish = _end = tinySTL::uninitialized_copy(other.begin(), other.end(), _element);
}

template <typename T>
template <class Iter>
vector<T>::vector(Iter start, Iter finish)
{
    _element = Alloc::allocate(finish - start);
    _finish = _end = tinySTL::uninitialized_copy(start, finish, _element);
}

template <typename T>
inline vector<T>::~vector()
{
    clear();
    Alloc::deallocate(_element);
    _end = _finish = _element = nullptr;
}

template <typename T>
template <class Iter>
void vector<T>::assign(Iter start, Iter finish)
{
    size_type newSize = finish - start;
    if (newSize > capacity())  // 1.空间不足
    {
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = Alloc::allocate(newSize);
        _end = _finish = copy(start, finish, _element);
    } else if (newSize <= size())  // 2.新元素比原来的元素少或一样多,自赋值
    {
        pointer t = copy(start, finish, _element);  //拷贝前一半,析构后一半
        destroy(t, _finish);
        _finish = t;
    } else  // 3.新元素比原来的元素多
    {
        Iter mid = tinySTL::advance(start, size());  //拷贝前一半,构造后一半
        pointer t = copy(start, mid, _element);
        _finish = uninitialized_copy(mid, finish, t);
    }
}

template <typename T>
void vector<T>::assign(diff_type count, const_reference value)
{
    if (count > capacity())  // 1.空间不足
    {
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = Alloc::allocate(count);
        _end = _finish = fill_n(_element, count, value);
    } else if (count <= size())  // 2.新元素比原来的元素少或一样多
    {
        pointer t = fill_n(_element, count, value);  //拷贝前一半,析构后一半
        destroy(t, _finish);
        _finish = t;
    } else  // 3.新元素比原来的元素多
    {
        diff_type mid = size();  //拷贝前一半,构造后一半
        fill(_element, _finish, value);
        _finish = tinySTL::uninitialized_fill_n(_finish, count - mid, value);
    }
}

template <typename T>
void vector<T>::reserve(size_type new_capacity)
{
    size_type old_size = size();
    if (new_capacity > capacity()) {  // reserve不能使容器变小
        T *new_elem = Alloc::allocate(new_capacity);
        tinySTL::uninitialized_copy(_element, _finish, new_elem);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = new_elem;
        _finish = new_elem + old_size;
        _end = new_elem + new_capacity;
    }
}

template <typename T>
void vector<T>::resize(size_type new_size)
{
    size_type old_size = size();
    if (new_size > old_size) {  //增大
        if (new_size > capacity()) reserve(new_size);
        _finish =
            tinySTL::uninitialized_fill_default(_finish, new_size - old_size);
    } else if (new_size < old_size) {  //缩小
        destroy(_element + new_size, _end);
        _end = _element + new_size;
    }
}

template <typename T>
inline void vector<T>::clear()
{
    if (_element) {
        destroy(_element, _finish);
        _finish = _element;
    }
}

template <typename T>
void vector<T>::insert(iterator pos, diff_type count, const_reference value)
{
    if (count == 0) return;
    if (_finish + count < _end) /*空间尚且足够*/
    {
        diff_type num_elem_after = end() - pos;
        pointer old_end = _finish;
        if (num_elem_after > count) /*新插入元素个数少于后面的元素个数*/
        {
            //构造在vector末尾新增的count个元素(以原来的结尾为蓝本)
            _finish = uninitialized_copy(old_end - count, old_end, old_end);
            //拷贝[pos+count,end-count)的元素
            copy_backward(pos /*.base()*/, old_end - count, old_end);
            fill_n(pos, count, value);
        } else /*新插入元素个数较多, count > num_elem_after */
        {
            uninitialized_fill_n(old_end, count - num_elem_after, value);
            _finish = uninitialized_copy(
                pos /*.base()*/, old_end, pos /*.base()*/ + count);
            fill(pos /*.base()*/, old_end, value);
        }
    } else /*空间不足*/
    {
        diff_type newCapacity = (_finish - _element) + count;
        pointer newSpace = Alloc::allocate(newCapacity);
        pointer finish;
        finish = uninitialized_copy(_element, pos /*.base()*/, newSpace);
        finish = uninitialized_fill_n(finish, count, value);  //构造count个value
        finish = uninitialized_copy(pos /*.base()*/, _finish, finish);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = newSpace;
        _end = _finish = finish;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
    const_reference value)
{
    if (_element == nullptr) {  //重定向位置
        _element = Alloc::allocate(2);
        _finish = _element;
        _end = _element + 2;
        pos = _element;
    }
    if (_finish < _end) { /* 空间还有 */
        copy_backward(pos, _finish, _finish + 1);
        construct(pos, value);
        ++_finish;
        return pos;
    } else { /*空间不足*/
        diff_type new_capacity = capacity() * 3 / 2;
        iterator new_space = Alloc::allocate(new_capacity);
        iterator ret = uninitialized_copy(_element, pos, new_space);
        construct(ret, value);
        pointer newfinish = uninitialized_copy(pos, _finish, ret + 1);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = new_space;
        _end = _element + new_capacity;
        _finish = newfinish;
        return ret;
    }
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(iterator start,
    iterator finish)
{
    iterator new_end = copy(finish, _finish, start);  // copy
    destroy(new_end, _finish);                        // delete
    _finish -= (finish - start);
    return start;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(iterator pos)
{
    copy(pos + 1, _finish, pos);  // copy
    destroy(--_finish);           // delete the last element
    return pos;
}

template <typename T>
void vector<T>::push_back(const_reference value)
{
    if (_finish == _end) {
        if (capacity() == 0) {
            _element = Alloc::allocate(2);  //此处应该大于1, 否则无法增大
            _finish = _element;
            _end = _element + 2;  //此处应该大于1, 否则无法增大
        } else {
            size_t new_cap = capacity() * 3 / 2;
            pointer newelem = Alloc::allocate(new_cap);
            pointer newfinish =
                tinySTL::uninitialized_copy(_element, _finish, newelem);
            destroy(_element, _finish);
            Alloc::deallocate(_element);
            _element = newelem;
            _finish = newfinish;
            _end = _element + new_cap;
        }
    }
    construct(_finish++, value);
}

}  // namespace tinySTL