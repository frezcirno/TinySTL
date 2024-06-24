#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include "allocator.h"
#include "uninitialized.h"
//#include "type_traits.h"
#include "construct.h"

namespace tiny
{

/* Forward Declaration */
template <typename T>
class list;

template <typename T>
struct _list_node
{
    T _data;
    _list_node<T> *_prev = nullptr, *_next = nullptr;
    _list_node(const T &value) : _data(value) {}
};

template <typename T>
class _list_iterator
{
public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef int diff_type;

    typedef _list_iterator<value_type> self;

    typedef _list_node<value_type> node_type;
    typedef node_type *link_type;
    friend class list<value_type>;

private:
    link_type _node;

public:
    _list_iterator() {} //构造
    _list_iterator(link_type node) : _node(node) /*默认复制构造*/ {}
    _list_iterator(const self &rhs) : _node(rhs._node) {} //复制

    self &operator=(const self &rhs)
    {
        this->_node = rhs._node;
        return *this;
    }
    ~_list_iterator() {} //析构
    reference &operator*() { return _node->_data; }
    pointer operator->() { return &(operator*()); }

    self &operator++()
    {
        _node = _node->_next;
        return *this;
    }
    self &operator--()
    {
        _node = _node->_prev;
        return *this;
    }
    self operator++(int)
    {
        self old(*this);
        ++*this;
        return old;
    }
    self operator--(int)
    {
        self old(*this);
        --*this;
        return old;
    }
    // diff_type operator-(const self &rhs); //迭代器减法
    friend bool operator==(const self &lhs, const self &rhs) { return lhs._node == rhs._node; }
    friend bool operator!=(const self &lhs, const self &rhs) { return lhs._node != rhs._node; }
};

//T 必须不能是const类型
template <typename T>
class list
{
public:
    typedef allocator<T> Alloc;

    typedef size_t size_type;
    typedef ptrdiff_t diff_type;
    typedef T value_type;
    typedef typename Alloc::pointer pointer;
    typedef typename Alloc::const_pointer const_pointer;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_reference const_reference;

    typedef _list_node<T> node_type;
    typedef node_type *node_pointer;
    typedef allocator<node_type> node_alloc;

    typedef _list_iterator<T> iterator;
    // typedef _list_iterator<const T> const_iterator;

    typedef list<T> self;

private:
    node_pointer create_node(const_reference value)
    {
        node_pointer ptr = node_alloc::allocate();
        construct(ptr, value);
        return ptr;
    }
    void delete_node(node_pointer ptr)
    {
        destroy(ptr);
        node_alloc::deallocate(ptr);
    }

public:
    //构造、析构、复制、初始化
    list();
    explicit list(size_type count, const_reference value) : list() { assign(count, value); }
    template <class Iter>
    list(Iter begin, Iter end) : list() { assign(begin, end); }
    list(const self &other) : list() { assign(other.begin(), other.end()); }
    self &operator=(const self &other)
    {
        assign(other.begin(), other.end());
        return *this;
    }
    ~list()
    {
        clear();
        node_alloc::deallocate(_entry); //_entry只分配内存不构造元素
    }

    template <class Iter>
    void assign(Iter begin, Iter end);
    void assign(size_type count, const_reference value);

    //元素访问
    reference front() { return *begin(); }
    reference back() { return *end()->_prev; }

    //迭代器
    iterator begin() { return iterator(_entry->_next); }
    iterator end() { return iterator(_entry); }
    //暂时未实现只读(const)迭代器
    iterator begin() const { return iterator(_entry->_next); }
    iterator end() const { return iterator(_entry); }

    //容量
    bool empty() { return _size == 0; }
    size_type size() { return _size; }

    //修改器
    void clear();
    iterator insert(iterator pos, const_reference value);
    iterator insert(iterator pos, size_type count, const_reference value);
    template <class Iter>
    iterator insert(iterator pos, Iter begin, Iter end);
    iterator erase(iterator pos);
    iterator erase(iterator begin, iterator end);

    void push_back(const_reference value) { insert(end(), value); }
    void pop_back()
    {
        iterator tmp = end();
        erase(--tmp);
    }
    void push_front(const_reference value) { insert(begin(), value); }
    void pop_front() { erase(begin()); }

    //操作
    void remove(const_reference value);

private:
    node_pointer _entry; //由于是双向循环链表,仅需一个节点即可表示整个链表
    size_type _size;
};

template <typename T>
inline list<T>::list() : _size(0)
{
    _entry = node_alloc::allocate(); //附加节点: 只分配空间,不构造元素
    _entry->_next = _entry->_prev = _entry;
}

template <typename T>
inline typename list<T>::iterator list<T>::insert(iterator pos, const_reference value)
{
    ++_size;
    node_pointer ret = create_node(value);
    ret->_prev = pos._node->_prev;
    ret->_next = pos._node;
    pos._node->_prev->_next = ret;
    pos._node->_prev = ret;
    return ret;
}

template <typename T>
template <class Iter>
typename list<T>::iterator list<T>::insert(iterator pos, Iter start, Iter finish)
{
    node_pointer newnode = pos._node->_prev;
    while (start != finish)
    {
        newnode->_next = create_node(*start);
        newnode->_next->_prev = newnode;
        newnode = newnode->_next;
        ++start;
        ++_size;
    }
    newnode->_next = pos._node;
    pos._node->_prev = newnode;
    return pos;
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos, size_type count, const_reference value)
{
    _size += count;
    node_pointer newnode = pos._node->_prev;
    while (count > 0)
    {
        newnode->_next = create_node(value);
        newnode->_next->_prev = newnode;
        newnode = newnode->_next;
        --count;
    }
    newnode->_next = pos._node;
    pos._node->_prev = newnode;
}

template <typename T>
inline typename list<T>::iterator list<T>::erase(iterator pos)
{
    --_size;
    node_pointer ret = pos._node->_next;
    pos._node->_prev->_next = pos._node->_next;
    pos._node->_next->_prev = pos._node->_prev;
    delete_node(pos._node);
    return iterator(ret);
}

template <typename T>
inline typename list<T>::iterator list<T>::erase(iterator start, iterator finish)
{
    iterator begin1 = start;
    --begin1;
    begin1._node->_next = finish._node;
    finish._node->_prev = begin1._node;
    while (start != finish)
    {
        --_size;
        node_pointer tmp = start._node;
        ++start;
        destroy(tmp);
        node_alloc::deallocate(tmp);
    }
    return finish;
}

template <typename T>
inline void list<T>::clear()
{
    _size = 0;
    iterator cur = begin();
    while (cur != end())
    {
        node_pointer tmp = cur._node;
        ++cur;
        delete_node(tmp);
    }
    _entry->_next = _entry;
    _entry->_prev = _entry;
}

template <typename T>
template <class Iter>
void list<T>::assign(Iter start, Iter finish)
{
    iterator cur = begin();
    for (; cur != end() && start != finish; ++cur, ++start)
        *cur = *start; //替换已存在节点
    if (start != finish)
        insert(end(), start, finish); //新节点个数多于原来节点个数,添加之
    else
        erase(cur, end()); //新节点个数少于或等于原来节点个数,删除之
}

template <typename T>
void list<T>::assign(size_type count, const_reference value)
{
    iterator cur = begin();
    for (; cur != end() && count > 0; ++cur, --count)
        *cur = value;
    if (count > 0)
        insert(end(), count, value); //新节点个数多于原来节点个数,添加之
    else
        erase(cur, end()); //新节点个数少于或等于原来节点个数,删除之
}

template <typename T>
void list<T>::remove(const_reference value)
{
    for (iterator cur = begin(); cur != end();)
    {
        if (*cur != value)
            ++cur;
        else
        {
            iterator p = cur;
            ++cur;
            erase(p);
        }
    }
}

} // namespace tiny