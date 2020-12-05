#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    bool taken[128][8];
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 8; j++) {
            taken[i][j] = false;
        }
    }
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
        taken[minRow][minCol] = true;
        if (getchar() == -1) {
            break;
        }
    }

    bool found = false;
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 8; j++) {
            if (taken[i][j]) {
                found = true;
            } else if (found) {
                printf("%d\n", i * 8 + j);
                return 0;
            }
        }
    }
}
