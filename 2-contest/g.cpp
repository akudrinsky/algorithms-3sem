/*
Совсем скоро начнется первый тур очередной всероссийской командной олимпиады школьников по палеонтологии (ВКОШП). На олимпиаду приехали команды из $$$n$$$ школ, от каждой школы приехало ровно по две команды. Команды уже заняли свои места, когда обнаружилось, что некоторые команды из одной школы сидят слишком близко. Перед организаторами олимпиады встала серьезная задача — пересадить участников олимпиады.

Столы, за которыми сидят команды, расставлены в один ряд. Расстояние между рабочими местами соседних команд оказалось равно 10 метрам. Организаторы хотят, чтобы минимальное расстояние между двумя командами из одной школы было как можно больше.

Пересаживая команду, организатором необходимо перенести на новое место все оборудование, приготовленное командой для исследований. Поэтому организаторы хотят пересадить команды так, чтобы сумма расстояний между старыми и новыми рабочими местами команд была как можно меньше.

Например, пусть в соревновании принимают участие по две команды школ 1, 2, 3 и 4. Пусть исходно команды распределены по рабочим местам следующим образом: 1, 3, 2, 2, 1, 4, 4, 3 (для каждой команды указан номер школы, которую она представляет). При таком распределении по рабочим местам команды из школы 2 сидят слишком близко, как и команды из школы 4. Пересадив команды в следующем порядке: 1, 3, 2, 4, 1, 3, 2, 4, жюри может добиться своего: команды из одной школы сидят на местах, расстояние между которыми не меньше 40 м, большего расстояния добиться нельзя. Сумма расстояний между старыми и новыми позициями для данного примера равна $$$0 + 0 + 0 + 20 + 0 + 20 + 30 + 10 = 80$$$ м, для исходного распределения команд она минимальна.

Вам задано исходное распределение команд по рабочим местам. Требуется пересадить их так, чтобы минимальное расстояние между командами из одной школы было как можно больше. При этом среди возможных новых размещений команд следует выбрать такое, чтобы сумма расстояний между старыми и новыми местами рабочими команд была минимально возможной.

Входные данные
В первой строке входного файла задано число $$$n$$$ — количество команд ($$$1 \le n \le 100$$$). Во второй строке задано исходное распределение команд по рабочим местам — последовательность $$$a_1, a_2, \ldots, a_{2n}$$$. Для каждой команды указан номер школы, которую она представляет. Гарантируется, что последовательность состоит из чисел от одного до $$$n$$$ и каждое число встречается ровно два раза.

Выходные данные
В единственную строку выходного файла выведите, каким образом следует пересадить команды, чтобы минимальное расстояние между командами из одной школы было как можно больше. При этом среди возможных новых размещений команд следует выбрать такое, чтобы сумма расстояний между старыми и новыми рабочими местами команд была минимально возможной. Если оптимальных ответов несколько, можно вывести любой из них.

Пример
Входные данные
4
1 3 2 2 1 4 4 3

Выходные данные
1 2 4 3 1 2 4 3 
*/

#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
 
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
    Edge(int from, int to, long long capacity, long long flow, long long cost);
    friend std::ostream& operator<<(std::ostream& out, const Edge& e);
 
    long long availableCap();
    int from;
    int to;
    long long capacity;
    long long flow;
    long long cost;
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

    // finds max flow of min cost in a graph.
    long long maxFlowMinCost(int sourceUID, int drainUID);

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
 
    // for BFS algorithm to check if it was in i'th vertice
    bool* used;
 
    unsigned long long maxFlow;


    // checks for negative cycle after maxflow algo
    // returns vertice from negative cycle or -1 if there are no of them
    int negCycle(int sourceUID); 

    // counts cost of edge with a given id
    long long edgeCost(int edgeID);
 
    // finds shortest path between vertices
    bool findShortPath(int startingVertice, int goal, bool(Graph::*criterion)(int));
 
    // updates flow between vertices on the shortest path
    void updateFlow(int startingVert, int goal);

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
    int nSchools = 0;
    scanf("%d\n", &nSchools);
    borders<int>* initialOrder = new borders<int>[nSchools];

    for (int i = 0; i < nSchools; ++i) {
        initialOrder[i].min = -1;
        initialOrder[i].max = -1;
    }

    for (int i = 0; i < 2 * nSchools; ++i) {
        int a = -1;
        scanf("%d", &a);
        --a;
        if (initialOrder[a].min == -1) {
            initialOrder[a].min = i;
        }
        else {
            initialOrder[a].max = i;
        }
    }

    Graph G(2 * nSchools + 2);
    int sourceUID = 2 * nSchools;
    int drainUID = 2 * nSchools + 1;

    for (int school = 0; school < nSchools; ++school) {
        // now school should have its teams at positions j and j + n
        for (int newPosition = nSchools; newPosition < 2 * nSchools; ++newPosition) {
            long long directCost = [school, newPosition, initialOrder, nSchools]() {
                long long absLeft = initialOrder[school].min - newPosition;
                if (absLeft < 0) {
                    absLeft = -absLeft;
                }
                long long absRight = initialOrder[school].max - newPosition - nSchools;
                if (absRight < 0) {
                    absRight = -absRight;
                }
                return absLeft + absRight;
            }();

            G.NewEdge(
                Edge(
                    school, 
                    newPosition, 
                    1, 
                    0, 
                    directCost
                )
            );
        }
    }

    for (int school = 0; school < nSchools; ++school) {
        G.NewEdge(Edge(sourceUID, school, 1, 0, 0));
        G.NewEdge(Edge(school + nSchools, drainUID, 1, 0, 0));
    }

    G.maxFlowMinCost(sourceUID, drainUID);

    delete [] initialOrder;
}
 
Edge::Edge(int from, 
            int to, 
            long long capacity, 
            long long flow,
            long long cost) 
            : 
            from(from), 
            to(to), 
            capacity(capacity), 
            flow(flow),
            cost(cost) {}
 
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

bool isRed(int value) {
    return value == 2 or value == 3 or value == 6 or value == 7;
};

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



    for (int i = 0; i < edges.size(); i += 2) {
        auto& edge = edges[i];
        if (isRed(edge.from) and isRed(edge.to)) {
            fprintf (pFile, "\t%d -> %d [color=red, fontcolor=red, label=\" %d[%lld/%lld]%lld$ \"]\n", edge.from, edge.to, i, edge.flow, edge.capacity, edge.cost);
        }
        else {
            fprintf (pFile, "\t%d -> %d [label=\" %d[%lld/%lld]%lld$ \"]\n", edge.from, edge.to, i, edge.flow, edge.capacity, edge.cost);

        }
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

int Graph::negCycle(int sourceUID) {
    LOGS("INFO >>> searching for neg cycle from %d\n", sourceUID)
    long long* distances = new long long[nVertice];

    fillArray(ancestors, nVertice, -1);
    fillArray(distances, nVertice, LLONG_MAX);

    distances[sourceUID] = LLONG_MIN;

    // source -> layer 1 -> layer 2 -> drain
    // source <- layer 1 <- layer 2 <- drain
    for (int i = 0; i < nVertice; ++i) {
        for (int edgeID = 0; edgeID < edges.size(); ++edgeID) {
            if (distances[edges[edgeID].to] > distances[edges[edgeID].from] + edgeCost(edgeID)) {
                distances[edges[edgeID].to] = distances[edges[edgeID].from] + edgeCost(edgeID);
                ancestors[edges[edgeID].to] = edges[edgeID].from;
                ON_DEBUG(
                    for (int i = 0; i < nVertice; ++i) {
                        printf("ancestor of %d is %d with dist=%lld\n", i, ancestors[i], distances[i]);
                    }
                    printf("\n");
                )
            }
        }
    }

    for (int edgeID = 0; edgeID < edges.size(); ++edgeID) {
        if (distances[edges[edgeID].to] > distances[edges[edgeID].from] + edgeCost(edgeID)) {
            int cur = edges[edgeID].from;
            for (int i = 0; i < nVertice; ++i) {
                cur = ancestors[cur];
            }
            delete [] distances;
            return cur;
        }
    }

exit:
    delete [] distances;
    return -1;
}

long long Graph::maxFlowMinCost(int sourceUID, int drainUID) {
    maxFlowEdmondsKarp(sourceUID, drainUID);
    int nSchool = (nVertice - 2) / 2;
    
    ON_DEBUG(
        //this->photo("before-maxFlowMinCost.png", sourceUID, drainUID);
    )

    int vertFromNegCycle = negCycle(sourceUID);
    int cur = vertFromNegCycle, ancestorCur = ancestors[cur];
    LOGS("INFO >>> %d in negative cycle\n", cur)

    // for prod
    //while (vertFromNegCycle != INT_MAX) {
    for (int i = 0; i < 5; ++i) {
        ON_DEBUG(
            this->photo("data/iter" + std::to_string(i) + ".png", sourceUID, drainUID);
        )
        cur = vertFromNegCycle, ancestorCur = ancestors[cur];
        int toAdd = 0; 

        do {
            if (ancestors[cur] == -1) {
                goto exit;
            }
            int edgeID = findEdge(ancestors[cur], cur, false);
            toAdd = toAdd < edges[edgeID].capacity ? toAdd : edges[edgeID].availableCap();
            cur = ancestorCur;
            LOGS("INFO >>> %d in negative cycle\n", cur)
            ancestorCur = ancestors[cur];
        }
        while (cur != vertFromNegCycle);

        int cur = vertFromNegCycle, ancestorCur = ancestors[cur];
        LOGS("INFO >>> %d in adding %d\n", cur, toAdd)
        do {
            int edgeID = findEdge(ancestors[cur], cur, false);
            edges[edgeID].flow += toAdd;
            edges[edgeID ^ 1].flow -= toAdd;
            cur = ancestorCur;
            LOGS("INFO >>> %d in adding %d\n", cur, toAdd)
            ancestorCur = ancestors[cur];
        }
        while (cur != vertFromNegCycle);

        vertFromNegCycle = negCycle(sourceUID);
    }

    ON_DEBUG(
        this->photo("res-maxFlowMinCost.png", sourceUID, drainUID);
    )

exit:
    int* newOrder = new int[2 * nSchool];

    for (int i = 0; i < nSchool; ++i) {
        for (auto edgeID : whichEdges[i]) {
            if (edges[edgeID].flow == edges[edgeID].capacity) {
                newOrder[edges[edgeID].to - nSchool] = i + 1;
                newOrder[edges[edgeID].to] = i + 1;
            }
        }
    }

    for (int i = 0; i < 2 * nSchool; ++i) {
        printf("%d ", newOrder[i]);
    }

    return 0;
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

long long Graph::edgeCost(int edgeID) {
    const Edge& edge = edges[edgeID];
    if (edge.capacity > edge.flow) {
        return edge.cost;
    }
    else {
        return INT_MAX + edge.cost;
    }
}
 
bool Graph::findShortPath(int startingVertice, int goal, bool(Graph::*criterion)(int)) {
    fillArray(used, nVertice, false);
    //pathLengths[startingVertice] = 0;
 
    //LOGS("INFO >>> bfs\n")
 
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
                        //printf("goto used\n");
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
    //LOGS("INFO >>> checking %d'th edge from %d to %d with %lld/%lld\n", edgeID, edges[edgeID].from, edges[edgeID].to, edges[edgeID].flow, edges[edgeID].capacity)

    return edges[edgeID].flow < edges[edgeID].capacity;
}
 
template<typename T>
void Graph::fillArray(T* array, int nElems, const T& value) {
    for (int i = 0; i < nElems; ++i) {
        array[i] = value;
    }
}
 
void Graph::NewEdge(const Edge& newEdge) {
    //LOGS("INFO >>> adding new edge %d -> %d with cap=%lld\n", newEdge.from, newEdge.to, newEdge.capacity)

    if (newEdge.from == newEdge.to) {
        return;
    }

    
    edges.push_back(newEdge);
    edges.push_back(Edge{newEdge.to, newEdge.from, 0, 0, -newEdge.cost});
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
            //std::cout << *this;
        )
    }
    auto ret = maxFlow;
    maxFlow = 0;

    LOGS("INFO >>> maxflow is %llu\n", ret);
 
    return ret;
}
 
void Graph::updateFlow(int startingVert, int goal) {
    //LOGS("INFO >>> updating flow\n")
    long long minCapacity = LLONG_MAX;
    int curVert = goal;
    while (curVert != startingVert) {
        //LOGS("INFO >>> by edge %d -> %d", edges[ancestors[curVert]].from, curVert)

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
 
    //LOGS("INFO >>> increased maxflow (%llu) by %lld\n", maxFlow, minCapacity)
 
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