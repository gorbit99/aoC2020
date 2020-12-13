#include "../utils/dynstring.h"
#include "../utils/dynarray.h"
#include <stdbool.h>

typedef struct Pos {
    int x;
    int y;
} Pos;

typedef struct Seat {
    char type;
    Pos visible[8];
    char newType;
} Seat;

char get_seat_type(Dynarr map, int x, int y) {
    return (*(Seat **)Dynarr_get(&map, x))[y].type;
}

void set_seat_type(Dynarr map, int x, int y, char type) {
    (*(Seat **)Dynarr_get(&map, x))[y].newType = type;
}

void update_seat(Dynarr map, int x, int y) {
    (*(Seat **)Dynarr_get(&map, x))[y].type = (*(Seat **)Dynarr_get(&map, x))[y].newType;
}

Pos get_chair_in_direction(int x, int y, int dx, int dy, int w, int h, Dynarr map) {
    x += dx;
    y += dy;
    while (x >= 0 && y >= 0 && x < w && y < h) {
        if (get_seat_type(map, x, y) == 'L') {
            return (Pos){x, y};
        }
        x += dx;
        y += dy;
    }
    return (Pos){-1, -1};
}

int get_taken(Dynarr map, int x, int y) {
    Seat seat = (*(Seat **)Dynarr_get(&map, x))[y];
    int c = 0;
    for (int i = 0; i < 8; i++) {
        Pos pos = seat.visible[i];
        if (pos.x == -1 && pos.y == -1) {
            continue;
        }
        if (get_seat_type(map, pos.x, pos.y) == '#') {
            c++;
        }
    }
    return c;
}

int main() {
    int w = 0;
    int h = 0;
    String curLine;
    Dynarr seats = Dynarr_create(Seat *);
    while ((curLine = String_readline()).len > 0) {
        h = curLine.len;
        Seat *row = (Seat *)malloc(sizeof(Seat) * curLine.len);
        for (int i = 0; i < curLine.len; i++) {
            row[i].type = curLine.data[i];
        }
        Dynarr_insert(&seats, &row);
        w++;
        String_free(&curLine);
    }

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[0] = get_chair_in_direction(x, y, -1, 0, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[1] = get_chair_in_direction(x, y, -1, -1, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[2] = get_chair_in_direction(x, y, 0, -1, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[3] = get_chair_in_direction(x, y, 1, -1, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[4] = get_chair_in_direction(x, y, 1, 0, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[5] = get_chair_in_direction(x, y, 1, 1, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[6] = get_chair_in_direction(x, y, 0, 1, w, h, seats);
            (*(Seat **)Dynarr_get(&seats, x))[y].visible[7] = get_chair_in_direction(x, y, -1, 1, w, h, seats);
        }
    }

    bool changed = true;
    int taken = 0;
    while (changed) {
        changed = false;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                char seat = get_seat_type(seats, x, y);
                if (seat == '.') {
                    continue;
                }
                int occupied = get_taken(seats, x, y);
                if (seat == 'L' && occupied == 0) {
                    set_seat_type(seats, x, y, '#');
                    changed = true;
                    taken++;
                } else if (seat == '#' && occupied >= 5) {
                    set_seat_type(seats, x, y, 'L');
                    taken--;
                    changed = true;
                }
            }
        }
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                update_seat(seats, x, y);
            }
        }
    }

    Dynarr_free(&seats);

    printf("%d\n", taken);
}
