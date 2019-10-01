#pragma once
/**
 * 双向循环链表
 * 类STL<list>
 * 简化接口版本
 */
#include "LinkListIterator.h"
template <typename T>
class LinkList {
   public:
    struct LinkListNode {
        T m_elem;
        LinkListNode *m_prev, *m_succ;
        LinkListNode(const T& elem, LinkListNode* prev = nullptr,
                     LinkListNode* succ = nullptr)
            : m_prev(prev), m_succ(succ) {}
    };

    LinkList() : m_front(new LinkListNode(0)), m_end(m_front), m_size(0) {
        m_front->m_succ = m_front->m_prev = m_front;
    }
    ~LinkList() {
        while (m_size > 0) pop_front();
        delete m_front;
    }

    void push_back(const T& elem);
    void pop_back();
    void push_front(const T& elem);
    void pop_front();

    T& accessAt(int index);
    void insertAt(int index, const T& elem);
    void eraseAt(int index);

    int size() { return m_size; }

    typedef LinkListIterator<T> Iterator;
    Iterator begin() { return LinkList<T>::Iterator(m_front->m_succ); }
    Iterator end() { return LinkList<T>::Iterator(m_front); }

   private:
    LinkListNode *m_front, *m_end;
    int m_size;
};

template <typename T>
inline void LinkList<T>::push_back(const T& elem) {
    m_front->m_prev = m_end->m_succ =
        new LinkListNode(elem, m_end, m_end->m_succ);
    m_size++;
}

template <typename T>
inline void LinkList<T>::pop_back() {
    if (!m_size) return;
    LinkList<T>::LinkListNode* p = m_end;
    p->m_prev->m_succ = p->m_succ;
    p->m_succ->m_prev = p->m_prev;
    m_end = p->m_prev;
    delete p;
    m_size--;
}

template <typename T>
inline void LinkList<T>::push_front(const T& elem) {
    m_front->m_succ = m_front->m_succ->m_prev =
        new LinkListNode(elem, m_front, m_front->m_succ);
    m_size++;
}

template <typename T>
inline void LinkList<T>::pop_front() {
    if (!m_size) return;
    LinkList<T>::LinkListNode* p = m_front->m_succ;
    m_front->m_succ = p->m_succ;
    p->m_succ->m_prev = m_front;
    delete p;
    m_size--;
}

template <typename T>
inline T& LinkList<T>::accessAt(int index) {
    if (index < 0 || index >= m_size) throw index;
    LinkList<T>::LinkListNode* p = m_front->m_succ;
    int k = 0;
    while (k < index) p = p->m_succ;
    return p->m_elem;
}

template <typename T>
inline void LinkList<T>::insertAt(int index, const T& elem) {
    if (index < 0 || index > m_size) throw index;
    LinkList<T>::LinkListNode* p = m_front;
    int k = 0;
    while (k < index) p = p->m_succ;
    p->m_succ = p->m_succ->m_prev = new LinkListNode(elem, p, p->m_succ);
    m_size++;
}

template <typename T>
inline void LinkList<T>::eraseAt(int index) {
    if (index < 0 || index >= m_size) throw index;
    LinkList<T>::LinkListNode* p = m_front;
    int k = 0;
    while (k < index) p = p->m_succ;
    LinkList<T>::LinkListNode* q = p->m_succ;
    p->m_succ = q->m_succ;
    q->m_succ->m_prev = p;
    delete q;
    m_size--;
}
