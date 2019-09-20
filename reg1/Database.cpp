#include <iostream>
#include "Database.h"
using namespace std;

istream& operator>>(istream& is, stu_t& elem) {
    string temp;
    is >> elem.id >> elem.name >> elem.gender >> elem.age >> elem.type;
    return is;
}

ostream& operator<<(ostream& os, const stu_t& elem) {
    os << left << setw(10) << elem.id;
    os << left << setw(10) << elem.name;
    os << left << setw(10) << elem.gender;
    os << left << setw(10) << elem.age;
    os << left << setw(10) << elem.type;
    return os;
}

bool database::Build() {
    int count = 0;
    stu_t elem;

    cout << "首先请建立考生信息系统" << endl;
    cout << "请输入考生人数：";
    cin >> count;
    cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
    for (int i = 0; i < count; i++) {
        cin >> elem;
		if (!m_student.insert(elem)) {
			cout << "考号重复" << endl;
		}
    }
    Print();
	cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作）\n\n";
    return true;
};

bool database::Insert() {
    stu_t elem;
    /*cout << "请选择您要插入的考生的位置：";
    cin >> index;
	if (index <= 0) {
		cout << "数字太小了" << endl;
		return -1;
	}
	if (index > m_student.size() + 1) {
		cout << "数字太大了" << endl;
		return -1;
	}
    */
    cout << "请依次输入要插入的考生的考号，姓名，性别，年龄及报考类别！" << endl;
    cin >> elem;
    if (!m_student.insert(elem))
    {
        cout << "学号重复,请检查输入数据" << endl;
	return false;
    }
    return true;
}

bool database::Remove() {
    stu_t elem; 
    cout << "请选择您要删除的考生的考号：";
    cin >> elem.id;
	auto it = m_student.find(elem);
    if (it == m_student.end()) {
        cout << "找不到考号为" << elem.id << "的考生" << endl;
        return false;
    }
    cout << "您删除的考生信息是：" << *it << endl;
	m_student.erase(it);//使用迭代器删除
    return true;
}

bool database::Search() const {
    stu_t elem;
    cout << "请选择您要查找的考生的考号：";
    cin >> elem.id;
	auto it = m_student.find(elem);
	if (it == m_student.end()) {
		cout << "找不到考号为" << elem.id << "的考生" << endl;
		return false;
	}
    cout << "考号      姓名      性别      年龄      报考类别  " << endl;
    cout << *it << endl;
    return true;
}

bool database::Update() {
    stu_t elem;
    cout << "请选择您要修改的考生的考号：";
    cin >> elem.id;
	if (!m_student.erase(elem)) {
		cout << "找不到考号为" << elem.id << "的考生" << endl;
		return false;
	}
    cout << "请依次输入要修改的考生的考号，姓名，性别，年龄及报考类别！" << endl;
    cin >> elem;
	m_student.insert(elem);
    return true;
}

void database::Stat() const {
    cout << "当前系统内一共有 " << m_student.size() << " 名考生" << endl;
}

void database::Print() const {
	cout << endl;
	cout << "考号      姓名      性别      年龄      报考类别  " << endl;
    for (auto it = m_student.begin(); it != m_student.end(); ++it){
	cout << *it << endl;
    }
}

bool database::Loop() {
    string operation;
    cout << "请选择您要进行的操作：";
    cin >> operation;
    switch (operation[0]) {
        case '0':
            return false;
        case '1':
            Insert();
            break;
        case '2':
            Remove();
            break;
        case '3':
            Search();
            break;
        case '4':
            Update();
            break;
        case '5':
            Stat();
            break;
        default:
			cout << "未知操作,请重新输入" << endl;
			cout << "输入提示: 1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作" << endl;
			break;
    }
    Print();
    return true;
}
/*
1234 a 男 1 aaa    8782 b 女 2 bbb    5314 c 男 3 ccc    2456 d 男 1 ddd    7253 e 女 2 eee    9432 f 男 3 fff

*/
