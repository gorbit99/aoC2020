#include <stdio.h>
#include <stddef.h>
#include <string.h>

int main() {
    int validPasswords = 0;

    int first;
    int second;
    char c;
    char pass[50];
    while (scanf("%d-%d %c: %[^\n]\n", &first, &second, &c, pass) == 4) {
        first--;
        second--;
        int passLen = strlen(pass);
        if ((first <= passLen && pass[first] == c) != (second <= passLen && pass[second] == c)) {
            validPasswords++;
        }
    }

    printf("%d\n", validPasswords);

    return 0;
}
