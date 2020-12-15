#include <stdlib.h>
#include <stdio.h>
#include "../utils/dynarray.h"

typedef struct Number {
    long long val;
    long long lastSeen;
} Number;

Number *findNumber(Dynarr *numbers, long long toFind) {
    for (int i = 0; i < numbers->len; i++) {
        Number *num = ((Number *)Dynarr_get(numbers, i));
        if (num->val == toFind) {
            return num;
        }
    }
    return NULL;
}

long long insertNumber(Dynarr *numbers, long long num, long long round) {
    Number *appearance = findNumber(numbers, num);
    if (appearance == NULL) {
        Number newNumber = {.val = num, .lastSeen = round};
        Dynarr_insert(numbers, &newNumber);
        return 0;
    }
    long long prev = appearance->lastSeen;
    appearance->lastSeen = round;
    return round - prev;
}

int main() {
    Dynarr numbers = Dynarr_create(Number);
    long long nextNum;
    long long round = 0;

    long long num;
    while (scanf("%lld,", &num) != EOF) {
        nextNum = insertNumber(&numbers, num, round);

        round++;
    }

    while (round < 2019) {
        nextNum = insertNumber(&numbers, nextNum, round);
        round++;
    }

    printf("%lld\n", nextNum);
}
