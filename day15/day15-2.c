#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct Number {
    long long val;
    long long lastSeen;
    struct Number *left;
    struct Number *right;
} Number;

long long insertNumber(Number **numbers, long long num, long long round) {
    while (*numbers != NULL && (*numbers)->val != num) {
        if ((*numbers)->val > num) {
            numbers = &(*numbers)->left;
        } else {
            numbers = &(*numbers)->right;
        }
    }
    if (*numbers == NULL) {
        *numbers = (Number *)malloc(sizeof(Number));
        (*numbers)->val = num;
        (*numbers)->lastSeen = round;
        return 0;
    }
    long long prev = (*numbers)->lastSeen;
    (*numbers)->lastSeen = round;
    return round - prev;
}

int main() {
    Number *numbers = NULL;
    long long nextNum;
    long long round = 0;

    long long num;
    while (scanf("%lld,", &num) != EOF) {
        nextNum = insertNumber(&numbers, num, round);

        round++;
    }

    while (round < 29999999) {
        nextNum = insertNumber(&numbers, nextNum, round);
        round++;
    }

    printf("%lld\n", nextNum);
}
