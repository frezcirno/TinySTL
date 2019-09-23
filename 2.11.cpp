#include<iostream>
using namespace std;

class List {
	private:
		class LinkNode {
			public:
				int data;
				LinkNode* link;
		};
		LinkNode* first;
		LinkNode* last;
	public:
		List(): first(nullptr), last(nullptr) {	}
		~List() {LinkNode*p; while(first != last) {p = first; first = first->link; delete p;	}}
		void insert(int x);
		void compare(int diff);
		void output();
};
int main() {
	List mylist;
	int num = 0, diff = 0;
	cin >> num >> diff;
	int x = 0;
	for (int i = 0; i < num; i++) {
		cin >> x;
		mylist.insert(x);
	}
	mylist.compare(diff);
	mylist.output();
	return 0;
}

void List::insert(int x) {
	LinkNode* newNode = new LinkNode;
	if (newNode == nullptr) cerr << "wrong insert!";
	newNode->data = x;

	if (first == nullptr) {
		first = newNode;
		first->link = nullptr;
		last = first;
	} else {
		last->link = newNode;
		last = newNode;
		newNode->link = nullptr;
	}
}

void List::compare( int diff) {
	LinkNode *now = first, *pred, *oldLast = last;
	if(now)	while(now != oldLast) {
		LinkNode *next = now->link;
		if(now->data >= diff) {
			last->link = now;
			now->link = nullptr;
			last = now;
			if(now == first)
				now = first = next;
			else{
				pred->link = next;
				now = next;
			}
		} else {
			pred = now;
			now = next;
		}
	}
}
void List::output() {
	LinkNode* p = first;
	if(p) {
		while(p->link != NULL) {
			cout << p->data << " ";
			p = p->link;
		}
		cout << p->data;
	}
}

