#include <iostream>

// удаление на хабре не оптимально, написать оптимально!
// при удалении брать ключ из того поддерева, что глубже! (чтобы минимизировать количество вращений)

class AVLTree {
public:
    AVLTree();
    ~AVLTree();

    void Add( int key );
    void Delete( int key );
    void InOrderDFS( void visit(int ) );

private:
    struct node {
        int key;
        unsigned char height;

        node* left;
        node* right;

        explicit node(int k) { key = k; left = right = nullptr; height = 1; }
    };
    node* root;

    void postOrderDFS(AVLTree::node* node, void visit(AVLTree::node*));
    void inOrderDFS( AVLTree::node*, void visit( int ) );

    node* remove(node* p, int k);
    node* add(node* p, int k);
    node* balance(node* p);
    node* rotate_right(node* p); // правый поворот вокруг p
    node* rotate_left(node* q); // левый поворот вокруг q

    unsigned char height(node* p);
    int bfactor(node* p);
    void fixheight(node *p);

    // TODO: объединить чтобы все делалось за один обход
    // TODO: при удалении брать ключ из того поддерева, что глубже! (чтобы минимизировать количество вращений)
    node* findmin(node* p); // поиск узла с минимальным ключом в дереве p
    node* removemin(node* p); // удаление узла с минимальным ключом из дерева p
};

unsigned char AVLTree::height(AVLTree::node* p)
{
    return p ? p->height : 0;
}

int AVLTree::bfactor(AVLTree::node* p)
{
    return height(p->right) - height(p->left);
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
    fixheight(q);
    return q;
}

AVLTree::node *AVLTree::rotate_left(AVLTree::node *q) {
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

AVLTree::node* AVLTree::balance(AVLTree::node *p) {
    fixheight(p);
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

AVLTree::node* AVLTree::add(node* p, int k) {
    if( !p ) return new node(k);
    if( k < p->key )
        p->left = add(p->left, k);
    else
        p->right = add(p->right, k);
    return balance(p);
}

void AVLTree::Add(int key) {
    root = add(root, key);
}

AVLTree::AVLTree() : root(nullptr) {}

void AVLTree::InOrderDFS( void visit(int ) )
{
    inOrderDFS(root, visit);
}

void AVLTree::inOrderDFS( AVLTree::node* node, void visit( int ) )
{
    if( node == nullptr ) {
        return;
    }

    inOrderDFS(node->left, visit);
    visit( node->key );
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

AVLTree::node *AVLTree::remove(AVLTree::node *p, int k) {
    if( !p ) return nullptr;
    if( k < p->key )
        p->left = remove(p->left,k);
    else if( k > p->key )
        p->right = remove(p->right,k);
    else //  k == p->key
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if( !r ) return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

void AVLTree::Delete(int key) {
    root = remove(root, key);
}

AVLTree::node* AVLTree::findmin(AVLTree::node* p) // поиск узла с минимальным ключом в дереве p
{
    return p->left?findmin(p->left):p;
}

AVLTree::node* AVLTree::removemin(AVLTree::node* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left == nullptr )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}


int main() {
    AVLTree tree;

    for (int i = 0; i < 15; i++) tree.Add(i);

    tree.InOrderDFS([](int key) { std::cout << key << " "; });

    return 0;
}
