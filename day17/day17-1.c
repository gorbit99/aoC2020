#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/dynstring.h"
#include <limits.h>

typedef struct Pos {
    int x;
    int y;
    int z;
} Pos;

typedef struct Cell {
    Pos pos;
    bool alive;
    bool nextState;
    struct Cell *next;
} Cell;

typedef struct Bounds {
    Pos min;
    Pos max;
} Bounds;

Cell *get_cell(Cell **cells, Pos pos) {
    while (*cells != NULL) {
        if ((*cells)->pos.x == pos.x 
                && (*cells)->pos.y == pos.y
                && (*cells)->pos.z == pos.z) {
            return *cells;
        }
        cells = &(*cells)->next;
    }
    *cells = (Cell *)malloc(sizeof(Cell));
    (*cells)->alive = false;
    (*cells)->nextState = false;
    (*cells)->next = NULL;
    (*cells)->pos = pos;
    return *cells;
}

void update_cells(Cell *cells) {
    while (cells != NULL) {
        cells->alive = cells->nextState;
        cells = cells->next;
    }
}

int get_neighbour_count(Cell *cells, Pos pos) {
    int count = 0;
    while (cells != NULL) {
        int dx = abs(cells->pos.x - pos.x);
        int dy = abs(cells->pos.y - pos.y);
        int dz = abs(cells->pos.z - pos.z);
        if (dx <= 1 && dy <= 1 && dz <= 1 && dx + dy + dz != 0) {
            if (cells->alive) {
                count++;
            }
        }
        cells = cells->next;
    }
    return count;
}

Bounds get_bounds(Cell *cells) {
    Bounds bounds = {
        {INT_MAX, INT_MAX, INT_MAX},
        {INT_MIN, INT_MIN, INT_MIN}
    };

    while (cells != NULL) {
        if (cells->pos.x < bounds.min.x) {
            bounds.min.x = cells->pos.x;
        }
        if (cells->pos.y < bounds.min.y) {
            bounds.min.y = cells->pos.y;
        }
        if (cells->pos.z < bounds.min.z) {
            bounds.min.z = cells->pos.z;
        }
        if (cells->pos.x > bounds.max.x) {
            bounds.max.x = cells->pos.x;
        }
        if (cells->pos.y > bounds.max.y) {
            bounds.max.y = cells->pos.y;
        }
        if (cells->pos.z > bounds.max.z) {
            bounds.max.z = cells->pos.z;
        }

        cells = cells->next;
    }

    return bounds;
}

int count_alive(Cell *cells) {
    int count = 0;
    while (cells != NULL) {
        if (cells->alive) {
            count++;
        }
        cells = cells->next;
    }
    return count;
}

int main() {
    Cell *cells = NULL;

    int y = 0;
    String line;
    while ((line = String_readline()).len > 0) {
        for (int x = 0; x < line.len; x++) {
            get_cell(&cells, (Pos){x, y, 0})->alive = line.data[x] == '#';
        }
        String_free(&line);
        y++;
    }

    for (int round = 0; round < 6; round++) {
        Bounds bounds = get_bounds(cells);
        bounds.min.x--;
        bounds.min.y--;
        bounds.min.z--;
        bounds.max.x++;
        bounds.max.y++;
        bounds.max.z++;

        for (int z = bounds.min.z; z <= bounds.max.z; z++) {
            for (int y = bounds.min.y; y <= bounds.max.y; y++) {
                for (int x = bounds.min.x; x <= bounds.max.x; x++) {
                    Cell *cell = get_cell(&cells, (Pos){x, y, z});
                    int neighbours = get_neighbour_count(cells, (Pos){x, y, z});
                    cell->nextState = cell->alive;
                    if (cell->alive && neighbours != 2 && neighbours != 3) {
                        cell->nextState = false;
                    }
                    else if (!cell->alive && neighbours == 3) {
                        cell->nextState = true;
                    }
                }
            }
        }
        update_cells(cells);
    }
    String_free(&line);

    printf("%d\n", count_alive(cells));
}
