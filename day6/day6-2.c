#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    int counts = 0;
    while (true) {
        bool found[26];
        for (int i = 0; i < 26; i++) {
            found[i] = true;
        }
        int f;
        while (true) {
            int i = 0;
            bool person[26] = {false};
            while ((f = getchar()) != '\n' && f != -1) {
                i++;
                person[f - 'a'] = true;
            }
            if (i != 0) {
                for (int i = 0; i < 26; i++) {
                    if (!person[i]) {
                        found[i] = false;
                    }
                }
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
