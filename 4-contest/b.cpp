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

int main()
{
    unsigned long long needed = 0;
    char* input = new char[100000];
    scanf("%s", input);
    scanf("%llu", &needed);

    SuffixArray solver(input);

    auto lcp = solver.GetLCP();
    auto suffArray = solver.GetSuffixArray();

    unsigned long long currentStr = 0;
    int initialStringLen = suffArray.len - 1;
    for (int i = 1; i < suffArray.len; ++i)
    {
        LOGS("%d %d %d\n", suffArray.len, suffArray.data[i], lcp.data[i - 1])
        currentStr += (initialStringLen - suffArray.data[i]) - lcp.data[i - 1];
        LOGS("currentStr = %llu\nneeded = %llu", currentStr, needed)
        if (currentStr >= needed)
        {
            *((input + suffArray.len - 1) - (currentStr - needed)) = '\0';
            printf("%s", input + suffArray.data[i]);
            break;
        }
    }

    if (currentStr < needed)
    {
        printf("%s", input + suffArray.data[suffArray.len - 1]);
    }
        
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