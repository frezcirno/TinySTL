#pragma once

#include <cstring>
#include <cstddef>
#include "allocator.h"
#include "construct.h"
#include "algorithm.h"
#include "uninitialized.h"

namespace tiny
{

template <typename T, size_t buf_siz = 0>
class deque
{
  public:
    class _deque_iterator
    {
        friend class deque<T, buf_siz>;

      public:
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef size_t size_type;
        typedef ptrdiff_t diff_type;

        typedef _deque_iterator self;

      public:
        T &operator*() { return *cur_; }
        T *operator->() { return &(operator*()); }
        T &operator*() const { return *cur_; }
        T *operator->() const { return &(operator*()); }

        size_type operator-(const self &x) const
        {
            return buffer_size() * (map_ - x.map_ - 1) + (cur_ - begin_) +
                   (x.end_ - x.cur_);
        }

        self &operator++()
        {
            ++cur_;
            if (cur_ == end_) {
                jump(map_ + 1);
                cur_ = begin_;
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
            if (cur_ == begin_) {
                jump(map_ - 1);
                cur_ = end_;
            }
            --cur_;
            return *this;
        }

        self operator--(int)
        {
            self old(*this);
            --*this;
            return old;
        }

        self &operator+=(diff_type n)
        {  // n can be negative
            diff_type index = n + (cur_ - begin_);
            if (index >= 0 && index < (diff_type)buffer_size())
                cur_ += n;
            else {
                diff_type buf_offset = index > 0 ?
                                           index / buffer_size() :
                                           (-index - 1) / buffer_size() - 1;
                jump(map_ + buf_offset);
                cur_ = index - buf_offset * buffer_size() + begin_;
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
        bool operator==(const self &x) const { return cur_ == x.cur_; }
        bool operator!=(const self &x) const { return cur_ != x.cur_; }
        bool operator<(const self &x) const
        {
            return (map_ == x.map_) ? (cur_ < x.cur_) : (map_ < x.map_);
        }
        bool operator>(const self &x) const
        {
            return (map_ == x.map_) ? (cur_ > x.cur_) : (map_ > x.map_);
        }

      private:
        _deque_iterator()
            : cur_(nullptr), begin_(nullptr), end_(nullptr), map_(nullptr)
        {
        }

        void jump(T **buf_pos)
        {
            map_ = buf_pos;
            begin_ = *buf_pos;
            end_ = *buf_pos + buffer_size();
        }

      private:
        T *cur_, *begin_, *end_;
        T **map_;
    };

  public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T *const_pointer;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t diff_type;

    typedef _deque_iterator iterator;
    typedef _deque_iterator const_iterator;

  private:
    typedef allocator<T> Alloc;
    typedef deque<T, buf_siz> self;

  public:
    deque(size_type n = 0) : map_(nullptr), start_(), finish_(), capacity_(0)
    {
        size_type num_buf = n / buffer_size() + 1;  // n=0时也配置1个
        // map创建
        capacity_ = num_buf + 2;           // 前后各留一个buffer的余量
        if (capacity_ < 8) capacity_ = 8;  // 最少8个buffer
        map_ = new T *[capacity_];
        // buffer创建
        T **nstart = map_ + (capacity_ - num_buf) / 2;  // 元素居中
        T **nfinish =
            (nstart + num_buf) - 1;  // 当n刚好整除时,nfinish独占一个buffer
        for (T **cur_buf = nstart; cur_buf <= nfinish; ++cur_buf)
            *cur_buf = Alloc::allocate(buffer_size());
        // 迭代器设置
        start_.jump(nstart);
        start_.cur_ = start_.begin_;
        finish_.jump(nfinish);
        finish_.cur_ = finish_.begin_ + n % buffer_size();
    }

    deque(size_type n, const_reference value) : deque(n)
    {
        // 元素构造
        for (T **cur_buf = start_.map_; cur_buf < finish_.map_; ++cur_buf)
            tiny::uninitialized_fill_n(*cur_buf, buffer_size(), value);
        tiny::uninitialized_fill(finish_.begin_, finish_.cur_, value);
    }

    template <class Iter>
    deque(Iter start, Iter finish) : deque()
    {
        assign(start, finish);
    }

    deque(const self &rhs) : deque() { assign(rhs.begin(), rhs.end()); }

    ~deque()
    {
        clear();
        Alloc::deallocate(start_.begin_);
        delete[] map_;
    }

    template <class Iter>
    void assign(Iter start, Iter finish)
    {
    }

    iterator begin() noexcept { return start_; }
    iterator end() noexcept { return finish_; }
    const_iterator begin() const noexcept { return start_; }
    const_iterator end() const noexcept { return finish_; }
    T &front() noexcept { return *start_; }
    T &back() noexcept { return *(finish_ - 1); }
    T &at(size_type index) { return start_[index]; }

    void clear()
    {
        for (T **cur_buf = start_.map_ + 1; cur_buf < finish_.map_; ++cur_buf) {
            tiny::destroy(cur_buf, cur_buf + buffer_size());
            Alloc::deallocate(*cur_buf);
        }
        // 处理首尾两个buffer
        if (start_.map_ != finish_.map_) {
            /*首尾buffer不重叠*/
            tiny::destroy(start_.begin_, start_.end_);
            tiny::destroy(finish_.begin_, finish_.end_);
            Alloc::deallocate(finish_.begin_);
        } else {
            /*首尾buffer重叠*/
            tiny::destroy(start_.cur_, finish_.cur_);
        }

        finish_ = start_;  // 保留一个空buffer
    }
    void push_back(const_reference value)
    {
        construct(finish_.cur_, value);
        if (finish_.cur_ + 1 != finish_.end_) {
            ++finish_.cur_;
        } else {  // map末尾已满, _finish迭代器后移一个buffer
            expand_map(1);
            *(finish_.map_ + 1) = Alloc::allocate(buffer_size());
            finish_.jump(finish_.map_ + 1);
            finish_.cur_ = finish_.begin_;
        }
    }
    void push_front(const_reference value)
    {
        if (start_.cur_ != start_.begin_) {
            --start_.cur_;
        } else {  // map开头已满
            expand_map(1, true);
            *(start_.map_ - 1) = Alloc::allocate(buffer_size());
            start_.jump(start_.map_ - 1);
            start_.cur_ = start_.end_ - 1;
        }
        construct(start_.cur_, value);
    }
    void pop_back()
    {
        tiny::destroy(finish_.cur_);
        if (finish_.cur_ != finish_.begin_) {
            --finish_.cur_;
        } else {
            Alloc::deallocate(finish_.begin_);
            finish_.jump(finish_.map_ - 1);
            finish_.cur_ = finish_.end_ - 1;
        }
    }
    void pop_front()
    {
        tiny::destroy(start_.cur_);
        if (start_.cur_ + 1 != start_.end_) {
            ++start_.cur_;
        } else {
            Alloc::deallocate(start_.begin_);
            start_.jump(start_.map_ + 1);
            start_.cur_ = start_.begin_;
        }
    }
    iterator insert(iterator pos, const_reference value)
    {
        size_type num_before = pos - start_;
        if (num_before < size() / 2) {
            /*前面元素少,向前移动*/
            push_front(front());  // 保证前面有空间
            tiny::copy(start_ + 1, pos + 1, start_);
            --pos;
        } else {
            /*后面元素少,向后移动*/
            push_back(back());  // 保证后面有空间
            tiny::copy_backward(pos, finish_, finish_ + 1);
        }
        *pos = value;
        return pos;
    }
    iterator erase(iterator pos)
    {
        size_type num_before = pos - start_;
        if (num_before < size() / 2) {
            /*前面元素少,前面向后补齐*/
            tiny::copy_backward(start_, pos, pos + 1);
            iterator new_start = start_ + 1;
            tiny::destroy(start_.cur_);
            if (start_.map_ != new_start.map_) Alloc::deallocate(start_.map_);
            start_ = new_start;
            return pos + 1;
        } else {
            /*后面元素少,后面向前补齐*/
            tiny::copy(pos + 1, finish_, pos);
            iterator new_finish = finish_ - 1;
            tiny::destroy(finish_.cur_);
            if (finish_.map_ != new_finish.map_)
                Alloc::deallocate(finish_.map_);
            finish_ = new_finish;
            return pos;
        }
    }

    size_type size() const noexcept { return finish_ - start_; }
    bool empty() const noexcept { return finish_ == start_; }

  private:
    /**用来计算deque中每个buffer内部的元素个数 */
    constexpr static size_t buffer_size(size_t siz, size_t elem_siz) noexcept
    {
        return (siz != 0) ? siz : ((elem_siz < 512) ? (512 / elem_siz) : 1);
    }

    void expand_map(size_type num_new_buf = 1, bool at_front = false)
    {
        size_type old_size = finish_.map_ - start_.map_ + 1;
        // 已使用的buffer个数
        size_type new_size = old_size + num_new_buf;
        // 一共需要的buffer个数
        T **new_nstart;
        if (capacity_ > 2 * new_size) {
            /*剩余空间足量,重新布局*/
            new_nstart = map_ + (capacity_ - num_new_buf) / 2;  // 元素居中
            if (at_front) new_nstart += num_new_buf;

            memmove(new_nstart, start_.map_, old_size * sizeof(T **));
        } else {
            /*剩余空间不足*/
            // 申请-
            size_type new_capacity = capacity_ + num_new_buf + 2;
            T **new_map = new T *[new_capacity];
            new_nstart = new_map + (new_capacity - num_new_buf) / 2;
            if (at_front) new_nstart += num_new_buf;
            // 拷贝-
            memmove(new_nstart, start_.map_, old_size * sizeof(T **));
            // 释放-
            delete[] map_;
            map_ = new_map;
            capacity_ = new_capacity;
        }
        start_.jump(new_nstart);
        finish_.jump(new_nstart + old_size - 1);
    }

  private:
    T **map_;
    iterator start_;
    iterator finish_;
    size_type capacity_;
    size_type buffer_size();
};

}  // namespace tiny