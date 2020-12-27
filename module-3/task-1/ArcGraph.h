#ifndef TASK_1_ARCGRAPH_H
#define TASK_1_ARCGRAPH_H

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

#endif //TASK_1_ARCGRAPH_H
