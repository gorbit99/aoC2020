#include "../utils/dynstring.h"
#include "../utils/dynarray.h"
#include <stdbool.h>

char get_chair(Dynarr input, int x, int y) {
    return ((String *)Dynarr_get(&input, x))->data[y];
}

void set_chair(Dynarr input, int x, int y, char c) {
    ((String *)Dynarr_get(&input, x))->data[y] = c;
}

int count_neighbours(Dynarr input, int x, int y) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            if (x + dx < 0 || x + dx >= input.len
                    || y + dy < 0 || y + dy >= ((String *)Dynarr_get(&input, 0))->len) {
                continue;
            }
            if (get_chair(input, x + dx, y + dy) == '#') {
                count++;
            }
        }
    }
    return count;
}

int main() {
    Dynarr input = Dynarr_create(String);
    String curLine;
    while ((curLine = String_readline()).len > 0) {
        Dynarr_insert(&input, &curLine);
    }

    bool changed = true;
    while (changed) {
        Dynarr copy = Dynarr_create(String);
        for (int i = 0; i < input.len; i++) {
            String strcopy = String_copy((String *)Dynarr_get(&input, i));
            Dynarr_insert(&copy, &strcopy);
        }
        changed = false;
        for (int x = 0; x < input.len; x++) {
            for (int y = 0; y < ((String *)Dynarr_get(&input, 0))->len; y++) {
                char chair = get_chair(input, x, y);
                if (chair == '.') {
                    continue;
                }
                int n = count_neighbours(input, x, y);
                if (chair == 'L' && n == 0) {
                    changed = true;
                    set_chair(copy, x, y, '#');
                } else if (chair == '#' && n >= 4) {
                    changed = true;
                    set_chair(copy, x, y, 'L');
                }
            }
        }
        Dynarr_free(&input);
        input = copy;
    }

    int c = 0;
    for (int x = 0; x < input.len; x++) {
        for (int y = 0; y < ((String *)Dynarr_get(&input, 0))->len; y++) {
            if (get_chair(input, x, y) == '#') {
                c++;
            }
        }
    }
    printf("%d\n", c);

    Dynarr_execute(&input, (void(*)(void *))String_free);
    Dynarr_free(&input);
}
