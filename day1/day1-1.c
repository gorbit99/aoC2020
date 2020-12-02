#include <stdio.h>
#include <stdlib.h>

int *getPuzzleInput(int *count) {
    int *values = (int *)malloc(sizeof(int) * 1);
    *count = 0;
    int capacity = 1;

    int n;
    while (scanf("%d", &n) == 1) {
        if (capacity == *count) {
            capacity *= 2;
            values = realloc(values, sizeof(int) * capacity);
        }
        values[*count] = n;
        (*count)++;
    }
    return values;
}

int compFunc(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main() {
    int count;
    int *values = getPuzzleInput(&count);
    qsort(values, count, sizeof(int), compFunc);
    
    int aIndex = 0;
    int bIndex = 1;

    while (values[aIndex] + values[bIndex] != 2020) {
        if (values[aIndex] + values[bIndex] < 2020) {
            bIndex++;
        }
        else {
            aIndex++;
            bIndex = aIndex + 1;
        }
    }

    printf("%d\n", values[aIndex] * values[bIndex]);
}
