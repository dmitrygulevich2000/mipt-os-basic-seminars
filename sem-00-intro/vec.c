#include <stdio.h>
#include <stdlib.h>

typedef struct vec {
    int* data;
    size_t size;
} vec_t;

vec_t zeros_vec(size_t size) {
    vec_t res = {
        .data = calloc(size, sizeof(int)),
        .size = size,
    };
    return res;
}

void print_vec(vec_t* vec) {
    if (vec->size == 0) {
        printf("[]\n");
        return;
    }

    printf("[");
    printf("%d", vec->data[0]);

    for (size_t i = 0; i < vec->size; ++i) {
        printf(", %d", vec->data[i + 1]);
    }
    printf("]\n");
}

void free_vec(vec_t* vec) { free(vec->data); }

int main() {
    vec_t vec1 = zeros_vec(0);
    print_vec(&vec1);

    vec_t vec2 = zeros_vec(3);
    vec2.data[0] = 1;
    vec2.data[1] = 2;
    vec2.data[2] = 3;
    print_vec(&vec2);

    return 0;
}
