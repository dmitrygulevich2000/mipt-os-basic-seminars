#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include <inttypes.h>

#include "./intrinsics.h"

float array_avg(const char* algo, float* array, size_t size);

float array_avg_c(float* array, size_t size) {
    float sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum / size;
}

extern float array_avg_sse(float* array, size_t size);
extern float array_avg_avx(float* array, size_t size);
extern float array_avg_avx_aligned(float* array, size_t size);

int main(int argc, char** argv) {
    const char ALGO_C[] = "c";
    const char *algo = ALGO_C;
    if (argc >= 2) {
        algo = argv[1];
    }
    printf("using %s algorithm\n", algo);

    size_t size = 0;
    scanf("%ld", &size);
    float* array = aligned_alloc(256, sizeof(float)*size);
    // float* array = malloc(sizeof(float)*size);
    for (size_t i = 0; i < size; ++i) {
        scanf("%f", array + i);
    }

    float expected = array_avg_c(array, size);

    float got = array_avg(algo, array, size);

    printf("calculated average: %g\n", got);
    assert(fabs(got - expected) < 1e-3);
    free(array);
}

float array_avg(const char* algo, float* array, size_t size) {
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    float result = 0;
    if (strcmp(algo, "c") == 0) {
        result = array_avg_c(array, size);
    } else if (strcmp(algo, "sse") == 0) {
        result = array_avg_sse(array, size);
    } else if (strcmp(algo, "avx") == 0) {
        result = array_avg_avx(array, size);
    } else if (strcmp(algo, "avx_aligned") == 0) {
        result = array_avg_avx_aligned(array, size);
    } else if (strcmp(algo, "intrin") == 0) {
        result = array_avg_intrin(array, size);
    } else {
        errx(1, "unknown algorithm %s", algo);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t elapsed_mcsec = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("elapsed time: %'" PRIu64 " mcs\n", elapsed_mcsec );

    return result;
}
