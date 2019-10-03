#pragma once
/**
 * 双向循环链表
 * 类STL<list>接口
 * 简化版本
 */
#include "allocator.hpp"
namespace tinySTL {

template <typename T>
struct _LinkListNode {
    T m_data;
    _LinkListNode<T>*m_prev, m_next;
    _LinkListNode(const T& value) : m_data(value) {}
};

template <typename T>
class _LinkListIterator {
    typedef _LinkListNode<T>* link_t;
    typedef _LinkListIterator<T> self;

   private:
    link_t m_node;

   public:
    //构造、析构
    _LinkListIterator() {}
    _LinkListIterator(link_t node) : m_node(node) {}
    _LinkListIterator(const link_t node) : m_node(node) {}
    ~_LinkListIterator() {}
    self& operator=(const self& rhs) {
        this->m_node = rhs.m_node;
        return *this;
    }

    self& operator++() {
        this->m_node = m_node->m_succ;
        return *this;
    }

    self& operator--() {
        m_node = m_node->m_prec;
        return *this;
    }

    const self operator++(int) {
        self old(*this);
        ++*this;
        return old;
    }

    const self operator--(int) {
        self old(*this);
        --*this;
        return old;
    }

    bool operator==(const self& rhs) { return m_node == rhs.m_node; }
    bool operator!=(const self& rhs) { return m_node != rhs.m_node; }
    T& operator*() { return m_node->m_data; }
    T& operator->() { return &(operator*()); }
};

template <typename T>
class LinkList {
   public:
    typedef _LinkListNode<T> _Node;
    typedef _LinkListIterator<T> Iterator;
    typedef const _LinkListIterator<T> const_Iterator;

    //构造、析构、复制、初始化
    LinkList();
    explicit LinkList(int count, const T& value);
    LinkList(const LinkList<T>& other);
    ~LinkList() {
        clear();
        allocator<_Node>::deallocate(m_entry);
    }
    LinkList<T>& operator=(const LinkList<T>& other);
    void assign(Iterator begin, Iterator end);
    void assign(int count, const T& value);

    //元素访问
    T& front() { return *begin(); }
    T& back() { return *end()->m_prev; }

    //迭代器
    Iterator begin() { return m_entry->m_next; }
    Iterator end() { return m_entry; }
    const_Iterator begin() const { return m_entry->m_next; }
    const_Iterator end() const { return m_entry; }

    //容量
    bool empty() { return m_size == 0; }
    int size() { return m_size; }

    //修改器
    void clear();
    Iterator insert(const Iterator& pos, const T& value);
    Iterator insert(const Iterator& pos, unsigned int count, const T& value);
    Iterator erase(const Iterator& pos);
    void push_back(const T& value) { insert(end(), value); }
    void pop_back() { erase(--end()); }
    void push_front(const T& value) { insert(begin(), value); }
    void pop_front() { erase(begin()); }

    //操作
    void remove(const T& value);

   private:
    _Node* m_entry;  //由于是双向循环链表,仅需一个节点即可表示整个链表
    int m_size;
};

template <typename T>
inline LinkList<T>::LinkList() : m_size(0) {
    m_entry = allocator<_Node>::allocate();  //附加节点: 只分配空间,不构造元素
    m_entry->m_next = m_entry->m_prev = m_entry;
}

template <typename T>
inline LinkList<T>::Iterator LinkList<T>::insert(const_Iterator& pos,
                                                 const T& value) {
    _Node* ret = new _Node(value);
    ret->m_prev = pos->m_prev;
    ret->m_next = pos->m_next;
    pos->m_prev->m_next = ret;
    pos->m_prev = ret;
    ++m_size;
    return ret;
}
template <typename T>
inline LinkList<T>::Iterator LinkList<T>::insert(const Iterator& pos,
                                                 unsigned int count,
                                                 const T& value) {
    while (--count) insert(pos, value);
}

template <typename T>
inline LinkList<T>::Iterator LinkList<T>::erase(const_Iterator& pos) {
    _Node* ret = pos->m_next;
    pos->m_prev->m_next = pos->m_next;
    pos->m_next->m_prev = pos->m_prev;
    delete pos->m_node;
    --m_size;
    return ret;
}

template <typename T>
inline void LinkList<T>::clear() {
    for (Iterator cur = begin(); cur != end(); cur++) delete cur.m_node;
    m_entry->m_next = m_entry;
    m_entry->m_prev = m_entry;
    m_size = 0;
}

template <typename T>
inline LinkList<T>::LinkList(int count, const T& value) : LinkList() {
    while (--count) this->push_back(value);
}

template <typename T>
inline LinkList<T>::LinkList(const LinkList<T>& other) : LinkList() {
    assign(other.begin(), other.end());
}

template <typename T>
inline LinkList<T>& LinkList<T>::operator=(const LinkList<T>& other) {
    assign(other.begin(), other.end());
    return *this;
}

template <typename T>
void LinkList<T>::assign(Iterator begin, Iterator end) {
    clear();
    while (begin != end) {
        push_back(*begin);
        ++begin;
    }
}

template <typename T>
void LinkList<T>::assign(int count, const T& value) {
    clear();
    while (--count) push_back(value);
}

template <typename T>
void LinkList<T>::remove(const T& value) {
    for (Iterator cur = 0; cur != end;) {
        Iterator p = cur;
        ++cur;
        if (*p == value) {
            erase(p);
            --m_size;
        }
    }
}

}  // namespace tinySTL