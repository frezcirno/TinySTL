#pragma once
#include <iostream>
#include <string>

class student
{
  private:
    int         _id;
    std::string _name;
    enum { male, female } _gender;
    unsigned    _age;
    std::string _type;

  public:
    student() {}
    student(int iid) : _id(iid) {}
    int id() const { return _id; }

    friend std::istream &operator>>(std::istream &is, student &elem);
    friend std::ostream &operator<<(std::ostream &os, const student &elem);

    friend void input_no_id(std::istream &is, student &elem);
};

std::istream &operator>>(std::istream &is, student &elem)
{
    int         iint;
    std::string istr;

    is >> iint;
    elem._id = iint;
    is >> istr;
    elem._name = istr;
    is >> istr;
    if (istr == "男")
        elem._gender = elem.male;
    else if (istr == "女")
        elem._gender = elem.female;
    is >> iint;
    elem._age = iint;
    is >> istr;
    elem._type = istr;
    return is;
}

void input_no_id(std::istream &is, student &elem)
{
    int         iint;
    std::string istr;

    is >> istr;
    elem._name = istr;
    is >> istr;
    if (istr == "男")
        elem._gender = elem.male;
    else if (istr == "女")
        elem._gender = elem.female;
    is >> iint;
    elem._age = iint;
    is >> istr;
    elem._type = istr;
}

std::ostream &operator<<(std::ostream &os, const student &elem)
{
    printf("%-10d%-10s%-10s%-10d%-10s", elem._id, elem._name.c_str(),
        (elem._gender == elem.male ? "男" : "女"), elem._age,
        elem._type.c_str());
    return os;
}