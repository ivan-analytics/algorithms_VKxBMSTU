#include <iostream>
#include <stack>

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
    std::stack<Node*> stack;
    stack.push(node);

    while(!stack.empty()) {
        Node* cur = stack.top();
        stack.pop();

        if (cur->Right != nullptr) stack.push(cur->Right);
        if (cur->Left != nullptr) stack.push(cur->Left);
        visit(cur);
    }
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
    std::stack<Node*> stack;
    stack.push(node);

    while(!stack.empty()) {
        Node* cur = stack.top();
        stack.pop();

        visit(cur->Key);
        if (cur->Right != nullptr) stack.push(cur->Right);
        if (cur->Left != nullptr) stack.push(cur->Left);
    }
}

int main()
{
    BinTree tree;
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        int key;
        std::cin >> key;
        tree.Add( key );
    }

    tree.PreOrderDFS([](int key) { std::cout << key << " "; });

    return 0;
}