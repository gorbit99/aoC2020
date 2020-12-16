#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dynarray.h"
#include "../utils/dynstring.h"

typedef struct Range {
    int min;
    int max;
} Range;

bool inRange(Range range, int num) {
    return num >= range.min && num <= range.max;
}

typedef struct Field {
    String name;
    Dynarr ranges;
} Field;

Dynarr parseTicket(String line) {
    char *rest = line.data;
    int num;
    int n;
    Dynarr fields = Dynarr_create(int);
    while (sscanf(rest, "%d,%n", &num, &n) == 1) {
        Dynarr_insert(&fields, &num);
        rest += n;
    }
    return fields;
}

bool matchesField(Field field, int num) {
    for (int i = 0; i < field.ranges.len; i++) {
        Range *range = (Range *)Dynarr_get(&field.ranges, i);
        if (inRange(*range, num)) {
            return true;
        }
    }
    return false;
}

int main() {
    Dynarr fields = Dynarr_create(Field);
    String line;
    //Fields
    while ((line = String_readline()).len > 0) {
        int n;
        String_scan(&line, "%*[^:]%n: ", &n);
        String name = String_substr(&line, 0, n);
        char *rest = line.data + n + 2;
        Field field;
        field.name = name;
        field.ranges = Dynarr_create(Range);
        Range range;
        while (sscanf(rest, "%d-%d or %n", &range.min, &range.max, &n) != EOF) {
            rest += n;
            Dynarr_insert(&field.ranges, &range);
        }
        Dynarr_insert(&fields, &field);

        String_free(&line);
    }
    String_free(&line);

    //My Ticket
    line = String_readline();
    String_free(&line);

    line = String_readline();
    Dynarr myTicket = parseTicket(line);
    String_free(&line);

    line = String_readline();
    String_free(&line);

    //Nearby Tickets
    line = String_readline();
    String_free(&line);
    
    long long depMult = 1;
    bool **fieldPossible = (bool **)malloc(sizeof(bool *) * fields.len);
    for (int i = 0; i < fields.len; i++) {
        fieldPossible[i] = (bool *)malloc(sizeof(bool) * fields.len);
        for (int j = 0; j < fields.len; j++) {
            fieldPossible[i][j] = true;
        }
    }

    while ((line = String_readline()).len > 0) {
        Dynarr ticket = parseTicket(line);
        String_free(&line);
        bool valid = true;
        for (int j = 0; j < ticket.len; j++) {
            int *num = (int *)Dynarr_get(&ticket, j);
            bool found = false;
            for (int k = 0; k < fields.len; k++) {
                Field *field = (Field *)Dynarr_get(&fields, k);
                if (matchesField(*field, *num)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                valid = false;
                break;
            }
        }
        if (valid) {
            for (int j = 0; j < ticket.len; j++) {
                int *num = (int *)Dynarr_get(&ticket, j);
                for (int k = 0; k < fields.len; k++) {
                    if (!fieldPossible[k][j]) {
                        continue;
                    }
                    Field *field = (Field *)Dynarr_get(&fields, k);
                    if (!matchesField(*field, *num)) {
                        fieldPossible[k][j] = false;
                    }
                }
            }
        }

        Dynarr_free(&ticket);
    }
    String_free(&line);


    while (true) {
        int filledCount = 0;
        for (int i = 0; i < fields.len; i++) {
            int possibleCount = 0;
            for (int j = 0; j < fields.len; j++) {
                if (fieldPossible[i][j]) {
                    possibleCount++;
                    if (possibleCount > 1) {
                        break;
                    }
                }
            }
            if (possibleCount == 1) {
                for (int j = 0; j < fields.len; j++) {
                    if (fieldPossible[i][j]) {
                        for (int k = 0; k < fields.len; k++) {
                            fieldPossible[k][j] = false;
                        }
                        Field *field = (Field *)Dynarr_get(&fields, i);
                        if (strncmp(field->name.data, "departure", strlen("departure")) == 0) {
                            int *num = Dynarr_get(&myTicket, j);
                            depMult *= *num;
                        }
                        break;
                    }
                }
            }
            if (possibleCount == 0) {
                filledCount++;
            }
        }
        if (filledCount == fields.len) {
            break;
        }

        for (int i = 0; i < fields.len; i++) {
            int possibleCount = 0;
            for (int j = 0; j < fields.len; j++) {
                if (fieldPossible[j][i]) {
                    possibleCount++;
                    if (possibleCount > 1) {
                        break;
                    }
                }
            }
            if (possibleCount == 1) {
                for (int j = 0; j < fields.len; j++) {
                    if (fieldPossible[j][i]) {
                        for (int k = 0; k < fields.len; k++) {
                            fieldPossible[j][k] = false;
                        }
                        Field *field = (Field *)Dynarr_get(&fields, j);
                        if (strncmp(field->name.data, "departure", strlen("departure")) == 0) {
                            int *num = Dynarr_get(&myTicket, i);
                            depMult *= *num;
                        }
                        break;
                    }
                }
            }
        }
    }

    Dynarr_free(&myTicket);
    for (int i = 0; i < fields.len; i++) {
        free(fieldPossible[i]);
        Field *field = Dynarr_get(&fields, i);
        String_free(&field->name);
        Dynarr_free(&field->ranges);
    }
    free(fieldPossible);
    Dynarr_free(&fields);

    printf("%lld\n", depMult);
}
