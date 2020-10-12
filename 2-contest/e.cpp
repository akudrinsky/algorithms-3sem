/*
Шрек и Осёл уже были на пути домой. 
Им оставалось только преодолеть лес, который отделял их от болота. 
Но они поссорились, поэтому не хотят идти вместе.
Лес представляет собой опушки, пронумерованы числами от 1 до $$$n$$$ и соединенные
$$$m$$$ дорожками (может быть несколько дорожек соединяющих две опушки, 
могут быть дорожки, соединяющие опушку с собой же). 
Из-за ссоры, если по дорожке прошел один из друзей, то второй по той же дорожке 
уже идти не может. Сейчас друзья находятся на опушке с номером $$$s$$$, 
а болото Шрека — на опушке с номером $$$t$$$. 
Помогите Шреку и Ослу добраться до болота.

Входные данные
В первой строке файла записаны четыре целых числа — $$$n$$$, $$$m$$$, 
$$$s$$$ и $$$t$$$ ($$$2 \le n \le 10^5$$$, $$$0 \le m \le 10^5$$$, $$$s \not= t$$$). 
В следующих $$$m$$$ строках записаны пары чисел. 
Пара чисел $$$(x, y)$$$ означает, что есть дорожка с опушки $$$x$$$ до опушки $$$y$$$ 
(из-за особенностей местности дорожки односторонние).

Выходные данные
Если существует решение, то выведите YES и на двух отдельных строчках сначала 
последовательность опушек для Шрека, затем путь для Осла.
Если решения не существует, выведите NO.
Если решений несколько, выведите любое.

Пример
Входные данные
3 3 1 3
1 2
1 3
2 3

Выходные данные
YES
1 3 
1 2 3 

*/

#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
#include <string>
 
//#define DEBUG
 
#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
    #define LOGS(...)\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, __func__);\
        fflush(stdout);
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

// Graph for flow algorithms.
class Graph {
public:
    Graph(int nVertice);
    ~Graph();
 
    // inserts edge in a graph
    void NewEdge(const Edge& newEdge);
 
    // finds max flow with Edmonds Karp algo
    long long maxFlowEdmondsKarp(int startingVert, int goal);

    // maybe d-specific
    bool isValid(int SourceUID, int DrainUID);

    // makes photo of a graph with graphviz
    void photo(std::string pictName, int SourceUID, int DrainUID);

    // generates path, by which there is a flow after E-K algo (reversed order)
    std::vector<int> genPath(int sourceUID, int drainUID);

    // searches for min cut in a graph between two edges
    std::vector<int> minCut(int startingVert, int goal);
 
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
    bool findShortPath(int startingVertice, int goal, bool(Graph::*criterion)(int));
 
    // updates flow between vertices on the shortest path
    void updateFlow(int startingVert, int goal);

    // makes photo of a graph with grahpviz
 
    // checks if you can push smth more through this edge
    bool edgeNotFull(int edgeID);

    // checks if there is a flow in a given edge
    bool edgeHasWater(int edgeID);
 
    // finds index of an edge between two vertices
    int findEdge(int from, int to, bool onlyDirect);
 
    // fills array with a value
    template<typename T>
    void fillArray(T* array, int nElems, const T& value);
};

template<typename T>
struct borders {
    T min;
    T max;
};






/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/





int main() {
    int nVerts = 0, nPaths = 0, sourceUID = 0, drainUID = 0;
    scanf("%d %d %d %d\n", &nVerts, &nPaths, &sourceUID, &drainUID);
    --sourceUID;
    --drainUID;
    Graph G(nVerts);

    for (int i = 0; i < nPaths; ++i) {
        Edge edge(0, 0, 1, 0);
        scanf("%d %d\n", &edge.from, &edge.to);
        --edge.from;
        --edge.to;
        G.NewEdge(edge);
    }

    ON_DEBUG (
        G.photo("initial.png", sourceUID, drainUID);
    )
    auto res = G.maxFlowEdmondsKarp(sourceUID, drainUID);
    LOGS("INFO >>> maxflow is %lld\n", res)

    if (res < 2) {
        printf("NO\n");
    }
    else {
        printf("YES\n");
        std::vector<int> pathOne = G.genPath(sourceUID, drainUID);
        ON_DEBUG (
            G.photo("path1.png", sourceUID, drainUID);
        )
        std::vector<int> pathTwo = G.genPath(sourceUID, drainUID);
        ON_DEBUG (
            G.photo("path2.png", sourceUID, drainUID);
        )
        for (int i = pathOne.size() - 1; i >= 0; --i) {
            printf("%d ", pathOne[i] + 1);
        }
        printf("\n");

        for (int i = pathTwo.size() - 1; i >= 0; --i) {
            printf("%d ", pathTwo[i] + 1);
        }
        printf("\n");
    }
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

bool Graph::isValid(int SourceUID, int DrainUID) {
    maxFlowEdmondsKarp(SourceUID, DrainUID);
    for (auto edge : edges) {
        if (edge.from == SourceUID and edge.flow != edge.capacity) {
            return false;
        }
        if (edge.to == DrainUID and edge.flow != edge.capacity) {
            return false;
        }
    }
    return true;
}

void Graph::photo(std::string pictName, int SourceUID, int DrainUID) {
    FILE* pFile = fopen ("tree_graph.dot", "w");
    fprintf (pFile, "digraph G{\n\tedge[color=\"chartreuse4\",fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"rectangle\",fontsize=15];\n");
    fprintf(pFile, "\nnode[group=a];\n");
    for (int i = 0; i < nVertice; i += 2) {
        if (i == SourceUID || i == DrainUID) {
            fprintf (pFile, "\t%d [shape=circle, fillcolor=red];\n", i);
        }
        else {
            fprintf (pFile, "\t%d [shape=record];\n", i);
        }
    }
    fprintf(pFile, "\nnode[group=b];\n");
    for (int i = 1; i < nVertice; i += 2) {
        if (i == SourceUID || i == DrainUID) {
            fprintf (pFile, "\t%d [shape=circle, fillcolor=red];\n", i);
        }
        else {
            fprintf (pFile, "\t%d [shape=record];\n", i);
        }
    }
    fputc('\n', pFile);

    for (int i = 0; i < edges.size(); i += 1) {
        auto& edge = edges[i];
        fprintf (pFile, "\t%d -> %d [label=\" %d[%lld/%lld] \"]\n", edge.from, edge.to, i, edge.flow, edge.capacity);
    }

    fputc('}', pFile);
    fclose(pFile);

    const int max_cmd_size = 50;
    char command[max_cmd_size] = "dot tree_graph.dot -T ";
    strcat (command, "png");
    strcat (command, " -o ");
    strcat (command, pictName.c_str());
    system (command);

    system("rm tree_graph.dot");
}

std::vector<int> Graph::genPath(int sourceUID, int drainUID) {
    bool pathExists = findShortPath(sourceUID, drainUID, &Graph::edgeHasWater);
    if (!pathExists) {
        return {};
    }

    std::vector<int> path;

    for (int cur = drainUID; cur != sourceUID;) {
        path.push_back(cur);
        int parent = edges[ancestors[cur]].from;
        edges[ancestors[cur]].flow -= 1;
        edges[ancestors[cur] ^ 1].flow += 1;
        cur = parent;
    }

    path.push_back(sourceUID);

    return path;
}
 
bool Graph::findShortPath(int startingVertice, int goal, bool(Graph::*criterion)(int)) {
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
            /*
            if (edgeID % 2 == 1 and edges[edgeID].flow == 0) {
                continue;
            } // directed graph
            */

            if ((this->*criterion)(edgeID)) {
                int to = edges[edgeID].to;
                if (!used[to]) {
                    used[to] = true;
                    vertToBFS.push(to);
                    ancestors[to] = edgeID;
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

std::vector<int> Graph::minCut(int startingVert, int goal) {
    //maxFlowEdmondsKarp(startingVert, goal);
    
    fillArray(used, nVertice, false);
    findShortPath(startingVert, goal, &Graph::edgeNotFull);

    std::vector<int> answer;
    for (int edgeID = 0; edgeID < edges.size(); ++edgeID) {
        if (used[edges[edgeID].from] and !used[edges[edgeID].to]) {
            answer.push_back((edgeID | 1) / 2);
        }
    }

    return answer;
}

bool Graph::edgeHasWater(int edgeID) {
    return edges[edgeID].flow != 0;
}

bool Graph::edgeNotFull(int edgeID) {
    LOGS("INFO >>> checking %d'th edge from %d to %d with %lld/%lld\n", edgeID, edges[edgeID].from, edges[edgeID].to, edges[edgeID].flow, edges[edgeID].capacity)

    return edges[edgeID].flow < edges[edgeID].capacity;
}
 
template<typename T>
void Graph::fillArray(T* array, int nElems, const T& value) {
    for (int i = 0; i < nElems; ++i) {
        array[i] = value;
    }
}
 
void Graph::NewEdge(const Edge& newEdge) {
    LOGS("INFO >>> adding new edge %d -> %d with cap=%lld\n", newEdge.from, newEdge.to, newEdge.capacity)

    if (newEdge.from == newEdge.to) {
        return;
    }

    
    edges.push_back(newEdge);
    edges.push_back(Edge{newEdge.to, newEdge.from, 0, 0});
    whichEdges[newEdge.from].push_back(edges.size() - 2);
    whichEdges[newEdge.to].push_back(edges.size() - 1);
    
    /*
    int edgeID = findEdge(newEdge.from, newEdge.to, true);

    if (edgeID != -1) {
        //LOGS("One way?\n")
        edges[edgeID].capacity += newEdge.capacity;
        edges[edgeID ^ 1].capacity += newEdge.capacity;
    }
    else {
        //LOGS("Or another?\n")
        edges.push_back(newEdge);
        edges.push_back(Edge{newEdge.to, newEdge.from, 0, 0});
        whichEdges[newEdge.from].push_back(edges.size() - 2);
        whichEdges[newEdge.to].push_back(edges.size() - 1);
    }
    */
}

long long Graph::maxFlowEdmondsKarp(int startingVert, int goal) {
    while(findShortPath(startingVert, goal, &Graph::edgeNotFull)) {
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
    //LOGS("INFO >>> updating flow\n")
    long long minCapacity = LLONG_MAX;
    int curVert = goal;
    while (curVert != startingVert) {
        LOGS("INFO >>> by edge %d -> %d", edges[ancestors[curVert]].from, curVert)

        minCapacity = std::min(minCapacity, edges[ancestors[curVert]].availableCap());
        /*
        if (edgeID % 2 == 0) {
            minCapacity = std::min(minCapacity, edges[edgeID].availableCap());
        }
        else {
            minCapacity = std::min(minCapacity, -edges[edgeID].flow);
        } // directed graph
        */
        curVert = edges[ancestors[curVert]].from;
    }
 
    curVert = goal;
    while (curVert != startingVert) {
        //int edgeID = findEdge(ancestors[curVert], curVert); // can preserve from prev
        edges[ancestors[curVert]].flow += minCapacity;
        edges[ancestors[curVert] ^ 1].flow -= minCapacity;
        curVert = edges[ancestors[curVert]].from;
    }
 
    LOGS("INFO >>> increased maxflow (%llu) by %lld\n", maxFlow, minCapacity)
 
    maxFlow += minCapacity;
}
 
int Graph::findEdge(int from, int to, bool onlyDirect) {
    if (!whichEdges[from].empty()) {
        for (auto edgeID : whichEdges[from]) {
            //LOGS("INFO >>> found edgeID is %d\n", edgeID)
            if (onlyDirect) {
                if (edgeID % 2 == 0 and edges[edgeID].to == to) {
                    return edgeID;
                }
            }
            else {
                if (edges[edgeID].to == to) {
                    return edgeID;
                }
            }
        }
    }
    return -1;
}