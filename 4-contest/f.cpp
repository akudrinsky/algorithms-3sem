/*
Сегодня на уроке преподаватель Массивов Автомат Укконевич рассказывал своим ученикам про строки, суффиксные структуры и всё такое. Например, он рассказал им, как сравнить две строки 𝐴 и 𝐵 лексикографически. Если одна из них является префиксом другой, то более короткая будет лексикографически меньше, иначе необходимо сравнить символы стоящие на первой позиции, в которой они отличаются. Строка с меньшим по номеру в алфавите символом на данной позиции и будет лексикографически меньше.

Чтобы проверить понимание учениками нового материала, Автомат Укконевич дал им следующее задание: найти 𝑘-ю лексикографически непустую уникальную подстроку строки 𝑆.

Так как учитель знает, что Александр Г. и Илья С. очень любят списывать у известного в узких кругах Демида Г., каждый школьник получил своё число 𝑘 и вынужден был обратиться к вам за помощью.

Входные данные
В первой строке входного файла находится строка 𝑆 (|𝑆|≤105). Вторая строка содержит число 𝑘 (1≤𝑘≤1018) — порядковый номер запрашиваемой подстроки.

Выходные данные
Если ответ существует, выведите искомую подстроку строки 𝑆. В противном случае выведите её лексикографически максимальную подстроку.
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <string>
#include <iostream>

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

#define INTERNAL(line) //fprintf(stderr, "ITER: %d on line %d\n", i, line);\

/*
        for (int j = 0; j < allData.size(); ++j)\
        {\
            fprintf(stderr, "stack[%d] = %d\n", j, stack[j]);\
        }\
        for (int j = 0; j < nStrings; ++j)\
        {\
            fprintf(stderr, "answer[%d] = %d\n", j, answer[j]);\
        }\
        for (int j = 0; j < allData.size(); ++j)\
        {\
            fprintf(stderr, "left[%d] = %d\n", j, leftBorders[j]);\
        }\
*/

class SuffixArray
{
public:
    struct LightArray
    {
        int* data;
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

class FenwickForSum 
{
public:
    std::vector<int> data;
    std::vector<int> fenwick;

    FenwickForSum (int maxLen);

    void Add(int x, int val, int total_length);

    int Sum(int x);

    int Query(int left, int right);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    unsigned long long nStrings = 0;
    std::string allData;

    std::cin >> nStrings;
    std::vector<int> dollars(nStrings, 0);
    //int totalLen = 0;

    int* stringIDs = new int[400000];

    for (int i = 0; i < nStrings; ++i)
    {
        std::string mini;
        std::cin >> mini;
        for (int j = 0; j < mini.size(); ++j)
        {
            stringIDs[allData.size() + j] = i;
        }
        stringIDs[allData.size() + mini.size() + 1] = 0;
        allData += mini;
        dollars[i] = allData.size();
        allData += "$";
    }

    /*
    fprintf(stderr, "stringIDs[i]\n");
    for (int i = 0; i < allData.size(); ++i)
    {
        fprintf(stderr, "stringIDs[%d] = %d\n", i, stringIDs[i]);
    }
    */

    SuffixArray solver(allData.c_str());

    auto lcp = solver.GetLCP();
    auto suffArray = solver.GetSuffixArray();

    for (int i = 0; i < lcp.len; ++i)
    {
        if (allData[suffArray.data[i]] == '$' || lcp.data[i] == -1 || allData[suffArray.data[i]] == '\0')
        {
            lcp.data[i] = 0;
            continue;
        }
        int suffID = stringIDs[suffArray.data[i]];
        //int nextSuffId = stringIDs[suffArray.data[i + 1]];
        //printf("%d (%d) - %d\n", dollars[suffID], suffID, suffArray.data[i]);
        lcp.data[i] = std::min(lcp.data[i], dollars[suffID] - suffArray.data[i]);
        //lcp.data[i] = std::min(lcp.data[i], dollars[nextSuffId + 1] - suffArray.data[i]);
    }

    int first = suffArray.data[0];
    for (int i = 0; i < suffArray.len - 1; ++i)
    {
        suffArray.data[i] = suffArray.data[i + 1];
    }
    suffArray.data[suffArray.len - 1] = 0;

    /*
    printf("sortedSuffIndexes\n");
    for (int i = 0; i < lcp.len; ++i)
    {
        printf("%3d) [id %3d, %3d common] %s\n", i, suffArray.data[i], lcp.data[i], allData.c_str() + suffArray.data[i]);
    }
    printf("\n\n");
    */

    int* answer = new int[400000];
    int* stack = new int[400000];
    int* leftBorders = new int[400000];

    for (int i = 0; i < allData.size(); ++i)
    {
        answer[i] = 0;
        stack[i] = 0;
        leftBorders[i] = 0;
    }

    FenwickForSum fnwk(400000);
    int stackPtr = 1;
    for (int i = 1; i <= allData.size(); ++i)
    {
        //if (i <= 14) fprintf(stderr, "Iter %d\n", i);
        //if (i <= 14) {INTERNAL(__LINE__)}
        while (lcp.data[i] < lcp.data[stack[stackPtr - 1]])
        {
            //fprintf(stderr, "left[stack[ss - 1]] = %d, stack[ss - 1] = %d, ss = %d\n", leftBorders[stack[stackPtr - 1]], stack[stackPtr - 1], stackPtr);

            int nDistinct = fnwk.Query(leftBorders[stack[stackPtr - 1]], i - 1);

            //fprintf(stderr, "nDistinct = %d\n", nDistinct);

            if (lcp.data[stack[stackPtr - 1]] > answer[nDistinct])
            {
                answer[nDistinct] = lcp.data[stack[stackPtr - 1]];
            }
            --stackPtr;
        }
        if (i <= 14) {INTERNAL(__LINE__)}

        if (lcp.data[stack[stackPtr - 1]] == lcp.data[i])
        {
            leftBorders[i] = leftBorders[stack[stackPtr - 1]];
        }
        else 
        {
            leftBorders[i] = stack[stackPtr - 1];
        }

        stack[stackPtr++] = i;
        if (i <= 14) {INTERNAL(__LINE__)}
        if (fnwk.data[stringIDs[suffArray.data[i]]])
        {
            fnwk.Add(fnwk.data[stringIDs[suffArray.data[i]]], -1, allData.size());
        }

        if (i <= 14) {INTERNAL(__LINE__)}
        fnwk.Add(i, 1, allData.size());
        fnwk.data[stringIDs[suffArray.data[i]]] = i;
    }

    for (int i = nStrings - 1; i >= 2; i--) {
        if (answer[i + 1] > answer[i]) {
            answer[i] = answer[i + 1];
        }
    }

    for (int i = 2; i <= nStrings; ++i)
    {
        std::cout << answer[i] << '\n';
    }

    delete [] answer;
    delete [] stack;
    delete [] leftBorders;
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
    return {sortedSuffIndexes, len};
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
        printf("%3d) [%3d] %s\n", i, sortedSuffIndexes[i], data + sortedSuffIndexes[i]);
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

    /*
    ON_DEBUG(
        printf("sortedSuffIndexes\n");
        for (int i = 0; i < len; ++i)
        {
            printf("%3d) [id %3d, %3d common] %s\n", i, sortedSuffIndexes[i], lcp[i], data + sortedSuffIndexes[i]);
        }
        printf("\n\n");
    )
    */

    return {lcp, len};
}

FenwickForSum::FenwickForSum (int maxLen):
    data (maxLen, 0),
    fenwick (maxLen, 0)
{}

void FenwickForSum::Add(int x, int val, int total_length) 
{
    /*
    for (int i = 0; i < total_length; ++i)
    {
        fprintf(stderr, "%d %d\n", fenwick[i], data[i]);
    }
    */
    do {
        fenwick[x] += val;
        x += x & -x;
    } while (x <= total_length);
}

int FenwickForSum::Sum(int x) 
{
    //LOGS("SUM request")
    int strings = 0;
    while (x) {
        //LOGS(" x = %d", x)
        strings += fenwick[x];
        x &= x - 1;
    }
    return strings;
}

int FenwickForSum::Query(int left, int right) 
{
    //fprintf(stderr, "Query(%d, %d)\n", left, right);
    return Sum(right) - Sum(left - 1);
}