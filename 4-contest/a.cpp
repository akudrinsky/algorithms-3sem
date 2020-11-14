#include <cstdio>
#include <cstring>
#include <vector>

#ifdef DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#define LOGS(...)                                                          \
    printf(__VA_ARGS__);                                                   \
    printf("\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, \
           __func__);
#else
#define LOGS(data, ...) ;
#define ON_DEBUG(...) ;
#endif

class SuffixArray
{
    SuffixArray(const char* data);

    ~SuffixArray();

    std::vector<int> GetSuffixArray();

private:
    static const int alphabetSize = 256;
    int* sortedSuffIndexes;
    int* classIndexes;
    int nClasses;
    int len;

    void zeroPhaze();

    void phaze(int phazeID);

    countClasses
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


int main()
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

SuffixArray::SuffixArray(const char* data)
    :
    nClasses(0)
{
    len = strlen(data) + 1;
    sortedSuffIndexes = new int[len];
    classIndexes = new int[len];

    zeroPhaze();

    for (int phazeID = 0; (1 << phazeID) < len; ++phazeID)
    {
        phaze(phazeID);
    }
}

void SuffixArray::zeroPhaze()
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
    for (int i = 0; i < len; ++i)
    {
        --counter[data[i]];
        sortedSuffIndexes[counter[data[i]]] = i;
    }

    classIndexes[sortedSuffIndexes[0]] = nClasses;
    nClasses = 1;
    for (int i = 0; i < len; ++i)
    {
        classIndexes[sortedSuffIndexes[i]] = nClasses;
        if (data[sortedSuffIndexes[i]] != data[sortedSuffIndexes[i - 1]])
        {
            ++nClasses;
        }
    }
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
            sortedSuffIndexesNew[i] += phazeLen;
        }
    }

    // stable sort by first half
    int counter[alphabetSize] = {};

    for (int i = 0; i < len; ++i)
    {
        ++counter[sortedSuffIndexesNew[i]];
    }

    for (int i = 1; i < nClasses; ++i)
    {
        counter[i] += counter[i - 1];
    }

    for (int i = len - 1; i >= 0; --i)
    {
        --counter[classIndexes[sortedSuffIndexesNew[i]]];
        sortedSuffIndexes[counter[classIndexes[sortedSuffIndexesNew[i]]]] = i;
    }

    classIndexesNew[sortedSuffIndexes[0]] = 0;
    nClasses = 1;
    for (int i = 1; i < len; ++i)
    {
        int firstHalfStarter = (sortedSuffIndexes[i] + phazeLen) & len;
        int secondHalfStarter = 
    }

    delete [] sortedSuffIndexes;
    delete [] classIndexes;

    classIndexes = classIndexesNew;
}

SuffixArray::~SuffixArray()
{
    delete [] sortedSuffIndexes;
}