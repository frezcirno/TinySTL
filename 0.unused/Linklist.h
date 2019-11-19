#pragma once
#include <functional>
template <typename T>
class Linklist {
	struct LinkNode {
		T data;
		LinkNode* next = nullptr;
	};

private:
	LinkNode* m_head;

public:
	Linklist();
	~Linklist();
	int get(int index, T& elem) const;
	//获得索引为index的数据结点,成功返回index
	//失败返回-1
	int count() const;
	//返回表中的数据个数
	int insert(int index, const T& elem);
	//插入一个节点,编号从0开始,成功返回index
	//失败返回-1
	int push_back(const T& elem);
	//在结尾插入一个结点,成功返回节点的index
	//失败返回-1
	int remove(int index);
	//删除一个节点,编号从0开始,成功返回index
	//失败返回-1
	int update(int index, const T& elem);
	//修改一个节点,编号从0开始,成功返回index
	//失败返回-1
	int search(std::function<bool(const T&)>proc) const;
	//查找一个节点,编号从0开始,成功返回该结点的index
	//失败返回-1
	int traversal(std::function<int(const T&)>proc) const;
	//遍历所有节点,当proc返回非0值时结束,返回对应的index
	//全部遍历过返回-1
};

template <typename T>
Linklist<T>::Linklist() {
    m_head = new LinkNode;
    m_head->next = nullptr;
}

template <typename T>
Linklist<T>::~Linklist() {
    LinkNode *p = m_head, *temp;
    while (p != nullptr) {
        temp = p;
        p = p->next;
        delete temp;
    }
}

template <typename T>
int Linklist<T>::get(int index, T& elem) const {
    LinkNode* p = m_head->next;
    int i;
    for (i = 0; i < index && p; i++, p = p->next)
        ;
    if (i != index || !p) {
        return -1;
    }
    elem = p->data;
    return index;
}

template <typename T>
int Linklist<T>::count() const {
    LinkNode* p = m_head->next;
    int i = 0;
    while (p) {
        i++;
        p = p->next;
    }
    return i;
}

template <typename T>
int Linklist<T>::insert(int index, const T& elem) {
    LinkNode* p = m_head;
    int i;
    for (i = 0; i < index && p; i++) {
        p = p->next;
    }
    if (i != index) {
        return i;
    }
    LinkNode* child = new LinkNode;
    child->data = elem;
    child->next = p->next;
    p->next = child;
    return index;
}

template <typename T>
int Linklist<T>::push_back(const T& elem) {
	return insert(count(), elem);
}

template <typename T>
int Linklist<T>::remove(int index) {
    LinkNode* p = m_head;
    int i;
    for (i = 0; i < index && p->next; i++, p = p->next)
        ;
    if (i != index || !p->next) {
        return -1;
    }

    LinkNode* temp = p->next;
    p->next = temp->next;
    delete temp;
    return index;
}

template <typename T>
int Linklist<T>::update(int index, const T& elem) {
    LinkNode* p = m_head;
    int i;
    for (i = 0; i < index && p; i++) {
        p = p->next;
    }
    if (i != index) {
        return i;
    }
    p->next->data = elem;
    return index;
}

template <typename T>
int Linklist<T>::search(std::function<bool(const T&)>proc) const {
	LinkNode* p = m_head->next;
	int i;
	for (i = 0; p; i++, p = p->next) {
		if (proc(p->data)) {
			break;
		}
	}
	if (!p) {
		return -1;
	}
	return i;
}

template <typename T>
int Linklist<T>::traversal(std::function<int(const T&)>proc) const {
	LinkNode* p = m_head->next;
	int i = 0;
	while (p && proc(p->data) == 0) {
		i++;
        p = p->next;
    }
    if (p) return i;
    return -1;
}