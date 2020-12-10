#include "../utils/dynarray.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int cmpFunc(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    Dynarr input = Dynarr_create(int);
    int first = 0;
    Dynarr_insert(&input, &first);

    int x;
    while (scanf("%d", &x) != EOF) {
        Dynarr_insert(&input, &x);
    }

    Dynarr_sort(&input, cmpFunc);
    int highest = *(int *)Dynarr_get(&input, input.len - 1);
    highest += 3;
    Dynarr_insert(&input, &highest);

    int oneDiff = 0;
    int threeDiff = 0;

    for (int i = 0; i < input.len - 1; i++) {
        int d = *(int *)Dynarr_get(&input, i + 1) - *(int *)Dynarr_get(&input, i);
        if (d == 1) {
            oneDiff++;
        }
        if (d == 3) {
            threeDiff++;
        }
    }

    printf("%d\n", oneDiff * threeDiff);

    Dynarr_free(&input);
    return 0;
}
