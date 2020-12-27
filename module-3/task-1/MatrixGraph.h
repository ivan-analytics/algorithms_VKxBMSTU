#ifndef TASK_1_MATRIXGRAPH_H
#define TASK_1_MATRIXGRAPH_H

#include <vector>
#include <cassert>
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

#endif //TASK_1_MATRIXGRAPH_H
