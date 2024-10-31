#include <pthread.h>
#include <stdio.h>

int counter = 0;

void* worker(void*) {
    for (int i = 0; i < 100000; ++i) {
        // read
        // increment
        // write
        counter += 1;
    }
}

int main() {
    pthread_t threads[2];
    for (int i = 0; i < 2; ++i) {
        pthread_create(threads + i, NULL, worker, NULL);
    }

    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("counter = %d\n", counter);
}
