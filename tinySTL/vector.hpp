#pragma once
#include "allocator.hpp"
#include "construct.hpp"
#include "uninitialized.hpp"
#include "algorithm.hpp"
#include "iterator.hpp"
namespace tinySTL
{

template <typename T> //T 不能是const, volatile类型
class vector
{
public:
    typedef allocator<T> Alloc;

    typedef size_t size_type;
    typedef ptrdiff_t diff_type;
    typedef T value_type;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_pointer const_pointer;
    typedef typename Alloc::const_reference const_reference;

    // typedef pointer_iterator<pointer> iterator;
    // typedef pointer_iterator<const_pointer> const_iterator; //只读迭代器,而非迭代器不可变
    typedef pointer iterator;
    typedef const_pointer const_iterator; //只读迭代器,而非迭代器不可变

private:
    pointer _element;
    pointer _finish; //last element+1
    pointer _end;    //end of capacity
    // diff_type _size, _capacity;

public:
    //构造、析构、赋值
    vector();
    explicit vector(diff_type count, const_reference value);
    template <class Iter>
    vector(Iter begin, Iter end) : vector() { assign(begin, end); }
    vector(const vector &other);                 //拷贝构造函数
    vector<T> &operator=(const vector<T> &other) //拷贝赋值运算符
    {
        assign(other.begin(), other.end());
        return *this;
    }
    ~vector();

    void assign(diff_type count, const_reference value);
    template <class Iter>
    void assign(Iter begin, Iter end);

    //元素访问
    reference at(diff_type pos)
    {
        if (pos >= (_finish - _element))
            throw pos;
        return _element[pos];
    }
    reference operator[](diff_type pos) { return _element[pos]; }
    reference front() { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *(end() - 1); }
    pointer data() { return _element; }

    //迭代器
    iterator begin() { return iterator(_element); }
    iterator end() { return iterator(_finish); }
    const_iterator begin() const { return _element; }
    const_iterator end() const { return _finish; }

    //容量
    bool empty() const { return (_element == _finish); }
    diff_type size() const { return (_finish - _element); }
    size_type capacity() const { return (_end - _element); }

    //修改器
    void clear();
    void insert(iterator pos, diff_type count, const_reference value);
    iterator insert(iterator pos, const_reference value);
    iterator erase(iterator begin, iterator end);
    iterator erase(iterator pos);
    void push_back(const_reference value);
    void pop_back() { destroy(--_finish); }
};

template <typename T>
inline vector<T>::vector()
{
    _finish = _element = Alloc::allocate(10);
    _end = _element + 10;
}

template <typename T>
inline vector<T>::vector(diff_type count, const_reference value)
{
    _element = Alloc::allocate(count);
    _finish = _end = uninitialized_fill_n(_element, count, value);
}

template <typename T>
inline vector<T>::vector(const vector &other)
{
    _element = Alloc::allocate(other.size());
    _finish = _end = uninitialized_copy(other.begin(), other.end(), _element);
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
void vector<T>::assign(Iter begin, Iter end)
{
    diff_type newSize = end - begin;
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
        _finish = uninitialized_copy(mid, end, t);
    }
}

template <typename T>
void vector<T>::assign(diff_type count, const_reference value)
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
        diff_type mid = (_finish - _element); //拷贝前一半,构造后一半
        fill(_element, _finish, value);
        _finish = uninitialized_fill_n(_finish, count - mid, value);
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
void vector<T>::insert(iterator pos, diff_type count, const_reference value)
{
    if (count == 0)
        return;
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
        }
        else /*新插入元素个数较多, count > num_elem_after */
        {
            uninitialized_fill_n(old_end, count - num_elem_after, value);
            _finish = uninitialized_copy(pos /*.base()*/, old_end, pos /*.base()*/ + count);
            fill(pos /*.base()*/, old_end, value);
        }
    }
    else /*空间不足*/
    {
        diff_type newCapacity = (_finish - _element) + count;
        pointer newSpace = Alloc::allocate(newCapacity);
        pointer finish;
        finish = uninitialized_copy(_element, pos /*.base()*/, newSpace);
        finish = uninitialized_fill_n(finish, count, value); //构造count个value
        finish = uninitialized_copy(pos /*.base()*/, _finish, finish);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = newSpace;
        _end = _finish = finish;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, const_reference value)
{
    if (_finish < _end)
    { /* 空间还有 */
        if (pos == _finish)
        { /* 插入到末尾,相当于push_back */
            construct(pos, value);
            ++_finish;
            return pos;
        }
        else
        {
            iterator old_end = _finish;
            construct(old_end, *(old_end - 1)); //在end()处构造一个工具元素
            ++_finish;
            copy_backward(pos, old_end, _finish);
            *pos = value;
            return pos;
        }
    }
    else
    { /*空间不足*/
        diff_type newCapacity = capacity() * 3 / 2;
        iterator newSpace = Alloc::allocate(newCapacity);
        iterator finish;
        finish = uninitialized_copy(_element, pos, newSpace);
        construct(finish, value);
        uninitialized_copy(pos, _finish, finish + 1);
        destroy(begin(), _finish);
        Alloc::deallocate(_element);
        _element = newSpace;
        ++_finish;
        return finish;
    }
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(iterator start,
                                                     iterator finish)
{
    iterator new_end = copy(finish, _finish, start); // copy
    destroy(new_end, _finish);                       // delete
    _finish -= (finish - start);
    return start;
}

template <typename T>
inline typename vector<T>::iterator vector<T>::erase(iterator pos)
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
        size_t new_cap = capacity() * 3 / 2;
        pointer newelem = Alloc::allocate(new_cap);
        pointer newfinish = uninitialized_copy(_element, _finish, newelem);
        destroy(_element, _finish);
        Alloc::deallocate(_element);
        _element = newelem;
        _finish = newfinish;
        _end = _element + new_cap;
    }
    construct(_finish++, value);
}

} // namespace tinySTL