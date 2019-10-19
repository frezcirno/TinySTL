#pragma once

#include "algorithm.hpp"
#include "allocator.hpp"
#include <utility>
namespace tinySTL
{

template <typename T>
class vector
{
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T &const_reference;

    typedef T *iterator;
    typedef const T *const_iterator; //只读迭代器

    typedef allocator<T> Alloc;

private:
    pointer _element;
    pointer _finish; //last element+1
    pointer _end;    //end of capacity
    // unsigned int _size, _capacity;

public:
    //构造、析构、赋值
    vector();
    ~vector();
    explicit vector(unsigned int count, const_reference value);
    vector(const vector &other);                 //拷贝构造函数
    vector<T> &operator=(const vector<T> &other) //拷贝赋值运算符
    {
        assign(other.begin(), other.end());
        return *this;
    }

    template <class Iter>
    void assign(Iter begin, Iter end);
    void assign(unsigned int count, const_reference value);

    //元素访问
    reference at(unsigned int pos)
    {
        if (pos >= (_finish - _element))
            throw pos;
        return _element[pos];
    }
    reference operator[](unsigned int pos) { return _element[pos]; }
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
    unsigned int size() const { return (_finish - _element); }
    int capacity() const { return (_end - _element); }

    //修改器
    void clear();
    void insert(const_iterator &pos, unsigned int count, const_reference value);
    iterator insert(const_iterator &pos, const_reference value);
    iterator erase(const_iterator &begin, const_iterator &end);
    iterator erase(const_iterator &pos);
    void push_back(const_reference value);
    void pop_back() { destroy(--_finish); }
};

template <typename T>
inline vector<T>::vector()
{
    _element = Alloc::allocate(10);
    _finish = _element;
    _end = _element + 10;
}

template <typename T>
inline vector<T>::vector(unsigned int count, const_reference value)
{
    _element = Alloc::allocate(count);
    _finish = _end = construct_n(_element, count, value);
}

template <typename T>
inline vector<T>::vector(const vector &other)
{
    _element = Alloc::allocate(other.size());
    _finish = _end = construct_copy(other.begin, other.end, _element);
}

template <typename T>
inline vector<T>::~vector()
{
    clear();
    Alloc::deallocate(_element);
}

template <typename T>
template <class Iter>
void vector<T>::assign(Iter begin, Iter end)
{
    unsigned int newSize = end - begin;
    if (_element + newSize > _end) //1.空间不足
    {
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = Alloc::allocate(newSize);
        _end = _finish = copy(begin, end, _element);
    }
    else if (_element + newSize <= _finish) //2.新元素比原来的元素少或一样多,自赋值
    {
        pointer t = copy(begin, end, _element); //拷贝前一半,析构后一半
        destroy(t, _finish);
        _finish = t;
    }
    else //3.新元素比原来的元素多
    {
        Iter mid = begin + (_finish - _element); //拷贝前一半,构造后一半
        pointer t = copy(begin, mid, _element);
        _finish = construct_copy(mid, end, t);
    }
}

template <typename T>
void vector<T>::assign(unsigned int count, const_reference value)
{
    if (_element + count > _end) //1.空间不足
    {
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = Alloc::allocate(count);
        _end = _finish = fill_n(_element, count, value);
    }
    else if (_element + count <= _finish) //2.新元素比原来的元素少或一样多
    {
        pointer t = fill_n(_element, count, value); //拷贝前一半,析构后一半
        destroy(t, _finish);
        _finish = t;
    }
    else //3.新元素比原来的元素多
    {
        unsigned int mid = (_finish - _element); //拷贝前一半,构造后一半
        fill(_element, _finish, value);
        _finish = construct_n(_finish, count - mid, value);
    }
}

template <typename T>
inline void vector<T>::clear()
{
    if (_element)
    {
        destroy(_element, _finish);
        _finish = _element;
    }
}

template <typename T>
void vector<T>::insert(const_iterator &pos, unsigned int count,
                       const_reference value)
{
    if (count == 0)
        return;
    if (_capacity > _size + count)
    {
        /*空间尚且足够*/
        unsigned int num_elem_after = end() - pos;
        iterator old_end = end();
        if (num_elem_after > count)
        {
            /*新插入元素个数少于后面的元素个数*/
            //构造在vector末尾新增的count个元素(以原来的结尾为蓝本)
            construct_copy(old_end - count, old_end, old_end);
            _size += count;
            //拷贝[pos+count,end-count)的元素
            copy_backward(pos, old_end - count, old_end);
            fill(pos, pos + count, value);
        }
        else
        {
            /*新插入元素个数较多*/
            construct_n(old_end, count - num_elem_after, value);
            construct_copy(pos, old_end, pos + count);
            _size += count;
            fill(pos, old_end, value);
        }
    }
    else
    {
        /*空间不足*/
        unsigned int newCapacity = _capacity * 3 / 2;
        iterator newSpace = Alloc::allocate(newCapacity);
        iterator finish;
        finish = construct_copy(begin(), pos, newSpace);
        finish = construct_n(finish, count, value); //构造count个value
        finish = construct_copy(pos, end(), finish);
        destroy(begin(), end());
        Alloc::deallocate(_element);
        _element = newSpace;
        _size += count;
        _capacity = newCapacity;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(const_iterator &pos,
                                               const_reference value)
{
    if (_finish < _end)
    { /* 空间还有 */
        if (pos == end())
        { /* 插入到末尾,相当于push_back */
            construct(pos, value);
            ++_finish;
            return pos;
        }
        else
        {
            iterator old_end = end();
            construct(old_end, *(old_end - 1)); //在end()处构造一个工具元素
            ++_finish;
            copy_backward(pos, old_end, old_end + 1);
            *pos = value;
            return pos;
        }
    }
    else
    { /*空间不足*/
        unsigned int newCapacity = _capacity * 3 / 2;
        iterator newSpace = Alloc::allocate(newCapacity);
        iterator finish;
        finish = construct_copy(begin(), pos, newSpace);
        construct(finish, value);
        construct_copy(pos, end(), finish + 1);
        destroy(begin(), end());
        Alloc::deallocate(_element);
        _element = newSpace;
        ++_finish;
        _capacity = newCapacity;
        return finish;
    }
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(const_iterator &start,
                                                     const_iterator &finish)
{
    iterator new_end = copy(finish, _finish, start); // copy
    destroy(new_end, _finish);                       // delete
    _finish -= (finish - start);
    return start;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(const_iterator &pos)
{
    copy(pos + 1, _finish, pos); // copy
    destroy(--_finish);          // delete the last element
    return pos;
}

template <typename T>
void vector<T>::push_back(const_reference value)
{
    if (_finish == _end)
    {
        pointer newelem = Alloc::allocate((_end - _element) * 3 / 2);
        pointer newfinish = construct_copy(_element, _end, newelem);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = newelem;
        _finish = newfinish;
    }
    construct(_finish++, value);
}

} // namespace tinySTL