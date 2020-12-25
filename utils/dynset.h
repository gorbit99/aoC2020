#ifndef DYNSET_H
#define DYNSET_H

#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#define Dynset_create(type, cmp) __Dynset_create(sizeof(type, cmp))

typedef struct Dynset {
    void *data;
    size_t elemSize;
    size_t capacity;
    size_t len;
    int (*cmp)(void *a, void *b);
    void (*insert(Dynset *dynset, void *elem));
    void *(*contains(Dynset *dynset, void *elem));
    bool (*is_empty(Dynset *dynset));
} Dynset;

Dynset __Dynset_create(size_t elemSize, int (*cmp(void *a, void *b))) {
    Dynset dynset;
    dynset.elemSize = elemSize;
    dynset.capacity = 0;
    dynset.len = 0;
    dynset.data = NULL;
    return dynset;
}

void Dynset_insert(Dynset *dynset, void *elem) {
}

void *Dynset_contains(Dynset *dynset, void *elem) {
}

bool Dynset_is_empty(Dynset *dynset) {
}

Dynset Dynset_union(Dynset *a, Dynset *b) {
}

Dynset Dynset_intersection(Dynset *a, Dynset *b) {
}

Dynset Dynset_difference(Dynset *a, Dynset *b) {
}

void Dynset_remove()

#endif
