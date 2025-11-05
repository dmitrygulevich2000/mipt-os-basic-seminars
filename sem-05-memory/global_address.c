#include <stdio.h>

int n = 10;

int main() {
    printf("&n = %p\n", &n);
    fgetc(stdin);
    n += 1;
    printf("n  = %d\n", n);
}
