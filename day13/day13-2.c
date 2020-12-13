#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

//ax + b = c (mod m)
long long solveCongruence(long long a, long long b, long long c, long long m) {
    c -= b;
    c = (c % m + m) % m;
    if (c % a != 0) {
        long long mult = solveCongruence(m, 0, a - c, a);
        c += m * mult;
    }
    c /= a;
    return c;
}

int main() {
    scanf("%*d");
    long long currentMult = 1;
    long long currentAdd = 0;

    char buf[100];
    long long i = 0;
    while (scanf("%[^,]%*c", buf) != EOF) {
        if (buf[0] == 'x') {
            i++;
            continue;
        }
        long long busId;
        sscanf(buf, "%lld", &busId);

        long long res = solveCongruence(currentMult, currentAdd, busId - i, busId);

        currentAdd += res * currentMult;
        currentMult *= busId;

        i++;
    }

    printf("%lld\n", currentAdd);

    return 0;
}
