#pragma once

#include "allocator.h"
#include "pair.h"

namespace tiny
{

template <typename _Key, typename _Ty>
struct BTree {
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

        Node **child;
        size_type n_child;

        Node() = delete;

        Node(const value_type &value, size_type n_child)
            : value(value), n_child(n_child)
        {
            child = new Node *[n_child];
            for (size_type i = 0; i < n_child; ++i) child[i] = nullptr;
        }

        ~Node() { delete[] child; }
    };

  public:
    typedef Node __node_type;
    typedef __node_type *__node_ptr;

  private:
    Node __root;
    size_type __max_child;
    size_type __size;

    constexpr static size_type MAX_CHILD = 16;

  public:
    BTree() : __root(value_type{}, MAX_CHILD), __max_child(0), __size(0) {}

    ~BTree() { clear(); }

    void clear()
    {
        Node *cur = __root.child[0];
        while (cur) {
            Node *next = cur->child[0];
            delete cur;
            cur = next;
        }
        __root.n_child = 0;
        __size = 0;
    }
};

}  // namespace tiny