#pragma once

#include <cstring>
#include "allocator.hpp"
namespace tinySTL {

/**用来计算deque中每个buffer内部的元素个数 */
inline unsigned int _buffer_size(unsigned int buf_siz, unsigned int elem_siz) {
    return (buf_siz != 0) ? (buf_siz)
                          : ((elem_siz < 512) ? (512 / elem_siz) : 1);
}

template <typename T, unsigned int buf_siz>
class _DequeIterator {
    typedef _DequeIterator<T, buf_siz> self;

   private:
    static unsigned int buffer_size() {
        static int _buf_siz = _buffer_size(buf_siz, sizeof(T));
        return _buf_siz;
    }

   public:
    _DequeIterator() {}
    ~_DequeIterator() {}
    void jump(T** buf_pos) {
        map_node = buf_pos;
        start = *buf_pos;
        finish = *buf_pos + buffer_size();
    }
    T& operator*() const { return *cur; }
    T* operator->() const { return &(operator*()); }

    unsigned int operator-(const self& x) const {
        return buffer_size() * (map_node - x.map_node - 1) + (cur - start) +
               (x.finish - x.cur);
    }

    self& operator++() {
        ++cur;
        if (cur == finish) {
            jump(map_node + 1);
            cur = start;
        }
        return *this;
    }

    self operator++(int) {
        self old(*this);
        ++*this;
        return old;
    }

    self& operator--() {
        if (cur == start) {
            jump(map_node - 1);
            cur = finish;
        }
        --cur;
        return *this;
    }

    self operator--(int) {
        self old(*this);
        --*this;
        return old;
    }

    self& operator+=(int n) {  //可正可负
        int index = n + (cur - start);
        if (index >= 0 && index < buffer_size())
            cur += n;
        else {
            int buf_offset = index > 0 ? index / buffer_size()
                                       : (-index - 1) / buffer_size() - 1;
            jump(map_node + buf_offset);
            cur = index - buf_offset * buffer_size() + start;
        }
        return *this;
    }

    self& operator-=(int n) { return *this += -n; }

    const self operator+(int n) {
        self tmp = *this;
        return tmp += n;
    }

    const self operator-(int n) const {
        self tmp = *this;
        return tmp += -n;
    }

    T& operator[](unsigned int n) { return *(*this + n); }
    bool operator==(const self& x) const { return cur == x.cur; }
    bool operator!=(const self& x) const { return cur != x.cur; }
    bool operator<(const self& x) const {
        return (map_node == x.map_node) ? (cur < x.cur)
                                        : (map_node < x.map_node);
    }
    bool operator>(const self& x) const {
        return (map_node == x.map_node) ? (cur > x.cur)
                                        : (map_node > x.map_node);
    }

   public:
    T* cur;        //指向当前元素
    T* start;      //当前buffer开头(前闭后开)
    T* finish;     //当前buffer结尾(前闭后开)
    T** map_node;  //指向中控器的元素
};

template <typename T, unsigned int buf_siz = 0>
class Deque {
   public:
    typedef _DequeIterator<T, buf_siz> Iterator;
    typedef allocator<T> Alloc;

    Deque(unsigned int n = 0);
    Deque(unsigned int n, const T& value);
    ~Deque();

    Iterator begin() { return start; }
    Iterator end() { return finish; }
    T& front() { return *start; }
    T& back() { return *(finish - 1); }

    void clear();
    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    Iterator insert(const Iterator& pos, const T& value);
    Iterator erase(const Iterator& pos);

    unsigned int size() const { return finish - start; }
    bool empty() const { return finish == start; }

   private:
    T** map;  //中控器
    Iterator start, finish;
    unsigned int m_capacity;
    static unsigned int buffer_size() {
        static int _buf_siz = _buffer_size(buf_siz, sizeof(T));
        return _buf_siz;
    }
    void expand_map(unsigned int num_new_buf = 1,
                    bool at_front = false);  //空间扩充算法
};

template <typename T, unsigned int buf_siz>
Deque<T, buf_siz>::Deque(unsigned int n)
    : start(), finish(), map(0), m_capacity(0) {
    unsigned int num_buf = n / buffer_size() + 1;
    // map创建
    m_capacity = num_buf + 2;            //前后各留一个buffer的余量
    if (m_capacity < 8) m_capacity = 8;  //最少8个buffer
    map = new T*[m_capacity];
    // buffer创建
    T** nstart = map + (m_capacity - num_buf) / 2;  // 元素居中
    T** nfinish = nstart + num_buf - 1;  //当n刚好整除时,nfinish独占一个buffer
    for (T** cur_buf = nstart; cur_buf <= nfinish; ++cur_buf)
        *cur_buf = Alloc::allocate(buffer_size());
    //迭代器设置
    start.jump(nstart);
    start.cur = start.start;
    finish.jump(nfinish);
    finish.cur = finish.start + n % buffer_size();
}

template <typename T, unsigned int buf_siz>
Deque<T, buf_siz>::Deque(unsigned int n, const T& value) : Deque(n) {
    //元素构造
    for (T** cur_buf = start.map_node; cur_buf < finish.map_node; ++cur_buf)
        construct_n(*cur_buf, buffer_size(), value);
    construct_range(finish.start, finish.cur, value);
}

template <typename T, unsigned int buf_siz>
Deque<T, buf_siz>::~Deque() {
    clear();
    delete[] start.start;
    delete[] map;
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::expand_map(unsigned int num_new_buf, bool at_front) {
    unsigned int old_size = finish.map_node - start.map_node + 1;
    //已使用的buffer个数
    unsigned int new_size = old_size + num_new_buf;
    //一共需要的buffer个数
    T** new_nstart;
    if (m_capacity > 2 * new_size) {
        /*剩余空间足量,重新布局*/
        new_nstart = map + (m_capacity - num_new_buf) / 2;  //元素居中
        if (at_front) new_nstart += num_new_buf;

        memmove(new_nstart, start.map_node, old_size * sizeof(T**));
    } else {
        /*剩余空间不足*/
        //申请-
        unsigned int new_capacity = m_capacity + num_new_buf + 2;
        T** new_map = new T*[new_capacity];

        new_nstart = new_map + (new_capacity - num_new_buf) / 2;
        if (at_front) new_nstart += num_new_buf;
        // 拷贝-
        memmove(new_nstart, start.map_node, old_size * sizeof(T**));
        // 释放-
        delete[] map;
        map = new_map;
        m_capacity = new_capacity;
    }
    start.jump(new_nstart);
    finish.jump(new_nstart + old_size - 1);
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::push_back(const T& value) {
    if (finish.cur != finish.finish) {
        construct(finish.cur, value);
        ++finish.cur;
    } else {  // map末尾已满
        expand_map(1);
        *(finish.map_node + 1) = Alloc::allocate(buffer_size());
        finish.jump(finish.map_node + 1);
        finish.cur = finish.start;
        construct(finish.cur, value);
    }
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::push_front(const T& value) {
    if (start.cur != start.start) {
        construct(start.cur, value);
        --start.cur;
    } else {  // map开头已满
        expand_map(1, true);
        *(start.map_node - 1) = new T*[buffer_size()];
        start.jump(start.map_node - 1);
        start.cur = start.finish - 1;
        construct(start.cur, value);
    }
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::pop_back() {
    if (finish.cur != finish.start) {
        destroy(finish.cur);
        --finish.cur;
    } else {
        destroy(finish.cur);
        delete[] finish.start;
        finish.jump(finish.map_node - 1);
        finish.cur = finish.finish - 1;
    }
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::pop_front() {
    if (start.cur != start.finish) {
        destroy(start.cur);
        ++start.cur;
    } else {
        destroy(start.cur);
        delete[] start.start;
        start.jump(start.map_node + 1);
        start.cur = start.start;
    }
}

template <typename T, unsigned int buf_siz>
void Deque<T, buf_siz>::clear() {
    for (T** cur_buf = start.map_node + 1; cur_buf < finish.map_node;
         ++cur_buf) {
        for (T* cur = *cur_buf; cur < *cur_buf + buffer_size(); ++cur)
            destroy(cur);
        delete[] * cur_buf;
    }
    //处理首尾两个buffer
    if (start.map_node != finish.map_node) {
        /*首尾buffer不重叠*/
        for (T* cur = start.start; cur < start.finish; ++cur) destroy(cur);
        for (T* cur = finish.start; cur < finish.finish; ++cur) destroy(cur);
        delete[] finish.map_node;
    } else {
        /*首尾buffer重叠*/
        for (T* cur = start.cur; cur < finish.cur; ++cur) destroy(cur);
    }

    finish = start;  //保留一个空buffer
}

template <typename T, unsigned int buf_siz>
typename Deque<T, buf_siz>::Iterator Deque<T, buf_siz>::insert(
    const Iterator& pos, const T& value) {
    unsigned int num_before = pos - start;
    if (num_before < size() / 2) {
        /*前面元素少,向前移动*/
        push_front(front());  //保证前面有空间
        Iterator stop = pos;
        ++stop;
        // move
        for (Iterator src = start + 1, dst = start; src != stop; ++pos, ++dst)
            *dst = *src;

    } else {
        /*后面元素少,向后移动*/
        push_back(back());  //保证前面有空间
        Iterator stop = pos;
        --stop;
        // move
        for (Iterator src = finish - 1, dst = finish; src != stop; --pos, --dst)
            *dst = *src;
    }
    *pos = value;
    return pos;
}

template <typename T, unsigned int buf_siz>
typename Deque<T, buf_siz>::Iterator Deque<T, buf_siz>::erase(
    const Iterator& pos) {
    unsigned int num_before = pos - start;
    if (num_before < size() / 2) {
        /*前面元素少,前面向后补齐*/
        // move_backward
        for (Iterator src = pos - 1, dst = pos; dst != start; ++pos, ++dst)
            *dst = *src;
        Iterator new_start = start + 1;
        destroy(start.cur);
        if (start.map_node != new_start.map_node) delete[] * start.map_node;
        start = new_start;
        return pos + 1;
    } else {
        /*后面元素少,后面向前补齐*/
        // move
        for (Iterator src = pos + 1, dst = pos; src != finish; ++pos, ++dst)
            *dst = *src;
        Iterator new_finish = finish - 1;
        destroy(finish.cur);
        if (finish.map_node != new_finish.map_node) delete[] * finish.map_node;
        finish = new_finish;
        return pos;
    }
}

}  // namespace tinySTL