#include "../tinySTL/map.hpp"
#include "../tinySTL/pair.hpp"
#include "student.h"
#include <iomanip>
#include <iostream>

using tinySTL::make_pair;
using tinySTL::map;
using tinySTL::pair;

class database
{
  public:
    // private:
    map<int, student> table;

  public:
    database() {}
    ~database() {}
    void init();
    void insert();
    void erase();
    void update();
    void search() const;
    void stat() const;
    void list() const;
};

void database::init()
{
    int count = 0;
    std::cout << "首先请建立考生信息系统" << std::endl;
    std::cout << "请输入考生人数：";
    std::cin >> count;
    std::cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！"
              << std::endl;
    student stu;
    for (int i = 0; i < count; i++) {
        std::cin >> stu;
        if (table.count(stu.id()))
            std::cout << "考号重复" << std::endl;
        else
            table[stu.id()] = stu;
    }
    list();
    std::cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为"
                 "统计，0为取消操作）\n\n";
};

void database::insert()
{
    int id;
    std::cout << "请输入要插入的考生的考号：";
    std::cin >> id;
    if (table.count(id)) {
        std::cout << "考号重复" << std::endl;
        return;
    }
    std::cout << "请依次输入考生的姓名，性别，年龄及报考类别: ";
    student stu(id);
    input_no_id(std::cin, stu);
    table[stu.id()] = stu;
}

void database::erase()
{
    int id;
    std::cout << "请选择您要删除的考生的考号：";
    std::cin >> id;
    auto it = table.find(id);
    if (it == table.end()) {
        std::cout << "找不到考号为 " << id << " 的考生" << std::endl;
        return;
    }
    std::cout << "您删除的考生信息是：" << it->second << std::endl;
    table.erase(it);  //使用迭代器删除
}

void database::search() const
{
    int id;
    std::cout << "请选择您要查找的考生的考号：";
    std::cin >> id;
    auto it = table.find(id);
    if (it == table.end()) {
        std::cout << "找不到考号为 " << id << " 的考生" << std::endl;
        return;
    }
    std::cout << "考号      姓名      性别      年龄      报考类别  "
              << std::endl;
    std::cout << it->second << std::endl;
}

void database::update()
{
    int id;
    std::cout << "请选择您要修改的考生的考号：";
    std::cin >> id;
    auto it = table.find(id);
    if (it == table.end()) {
        std::cout << "找不到考号为 " << id << " 的考生" << std::endl;
        return;
    }
    std::cout << "请依次输入要修改的考生的姓名，性别，年龄及报考类别！"
              << std::endl;
    student stu(id);
    input_no_id(std::cin, stu);
    it->second = stu;
}

void database::stat() const
{
    std::cout << "当前系统内一共有 " << table.size() << " 名考生" << std::endl;
}

void database::list() const
{
    std::cout << "\n考号      姓名      性别      年龄      报考类别\n";
    if (table.size() == 0) {
        std::cout << "系统内暂无数据" << std::endl;
        return;
    }
    for (auto it = table.begin(); it != table.end(); ++it)
        std::cout << it->second << std::endl;
}

int main(int argc, char const *argv[])
{
    database db;
    db.init();

    std::string operation;
    bool        quit = false;
    while (!quit) {
        std::cout << "请选择您要进行的操作：";
        std::cin >> operation;
        switch (operation[0]) {
            case '0': quit = true; break;
            case '1': db.insert(); break;
            case '2': db.erase(); break;
            case '3': db.search(); break;
            case '4': db.update(); break;
            case '5': db.stat(); break;
            default:
                std::cout << "未知操作,请重新输入" << std::endl;
                std::cout << "输入提示: "
                             "1为插入，2为删除，3为查找，4为修改，5为统计，0为"
                             "取消操作"
                          << std::endl;
                break;
        }

        if (db.table.size() < 50)
            db.list();
        else
            std::cout << "当前系统内共有 " << db.table.size() << " 条数据"
                      << std::endl;
    }
    return 0;
}
