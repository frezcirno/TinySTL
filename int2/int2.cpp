#include <iostream>
using namespace std;

struct LinkNode {
	int data;
	LinkNode* next = nullptr;
	LinkNode(int elem, LinkNode* next = nullptr) :data(elem), next(next) {}
};

class Linklist {
	LinkNode* m_head;
	int m_size;
public:
	Linklist() :m_head(nullptr), m_size(0) {}
	~Linklist() { LinkNode* x, * p = m_head; while (p) { x = p; p = p->next; delete x; } }
	bool get(int index, int& elem) const;
	void push_back(int elem);
	int count() { return m_size; }
	void print() { LinkNode* p = m_head; while (p) { cout << p->data << " "; p = p->next; } }
	bool insert(int i, int& elem);
	bool erase(int i, int& elem);
};

bool Linklist::get(int index, int& elem) const {
	LinkNode* p = m_head;	int i;
	for (i = 0; i < index && p; i++, p = p->next) continue;
	if (i != index || !p) return false;
	elem = p->data;
	return true;
}

void Linklist::push_back(int elem) {
	LinkNode ** rpx = &m_head;
	while (*rpx) rpx = &(**rpx).next;
	*rpx = new LinkNode(elem);
	m_size++;
}

bool Linklist::insert(int i, int &elem) {
	LinkNode **pp = &m_head; int k = 0;
	while((*pp) && (k < i)) { pp = &(**pp).next; k++; }
	if (k != i) return false;
	*pp = new LinkNode(elem, (*pp)->next);
	return true;
}

bool Linklist::erase(int i, int &elem) {
	LinkNode **pp = &m_head; int k = 0;
	while((*pp) && (k < i)) { pp = &(**pp).next; k++; }
	if (k != i) return false;
	elem = (**pp).data; delete *pp;
	return true;
}

void buildlist(Linklist& l) {
	int i;
	while (cin >> i && i != -1) l.push_back(i);
}

void intersect(Linklist& l1, Linklist& l2, Linklist& l3) {
	int i1, i2;
	int a, b;
	i1 = i2 = 0;
	while (true)
	{
		if (!l1.get(i1, a) || !l2.get(i2, b)) return;
		if (a < b) i1++;
		else if (a > b) i2++;
		else {  i1++; i2++; l3.push_back(a);  }
	}
}

int main() {
	Linklist l1, l2, l3;
	buildlist(l1);	buildlist(l2);
	intersect(l1, l2, l3);
	if (l3.count() == 0) cout << "NULL"; 
	else l3.print();
	return 0;
}
