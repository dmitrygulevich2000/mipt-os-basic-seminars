// for sigabbrev_np
#define _GNU_SOURCE

#include <assert.h>
#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/signalfd.h>
#include <unistd.h>

int main() {
    printf("pid: %d\n", getpid());

    // create sigset = {SIGINT}
    sigset_t sigint;
    sigemptyset(&sigint);
    sigaddset(&sigint, SIGINT);

    // block sigint delivery
    if (sigprocmask(SIG_BLOCK, &sigint, NULL) != 0) {
        err(1, "sigprocmask");
    }
    // create signalfd
    int sigfd = signalfd(-1, &sigint, 0);

    // do select in loop
    struct signalfd_siginfo siginfo;
    char buffer[4096];
    while (1) {
        // prepare fd set
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(sigfd, &fds);
        // await for read opportunity
        int ready_count = select(sigfd + 1, &fds, NULL, NULL, NULL);
        if (ready_count < 0) {
            err(1, "select");
        }

        // handle ready
        if (FD_ISSET(sigfd, &fds)) {
            read(sigfd, (char*)&siginfo, sizeof(siginfo));
            assert(siginfo.ssi_signo == SIGINT);
            break;
        } else if (FD_ISSET(STDIN_FILENO, &fds)) {
            ssize_t rbytes = read(STDIN_FILENO, buffer, sizeof(buffer));
            write(STDOUT_FILENO, buffer, rbytes);
        }
    }

    close(sigfd);
    printf("graceful shutdown\n");
}
