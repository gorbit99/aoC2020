#include <stdio.h>
#include "../utils/dynstring.h"

void skip_whitespace(char **str) {
    int n = -1;
    sscanf(*str, "%*[ \t\n]%n", &n);
    if (n != -1) {
        *str += n;
    }
}

long long evaluate(char **str) {
    int n;
    long long res;
    if (**str == '(') {
        (*str)++;
        res = evaluate(str);
    } else {
        sscanf(*str, "%lld%n", &res, &n);
        *str += n;
    }

    skip_whitespace(str);

    while (**str != ')' && **str != '\0') {
        char op;
        sscanf(*str, "%c", &op);
        (*str)++;
        skip_whitespace(str);
        long long arg;
        if (**str == '(') {
            (*str)++;
            arg = evaluate(str);
        } else {
            sscanf(*str, "%lld%n", &arg, &n);
            *str += n;
        }
        switch(op) {
            case '+':
                res += arg;
                break;
            case '*':
                res *= arg;
                break;
        }
        skip_whitespace(str);
    }
    (*str)++;
    return res;
}

int main() {
    long long sum = 0;
    String line;
    while ((line = String_readline()).len > 0) {
        char *str = line.data;
        sum += evaluate(&str);
    }


    printf("%lld\n", sum);

    return 0;
}
