#include <stdio.h>
#include <vector>

//#define DEBUG

#ifdef DEBUG
    #define LOGS(...) printf(__VA_ARGS__);
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define LOGS(data, ...) ;
    #define ON_DEBUG(...) ;
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
        was = new vert_id[left_part];

        usedLeft = new bool[left_part];
        usedRight = new bool[right_part];

        for (int i = 0; i < right_part; ++i) {
            combination[i] = unknownVert;
        }

        for (int i = 0; i < left_part; ++i) {
            was[i] = unknownVert;
        }

        for (int i = 0; i < left_part; ++i) {
            nillArray(usedRight, right_part, false);
            vert_id neighbour = unknownVert;
            scanf("%d", &neighbour);
            while (neighbour != 0) {      
                usedRight[neighbour - 1] = true;
                scanf("%d", &neighbour);
            }
            for (int j = 0; j < right_part; ++j) {
                if (!usedRight[j]) {
                    //printf("known edge: (%d)<->(%d)\n", i + 1, j + 1);
                    edges[i].push_back(j);
                }
            }
            LOGS("INFO >>> vertice known\n")
        }
        nillArray(usedRight, right_part, false);
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

        for (vert_id vert = 0; vert < right_part; ++vert) {
            if (combination[vert] != unknownVert) {
                LOGS("INFO >>> edge in max matching (%d)<->(%d)\n", combination[vert] + 1, vert + 1);
                vert_id destination = combination[vert];
                for (auto i = edges[destination].begin(); i < edges[destination].end(); ++i) {
                    if (*i == vert) {
                        edges[destination].erase(i);
                        break;
                    }
                }
            }
        }
    }

    void findMVC() {
        nillArray(usedLeft, left_part, false);
        nillArray(usedRight, right_part, false);

        bool* wasInMatching = new bool[left_part];
        nillArray(wasInMatching, left_part, false);

        for (int i = 0; i < right_part; ++i) {
            if (combination[i] != unknownVert) {
                wasInMatching[combination[i]] = true;
            }
        }

        for (vert_id vert = 0; vert < left_part; ++vert) {
            if (!wasInMatching[vert]) {
                dfsMVC(vert, true);
            }
        }

        delete [] wasInMatching;

        // need L+ and R- (https://neerc.ifmo.ru/wiki/index.php?title=%D0%A1%D0%B2%D1%8F%D0%B7%D1%8C_%D0%BC%D0%B0%D0%BA%D1%81%D0%B8%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D0%B3%D0%BE_%D0%BF%D0%B0%D1%80%D0%BE%D1%81%D0%BE%D1%87%D0%B5%D1%82%D0%B0%D0%BD%D0%B8%D1%8F_%D0%B8_%D0%BC%D0%B8%D0%BD%D0%B8%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D0%B3%D0%BE_%D0%B2%D0%B5%D1%80%D1%88%D0%B8%D0%BD%D0%BD%D0%BE%D0%B3%D0%BE_%D0%BF%D0%BE%D0%BA%D1%80%D1%8B%D1%82%D0%B8%D1%8F_%D0%B2_%D0%B4%D0%B2%D1%83%D0%B4%D0%BE%D0%BB%D1%8C%D0%BD%D1%8B%D1%85_%D0%B3%D1%80%D0%B0%D1%84%D0%B0%D1%85)
        printf("%d %d\n", usedLeftCount, right_part - usedRightCount);
        for (vert_id boy = 0; boy < left_part; ++boy) {
            if (usedLeft[boy]) {
                printf("%d ", boy + 1);
            }
        }
        putchar('\n');
        for (vert_id girl = 0; girl < right_part; ++girl) {
            if (!usedRight[girl]) {
                printf("%d ", girl + 1);
            }
        }
    }

    void view_graph() {
        printf("\n\n<<< Viewing graph >>>\nLeft part has %d vertices, right part has %d\n", left_part, right_part);
        for (vert_id vert = 0; vert < left_part; ++vert) {
            if (edges[vert].size() != 0) {
                //printf("\tFor vertice %d neighbours are: ", vert);
                for (auto elem : edges[vert]) {
                    printf("(%d)->(%d)\n", vert + 1, elem + 1);
                }
            }
        }
        for (vert_id vert = 0; vert < right_part; ++vert) {
            if (combination[vert] != unknownVert) {
                printf("(%d)<-(%d)\n", combination[vert] + 1, vert + 1);
            }
        }
        printf("<<< That's all for this graph >>> \n\n\n");
    }

    ~bipartite_graph() {
        delete [] edges;
        delete [] was;
        delete [] combination;
        delete [] usedLeft;
        delete [] usedRight;
    }

private:
    const int left_part, right_part;

    bool* usedLeft, * usedRight;
    int usedLeftCount = 0, usedRightCount = 0;

    std::vector<vert_id>* edges;
    vert_id* combination;
    vert_id* was;

    const vert_id unknownVert = -1;

    int max_edges = 0;

    void dfsMVC(vert_id vert, bool isLeft) {
        if (isLeft and !usedLeft[vert]) {
            LOGS("INFO >>> dfs(l) from %d\n", vert)
            usedLeft[vert] = true;
            ++usedLeftCount;
            for (vert_id to = 0; to < edges[vert].size(); ++to) {
                LOGS("INFO >>> dfs(l) from %d to %d\n", vert, edges[vert][to])
                dfsMVC(edges[vert][to], false);
            }
        }
        else if (!isLeft and !usedRight[vert]) {
            usedRight[vert] = true;
            LOGS("INFO >>> dfs(r) from %d to %d\n", vert, combination[vert])
            ++usedRightCount;
            if (combination[vert] != unknownVert) {
                dfsMVC(combination[vert], true);
            }
        }
    }

    template<typename T>
    void nillArray(T* array, int size, T value) {
        for (int i = 0; i < size; ++i) {
            array[i] = value;
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
            if (combination[to] == unknownVert or kuhn_algo(combination[to])) {
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
    int nTasks = 0;
    scanf("%d\n", &nTasks);

    for (int i = 0; i < nTasks; ++i) {
        int left = 0, right = 0;
        scanf("%d %d\n", &left, &right);
        bipartite_graph graph(left, right);
        graph.max_matching();
        //graph.view_graph();
        graph.findMVC();
        printf("\n\n");
    }
}
