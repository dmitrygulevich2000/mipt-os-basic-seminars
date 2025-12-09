// for F_GETPIPE_SZ
#define _GNU_SOURCE

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define COLOR_COUNT 5
const char* COLORS[COLOR_COUNT] = {
    "\033[0;31m", // red
    "\033[0;32m", // green
    "\033[0;34m", // blue
    "\033[1;33m", // yellow
    "\033[1;36m"  // light cyan
};
const char* NO_COLOR = "\033[0m";

typedef struct {
    const char* color;
    int source_fd;
} painter_arg;

void* paint_numbers(void* arg) {
    painter_arg* parg = arg;
    printf("painter pid: %d\n", getpid());

    uint64_t number;
    while (1) {
        ssize_t rbytes = read(parg->source_fd, &number, sizeof(number));
        if (rbytes < 0) {
            err(1, "read pipe");
        }
        if (rbytes == 0) {
            printf("numbers are gone\n");
            return NULL;
        }

        printf("%s%lu%s\n", parg->color, number, NO_COLOR);
    }

    return NULL;
}

void generate_numbers(uint64_t limit, int dest_fd) {
    printf("generator pid: %d\n", getpid());
    for (uint64_t num = 0; num < limit; ++num) {
        ssize_t wbytes = write(dest_fd, &num, sizeof(num));
        if (wbytes < 0) {
            if (errno == EPIPE) {
                printf("all painters are dead :(\n");
                return;
            } else {
                err(1, "write pipe");
            }
        }
    }
}

int main(int argc, char** argv) {
    // parse args
    if (argc != 2) {
        errx(1, "usage: ./a.out painters_count");
    }
    int painters_count = atoi(argv[1]);
    assert(painters_count > 0);
    assert(painters_count <= COLOR_COUNT);

    // create "queue"
    int numbers_pipe[2];
    pipe(numbers_pipe);

    // print some parameters
    printf("PIPE_BUF = %d\n", PIPE_BUF);
    printf("pipe capacity = %d\n", fcntl(numbers_pipe[0], F_GETPIPE_SZ));

    // launch painter threads
    pthread_t threads[painters_count];
    painter_arg args[painters_count];
    for (int i = 0; i < painters_count; ++i) {
        args[i].color = COLORS[i];
        args[i].source_fd = numbers_pipe[0];
        pthread_create(threads + i, NULL, paint_numbers, args + i);
        pthread_detach(threads[i]);
    }

    // wrote numbers to pipe
    generate_numbers(1000, numbers_pipe[1]);

    // notify EOF and wait for pressing button
    close(numbers_pipe[1]);
    printf("all numbers was produced, press any button to quit\n");
    fgetc(stdin);
    return 0;
}
