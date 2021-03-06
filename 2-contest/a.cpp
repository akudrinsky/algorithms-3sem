#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
 
//#define DEBUG
 
#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
    #define LOGS(...)\
        printf(__VA_ARGS__);\
        printf("\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, __func__);
#else
    #define LOGS(data, ...) ;
    #define ON_DEBUG(...) ;
#endif
 
 
struct Edge {
    Edge();
    Edge(int from, int to, long long capacity, long long flow);
    friend std::ostream& operator<<(std::ostream& out, const Edge& e);
 
    long long availableCap();
    int from;
    int to;
    long long capacity;
    long long flow;
};
 
// Graph for flow algorithms. S is 0'th vertice, T is (n - 1)'th
class Graph {
public:
    Graph(int nVertice);
    ~Graph();
 
    // inserts edge in a graph
    void NewEdge(const Edge& newEdge);
 
    // finds max flow with Edmonds Karp algo
    long long maxFlowEdmondsKarp(int startingVert, int goal);
 
    // shows all edges of graph
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);
 
private:
    // number of vertices in graph
    const int nVertice;
 
    // vector of all edges in graph
    std::vector<Edge> edges;       
 
    // whichEdges[i] - indexes of edges in a graph
    std::vector<int>* whichEdges;
 
    // for BFS algorithm to construct path between 0 and n - 1
    int* ancestors;
 
    // for BFS algorithm to save lengths of paths
    //int* pathLengths;
 
    // for BFS algorithm to check if it was in i'th vertice
    bool* used;
 
    unsigned long long maxFlow;
 
    // finds shortest path between vertices
    bool findShortPath(int startingVertice, int goal);
 
    // updates flow between vertices on the shortest path
    void updateFlow(int startingVert, int goal);
 
    // checks if you can push smth more through this edge
    bool edgeNotFull(int edgeID);
 
    // finds index of an edge between two vertices
    int findEdge(int from, int to);
 
    // fills array with a value
    template<typename T>
    void fillArray(T* array, int nElems, const T& value);
};
 
int main() {
    int nVertice = 0, nEdges;
    scanf("%d %d", &nVertice, &nEdges);
 
    Graph G(nVertice);
 
    Edge e(0, 0, 0, 0);
    for (int i = 0; i < nEdges; ++i) {
        scanf("%d %d %lld", &e.from, &e.to, &e.capacity);
        --e.from;
        --e.to;
        G.NewEdge(e);
    }
 
    LOGS("INFO >>> graph is ready\n")
 
    printf("%lld\n", G.maxFlowEdmondsKarp(0, nVertice - 1));
}
 
Edge::Edge(int from, 
            int to, 
            long long capacity, 
            long long flow) 
            : 
            from(from), 
            to(to), 
            capacity(capacity), 
            flow(flow) {}
 
Edge::Edge() : 
            from(0), 
            to(0), 
            capacity(0), 
            flow(0) {}
 
std::ostream& operator<<(std::ostream& out, const Edge& e) {
    out << e.from << " -(" << e.flow << '/' << e.capacity << ")-> " << e.to;
    return out;
}
 
long long Edge::availableCap() {
    return capacity - flow;
}
 
std::ostream& operator<<(std::ostream& out, const Graph& g) {
    out << "printing graph\n";
    for (int i = 0; i < g.edges.size(); ++i) {
        out << '\t' << g.edges[i] << '\n';
        if (i % 2 == 1) {
            out << '\n';
        }
    }
    out << "the end\n";
 
    return out;
}
 
Graph::Graph(int nVertice)
            : 
            nVertice(nVertice), 
            whichEdges(new std::vector<int>[nVertice]), 
            edges(0),
            ancestors(new int[nVertice]), 
            //pathLengths(new int[nVertice]),
            used(new bool[nVertice]),
            maxFlow(0) {}
 
Graph::~Graph() {
    delete [] whichEdges;
    delete [] ancestors;
    //delete [] pathLengths;
    delete [] used;
}
 
bool Graph::findShortPath(int startingVertice, int goal) {
    fillArray(used, nVertice, false);
    //pathLengths[startingVertice] = 0;
 
    LOGS("INFO >>> bfs\n")
 
    std::queue<int> vertToBFS;
 
    vertToBFS.push(startingVertice);
    used[startingVertice] = true;
    ancestors[startingVertice] = -1;
 
    while (!vertToBFS.empty()) {
        int vert = vertToBFS.front();
        vertToBFS.pop();
 
        for (auto edgeID : whichEdges[vert]) {
            if (edgeNotFull(edgeID)) {
                int to = edges[edgeID].to;
                if (!used[to]) {
                    used[to] = true;
                    vertToBFS.push(to);
                    ancestors[to] = vert;
                    //pathLengths[to] = pathLengths[vert] + 1;
                }
                if (to == goal) {
                    ON_DEBUG(
                        printf("goto used\n");
                        int curVert = goal;
                        
                    )
                    goto exit;
                }
            }
        }
    }
 
exit:
    if (used[goal]) {
        return true;
    }
    else {
        return false;
    }
}
 
bool Graph::edgeNotFull(int edgeID) {
    return edges[edgeID].capacity > edges[edgeID].flow;
    // return edges[edgeID].capacity > edges[edgeID].flow;
}
 
template<typename T>
void Graph::fillArray(T* array, int nElems, const T& value) {
    for (int i = 0; i < nElems; ++i) {
        array[i] = value;
    }
}
 
void Graph::NewEdge(const Edge& newEdge) {
    int edgeID = 0;
    if ((edgeID = findEdge(newEdge.from, newEdge.to)) != -1) {
        edges[edgeID].capacity += newEdge.capacity;
        //edges[edgeID ^ 1].capacity += newEdge.capacity;
    }
    else {
        edges.push_back(newEdge);
        //edges.push_back(Edge{newEdge.to, newEdge.from, newEdge.capacity, 0});
        whichEdges[newEdge.from].push_back(edges.size() - 1);
        //whichEdges[newEdge.to].push_back(edges.size() - 1);
    }
}
 
long long Graph::maxFlowEdmondsKarp(int startingVert, int goal) {
    while(findShortPath(startingVert, goal)) {
        updateFlow(startingVert, goal);
        ON_DEBUG (
            std::cout << *this;
        )
    }
    auto ret = maxFlow;
    maxFlow = 0;
 
    return ret;
}
 
void Graph::updateFlow(int startingVert, int goal) {
    LOGS("INFO >>> updating flow\n")
    long long minCapacity = LLONG_MAX;
    int curVert = goal;
    while (curVert != startingVert) {
        //LOGS("INFO >>> by edge %d -> %d", ancestors[curVert], curVert)
        int edgeID = findEdge(ancestors[curVert], curVert);
        minCapacity = std::min(minCapacity, edges[edgeID].availableCap());
        curVert = ancestors[curVert];
    }
 
    curVert = goal;
    while (curVert != startingVert) {
        int edgeID = findEdge(ancestors[curVert], curVert); // can preserve from prev
        edges[edgeID].flow += minCapacity;
        //edges[edgeID ^ 1].flow -= minCapacity;
        curVert = ancestors[curVert];
    }
 
    LOGS("INFO >>> increased maxflow (%llu) by %lld\n", maxFlow, minCapacity)
 
    maxFlow += minCapacity;
}
 
int Graph::findEdge(int from, int to) {
    for (auto edgeID : whichEdges[from]) {
        if (edges[edgeID].to == to) {
            return edgeID;
        }
    }
 
    return -1;
}