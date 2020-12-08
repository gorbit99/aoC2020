#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Instruction {
    char opcode[4];
    int num;
} Instruction;

int main() {
    Instruction *instructions = (Instruction *)malloc(sizeof(Instruction) * 1);
    int instrCap = 1;
    int instrCount = 0;
    while (scanf("%3s %d\n", instructions[instrCount].opcode, &instructions[instrCount].num) != EOF) {
        instrCount++;
        if (instrCap == instrCount) {
            instrCap *= 2;
            instructions = (Instruction *)realloc(instructions, sizeof(Instruction) * instrCap);
        }
    }

    for (int changed = 0; changed < instrCount; changed++) {
        if (strcmp(instructions[changed].opcode, "acc") == 0) {
            continue;
        }
        if (strcmp(instructions[changed].opcode, "nop") == 0) {
            strcpy(instructions[changed].opcode, "jmp");
        } else {
            strcpy(instructions[changed].opcode, "nop");
        }

        bool *seen = (bool *)malloc(sizeof(bool) * instrCount);
        for (int i = 0; i < instrCount; i++) {
            seen[i] = false;
        }
        int ip = 0;
        int acc = 0;
        bool good = false;
        while (!seen[ip]) {
            seen[ip] = true;
            if (strcmp(instructions[ip].opcode, "acc") == 0) {
                acc += instructions[ip].num;
                ip++;
            } else if (strcmp(instructions[ip].opcode, "jmp") == 0) {
                ip += instructions[ip].num;
            } else {
                ip++;
            }

            if (ip == instrCount) {
                good = true;
                printf("%d\n", acc);
                break;
            }
        }

        free(seen);
        if (good) {
            break;
        }
        if (strcmp(instructions[changed].opcode, "nop") == 0) {
            strcpy(instructions[changed].opcode, "jmp");
        } else {
            strcpy(instructions[changed].opcode, "nop");
        }
    }

    free(instructions);
}
