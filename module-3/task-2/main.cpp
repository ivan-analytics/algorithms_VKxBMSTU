#include <queue>
#include <deque>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <cassert>

using std::queue;
using std::unordered_set;
using std::deque;
using std::cin;
using std::cout;


struct IGraph {
    virtual ~IGraph() = default;

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

ListGraph::ListGraph(int vertexCount ) :
        adjacencyLists( vertexCount )
{}

ListGraph::ListGraph(const IGraph& graph )
{
    adjacencyLists.resize( graph.VerticesCount() );
    for( int from = 0; from < adjacencyLists.size(); ++from )
        adjacencyLists[from] = graph.GetNextVertices( from );
}

void ListGraph::AddEdge(int from, int to )
{
    assert( from >= 0 && from < adjacencyLists.size() );
    assert( to >= 0 && to < adjacencyLists.size() );

    adjacencyLists[from].push_back( to );
    adjacencyLists[to].push_back( from ); // добавлено чтобы граф был неориентированным
}

int ListGraph::VerticesCount() const
{
    return adjacencyLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex ) const
{
    assert( vertex >= 0 && vertex < adjacencyLists.size() );
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex ) const
{
    assert( vertex >= 0 && vertex < adjacencyLists.size() );
    vector<int> result;
    for( int from = 0; from < adjacencyLists.size(); ++from )
        for( int i = 0; i < adjacencyLists[from].size(); ++i )
            if( adjacencyLists[from][i] == vertex )
                result.push_back( from );
    return result;
}




#define NULL_INT -1
int min_paths_num(const IGraph& graph, int from, int to) {
    vector<bool> visited( graph.VerticesCount(), false );
    unordered_set<int> prev_layers_vertices;
    vector<int> min_paths_cnt( graph.VerticesCount(), 0 );
    deque<int> bfsQueue;
    bfsQueue.push_back(NULL_INT);
    bfsQueue.push_back(from);
    visited[from] = true;
    min_paths_cnt[from] = 1;

    while( !bfsQueue.empty() ) {
        int current = bfsQueue.front();
        bfsQueue.pop_front();
        // заполняем множество вершин текущим слоем
        if (current == NULL_INT) {
            for (auto it = bfsQueue.begin(); *it != NULL_INT && it != bfsQueue.end(); it++) {
                prev_layers_vertices.insert(*it);
                bfsQueue.push_back(NULL_INT);
            }
            continue;
        }
        vector<int> adjacentVeticies = graph.GetNextVertices(current);

        for (int &adjacentVeticie : adjacentVeticies) {
            if (!visited[adjacentVeticie]) {
                bfsQueue.push_back(adjacentVeticie);
                visited[adjacentVeticie] = true;
            }

            // если вершина не из множества вершин предыдущих/текущего слоев,
            // то добавляем к ней возможные минимальные пути
            auto search = prev_layers_vertices.find(adjacentVeticie);
            if (search == prev_layers_vertices.end())
                min_paths_cnt[adjacentVeticie] += min_paths_cnt[current];
        }
    }

    return min_paths_cnt[to];
}

int main()
{
    int v; cin >> v;
    ListGraph graph(v);
    int n; cin >> n;
    for (int i = 0; i < n; i++) {
        int from; int to;
        cin >> from >> to;
        graph.AddEdge(from, to);
    }
    int u; int w;
    cin >> u >> w;

    std::cout << min_paths_num( graph, u, w );
    return 0;
}