#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <queue>
#include <unordered_set>
#include <iostream>

using std::queue;
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

    BFS( graph, 0, []( int vertex ) { std::cout << vertex << " "; } );
    return 0;
}