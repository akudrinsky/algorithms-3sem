/*
Дано 𝐾 строк из маленьких латинских букв. Требуется найти их наибольшую общую подстроку.

Входные данные
В первой строке число 𝐾 (1⩽𝐾⩽10).

В следующих 𝐾 строках — собственно 𝐾 строк (длины строк от 1 до 10000).

Выходные данные
Выведите наибольшую общую подстроку.
*/
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>
#include <array>

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

    void Photo(const std::string filename) const;

    static std::string BigggestCommonSubstring(const std::vector<std::string>& patterns);

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

    static void setMarks(
        SuffixTree& tree,
        nodeID curNode,
        std::vector<std::array<char, 10>>& marks,
        std::vector<long long>& patBounds);

    static void checkMarks(
        SuffixTree& tree,
        nodeID curNode,
        const std::vector<char>& allPresented,
        long long& bestLen,
        nodeID& bestND,
        long long curLen);

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

        bool IsLeaf();
    };

    bool findPath(
        std::vector<nodeID>& path, 
        SuffixTree::nodeID curNode, 
        nodeID searchFor);

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

    int nStrings = 0;
    std::cin >> nStrings;

    std::vector<std::string> patterns(nStrings);
    for (int i = 0; i < patterns.size(); ++i)
        std::cin >> patterns[i];

    std::cout << SuffixTree::BigggestCommonSubstring(patterns) << '\n';
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

bool SuffixTree::Node::IsLeaf()
{
    return children.size() == 0;
}

/*--------------------------------------------------------------------------*/

SuffixTree::nodeID SuffixTree::NewNode(SuffixTree::strIdx position, long long len)
{
    nodes.emplace_back(position, len);
    return nodes.size() - 1;
}

/*--------------------------------------------------------------------------*/

std::string SuffixTree::BigggestCommonSubstring(const std::vector<std::string>& patterns)
{
    if (patterns.size() == 0)
        return "";
    
    if (patterns.size() == 1)
        return patterns[0];

    SuffixTree tree;
    for (int i = 0; i < patterns.size(); ++i)
    {
        tree.Extend(patterns[i] + std::to_string(i));
    }

    ON_DEBUG(
        tree.Photo("ex.png");
    )
    
    std::vector<std::array<char, 10>> marks(tree.nodes.size());
    std::vector<long long> patBounds(patterns.size());
    patBounds[0] = patterns[0].size();
    for (int i = 1; i < patterns.size(); ++i)
        patBounds[i] = patBounds[i - 1] + patterns[i].size() + 1;

    setMarks(tree, rootID, marks, patBounds);

    ON_DEBUG(
        fprintf(stderr, "patBounds:\n");
        for (int i = 0; i < patBounds.size(); ++i)
            fprintf(stderr, "%3d) %lld\n", i, patBounds[i]);
        fprintf(stderr, "\n\nmarks:\n");
        for (int i = 0; i < marks.size(); ++i)
        {
            fprintf(stderr, "%3d) ", i);
            for (int j = 0; j < marks[i].size(); ++j)
            {
                if (marks[i][j])
                    fprintf(stderr, "%2d ", j);
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
        tree.Photo("ex.png");
    )

    std::vector<char> allPresented(marks.size(), 1);
    for (int i = 0; i < allPresented.size(); ++i)
    {
        for (int j = 0; j < patterns.size(); ++j)
        {
            if (marks[i][j] == 0)
            {
                allPresented[i] = 0;
            }
        }
    }

    ON_DEBUG(
        fprintf(stderr, "allPresented:\n");
        for (int i = 0; i < allPresented.size(); ++i)
            fprintf(stderr, "%3d) %d\n", i, allPresented[i]);
        fprintf(stderr, "\n\n");
    )

    long long bestLen = 0;
    nodeID bestND = rootID;
    checkMarks(
        tree,
        rootID,
        allPresented,
        bestLen,
        bestND,
        0);
    
    LOGS("best len = %lld of best node %lld", bestLen, bestND)
    
    std::vector<nodeID> path;
    tree.findPath(path, rootID, bestND);
    ON_DEBUG(
        fprintf(stderr, "path:\n");
        for (int i = 0; i < path.size(); ++i)
            fprintf(stderr, "%3d) %lld\n", i, path[i]);
        fprintf(stderr, "\n\n");
    )
    if (path.size() == 0 or (path.size() == 1 and path[0] == rootID))
        return "";

    std::string substr = "";
    for (auto id : path)
    {
        if (id != rootID)
            substr += tree.data.substr(tree.nodes[id].startIndex, tree.nodes[id].len);
    }
    return substr;
}   

/*--------------------------------------------------------------------------*/

void SuffixTree::setMarks(
    SuffixTree& tree,
    SuffixTree::nodeID curNode,  
    std::vector<std::array<char, 10UL>> &marks, 
    std::vector<long long> &patBounds)
{
    Node& nd = tree.nodes[curNode];
    marks[curNode][std::lower_bound(
        patBounds.begin(), 
        patBounds.end(), 
        nd.startIndex) - patBounds.begin()] = 1;

    if (nd.children.size() == 0)
        return;
    
    for (auto& child : nd.children)
    {
        nodeID childID = child.second;
        setMarks(tree, childID, marks, patBounds);
        for (int i = 0; i < marks[curNode].size(); ++i)
        {
            marks[curNode][i] |= marks[childID][i];
        }
    }
}

/*--------------------------------------------------------------------------*/

void SuffixTree::checkMarks(
    SuffixTree &tree, 
    SuffixTree::nodeID curNode, 
    const std::vector<char> &allPresented, 
    long long &bestLen, 
    nodeID& bestNode,
    long long curLen)
{
    Node& nd = tree.nodes[curNode];
    if (curNode != rootID)
    {
        if (allPresented[curNode] and curLen > bestLen)
        {
            bestLen = curLen;
            bestNode = curNode;
        }
    }


    for (auto& child : nd.children)
    {
        nodeID childID = child.second;

        long long childLen = 0;
        if (tree.nodes[childID].len < infinity)
            childLen = curLen + tree.nodes[childID].len;
        else
            childLen = curLen + (tree.data.size() - tree.nodes[childID].startIndex);
        
        checkMarks(tree, childID, allPresented, bestLen, bestNode, childLen);
    }
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

bool SuffixTree::findPath(
    std::vector<long long>& path, 
    SuffixTree::nodeID curNode,
    SuffixTree::nodeID searchFor)
{
    Node& nd = nodes[curNode];
    path.push_back(curNode);
    if (curNode == searchFor)
        return true;

    for (auto& child : nd.children)
    {
        nodeID childID = child.second;
        if (findPath(path, childID, searchFor))
            return true;
    }

    path.pop_back();
    return false;
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

void SuffixTree::Photo(std::string filename) const
{
    FILE* pFile = fopen ("tree_graph.dot", "w");
    fprintf (pFile, "digraph G{\n\tedge[fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"rectangle\",fontsize=15, dpi=10000];\n");

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