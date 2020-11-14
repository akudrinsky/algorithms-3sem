/*
Просто Ахо-Корасик
*/

#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>
#include <iostream>

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

class AhoCorasik
{
public:
    AhoCorasik(std::vector<std::string>& patterns);

    ~AhoCorasik();

    void SearchIn(const std::string& str);

    bool CheckInf(const std::vector<char>& alphabet);

    void photo(std::string filename);

private:
    struct Node
    {
        std::unordered_map<char, Node*> go;
        Node* nearestTerminal;
        Node* suffixLink;
        std::unordered_set<int> patternIDs;

        explicit Node(Node* forFailure = nullptr);
        Node* safeGo(char symbol);
        bool isTerminal();
    };

    Node root;
    Node* automatonState;
    const std::vector<std::string>& patterns;

    bool checkRec(const Node* cur, std::unordered_map<const Node*, char>& color, const std::vector<char>& alphabet);

    void step(char symbol);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


int main()
{
    int nPatterns = 0;
    std::cin >> nPatterns;
    std::vector<std::string> patterns(nPatterns);

    for (int i = 0; i < nPatterns; ++i)
    {
        std::cin >> patterns[i];
        //LOGS("PATTERN: <%s>\n", patterns[i]);
    }

    AhoCorasik solver(patterns);

    if (solver.CheckInf({'0', '1'}))
        std::cout << "TAK";
    else
        std::cout << "NIE";
    

    return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

AhoCorasik::AhoCorasik(std::vector<std::string>& patterns)
    :
    root(),
    automatonState(nullptr),
    patterns(patterns)
{
    for (auto line = patterns.begin(); line < patterns.end(); ++line)
    {
        Node* curNode = &root;
        for (int i = 0; i < line->size(); ++i)
        {
            Node* childNode = curNode->safeGo((*line)[i]);
            if (childNode == nullptr)
            {
                childNode = new Node(&root);
                curNode->go[(*line)[i]] = childNode;
            }
            
            curNode = childNode;
        }

        curNode->patternIDs.insert(line - patterns.begin());
    }
    //ON_DEBUG(photo("structure.png");)
    LOGS("structure ready\n")

    std::queue<Node*> nodesQueue;
    nodesQueue.push(&root);
    while (!nodesQueue.empty())
    {
        Node* curNode = nodesQueue.front();
        nodesQueue.pop();
        for (
            auto sonIter = curNode->go.begin(); 
            sonIter != curNode->go.end(); 
            ++sonIter
        )
        {
            char symbol = sonIter->first;
            LOGS("edge by <%c>\n", symbol)
            Node* child = sonIter->second;

            Node* tempNode = curNode->suffixLink;
            //LOGS("suf link before updating: %p, root %p\n", tempNode, &root)
            while (tempNode != nullptr) 
            {
                Node* failCandidate = tempNode->safeGo(symbol);
                if (failCandidate != nullptr) 
                {
                    child->suffixLink = failCandidate;
                    break;
                }
                tempNode = tempNode->suffixLink;
            }

            child->nearestTerminal = child->suffixLink->isTerminal() ? child->suffixLink : child->suffixLink->nearestTerminal;
    
            nodesQueue.push(child);
        }
    }
    ON_DEBUG(photo("afterConstructing.png");)
}

AhoCorasik::~AhoCorasik()
{
    // TODO: delete everything
}

void AhoCorasik::SearchIn(const std::string& str)
{
    std::vector<std::vector<int>> occurences(patterns.size());
    automatonState = &root;
    //LOGS("searching in %s\n", str)
    for (int i = 0; str[i] != '\0'; ++i)
    {
        step(str[i]);
        ON_DEBUG(
            photo(std::to_string(i) + ".png");
        )

        if (automatonState->isTerminal()) 
        {
            for (auto patternID : automatonState->patternIDs)
            {
                LOGS("patternID of automaton is %d\n", patternID)
                occurences.at(patternID).push_back(i - patterns.at(patternID).size() + 2);
            }
        }

        LOGS("Term jump %d, automaton at %p\n", i, automatonState)
        Node* tempNode = automatonState->nearestTerminal;
        while (tempNode != nullptr and tempNode != &root) 
        {
            for (auto patternID : tempNode->patternIDs)
            {
                LOGS("patternID of automaton is %d\n", patternID)
                occurences.at(patternID).push_back(i - patterns.at(patternID).size() + 2); 
            }
            tempNode = tempNode->nearestTerminal;
        }
    }

    for (int i = 0; i < occurences.size(); ++i) {
        std::sort(occurences[i].begin(), occurences[i].end());
        printf("%zu", occurences[i].size());
        if (occurences[i].size() > 0)
        {
            for (auto elem : occurences[i])
            {
                printf(" %d", elem);
            }
        }
        printf("\n");
    }
}

AhoCorasik::Node* AhoCorasik::Node::safeGo(char symbol)
{
    LOGS("safeGo by <%c>\n", symbol)
    auto iter = go.find(symbol);
    return iter == go.end() ? nullptr : iter->second;
}

void AhoCorasik::step(char symbol)
{
    while (automatonState != nullptr)
    {
        LOGS("step loop\n")
        Node* candidate = automatonState->safeGo(symbol);
        if (candidate != nullptr)
        {
            LOGS("Found node in step\n")
            automatonState = candidate;
            return;
        }
        automatonState = automatonState->suffixLink;
    }
    automatonState = &root;
}

AhoCorasik::Node::Node(Node* forFailure)
    : 
    suffixLink(forFailure),
    nearestTerminal(nullptr),
    go({}),
    patternIDs()
{}

bool AhoCorasik::Node::isTerminal()
{
    return patternIDs.size() > 0;
}

bool AhoCorasik::CheckInf(const std::vector<char>& alphabet)
{
    std::unordered_map<const AhoCorasik::Node*, char> colors;
    return checkRec(&root, colors, alphabet);
}

bool AhoCorasik::checkRec(
    const AhoCorasik::Node* cur, 
    std::unordered_map<const AhoCorasik::Node*, char>& colors, 
    const std::vector<char>& alphabet
) 
{
    if (colors.count(cur) == 0)
        colors.insert({cur, '?'});

    char& curNodeColor = colors.at(cur);
    if (curNodeColor == 'f') 
        return false;
    if (curNodeColor == 't') 
        return true;
        
    curNodeColor = 't';

    for (const char& c: alphabet) 
    {
        const Node* next = cur;
        while (next != &root and next->go.count(c) == 0)
            next = next->suffixLink;

        if (next->go.count(c) != 0)
            next = next->go.at(c);
        else
            next = &root;
        
        if ((next->nearestTerminal == nullptr) and checkRec(next, colors, alphabet))
            return true;
    }

    curNodeColor = 'f';
    return false;
}

void AhoCorasik::photo(std::string pictName) {
    LOGS("Making photo\n")
    FILE* pFile = fopen ("tree_graph.dot", "w");
    fprintf (pFile, "digraph G{\n");

    std::string name;
    Node* cur = &root;

    std::queue<Node*> verts;
    verts.push(&root);
    fprintf (pFile, "\t%lu [shape=circle, style=\"filled\", color=\"#e00006ff\", label=\"ROOT\"];\n", &root);

    while (!verts.empty())
    {
        Node* cur = verts.front();
        verts.pop();
        for (
            auto sonIter = cur->go.begin(); 
            sonIter != cur->go.end(); 
            ++sonIter
        )
        {
            Node* child = sonIter->second;
            if (child == automatonState)
                fprintf (pFile, "\t%lu [shape=circle, style=\"filled\", color=yellow, label=\"\"];\n", child);
            else if (child->isTerminal())
                fprintf (pFile, "\t%lu [shape=circle, style=\"filled\", color=\"#e27dd6ff\", label=\"\"];\n", child);
            else
                fprintf (pFile, "\t%lu [shape=circle, style=\"filled\", color=\"#e27006ff\", label=\"\"];\n", child);
            verts.push(child);
        }
    }

    verts.push(&root);
    while (!verts.empty())
    {
        Node* cur = verts.front();
        verts.pop();
        for (
            auto sonIter = cur->go.begin(); 
            sonIter != cur->go.end(); 
            ++sonIter
        )
        {
            Node* child = sonIter->second;
            fprintf 
            (
                pFile, 
                "\t%lu -> %lu [color=black, label=\"%c\"]\n", 
                cur, 
                child,
                sonIter->first
            );
            if (child->suffixLink != nullptr)
                fprintf 
                (
                    pFile, 
                    "\t%lu -> %lu [color=green, constraint=false]\n", 
                    child, 
                    child->suffixLink
                );
            if (child->nearestTerminal != nullptr)
                fprintf 
                (
                    pFile, 
                    "\t%lu -> %lu [color=red, constraint=false]\n", 
                    child, 
                    child->nearestTerminal
                );
            verts.push(child);
        }
    }

    fputc('}', pFile);
    fclose(pFile);

    const int max_cmd_size = 50;
    char command[max_cmd_size] = "dot tree_graph.dot -T ";
    strcat (command, "png");
    strcat (command, " -o ");
    strcat (command, pictName.c_str());
    system (command);

    system("rm tree_graph.dot");
    LOGS("It was made\n")
}