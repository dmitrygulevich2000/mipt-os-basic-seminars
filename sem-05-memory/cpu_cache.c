#include <err.h>
#include <inttypes.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    uint64_t value;
    // char padding[56];
} thick_uint;

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "usage: ./a.out arr_size\n");
    }
    srandom(0);

    printf("sizeof(thick_uint)  = %lu\n", sizeof(thick_uint));
    printf("alignof(thick_uint) = %lu\n", alignof(thick_uint));

    size_t size = strtoul(argv[1], NULL, 10);
    thick_uint* arr = malloc(size * 128);

    // printf("generating random array of size %lu\n", size);
    // for (size_t i = 0; i < size; ++i) {
    //     *(uint64_t*)(arr + i) = (uint64_t)random() % 1000;
    // }

    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    uint64_t sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += arr[i].value;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t elapsed_mcsec =
        1000 * (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1000000;
    printf("elapsed time: %'" PRIu64 " ms\n", elapsed_mcsec);

    printf("sum = %lu\n", sum);
}