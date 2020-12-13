#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int x = 0;
    int y = 0;
    int angle = 3;

    char c;
    int amount;
    while (scanf("%c%d%*c", &c, &amount) != EOF) {
        switch (c) {
            case 'N': 
                y += amount;
                break;
            case 'S':
                y -= amount;
                break;
            case 'E':
                x += amount;
                break;
            case 'W':
                x -= amount;
                break;
            case 'L':
                angle = (angle + amount / 90) % 4;
                break;
            case 'R':
                angle = (angle - amount / 90 + 4) % 4;
                break;
            case 'F':
                switch (angle) {
                    case 0:
                        y += amount;
                        break;
                    case 1:
                        x -= amount;
                        break;
                    case 2:
                        y -= amount;
                        break;
                    case 3:
                        x += amount;
                        break;
                }
                break;
        }
    }

    printf("%d\n", abs(x) + abs(y));
}
