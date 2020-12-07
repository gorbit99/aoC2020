#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct String {
    char *str;
    int len;
} String;

struct Bag;

typedef struct HeldBag {
    struct Bag *bag;
    int num;

} HeldBag;

typedef struct Bag {
    String color;
    int heldCount;
    struct HeldBag *held;
} Bag;

String readLine() {
    char *buf = malloc(sizeof(char) * 1);
    int capacity = 1;
    int count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != -1) {
        if (count == capacity) {
            capacity *= 2;
            buf = (char *)realloc(buf, sizeof(char) * capacity);
        }
        buf[count] = c;
        count++;
    }
    if (capacity == count) {
        buf = (char *)realloc(buf, sizeof(char) * (count + 1));
    }
    buf[count] = '\0';
    return (String){buf, count};
}

String string(char *str) {
    String res;
    res.len = strlen(str);
    res.str = (char *)malloc(sizeof(char) * (res.len + 1));
    strcpy(res.str, str);
    return res;
}

bool canContainGold(Bag *bag) {
    if (strcmp(bag->color.str, "shiny gold") == 0) {
        return true;
    }

    for (int i = 0; i < bag->heldCount; i++) {
        if (canContainGold(bag->held[i].bag)) {
            return true;
        }
    }
    return false;
}

int main() {
    String *input = (String *)malloc(sizeof(String));
    int inputCount = 0;
    int inputCap = 1;
    for (String str = readLine(); str.len != 0; str = readLine()) {
        if (inputCap == inputCount) {
            inputCap *= 2;
            input = (String *)realloc(input, sizeof(String) * inputCap);
        }
        input[inputCount] = str;
        inputCount++;
    }

    Bag *bags = (Bag *)malloc(sizeof(Bag) * inputCount);
    for (int i = 0; i < inputCount; i++) {
        char adj[50];
        char color[50];
        sscanf(input[i].str, "%s %s", adj, color);
        strcat(adj, " ");
        strcat(adj, color);
        bags[i].color = string(adj);
        bags[i].held = NULL;
        bags[i].heldCount = 0;
    }

    for (int i = 0; i < inputCount; i++) {
        int n;
        sscanf(input[i].str, "%*s %*s %*s %*s %n", &n);
        char *left = input[i].str + n;
        while (left[0] != '\0' && left[0] != 'n') {
            if (left[0] == ',') {
                left += 2;
            }
            int num;
            char adj[50];
            char color[50];
            sscanf(left, "%d %s %s %*s%n", &num, adj, color, &n);
            strcat(adj, " ");
            strcat(adj, color);
            Bag *held = NULL;
            for (int j = 0; j < inputCount; j++) {
                if (strcmp(bags[j].color.str, adj) == 0) {
                    held = &bags[j];
                    break;
                }
            }
            bags[i].held = (HeldBag *)realloc(bags[i].held, sizeof(HeldBag) * (bags[i].heldCount + 1));
            bags[i].held[bags[i].heldCount] = (HeldBag){held, num};
            bags[i].heldCount++;
            left += n;
        }
    }

    int result = 0;
    for (int i = 0; i < inputCount; i++) {
        if (canContainGold(&bags[i])) {
            result++;
        }
    }
    printf("%d\n", result - 1);
}
