#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int x;
    scanf("%d", &x);

    int len = ceil(log10(x));
    int *nums = (int *)malloc(sizeof(int) * len);

    for (int i = 0; i < len; i++) {
        nums[len - i - 1] = x % 10;
        x /= 10;
    }

    int currentCup = 0;

    for (int i = 0; i < 100; i++) {
        int cur = nums[currentCup];
        int a = nums[(currentCup + 1) % len];
        int b = nums[(currentCup + 2) % len];
        int c = nums[(currentCup + 3) % len];

        int dest = (nums[currentCup] - 1 - 1 + len) % len + 1;
        while (dest == a || dest == b || dest == c) {
            dest = (dest - 1 - 1 + len) % len + 1;
        }

        int destPos = 0;
        while (nums[destPos] != dest) {
            destPos++;
        }

        for (int i = currentCup; i != destPos; i = (i - 1 + len) % len) {
            nums[(i + 3) % len] = nums[i];
        }

        nums[(destPos + 1) % len] = a;
        nums[(destPos + 2) % len] = b;
        nums[(destPos + 3) % len] = c;

        for (int i = 0; i < len; i++) {
            if (nums[i] == cur) {
                currentCup = (i + 1) % len;
                break;
            }
        }
    }

    for (int i = 0; i < len; i++) {
        if (nums[i] == 1) {
            for (int j = 1; j < len; j++) {
                printf("%d", nums[(i + j) % len]);
            }
            printf("\n");
            break;
        }
    }
}
