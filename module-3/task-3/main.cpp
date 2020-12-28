#include <iostream>
#include <cassert>
#include <vector>
#include <set>

using std::cin;
using std::cout;
using std::pair;
using std::vector;
using std::set;


struct IWeightGraph {
    virtual ~IWeightGraph() = default;

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};

using std::vector;

class WeightListGraph : public IWeightGraph {
public:
    explicit WeightListGraph(int vertexCount );
    explicit WeightListGraph(const IWeightGraph& graph );

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;
    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override; // vertex, weight
    std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const override; // vertex, weight

private:
    vector<vector< pair<int, int> >> adjacencyLists; // vertex, weight
};

WeightListGraph::WeightListGraph(int vertexCount ) :
        adjacencyLists( vertexCount )
{}


WeightListGraph::WeightListGraph(const IWeightGraph& graph )
{
    adjacencyLists.resize( graph.VerticesCount() );
    for( int from = 0; from < adjacencyLists.size(); ++from )
        adjacencyLists[from] = graph.GetNextVertices( from );
}

void WeightListGraph::AddEdge(int from, int to, int weight )
{
    assert( from >= 0 && from < adjacencyLists.size() );
    assert( to >= 0 && to < adjacencyLists.size() );
    assert( weight >= 0 );

    adjacencyLists[from].push_back( std::make_pair(to, weight) );
    //// adjacencyLists[to].push_back( from ); // !!! добавлено чтобы граф был неориентированным
}

int WeightListGraph::VerticesCount() const
{
    return adjacencyLists.size();
}

std::vector<std::pair<int, int>> WeightListGraph::GetNextVertices(int vertex ) const
{
    assert( vertex >= 0 && vertex < adjacencyLists.size() );
    return adjacencyLists[vertex];
}

std::vector<std::pair<int, int>> WeightListGraph::GetPrevVertices(int vertex ) const
{
    assert( vertex >= 0 && vertex < adjacencyLists.size() );
    vector< pair<int, int> > result;
    for( int from = 0; from < adjacencyLists.size(); ++from )
        for( int i = 0; i < adjacencyLists[from].size(); ++i )
            if( adjacencyLists[from][i].first == vertex )
                result.emplace_back(from, adjacencyLists[from][i].second );
    return result;
}

int min_path(const IWeightGraph& graph, int from, int to) {
    vector<int> parents(graph.VerticesCount(), -1);
    vector<int> path_min_weights(graph.VerticesCount(), INT_MAX);

    // !!! проверить работу сета, мб компаратор неподходящий, должен корректно сравнивать по весам
    auto cmp = [](pair<int, int> p1, pair<int, int> p2) { return p1.second < p2.second; };
    set<pair<int, int>, decltype(cmp)> priority_queue(cmp);

    path_min_weights[from] = 0;
    priority_queue.insert(std::make_pair(from, path_min_weights[from]));

    

}


int main() {


    return 0;
}



















