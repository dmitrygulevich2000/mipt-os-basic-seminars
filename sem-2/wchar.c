#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

void dump_int(unsigned int* mem, size_t count) {
    for (int i = 0; i < count; ++i) {
        printf("%u ", mem[i]);
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "C.UTF-8");

    printf("sizeof(wchar_t) = %ld\n", sizeof(wchar_t));
    printf("fwide(stdin) = %d\n", fwide(stdin, 0));

    wchar_t* wstr = calloc(10, sizeof(wchar_t));
    scanf("%ls ", wstr);
    //wchar_t* wstr = L"Привет";

    printf("wsclen(wstr) = %zd\n", wcslen(wstr));
    dump_int(wstr, 10);
    printf("\"%ls\"\n", wstr);
    
    free(wstr);
}
