#pragma once

#include "allocator.h"
#include "pair.h"
#include <stdlib.h>

namespace tinySTL
{

template <typename _Key, typename _Ty>
struct SkipList {
  public:
    typedef _Key key_type;
    typedef _Ty mapped_type;
    typedef pair<const _Key, _Ty> value_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef const value_type *const_pointer;
    typedef const value_type &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  private:
    struct Node {
        value_type value;

        Node **forward;

        Node() = delete;

        Node(const value_type &value, size_type n_level) : value(value)
        {
            forward = new Node *[n_level];
            for (size_type i = 0; i < n_level; ++i) forward[i] = nullptr;
        }

        ~Node() { delete[] forward; }
    };

  public:
    typedef Node __node_type;
    typedef __node_type *__node_ptr;

  private:
    Node __head;
    size_type __max_level;
    size_type __size;

    constexpr static size_type MAX_LEVEL = 16;

  public:
    SkipList() : __head(value_type{}, MAX_LEVEL), __max_level(0), __size(0) {}

    ~SkipList() { clear(); }

    void clear()
    {
        Node *cur = __head.forward[0];
        while (cur) {
            Node *next = cur->forward[0];
            delete cur;
            cur = next;
        }
        __head.forward[0] = nullptr;
        __max_level = 0;
        __size = 0;
    }

    size_type size() const noexcept { return __size; }

    bool empty() const noexcept { return __size == 0; }

    mapped_type &operator[](const key_type &key)
    {
        __node_ptr node = find(key);
        if (!node) { node = _insert(key, value_type{key, _Ty()}); }
        return node->value.second;
    }

    const mapped_type &operator[](const key_type &key) const
    {
        __node_ptr node = find(key);
        if (!node) { node = _insert(key, value_type{key, _Ty()}); }
        return node->value.second;
    }

    /*
    L0  1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> NULL
    L1  1 ------> 3 ------> 5 ------> 7 ------> 9 -> 10 -> NULL
    L2  1 ----------------> 5 ----------------> 9 -> 10 -> NULL
    L3  1 ------------------------------------> 9 -> 10 -> NULL
    L4  1 -----------------------------------------> 10 -> NULL
    ...
    Ln  1 -----------------------------------------------> NULL
    */
    Node *_find_prev(const key_type &key) const noexcept
    {
        Node *cur = (Node *)&__head;
        for (ssize_t i = __max_level; i >= 0; --i) {
            while (1) {
                // go right
                Node *next = cur->forward[i];
                if (!next || next->value.first >= key) break;
                cur = next;
            }
            // go down
        }
        return cur;
    }

    __node_ptr find(const key_type &key) const noexcept
    {
        Node *prev = _find_prev(key);
        prev = prev->forward[0];
        if (prev && prev->value.first == key) return prev;
        return nullptr;
    }

    size_type count(const key_type &key) const noexcept
    {
        return find(key) ? 1 : 0;
    }

    /**
     * @brief Returns a random level in the range [0, MAX_LEVEL)
     */
    size_t _random_level() const noexcept
    {
        size_t level = 0;
        while (rand() & 1 && level < MAX_LEVEL - 1) ++level;
        return level;
    }

    template <typename _Kt, typename _Arg>
    __node_ptr _insert(_Kt &&__key, _Arg &&__value)
    {
        Node *cur = &__head;
        Node *update[MAX_LEVEL];
        for (ssize_t i = __max_level; i >= 0; --i) {
            while (1) {
                // go right
                Node *next = cur->forward[i];
                if (!next || next->value.first >= __key) break;
                cur = next;
            }
            update[i] = cur;
            // go down
        }

        cur = cur->forward[0];
        if (cur && cur->value.first == __key) {
            cur->value.second = __value.second;
            return cur;
        }

        size_type new_level = _random_level();

        if (new_level > __max_level) {
            for (size_t i = __max_level + 1; i <= new_level; ++i)
                update[i] = &__head;
            __max_level = new_level;
        }

        Node *x = new Node(forward<_Arg>(__value), new_level);
        __size++;

        for (size_t i = 0; i <= new_level; ++i) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }

        return x;
    }

    __node_ptr insert(value_type &&__value)
    {
        value_type val(forward<value_type>(__value));
        return _insert(move(val).first, move(val));
    }

    void erase(const key_type &key)
    {
        Node *prev = &__head;
        Node *update[MAX_LEVEL];
        for (ssize_t i = __max_level; i >= 0; --i) {
            while (1) {
                // go right
                Node *next = prev->forward[i];
                if (!next || next->value.first >= key) break;
                prev = next;
            }
            update[i] = prev;
            // go down
        }

        prev = prev->forward[0];
        if (!prev || prev->value.first != key) return;

        for (size_t i = 0; i <= __max_level; ++i) {
            if (update[i]->forward[i] != prev) break;
            update[i]->forward[i] = prev->forward[i];
        }

        delete prev;
        __size--;

        while (__max_level > 0 && !__head->forward[__max_level]) --__max_level;
    }
};

}  // namespace tinySTL
