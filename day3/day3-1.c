#include <stdio.h>
#include <stdlib.h>

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

    char **input = (char **)malloc(sizeof(char *) * 2);
    input[0] = firstLine;
    input[1] = (char *)malloc(sizeof(char) * lineLen + 1);
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

    const int descentY = 1;
    const int descentX = 3;
    int posX = 0;
    int posY = 0;

    int treeCount = 0;
    while (posY < lineCount - 1) {
        posX += descentX;
        posY += descentY;
        if (input[posY][posX % lineLen] == '#') {
            treeCount++;
        }
    }

    printf("%d\n", treeCount);
}
