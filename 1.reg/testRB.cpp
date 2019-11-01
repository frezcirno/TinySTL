#include <iostream>
#include "RedBlack.h"
#include <stdlib.h>
#include <time.h>
#include <set>
#include <vector>
using namespace std;

struct Stud
{
	string name;
	int id;
	int gender;
	Stud(char* name, int id, int gender) :name(name), id(id), gender(gender) {}
	bool operator<(const Stud& x)const { return id < x.id; }
	bool operator>(const Stud& x)const { return id > x.id; }
	bool operator==(const Stud& x)const { return id == x.id; }
	bool operator!=(const Stud& x)const { return id != x.id; }
};

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    vector<Stud> v;
    for (int i = 0; i < 100; i++)
    {
		
		v.push_back(Stud("wangxiaoming", rand(), rand() % 2));
    }
    
    RedBlack<Stud> rbt;
	
    
    for (int i = 0; i < 100; i++)
    {
        rbt.insert(v[i]);
    }

	auto it = rbt.find(v[0]);
	cout << endl << it->id << it->name << it->gender << endl;

    return 0;
}
