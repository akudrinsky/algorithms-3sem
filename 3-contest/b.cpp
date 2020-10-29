/*
Кубики
Имя входного файла: стандартный ввод
Имя выходного файла: стандартный вывод
Ограничение по времени: 1.5 секунд
Ограничение по памяти: 256 мегабайт
Привидение Петя любит играть со своими кубиками. 
Он любит выкладывать их в ряд и разглядывать свое творение. 
Однако недавно друзья решили подшутить над Петей и поставили в его игровой комнате зеркало. 
Ведь всем известно, что привидения не отражаются в зеркале
А кубики отражаются. Теперь Петя видит перед собой N цветных кубиков, но не знает, какие из этих кубиков настоящие, а какие — всего лишь отражение в зеркале. 
Помогите Пете! Выясните, сколько кубиков может быть у Пети. 
Петя видит отражение всех кубиков в зеркале и часть кубиков, которая находится перед ним. 
Часть кубиков может быть позади Пети, их он не видит.

Формат входных данных
Первая строка входного файла содержит число N (1 6 N 6 106) и количество различных цветов,
в которые могут быть раскрашены кубики — M (1 6 M 6 106). 
Следующая строка содержит Nцелых чисел от 1 до M — цвета кубиков.

Формат выходных данных
Выведите в выходной файл все такие K, что у Пети может быть K кубиков в подрядке возрастания


Пример
стандартный ввод
6 2
1 1 2 2 1 1

стандартный вывод
3 5 6

*/

// https://algorithmica.org/ru/hashing

#include <cstdio>
#include <climits>
#include <vector>
#include <omp.h>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>

#pragma GCC optimize("Ofast")
#pragma GCC 

// for DEBUG mode give -DDEBUG option to g++
#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
    #define LOGS(...)\
        printf(__VA_ARGS__);\
        printf("\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, __func__);
#else
    #define LOGS(data, ...) ;
    #define ON_DEBUG(...) ;
#endif

/*
// Class that represents suffix tree on arrays of type T
template<typename T>
class SuffixTree {
public:
    SuffixTree(int nLetters);

    ~SuffixTree();

    bool NewElem(T* firstLetter, T* lastLetter);

    bool HasElem(T* firstLetter, T* lastLetter);

    bool Visualise(std::string filename);

    class Node {
    public:


    private:
        bool isTerminal;
        Node children[];
        Node* suffixLink;
    };

private:
    int nLetters;


};
*/

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

struct Direction {
    int startingIndex;
    int endingIndex;
    inline int by(int index) const;
    inline int length() const;
};

bool equalStrings(int* array, const Direction& first, const Direction& second);

int main() {
    LOGS("Starting with DEBUG mode\n")

    int nElems = 0, nColors = 0;
    scanf("%d %d\n", &nElems, &nColors);
    int* elems = new int[nElems];
    for (int i = 0; i < nElems; ++i) {
        scanf("%d", elems + i);
    }

    LOGS("All input data was read\n")

    //#pragma omp for schedule (static, 10)
    for (int i = nElems / 2 - 1; i >= 0; --i) {
        if (equalStrings(elems, {i, 0}, {i + 1, 2 * i + 1})) {
            printf("%d ", nElems - i - 1);
        }
    }

    printf("%d ", nElems);

    delete [] elems;
}

inline int Direction::length() const {
    return abs(startingIndex - endingIndex) + 1;
}

inline int Direction::by(int index) const {
    if (startingIndex < endingIndex) {
        return startingIndex + index;
    }
    else if (startingIndex == endingIndex) {
        return startingIndex;
    }
    else {
        return startingIndex - index;
    }
}

bool equalStrings(int* array, const Direction& first, const Direction& second) {
    if (first.length() != second.length()) {
        return false;
    }

    int commonLength = first.length();

    for (int i = 0; i < commonLength; ++i) {
        if (array[first.by(i)] != array[second.by(i)]) {
            return false;
        }
    }

    return true;
}