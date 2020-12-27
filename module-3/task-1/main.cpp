#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <queue>
#include <deque>
#include <unordered_set>
#include <iostream>

using std::queue;
using std::deque;
using std::cin;
using std::cout;

void BFS( const IGraph& graph, int vertex, void (*visit)(int) )
{
    vector<bool> visited( graph.VerticesCount(), false );
    queue<int> bfsQueue;
    bfsQueue.push( vertex );
    visited[vertex] = true;

    while( !bfsQueue.empty() ) {
        int current = bfsQueue.front();
        bfsQueue.pop();
        visit( current );
        vector<int> adjacentVeticies = graph.GetNextVertices( current );
        for(int & adjacentVeticie : adjacentVeticies)
            if( !visited[adjacentVeticie] ) {
                bfsQueue.push( adjacentVeticie );
                visited[adjacentVeticie] = true;
            }
    }
}

#define NULL_INT -1
int min_paths_num(const IGraph& graph, int from, int to) {
    vector<bool> visited( graph.VerticesCount(), false );
    unordered_set<int> prev_layers_vertices;
    vector<int> min_paths_cnt( graph.VerticesCount(), 0 );
    deque<int> bfsQueue;
    bfsQueue.push_back(NULL_INT);
    bfsQueue.push_back(from);
    visited[from] = true;
    min_paths_cnt[from] = 1;

    while( !bfsQueue.empty() ) {
        int current = bfsQueue.front();
        bfsQueue.pop_front();
        // заполняем множество вершин текущим слоем
        if (current == NULL_INT) {
            for (auto it = bfsQueue.begin(); *it != NULL_INT && it != bfsQueue.end(); it++) {
                prev_layers_vertices.insert(*it);
            }
            continue;
        }
        vector<int> adjacentVeticies = graph.GetNextVertices(current);
        bfsQueue.push_back(NULL_INT);
        for (int &adjacentVeticie : adjacentVeticies) {
            if (!visited[adjacentVeticie]) {
                bfsQueue.push_back(adjacentVeticie);
                visited[adjacentVeticie] = true;
            }

            // если вершина не из множества вершин предыдущих/текущего слоев,
            // то добавляем к ней возможные минимальные пути
            auto search = prev_layers_vertices.find(adjacentVeticie);
            if (search == prev_layers_vertices.end())
                min_paths_cnt[adjacentVeticie] += min_paths_cnt[current];
        }
    }

    return min_paths_cnt[to];
}

int main()
{
//  5 --------_
//  |           \
//  |            \
//  |            |
//  0 ----> 1    |
//  | \  /    \ /
//  3   4 ---> 2 ------> 6 ----- > 7
//    int v; cin >> v;
    ListGraph graph(8);
//    int n; cin >> n;
//    for (int i = 0; i < n; i++) {
//        int from; int to;
//        cin >> from >> to;
//        graph.AddEdge(from, to);
//    }
//    int u; int w;
//    cin >> u >> w;



    graph.AddEdge( 0, 1 );
    graph.AddEdge( 0, 3 );
    graph.AddEdge( 0, 4 );
    graph.AddEdge( 1, 4 );
    graph.AddEdge( 4, 2 );
    graph.AddEdge( 1, 2 );
    graph.AddEdge( 0, 5 );
    graph.AddEdge( 5, 2 );
    graph.AddEdge( 2, 6 );
    graph.AddEdge( 6, 7 );

    // BFS( graph, 0, []( int vertex ) { std::cout << vertex << " "; } );
    // std::cout << min_paths_num( graph, u, w );
    std::cout << min_paths_num( graph, 0, 7 );

    return 0;
}