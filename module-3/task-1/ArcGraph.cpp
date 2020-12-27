#include <vector>
#include <cassert>
#include "IGraph.h"

using std::vector;
using std::pair;

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int vertexCount );
    explicit ArcGraph(const IGraph& graph );

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    vector < pair<int, int> > edgeList; // from - to
    const int vertexCount;
};

ArcGraph::ArcGraph(int vertexCount) : vertexCount(vertexCount)
{}

ArcGraph::ArcGraph(const IGraph &graph) : vertexCount(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); from++) {
        auto adjacentVertices = graph.GetNextVertices(from);
        for (int& to : adjacentVertices)
            edgeList.emplace_back(from, to);
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );
    edgeList.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return vertexCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vector<int> next;
    for (const auto & i : edgeList)
        if (i.first == vertex)
            next.push_back(i.second);
    return next;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vector<int> prev;
    for (const auto & i : edgeList)
        if (i.second == vertex)
            prev.push_back(i.first);
    return prev;
}
