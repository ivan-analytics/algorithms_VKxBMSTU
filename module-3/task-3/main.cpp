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
    adjacencyLists[to].push_back( std::make_pair(from, weight) ); // добавлено чтобы граф был неориентированным
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

int Relax(vector<int>& path_min_weights, int from_to_edge_weight, int from, int to, vector<int>& parents) {
    if (path_min_weights[to] > path_min_weights[from] + from_to_edge_weight) {
        int old_weight = path_min_weights[to];
        path_min_weights[to] = path_min_weights[from] + from_to_edge_weight;
        parents[to] = from;
        return old_weight;
    }
    return -1;
}


#define INT_MAX 2147483647
int min_path(const IWeightGraph& graph, int from, int to) {
    vector<int> parents(graph.VerticesCount(), -1);
    vector<int> path_min_weights(graph.VerticesCount(), INT_MAX);

    auto cmp = [](pair<int, int> p1, pair<int, int> p2) {
        if (p1.second < p2.second)
            return true;
        else if (p1.second > p2.second)
            return false;
        else return p1.first < p2.first;
    };
    set<pair<int, int>, decltype(cmp)> priority_queue(cmp);

    path_min_weights[from] = 0;
    priority_queue.insert(std::make_pair(from, path_min_weights[from]));

    while ( !priority_queue.empty() ) {
        auto cur = priority_queue.begin();
        pair<int, int> current = *cur; priority_queue.erase(cur);
        auto next_vertices = graph.GetNextVertices(current.first);
        for (int i = 0; i < next_vertices.size(); i++) {
            // встречаем вершину первый раз - задаем текущий путь
            if (path_min_weights[next_vertices[i].first] == INT_MAX) {
                path_min_weights[next_vertices[i].first] = path_min_weights[current.first] + next_vertices[i].second;
                parents[next_vertices[i].first] = current.first;
                priority_queue.insert(std::make_pair(next_vertices[i].first, path_min_weights[next_vertices[i].first]));
                // уже встречали вершину - пробуем оптимизировать путь с помощью Relax
            } else {
                int old_weight = Relax(path_min_weights, next_vertices[i].second, current.first, next_vertices[i].first, parents);
                if (old_weight != -1) {
                    // заменяем элемент в очереди уже с актуальным весом пути
                    auto el = priority_queue.find(std::make_pair(next_vertices[i].first, old_weight));
                    if (el != priority_queue.end()) priority_queue.erase(el);
                    priority_queue.insert(std::make_pair(next_vertices[i].first, path_min_weights[i]));
                }
            }

        }

    }

    return path_min_weights[to];
}


int main() {
    int N;
    cin >> N;

    WeightListGraph graph(N);

    int M;
    cin >> M;
    for (int i = 0; i < M; i++) {
        int from; int to; int weight;
        cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    int from; int to;
    cin >> from >> to;

    std::cout << min_path(graph, from, to);
    return 0;
}



















