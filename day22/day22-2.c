#include <stdio.h>
#include <stdbool.h>
#include "../utils/dynstring.h"
#include "../utils/dynarray.h"

typedef struct QueueElem {
    long long val;
    struct QueueElem *next;
} QueueElem;

typedef struct Queue {
    QueueElem *first;
    QueueElem *last;
    int len;
} Queue;

Queue Queue_create() {
    Queue queue;
    queue.first = NULL;
    queue.last = NULL;
    queue.len = 0;
    return queue;
}

void Queue_push(Queue *queue, long long val) {
    queue->len++;
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
    queue->len--;
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

Queue Queue_copy(Queue *queue, int depth) {
    Queue copy = Queue_create();
    QueueElem *cur = queue->first;
    while (depth != 0 && cur != NULL) {
        Queue_push(&copy, cur->val);
        depth--;
        cur = cur->next;
    }
    return copy;
}

bool Queue_equals(Queue *a, Queue *b) {
    QueueElem *aFront = a->first;
    QueueElem *bFront = b->first;
    while (aFront != NULL && bFront != NULL) {
        if (aFront->val != bFront->val) {
            return false;
        }
        aFront = aFront->next;
        bFront = bFront->next;
    }
    return aFront == NULL && bFront == NULL;
}

void Queue_free(Queue *queue) {
    while(queue->first != NULL) {
        QueueElem *temp = queue->first->next;
        free(queue->first);
        queue->first = temp;
    }
}

typedef struct GameState {
    Queue p1;
    Queue p2;
} GameState;

int recursive_combat(Queue *p1, Queue *p2) {
    static int gamesRunning = 0;
    gamesRunning++;
    int curGame = gamesRunning;
    Dynarr seenStates = Dynarr_create(GameState);
    int round = 1;
    while (!Queue_is_empty(p1) && !Queue_is_empty(p2)) {

        round++;
        for (int i = 0; i < seenStates.len; i++) {
            GameState *state = (GameState *)seenStates.get(&seenStates, i);
            if (Queue_equals(&state->p1, p1) && Queue_equals(&state->p2, p2)) {
                seenStates.free(&seenStates);
                return 1;
            }
        }
        Queue p1Copy = Queue_copy(p1, -1);
        Queue p2Copy = Queue_copy(p2, -1);
        GameState state = {p1Copy, p2Copy};
        seenStates.insert(&seenStates, &state);

        long long a = Queue_front(p1);
        long long b = Queue_front(p2);
    
        Queue_pop(p1);
        Queue_pop(p2);

        if (p1->len >= a && p2->len >= b) {
            Queue newP1Deck = Queue_copy(p1, a);
            Queue newP2Deck = Queue_copy(p2, b);
            if (recursive_combat(&newP1Deck, &newP2Deck) == 1) {
                Queue_push(p1, a);
                Queue_push(p1, b);
            } else {
                Queue_push(p2, b);
                Queue_push(p2, a);
            }
            Queue_free(&newP1Deck);
            Queue_free(&newP2Deck);
        } else {
            if (a > b) {
                Queue_push(p1, a);
                Queue_push(p1, b);
            } else {
                Queue_push(p2, b);
                Queue_push(p2, a);
            }
        }
    }

    seenStates.free(&seenStates);
    if (Queue_is_empty(p1)) {
        return 2;
    }
    return 1;
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

    if (recursive_combat(&p1, &p2) == 2) {
        Queue_free(&p1);
        p1 = p2;
    } else {
        Queue_free(&p2);
    }

    long long score = 0;
    while (!Queue_is_empty(&p1)) {
        score += cardCount * Queue_front(&p1);
        Queue_pop(&p1);
        cardCount--;
    }

    printf("%lld\n", score);
}
