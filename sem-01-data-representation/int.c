#include <stdio.h>

int main() {
    char x;
    printf("Enter 1-byte integer: ");
    scanf("%hhd", &x);
    printf("x  = %4hhd, %08hhb, %03hhu\n", x, x, x);
    printf("~x = %4hhd, %08hhb, %03hhu\n", ~x, ~x, ~x);
    printf("-x = %4hhd, %08hhb, %03hhu\n", -x, -x, -x);
}
