#pragma once
template <typename T>
class LinkList {
    public:
        LinkList():
            m_front(new LinkListNode(0xabcdabcd, nullptr, nullptr)),
            m_end(m_front),
            m_size(0) {}
        ~LinkList() { while( size() > 0 ) pop_front(); }
        void push_back(const T& elem);
        void pop_back();
        void push_front(const T& elem);
        void pop_front();

        T& accessAt(int index);
        void insertAt(int index, const T& elem);
        void eraseAt(int index);
        
        int size() { return m_size; }
    private:
        struct LinkListNode{
            T m_elem;
            LinkListNode *m_prev, *m_succ;
            LinkListNode(const T& elem,LinkListNode *m_prev = nullptr, LinkListNode *m_succ = nullptr):m_prev(prev),m_succ(succ){}
        };
        LinkListNode *m_front, *m_end;
        int m_size;
}

template <typename T>
inline void LinkList::push_back(const T& elem){
    m_end->m_succ = new LinkListNode(elem, m_end, m_end->m_succ);
    m_size++;
}

template <typename T>
inline void LinkList::pop_back(){
    if (!m_size) return ;
    m_end = m_end->m_prev;
    delete m_end->m_succ;
    m_end->m_succ = nullptr;
}

template <typename T>
inline void LinkList::push_front(const T& elem){
    m_front->m_succ = new LinkListNode(elem, m_front, m_front->m_succ);
    m_size++;
}

template <typename T>
inline void LinkList::pop_front(){
    if(!m_size) return;
    LinkListNode *p = m_front->m_succ;
    m_front->m_succ = p->m_succ;
    p->m_succ->m_prev = m_front;
    delete p;
    m_size--;
}

template <typename T>
inline T& LinkList::accessAt(int index){
    if (index > m_size) throw;
    LinkListNode *p = m_front;
    int k=0;
    while (k<index) {
        p=p->m_succ;
        
    }
    return 
}
template <typename T>
inline void LinkList::insertAt(int index, const T& elem);
template <typename T>
inline void LinkList::eraseAt(int index);
