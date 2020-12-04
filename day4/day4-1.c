#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *read_field() {
    int firstChar = getchar();
    if (firstChar == -1 || firstChar == '\n') {
        return NULL;
    }
    char *buffer = (char *)malloc(sizeof(char) * 4);
    buffer[0] = firstChar;
    scanf("%3s", buffer + 1);
    scanf("%*s");
    getchar();
    return buffer;
}

typedef struct Data {
    char *dataId;
    bool found;
} Data;

int main() {
    Data data[7] = {
        {.dataId = "byr", .found = false},
        {.dataId = "iyr", .found = false},
        {.dataId = "eyr", .found = false},
        {.dataId = "hgt", .found = false},
        {.dataId = "hcl", .found = false},
        {.dataId = "ecl", .found = false},
        {.dataId = "pid", .found = false},
    };

    int validIds = 0;
    while (!feof(stdin)) {
        char *buf = read_field();
        if (buf == NULL) {
            bool valid = true;
            for (int i = 0; i < 7; i++) {
                if (!data[i].found) {
                    valid = false;
                }
                data[i].found = false;
            }
            if (valid) {
                validIds++;
            }
        } else {
            for (int i = 0; i < 7; i++) {
                if (strncmp(buf, data[i].dataId, 3) == 0) {
                    data[i].found = true;
                    break;
                }
            }
        }
        free(buf);
    }

    printf("%d\n", validIds);
}
