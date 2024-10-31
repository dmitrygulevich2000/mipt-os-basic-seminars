#include <pthread.h>
#include <stdio.h>

int x, y;
int read_x, read_y;

void* thread_one(void*) {
    x = 1;
    read_y = y;
}

void* thread_two(void*) {
    y = 1;
    read_x = x;
}

int main() {
    int iter = 0;
    do {
        x = 0; y = 0;
        read_x = 0; read_y = 0;

        pthread_t t1;
        pthread_t t2;
        pthread_create(&t1, NULL, thread_one, NULL);
        pthread_create(&t2, NULL, thread_two, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        ++iter;
        if (iter % 50000 == 0) {
            printf("%d iters done\n", iter);
        }
    } while (!(read_x == 0 && read_y == 0));

    printf("read_x = %d, read_y = %d encountered after %d iterations\n", read_x, read_y, iter);
}