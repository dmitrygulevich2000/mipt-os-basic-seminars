#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

void swap(uint64_t* lhs, uint64_t* rhs) {
    uint64_t tmp = *lhs;
    *lhs = *rhs;
    *rhs = tmp;
}

void shuffle(uint64_t* array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        size_t j = i + (size_t)random() % (size - i);
        swap(array + i, array + j);
    }
}

uint64_t* generate_permutation(size_t size) {
    uint64_t* array = calloc(size, sizeof(uint64_t));
    for (size_t i = 0; i < size; ++i) {
        array[i] = i;
    }
    shuffle(array, size);
    return array;
}

///////////////////////////////////////////////////////////////////////

double measure_timing(size_t memory_size, size_t query_count) {
    srandom(5);
    size_t element_count = memory_size / sizeof(uint64_t);
    uint64_t* permutation = generate_permutation(element_count);
    size_t read_index = 0;

    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < query_count; ++i) {
        read_index = permutation[read_index];
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t total_ns = 1000000000*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    
    fprintf(stderr, "last index: %ld\n", read_index);

    free(permutation);
    return (double)total_ns / query_count;
}

///////////////////////////////////////////////////////////////////////

int main() {
    size_t queryCount = 10000;
    for (size_t size = 512; size < 512 * 1024 * 1024; size = (int)((double)size * 1.5)) {
        double avg_access_time_ns = measure_timing(size, queryCount);
        printf("%ld: %f\n", size, avg_access_time_ns);
    }
}

