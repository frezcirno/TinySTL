#pragma once
#include <algorithm>//使用STL中的swap()及max()
using namespace std;

template <typename K>
class RedBlack
{
	enum RBColor { RED, BLACK };
    struct RBNode {
		K key;
        int height;
        RBColor color; //默认新节点颜色为RED
        RBNode *parent, *left, *right;
        RBNode() : parent(nullptr), left(nullptr), right(nullptr), height(0), color(RED) {}
        RBNode(K key, RBNode *parent = nullptr, RBNode *left = nullptr, RBNode *right = nullptr, int height = 0, RBColor color = RED) : key(key), parent(parent), left(left), right(right), height(height), color(color) {}
        RBNode *uncle(); //返回当前节点父亲的父亲的另一个孩子
        RBNode *succ();  //返回当前节点的直接后继,不存在返回null
    };

public:
	struct iterator	{//单向迭代器
		RBNode* pNode;
		iterator(RBNode* p = nullptr) :pNode(p) {}
		iterator& operator=(iterator& x) { pNode = x->pNode; return *this; }
		K& operator*() { return pNode->key; }
		K* operator->() { return &pNode->key; }
		iterator& operator++() { iterator old = *this; this->pNode = this->pNode->succ(); return old; }
		iterator& operator++(int) { this->pNode = this->pNode->succ(); return *this; }
		bool operator==(iterator& x) { return pNode == x.pNode; }
		bool operator!=(iterator& x) { return pNode != x.pNode; }
	};

    RedBlack() : m_root(nullptr), m_hot(nullptr), m_size(0) {}
	~RedBlack() { clear(); }

	int size() const { return m_size; }
	void clear() { while (m_size > 0) removeAt(m_root); }

	bool insert(const K &key);		//在树中插入elem,插入成功返回true;若elem已存在,返回false
    bool erase(const K &key);     //在树中删除elem,删除成功返回true;若elem不存在,返回false

	iterator begin() const { RBNode* p = m_root; if (p) while (p->left) p = p->left; return iterator(p); }
	iterator end() const { return iterator(); }
	iterator find(const K& key) const { return iterator(search(key)); }
	bool erase(iterator& it) { if (it.pNode) { erase(it.pNode->key); return true; } return false; }


private:
	int m_size;
	RBNode* m_root;
	mutable RBNode* m_hot;
	RBNode*& search(const K& key) const; //命中元素时,返回该元素的父节点指向该元素的指针的引用
									//元素不存在时,返回父节点指向该null元素的指针的引用(相当于 m_hot->left 或 m_hot->right)
									//(树为空时,返回m_root的引用,m_hot为null)
									//同时更新m_hot为该结点的父节点

	RBNode* removeAt(RBNode*& rx);       //功能:在树中删除rx指向的节点
												//返回:取代rx的节点指针
												//副作用:更新m_hot为该结点的父节点

	int updateHeight(RBNode* x); //更新节点的黑高度
								 //公式:黑高度=max(左右节点黑高度)+(当前为黑节点)
	RBNode* zig(RBNode* x);
	RBNode* zag(RBNode* x);
	void solveRed(RBNode* x);   //修复双红缺陷
	void solveBlack(RBNode* x); //修复双黑缺陷
};

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::RBNode::succ()
{
    RBNode *s = this;
    if (right) { //找到右子树中最靠左的节点
        s = right;
        while (s->left)
            s = s->left;
    } else {
        while (s->parent && s->parent->right && s->parent->right == s)
            s = s->parent; //尽可能向左上方移动
        s = s->parent;     //向右上方移动
    }
    return s;
}

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::RBNode::uncle()
{
    RBNode *s = this;
    RBNode *p = s->parent;
    if (!p) return nullptr;
    RBNode *g = p->parent;
    if (!g) return nullptr;
    RBNode *gl = g->left, *gr = g->right;
    return (p == gl) ? gr : gl;
}

#define stature(x) ((!(x)) ? (0) : ((x)->height))

template <typename K>
int RedBlack<K>::updateHeight(RBNode *x)
{
    x->height = max(stature(x->left), stature(x->right)); //外部节点=0
    return (x->color == BLACK ? ++x->height : x->height); //黑节点+1
}

template <typename K> 
typename RedBlack<K>::RBNode *RedBlack<K>::zig(RedBlack<K>::RBNode *x) {
    /* assert: (x != nullptr) && (x->left != nullptr) */
    RBNode* lc = x->left, *lcrc = lc->right, *p = x->parent;
	RBNode*& rpx = (p == nullptr ? m_root : (p->left == x ? p->left : p->right));
    x->left = lcrc; 
    if( lcrc ) lcrc->parent = x;
    updateHeight(x); 
    lc->right = x;
    x->parent = lc;
    updateHeight(lc);
    lc->parent = p;
    rpx = lc; 
    return lc;
}

template <typename K> 
typename RedBlack<K>::RBNode *RedBlack<K>::zag(RedBlack<K>::RBNode *x) {
    /* assert: (x != nullptr) && (x->right != nullptr) */
    RBNode* rc = x->right, *rclc = rc->left, *p = x->parent;
	RBNode*& rpx = (p == nullptr ? m_root : (p->left == x ? p->left : p->right));
    x->right = rclc; 
    if( rclc ) rclc->parent = x; 
    updateHeight(x);
    rc->left = x; 
    x->parent = rc; 
    updateHeight(rc);
    rc->parent = p; 
    rpx = rc; 
    return rc;
}

template <typename K>
typename RedBlack<K>::RBNode *&RedBlack<K>::search(const K &rKey) const{
	if (!m_root || (!(m_root->key < rKey) && !(rKey < m_root->key))) { m_hot = nullptr; return const_cast<RBNode *&>(m_root); }	//单独考虑退化情况
	m_hot = m_root;
    for (;;){
        RBNode *&rpx = (rKey < m_hot->key) ? m_hot->left : m_hot->right;	//此处一定要重新定义rp
        if (!rpx || (!(rKey < rpx->key) && !(rpx->key < rKey))) return rpx; //命中元素时,返回该元素的父节点指向该元素的指针的引用
                                                //查找失败时(!rpx),返回父节点指向该元素(null)的指针的引用(相当于 m_hot->left 或 m_hot->right)
                                                //(树为空时,返回m_root的引用,m_hot为null)
        m_hot = rpx;
    }
}

template <typename K>
bool RedBlack<K>::insert(const K &key)
{
    RedBlack<K>::RBNode *&x = search(key);
    if (x) return false;
    x = new RedBlack<K>::RBNode(key, m_hot, nullptr, nullptr, 0); //(插入一定是在外部节点)
    m_size++;
    solveRed(x);
    return true;
}

template <typename K>
typename RedBlack<K>::RBNode *RedBlack<K>::removeAt(RedBlack<K>::RBNode *&rpx) {  //rx为引用必不为空
    RBNode *x = rpx, *succ = nullptr, *p = x->parent;
    if (!x->left)       succ = rpx = x->right;//无左子树,让右子树(可能为null)取而代之,(rx为引用,附带设置了子节点)
    else if (!x->right) succ = rpx = x->left; //无右子树,让左子树取而代之,(rx为引用,附带设置了子节点)
    else {                          
        succ = x->succ();           //有双子树,必有后继
        swap(x->key, succ->key);  //交换x与x的后继,转化为无左子树的情况
        x = succ;
        p = succ->parent;
        succ = succ->right; 
        (p == rpx ? p->right : p->left) = succ;  //因为x位置已经变化,故手动设置子节点
    }
    if (succ) succ->parent = p;
	delete x; m_size--;
    m_hot = p;          //更新m_hot为删去节点的父亲
    return succ;
}

template <typename K>
bool RedBlack<K>::erase(const K &key)
{
    RBNode *&rx = search(key);
    if (!rx) return false;
    RBNode *r = removeAt(rx);   //r为指向取代x位置的节点的指针,可能为null
    if (m_size == 0) return true; //删除树中最后一个节点(说明树中只有根节点)
    if (m_hot == nullptr) {     //删掉的是根节点(说明树中仅有root和一个红子节点),直接将r染成黑色
        m_root->color = BLACK;
        updateHeight(m_root);
        return true;
    }
    if ( (stature(m_hot->left) == stature(m_hot->right)) &&
        (m_hot->height == (m_hot->color == BLACK ? stature(m_hot->left) + 1 : stature(m_hot->left)))) {
        //m_hot的黑高度没有发生更新,红黑树依旧平衡
        return true;
    }
    if (r && r->color == RED){  //若r为红色(即x为黑色),不违反红黑树性质,将r染成黑色即可
        r->color = BLACK;
        r->height++;
        return true;
    }
    /* assert: r一定为黑色(或为null), m_hot指向父节点(必不为null) */
    solveBlack(r);
    return true;
}

template <typename K>
void RedBlack<K>::solveRed(RedBlack<K>::RBNode *x)
{
    /* 1.插入发生在在根节点
     *  直接染成黑色 */
    if (x->parent == nullptr) {/*x == m_root*/ 
        x->color = BLACK;
        x->height++;
        return;
    }
    /* 2.插入节点的父节点为黑色,不违反红黑树性质 */
    RBNode *p = x->parent;
    if (p->color == BLACK) 
		return;
    /* 3.插入节点的父节点为红色
     *   检查插入节点的uncle节点颜色 */
    RBNode *g = p->parent;
    RBNode *u = x->uncle();

    if (!u || u->color == BLACK) {/* 3.1 插入节点的uncle节点颜色为黑(或为外部节点),进行调整 */
        if (g->left == p) {	//下面先调整颜色再调整拓扑结构,否则高度更新会出问题
            if (p->left == x) {
                p->color = BLACK;
                g->color = RED;
                zig(g);
            } else { 
                x->color = BLACK;
                p->color = g->color = RED;
                zag(p);
                zig(g);
            }
        } else {
            if (p->right == x) { 
                p->color = BLACK;
                g->color = RED;
                zag(g);
            } else { 
                x->color = BLACK;
                p->color = g->color = RED;
                zig(p);
                zag(g);
            }
        }
    } else {/* 3.2 插入节点的uncle节点颜色为红 */
		p->color = BLACK; p->height++;
		u->color = BLACK; u->height++;
        g->color = RED;
        solveRed(g);
    }
}

template <typename K> void RedBlack<K>::solveBlack(RBNode *r){
    /* assert: r一定为黑色(或为null), m_hot指向父节点(必不为null) */
    RBNode *p = (r ? r->parent : m_hot);    //1.2.2 情况递归调用时不能使用m_hot
    if (!p) return; //删去的是根节点,树中只剩x一个节点,不需要调整
    RBNode *s = (p->left == r) ? p->right : p->left;    //sibiling
    if (s->color == BLACK) { // 1.被删去的x的兄弟节点(必定存在)为黑节点
        RBNode *t = nullptr;    //寻找s的红孩子
        if (s->left && s->left->color == RED) t = s->left;
        if (s->right && s->right->color == RED) t = s->right;
        if (t) {    /* 1.1 被删去的x的兄弟节点有红孩子*/
            RBColor pOld = p->color;
            RBNode *mid = nullptr;
            if (p->left == s) { //p,s,t进行3+4重构,左右节点变黑,中间节点保持原来p的颜色
                if (s->left == t) {
                    zig(p);
                    mid = s;
                } else {
                    zag(s);
                    zig(p);
                    mid = t;
                }
            } else {
                if (s->right == t) {
                    zag(p);
                    mid = s;
                } else {
                    zig(s);
                    zag(p);
                    mid = t;
                }
            }
            if (mid->left) mid->left->color = BLACK;
            if (mid->right) mid->right->color = BLACK;
            mid->color = pOld;
        } else {    /* 1.2 被删去的x的兄弟节点没有红孩子*/
            s->color = RED; //s变成红色,与x的删除相抵消
            s->height--;
            if (p && p->color == RED) p->color = BLACK; //1.2.1 p为红色->p变成黑色,修复完成
            else {  //1.2.2 p为黑色->黑高度误差向上传递
                p->height--;
                solveBlack(p);
            }
        }
    } else {    /* 2.被删去的x的兄弟节点为红节点,交换s与p的位置和颜色 */
        if (p->left == s) zig(p); else zag(p);
        s->color = BLACK;
        p->color = RED; 
        solveBlack(r); //再次调用solveBlack,此时sibiling节点必为黑色
    }
}
