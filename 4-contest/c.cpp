/*
Постройте суффиксный массив для заданной строки 𝑠.

Входные данные
Первая строка входного файла содержит строку 𝑠 (1≤|𝑠|≤400000). Строка состоит из строчных латинских букв.

Выходные данные
Выведите |𝑠| различных чисел — номера первых символов суффиксов строки 𝑠 так, чтобы соответствующие суффиксы были упорядочены в лексикографически возрастающем порядке.
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

#ifdef DEBUG
    #define LOGS(...)\
        fprintf(stderr, "-----LOG-----\n~~~~~~~~~~~~~\n");\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n~~~~~~~~~~~~~\nFILE: %s\nLINE: %d\nFUNC: %s", __FILE__, __LINE__, __func__);\
        fprintf(stderr, "\n---LOG-END---\n\n\n\n");
    #define ON_DEBUG(...)\
        __VA_ARGS__
#else
    #define LOGS(...) ;
    #define ON_DEBUG(...)\
        ;
#endif

template<typename T>
class SuffixArray
{
public:
    struct LightArray
    {
        const long long* data;
        long long len;
        ~LightArray();
    };

    SuffixArray(const T* data, long long dataLen, long long alphabetSize);

    ~SuffixArray();

    LightArray GetSuffixArray();

    LightArray GetLCP();

private:
    const long long alphabetSize;
    const T* data;
    long long* sortedSuffIndexes;
    long long* classIndexes;
    long long nClasses;
    long long len;

    void zeroPhaze();

    void printInternal(long long lineNum);

    void phaze(long long phazeID);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

namespace
{
    struct Substring
    {
        long long start;
        long long end;
        long long nOccurences;
    };
}

class compareInts
{
public:
    compareInts(const long long* array);

    bool operator()(long long first, long long second) const;

private:
    const long long* array;
};

int main()
{
    long long nLetters = 0, nCharacters = 0;
    scanf("%lld %lld", &nLetters, &nCharacters);
    long long* input = new long long[nLetters + 1];
    for (long long i = 0; i < nLetters; ++i)
    {
        scanf("%lld", input + i);
    }
    input[nLetters] = 0;

    SuffixArray<long long> solver(input, nLetters, nCharacters + 1);
    LOGS("Init")

    auto lcp = solver.GetLCP();
    auto suffArray = solver.GetSuffixArray();

    LOGS("MAIN")

    std::vector<long long> indexPermutation(lcp.len);
    for (long long i = 0; i < lcp.len; ++i)
    {
        indexPermutation[i] = i;
    }

    std::sort(indexPermutation.begin(), indexPermutation.end(), compareInts(lcp.data));

    std::set<long long> indexes;
    indexes.emplace(indexPermutation[0]);

    Substring best = {
        .start = 0,
        .end = nLetters, // not including
        .nOccurences = 1
    };

    for (long long i = 1; i < lcp.len; ++i)
    {
        indexes.emplace(indexPermutation[i]);
        auto prev = indexes.lower_bound(indexPermutation[i]);
        if (prev != indexes.begin())
            --prev;
        
        auto next = indexes.upper_bound(indexPermutation[i]);
        LOGS(
            "Found %d strings from %d to %d index", 
            *next - *prev, 
            suffArray.data[indexPermutation[i]], 
            suffArray.data[indexPermutation[i]] + lcp.data[indexPermutation[i]]
        )
    
        if ((best.end - best.start) * best.nOccurences < 
            lcp.data[indexPermutation[i]] * (*next - *prev))
        {
            LOGS("Updated previous best")
            best.start = suffArray.data[indexPermutation[i]];
            best.end = suffArray.data[indexPermutation[i]] + 
                lcp.data[indexPermutation[i]];
            best.nOccurences = *next - *prev;
        }
    }

    printf("%lld\n%lld\n", best.nOccurences * (best.end - best.start), best.end - best.start);
    for (long long i = 0; i < best.end - best.start; ++i)
    {
        printf("%lld ", input[best.start + i]);
    }
    printf("\n");
    delete [] input;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


compareInts::compareInts(const long long* array)
    :
    array(array)
{}

bool compareInts::operator()(long long first, long long second) const
{
    return array[first] < array[second];
}

template<typename T>
SuffixArray<T>::SuffixArray(const T* data, long long dataLen, long long alphabetSize)
    :
    nClasses(0),
    data(data),
    len(dataLen + 1),
    alphabetSize(alphabetSize)
{
    LOGS("len is %d, alph size %d", len, alphabetSize);
    sortedSuffIndexes = new long long[len];
    classIndexes = new long long[len];

    zeroPhaze();

    for (long long phazeID = 0; (1 << phazeID) < len; ++phazeID)
    {
        phaze(phazeID);
    }
    //ON_DEBUG(printInternal(__LINE__);)
}

template<typename T>
void SuffixArray<T>::zeroPhaze()
{
    long long* counter = new long long[alphabetSize];
    memset(counter, 0, sizeof(long long) * alphabetSize);

    // count elements by value
    for (long long i = 0; i < len; ++i)
    {
        ++counter[data[i]];
    }

    // change to indexes range
    for (long long i = 1; i < alphabetSize; ++i)
    {    
        counter[i] += counter[i - 1];
        //fprintf(stderr, "%d\n", counter[i]);
    }

    // get permutation 
    // <<sortedSuffIndexes[i] = j>> == <<i'th elem in data is j'th in sorted>>
    for (long long i = len - 1; i >= 0; --i)
    {
        --counter[data[i]];
        
        sortedSuffIndexes[counter[data[i]]] = i;
    }

    classIndexes[sortedSuffIndexes[0]] = nClasses;
    nClasses = 1;
    for (long long i = 1; i < len; ++i)
    {
        if (data[sortedSuffIndexes[i]] != data[sortedSuffIndexes[i - 1]])
        {
            ++nClasses;
        }
        classIndexes[sortedSuffIndexes[i]] = nClasses - 1;
    }

    delete [] counter;
}

template<typename T>
SuffixArray<T>::LightArray::~LightArray()
{
    //delete [] data;
    len = -1;
}

template<typename T>
void SuffixArray<T>::phaze(long long phazeID)
{
    LOGS("Phaze %d", phazeID)
    const long long phazeLen = 1 << phazeID;
    long long* sortedSuffIndexesNew = new long long[len];
    long long* classIndexesNew = new long long[len];

    // sort by second half of string
    for (long long i = 0; i < len; ++i)
    {
        sortedSuffIndexesNew[i] = sortedSuffIndexes[i] - phazeLen;
        if (sortedSuffIndexesNew[i] < 0)
        {
            sortedSuffIndexesNew[i] += len;
        }
    }

    // stable sort by first half
    long long* counter = new long long[len];
    memset(counter, 0, sizeof(long long) * len);

    for (long long i = 0; i < len; ++i)
    {
        ++counter[classIndexes[sortedSuffIndexesNew[i]]];
    }

    for (long long i = 1; i < nClasses; ++i)
    {
        counter[i] += counter[i - 1];
    }

    for (long long i = len - 1; i >= 0; --i)
    {
        --counter[classIndexes[sortedSuffIndexesNew[i]]];
        sortedSuffIndexes[counter[classIndexes[sortedSuffIndexesNew[i]]]] = sortedSuffIndexesNew[i];
    }

    classIndexesNew[sortedSuffIndexes[0]] = 0;
    nClasses = 1;
    for (long long i = 1; i < len; ++i)
    {
        long long firstStarter = (sortedSuffIndexes[i] + phazeLen) % len;
        long long secondStarter = (sortedSuffIndexes[i - 1] + phazeLen) % len;
        if (classIndexes[sortedSuffIndexes[i]] != classIndexes[sortedSuffIndexes[i - 1]] 
            or classIndexes[firstStarter] != classIndexes[secondStarter])
        {
            ++nClasses;
        }
        classIndexesNew[sortedSuffIndexes[i]] = nClasses - 1;
    }

    delete [] classIndexes;
    delete [] sortedSuffIndexesNew;
    delete [] counter;

    classIndexes = classIndexesNew;
}

template<typename T>
typename SuffixArray<T>::LightArray SuffixArray<T>::GetSuffixArray()
{
    return {sortedSuffIndexes + 1, len - 1};
}

template<typename T>
SuffixArray<T>::~SuffixArray()
{
    delete [] sortedSuffIndexes;
}

template<typename T>
void SuffixArray<T>::printInternal(long long lineNum)
{
    printf("sortedSuffIndexes at %d:\n", lineNum);
    for (long long i = 0; i < len; ++i)
    {
        printf("%3d) [%3d] %s$\n", i, sortedSuffIndexes[i], data + sortedSuffIndexes[i]);
    }
    printf("\n\n");
}

template<typename T>
typename SuffixArray<T>::LightArray SuffixArray<T>::GetLCP()
{
    long long* lcp = new long long[len];
    long long* reverseSuff = new long long[len];

    for (long long i = 0; i < len; ++i)
    {
        reverseSuff[sortedSuffIndexes[i]] = i;
    }

    long long k = 0;
    for (long long i = 0; i < len; ++i)
    {
        if (k > 0)
        {
            --k;
        }
        if (reverseSuff[i] == len - 1)
        {
            lcp[len - 1] = -1;
            k = 0;
            continue;
        }
        else
        {
            long long j = sortedSuffIndexes[reverseSuff[i] + 1];
            while (std::max(i + k, j + k) < len and data[i + k] == data[j + k])
            {
                ++k;
            }
            lcp[reverseSuff[i]] = k;
        }
    }

    delete [] reverseSuff;

    ON_DEBUG(
        printf("sortedSuffIndexes\n");
        for (long long i = 0; i < len; ++i)
        {
            printf("%3d) [id %3d, %3d common] ", i, sortedSuffIndexes[i], lcp[i]);
            for (long long j = sortedSuffIndexes[i]; j < len - 1; ++j)
                printf("%2d ", data[j]);
            printf(" $\n");
        }
        printf("\n\n");
    )

    return {lcp, len};
}