#include<iostream>
using namespace std;

class List {
private:
	class LinkNode
	{
	public:
		int data;
		LinkNode* link;
	};
	LinkNode* first;
	LinkNode* last;
	LinkNode* now;
public:
	void insert(int x);
	void compare( int diff);
	void output(int num);
};
List mylist;
int main()
{
	int num = 0,diff=0;
	cin >> num >> diff;
	for (int i = 0; i < num; i++)
	{
		int x = 0;
		cin >> x;
		mylist.insert(x);
	}
	for (int i = 0; i < num; i++)
	{
		mylist.compare(diff);
			
	}
	mylist.output(num);
	return 0;
}

void List::insert(int x)
{
	LinkNode* newNode= new LinkNode;
	newNode->data = x;
	if (first == nullptr)
	{
		if (newNode == nullptr)
			cerr << "wrong insert!";
		else {
			first = newNode;
			now = first; last = first;
			first->link = nullptr;
		}
	}
	else
	{
		last->link = newNode;
		last=newNode;
		newNode->link = nullptr;
	}
}

void List::compare( int diff)
{
	if (now->data >= diff)
	{
		if (now == first)
		{
			LinkNode* p = now;
			first = first->link;			
			last->link = p;
			last = p;
			now = first;
		}
		else
		{
			LinkNode* p = now;
			LinkNode* q = first;
			while(q->link!=p)
			{
				q = q->link;
			}
			q->link = p->link;
			now = p->link;
			last->link = p;
			last = p;
		}
		
	}
	else
	{
		now = now->link;
	}	
}
void List::output(int num)
{
	LinkNode* p = first;
	for(int k=0;k<(num-1);k++)
	{
		cout << p->data << " ";
		p = p->link;
	}
	cout << p->data;

}

