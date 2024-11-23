// for sigabbrev_np
#define _GNU_SOURCE

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t received_signum = 0;

void store_signum(int signum) {
    received_signum = signum;
}

int main() {
    printf("pid: %d\n", getpid());

    struct sigaction sigact;
    memset(&sigact, 0, sizeof(struct sigaction));
    sigact.sa_handler = store_signum;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;

    // SIGSTOP, SIGKILL -> Invalid argument error
    int signals[] = {SIGINT, SIGTERM, SIGQUIT, SIGTSTP, SIGHUP, SIGALRM, SIGSTOP};
    for (int i = 0; i < sizeof(signals) / sizeof(int); ++i) { 
        if (sigaction(signals[i], &sigact, NULL) != 0) {
            err(1, "sigaction");
        }
    }

    printf("awaiting signals...\n");
    while (received_signum != SIGQUIT) {
        received_signum = 0;
        pause();
        printf("received signal: SIG%s (%s)\n", sigabbrev_np(received_signum), strsignal(received_signum));
    }

    printf("graceful shutdown\n");
}
