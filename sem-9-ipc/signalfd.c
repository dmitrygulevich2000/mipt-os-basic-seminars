// for sigabbrev_np
#define _GNU_SOURCE

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <unistd.h>

int main() {
    printf("pid: %d\n", getpid());
    
    int signals[] = {SIGINT, SIGTERM, SIGQUIT, SIGTSTP, SIGHUP, SIGALRM};

    // create sigset
    sigset_t sigset;
    sigemptyset(&sigset);
    for (int i = 0; i < sizeof(signals) / sizeof(SIGINT); ++i) { 
        sigaddset(&sigset, signals[i]);
    }

    // block signals delivery
    if (sigprocmask(SIG_BLOCK, &sigset, NULL) != 0) {
        err(1, "sigprocmask");
    }
    // create signalfd
    int sigfd = signalfd(-1, &sigset, 0);

    // read signals in loop
    struct signalfd_siginfo siginfo;
    ssize_t rbytes = 0;
    while ((rbytes = read(sigfd, (char*)&siginfo, sizeof(siginfo))) > 0) {
        printf("received signal: SIG%s (%s)\n", sigabbrev_np(siginfo.ssi_signo), strsignal(siginfo.ssi_signo));
        if (siginfo.ssi_signo == SIGQUIT) {
            break;
        }
    }
    if (rbytes < 0) {
        err(1, "read from signalfd");
    }

    close(sigfd);
    printf("graceful shutdown\n");
}
