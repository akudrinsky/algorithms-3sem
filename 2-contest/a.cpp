#include <stdio.h>
#include <vector>

struct edge {
    int from;
    int to;
    long long capacity;
    long long flow;
};

class Graph {
public:
    Graph(int nVertice): nVertice(nVertice), whichEdges(nVertice), edges(0) {}

    // inserts edge in a graph
    void NewEdge(const edge& newEdge);



private:
    // number of vertices in graph
    const int nVertice;

    // vector of all edges in graph
    std::vector<edge> edges;       

    // whichEdges[i] - indexes of edges in a graph
    std::vector<std::vector<int>> whichEdges; 
};

int main() {

}

void Graph::NewEdge(const edge& newEdge) {
    edges.push_back(newEdge);
    edges.push_back(edge{newEdge.to, newEdge.from, 0, 0});
    whichEdges[newEdge.from].push_back(edges.size() - 2);
    whichEdges[newEdge.to].push_back(edges.size() - 1);
}