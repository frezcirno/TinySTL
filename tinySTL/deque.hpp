#pragma once
#include <cstring>
#include "allocator.hpp"
namespace tinySTL
{

/**用来计算deque中每个buffer内部的元素个数 */
inline unsigned int _buffer_size(unsigned int buf_siz, unsigned int elem_siz)
{
    return (buf_siz != 0) ? (buf_siz)
                          : ((elem_siz < 512) ? (512 / elem_siz) : 1);
}

template <typename T, unsigned int buf_siz>
class _DequeIterator
{
public:
    typedef T value_type;
    typedef _DequeIterator<T, buf_siz> self;

public:
    T *cur;       //指向当前元素
    T *_start;    //当前buffer开头(前闭后开)
    T *_finish;   //当前buffer结尾(前闭后开)
    T **map_node; //指向中控器的元素

private:
    static unsigned int buffer_size()
    {
        static int _buf_siz = _buffer_size(buf_siz, sizeof(T));
        return _buf_siz;
    }

public:
    _DequeIterator() {}
    ~_DequeIterator() {}
    void jump(T **buf_pos)
    {
        map_node = buf_pos;
        _start = *buf_pos;
        _finish = *buf_pos + buffer_size();
    }
    T &operator*() const { return *cur; }
    T *operator->() const { return &(operator*()); }

    unsigned int operator-(const self &x) const
    {
        return buffer_size() * (map_node - x.map_node - 1) + (cur - _start) +
               (x._finish - x.cur);
    }

    self &operator++()
    {
        ++cur;
        if (cur == _finish)
        {
            jump(map_node + 1);
            cur = _start;
        }
        return *this;
    }

    self operator++(int)
    {
        self old(*this);
        ++*this;
        return old;
    }

    self &operator--()
    {
        if (cur == _start)
        {
            jump(map_node - 1);
            cur = _finish;
        }
        --cur;
        return *this;
    }

    self operator--(int)
    {
        self old(*this);
        --*this;
        return old;
    }

    self &operator+=(int n)
    { //可正可负
        int index = n + (cur - _start);
        if (index >= 0 && index < buffer_size())
            cur += n;
        else
        {
            int buf_offset = index > 0 ? index / buffer_size()
                                       : (-index - 1) / buffer_size() - 1;
            jump(map_node + buf_offset);
            cur = index - buf_offset * buffer_size() + _start;
        }
        return *this;
    }

    self &operator-=(int n) { return *this += -n; }

    const self operator+(int n)
    {
        self tmp = *this;
        return tmp += n;
    }

    const self operator-(int n) const
    {
        self tmp = *this;
        return tmp += -n;
    }

    T &operator[](unsigned int n) { return *(*this + n); }
    bool operator==(const self &x) const { return cur == x.cur; }
    bool operator!=(const self &x) const { return cur != x.cur; }
    bool operator<(const self &x) const
    {
        return (map_node == x.map_node) ? (cur < x.cur)
                                        : (map_node < x.map_node);
    }
    bool operator>(const self &x) const
    {
        return (map_node == x.map_node) ? (cur > x.cur)
                                        : (map_node > x.map_node);
    }
};

template <typename T, unsigned int buf_siz = 0>
class deque
{
public:
    typedef _DequeIterator<T, buf_siz> iterator;
    typedef allocator<T> Alloc;

    deque(unsigned int n = 0);
    deque(unsigned int n, const T &value);
    ~deque();

    iterator begin() { return _start; }
    iterator end() { return _finish; }
    T &front() { return *_start; }
    T &back() { return *(_finish - 1); }

    void clear();
    void push_back(const T &value);
    void push_front(const T &value);
    void pop_back();
    void pop_front();
    iterator insert(const iterator &pos, const T &value);
    iterator erase(const iterator &pos);

    unsigned int size() const { return _finish - _start; }
    bool empty() const { return _finish == _start; }

private:
    T **_map; //中控器
    iterator _start;
    iterator _finish;
    unsigned int _capacity; //buffer的数量

private:
    static unsigned int buffer_size()
    {
        static int _buf_siz = _buffer_size(buf_siz, sizeof(T));
        return _buf_siz;
    }
    void expand_map(unsigned int num_new_buf = 1,
                    bool at_front = false); //空间扩充算法
};

template <typename T, unsigned int buf_siz>
deque<T, buf_siz>::deque(unsigned int n)
    : _start(), _finish(), _map(0), _capacity(0)
{
    unsigned int num_buf = n / buffer_size() + 1;
    // map创建
    _capacity = num_buf + 2; //前后各留一个buffer的余量
    if (_capacity < 8)
        _capacity = 8; //最少8个buffer
    _map = new T *[_capacity];
    // buffer创建
    T **nstart = _map + (_capacity - num_buf) / 2; // 元素居中
    T **nfinish = nstart + num_buf - 1;            //当n刚好整除时,nfinish独占一个buffer
    for (T **cur_buf = nstart; cur_buf <= nfinish; ++cur_buf)
        *cur_buf = Alloc::allocate(buffer_size());
    //迭代器设置
    _start.jump(nstart);
    _start.cur = _start._start;
    _finish.jump(nfinish);
    _finish.cur = _finish._start + n % buffer_size();
}

template <typename T, unsigned int buf_siz>
deque<T, buf_siz>::deque(unsigned int n, const T &value) : deque(n)
{
    //元素构造
    for (T **cur_buf = _start.map_node; cur_buf < _finish.map_node; ++cur_buf)
        construct_n(*cur_buf, buffer_size(), value);
    construct_range(_finish._start, _finish.cur, value);
}

template <typename T, unsigned int buf_siz>
deque<T, buf_siz>::~deque()
{
    clear();
    delete[] _start._start;
    delete[] _map;
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::expand_map(unsigned int num_new_buf, bool at_front)
{
    unsigned int old_size = _finish.map_node - _start.map_node + 1;
    //已使用的buffer个数
    unsigned int new_size = old_size + num_new_buf;
    //一共需要的buffer个数
    T **new_nstart;
    if (_capacity > 2 * new_size)
    {
        /*剩余空间足量,重新布局*/
        new_nstart = _map + (_capacity - num_new_buf) / 2; //元素居中
        if (at_front)
            new_nstart += num_new_buf;

        memmove(new_nstart, _start.map_node, old_size * sizeof(T **));
    }
    else
    {
        /*剩余空间不足*/
        // 申请-
        unsigned int new_capacity = _capacity + num_new_buf + 2;
        T **new_map = new T *[new_capacity];

        new_nstart = new_map + (new_capacity - num_new_buf) / 2;
        if (at_front)
            new_nstart += num_new_buf;
        // 拷贝-
        memmove(new_nstart, _start.map_node, old_size * sizeof(T **));
        // 释放-
        delete[] _map;
        _map = new_map;
        _capacity = new_capacity;
    }
    _start.jump(new_nstart);
    _finish.jump(new_nstart + old_size - 1);
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::push_back(const T &value)
{
    if (_finish.cur != _finish._finish)
    {
        construct(_finish.cur, value);
        ++_finish.cur;
    }
    else
    { // map末尾已满
        expand_map(1);
        *(_finish.map_node + 1) = Alloc::allocate(buffer_size());
        _finish.jump(_finish.map_node + 1);
        _finish.cur = _finish._start;
        construct(_finish.cur, value);
    }
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::push_front(const T &value)
{
    if (_start.cur != _start._start)
    {
        construct(_start.cur, value);
        --_start.cur;
    }
    else
    { // map开头已满
        expand_map(1, true);
        *(_start.map_node - 1) = new T *[buffer_size()];
        _start.jump(_start.map_node - 1);
        _start.cur = _start._finish - 1;
        construct(_start.cur, value);
    }
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::pop_back()
{
    if (_finish.cur != _finish._start)
    {
        destroy(_finish.cur);
        --_finish.cur;
    }
    else
    {
        destroy(_finish.cur);
        delete[] _finish._start;
        _finish.jump(_finish.map_node - 1);
        _finish.cur = _finish._finish - 1;
    }
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::pop_front()
{
    if (_start.cur != _start._finish)
    {
        destroy(_start.cur);
        ++_start.cur;
    }
    else
    {
        destroy(_start.cur);
        delete[] _start._start;
        _start.jump(_start.map_node + 1);
        _start.cur = _start._start;
    }
}

template <typename T, unsigned int buf_siz>
void deque<T, buf_siz>::clear()
{
    for (T **cur_buf = _start.map_node + 1; cur_buf < _finish.map_node;
         ++cur_buf)
    {
        for (T *cur = *cur_buf; cur < *cur_buf + buffer_size(); ++cur)
            destroy(cur);
        delete[](*cur_buf);
    }
    //处理首尾两个buffer
    if (_start.map_node != _finish.map_node)
    {
        /*首尾buffer不重叠*/
        for (T *cur = _start._start; cur < _start._finish; ++cur)
            destroy(cur);
        for (T *cur = _finish._start; cur < _finish._finish; ++cur)
            destroy(cur);
        delete[] _finish.map_node;
    }
    else
    {
        /*首尾buffer重叠*/
        for (T *cur = _start.cur; cur < _finish.cur; ++cur)
            destroy(cur);
    }

    _finish = _start; //保留一个空buffer
}

template <typename T, unsigned int buf_siz>
typename deque<T, buf_siz>::iterator deque<T, buf_siz>::insert(
    const iterator &pos, const T &value)
{
    unsigned int num_before = pos - _start;
    if (num_before < size() / 2)
    {
        /*前面元素少,向前移动*/
        push_front(front()); //保证前面有空间
        iterator stop = pos;
        ++stop;
        // move
        for (iterator src = _start + 1, dst = _start; src != stop; ++pos, ++dst)
            *dst = *src;
    }
    else
    {
        /*后面元素少,向后移动*/
        push_back(back()); //保证前面有空间
        iterator stop = pos;
        --stop;
        // move
        for (iterator src = _finish - 1, dst = _finish; src != stop; --pos, --dst)
            *dst = *src;
    }
    *pos = value;
    return pos;
}

template <typename T, unsigned int buf_siz>
typename deque<T, buf_siz>::iterator deque<T, buf_siz>::erase(
    const iterator &pos)
{
    unsigned int num_before = pos - _start;
    if (num_before < size() / 2)
    {
        /*前面元素少,前面向后补齐*/
        // move_backward
        for (iterator src = pos - 1, dst = pos; dst != _start; ++pos, ++dst)
            *dst = *src;
        iterator new_start = _start + 1;
        destroy(_start.cur);
        if (_start.map_node != new_start.map_node)
            delete[] * _start.map_node;
        _start = new_start;
        return pos + 1;
    }
    else
    {
        /*后面元素少,后面向前补齐*/
        // move
        for (iterator src = pos + 1, dst = pos; src != _finish; ++pos, ++dst)
            *dst = *src;
        iterator new_finish = _finish - 1;
        destroy(_finish.cur);
        if (_finish.map_node != new_finish.map_node)
            delete[] * _finish.map_node;
        _finish = new_finish;
        return pos;
    }
}

} // namespace tinySTL