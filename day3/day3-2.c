#include <stdio.h>
#include <stdlib.h>

typedef struct Slope {
    int x;
    int y;
} Slope;


int main() {
    char *firstLine = (char *)malloc(sizeof(char) * 1);
    int lineLen = 0;
    int capacity = 1;

    int c;
    while ((c = getchar()) != '\n') {
        if (capacity <= lineLen) {
            capacity *= 2;
            firstLine = (char *)realloc(firstLine, sizeof(char) * capacity);
        }
        firstLine[lineLen] = (char)c;
        lineLen++;
    }

    printf("%d\n", lineLen);

    char **input = (char **)malloc(sizeof(char *) * 2);
    input[0] = firstLine;
    input[1] = (char *)malloc(sizeof(char) * (lineLen + 1));
    int lineCount = 1;
    capacity = 2;
    while (scanf("%[^\n]%*c", input[lineCount]) == 1) {
        lineCount++;
        if (capacity <= lineCount) {
            capacity *= 2;
            input = realloc(input, sizeof(char *) * capacity);
        }
        input[lineCount] = (char *)malloc(sizeof(char) * lineLen);
    }

    const int slopeCount = 5;
    Slope slopes[5] = {
        {.x = 1, .y = 1},
        {.x = 3, .y = 1},
        {.x = 5, .y = 1},
        {.x = 7, .y = 1},
        {.x = 1, .y = 2}
    };

    long result = 1;
    for (int s = 0; s < slopeCount; s++) {
        int descentX = slopes[s].x;
        int descentY = slopes[s].y;
        int posX = 0;
        int posY = 0;

        long treeCount = 0;
        while (posY < lineCount - 1) {
            posX += descentX;
            posY += descentY;
            if (input[posY][posX % lineLen] == '#') {
                treeCount++;
            }
        }
        result *= treeCount;
    }

    for (int i = 0; i < lineCount; i++) {
        free(input[i]);
    }
    free(input);

    printf("%ld\n", result);
}
