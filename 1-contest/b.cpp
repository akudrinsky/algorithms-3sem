#include <stdio.h>
#include <vector>

//#define DEBUG

#ifdef DEBUG
    #define LOGS(...) printf(__VA_ARGS__);
#else
    #define LOGS(data, ...) ;
#endif

class bipartite_graph {
public:
    typedef int vert_id;

    bipartite_graph(const char* filename, const char* output): output(output) {
        FILE* input = fopen(filename, "r");
        fscanf(input, "%d %d %d %d\n", &rowsNumber, &columnsNumber, &two_price, &one_price);

        left_part = (rowsNumber / 2) * columnsNumber + (rowsNumber % 2) * ((columnsNumber + 1) / 2);
        right_part = rowsNumber * columnsNumber - left_part;

        LOGS("INFO >>> %d in left, %d in right\n", left_part, right_part)


        LOGS("INFO >>> preparing internal fields\n")
        initInternal();

        LOGS("INFO >>> allocating matrix\n")
        allocateMatrix();

        LOGS("INFO >>> filling matrix with input data\n")
        fillMatrix(input);

        LOGS("INFO >>> counting free fields\n")
        countFree();

        if (one_price * 2 <= two_price) {
            fclose(input);
            FILE* pFile = fopen(output, "w");
            fprintf(pFile, "%d\n", freeFields * one_price);
            fclose(pFile);
            freeMatrix();
            return;
        }

        LOGS("INFO >>> making edges\n")
        getEdges();
        
        LOGS("INFO >>> freeing matrix\n")
        freeMatrix();
        LOGS("INFO >>> bipartite_graph created\n")
    }

    void max_matching() {
        for (vert_id vert = 0; vert < left_part; ++vert) {
            LOGS("VERT %d\n", vert)
            for (vert_id i = 0; i < left_part; ++i) {
                used[i] = 0;
            }
            if (kuhn_algo(vert)) {
                ++max_edges;
            }
        }
        FILE* pFile = fopen(output, "w");
        fprintf(pFile, "%d\n", two_price * max_edges + one_price * (freeFields - 2 * max_edges));
        fclose(pFile);
    }

    void view_graph() {
        printf("\n\n<<< Viewing graph >>>\nLeft part has %d vertices, right part has %d\n", rowsNumber, columnsNumber);
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
    }

private:
    enum direction {
        LEFT = 0,
        RIGHT = 1, 
        UP = 2, 
        DOWN = 3,
    };
    int rowsNumber = 0, columnsNumber = 0;
    int left_part = 0, right_part = 0;
    int two_price = 0, one_price = 0;

    int freeFields = 0;

    bool** occupied = nullptr;

    const char* output;

    std::vector<vert_id>* edges;
    std::vector<vert_id> combination;
    std::vector<vert_id> used;

    int max_edges = 0;

    void countFree() {
        freeFields = 0;
        for (int i = 0; i < rowsNumber; ++i) {
            for (int j = 0; j < columnsNumber; ++j) {
                if(!occupied[i][j]) {
                    ++freeFields;
                }
            }
        }
    }

    bool kuhn_algo(vert_id vertice) {
        if (used[vertice]) {
            return false;
        }
        used[vertice] = true;
        for (unsigned long i = 0; i < edges[vertice].size(); ++i) {
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

    void allocateMatrix() {
        occupied = new bool*[rowsNumber];
        for (int row = 0; row < rowsNumber; ++row) {
            occupied[row] = new bool[columnsNumber];
        }
    }

    void freeMatrix() {
        for (int row = 0; row < rowsNumber; ++row) {
            delete [] occupied[row];
        }
        delete [] occupied;
    }

    void initInternal() {
        if (rowsNumber < 0 or columnsNumber < 0) {
            printf("INFO >>> problems in creating graph: negative number of vertices in one of the parts\n");
            return;
        }

        edges = new std::vector<vert_id>[left_part];
        combination = std::vector<vert_id>(right_part, -1);
        used = std::vector<vert_id>(left_part, -1);
    }

    void fillMatrix(FILE* input) {
        char cell = 0;
        for (int row = 0; row < rowsNumber; ++row) {
            for (int column = 0; column < columnsNumber; ++column) {
                cell = fgetc(input);
                if (cell == '.') {
                    occupied[row][column] = true;
                }
                else if (cell == '*') {
                    occupied[row][column] = false;
                }
                else {
                    LOGS("ERROR >>> unknown symbol: %d\n", cell)
                }
            }
            fgetc(input);
        }
        LOGS("INFO >>> Matrix filled\n")
    }

    void getEdges() {
        for (int row = 0; row < rowsNumber; ++row) {
            for (int col = 0; col < columnsNumber; ++col) {
                //isRight = ((row + col) % 2 == 0) ? false : true;
                if (isLeft(row, col)) {
                    checkNeighbours(row, col);
                }
            }
        }
    }

    void checkNeighbours(int row, int col) {
        LOGS("INFO >>> checking (row=%d, col=%d)\n", row, col)
        if (!occupied[row][col]) {
            if (row == 0) {
                if (col == 0) {
                    addNeighbour(row, col, RIGHT);
                    addNeighbour(row, col, DOWN);
                }
                else if (col == columnsNumber - 1) {
                    addNeighbour(row, col, LEFT);
                    addNeighbour(row, col, DOWN);
                }
                else {
                    addNeighbour(row, col, LEFT);
                    addNeighbour(row, col, RIGHT);
                    addNeighbour(row, col, DOWN);
                }
            }
            else if (row == rowsNumber - 1) {
                if (col == 0) {
                    addNeighbour(row, col, RIGHT);
                    addNeighbour(row, col, UP);
                }
                else if (col == columnsNumber - 1) {
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
                else if (col == columnsNumber - 1) {
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
    }

    void addNeighbour(int fromRow, int fromCol, direction Dir) {
        LOGS("INFO >>> adding neighbour %d of (row=%d, col=%d)\n", Dir, fromRow, fromCol)
        int toRow = fromRow, toCol = fromCol;
        switch (Dir) {
            case LEFT: {
                if (leftOccupied(fromRow, fromCol)) {
                    return;
                }
                --toCol;
                break;
            }
            case RIGHT: {
                if (rightOccupied(fromRow, fromCol)) {
                    return;
                }
                ++toCol;
                break;
            }
            case UP: {
                LOGS("UP\n")
                if (upOccupied(fromRow, fromCol)) {
                    return;
                }
                --toRow;
                break;
            }
            case DOWN: {
                if (downOccupied(fromRow, fromCol)) {
                    return;
                }
                ++toRow;
                break;
            }
        }
        int fromId = (fromRow * columnsNumber + fromCol) / 2;
        int toId = (toRow * columnsNumber + toCol) / 2;

        LOGS("INFO >>> edge (row=%d, col=%d, id=%d) -> (row=%d, col=%d, id=%d)\n", 
            fromRow, fromCol, fromId, 
            toRow, toCol, toId
        );

        if (isLeft(fromRow, fromCol)) {
            edges[fromId].push_back(toId);
        }
        else {
            edges[toId].push_back(fromId);
        }
    }

    bool isLeft(int Row, int Col) {
        return (Row + Col) % 2 == 0;
    }

    bool leftOccupied(int row, int col) {
        return occupied[row][col - 1];
    }

    bool rightOccupied(int row, int col) {
        return occupied[row][col + 1];
    }

    bool upOccupied(int row, int col) {
        return occupied[row - 1][col];
    }

    bool downOccupied(int row, int col) {
        return occupied[row + 1][col];
    }
};

int main() {
    bipartite_graph graph("dominoes.in", "dominoes.out");

    //graph.view_graph();

    graph.max_matching();
}
