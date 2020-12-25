#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

typedef struct ListElem {
    int val;
    struct ListElem *next;
    struct ListElem *prevNum;
} ListElem;

int main() {
    int x;
    scanf("%d", &x);

    int len = ceil(log10(x));

    ListElem *last;
    ListElem *cur = NULL;

    for (int i = 0; i < len; i++) {
        ListElem *newElem = (ListElem *)malloc(sizeof(ListElem));
        newElem->val = x % 10;
        x /= 10;
        newElem->next = cur;
        if (cur == NULL) {
            last = newElem;
        }
        cur = newElem;
    }
    ListElem *largest = NULL;
    ListElem *smallest = NULL;
    for (ListElem *a = cur; a != NULL; a = a->next) {
        for (ListElem *b = cur; b != NULL; b = b->next) {
            if (a->val == (b->val - 1 - 1) % len + 1) {
                b->prevNum = a;
            }
        }
        if (a->val == len) {
            largest = a;
        }
        if (a->val == 1) {
            smallest = a;
        }
    }

    for (int i = len; i < 1000000; i++) {
        ListElem *newElem = (ListElem *)malloc(sizeof(ListElem));
        newElem->val = i + 1;
        last->next = newElem;
        last = newElem;
        newElem->prevNum = largest;
        largest = newElem;
    }
    last->next = cur;
    smallest->prevNum = largest;

    for (int i = 0; i < 10000000; i++) {
        ListElem *taken = cur->next;

        cur->next = cur->next->next->next->next;

        ListElem *dest = cur->prevNum;
        while (dest == taken || dest == taken->next || dest == taken->next->next) {
            dest = dest->prevNum;
        }

        taken->next->next->next = dest->next;
        dest->next = taken;

        cur = cur->next;
    }

    while (cur->val != 1) {
        cur = cur->next;
    }
    cur = cur->next;
    printf("%lld\n", (long long)cur->val * cur->next->val);
}
