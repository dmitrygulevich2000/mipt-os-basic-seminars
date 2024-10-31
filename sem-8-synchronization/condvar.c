#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <unistd.h>

pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char str[256];

void* ping_fun(void*) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);

        // "while" due to spurious wakeups
        while (strcmp(str, "pong") != 0) {
           // unlock mutex + fall asleep
           pthread_cond_wait(&changed, &mutex);
           // lock mutex
        }

        // what if unlock mutex after check but before falling asleep?
        // if (strcmp(str, "pong") != 0) {
        //     pthread_mutex_unlock(&mutex);
        //     // here other thread writes "pong" and signal
        //     sched_yield();
        //     sched_yield();
        //     sched_yield();
        //     pthread_mutex_lock(&mutex);
        //     pthread_cond_wait(&changed, &mutex);
        // }

        sprintf(str, "ping");
        printf("ping\n");

        pthread_cond_signal(&changed);
        pthread_mutex_unlock(&mutex);
    }
}

void* pong_fun(void*) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);

        while (strcmp(str, "ping") != 0) {
            pthread_cond_wait(&changed, &mutex);
        }
        sprintf(str, "pong");
        printf("pong\n");

        pthread_cond_signal(&changed);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    sprintf(str, "pong");

    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, ping_fun, NULL);
    pthread_create(&t2, NULL, pong_fun, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
