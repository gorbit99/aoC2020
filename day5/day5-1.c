#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    int maxId = 0;
    while (true) {
        int minRow = 0;
        int maxRow = 127;
        for (int i = 0; i < 7; i++) {
            int c = getchar();
            if (c == 'F') {
                maxRow -= (maxRow - minRow + 1) / 2;
            } else {
                minRow += (maxRow - minRow + 1) / 2;
            }
        }
        int minCol = 0;
        int maxCol = 7;
        for (int i = 0; i < 3; i++) {
            int c = getchar();
            if (c == 'R') {
                minCol += (maxCol - minCol + 1) / 2;
            } else {
                maxCol -= (maxCol - minCol + 1) / 2;
            }
        }
        if (minRow * 8 + minCol > maxId) {
            maxId = minRow * 8 + minCol;
        }
        if (getchar() == -1) {
            break;
        }
    }
    
    printf("%d\n", maxId);
}
