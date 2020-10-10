/*
Вася и Сережа играют в следующую игру. 
В некоторых клетках клетчатого листка Сережа рисует один из символов 'H', 'O', 'N' или 'C', 
после чего Вася должен провести между некоторыми находящимися в соседних клетках 
символами линии так, 
чтобы получилось корректное изображение химической молекулы. 

К сожалению, Сережа любит рисовать много символов, и Вася не может сразу определить, 
возможно ли вообще нарисовать линии нужным способом. 
Помогите ему написать программу, которая даст ответ на этот вопрос.

В этой задаче проведенные между символами химических элементов линии будем 
считать корректным изображением молекулы, если они удовлетворяют следующим условиям:

1) каждая линия соединяет символы, нарисованные в соседних (по стороне) клетках,
2) между каждой парой символов проведено не более одной линии,
3) от каждого элемента отходит ровно столько линий, какова валентность 
этого элемента (1 для H, 2 для O, 3 для N, 4 для C),
4) пустые клетки ни с чем не соединены, и
5) хотя бы в одной клетке нарисован какой-то символ.

Входные данные
Первая строка входного файла содержит два натуральных числа 𝑛n и 𝑚m (1≤𝑛,𝑚≤501≤n,m≤50) 
— размеры листочка, на котором рисует Сережа. 
Далее следуют 𝑛n строк по 𝑚m символов в каждой, задающих конфигурацию 
химических элементов, которую нарисовал Сережа; пустые клетки задаются символом '.'.

Выходные данные
В выходной файл выведите одно слово: 'Valid', если линии провести 
требуемым образом можно, и 'Invalid', если нельзя.
*/

#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
 
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

class GraphFill {
public:
    GraphFill(Graph& G, char** matrix, int nRows, int nCols);

    void addEdges();

    const int SourceUID;
    const int DrainUID;

private:
    Graph& G;
    char** matrix;
    int nRows; 
    int nCols;

    enum direction {
        LEFT = 0,
        RIGHT = 1, 
        UP = 2, 
        DOWN = 3,
    };

    inline bool isWhite(int row, int column);

    inline int getCap(int row, int column);

    inline int getUID(int row, int column);

    void checkNeighbours(int row, int col);

    void addNeighbour(int fromRow, int fromCol, direction Dir);

};










/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/












int main() {
    int nRows = 0, nCols;
    scanf("%d %d\n", &nRows, &nCols);
 
    char** matrix = new char*[nRows];

    for (int i = 0; i < nRows; ++i) {
        matrix[i] = new char[nCols];
    }
    bool wasNotDot = false;
    for (int i = 0; i < nRows; ++i) {
        scanf("%[^\n]", matrix[i]);
        //printf("read line \"%s\"\n", matrix[i]);
        for (int j = 0; j < nCols; ++j) {
            if (matrix[i][j] != '.') {
                wasNotDot = true;
            }
        }
        getchar();
    }

    if (!wasNotDot) {
        printf("Invalid");
        return 0;
    }

    Graph G(nRows * nCols + 2);

    GraphFill gFill(G, matrix, nRows, nCols);
    gFill.addEdges();

    //G.photo("data/before.png", gFill.SourceUID, gFill.DrainUID);

    // Check if all vertices, except S and T are satiated
    if (G.isValid(gFill.SourceUID, gFill.DrainUID)) {
        printf("Valid");
    }
    else {
        printf("Invalid");
    }

    //G.photo("data/after.png", gFill.SourceUID, gFill.DrainUID);

    for (int i = 0; i < nRows; ++i) {
        delete [] matrix[i];
    }
    delete [] matrix;

}

GraphFill::GraphFill(Graph& G, char** matrix, int nRows, int nCols) : 
    G(G), 
    matrix(matrix), 
    nRows(nRows), 
    nCols(nCols), 
    SourceUID(nRows * nCols), 
    DrainUID(nRows * nCols + 1) 
    {}

inline bool GraphFill::isWhite(int row, int column) {
    return (row + column) % 2 == 0;
}

inline int GraphFill::getUID(int row, int column) {
    return row * nCols + column;
}

inline int GraphFill::getCap(int row, int column) {
    switch (matrix[row][column]) {
        case '.': {
            return 0;
        }
        case 'H': {
            return 1;
        }
        case 'O': {
            return 2;
        }
        case 'N': {
            return 3;
        }
        case 'C': {
            return 4;
        }
        default: {
            LOGS("ERROR >>> unknown symbol \"%c\" with code %d in matrix[%d][%d]\n", matrix[row][column],  matrix[row][column], row, column)
            return -1;
        }
    };
}

void GraphFill::addEdges() {
    for (int row = 0; row < nRows; ++row) {
        for (int col = 0; col < nCols; ++col) {
            if (matrix[row][col] != '.') {
                if (isWhite(row, col)) {
                    LOGS("white vert %d[%d, %d]\n", getUID(row, col), row, col)
                    G.NewEdge({SourceUID, getUID(row, col), getCap(row, col), 0});
                    checkNeighbours(row, col);
                }
                else {
                    LOGS("black vert %d[%d, %d] with cap %d\n", getUID(row, col), row, col, getCap(row, col))
                    G.NewEdge({getUID(row, col), DrainUID, getCap(row, col), 0});
                }
            }
        }
    }
}

void GraphFill::addNeighbour(int fromRow, int fromCol, direction Dir) {
    if (getCap(fromRow, fromCol) == 0) {
        return;
    }

    switch (Dir) {
        case LEFT: {
            if (fromCol - 1 < 0) {
                break;
            }
            if (getCap(fromRow, fromCol - 1) == 0) {
                break;
            }
            G.NewEdge(
                Edge(
                    getUID(fromRow, fromCol), 
                    getUID(fromRow, fromCol - 1), 
                    1, 
                    0
                )
            );
            break;
        }
        case RIGHT: {
            if (fromCol + 1 >= nCols) {
                break;
            }
            if (getCap(fromRow, fromCol + 1) == 0) {
                break;
            }
            G.NewEdge(
                Edge(
                    getUID(fromRow, fromCol), 
                    getUID(fromRow, fromCol + 1), 
                    1, 
                    0
                )
            );
            break;
        }
        case UP: {
            if (fromRow - 1 < 0) {
                break;
            }
            if (getCap(fromRow - 1, fromCol) == 0) {
                break;
            }
            G.NewEdge(
                Edge(
                    getUID(fromRow, fromCol), 
                    getUID(fromRow - 1, fromCol), 
                    1, 
                    0
                )
            );
            break;
        }
        case DOWN: {
            if (fromRow + 1 >= nRows) {
                break;
            }
            if (getCap(fromRow + 1, fromCol) == 0) {
                break;
            }
            G.NewEdge(
                Edge(
                    getUID(fromRow, fromCol), 
                    getUID(fromRow + 1, fromCol), 
                    1, 
                    0
                )
            );
            break;
        }
        default: {
            LOGS("ERROR >>> unknown direction: %d", Dir)
        }
    }
}

void GraphFill::checkNeighbours(int row, int col) {
    LOGS("Adding neighs to [%d, %d] with id %d\n", row, col, getUID(row, col))

    if (row == 0) {
        if (col == 0) {
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, DOWN);
        }
        else if (col == nCols - 1) {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, DOWN);
        }
        else {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, DOWN);
        }
    }
    else if (row == nRows - 1) {
        if (col == 0) {
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, UP);
        }
        else if (col == nCols - 1) {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, UP);
        }
        else {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, UP);
            }
        }
    else {
        if (col == 0) {
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, UP);
            addNeighbour(row, col, DOWN);
        }
        else if (col == nCols - 1) {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, UP);
            addNeighbour(row, col, DOWN);
        }
         else {
            addNeighbour(row, col, LEFT);
            addNeighbour(row, col, RIGHT);
            addNeighbour(row, col, UP);
            addNeighbour(row, col, DOWN);
        }
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
        if (edge.capacity == 1 and edge.flow >= 0) {
            fprintf (pFile, "\t%d -> %d [label=\" %lld/inf \"]\n", edge.from, edge.to, edge.flow);
            continue;
        }
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