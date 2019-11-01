#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct treeNode {
    string m_name;
    vector<treeNode*> m_child;
    treeNode(const string& name) : m_name(name) {}
    ~treeNode() {}
};

class fmyTree {
   public:
    fmyTree() {}
    ~fmyTree() { removeAt(m_root); }
    void Build();
    bool BuildFamily();
    bool AddChild();
    bool BreakFamily();
    bool Rename();
    bool Loop();

   private:
    treeNode* m_root;
    treeNode* searchAt(treeNode* where, const string& name);
    void removeAt(treeNode* parent);
};

void fmyTree::Build() {
    string name;
    cout << "首先建立一个家谱！" << endl;
    cout << "请输入祖先的姓名：";
    cin >> name;
    m_root = new treeNode(name);
    cout << "此家谱的祖先是：" << name << endl;
};

bool fmyTree::BuildFamily() {
    string name;
    cout << "请输入要建立家庭的人的姓名：";
    cin >> name;

    treeNode* parent = searchAt(m_root, name);
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
    for (auto&& it : parent->m_child) {
        cout << it->m_name << "    ";
    }

    cout << endl;
    return true;
}

bool fmyTree::AddChild() {
    string name;
    cout << "请输入要添加子女的人的姓名：";
    cin >> name;

    treeNode* parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    cout << "请输入" << name << "新添加的子女的姓名：";
    cin >> name;
    parent->m_child.push_back(new treeNode(name));

    cout << name << "的第一代子孙是：";
    for (auto&& it : parent->m_child) {
        cout << it->m_name << "    ";
    }

    cout << endl;
    return true;
}

bool fmyTree::BreakFamily() {
    string name;
    cout << "请输入要解散家庭的人的姓名：";
    cin >> name;

    treeNode* parent = searchAt(m_root, name);
    if (!parent) {
        cout << "查无此人！" << endl;
        return false;
    }

    cout << "要解散家庭的人是：" << name << endl;

    cout << name << "的第一代子孙是：";
    for (auto&& it : parent->m_child) cout << it->m_name << "    ";
    cout << endl;

    for (int i = 0; i < parent->m_child.size(); i++) {
        removeAt(parent->m_child[i]);
    }

    parent->m_child.clear();
    return true;
}

bool fmyTree::Rename() {
    string name;
    cout << "请输入要修改姓名的人的目前姓名：";
    cin >> name;

    treeNode* parent = searchAt(m_root, name);
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

bool fmyTree::Loop() {
    string op;
    cout << endl << "请输入要执行的操作：";
    cin >> op;
    switch (op[0]) {
        case 'A':
            BuildFamily();
            break;
        case 'B':
            AddChild();
            break;
        case 'C':
            BreakFamily();
            break;
        case 'D':
            Rename();
            break;
        case 'E':
            return false;
            break;
        default:
            cout << "输入有误" << endl;
            break;
    }
    return true;
}

treeNode* fmyTree::searchAt(treeNode* where, const string& name) {
    if (where->m_name == name) return where;
    for (int i = 0; i < where->m_child.size(); i++) {
        treeNode* res = searchAt(where->m_child[i], name);
        if (res) return res;
    }
    return nullptr;
}

void fmyTree::removeAt(treeNode* where) {
    for (int i = 0; i < where->m_child.size(); i++) {
        removeAt(where->m_child[i]);
    }
    delete where;
}

int main() {
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
    while (tree.Loop())
        ;
    return 0;
}
