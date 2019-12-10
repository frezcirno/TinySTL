#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstring>
#include <cstddef>
#include "allocator.h"
#include "construct.h"
#include "algorithm.h"
namespace tinySTL
{


/**用来计算deque中每个buffer内部的元素个数 */
inline size_t _buffer_size(size_t buf_siz, size_t elem_siz) noexcept
{
    return (buf_siz != 0) ? (buf_siz) :
                            ((elem_siz < 512) ? (512 / elem_siz) : 1);
}

template <typename T, size_t buf_siz>
class _deque_iterator
{
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef ptrdiff_t diff_type;

    typedef _deque_iterator<T, buf_siz> self;
    T *cur;        //指向当前元素
    T *_start;     //当前buffer开头(前闭后开)
    T *_finish;    //当前buffer结尾(前闭后开)
    T **map_node;  //指向中控器的元素

  private:
    size_type buffer_size;

  public:
    _deque_iterator()
        : cur(nullptr), _start(nullptr), _finish(nullptr), map_node(nullptr),
          buffer_size(_buffer_size(buf_siz, sizeof(T)))
    {
    }
    void jump(T **buf_pos)
    {
        map_node = buf_pos;
        _start = *buf_pos;
        _finish = *buf_pos + buffer_size;
    }
    T &operator*() { return *cur; }
    T *operator->() { return &(operator*()); }
    T &operator*() const { return *cur; }
    T *operator->() const { return &(operator*()); }

    size_type operator-(const self &x) const
    {
        return buffer_size * (map_node - x.map_node - 1) + (cur - _start) +
               (x._finish - x.cur);
    }

    self &operator++()
    {
        ++cur;
        if (cur == _finish) {
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
        if (cur == _start) {
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

    self &operator+=(diff_type n)
    {  //可正可负
        diff_type index = n + (cur - _start);
        if (index >= 0 && index < (diff_type)buffer_size)
            cur += n;
        else {
            diff_type buf_offset = index > 0 ? index / buffer_size :
                                               (-index - 1) / buffer_size - 1;
            jump(map_node + buf_offset);
            cur = index - buf_offset * buffer_size + _start;
        }
        return *this;
    }

    self &operator-=(diff_type n) { return *this += -n; }

    const self operator+(diff_type n)
    {
        self tmp = *this;
        return tmp += n;
    }

    const self operator-(diff_type n) const
    {
        self tmp = *this;
        return tmp += -n;
    }

    T &operator[](size_type n) { return *(*this + n); }
    bool operator==(const self &x) const { return cur == x.cur; }
    bool operator!=(const self &x) const { return cur != x.cur; }
    bool operator<(const self &x) const
    {
        return (map_node == x.map_node) ? (cur < x.cur) :
                                          (map_node < x.map_node);
    }
    bool operator>(const self &x) const
    {
        return (map_node == x.map_node) ? (cur > x.cur) :
                                          (map_node > x.map_node);
    }
};

template <typename T, size_t buf_siz = 0> class deque
{
  public:
    typedef T         value_type;
    typedef T *       pointer;
    typedef T &       reference;
    typedef const T * const_pointer;
    typedef const T & const_reference;
    typedef size_t    size_type;
    typedef ptrdiff_t diff_type;

    typedef _deque_iterator<T, buf_siz> iterator;
    typedef _deque_iterator<T, buf_siz> const_iterator;

  private:
    typedef allocator<T>      Alloc;
    typedef deque<T, buf_siz> self;

  private:
    T **      _map;  //中控器
    iterator  _start;
    iterator  _finish;
    size_type _capacity;  // buffer的数量

  public:
    deque(size_type n = 0);

    deque(size_type n, const_reference value);

    template <class Iter>
    deque(Iter start, Iter finish) : deque()
    {
        assign(start, finish);
    }
    
	deque(const self &rhs) : deque() { assign(rhs.begin(), rhs.end()); }
    
	~deque();

    template <class Iter> void assign(Iter start, Iter finish);

    iterator       begin() noexcept { return _start; }
    iterator       end() noexcept { return _finish; }
    const_iterator begin() const noexcept { return _start; }
    const_iterator end() const noexcept { return _finish; }
    T &            front() noexcept { return *_start; }
    T &            back() noexcept { return *(_finish - 1); }

    void     clear();
    void     push_back(const_reference value);
    void     push_front(const_reference value);
    void     pop_back();
    void     pop_front();
    iterator insert(iterator pos, const_reference value);
    iterator erase(iterator pos);

    size_type size() const noexcept { return _finish - _start; }
    bool      empty() const noexcept { return _finish == _start; }

  private:
    size_type buffer_size;
	//空间扩充算法
    void expand_map(size_type num_new_buf = 1, bool at_front = false);
};

template <typename T, size_t buf_siz>
deque<T, buf_siz>::deque(size_type n)
    : _map(nullptr), _start(), _finish(), _capacity(0),
      buffer_size(_buffer_size(buf_siz, sizeof(T)))
{
    size_type num_buf = n / buffer_size + 1;// n=0时也配置1个
    // map创建
    _capacity = num_buf + 2;           //前后各留一个buffer的余量
    if (_capacity < 8) _capacity = 8;  //最少8个buffer
    _map = new T *[_capacity];
    // buffer创建
    T **nstart = _map + (_capacity - num_buf) / 2;  // 元素居中
    T **nfinish = (nstart + num_buf) - 1;  //当n刚好整除时,nfinish独占一个buffer
    for (T **cur_buf = nstart; cur_buf <= nfinish; ++cur_buf)
        *cur_buf = Alloc::allocate(buffer_size);
    //迭代器设置
    _start.jump(nstart);
    _start.cur = _start._start;
    _finish.jump(nfinish);
    _finish.cur = _finish._start + n % buffer_size;
}

template <typename T, size_t buf_siz>
deque<T, buf_siz>::deque(size_type n, const_reference value) : deque(n)
{
    //元素构造
    for (T **cur_buf = _start.map_node; cur_buf < _finish.map_node; ++cur_buf)
        uninitialized_fill_n(*cur_buf, buffer_size, value);
    uninitialized_fill(_finish._start, _finish.cur, value);
}

template <typename T, size_t buf_siz> deque<T, buf_siz>::~deque()
{
    clear();
    Alloc::deallocate(_start._start);
    delete[] _map;
}

template <typename T, size_t buf_siz>
void deque<T, buf_siz>::expand_map(size_type num_new_buf, bool at_front)
{
    size_type old_size = _finish.map_node - _start.map_node + 1;
    //已使用的buffer个数
    size_type new_size = old_size + num_new_buf;
    //一共需要的buffer个数
    T **new_nstart;
    if (_capacity > 2 * new_size) {
        /*剩余空间足量,重新布局*/
        new_nstart = _map + (_capacity - num_new_buf) / 2;  //元素居中
        if (at_front) new_nstart += num_new_buf;

        memmove(new_nstart, _start.map_node, old_size * sizeof(T **));
    } else {
        /*剩余空间不足*/
        // 申请-
        size_type new_capacity = _capacity + num_new_buf + 2;
        T **      new_map = new T *[new_capacity];
        new_nstart = new_map + (new_capacity - num_new_buf) / 2;
        if (at_front) new_nstart += num_new_buf;
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

template <typename T, size_t buf_siz>
void deque<T, buf_siz>::push_back(const_reference value)
{
    construct(_finish.cur, value);
    if (_finish.cur + 1 != _finish._finish) {
        ++_finish.cur;
    } else {  // map末尾已满, _finish迭代器后移一个buffer
        expand_map(1);
        *(_finish.map_node + 1) = Alloc::allocate(buffer_size);
        _finish.jump(_finish.map_node + 1);
        _finish.cur = _finish._start;
    }
}

template <typename T, size_t buf_siz>
void deque<T, buf_siz>::push_front(const_reference value)
{
    if (_start.cur != _start._start) {
        --_start.cur;
    } else {  // map开头已满
        expand_map(1, true);
        *(_start.map_node - 1) = Alloc::allocate(buffer_size);
        _start.jump(_start.map_node - 1);
        _start.cur = _start._finish - 1;
    }
    construct(_start.cur, value);
}

template <typename T, size_t buf_siz> void deque<T, buf_siz>::pop_back()
{
    destroy(_finish.cur);
    if (_finish.cur != _finish._start) {
        --_finish.cur;
    } else {
        Alloc::deallocate(_finish._start);
        _finish.jump(_finish.map_node - 1);
        _finish.cur = _finish._finish - 1;
    }
}

template <typename T, size_t buf_siz> void deque<T, buf_siz>::pop_front()
{
    destroy(_start.cur);
    if (_start.cur + 1 != _start._finish) {
        ++_start.cur;
    } else {
        Alloc::deallocate(_start._start);
        _start.jump(_start.map_node + 1);
        _start.cur = _start._start;
    }
}

template <typename T, size_t buf_siz> void deque<T, buf_siz>::clear()
{
    for (T **cur_buf = _start.map_node + 1; cur_buf < _finish.map_node;
         ++cur_buf) {
        destroy(cur_buf, cur_buf + buffer_size);
        Alloc::deallocate(*cur_buf);
    }
    //处理首尾两个buffer
    if (_start.map_node != _finish.map_node) {
        /*首尾buffer不重叠*/
        destroy(_start._start, _start._finish);
        destroy(_finish._start, _finish._finish);
        Alloc::deallocate(_finish._start);
    } else {
        /*首尾buffer重叠*/
        destroy(_start.cur, _finish.cur);
    }

    _finish = _start;  //保留一个空buffer
}

template <typename T, size_t buf_siz>
typename deque<T, buf_siz>::iterator deque<T, buf_siz>::insert(iterator pos,
    const_reference                                                     value)
{
    size_type num_before = pos - _start;
    if (num_before < size() / 2) {
        /*前面元素少,向前移动*/
        push_front(front());  //保证前面有空间
        copy(_start + 1, pos + 1, _start);
        --pos;
    } else {
        /*后面元素少,向后移动*/
        push_back(back());  //保证后面有空间
        copy_backward(pos, _finish, _finish + 1);
    }
    *pos = value;
    return pos;
}

template <typename T, size_t buf_siz>
typename deque<T, buf_siz>::iterator deque<T, buf_siz>::erase(iterator pos)
{
    size_type num_before = pos - _start;
    if (num_before < size() / 2) {
        /*前面元素少,前面向后补齐*/
        copy_backward(_start, pos, pos + 1);
        iterator new_start = _start + 1;
        destroy(_start.cur);
        if (_start.map_node != new_start.map_node)
            Alloc::deallocate(_start.map_node);
        _start = new_start;
        return pos + 1;
    } else {
        /*后面元素少,后面向前补齐*/
        copy(pos + 1, _finish, pos);
        iterator new_finish = _finish - 1;
        destroy(_finish.cur);
        if (_finish.map_node != new_finish.map_node)
            Alloc::deallocate(_finish.map_node);
        _finish = new_finish;
        return pos;
    }
}
template <typename T, size_t buf_siz>
template <class Iter>
void deque<T, buf_siz>::assign(Iter start, Iter finish)
{
    // TODO
}
}  // namespace tinySTL