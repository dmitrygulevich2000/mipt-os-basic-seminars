// for sigabbrev_np
#define _GNU_SOURCE

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t received_sigint = 0;

void sigint_handler(int signum) {
    received_sigint = 1;
}

int main() {
    printf("pid: %d\n", getpid());

    // register signal handler
    struct sigaction sigact;
    memset(&sigact, 0, sizeof(struct sigaction));
    sigact.sa_handler = sigint_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sigact, NULL) != 0) {
            err(EXIT_FAILURE, "sigaction");
    }

    // do work and periodically check sigint
    char buffer[4096];
    while (received_sigint != 1) {
        ssize_t rbytes = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (rbytes < 0) {
            err(1, "read");
        }
        if (rbytes == 0) {
            break;
        }
        if (write(STDOUT_FILENO, buffer, rbytes) < 0) {
            err(1, "write");
        }
    }

    printf("graceful shutdown\n");
}
