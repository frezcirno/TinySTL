#include <iostream>
#include <string>
#include "../tinySTL/vector.hpp"
using namespace std;

struct treeNode {
    string m_name;
    tinySTL::vector<treeNode *> m_child;
    treeNode(const string &name) : m_name(name) {}
};

class fmyTree
{
  public:
    fmyTree() {}
    ~fmyTree() { removeBelow(m_root); }
    void Build();
    bool BuildFamily();
    bool AddChild();
    bool BreakFamily();
    bool Rename();
    void printTree()
    {
        tinySTL::vector<bool> tmp;
        _printTree(m_root, tmp);
    }

  private:
    treeNode *m_root = nullptr;

    treeNode *searchAt(treeNode *where, const string &name)
    {
        if (where->m_name == name) return where;
        for (auto it = where->m_child.begin(); it != where->m_child.end();
             ++it) {
            treeNode *res = searchAt(*it, name);
            if (res) return res;
        }
        return nullptr;
    }

    void removeBelow(treeNode *where)
    {
        for (auto it = where->m_child.begin(); it != where->m_child.end();
             ++it) {
            removeBelow(*it);
        }
        delete where;
    }
    void _printTree(treeNode *tree, tinySTL::vector<bool> &prefix);
};

void fmyTree::Build()
{
    string name;
    cout << "首先建立一个家谱！" << endl;
    cout << "请输入祖先的姓名：";
    cin >> name;
    m_root = new treeNode(name);
    cout << "此家谱的祖先是：" << name << endl;
};

bool fmyTree::BuildFamily()
{
    string name;
    cout << "请输入要建立家庭的人的姓名：";
    cin >> name;

    treeNode *parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    int count;
    cout << "请输入" << name << "的儿女人数：";
    cin >> count;
    cout << "请依次输入" << name << "的儿女的姓名：";
    for (int i = 0; i < count; i++) {
        cin >> name;
        parent->m_child.push_back(new treeNode(name));
    }
    cout << name << "的第一代子孙是：";
    for (auto it = parent->m_child.begin(); it != parent->m_child.end(); ++it)
        cout << (*it)->m_name << "    ";
    cout << endl;

    return true;
}

bool fmyTree::AddChild()
{
    string name;
    cout << "请输入要添加子女的人的姓名：";
    cin >> name;

    treeNode *parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    cout << "请输入 " << name << " 新添加的子女的姓名：";
    cin >> name;
    parent->m_child.push_back(new treeNode(name));

    cout << name << " 的第一代子孙是：";
    for (auto it = parent->m_child.begin(); it != parent->m_child.end(); ++it)
        cout << (*it)->m_name << "    ";
    cout << endl;

    return true;
}

bool fmyTree::BreakFamily()
{
    string name;
    cout << "请输入要解散家庭的人的姓名：";
    cin >> name;

    treeNode *parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    cout << "要解散家庭的人是：" << name << endl;

    cout << name << " 的第一代子孙是：";
    for (auto it = parent->m_child.begin(); it != parent->m_child.end(); ++it) {
        cout << (*it)->m_name << "    ";
        removeBelow(*it);
    }
    cout << endl;

    parent->m_child.clear();
    return true;
}

bool fmyTree::Rename()
{
    string name;
    cout << "请输入要修改姓名的人的目前姓名：";
    cin >> name;

    treeNode *parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    cout << "请输入更改后的姓名：";
    cin >> name;

    cout << parent->m_name << "已更名为" << name << endl;
    parent->m_name = name;
    return true;
}

void fmyTree::_printTree(treeNode *tree, tinySTL::vector<bool> &prefix)
{
    int i;
    int size = prefix.size();
    for (i = 0; i < size - 1; i++) printf("%s", (prefix[i] ? "│  " : "   "));
    for (; i < size; i++) printf("%s", (prefix[i] ? "├--" : "└--"));

    cout << tree->m_name << endl;
    
	size = tree->m_child.size();
    prefix.push_back(true);
    for (i = 0; i < size - 1; i++) _printTree(tree->m_child[i], prefix);
    prefix.back() = false;
    for (; i < size; i++) _printTree(tree->m_child[i], prefix);
    prefix.pop_back();
}

int main()
{
    fmyTree tree;
    cout << "==================================================" << endl;
    cout << "**            请选择要执行的操作：              **" << endl;
    cout << "**              A --- 完善家庭                  **" << endl;
    cout << "**              B --- 添加家庭成员              **" << endl;
    cout << "**              C --- 解散局部家庭              **" << endl;
    cout << "**              D --- 更改家庭成员姓名          **" << endl;
    cout << "**              E --- 退出程序                  **" << endl;
    cout << "==================================================" << endl;
    tree.Build();

    string op;
    bool quit = false;
    while (!quit) {
        cout << endl << "请输入要执行的操作：";
        cin >> op;
        switch (toupper(op[0])) {
            case 'A': tree.BuildFamily(); break;
            case 'B': tree.AddChild(); break;
            case 'C': tree.BreakFamily(); break;
            case 'D': tree.Rename(); break;
            case 'E': quit = true; break;
            default: cout << "输入有误" << endl; break;
        }
        tree.printTree();
    }
    return 0;
}
