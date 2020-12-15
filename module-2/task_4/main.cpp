#include <iostream>
#include <vector>
#include <cassert>
using std::cin;
using std::cout;

// удаление на хабре не оптимально, написать оптимально!
// при удалении брать ключ из того поддерева, что глубже! (чтобы минимизировать количество вращений)

class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    int Add( int key );
    void Delete( int position );
    void InOrderDFS( void visit(int, int ) );

private:
    struct node {
        int key;
        unsigned char height;
        int children_num; // including root node itself

        node* left;
        node* right;

        explicit node(int k) { key = k; left = right = nullptr; height = 1; children_num = 1; }
    };
    node* root;

    void postOrderDFS(AVLTree::node* node, void visit(AVLTree::node*));
    void inOrderDFS( AVLTree::node*, void visit( int, int ) );

    node* remove(node* p, int position);
    std::pair<AVLTree::node*, int> add(node* p, int k, int pos);
    node* balance(node* p);
    node* rotate_right(node* p); // правый поворот вокруг p
    node* rotate_left(node* q); // левый поворот вокруг q

    unsigned char height(node* p);
    int bfactor(node* p);
    void fixheight(node *p);
    int children_num(AVLTree::node* p);
    void fix_children_num(AVLTree::node *p);

    std::pair<AVLTree::node*, AVLTree::node*> find_and_remove_min(AVLTree::node* cur);
    std::pair<AVLTree::node*, AVLTree::node*> find_and_remove_max(AVLTree::node* cur);
    // node* findmin(node* p); // поиск узла с минимальным ключом в дереве p
    // node* removemin(node* p); // удаление узла с минимальным ключом из дерева p
};

unsigned char AVLTree::height(AVLTree::node* p)
{
    return p ? p->height : 0;
}

int AVLTree::children_num(AVLTree::node* p)
{
    return p ? p->children_num : 0;
}

int AVLTree::bfactor(AVLTree::node* p)
{
    return height(p->right) - height(p->left);
}

void AVLTree::fix_children_num(AVLTree::node *p) {
    auto c_left = children_num(p->left);
    auto c_right = children_num(p->right);

    p->children_num = c_left + c_right + 1;
}

void AVLTree::fixheight(AVLTree::node *p) {
    auto h_left = height(p->left);
    auto h_right = height(p->right);

    p->height = (h_right > h_left ? h_right : h_left) + 1;
}


AVLTree::node *AVLTree::rotate_right(AVLTree::node *p) {
    node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fix_children_num(p);
    fixheight(q);
    fix_children_num(q);
    return q;
}

AVLTree::node *AVLTree::rotate_left(AVLTree::node *q) {
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fix_children_num(q);
    fixheight(p);
    fix_children_num(p);
    return p;
}

AVLTree::node* AVLTree::balance(AVLTree::node *p) {
    fixheight(p);
    fix_children_num(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    return p;
}

std::pair<AVLTree::node*, int> AVLTree::add(node* p, int k, int pos) {
    if( !p ) {
        // нашли куда вставлять
        return std::make_pair(new node(k), pos);
    }
    std::pair<AVLTree::node*, int> res;
    if( k < p->key ) {
        res = add(p->left, k, (int)(pos + children_num(p->right) + 1));
        p->left = res.first;
    }
    else {
        res = add(p->right, k, pos);
        p->right = res.first;
    }
    return std::make_pair(balance(p), res.second);
}

int AVLTree::Add(int key) {
    auto res = add(root, key, 0);
    root = res.first;
    return res.second;
}



AVLTree::AVLTree() : root(nullptr) {}

void AVLTree::InOrderDFS( void visit(int, int) )
{
    inOrderDFS(root, visit);
}

void AVLTree::inOrderDFS( AVLTree::node* node, void visit( int, int ) )
{
    if( node == nullptr ) {
        return;
    }

    inOrderDFS(node->left, visit);
    visit( node->key , (int)node->children_num);
    inOrderDFS(node->right, visit);
}

void AVLTree::postOrderDFS(AVLTree::node* node, void visit(AVLTree::node* ) )
{
    if( node == nullptr ) {
        return;
    }

    postOrderDFS(node->left, visit);
    postOrderDFS(node->right, visit);
    visit( node );
}

AVLTree::~AVLTree() {
    postOrderDFS( root, []( node* node ) { delete node; } );
}

AVLTree::node *AVLTree::remove(AVLTree::node *p, int position) {
    if( !p ) return nullptr;
    if( position <= children_num(p->left) )
        p->left = remove(p->left, position);
    else if( position > children_num(p->left) + 1 )
        p->right = remove(p->right, position - (children_num(p->left) + 1));
    else //  position == position of p node
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if( !r ) return q;
        if( !q ) return r;

        // если есть оба поддерева
        if (q->height < r->height) {
            auto res = find_and_remove_max(q);
            node* max = res.second;
            max->left = res.first;
            max->right = r;
            return balance(max);
        } else {
            auto res = find_and_remove_min(r);
            node* min = res.second;
            min->right = res.first;
            min->left = q;
            return balance(min);
        }


    }
    return balance(p);
}

void AVLTree::Delete(int position) {
    root = remove(root, children_num(root) - (position + 1) + 1);
}

//AVLTree::node* AVLTree::findmin(AVLTree::node* p) // поиск узла с минимальным ключом в дереве p
//{
//    return p->left ? findmin(p->left) : p;
//}
//
//AVLTree::node* AVLTree::removemin(AVLTree::node* p) // удаление узла с минимальным ключом из дерева p
//{
//    if( p->left == nullptr )
//        return p->right;
//    p->left = removemin(p->left);
//    return balance(p);
//}

std::pair<AVLTree::node*, AVLTree::node*> AVLTree::find_and_remove_min(AVLTree::node* cur) {
    if(cur->left == nullptr ) {
        // нашли удаляемый узел
        return std::make_pair(cur->right, cur);
    }
    auto res = find_and_remove_min(cur->left);
    cur->left = res.first;
    return std::make_pair(balance(cur), res.second);
}

std::pair<AVLTree::node *, AVLTree::node *> AVLTree::find_and_remove_max(AVLTree::node *cur) {
    if(cur->right == nullptr ) {
        // нашли удаляемый узел
        return std::make_pair(cur->left, cur);
    }
    auto res = find_and_remove_max(cur->right);
    cur->right = res.first;
    return std::make_pair(balance(cur), res.second);
}


const static int COMMAND_ADD = 1;
const static int COMMAND_REMOVE = 2;
int main() {
    AVLTree tree;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int command;
        int param;
        cin >> command;
        cin >> param;

        if (command == COMMAND_ADD) {
            assert(param > 0);
            std::cout << tree.Add(param) << std::endl;
        } else if (command == COMMAND_REMOVE) {
            tree.Delete(param);
        } else assert(false);
    }


    //for (int i = 0; i < 15; i++) std::cout << tree.Add(i) << " ";

//    std::cout << tree.Add(100) << " ";
//    std::cout << tree.Add(200) << " ";
//    std::cout << tree.Add(50) << " ";
//    tree.Delete(1);
//    std::cout << tree.Add(150) << " ";
//
//    std::cout << std::endl;
//    tree.InOrderDFS([](int key, int children_num) { std::cout << key << " "; });
//    std::cout << std::endl;
    // tree.InOrderDFS([](int key, int children_num) { std::cout << key << " " << children_num << std::endl; });

    return 0;
}
