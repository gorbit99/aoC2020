#include <stdio.h>
#include <stdbool.h>
#include "../utils/dynstring.h"
#include "../utils/dynarray.h"

typedef struct Position {
    int q;
    int r;
} Position;

int main() {
    String line;
    Dynarr seen = Dynarr_create(Position);
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
        bool found = false;
        for (int i = 0; i < seen.len; i++) {
            Position *pos = (Position *)seen.get(&seen, i);
            if (pos->q == q && pos->r == r) {
                seen.remove(&seen, i);
                found = true;
                break;
            }
        }

        if (!found) {
            Position pos = {q, r};
            seen.insert(&seen, &pos);
        }

        String_free(&line);
    }
    String_free(&line);

    printf("%d\n", seen.len);
    seen.free(&seen);
}
