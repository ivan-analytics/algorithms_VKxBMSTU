#ifndef TASK_1_LISTGRAPH_H
#define TASK_1_LISTGRAPH_H

#include <vector>
#include <cassert>
#include "IGraph.h"

using std::vector;

class ListGraph : public IGraph {
public:
    explicit ListGraph(int vertexCount );
    explicit ListGraph(const IGraph& graph );

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    vector<vector<int>> adjacencyLists;
};

#endif //TASK_1_LISTGRAPH_H
