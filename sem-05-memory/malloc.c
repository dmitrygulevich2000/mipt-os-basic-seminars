#include <stdio.h>
#include <stdlib.h>

int main() {
    // fill available heap
    void* mem1 = malloc(127 * 1024);

    fgetc(stdin);

    // interesting call
    // try change size to 128 KB
    void* mem2 = malloc(127 * 1024);
}
