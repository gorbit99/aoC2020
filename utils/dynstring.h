#ifndef DYNSTRING_H
#define DYNSTRING_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct String {
    char *data;
    size_t len;
} String;

String string(char *str) {
    String result;
    result.len = strlen(str);
    result.data = (char *)malloc(sizeof(char) * (result.len + 1));
    return result;
}

String readline() {
    char *buf = (char *)malloc(sizeof(char) * 1);
    int capacity = 1;
    int count = 0;
    int c;
    while ((c = getchar()) != '\n' && c != -1) {
        if (capacity == count + 1) {
            capacity *= 2;
            buf = (char *)realloc(buf, sizeof(char) * capacity);
        }
        buf[count] = c;
        count++;
    }
    buf[count] = '\0';
    String result;
    result.data = buf;
    result.len = count;
    return result;
}

int scanString(String str, char *format, ...) {
    va_list args;
    va_start(args, format);
    return vsscanf(str.data, format, args);
    va_end(args);
}

String subString(String str, int begin, int len) {
    if (begin + len > str.len) {
        len = str.len - begin;
    }
    String result;
    result.len = len;
    result.data = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(result.data, str.data + begin, len);
    return result;
}

void freeString(String str) {
    free(str.data);
}

void printString(String str) {
    printf("%s", str.data);
}

void concatString(String a, String b) {
    a.data = (char *)realloc(a.data, sizeof(char) * (a.len + b.len + 1));
    strcat(a.data, b.data);
    a.len += b.len;
}

String copyString(String str) {
    return string(str.data);
}

#endif
