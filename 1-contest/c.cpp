#include <stdio.h>
#include <math.h>
#include <vector>

//#define DEBUG

#ifdef DEBUG
    #define LOGS(...) printf(__VA_ARGS__);
#else
    #define LOGS(...) ;
#endif

class bipartite_graph {
public:
    typedef int vert_id;

    bipartite_graph(int nVert): nVert(nVert),
                                edges(new std::vector<vert_id>[nVert]), 
                                combination(new vert_id[nVert]), 
                                was(new vert_id[nVert]) {
        if (nVert < 0) {
            printf("INFO >>> problems in creating graph: negative number of vertices in one of parts\n");
        }

        for (int i = 0; i < nVert; ++i) {
            combination[i] = -1;
            was[i] = -1;
        } 
        LOGS("INFO >>> creating edges\n")
        getEdges();

        LOGS("INFO >>> bipartite_graph created\n")
    }

    void max_matching() {
        for (vert_id vert = 0; vert < nVert; ++vert) {
            //LOGS("VERT %d\n", vert)
            for (vert_id i = 0; i < nVert; ++i) {
                was[i] = 0;
            }
            if (kuhn_algo(vert)) {
                ++max_edges;
            }
        }
        printf("%d", nVert - max_edges);

        /*
        for (vert_id vert = 0; vert < nVert; ++vert) {
            if (combination[vert] != -1) {
                printf("%d %d\n", combination[vert] + 1, vert + 1);
            }
        }
        */
    }

    void view_graph() {
        printf("\n\n<<< Viewing graph >>>\nLeft part has %d vertices, right part has %d\n", nVert, nVert);
        for (vert_id vert = 0; vert < nVert; ++vert) {
            if (edges[vert].size() != 0) {
                printf("\tFor vertice %d neighbours are: ", vert);
                for (auto elem : edges[vert]) {
                    printf("%d ", elem);
                }
                printf("\n");
            }
            else {
                printf("\tNo neighbours for vertice %d\n", vert);
            }
        }
        printf("<<< That's all for this graph >>> \n\n\n");
    }

    ~bipartite_graph() {
        delete [] edges;
        delete [] was;
        delete [] combination;
    }

private:
    typedef int time;

    struct position {
        int x;
        int y;
    };

    struct order {
        position from;
        position to;
        time when;
        void setTime(char strTime[5]) {
            const int minInHour = 60;
            when = minInHour * (10 * (strTime[0] - '0') + (strTime[1] - '0')) + 
                    (10 * (strTime[3] - '0') + (strTime[4] - '0'));
        }
    };

    const int nVert;

    std::vector<vert_id>* edges; // edges[i] - list of all edges from vertice i to vertices in right half
    vert_id* combination;
    vert_id* was;

    order* orders = nullptr;

    int max_edges = 0;

    void getEdges() {
        orders = new order[nVert];
        char strTime[6] = {};
        for (int i = 0; i < nVert; ++i) {
            LOGS("INFO >>> working with %d event\n", i)
            scanf("%5s %d %d %d %d\n", strTime, &(orders[i].from.x), &(orders[i].from.y), 
                                                    &(orders[i].to.x), &(orders[i].to.y));                                     
            orders[i].setTime(strTime);
            LOGS("INFO >>> {%d: (%d, %d) -> (%d, %d)}\n", orders[i].when, orders[i].from.x, orders[i].from.y, orders[i].to.x, orders[i].to.y)
            seekPossible(i);
        }
        delete [] orders;
        orders = nullptr;
    }

    void seekPossible(vert_id index) {
        for (vert_id i = 0; i < index; ++i) {
            seekEdge(i, index);
        }
    }

    void seekEdge(vert_id first, vert_id second) {
        LOGS("INFO >>> seeking edge %d -> %d\n", first, second)
        time toGo = abs(orders[second].from.x - orders[first].to.x) + 
                    abs(orders[second].from.y - orders[first].to.y);
        time previousOrder = abs(orders[first].from.x - orders[first].to.x) + 
                                abs(orders[first].from.y - orders[first].to.y);
        LOGS("\t\t time to go %d + %d minutes, needed %d minutes\n", toGo, previousOrder, abs(orders[first].when - orders[second].when))
        if (toGo + previousOrder < abs(orders[first].when - orders[second].when)) {
            edges[first].push_back(second);
            // maybe reversed also?
        }
    }

    bool kuhn_algo(vert_id vertice) {
        if (was[vertice]) {
            return false;
        }
        was[vertice] = true;
        for (int i = 0; i < edges[vertice].size(); ++i) {
            LOGS("KUHN EDGE: %d -> *(%d)\n", vertice, i)
            vert_id to = edges[vertice][i];
            if (combination[to] == -1 or kuhn_algo(combination[to])) {
                combination[to] = vertice;
                LOGS("NOTE >>> Candidate for answer: %d -> %d\n", vertice, to)
                return true;
            }
            LOGS("INFO >>> kuhn iteration\n")
        }
        return false;
    }
};

int main() {
    int nVert = 0;
    scanf("%d\n", &nVert);

    bipartite_graph graph(nVert);

    //graph.view_graph();

    graph.max_matching();
}
