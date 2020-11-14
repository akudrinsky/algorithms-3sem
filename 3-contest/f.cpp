/*
Вы работаете в компании, специализирующейся в технологиях, связанных с интернетом, и ваш текущий проект — спам-фильтр. Фильтр определяет, содержит ли строка спам, используя словарь спам-слов. Если в строке содержится хотя бы одно слово из этого словаря как подстрока, фильтр считает, что исходная строка подозрительна.

Вы стали решать более интересную задачу: сколько существует различных подозрительных строк длины 𝑛, состоящих из строчных букв латинского алфавита для данного словаря спам-слов. Найдите ответ по модулю 10000.

Входные данные
В первой строке содержатся два числа 𝑛 и 𝑘 (1⩽𝑛<100,1⩽𝑡⩽10) — требуемая длина слов и количество слов в словаре спам-слов соотвественно.

Следующие 𝑘 строк являются строками словаря. Гарантируется, что они состоят из строчных латинских букв, они не пустые, и их длина не превосходит 10 символов.

Выходные данные
Выведите ответ по модулю 10000.
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

    long long CountBad(size_t len, const std::vector<char>& allSymbols, int mod);

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
    int len = 0;
    int nPatterns = 0;
    std::cin >> len >> nPatterns;
    std::vector<std::string> patterns(nPatterns);

    for (int i = 0; i < nPatterns; ++i)
    {
        std::cin >> patterns[i];
        //LOGS("PATTERN: <%s>\n", patterns[i]);
    }

    AhoCorasik solver(patterns);

    const int nLetters = 26;
    std::vector<char> allSymbols;
    for (char symbol = 'a'; symbol < 'a' + nLetters; ++symbol)
        allSymbols.push_back(symbol);

    std::cout << solver.CountBad(len, allSymbols, 10000);

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

std::vector<std::vector<int>> multMatrix(
    std::vector<std::vector<int>> first, 
    std::vector<std::vector<int>> second, 
    int mod
)
{
    int dim = first.size();
    std::vector<std::vector<int>> product(dim, std::vector<int>(dim, 0));
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            for (int k = 0; k < dim; ++k)
            {
                product[i][j] += first[i][k] * second[k][j];
                product[i][j] %= mod;
            }
        }
    }
    return product;
}

std::vector<std::vector<int>> powerMatrix(
    std::vector<std::vector<int>> matrix, 
    int pow,
    int mod
)
{
    auto result = matrix;
    --pow;
    for (; pow > 0; pow >>= 1)
    {
        if (pow & 1)
            result = multMatrix(result, matrix, mod);
        
        matrix = multMatrix(matrix, matrix, mod);
    }
    return result;
}

long long AhoCorasik::CountBad(
    size_t len, 
    const std::vector<char>& allSymbols, 
    int mod
)
{
    std::unordered_map<Node*, int> allNodes;
    int maxID = 0;
    std::queue<Node*> verts;
    verts.push(&root);

    while (!verts.empty())
    {
        Node* cur = verts.front();
        allNodes.insert({cur, maxID});
        ++maxID;
        verts.pop();
        for (
            auto sonIter = cur->go.begin(); 
            sonIter != cur->go.end(); 
            ++sonIter
        )
        {
            Node* child = sonIter->second;
            verts.push(child);
        }
    }

    std::vector<std::vector<int>> matrix(allNodes.size(), std::vector<int>(allNodes.size(), 0));

    for (auto elem : allNodes)
    {
        Node* curNode = elem.first;
        int i = elem.second;

        if (curNode->nearestTerminal != nullptr or curNode->isTerminal())
        {
            matrix[i][i] += allSymbols.size();
        }
        else
        {
            for (auto symbol: allSymbols)
            {
                Node* to = curNode;
                while (to != &root and to->go.count(symbol) == 0)
                    to = to->suffixLink;

                if (to->go.count(symbol) != 0)
                    to = to->go.at(symbol);
                else
                    to = &root;

                matrix[i][allNodes.at(to)]++;
            }
        }
    }

    ON_DEBUG(
        for (int i = 0; i < matrix.size(); ++i)
        {
            for (int j = 0; j < matrix[i].size(); ++j)
            {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '\n';
        }
    )

    auto powMatrix = powerMatrix(matrix, len, mod);

    uint64_t result = 0;
    uint64_t nNodes = allNodes.size();
    for (auto& elem : allNodes)
    {
        if (elem.first->nearestTerminal != nullptr or elem.first->isTerminal())
        {
            result = (result + powMatrix.at(0).at(elem.second)) % mod;
        }
    }
    return result;
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