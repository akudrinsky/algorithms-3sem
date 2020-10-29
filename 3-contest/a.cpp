#include <cstdio>
#include <climits>
#include <vector>
#include <queue>
#include <ostream>
#include <iostream>
#include <cstring>
 
//#define DEBUG
 
#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
    #define LOGS(...)\
        printf(__VA_ARGS__);\
        printf("\t\tin file %s, line %d, function %s\n\n", __FILE__, __LINE__, __func__);
#else
    #define LOGS(data, ...) ;
    #define ON_DEBUG(...) ;
#endif

int main() {
    char text[50000] = {};
    char sample[50000] = {};
    char* curLast = text;

    scanf("%s\n%s", text, sample);
    while (curLast[0] != '\0') {
        char* index = strstr(curLast, sample);
        if (index != NULL) {
            printf("%ld ", index - text);
            curLast = index + 1;
        }
        else {
            return 0;
        }
    }
}