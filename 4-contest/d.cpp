/*
Сейчас Эркюль Пуаро занят разоблачением международного преступного синдиката, занимающегося контрабандой предметов искусства. Полиция, сотрудничающая с Пуаро, перехватила зашифрованное письмо, содержащее информацию о месте и времени предстоящей сделки, на которой будет присутствовать и глава синдиката. Чтобы сорвать сделку и задержать главу синдиката, необходимо расшифровать перехваченное письмо.

Эркюль знает, что ключ к шифру вычисляется из строки 𝑠. Обозначим за 𝑓(𝑤) длину максимального суффикса 𝑤, не равного 𝑤, который является и префиксом 𝑤. Например, 𝑓(𝚊𝚋𝚌)=0, 𝑓(𝚊𝚋𝚊𝚋)=2, 𝑓(𝚊𝚊𝚊)=2. Тогда ключом является максимум по всем 𝑡, являющимся подстроками 𝑠, величины (|𝑡|+𝑓(𝑡)2). Помогите Эркюлю вычислить ключ.

Входные данные
В единственной строке дана строка 𝑠, состоящая из строчный латинских букв (1≤|𝑠|≤500000).

Выходные данные
Выведите единственное целое число — искомый ключ к шифру.
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <algorithm>
#include <math.h>

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

template<typename compare>
class sparse_table {
private:
    std::vector<int> initial_values;
    int* *table;
    int* two_powers;
    compare cmp;
    
    inline int max_of (int el1, int pow1, int el2, int pow2) const {
        return 
            (cmp(initial_values[table[pow1][el1]], initial_values[table[pow2][el2]]))   
            ?
            table[pow1][el1] 
            :
            table[pow2][el2];
    }
    
    int height () const {
        if (initial_values.size () > 1)
            return two_powers[initial_values.size () - 1];
        else
            return 1;
    }
    
public:
    sparse_table () = delete;
    
    sparse_table (std::vector<int>& on_which) :  initial_values(on_which) {
        long long n = on_which.size ();
        two_powers = new int[n];
        two_powers[0] = 0;
        two_powers[1] = 1;
        
        for (int i = 2; i < n; ++i) {
            two_powers[i] = two_powers[i / 2] + 1;
            //std::cout << "power " << two_powers[i] << '\n';
        }
        
        int logn = height ();
        
        table = new int*[logn];
        
        for (int i = 0; i < logn; ++i) {
            table[i] = new int[n];
        }
        
        for (int i = 0; i < n; ++i) {
            table[0][i] = i;
        }
        
        for (int i = 1; i < logn; ++i) {
            for (int j = 0; j < n; ++j) {
                if (j + pow (2, i) <= n)
                    table[i][j] = max_of (j, i - 1, j + pow (2, i - 1), i - 1);
                else {
                    table[i][j] = -1;
                }
            }
        }
    }
    
    ~sparse_table () {
        long long n = initial_values.size ();
        
        for (int i = 0; i < two_powers[n - 1]; ++i) {
            delete [] table[i];
        }
        
        delete [] table;
        
        delete [] two_powers;
    }
    
    int get_max (int left, int right) const {
        if (left == right)
            return left;

        --left;
        --right;
        
        int power = two_powers[right - left] - 1;
        //std::cout << "left: " << left << " right: " << right << " 2^: " << power << '\n';
        
        return max_of (left, power, right - pow (2, power) + 1, power) + 1;
    }
};

struct mincmp
{
    bool operator()(int a, int b) const
    {
        return a < b;
    }
};

struct maxcmp
{
    bool operator()(int a, int b) const
    {
        return a > b;
    }
};

struct suffixPair
{
    int firstSufInSorted;
    int lcpWithNext;
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

    SuffixArray solver(input);

    auto lcp = solver.GetLCP();
    auto suffArray = solver.GetSuffixArray();

    std::vector<suffixPair> suffixes;
    for (int i = 0; i < suffArray.len; ++i)
    {
        suffixes.push_back(suffixPair{.firstSufInSorted = i, .lcpWithNext = lcp.data[i]});
    }
    std::sort(
        suffixes.begin(), 
        suffixes.end(), 
        [](const suffixPair &first, const suffixPair &second) -> bool
        {
            return first.lcpWithNext < second.lcpWithNext;
        });

    ON_DEBUG(
        printf("sorted by lcp\n");
        for (int i = 0; i < suffixes.size(); ++i)
        {
            printf("%d) lcpWithNext = %3d, firstSufInSorted = %3d\n", i, suffixes[i].lcpWithNext, suffixes[i].firstSufInSorted);
        }
        printf("\n");
    )

    std::vector<int> _lcpVec(lcp.len);
    for (int i = 0; i < lcp.len; ++i)
        _lcpVec[i] = lcp.data[i];

    sparse_table<maxcmp> tableMAX(_lcpVec);
    sparse_table<mincmp> tableMIN(_lcpVec);

    printf("max: %d, min: %d\n", tableMAX.get_max(0, 2), tableMIN.get_max(0, 2));

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
        printf("sortedSuffIndexes of string \"%s$\"\n", data);
        for (int i = 0; i < len; ++i)
        {
            printf("%3d) [id %3d, %3d common] %s$\n", i, sortedSuffIndexes[i], lcp[i], data + sortedSuffIndexes[i]);
        }
        printf("\n\n");
    )

    return {lcp, len};
}