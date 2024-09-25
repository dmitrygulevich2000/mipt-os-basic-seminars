#include <stdio.h>
#include <unistd.h>

int main() {
    char s[4];
    char n[6];    

    scanf("%4c", s);
    scanf(" ");
    scanf("%6c", n);
    
    printf("%.4s", s);
    printf(" ");
    printf("%.6s", n);
    printf("\n");
    printf("exiting...\n");

    return 0;
}

