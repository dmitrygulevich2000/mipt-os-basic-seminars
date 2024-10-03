#include<stdio.h>

int sum_result = 1000;

extern void array_sum();
// extern int array_sum(int*, int);

int main() {
    // int arr[] = {1, 2, 3};

    array_sum();
    printf("%d\n", sum_result);
}
