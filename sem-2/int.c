#include <stdio.h>

void dump_binary(char* mem, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%08hhb ", mem[i]);
    }
    printf("\n");
}

int main() {
    unsigned int y = (1 << 2) + (1 << 8) + (1 << 9);
    printf("%032b\n", y);
    dump_binary((char*)&y, 4);

    printf("\n");

    char x = 10;
    printf("%4hhd, %08hhb\n", x, x);
    printf("%4hhd, %08hhb\n", ~x, ~x);
    printf("%4hhd, %08hhb %4hhu\n", -x, -x, -x);
}
