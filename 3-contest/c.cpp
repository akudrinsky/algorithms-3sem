/*
Ученые работают на раскопках окаменелых останков древних существ на планете соседней звездной системы. В процессе исследования ученые пытаются понять, как именно цепочки ДНК различных существ составлялись из генов.

Цепочки ДНК всех исследуемых существ представляют собой последовательности нуклеотидов. Каждый нуклеотид обозначется строчной буквой латинского алфавита. Таким образом, цепочка ДНК представляет собой строку, составленную из строчных букв латинского алфавита.

Ген также представляет собой строку из строчных букв латинского алфавита. Известно, что в любом корректном наборе генов никакая строка не является префиксом другой строки.

Будем говорить, что цепочку ДНК 𝑑 можно расшифровать с использованием набора генов 𝐺, если 𝑑 можно представить как результат последовательной записи одного или нескольких генов: 𝑑=𝑔1𝑔2…𝑔𝑘, где 𝑔𝑖 — гены из набора 𝐺. Один и тот же ген может входить в расшифровку ДНК несколько раз.

Для обработки информации ученым требуется разработать компьютерную систему, которая будет поддерживать корректный набор генов 𝐺 и массив цепочек ДНК существ 𝐷. По мере анализа останков, ученые могут добавлять новый ген в набор 𝐺 или добавлять новую цепочку ДНК в массив 𝐷. Гарантируется, что ни в какой момент времени не существует двух генов, один из которых является префиксом другого.

После каждой операции ученые хотят знать, какие цепочки ДНК в массиве 𝐷 можно расшифровать, используя текущий набор генов 𝐺. После 𝑖-й операции система должна сообщать 𝑘𝑖 — количество цепочек ДНК, находящихся в массиве 𝐷, которые впервые стало можно расшифровать после 𝑖-й операции, а затем 𝑘𝑖 чисел — номера этих цепочек. Результат очередной операции должен быть получен до того, как станет известна следующая операция.

Помогите ученым разработать такую систему.

Входные данные
В первой строке находится число 𝑛 — количество операций, которые необходимо выполнить (1≤𝑛≤100000).

В следующих 𝑛 строках находятся описания операций, 𝑖-я строка начинается с символа «+», если эта операция — добавление нового гена в набор 𝐺, или с символа «?», если эта операция — добавление цепочки ДНК в конец массива 𝐷. Далее через пробел находится строка 𝑥𝑖, состоящая из строчных латинских букв, которую необходимо использовать, чтобы получить строку 𝑠𝑖, которая задает добавляемый в этой операции ген или цепочку ДНК.

Для получения строки 𝑠𝑖 из строки 𝑥𝑖, необходимо выполнить следующие действия. Если 𝑖=1, то 𝑠𝑖=𝑥𝑖. Иначе пусть число впервые расшифрованных цепочек ДНК после предыдущей операции равно 𝑘𝑖−1. Выполним 𝑘𝑖−1 раз следующее действие: перенесем первый символ 𝑥𝑖 в конец. Иначе говоря, выполним циклический сдвиг строки 𝑥𝑖 влево на 𝑘𝑖−1. Получившаяся строка равна 𝑠𝑖 — ген или цепочка ДНК, которую необходимо добавить на 𝑖-й операции.

Все строки не пусты, суммарный размер строк во всех операциях не превышает 106.

Гарантируется, что ни в какой момент времени не существует двух генов, один из которых является префиксом другого.

Выходные данные
Выведите 𝑛 строк.

В 𝑖-й строке выведите сначала число 𝑘𝑖 — количество цепочек ДНК, находящихся в массиве 𝐷, которые впервые стало можно расшифровать после 𝑖-й операции, а затем 𝑘𝑖 чисел — номера этих цепочек. Цепочки нумеруются с единицы в порядке добавления в массив 𝐷. Номера цепочек в одной строке можно выводить в любом порядке.
*/

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class DNAfromGens {
   public:
    DNAfromGens() : root(new Node) {}

    std::vector<long long> NewDNA(const std::string& inputString);

    std::vector<long long> NewGene(const std::string& inputString);

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

        DNA(Node* node, const std::string& inputString);
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

        std::vector<long long> ans(0);

        if (op == '+') {
            ans = ProblemSolver.NewGene(inputString);
        } else {
            ans = ProblemSolver.NewDNA(std::move(inputString));
        }

        k = ans.size();
        std::cout << k << " ";

        for (const auto& id : ans) std::cout << (id + 1) << " ";

        std::cout << "\n";
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//-------------------------------------IMPL-------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

std::vector<long long> DNAfromGens::NewDNA(const std::string& inputString) {
    const long long id = DNAsToProcess.size();
    root->pending.insert(id);
    DNAsToProcess.emplace_back(root, std::move(inputString));
    if (pushDNA(id)) return {id};
    return {};
}

std::vector<long long> DNAfromGens::NewGene(const std::string& inputString) {
    std::vector<long long> to_update;

    Node* curNode = root;
    for (const auto& c : inputString) {
        if (!curNode->children.count(c)) {
            curNode->children.insert({c, new Node});

            for (const int& id : curNode->pending) {
                const DNA& dna = DNAsToProcess[id];
                if (dna.inputString[dna.pos] == c) to_update.push_back(id);
            }
        }

        curNode = curNode->children.at(c);
    }

    std::vector<long long> ans;
    for (const long long& id : to_update)
        if (pushDNA(id)) ans.push_back(id);


    return ans;
}

DNAfromGens::~DNAfromGens() { delete root; }

bool DNAfromGens::Node::isTerminal() const { return children.empty(); }

DNAfromGens::DNA::DNA(DNAfromGens::Node* node, const std::string& inputString)
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