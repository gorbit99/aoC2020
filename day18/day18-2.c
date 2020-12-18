#include <stdio.h>
#include <stdbool.h>
#include "../utils/dynstring.h"
#include "../utils/dynarray.h"

typedef struct Part {
    enum PartType {
        OPERAND,
        OPERATOR,
        PAREN
    } type;

    union {
        long long num;
        char op;
    } value;
} Part;

typedef struct QueueElem {
    Part part;
    struct QueueElem *next;
} QueueElem;

typedef struct Queue {
    QueueElem *first;
    QueueElem *last;
} Queue;

void Queue_push(Queue *queue, Part elem) {
    if (queue->first == NULL) {
        queue->first = (QueueElem *)malloc(sizeof(QueueElem));
        queue->first->next = NULL;
        queue->first->part = elem;
        queue->last = queue->first;
        return;
    }
    queue->last->next = (QueueElem *)malloc(sizeof(QueueElem));
    queue->last = queue->last->next;
    queue->last->part = elem;
    queue->last->next = NULL;
}

Queue Queue_create() {
    Queue newQueue = {NULL, NULL};
    return newQueue;
}

Part Queue_pop(Queue *queue) {
    Part elem = queue->first->part;
    QueueElem *temp = queue->first->next;
    free(queue->first);
    queue->first = temp;
    return elem;
}

Part Queue_front(Queue *queue) {
    return queue->first->part;
}

bool Queue_is_empty(Queue *queue) {
    return queue->first == NULL;
}

Part read_token(char **str) {
    int n = -1;
    Part part;
    if (**str == '(' || **str == ')') {
        part.type = PAREN;
        part.value.op = **str;
        (*str)++;
    }
    else if (strchr("*+", **str)) {
        part.type = OPERATOR;
        part.value.op = **str;
        (*str)++;
    } else {
        long long val;
        sscanf(*str, "%lld%n", &val, &n);
        *str += n;
        Part part;
        part.type = OPERAND;
        part.value.num = val;
    }

    n = -1;
    sscanf(*str, "%*[ ]%n", &n);
    if (n != -1) {
        *str += n;
    }

    return part;
}

long long evaluate(char *str) {
    Queue output = Queue_create();
    Dynarr opStack = Dynarr_create(Part);
    while (*str != '\0') {
        Part token = read_token(&str);
        switch (token.type) {
            case OPERAND:
                Queue_push(&output, token);
                break;
            case OPERATOR:
                while (opStack.len > 0) {
                    Part op = *(Part *)Dynarr_top(&opStack);
                    if (op.type != OPERATOR || !(token.value.op == '*' && op.value.op == '+')) {
                        break;
                    }
                    Dynarr_pop(&opStack);
                    Queue_push(&output, op);
                }
                Dynarr_push(&opStack, &token);
                break;
            case PAREN:
                if (token.value.op == '(') {
                    Dynarr_push(&opStack, &token);
                } else {
                    Part op;
                    while ((op = *(Part *)Dynarr_top(&opStack)).type != PAREN) {
                        Dynarr_pop(&opStack);
                        Queue_push(&output, op);
                    }
                    Dynarr_pop(&opStack);
                }
                break;
        }
    }

    while (opStack.len > 0) {
        Part op = *(Part *)Dynarr_top(&opStack);
        Dynarr_pop(&opStack);
        Queue_push(&output, op);
    }

    Dynarr_free(&opStack);
    Dynarr argStack = Dynarr_create(long long);
    while (!Queue_is_empty(&output)) {
        Part top = Queue_front(&output);
        Queue_pop(&output);

        if (top.type == OPERAND) {
            Dynarr_push(&argStack, &top.value.num);
        } else {
            long long a = *(long long *)Dynarr_top(&argStack);
            Dynarr_pop(&argStack);
            long long b = *(long long *)Dynarr_top(&argStack);
            Dynarr_pop(&argStack);

            if (top.value.op == '+') {
                a += b;
            } else {
                a *= b;
            }
            Dynarr_push(&argStack, &a);
        }
    }

    long long res = *(long long *)Dynarr_top(&argStack);
    Dynarr_free(&argStack);
    return res;
}

int main() {
    long long sum = 0;
    String line;
    while ((line = String_readline()).len > 0) {
        sum += evaluate(line.data);
        String_free(&line);
    }
    String_free(&line);
    printf("%lld\n", sum);
}
