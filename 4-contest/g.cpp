/*
Очень известный автор не менее известной книги решил написать продолжение своего произведения. Он писал все свои книги на компьютере, подключенном к интернету. Из-за такой неосторожности мальчику Ненокку удалось получить доступ к еще ненаписанной книге. Каждый вечер мальчик залазил на компьютер писателя и записывал на свой компьютер новые записи. Ненокку, записав на свой компьютер очередную главу, заинтересовался, а использовал ли хоть раз писатель слово "книга". Но он не любит читать книги (он лучше полазает в интернете), и поэтому он просит вас узнать есть ли то или иное слово в тексте произведения. Но естественно его интересует не только одно слово, а достаточно много.

Входные данные
В каждой строчке входного файла записана одна из двух записей.

? <слово> (<слово> — это набор не более 50 латинских символов): запрос проверки существования подстроки <слово> в произведении;
A <текст> (<текст> — это набор не более 105 латинских символов): добавление в произведение <текст>.
Писатель только начал работать над произведением, поэтому он не мог написать более 105 символов. Суммарная длина всех запросов не превосходит 15 мегабайт плюс 12140 байт.

Выходные данные
Выведите на каждую строчку типа 1 "YES", если существует подстрока <слово>, и "NO" в противном случае. Не следует различать регистр букв.
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
    char type = '\0';
    std::string query;
    while (std::cin >> type >> query)
    {
        //std::cin >> type >> query;
        std::transform(
            query.begin(), 
            query.end(), 
            query.begin(),
            [](unsigned char c) {return std::tolower(c);});
        if (type == '?')
        {
            if (tree.IsInside(query))
                std::cout << "YES";
            else
                std::cout << "NO";
            std::cout << "\n";
        }
        else if (type == 'A')
            tree.Extend(query);
    }

    ON_DEBUG
    (
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
    LOGS("%d", pos - symToWorkOn)
    while (symToWorkOn > nodes[nodes[curNode].children[data[pos - symToWorkOn]]].len)
    {
        curNode = nodes[curNode].safeGo(data[pos - symToWorkOn]);
        symToWorkOn -= nodes[curNode].len;
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::extendByChar(SuffixTree::strIdx ind)
{
    LOGS("Extending by %d index", ind)
    // Adding new suffix of data of given stage
    ++symToWorkOn;
    nodeID last = rootID;

    while (symToWorkOn > 0)
    {
        LOGS("curNode = %d\nsymToWorkOn = %d", curNode, symToWorkOn)
        goByEdges(ind + 1);
        char edgeChar = data[ind + 1 - symToWorkOn];
        nodeID& destination = nodes[curNode].children[edgeChar];
        char temp = data[nodes[destination].startIndex + symToWorkOn - 1];

        if (destination == rootID)
        {
            destination = NewNode(ind + 1 - symToWorkOn, infinity);
            nodes[last].suffLink = curNode;
            last = rootID;
        }
        else if (temp == data[ind])
        {
            nodes[last].suffLink = curNode;
            return;
        }
        else
        {
            nodeID nd = NewNode(nodes[destination].startIndex, symToWorkOn - 1);
            nodes[nd].children[data[ind]] = NewNode(ind, infinity);
            nodes[nd].children[temp] = destination;
            nodes[destination].startIndex += symToWorkOn - 1;
            nodes[destination].len -= symToWorkOn - 1;
            destination = nd;

            nodes[last].suffLink = nd;
            last = nd;
        }

        if (curNode == rootID)
            --symToWorkOn;
        else 
            curNode = nodes[curNode].suffLink;
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::Extend(const std::string &additionalData)
{
    long long previousSize = data.size();
    data += additionalData;
    for (strIdx i = previousSize; i < data.size(); ++i)
    {
        extendByChar(i);
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::photo(std::string filename) const
{
    FILE* pFile = fopen ("tree_graph.dot", "w");
    fprintf (pFile, "digraph G{\n\tedge[fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"circle\",fontsize=15, dpi=3000];\n");

    fprintf(pFile, "\tdescr [label=\"symToWorkOn=%d\", shape=\"rectangle\"\n]", symToWorkOn);
    fprintf(pFile, "\t%d [label=\"root\"]\n", rootID);
    for (long long i = 1; i < nodes.size(); ++i) 
    {
        fprintf(pFile, "\t%d [label=\"%d: %s\"]\n", i, i, data.substr(nodes[i].startIndex, nodes[i].len).c_str());
    }

    for (long long idx = 0; idx < nodes.size(); ++idx) 
    {
        for (auto edge : nodes[idx].children)
        {
            fprintf(pFile, "\t%lu -> %lu\n", idx, edge.second);
        }
        fprintf(pFile, "\t%lu -> %lu [style=dashed color=dimgrey constraint=false]\n", idx, nodes[idx].suffLink);
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
    curNode(rootID)
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
        LOGS("Searching for letter %d in %d node", idx, ndID)
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