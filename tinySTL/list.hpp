#pragma once
/**
 * 双向循环链表
 * 类STL<list>接口
 * 简化版本
 */
#include "allocator.hpp"
namespace tinySTL
{

/* Declartion */
template <typename T>
class list;

template <typename T>
struct _list_node
{
    T _data;
    _list_node<T> *_prev, *_next;
    _list_node(const T &value) : _data(value) {}
};

template <typename T>
class _list_iterator
{
public:
    typedef T value_type;
    typedef _list_node<T> *link_t;
    typedef _list_iterator<T> self;
    friend class list<T>;

private:
    link_t _node;

public:
    //构造、析构
    _list_iterator() {}
    // _list_iterator(link_t node) : _node(node) {}
    _list_iterator(const link_t node) : _node(node) {}
    _list_iterator(const _list_iterator &rhs) : _node(rhs._node) {}
    ~_list_iterator() {}
    self &operator=(const self &rhs)
    {
        this->_node = rhs._node;
        return *this;
    }

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

    const self operator++(int)
    {
        self old(*this);
        ++*this;
        return old;
    }

    const self operator--(int)
    {
        self old(*this);
        --*this;
        return old;
    }

    bool operator==(const self &rhs) const { return _node == rhs._node; }
    bool operator!=(const self &rhs) const { return _node != rhs._node; }
    T &operator*() { return _node->_data; }
    T *operator->() { return &(operator*()); }
};

template <typename T>
class list
{
public:
    typedef _list_node<T> node_type;
    typedef _list_iterator<T> iterator;
    typedef const _list_iterator<T> const_iterator;

public:
    //构造、析构、复制、初始化
    list();
    explicit list(int count, const T &value);
    list(const list<T> &other);
    ~list()
    {
        clear();
        allocator<node_type>::deallocate(_entry);
    }
    list<T> &operator=(const list<T> &other);
    void assign(iterator begin, iterator end);
    void assign(int count, const T &value);

    //元素访问
    T &front() { return *begin(); }
    T &back() { return *end()->_prev; }

    //迭代器
    iterator begin() { return iterator(_entry->_next); }
    iterator end() { return iterator(_entry); }
    const_iterator begin() const { return iterator(_entry->_next); }
    const_iterator end() const { return iterator(_entry); }

    //容量
    bool empty() { return _size == 0; }
    int size() { return _size; }

    //修改器
    void clear();
    iterator insert(iterator pos, const T &value);
    iterator insert(iterator pos, unsigned int count, const T &value);
    iterator erase(iterator pos);
    void push_back(const T &value) { insert(end(), value); }
    void pop_back() { erase(--end()); }
    void push_front(const T &value) { insert(begin(), value); }
    void pop_front() { erase(begin()); }

    //操作
    void remove(const T &value);

private:
    node_type *_entry; //由于是双向循环链表,仅需一个节点即可表示整个链表
    int _size;
};

template <typename T>
inline list<T>::list() : _size(0)
{
    _entry = allocator<node_type>::allocate(); //附加节点: 只分配空间,不构造元素
    _entry->_next = _entry->_prev = _entry;
}

template <typename T>
inline typename list<T>::iterator list<T>::insert(iterator pos,
                                                  const T &value)
{
    node_type *ret = new node_type(value);
    ret->_prev = pos._node->_prev;
    ret->_next = pos._node;
    pos._node->_prev->_next = ret;
    pos._node->_prev = ret;
    ++_size;
    return ret;
}
template <typename T>
inline typename list<T>::iterator list<T>::insert(iterator pos,
                                                  unsigned int count,
                                                  const T &value)
{
    while (--count)
        insert(pos, value);
}

template <typename T>
inline typename list<T>::iterator list<T>::erase(iterator pos)
{
    node_type *ret = pos._node->_next;
    pos._node->_prev->_next = pos._node->_next;
    pos._node->_next->_prev = pos._node->_prev;
    delete pos._node;
    --_size;
    return ret;
}

template <typename T>
inline void list<T>::clear()
{
    node_type *cur = _entry->_next;
    while (cur != _entry)
    {
        delete cur;
        cur = cur->_next;
    }
    _entry->_next = _entry;
    _entry->_prev = _entry;
    _size = 0;
}

template <typename T>
inline list<T>::list(int count, const T &value) : list()
{
    while (--count)
        this->push_back(value);
}

template <typename T>
inline list<T>::list(const list<T> &other) : list()
{
    assign(other.begin(), other.end());
}

template <typename T>
inline list<T> &list<T>::operator=(const list<T> &other)
{
    assign(other.begin(), other.end());
    return *this;
}

template <typename T>
void list<T>::assign(iterator begin, iterator end)
{
    clear();
    while (begin != end)
    {
        push_back(*begin);
        ++begin;
    }
}

template <typename T>
void list<T>::assign(int count, const T &value)
{
    clear();
    while (--count)
        push_back(value);
}

template <typename T>
void list<T>::remove(const T &value)
{
    for (iterator cur = begin(); cur != end();)
    {
        iterator p = cur;
        ++cur;
        if (*p == value)
        {
            erase(p);
            --_size;
        }
    }
}

} // namespace tinySTL