#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dynarray.h"
#include <limits.h>

const int PREAMBLE = 25;

int main() {
    Dynarr dynarray = Dynarr_create(long long);
    for (int i = 0; i < PREAMBLE; i++) {
        long long x;
        scanf("%lld", &x);
        Dynarr_insert(&dynarray, &x);
    }

    long long invalid = -1;
    long long current;
    while (scanf("%lld", &current) != EOF) {
        bool found = false;
        for (int i = dynarray.len - PREAMBLE; i < dynarray.len - 1; i++) {
            for (int j = i + 1; j < dynarray.len; j++) {
                if (*(long long *)Dynarr_get(&dynarray, i) + *(long long *)Dynarr_get(&dynarray, j) == current) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        if (!found && invalid == -1) {
            invalid = current;
        }

        Dynarr_insert(&dynarray, &current);
    }

    long long sum = 0;
    int i = 0;
    int len = 0;
    while (sum != invalid) {
        if (sum < invalid && i + len < dynarray.len) {
            len++;
            sum += *(long long *)Dynarr_get(&dynarray, i + len - 1);
        } else {
            i++;
            len = 1;
            sum = *(long long *)Dynarr_get(&dynarray, i);
        }
    }

    long long min = LLONG_MAX;
    long long max = LLONG_MIN;
    for (int x = i; x < i + len; x++) {
        long long c = *(long long *)Dynarr_get(&dynarray, x);
        if (c < min) {
            min = c;
        }
        if (c > max) {
            max = c;
        }
    }

    printf("%lld\n", min + max);
}
