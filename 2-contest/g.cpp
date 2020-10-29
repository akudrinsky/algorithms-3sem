/*
Совсем скоро начнется первый тур очередной всероссийской командной олимпиады школьников по палеонтологии (ВКОШП). На олимпиаду приехали команды из $$$n$$$ школ, от каждой школы приехало ровно по две команды. Команды уже заняли свои места, когда обнаружилось, что некоторые команды из одной школы сидят слишком близко. Перед организаторами олимпиады встала серьезная задача — пересадить участников олимпиады.

Столы, за которыми сидят команды, расставлены в один ряд. Расстояние между рабочими местами соседних команд оказалось равно 10 метрам. Организаторы хотят, чтобы минимальное расстояние между двумя командами из одной школы было как можно больше.

Пересаживая команду, организатором необходимо перенести на новое место все оборудование, приготовленное командой для исследований. Поэтому организаторы хотят пересадить команды так, чтобы сумма расстояний между старыми и новыми рабочими местами команд была как можно меньше.

Например, пусть в соревновании принимают участие по две команды школ 1, 2, 3 и 4. Пусть исходно команды распределены по рабочим местам следующим образом: 1, 3, 2, 2, 1, 4, 4, 3 (для каждой команды указан номер школы, которую она представляет). При таком распределении по рабочим местам команды из школы 2 сидят слишком близко, как и команды из школы 4. Пересадив команды в следующем порядке: 1, 3, 2, 4, 1, 3, 2, 4, жюри может добиться своего: команды из одной школы сидят на местах, расстояние между которыми не меньше 40 м, большего расстояния добиться нельзя. Сумма расстояний между старыми и новыми позициями для данного примера равна $$$0 + 0 + 0 + 20 + 0 + 20 + 30 + 10 = 80$$$ м, для исходного распределения команд она минимальна.

Вам задано исходное распределение команд по рабочим местам. Требуется пересадить их так, чтобы минимальное расстояние между командами из одной школы было как можно больше. При этом среди возможных новых размещений команд следует выбрать такое, чтобы сумма расстояний между старыми и новыми местами рабочими команд была минимально возможной.

Входные данные
В первой строке входного файла задано число $$$n$$$ — количество команд ($$$1 \le n \le 100$$$). Во второй строке задано исходное распределение команд по рабочим местам — последовательность $$$a_1, a_2, \ldots, a_{2n}$$$. Для каждой команды указан номер школы, которую она представляет. Гарантируется, что последовательность состоит из чисел от одного до $$$n$$$ и каждое число встречается ровно два раза.

Выходные данные
В единственную строку выходного файла выведите, каким образом следует пересадить команды, чтобы минимальное расстояние между командами из одной школы было как можно больше. При этом среди возможных новых размещений команд следует выбрать такое, чтобы сумма расстояний между старыми и новыми рабочими местами команд была минимально возможной. Если оптимальных ответов несколько, можно вывести любой из них.

Пример
Входные данные
4
1 3 2 2 1 4 4 3

Выходные данные
1 2 4 3 1 2 4 3 
*/

#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
 
#define DEBUG
 
#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
    #define LOGS(...)\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, __func__);\
        fflush(stdout);
#else
    #define LOGS(data, ...) ;
    #define ON_DEBUG(...) ;
#endif

#include <vector>
#include <limits>

template<typename T>
struct borders {
    T min;
    T max;
};

const int inf = std::numeric_limits<int>::max();

std::vector<std::pair<int, int>> hungarian(const std::vector<std::vector<int>> &matrix) {
    int height = matrix.size(), width = matrix[0].size();
 
    std::vector<int> u(height, 0), v(width, 0);

    std::vector<int> markIndices(width, -1);
   
    for (int i = 0; i < height; i++) {
        std::vector<int> links(width, -1);
        std::vector<int> mins(width, inf);
        std::vector<int> visited(width, 0);
      
        int markedI = i, markedJ = -1, j;
        while (markedI != -1) {
            j = -1;
            for (int j1 = 0; j1 < width; j1++) {
                if (!visited[j1]) {
                    if (matrix[markedI][j1] - u[markedI] - v[j1] < mins[j1]) {
                        mins[j1] = matrix[markedI][j1] - u[markedI] - v[j1];
                        links[j1] = markedJ;
                    }
                    if (j == -1 || mins[j1] < mins[j]) {
                        j = j1;
                    }
                }
            }
            
            int delta = mins[j];
            for (int j1 = 0; j1 < width; j1++) {
                if (visited[j1]) {
                    u[markIndices[j1]] += delta;
                    v[j1] -= delta;
                } 
                else {
                    mins[j1] -= delta;
                }
            }
            u[i] += delta;

            visited[j] = 1;
            markedJ = j;
            markedI = markIndices[j];   
        }
      
        for (; links[j] != -1; j = links[j]) {
            markIndices[j] = markIndices[links[j]];
        }
        markIndices[j] = i;
    }
   
    std::vector<std::pair<int, int>> result;
    for (int j = 0; j < width; j++) {
        if (markIndices[j] != -1) {
            result.push_back(std::pair<int, int>(markIndices[j], j));
        }
    }
    return result;
}

int main() {
    int nSchools = 0;
    scanf("%d\n", &nSchools);
    borders<int>* initialOrder = new borders<int>[nSchools];

    for (int i = 0; i < nSchools; ++i) {
        initialOrder[i].min = -1;
        initialOrder[i].max = -1;
    }

    for (int i = 0; i < 2 * nSchools; ++i) {
        int a = -1;
        scanf("%d", &a);
        --a;
        if (initialOrder[a].min == -1) {
            initialOrder[a].min = i;
        }
        else {
            initialOrder[a].max = i;
        }
    }

    std::vector<std::vector<int>> matrix(nSchools, std::vector<int>(nSchools));

    for (int school = 0; school < nSchools; ++school) {
        // now school should have its teams at positions j and j + n
        for (int newPosition = 0; newPosition < nSchools; ++newPosition) {
            long long absLeft = initialOrder[school].min - newPosition;
            if (absLeft < 0) {
                absLeft = -absLeft;                
            }

            long long absRight = initialOrder[school].max - newPosition - nSchools;
            if (absRight < 0) {
                absRight = -absRight;
            }
            
            matrix[school][newPosition] = absLeft + absRight;
        }
    }

    auto pairs = hungarian(matrix);

    std::vector<int> answer(2 * nSchools);
    for (auto elem : pairs) {
        answer[elem.second] = elem.first;
        answer[elem.second + nSchools] = elem.first;
    }

    for (auto iter : answer) {
        printf("%d ", iter + 1);
    }

    delete [] initialOrder;
}