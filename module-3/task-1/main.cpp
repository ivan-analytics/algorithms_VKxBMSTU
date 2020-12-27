#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <queue>
#include <iostream>

using std::queue;

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
//  0---->1
//  | \  /
//  3   4--->2
    ArcGraph graph(5);
    graph.AddEdge( 0, 1 );
    graph.AddEdge( 0, 3 );
    graph.AddEdge( 0, 4 );
    graph.AddEdge( 1, 4 );
    graph.AddEdge( 4, 2 );

    BFS( graph, 0, []( int vertex ) { std::cout << vertex << " "; } );

    return 0;
}