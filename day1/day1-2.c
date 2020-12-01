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

void sortValues(int *values, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (values[i] > values[j]) {
                int temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }
}

int main() {
    int count;
    int *values = getPuzzleInput(&count);
    sortValues(values, count);
    
    int aIndex = 0;
    int bIndex = 1;
    int cIndex = 2;

    while (values[aIndex] + values[bIndex] + values[cIndex] != 2020) {
        if (values[bIndex] + values[cIndex] > 2020) {
            aIndex++;
            bIndex = aIndex + 1;
            cIndex = aIndex + 2;
        }
        else if (values[aIndex] + values[bIndex] + values[cIndex] > 2020) {
            bIndex++;
            cIndex = bIndex + 1;
        }
        else {
            bIndex++;
        }
    }

    printf("%d\n", values[aIndex] * values[bIndex] * values[cIndex]);
}
