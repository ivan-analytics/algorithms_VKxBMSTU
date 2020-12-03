#include <iostream>
#include <stack>

// 2_2. Обход дерева в порядке pre-order

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
    void postOrderDFS( Node* node, void visit(Node* ) );
};

BinTree::BinTree() : root( nullptr )
{
}

BinTree::~BinTree()
{
    postOrderDFS( root, []( Node* node ) { delete node; } );
}

void BinTree::postOrderDFS( Node* node, void visit(Node* ) )
{
    std::stack<Node*> main_stack;
    std::stack<Node*> visit_stack;
    main_stack.push(nullptr);
    main_stack.push(node);
    visit_stack.push(node);

    while(!main_stack.empty()) {
        Node* cur = main_stack.top();
        main_stack.pop();

        if (cur == nullptr) {
            Node* visit_node = visit_stack.top();
            visit_stack.pop();

            visit(visit_node);
        }

        if ((cur != nullptr) && (cur->Right != nullptr)) {
            main_stack.push(nullptr);
            main_stack.push(cur->Right);
            visit_stack.push(cur->Right);
        }
        if ((cur != nullptr) && (cur->Left != nullptr)) {
            main_stack.push(nullptr);
            main_stack.push(cur->Left);
            visit_stack.push(cur->Left);
        }
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

void BinTree::preOrderDFS(Node* node, void visit(int ) )
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