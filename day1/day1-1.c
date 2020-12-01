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
