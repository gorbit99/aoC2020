#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int PREAMBLE = 25;

int main() {
    long long slidingWindow[PREAMBLE];
    for (int i = 0; i < PREAMBLE; i++) {
        scanf("%lld", &slidingWindow[i]);
    }
    int slidingPosition = 0;

    long long current;
    while (scanf("%lld", &current) != EOF) {
        bool found = false;
        for (int i = 0; i < PREAMBLE - 1; i++) {
            for (int j = i + 1; j < PREAMBLE; j++) {
                if (slidingWindow[i] + slidingWindow[j] == current) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        if (!found) {
            printf("%lld\n", current);
            return 0;
        }
        slidingWindow[slidingPosition] = current;
        slidingPosition = (slidingPosition + 1) % PREAMBLE;
    }

    return 0;
}
