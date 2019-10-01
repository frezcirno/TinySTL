#pragma once

template <typename T>
class LinkList;  //前置声明

template <typename T>
class LinkListIterator {
   public:
    LinkListIterator(typename LinkList<T>::LinkListNode* node) : m_node(node) {}
    ~LinkListIterator() {}
    LinkListIterator& operator++() {
        this->m_node = m_node->m_succ;
        return *this;
    }
    LinkListIterator& operator--() {
        m_node = m_node->m_prec;
        return *this;
    }
    const LinkListIterator operator++(int) {
        LinkListIterator origin(*this);
        ++*this;
        return origin;
    }
    const LinkListIterator operator--(int) {
        LinkListIterator origin(*this);
        --*this;
        return origin;
    }
    bool operator==(const LinkListIterator& rhs) {
        return m_node == rhs.m_node;
    }
    bool operator!=(const LinkListIterator& rhs) {
        return m_node != rhs.m_node;
    }
    T& operator*() { return m_node->m_elem; }
    T& operator->() { return *this; }

   private:
    typename LinkList<T>::LinkListNode* m_node;
};