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
    void (*insert)(struct Dynarr *dynarr, void *elem);
    void (*remove)(struct Dynarr *dynarr, int index);
    void (*free)(struct Dynarr *dynarr);
    void *(*get)(struct Dynarr *dynarr, int index);
    void (*sort)(struct Dynarr *dynarr, int (*cmp)(const void *a, const void *b));
    void (*execute)(struct Dynarr *dynarr, void (*fn)(void *));
    void *(*top)(struct Dynarr *dynarr);
    void (*pop)(struct Dynarr *dynarr);
    void (*push)(struct Dynarr *dynarr, void *elem);
} Dynarr;

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

Dynarr __Dynarr_create(int elemSize) {
    Dynarr result;
    result.capacity = 0;
    result.len = 0;
    result.data = NULL;
    result.elemSize = elemSize;
    result.insert = Dynarr_insert;
    result.remove = Dynarr_remove;
    result.free = Dynarr_free;
    result.get = Dynarr_get;
    result.sort = Dynarr_sort;
    result.execute = Dynarr_execute;
    result.top = Dynarr_top;
    result.pop = Dynarr_pop;
    result.push = Dynarr_push;
    return result;
}

#endif
