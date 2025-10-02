#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const size_t CAP = 13;

int main() {
    char str[CAP];
    dump_binary(str, CAP);
    
    scanf("%s", str);
    dump_binary(str, CAP);
    dump_hex(str, CAP);

    printf("\"%s\"\n", str);
    char* pos = strstr(str, "При");
    printf("strlen = %ld\n", strlen(str));
    printf("startswith При: %d\n", pos != NULL && pos == str);
}

