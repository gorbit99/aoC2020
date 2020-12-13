#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int x = 0;
    int y = 0;
    int wx = 10;
    int wy = 1;

    char c;
    int amount;
    while (scanf("%c%d%*c", &c, &amount) != EOF) {
        switch (c) {
            case 'N': 
                wy += amount;
                break;
            case 'S':
                wy -= amount;
                break;
            case 'E':
                wx += amount;
                break;
            case 'W':
                wx -= amount;
                break;
            case 'L':
                switch (amount) {
                    case 90:
                    {
                        int t = -wy;
                        wy = wx;
                        wx = t;
                        break;
                    }
                    case 180:
                    {
                        wx = -wx;
                        wy = -wy;
                        break;
                    }
                    case 270:
                    {
                        int t = -wx;
                        wx = wy;
                        wy = t;
                        break;
                    }
                }
                break;
            case 'R':
                switch (amount) {
                    case 270:
                    {
                        int t = -wy;
                        wy = wx;
                        wx = t;
                        break;
                    }
                    case 180:
                    {
                        wx = -wx;
                        wy = -wy;
                        break;
                    }
                    case 90:
                    {
                        int t = -wx;
                        wx = wy;
                        wy = t;
                        break;
                    }
                }
                break;
            case 'F':
                x += wx * amount;
                y += wy * amount;
                break;
        }
    }

    printf("%d\n", abs(x) + abs(y));
}
