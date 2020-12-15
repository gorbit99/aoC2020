#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct MemoryCell {
    long long value;
    struct MemoryCell *left;
    struct MemoryCell *right;
    bool isFloating;
    bool leaf;
} MemoryCell;

void copy(MemoryCell **to, MemoryCell *from) {
    if (from == NULL) {
        *to = NULL;
        return;
    }

    *to = (MemoryCell *)malloc(sizeof(MemoryCell));
    (*to)->isFloating = from->isFloating;
    (*to)->leaf = from->leaf;
    (*to)->value = from->value;
    copy(&(*to)->left, from->left);
    copy(&(*to)->right, from->right);
}

void insert(MemoryCell **memory, char *address, long long value) {
    if (*memory == NULL) {
        *memory = (MemoryCell *)malloc(sizeof(MemoryCell));
        (*memory)->isFloating = *address == 'X';
        (*memory)->leaf = false;
        (*memory)->left = NULL;
        (*memory)->right = NULL;
    }
    if (*address == '\0') {
        (*memory)->value = value;
        (*memory)->leaf = true;
        return;
    }
    switch(*address) {
        case '0':
            if ((*memory)->isFloating) {
                copy(&(*memory)->right, (*memory)->left);
            }
            insert(&(*memory)->left, address + 1, value);
            (*memory)->isFloating = false;
            break;
        case '1':
            if ((*memory)->isFloating) {
                copy(&(*memory)->right, (*memory)->left);
            }
            insert(&(*memory)->right, address + 1, value);
            (*memory)->isFloating = false;
            break;
        case 'X':
            if ((*memory)->isFloating) {
                insert(&(*memory)->left, address + 1, value);
            } else {
                insert(&(*memory)->left, address + 1, value);
                insert(&(*memory)->right, address + 1, value);
            }
            break;
    }
}

char *applyMask(long long address, char *mask) {
    char *result = (char *)malloc(sizeof(char) * 37);
    result[36] = '\0';
    for (int i = 0; i < 36; i++) {
        switch(mask[35 - i]) {
            case '0':
                result[35 - i] = (address & 1) + '0';
                break;
            case '1':
                result[35 - i] = '1';
                break;
            case 'X':
                result[35 - i] = 'X';
                break;
        }
        address >>= 1;
    }
    return result;
}

long long sumMemory(MemoryCell *memory) {
    if (memory == NULL) {
        return 0;
    }
    if (memory->leaf) {
        return memory->value;
    }
    if (memory->isFloating) {
        return sumMemory(memory->left) * 2;
    }
    return sumMemory(memory->left) + sumMemory(memory->right);
}

void freeMemory(MemoryCell *memory) {
    if (memory == NULL) {
        return;
    }
    freeMemory(memory->left);
    freeMemory(memory->right);
    free(memory);
}

int main() {
    char buf[4];
    char mask[37] = "";
    MemoryCell *memory = NULL;
    while (scanf("%3s", buf) != EOF) {
        if (strcmp(buf, "mem") == 0) {
            long long address;
            long long value;
            scanf("[%lld] = %lld", &address, &value);
            char *applied = applyMask(address, mask);
            insert(&memory, applied, value);
            free(applied);
        } else {
            scanf("k = %36s", mask);
        }
    }

    printf("%lld\n", sumMemory(memory));

    freeMemory(memory);
}
