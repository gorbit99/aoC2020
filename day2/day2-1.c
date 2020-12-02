#include <stdio.h>
#include <stddef.h>

int main() {
    int validPasswords = 0;

    int min;
    int max;
    char c;
    char pass[50];
    while (scanf("%d-%d %c: %[^\n]\n", &min, &max, &c, pass) == 4) {
        int cCount = 0;
        for (int i = 0; pass[i] != '\0'; i++) {
            if (pass[i] == c) {
                cCount++;
            }
        }
        if (cCount >= min && cCount <= max) {
            validPasswords++;
        }
    }

    printf("%d\n", validPasswords);

    return 0;
}
