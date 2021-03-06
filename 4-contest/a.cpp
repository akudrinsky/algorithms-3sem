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
#include <stack>

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

class SuffixArray
{
public:
    struct LightArray
    {
        const int* data;
        int len;
        ~LightArray();
    };

    SuffixArray(const char* data);

    ~SuffixArray();

    LightArray GetSuffixArray();

    LightArray GetLCP();

private:
    static const int alphabetSize = 256;
    const char* data;
    int* sortedSuffIndexes;
    int* classIndexes;
    int nClasses;
    int len;

    void zeroPhaze(const char* data);

    void printInternal(int lineNum);

    void phaze(int phazeID);
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
        int start;
        int nOccurences;
        int len;
    };
}


int main()
{
    int nLetters = 0, nCharacters = 0;
    char* input = new char[400000];
    scanf("%d %d", &nLetters, &nCharacters);
    scanf("%s", input);

    SuffixArray solver(input);

    auto lcp = solver.GetLCP();
    auto suffArray = solver.GetSuffixArray();

    std::stack<Substring> Stack;

    Substring best = {
        .len = lcp.len,
        .nOccurences = 1,
        .start = 0
    };

    for (int i = 0; i < nLetters; ++i) {
        int currentOccurencesNum = 1;
        while (!Stack.empty() and lcp.data[i] <= Stack.top().len) {
            Substring topSubstring = Stack.top();
            Stack.pop();
            LOGS(
                "Substring{\n"
                "    .len = %d, \n"
                "    .start = %d, \n"
                "    .nOccurences = %d\n"
                "} was popped from stack at iter %d",
                topSubstring.len,
                topSubstring.start, 
                topSubstring.nOccurences,
                i
            )
            currentOccurencesNum += topSubstring.nOccurences;
            if (currentOccurencesNum * topSubstring.len > best.nOccurences * best.len) {
                best.nOccurences = currentOccurencesNum;
                best.len = topSubstring.len;
                best.start = suffArray.data[topSubstring.start];
            }		
        }
        if (Stack.empty() or lcp.data[i] > Stack.top().len) {
            Stack.push(
                Substring{
                    .len = lcp.data[i], 
                    .start = i, 
                    .nOccurences = currentOccurencesNum
                }
            );
            LOGS(
                "Substring{\n"
                "    .len = %d, \n"
                "    .start = %d, \n"
                "    .nOccurences = %d\n"
                "} was pushed to stack",
                lcp.data[i],
                i, 
                currentOccurencesNum
            )
        }
    }

    printf(
        "ANSWER:\n%d\n%d\n%d\n", 
        best.len * best.nOccurences,
        best.len, 
        best.start
    );
        
    delete [] input;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

SuffixArray::SuffixArray(const char* data)
    :
    nClasses(0),
    data(data)
{
    len = strlen(data) + 1;
    sortedSuffIndexes = new int[len];
    classIndexes = new int[len];

    zeroPhaze(data);

    for (int phazeID = 0; (1 << phazeID) < len; ++phazeID)
    {
        phaze(phazeID);
    }
    //ON_DEBUG(printInternal(__LINE__);)
}

void SuffixArray::zeroPhaze(const char* data)
{
    int counter[alphabetSize] = {};

    // count elements by value
    for (int i = 0; i < len; ++i)
    {
        ++counter[data[i]];
    }

    // change to indexes range
    for (int i = 1; i < alphabetSize; ++i)
    {    
        counter[i] += counter[i - 1];
    }

    // get permutation 
    // <<sortedSuffIndexes[i] = j>> == <<i'th elem in data is j'th in sorted>>
    for (int i = len - 1; i >= 0; --i)
    {
        --counter[data[i]];
        sortedSuffIndexes[counter[data[i]]] = i;
    }

    classIndexes[sortedSuffIndexes[0]] = nClasses;
    nClasses = 1;
    for (int i = 1; i < len; ++i)
    {
        if (data[sortedSuffIndexes[i]] != data[sortedSuffIndexes[i - 1]])
        {
            ++nClasses;
        }
        classIndexes[sortedSuffIndexes[i]] = nClasses - 1;
    }
}

SuffixArray::LightArray::~LightArray()
{
    //delete [] data;
    len = -1;
}

void SuffixArray::phaze(int phazeID)
{
    const int phazeLen = 1 << phazeID;
    int* sortedSuffIndexesNew = new int[len];
    int* classIndexesNew = new int[len];

    // sort by second half of string
    for (int i = 0; i < len; ++i)
    {
        sortedSuffIndexesNew[i] = sortedSuffIndexes[i] - phazeLen;
        if (sortedSuffIndexesNew[i] < 0)
        {
            sortedSuffIndexesNew[i] += len;
        }
    }

    // stable sort by first half
    int* counter = new int[len];
    memset(counter, 0, sizeof(int) * len);

    for (int i = 0; i < len; ++i)
    {
        ++counter[classIndexes[sortedSuffIndexesNew[i]]];
    }

    for (int i = 1; i < nClasses; ++i)
    {
        counter[i] += counter[i - 1];
    }

    for (int i = len - 1; i >= 0; --i)
    {
        --counter[classIndexes[sortedSuffIndexesNew[i]]];
        sortedSuffIndexes[counter[classIndexes[sortedSuffIndexesNew[i]]]] = sortedSuffIndexesNew[i];
    }

    classIndexesNew[sortedSuffIndexes[0]] = 0;
    nClasses = 1;
    for (int i = 1; i < len; ++i)
    {
        int firstStarter = (sortedSuffIndexes[i] + phazeLen) % len;
        int secondStarter = (sortedSuffIndexes[i - 1] + phazeLen) % len;
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

SuffixArray::LightArray SuffixArray::GetSuffixArray()
{
    return {sortedSuffIndexes + 1, len - 1};
}

SuffixArray::~SuffixArray()
{
    delete [] sortedSuffIndexes;
}

void SuffixArray::printInternal(int lineNum)
{
    printf("sortedSuffIndexes at %d:\n", lineNum);
    for (int i = 0; i < len; ++i)
    {
        printf("%3d) [%3d] %s$\n", i, sortedSuffIndexes[i], data + sortedSuffIndexes[i]);
    }
    printf("\n\n");
}

SuffixArray::LightArray SuffixArray::GetLCP()
{
    int* lcp = new int[len];
    int* reverseSuff = new int[len];

    for (int i = 0; i < len; ++i)
    {
        reverseSuff[sortedSuffIndexes[i]] = i;
    }

    int k = 0;
    for (int i = 0; i < len; ++i)
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
            int j = sortedSuffIndexes[reverseSuff[i] + 1];
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
        for (int i = 0; i < len; ++i)
        {
            printf("%3d) [id %3d, %3d common] %s$\n", i, sortedSuffIndexes[i], lcp[i], data + sortedSuffIndexes[i]);
        }
        printf("\n\n");
    )

    return {lcp, len};
}