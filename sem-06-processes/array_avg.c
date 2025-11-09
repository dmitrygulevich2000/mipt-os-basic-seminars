#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    double* data;
    size_t size;
} array_t;

void* summer(void* raw_array) {
    array_t* array = (array_t*)raw_array;
    double sum = 0;
    for (size_t i = 0; i < array->size; ++i) {
        sum += array->data[i];
    }

    double* res = malloc(sizeof(double));
    *res = sum;
    return res;
}

double array_avg_parallel(double* array, size_t size,
                          int thread_num) {
    // size must be multiple of thread_num
    pthread_t* threads = malloc(sizeof(pthread_t) * thread_num);
    array_t* ranges = malloc(sizeof(array_t) * thread_num);
    for (size_t i = 0; i < thread_num; ++i) {
        ranges[i].size = size / thread_num;
        ranges[i].data = array + i * size / thread_num;
        pthread_create(threads + i, NULL, summer, ranges + i);
    }

    double sum = 0;
    for (size_t i = 0; i < thread_num; ++i) {
        void* raw_range_sum;
        pthread_join(threads[i], &raw_range_sum);
        sum += *(double*)raw_range_sum;
        free(raw_range_sum);
    }

    free(threads);
    free(ranges);
    return sum / size;
}

double array_avg(double* array, size_t size, int thread_num) {
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    double avg = array_avg_parallel(array, size, thread_num);

    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t elapsed_mcsec =
        1000000 * (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1000;
    printf("elapsed time: %'" PRIu64 " mcs\n", elapsed_mcsec);

    return avg;
}

double array_avg_c(double* array, size_t size);

int main(int argc, char** argv) {
    // parse args
    int thread_num = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc > 1) {
        thread_num = atoi(argv[1]);
    }
    printf("using %d thread(s)\n", thread_num);

    // create array
    size_t size = 0;
    scanf("%ld", &size);
    double* array = malloc(sizeof(double) * size);
    for (size_t i = 0; i < size; ++i) {
        scanf("%lf", array + i);
    }

    // do calculations
    double expected = array_avg_c(array, size);
    double got = array_avg(array, size, thread_num);

    printf("calculated average: %lg\n", got);
    assert(fabs(got - expected) < 1e-3);
    free(array);
}

double array_avg_c(double* array, size_t size) {
    double sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum / size;
}
