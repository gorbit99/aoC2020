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
    dynarr->len--;
}

void Dynarr_free(Dynarr *dynarr) {
    free(dynarr->data);
}

void *Dynarr_get(Dynarr *dynarr, int index) {
    return (void *)((char *)dynarr->data + index * dynarr->elemSize);
}

void Dynarr_sort(Dynarr *dynarr, int (*cmp)(const void *a, const void *b)) {
    qsort(dynarr->data, dynarr->len, dynarr->elemSize, cmp);
}

void Dynarr_execute(Dynarr *dynarr, void (*fn)(void *)) {
    for (int i = 0; i < dynarr->len; i++) {
        fn((void *)((char *)dynarr->data + i * dynarr->elemSize));
    }
}

void *Dynarr_top(Dynarr *dynarr) {
    return Dynarr_get(dynarr, dynarr->len - 1);
}

void Dynarr_pop(Dynarr *dynarr) {
    Dynarr_remove(dynarr, dynarr->len - 1);
}

void Dynarr_push(Dynarr *dynarr, void *elem) {
    Dynarr_insert(dynarr, elem);
}

Dynarr Dynarr_deep_copy(Dynarr *original) {
    Dynarr copy;
    copy.elemSize = original->elemSize;
    copy.len = original->len;
    copy.data = malloc(copy.len * copy.elemSize);
    memcpy(copy.data, original->data, copy.len * copy.elemSize);
    copy.capacity = copy.len;
    return copy;
}

#endif
