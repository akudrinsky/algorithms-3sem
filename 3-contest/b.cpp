#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class DNAfromGens {
   public:
    DNAfromGens() : root(new Node) {}

    void NewDNA(std::string&& inputString);

    void NewGene(const std::string& inputString);

    ~DNAfromGens();

   private:
    struct Node {
        std::unordered_map<char, Node*> children;
        std::unordered_set<long long> pending;

        bool isTerminal() const;
    };

    struct DNA {
        const std::string inputString;
        long long pos;
        Node* node;

        DNA(Node* node, std::string&& inputString);
    };

    std::vector<DNA> DNAsToProcess;
    Node* root;

    bool pushDNA(long long id);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------MAIN-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

int main() {
    long long n = 0;
    std::cin >> n;

    DNAfromGens ProblemSolver;

    long long k = 0;
    for (int i = 0; i < n; ++i) {
        char op = 0;
        std::cin >> op;

        std::string inputString;
        std::cin >> inputString;

        k %= inputString.length();
        if (k > 0) {
            inputString = inputString.substr(k, inputString.length() - k) +
                          inputString.substr(0, k);
        }

        if (op == '+') {
            ProblemSolver.NewGene(inputString);
        } else {
            ProblemSolver.NewDNA(std::move(inputString));
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void DNAfromGens::NewDNA(std::string&& inputString) {
    const long long id = DNAsToProcess.size();
    root->pending.insert(id);
    DNAsToProcess.emplace_back(root, std::move(inputString));
    if (pushDNA(id)) 
        std::cout << 1 << ' ' << id + 1 << '\n';
    else 
        std::cout << 0 << '\n';
}

void DNAfromGens::NewGene(const std::string& inputString) {
    std::vector<long long> toUpdate;

    Node* curNode = root;
    for (const auto& c : inputString) {
        if (!curNode->children.count(c)) {
            curNode->children.insert({c, new Node});

            for (const int& id : curNode->pending) {
                const DNA& dna = DNAsToProcess[id];
                if (dna.inputString[dna.pos] == c) toUpdate.push_back(id);
            }
        }

        curNode = curNode->children.at(c);
    }

    std::vector<long long> ans;
    for (auto& id : toUpdate)
        if (pushDNA(id)) ans.push_back(id);

    std::cout << ans.size() << ' ';
    for (auto elem : ans)
        std::cout << elem  + 1 << ' ';
    std::cout << '\n';
}

DNAfromGens::~DNAfromGens() { delete root; }

bool DNAfromGens::Node::isTerminal() const { return children.empty(); }

DNAfromGens::DNA::DNA(DNAfromGens::Node* node, std::string&& inputString)
    : node(node), inputString(inputString), pos(0) {}

bool DNAfromGens::pushDNA(long long id) {
    DNA& dna = DNAsToProcess[id];
    Node* curNode = dna.node;
    curNode->pending.erase(id);

    while (dna.pos < dna.inputString.length()) {
        const char& c = dna.inputString[dna.pos];

        if (curNode->children.count(c)) {
            curNode = curNode->children[c];
            ++dna.pos;
        } else if (curNode->isTerminal() and curNode != root)
            curNode = root;
        else
            break;
    }

    if (dna.pos == dna.inputString.length() and curNode->isTerminal())
        return true;

    if (dna.pos < dna.inputString.length()) {
        curNode->pending.insert(id);
        dna.node = curNode;
    } else
        dna.node = nullptr;

    return false;
}