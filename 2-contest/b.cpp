/*
В одном из отделов крупной организации работает $$$n$$$ человек. 
Как практически все сотрудники этой организации, 
они любят пить чай в перерывах между работой. 
При этом они достаточно дисциплинированы и делают в день ровно один перерыв, 
во время которого пьют чай. Для того, чтобы этот перерыв был максимально приятным, 
каждый из сотрудников этого отдела обязательно пьет чай одного из своих любимых сортов. 
В разные дни сотрудник может пить чай разных сортов. 
Для удобства пронумеруем сорта чая числами от 1 до $$$m$$$.
Недавно сотрудники отдела купили себе большой набор чайных пакетиков, 
который содержит $$$a_1$$$ пакетиков чая сорта номер 1, $$$a_2$$$ пакетиков чая сорта 
номер 2, ..., $$$a_m$$$ пакетиков чая сорта номер $$$m$$$. 
Теперь они хотят знать, на какое максимальное число дней им может хватить купленного 
набора так, чтобы в каждый из дней каждому из сотрудников доставался пакетик чая 
одного из его любимых сортов. 
Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает 
из одного пакетика. 
При этом пакетики чая не завариваются повторно.

Входные данные
Первая строка содержит два целых числа $$$n$$$ и $$$m$$$ ($$$1 \le n, m \le 50$$$). 
Вторая строка содержит $$$m$$$ целых чисел $$$a_1$$$, ..., $$$a_m$$$ 
($$$1 \le a_i \le 10^6$$$ для всех $$$i$$$ от 1 до $$$m$$$).
Далее следуют $$$n$$$ строк — $$$i$$$-я из этих строк описывает любимые сорта 
$$$i$$$-го сотрудника отдела и имеет следующий формат: 
сначала следует положительное число $$$k_i$$$ — количество любимых сортов чая этого 
сотрудника, а затем идут $$$k_i$$$ различных чисел от 1 до $$$m$$$ — 
номера этих сортов. 

Выходные данные
Выведите одно целое число — искомое максимальное количество дней.

Пример
Входные данные
2 3
3 2 1
2 1 2
2 1 3
Выходные данные
3

*/

#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
 
#define DEBUG
 
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
    void photo(const char* pictName, int SourceUID, int DrainUID);

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
    bool findShortPath(int startingVertice, int goal);
 
    // updates flow between vertices on the shortest path
    void updateFlow(int startingVert, int goal);

    // makes photo of a graph with grahpviz
 
    // checks if you can push smth more through this edge
    bool edgeNotFull(int edgeID);
 
    // finds index of an edge between two vertices
    int findEdge(int from, int to);
 
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
    int nPeople = 0, nTees = 0;
    scanf("%d %d\n", &nPeople, &nTees);

    int sourceID = 0;
    int drainUID = 1;

    borders<int> peopleVerts = {drainUID + 1, drainUID + nPeople + 1};
    borders<int> teesVerts = {drainUID + nPeople + 1, drainUID + nPeople + nTees + 1};

    LOGS("%d people with indexes [%d, %d), %d tees with indexes [%d, %d)", nPeople, peopleVerts.min, peopleVerts.max, nTees, teesVerts.min, teesVerts.max)

    long long teaNum = 0;
    int* teaStore = new int[nTees];
    for (int i = 0; i < nTees; ++i) {
        scanf("%d", teaStore + i);
        teaNum += teaStore[i];
    }

    std::vector<int>* loves = new std::vector<int>[nPeople];
    for (int person = 0; person < nPeople; ++person) {
        int lovedTeesNum = 0;
        scanf("%d", &lovedTeesNum);
        for (int i = 0; i < lovedTeesNum; ++i) {
            int a = 0;
            scanf("%d", &a);
            loves[person].push_back(a - 1);
        }
    }

    borders<long long> binSearch = {0, teaNum / nPeople + 1};

    while (binSearch.min < binSearch.max - 1) {
        Graph G(nPeople + nTees + 2);
        long long toCheck = (binSearch.min + binSearch.max) / 2;

        for (int person = peopleVerts.min; person < peopleVerts.max; ++person) {
            G.NewEdge({sourceID, person, toCheck, 0});

            for (auto teaID : loves[person - peopleVerts.min]) {
                G.NewEdge({person, teesVerts.min + teaID, teaStore[teaID], 0});
            }
        }
        
        for (int teaID = 0; teaID < nTees; ++teaID) {
            G.NewEdge({teesVerts.min + teaID, drainUID, teaStore[teaID], 0});
        }

        int maxFlow = G.maxFlowEdmondsKarp(sourceID, drainUID);
        
        if (maxFlow == toCheck * nPeople) {
            binSearch.min = toCheck;
        }
        else {
            binSearch.max = toCheck;
        }

        ON_DEBUG(
            if (binSearch.min < binSearch.max - 1) {
                G.photo("ex.png", sourceID, drainUID);
            }
        )
    }

    printf("%lld", binSearch.min);

    delete [] teaStore;
    delete [] loves;
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

void Graph::photo(const char* pictName, int SourceUID, int DrainUID) {
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

    for (auto edge : edges) {
        if (edge.capacity > 0 and edge.flow >= 0) {
            fprintf (pFile, "\t%d -> %d [label=\" %lld/%lld \"]\n", edge.from, edge.to, edge.flow, edge.capacity);
        }
    }

    fputc('}', pFile);
    fclose(pFile);

    const int max_cmd_size = 50;
    char command[max_cmd_size] = "dot tree_graph.dot -T ";
    strcat (command, "png");
    strcat (command, " -o ");
    strcat (command, pictName);
    system (command);

    system("rm tree_graph.dot");
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

std::vector<int> Graph::minCut(int startingVert, int goal) {
    //maxFlowEdmondsKarp(startingVert, goal);
    
    fillArray(used, nVertice, false);
    findShortPath(startingVert, goal);

    std::vector<int> answer;
    for (int edgeID = 0; edgeID < edges.size(); ++edgeID) {
        if (used[edges[edgeID].from] and !used[edges[edgeID].to]) {
            answer.push_back((edgeID | 1) / 2);
        }
    }

    return answer;
}
 
bool Graph::edgeNotFull(int edgeID) {
    return edges[edgeID].capacity > edges[edgeID].flow;
}
 
template<typename T>
void Graph::fillArray(T* array, int nElems, const T& value) {
    for (int i = 0; i < nElems; ++i) {
        array[i] = value;
    }
}
 
void Graph::NewEdge(const Edge& newEdge) {
    LOGS("INFO >>> adding new edge %d -> %d with cap=%lld\n", newEdge.from, newEdge.to, newEdge.capacity)
    int edgeID = 0;
    if ((edgeID = findEdge(newEdge.from, newEdge.to)) != -1) {
        //LOGS("One way?\n")
        edges[edgeID].capacity += newEdge.capacity;
        edges[edgeID ^ 1].capacity += newEdge.capacity;
    }
    else {
        //LOGS("Or another?\n")
        edges.push_back(newEdge);
        edges.push_back(Edge{newEdge.to, newEdge.from, newEdge.capacity, 0});
        whichEdges[newEdge.from].push_back(edges.size() - 2);
        whichEdges[newEdge.to].push_back(edges.size() - 1);
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
    //LOGS("INFO >>> updating flow\n")
    long long minCapacity = LLONG_MAX;
    int curVert = goal;
    while (curVert != startingVert) {
        LOGS("INFO >>> by edge %d -> %d", ancestors[curVert], curVert)
        int edgeID = findEdge(ancestors[curVert], curVert);
        minCapacity = std::min(minCapacity, edges[edgeID].availableCap());
        curVert = ancestors[curVert];
    }
 
    curVert = goal;
    while (curVert != startingVert) {
        int edgeID = findEdge(ancestors[curVert], curVert); // can preserve from prev
        edges[edgeID].flow += minCapacity;
        edges[edgeID ^ 1].flow -= minCapacity;
        curVert = ancestors[curVert];
    }
 
    LOGS("INFO >>> increased maxflow (%llu) by %lld\n", maxFlow, minCapacity)
 
    maxFlow += minCapacity;
}
 
int Graph::findEdge(int from, int to) {
    if (!whichEdges[from].empty()) {
        for (auto edgeID : whichEdges[from]) {
            //LOGS("INFO >>> found edgeID is %d\n", edgeID)
            if (edges[edgeID].to == to) {
                return edgeID;
            }
        }
    }
    return -1;
}