#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include "RedBlack.h"

struct stu_t {
	int id;
	std::string name;
	std::string gender;
	int age;
	std::string type;
	bool operator<(const stu_t& x)const { return id < x.id; }
};
std::istream& operator>>(std::istream& is, stu_t& elem);
std::ostream& operator<<(std::ostream& os, const stu_t& elem);

class database {
private:
	//Linklist<stu_t> m_student;
	RedBlack<stu_t> m_student;


public:
	database() {}
	~database() {}
	bool Build();
	bool Insert();
	bool Remove();
    bool Update();
	bool Search() const;
	void Stat() const;
	void Print() const;
	bool Loop();
};
