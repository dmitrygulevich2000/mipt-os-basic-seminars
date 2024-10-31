#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>

int var = 0;
int zero_observed = 0;

void* reader(void*) {
    // usleep(1000 * 100);
    if (var == 0) {
        zero_observed = 1;
    }
}

void* writer(void*) {
    var = 1;
}

int main() {
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t2, NULL, writer, NULL);
    pthread_create(&t1, NULL, reader, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("var = %d, zero_observed: %d\n", var, zero_observed);
}
