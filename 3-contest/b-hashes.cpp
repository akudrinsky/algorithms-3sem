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
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>

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

class AbstractStringHash {
public:
    virtual unsigned long long* operator()(
        const int* data, 
        const unsigned long long powers, 
        int nElems, 
        unsigned long long mod,
        bool isForward
    ) const = 0;
};

class PolyHash : private AbstractStringHash {
public:
    virtual unsigned long long* operator()(
        const int* data, 
        const unsigned long long powers, 
        int nElems, 
        unsigned long long mod,
        bool isForward
    ) const override;
};

struct DirectedString {
    int startingIndex;  // including
    int endingIndex;    // including

    int len() const;
    int by(int i) const;
};

template <typename AbstractStringHash>
class HashString : public AbstractStringHash {
public:
    HashString(
        const int* data, 
        int nElems, 
        unsigned long long forPowers, 
        unsigned long long mod
    );

    bool equalStrings(const DirectedString& first, const DirectedString& second);

    unsigned long long stringHash(const DirectedString& str);

    ~HashString();

private:
    const int* data;
    unsigned long long* forwardHashes;
    unsigned long long* backwardHashes;
    unsigned long long* nElemsPowers;
    unsigned long long forPowers;
    unsigned long long mod;
    int nElems;

    bool reallyEqual(const DirectedString& first, const DirectedString& second);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int main() {
    LOGS("Starting with DEBUG mode\n")

    int nElems = 0, nColors = 0;
    scanf("%d %d\n", &nElems, &nColors);
    int* elems = new int[nElems];
    for (int i = 0; i < nElems; ++i) {
        scanf("%d", elems + i);
    }

    LOGS("All input data was read\n")

    HashString<PolyHash> solver(elems, nElems, 1000003, 10004983);

    DirectedString first = {};
    DirectedString second = {};

    for (int i = nElems / 2 - 1; i >= 0; --i) {
        DirectedString first = {i, 0};
        DirectedString second = {i + 1, 2 * i + 1};

        if (solver.equalStrings(first, second)) {
            printf("%d ", nElems - i - 1);
        }
    }
    printf("%d ", nElems);

    delete [] elems;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int DirectedString::len() const {
    return abs(endingIndex - startingIndex) + 1;
}

int DirectedString::by(int i) const {
    if (startingIndex < endingIndex) {
        return startingIndex + i;
    }
    else if (startingIndex == endingIndex) {
        return startingIndex;
    }
    else {
        return startingIndex - i;
    }
}

unsigned long long* PolyHash::operator()(
    const int* data, 
    const unsigned long long power, 
    int nElems, 
    unsigned long long mod,
    bool isForward
) const {
    unsigned long long* result = new unsigned long long[nElems + 1];
    if (isForward) {
        LOGS("Counting forward hashes:\n")
        result[0] = 0;
        for (int i = 1; i <= nElems; ++i) {
            result[i] = (result[i - 1] * power + data[i - 1]) % mod;            
        }
        ON_DEBUG(
            for (int i = 0; i <= nElems; ++i) {
                printf("result[%d] = %llu\n", i, result[i]);           
            }
        )
        LOGS("That's it\n")
    }
    else {
        LOGS("Counting backward hashes\n")
        result[nElems - 1] = 0;
        for (int i = nElems - 1; i >= 0; --i) {
            result[i] = (result[i + 1] * power + data[i]) % mod;            
        }
        ON_DEBUG(
            for (int i = 0; i <= nElems; ++i) {
                printf("result[%d] = %llu\n", i, result[i]);           
            }
        )
        LOGS("That's it\n")
    }

    return result;
}

template <typename AbstractStringHash>
HashString<AbstractStringHash>::HashString(
    const int* data, 
    int nElems, 
    unsigned long long forPowers, 
    unsigned long long mod
) 
    : 
    data(data), 
    nElems(nElems),
    forwardHashes(nullptr),
    backwardHashes(nullptr),
    nElemsPowers(new unsigned long long[nElems]),
    forPowers(forPowers),
    mod(mod) {

    nElemsPowers[0] = 1;
    for (int i = 1; i < nElems; ++i) {
        nElemsPowers[i] = (nElemsPowers[i - 1] * forPowers) % mod;
        // ???!?!?
        /*
        if (nElemsPowers[i] == 0) {
            nElemsPowers[i] = 1;
        }
        */
    }

    ON_DEBUG(
        printf("Powers:\n");
        for (int i = 0; i < nElems; ++i) {
            printf("%llu ", nElemsPowers[i]);
        }
        LOGS("\nThat's it\n")
    )

    PolyHash hashFunc;
    
    forwardHashes = hashFunc(data, forPowers, nElems, mod, true);
    backwardHashes = hashFunc(data, forPowers, nElems, mod, false);

    LOGS("HashString created\n")
}

template <typename AbstractStringHash>
HashString<AbstractStringHash>::~HashString() {
    delete [] forwardHashes;
    delete [] backwardHashes;
    delete [] nElemsPowers;
}

template <typename AbstractStringHash>
bool HashString<AbstractStringHash>::equalStrings(
    const DirectedString& first, 
    const DirectedString& second
) {
    LOGS("Comparing (%d, %d) with (%d, %d) (their hashes are %llu and %llu)\n", 
        first.startingIndex, 
        first.endingIndex, 
        second.startingIndex, 
        second.endingIndex,
        stringHash(first),
        stringHash(second)
    )
    if (stringHash(first) == stringHash(second)) {
        LOGS("Equal by hash\n")
        return reallyEqual(first, second);
    }
    else {
        LOGS("NOT equal by hash\n")
        return false;
    }
}

template <typename AbstractStringHash>
bool HashString<AbstractStringHash>::reallyEqual(
    const DirectedString& first, 
    const DirectedString& second
) {
    if (first.len() != second.len()) {
        LOGS("Really (%d, %d) and (%d, %d) are not so equal because they have different length (%d vs. %d)\n", 
            first.startingIndex, 
            first.endingIndex, 
            second.startingIndex, 
            second.endingIndex,
            first.len(),
            second.len()
        )
        return false;
    }

    int len = std::min(first.len(), second.len());

    for (int i = 0; i < len; ++i) {
        if (data[first.by(i)] != data[second.by(i)]) {
            LOGS("Really (%d, %d) and (%d, %d) are not so equal by %d symbol\n", 
                first.startingIndex, 
                first.endingIndex, 
                second.startingIndex, 
                second.endingIndex, 
                i
            )
            return false;
        }
    }

    LOGS("(%d, %d) and (%d, %d) are really equal\n", 
        first.startingIndex, 
        first.endingIndex, 
        second.startingIndex, 
        second.endingIndex
    )
    return true;
}

template <typename AbstractStringHash>
unsigned long long HashString<AbstractStringHash>::stringHash(const DirectedString& str) {
    if (str.startingIndex < str.endingIndex) {
        LOGS("Hash of string (forward) (%d, %d) is:\nforwardHashes[%d] - forwardHashes[%d] * nElemsPowers[%d] = %llu\n", 
            str.startingIndex, 
            str.endingIndex, 
            str.endingIndex + 1, 
            str.startingIndex, 
            str.endingIndex - str.startingIndex + 1, 
            (
                (
                    mod + 
                    forwardHashes[str.endingIndex + 1]
                ) - 
                (
                    forwardHashes[str.startingIndex] *
                    nElemsPowers[str.endingIndex - str.startingIndex + 1]
                ) % 
                mod
            ) % 
            mod
        )

        return (
                    (
                        mod + 
                        forwardHashes[str.endingIndex + 1]
                    ) - 
                    (
                        forwardHashes[str.startingIndex] *
                        nElemsPowers[str.endingIndex - str.startingIndex + 1]
                    ) % 
                    mod
                ) % 
                mod;
    }
    else if (str.startingIndex == str.endingIndex) {
        LOGS("Hash of string (%d, %d) is %d\n", str.startingIndex, str.endingIndex, 0)
        return 0;
    }
    else {
        LOGS("Hash of string (backward) (%d, %d) is:\nbackwardHashes[%d] - backwardHashes[%d] * nElemsPowers[%d] = %llu\n", 
            str.startingIndex, 
            str.endingIndex, 
            str.endingIndex, 
            str.startingIndex + 1,
            str.startingIndex - str.endingIndex + 1,
            (
                (
                    mod + 
                    backwardHashes[str.endingIndex]
                ) - 
                (
                    backwardHashes[str.startingIndex + 1] * 
                    nElemsPowers[str.startingIndex - str.endingIndex + 1]
                ) % 
                mod
            ) % 
            mod
        )

        return (
                    (
                        mod + 
                        backwardHashes[str.endingIndex]
                    ) - 
                    (
                        backwardHashes[str.startingIndex + 1] * 
                        nElemsPowers[str.startingIndex - str.endingIndex + 1]
                    ) % 
                    mod
                ) % 
                mod;
    }
}
