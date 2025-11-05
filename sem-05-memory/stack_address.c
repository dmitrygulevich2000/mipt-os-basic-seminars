#include <stdio.h>

int main() {
    int n = 10;
    printf("%p ~ %ld GB\n", &n, (size_t)&n / 1024 / 1024 / 1024);
}
