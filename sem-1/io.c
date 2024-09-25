#include <stdio.h>
#include <stdlib.h>

void print_array(int* arr, int size) {    
    printf("[ ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

int main() {
    int n = 0;
    printf("Enter positive integer n: ");
    scanf("%d", &n);
    printf("n is %d, %#o, %x\n\n", n, n, n);


    float x = 0;
    printf("Enter floating point number x: ");
    scanf("%f", &x);
    printf("x is %.3f, %e\n\n", x, x); // .m precision


    char name[10];
    char surname[10];
    printf("Enter your name and surname: ");
    scanf("%s %s", name, surname);
    printf("Hello, %s %s!\n\n", name, surname);

    // new, delete[]
    int size;
    printf("Enter size of array and it's elements: ");
    scanf("%d", &size);
    int* array = calloc(size, sizeof(int));
    for (int i = i = 0; i < size; ++i) {
        scanf("%d", &array[i]);
    }
    print_array(array, size);
    free(array);


    printf("\n");
    char* line = NULL;
    size_t linecap = 0;
    printf("Enter any line:\n");
    char c = fgetc(stdin);
    getline(&line, &linecap, stdin);
    printf("(reading char with ascii code %hhd)\n", c);
    printf("Your line is \"%s\" ", line);
    printf("and located on address %p\n", line);
    free(line);

    return 0;
}
