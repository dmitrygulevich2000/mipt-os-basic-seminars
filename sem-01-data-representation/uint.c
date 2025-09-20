#include <stdio.h>

void dump_binary(char* mem, size_t n) {
    for (int i = 0; i < n; ++i) {
        printf("%08hhb ", mem[i]);
    }
    printf("\n");
}

int main() {
    unsigned int y = (1 << 2) + (1 << 8) + (1 << 9);
    printf("%d\n", y);
    printf("%032b\n", y);
    dump_binary((char*)&y, 4);

    unsigned short z = (unsigned short)y;
    printf("%016b\n", z);
    dump_binary((char*)&z, 2);
}
