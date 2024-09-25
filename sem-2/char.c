#include <stdio.h>
#include <stdlib.h>

void dump_binary(char* mem, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%08hhb ", mem[i]);
    }
     printf("\n");
}

void dump_hex(char* mem, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%02hhx ", mem[i]);
    }
    printf("\n");
}

int main() {
    size_t CAP = 13;

    char name[CAP];
    dump_binary(name, CAP);
    
    scanf("%s", name);
    dump_binary(name, CAP);
    dump_hex(name, CAP);

    printf("\"%s\"\n", name);
}

