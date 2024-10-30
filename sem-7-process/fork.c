#include <errno.h>
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char log_fmt[4096];
    snprintf(log_fmt, sizeof(log_fmt), "%d: %s", getpid(), fmt);
    vprintf(log_fmt, args);

    va_end(args);
}

void do_work(int tasks) {
    for (int i = 0; i < tasks; ++i) {
        // yield different output order
        // sched_yield();
        // sleep(1);
        log_info("done task %d\n", i);
    }
}

int main() {
    log_info("starting program\n");

    pid_t pid = fork();
    // code below executing in both child and parent
    if (pid < 0) {
        log_info("fork: %s", strerror(errno));
        exit(1);
    }
    if (pid == 0) {
        log_info("running child\n");
        do_work(10);
    } else {
        log_info("running parent\n");
        do_work(10);

        int status = 0;
        waitpid(pid, &status, 0);
        log_info("child %d finished with code %d\n", pid, WEXITSTATUS(status));
    }

    log_info("exiting\n");
}
