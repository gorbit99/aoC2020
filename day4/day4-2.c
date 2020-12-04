#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Id {
    int byr;
    int iyr;
    int eyr;
    int hgt;
    char hgtUnit[4];
    char hcl[9];
    char ecl[5];
    char pid[11];
} Id;

Id id_read() {
    Id id;
    id.byr = -1;
    id.iyr = -1;
    id.eyr = -1;
    id.hgt = -1;
    id.hgtUnit[0] = '\0';
    id.hcl[0] = '\0';
    id.ecl[0] = '\0';
    id.pid[0] = '\0';
    while (true) {
        int firstChar = getchar();
        if (firstChar == '\n' || firstChar == -1) {
            return id;
        }
        char buf[100];
        buf[0] = firstChar;
        scanf("%99s%*c", buf + 1);
        if (strncmp(buf, "byr", 3) == 0) {
            sscanf(buf + 4, "%d%*c", &id.byr);
        }
        else if (strncmp(buf, "iyr", 3) == 0) {
            sscanf(buf + 4, "%d%*c", &id.iyr);
        }
        else if (strncmp(buf, "eyr", 3) == 0) {
            sscanf(buf + 4, "%d%*c", &id.eyr);
        }
        else if (strncmp(buf, "hgt", 3) == 0) {
            sscanf(buf + 4, "%d%3s%*c", &id.hgt, id.hgtUnit);
        }
        else if (strncmp(buf, "hcl", 3) == 0) {
            sscanf(buf + 4, "%8s%*c", id.hcl);
        }
        else if (strncmp(buf, "ecl", 3) == 0) {
            sscanf(buf + 4, "%4s%*c", id.ecl);
        }
        else if (strncmp(buf, "pid", 3) == 0) {
            sscanf(buf + 4, "%10s%*c", id.pid);
        }
    }
    getchar();
}

int main() {
    int validIds = 0;
    while (!feof(stdin)) {
        Id id = id_read();
        if (id.byr < 1920 || id.byr > 2002) {
            continue;
        }
        if (id.iyr < 2010 || id.iyr > 2020) {
            continue;
        }
        if (id.eyr < 2020 || id.eyr > 2030) {
            continue;
        }
        if (strcmp(id.hgtUnit, "cm") == 0) {
            if (id.hgt < 150 || id.hgt > 193) {
                continue;
            }
        }
        else if (strcmp(id.hgtUnit, "in") == 0) {
            if (id.hgt < 59 || id.hgt > 76) {
                continue;
            }
        } else {
            continue;
        }
        if (strlen(id.hcl) != 7 || id.hcl[0] != '#') {
            continue;
        }
        bool validHCL = true;
        for (int i = 0; i < 6; i++) {
            char c = id.hcl[i + 1];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                validHCL = false;
                break;
            }
        }
        if (!validHCL) {
            continue;
        }
        char possibleColors[7][4] = {
            "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
        };
        bool anyValid = false;
        for (int i = 0; i < 7; i++) {
            if (strcmp(possibleColors[i], id.ecl) == 0) {
                anyValid = true;
                break;
            }
        }
        if (!anyValid) {
            continue;
        }
        if (strlen(id.pid) != 9) {
            continue;
        }
        bool pidValid = true;
        for (int i = 0; i < 9; i++) {
            if (id.pid[i] < '0' || id.pid[i] > '9') {
                pidValid = false;
                break;
            }
        }
        if (!pidValid) {
            continue;
        }
        validIds++;
    }
    printf("%d\n", validIds);
}
