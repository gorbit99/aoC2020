#include <stdio.h>
#include <stdbool.h>
#include "../utils/dynstring.h"

typedef struct QueueElem {
    long long val;
    struct QueueElem *next;
} QueueElem;

typedef struct Queue {
    QueueElem *first;
    QueueElem *last;
} Queue;

Queue Queue_create() {
    Queue queue;
    queue.first = NULL;
    queue.last = NULL;
    return queue;
}

void Queue_push(Queue *queue, long long val) {
    if (queue->first == NULL) {
        queue->first = (QueueElem *)malloc(sizeof(QueueElem));
        queue->first->next = NULL;
        queue->first->val = val;
        queue->last = queue->first;
        return;
    }

    queue->last->next = (QueueElem *)malloc(sizeof(QueueElem));
    queue->last->next->next = NULL;
    queue->last->next->val = val;
    queue->last = queue->last->next;
}

long long Queue_front(Queue *queue) {
    return queue->first->val;
}

void Queue_pop(Queue *queue) {
    QueueElem *temp = queue->first->next;
    free(queue->first);
    queue->first = temp;
    if (queue->first == NULL) {
        queue->last = NULL;
    }
}

bool Queue_is_empty(Queue *queue) {
    return queue->first == NULL;
}

int main() {
    Queue p1 = Queue_create();
    Queue p2 = Queue_create();
    int cardCount = 0;

    scanf("%*[^\n]%*c");
    long long n;
    while (scanf("%lld", &n) == 1) {
        Queue_push(&p1, n);
        cardCount++;
    }

    scanf("%*c%*[^\n]%*c");
    while (scanf("%lld", &n) == 1) {
        Queue_push(&p2, n);
        cardCount++;
    }

    while (!Queue_is_empty(&p1) && !Queue_is_empty(&p2)) {
        long long a = Queue_front(&p1);
        long long b = Queue_front(&p2);
        Queue_pop(&p1);
        Queue_pop(&p2);

        if (a > b) {
            Queue_push(&p1, a);
            Queue_push(&p1, b);
        } else {
            Queue_push(&p2, b);
            Queue_push(&p2, a);
        }
    }

    if (Queue_is_empty(&p1)) {
        p1 = p2;
    }

    long long score = 0;
    while (!Queue_is_empty(&p1)) {
        score += cardCount * Queue_front(&p1);
        Queue_pop(&p1);
        cardCount--;
    }

    printf("%lld\n", score);
}
