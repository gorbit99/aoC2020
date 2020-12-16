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

typedef struct Ticket {
    Dynarr fields;
} Ticket;

Ticket parseTicket(String line) {
    char *rest = line.data;
    int num;
    int n;
    Ticket ticket;
    ticket.fields = Dynarr_create(int);
    while (sscanf(rest, "%d,%n", &num, &n) == 1) {
        Dynarr_insert(&ticket.fields, &num);
        rest += n;
    }
    return ticket;
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
    Ticket myTicket = parseTicket(line);
    String_free(&line);

    line = String_readline();
    String_free(&line);

    //Nearby Tickets
    line = String_readline();
    String_free(&line);
    
    Dynarr nearbyTickets = Dynarr_create(Ticket);

    while ((line = String_readline()).len > 0) {
        Ticket ticket = parseTicket(line);
        String_free(&line);
        Dynarr_insert(&nearbyTickets, &ticket);
    }

    int invalidSum = 0;
    for (int i = 0; i < nearbyTickets.len; i++) {
        Ticket *ticket = (Ticket *)Dynarr_get(&nearbyTickets, i);
        for (int j = 0; j < ticket->fields.len; j++) {
            int *num = (int *)Dynarr_get(&ticket->fields, j);
            bool found = false;
            for (int k = 0; k < fields.len; k++) {
                Field *field = (Field *)Dynarr_get(&fields, k);
                for (int l = 0; l < field->ranges.len; l++) {
                    Range *range = Dynarr_get(&field->ranges, l);
                    if (inRange(*range, *num)) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
            if (!found) {
                invalidSum += *num;
            }
        }
    }

    printf("%d\n", invalidSum);
}
