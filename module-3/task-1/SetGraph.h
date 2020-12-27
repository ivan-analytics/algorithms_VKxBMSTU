#ifndef TASK_1_SETGRAPH_H
#define TASK_1_SETGRAPH_H

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

#endif //TASK_1_SETGRAPH_H
