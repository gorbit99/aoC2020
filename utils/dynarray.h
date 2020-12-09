#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>
#include <memory.h>

#define Dynarr_create(type) __Dynarr_create(sizeof(type))

typedef struct Dynarr {
    void *data;
    int len;
    int capacity;
    int elemSize;
} Dynarr;

Dynarr __Dynarr_create(int elemSize) {
    Dynarr result;
    result.capacity = 0;
    result.len = 0;
    result.data = NULL;
    result.elemSize = elemSize;
    return result;
}

void Dynarr_insert(Dynarr *dynarr, void *elem) {
    if (dynarr->capacity == dynarr->len) {
        if (dynarr->capacity == 0) {
            dynarr->capacity = 1;
        } else {
            dynarr->capacity *= 2;
        }
        dynarr->data = (void *)realloc(dynarr->data, dynarr->elemSize * dynarr->capacity);
    }
    memcpy((char *)dynarr->data + dynarr->elemSize * dynarr->len, elem, dynarr->elemSize);
    dynarr->len++;
}

void Dynarr_remove(Dynarr *dynarr, int index) {
    memmove((char *)dynarr->data + dynarr->elemSize * index,
            (char *)dynarr->data + dynarr->elemSize * (index + 1),
            dynarr->elemSize * (dynarr->len - index - 1));
}

void Dynarr_free(Dynarr *dynarr) {
    free(dynarr->data);
}

void *Dynarr_get(Dynarr *dynarr, int index) {
    return (void *)((char *)dynarr->data + index * dynarr->elemSize);
}

#endif
