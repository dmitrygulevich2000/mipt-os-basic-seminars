#include <pthread.h>
#include <stdio.h>

#include <stdatomic.h>
#include <stdbool.h>

// mutual exclusion
pthread_mutex_t mutex;
int counter = 0;

//////////////////////////

void my_init();

void my_lock();

void my_unlock();

///////////////////////////

void* worker(void*) {
    for (int i = 0; i < 100000; ++i) {
        pthread_mutex_lock(&mutex);
        // my_lock();
        // critical section start

        counter += 1;

        // critical section end
        // my_unlock();
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    my_init();

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

////////////// spin-lock [mutex] implementation

volatile _Atomic bool locked;

void my_init() {
    atomic_store(&locked, false);
}

void my_lock() {
    while (atomic_exchange(&locked, true)) {
        sched_yield();
    }
}

void my_unlock() {
    atomic_store(&locked, false);
}
