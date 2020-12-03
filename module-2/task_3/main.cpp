#include <iostream>
#include <stack>
#include <queue>
#include <cassert>

// 3_2. Декартово дерево. Разница ширин

class BinTree {
public:
    BinTree();
    ~BinTree();

    void Add( int key );
    void PreOrderDFS(void visit(int ) );

    int GetMaxWidth();

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

int BinTree::GetMaxWidth() {
    std::queue<Node*> queue;
    queue.push(root);
    queue.push(nullptr);

    int maxWidth = -1;

    while (!queue.empty()) {
        Node* cur = queue.front();
        queue.pop();

        // случай начала нового слоя
        if (cur == nullptr) {
            if (queue.empty()) break;
            int layerWidth = queue.size();
            if (layerWidth > maxWidth) maxWidth = layerWidth;

            queue.push(nullptr);
        }
        else {
            if (cur->Right != nullptr) queue.push(cur->Right);
            if (cur->Left != nullptr) queue.push(cur->Left);
        }
    }

    return maxWidth;
}

class DecartTree {
public:
    DecartTree();
    ~DecartTree();

    void Add( int key, int priority );
    void InOrderDFS(void visit(int ));

    int GetMaxWidth();

private:
    struct Node {
        Node* Left;
        Node* Right;
        int Priority;
        int Key;

        explicit Node( int key, int priority ) : Left( nullptr ), Right( nullptr ), Key( key ), Priority( priority ) {}
    };
    Node* root;

    void inOrderDFS(Node* node, void visit(int ) );
    void postOrderDFS( Node* node, void visit( Node* ) );
    void split(Node* currentNode, int key, Node*& left, Node*& right );
};

DecartTree::DecartTree() : root(nullptr)
{
}

DecartTree::~DecartTree()
{
    postOrderDFS( root, []( Node* node ) { delete node; } );
}

void DecartTree::split(Node* currentNode, int key, Node*& left, Node*& right ) {
    if( currentNode == nullptr ) {
        left = nullptr;
        right = nullptr;
    } else if( currentNode->Key <= key ) {
        split(currentNode->Right, key, currentNode->Right, right);
        left = currentNode;
    } else {
        split(currentNode->Left, key, left, currentNode->Left);
        right = currentNode;
    }
}

void DecartTree::postOrderDFS(Node* node, void visit(Node* ) ) // для несбалансированного дерева не применимо
{
    if( node == nullptr ) {
        return;
    }

    postOrderDFS(node->Left, visit);
    postOrderDFS(node->Right, visit);
    visit( node );
}

int DecartTree::GetMaxWidth() {
    std::queue<Node*> queue;
    queue.push(root);
    queue.push(nullptr);

    int maxWidth = -1;

    while (!queue.empty()) {
        Node* cur = queue.front();
        queue.pop();

        // случай начала нового слоя
        if (cur == nullptr) {
            if (queue.empty()) break;
            int layerWidth = queue.size();
            if (layerWidth > maxWidth) maxWidth = layerWidth;

            queue.push(nullptr);
        }
        else {
            if (cur->Right != nullptr) queue.push(cur->Right);
            if (cur->Left != nullptr) queue.push(cur->Left);
        }
    }

    return maxWidth;
}

void DecartTree::Add(int key, int priority )
{
    enum Direction {
        RIGHT,
        LEFT,
        FAILED
    };
    Node* newNode = new Node( key, priority );

    // находим первый элемент с меньшим приоритетом
    Direction direction = Direction::FAILED;
    Node* prev = nullptr;
    Node* cur = root;

    // случай если еще нет элементов
    if (cur == nullptr) {
        root = newNode;
        return;
    }

    // случай когда вставляемый элемент имеет наибольший приоритет
    if (cur->Priority < priority) {
        split(root, key, newNode->Left, newNode->Right);
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

    if (direction == Direction::FAILED) assert(false);

    // подцепляем родителя к новому элементу
    if (direction == Direction::RIGHT) prev->Right = newNode;
    else prev->Left = newNode;

    split(cur, key, newNode->Left, newNode->Right);
}


void DecartTree::InOrderDFS(void visit(int ))
{
    inOrderDFS(root, visit);
}

void DecartTree::inOrderDFS(Node* node, void visit( int ) )
{
    if( node == nullptr ) {
        return;
    }

    inOrderDFS(node->Left, visit);
    visit( node->Key );
    inOrderDFS(node->Right, visit);
}

int main()
{
    DecartTree tree;
    BinTree binTree;

    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        int key, priority;
        std::cin >> key >> priority;
        tree.Add( key, priority );
        binTree.Add( key );
    }

    std::cout << tree.GetMaxWidth() - binTree.GetMaxWidth() << std::endl;

    return 0;
}

