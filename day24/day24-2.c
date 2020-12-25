#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/dynstring.h"
#include "../utils/dynarray.h"

typedef struct Position {
    int q;
    int r;
} Position;

typedef struct Tile {
    Position pos;
    bool alive;
    bool nextState;
    struct Tile *next;
    int aliveNeighbours;
} Tile;

typedef struct Bounds {
    Position min;
    Position max;
} Bounds;

Tile *get_tile(Tile **tiles, Position pos) {
    while (*tiles != NULL) {
        if ((*tiles)->pos.q == pos.q 
                && (*tiles)->pos.r == pos.r) {
            return *tiles;
        }
        tiles = &(*tiles)->next;
    }
    *tiles = (Tile *)malloc(sizeof(Tile));
    (*tiles)->alive = false;
    (*tiles)->nextState = false;
    (*tiles)->next = NULL;
    (*tiles)->pos = pos;
    (*tiles)->aliveNeighbours = 0;
    return *tiles;
}

void add_to_neighbours(Tile **tiles, Tile *tile, int val) {
    Tile fakeFirst;
    fakeFirst.next = *tiles;
    int bitmask = 0;
    Position neighbours[] = {
        {tile->pos.q + 1, tile->pos.r},
        {tile->pos.q + 1, tile->pos.r - 1},
        {tile->pos.q, tile->pos.r - 1},
        {tile->pos.q - 1, tile->pos.r},
        {tile->pos.q - 1, tile->pos.r + 1},
        {tile->pos.q, tile->pos.r + 1}
    };

    Tile *cur = &fakeFirst;
    while (cur->next != NULL) {
        cur = cur->next;
        for (int i = 0; i < 6; i++) {
            if (bitmask & (1 << i)) {
                continue;
            }
            if (cur->pos.q == neighbours[i].q && cur->pos.r == neighbours[i].r) {
                bitmask |= 1 << i;
                cur->aliveNeighbours += val;
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        if (bitmask & (1 << i)) {
            continue;
        }
        Tile *newTile = (Tile *)malloc(sizeof(Tile));
        newTile->alive = false;
        newTile->nextState = false;
        newTile->next = NULL;
        newTile->pos = neighbours[i];
        newTile->aliveNeighbours = 1;
        cur->next = newTile;
        cur = cur->next;
    }

    *tiles = fakeFirst.next;
}

void set_tile(Tile **tiles, Tile *tile, bool val) {
    if (tile->alive == val) {
        return;
    }
    tile->alive = val;
    add_to_neighbours(tiles, tile, tile->alive ? 1 : -1);
}

void update_tiles(Tile **tiles) {
    Tile *cur = *tiles;
    while (cur != NULL) {
        set_tile(tiles, cur, cur->nextState);
        cur = cur->next;
    }
}

Bounds get_bounds(Tile *tiles) {
    Bounds bounds = {
        {INT_MAX, INT_MAX},
        {INT_MIN, INT_MIN}
    };

    while (tiles != NULL) {
        if (tiles->pos.q < bounds.min.q) {
            bounds.min.q = tiles->pos.q;
        }
        if (tiles->pos.r < bounds.min.r) {
            bounds.min.r = tiles->pos.r;
        }
        if (tiles->pos.q > bounds.max.q) {
            bounds.max.q = tiles->pos.q;
        }
        if (tiles->pos.r > bounds.max.r) {
            bounds.max.r = tiles->pos.r;
        }

        tiles = tiles->next;
    }

    return bounds;
}

int count_alive(Tile *tiles) {
    int count = 0;
    while (tiles != NULL) {
        if (tiles->alive) {
            count++;
        }
        tiles = tiles->next;
    }
    return count;
}

int main() {
    Tile *tiles = NULL;
    String line;
    while ((line = String_readline()).len > 0) {
        int q = 0;
        int r = 0;

        char *str = line.data;
        while (*str != '\0') {
            switch (*str) {
                case 'e':
                    q++;
                    str++;
                    break;
                case 'w':
                    q--;
                    str++;
                    break;
                case 'n':
                    if (*(str + 1) == 'w') {
                        r--;
                        str += 2;
                    } else {
                        q++;
                        r--;
                        str += 2;
                    }
                    break;
                case 's':
                    if (*(str + 1) == 'w') {
                        q--;
                        r++;
                        str += 2;
                    } else {
                        r++;
                        str += 2;
                    }
                    break;
            }
        }
        Position pos = {q, r};
        Tile *tile = get_tile(&tiles, pos);
        set_tile(&tiles, tile, !tile->alive);

        String_free(&line);
    }
    String_free(&line);

    for (int round = 0; round < 100; round++) {
        Bounds bounds = get_bounds(tiles);
        bounds.min.q--;
        bounds.min.r--;
        bounds.max.r++;
        bounds.max.q++;

        Tile *tile = tiles;
        while (tile != NULL) {
            tile->nextState = tile->alive;
            int neighbours = tile->aliveNeighbours;
            if (tile->alive && (neighbours == 0 || neighbours > 2)) {
                tile->nextState = false;
            }
            else if (!tile->alive && neighbours == 2) {
                tile->nextState = true;
            }
            tile = tile->next;
        }
        update_tiles(&tiles);
    }

    printf("%d\n", count_alive(tiles));
}
