#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int main() {
    int earliest;
    scanf("%d", &earliest);

    int earliestDep = INT_MAX;
    int earliestBus = 0;
    char buf[100];
    while (scanf("%[^,]%*c", buf) >= 1) {
        if (buf[0] == 'x') {
            continue;
        }
        int busId;
        sscanf(buf, "%d", &busId);
        int dep = earliest + (busId - earliest % busId) % busId;
        if (dep < earliestDep) {
            earliestDep = dep;
            earliestBus = busId;
        }
    }

    printf("%d\n", (earliestDep - earliest) * earliestBus);

    return 0;
}
