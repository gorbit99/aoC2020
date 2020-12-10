#include "../utils/dynarray.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int cmpFunc(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

long long getWays(Dynarr input, int curId, int curJoltage, long long *saved) {
    if (curId + 1 == input.len) {
        saved[curId] = 1;
        return 1;
    }
    long long ways = 0;
    for (int i = 0; i < 3; i++) {
        if (curId + i + 1 >= input.len || *(int *)Dynarr_get(&input, curId + i + 1) - curJoltage > 3) {
            break;
        }
        if (saved[curId + i + 1] != -1) {
            ways += saved[curId + i + 1];
        } else {
            ways += getWays(input, curId + i + 1, *(int *)Dynarr_get(&input, curId + i + 1), saved);
        }
    }
    if (curId != -1) {
        saved[curId] = ways;
    }
    return ways;
}

int main() {
    Dynarr input = Dynarr_create(int);

    int x;
    while (scanf("%d", &x) != EOF) {
        Dynarr_insert(&input, &x);
    }

    Dynarr_sort(&input, cmpFunc);

    long long *saved = (long long *)malloc(sizeof(long long) * input.len);
    for (int i = 0; i < input.len; i++) {
        saved[i] = -1;
    }

    printf("%lld\n", getWays(input, -1, 0, saved));

    Dynarr_free(&input);
    free(saved);
    return 0;
}
