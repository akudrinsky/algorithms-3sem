/*
Дана строка. Найдите для каждого её префикса количество различных подстрок в нём.

Входные данные
В единственной строке входных данных содержится непустая строка 𝑆, состоящая из 𝑁 (1≤𝑁≤2⋅105) маленьких букв английского алфавита.

Выходные данные
Выведите 𝑁 строк, в 𝑖-й строке должно содержаться количество различных подстрок в 𝑖-м префиксе строки 𝑆.
*/
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <iostream>

/*--------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*/

class SuffixTree
{
public:
    SuffixTree();

    SuffixTree(const std::string& data);

    ~SuffixTree() = default;

    void Extend(const std::string& additionalData);

    bool IsInside(const std::string& pattern);

    void photo(const std::string filename) const;

private:
    typedef long long nodeID;
    typedef long long strIdx;
    struct Node;

    // all input data
    std::string data;

    // list of all nodes. Tree is a structure of indexes of nodes in this vector
    std::vector<Node> nodes;

    // current node we are working on
    nodeID curNode;

    // number of iterations we did nothing
    long long symToWorkOn;

    // number of substrings
    long long nSubstr;
    long long nLeaves;

    enum 
    {
        rootID = 0,
        infinity = INT32_MAX,
    };

    nodeID NewNode(strIdx position, long long len);

    struct Node
    {
        // Including
        strIdx startIndex;
        // Not including
        long long len;
        // Link to nearest suffix.
        nodeID suffLink;
        // its children
        std::map<char, nodeID> children;

        nodeID safeGo(char symbol);

        Node(strIdx left = 0, long long len = 0);
    };

    void extendByChar(strIdx ind);

    void goByEdges(strIdx pos);
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
    SuffixTree tree;
    std::string query;
    std::cin >> query;
    
    tree.Extend(query);

    ON_DEBUG(
        tree.photo("ex.png");
    )
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

SuffixTree::Node::Node(SuffixTree::strIdx left, long long len)
    :
    startIndex(left),
    len(len),
    suffLink(rootID)
{}

/*--------------------------------------------------------------------------*/

SuffixTree::nodeID SuffixTree::Node::safeGo(char symbol)
{
    auto nd = children.find(symbol);
    return nd == children.end() ? rootID : nd->second;
}

/*--------------------------------------------------------------------------*/

SuffixTree::nodeID SuffixTree::NewNode(SuffixTree::strIdx position, long long len)
{
    nodes.emplace_back(position, len);
    return nodes.size() - 1;
}

/*--------------------------------------------------------------------------*/

void SuffixTree::goByEdges(SuffixTree::strIdx pos)
{
    LOGS("%lld", pos - symToWorkOn)
    while (symToWorkOn > nodes[nodes[curNode].children[data[pos - symToWorkOn]]].len)
    {
        curNode = nodes[curNode].safeGo(data[pos - symToWorkOn]);
        symToWorkOn -= nodes[curNode].len;
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::extendByChar(SuffixTree::strIdx ind)
{
    LOGS("Extending by %lld index", ind)
    // Adding new suffix of data of given stage
    ++symToWorkOn;
    nodeID last = rootID;

    while (symToWorkOn > 0)
    {
        LOGS("curNode = %lld\nsymToWorkOn = %lld", curNode, symToWorkOn)
        goByEdges(ind + 1);
        char edgeChar = data[ind + 1 - symToWorkOn];
        nodeID& destination = nodes[curNode].children[edgeChar];
        char temp = data[nodes[destination].startIndex + symToWorkOn - 1];

        if (destination == rootID)
        {
            destination = NewNode(ind + 1 - symToWorkOn, infinity);
            ++nLeaves;
            nodes[last].suffLink = curNode;
            last = rootID;
        }
        else if (temp == data[ind])
        {
            nodes[last].suffLink = curNode;
            nSubstr += nLeaves;
            return;
        }
        else
        {
            nodeID nd = NewNode(nodes[destination].startIndex, symToWorkOn - 1);
            nSubstr += symToWorkOn - 1;
            nodes[nd].children[data[ind]] = NewNode(ind, infinity);
            ++nLeaves;
            nodes[nd].children[temp] = destination;
            nodes[destination].startIndex += symToWorkOn - 1;
            nodes[destination].len -= symToWorkOn - 1;
            nSubstr -= symToWorkOn - 1;
            destination = nd;

            nodes[last].suffLink = nd;
            last = nd;
        }

        if (curNode == rootID)
            --symToWorkOn;
        else 
            curNode = nodes[curNode].suffLink;
    }
    nSubstr += nLeaves;
}

/*--------------------------------------------------------------------------*/

void SuffixTree::Extend(const std::string &additionalData)
{
    long long previousSize = data.size();
    data += additionalData;
    for (strIdx i = previousSize; i < data.size(); ++i)
    {
        extendByChar(i);
        std::cout << nSubstr << '\n';
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::photo(std::string filename) const
{
    FILE* pFile = fopen ("tree_graph.dot", "w");
    fprintf (pFile, "digraph G{\n\tedge[fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"circle\",fontsize=15, dpi=3000];\n");

    fprintf(pFile, "\tdescr [label=\"symToWorkOn=%lld\", shape=\"rectangle\"\n]", symToWorkOn);
    fprintf(pFile, "\t%d [label=\"root\"]\n", rootID);
    for (long long i = 1; i < nodes.size(); ++i) 
    {
        fprintf(pFile, "\t%lld [label=\"%lld: %s\"]\n", i, i, data.substr(nodes[i].startIndex, nodes[i].len).c_str());
    }

    for (long long idx = 0; idx < nodes.size(); ++idx) 
    {
        for (auto edge : nodes[idx].children)
        {
            fprintf(pFile, "\t%lld -> %lld\n", idx, edge.second);
        }
        fprintf(pFile, "\t%lld -> %lld [style=dashed color=dimgrey constraint=false]\n", idx, nodes[idx].suffLink);
    }

    fputc('}', pFile);
    fclose(pFile);

    std::string command = "dot tree_graph.dot -T png -o " + filename;
    system (command.c_str());

    system("rm tree_graph.dot");
}

/*--------------------------------------------------------------------------*/

SuffixTree::SuffixTree()
    :
    symToWorkOn(0),
    curNode(rootID),
    nSubstr(0),
    nLeaves(0)
{
    nodes.emplace_back(0, infinity);
}

/*--------------------------------------------------------------------------*/

SuffixTree::SuffixTree(const std::string& data)
    :
    SuffixTree()
{
    Extend(data);
}

/*--------------------------------------------------------------------------*/

bool SuffixTree::IsInside(const std::string &pattern)
{
    auto child = nodes[rootID].children.find(pattern[0]);
    if (child == nodes[rootID].children.end())
        return false;
    nodeID ndID = child->second;
    for (long long idx = 0; idx < pattern.size(); /* */)
    {
        LOGS("Searching for letter %lld in %lld node", idx, ndID)
        for (long long letterID = 0; letterID < nodes[ndID].len and idx < pattern.size(); ++letterID)
        {
            if (pattern[idx] == data[nodes[ndID].startIndex + letterID])
                ++idx;
            else
                return false;
        }
        if (idx == pattern.size())
            break;

        auto child = nodes[ndID].children.find(pattern[idx]);
        if (child == nodes[ndID].children.end())
            return false;
        ndID = child->second;
    }
    return true;
}

/*--------------------------------------------------------------------------*/