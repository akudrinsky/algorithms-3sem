/*
Одна из новых возможностей текстового редактора «World XP» – это сортировка слов
в предложении. Выход новой бета-версии редактора должен состоятся не позднее,
чем через пять часов, а заявленная функция еще не реализована.
 
Требуется написать программу, осуществляющую сортировку слов в предложении. При
этом все символы, отличные от букв, должны сохраниться и не поменять своего
положения относительно вхождений слов. Для упрощения при подаче входных данных
на вход вашей программы все такие символы будут заменены на символ «.» (точка).
Таким образом символ «.» имеет смысл разделителя между словами. Например, строка
«..aba.a..ba» после сортировки пример вид «..a.aba..ba», а строка «c..bb.a»
примет вид «a..bb.c». Слова следует сортировать лексикографически, как в
словаре.
 
Входные данные
Входной файл содержит единственную строку, содержащую только прописные латинские
буквы и символ «.». Слова могут разделяться любым количеством символов «.»,
строка может как начинаться, так и заканчиваться последовательностью точек.
Длина заданной строки не менее 1 символа и не превосходит 106 символов.
 
Выходные данные
В выходной файл выведите строку после сортировки слов в ней.
*/
 
#include <algorithm>
#include <cstdio>
#include <cstring>
 
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
 
struct String {
    char* beginning;
    int len;
};
 
int countElems(char* beginning, char elem, bool contains) {
    char* cur = beginning;
    while (!((*cur == elem) ^ contains) and *cur != '\0') {
        ++cur;
    }
    return cur - beginning;
}
 
void process(char* rawData, String* toProcess, int* dots, const int maxSize,
             int& nWords) {
    nWords = 0;
 
    while (*rawData != '\0') {
        int nDots = countElems(rawData, '.', true);
        dots[nWords] = nDots;
        LOGS("nDots %d\n", dots[nWords])
        rawData += nDots;
 
        int nNotDots = countElems(rawData, '.', false);
        if (nNotDots == 0) {
            continue;
        }
        LOGS("nNotDots %d\n", nNotDots)
        toProcess[nWords].beginning = rawData;
        toProcess[nWords].len = nNotDots;
        rawData += nNotDots;
 
        ++nWords;
    }
}
 
int Comparator(const void* first, const void* second) {
    const String* firstStr = (String*)first;
    const String* secondStr = (String*)second;
 
    const int len = firstStr->len < secondStr->len ? firstStr->len
                                                   : secondStr->len;
    const int res = strncmp(firstStr->beginning, secondStr->beginning, len);
 
    if (res) return res;
    if (firstStr->len == secondStr->len) return 0;
    return firstStr->len < secondStr->len ? -1 : 1;
}
 
int main() {
    const int maxSize = 1000000;
    char* allData = new char[maxSize];
 
    scanf("%s", allData);
 
    String* processedData = new String[maxSize];
    int* dots = new int[maxSize];
 
    int nWords = 0;
 
    LOGS("1");
 
    process(allData, processedData, dots, maxSize, nWords);
 
    LOGS("2");
 
    qsort(processedData, nWords, sizeof(String), Comparator);
 
    LOGS("3");
 
    ON_DEBUG(for (int i = 0; i <= nWords; ++i) {
        printf("counted %d dots, then %d notDots\n", dots[i],
               processedData[i].len);
    })
 
    for (int i = 0; i < nWords; ++i) {
        for (int j = 0; j < dots[i]; ++j) {
            printf(".");
        }
        for (int j = 0; j < processedData[i].len; ++j) {
            printf("%c", processedData[i].beginning[j]);
        }
    }
    for (int j = 0; j < dots[nWords]; ++j) {
        printf(".");
    }
}