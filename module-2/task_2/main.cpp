#include <iostream>

class BinTree {
public:
    BinTree();
    ~BinTree();

    void Add( int key );
    void PreOrderDFS(void visit(int ) );

private:
    struct Node {
        Node* Left;
        Node* Right;
        int Key;

        explicit Node( int key ) : Left( nullptr ), Right( nullptr ), Key( key ) {}
    };
    Node* root;

    void add( Node*& node, int key );
    void preOrderDFS(Node* node, void visit(int ) );
    void postOrderDFS( Node* node, void visit( Node* ) );
};

BinTree::BinTree() : root( nullptr )
{
}

BinTree::~BinTree()
{
    postOrderDFS( root, []( Node* node ) { delete node; } );
}

// TODO: заменить рекурсивный обход итеративным
void BinTree::postOrderDFS( Node* node, void visit( Node* ) ) // для несбалансированного дерева не применимо
{
    if( node == nullptr ) {
        return;
    }

    postOrderDFS( node->Left, visit );
    postOrderDFS( node->Right, visit );
    visit( node );
}

void BinTree::Add( int key )
{
    add( root, key );
}

void BinTree::add( Node*& node, int key )
{
    if( node == nullptr ) {
        node = new Node( key );
        return;
    }

    if( key < node->Key ) {
        add( node->Left, key );
    } else {
        add( node->Right, key );
    }
}

void BinTree::PreOrderDFS(void visit(int ) )
{
    preOrderDFS(root, visit);
}

// TODO: заменить рекурсивный обход итеративным
void BinTree::preOrderDFS(Node* node, void visit(int ) ) // для несбалансированного дерева не применимо
{
    if( node == nullptr ) {
        return;
    }

    visit( node->Key );
    preOrderDFS(node->Left, visit);
    preOrderDFS(node->Right, visit);
}

int main()
{
    BinTree tree;
    tree.Add( 10 );
    tree.Add( 5 );
    tree.Add( 20 );
    tree.Add( 1 );
    tree.Add( 7 );
    tree.Add( 15 );
    tree.Add( 17 );

    tree.PreOrderDFS([](int key) { std::cout << key << " "; });

    return 0;
}