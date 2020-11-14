#include <stdio.h>
#include <vector>

#define DEBUG

#ifdef DEBUG
    #define LOGS(...) printf(__VA_ARGS__);
#else
    #define LOGS(data, ...) ;
#endif

class bipartite_graph {
public:
    typedef int vert_id;

    bipartite_graph(int left_part, int right_part): left_part(left_part), right_part(right_part) {
        if (left_part < 0 or right_part < 0) {
            printf("INFO >>> problems in creating graph: negative number of vertices in one of parts\n");
        }

        edges = new std::vector<vert_id>[left_part];
        combination = new vert_id[right_part];
        was = new vert_id[right_part];

        for (int i = 0; i < right_part; ++i) {
            combination[i] = -1;
            was[i] = -1;
        }

        for (int i = 0; i < left_part; ++i) {
            vert_id neighbour = -1;
            scanf("%d", &neighbour);
            while (neighbour != 0) {
                --neighbour;                    
                edges[i].push_back(neighbour);
                scanf("%d", &neighbour);
            }
            LOGS("INFO >>> vertice known\n")
        }
        LOGS("INFO >>> bipartite_graph created\n")
    }

    void max_matching() {
        for (vert_id vert = 0; vert < left_part; ++vert) {
            LOGS("VERT %d\n", vert)
            for (vert_id i = 0; i < left_part; ++i) {
                was[i] = 0;
            }
            if (kuhn_algo(vert)) {
                ++max_edges;
            }
        }
        printf("%d\n", max_edges);

        for (vert_id vert = 0; vert < right_part; ++vert) {
            if (combination[vert] != -1) {
                printf("%d %d\n", combination[vert] + 1, vert + 1);
            }
        }
    }

    void view_graph() {
        printf("\n\n<<< Viewing graph >>>\nLeft part has %d vertices, right part has %d\n", left_part, right_part);
        for (vert_id vert = 0; vert < left_part; ++vert) {
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
    const int left_part, right_part;

    std::vector<vert_id>* edges;
    vert_id* combination;
    vert_id* was;

    int max_edges = 0;

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
    int left_part = 0, right_part = 0;
    scanf("%d %d", &left_part, &right_part);

    bipartite_graph graph(left_part, right_part);

    //graph.view_graph();

    graph.max_matching();
}
