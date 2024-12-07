// for F_GETPIPE_SZ
#define _GNU_SOURCE

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
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

void paint_numbers(const char* color, int source_fd) {
    printf("painter pid: %d\n", getpid());
    
    uint32_t number;
    while (1) {
        ssize_t rbytes = read(source_fd, &number, sizeof(number));
        if (rbytes < 0) {
            err(1, "read pipe");
        }
        if (rbytes == 0) {
            printf("numbers are gone\n");
            return;
        }

        printf("%s%d%s\n", color, number, NO_COLOR);
    }
}

void generate_numbers(uint32_t limit, int dest_fd) {
    printf("generator pid: %d\n", getpid());
    for (uint32_t num = 0; num < limit; ++num) {
        ssize_t wbytes = write(dest_fd, &num, sizeof(num));
        if (wbytes < 0) {
            if (errno == EPIPE) {
                printf("all painters are dead :(\n");
                return;
            } else {
                err(1, "write pipe");
            }
        }
        // try uncommend this and terminate all painters or generator
        // sleep(1);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "usage: ./a.out painters_count");
    }
    int painters_count = atoi(argv[1]);
    assert(painters_count > 0);
    assert(painters_count <= COLOR_COUNT);

    signal(SIGPIPE, SIG_IGN);
    // children will be zombies without it :O
    signal(SIGCHLD, SIG_IGN);

    int numbers_pipe[2];
    pipe(numbers_pipe);

    printf("PIPE_BUF = %d\n", PIPE_BUF);
    printf("pipe capacity = %d\n", fcntl(numbers_pipe[0], F_GETPIPE_SZ));

    for (int i = 0; i < painters_count; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            close(numbers_pipe[1]);
            paint_numbers(COLORS[i], numbers_pipe[0]);
            return 0;
        }
    }

    close(numbers_pipe[0]);
    generate_numbers(200, numbers_pipe[1]);

    close(numbers_pipe[1]);
    printf("all numbers was produced, press any button to quit\n");
    fgetc(stdin);
    return 0;
}