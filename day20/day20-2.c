#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/dynarray.h"
#include "../utils/dynstring.h"

typedef struct Tile {
    long long id;
    Dynarr lines;
    int rot;
    bool xFlip;
    bool yFlip;
} Tile;

bool read_tile(Tile *tile) {
    String line = String_readline();
    if (line.len == 0) {
        String_free(&line);
        return false;
    }
    long long id;
    String_scan(&line, "Tile %lld:", &id);
    tile->id = id;
    tile->lines = Dynarr_create(String);
    String_free(&line);
    while ((line = String_readline()).len > 0) {
        tile->lines.insert(&tile->lines, &line);
    }
    String_free(&line);
    tile->rot = 0;
    tile->xFlip = false;
    tile->yFlip = false;

    return true;
}

void free_tile(Tile *tile) {
    tile->lines.execute(&tile->lines, (void (*)(void *))String_free);
    tile->lines.free(&tile->lines);
}

int tile_width(Tile *tile) {
    return ((String *)tile->lines.get(&tile->lines, 0))->len;
}

char sample_point(Tile *tile, int x, int y, int rot, bool xFlip, bool yFlip) {
    int w = tile_width(tile);
    switch (rot) {
        case 0:
            break;
        case 1: {
            int temp = x;
            x = y;
            y = w - temp - 1;
            break;
        }
        case 2: {
            x = w - x - 1;
            y = w - y - 1;
            break;
        }
        case 3: {
            int temp = x;
            x = w - y - 1;
            y = temp;
            break;
        }
    } 
    if (xFlip) {
        x = w - x - 1;
    }
    if (yFlip) {
        y = w - y - 1;
    }
    return ((String *)tile->lines.get(&tile->lines, y))->data[x];
}

char sample_str(char **str, int x, int y, int rot, bool xFlip, bool yFlip) {
    int w = strlen(str[0]);
    switch (rot) {
        case 0:
            break;
        case 1: {
            int temp = x;
            x = y;
            y = w - temp - 1;
            break;
        }
        case 2: {
            x = w - x - 1;
            y = w - y - 1;
            break;
        }
        case 3: {
            int temp = x;
            x = w - y - 1;
            y = temp;
            break;
        }
    } 
    if (xFlip) {
        x = w - x - 1;
    }
    if (yFlip) {
        y = w - y - 1;
    }
    return str[x][y];
}

int main() {
    Dynarr tiles = Dynarr_create(Tile);
    Tile tile;
    while (read_tile(&tile)) {
        tiles.insert(&tiles, &tile);
    }

    int size = sqrt(tiles.len);
    Tile **grid = (Tile **)malloc(sizeof(Tile *) * size);
    for (int x = 0; x < size; x++) {
        grid[x] = (Tile *)malloc(sizeof(Tile) * size);
        for (int y = 0; y < size; y++) {
            grid[x][y].id = -1;
        }
    }

    grid[0][0] = *(Tile *)tiles.get(&tiles, 0);
    tiles.remove(&tiles, 0);
    int w = tile_width(&grid[0][0]);
    while (tiles.len > 0) {
        for (int i = 0; i < tiles.len; i++) {
            Tile *tile = (Tile *)tiles.get(&tiles, i);
            bool found;
            for (int x = 0; x < size; x++) {
                for (int y = 0; y < size; y++) {
                    if (grid[x][y].id == -1) {
                        continue;
                    }
                    found = false;
                    for (int flip = 0; flip < 4; flip++) {
                        for (int rot = 0; rot < 4; rot++) {
                            tile->rot = rot;
                            tile->xFlip = flip & 1;
                            tile->yFlip = flip & 2;
                            if (y == 0 || grid[x][y - 1].id == -1) {
                                found = true;
                                for (int d = 0; d < w; d++) {
                                    char a = sample_point(tile, d, w - 1, (rot + 2) % 4, tile->xFlip, tile->yFlip);
                                    char b = sample_point(&grid[x][y], w - d - 1, w - 1, grid[x][y].rot,
                                            grid[x][y].xFlip, grid[x][y].yFlip);
                                    if (a != b) {
                                        found = false;
                                        break;
                                    }
                                }
                                if (found) {
                                    if (y == 0) {
                                        for (int dy = size - 1; dy >= 1; dy--) {
                                            for (int dx = 0; dx < size; dx++) {
                                                grid[dx][dy] = grid[dx][dy - 1];
                                            }
                                        }
                                        for (int dx = 0; dx < size; dx++) {
                                            grid[dx][0].id = -1;
                                        }
                                        y++;
                                    }
                                    grid[x][y - 1] = *tile;
                                    break;
                                }
                            }
                            if (x == 0 || grid[x - 1][y].id == -1) {
                                found = true;
                                for (int d = 0; d < w; d++) {
                                    char a = sample_point(tile, w - 1, d, (rot + 2) % 4, tile->xFlip, tile->yFlip);
                                    char b = sample_point(&grid[x][y], w - 1, w - d - 1,
                                            grid[x][y].rot, grid[x][y].xFlip, grid[x][y].yFlip);
                                    if (a != b) {
                                        found = false;
                                        break;
                                    }
                                }
                                if (found) {
                                    if (x == 0) {
                                        for (int dy = 0; dy < size; dy++) {
                                            for (int dx = size - 1; dx >= 1; dx--) {
                                                grid[dx][dy] = grid[dx - 1][dy];
                                            }
                                        }
                                        for (int dy = 0; dy < size; dy++) {
                                            grid[0][dy].id = -1;
                                        }
                                        x++;
                                    }
                                    grid[x - 1][y] = *tile;
                                    break;
                                }
                            }
                            if (y == size - 1 || grid[x][y + 1].id == -1) {
                                found = true;
                                for (int d = 0; d < w; d++) {
                                    char a = sample_point(tile, d, 0, (rot + 2) % 4, tile->xFlip, tile->yFlip);
                                    char b = sample_point(&grid[x][y], w - d - 1, 0,
                                            grid[x][y].rot, grid[x][y].xFlip, grid[x][y].yFlip);
                                    if (a != b) {
                                        found = false;
                                        break;
                                    }
                                }
                                if (found) {
                                    if (y == size - 1) {
                                        for (int dy = 0; dy < size - 1; dy++) {
                                            for (int dx = 0; dx < size; dx++) {
                                                grid[dx][dy] = grid[dx][dy + 1];
                                            }
                                        }
                                        for (int dx = 0; dx < size; dx++) {
                                            grid[dx][size - 1].id = -1;
                                        }
                                        y--;
                                    }
                                    grid[x][y + 1] = *tile;
                                    break;
                                }
                            }
                            if (x == size - 1 || grid[x + 1][y].id == -1) {
                                found = true;
                                for (int d = 0; d < w; d++) {
                                    char a = sample_point(tile, 0, d, (rot + 2) % 4, tile->xFlip, tile->yFlip);
                                    char b = sample_point(&grid[x][y], 0, w - d - 1,
                                            grid[x][y].rot, grid[x][y].xFlip, grid[x][y].yFlip);
                                    if (a != b) {
                                        found = false;
                                        break;
                                    }
                                }
                                if (found) {
                                    if (x == size - 1) {
                                        for (int dy = 0; dy < size; dy++) {
                                            for (int dx = 0; dx < size - 1; dx++) {
                                                grid[dx][dy] = grid[dx + 1][dy];
                                            }
                                        }
                                        for (int dy = 0; dy < size; dy++) {
                                            grid[size - 1][dy].id = -1;
                                        }
                                        x--;
                                    }
                                    grid[x + 1][y] = *tile;
                                    break;
                                }
                            }
                        }
                        if (found) {
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
            if (found) {
                tiles.remove(&tiles, i);
                i--;
            }
        }
    }

    char **res = (char **)malloc(sizeof(char *) * size * (w - 2));
    for (int i = 0; i < size * (w - 2); i++) {
        res[i] = (char *)malloc(sizeof(char) * (size * (w - 2) + 1));
        res[i][size * (w - 2)] = '\0';
    }

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            for (int dx = 0; dx < w - 2; dx++) {
                for (int dy = 0; dy < w - 2; dy++) {
                    res[x * (w - 2) + dx][y * (w - 2) + dy] = 
                        sample_point(&grid[x][y], dx + 1, dy + 1, (grid[x][y].rot + 2) % 4, grid[x][y].xFlip, grid[x][y].yFlip);
                }
            }
            free_tile(&grid[x][y]);
        }
        free(grid[x]);
    }
    free(grid);
    tiles.free(&tiles);

    size = size * (w - 2);
    int thingCount = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (res[x][y] == '#') {
                thingCount++;
            }
        }
    }

    char monster[3][21] = {
        "                  # ",
        "#    ##    ##    ###",
        " #  #  #  #  #  #   "
    };

    for (int rot = 0; rot < 4; rot++) {
        for (int flip = 0; flip < 4; flip++) {
            int foundMonsters = 0;

            for (int x = 0; x < size - 20; x++) {
                for (int y = 0; y < size - 3; y++) {
                    bool matches = true;
                    for (int dx = 0; dx < 20; dx++) {
                        for (int dy = 0; dy < 20; dy++) {
                            if (monster[dy][dx] == '#' && sample_str(res, x + dx, y + dy, rot, flip & 1, flip & 2) != '#') {
                                matches = false;
                                break;
                            }
                        }
                        if (!matches) {
                            break;
                        }
                    }
                    if (matches) {
                        foundMonsters++;
                    }
                }
            }

            if (foundMonsters > 0) {
                thingCount -= 15 * foundMonsters;
                printf("%d\n", thingCount);
                for (int i = 0; i < size; i++) {
                    free(res[i]);
                }
                free(res);
                return 0;
            }
        }
    }
}
