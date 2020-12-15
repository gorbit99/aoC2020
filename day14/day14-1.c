#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryAddress {
    long long address;
    long long value;
    struct MemoryAddress *next;
} MemoryAddress;

void change(MemoryAddress **list, long long address, long long value) {
    if (*list == NULL) {
        *list = (MemoryAddress *)malloc(sizeof(MemoryAddress));
        (*list)->address = address;
        (*list)->value = value;
        (*list)->next = NULL;
        return;
    }

    MemoryAddress *current = *list;

    while (current != NULL) {
        if (current->address == address) {
            current->value = value;
            return;
        }
        if (current->next == NULL || current->next->address > address) {
            MemoryAddress *newAddress = (MemoryAddress *)malloc(sizeof(MemoryAddress));
            newAddress->address = address;
            newAddress->value = value;
            newAddress->next = current->next;
            current->next = newAddress;
            return;
        }

        current = current->next;
    }
}

void freeMemory(MemoryAddress *list) {
    while (list != NULL) {
        MemoryAddress *temp = list->next;
        free(list);
        list = temp;
    }
}

void parseMask(char *buf, long long *andMask, long long *orMask) {
    *orMask = 0;
    *andMask = 0;
    while (*buf != '\0') {
        *orMask *= 2;
        *andMask *= 2;
        switch (*buf) {
            case 'X':
                *andMask |= 1;
                break;
            case '0':
                break;
            case '1':
                *orMask |= 1;
                break;
        }
        buf++;
    }
}

long long sumMemory(MemoryAddress *memory) {
    long long sum = 0;
    while (memory != NULL) {
        sum += memory->value;
        memory = memory->next;
    }
    return sum;
}

int main() {
    long long orMask = 0;
    long long andMask = ~(0ll);

    MemoryAddress *memory = NULL;
    
    char buf[4];
    while (scanf("%3s", buf) != EOF) {
        if (strcmp(buf, "mem") == 0) {
            long long address;
            long long value;
            scanf("[%lld] = %lld%*c", &address, &value);
            value &= andMask;
            value |= orMask;
            change(&memory, address, value);
        } else {
            char mask[37];
            scanf("k = %[^\n]%*c", mask);
            parseMask(mask, &andMask, &orMask);
        }
    }

    printf("%lld\n", sumMemory(memory));

    freeMemory(memory);
}
