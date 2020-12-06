#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    int counts = 0;
    while (true) {
        bool found[26] = {false};
        int f;
        while (true) {
            int i = 0;
            while ((f = getchar()) != '\n' && f != -1) {
                i++;
                found[f - 'a'] = true;
            }
            if (i == 0) {
                break;
            }
        }

        for (int i = 0; i < 26; i++) {
            if (found[i]) {
                counts++;
            }
        }
        if (f == -1) {
            break;
        }
    }

    printf("%d\n", counts);
}
