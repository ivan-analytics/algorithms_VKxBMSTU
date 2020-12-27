#include <vector>
#include <unordered_set>
#include <cassert>
#include "IGraph.h"

using std::vector;
using std::unordered_set;

class SetGraph : public IGraph {
public:
    explicit SetGraph(int vertexCount );
    explicit SetGraph(const IGraph& graph );

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    vector< unordered_set<int> > adjacencyLists;
};

SetGraph::SetGraph(int vertexCount) : adjacencyLists( vertexCount )
{}

SetGraph::SetGraph(const IGraph &graph) {
    adjacencyLists.resize(graph.VerticesCount());
    for (int i = 0; i < adjacencyLists.size(); i++) {
        auto next = graph.GetNextVertices( i );
        for (int j = 0; j < next.size(); i++ )
            adjacencyLists[i].insert( next[j] );
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < adjacencyLists.size() );
    assert( to >= 0 && to < adjacencyLists.size() );

    adjacencyLists[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return adjacencyLists.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vector<int> next;
    for (auto i = adjacencyLists[vertex].begin(); i != adjacencyLists[vertex].end(); ++i) {
        next.push_back(*i);
    }
    return next;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert( vertex >= 0 && vertex < adjacencyLists.size() );
    vector<int> result;
    for( int from = 0; from < adjacencyLists.size(); ++from )
        for( auto i = adjacencyLists[from].begin(); i != adjacencyLists[from].end(); ++i )
            if( *i == vertex )
                result.push_back( from );
    return result;
}
