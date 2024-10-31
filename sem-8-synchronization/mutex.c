#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
int counter = 0;

void* worker(void*) {
    for (int i = 0; i < 100000; ++i) {
        pthread_mutex_lock(&mutex);
        // critical section
        counter += 1;
        // critical section end
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[2];
    for (int i = 0; i < 2; ++i) {
        pthread_create(threads + i, NULL, worker, NULL);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("counter = %d\n", counter);
    pthread_mutex_destroy(&mutex);
}
