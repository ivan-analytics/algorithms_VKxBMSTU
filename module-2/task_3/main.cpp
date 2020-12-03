#include <iostream>
#include <stack>

// TODO: исправить post order итеративную реализацию в задаче 2 !!!

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

void BinTree::postOrderDFS( Node* node, void visit( Node* ) )
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

class DecartTree {
public:
    DecartTree();
    ~DecartTree();

    void Add( int key, int priority );
    void InOrderDFS(void visit(int ) );

private:
    struct Node {
        Node* Left;
        Node* Right;
        int Priority;
        int Key;

        explicit Node( int key, int priority ) : Left( nullptr ), Right( nullptr ), Key( key ), Priority( priority ) {}
    };
    Node* root;

    void inOrderDFS(int curDepth, int& maxDepth, Node* node, void visit(int ) );
    void postOrderDFS( Node* node, void visit( Node* ) );
    void Split( Node* currentNode, int key, Node*& left, Node*& right );
};

DecartTree::DecartTree() : root(nullptr)
{
}

DecartTree::~DecartTree()
{
    postOrderDFS( root, []( Node* node ) { delete node; } );
}

void DecartTree::Split(Node* currentNode, int key, Node*& left, Node*& right ) {
    if( currentNode == nullptr ) {
        left = nullptr;
        right = nullptr;
    } else if( currentNode->Key <= key ) {
        Split( currentNode->Right, key, currentNode->Right, right );
        left = currentNode;
    } else {
        Split( currentNode->Left, key, left, currentNode->Left );
        right = currentNode;
    }
}

void DecartTree::postOrderDFS(Node* node, void visit(Node* ) ) // для несбалансированного дерева не применимо
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

void DecartTree::Add(int key, int priority )
{
    enum Direction {
        RIGHT,
        LEFT
    };
    Node* newNode = new Node( key, priority );

    // находим первый элемент с меньшим приоритетом
    Direction direction;
    Node* prev = nullptr;
    Node* cur = root;

    // случай если еще нет элементов
    if (cur == nullptr) {
        root = newNode;
        return;
    }

    // случай когда вставляемый элемент имеет наибольший приоритет
    if (cur->Priority < priority) {
        Split(root, key, newNode->Left, newNode->Right);
        root = newNode;
        return;
    }

    while ((cur != nullptr) && (cur->Priority >= priority)) {
        prev = cur;
        if (key >= cur->Key) {
            cur = cur->Right;
            direction = Direction::RIGHT;
        }
        else {
            cur = cur->Left;
            direction = Direction::LEFT;
        }
    }

    // подцепляем родителя к новому элементу
    if (direction == Direction::RIGHT) prev->Right = newNode;
    else prev->Left = newNode;

    Split(cur, key, newNode->Left, newNode->Right);
}


void DecartTree::InOrderDFS(void visit(int ) )
{
    int maxDepth = -1;
    inOrderDFS(1, maxDepth, root, visit);
    std::cout << std::endl << "Height of tree: " << maxDepth;
}

void DecartTree::inOrderDFS(int curDepth, int& maxDepth, Node* node, void visit(int ) )
{
    if( node == nullptr ) {
        return;
    }
    if (curDepth > maxDepth) maxDepth = curDepth;
    inOrderDFS(curDepth+1, maxDepth, node->Left, visit);
    visit( node->Key );
    inOrderDFS(curDepth+1, maxDepth, node->Right, visit);
}



int main()
{
    DecartTree tree;
    BinTree binTree;

    tree.Add( 5, 11 );
    tree.Add( 18, 8 );
    tree.Add( 25, 7 );
    tree.Add( 50, 12 );
    tree.Add( 30, 30 );
    tree.Add( 15, 15 );
    tree.Add( 20, 10 );
    tree.Add( 22, 5 );
    tree.Add( 40, 20 );
    tree.Add( 45, 9 );

    binTree.Add( 5 );
    binTree.Add( 18 );
    binTree.Add( 25 );
    binTree.Add( 50 );
    binTree.Add( 30 );
    binTree.Add( 15 );
    binTree.Add( 20 );
    binTree.Add( 22 );
    binTree.Add( 40 );
    binTree.Add( 45 );

//    size_t n;
//    std::cin >> n;
//    for (size_t i = 0; i < n; i++) {
//        int key;
//        std::cin >> key;
//        tree.Add( key );
//    }

    tree.InOrderDFS([](int key) { std::cout << key << " "; });
    std::cout << std::endl;
    binTree.PreOrderDFS([](int key) { std::cout << key << " "; });

    return 0;
}

//10
//5 11
//18 8
//25 7
//50 12
//30 30
//15 15
//20 10
//22 5
//40 20
//45 9
