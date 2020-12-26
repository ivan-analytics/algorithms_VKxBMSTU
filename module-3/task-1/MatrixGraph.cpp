#include <vector>
#include <cassert>
#include <iostream>
#include "IGraph.h"

using std::vector;

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(int vertexCount );
    explicit MatrixGraph(const IGraph& graph );

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    vector< vector<bool> > adjacencyMatrix;
};

MatrixGraph::MatrixGraph(int vertexCount) : adjacencyMatrix( vertexCount ) {
    for (int i = 0; i < vertexCount; i++)
        adjacencyMatrix[i].resize(vertexCount, false);
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    adjacencyMatrix.resize(graph.VerticesCount());
    for (int i = 0; i < adjacencyMatrix.size(); i++) {
        adjacencyMatrix[i].resize(adjacencyMatrix.size(), false);
        auto vertices = graph.GetNextVertices(i);
        for (int vertice : vertices)
            adjacencyMatrix[i][vertice] = true;
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    assert( from >= 0 && from < VerticesCount() );
    assert( to >= 0 && to < VerticesCount() );
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vector<int> next_vertices;
    for (int i = 0; i < VerticesCount(); i++)
        if (adjacencyMatrix[vertex][i])
            next_vertices.push_back(i);
    return next_vertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert( vertex >= 0 && vertex < VerticesCount() );
    vector<int> prev_vertices;
    for (int i = 0; i < VerticesCount(); i++)
        if (adjacencyMatrix[i][vertex])
            prev_vertices.push_back(i);
    return prev_vertices;
}
