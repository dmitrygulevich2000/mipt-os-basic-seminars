#include <err.h>
#include <inttypes.h>
#include <limits.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const long MY_RAND_MAX = 1 << 16;

float random_float(float a, float b) {
    long r = random() % MY_RAND_MAX;
    // shift + scale * random()/LONG_MAX
    return a + (float)r / ((float)(MY_RAND_MAX) / (b - a));
}

float** random_matrix(size_t n) {
    float** rows = malloc(n * sizeof(float*));
    for (size_t i = 0; i < n; ++i) {
        rows[i] = malloc(n * sizeof(float));
        for (size_t j = 0; j < n; ++j) {
            rows[i][j] = random_float(-5, 5);
        }
    }
    return rows;
}

float** mat_mul(float** a, float** b, size_t n) {
    float** rows = malloc(n * sizeof(float*));
    for (size_t i = 0; i < n; ++i) {
        rows[i] = calloc(n, sizeof(float));
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                rows[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return rows;
}

float** mat_mul_cache_friendly(float** a, float** b, size_t n) {
    float** rows = malloc(n * sizeof(float*));
    for (size_t i = 0; i < n; ++i) {
        rows[i] = calloc(n, sizeof(float));
        for (size_t j = 0; j < n; ++j) {
            for (size_t k = 0; k < n; ++k) {
                rows[i][j] += a[i][k] * b[j][k];
            }
        }
    }
    return rows;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "usage: ./a.out matrix_size\n");
    }
    srandom(0);

    size_t size = strtoul(argv[1], NULL, 10);
    float** a = random_matrix(size);
    float** b = random_matrix(size);

    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // float** c = mat_mul_cache_friendly(a, b, size);
    float** c = mat_mul(a, b, size);

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t elapsed_msec =
        (1000000 * (end.tv_sec - start.tv_sec) +
         (end.tv_nsec - start.tv_nsec) / 1000) /
        1000;
    printf("elapsed time: %'" PRIu64 " ms\n", elapsed_msec);

    printf("c[0][0] = %f\n", c[0][0]);
    // TODO release memory
}